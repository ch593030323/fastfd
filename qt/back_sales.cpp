#include "back_sales.h"
#include "ui_back_sales.h"
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QTimeEdit>
#include <QTime>
#include "lds_tableview.h"
#include <QDebug>
#include "n_func.h"

back_sales::back_sales(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::back_sales)
{
    //this->setWindowFlags(Qt::FramelessWindowHint|Qt::Dialog);this->setStyleSheet("QDialog{border:2px solid gray}");
    ui->setupUi(this);
    init_control();

    ui->dateTimeEdit->setDateTime(QDateTime(n_func::f_get_sysdatetime().date(), QTime(0,0,0)));
    ui->dateTimeEdit_2->setDateTime(QDateTime(n_func::f_get_sysdatetime().date(), QTime(23,59,59)));
    ui->timeEdit->setDisplayFormat("hh:mm");
    ui->timeEdit_2->setDisplayFormat("hh:mm");
}

back_sales::~back_sales()
{
    delete ui;
}

void back_sales::init_control()
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
    labels<<tr("行号")<<tr("单号")<<tr("付款号")<<tr("餐桌")<<tr("品码")<<tr("品名")<<tr("价格")<<tr("数量")<<tr("税收")<<tr("折扣")<<tr("赠送")<<tr("金额")<<tr("点菜量")<<tr("退量")<<tr("退菜人")<<tr("退菜时间")<<tr("折扣人")<<tr("折扣时间")<<tr("厨打做法")<<tr("套菜")<<tr("特价")<<tr("赠送人")<<tr("赠送时间")<<tr("点菜人")<<tr("时间");
    model_top = new QStandardItemModel();
    model_top->setHorizontalHeaderLabels(labels);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);ui->tableView->setModel(model_top);
    on_pushButton_10_clicked();//检索
}

void back_sales::on_pushButton_12_clicked()
{
    this->close();
}

void back_sales::on_checkBox_2_clicked()
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

