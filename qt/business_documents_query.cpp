#include "business_documents_query.h"
#include "lds_tableview.h"
#include <QSqlQueryModel>
#include <QSqlRecord>
#include "lds_tableview_delegate_check.h"
#include "lds_messagebox.h"
#include "lds_messagebox.h"
#include <QSqlQuery>
#include "fast_order_2.h"
#include"n_func.h"
#include <QSettings>
#include "lds_messagebox.h"
#include <QDesktopWidget>
#include <QApplication>
#include "lds_tableview_delegate_check.h"
#include <QSqlError>
#include "backheader.h"
#include "trans_u2g.h"
#include "fast_order_2.h"
#include "fast_pay.h"

business_documents_query::business_documents_query(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::business_documents_query)
{
    //this->setWindowFlags(Qt::FramelessWindowHint|Qt::Dialog);this->setStyleSheet("QDialog{border:2px solid gray}");
    ui->setupUi(this);
    init_data();

    ui->dateTimeEdit->setDateTime(QDateTime(n_func::f_get_sysdatetime().date(), QTime(0,0,0)));
    ui->dateTimeEdit_2->setDateTime(QDateTime(n_func::f_get_sysdatetime().date(), QTime(23,59,59)));


}

business_documents_query::~business_documents_query()
{
    delete ui;
}

void business_documents_query::init_data()
{
    ui->tableView->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置属性为只读


    QStringList labels;
    labels<<tr("行号")<<tr("付款号")<<tr("账单号")<<tr("餐桌")<<tr("人数")<<tr("实收")
         <<tr("应收")<<tr("消费")<<tr("税收")<<tr("折扣")<<tr("赠送")<<tr("抹零")<<tr("服务费")<<tr("低消")<<tr("操作员")<<tr("时间")<<tr("Cccc")<<tr("bbbbb")<<tr("ddddd")<<tr("开台备注");
    model_top = new SqlQueryModel_Linenum1();
    model_top->setlist(labels);

    QSqlQueryModel model1;
    model1.setQuery("Select NOW()");
    QDateTime time =model1.record(0).value(0).toDateTime();
    QString str = time.toString("yyyy-MM-dd");
    QString str1 = "00:00";
    QString str2 = str + " " + str1;
    QDateTime time_box1;
    time_box1 = QDateTime::fromString(str2, "yyyy-MM-dd hh:mm");
    ui->dateTimeEdit->setDateTime(time_box1);
    ui->dateTimeEdit_2->setDateTime(model1.record(0).value(0).toDateTime());
    QString date1 = ui->dateTimeEdit->dateTime().toString("yyyy-MM-dd hh:mm");
    QString date2 = ui->dateTimeEdit_2->dateTime().toString("yyyy-MM-dd hh:mm");
    model_top->setQuery(QString("SELECT ch_state,ch_payno, ch_billno,IFNULL((Select vch_tablename from cey_bt_table where ch_tableno = cey_u_checkout_master.ch_tableno),'Q000'),(Select sum(int_person) from cey_u_table where ch_payno = cey_u_checkout_master.ch_payno),(num_cost - num_blotout   + num_rate + num_lowcost + num_service - num_present) as shishou , (num_cost) as yingshou ,num_cost,num_rate,num_discount,num_present,num_blotout,num_service,num_lowcost,vch_operID,dt_operdate,case when ch_state = 'Y' then IFNULL((select sum(cey_u_checkout_detail.num_payamount) from cey_u_checkout_detail where cey_u_checkout_detail.ch_payno = cey_u_checkout_master.ch_payno and cey_u_checkout_detail.ch_paymodeno = 'YY'),0.00) else 0 end,case when ch_state = 'Y' then IFNULL((select sum(cey_u_checkout_detail.num_payamount) from cey_u_checkout_detail where cey_u_checkout_detail.ch_payno = cey_u_checkout_master.ch_payno and cey_u_checkout_detail.ch_paymodeno = 'ZZ'),0.00) else 0 end,ch_state,IFNULL((select vch_memo from cey_u_table where ch_tableno = cey_u_checkout_master.ch_tableno and ch_billno = cey_u_checkout_master.ch_billno and ch_payno = cey_u_checkout_master.ch_payno), '') FROM cey_u_checkout_master Where(dt_operdate >= '%1')and( dt_operdate <= '%2')and ch_state = 'Y' order by ch_payno").arg(date1).arg(date2));
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);ui->tableView->setModel(model_top);
    connect(ui->tableView->tableView,SIGNAL(clicked(QModelIndex)),SLOT(tableView_clicked(QModelIndex)));
    ui->tableView->tableView->setColumnHidden(16,true);
    ui->tableView->tableView->setColumnHidden(17,true);
    ui->tableView->tableView->setColumnHidden(18,true);

    if(model_top->rowCount() > 0)
    {
        ui->tableView->tableView->selectRow(0);
        ui->tableView->updateSum(4);
        ui->tableView->updateSum(5);
        ui->tableView->updateSum(6);
        ui->tableView->updateSum(7);
        ui->tableView->updateSum(8);
        ui->tableView->updateSum(9);
        ui->tableView->updateSum(10);
        ui->tableView->updateSum(11);
        ui->tableView->updateSum(12);
        ui->tableView->updateSum(13);
    }

    //菜品消费合计
    ui->tableView_2->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置属性为只读
    ui->tableView_2->tableView->setItemDelegate(new lds_tableview_delegate_check(QVector<int>()
                                                                                 <<14
                                                                                 <<15
                                                                                 <<16
                                                                                 <<17
                                                                                 ,
                                                                                 ui->tableView_2->tableView));

    model_left = new SqlQueryModel_Linenum1();
    model_left->setlist(QStringList() << tr("行号") << tr("品号") << tr("品名") << tr("单位") << tr("价格") << tr("数量") << tr("退量") << tr("数量") << tr("折扣") << tr("金额") << tr("税率") << tr("特殊做法") << tr("点菜人") << tr("时间") << tr("厨打") << tr("套菜") << tr("特价") << tr("赠送"));
    model_left->setQuery(QString("SELECT a.ch_tableno,a.ch_dishno,b.vch_dishname,c.vch_unitname,a.num_price,a.num_num,a.num_back,(num_num - num_back) as tuishuliang,a.int_discount,(a.num_price * (a.num_num - a.num_back) * a.int_discount * 0.01) as jine,b.int_rate,a.vch_print_memo,a.vch_operID,a.dt_operdate,a.ch_printflag,a.ch_suitflag,a.ch_specialflag,a.ch_presentflag,a.num_price_add,a.vch_back_operID,a.dt_back_operdate,a.vch_dis_operID,a.dt_dis_operdate, a.ch_suitno,a.vch_pre_operID,a.dt_pre_operdate,a.ch_togono,a.ch_payno  FROM cey_bt_dish b, cey_u_orderdish a ,cey_bt_unit c  WHERE (c.ch_unitno=b.ch_unitno) and(a.ch_dishno = b.ch_dishno)and( a.ch_payno = '%1')").arg(model_top->record(0).value(1).toString()));
    ui->tableView_2->setModel(model_left);
    ui->tableView_2->tableView->setColumnHidden(18,true);
    ui->tableView_2->tableView->setColumnHidden(19,true);
    ui->tableView_2->tableView->setColumnHidden(20,true);
    ui->tableView_2->tableView->setColumnHidden(21,true);
    ui->tableView_2->tableView->setColumnHidden(22,true);
    ui->tableView_2->tableView->setColumnHidden(23,true);
    ui->tableView_2->tableView->setColumnHidden(24,true);
    ui->tableView_2->tableView->setColumnHidden(25,true);
    ui->tableView_2->tableView->setColumnHidden(26,true);
    ui->tableView_2->tableView->setColumnHidden(27,true);
    if(model_left->rowCount() > 0)
    {
        ui->tableView_2->tableView->selectRow(0);
        ui->tableView_2->updateSum(4);
        ui->tableView_2->updateSum(5);
        ui->tableView_2->updateSum(6);
        ui->tableView_2->updateSum(7);
        ui->tableView_2->updateSum(9);
        ui->tableView_2->updateSum(10);
    }
    //消费明细表
    ui->tableView_3->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置属性为只读
    model_right = new SqlQueryModel_Linenum1();
    model_right->setlist(QStringList()<<tr("行号")<<tr("收银方式")<<tr("付款额")<<tr("找零")<<tr("实收")<<tr("面额") << tr("编号"));
    model_right->setQuery(QString("SELECT a.ch_payno,b.vch_paymodename,a.num_payamount,a.num_change,a.num_realamount,a.num_face,a.vch_voucherno,a.vch_memo,a.vch_operID,a.dt_operdate,a.ch_paymodeno  FROM cey_u_checkout_detail a ,cey_bt_paymode b where a.ch_paymodeno = b.ch_paymodeno and a.ch_payno = '%1'").arg(model_top->record(0).value(1).toString()));
    ui->tableView_3->tableView->setColumnHidden(7,true);
    ui->tableView_3->tableView->setColumnHidden(8,true);
    ui->tableView_3->tableView->setColumnHidden(9,true);
    ui->tableView_3->tableView->setColumnHidden(10,true);
    ui->tableView_3->setModel(model_right);
    if(model_right->rowCount() > 0)
    {
        ui->tableView_3->updateSum(2);
        ui->tableView_3->updateSum(3);
        ui->tableView_3->updateSum(4);
        ui->tableView_3->updateSum(5);
        ui->tableView_3->tableView->selectRow(0);
    }
    //    ui->tableView->tableView->resizeColumnsToContents();
    //    ui->tableView_2->tableView->resizeColumnsToContents();
    //    ui->tableView_3->tableView->resizeColumnsToContents();
}

