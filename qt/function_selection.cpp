#include "function_selection.h"
#include "ui_function_selection.h"
#include "estimated_clear.h"
#include "special_query.h"
#include "system_setting.h"
#include "n_func.h"
#include <QSqlQueryModel>
#include "lds_messagebox.h"
#include <QSqlRecord>
#include "modify_password.h"
#include "lock_screen.h"
#include"business_documents_query.h"
#include "consumption_veiw.h"
#include "shift_work.h"
#include "back/w_m_member_list.h"
#include "n_func.h"
#include "public_mainprinter.h"
#include <QSettings>
#include "kitchen_leakage.h"
#include "power_oper.h"
#include "backheader.h"
#include "back_view_dialog1.h"
#include "w_scr_dish_switch.h"

function_selection::function_selection(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::function_selection)
{
    ui->setupUi(this);
    //this->setWindowFlags(Qt::FramelessWindowHint|Qt::Dialog);this->setStyleSheet("QDialog{border:2px solid gray}");
    btn_t_f = false;
    fast_model.clear();
    sys_model.clear();
}

function_selection::~function_selection()
{
    delete ui;
}

void function_selection::static_toswitch()
{
    //是否需要有交班人
    QSqlQuery query;
    QString errtext;

    w_scr_dish_switch dialog;
    if(n_func::f_get_sysparm("handover_oper")=="1"){
        switch(0){
        case 0:
            if(QDialog::Rejected==backheader::static_dialogexec(dialog, tr("输入接班人"))){
                return;
            }
        case 1:
            if(dialog.ui->lineEdit->text().isEmpty()){
                errtext=tr("操作员代码不能为空!");
                break;
            }
            if(dialog.ui->lineEdit->text()==n_func::gs_operid){
                errtext=tr("接班人和交班人不能为同一人!");
                break;
            }
        case 2:
            if(dialog.ui->lineEdit->text()=="0000"){
                if(dialog.ui->lineEdit_2->text()!=n_func::f_m_get_parameter("system", "0000")){
                    errtext=tr("操作密码错误!");
                }
                break;
            }
        case 3:
            query.exec(QString("Select vch_operID,vch_password from cey_sys_operator where ch_flag = 'Y' and vch_operID ='%1' ")
                       .arg(dialog.ui->lineEdit->text()));
            if(!query.next()){
                errtext=tr("操作员代码不存在!");
                break;
            }

            query.exec(QString("Select vch_operID,vch_password from cey_sys_operator where ch_flag = 'Y' and vch_operID ='%1' and vch_password='%2' ")
                       .arg(dialog.ui->lineEdit->text())
                       .arg(dialog.ui->lineEdit_2->text())
                       );
            if(!query.next()){
                errtext=tr("操作密码错误!");
                break;
            }
        }
    }
    if(!errtext.isEmpty()){
        lds_messagebox::warning(0, tr("消息提示"),errtext);
        return;
    }

    //开始交班
    shift_work work(dialog.ui->lineEdit->text());
    backheader::static_dialogexec(work, tr("交班对账表"));
    if(!work.btn_t_f) return;
    //退出重新登录
    public_sql::save_login->hideall_and_back_login(dialog.ui->lineEdit->text(), dialog.ui->lineEdit_2->text());

}

void function_selection::on_pushButton_12_clicked()//退出
{
    this->close();
}

void function_selection::to_discount()//菜品估清查询
{
    estimated_clear clear;
    clear.resize(backheader::mainwindowsize);
    /*clear.exec();*/backheader::static_dialogexec(clear, tr("菜品估清查询"));

    this->close();
}

void function_selection::to_present()//特价菜品查询
{
    //需权限
    if(n_func::gs_operid != "0000")
    {
        QSqlQueryModel model;
        model.setQuery(QString("Select count(*) from cey_sys_oper_purview where vch_operID ='%1' and ch_purviewno ='0205' ").arg(n_func::gs_operid));
        if(model.record(0).value(0).toInt() == 0)
        {
            ;
            lds_messagebox::warning(this, tr("消息提示"),tr("该操作员无此操作权限!"));
            
            return;
        }
    }

    special_query query(this);
    query.resize(backheader::mainwindowsize);
    /*query->exec();*/backheader::static_dialogexec(query, tr("菜品特价查询"));
    this->close();
}

void function_selection::to_takebill()//营业单据查询
{
    if(n_func::gs_operid != "0000")
    {
        QSqlQueryModel model;
        ;
        model.setQuery(QString("select count(*)  from cey_sys_oper_purview  where ch_purviewno = '0201' and vch_operID ='%1'").arg(n_func::gs_operid));
        if(model.record(0).value(0).toInt() == 0)
        {
            lds_messagebox::warning(this, tr("消息提示"),tr("该操作员无此操作权限!"));
            
            return;
        }
    }

    business_documents_query query(this);
    query.resize(backheader::mainwindowsize);
    /*query->exec();*/backheader::static_dialogexec(query, tr("营业单据查询"));
}

