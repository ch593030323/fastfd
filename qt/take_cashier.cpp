#include "take_cashier.h"
#include "ui_take_cashier.h"
#include <QStandardItem>
#include "lds_tableview_delegate_check.h"
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QDateTime>
#include "lds_messagebox.h"
#include "n_func.h"
#include "fast_order_2.h"
#include "lds_messagebox.h"
#include "waimai_pay.h"
#include <QSettings>
#include "public_mainprinter.h"
#include "backheader.h"
#include "lds_messagebox.h"

int take_cashier::row;
SqlQueryModel_Linenum1 *take_cashier::model_top;
SqlQueryModel_Linenum1 *take_cashier::model_bottom;

take_cashier::take_cashier(fast_order_2 *parent) :
    QDialog(parent),
    ui(new Ui::take_cashier),
    _parent(parent)
{
    ui->setupUi(this);
    //this->setWindowFlags(Qt::FramelessWindowHint|Qt::Dialog);this->setStyleSheet("QDialog{border:2px solid gray}");

    init_control();
    row = 0;
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->tableView_2->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_2->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setColumnWidth(1, 200);
    ui->tableView->setColumnHidden(0, true);

    connect(ui->tableView,SIGNAL(clicked(QModelIndex)),this,SLOT(update_quitbutton(QModelIndex)));
    update_quitbutton(ui->tableView->currentIndex());

}

take_cashier::~take_cashier()
{
    delete ui;
}

void take_cashier::init_control()
{
    QStringList labels;
    labels<<tr("行号")<<tr("单号") << tr("付款方式")<<tr("消费")<<tr("税收")<<tr("折扣")<<tr("赠送")
         <<tr("抹零")<<tr("应收")<<tr("订餐人")<<tr("地址")<<tr("电话") << "sdsf" <<tr("送餐人");
    model_top = new SqlQueryModel_Linenum1();
    model_top->setQuery(QString("SELECT 'id' = 1, cey_u_togo.ch_togono ,cey_u_togo.ch_payno ,cey_u_togo.num_cost, cey_u_togo.num_rate,cey_u_togo.num_discount ,cey_u_togo.num_present ,cey_u_togo.num_blotout ,cey_u_togo.num_cost + cey_u_togo.num_rate -cey_u_togo.num_discount  - cey_u_togo.num_present -  cey_u_togo.num_blotout, cey_u_togo.vch_linkman,cey_u_togo.vch_address  ,cey_u_togo.vch_tel ,cey_u_togo.ch_state ,cey_u_togo.vch_operID ,cey_u_togo.vch_waiter ,cey_u_togo.dt_operdate,cey_u_togo.ch_billno  FROM cey_u_togo  WHERE(cey_u_togo.ch_state = 'N')  "));
    qDebug() << model_top->query().lastQuery();

    model_top->setlist(labels);
    ui->tableView->setModel(model_top);

    ui->tableView->setColumnHidden(2,true);
    ui->tableView->setColumnHidden(12,true);
    ui->tableView->setColumnHidden(14,true);
    ui->tableView->setColumnHidden(15,true);
    ui->tableView->setColumnHidden(16,true);
    ui->tableView->selectRow(0);

    //    lds_tableview_delegate_check * delegate = new lds_tableview_delegate_check(16,ui->tableView_2);//第6,9行为点菜行号，不显示,在折扣中有用
    //    ui->tableView_2->setItemDelegate(delegate);
    //    ui->tableView_2->verticalHeader()->hide();
    //    ui->tableView_2->horizontalHeader()->setStyleSheet("QHeaderView::section{background-color: qlineargradient(spread:reflect, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(255, 255, 74, 255), stop:1 rgba(255, 255,21, 255));color:black;height:35px;border-radius:0px;font-size:10pt;}");
    //    ui->tableView_2->setSelectionBehavior(QAbstractItemView::SelectRows);
    //    //    ui->tableView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //    //    ui->tableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //    ui->tableView_2->verticalHeader()->setDefaultSectionSize(40);
    //    //ui->tableView_2->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置属性为只读

    //#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
    //    ui->tableView_2->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents );//文字自适应表格
    //#else
    //    ui->tableView_2->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents );//文字自适应表格
    //#endif
    QStringList labels_1;
    labels_1<< tr("a") << tr("b") << tr("c") << tr("行号")<<tr("品号")<<tr("品名")<<tr("单位")<<tr("价格")<<tr("数量")
            <<tr("折扣")<<tr("税率")<<tr("金额")<<tr("特殊做法")<<tr("点菜人")<<tr("时间")
           <<tr("厨打")<<tr("套菜")<<tr("特价")<<tr("赠送");
    model_bottom = new SqlQueryModel_Linenum1();
    model_bottom->setQuery(QString("SELECT a.int_flowID,a.ch_billno,a.ch_tableno,a.int_id,a.ch_dishno,b.vch_dishname,c.vch_unitname,a.num_price,a.num_num,a.int_discount,b.int_rate,a.num_price * a.num_num - (100 - a.int_discount) * 0.01 +  a.num_price * a.num_num *b.int_rate *0.01,   a.vch_print_memo,a.vch_operID,a.dt_operdate,a.ch_printflag,a.ch_suitflag,a.ch_specialflag,a.ch_presentflag,a.num_back,a.vch_back_operID,a.dt_back_operdate,a.num_price_org,a.num_price_add,a.vch_dis_operID,a.dt_dis_operdate,a.vch_memberno,a.ch_consult,a.ch_suitno,a.vch_pre_operID,a.dt_pre_operdate,a.ch_togono,a.ch_payno , e.ch_labelprint_flag FROM cey_bt_dish_subtype e, cey_bt_unit c, cey_bt_dish b,cey_u_orderdish a WHERE b.ch_unitno = c.ch_unitno and a.ch_dishno = b.ch_dishno and  a.ch_billno='%1' and a.ch_tableno = 'Q000'  and e.ch_sub_typeno = b.ch_sub_typeno").arg(model_top->record(0).value(16).toString()));

    model_bottom->setlist(labels_1);
    ui->tableView_2->setModel(model_bottom);
    ui->tableView_2->setColumnHidden(0,true);
    ui->tableView_2->setColumnHidden(1,true);
    ui->tableView_2->setColumnHidden(2,true);
    // ui->tableView_2->setColumnHidden(12,true);
    // ui->tableView_2->setColumnHidden(18,true);
    ui->tableView_2->setColumnHidden(19,true);
    ui->tableView_2->setColumnHidden(20,true);
    ui->tableView_2->setColumnHidden(21,true);
    ui->tableView_2->setColumnHidden(22,true);
    ui->tableView_2->setColumnHidden(23,true);
    ui->tableView_2->setColumnHidden(24,true);
    ui->tableView_2->setColumnHidden(25,true);
    ui->tableView_2->setColumnHidden(26,true);
    ui->tableView_2->setColumnHidden(27,true);
    ui->tableView_2->setColumnHidden(28,true);
    ui->tableView_2->setColumnHidden(29,true);
    ui->tableView_2->setColumnHidden(30,true);
    ui->tableView_2->setColumnHidden(31,true);
    ui->tableView_2->setColumnHidden(32,true);
    ui->tableView_2->setColumnHidden(33,true);
    ui->tableView_2->selectRow(0);

}

