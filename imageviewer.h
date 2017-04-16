#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QWidget>

class QImage;

class ImageViewer : public QWidget
{
    Q_OBJECT
public:
    explicit ImageViewer(QWidget *parent = 0);

public slots:
    void setImage(const QImage *pImg);
    void onHandMode();
    void onCrossMode();

protected:
    void paintEvent(QPaintEvent *e);
    void wheelEvent(QWheelEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

    void fitWidth();

private:
    QImage *m_pImage;
    float m_fZoom;
    QPoint m_pTopLeft;
    QPoint m_pPressPosition;
};

#endif // IMAGEVIEWER_H
