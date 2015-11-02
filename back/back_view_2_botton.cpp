#include "back_view_2_botton.h"
#include <QPainter>
#include <QPainterPath>
#include <QStylePainter>
#include <QStyleOptionToolButton>
#include <QApplication>
#include <QDesktopWidget>
#include <qdebug.h>
#include "backheader.h"

QList<QToolButton *> back_view_2_botton::toollist;
back_view_2_botton::back_view_2_botton(QWidget *parent):
    QToolButton(parent),
    isfocus(false)
{
    toollist.append(this);

}

void back_view_2_botton::focusInEvent(QFocusEvent *e)
{
    isfocus=true;
    QToolButton::focusInEvent(e);
}

void back_view_2_botton::focusOutEvent(QFocusEvent *e)
{
    isfocus=false;
    QToolButton::focusOutEvent(e);
}

void back_view_2_botton::paintEvent(QPaintEvent *e)
{
    QStylePainter p(this);
    QStyleOptionToolButton opt;
    initStyleOption(&opt);
    QIcon _icon=opt.icon;opt.icon=QIcon();
    QSize _icon_size=opt.iconSize;opt.iconSize=QSize();
    QString _text=opt.text;opt.text=QString();
    //样式表
    p.drawComplexControl(QStyle::CC_ToolButton, opt);
    //icon
    p.setRenderHint(QPainter::SmoothPixmapTransform);
    p.drawPixmap(
                QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, _icon_size, QRect(0, 0, this->width(), this->height()*3.0/5.0)),
                _icon.pixmap(_icon_size));
    //下部的阴影
    QPainterPath path;
    path.moveTo(0, this->height()*3.0/5.0);
    path.lineTo(this->width()/2.0-5.0, this->height()*3.0/5.0);
    path.lineTo(this->width()/2.0, this->height()*3.0/5.0-5.0);
    path.lineTo(this->width()/2.0+5.0, this->height()*3.0/5.0);
    path.lineTo(this->width(), this->height()*3.0/5.0);
    path.lineTo(this->width(), this->height());
    path.lineTo(0, this->height());
    p.fillPath(path, QColor(78,78,78,60));
    //文字
    QFont font=this->font();
    p.setFont(font);
    p.setPen(Qt::white);
    p.drawText(QRect(0, this->height()*3.0/5.0, this->width(), this->height()*2.0/5.0), int(Qt::AlignCenter|Qt::TextWordWrap), _text);
    //有焦点的话，边框
    if(isfocus){
        QPen pen;
        pen.setColor(QColor("black"));
        pen.setWidth(4);
        pen.setJoinStyle(Qt::MiterJoin);
        p.setPen(pen);
        p.drawRect(this->rect().adjusted(2,2,-2,-2));
    }
}

