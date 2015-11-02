#ifndef FRONTVIEWDIALOG_TOOLBTN_H
#define FRONTVIEWDIALOG_TOOLBTN_H

#include <QPushButton>
//本btn，通过重写is_like_tab和mouseReleaseEvent和focusOutEvent的方法，实现点击后总是按下的效果
//is_can_released，可以点击第二次，释放
//is_muti_draw_self，可以绘制自定义的图片
class frontViewDialog_toolbtn : public QPushButton
{
    Q_OBJECT
public:
    enum THISSTATE{
        NOSTATE=-1,
        KONGXIAN=0,
        YUDING,
        XUANZHONG
    };
    explicit frontViewDialog_toolbtn(QWidget *parent = 0);
    QString index;
    THISSTATE state;

    bool is_like_tab;
    bool is_can_released;
    bool is_use_pixmap;
    bool ispressed;
    bool is_like_label;
    void set_btn_property(const QString &property);
    QColor presscolor;
signals:
    void thisclicked(const QString &sqr);
    void thisdoubleclicked(const QString &sqr);
protected:
    void focusInEvent(QFocusEvent *e);
    void focusOutEvent(QFocusEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseDoubleClickEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

    void paintEvent(QPaintEvent *);
private:
    QIcon _icon;
    QSize _icon_size;
    QString _text;
    bool focus_pressed;
};

#endif // FRONTVIEWDIALOG_TOOLBTN_H