void business_documents_query::on_pushButton_6_clicked()
{
    this->close();
}

void business_documents_query::on_pushButton_2_clicked()//检索
{
    ui->pushButton_4->setEnabled(true);
    QStringList labels;
    labels<<tr("行号")<<tr("付款号")<<tr("账单号")<<tr("餐桌")<<tr("人数")<<tr("实收")
<<tr("应收")<<tr("消费")<<tr("税收")<<tr("折扣")<<tr("赠送")<<tr("抹零")<<tr("服务费")<<tr("低消")<<tr("操作员")<<tr("时间")<<tr("Cccc")<<tr("bbbbb")<<tr("ddddd")<<tr("开台备注");
    model_top->clear();
    model_top->setlist(labels);

    QString date1 = ui->dateTimeEdit->dateTime().toString("yyyy-MM-dd hh:mm");
    QString date2 = ui->dateTimeEdit_2->dateTime().toString("yyyy-MM-dd hh:mm");
    model_top->setQuery(QString("SELECT ch_state,ch_payno, ch_billno,IFNULL((Select vch_tablename from cey_bt_table where ch_tableno = cey_u_checkout_master.ch_tableno),'Q000'),(Select sum(int_person) from cey_u_table where ch_payno = cey_u_checkout_master.ch_payno),(num_cost - num_blotout   + num_rate + num_lowcost + num_service - num_present) as shishou , (num_cost) as yingshou ,num_cost,num_rate,num_discount,num_present,num_blotout,num_service,num_lowcost,vch_operID,dt_operdate,case when ch_state = 'Y' then IFNULL((select sum(cey_u_checkout_detail.num_payamount) from cey_u_checkout_detail where cey_u_checkout_detail.ch_payno = cey_u_checkout_master.ch_payno and cey_u_checkout_detail.ch_paymodeno = 'YY'),0.00) else 0 end,case when ch_state = 'Y' then IFNULL((select sum(cey_u_checkout_detail.num_payamount) from cey_u_checkout_detail where cey_u_checkout_detail.ch_payno = cey_u_checkout_master.ch_payno and cey_u_checkout_detail.ch_paymodeno = 'ZZ'),0.00) else 0 end,ch_state,IFNULL((select vch_memo from cey_u_table where ch_tableno = cey_u_checkout_master.ch_tableno and ch_billno = cey_u_checkout_master.ch_billno and ch_payno = cey_u_checkout_master.ch_payno), '') FROM cey_u_checkout_master Where(dt_operdate >= '%1')and( dt_operdate <= '%2')and ch_state = 'Y' order by ch_payno").arg(date1).arg(date2));
    ui->tableView->tableView->setColumnHidden(16,true);
    ui->tableView->tableView->setColumnHidden(17,true);
    ui->tableView->tableView->setColumnHidden(18,true);
    if(model_top->rowCount() > 0)
    {
        ui->tableView->tableView->selectRow(0);
        ui->tableView->updateSum(4);
        ui->tableView->updateSum(5);
        ui->tableView->updateSum(6);
        ui->tableView->updateSum(7);
        ui->tableView->updateSum(8);
        ui->tableView->updateSum(9);
        ui->tableView->updateSum(10);
        ui->tableView->updateSum(11);
        ui->tableView->updateSum(12);
        ui->tableView->updateSum(13);
    }
    //菜品消费合计
    model_left->setQuery(QString("SELECT a.ch_tableno,a.ch_dishno,b.vch_dishname,c.vch_unitname,a.num_price,a.num_num,a.num_back,(num_num - num_back) as tuishuliang,a.int_discount,(a.num_price * (a.num_num - a.num_back) * a.int_discount * 0.01) as jine,b.int_rate,a.vch_print_memo,a.vch_operID,a.dt_operdate,a.ch_printflag,a.ch_suitflag,a.ch_specialflag,a.ch_presentflag,a.num_price_add,a.vch_back_operID,a.dt_back_operdate,a.vch_dis_operID,a.dt_dis_operdate, a.ch_suitno,a.vch_pre_operID,a.dt_pre_operdate,a.ch_togono,a.ch_payno  FROM cey_bt_dish b, cey_u_orderdish a ,cey_bt_unit c  WHERE (c.ch_unitno=b.ch_unitno) and(a.ch_dishno = b.ch_dishno)and( a.ch_payno = '%1')").arg(model_top->record(0).value(1).toString()));
    ui->tableView_2->setModel(model_left);
    ui->tableView_2->tableView->setColumnHidden(18,true);
    ui->tableView_2->tableView->setColumnHidden(19,true);
    ui->tableView_2->tableView->setColumnHidden(20,true);
    ui->tableView_2->tableView->setColumnHidden(21,true);
    ui->tableView_2->tableView->setColumnHidden(22,true);
    ui->tableView_2->tableView->setColumnHidden(23,true);
    ui->tableView_2->tableView->setColumnHidden(24,true);
    ui->tableView_2->tableView->setColumnHidden(25,true);
    ui->tableView_2->tableView->setColumnHidden(26,true);
    ui->tableView_2->tableView->setColumnHidden(27,true);
    if(model_left->rowCount() > 0)
    {
        ui->tableView_2->tableView->selectRow(0);
        ui->tableView_2->updateSum(4);
        ui->tableView_2->updateSum(5);
        ui->tableView_2->updateSum(6);
        ui->tableView_2->updateSum(7);
        ui->tableView_2->updateSum(9);
        ui->tableView_2->updateSum(10);
    }
    //消费明细表
    model_right->setlist(QStringList()<<tr("行号")<<tr("收银方式")<<tr("付款额")<<tr("找零")<<tr("实收")<<tr("面额") << tr("编号"));
    model_right->setQuery(QString("SELECT a.ch_payno,b.vch_paymodename,a.num_payamount,a.num_change,a.num_realamount,a.num_face,a.vch_voucherno,a.vch_memo,a.vch_operID,a.dt_operdate,a.ch_paymodeno  FROM cey_u_checkout_detail a ,cey_bt_paymode b where a.ch_paymodeno = b.ch_paymodeno and a.ch_payno = '%1'").arg(model_top->record(0).value(1).toString()));
    ui->tableView_3->tableView->setColumnHidden(7,true);
    ui->tableView_3->tableView->setColumnHidden(8,true);
    ui->tableView_3->tableView->setColumnHidden(9,true);
    ui->tableView_3->tableView->setColumnHidden(10,true);
    if(model_right->rowCount() > 0)
    {
        ui->tableView_3->updateSum(2);
        ui->tableView_3->updateSum(3);
        ui->tableView_3->updateSum(4);
        ui->tableView_3->updateSum(5);
        ui->tableView_3->tableView->selectRow(0);
    }
}

