#include "back_cashier_query.h"
#include "ui_back_cashier_query.h"
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QTimeEdit>
#include <QTime>
#include "lds_tableview.h"
#include <QDebug>
#include <QDesktopWidget>
#include <QApplication>
#include "n_func.h"

back_cashier_query::back_cashier_query(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::back_cashier_query)
{

    ui->setupUi(this);

    //this->setWindowFlags(Qt::FramelessWindowHint|Qt::Dialog);this->setStyleSheet("QDialog{border:2px solid gray}");
    ui->dateTimeEdit->setDateTime(QDateTime(n_func::f_get_sysdatetime().date(), QTime(0,0,0)));
    ui->dateTimeEdit_2->setDateTime(QDateTime(n_func::f_get_sysdatetime().date(), QTime(23,59,59)));

    init_control();
//    QDesktopWidget *d=QApplication::desktop();
//    this->setFixedSize(d->width(),d->height() - 40);
}

back_cashier_query::~back_cashier_query()
{
    delete ui;
}

void back_cashier_query::init_control()
{
    QSqlQueryModel model;
    model.setQuery("Select NOW()");
    QDateTime time =model.record(0).value(0).toDateTime();
    QString str = time.toString("yyyy-MM-dd");
    QString str1 = "00:00";
    QString str2 = str + " " + str1;
    QDateTime time_box1;
    time_box1 = QDateTime::fromString(str2, "yyyy-MM-dd hh:mm");
    ui->dateTimeEdit->setDateTime(time_box1);
    ui->dateTimeEdit_2->setDateTime(model.record(0).value(0).toDateTime());
    QString date1 = ui->dateTimeEdit->dateTime().toString("yyyy-MM-dd hh:mm");
    QString date2 = ui->dateTimeEdit_2->dateTime().toString("yyyy-MM-dd hh:mm");

    QTime timer;
    timer.setHMS(00,00,00);
    ui->timeEdit->setEnabled(false);
    ui->timeEdit->setTime(timer);

    ui->timeEdit_2->setEnabled(false);
    timer.setHMS(23,59,59);
    ui->timeEdit_2->setTime(timer);

    QStringList labels;
    labels<<tr("行号")<<tr("付款号")<<tr("单号")<<tr("餐桌")<<tr("收银代码")<<tr("收银方式")<<tr("收银金额")<<tr("找零")<<tr("实收金额")<<tr("面额")<<tr("凭证号")<<tr("备注")<<tr("收银员")<<tr("时间");
    model_top = new QStandardItemModel();
    model_top->setHorizontalHeaderLabels(labels);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);ui->tableView->setModel(model_top);

    model.clear();
    model.setQuery(QString("SELECT a.ch_payno,b.ch_billno,ifnull((select cey_bt_table.vch_tablename from cey_bt_table where b.ch_tableno =cey_bt_table.ch_tableno),'Q000'),a.ch_paymodeno,(select cey_bt_paymode.vch_paymodename from cey_bt_paymode where a.ch_paymodeno = cey_bt_paymode.ch_paymodeno),a.num_payamount,a.num_change,a.num_realamount,a.num_face,a.vch_voucherno,a.vch_memo,b.vch_operID,b.dt_operdate  FROM cey_u_checkout_detail a,cey_u_checkout_master b WHERE(a.ch_payno = b.ch_payno)and(b.ch_state = 'Y')and(left(b.dt_operdate,16) >= '%1') and(left(b.dt_operdate,16) <= '%2') and(left(RIGHT(b.dt_operdate,8),5) >= '0')and(left(RIGHT(b.dt_operdate,8),5) <= '3' )").arg(date1).arg(date2));
    for(int i = 0; i < model.rowCount(); i++)
    {
        int row = model_top->rowCount();
        model_top->setRowCount(row + 1);
        model_top->setItem(row,0,new QStandardItem(QString::number(row+1))); //序号
        model_top->setItem(row,1,new QStandardItem(model.record(i).value(0).toString())); //付款号
        model_top->setItem(row,2,new QStandardItem(model.record(i).value(1).toString())); //单号
        model_top->setItem(row,3,new QStandardItem(model.record(i).value(2).toString())); //餐桌
        model_top->setItem(row,4,new QStandardItem(model.record(i).value(3).toString())); //收银代码
        model_top->setItem(row,5,new QStandardItem(model.record(i).value(4).toString())); //收银方式
        model_top->setItem(row,6,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(5).toFloat()))); //收银金额
        model_top->setItem(row,7,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(6).toFloat()))); //找零
        model_top->setItem(row,8,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(7).toFloat()))); //实收金额
        model_top->setItem(row,9,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(8).toFloat()))); //面额
        QString pingzhenghao;
        if(model.record(i).value(9).toString() != "null")
        {
            pingzhenghao = model.record(i).value(9).toString();
        }
        model_top->setItem(row,10,new QStandardItem(pingzhenghao)); //凭证号
        QString beizhu;
        if(model.record(i).value(10).toString() != "null")
        {
            beizhu = model.record(i).value(10).toString();
        }
        model_top->setItem(row,11,new QStandardItem(beizhu)); //备注
        model_top->setItem(row,12,new QStandardItem(model.record(i).value(11).toString())); //收银员
        model_top->setItem(row,13,new QStandardItem(model.record(i).value(12).toDateTime().toString("yyyy-MM-dd hh:mm:ss"))); //时间
    }

    QFont font;
    font.setPointSize(12);
    if(model_top->rowCount() > 0)
    {
        ui->tableView->updateSum(6);
        ui->tableView->updateSum(7);
        ui->tableView->updateSum(8);
        ui->tableView->updateSum(9);
        ui->tableView->tableView->selectRow(0);
    }

}

