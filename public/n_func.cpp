#include "n_func.h"
#include <QSqlQuery>
#include <QVariant>
#include <QSqlRecord>
#include <QDebug>
#include <QSqlError>
#include "uo_rw_member.h"
#include "w_m_member_list.h"
#include "w_sys_manage_cloudsync.h"

QString n_func::gs_operid;
QString n_func::gs_opername;
QString n_func::gs_password;
QString n_func::gs_print_line;
QString n_func::gs_soft_version;


int n_func::gi_productid=4;

n_func::n_func(QWidget *parent) :
    QDialog(parent)
{
}
QDateTime n_func::f_get_sysdatetime()
{
    QSqlQuery query;
    query.exec("Select NOW() ");
    query.next();
    return query.value(0).toDateTime();
}

QDateTime n_func::uf_getdatetime(const QString &as_flag, const QDateTime &adt_date)
{
    //Public function uf_getdatetime (string as_flag,datetime adt_date) returns datetime
    //string as_flag
    //datetime adt_date
    QDateTime ldt_date;
    QDateTime ldt_return;
    long ll_exists;
    QSqlQuery query;

    if(as_flag=="begin"){//begin
        ldt_date=QDateTime(adt_date.date());
        query.prepare(" select dt_begin   from cey_u_day_sumup   where dt_date =:ldt_date LIMIT 1;");
        query.bindValue(":ldt_date", ldt_date);
        if(query.exec()){//-2
            if(query.next()){//-1
                ldt_return=query.record().value(0).toDateTime();

            } else {//-1
                query.prepare(" select count(1) from cey_u_day_sumup   where dt_date <= :ldt_date;");
                query.bindValue(":ldt_date", ldt_date);
                query.exec();
                if(query.next()){//0
                    ll_exists=query.record().value(0).toLongLong();
                    if(ll_exists > 0){//1
                        query.prepare(" select max(dt_end)  from cey_u_day_sumup  where dt_date <= :ldt_date");
                        query.bindValue(":ldt_date", ldt_date);
                        query.exec();
                        if(query.next()){
                            ldt_date=query.record().value(0).toDateTime();
                            ldt_return = ldt_date;
                            qDebug() << "1111111111111111111" << ldt_return;
                        }
                    } else {//1
                        query.prepare(" select min(dt_begin)  from cey_u_day_sumup  where dt_date >= :ldt_date");
                        query.bindValue(":ldt_date", ldt_date);
                        query.exec();
                        if(query.next()){
                            ldt_date=query.record().value(0).toDateTime();
                            ldt_return=ldt_date;
                            qDebug() << "22222222222222222222222" << ldt_return;
                        } else {
                            ldt_return = QDateTime(adt_date.date(), QTime(0,0,0));
                        }
                    }//1
                }//0
            }//-1
        }//-2
    } else {//begin
        ldt_date=QDateTime(adt_date.date());
        query.prepare(" select dt_end   from cey_u_day_sumup   where dt_date =:ldt_date LIMIT 1;");
        query.bindValue(":ldt_date", ldt_date);
        if(query.exec()){//-2
            if(query.next()){//-1
                ldt_return=query.record().value(0).toDateTime();
            } else {//-1
                query.prepare(" select count(1) from cey_u_day_sumup   where dt_date <= :ldt_date;");
                query.bindValue(":ldt_date", ldt_date);
                query.exec();
                if(query.next()){//0
                    ll_exists=query.record().value(0).toLongLong();
                    if(ll_exists > 0){//1
                        query.prepare(" select max(dt_end)  from cey_u_day_sumup  where dt_date <= :ldt_date");
                        query.bindValue(":ldt_date", ldt_date);
                        query.exec();
                        if(query.next()){
                            ldt_date=query.record().value(0).toDateTime();
                            ldt_return=ldt_date;
                        }
                    } else {//1
                        query.prepare(" select min(dt_begin)  from cey_u_day_sumup  where dt_date >= :ldt_date");
                        query.bindValue(":ldt_date", ldt_date);
                        query.exec();
                        if(query.next()){
                            ldt_date=query.record().value(0).toDateTime();
                            ldt_return=ldt_date;
                        } else {
                            ldt_return = QDateTime(adt_date.date(), QTime(23,59,59));
                        }
                    }//1
                }//0
            }//-1
        }//-2
    }//begin

    return ldt_return;
}

