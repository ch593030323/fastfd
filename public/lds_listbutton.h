#ifndef LDS_LISTBUTTON_H
#define LDS_LISTBUTTON_H

#include <QFrame>

class lds_listbutton : public QFrame
{
    Q_OBJECT
public:
    explicit lds_listbutton(QWidget *parent = 0);
    bool border_is_draw;//是否绘制border
    QString text;//设置文本内容
    int _index;//用于发送点击信号的索引

    QPixmap pressed_pixmap;
    QPixmap released_pixmap;

    void set_ischecked();

    static lds_listbutton *pre_focus_button;

signals:
    void clicked(int index);
protected:
    virtual void mousePressEvent(QMouseEvent *e);
    virtual void mouseReleaseEvent(QMouseEvent *e);
    virtual void paintEvent(QPaintEvent *e);

private:
    QRect rect;
    int radius;
    int border_bottom_width;
    int border_bottom_height;

    QColor pressed_background_color;
    QColor released_background_color;
    QColor released_border_color;
    QColor pressed_border_color;
    QColor pressed_text_color;

    QSize icon_text_size;
    bool ischecked;

};

#endif // LDS_LISTBUTTON_H
