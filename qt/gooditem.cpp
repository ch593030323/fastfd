#include "gooditem.h"
#include <QPainter>
#include <QFont>
#include "fast_order_2.h"
#include <QDesktopWidget>
#include <QApplication>

GoodItem::GoodItem(QString barcode,QString name,QWidget *parent) :
    QWidget(parent),_barcode(barcode),_name(name),_isSelect(false)
{
    QDesktopWidget *d=QApplication::desktop();
    if(d->width() != 800)
        _rect = QRect(0,0,100,60);
    else
        _rect = QRect(0,0,110,72);

    resize(_rect.width(),_rect.height());
    //this->setAttribute(Qt::WA_TranslucentBackground);
    setMouseTracking(true);
}

void GoodItem::setName(QString name)
{
    _name = name;
    repaint();
}

void GoodItem::mousePressEvent(QMouseEvent *)
{
    _isSelect = true;
    repaint();
    emit touch(_barcode);//发送的是编码
}

void GoodItem::paintEvent(QPaintEvent *)
{

    if(_name.trimmed().isEmpty())
        return;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    QFont font;
    font.setPointSize(13);
    painter.setFont(font);
    painter.setPen(Qt::NoPen);
    // fast_order_2::currentSkin.itemGradientWay = 1;
    // if(1 == fast_order_2::currentSkin.itemGradientWay)    //渐变1
    // {
    QLinearGradient gradient(this->rect().topLeft(),this->rect().bottomLeft());
    if(!_isSelect)
    {
        gradient.setColorAt(1,QColor("#FFFFFF"));
    }
    else
    {
        gradient.setColorAt(1,QColor("#FFF4D6"));
    }
    painter.setBrush(gradient);
//    if(fast_order_2::currentSkin.itemRounded)
//        painter.drawRoundedRect(_rect,10,10);
//    else
        painter.drawRect(_rect);

    //painter.setFont(Document::NormalFont);
    if(!_isSelect)
    {
        painter.setPen(QColor("#7C7C7C"));
    }
    else
    {
        painter.setPen(QColor("#FB7422"));
    }
    painter.drawText(_rect,Qt::AlignCenter|Qt::TextWordWrap,_name);
}

void GoodItem::setSelect(bool bo)
{
    _isSelect = bo;
    repaint();
}