void take_cashier::init_fastmodel()
{
    //    fast_order_2::model_dalei.setQuery(QString("SELECT  * FROM cey_bt_dish_type"));
    //    for(int i = 0; i < fast_order_2::model_dalei.rowCount(); i++)
    //    {
    //        if(i == fast_order_2::num_da)
    //        {
    //            fast_order_2::dalei = fast_order_2::model_dalei.record(i).value(0).toString();
    //        }
    //    }

    //    // model_xiaolei->clear();
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
}


void take_cashier::on_pushButton_5_clicked()//退出
{
    this->close();
}

void take_cashier::on_tableView_clicked(const QModelIndex &index)
{
    row = ui->tableView->currentIndex().row();
    int row1 = ui->tableView->currentIndex().row();
    if(row1 <= -1)
    {
        return;
    }
    model_bottom->clear();
    model_bottom->setQuery(QString("SELECT a.int_flowID,a.ch_billno,a.ch_tableno,a.int_id,a.ch_dishno,b.vch_dishname,c.vch_unitname,a.num_price,a.num_num,a.int_discount,b.int_rate,a.num_price * a.num_num - (100 - a.int_discount) * 0.01 +  a.num_price * a.num_num *b.int_rate *0.01,   a.vch_print_memo,a.vch_operID,a.dt_operdate,a.ch_printflag,a.ch_suitflag,a.ch_specialflag,a.ch_presentflag,a.num_back,a.vch_back_operID,a.dt_back_operdate,a.num_price_org,a.num_price_add,a.vch_dis_operID,a.dt_dis_operdate,a.vch_memberno,a.ch_consult,a.ch_suitno,a.vch_pre_operID,a.dt_pre_operdate,a.ch_togono,a.ch_payno , e.ch_labelprint_flag FROM cey_bt_dish_subtype e, cey_bt_unit c, cey_bt_dish b,cey_u_orderdish a WHERE b.ch_unitno = c.ch_unitno and a.ch_dishno = b.ch_dishno and  a.ch_billno='%1' and a.ch_tableno = 'Q000'  and e.ch_sub_typeno = b.ch_sub_typeno").arg(model_top->record(row1).value(16).toString()));
    ui->tableView_2->selectRow(0);

}