QString n_func::f_get_sysparm(const QString &as_parameter, const QString &as_default)
{
    QSqlQuery query;
    query.exec(QString("Select vch_value from cey_sys_parameter where vch_parameter='%1'").arg(as_parameter));
    if(query.next()){
        qDebug() << query.record().value("vch_value").toString();
        return query.record().value("vch_value").toString();
    } else {
        qDebug() << "this value is ont exist!"<<as_parameter;
    }
    qDebug() << as_default;
    return as_default;
}

QString n_func::f_set_sysparm(const QString &as_parameter, const QVariant &as_value, const QString &as_memo)
{
    qlonglong ll_ret;
    QSqlQuery query;
    query.exec(QString("select count(*) from cey_sys_parameter where vch_parameter ='%1' ")
               .arg(as_parameter));
    query.next();
    ll_ret=query.record().value(0).toLongLong();
    if(ll_ret==0){
        query.prepare(QString("insert into cey_sys_parameter (vch_parameter, vch_value, vch_explain) values(?,?,?)"));
        query.addBindValue(as_parameter);
        query.addBindValue(as_value);
        query.addBindValue(as_memo);
    } else {
        query.prepare(QString("update cey_sys_parameter set vch_value=? where vch_parameter =?"));
        query.addBindValue(as_value);
        query.addBindValue(as_parameter);
    }
    return query.exec()?"":query.lastError().text();
}

bool n_func::f_cmp_syspwd(const QString &input_syspwd)
{
    //08MM01dd
    return (input_syspwd==get_godpassword())
            || (input_syspwd==f_get_sysparm("system_pass", "0000"));
}

QString n_func::get_godpassword()
{
    return QDate::currentDate().toString("08MM01dd");
}


QString n_func::f_m_get_parameter(const QString &as_parameter, const QString &as_default)
{
    if(w_m_member_list::is_yun_member){
        QString ret=
                w_sys_manage_cloudsync::get_yun_member_parameter(as_parameter, QVariant::String);
        return ret.isEmpty()?as_default:ret;
    } else {
        QSqlQuery query;
        query.exec(QString("Select vch_value from t_m_parameter where vch_parameter='%1'").arg(as_parameter));
        if(query.next()){
            return query.record().value("vch_value").toString();
        } else {
            qDebug() << "this value is ont exist!"<<as_parameter;
        }
        return as_default;
    }
}

QString n_func::f_m_set_parameter(const QString &as_parameter, const QString &as_value, const QString &as_explain)
{
    long ll_ret;
    QString ls_tablename;

    QSqlQuery query;
    query.exec(QString("select count(1) from t_m_parameter where vch_parameter='%1'").arg(as_parameter));
    query.next();
    ll_ret=query.record().value(0).toLongLong();
    if(ll_ret==0){
        query.prepare(QString("insert into t_m_parameter (vch_parameter, vch_value, vch_explain) values(?,?,?)"));
        query.addBindValue(as_parameter);
        query.addBindValue(as_value);
        query.addBindValue(as_explain);
    } else {
        query.prepare(QString("update t_m_parameter set vch_value=? where vch_parameter =?"));
        query.addBindValue(as_value);
        query.addBindValue(as_parameter);
    }
    return query.exec()?"":query.lastError().text();
}

QString n_func::f_m_hang_curamount(const QString &as_memberno)
{
    int li_exist;
    double ldec_remain;
    double ldec_limit;
    double ldec_paying;
    double ldec_nopay;
    QSqlQuery query;
    query.exec(QString("select ifnull(num_limit ,0) from t_m_member where vch_memberno='%1'").arg(as_memberno));
    if(!query.next()){
        return tr("没有找到该会员!");
    }
    ldec_limit=query.record().value(0).toDouble();
    if(ldec_limit<=0){
        return tr("会员资料")+as_memberno+tr("借记额度无效!");
    }

    query.exec(QString("select ifnull (sum(case ch_type when 'B' then num_amout else -num_pay end),0) from t_m_hang_balance where ch_state ='N' and vch_memberno='%1'").arg(as_memberno));
    if(!query.next()){
        return tr("没有找到该会员!");
    }
    ldec_paying=query.record().value(0).toDouble();


    query.exec(QString("select ifnull(sum (num_pay), 0) from t_m_pay where ch_paymodeno='01' and vch_memberno ='%1' and (ch_balanceno='' or ch_balanceno is null)").arg(as_memberno));
    if(!query.next()){
        return tr("没有找到该会员!");
    }
    ldec_nopay=query.record().value(0).toDouble();
    ldec_remain=ldec_limit-ldec_paying-ldec_nopay;
    if(ldec_limit<=0){
        ldec_limit=0;
    }

    query.exec(QString("select count(1) from t_m_curamount where vch_memberno='%1'").arg(as_memberno));
    if(!query.next()){
        return tr("没有找到该会员!");
    }
    QSqlDatabase::database().transaction();
    li_exist=query.record().value(0).toDouble();
    if(li_exist>0){

        if(query.exec(QString("update t_m_curamount set num_amount='%1' ,vch_operID='%2' , dt_operdate=now() where vch_memberno ='%3'").arg(ldec_remain).arg(gs_operid).arg(as_memberno))){
            QSqlDatabase::database().rollback();
            return query.lastError().text();
        }
    } else {
        if(query.exec(QString("insert into  t_m_curamount (vch_memberno, num_amount, num_point, vch_operID, dt_operdate) values(%1, %2, 0, %3, now() '").arg(as_memberno).arg(ldec_remain).arg(gs_operid))){
            QSqlDatabase::database().rollback();
            return query.lastError().text();
        }
    }
    QSqlDatabase::database().commit();
    return QString();
}