void back_sales::on_pushButton_10_clicked()//检索
{
    QSqlQueryModel model;
    QString date1 = ui->dateTimeEdit->dateTime().toString("yyyy-MM-dd hh:mm");
    QString date2 = ui->dateTimeEdit_2->dateTime().toString("yyyy-MM-dd hh:mm");
    QString time1 = ui->timeEdit->time().toString("hh:mm");
    QString time2 = ui->timeEdit_2->time().toString("hh:mm");

    if(ui->checkBox->isChecked() == false && ui->checkBox_2->isChecked() == false)
    {
        model_top->removeRows(0,model_top->rowCount());
        model.setQuery(QString("SELECT a.int_flowID,a.ch_billno,a.ch_payno,a.ch_tableno,a.ch_dishno,c.vch_dishname,a.num_price + a.num_price_add as price,a.num_num,a.num_back,a.vch_back_operID,a.dt_back_operdate,a.int_discount,a.vch_dis_operID,a.dt_dis_operdate, a.vch_print_memo,a.ch_suitflag,a.ch_specialflag,a.ch_presentflag,a.vch_pre_operID,a.dt_pre_operdate,a.vch_operID,a.dt_operdate,(a.num_num * c.num_price*int_rate*0.01)  FROM cey_u_orderdish a ,cey_u_checkout_master b,cey_bt_dish c Where (a.ch_dishno = c.ch_dishno)and(a.ch_payno = b.ch_payno)and (b.ch_state = 'Y')  and (left(b.dt_operdate,16) > '%1') and (left(b.dt_operdate,16) <= '%2')").arg(date1).arg(date2));
        for(int i = 0; i < model.rowCount(); i++)
        {
            int row = model_top->rowCount();
            model_top->setRowCount(row + 1);
            model_top->setItem(row,0,new QStandardItem(QString::number(row+1))); //序号
            model_top->setItem(row,1,new QStandardItem(model.record(i).value(1).toString()));//单号
            model_top->setItem(row,2,new QStandardItem(model.record(i).value(2).toString()));//付款号
            model_top->setItem(row,3,new QStandardItem(model.record(i).value(3).toString()));//餐桌
            model_top->setItem(row,4,new QStandardItem(model.record(i).value(4).toString()));//品码
            model_top->setItem(row,5,new QStandardItem(model.record(i).value(5).toString()));//品名
            model_top->setItem(row,6,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(6).toFloat())));//价格
            model_top->setItem(row,7,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value("num_num").toFloat()-model.record(i).value("num_back").toFloat())));//数量
            model_top->setItem(row,8,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(22).toFloat())));//税收
            model_top->setItem(row,9,new QStandardItem(model.record(i).value(11).toString()));//折扣
            model_top->setItem(row,10,new QStandardItem(model.record(i).value(17).toString()));//赠送
            float jine = 0.00;
            jine = model.record(i).value("num_num").toFloat()
                    *model.record(i).value("price").toFloat()
                    *model.record(i).value("int_discount").toFloat()
                    *0.01
                    *(model.record(i).value("ch_presentflag").toString()=="Y"?0:1);
            model_top->setItem(row,11,new QStandardItem(QString().sprintf("%0.2f",jine)));//金额
            model_top->setItem(row,12,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(7).toFloat())));//点菜量
            model_top->setItem(row,13,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(8).toFloat())));//退量
            model_top->setItem(row,14,new QStandardItem(model.record(i).value(9).toString()));//退菜人
            model_top->setItem(row,15,new QStandardItem(model.record(i).value(10).toDateTime().toString("yyyy-MM-dd hh:mm:ss")));//退菜时间
            model_top->setItem(row,16,new QStandardItem(model.record(i).value(12).toString()));//折扣人
            model_top->setItem(row,17,new QStandardItem(model.record(i).value(13).toDateTime().toString("yyyy-MM-dd hh:mm:ss")));//折扣时间
            model_top->setItem(row,18,new QStandardItem(model.record(i).value(14).toString()));//厨打做法
            model_top->setItem(row,19,new QStandardItem(model.record(i).value(15).toString()));//套菜
            model_top->setItem(row,20,new QStandardItem(model.record(i).value(16).toString()));//特价
            model_top->setItem(row,21,new QStandardItem(model.record(i).value(18).toString()));//赠送人
            model_top->setItem(row,22,new QStandardItem(model.record(i).value(19).toDateTime().toString("yyyy-MM-dd hh:mm:ss")));//赠送时间
            model_top->setItem(row,23,new QStandardItem(model.record(i).value(20).toString()));//点菜人
            model_top->setItem(row,24,new QStandardItem(model.record(i).value(21).toDateTime().toString("yyyy-MM-dd hh:mm:ss")));//时间
        }
    }
    else if(ui->checkBox->isChecked() == true && ui->checkBox_2->isChecked() == false)//快餐退单和外卖退单
    {
        model_top->removeRows(0,model_top->rowCount());
        model.setQuery(QString("SELECT a.int_flowID,a.ch_billno,a.ch_payno,a.ch_tableno,a.ch_dishno,c.vch_dishname,a.num_price + a.num_price_add as price,a.num_num,case when b.ch_state = 'P' then num_num else num_back End num_back, a.vch_back_operID, a.dt_back_operdate, a.int_discount,a.vch_dis_operID,a.dt_dis_operdate,a.vch_print_memo,a.ch_suitflag, a.ch_specialflag, a.ch_presentflag, a.vch_pre_operID, a.dt_pre_operdate, a.vch_operID,a.dt_operdate,(a.num_num * c.num_price*int_rate*0.01)   FROM cey_u_orderdish a  ,cey_u_checkout_master b ,cey_bt_dish c Where (a.ch_dishno = c.ch_dishno) and( a.ch_payno = b.ch_payno)and (b.ch_state = 'P' or b.ch_type = 'Y')   and (left(b.dt_operdate,16) > '%1') and (left(b.dt_operdate,16) <= '%2')").arg(date1).arg(date2));
        for(int i = 0; i < model.rowCount(); i++)
        {
            int row = model_top->rowCount();
            model_top->setRowCount(row + 1);
            model_top->setItem(row,0,new QStandardItem(QString::number(row+1))); //序号
            model_top->setItem(row,1,new QStandardItem(model.record(i).value(1).toString()));//单号
            model_top->setItem(row,2,new QStandardItem(model.record(i).value(2).toString()));//付款号
            model_top->setItem(row,3,new QStandardItem(model.record(i).value(3).toString()));//餐桌
            model_top->setItem(row,4,new QStandardItem(model.record(i).value(4).toString()));//品码
            model_top->setItem(row,5,new QStandardItem(model.record(i).value(5).toString()));//品名
            model_top->setItem(row,6,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(6).toFloat())));//价格
            model_top->setItem(row,7,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value("num_num").toFloat()-model.record(i).value("num_back").toFloat())));//数量
            model_top->setItem(row,8,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(22).toFloat())));//税收
            model_top->setItem(row,9,new QStandardItem(model.record(i).value(11).toString()));//折扣
            model_top->setItem(row,10,new QStandardItem(model.record(i).value(17).toString()));//赠送
            float jine = 0.00;
            jine = model.record(i).value("num_num").toFloat()
                    *model.record(i).value("price").toFloat()
                    *model.record(i).value("int_discount").toFloat()
                    *0.01
                    *(model.record(i).value("ch_presentflag").toString()=="Y"?0:1);
            model_top->setItem(row,11,new QStandardItem(QString().sprintf("%0.2f",jine)));//金额
            model_top->setItem(row,12,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(7).toFloat())));//点菜量
            model_top->setItem(row,13,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(8).toFloat())));//退量
            model_top->setItem(row,14,new QStandardItem(model.record(i).value(9).toString()));//退菜人
            model_top->setItem(row,15,new QStandardItem(model.record(i).value(10).toDateTime().toString("yyyy-MM-dd hh:mm:ss")));//退菜时间
            model_top->setItem(row,16,new QStandardItem(model.record(i).value(12).toString()));//折扣人
            model_top->setItem(row,17,new QStandardItem(model.record(i).value(13).toDateTime().toString("yyyy-MM-dd hh:mm:ss")));//折扣时间
            model_top->setItem(row,18,new QStandardItem(model.record(i).value(14).toString()));//厨打做法
            model_top->setItem(row,19,new QStandardItem(model.record(i).value(15).toString()));//套菜
            model_top->setItem(row,20,new QStandardItem(model.record(i).value(16).toString()));//特价
            model_top->setItem(row,21,new QStandardItem(model.record(i).value(18).toString()));//赠送人
            model_top->setItem(row,22,new QStandardItem(model.record(i).value(19).toDateTime().toString("yyyy-MM-dd hh:mm:ss")));//赠送时间
            model_top->setItem(row,23,new QStandardItem(model.record(i).value(20).toString()));//点菜人
            model_top->setItem(row,24,new QStandardItem(model.record(i).value(21).toDateTime().toString("yyyy-MM-dd hh:mm:ss")));//时间
        }
    }
    else if(ui->checkBox->isChecked() == false && ui->checkBox_2->isChecked() == true)//按时间段查询
    {
        model_top->removeRows(0,model_top->rowCount());
        model.setQuery(QString("SELECT a.int_flowID,a.ch_billno,a.ch_payno,a.ch_tableno,a.ch_dishno,c.vch_dishname,a.num_price + a.num_price_add as price,a.num_num,a.num_back,a.vch_back_operID,a.dt_back_operdate,a.int_discount,a.vch_dis_operID,a.dt_dis_operdate, a.vch_print_memo,a.ch_suitflag,a.ch_specialflag,a.ch_presentflag,a.vch_pre_operID,a.dt_pre_operdate,a.vch_operID,a.dt_operdate,(a.num_num * c.num_price*int_rate*0.01)  FROM cey_u_orderdish a ,cey_u_checkout_master b,cey_bt_dish c Where (a.ch_dishno = c.ch_dishno)and(a.ch_payno = b.ch_payno)and (b.ch_state = 'Y')  and (left(b.dt_operdate,16) > '%1') and (left(b.dt_operdate,16) <= '%2') and(left(RIGHT(b.dt_operdate,8),5)) >= '%3' and(left(RIGHT(b.dt_operdate,8),5)) <= '%4' ").arg(date1).arg(date2).arg(time1).arg(time2));
        for(int i = 0; i < model.rowCount(); i++)
        {
            int row = model_top->rowCount();
            model_top->setRowCount(row + 1);
            model_top->setItem(row,0,new QStandardItem(QString::number(row+1))); //序号
            model_top->setItem(row,1,new QStandardItem(model.record(i).value(1).toString()));//单号
            model_top->setItem(row,2,new QStandardItem(model.record(i).value(2).toString()));//付款号
            model_top->setItem(row,3,new QStandardItem(model.record(i).value(3).toString()));//餐桌
            model_top->setItem(row,4,new QStandardItem(model.record(i).value(4).toString()));//品码
            model_top->setItem(row,5,new QStandardItem(model.record(i).value(5).toString()));//品名
            model_top->setItem(row,6,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(6).toFloat())));//价格
            model_top->setItem(row,7,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value("num_num").toFloat()-model.record(i).value("num_back").toFloat())));//数量
            model_top->setItem(row,8,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(22).toFloat())));//税收
            model_top->setItem(row,9,new QStandardItem(model.record(i).value(11).toString()));//折扣
            model_top->setItem(row,10,new QStandardItem(model.record(i).value(17).toString()));//赠送
            float jine = 0.00;
            jine = model.record(i).value("num_num").toFloat()
                    *model.record(i).value("price").toFloat()
                    *model.record(i).value("int_discount").toFloat()
                    *0.01
                    *(model.record(i).value("ch_presentflag").toString()=="Y"?0:1);
            model_top->setItem(row,11,new QStandardItem(QString().sprintf("%0.2f",jine)));//金额
            model_top->setItem(row,12,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(7).toFloat())));//点菜量
            model_top->setItem(row,13,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(8).toFloat())));//退量
            model_top->setItem(row,14,new QStandardItem(model.record(i).value(9).toString()));//退菜人
            model_top->setItem(row,15,new QStandardItem(model.record(i).value(10).toDateTime().toString("yyyy-MM-dd hh:mm:ss")));//退菜时间
            model_top->setItem(row,16,new QStandardItem(model.record(i).value(12).toString()));//折扣人
            model_top->setItem(row,17,new QStandardItem(model.record(i).value(13).toDateTime().toString("yyyy-MM-dd hh:mm:ss")));//折扣时间
            model_top->setItem(row,18,new QStandardItem(model.record(i).value(14).toString()));//厨打做法
            model_top->setItem(row,19,new QStandardItem(model.record(i).value(15).toString()));//套菜
            model_top->setItem(row,20,new QStandardItem(model.record(i).value(16).toString()));//特价
            model_top->setItem(row,21,new QStandardItem(model.record(i).value(18).toString()));//赠送人
            model_top->setItem(row,22,new QStandardItem(model.record(i).value(19).toDateTime().toString("yyyy-MM-dd hh:mm:ss")));//赠送时间
            model_top->setItem(row,23,new QStandardItem(model.record(i).value(20).toString()));//点菜人
            model_top->setItem(row,24,new QStandardItem(model.record(i).value(21).toDateTime().toString("yyyy-MM-dd hh:mm:ss")));//时间
        }
    }
    else
    {
        model_top->removeRows(0,model_top->rowCount());
        model.setQuery(QString("SELECT a.int_flowID,a.ch_billno,a.ch_payno,a.ch_tableno,a.ch_dishno,c.vch_dishname,a.num_price + a.num_price_add as price,a.num_num,case when b.ch_state = 'P' then num_num else num_back End num_back, a.vch_back_operID, a.dt_back_operdate, a.int_discount,a.vch_dis_operID,a.dt_dis_operdate,a.vch_print_memo,a.ch_suitflag, a.ch_specialflag, a.ch_presentflag, a.vch_pre_operID, a.dt_pre_operdate, a.vch_operID,a.dt_operdate,(a.num_num * c.num_price*int_rate*0.01)   FROM cey_u_orderdish a  ,cey_u_checkout_master b ,cey_bt_dish c Where (a.ch_dishno = c.ch_dishno) and( a.ch_payno = b.ch_payno)and (b.ch_state = 'P' or b.ch_type = 'Y')   and (left(b.dt_operdate,16) > '%1') and (left(b.dt_operdate,16) <= '%2') and(left(RIGHT(b.dt_operdate,8),5)) >= '%3' and(left(RIGHT(b.dt_operdate,8),5)) <= '%4'").arg(date1).arg(date2).arg(time1).arg(time2));
        for(int i = 0; i < model.rowCount(); i++)
        {
            int row = model_top->rowCount();
            model_top->setRowCount(row + 1);
            model_top->setItem(row,0,new QStandardItem(QString::number(row+1))); //序号
            model_top->setItem(row,1,new QStandardItem(model.record(i).value(1).toString()));//单号
            model_top->setItem(row,2,new QStandardItem(model.record(i).value(2).toString()));//付款号
            model_top->setItem(row,3,new QStandardItem(model.record(i).value(3).toString()));//餐桌
            model_top->setItem(row,4,new QStandardItem(model.record(i).value(4).toString()));//品码
            model_top->setItem(row,5,new QStandardItem(model.record(i).value(5).toString()));//品名
            model_top->setItem(row,6,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(6).toFloat())));//价格
            model_top->setItem(row,7,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value("num_num").toFloat()-model.record(i).value("num_back").toFloat())));//数量
            model_top->setItem(row,8,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(22).toFloat())));//税收
            model_top->setItem(row,9,new QStandardItem(model.record(i).value(11).toString()));//折扣
            model_top->setItem(row,10,new QStandardItem(model.record(i).value(17).toString()));//赠送
            float jine = 0.00;
            jine = model.record(i).value("num_num").toFloat()
                    *model.record(i).value("price").toFloat()
                    *model.record(i).value("int_discount").toFloat()
                    *0.01
                    *(model.record(i).value("ch_presentflag").toString()=="Y"?0:1);
            model_top->setItem(row,11,new QStandardItem(QString().sprintf("%0.2f",jine)));//金额
            model_top->setItem(row,12,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(7).toFloat())));//点菜量
            model_top->setItem(row,13,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(8).toFloat())));//退量
            model_top->setItem(row,14,new QStandardItem(model.record(i).value(9).toString()));//退菜人
            model_top->setItem(row,15,new QStandardItem(model.record(i).value(10).toDateTime().toString("yyyy-MM-dd hh:mm:ss")));//退菜时间
            model_top->setItem(row,16,new QStandardItem(model.record(i).value(12).toString()));//折扣人
            model_top->setItem(row,17,new QStandardItem(model.record(i).value(13).toDateTime().toString("yyyy-MM-dd hh:mm:ss")));//折扣时间
            model_top->setItem(row,18,new QStandardItem(model.record(i).value(14).toString()));//厨打做法
            model_top->setItem(row,19,new QStandardItem(model.record(i).value(15).toString()));//套菜
            model_top->setItem(row,20,new QStandardItem(model.record(i).value(16).toString()));//特价
            model_top->setItem(row,21,new QStandardItem(model.record(i).value(18).toString()));//赠送人
            model_top->setItem(row,22,new QStandardItem(model.record(i).value(19).toDateTime().toString("yyyy-MM-dd hh:mm:ss")));//赠送时间
            model_top->setItem(row,23,new QStandardItem(model.record(i).value(20).toString()));//点菜人
            model_top->setItem(row,24,new QStandardItem(model.record(i).value(21).toDateTime().toString("yyyy-MM-dd hh:mm:ss")));//时间
        }
    }

    QFont font;
    font.setPointSize(12);
    if(model_top->rowCount() > 0)
    {

        ui->tableView->updateSum(7);
        ui->tableView->updateSum(8);
        ui->tableView->updateSum(11);
        ui->tableView->updateSum(12);
        ui->tableView->updateSum(13);

    }
    ui->tableView->tableView->selectRow(0);

}

void back_sales::on_pushButton_11_clicked()//导出
{

}