void business_documents_query::tableView_clicked(const QModelIndex &index)
{
    QSqlQueryModel model;
    int row = ui->tableView->tableView->currentIndex().row();
    if(row <= -1)
    {
        ;
        lds_messagebox::warning(this, tr("消息提示"),tr("请选择一条数据!"));
        
        return;
    }
    //菜品消费合计
    model_left->setQuery(QString("SELECT a.ch_tableno,a.ch_dishno,b.vch_dishname,c.vch_unitname,a.num_price,a.num_num,a.num_back,(num_num - num_back) as tuishuliang,a.int_discount,(a.num_price * (a.num_num - a.num_back) * a.int_discount * 0.01) as jine,b.int_rate,a.vch_print_memo,a.vch_operID,a.dt_operdate,a.ch_printflag,a.ch_suitflag,a.ch_specialflag,a.ch_presentflag,a.num_price_add,a.vch_back_operID,a.dt_back_operdate,a.vch_dis_operID,a.dt_dis_operdate, a.ch_suitno,a.vch_pre_operID,a.dt_pre_operdate,a.ch_togono,a.ch_payno  FROM cey_bt_dish b, cey_u_orderdish a ,cey_bt_unit c  WHERE (c.ch_unitno=b.ch_unitno) and(a.ch_dishno = b.ch_dishno)and( a.ch_payno = '%1')").arg(model_top->record(row).value(1).toString()));
    ui->tableView_2->tableView->setModel(model_left);
    ui->tableView_2->tableView->setColumnHidden(18,true);
    ui->tableView_2->tableView->setColumnHidden(19,true);
    ui->tableView_2->tableView->setColumnHidden(20,true);
    ui->tableView_2->tableView->setColumnHidden(21,true);
    ui->tableView_2->tableView->setColumnHidden(22,true);
    ui->tableView_2->tableView->setColumnHidden(23,true);
    ui->tableView_2->tableView->setColumnHidden(24,true);
    ui->tableView_2->tableView->setColumnHidden(25,true);
    ui->tableView_2->tableView->setColumnHidden(26,true);
    ui->tableView_2->tableView->setColumnHidden(27,true);
    if(model_left->rowCount() > 0)
    {
        ui->tableView_2->tableView->selectRow(0);
        ui->tableView_2->updateSum(4);
        ui->tableView_2->updateSum(5);
        ui->tableView_2->updateSum(6);
        ui->tableView_2->updateSum(7);
        ui->tableView_2->updateSum(9);
        ui->tableView_2->updateSum(10);
    }
    //消费明细表
    model_right->setQuery(QString("SELECT a.ch_payno,b.vch_paymodename,a.num_payamount,a.num_change,a.num_realamount,a.num_face,a.vch_voucherno,a.vch_memo,a.vch_operID,a.dt_operdate,a.ch_paymodeno  FROM cey_u_checkout_detail a ,cey_bt_paymode b where a.ch_paymodeno = b.ch_paymodeno and a.ch_payno = '%1'").arg(model_top->record(row).value(1).toString()));
    ui->tableView_3->tableView->setColumnHidden(7,true);
    ui->tableView_3->tableView->setColumnHidden(8,true);
    ui->tableView_3->tableView->setColumnHidden(9,true);
    ui->tableView_3->tableView->setColumnHidden(10,true);
    if(model_right->rowCount() > 0){
        ui->tableView_3->updateSum(2);
        ui->tableView_3->updateSum(3);
        ui->tableView_3->updateSum(4);
        ui->tableView_3->updateSum(5);
    }
}

