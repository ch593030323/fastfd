#include "lds_roundedwidget.h"
#include <QPainter>

lds_roundedwidget::lds_roundedwidget(QWidget *parent) :
    QWidget(parent)
{
    setAttribute(Qt::WA_TranslucentBackground, true);
    setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
}

void lds_roundedwidget::setBgColor(const QColor &color)
{
    bgcolor=color;
}

QColor lds_roundedwidget::getBgColor() const
{
    return bgcolor;
}

void lds_roundedwidget::paintEvent(QPaintEvent *e)
{
    QPainter p(this);
    p.setPen(Qt::NoPen);
    p.fillRect(this->rect(), Qt::transparent);
    p.setBrush(bgcolor);
    p.setRenderHint(QPainter::Antialiasing);
    p.drawRoundedRect(0, 0, width(), height(), 20, 20);
}
