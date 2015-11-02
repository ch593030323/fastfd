#include "lds_listbutton.h"
#include <QPainter>
#include <QStyle>
#include <QApplication>
lds_listbutton *lds_listbutton::pre_focus_button = 0;


lds_listbutton::lds_listbutton(QWidget *parent) :
    QFrame(parent)
{
    radius=0;//圆角

    pressed_background_color=QColor(175, 175, 175,255/3);//press的背景色
    released_background_color=QColor("transparent");//释放的背景色

    border_is_draw=true;//是否绘制下滑线
    border_bottom_height=2;//border的高度
    released_border_color=QColor(175, 175, 175);//border的颜色
    pressed_border_color=QColor(175, 175, 175);//press border的颜色

    icon_text_size=QSize(100,60);//图标，文字的rect
    pressed_pixmap = QPixmap(":/image/tabwidget_select_focus.png");//press的图标
    released_pixmap = QPixmap(":/image/tabwidget_select_nofocus.png");//释放是的图标

    pressed_text_color=QColor("#AF302D");//press 文字的颜色
    text = tr("会员管理");//文字


    ischecked=false;
    this->setMinimumWidth(120);
    this->setFrameShape(QFrame::NoFrame);
}

void lds_listbutton::set_ischecked()
{
    pre_focus_button=this;
    ischecked=true;
}

void lds_listbutton::mousePressEvent(QMouseEvent *e)
{
    QWidget::mousePressEvent(e);
    if(pre_focus_button==this){
    } else {
        ischecked=true;
        this->update();
        if(pre_focus_button){
            pre_focus_button->ischecked=false;
            pre_focus_button->update();
        }
        pre_focus_button=this;
    }
}

void lds_listbutton::mouseReleaseEvent(QMouseEvent *e)
{
    QWidget::mouseReleaseEvent(e);
//    update();

    emit clicked(_index);
}

void lds_listbutton::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);

    rect = QRect(QPoint(0,0), this->size());
    border_bottom_width=rect.width()-2*radius;

    //圆角矩形
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setBrush(ischecked?pressed_background_color:released_background_color);
    painter.drawRoundedRect(rect, radius, radius);

    //下划线
    if(border_is_draw){
        QPen pen;
        pen.setWidth(border_bottom_height);
        pen.setBrush(ischecked?pressed_border_color:released_border_color);
        pen.setStyle(Qt::DashLine);
        painter.setPen(pen);
        painter.setBrush(Qt::NoBrush);
        painter.drawLine(QPoint(radius,rect.height()-border_bottom_height/2),
                         QPoint(rect.width()-radius, rect.height()-border_bottom_height/2));
    }

    //图标
    QRect itrect=QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, icon_text_size,rect);
    QPixmap icon_pixmap= ischecked?pressed_pixmap:released_pixmap;
    painter.drawPixmap(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignVCenter|Qt::AlignLeft, icon_pixmap.size(), itrect), icon_pixmap);

    //文字
    itrect.setTopLeft(itrect.topLeft()+QPoint(icon_pixmap.width(), 0));
    painter.setPen(ischecked?pressed_text_color:Qt::black);
    painter.drawText(itrect, int(Qt::AlignCenter), text);


}
