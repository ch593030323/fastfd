#include "back_business_day.h"
#include "ui_back_business_day.h"
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QDebug>
#include "lds_messagebox.h"
#include <QSqlQuery>
#include "n_func.h"
#include <QSqlError>

back_business_day::back_business_day(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::back_business_day)
{
    //this->setWindowFlags(Qt::FramelessWindowHint|Qt::Dialog);this->setStyleSheet("QDialog{border:2px solid gray}");
    ui->setupUi(this);
    init_control();
}

back_business_day::~back_business_day()
{
    delete ui;
}

void back_business_day::init_control()
{
    QSqlQueryModel model;
    model.setQuery(QString("Select  dt_begin , dt_end,dt_date  from cey_u_day_sumup where dt_date =( Select max( dt_date)from cey_u_day_sumup )"));

    ui->dateTimeEdit->setEnabled(false);

    QDateTime time =model.record(0).value(1).toDateTime();
    QString str = time.toString("yyyy-MM-dd hh:mm:ss");
    QDateTime time_box1;
    time_box1 = QDateTime::fromString(str, "yyyy-MM-dd hh:mm:ss");
    ui->dateTimeEdit->setDateTime(time_box1);

    model.clear();
    model.setQuery(QString("select NOW()"));
    QDateTime time1 =model.record(0).value(0).toDateTime();
    QString str1 = time1.toString("yyyy-MM-dd hh:mm:ss");
    QDateTime time_box2;
    time_box2 = QDateTime::fromString(str1, "yyyy-MM-dd hh:mm:ss");
    ui->dateTimeEdit_2->setDateTime(time_box2);

//    QString day = ui->dateTimeEdit->dateTime().addDays(1).toString("yyyy-MM-dd");//日结日期
//    int yy = day.left(4).toInt();
//    int mm = day.mid(5,2).toInt();
//    int dd = day.right(2).toInt();

    ui->dateEdit->setDisplayFormat("yyyy-MM-dd");
    ui->dateEdit->setDate(n_func::f_get_sysdatetime().date());
}

void back_business_day::on_pushButton_12_clicked()//退出
{
    this->close();
}

void back_business_day::on_pushButton_13_clicked()//日结
{
    QSqlQueryModel model;
    model.setQuery(QString("select NOW()"));
    QDateTime sys_time = model.record(0).value(0).toDateTime();
    ;
    QSqlQuery query;
    QSqlDatabase::database().transaction();

    if(ui->dateTimeEdit_2->dateTime() <= ui->dateTimeEdit->dateTime())
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("本次日结时间不能小于或等于上次日结时间!"));

        return;
    }

    if(ui->dateTimeEdit_2->dateTime() > sys_time)
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("本次日结时间不能大于当期系统时间!"));

        return;
    }

    if(ui->dateTimeEdit->date() > ui->dateEdit->date() || ui->dateTimeEdit_2->date() < ui->dateEdit->date())
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("营业日必须是上次与本次日结时间之间的某一日期!"));

        return;
    }

    if(ui->dateEdit->isEnabled() == true)
    {
        model.clear();
        model.setQuery(QString("Select count( 1) from cey_u_day_sumup  where dt_date = '%1' ").arg(ui->dateEdit->text()));
        if(model.record(0).value(0).toInt() > 0)
        {
            lds_messagebox::warning(this, tr("消息提示"),tr("指定营业日已经日结,试一试最后日结可否修改!"));

            return;
        }

        query.prepare(QString("Insert into cey_u_day_sumup(dt_date , dt_begin , dt_end , vch_operID , dt_operdate)values(:yingyeri , :shangci , :benci , :oper , :date )"));
        query.bindValue(":yingyeri",ui->dateEdit->text());
        query.bindValue(":shangci",ui->dateTimeEdit->text());
        query.bindValue(":benci",ui->dateTimeEdit_2->text());
        query.bindValue(":oper",n_func::gs_operid);
        query.bindValue(":date",sys_time.toString("yyyy-MM-dd hh:mm:ss"));
        if(query.exec())
        {

        }
        else
        {
            QSqlDatabase::database().rollback();
           qDebug() << query.lastError();  lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_u_day_sumup数据库失败!日结失败!"));

            return;
        }
    }
    else
    {
        model.clear();
        model.setQuery(QString("Select count( 1) from cey_u_day_sumup  where dt_date > '%1'").arg(ui->dateEdit->text()));

        if(model.record(0).value(0).toInt() > 0)
        {
            lds_messagebox::warning(this, tr("消息提示"),tr("指定营业日非最后营业日,请核实!"));

            return;
        }

        model.clear();
        model.setQuery(QString("Select count( 1) from cey_u_day_sumup  where dt_date = '%1'").arg(ui->dateEdit->text()));

        if(model.record(0).value(0).toInt() == 0)
        {
            lds_messagebox::warning(this, tr("消息提示"),tr("指定最后营业日系统不存在,请核实!"));

            return;
        }

        query.prepare(QString("update cey_u_day_sumup set dt_end = :benci , vch_operID = :oper , dt_operdate = :sys_time where dt_date = :yingyeri "));
        query.bindValue(":benci",ui->dateTimeEdit_2->text());
        query.bindValue(":oper",n_func::gs_operid);
        query.bindValue(":sys_time",sys_time.toString("yyyy-MM-dd hh:mm:ss"));
        query.bindValue(":yingyeri",ui->dateEdit->text());
        if(query.exec())
        {

        }
        else
        {
            QSqlDatabase::database().rollback();
            lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_u_day_sumup数据库失败!日结失败!"));

            return;
        }
    }

    model.clear();
    model.setQuery(QString("select count( 1)from cey_u_table  where IFNULL( ch_payno , '')='' and ch_state ='1' and Left(ch_billno , 1)='B' and dt_operdate >= '%1'  and dt_operdate < '%2' ").arg(ui->dateTimeEdit->text()).arg(ui->dateTimeEdit_2->text()));
    if(model.record(0).value(0).toInt() > 0)
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("日结失败!"));

        return;
    }
    else
    {
        QSqlDatabase::database().commit();
        lds_messagebox::warning(this, tr("消息提示"),tr("日结成功!"));
        return;
    }
}

