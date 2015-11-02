#include "lds_tableview_delegate_check.h"
#include <QCheckBox>
#include <QApplication>
#include <QTableView>
#include <QPainter>
#include <QVariant>

lds_tableview_delegate_check::lds_tableview_delegate_check(QVector<int> &check_pos, QObject *parent, bool ischecked) :
    QItemDelegate(parent),_check_pos(check_pos),ischeckable(false)
{
    if(ischecked){
        setClickable();
    }
}

lds_tableview_delegate_check::lds_tableview_delegate_check(int aftercolumn, QObject *parent, bool ischecked):
    QItemDelegate(parent),ischeckable(false)
{
    int max=100;
    QVector<int> vpos;

    for(int m=0;m<max;m++){
        vpos<<(aftercolumn+m);
    }
    _check_pos = vpos;
    if(ischeckable){
        setClickable();
    }
}

lds_tableview_delegate_check::~lds_tableview_delegate_check()
{
    if(ischeckable&&this->parent()){
        QTableView *v=qobject_cast<QTableView *>(this->parent());
        if(v){
            disconnect(v,SIGNAL(clicked(QModelIndex)),this,SLOT(inter_clicked(QModelIndex)));
        }
    }
}


void lds_tableview_delegate_check::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(_check_pos.contains(index.column())){
        QColor bg=qvariant_cast<QColor>(index.data(Qt::BackgroundRole));
        if(bg.isValid()){
            painter->save();
            painter->setPen(Qt::NoPen);
            painter->setBrush(bg);
            painter->drawRect(option.rect);
            painter->restore();
        }
        QPixmap p(
                    index.data().toString()=="Y"?
                        (ischeckable&&index.flags()&Qt::ItemIsEditable? ":/image/widgets_pic/check_on":":/image//widgets_pic/disable_check_on" ):
                        (ischeckable&&index.flags()&Qt::ItemIsEditable? ":/image/widgets_pic/check_off":":/image//widgets_pic/disable_check_off")
                        );
        p=p.scaled(p.size()*0.5);
        painter->drawPixmap(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, p.size(), option.rect), p);
    } else {
        return QItemDelegate::paint(painter, option, index);
    }
}

QWidget *lds_tableview_delegate_check::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(_check_pos.contains(index.column())){
        return 0;
    }
    return QItemDelegate::createEditor(parent, option, index);
}

void lds_tableview_delegate_check::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QItemDelegate::setEditorData(editor, index);
}

void lds_tableview_delegate_check::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QItemDelegate::setModelData(editor, model, index);
}

void lds_tableview_delegate_check::inter_clicked(const QModelIndex &index)
{
    if(index.flags()&Qt::ItemIsEditable
            && index.flags()&Qt::ItemIsSelectable
            && index.flags()&Qt::ItemIsEnabled
            &&_check_pos.contains(index.column())){
        QAbstractItemModel *model=qobject_cast<QTableView *>(this->parent())->model();
        if(model){
            if(index.data().toString()=="Y"){
                model->setData(index, "N");
            } else {
                model->setData(index, "Y");
            }
        }
        emit checkclick(index);
    }
}

void lds_tableview_delegate_check::setClickable()
{
    if(ischeckable) return;
    ischeckable=true;
    if(this->parent()){
        QTableView *v=qobject_cast<QTableView *>(this->parent());
        if(v){
            connect(v,SIGNAL(clicked(QModelIndex)),this,SLOT(inter_clicked(QModelIndex)));
        }
    }
}

bool lds_tableview_delegate_check::clickable()
{
    return ischeckable;
}