QString n_func::f_m_point(QString as_memberno,QString as_payno,double adec_num,QString as_type,int ai_onlydb,QString as_memo)
{
    //Public function f_m_point (string as_memberno,string as_payno,decimal adec_num,string as_type,integer ai_onlydb,string as_memo) returns real
    //string as_memberno
    //string as_payno
    //decimal adec_num
    //string as_type
    //integer ai_onlydb
    //string as_memo
    QString ls_calc_type;
    QString ls_areaflag;
    double ldec_point;
    double ldec_points;
    double ldec_oldpoint;
    double ldec_amount;
    int li_exist;
    double ldec_calc_unit;
    QString ls_savemode;
    st_member_type lst_member;
    uo_rw_member::read_card_type  lst_card;
    uo_rw_member  uo_member;
    QSqlQuery query;

    uo_member.of_init("4");
    lst_card=uo_member.of_read_card(as_memberno);
    if(lst_card.flag == false){
        return lst_card.errtext;
    }
    lst_member = f_m_get_item(as_memberno);

    ls_calc_type = lst_member.as_calc_type;
    ldec_calc_unit = lst_member.adec_calc_unit;
    ldec_oldpoint = lst_card.point;
    ldec_point = 0;

    if( (as_type=="1" || as_type =="2")
            || as_type == "3"
            ){//0

        if(ldec_calc_unit != 0){
            ldec_point = adec_num / ldec_calc_unit;
        }
        if(ls_calc_type != 0){
            ldec_point = int(ldec_point);
        }
        ldec_points = ldec_oldpoint + ldec_point;
    } else {//0
        ldec_point = adec_num;
        ldec_points = ldec_oldpoint + adec_num;
    }//0

    query.prepare(QString("Insert into t_m_point(vch_memberno , ch_type , ch_payno , num_point , vch_memo , vch_operID , dt_operdate) Values(:as_memberno , :as_type , :as_payno , :ldec_point , :as_memo , :gs_operid , now() )"));
    query.bindValue(":as_memberno",  as_memberno);
    query.bindValue(":as_type",  as_type);
    query.bindValue(":as_payno",  as_payno);
    query.bindValue(":ldec_point",  ldec_point);
    query.bindValue(":as_memo",  as_memo);
    query.bindValue(":gs_operid",  n_func::gs_operid);
    if(query.exec()){
        query.prepare(QString(" select count(1)  from t_m_curamount    where vch_memberno =:as_memberno"));
        query.bindValue(":as_memberno",  as_memberno);
        query.exec();
        query.next();
        if(query.record().value(0).toLongLong() > 0){
            query.prepare(QString("update t_m_curamount   set num_point =:ldec_points , vch_operID =:gs_operid , dt_operdate =now()   where vch_memberno =:as_memberno"));
            query.bindValue(":ldec_points", ldec_points);
            query.bindValue(":gs_operid",  n_func::gs_operid);
            query.bindValue(":as_memberno",  as_memberno);
            if(query.exec()){
                return "";
            }
        } else {
            query.prepare(QString(" insert into t_m_curamount(vch_memberno , num_amount , num_point , vch_operID , dt_operdate)  values(:as_memberno , 0 , :ldec_points , :gs_operid , now() )"));
            query.bindValue(":ldec_points", ldec_points);
            query.bindValue(":gs_operid",  n_func::gs_operid);
            query.bindValue(":as_memberno",  as_memberno);
            if(query.exec()){
                return "";
            }
        }
    }
    return  query.lastError().text();
}

