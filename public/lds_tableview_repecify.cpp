#include "lds_tableview_repecify.h"
#include <QModelIndex>
lds_tableview_repecify::lds_tableview_repecify(){}

/**
moveSection(12, 1)
from
0 1 2 3 4 5 6 7 8 9 10 11 12
to
0 12 1 2 3 4 5 6 7 8 9 10 11
moveSection(1, 12)
from
0 1 2 3 4 5 6 7 8 9 10 11 12
to
0 2 3 4 5 6 7 8 9 10 11 12 1
swapSections(1, 12)
*/
QList<int> lds_tableview_repecify::rePecifyHeader(QTableView *tableview, const QStringList &tos)
{
    QHeaderView* hheader=tableview->horizontalHeader();
    QList<int> intv_begin, intv_end;
    QList<int> intv_begin_tmp;
    QMap<QString, int> header_c;

    for(int c=0;c<hheader->count();c++){
        if(tos.contains(hheader->model()->headerData(c,Qt::Horizontal).toString())){
            header_c.insert(hheader->model()->headerData(c,Qt::Horizontal).toString(), c);//将表头作为key，列数作为value 插入map
            intv_begin << c;//获取排序完成前的 列数
        } else {
            tableview->setColumnHidden(c, true);//隐藏header
        }
    }
    foreach(QString str , tos){
        intv_end<<header_c.take(str);//获取排序完成后的 列数
    }
    //开始转换
    intv_begin_tmp=intv_begin;
    if(intv_begin.count()==intv_end.count()){
        for(int c=0,c_count=intv_end.count();c<c_count;c++){
            for(int cc=c,cc_count=intv_begin.count();cc<cc_count;cc++){
                if(intv_begin[cc]==intv_end[c]){
                    if(c==cc){
                    } else {
                        hheader->swapSections(intv_begin_tmp[c], intv_begin_tmp[cc]);
                        intv_begin.swap(c,cc);
                    }
                    break;
                }
            }
        }
    } else {
        qDebug() << "列数冲突" <<  intv_begin.count() << intv_end.count();
    }
    return intv_end;
}

QString lds_tableview_repecify::saveHeader(const QString &text, QStringList &ret)
{
    ret<<text;
    return text;
}
