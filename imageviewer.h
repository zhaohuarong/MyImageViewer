#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QWidget>
#include <QMutex>

class QImage;

enum EViewer_Mode
{
    Hand_Move_Mode = 1,              //ƽ��ģʽ
    Selected_Zoom_Mode          //����Ŵ�ģʽ
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
    float m_fZoom;          //��ǰ���ű�
    QPoint m_posTopLeft;    //��ǰ��ʾ�����λ��(ͼ������)
    EViewer_Mode m_currentMode;
    QPoint m_posPressed;    //��갴�µ�λ��(��������)
    QRect m_rectSelected;   //�û�ѡ������(��������)
};

#endif // IMAGEVIEWER_H