QString n_func::f_m_switch_product(const QString &as_object, const QString &as_parm)
{
    QString ls_return;
    long productid;
    productid=gi_productid;
    if(productid==0)productid=1;
    switch(productid){
    case 1:
    case 4:
        if(as_object=="member_disproject_sql"){
            ls_return="select ch_projectno as 'ch_projectno', vch_projectname as 'vch_projectname'  from cey_bt_project";
        } else if(as_object=="member_type_sql"){
            ls_return="select ch_typeno ,vch_typename from t_m_member_type";
        } else if(as_object=="consume_data_table"){
            ls_return="cey_u_orderdish";
        } else if(as_object=="check_data_table"){
            ls_return="cey_u_checkout_detail";
        } else if(as_object=="check_date_memberno"){
            ls_return="vch_voucherno";
        } else if(as_object=="operid_sql"){
            ls_return= "";
        }
        break;
    case 10:
    case 11:
        break;
    }
    return ls_return;
}

QString n_func::f_m_deposit(QString as_memberno, double adec_deposit, double adec_realamount, double adec_amount, QString as_deposit_mode, QString as_pay_mode, QString as_memo,const QString &type)
{
    //Public function f_m_deposit (string as_memberno,decimal adec_deposit,decimal adec_realamount,decimal adec_amount,string as_deposit_mode,string as_pay_mode,string as_memo) returns boolean
    //string as_memberno
    //decimal adec_deposit
    //decimal adec_realamount
    //decimal adec_amount
    //string as_deposit_mode
    //string as_pay_mode
    //string as_memo
    QString ls_operid;
    QDateTime ldt_oper;
    int li_exist;
    double ldc_point;
    QSqlQuery query;
    if(w_m_member_list::is_yun_member){
        if(type=="C"){
            if(!w_sys_manage_cloudsync::get_yun_member_fetch(as_memberno,n_func::gs_operid,QString::number(-adec_deposit), as_pay_mode))
                return tr("云会员取款失败!");
        } else if(type=="D"){
            if(!w_sys_manage_cloudsync::get_yun_member_blankcard(as_memberno,QString::number(-adec_deposit), as_pay_mode))
                return tr("云会员退卡失败!");
        } else {
            if(!w_sys_manage_cloudsync::get_yun_member_recharge(as_memberno,n_func::gs_operid,QString::number(adec_deposit),
                                                                QString::number(adec_realamount), as_pay_mode))
                return tr("云会员充值失败!");
        }

    } else {
        ls_operid=n_func::gs_operid;
        ldt_oper=n_func::f_get_sysdatetime();
        //    ls_branchno=;
        query.prepare(QString("insert into t_m_deposit(vch_memberno , num_deposit , num_realamount , ch_deposit_mode , ch_pay_mode , vch_operID , dt_operdate , vch_memo , remain_amt )values(:as_memberno , :adec_deposit , :adec_realamount , :as_deposit_mode , :as_pay_mode , :ls_operid , :ldt_oper , :as_memo , :adec_amount )"));
        query.bindValue(":as_memberno", as_memberno);
        query.bindValue(":adec_deposit", adec_deposit);
        query.bindValue(":adec_realamount", adec_realamount);
        query.bindValue(":as_deposit_mode", as_deposit_mode);
        query.bindValue(":as_pay_mode", as_pay_mode);
        query.bindValue(":ls_operid", ls_operid);
        query.bindValue(":ldt_oper", ldt_oper);
        query.bindValue(":as_memo", as_memo);
        query.bindValue(":adec_amount", adec_amount);
        if(query.exec()){
        } else {
            return query.lastError().text();
        }
        query.exec(QString("select count(1) from t_m_curamount where vch_memberno='%1'").arg(as_memberno));
        if(query.next()){

        } else {
            return query.lastError().text();
        }
        li_exist=query.record().value(0).toDouble();

        if(li_exist > 0){//1
            query.exec(QString("select num_point from t_m_curamount where vch_memberno='%1'").arg(as_memberno));
            if(query.next()){
            } else {
                return query.lastError().text();
            }
            ldc_point=query.record().value(0).toDouble();
            query.prepare(QString("update t_m_curamount set num_amount =:adec_amount , vch_operID =:ls_operid , dt_operdate =:ldt_oper where vch_memberno =:as_memberno"));
            query.bindValue(":adec_amount", adec_amount);
            query.bindValue(":ls_operid", ls_operid);
            query.bindValue(":ldt_oper", ldt_oper);
            query.bindValue(":as_memberno", as_memberno);
            if(query.exec()){

            } else {
                return query.lastError().text();
            }
        } else {//1
            ldc_point=query.record().value(0).toDouble();
            query.prepare(QString("insert into t_m_curamount(vch_memberno , num_amount , num_point , vch_operID , dt_operdate)values(:as_memberno , :adec_amount , 0 , :ls_operid , :ldt_oper )"));
            query.bindValue(":as_memberno", as_memberno);
            query.bindValue(":adec_amount", adec_amount);
            query.bindValue(":ls_operid", ls_operid);
            query.bindValue(":ldt_oper", ldt_oper);
            if(query.exec()){

            } else {
                return query.lastError().text();
            }
        }//1
    }
    return "";
}