void take_cashier::on_pushButton_3_clicked()//
{
    QSqlQuery query;
    QSqlDatabase::database().transaction();

    int row = ui->tableView->currentIndex().row();
    if(row <= -1)
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("请选择要退货的单据!"));
        
        return;
    }

    QString waimaidanhao=model_top->record(row).value(1).toString();
    if(0==lds_messagebox::warning(this, tr("消息提示"),tr("是否确定将指定外卖退货?"),tr("确定"),tr("取消")))
    {
        QSqlQueryModel model;
        model.setQuery(QString("Select NOW()"));
        QString timer = model.record(0).value(0).toDateTime().toString("yyyy-MM-dd hh:mm:ss");

        query.prepare(QString("update cey_u_togo set ch_state ='P' ,vch_operID =:operid,dt_operdate=:time where ch_togono =:waimaihao"));
        query.bindValue(":operid",n_func::gs_operid);
        query.bindValue(":time",timer);
        query.bindValue(":waimaihao",waimaidanhao);
        if(query.exec())
        {

        }
        else
        {
            QSqlDatabase::database().rollback();
            lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_u_togo数据失败!"));
            
            return;
        }

        query.prepare(QString("update cey_u_orderdish set num_back =num_num,num_num =0,vch_back_operID =:operid,dt_back_operdate=:time where ch_togono =:waimaihao;"));
        query.bindValue(":operid",n_func::gs_operid);
        query.bindValue(":time",timer);
        query.bindValue(":waimaihao",model_top->record(row).value(1).toString());
        if(query.exec())
        {

        }
        else
        {
            QSqlDatabase::database().rollback();
            lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_u_orderdish数据失败!"));
            
            return;
        }

        QSqlDatabase::database().commit();

        model_top->clear();
        model_top->setQuery(QString("SELECT 'id' = 1, cey_u_togo.ch_togono ,cey_u_togo.ch_payno ,cey_u_togo.num_cost, cey_u_togo.num_rate,cey_u_togo.num_discount ,cey_u_togo.num_present ,cey_u_togo.num_blotout ,cey_u_togo.num_cost + cey_u_togo.num_rate -cey_u_togo.num_discount  - cey_u_togo.num_present -  cey_u_togo.num_blotout, cey_u_togo.vch_linkman,cey_u_togo.vch_address  ,cey_u_togo.vch_tel ,cey_u_togo.ch_state ,cey_u_togo.vch_operID ,cey_u_togo.vch_waiter ,cey_u_togo.dt_operdate,cey_u_togo.ch_billno  FROM cey_u_togo  WHERE(cey_u_togo.ch_state = 'N')  "));
        if(row >= model_top->rowCount())
        {
            ui->tableView->selectRow(model_top->rowCount() -1);
        }
        else
        {
            ui->tableView->selectRow(row);
        }

        if(model_top->rowCount() > 0)
        {
            model_bottom->clear();
            model_bottom->setQuery(QString("SELECT a.int_flowID,a.ch_billno,a.ch_tableno,a.int_id,a.ch_dishno,b.vch_dishname,c.vch_unitname,a.num_price,a.num_num,a.int_discount,b.int_rate,a.num_price * a.num_num - (100 - a.int_discount) * 0.01 +  a.num_price * a.num_num *b.int_rate *0.01,   a.vch_print_memo,a.vch_operID,a.dt_operdate,a.ch_printflag,a.ch_suitflag,a.ch_specialflag,a.ch_presentflag,a.num_back,a.vch_back_operID,a.dt_back_operdate,a.num_price_org,a.num_price_add,a.vch_dis_operID,a.dt_dis_operdate,a.vch_memberno,a.ch_consult,a.ch_suitno,a.vch_pre_operID,a.dt_pre_operdate,a.ch_togono,a.ch_payno , e.ch_labelprint_flag FROM cey_bt_dish_subtype e, cey_bt_unit c, cey_bt_dish b,cey_u_orderdish a WHERE b.ch_unitno = c.ch_unitno and a.ch_dishno = b.ch_dishno and  a.ch_billno='%1' and a.ch_tableno = 'Q000'  and e.ch_sub_typeno = b.ch_sub_typeno").arg(model_top->record(ui->tableView->currentIndex().row()).value(16).toString()));
            ui->tableView_2->selectRow(0);
        }
        else
        {
            model_bottom->clear();
        }
        init_fastmodel();
    }
}

