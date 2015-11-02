#include "sortitem.h"
#include <QPainter>
#include <QGridLayout>
#include <QDesktopWidget>
#include "fast_order_2.h"
#include <QApplication>

SortItem::SortItem(QString kindId,QString name,bool select,QWidget *parent) :
    QWidget(parent),_kindId(kindId),_name(name),_isSelect(select),_hasInit(false)
{
    QDesktopWidget *d=QApplication::desktop();
    if(d->width() != 800)
        _rect = QRect(0,0,100,60);
    else
        _rect = QRect(0,0,110,72);
    resize(_rect.width(),_rect.height());
    _hasInit = true;
    setMouseTracking(true);
}

void SortItem::mousePressEvent(QMouseEvent *)
{
    _isSelect = true;
    repaint();
    emit touch(_kindId);
}

void SortItem::paintEvent(QPaintEvent *)
{
    if(_name.isEmpty())
        return;
    QFont font;
    font.setPointSize(12);
    QPainter painter(this);
    painter.setFont(font);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(Qt::NoPen));
    QLinearGradient gradient(this->rect().topLeft(),this->rect().bottomLeft());
    if(!_isSelect)
    {
        gradient.setColorAt(1,QColor("#F4B206"));
    }
    else
    {
        gradient.setColorAt(1,QColor("#FFFFFF"));
    }
    painter.setBrush(gradient);
//    if(fast_order_2::currentSkin.sortRounded)
//        painter.drawRoundedRect(_rect,0,0);
//    else
        painter.drawRect(_rect);
    if(!_isSelect)
    {
        painter.setPen(QColor("#FFFFFF"));
    }
    else
    {
        painter.setPen(QColor("#7C7C7C"));
    }
    //painter.setFont(Document::NormalFont);//界面信息是什么东东？
    painter.drawText(_rect,Qt::AlignCenter|Qt::TextWordWrap,_name);
}

void SortItem::setSelect(bool bo)
{
    _isSelect = bo;
    repaint();
}

void SortItem::setRect(QRect rect)
{
    _rect = rect;
    repaint();
}

void SortItem::setName(QString name)
{
    _name = name;
    repaint();
}