void business_documents_query::on_pushButton_4_clicked()//反结帐
{
    int row = ui->tableView->tableView->currentIndex().row();
    if(row <= -1)
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("请选择一条需要反结帐的数据!"));
        
        return;
    }

    QSqlQueryModel model_l;
    model_l.setQuery("Select NOW()");
    QString time = model_l.record(0).value(0).toDateTime().toString("yyyy-MM-dd hh:mm:ss");

    QSqlQuery query;
    QSqlDatabase::database().transaction();
    if(0==lds_messagebox::warning(this, tr("消息提示"),tr("请注意,快餐将做退单处理,是否继续?"),tr("确认"),tr("取消")))
    {
        query.prepare(QString("update cey_u_checkout_master set ch_state =:state , vch_memberno =null , vch_operID2 =:oper , dt_operdate2 =:timer where ch_payno =:fukuanhao "));
        query.bindValue(":state","P");
        query.bindValue(":oper",n_func::gs_operid);
        query.bindValue(":timer",time);
        query.bindValue(":fukuanhao",model_top->record(row).value(1).toString());
        if(query.exec())
        {

        }
        else
        {
            QSqlDatabase::database().rollback();
            lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_u_checkout_master失败!"));
            
            return;
        }

        query.prepare(QString("update cey_u_orderdish set vch_back_operID =:oper , dt_back_operdate =NOW()where ch_payno =:fukuanhao "));
        query.bindValue(":oper",n_func::gs_operid);
        query.bindValue(":fukuanhao",model_top->record(row).value(1).toString());
        if(query.exec())
        {

        }
        else
        {
            QSqlDatabase::database().rollback();
            lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_u_orderdish失败!"));
            
            return;
        }

        query.prepare(QString("insert into cey_u_uncheckout_log(ch_billno , ch_payno_un , num_cost_un , vch_operid , dt_operdate )values (:danhao,:fukuanhao,:shishou,:oper,:timer)"));
        query.bindValue(":danhao",model_top->record(row).value(2).toString());
        query.bindValue(":fukuanhao",model_top->record(row).value(1).toString());
        query.bindValue(":shishou",model_top->record(row).value(5).toString());
        query.bindValue(":oper",n_func::gs_operid);
        query.bindValue(":timer",time);
        if(query.exec())
        {
        }
        else
        {
            QSqlDatabase::database().rollback();
            qDebug() << query.lastError();  lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_u_uncheckout_log失败!"));
            return;
        }

        //2
        {
            QSqlQueryModel cook;
            for(int i = 0; i  < model_left->rowCount(); i++)
            {
                cook.clear();
                int row = fast_order_2::modelBase.rowCount();
                fast_order_2::modelBase.setRowCount(row + 1);
                fast_order_2::modelBase.setItem(row,0,new QStandardItem(QString::number(i+1)));//行号
                fast_order_2::modelBase.setItem(row,1,new QStandardItem(model_left->record(i).value("ch_dishno").toString()));//品码
                cook.setQuery(QString("select ch_dish_typeno,int_rate,ch_curprice from cey_bt_dish where ch_dishno = '%1' ").arg(model_left->record(i).value("ch_dishno").toString()));
                fast_order_2::modelBase.setItem(row,2,new QStandardItem(model_left->record(i).value("vch_dishname").toString()));//品名
                fast_order_2::modelBase.setItem(row,3,new QStandardItem(QString().sprintf("-%0.2f",model_left->record(i).value("num_num").toFloat())));//数量
                if(model_left->record(i).value("ch_suitflag").toString() == tr("Y"))
                {
                    fast_order_2::modelBase.setItem(row,4,new QStandardItem(tr("0.00")));//价格
                }
                else
                {
                    fast_order_2::modelBase.setItem(row,4,new QStandardItem(QString().sprintf("%0.2f",model_left->record(i).value("num_price").toFloat())));//价格
                }
                fast_order_2::modelBase.setItem(row,5,new QStandardItem(model_left->record(i).value("int_discount").toString()));//折扣值
                float aa = 0.00;
                aa = model_left->record(i).value("num_num").toFloat() * model_left->record(i).value("num_price").toFloat() * (model_left->record(i).value("int_discount").toFloat()) * 0.01 + model_left->record(i).value("num_price_add").toFloat()/* + cook.record(i).value(5).toFloat() + bb*/;
                if(model_left->record(i).value("ch_suitflag").toString() == tr("Y"))
                {
                    fast_order_2::modelBase.setItem(row,6,new QStandardItem(tr("0.00")));//价格
                }
                else
                {
                    fast_order_2::modelBase.setItem(row,6,new QStandardItem(QString().sprintf("%0.2f",aa)));//总价
                }
                fast_order_2::modelBase.setItem(row,7,new QStandardItem(model_left->record(i).value("vch_print_memo").toString()));//特殊做法
                fast_order_2::modelBase.setItem(row,8,new QStandardItem(model_left->record(i).value("ch_presentflag").toString()));//赠送
                fast_order_2::modelBase.setItem(row,9,new QStandardItem(model_left->record(i).value("ch_suitflag").toString()));//套菜
                fast_order_2::modelBase.setItem(row,10,new QStandardItem(model_left->record(i).value("ch_specialflag").toString()));//特价
                fast_order_2::modelBase.setItem(row,11,new QStandardItem("N"));//是否优惠
                fast_order_2::modelBase.setItem(row,12,new QStandardItem(model_left->record(i).value("vch_unitname").toString()));//单位
                fast_order_2::modelBase.setItem(row,13,new QStandardItem(model_left->record(i).value("vch_pre_operID").toString()));//赠送人

                fast_order_2::modelBase.setItem(row,14,new QStandardItem(model_left->record(i).value("vch_dis_operID").toString()));//定折人
                fast_order_2::modelBase.setItem(row,15,new QStandardItem(cook.record(i).value("ch_curprice").toString()));//是否实价菜
                QString zitaocai;
                if(model_left->record(i).value("ch_suitflag").toString() == "Y")
                {
                    QSqlQueryModel tt;
                    tt.setQuery(QString("select ch_suitno from cey_bt_dish_suit where ch_dishno ='%1' ").arg(model_left->record(i).value("ch_dishno").toString()));
                    zitaocai = tt.record(0).value("ch_suitno").toString();
                }
                fast_order_2::modelBase.setItem(row,16,new QStandardItem(zitaocai));//子套菜所在主套菜编码
                float bb = 0.00;
                bb = model_left->record(i).value("num_price").toFloat() * cook.record(0).value("int_rate").toFloat() * 0.01;
                fast_order_2::modelBase.setItem(row,17,new QStandardItem(QString().sprintf("%0.2f",bb)));//税点
                QString zhekou;
                if(model_left->record(i).value("int_discount").toFloat() != 100)
                {
                    zhekou = tr("Y");
                }
                else
                {
                    zhekou = "N";
                }
                fast_order_2::modelBase.setItem(row,18,new QStandardItem(zhekou));//是否折扣
                fast_order_2::modelBase.setItem(row,19,new QStandardItem(model_left->record(i).value("dt_dis_operdate").toDateTime().toString("yyyy-MM-dd hh:mm:ss")));//折扣时间
                fast_order_2::modelBase.setItem(row,20,new QStandardItem(model_left->record(i).value("dt_pre_operdate").toDateTime().toString("yyyy-MM-dd hh:mm:ss")));//赠送时间
                fast_order_2::modelBase.setItem(row,21,new QStandardItem(QString().sprintf("%0.2f",model_left->record(i).value("num_price_add").toFloat())));//做法加价
                fast_order_2::modelBase.setItem(row,22,new QStandardItem(cook.record(0).value("ch_dish_typeno").toString()));//所属大类编码

                cook.setQuery(QString("select ch_sub_typeno from cey_bt_dish where ch_dishno = '%1' ").arg(model_left->record(i).value("ch_dishno").toString()));
                fast_order_2::modelBase.setItem(row,23,new QStandardItem(cook.record(0).value("ch_sub_typeno").toString()));//所属小类编码
            }

            //保存
            QString pre_deskno=frontViewDialog_virtual::cur_desk_no;
            frontViewDialog_virtual::cur_desk_no="";
            fast_pay().fast_pay_print("", tr("反结账"),
                                      fast_pay::PRINT_BILL
                                      |fast_pay::PRINT_ONLY_COSUME);
            //还原
            frontViewDialog_virtual::cur_desk_no=pre_deskno;
            fast_order_2::modelBase.removeRows(0, fast_order_2::modelBase.rowCount());
        }
        //2
        QSqlDatabase::database().commit();
    }
    else
    {
        return;
    }

    //更新tableview
    QStringList labels;
    labels<<tr("行号")<<tr("付款号")<<tr("账单号")<<tr("餐桌")<<tr("人数")<<tr("实收")
