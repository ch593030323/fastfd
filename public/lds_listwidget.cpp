#include "lds_listwidget.h"
#include <QScrollBar>
#include <QMouseEvent>
lds_listwidget::lds_listwidget(QWidget *parent):QListWidget(parent)
{
    this->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    this->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
}

lds_listwidget::~lds_listwidget()
{

}

void lds_listwidget::mousePressEvent(QMouseEvent *event)
{
    ismoving = false;
    pressp = event->pos();
    pressv = this->verticalScrollBar()->value();
    pressh = this->horizontalScrollBar()->value();
    QListWidget::mousePressEvent(event);
}

void lds_listwidget::mouseMoveEvent(QMouseEvent *event)
{
    int voff = event->pos().y() - pressp.y();
    int hoff = event->pos().x() - pressp.x();

    //若是移动超过10*10打矩形，才会真正移动
    if(qAbs(voff) >= 10 || qAbs(hoff) >= 10){
        ismoving = true;
        voff = -voff;
        hoff = -hoff;

        if((pressv + voff) <= this->verticalScrollBar()->maximum()+10
                && (pressv + voff) >= 0-10)
            this->verticalScrollBar()->setValue(pressv + voff);
        if((pressh + hoff) <= this->horizontalScrollBar()->maximum()+10
                && (pressh + hoff) >= 0-10)
            this->horizontalScrollBar()->setValue(pressh + hoff);
    } else {
        ismoving = false;
    }

    QListWidget::mouseMoveEvent(event);
}

void lds_listwidget::mouseReleaseEvent(QMouseEvent *event)
{
    QListWidget::mouseReleaseEvent(event);
}

