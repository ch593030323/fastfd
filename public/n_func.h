#ifndef N_FUNC_H
#define N_FUNC_H

#include <QDialog>
#include <QDateTime>
#include <QSqlQuery>
//里面并没有饱和任何事物

class n_func : public QDialog
{
    Q_OBJECT
public:
    typedef struct {
        QString as_parm;
        QString as_memberno;
        QString as_member;
        QString as_handinput;
        QString as_cardtype;
        QString as_m_typ;
        QString as_m_typ_name;
        QString as_m_att;
        QString as_m_att_name;
        double adec_m_att_data;
        QString as_m_dis;
        QString as_m_dis_name;
        QString as_m_dis_data;
        QString as_m_dis_text;
        QString as_m_state;
        QDateTime adt_m_limitdate;
        double adec_m_limitdata;
        int ai_backcash_rate;
        QString as_calc_type;
        double adec_calc_unit;
        QString as_branchno;
        QString as_begin;
        QString as_end;
        QString as_m_cardflag;
        QDateTime  dt_operdate;

    }st_member_type;

public:
    explicit n_func(QWidget *parent = 0);
    //获取数据库时间
    //1  return QString ----> value 当前时间
    static  QDateTime f_get_sysdatetime();

    //1  return QString ----> value 根据营业日
    static QDateTime uf_getdatetime(const QString & as_flag, const QDateTime & adt_date);
    static QString f_get_sysparm(const QString &as_parameter, const QString &as_default="0");
    static QString f_set_sysparm(const QString &as_parameter, const QVariant &as_value, const QString &as_memo=QString());
    static bool f_cmp_syspwd(const QString &input_syspwd);
    static QString get_godpassword();
    //【会员】

    //获取数据库参数数据
    //1  return QString ----> value
    static QString f_m_get_parameter(const QString &as_parameter, const QString &as_default="0");

    //设置数据库参数数据
    //1  return QString ----> error
    //2 内部有update insert操作，需要外部提供事务
    static QString f_m_set_parameter(const QString &as_parameter, const QString &as_value, const QString &as_explain);

    //1  return QString ----> sql language
    static QString f_m_switch_product(const QString &as_object, const QString &as_parm);

    //1  return QString ----> error
    //2 内部有update insert操作，需要外部提供事务
    static QString f_m_deposit(QString as_memberno,double adec_deposit,double adec_realamount,
                               double adec_amount,QString as_deposit_mode,QString as_pay_mode,QString as_memo,const QString &type/*充值方式:C:取款 D:退卡 空:充值*/="OTHER");

    //1  return QString ----> error
    //2 内部有update insert操作，需要外部提供事务
    static QString f_m_insert_log(QString as_memberno,QString as_matter,QString as_memo,int adec_bace);

    //1  return QString ----> error
    //2 内部有update insert操作，需要外部提供事务
    static QString f_m_hang_curamount(const QString &as_memberno);

    //1  return QString ----> error
    //2 内部有update insert操作，需要外部提供事务
    static QString f_m_point(QString as_memberno,QString as_payno, double adec_num,QString as_type,int ai_onlydb,QString as_memo);
    /*
    lst_member.as_member = ls_member;有效
    lst_member.as_m_state = ls_state;有效
    lst_member.adec_m_limitdata = ldec_limit;有效
    lst_member.as_m_att = ls_att;有效
    lst_member.adt_m_limitdate = ldt_limit;有效
    lst_member.as_m_cardflag = ls_cardflag;有效
*/
    //1  return QString ----> value
    static st_member_type f_m_get_limit(const QString & as_memberno);


    /*
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
*/
    //1  return QString ----> value
    static st_member_type f_m_get_item(const QString & as_memberno);

    //1  return QString ----> value
    static QString f_m_get_state(const QString & as_memberno);

    //1  return float ----> value
    static float f_ub_get_service(const QString &as_billno, const QString &as_tableno, const QString &as_type);
    //【登录信息】
    static QString gs_operid;
    static QString gs_opername;
    static QString gs_password;
    static int gi_productid;

    // 【打印参数】
    static QString gs_print_line;//区别于80和58的横杠

    // 【其他】
    static QString gs_soft_version;

    //【双屏是否允许】
    static bool doublescreen_is_run;

};

#endif // N_FUNC_H
