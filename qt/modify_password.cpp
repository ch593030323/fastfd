#include "modify_password.h"
#include "ui_modify_password.h"
#include "n_func.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include "lds_messagebox.h"
#include <QSqlDatabase>
#include "fast_order_2.h"
#include"backheader.h"

modify_password::modify_password(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::modify_password)
{
    ui->setupUi(this);
    //this->setWindowFlags(Qt::FramelessWindowHint|Qt::Dialog);this->setStyleSheet("QDialog{border:2px solid gray}");
    ui->lineEdit->setEnabled(false);
    ui->lineEdit->setText(n_func::gs_operid);
    ui->lineEdit_2->setEchoMode(QLineEdit::Password);
    ui->lineEdit_3->setEchoMode(QLineEdit::Password);
    ui->lineEdit_4->setEchoMode(QLineEdit::Password);
}

modify_password::~modify_password()
{
    delete ui;
}

void modify_password::on_pushButton_5_clicked()
{
    this->close();
}

void modify_password::on_pushButton_4_clicked()
{
    ;
    QSqlQueryModel model;
    QSqlQuery query;
    QSqlDatabase::database().transaction();

    if(ui->lineEdit_2->text().trimmed().isEmpty() || ui->lineEdit_3->text().trimmed().isEmpty() || ui->lineEdit_4->text().trimmed().isEmpty())
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("内容填写不完整,请核实!"));
        
        return;
    }

    if(ui->lineEdit->text() == "0000")
    {
        if(!n_func::f_cmp_syspwd(ui->lineEdit_2->text().trimmed()))//ui->lineEdit_2->text().trimmed() != n_func::f_get_sysparm("system_pass", "0000"))
        {
            lds_messagebox::warning(this, tr("消息提示"),tr("旧密码错误!"));

            return;
        }

        if(ui->lineEdit_3->text().trimmed() != ui->lineEdit_4->text().trimmed())
        {
            lds_messagebox::warning(this, tr("消息提示"),tr("新密码与确认密码不一致,请核实!"));

            return;
        }

        if(!(n_func::f_set_sysparm("system_pass", ui->lineEdit_4->text().trimmed(), tr("系统管理员密码"))).isEmpty()){
            QSqlDatabase::database().rollback();
            lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));
            return;
        }
    }
    else
    {
        model.setQuery(QString("select vch_password from cey_sys_operator where vch_operID = '%1' ").arg(ui->lineEdit->text()));
        if(ui->lineEdit_2->text().trimmed() != model.record(0).value(0).toString().trimmed())
        {
            lds_messagebox::warning(this, tr("消息提示"),tr("旧密码错误!"));

            return;
        }

        if(ui->lineEdit_3->text().trimmed() != ui->lineEdit_4->text().trimmed())
        {
            lds_messagebox::warning(this, tr("消息提示"),tr("新密码与确认密码不一致,请核实!"));

            return;
        }

        query.prepare(QString("update cey_sys_operator set vch_password= :passowrd  where vch_operID =:oper "));
        query.bindValue(":passowrd",ui->lineEdit_4->text().trimmed());
        query.bindValue(":oper",ui->lineEdit->text().trimmed());
        if(query.exec())
        {

        }
        else
        {
            QSqlDatabase::database().rollback();
            lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_operator失败!"));

            return;
        }
    }
    QSqlDatabase::database().commit();

    lds_messagebox::warning(this, tr("消息提示"),tr("修改密码成功,请记住新密码!"));


    //    fast_order_2::model_dalei.setQuery(QString("SELECT  * FROM cey_bt_dish_type"));
    //    for(int i = 0; i < fast_order_2::model_dalei.rowCount(); i++)
    //    {
    //        if(i == fast_order_2::num_da)
    //        {
    //            fast_order_2::dalei = fast_order_2::model_dalei.record(i).value(0).toString();
    //        }
    //    }

    //    fast_order_2::model_xiaolei.setQuery(QString("SELECT  * FROM cey_bt_dish_subtype where ch_dish_typeno = '%1' ").arg(fast_order_2::dalei));
    //    for(int i = 0; i < fast_order_2::model_xiaolei.rowCount(); i++)
    //    {
    //        if(i == fast_order_2::num_xiao)
    //        {
    //            fast_order_2::xiaolei = fast_order_2::model_xiaolei.record(0).value(0).toString();
    //        }
    //    }
    //    if(fast_order_2::changyongcook == false)
    //    {
    //        fast_order_2::model_cook.setQuery(QString("SELECT  * FROM cey_bt_dish where ch_dish_typeno = '%1' and ch_sub_typeno ='%2' and (ch_stopflag = null or ch_stopflag = 'N') ").arg(fast_order_2::dalei).arg(fast_order_2::xiaolei));
    //    }
    //    else
    //    {
    //        fast_order_2::model_cook.setQuery(QString("select * from cey_bt_dish where ch_off = 'Y' and (ch_stopflag = null or ch_stopflag = 'N') "));
    //    }

    this->close();
}