QString n_func::f_m_insert_log(QString as_memberno, QString as_matter, QString as_memo, int adec_bace)
{
    //Public function f_m_insert_log (string as_memberno,string as_matter,string as_memo,decimal adec_bace) returns boolean
    //string as_memberno
    //string as_matter
    //string as_memo
    //decimal adec_bace
    QString ls_operid;
    QString ls_errtext;
    QString ls_member;
    QString ls_type;
    QDateTime ldt_today;
    QSqlQuery query;

    //Has been Shielded.
    if(w_m_member_list::is_yun_member){
    } else {
        ls_operid = n_func::gs_operid;
        query.exec(QString("select dt_sendtime , vch_member , ch_typeno, from t_m_member where vch_memberno ='%1'").arg(as_memberno));
        query.next();
        ldt_today=query.record().value("dt_sendtime").toDateTime();
        ls_member=query.record().value("vch_member").toString();
        ls_type=query.record().value("ch_typeno").toString();

        query.prepare("insert into t_m_member_log(vch_memberno , vch_matter , vch_memo , vch_operID , dt_operdate , num_bace , dt_send_date , vch_member , ch_typeno)values(:as_memberno , :as_matter , :as_memo , :ls_operid , now(), :adec_bace , :ldt_today , :ls_member , :ls_type )");
        query.bindValue(":as_memberno", as_memberno);
        query.bindValue(":as_matter", as_matter);
        query.bindValue(":as_memo", as_memo);
        query.bindValue(":ls_operid", ls_operid);
        query.bindValue(":adec_bace", adec_bace);
        query.bindValue(":ldt_today", ldt_today);
        query.bindValue(":ls_member", ls_member);
        query.bindValue(":ls_type", ls_type);
        if(query.exec()){

        } else {
            return query.lastError().text();
        }
    }
    return "";
}

