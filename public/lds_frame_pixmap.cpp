#include "lds_frame_pixmap.h"
#include<QPainter>
#include<QStyle>

lds_frame_pixmap::lds_frame_pixmap(QWidget *parent) :
    QFrame(parent)
{
    labelicon=new QLabel(this);
    labelicon->setGeometry(QRect(0,0,0,0));
}

void lds_frame_pixmap::drawPixmap(const QPixmap &pixmap)
{
    if(pixmap.isNull())
        labelicon->lower();
    else
        labelicon->raise();
    labelicon->setGeometry(QRect(QPoint(0,0), this->size()));

    QPixmap p(labelicon->size());
    QColor c("whitesmoke");
    c.setAlpha(255/3);
    p.fill(c);
    QPainter painter(&p);
    painter.translate(labelicon->width()/2, labelicon->height()/2);
    painter.rotate(30);
    painter.drawPixmap(QPoint(-255.0/2.0, -255.0/3.0/2.0), pixmap);
    labelicon->setPixmap(p);
}
