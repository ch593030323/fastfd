#include "back_anti_checkout.h"
#include "ui_back_anti_checkout.h"
#include <QSqlQueryModel>
#include <QSqlRecord>
#include "lds_tableview_delegate_check.h"
#include <QDesktopWidget>
#include <QApplication>
#include "n_func.h"

back_anti_checkout::back_anti_checkout(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::back_anti_checkout)
{
    ui->setupUi(this);
    //this->setWindowFlags(Qt::FramelessWindowHint|Qt::Dialog);this->setStyleSheet("QDialog{border:2px solid gray}");
    init_control();
    ui->dateTimeEdit->setDateTime(QDateTime(n_func::f_get_sysdatetime().date(), QTime(0,0,0)));
    ui->dateTimeEdit_2->setDateTime(QDateTime(n_func::f_get_sysdatetime().date(), QTime(23,59,59)));
//    QDesktopWidget *d=QApplication::desktop();
//    this->setFixedSize(d->width(),d->height() - 40);
}

back_anti_checkout::~back_anti_checkout()
{
    delete ui;
}

void back_anti_checkout::init_control()
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

    //ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置属性为只读

    QStringList labels1;
    labels1<<tr("序号")<<tr("单号")<<tr("结帐号")<<tr("消费总额")<<tr("操作员")<<tr("时间")<<tr("新结帐号")<<tr("桌号")<<tr("点菜人");
    model_top = new QStandardItemModel();
    model_top->setHorizontalHeaderLabels(labels1);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);ui->tableView->setModel(model_top);
    model.clear();
    model.setQuery(QString("SELECT DISTINCT b.ch_billno,b.ch_payno_un,b.num_cost_un,b.vch_operid,b.dt_operdate,a.ch_payno,a.ch_tableno,a.vch_operID FROM cey_u_orderdish a,cey_u_uncheckout_log b WHERE(b.ch_billno = a.ch_billno)and(b.dt_operdate >= '%1')AND(b.dt_operdate < '%2' )").arg(date1).arg(date2));
    for(int i = 0; i < model.rowCount(); i++)
    {
        int row = model_top->rowCount();
        model_top->setRowCount(row + 1);
        model_top->setItem(row,0,new QStandardItem(QString::number(row+1))); //序号
        model_top->setItem(row,1,new QStandardItem(model.record(i).value(0).toString())); //单号
        model_top->setItem(row,2,new QStandardItem(model.record(i).value(1).toString()));//结帐号
        model_top->setItem(row,3,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(2).toFloat())));//消费总额
        model_top->setItem(row,4,new QStandardItem(model.record(i).value(3).toString()));//操作员员
        model_top->setItem(row,5,new QStandardItem(model.record(i).value(4).toDateTime().toString("yyyy-MM-dd hh:mm:ss")));//时间
        model_top->setItem(row,6,new QStandardItem(model.record(i).value(5).toString()));//新结帐号
        model_top->setItem(row,7,new QStandardItem(model.record(i).value(6).toString()));//桌号
        model_top->setItem(row,8,new QStandardItem(model.record(i).value(7).toString()));//点菜人
    }

    ui->tableView->selectRow(0);

    //ui->tableView_2->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置属性为只读
    lds_tableview_delegate_check * delegate = new lds_tableview_delegate_check(11,ui->tableView_2);//第6,9行为点菜行号，不显示,在折扣中有用
    ui->tableView_2->setItemDelegate(delegate);

    QStringList labels;
    labels<<tr("行号")<<tr("品号")<<tr("品名")<<tr("单位")<<tr("价格")<<tr("加价")<<tr("数量")<<tr("折扣")<<tr("税率")<< tr("税收") << tr("金额") << tr("赠送")<<tr("特价");
    model_left = new QStandardItemModel();
    model_left->setHorizontalHeaderLabels(labels);
    ui->tableView_2->setModel(model_left);
    if(model_top->rowCount() > 0)
    {
        model.clear();
        model.setQuery(QString("SELECT a.ch_dishno,IFNULL(b.vch_dishname,a.ch_dishno),(select d.vch_unitname from cey_bt_unit d where d.ch_unitno=b.ch_unitno)as k,a.num_num,a.num_price,a.num_price_add,a.int_discount,a.ch_presentflag,a.ch_specialflag,b.int_rate,a.num_num*(b.int_rate*b.num_price*0.01) FROM cey_u_orderdish a,cey_bt_dish b,cey_u_uncheckout_log c WHERE  a.ch_dishno = b.ch_dishno  and  a.ch_billno = c.ch_billno and a.ch_billno = '%1'").arg(model_top->item(0,1)->text()));
        for(int i = 0; i < model.rowCount(); i++)
        {
            int row = model_left->rowCount();
            model_left->setRowCount(row + 1);
            model_left->setItem(row,0,new QStandardItem(QString::number(row+1))); //序号
            model_left->setItem(row,1,new QStandardItem(model.record(i).value(0).toString())); //品码
            model_left->setItem(row,2,new QStandardItem(model.record(i).value(1).toString())); //品名
            model_left->setItem(row,3,new QStandardItem(model.record(i).value(2).toString())); //单位
            model_left->setItem(row,4,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(4).toFloat()))); //价格
            model_left->setItem(row,5,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(5).toFloat()))); //加价
            model_left->setItem(row,6,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(3).toFloat()))); //数量
            QString zhe = tr("折扣:")+model.record(i).value(6).toString() + tr("%");
            model_left->setItem(row,7,new QStandardItem(zhe)); //折扣
            model_left->setItem(row,8,new QStandardItem(model.record(i).value(9).toString())); //税率
            model_left->setItem(row,9,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(10).toFloat()))); //税收
            float jine = 0.00;
            jine = model.record(i).value(3).toFloat() * model.record(i).value(4).toFloat() * model.record(i).value(6).toFloat() * 0.01 + model.record(i).value(5).toFloat() + model.record(i).value(10).toFloat();
            model_left->setItem(row,10,new QStandardItem(QString().sprintf("%0.2f",jine))); //金额
            model_left->setItem(row,11,new QStandardItem(model.record(i).value(7).toString())); //赠送
            model_left->setItem(row,12,new QStandardItem(model.record(i).value(8).toString())); //特价
        }
    }

    ui->tableView_2->selectRow(0);

    ui->tableView_3->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置属性为只读
    QStringList labels2;
    labels2<<tr("付款方式")<<tr("付款金额")<<tr("找零")<<tr("实收")<<tr("面额")<<tr("凭证");
    model_right = new QStandardItemModel();
    model_right->setHorizontalHeaderLabels(labels2);
    ui->tableView_3->setModel(model_right);

    if(model_top->rowCount() > 0)
    {
        model.clear();
        model.setQuery(QString("SELECT IFNULL((select b.vch_paymodename from cey_bt_paymode b where b.ch_paymodeno=a.ch_paymodeno),a.ch_paymodeno),a.num_payamount ,a.num_change ,a.num_realamount ,a.num_face ,a.vch_voucherno  FROM cey_u_checkout_detail a  WHERE  a.ch_payno ='%1'").arg(model_top->item(0,2)->text()));
        for(int i = 0; i < model.rowCount(); i++)
        {
            int row = model_right->rowCount();
            model_right->setRowCount(row + 1);
            model_right->setItem(row,0,new QStandardItem(model.record(i).value(0).toString())); //付款方式
            model_right->setItem(row,1,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(1).toFloat())));//付款金额
            model_right->setItem(row,2,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(2).toFloat())));//找零
            model_right->setItem(row,3,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(3).toFloat())));//实收金额
            model_right->setItem(row,4,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(4).toFloat())));//面额
            QString str;
            if(model.record(i).value(5).toString() != "null")
            {
                str = model.record(i).value(5).toString();
            }
            model_right->setItem(row,5,new QStandardItem(str));//凭证
        }
    }

    ui->tableView_3->selectRow(0);

}

