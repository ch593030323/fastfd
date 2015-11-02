#include "pre_autoexpand_help.h"
pre_autoexpand_help::pre_autoexpand_help(QObject *parent) :
    QObject(parent)
{
    glayout=new QGridLayout;
    space=6;
    glayout->setSpacing(6);
}

pre_autoexpand_help::PVN pre_autoexpand_help::get_next_actualcount()
{
    int count=hcount*vcount;
    int p,v,n;
    int previous_count=0;
    QString _sql;
    v=count;
    foreach(PVN v, pvnlist){
        previous_count+=v.v;
    }

    if(pvnlist.isEmpty())//开始
    {
        p=-2;
        query.exec(QString("%1 limit %2,%3").arg(sql).arg(count).arg(1));
        if(query.next()){
            n=1;
        } else {
            n=-2;
        }
        _sql=QString("%1 limit %2,%3").arg(sql).arg(0).arg(v);
    } else {//之前已经有过
        p=-1;
        query.exec(QString("%1 limit %2,%3").arg(sql).arg(previous_count+count).arg(1));
        if(query.next()){
            n=1;
        } else {
            n=-2;
        }
        _sql=QString("%1 limit %2,%3").arg(sql).arg(previous_count).arg(v);
    }
    pvnlist.append(PVN(p,v,n,_sql));
    return pvnlist.last();
}

pre_autoexpand_help::PVN pre_autoexpand_help::get_previous_actualcount()
{
    if(pvnlist.isEmpty()) return PVN(-2,-1,-2);
    pvnlist.pop_back();
    if(pvnlist.isEmpty()) return PVN(-2,-1,-2);
    return pvnlist.last();
}

void pre_autoexpand_help::glayout_deleteall()
{
    for(int index=0,index_count=glayout->count();index<index_count;index++){
        QWidget* widget = glayout->itemAt(0)->widget();
        glayout->removeWidget(widget);
        delete widget;
    }
    pagekeylist.clear();
}

void pre_autoexpand_help::pvnlist_clear()
{
    pvnlist.clear();
}