void take_cashier::on_pushButton_4_clicked()//付款
{
    int row = ui->tableView->currentIndex().row();
    if(row <= -1)
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("请选择要付款的单据!"));
        
        return;
    }

    QString str1 = QString().sprintf("%0.2f",model_top->record(row).value(3).toFloat());//消费
    QString str2 = QString().sprintf("%0.2f",model_top->record(row).value(4).toFloat());//税收
    QString str3 = QString().sprintf("%0.2f",model_top->record(row).value(5).toFloat());//折扣
    QString str4 = QString().sprintf("%0.2f",model_top->record(row).value(6).toFloat());//赠送
    QString str5 = QString().sprintf("%0.2f",model_top->record(row).value(7).toFloat());//抹零
    QString str6 = model_top->record(row).value(16).toString();//单号
    QString str7 = model_top->record(row).value(1).toString();//外卖号
    waimai_pay waimai(str1,str3,str4,str5,str2,str6,str7,_parent);waimai.show();
    /*waimai.exec();*/backheader::static_dialogexec(waimai, tr("快餐付款"));
    if(waimai.btn_t_f == true)
    {
        model_top->clear();
        model_top->setQuery(QString("SELECT 'id' = 1, cey_u_togo.ch_togono ,cey_u_togo.ch_payno ,cey_u_togo.num_cost, cey_u_togo.num_rate,cey_u_togo.num_discount ,cey_u_togo.num_present ,cey_u_togo.num_blotout ,cey_u_togo.num_cost + cey_u_togo.num_rate -cey_u_togo.num_discount  - cey_u_togo.num_present -  cey_u_togo.num_blotout, cey_u_togo.vch_linkman,cey_u_togo.vch_address  ,cey_u_togo.vch_tel ,cey_u_togo.ch_state ,cey_u_togo.vch_operID ,cey_u_togo.vch_waiter ,cey_u_togo.dt_operdate,cey_u_togo.ch_billno  FROM cey_u_togo  WHERE(cey_u_togo.ch_state = 'N')  "));
        if(row >= model_top->rowCount())
        {
            ui->tableView->selectRow(model_top->rowCount() -1);
        }
        else
        {
            ui->tableView->selectRow(row);
        }

        if(model_top->rowCount() > 0)
        {
            model_bottom->clear();
            model_bottom->setQuery(QString("SELECT a.int_flowID,a.ch_billno,a.ch_tableno,a.int_id,a.ch_dishno,b.vch_dishname,c.vch_unitname,a.num_price,a.num_num,a.int_discount,b.int_rate,a.num_price * a.num_num - (100 - a.int_discount) * 0.01 +  a.num_price * a.num_num *b.int_rate *0.01,   a.vch_print_memo,a.vch_operID,a.dt_operdate,a.ch_printflag,a.ch_suitflag,a.ch_specialflag,a.ch_presentflag,a.num_back,a.vch_back_operID,a.dt_back_operdate,a.num_price_org,a.num_price_add,a.vch_dis_operID,a.dt_dis_operdate,a.vch_memberno,a.ch_consult,a.ch_suitno,a.vch_pre_operID,a.dt_pre_operdate,a.ch_togono,a.ch_payno , e.ch_labelprint_flag FROM cey_bt_dish_subtype e, cey_bt_unit c, cey_bt_dish b,cey_u_orderdish a WHERE b.ch_unitno = c.ch_unitno and a.ch_dishno = b.ch_dishno and  a.ch_billno='%1' and a.ch_tableno = 'Q000'  and e.ch_sub_typeno = b.ch_sub_typeno").arg(model_top->record(ui->tableView->currentIndex().row()).value(16).toString()));
            ui->tableView_2->selectRow(0);
        }
        else
        {
            model_bottom->clear();
        }
    }
}

void take_cashier::update_quitbutton(const QModelIndex &current)
{
    qDebug() << current;
    if(model_top->data(model_top->index(current.row(), 1)).toString().startsWith("T")){//若是微信外卖
        ui->pushButton_3->hide();
    } else {
        ui->pushButton_3->show();
    }
}