void back_anti_checkout::on_pushButton_12_clicked()
{
    this->close();
}

void back_anti_checkout::on_tableView_clicked(const QModelIndex &index)
{
    int row = ui->tableView->currentIndex().row();
    if(row <= -1)
    {
        return;
    }

    QSqlQueryModel model;
    QFont font;
    font.setPointSize(12);
    model_left->clear();
    QStringList labels;
    labels<<tr("行号")<<tr("品号")<<tr("品名")<<tr("单位")<<tr("价格")<<tr("加价")<<tr("数量")<<tr("折扣")<<tr("税率")<< tr("税收") << tr("金额") << tr("赠送")<<tr("特价");
    model_left->setHorizontalHeaderLabels(labels);
    if(model_top->rowCount() > 0)
    {
        model.clear();
        model.setQuery(QString("SELECT a.ch_dishno,IFNULL(b.vch_dishname,a.ch_dishno),(select d.vch_unitname from cey_bt_unit d where d.ch_unitno=b.ch_unitno)as k,a.num_num,a.num_price,a.num_price_add,a.int_discount,a.ch_presentflag,a.ch_specialflag,b.int_rate,a.num_num*(b.int_rate*b.num_price*0.01) FROM cey_u_orderdish a,cey_bt_dish b,cey_u_uncheckout_log c WHERE  a.ch_dishno = b.ch_dishno  and  a.ch_billno = c.ch_billno and a.ch_billno = '%1'").arg(model_top->item(row,1)->text()));
        for(int i = 0; i < model.rowCount(); i++)
        {
            int row = model_left->rowCount();
            model_left->setRowCount(row + 1);
            model_left->setItem(row,0,new QStandardItem(QString::number(row+1))); //序号
            model_left->setItem(row,1,new QStandardItem(model.record(i).value(0).toString())); //品码
            model_left->setItem(row,2,new QStandardItem(model.record(i).value(1).toString())); //品名
            model_left->setItem(row,3,new QStandardItem(model.record(i).value(2).toString())); //单位
            model_left->setItem(row,4,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(4).toFloat()))); //价格
            model_left->setItem(row,5,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(5).toFloat()))); //加价
            model_left->setItem(row,6,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(3).toFloat()))); //数量
            QString zhe = tr("折扣:")+model.record(i).value(6).toString() + tr("%");
            model_left->setItem(row,7,new QStandardItem(zhe)); //折扣
            model_left->setItem(row,8,new QStandardItem(model.record(i).value(9).toString())); //税率
            model_left->setItem(row,9,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(10).toFloat()))); //税收
            float jine = 0.00;
            jine = model.record(i).value(3).toFloat() * model.record(i).value(4).toFloat() * model.record(i).value(6).toFloat() * 0.01 + model.record(i).value(5).toFloat() + model.record(i).value(10).toFloat();
            model_left->setItem(row,10,new QStandardItem(QString().sprintf("%0.2f",jine))); //金额
            model_left->setItem(row,11,new QStandardItem(model.record(i).value(7).toString())); //赠送
            model_left->setItem(row,12,new QStandardItem(model.record(i).value(8).toString())); //特价
        }
    }

    ui->tableView_2->selectRow(0);

    model_right->clear();
    QStringList labels2;
    labels2<<tr("付款方式")<<tr("付款金额")<<tr("找零")<<tr("实收")<<tr("面额")<<tr("凭证");
    model_right->setHorizontalHeaderLabels(labels2);
    ui->tableView_3->setModel(model_right);

    if(model_top->rowCount() > 0)
    {
        model.clear();
        model.setQuery(QString("SELECT IFNULL((select b.vch_paymodename from cey_bt_paymode b where b.ch_paymodeno=a.ch_paymodeno),a.ch_paymodeno),a.num_payamount ,a.num_change ,a.num_realamount ,a.num_face ,a.vch_voucherno  FROM cey_u_checkout_detail a  WHERE  a.ch_payno ='%1'").arg(model_top->item(row,2)->text()));
        for(int i = 0; i < model.rowCount(); i++)
        {
            int row = model_right->rowCount();
            model_right->setRowCount(row + 1);
            model_right->setItem(row,0,new QStandardItem(model.record(i).value(0).toString())); //付款方式
            model_right->setItem(row,1,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(1).toFloat())));//付款金额
            model_right->setItem(row,2,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(2).toFloat())));//找零
            model_right->setItem(row,3,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(3).toFloat())));//实收金额
            model_right->setItem(row,4,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(4).toFloat())));//面额
            QString str;
            if(model.record(i).value(5).toString() != "null")
            {
                str = model.record(i).value(5).toString();
            }
            model_right->setItem(row,5,new QStandardItem(str));//凭证
        }
    }



    ui->tableView_3->selectRow(0);
}

