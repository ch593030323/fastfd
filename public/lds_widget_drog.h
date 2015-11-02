#ifndef LDS_WIDGET_DROG_H
#define LDS_WIDGET_DROG_H

#include <QWidget>

class lds_widget_drog : public QWidget
{
    Q_OBJECT
public:
    explicit lds_widget_drog(QWidget *parent = 0);
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
private:
    bool mouse_press; //按下鼠标左键
    QPoint move_point; //移动的距离
};

#endif // LDS_WIDGET_DROG_H
