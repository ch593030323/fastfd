#include "frontviewdialog_toolbtn.h"
#include <QPainter>
#include <QtDebug>
#include <QMouseEvent>
#include <QStylePainter>
#include <QStyleOptionButton>
#include <QLinearGradient>

frontViewDialog_toolbtn::frontViewDialog_toolbtn(QWidget *parent) :
    QPushButton(parent),
    is_like_tab(false),
    ispressed(false),
    focus_pressed(false),
    is_can_released(false),
    state(NOSTATE),
    is_use_pixmap(false),
    is_like_label(false)
{
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->setFocusPolicy(Qt::NoFocus);
}

void frontViewDialog_toolbtn::set_btn_property(const QString &property)
{
    this->setProperty("outer_stylesheet", property);
}

void frontViewDialog_toolbtn::focusInEvent(QFocusEvent *e)
{
    focus_pressed=true;
    QPushButton::focusInEvent(e);
}

void frontViewDialog_toolbtn::focusOutEvent(QFocusEvent *e)
{
    focus_pressed=false;
    update();
    //    if(is_like_tab){
    //当失去焦点是，不再 update
    //    QToolButton::focusOutEvent(e);
    //    } else {
    //        QToolButton::focusOutEvent(e);
    //    }
}

void frontViewDialog_toolbtn::mousePressEvent(QMouseEvent *e)
{
    if(is_like_label) return;

    QPushButton::mousePressEvent(e);
}

void frontViewDialog_toolbtn::mouseDoubleClickEvent(QMouseEvent *e)
{
    QPushButton::mouseDoubleClickEvent(e);
    emit thisdoubleclicked(index);
}

void frontViewDialog_toolbtn::mouseReleaseEvent(QMouseEvent *e)
{
    if(is_like_label) return;

    if(this->rect().contains(e->pos())){
        if(!is_like_tab){
            if(ispressed){QPushButton::mouseReleaseEvent(e);ispressed=false;}
            else ispressed=true;
        }
        update();
        emit thisclicked(index);
    }
}

void frontViewDialog_toolbtn::paintEvent(QPaintEvent *)
{
    QStylePainter p(this);
    QStyleOptionButton opt;
    initStyleOption(&opt);
    _icon_size=opt.iconSize;
    _icon=opt.icon;
    opt.icon=QIcon();

    _text=opt.text;
    opt.text.clear();
    p.save();
    QRect parent_rect=QRect(2,2,this->width()-4,this->height()-4);
    QRect pixmap_rect=QStyle::alignedRect(Qt::LeftToRight, Qt::AlignHCenter|Qt::AlignTop,_icon_size, parent_rect);
    QPen pen;
    pen.setColor(Qt::red);
    pen.setWidth(3);
    p.setPen(Qt::NoPen);
    p.setRenderHint(QPainter::Antialiasing);
    switch(state){
    case KONGXIAN:{
        p.setBrush(QColor(139,199,104));
        p.drawRoundedRect(this->rect(),10,10);
    }
        break;
    case YUDING:{
        p.setBrush(QColor(126,126,126));
        p.drawRoundedRect(this->rect(),10,10);
    }
        break;
    case XUANZHONG:{
        p.setBrush(QColor(219,96,76));
        p.drawRoundedRect(this->rect(),10,10);
    }
        break;
    case NOSTATE:
        p.setPen(pen);
        p.drawControl(QStyle::CE_PushButton, opt);
        if(is_use_pixmap){
            _icon_size=QSize(parent_rect.width(), 55);//p.size();
            if(!_icon.isNull())
                p.drawPixmap(pixmap_rect, _icon.pixmap(_icon_size).scaled(_icon_size, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }
    }
    p.restore();
    QRect text_rect;
    if(is_use_pixmap){
        text_rect=QRect(3, parent_rect.top()+_icon_size.height()+3, this->width()-6, this->height()-_icon_size.height()-6);
    } else {
        text_rect=QRect(3, parent_rect.top()+3, this->width()-6, this->height()-6);
    }

    if(opt.state & QStyle::State_Sunken){
        if(state==NOSTATE)p.setPen(presscolor.isValid()?presscolor:QColor("red"));
        else p.setPen(QColor("midnightblue"));
    }
    p.drawText(text_rect, int(Qt::AlignCenter|Qt::TextWordWrap), _text);
}