<<tr("应收")<<tr("消费")<<tr("税收")<<tr("折扣")<<tr("赠送")<<tr("抹零")<<tr("服务费")<<tr("低消")<<tr("操作员")<<tr("时间")<<tr("Cccc")<<tr("bbbbb")<<tr("ddddd")<<tr("开台备注");
    model_top->clear();
    model_top->setlist(labels);

    QString date1 = ui->dateTimeEdit->dateTime().toString("yyyy-MM-dd hh:mm");
    QString date2 = ui->dateTimeEdit_2->dateTime().toString("yyyy-MM-dd hh:mm");
    model_top->setQuery(QString("SELECT ch_state,ch_payno, ch_billno,IFNULL((Select vch_tablename from cey_bt_table where ch_tableno = cey_u_checkout_master.ch_tableno),'Q000'),(Select sum(int_person) from cey_u_table where ch_payno = cey_u_checkout_master.ch_payno),(num_cost - num_blotout   + num_rate + num_lowcost + num_service - num_present) as shishou , (num_cost) as yingshou ,num_cost,num_rate,num_discount,num_present,num_blotout,num_service,num_lowcost,vch_operID,dt_operdate,case when ch_state = 'Y' then IFNULL((select sum(cey_u_checkout_detail.num_payamount) from cey_u_checkout_detail where cey_u_checkout_detail.ch_payno = cey_u_checkout_master.ch_payno and cey_u_checkout_detail.ch_paymodeno = 'YY'),0.00) else 0 end,case when ch_state = 'Y' then IFNULL((select sum(cey_u_checkout_detail.num_payamount) from cey_u_checkout_detail where cey_u_checkout_detail.ch_payno = cey_u_checkout_master.ch_payno and cey_u_checkout_detail.ch_paymodeno = 'ZZ'),0.00) else 0 end,ch_state,IFNULL((select vch_memo from cey_u_table where ch_tableno = cey_u_checkout_master.ch_tableno and ch_billno = cey_u_checkout_master.ch_billno and ch_payno = cey_u_checkout_master.ch_payno), '') FROM cey_u_checkout_master Where(dt_operdate >= '%1')and( dt_operdate <= '%2')and ch_state = 'Y' order by ch_payno").arg(date1).arg(date2));
    ui->tableView->tableView->setColumnHidden(16,true);
    ui->tableView->tableView->setColumnHidden(17,true);
    ui->tableView->tableView->setColumnHidden(18,true);
    if(model_top->rowCount() > 0)
    {
        ui->tableView->tableView->selectRow(0);
        ui->tableView->updateSum(4);
        ui->tableView->updateSum(5);
        ui->tableView->updateSum(6);
        ui->tableView->updateSum(7);
        ui->tableView->updateSum(8);
        ui->tableView->updateSum(9);
        ui->tableView->updateSum(10);
        ui->tableView->updateSum(11);
        ui->tableView->updateSum(12);
        ui->tableView->updateSum(13);
    }
    //菜品消费合计
    model_left->setQuery(QString("SELECT a.ch_tableno,a.ch_dishno,b.vch_dishname,c.vch_unitname,a.num_price,a.num_num,a.num_back,(num_num - num_back) as tuishuliang,a.int_discount,(a.num_price * (a.num_num - a.num_back) * a.int_discount * 0.01) as jine,b.int_rate,a.vch_print_memo,a.vch_operID,a.dt_operdate,a.ch_printflag,a.ch_suitflag,a.ch_specialflag,a.ch_presentflag,a.num_price_add,a.vch_back_operID,a.dt_back_operdate,a.vch_dis_operID,a.dt_dis_operdate, a.ch_suitno,a.vch_pre_operID,a.dt_pre_operdate,a.ch_togono,a.ch_payno  FROM cey_bt_dish b, cey_u_orderdish a ,cey_bt_unit c  WHERE (c.ch_unitno=b.ch_unitno) and(a.ch_dishno = b.ch_dishno)and( a.ch_payno = '%1')").arg(model_top->record(row).value(1).toString()));
    ui->tableView_2->tableView->setModel(model_left);
    ui->tableView_2->tableView->setColumnHidden(18,true);
    ui->tableView_2->tableView->setColumnHidden(19,true);
    ui->tableView_2->tableView->setColumnHidden(20,true);
    ui->tableView_2->tableView->setColumnHidden(21,true);
    ui->tableView_2->tableView->setColumnHidden(22,true);
    ui->tableView_2->tableView->setColumnHidden(23,true);
    ui->tableView_2->tableView->setColumnHidden(24,true);
    ui->tableView_2->tableView->setColumnHidden(25,true);
    ui->tableView_2->tableView->setColumnHidden(26,true);
    ui->tableView_2->tableView->setColumnHidden(27,true);
    if(model_left->rowCount() > 0)
    {
        ui->tableView_2->tableView->selectRow(0);
        ui->tableView_2->updateSum(4);
        ui->tableView_2->updateSum(5);
        ui->tableView_2->updateSum(6);
        ui->tableView_2->updateSum(7);
        ui->tableView_2->updateSum(9);
        ui->tableView_2->updateSum(10);
    }
    //消费明细表

    model_right->setQuery(QString("SELECT a.ch_payno,b.vch_paymodename,a.num_payamount,a.num_change,a.num_realamount,a.num_face,a.vch_voucherno,a.vch_memo,a.vch_operID,a.dt_operdate,a.ch_paymodeno  FROM cey_u_checkout_detail a ,cey_bt_paymode b where a.ch_paymodeno = b.ch_paymodeno and a.ch_payno = '%1'").arg(model_top->record(row).value(1).toString()));
    ui->tableView_3->tableView->setColumnHidden(7,true);
    ui->tableView_3->tableView->setColumnHidden(8,true);
    ui->tableView_3->tableView->setColumnHidden(9,true);
    ui->tableView_3->tableView->setColumnHidden(10,true);
    if(model_right->rowCount() > 0)
    {
        ui->tableView_3->updateSum(2);
        ui->tableView_3->updateSum(3);
        ui->tableView_3->updateSum(4);
        ui->tableView_3->updateSum(5);
        ui->tableView_3->tableView->selectRow(0);
    }

    lds_messagebox::warning(this, tr("反结账"), tr("反结账成功"));
}

