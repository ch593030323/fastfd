#include "lds_tableview.h"
#include <QItemSelectionModel>
#include <QDebug>
#include <QHeaderView>
#include <QScrollBar>
#include <QMouseEvent>
#include <QPainter>

lds_tableView::lds_tableView(QWidget *parent) :
    QTableView(parent)
{
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setSelectionMode(QAbstractItemView::SingleSelection);
    currow=-1;

    stack_row.push(-1);
    stack_row.push(-1);

    //!可拖动---------------begin
    this->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    this->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);

}

void lds_tableView::mousePressEvent(QMouseEvent *event)
{
    ismoving = false;
    pressp = event->pos();
    pressv = this->verticalScrollBar()->value();
    pressh = this->horizontalScrollBar()->value();
    QTableView::mousePressEvent(event);
    if(this->indexAt(event->pos()).isValid()){
    } else {
        this->clearSelection();
    }
}

void lds_tableView::mouseMoveEvent(QMouseEvent *event)
{
    int voff = event->pos().y() - pressp.y();
    int hoff = event->pos().x() - pressp.x();

    //若是移动超过20*20打矩形，才会真正移动
    if(qAbs(voff) >= 20 || qAbs(hoff) >= 20){
        ismoving = true;
        voff = -voff;
        hoff = -hoff;

        if((pressv + voff) <= this->verticalScrollBar()->maximum()+20
                && (pressv + voff) >= 0-20)
            this->verticalScrollBar()->setValue(pressv + voff);
        //        if((pressh + hoff) <= this->horizontalScrollBar()->maximum()+20
        //                && (pressh + hoff) >= 0-20)
        //            this->horizontalScrollBar()->setValue(pressh + hoff);
    } else {
        ismoving = false;
    }

    QTableView::mouseMoveEvent(event);
}

void lds_tableView::mouseReleaseEvent(QMouseEvent *event)
{
    QTableView::mouseReleaseEvent(event);
}

void lds_tableView::paintEvent(QPaintEvent *e)
{
    QTableView::paintEvent(e);
    QPainter painter(this->viewport());

    QFont font=this->font();
    font.setPixelSize(17);
    font.setBold(true);
    painter.setFont(font);
    painter.setPen(Qt::red);
    painter.drawText(QRect(20, 0,this->width()-40,this->height()),int(Qt::AlignCenter|Qt::TextWrapAnywhere),paintinfo);
}

//!可拖动---------------end
void lds_tableView::restore()
{
    this->selectRow(currow);
}

void lds_tableView::restore_rows()
{
    foreach(int r, currows)
        selectRow(r);
}

int lds_tableView::getcurrow()
{
    return currow;
}

QSet<int> lds_tableView::getcurrows()
{
    return currows;
}

void lds_tableView::select_beforerow()
{
    this->selectRow(stack_row.last());
}

int lds_tableView::get_beforerow()
{
    return stack_row.last();
}

void lds_tableView::saverow(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(deselected)
    //有选中
    if(selected.indexes().count()>0){
        QModelIndexList indexs=selected.indexes();
        foreach(QModelIndex index, indexs){
            currows.insert(index.row());
        }
    }
    if(deselected.indexes().count()>0){
        QModelIndexList indexs=deselected.indexes();
        foreach(QModelIndex index, indexs){
            currows.remove(index.row());
        }
    }
    if(selected.indexes().count() > 0){
        currow=selected.indexes().last().row();\
        stack_row.push_front(currow);
        stack_row.pop_back();

        emit selectchanged(currow);
        emit selectschanged(currows);
    }
}

void lds_tableView::setModel(QAbstractItemModel *model)
{
    QItemSelectionModel *selectmodel = this->selectionModel();
    QTableView::setModel(model);
    if(selectmodel)delete selectmodel;

    selectmodel=new QItemSelectionModel(model);
    this->setSelectionModel(selectmodel);
}

void lds_tableView::setSelectionModel(QItemSelectionModel *selectionModel)
{
    //1若是有则会disconnet and delete
    if(this->selectionModel()){
        disconnect(this->selectionModel(),SIGNAL(selectionChanged(QItemSelection,QItemSelection)),this,SLOT(saverow(QItemSelection,QItemSelection)));
        delete this->selectionModel();
    }
    //2
    QTableView::setSelectionModel(selectionModel);
    connect(selectionModel,SIGNAL(selectionChanged(QItemSelection,QItemSelection)),this,SLOT(saverow(QItemSelection,QItemSelection)));
}

QList<int> lds_tableView::rePecifyHeader(const QStringList &tos)
{
    return lds_tableview_repecify::rePecifyHeader(this, tos);
}

void lds_tableView::update_paintinfo(const QString &value)
{
    if(paintinfo.isEmpty()
            &&value.isEmpty()) return;
    paintinfo=value;
    update();
}

void lds_tableView::clearSelection()
{
    currows.clear();
    QTableView::clearSelection();
    this->setCurrentIndex(QModelIndex());
}

int lds_tableView::gonext()
{
    this->selectRow(currow+1);
    return this->currentIndex().row();
}

int lds_tableView::goprevious()
{
    this->selectRow(currow-1);
    return this->currentIndex().row();
}
