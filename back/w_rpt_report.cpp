#include "w_rpt_report.h"
#include <QDebug>

int w_rpt_report::frame_left_fixed_width=199;

datetimeModel::datetimeModel(QObject *parent):QSqlQueryModel(parent){

}

QVariant datetimeModel::data(const QModelIndex &item, int role) const
{
    if(role==Qt::DisplayRole){
        if(QSqlQueryModel::data(item, role).type()==QVariant::DateTime){
            return QSqlQueryModel::data(item, role).toDateTime().toString("yyyy-MM-dd hh:mm:ss");
        }
        if(this->headerData(item.column(), Qt::Horizontal,  Qt::DisplayRole).toString().endsWith(tr("率"))
                ||this->headerData(item.column(), Qt::Horizontal,  Qt::DisplayRole).toString().endsWith(tr("百分比"))
                ||this->headerData(item.column(), Qt::Horizontal,  Qt::DisplayRole).toString().endsWith(tr("比例"))
                ){
            return   QString().sprintf("%.02f",QSqlQueryModel::data(item, role).toFloat()*100) +"%";
        }
    }
    return QSqlQueryModel::data(item, role);
}


datetimestandModel::datetimestandModel(QObject *parent):QStandardItemModel(parent)
{

}

QVariant datetimestandModel::data(const QModelIndex &item, int role) const
{
    if(role==Qt::DisplayRole){
        if(QStandardItemModel::data(item, role).type()==QVariant::DateTime){
            return QStandardItemModel::data(item, role).toDateTime().toString("yyyy-MM-dd hh:mm:ss");
        }
        if(this->headerData(item.column(), Qt::Horizontal,  Qt::DisplayRole).toString().endsWith(tr("率"))
                ||this->headerData(item.column(), Qt::Horizontal,  Qt::DisplayRole).toString().endsWith(tr("百分比"))
                ||this->headerData(item.column(), Qt::Horizontal,  Qt::DisplayRole).toString().endsWith(tr("比例"))
                ){
            return   QString().sprintf("%.02f",QStandardItemModel::data(item, role).toFloat()*100) +"%";
        }
    }
    return QStandardItemModel::data(item, role);
}


w_rpt_report::w_rpt_report(QWidget *parent)
{

}

QStringList w_rpt_report::querymodelsetfilters(const QString &sql)
{
    QStringList rets;
    if(sql.isEmpty()) return rets;
    rets << QObject::tr("全部");
    QSqlQuery query;
    query.exec(sql);
    while(query.next()){
        rets << QString("%1(%2)")
                .arg(query.record().value(1).toString())
                .arg(query.record().value(0).toString());
    }
    return rets;
}

QString w_rpt_report::querymodelordersort(QTableView *table)
{
    QString ret="0 ASC";
    if(table->horizontalHeader()){
        ret=QString("%1 %2").arg(qMax(0, table->horizontalHeader()->sortIndicatorSection()))
                .arg(table->horizontalHeader()->sortIndicatorOrder()==Qt::AscendingOrder? "ASC" : "DESC");
    }
    return ret;
}

qlonglong w_rpt_report::sqltime_mindiff(const QDateTime &dt_begin, const QDateTime &da_end)
{
    QSqlQuery query;
    query.prepare("select ABS(round(TIME_TO_SEC(TIMEDIFF(:dt_begin , :da_end))/60)) as 'minutediff' ");
    query.bindValue(":dt_begin", dt_begin);
    query.bindValue(":da_end", da_end);
    query.exec();
    query.next();
    return query.record().value("minutediff").toLongLong();
}