void back_business_day::on_pushButton_11_clicked()//反日结
{
    QSqlQueryModel model;
    ;
    QSqlQuery query;
    QSqlDatabase::database().transaction();

    if(ui->dateEdit->isEnabled() == true)
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("当前状态为本次日结状态,请选择最后日结!"));

        return;
    }
    else
    {
       model.clear();
       model.setQuery(QString("Select count( 1) from cey_u_day_sumup  where dt_date > '%1'").arg(ui->dateEdit->text()));
       if(model.record(0).value(0).toInt() > 0)
       {
           lds_messagebox::warning(this, tr("消息提示"),tr("指定营业日非最后营业日,请核实!"));

           return;
       }

       model.clear();
       model.setQuery(QString("Select count( *) from cey_u_day_sumup  where dt_date = '%1'").arg(ui->dateEdit->text()));
       if(model.record(0).value(0).toInt() == 0)
       {
           lds_messagebox::warning(this, tr("消息提示"),tr("指定最后营业日系统不存在,请核实!"));

           return;
       }

       query.prepare(QString("Delete from cey_u_day_sumup  where dt_date = :date "));
       query.bindValue(":date",ui->dateEdit->text());
       if(query.exec())
       {
           QSqlDatabase::database().commit();
           lds_messagebox::warning(this, tr("消息提示"),tr("反日结成功!"));

           return;
       }
       else
       {
           QSqlDatabase::database().rollback();
           lds_messagebox::warning(this, tr("消息提示"),tr("反日结失败!"));

           return;
       }
    }
}

void back_business_day::on_pushButton_10_clicked()//最后日结/本次日结
{
    QSqlQueryModel model;
    if(ui->pushButton_10->text() == tr("最后日结"))
    {
        ui->pushButton_10->setText(tr("本次日结"));
        ui->dateEdit->setEnabled(false);
        model.clear();
        model.setQuery(QString("Select  dt_begin , dt_end,dt_date  from cey_u_day_sumup where dt_date =( Select max( dt_date)from cey_u_day_sumup )"));
        ui->dateTimeEdit->setDateTime(model.record(0).value(0).toDateTime());
        ui->dateTimeEdit_2->setDateTime(model.record(0).value(1).toDateTime());
        ui->dateEdit->setDate(model.record(0).value(2).toDate());
    }
    else
    {
        model.clear();
        ui->pushButton_10->setText(tr("最后日结"));
        ui->dateEdit->setEnabled(true);
        model.setQuery(QString("Select  dt_begin , dt_end,dt_date  from cey_u_day_sumup where dt_date =( Select max( dt_date)from cey_u_day_sumup )"));

        ui->dateTimeEdit->setEnabled(false);

        QDateTime time =model.record(0).value(1).toDateTime();
        QString str = time.toString("yyyy-MM-dd hh:mm:ss");
        QDateTime time_box1;
        time_box1 = QDateTime::fromString(str, "yyyy-MM-dd hh:mm:ss");
        ui->dateTimeEdit->setDateTime(time_box1);

        model.clear();
        model.setQuery(QString("select NOW()"));
        QDateTime time1 =model.record(0).value(0).toDateTime();
        QString str1 = time1.toString("yyyy-MM-dd hh:mm:ss");
        QDateTime time_box2;
        time_box2 = QDateTime::fromString(str1, "yyyy-MM-dd hh:mm:ss");
        ui->dateTimeEdit_2->setDateTime(time_box2);

        QString day = ui->dateTimeEdit->dateTime().addDays(1).toString("yyyy-MM-dd");//日结日期
        int yy = day.left(4).toInt();
        int mm = day.mid(5,2).toInt();
        int dd = day.right(2).toInt();
        QDate date;
        date.setDate(yy,mm,dd);
        ui->dateEdit->setDate(date);
    }
}

void back_business_day::on_commandLinkButton_clicked()
{

}