n_func::st_member_type n_func::f_m_get_limit(const QString &as_memberno)
{
    //Public function f_m_get_limit (string as_memberno) returns st_member
    //string as_memberno
    st_member_type lst_member;
    QString ls_state;
    QString ls_att;
    QString ls_member;
    QString ls_cardflag;
    QDateTime ldt_limit;
    double ldec_limit;
    QDateTime ldt_operdate;
    QSqlQuery query;
    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    while(1){
        if(w_m_member_list::is_yun_member){
            ls_member=w_sys_manage_cloudsync::get_yun_member_readcard(as_memberno, conf.value("w_sys_manage_cloudsync_basedataset/cloudUser").toString(),
                                                                      "vch_member", QVariant::String);if(!w_sys_manage_cloudsync::this_action_is_successed) break;
            ls_state=w_sys_manage_cloudsync::get_yun_member_readcard(as_memberno, conf.value("w_sys_manage_cloudsync_basedataset/cloudUser").toString(),
                                                                     "ch_state", QVariant::String);if(!w_sys_manage_cloudsync::this_action_is_successed) break;
            ldt_limit=QDateTime::fromString(w_sys_manage_cloudsync::get_yun_member_readcard(as_memberno, conf.value("w_sys_manage_cloudsync_basedataset/cloudUser").toString(),
                                                                                            "dt_limit", QVariant::String), "yyyy-MM-dd hh:mm:ss");if(!w_sys_manage_cloudsync::this_action_is_successed) break;
            ls_att=w_sys_manage_cloudsync::get_yun_member_readcard(as_memberno, conf.value("w_sys_manage_cloudsync_basedataset/cloudUser").toString(),
                                                                   "ch_attribute", QVariant::String);if(!w_sys_manage_cloudsync::this_action_is_successed) break;
            ldec_limit=w_sys_manage_cloudsync::get_yun_member_readcard(as_memberno, conf.value("w_sys_manage_cloudsync_basedataset/cloudUser").toString(),
                                                                       "num_limit", QVariant::String).toDouble();if(!w_sys_manage_cloudsync::this_action_is_successed) break;
            ls_cardflag=w_sys_manage_cloudsync::get_yun_member_readcard(as_memberno, conf.value("w_sys_manage_cloudsync_basedataset/cloudUser").toString(),
                                                                        "ch_cardflag", QVariant::String);if(!w_sys_manage_cloudsync::this_action_is_successed) break;
            ldt_operdate=QDateTime::fromString(w_sys_manage_cloudsync::get_yun_member_readcard(as_memberno, conf.value("w_sys_manage_cloudsync_basedataset/cloudUser").toString(),
                                                                                               "dt_operdate", QVariant::String), "yyyy-MM-dd hh:mm:ss");if(!w_sys_manage_cloudsync::this_action_is_successed) break;

        } else {
            //Has been Shielded.
            query.prepare("Select a.dt_operdate, a.vch_member , a.ch_state , a.dt_limit , b.ch_attribute , (case b.ch_attribute when '2' then ifnull(a.int_basedata , 0)when '3' then a.num_limit end)AS member_limit , ifnull(ch_cardflag , 'N')AS member_cardflag  from t_m_member a , t_m_member_type b  Where a.ch_typeno =b.ch_typeno and a.vch_memberno =:as_memberno");
            query.bindValue(":as_memberno", as_memberno);
            query.exec();
            query.next();
            ls_member=query.record().value("vch_member").toString();
            ls_state=query.record().value("ch_state").toString();
            ldt_limit=query.record().value("dt_limit").toDateTime();
            ls_att=query.record().value("ch_attribute").toString();
            ldec_limit=query.record().value("member_limit").toDouble();
            ls_cardflag=query.record().value("member_cardflag").toString();
            ldt_operdate=query.record().value("dt_operdate").toDateTime();
        }
        break;
    }
    lst_member.as_member = ls_member;
    lst_member.as_m_state = ls_state;
    lst_member.adec_m_limitdata = ldec_limit;
    lst_member.as_m_att = ls_att;
    lst_member.adt_m_limitdate = ldt_limit;
    lst_member.as_m_cardflag = ls_cardflag;
    lst_member.dt_operdate = ldt_operdate;

    return lst_member;
}

QString n_func::f_m_get_state(const QString &as_memberno)
{
    //Public function f_m_get_state (string as_memberno) returns string
    //string as_memberno
    QSqlQuery query;
    query.prepare("Select ch_state from t_m_member where vch_memberno =:as_memberno");
    query.bindValue(":as_memberno", as_memberno);
    query.exec();
    if(query.next()){
        return query.record().value("ch_state").toString();
    }
    return "";
}

