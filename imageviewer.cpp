#include <QDebug>
#include <QWheelEvent>
#include <QPainter>

#include "imageviewer.h"

ImageViewer::ImageViewer(QWidget *parent) :
    QWidget(parent),
    m_pImage(NULL),
    m_fZoom(1.0),
    m_pTopLeft(0, 0),
    m_pPressPosition(0, 0),
    m_selectedRect(0, 0, 0, 0)
{
    QPalette pal(palette());
    pal.setColor(QPalette::Background, Qt::black);
    setAutoFillBackground(true);
    setPalette(pal);
}

void ImageViewer::setImage(const QImage *pImg)
{
    if(m_pImage == NULL)
    {
        m_pImage = new QImage(*pImg);
        memcpy(m_pImage->bits(), pImg->bits(), pImg->byteCount());
        fitWidth();
    }
    else if(m_pImage != NULL && (m_pImage->width() != pImg->width() || m_pImage->height() != pImg->height()))
    {
        delete m_pImage;
        m_pImage = new QImage(*pImg);
        memcpy(m_pImage->bits(), pImg->bits(), pImg->byteCount());
        fitWidth();
    }
    else
    {
        memcpy(m_pImage->bits(), pImg->bits(), pImg->byteCount());
    }

    update();
}

void ImageViewer::onHandMode()
{
    setCursor(Qt::OpenHandCursor);
}

void ImageViewer::onCrossMode()
{
    setCursor(Qt::CrossCursor);
}


void ImageViewer::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);

    QPainter painter(this);
    if(m_pImage != NULL)
    {
        QSize size = m_pImage->size();
        painter.drawImage(m_pTopLeft, m_pImage->scaled(size / m_fZoom));
    }

    if(m_selectedRect.width() != 0 && m_selectedRect.height() != 0)
    {
        painter.setPen(Qt::green);
        painter.drawRect(m_selectedRect);
    }
}

void ImageViewer::wheelEvent(QWheelEvent *e)
{
    qDebug() << "wheel event";
//    m_fZoom *= (e->delta() > 0) ? 1.1 : 0.9;
    float zoom = m_fZoom * ((e->delta() > 0) ? 1.1 : 0.9);

    m_pTopLeft = QPoint(m_pTopLeft.x() - (width() / zoom - width() / m_fZoom) / 2, m_pTopLeft.y() - (height() / zoom - height() / m_fZoom) / 2);
    m_fZoom = zoom;
    update();
}

void ImageViewer::mouseMoveEvent(QMouseEvent *e)
{
    if(cursor().shape() == Qt::ClosedHandCursor)
    {
        QPoint pDelta = e->pos() - m_pPressPosition;
        m_pTopLeft += pDelta;
        m_pPressPosition = e->pos();
    }
    else if(cursor().shape() == Qt::CrossCursor)
    {
        m_selectedRect.setBottomRight(e->pos());
    }
    update();
}

void ImageViewer::mousePressEvent(QMouseEvent *e)
{
    if(cursor().shape() == Qt::OpenHandCursor)
    {
        setCursor(Qt::ClosedHandCursor);
        m_pPressPosition = e->pos();
    }
    else if(cursor().shape() == Qt::CrossCursor)
    {
        m_selectedRect.setTopLeft(e->pos());
    }
}

void ImageViewer::mouseReleaseEvent(QMouseEvent *e)
{
    Q_UNUSED(e);

    if(cursor().shape() == Qt::ClosedHandCursor)
    {
        setCursor(Qt::OpenHandCursor);
    }
}

void ImageViewer::fitWidth()
{
#if 1
    //宽度适应
    m_fZoom = (float)m_pImage->width() / width();
#else
    //高度适应
    m_fZoom = (float)m_pImage->height() / height();
#endif

    //中心点对齐
    m_pTopLeft = QPoint(m_pTopLeft.x() - (m_pImage->width() / m_fZoom - width()) / 2, m_pTopLeft.y() - (m_pImage->height() / m_fZoom - height()) / 2);
}
