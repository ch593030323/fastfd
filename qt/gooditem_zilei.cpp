#include "gooditem_zilei.h"
#include <QPainter>
#include <QFont>
#include "fast_order_2.h"
#include <QDesktopWidget>
#include <QApplication>

goodItem_zilei::goodItem_zilei(QString barcode,QString name,QWidget *parent) :
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

void goodItem_zilei::setName(QString name)
{
    _name = name;
    repaint();
}

void goodItem_zilei::mousePressEvent(QMouseEvent *)
{
    _isSelect = true;
    repaint();
    emit touch_zilei(_barcode);//发送的是编码
}

void goodItem_zilei::paintEvent(QPaintEvent *)
{
    if(_name.isEmpty())
        return;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    QFont font;
    font.setPointSize(13);
    painter.setFont(font);
    painter.setPen(Qt::NoPen);

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

//    if(fast_order_2::currentSkin.itemRounded)
//        painter.drawRoundedRect(_rect,10,10);
//    else
        painter.drawRect(_rect);
    //painter.setFont(Document::NormalFont);
    if(!_isSelect)
    {
        painter.setPen(QColor("#FFFFFF"));
    }
    else
    {
        painter.setPen(QColor("#7C7C7C"));
    }
    painter.drawText(_rect,Qt::AlignCenter|Qt::TextWordWrap,_name);
}

void goodItem_zilei::setSelect(bool bo)
{
    _isSelect = bo;
    repaint();
}