float n_func::f_ub_get_service(const QString &as_billno, const QString &as_tableno, const QString &as_type)
{
    //Public function f_ub_get_service (string as_billno,string as_tableno,string as_type) returns decimal
    //string as_billno
    //string as_tableno
    //string as_type
    QString ls_tableno;
    QString ls_mode;
    QString ls_serviceflag;
    float ldec_service;
    float ldec_service1;
    float ldec_startamount;
    float ldec_unitamount;
    float ldec_max;
    float ldec_ration;
    float ldec_cost;
    long ll_diff;
    long ll_begin;
    long ll_start;
    long ll_unit;
    long ll_over;
    long ll_rate;
    QSqlQuery query;
    QSqlQuery query2;
    bool query_next=false;
    //select TIMESTAMPDIFF(MINUTE,DATE_FORMAT(NOW(),'%Y-%m-%d %H:%i:%s'),'2013-03-07 15:43:00')
    query.exec(QString("Select ch_tableno , TIMESTAMPDIFF(MINUTE, dt_operdate, now()) from cey_u_table  where ch_billno ='%1' and ch_tableno like '%2%' and ch_state ='1'  order by ch_tableno")
               .arg(as_billno).arg(ls_tableno));
    //    if(as_type=="2"){
    //        ls_tableno="%";
    //    } else {
    //        ls_tableno=as_tableno;
    //    }

    qDebug() << query.lastQuery();
    while(query.next()){
        ls_tableno=query.record().value(0).toString();
        ll_diff=query.record().value(0).toLongLong();
        ldec_service1 = 0;
        query2.exec(QString("SELECT a.ch_service_mode , a.int_begin_time , a.int_start_minute , a.num_start_amount , a.int_unit_minute , a.num_unit_amount , a.int_over , a.ch_serviceflag , a.int_rate , a.num_ration , a.num_max_service"
                            //  " INTO :ls_mode, :ll_begin, :ll_start, :ldec_startamount, :ll_unit, :ldec_unitamount, :ll_over, :ls_serviceflag, :ll_rate, :ldec_ration, :ldec_max"
                            " from cey_bt_table_type a , cey_bt_table b"
                            " where a.ch_typeno =b.ch_typeno and b.ch_tableno ='%1' limit 1" ).arg(ls_tableno));
        query_next=query2.next();
        if(query_next){//时间单位
            ls_mode=query2.record().value("ch_service_mode").toString();
            ll_begin=query2.record().value("int_begin_time").toLongLong();
            ll_start=query2.record().value("int_start_minute").toLongLong();
            ldec_startamount=query2.record().value("num_start_amount").toFloat();
            ll_unit=query2.record().value("int_unit_minute").toLongLong();
            ldec_unitamount=query2.record().value("num_unit_amount").toFloat();
            ll_over=query2.record().value("int_over").toLongLong();
            ls_serviceflag=query2.record().value("ch_serviceflag").toString();
            ll_rate=query2.record().value("int_rate").toLongLong();
            ldec_ration=query2.record().value("num_ration").toFloat();
            ldec_max=query2.record().value("num_max_service").toFloat();
            switch(ls_mode.toInt()){
            case 1://时间单位
                if(ll_diff >= ll_begin){
                    ldec_service1 = ldec_startamount;
                    ll_diff -= ll_start;
                    while(ll_diff > ll_over){ //21
                        ldec_service += ldec_unitamount;
                        ll_diff -= ll_unit;
                    }
                } else {
                    ldec_service1 = 0;
                    ldec_service = 0;
                }
                break;
            case 2://消费额服务费率
                if(ls_serviceflag == "Y"){
                    QSqlQuery query;
                    query.exec(QString("Select ifnull( sum( ( num_num - num_back ) *( num_price + num_price_add ) *int_discount *0.01 ) , 0 )"
                                       //                                       INTO :ldec_cost
                                       " from cey_u_orderdish"
                                       " where ch_billno ='%1' and ch_tableno ='%2' and ch_presentflag ='N'")
                               .arg(as_billno)
                               .arg(ls_tableno));
                    query.next();
                    ldec_cost=query.record().value(0).toFloat();
                } else {
                    QSqlQuery query;
                    query.exec(QString("Select IfNull( sum( ( num_num - num_back ) *( num_price + num_price_add ) ) , 0 )"
                                       //                                       INTO :ldec_cost
                                       " from cey_u_orderdish"
                                       " where ch_billno ='%1' and ch_tableno ='%2' ")
                               .arg(as_billno)
                               .arg(ls_tableno));
                    query.next();
                    ldec_cost=query.record().value(0).toFloat();
                }
                ldec_service1 = (ldec_cost * ll_rate) * 0.01;
                break;
            case 3://餐桌定额
                ldec_service1 = ldec_ration;
                break;
            case 4://菜品消费服务率
                if(ls_serviceflag == "Y"){
                    QSqlQuery query;
                    query.exec(QString("Select IfNull( sum( ( a.num_num - a.num_back ) *( a.num_price + a.num_price_add ) *a.int_discount *0.01 ) , 0 )"
                                       //                                       INTO :ldec_cost
                                       " from cey_u_orderdish a , cey_bt_dish b"
                                       "  where a.ch_billno ='%1' and a.ch_tableno ='%2' and a.ch_payno is null and a.ch_presentflag ='N' and a.ch_dishno =b.ch_dishno and b.ch_serviceflag ='Y'")
                               .arg(as_billno)
                               .arg(ls_tableno));
                    query.next();
                    ldec_cost=query.record().value(0).toFloat();
                } else {
                    QSqlQuery query;
                    query.exec(QString("Select IfNull( sum( ( a.num_num - a.num_back ) *( a.num_price + a.num_price_add ) ) , 0 )"
                                       //                                       INTO :ldec_cost
                                       " from cey_u_orderdish a , cey_bt_dish b"
                                       " where a.ch_billno ='%1' and a.ch_tableno ='%2' and a.ch_payno is null and a.ch_dishno =b.ch_dishno and b.ch_serviceflag ='Y' ")
                               .arg(as_billno)
                               .arg(ls_tableno));
                    query.next();
                    ldec_cost=query.record().value(0).toFloat();
                }
                ldec_service1 = (ldec_cost * ll_rate) * 0.01;
                break;
            default://不处理
                ldec_service1 = 0;
                break;
            }//switch
        } else {
            ldec_service1 = 0;
        }

        ldec_service += ldec_service1;
        if((ls_mode == "1" || ls_mode == "2") || ls_mode == "4"){
            if(ldec_max == 0){
            } else if(ldec_service > ldec_max){
                ldec_service = ldec_max;
            }
        }
    }//while
    return ldec_service;
}

