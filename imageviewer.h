#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QWidget>
#include <QMutex>

class QImage;

enum EViewer_Mode
{
    Hand_Move_Mode = 1,              //平移模式
    Selected_Zoom_Mode          //拉框放大模式
};

class ImageViewer : public QWidget
{
    Q_OBJECT
public:
    explicit ImageViewer(QWidget *parent = 0);

    void setImage(const QImage *pImg);
    void setCurrentMode(EViewer_Mode mode);

protected:
    void paintEvent(QPaintEvent *e);
    void wheelEvent(QWheelEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

private:
    QImage *m_pImage;
    QMutex m_mutexImage;
    float m_fZoom;          //当前缩放比
    QPoint m_posTopLeft;    //当前显示区域的位置(图像坐标)
    EViewer_Mode m_currentMode;
    QPoint m_posPressed;    //鼠标按下的位置(窗体坐标)
    QRect m_rectSelected;   //用户选择区域(窗体坐标)
};

#endif // IMAGEVIEWER_H