void back_cashier_query::on_pushButton_12_clicked()
{
    this->close();
}

void back_cashier_query::on_checkBox_2_clicked()
{

    if(ui->checkBox_2->isChecked() == true)
    {
        ui->timeEdit->setEnabled(true);
        ui->timeEdit_2->setEnabled(true);
    }
    else
    {
        ui->timeEdit->setEnabled(false);
        ui->timeEdit_2->setEnabled(false);
    }
}

void back_cashier_query::on_pushButton_10_clicked()//检索
{
    QSqlQueryModel model;
    QString date1 = ui->dateTimeEdit->dateTime().toString("yyyy-MM-dd hh:mm");
    QString date2 = ui->dateTimeEdit_2->dateTime().toString("yyyy-MM-dd hh:mm");
    QString time1 = ui->timeEdit->text().left(5);
    QString time2 = ui->timeEdit_2->text().left(5);

    if(ui->checkBox->isChecked() == false && ui->checkBox_2->isChecked() == false)
    {
        model_top->removeRows(0,model_top->rowCount());
        model.setQuery(QString("SELECT a.ch_payno,b.ch_billno,ifnull((select cey_bt_table.vch_tablename from cey_bt_table where b.ch_tableno =cey_bt_table.ch_tableno),'Q000'),a.ch_paymodeno,(select cey_bt_paymode.vch_paymodename from cey_bt_paymode where a.ch_paymodeno = cey_bt_paymode.ch_paymodeno),a.num_payamount,a.num_change,a.num_realamount,a.num_face,a.vch_voucherno,a.vch_memo,b.vch_operID,b.dt_operdate  FROM cey_u_checkout_detail a,cey_u_checkout_master b WHERE(a.ch_payno = b.ch_payno)and(b.ch_state = 'Y')and(left(b.dt_operdate,16) >= '%1') and(left(b.dt_operdate,16) <= '%2') and(left(RIGHT(b.dt_operdate,8),5) >= '0')and(left(RIGHT(b.dt_operdate,8),5) <= '3' )").arg(date1).arg(date2));
        for(int i = 0; i < model.rowCount(); i++)
        {
            int row = model_top->rowCount();
            model_top->setRowCount(row + 1);
            model_top->setItem(row,0,new QStandardItem(QString::number(row+1))); //序号
            model_top->setItem(row,1,new QStandardItem(model.record(i).value(0).toString())); //付款号
            model_top->setItem(row,2,new QStandardItem(model.record(i).value(1).toString())); //单号
            model_top->setItem(row,3,new QStandardItem(model.record(i).value(2).toString())); //餐桌
            model_top->setItem(row,4,new QStandardItem(model.record(i).value(3).toString())); //收银代码
            model_top->setItem(row,5,new QStandardItem(model.record(i).value(4).toString())); //收银方式
            model_top->setItem(row,6,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(5).toFloat()))); //收银金额
            model_top->setItem(row,7,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(6).toFloat()))); //找零
            model_top->setItem(row,8,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(7).toFloat()))); //实收金额
            model_top->setItem(row,9,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(8).toFloat()))); //面额
            QString pingzhenghao;
            if(model.record(i).value(9).toString() != "null")
            {
                pingzhenghao = model.record(i).value(9).toString();
            }
            model_top->setItem(row,10,new QStandardItem(pingzhenghao)); //凭证号
            QString beizhu;
            if(model.record(i).value(10).toString() != "null")
            {
                beizhu = model.record(i).value(10).toString();
            }
            model_top->setItem(row,11,new QStandardItem(beizhu)); //备注
            model_top->setItem(row,12,new QStandardItem(model.record(i).value(11).toString())); //收银员
            model_top->setItem(row,13,new QStandardItem(model.record(i).value(12).toDateTime().toString("yyyy-MM-dd hh:mm:ss"))); //时间
        }
    }
    else if(ui->checkBox->isChecked() == true && ui->checkBox_2->isChecked() == false)//作废单据
    {
        model_top->removeRows(0,model_top->rowCount());
        model.setQuery(QString("SELECT a.ch_payno,b.ch_billno,ifnull((select cey_bt_table.vch_tablename from cey_bt_table where b.ch_tableno =cey_bt_table.ch_tableno),'Q000'),a.ch_paymodeno,(select cey_bt_paymode.vch_paymodename from cey_bt_paymode where a.ch_paymodeno = cey_bt_paymode.ch_paymodeno),a.num_payamount,a.num_change,a.num_realamount,a.num_face,a.vch_voucherno,a.vch_memo,b.vch_operID,b.dt_operdate  FROM cey_u_checkout_detail a,cey_u_checkout_master b WHERE(a.ch_payno = b.ch_payno)and(b.ch_state = 'P')and(left(b.dt_operdate,16) >= '%1') and(left(b.dt_operdate,16) <= '%2') and(left(RIGHT(b.dt_operdate,8),5) >= '0')and(left(RIGHT(b.dt_operdate,8),5) <= '3' )").arg(date1).arg(date2));
        for(int i = 0; i < model.rowCount(); i++)
        {
            int row = model_top->rowCount();
            model_top->setRowCount(row + 1);
            model_top->setItem(row,0,new QStandardItem(QString::number(row+1))); //序号
            model_top->setItem(row,1,new QStandardItem(model.record(i).value(0).toString())); //付款号
            model_top->setItem(row,2,new QStandardItem(model.record(i).value(1).toString())); //单号
            model_top->setItem(row,3,new QStandardItem(model.record(i).value(2).toString())); //餐桌
            model_top->setItem(row,4,new QStandardItem(model.record(i).value(3).toString())); //收银代码
            model_top->setItem(row,5,new QStandardItem(model.record(i).value(4).toString())); //收银方式
            model_top->setItem(row,6,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(5).toFloat()))); //收银金额
            model_top->setItem(row,7,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(6).toFloat()))); //找零
            model_top->setItem(row,8,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(7).toFloat()))); //实收金额
            model_top->setItem(row,9,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(8).toFloat()))); //面额
            QString pingzhenghao;
            if(model.record(i).value(9).toString() != "null")
            {
                pingzhenghao = model.record(i).value(9).toString();
            }
            model_top->setItem(row,10,new QStandardItem(pingzhenghao)); //凭证号
            QString beizhu;
            if(model.record(i).value(10).toString() != "null")
            {
                beizhu = model.record(i).value(10).toString();
            }
            model_top->setItem(row,11,new QStandardItem(beizhu)); //备注
            model_top->setItem(row,12,new QStandardItem(model.record(i).value(11).toString())); //收银员
            model_top->setItem(row,13,new QStandardItem(model.record(i).value(12).toDateTime().toString("yyyy-MM-dd hh:mm:ss"))); //时间
        }
    }
    else if(ui->checkBox->isChecked() == false && ui->checkBox_2->isChecked() == true)//按时间段查询
    {
        model_top->removeRows(0,model_top->rowCount());
        model.clear();
        model.setQuery(QString("SELECT a.ch_payno,b.ch_billno,ifnull((select cey_bt_table.vch_tablename from cey_bt_table where b.ch_tableno =cey_bt_table.ch_tableno),'Q000'),a.ch_paymodeno,(select cey_bt_paymode.vch_paymodename from cey_bt_paymode where a.ch_paymodeno = cey_bt_paymode.ch_paymodeno),a.num_payamount,a.num_change,a.num_realamount,a.num_face,a.vch_voucherno,a.vch_memo,b.vch_operID,b.dt_operdate  FROM cey_u_checkout_detail a,cey_u_checkout_master b WHERE(a.ch_payno = b.ch_payno)and(b.ch_state = 'Y')and(left(b.dt_operdate,16) >= '%1') and(left(b.dt_operdate,16) <= '%2') and(left(RIGHT(b.dt_operdate,8),5)) >= '%3' and(left(RIGHT(b.dt_operdate,8),5)) <= '%4'").arg(date1).arg(date2).arg(time1).arg(time2));
        for(int i = 0; i < model.rowCount(); i++)
        {
            int row = model_top->rowCount();
            model_top->setRowCount(row + 1);
            model_top->setItem(row,0,new QStandardItem(QString::number(row+1))); //序号
            model_top->setItem(row,1,new QStandardItem(model.record(i).value(0).toString())); //付款号
            model_top->setItem(row,2,new QStandardItem(model.record(i).value(1).toString())); //单号
            model_top->setItem(row,3,new QStandardItem(model.record(i).value(2).toString())); //餐桌
            model_top->setItem(row,4,new QStandardItem(model.record(i).value(3).toString())); //收银代码
            model_top->setItem(row,5,new QStandardItem(model.record(i).value(4).toString())); //收银方式
            model_top->setItem(row,6,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(5).toFloat()))); //收银金额
            model_top->setItem(row,7,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(6).toFloat()))); //找零
            model_top->setItem(row,8,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(7).toFloat()))); //实收金额
            model_top->setItem(row,9,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(8).toFloat()))); //面额
            QString pingzhenghao;
            if(model.record(i).value(9).toString() != "null")
            {
                pingzhenghao = model.record(i).value(9).toString();
            }
            model_top->setItem(row,10,new QStandardItem(pingzhenghao)); //凭证号
            QString beizhu;
            if(model.record(i).value(10).toString() != "null")
            {
                beizhu = model.record(i).value(10).toString();
            }
            model_top->setItem(row,11,new QStandardItem(beizhu)); //备注
            model_top->setItem(row,12,new QStandardItem(model.record(i).value(11).toString())); //收银员
            model_top->setItem(row,13,new QStandardItem(model.record(i).value(12).toDateTime().toString("yyyy-MM-dd hh:mm:ss"))); //时间
        }
    }
    else
    {
        model_top->removeRows(0,model_top->rowCount());
        model.clear();
        model.setQuery(QString("SELECT a.ch_payno,b.ch_billno,ifnull((select cey_bt_table.vch_tablename from cey_bt_table where b.ch_tableno =cey_bt_table.ch_tableno),'Q000'),a.ch_paymodeno,(select cey_bt_paymode.vch_paymodename from cey_bt_paymode where a.ch_paymodeno = cey_bt_paymode.ch_paymodeno),a.num_payamount,a.num_change,a.num_realamount,a.num_face,a.vch_voucherno,a.vch_memo,b.vch_operID,b.dt_operdate  FROM cey_u_checkout_detail a,cey_u_checkout_master b WHERE(a.ch_payno = b.ch_payno)and(b.ch_state = 'P')and(left(b.dt_operdate,16) >= '%1') and(left(b.dt_operdate,16) <= '%2') and(left(RIGHT(b.dt_operdate,8),5)) >= '%3' and(left(RIGHT(b.dt_operdate,8),5)) <= '%4'").arg(date1).arg(date2).arg(time1).arg(time2));
        for(int i = 0; i < model.rowCount(); i++)
        {
            int row = model_top->rowCount();
            model_top->setRowCount(row + 1);
            model_top->setItem(row,0,new QStandardItem(QString::number(row+1))); //序号
            model_top->setItem(row,1,new QStandardItem(model.record(i).value(0).toString())); //付款号
            model_top->setItem(row,2,new QStandardItem(model.record(i).value(1).toString())); //单号
            model_top->setItem(row,3,new QStandardItem(model.record(i).value(2).toString())); //餐桌
            model_top->setItem(row,4,new QStandardItem(model.record(i).value(3).toString())); //收银代码
            model_top->setItem(row,5,new QStandardItem(model.record(i).value(4).toString())); //收银方式
            model_top->setItem(row,6,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(5).toFloat()))); //收银金额
            model_top->setItem(row,7,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(6).toFloat()))); //找零
            model_top->setItem(row,8,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(7).toFloat()))); //实收金额
            model_top->setItem(row,9,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(8).toFloat()))); //面额
            QString pingzhenghao;
            if(model.record(i).value(9).toString() != "null")
            {
                pingzhenghao = model.record(i).value(9).toString();
            }
            model_top->setItem(row,10,new QStandardItem(pingzhenghao)); //凭证号
            QString beizhu;
            if(model.record(i).value(10).toString() != "null")
            {
                beizhu = model.record(i).value(10).toString();
            }
            model_top->setItem(row,11,new QStandardItem(beizhu)); //备注
            model_top->setItem(row,12,new QStandardItem(model.record(i).value(11).toString())); //收银员
            model_top->setItem(row,13,new QStandardItem(model.record(i).value(12).toDateTime().toString("yyyy-MM-dd hh:mm:ss"))); //时间
        }
    }

    QFont font;
    font.setPointSize(12);
    if(model_top->rowCount() > 0)
    {
        ui->tableView->updateSum(6);
        ui->tableView->updateSum(7);
        ui->tableView->updateSum(8);
        ui->tableView->updateSum(9);
        ui->tableView->tableView->selectRow(0);

    }
}

void back_cashier_query::on_pushButton_11_clicked()//导出
{

}
