#include "sortitem_restaurant.h"
#include <QPainter>
#include <QGridLayout>
#include <QDesktopWidget>
#include "fast_order_2.h"
#include <QApplication>

sortitem_restaurant::sortitem_restaurant(QString kindId,QString name,bool select,QWidget *parent) :
    QWidget(parent),_kindId(kindId),_name(name),_isSelect(select),_hasInit(false)
{
    QDesktopWidget *d=QApplication::desktop();
    if(d->width() != 800)
        _rect = QRect(0,0,110,60);
    else
        _rect = QRect(0,0,110,72);
    resize(_rect.width(),_rect.height());
    _hasInit = true;
    setMouseTracking(true);
}

void sortitem_restaurant::mousePressEvent(QMouseEvent *)
{
    _isSelect = true;
    repaint();
    emit touch_restaurant(_kindId);
}

void sortitem_restaurant::paintEvent(QPaintEvent *)
{
    if(_name.isEmpty())
        return;
    QFont font;
    font.setPointSize(14);
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
        gradient.setColorAt(1,QColor("#FEEAE9"));
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
        painter.setPen(QColor("#F54180"));
    }
    //painter.setFont(Document::NormalFont);//界面信息是什么东东？
    painter.drawText(_rect,Qt::AlignCenter|Qt::TextWordWrap,_name);
}

void sortitem_restaurant::setSelect(bool bo)
{
    _isSelect = bo;
    repaint();
}

void sortitem_restaurant::setRect(QRect rect)
{
    _rect = rect;
    repaint();
}

void sortitem_restaurant::setName(QString name)
{
    _name = name;
    repaint();
}
