#include "manager_view.h"
#include "table_lock.h"
#include "estimated_clear.h"
#include "special_query.h"
#include "n_func.h"
#include <QSqlQueryModel>
#include <QSqlRecord>
#include "lds_messagebox.h"
#include "consumption_veiw.h"
#include "back/w_m_member_list.h"
#include "system_setting.h"
#include "modify_password.h"
#include "lock_screen.h"
#include "fast_order_2.h"
#include <QSettings>
#include <table_statistics.h>
#include "backheader.h"

manager_view::manager_view(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::manager_view)
{
    ui->setupUi(this);
    //this->setWindowFlags(Qt::FramelessWindowHint|Qt::Dialog);this->setStyleSheet("QDialog{border:2px solid gray}");
    fast_model.clear();
}

manager_view::~manager_view()
{
    delete ui;
}

void manager_view::on_pushButton_clicked()//锁台管理
{
    //this->close();
    table_lock lock(this);
    /*lock->exec();*/backheader::static_dialogexec(lock, tr("桌位加(解)锁"));
}

void manager_view::on_pushButton_2_clicked()//当前台况统计
{
    table_statistics statistics(this);
    backheader::static_dialogexec(statistics, tr("餐桌类型统计"));
}

void manager_view::on_pushButton_3_clicked()//菜品估清查询
{
    estimated_clear clear(this);
    backheader::static_dialogexec(clear, tr("菜品估清查询"));
}

void manager_view::on_pushButton_4_clicked()//菜品特价查询
{
    //this->close();
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
    //this->close();
}

void manager_view::on_pushButton_6_clicked()//营业情况统计
{
    //this->close();
    if(n_func::gs_operid != "0000")
    {
        QSqlQueryModel model;
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

void manager_view::on_pushButton_8_clicked()//会员充值
{
    //this->close();
    w_m_member_list::statictorecharge(this, 0, 0,"");
}

void manager_view::on_pushButton_7_clicked()//系统参数设置
{
    system_setting *setting = new system_setting(this);
    /*setting->exec();*/backheader::static_dialogexec(*setting, "");
    if(setting->btn_t_f == true)
    {
        QSettings *configIniRead = new QSettings(public_sql::settings_ini_dir_file, QSettings::IniFormat);
        fast_model = configIniRead->value("system_setting/yingyemoshi").toString();
    }
    //this->close();
}

void manager_view::on_pushButton_5_clicked()//密码修改
{
    modify_password password(this);
    /*password->exec();*/backheader::static_dialogexec(password, tr("密码修改"));
}

void manager_view::on_pushButton_13_clicked()//锁屏
{
    //this->close();
    lock_screen lock("1", this);
    /*lock->exec();*/backheader::static_dialogexec(lock, tr("前台锁屏"));
}