void business_documents_query::on_checkBox_clicked()//作废
{
    QString date1 = ui->dateTimeEdit->dateTime().toString("yyyy-MM-dd hh:mm");
    QString date2 = ui->dateTimeEdit_2->dateTime().toString("yyyy-MM-dd hh:mm");

    if(ui->checkBox->isChecked() == false)
    {
        ui->pushButton_4->setEnabled(true);
        model_top->setQuery(QString("SELECT ch_state,ch_payno, ch_billno,IFNULL((Select vch_tablename from cey_bt_table where ch_tableno = cey_u_checkout_master.ch_tableno),'Q000'),(Select sum(int_person) from cey_u_table where ch_payno = cey_u_checkout_master.ch_payno),(num_cost - num_blotout   + num_rate + num_lowcost + num_service - num_present) as shishou , (num_cost) as yingshou ,num_cost,num_rate,num_discount,num_present,num_blotout,num_service,num_lowcost,vch_operID,dt_operdate,case when ch_state = 'Y' then IFNULL((select sum(cey_u_checkout_detail.num_payamount) from cey_u_checkout_detail where cey_u_checkout_detail.ch_payno = cey_u_checkout_master.ch_payno and cey_u_checkout_detail.ch_paymodeno = 'YY'),0.00) else 0 end,case when ch_state = 'Y' then IFNULL((select sum(cey_u_checkout_detail.num_payamount) from cey_u_checkout_detail where cey_u_checkout_detail.ch_payno = cey_u_checkout_master.ch_payno and cey_u_checkout_detail.ch_paymodeno = 'ZZ'),0.00) else 0 end,ch_state,IFNULL((select vch_memo from cey_u_table where ch_tableno = cey_u_checkout_master.ch_tableno and ch_billno = cey_u_checkout_master.ch_billno and ch_payno = cey_u_checkout_master.ch_payno), '') FROM cey_u_checkout_master Where(dt_operdate >= '%1')and( dt_operdate <= '%2')and ch_state = 'Y' order by ch_payno").arg(date1).arg(date2));
        ui->tableView->tableView->setColumnHidden(16,true);
        ui->tableView->tableView->setColumnHidden(17,true);
        ui->tableView->tableView->setColumnHidden(18,true);
        if(model_top->rowCount() > 0)
        {
            ui->tableView->tableView->selectRow(0);
            ui->tableView->updateSum(4);
            ui->tableView->updateSum(5);
            ui->tableView->updateSum(6);
            ui->tableView->updateSum(7);
            ui->tableView->updateSum(8);
            ui->tableView->updateSum(9);
            ui->tableView->updateSum(10);
            ui->tableView->updateSum(11);
            ui->tableView->updateSum(12);
            ui->tableView->updateSum(13);
        }
        //菜品消费合计
        model_left->setQuery(QString("SELECT a.ch_tableno,a.ch_dishno,b.vch_dishname,c.vch_unitname,a.num_price,a.num_num,a.num_back,(num_num - num_back) as tuishuliang,a.int_discount,(a.num_price * (a.num_num - a.num_back) * a.int_discount * 0.01) as jine,b.int_rate,a.vch_print_memo,a.vch_operID,a.dt_operdate,a.ch_printflag,a.ch_suitflag,a.ch_specialflag,a.ch_presentflag,a.num_price_add,a.vch_back_operID,a.dt_back_operdate,a.vch_dis_operID,a.dt_dis_operdate, a.ch_suitno,a.vch_pre_operID,a.dt_pre_operdate,a.ch_togono,a.ch_payno  FROM cey_bt_dish b, cey_u_orderdish a ,cey_bt_unit c  WHERE (c.ch_unitno=b.ch_unitno) and(a.ch_dishno = b.ch_dishno)and( a.ch_payno = '%1')").arg(model_top->record(0).value(1).toString()));
        ui->tableView_2->tableView->setModel(model_left);
        ui->tableView_2->tableView->setColumnHidden(18,true);
        ui->tableView_2->tableView->setColumnHidden(19,true);
        ui->tableView_2->tableView->setColumnHidden(20,true);
        ui->tableView_2->tableView->setColumnHidden(21,true);
        ui->tableView_2->tableView->setColumnHidden(22,true);
        ui->tableView_2->tableView->setColumnHidden(23,true);
        ui->tableView_2->tableView->setColumnHidden(24,true);
        ui->tableView_2->tableView->setColumnHidden(25,true);
        ui->tableView_2->tableView->setColumnHidden(26,true);
        ui->tableView_2->tableView->setColumnHidden(27,true);
        if(model_left->rowCount() > 0)
        {
            ui->tableView_2->tableView->selectRow(0);
            ui->tableView_2->updateSum(4);
            ui->tableView_2->updateSum(5);
            ui->tableView_2->updateSum(6);
            ui->tableView_2->updateSum(7);
            ui->tableView_2->updateSum(9);
            ui->tableView_2->updateSum(10);
        }
        //消费明细表
        model_right->setQuery(QString("SELECT a.ch_payno,b.vch_paymodename,a.num_payamount,a.num_change,a.num_realamount,a.num_face,a.vch_voucherno,a.vch_memo,a.vch_operID,a.dt_operdate,a.ch_paymodeno  FROM cey_u_checkout_detail a ,cey_bt_paymode b where a.ch_paymodeno = b.ch_paymodeno and a.ch_payno = '%1'").arg(model_top->record(0).value(1).toString()));
        ui->tableView_3->tableView->setColumnHidden(7,true);
        ui->tableView_3->tableView->setColumnHidden(8,true);
        ui->tableView_3->tableView->setColumnHidden(9,true);
        ui->tableView_3->tableView->setColumnHidden(10,true);
        if(model_right->rowCount() > 0)
        {
            ui->tableView_3->updateSum(2);
            ui->tableView_3->updateSum(3);
            ui->tableView_3->updateSum(4);
            ui->tableView_3->updateSum(5);
            ui->tableView_3->tableView->selectRow(0);
        }
    }
    else
    {
        ui->pushButton_4->setEnabled(false);
        model_top->setQuery(QString("SELECT ch_state,ch_payno, ch_billno,IFNULL((Select vch_tablename from cey_bt_table where ch_tableno = cey_u_checkout_master.ch_tableno),'Q000'),(Select sum(int_person) from cey_u_table where ch_payno = cey_u_checkout_master.ch_payno),(num_cost - num_blotout   + num_rate + num_lowcost + num_service - num_present) as shishou , (num_cost) as yingshou ,num_cost,num_rate,num_discount,num_present,num_blotout,num_service,num_lowcost,vch_operID,dt_operdate,case when ch_state = 'Y' then IFNULL((select sum(cey_u_checkout_detail.num_payamount) from cey_u_checkout_detail where cey_u_checkout_detail.ch_payno = cey_u_checkout_master.ch_payno and cey_u_checkout_detail.ch_paymodeno = 'YY'),0.00) else 0 end,case when ch_state = 'Y' then IFNULL((select sum(cey_u_checkout_detail.num_payamount) from cey_u_checkout_detail where cey_u_checkout_detail.ch_payno = cey_u_checkout_master.ch_payno and cey_u_checkout_detail.ch_paymodeno = 'ZZ'),0.00) else 0 end,ch_state,IFNULL((select vch_memo from cey_u_table where ch_tableno = cey_u_checkout_master.ch_tableno and ch_billno = cey_u_checkout_master.ch_billno and ch_payno = cey_u_checkout_master.ch_payno), '') FROM cey_u_checkout_master Where(dt_operdate >= '%1')and( dt_operdate <= '%2')and ch_state = 'P' order by ch_payno").arg(date1).arg(date2));
        ui->tableView->tableView->setColumnHidden(16,true);
        ui->tableView->tableView->setColumnHidden(17,true);
        ui->tableView->tableView->setColumnHidden(18,true);
        if(model_top->rowCount() > 0)
        {
            ui->tableView->tableView->selectRow(0);
            ui->tableView->updateSum(4);
            ui->tableView->updateSum(5);
            ui->tableView->updateSum(6);
            ui->tableView->updateSum(7);
            ui->tableView->updateSum(8);
            ui->tableView->updateSum(9);
            ui->tableView->updateSum(10);
            ui->tableView->updateSum(11);
            ui->tableView->updateSum(12);
            ui->tableView->updateSum(13);
        }
        //菜品消费合计
        model_left->setQuery(QString("SELECT a.ch_tableno,a.ch_dishno,b.vch_dishname,c.vch_unitname,a.num_price,a.num_num,a.num_back,(num_num - num_back) as tuishuliang,a.int_discount,(a.num_price * (a.num_num - a.num_back) * a.int_discount * 0.01) as jine,b.int_rate,a.vch_print_memo,a.vch_operID,a.dt_operdate,a.ch_printflag,a.ch_suitflag,a.ch_specialflag,a.ch_presentflag,a.num_price_add,a.vch_back_operID,a.dt_back_operdate,a.vch_dis_operID,a.dt_dis_operdate, a.ch_suitno,a.vch_pre_operID,a.dt_pre_operdate,a.ch_togono,a.ch_payno  FROM cey_bt_dish b, cey_u_orderdish a ,cey_bt_unit c  WHERE (c.ch_unitno=b.ch_unitno) and(a.ch_dishno = b.ch_dishno)and( a.ch_payno = '%1')").arg(model_top->record(0).value(1).toString()));
        ui->tableView_2->tableView->setModel(model_left);
        ui->tableView_2->tableView->setColumnHidden(18,true);
        ui->tableView_2->tableView->setColumnHidden(19,true);
        ui->tableView_2->tableView->setColumnHidden(20,true);
        ui->tableView_2->tableView->setColumnHidden(21,true);
        ui->tableView_2->tableView->setColumnHidden(22,true);
        ui->tableView_2->tableView->setColumnHidden(23,true);
        ui->tableView_2->tableView->setColumnHidden(24,true);
        ui->tableView_2->tableView->setColumnHidden(25,true);
        ui->tableView_2->tableView->setColumnHidden(26,true);
        ui->tableView_2->tableView->setColumnHidden(27,true);
        if(model_left->rowCount() > 0)
        {
            ui->tableView_2->tableView->selectRow(0);
            ui->tableView_2->updateSum(4);
            ui->tableView_2->updateSum(5);
            ui->tableView_2->updateSum(6);
            ui->tableView_2->updateSum(7);
            ui->tableView_2->updateSum(9);
            ui->tableView_2->updateSum(10);
        }
        //消费明细表
        model_right->setQuery(QString("SELECT a.ch_payno,b.vch_paymodename,a.num_payamount,a.num_change,a.num_realamount,a.num_face,a.vch_voucherno,a.vch_memo,a.vch_operID,a.dt_operdate,a.ch_paymodeno  FROM cey_u_checkout_detail a ,cey_bt_paymode b where a.ch_paymodeno = b.ch_paymodeno and a.ch_payno = '%1'").arg(model_top->record(0).value(1).toString()));
        ui->tableView_3->tableView->setColumnHidden(7,true);
        ui->tableView_3->tableView->setColumnHidden(8,true);
        ui->tableView_3->tableView->setColumnHidden(9,true);
        ui->tableView_3->tableView->setColumnHidden(10,true);
        if(model_right->rowCount() > 0)
        {
            ui->tableView_3->updateSum(2);
            ui->tableView_3->updateSum(3);
            ui->tableView_3->updateSum(4);
            ui->tableView_3->updateSum(5);
            ui->tableView_3->tableView->selectRow(0);
        }
    }
}

