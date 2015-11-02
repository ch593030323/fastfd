#include "uo_rw_member.h"
#include "n_func.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QVariant>
#include "reader_rf.h"
#include <QSqlTableModel>
#include "public_sql.h"
#include <QSettings>
#include "w_sys_manage_cloudsync.h"
#include "w_m_member_list.h"


uo_rw_member::uo_rw_member(QObject *parent) :
    QObject(parent)
{
}

uo_rw_member::read_card_type uo_rw_member::of_read_card(const QString &as_memberno)
{
    //Public function of_read_card (string as_memberno) returns read_card
    //string as_memberno

    read_card_type lst_card;
    QString ls_att;
    QString ls_err;
    QString cardname;
    QString card_type;
    double ldec_amount;
    double ldec_point;
    int li_exist;
    QString cardno;
    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);

    //Has been Shielded.
    lst_card.flag = false;
    lst_card.errtext=tr("会员读卡失败");
    if(as_memberno.isEmpty()){
        lst_card.errtext=tr("会员号不能为空！");
        return lst_card;
    }

    if(is_cardtype!="1" && is_cardtype!="2" && is_cardtype!="3"){
        lst_card.errtext = tr("读卡无效，卡类型：无会员卡！");
        return lst_card;
    }

    QSqlQuery query;
    //    query.prepare("Select b.ch_attribute from t_m_member a , t_m_member_type b Where a.ch_typeno =b.ch_typeno and vch_memberno =:as_memberno");
    //    query.bindValue(":as_memberno", as_memberno);
    //    query.exec();
    //    query.next();
    //    ls_att=query.record().value("ch_attribute").toString();
    cardno=as_memberno;
    while(1){
        if(w_m_member_list::is_yun_member){
            ldec_amount=w_sys_manage_cloudsync::get_yun_member_readcard(cardno, conf.value("w_sys_manage_cloudsync_basedataset/cloudUser").toString(),
                                                                        "num_amount", QVariant::Double).toDouble();if(!w_sys_manage_cloudsync::this_action_is_successed) break;
            ldec_point=w_sys_manage_cloudsync::get_yun_member_readcard(cardno, conf.value("w_sys_manage_cloudsync_basedataset/cloudUser").toString(),
                                                                       "num_point", QVariant::Double).toDouble();if(!w_sys_manage_cloudsync::this_action_is_successed) break;
        } else {
            query.prepare("Select ifnull(num_amount , 0), ifnull(num_point , 0) from t_m_curamount  where vch_memberno =:as_memberno");
            query.bindValue(":as_memberno", as_memberno);
            query.exec();
            query.next();
            ldec_amount=query.record().value(0).toDouble();
            ldec_point=query.record().value(1).toDouble();
        }
        lst_card.amount = ldec_amount;//会员余额
        lst_card.point = ldec_point;//会员积分
        lst_card.cardid = as_memberno;//会员卡号

        //会员类型
        if(w_m_member_list::is_yun_member){
            cardname=w_sys_manage_cloudsync::get_yun_member_readcard(cardno, conf.value("w_sys_manage_cloudsync_basedataset/cloudUser").toString(),
                                                                     "vch_member", QVariant::String);if(!w_sys_manage_cloudsync::this_action_is_successed) break;
            //会员类型
            card_type=w_sys_manage_cloudsync::get_yun_member_readcard(cardno, conf.value("w_sys_manage_cloudsync_basedataset/cloudUser").toString(),
                                                                      "vch_typename", QVariant::String);if(!w_sys_manage_cloudsync::this_action_is_successed) break;
        } else {
            query.prepare("select (select vch_typename from t_m_member_type where t_m_member_type.ch_typeno=t_m_member.ch_typeno), vch_member  from t_m_member where vch_memberno =:as_memberno");
            query.bindValue(":as_memberno", as_memberno);
            query.exec();
            query.next();
            cardname=query.record().value(1).toString();//会员名称
            card_type=query.record().value(0).toString();//会员类型
        }

        lst_card.flag = true;
        break;
    }
    return lst_card;
}

void uo_rw_member::of_init(const QString &as_action)
{
    QString ls_value=n_func::f_m_get_parameter("vip_card_type", tr("会员卡"));
    if(ls_value==tr("IC卡")){
        is_cardtype="1";

    } else if(ls_value==tr("磁卡")){
        is_cardtype="2";

    } else if(ls_value==tr("明卡")){
        is_cardtype="3";

    } else {
        is_cardtype="4";

    }
    if(as_action.isEmpty()){
        return ;
    }
    is_action=as_action;
    if(is_action=="1"){
        this->text=tr("读卡");
    } else if(is_action=="2"){
        this->text=tr("发卡");

    } else if(is_action=="3"){
        this->text=tr("清除");

    } else if(is_action=="4"){

    } else if(is_action=="5"){

    }

    if(is_action!="4"){
        visible=true;
    }
}

