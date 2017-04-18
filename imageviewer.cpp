#include <QDebug>
#include <QWheelEvent>
#include <QImage>
#include <QPainter>

#include "imageviewer.h"

ImageViewer::ImageViewer(QWidget *parent) :
    QWidget(parent),
    m_pImage(NULL),
    m_fZoom(1.0),
    //m_rectSource(0, 0, 0, 0),
    m_posTopLeft(0, 0),
    m_currentMode(Hand_Move_Mode),
    m_posPressed(0, 0),
    m_rectSelected(0, 0, 0, 0)
{
    QPalette pal(palette());
    pal.setColor(QPalette::Background, Qt::black);
    setAutoFillBackground(true);
    setPalette(pal);

    setCurrentMode(Hand_Move_Mode);
}

void ImageViewer::setImage(const QImage *pImg)
{
    if(pImg == NULL)
        return;
    m_mutexImage.lock();
    if(m_pImage == NULL)
    {
        m_pImage = new QImage(*pImg);
    }
    else if(m_pImage != NULL && (m_pImage->width() != pImg->width() || m_pImage->height() != pImg->height()))
    {
        delete m_pImage;
        m_pImage = new QImage(*pImg);
    }

    memcpy(m_pImage->bits(), pImg->bits(), pImg->byteCount());
    m_mutexImage.unlock();
    update();
}

void ImageViewer::setCurrentMode(EViewer_Mode mode)
{
    m_currentMode = mode;
    switch(m_currentMode)
    {
    case Hand_Move_Mode:
        setCursor(Qt::OpenHandCursor);
        break;
    case Selected_Zoom_Mode:
        setCursor(Qt::CrossCursor);
        break;
    }
}

void ImageViewer::wheelEvent(QWheelEvent *e)
{
    if(m_pImage == NULL)
        return;
    float zoom = m_fZoom * ((e->delta() > 0) ? 1.1 : 0.9);

    //中心点放大
    m_posTopLeft.setX(m_posTopLeft.x() - (zoom - m_fZoom) * width() / 2);
    m_posTopLeft.setY(m_posTopLeft.y() - (zoom - m_fZoom) * height() / 2);

    m_fZoom = zoom;
    update();
}

void ImageViewer::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);
    QPainter painter(this);
    if(m_pImage != NULL && m_pImage->width() != 0 && m_pImage->height() != 0)
    {
        //按照宽度计算source rect的宽高
        int w = (float)(width()) * m_fZoom;
        int h = (float)(height()) / width() * w;

        m_mutexImage.lock();
        painter.drawImage(rect(), *m_pImage, QRect(m_posTopLeft.x(), m_posTopLeft.y(), w, h));
        m_mutexImage.unlock();


        // draw selected rect
        painter.setPen(Qt::green);
        painter.drawRect(m_rectSelected);
    }
}

void ImageViewer::mouseMoveEvent(QMouseEvent *e)
{
    if(cursor().shape() == Qt::ClosedHandCursor)
    {
        QPoint pDelta = e->pos() - m_posPressed;
        m_posPressed = e->pos();

        m_posTopLeft -= QPoint((float)pDelta.x() * m_fZoom, (float)pDelta.y() * m_fZoom);
        update();
    }
    else if(m_currentMode == Selected_Zoom_Mode)
    {
        m_rectSelected.setBottomRight(e->pos());
        update();
    }
}

void ImageViewer::mousePressEvent(QMouseEvent *e)
{
    m_posPressed = e->pos();
    if(m_currentMode == Hand_Move_Mode)
    {
        setCursor(Qt::ClosedHandCursor);
    }
    else if(m_currentMode == Selected_Zoom_Mode)
    {
        m_rectSelected.setTopLeft(m_posPressed);
    }
}

void ImageViewer::mouseReleaseEvent(QMouseEvent *e)
{
    Q_UNUSED(e);
    if(m_currentMode == Hand_Move_Mode)
    {
        setCursor(Qt::OpenHandCursor);
    }
    else if(m_currentMode == Selected_Zoom_Mode)
    {
        qDebug() << m_rectSelected;
        if(m_rectSelected.width() > 0 && m_rectSelected.height() > 0)
        {
            m_posTopLeft += m_rectSelected.topLeft() * m_fZoom;
            m_fZoom *= (float)m_rectSelected.width() / width();
            m_rectSelected = QRect(0, 0, 0, 0);
            update();
        }
    }
}
