#include "lds_treewidget.h"
#include <QDebug>
#include <QMouseEvent>

lds_treeWidget::lds_treeWidget(QWidget *parent) :
    QTreeWidget(parent)
{
    rootitem=0;
    stack_row.push(-1);
    stack_row.push(-1);

    connect(this,SIGNAL(clicked(QModelIndex)),this,SLOT(savedeep()));
}

void lds_treeWidget::deleteAllItem()
{
    for(int r=0,r_count=rootitem->childCount();r<r_count;r++){
        if(rootitem->child(r)){
            for(int rr=0,rr_count=rootitem->child(r)->childCount();rr<rr_count;rr++){
                delete rootitem->child(r)->takeChild(0);
            }
        }
        delete rootitem->takeChild(0);
    }
}

void lds_treeWidget::select_beforerow()
{
    this->set_0_Index(stack_row.last());
}

int lds_treeWidget::get_beforerow()
{
    return stack_row.last();
}

int lds_treeWidget::deepofitem(QTreeWidgetItem *item)
{
    int deep = 0;
    if(item){
        while(item->parent()){
            deep++;
            item=item->parent();
        }
    }
    return deep;
}

void lds_treeWidget::set_0_Index(int index)
{
    treedeepvect.clear();
    treedeepvect << 0;
    treedeepvect <<index;
    setcurdeep();
}

int lds_treeWidget::get_0_Index()
{
    if(treedeepvect.count() >=2){
        if(treedeepvect[0]==0){
            return treedeepvect[1];
        }
    }
    return -1;
}

void lds_treeWidget::restore()
{
    //011
    setcurdeep();
}

void lds_treeWidget::setcurdeep()
{
    QTreeWidgetItem *fitem=rootitem;
    if(rootitem){
        for(int c=0,c_count=treedeepvect.count();c<c_count;c++){
            if(c==0){
                fitem=this->topLevelItem(treedeepvect[c]);
            } else {
                fitem=fitem->child(treedeepvect[c]);
            }
        }
        this->setCurrentItem(fitem);
        this->expandItem(fitem);

        savedeep();
    }
}

void lds_treeWidget::savedeep()
{
    treedeepvect.clear();
    QTreeWidgetItem *fitem=this->currentItem();
    while(fitem){
        if(!fitem->parent()) break;
        treedeepvect.push_front(fitem->parent()->indexOfChild(fitem));
        fitem=fitem->parent();
    }
    treedeepvect.push_front(indexOfTopLevelItem(fitem));

    stack_row.push_front(this->currentIndex().row());
    stack_row.pop_back();
}

void lds_treeWidget::mousePressEvent(QMouseEvent *event)
{
    QTreeWidgetItem*item= itemAt(event->pos());
    QTreeWidgetItem*item0= this->currentItem();
    QModelIndex cur=this->currentIndex();

    if(item
            &&item0
            &&item==item0){
        this->setCurrentItem(item);
        if(this->isExpanded(cur))
            this->collapse(cur);
        else
            this->expand(cur);
    } else {
        if(item)this->setCurrentItem(item);
    }
    emit pressed(cur);
    savedeep();
}