QString uo_rw_member::of_send_card(const QString &as_memberno)
{
    //Public function of_send_card (string as_memberno) returns boolean
    //string as_memberno
    QString ls_cardflag;
    QString ls_state;
    QString ls_cardno;
    QString ls_memberno;
    QString ls_err;
    QString ls_cardtype;
    int li_exists;
    QDateTime ldt_sendtime;
    int li_basedata;
    n_func::st_member_type lst_member;
    QSqlQuery query;

    if(as_memberno.isEmpty()){
        return tr("会员号不能为空!");
    } else {
        query.prepare("select ch_cardflag , ch_state , ifnull(int_basedata , 0) as int_basedata, ch_typeno , dt_sendtime from t_m_member  where vch_memberno =:as_memberno");
        query.bindValue(":as_memberno", as_memberno);
        query.exec();
        if(query.next()){//0
            ls_cardflag=query.record().value("ch_cardflag").toString();
            ls_state=query.record().value("ch_state").toString();
            li_basedata=query.record().value("int_basedata").toInt();
            ls_cardtype=query.record().value("ch_typeno").toString();
            ldt_sendtime=query.record().value("dt_sendtime").toDateTime();

            if(li_basedata==0){
                query.prepare("select ifnull(int_card_baseamount , 0) , now()  from t_m_member_type where ch_typeno =:ls_cardtype");
                query.bindValue(":ls_cardtype", ls_cardtype);
                query.exec();
                if(query.next()){
                    li_basedata=query.record().value(0).toInt();
                    ldt_sendtime=query.record().value(1).toDateTime();
                } else {
                    return tr("发卡失败：会员类型不存在!");
                }
            }

            if(ls_state!="1"){
                return tr("发卡失败：会员状态非<正常>，不能发卡!");
            }
            if(ls_cardflag=="Y"){
                return tr("该会员已经发卡，如需重发请对旧卡进行相应操作！~r~nIC卡可能其进行蕊片清除，磁卡可对其补卡!");
            }

            query.prepare("select vch_cardno from t_m_member  where vch_memberno =:as_memberno");
            query.bindValue(":as_memberno", as_memberno);
            query.exec();
            query.next();
            ls_cardno=query.record().value("vch_cardno").toString();
            if(ls_cardno.isEmpty()){
                return tr("vch_cardno is empty!");
            }


            if(is_cardtype=="1"){
                //ic卡，需要写密码，并且写卡号
                QString icpasswd=n_func::f_m_get_parameter("vip_ic_password");
                if(icpasswd.isEmpty()){
                    return tr("ic卡密码不能为空!");
                } else {
                    Reader_RF reader;
                    //修改密码成功
                    if(reader.pwdChange("FFFFFFFFFFFF", icpasswd=="FFFFFF"?"FFFFFFFFFFFF":icpasswd.toLocal8Bit().toHex())){//0
                        reader.setDefPwd(icpasswd=="FFFFFF"?"FFFFFFFFFFFF":icpasswd.toLocal8Bit().toHex());
                        //写数据
                        if(reader.writecard(ls_cardno, "", "")){//1

                        }else {//1
                            return tr("ic卡，写入卡号失败!")+reader.lastError();
                        }//1
                    }else {//0
                        return tr("ic卡，修改密码失败!")+reader.lastError();
                    }//0
                }
            } else if(is_cardtype=="2"){//磁卡
                lst_member.as_cardtype=is_cardtype;
            } else {
                return tr("发卡类型为 无会员卡，发卡失败!");
            }

            query.prepare("update t_m_member set ch_cardflag ='Y' , dt_operdate =now() , int_basedata =:li_basedata , dt_sendtime =:ldt_sendtime  Where vch_memberno =:as_memberno");
            query.bindValue(":li_basedata", li_basedata);
            query.bindValue(":ldt_sendtime", ldt_sendtime);
            query.bindValue(":as_memberno", as_memberno);
            if(query.exec()){
                /*success*/
                return "";
            } else {
                return query.lastError().text();
            }

        } else {//0
            return tr("发卡失败：会员[%1]记录不存在!").arg(as_memberno);
        }//0
    }
    return tr("发卡失败");
}
