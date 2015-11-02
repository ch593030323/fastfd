#include "lock_screen.h"
#include "ui_lock_screen.h"
#include <QSqlQueryModel>
#include <QSqlRecord>
#include "lds_messagebox.h"
#include "n_func.h"
#include <QSettings>
#include <QDebug>
#include "public_sql.h"
#include"backheader.h"

lock_screen::lock_screen(const QString &str,QWidget *parent) :
    type(str),QDialog(parent),
    ui(new Ui::lock_screen)
{
    ui->setupUi(this);
    ui->lineEdit_2->setEchoMode(QLineEdit::Password);
    ui->lineEdit->setFocus();
}

lock_screen::~lock_screen()
{
    delete ui;
}

void lock_screen::on_pushButton_4_clicked()
{
    QSettings *configIniRead = new QSettings(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    QSqlQueryModel model;
    ;

    if(ui->lineEdit->text().trimmed().isEmpty() || ui->lineEdit_2->text().trimmed().isEmpty())
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("请输入完整的用户名和密码!"));
        
        return;
    }

    QString str = configIniRead->value("system_setting/jiesuoyuangong").toString();
    if(str == "1" && type == "1")
    {
        if(ui->lineEdit->text().trimmed() != n_func::gs_operid)
        {
            lds_messagebox::warning(this, tr("消息提示"),tr("锁屏解锁必须是系统登录员工,请核实!"));
            
            return;
        }
    }

    if(ui->lineEdit->text().trimmed() == "0000")
    {
        if(!n_func::f_cmp_syspwd(ui->lineEdit_2->text().trimmed()))//ui->lineEdit_2->text().trimmed() != n_func::f_get_sysparm("system_pass", "0000"))
        {
            lds_messagebox::warning(this, tr("消息提示"),tr("密码输入错误,请重新输入!"));
            
            return;
        }
    }
    else
    {
        model.setQuery(QString("select count(*)  from cey_sys_operator  where vch_operID = '%1' and ch_flag ='%2' ").arg(ui->lineEdit->text().trimmed()).arg(tr("Y")));
        if(model.record(0).value(0).toInt() == 0)
        {
            lds_messagebox::warning(this, tr("消息提示"),tr("用户名代码不存在，请核实!"));
            
            return;
        }

        model.clear();
        model.setQuery(QString("select vch_password  from cey_sys_operator  where vch_operID = '%1' and ch_flag ='%2' ").arg(ui->lineEdit->text().trimmed()).arg(tr("Y")));
        if(model.record(0).value(0).toString().trimmed() != ui->lineEdit_2->text().trimmed())
        {
            lds_messagebox::warning(this, tr("消息提示"),tr("密码输入错误,请重新输入!"));
            
            return;
        }
    }

    this->reject();
}

void lock_screen::on_pushButton_clicked()//1
{
    if(ui->lineEdit->hasFocus())
    {
        ui->lineEdit->insert("1");
    }
    if(ui->lineEdit_2->hasFocus())
    {
        ui->lineEdit_2->insert("1");
    }
}

void lock_screen::on_pushButton_2_clicked()//2
{
    if(ui->lineEdit->hasFocus())
    {
        ui->lineEdit->insert("2");
    }
    if(ui->lineEdit_2->hasFocus())
    {
        ui->lineEdit_2->insert("2");
    }
}

void lock_screen::on_pushButton_3_clicked()//3
{
    if(ui->lineEdit->hasFocus())
    {
        ui->lineEdit->insert("3");
    }
    if(ui->lineEdit_2->hasFocus())
    {
        ui->lineEdit_2->insert("3");
    }
}

void lock_screen::on_pushButton_5_clicked()//4
{
    if(ui->lineEdit->hasFocus())
    {
        ui->lineEdit->insert("4");
    }
    if(ui->lineEdit_2->hasFocus())
    {
        ui->lineEdit_2->insert("4");
    }
}

void lock_screen::on_pushButton_8_clicked()//5
{
    if(ui->lineEdit->hasFocus())
    {
        ui->lineEdit->insert("5");
    }
    if(ui->lineEdit_2->hasFocus())
    {
        ui->lineEdit_2->insert("5");
    }
}

void lock_screen::on_pushButton_7_clicked()//6
{
    if(ui->lineEdit->hasFocus())
    {
        ui->lineEdit->insert("6");
    }
    if(ui->lineEdit_2->hasFocus())
    {
        ui->lineEdit_2->insert("6");
    }
}

void lock_screen::on_pushButton_6_clicked()//7
{
    if(ui->lineEdit->hasFocus())
    {
        ui->lineEdit->insert("7");
    }
    if(ui->lineEdit_2->hasFocus())
    {
        ui->lineEdit_2->insert("7");
    }
}

void lock_screen::on_pushButton_13_clicked()//8
{
    if(ui->lineEdit->hasFocus())
    {
        ui->lineEdit->insert("8");
    }
    if(ui->lineEdit_2->hasFocus())
    {
        ui->lineEdit_2->insert("8");
    }
}

void lock_screen::on_pushButton_12_clicked()//9
{
    if(ui->lineEdit->hasFocus())
    {
        ui->lineEdit->insert("9");
    }
    if(ui->lineEdit_2->hasFocus())
    {
        ui->lineEdit_2->insert("9");
    }
}

void lock_screen::on_pushButton_10_clicked()//0
{
    if(ui->lineEdit->hasFocus())
    {
        ui->lineEdit->insert("0");
    }
    if(ui->lineEdit_2->hasFocus())
    {
        ui->lineEdit_2->insert("0");
    }
}

void lock_screen::on_pushButton_9_clicked()//.
{
    if(ui->lineEdit->hasFocus())
    {
        ui->lineEdit->insert(tr("."));
    }
    if(ui->lineEdit_2->hasFocus())
    {
        ui->lineEdit_2->insert(tr("."));
    }
}

void lock_screen::on_pushButton_11_clicked()//d
{
    if(ui->lineEdit->hasFocus())
    {
        ui->lineEdit->backspace();
    }
    if(ui->lineEdit_2->hasFocus())
    {
        ui->lineEdit_2->backspace();
    }
}

void lock_screen::closeEvent(QCloseEvent *e)
{
    e->ignore();
}
