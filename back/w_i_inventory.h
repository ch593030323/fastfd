#ifndef W_I_INVENTORY_H
#define W_I_INVENTORY_H

#include <QObject>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>
#include <QStringList>
#include <QStandardItemModel>
#include <QtDebug>
#include "backheader.h"

//w_i_inventory::MODEL_INVENTORY_INIT();

class w_i_inventory : public QObject
{
    Q_OBJECT
public:
    explicit w_i_inventory(QObject *parent = 0);
    //会员状态对照表
    static QStandardItemModel MODEL_INVENTORY_TYPE;//t_m_member, ch_state
    //报表状态，审核，作废之类
    static QStandardItemModel MODEL_INVENTORY_STATE_TYPE;//t_m_member, ch_state

    static int MODEL_INVENTORY_INIT();
};

static QStringList inventory_queryfilter(const QString &sql){
    QStringList rets;
    if(sql.isEmpty()) return rets;
    QSqlQuery query;
    query.exec(sql);
    while(query.next()){
        rets << QString("%1(%2)")
                .arg(query.record().value(1).toString())
                .arg(query.record().value(0).toString());
    }
    return rets;
}

//获取操作员的 name 更具id
static QString inventory_getopername(const QString &operid){
    QSqlQuery query;
    query.exec(QString("select * from (Select vch_operID,vch_operator from cey_sys_operator union Select '0000' as vch_operID,'system' as vch_operator )t where t.vch_operID='%1'  ")
               .arg(operid));
    query.next();
    return query.record().value("vch_operator").toString();

}

#endif // W_I_INVENTORY_H