void function_selection::to_change()//营业情况统计
{
    if(n_func::gs_operid != "0000")
    {
        QSqlQueryModel model;
        ;
        model.setQuery(QString("select count(*)  from cey_sys_oper_purview  where ch_purviewno = '0224' and vch_operID ='%1'").arg(n_func::gs_operid));
        if(model.record(0).value(0).toInt() == 0)
        {
            lds_messagebox::warning(this, tr("消息提示"),tr("该操作员无此操作权限!"));
            
            return;
        }
    }

    consumption_veiw consumption(this);
    consumption.resize(backheader::mainwindowsize);
    /*consumption->exec();*/backheader::static_dialogexec(consumption, tr("消费情况统计表"));
}

void function_selection::to_exitbill()//交班操作
{
    function_selection::static_toswitch();
}

void function_selection::to_allbillcancel()//开钱箱
{
    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    QString instructionedit = conf.value("system_setting/qianxiangzhiling").toString();
    blockIns bins;
    bins.append(pieceIns(instructionedit.toLocal8Bit(),""));
    backheader::asyn_dataprint(bins);


    //    ;
    //    if(n_func::gs_operid != "0000")
    //    {
    //        QSqlQueryModel model;
    //        model.setQuery(QString("select count(*)  from cey_sys_oper_purview  where ch_purviewno = '0209' and vch_operID ='%1'").arg(n_func::gs_operid));
    //        if(model.record(0).value(0).toInt() == 0)
    //        {
    //            lds_messagebox::warning(this, tr("消息提示"),tr("该操作员无此操作权限!"));
    //
    //            return;
    //        }
    //    }


    //    QSettings *configIniRead = new QSettings(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    //    QStringList instructionedit = configIniRead->value("system_setting/qianxiangzhiling").toStringList();
    //    QString instr;
    //    for(int i = 0; i < instructionedit.count(); i++)
    //    {
    //        if(i != instructionedit.count() -1)
    //        {
    //            instr.append(instructionedit[i]);
    //            instr.append(",");
    //        }
    //        else
    //        {
    //            instr.append(instructionedit[i]);
    //        }
    //    }

    //        blockIns b;
    //        QString path;
    //        QString duankoutype = configIniRead->value("system_setting/dyjjiekouleixing").toString();
    //        if(duankoutype == tr("串口打印") || duankoutype == tr("USB打印"))
    //        {
    //            path = configIniRead->value("system_setting/dayinjiduankou").toString();
    //        }
    //        else
    //        {
    //            path = configIniRead->value("system_setting/dayinjiip").toString() + ":9100";
    //        }
    //        b+=backheader::printer->drawText(instr.toLocal8Bit(),"");
    //        b=backheader::printer->setPrintCount(b, 1);
    //        backheader::asyn_dataprint(b);
}

void function_selection::on_pushButton_8_clicked()//系统参数设置
{

    system_setting setting;
    backheader::static_dialogexec(setting, tr("系统设置"));
    if(setting.btn_t_f == true)
    {
        QSettings configIniRead(public_sql::settings_ini_dir_file, QSettings::IniFormat);
        sys_model = configIniRead.value("system_setting/yingyemoshi").toString();
    }
    this->close();
}

void function_selection::to_num()//会员充值
{
    w_m_member_list::statictorecharge(this, 0, 0,"");
}

void function_selection::to_member_qukuan()//会员取款
{
    w_m_member_list::statictorecharge(this, 0, 0,"C");
}

void function_selection::on_pushButton_5_clicked()//常用菜
{
    btn_t_f = true;
    this->close();
}

void function_selection::to_sys_xiugaimima()//密码修改
{
    modify_password password;
    /*password->exec();*/backheader::static_dialogexec(password, tr("密码修改"));
}

void function_selection::to_suopint()//锁屏
{
    lock_screen lock("1");
    /*lock->exec();*/backheader::static_dialogexec(lock, tr("前台锁屏"));
}

void function_selection::on_pushButton_11_clicked()//酒楼模式
{

    //    fast_model = "talbe_model";
    //    this->close();
}

void function_selection::to_sys_chufangdaying()//厨房打印查询
{
    kitchen_leakage leakage;
    leakage.resize(backheader::mainwindowsize);
    /*leakage->exec();*/backheader::static_dialogexec(leakage, tr("厨房打印漏打单"));
}