void business_documents_query::on_pushButton_5_clicked()//打印
{

    int row=ui->tableView->tableView->currentIndex().row();
    QString danhao;
    if(row>=0){
        danhao=model_top->record(row).value(1).toString();//PXXX
        QFile file(public_sql::fastpay_printdata_dir+ "/"+danhao);
        if(file.open(QFile::ReadOnly)){
            QByteArray ba = file.readAll();
            blockIns printdata;
            QDataStream in(&ba, QIODevice::ReadOnly);    // read the data serialized from the file
            while(!in.atEnd()){
                pieceIns pins;
                in>>pins.first;
                in>>pins.second;
                printdata.append(pins);
            }
            if(printdata.count() > 0){
                printdata[0].second=backheader::printer->drawText("", tr("付款账单(补打)")).second;
            }
            backheader::asyn_dataprint(printdata);
        }
    }
    return;
    //    {
    //        QByteArray byte;
    //        QDataStream in(&byte, QIODevice::WriteOnly);
    //        foreach(pieceIns ins, b){
    //            in<<ins.first;
    //            in<<ins.second;
    //        }
    //        QFile file(public_sql::fastpay_printdata_dir+"/"+wdanhao);
    //        file.open(QFile::WriteOnly);
    //        file.write(byte);
    //        file.close();
    //    }
    //    int row = ui->tableView->tableView->currentIndex().row();
    //    if(row <= -1)
    //    {
    //        lds_messagebox::warning(this, tr("消息提示"),tr("请选择一条要补打的数据!"));

    //        return;
    //    }
    //    QString title = tr("补打信息");
    //    QByteArray content;

    //    QSettings *configIniRead = new QSettings(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    //    QString print_type = configIniRead->value("system_setting/dyjjiekouleixing").toString();
    //    if(print_type != "1")//58打印机
    //    {
    //        QString str;
    //        str = tr("付款号:") + model_top->record(row).value(1).toString() + tr("    ");
    //        content.append(str);
    //        str = tr("人数:") + model_top->record(row).value(4).toString() + "\n";
    //        content.append(str);
    //        str = tr("时间:") + model_top->record(row).value(15).toString() + "\n";
    //        content.append(str);
    //        str = tr("工号:") + model_top->record(row).value(14).toString() + tr("    ");
    //        content.append(str);
    //        if(model_top->record(row).value(14).toString() == "0000")
    //        {
    //            str = tr("服务员名称:system\n");
    //            content.append(str);
    //        }
    //        else
    //        {
    //            QSqlQueryModel model;
    //            model.setQuery(QString("select vch_operator from cey_sys_operator where vch_operID = '%1' ").arg(model_top->record(row).value(14).toString()));
    //            str = tr("服务员名称:") + model.record(0).value(0).toString().trimmed() + "\n";
    //            content.append(str);
    //        }

    //        str = tr("餐桌:Q000\n");
    //        content.append(str);

    //        content.append(tr("------------------------------\n"));
    //        content.append(tr("品名      数量   价格    金额\n"));
    //        content.append(tr("------------------------------\n"));
    //        for(int i = 0; i < model_left->rowCount(); i++)
    //        {
    //            QString str;
    //            str = QString::number(i + 1) + tr(".");
    //            if(str.length() < 3)
    //            {
    //                str.append(tr(" "));
    //            }
    //            content.append(str);

    //            if(model_left->record(i).value(15).toString() == "P")
    //            {
    //                str = tr("套)");
    //                content.append(str);
    //            }
    //            if(model_left->record(i).value(15).toString() == tr("Y"))
    //            {
    //                str = tr("细)");
    //                content.append(str);
    //            }
    //            if(model_left->record(i).value(16).toString() == tr("Y"))
    //            {
    //                str = tr("特)");
    //                content.append(str);
    //            }
    //            if(model_left->record(i).value(17).toString() == tr("Y"))
    //            {
    //                str = tr("赠)");
    //                content.append(str);
    //            }

    //            str = model_left->record(i).value(2).toString() + "\n";
    //            content.append(str);

    //            str = model_left->record(i).value(5).toString();
    //            if(str.length() < 10)
    //            {
    //                int a = 10 - str.length();
    //                for(int k = 0; k < a; k++)
    //                {
    //                    str.append(" ");
    //                }
    //            }
    //            content.append(str);

    //            str = model_left->record(i).value(4).toString();
    //            if(str.length() < 10)
    //            {
    //                int a = 10 - str.length();
    //                for(int k = 0; k < a; k++)
    //                {
    //                    str.append(" ");
    //                }
    //            }
    //            content.append(str);

    //            str = model_left->record(i).value(9).toString();
    //            content.append(str);
    //            content.append("\n");
    //        }
    //        content.append(tr("------------------------------\n"));
    //        str = tr("消费:") + model_top->record(row).value(7).toString();
    //        int count = 2;
    //        int count_n = 0;
    //        if(str.length() < 15)
    //        {
    //            int b = 15- str.length();
    //            for(int k = 0; k < b; k++)
    //            {
    //                str.append(tr(" "));
    //            }
    //        }
    //        count_n++;
    //        content.append(str);
    //        if(count_n == count)
    //        {
    //            count_n = 0;
    //            content.append("\n");
    //        }

    //        if(model_top->record(row).value(9).toString().toFloat() > 0)
    //        {
    //            str = tr("折扣:") + model_top->record(row).value(9).toString();
    //            if(str.length() < 15)
    //            {
    //                int b = 15- str.length();
    //                for(int k = 0; k < b; k++)
    //                {
    //                    str.append(tr(" "));
    //                }
    //            }
    //            count_n++;
    //            content.append(str);
    //            if(count_n == count)
    //            {
    //                count_n = 0;
    //                content.append("\n");
    //            }
    //        }

    //        if(model_top->record(row).value(10).toString().toFloat() > 0)
    //        {
    //            str = tr("赠送:") + model_top->record(row).value(10).toString();
    //            if(str.length() < 15)
    //            {
    //                int b = 15- str.length();
    //                for(int k = 0; k < b; k++)
    //                {
    //                    str.append(tr(" "));
    //                }
    //            }
    //            count_n++;
    //            content.append(str);
    //            if(count_n == count)
    //            {
    //                count_n = 0;
    //                content.append("\n");
    //            }
    //        }

    //        if(model_top->record(row).value(11).toString().toFloat() > 0)
    //        {
    //            str = tr("抹零:") + model_top->record(row).value(11).toString();
    //            if(str.length() < 15)
    //            {
    //                int b = 15- str.length();
    //                for(int k = 0; k < b; k++)
    //                {
    //                    str.append(tr(" "));
    //                }
    //            }
    //            count_n++;
    //            content.append(str);
    //            if(count_n == count)
    //            {
    //                count_n = 0;
    //                content.append("\n");
    //            }
    //        }

    //        if(model_top->record(row).value(8).toString().toFloat() > 0)
    //        {
    //            str = tr("税收:") +model_top->record(row).value(8).toString();
    //            if(str.length() < 15)
    //            {
    //                int b = 15- str.length();
    //                for(int k = 0; k < b; k++)
    //                {
    //                    str.append(tr(" "));
    //                }
    //            }
    //            count_n++;
    //            content.append(str);
    //            if(count_n == count)
    //            {
    //                count_n = 0;
    //                content.append("\n");
    //            }
    //        }

    //        if(model_top->record(row).value(6).toString().toFloat() > 0)
    //        {
    //            str = tr("应收:") + model_top->record(row).value(6).toString();
    //            if(str.length() < 15)
    //            {
    //                int b = 15- str.length();
    //                for(int k = 0; k < b; k++)
    //                {
    //                    str.append(tr(" "));
    //                }
    //            }
    //            count_n++;
    //            content.append(str);
    //            if(count_n == count)
    //            {
    //                count_n = 0;
    //                content.append("\n");
    //            }
    //        }
    //    }
    //    else//80打印机
    //    {
    //        QString str;
    //        str = tr("付款号:") + model_top->record(row).value(1).toString() + tr("    ");
    //        content.append(str);
    //        str = tr("人数:") + model_top->record(row).value(5).toString() + "\n";
    //        content.append(str);
    //        str = tr("时间:") + model_top->record(row).value(15).toString() + "\n";
    //        content.append(str);
    //        str = tr("工号:") + model_top->record(row).value(14).toString() + tr("    ");
    //        content.append(str);
    //        if(model_top->record(row).value(14).toString() == "0000")
    //        {
    //            str = tr("服务员名称:system\n");
    //            content.append(str);
    //        }
    //        else
    //        {
    //            QSqlQueryModel model;
    //            model.setQuery(QString("select vch_operator from cey_sys_operator where vch_operID = '%1' ").arg(model_top->record(row).value(14).toString()));
    //            str = tr("服务员名称:") + model.record(0).value(0).toString().trimmed() + "\n";
    //            content.append(str);
    //        }

    //        str = tr("餐桌:Q000\n");
    //        content.append(str);

    //        content.append(tr("------------------------\n"));
    //        content.append(tr("   品名              数量      价格      金额\n"));
    //        content.append(tr("------------------------\n"));
    //        for(int i = 0; i < model_left->rowCount(); i++)
    //        {
    //            QString str;
    //            str = QString::number(i + 1) + tr(".");
    //            if(str.length() < 3)
    //            {
    //                str.append(tr(" "));
    //            }
    //            content.append(str);

    //            if(model_left->record(i).value(15).toString() == "P")
    //            {
    //                str = tr("套)");
    //                content.append(str);
    //            }
    //            else if(model_left->record(i).value(15).toString() == tr("Y"))
    //            {
    //                str = tr("细)");
    //                content.append(str);
    //            }
    //            else if(model_left->record(i).value(16).toString() == tr("Y"))
    //            {
    //                str = tr("特)");
    //                content.append(str);
    //            }
    //            else if(model_left->record(i).value(17).toString() == tr("Y"))
    //            {
    //                str = tr("赠)");
    //                content.append(str);
    //            }
    //            else
    //            {
    //                content.append("   ");
    //            }

    //            str = model_left->record(i).value(2).toString();
    //            if(str.length() < 10)
    //            {
    //                int a = 15 - str.length() * 2;
    //                for(int k = 0; k < a; k++)
    //                {
    //                    str.append(" ");
    //                }
    //            }
    //            content.append(str);

    //            str = model_left->record(i).value(5).toString();
    //            if(str.length() < 10)
    //            {
    //                int a = 10 - str.length();
    //                for(int k = 0; k < a; k++)
    //                {
    //                    str.append(" ");
    //                }
    //            }
    //            content.append(str);

    //            str = model_left->record(i).value(4).toString();
    //            if(str.length() < 10)
    //            {
    //                int a = 10 - str.length();
    //                for(int k = 0; k < a; k++)
    //                {
    //                    str.append(" ");
    //                }
    //            }
    //            content.append(str);

    //            str = model_left->record(i).value(9).toString();
    //            content.append(str);
    //            content.append("\n");
    //        }
    //        content.append(tr("------------------------\n"));
    //        str = tr("消费:") + model_top->record(row).value(7).toString();
    //        int count = 3;
    //        int count_n = 0;
    //        if(str.length() < 15)
    //        {
    //            int b = 15- str.length();
    //            for(int k = 0; k < b; k++)
    //            {
    //                str.append(tr(" "));
    //            }
    //        }
    //        count_n++;
    //        content.append(str);
    //        if(count_n == count)
    //        {
    //            count_n = 0;
    //            content.append("\n");
    //        }

    //        if(model_top->record(row).value(9).toString().toFloat() > 0)
    //        {
    //            str = tr("折扣:") + model_top->record(row).value(9).toString();
    //            if(str.length() < 15)
    //            {
    //                int b = 15- str.length();
    //                for(int k = 0; k < b; k++)
    //                {
    //                    str.append(tr(" "));
    //                }
    //            }
    //            count_n++;
    //            content.append(str);
    //            if(count_n == count)
    //            {
    //                count_n = 0;
    //                content.append("\n");
    //            }
    //        }

    //        if(model_top->record(row).value(10).toString().toFloat() > 0)
    //        {
    //            str = tr("赠送:") + model_top->record(row).value(10).toString();
    //            if(str.length() < 15)
    //            {
    //                int b = 15- str.length();
    //                for(int k = 0; k < b; k++)
    //                {
    //                    str.append(tr(" "));
    //                }
    //            }
    //            count_n++;
    //            content.append(str);
    //            if(count_n == count)
    //            {
    //                count_n = 0;
    //                content.append("\n");
    //            }
    //        }

    //        if(model_top->record(row).value(11).toString().toFloat() > 0)
    //        {
    //            str = tr("抹零:") + model_top->record(row).value(11).toString();
    //            if(str.length() < 15)
    //            {
    //                int b = 15- str.length();
    //                for(int k = 0; k < b; k++)
    //                {
    //                    str.append(tr(" "));
    //                }
    //            }
    //            count_n++;
    //            content.append(str);
    //            if(count_n == count)
    //            {
    //                count_n = 0;
    //                content.append("\n");
    //            }
    //        }

    //        if(model_top->record(row).value(8).toString().toFloat() > 0)
    //        {
    //            str = tr("税收:") + model_top->record(row).value(8).toString();
    //            int count = 2;
    //            int count_n = 0;
    //            if(str.length() < 15)
    //            {
    //                int b = 15- str.length();
    //                for(int k = 0; k < b; k++)
    //                {
    //                    str.append(tr(" "));
    //                }
    //            }
    //            count_n++;
    //            content.append(str);
    //            if(count_n == count)
    //            {
    //                count_n = 0;
    //                content.append("\n");
    //            }
    //        }

    //        if(model_top->record(row).value(6).toString().toFloat() > 0)
    //        {
    //            str = tr("应收:") + model_top->record(row).value(6).toString();
    //            if(str.length() < 15)
    //            {
    //                int b = 15- str.length();
    //                for(int k = 0; k < b; k++)
    //                {
    //                    str.append(tr(" "));
    //                }
    //            }
    //            count_n++;
    //            content.append(str);
    //            if(count_n == count)
    //            {
    //                count_n = 0;
    //                content.append("\n");
    //            }
    //        }
    //    }


    //    //    printer = new public_mainprinter();
    //    blockIns b;
    //    QString path;
    //    QString type = configIniRead->value("system_setting/dyjjiekouleixing").toString();
    //    if(type == tr("串口打印") || type == tr("USB打印"))
    //    {
    //        path = configIniRead->value("system_setting/dayinjiduankou").toString();
    //    }
    //    else
    //    {
    //        path = configIniRead->value("system_setting/dayinjiip").toString() + ":9100";
    //    }
    //    // path = "192.168.1.172:9100";
    //    b+=backheader::printer->drawText("1B40 1B6101 1C5701",title);//居中不放大
    //    b += backheader::printer->drawText("1B40,1C5700", content, Qt::AlignLeft);
    //    b += backheader::printer->drawText("1B6405 1D5601", "");//走纸5x， 切纸
    //    b=backheader::printer->setPrintCount(b, 1);
    //    //    printer->asynchPrint(b, path);
    //    backheader::asyn_dataprint(b);

    //    //    connect(printer,SIGNAL(errorString(QString)),this,SLOT(print_lost(QString)));
}

void business_documents_query::print_lost(const QString &str)
{
    ;
    lds_messagebox::warning(this, tr("消息提示"),str);

}
