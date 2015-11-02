#ifndef W_M_MEMBER_H
#define W_M_MEMBER_H

#include <QDialog>
#include <QSqlQuery>
#include <QStandardItemModel>
#include <QSqlRecord>
#include "backheader.h"


//主界面
class w_m_member: public QDialog
{
    Q_OBJECT
public:
    explicit w_m_member(QWidget *parent = 0);
public:
    //会员状态对照表
    static QStandardItemModel VIP_MODEL_STATE;//t_m_member, ch_state
    //持卡状态对照表
    static QStandardItemModel VIP_MODEL_FLAG;// t_m_member,ch_cardflag
    //会员卡类型对照表
    static QStandardItemModel VIP_MODEL_ATTRI;//t_m_member_type ch_attribute
    //会员卡优惠方式对照表
    static QStandardItemModel VIP_MODEL_FAVOUR;//t_m_member_type ch_favourable
    //会员卡积分
    static QStandardItemModel VIP_MODEL_CALC;//t_m_member_type ch_calc
    //会员卡是否取整
    static QStandardItemModel VIP_MODEL_CALCTYPE;//t_m_member_type ch_calc_type
    //会员卡消费积分范围
    static QStandardItemModel VIP_MODEL_AREA;//t_m_member_type ch_area_flag
    //会员卡积分类型
    static QStandardItemModel VIP_MODEL_POINT_TYPE;//t_m_point ch_type
    //充值支付方式
    static QStandardItemModel RECHARGE_MODEL_PAYMODE;//t_m_member_type ch_area_flag

    static int VIP_MODEL_INIT();

    //querymodelsetfilters("select * from cey_bt_table")
    static QStringList get_viptypeno_name(const QString &sql){//
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
};

#endif // W_M_MEMBER_H