void back_anti_checkout::on_pushButton_10_clicked()
{
    QString date1 = ui->dateTimeEdit->dateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString date2 = ui->dateTimeEdit_2->dateTime().toString("yyyy-MM-dd hh:mm:ss");
    QStringList labels1;
    labels1<<tr("序号")<<tr("单号")<<tr("结帐号")<<tr("消费总额")<<tr("操作员")<<tr("时间")<<tr("新结帐号")<<tr("桌号")<<tr("点菜人");
    model_top = new QStandardItemModel();
    model_top->setHorizontalHeaderLabels(labels1);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);ui->tableView->setModel(model_top);
    QSqlQueryModel model;
    model.setQuery(QString("SELECT DISTINCT b.ch_billno,b.ch_payno_un,b.num_cost_un,b.vch_operid,b.dt_operdate,a.ch_payno,a.ch_tableno,a.vch_operID FROM cey_u_orderdish a,cey_u_uncheckout_log b WHERE(b.ch_billno = a.ch_billno)and(b.dt_operdate >= '%1')AND(b.dt_operdate < '%2' )").arg(date1).arg(date2));
    for(int i = 0; i < model.rowCount(); i++)
    {
        int row = model_top->rowCount();
        model_top->setRowCount(row + 1);
        model_top->setItem(row,0,new QStandardItem(QString::number(row+1))); //序号
        model_top->setItem(row,1,new QStandardItem(model.record(i).value(0).toString())); //单号
        model_top->setItem(row,2,new QStandardItem(model.record(i).value(1).toString()));//结帐号
        model_top->setItem(row,3,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(2).toFloat())));//消费总额
        model_top->setItem(row,4,new QStandardItem(model.record(i).value(3).toString()));//操作员员
        model_top->setItem(row,5,new QStandardItem(model.record(i).value(4).toDateTime().toString("yyyy-MM-dd hh:mm:ss")));//时间
        model_top->setItem(row,6,new QStandardItem(model.record(i).value(5).toString()));//新结帐号
        model_top->setItem(row,7,new QStandardItem(model.record(i).value(6).toString()));//桌号
        model_top->setItem(row,8,new QStandardItem(model.record(i).value(7).toString()));//点菜人
    }

    ui->tableView->selectRow(0);

    ui->tableView_2->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置属性为只读
    lds_tableview_delegate_check *delegate = new lds_tableview_delegate_check(11, ui->tableView_2);
    ui->tableView_2->setItemDelegate(delegate);

    QStringList labels;
    labels<<tr("行号")<<tr("品号")<<tr("品名")<<tr("单位")<<tr("价格")<<tr("加价")<<tr("数量")<<tr("折扣")<<tr("税率")<<tr("税收") << tr("金额") << tr("赠送")<<tr("特价");
    model_left = new QStandardItemModel();
    model_left->setHorizontalHeaderLabels(labels);
    ui->tableView_2->setModel(model_left);
    if(model_top->rowCount() > 0)
    {
        model.clear();
        model.setQuery(QString("SELECT a.ch_dishno,IFNULL(b.vch_dishname,a.ch_dishno),(select d.vch_unitname from cey_bt_unit d where d.ch_unitno=b.ch_unitno)as k,a.num_num,a.num_price,a.num_price_add,a.int_discount,a.ch_presentflag,a.ch_specialflag,b.int_rate,a.num_num*(b.int_rate*b.num_price*0.01) FROM cey_u_orderdish a,cey_bt_dish b,cey_u_uncheckout_log c WHERE  a.ch_dishno = b.ch_dishno  and  a.ch_billno = c.ch_billno and a.ch_billno = '%1'").arg(model_top->item(0,1)->text()));
        for(int i = 0; i < model.rowCount(); i++)
        {
            int row = model_left->rowCount();
            model_left->setRowCount(row + 1);
            model_left->setItem(row,0,new QStandardItem(QString::number(row+1))); //序号
            model_left->setItem(row,1,new QStandardItem(model.record(i).value(0).toString())); //品码
            model_left->setItem(row,2,new QStandardItem(model.record(i).value(1).toString())); //品名
            model_left->setItem(row,3,new QStandardItem(model.record(i).value(2).toString())); //单位
            model_left->setItem(row,4,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(4).toFloat()))); //价格
            model_left->setItem(row,5,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(5).toFloat()))); //加价
            model_left->setItem(row,6,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(3).toFloat()))); //数量
            QString zhe = tr("折扣:")+model.record(i).value(6).toString() + tr("%");
            model_left->setItem(row,7,new QStandardItem(zhe)); //折扣
            model_left->setItem(row,8,new QStandardItem(model.record(i).value(9).toString())); //税率
            model_left->setItem(row,9,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(10).toFloat()))); //税收
            float jine = 0.00;
            jine = model.record(i).value(3).toFloat() * model.record(i).value(4).toFloat() * model.record(i).value(6).toFloat() * 0.01 + model.record(i).value(5).toFloat() + model.record(i).value(10).toFloat();
            model_left->setItem(row,10,new QStandardItem(QString().sprintf("%0.2f",jine))); //金额
            model_left->setItem(row,11,new QStandardItem(model.record(i).value(7).toString())); //赠送
            model_left->setItem(row,12,new QStandardItem(model.record(i).value(8).toString())); //特价
        }
    }


    ui->tableView_2->selectRow(0);

    ui->tableView_3->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置属性为只读
    QStringList labels2;
    labels2<<tr("付款方式")<<tr("付款金额")<<tr("找零")<<tr("实收")<<tr("面额")<<tr("凭证");
    model_right = new QStandardItemModel();
    model_right->setHorizontalHeaderLabels(labels2);
    ui->tableView_3->setModel(model_right);

    if(model_top->rowCount() > 0)
    {
        model.clear();
        model.setQuery(QString("SELECT IFNULL((select b.vch_paymodename from cey_bt_paymode b where b.ch_paymodeno=a.ch_paymodeno),a.ch_paymodeno),a.num_payamount ,a.num_change ,a.num_realamount ,a.num_face ,a.vch_voucherno  FROM cey_u_checkout_detail a  WHERE  a.ch_payno ='%1'").arg(model_top->item(0,2)->text()));
        for(int i = 0; i < model.rowCount(); i++)
        {
            int row = model_right->rowCount();
            model_right->setRowCount(row + 1);
            model_right->setItem(row,0,new QStandardItem(model.record(i).value(0).toString())); //付款方式
            model_right->setItem(row,1,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(1).toFloat())));//付款金额
            model_right->setItem(row,2,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(2).toFloat())));//找零
            model_right->setItem(row,3,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(3).toFloat())));//实收金额
            model_right->setItem(row,4,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(4).toFloat())));//面额
            QString str;
            if(model.record(i).value(5).toString() != "null")
            {
                str = model.record(i).value(5).toString();
            }
            model_right->setItem(row,5,new QStandardItem(str));//凭证
        }
    }


    ui->tableView_3->selectRow(0);
}

void back_anti_checkout::on_pushButton_11_clicked()//导出
{

}