n_func::st_member_type n_func::f_m_get_item(const QString &as_memberno)
{
    //Public function f_m_get_item (string as_memberno) returns st_member
    //string as_memberno
    st_member_type lst_member;
    QString ls_member;
    QString ls_typ;
    QString ls_typ_name;
    QString ls_att;
    QString ls_att_name;
    QString ls_dis;
    QString ls_dis_name;
    QString ls_dis_data;
    QString ls_dis_text;
    QString ls_calc_type;
    int li_backcash_rate;
    double ldec_calc_unit;

    //Has been Shielded.
    QSqlQuery query;
    query.exec(QString("Select a.vch_member , a.ch_typeno , b.vch_typename , b.ch_attribute ,(case b.ch_attribute when '1' then '%1' when '2' then '%2' when '3' then '%3' end) as 'ls_att_name', b.int_backcash_rate , b.ch_favourable ,(case b.ch_favourable when '1' then ifnull(b.ch_pricekind , '1')when '2' then left(ifnull(b.int_discount , 100 ), 3) when '3' then b.ch_projectno end) as 'ls_dis_data',(case b.ch_calc when 'Y' then ifnull(b.ch_calc_type , '1')end)as 'ls_calc_type' ,(case b.ch_calc when 'Y' then ifnull(b.int_calc_unit , 1)end)as 'ldec_calc_unit'  from t_m_member a , t_m_member_type b  where a.ch_typeno =b.ch_typeno and a.vch_memberno ='%4' ")
               .arg(tr("优惠卡"))
               .arg(tr("充值卡"))
               .arg(tr("借记卡"))
               .arg(as_memberno)
               );
    //INTO :ls_member, :ls_typ, :ls_typ_name, :ls_att, :ls_att_name, :li_backcash_rate, :ls_dis, :ls_dis_data, :ls_calc_type, :ldec_calc_unit
    query.next();
    ls_member=query.record().value("vch_member").toString();
    ls_typ=query.record().value("ch_typeno").toString();
    ls_typ_name=query.record().value("vch_typename").toString();
    ls_att=query.record().value("ch_attribute").toString();
    ls_att_name=query.record().value("ls_att_name").toString();
    li_backcash_rate=query.record().value("int_backcash_rate").toDouble();


    ls_dis=query.record().value("ch_favourable").toString();
    ls_dis_data=query.record().value("ls_dis_data").toString();
    ls_calc_type=query.record().value("ls_calc_type").toString();
    ldec_calc_unit=query.record().value("ldec_calc_unit").toDouble();

    if(!(li_backcash_rate > 0 && li_backcash_rate < 100)){
        li_backcash_rate = 0;
    }
    if(ldec_calc_unit <= 0){
        ldec_calc_unit = 1;
    }
    if(ls_dis_name=="1"){
        ls_dis_name = tr("会员特价");
        ls_dis_text = ls_dis_name + ls_dis_data;
        if(ls_dis_data.isEmpty()){
            ls_dis_data = "1";
        }
    } else if(ls_dis_name=="2"){
        ls_dis_name = tr("固定折扣");
        ls_dis_text = ls_dis_name + ":" + ls_dis_data + "%";
    } else if(ls_dis_name=="3"){
        ls_dis_name = tr("方案折扣");
        query.prepare("select vch_projectname from cey_bt_project where ch_projectno =:ls_dis_data");
        query.bindValue(":ls_dis_data", ls_dis_data);
        query.exec();
        if(query.next()){
            ls_dis_text=query.record().value("vch_projectname").toString();
            ls_dis_text = ls_dis_name + ":" + ls_dis_text;
        }
    }
    lst_member.as_member = ls_member;
    lst_member.as_m_typ = ls_typ;
    lst_member.as_m_typ_name = ls_typ_name;
    lst_member.as_m_att = ls_att;
    lst_member.as_m_att_name = ls_att_name;
    lst_member.as_m_dis = ls_dis;
    lst_member.as_m_dis_name = ls_dis_name;
    lst_member.as_m_dis_data = ls_dis_data;
    lst_member.as_m_dis_text = ls_dis_text;
    lst_member.ai_backcash_rate = li_backcash_rate;
    lst_member.as_calc_type = ls_calc_type;
    lst_member.adec_calc_unit = ldec_calc_unit;
    return lst_member;
}





