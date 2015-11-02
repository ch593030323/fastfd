#include "frontviewdialog_list.h"
#include <QListWidgetItem>
#include "ui_frontViewDialog_list_item_dialog.h"
#include <QScrollBar>
#include <QMouseEvent>
#include <QtDebug>
#include "frontviewdialog_frame_list_item.h"

frontViewDialog_list::frontViewDialog_list(QWidget *parent) :
    lds_listwidget(parent)
{

}

frontViewDialog_list::~frontViewDialog_list()
{
}


void frontViewDialog_list::test()
{
}

void frontViewDialog_list::setCurrentRow(int row)
{
    lds_listwidget::setCurrentRow(row);
    frontviewdialog_frame_list_item *w=qobject_cast<frontviewdialog_frame_list_item *>(this->itemWidget(this->item(row)));
    if(w){
        w->setChecked(true);
    }
}

void frontViewDialog_list::to_payfor_info(const QString &info)
{
    if(paintinfo.isEmpty()
            &&info.isEmpty()) return;
    paintinfo=info;
    update();
}

void frontViewDialog_list::paintEvent(QPaintEvent *e)
{
    QListWidget::paintEvent(e);
    QPainter painter(this->viewport());
    QFont font=this->font();
    font.setPixelSize(17);
    font.setBold(true);
    painter.setFont(font);
    painter.setPen(Qt::red);
    painter.drawText(QRect(20, 0,this->width()-40,this->height()),paintinfo);
}
