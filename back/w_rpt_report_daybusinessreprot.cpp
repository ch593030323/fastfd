#include "w_rpt_report_daybusinessreprot.h"
//日营业情况统计
#include "ui_w_rpt_report_daybusinessreprot_dialog.h"
#include "lds_messagebox.h"
#include <QVariant>
#include <QStandardItemModel>
#include <QSettings>
#include "n_func.h"

w_rpt_report_daybusinessreprot::w_rpt_report_daybusinessreprot(QWidget *parent) :
    QDialog(parent), ui(new Ui_w_rpt_report_daybusinessreprot_Dialog)
{
    ui->setupUi(this);
    ui->frame->setFixedWidth(w_rpt_report::frame_left_fixed_width);
    //! 开始结束时间
    QStandardItemModel *datestart=new QStandardItemModel;
    QSqlQuery query;
    query.exec("select dt_date, dt_begin,dt_end from cey_u_day_sumup");
    while(query.next()){
        QList<QStandardItem *> items;
        items << new QStandardItem(query.record().value("dt_date").toDateTime().toString("yyyy-MM-dd"));
        items << new QStandardItem(query.record().value("dt_begin").toDateTime().toString("yyyy-MM-dd hh:mm:ss"));
        items << new QStandardItem(query.record().value("dt_end").toDateTime().toString("yyyy-MM-dd hh:mm:ss"));
        datestart->appendRow(items);
    }
    datestart->setHorizontalHeaderLabels(QStringList() << tr("营业日") << tr("起始时间") << tr("结束时间"));
    ui->comboBox_start->setModel(datestart);
    ui->comboBox_start->setMaxVisibleItems(5);
    ui->comboBox_start->setCurrentIndex(-1);
    ui->comboBox_start->setView(new QTableView);
    static_cast<QTableView *>(ui->comboBox_start->view())->setFixedWidth(500);
    static_cast<QTableView *>(ui->comboBox_start->view())->resizeColumnsToContents();

    ui->comboBox_end->setModel(datestart);
    ui->comboBox_end->setMaxVisibleItems(5);
    ui->comboBox_end->setCurrentIndex(-1);
    ui->comboBox_end->setView(new QTableView);
    static_cast<QTableView *>(ui->comboBox_end->view())->setFixedWidth(500);
    static_cast<QTableView *>(ui->comboBox_end->view())->resizeColumnsToContents();

    tab1sqlinit();

    standmodel=new datetimestandModel;

    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);ui->tableView->setModel(standmodel);
    //! toolbar
    ui->widget->setup(QStringList()<<tr("查询")<<tr("退出"), Qt::AlignTop);
    connect(ui->widget->index_widget(tr("查询")),SIGNAL(clicked()),this,SLOT(toselect()));
    connect(ui->widget->index_widget(tr("退出")),SIGNAL(clicked()),this,SLOT(toexit()));
    //    connect(ui->tableView->tableView->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(toselect()));
    //! after
    //导入
    ui->comboBox_beginmonth->addItems(QStringList() << tr("上月") << tr("当月"));
    loadbeginset();
    //上月，当月的com
    connect(ui->comboBox_beginmonth,SIGNAL(currentIndexChanged(int)),this,SLOT(updatecombegindate(int)));
    //保存日和月
    connect(ui->comboBox_beginmonth,SIGNAL(currentIndexChanged(int)),this,SLOT(savebeginset()));
    connect(ui->spinBox_beginday,SIGNAL(valueChanged(int)),this,SLOT(savebeginset()));

    ui->pushButton->setLabelText(tr(
                                "如果设置营业日大于统计月份的最大营业日，则按第二月的第一个营业日为起始日;\n"
                                "起始日期为当月，则不能大于等于16号，为上月，则不能小于等于15号;\n"
                                ));
    //tabwidget header
    ui->tabWidget->setTabText(0, tr("日营业情况统计"));

}

void w_rpt_report_daybusinessreprot::tab1sqlinit()
{

}

void w_rpt_report_daybusinessreprot::toselect()
{
    //ue_query (none) returns (none)
    QDateTime ldt_begin;
    QDateTime ldt_end;
    QDateTime ldt_begin_m;
    QDateTime ldt_end_m;
    QDateTime ldt_date;
    QDateTime ldt_mb;
    QDateTime ldt_me;
    QString ls_begin;
    QString ls_end;
    QString ls_begin_m;
    QString ls_end_m;
    QString ls_flag;
    long ll_begin;
    QString ls_itemtype;
    QString ls_item;
    QString ls_rate;
    QString ls_date;
    QString ls_date_end;
    double ldec_profit;
    double ldec_profit_m;
    long ll_count;
    long ll_count_month;
    long ll_row;
    long ll_table;
    long ll_person;
    long ll_minute;
    long ll_table_m;
    long ll_person_m;
    long ll_minute_m;
    double ldec_cost;
    double ldec_num;
    double ldec_discount;
    double ldec_present;
    double ldec_service;
    double ldec_lowcost;
    double ldec_amount;
    double ldec_cost_m;
    double ldec_num_m;
    double ldec_discount_m;
    double ldec_present_m;
    double ldec_service_m;
    double ldec_lowcost_m;
    double ldec_amount_m;
    double ldec_blotout;
    double ldec_free;
    double ldec_blotout_m;
    double ldec_free_m;
    double ldec_type;
    double ldec_type_m;
    QString ls_typeno;
    QString ls_typename;
    //    DECLARE c_series CURSOR FOR
    //    Select ch_dish_typeno , vch_dish_typename
    //    from cey_bt_dish_type
    //    order by ch_dish_typeno
    //    USING sqlca;
    QString ls_paymodeno;
    QString ls_paymodename;
    QString ls_incomeflag;
    double ldec_total;
    double ldec_total_m;
    double ldec_total1;
    double ldec_total1_m;
    //    DECLARE c_pay CURSOR FOR
    //    Select a.ch_paymodeno , IFNULL(c.vch_paymodename , a.ch_paymodeno), IFNULL(c.ch_incomeflag , 'Y'), sum(a.num_realamount )
    //    from cey_u_checkout_detail a , cey_u_checkout_master b , cey_bt_paymode c
    //    where a.ch_paymodeno *=c.ch_paymodeno and a.ch_payno =b.ch_payno and(b.ch_state ='Y')and b.dt_operdate >= :ldt_begin_m and b.dt_operdate <= :ldt_end_m
    //    group by a.ch_paymodeno , IFNULL(c.vch_paymodename , a.ch_paymodeno), IFNULL(c.ch_incomeflag , 'Y' )
    //    having sum(a.num_realamount)<> 0
    //    order by a.ch_paymodeno
    //    USING sqlca;
    double ldec_deposit_m;
    double ldec_deposit;
    QSqlQuery query, query_c_series,query_c_pay;

//    QString ls_begin;
//    QString ls_end;
   // ls_begin=n_func::uf_getdatetime("begin", QDateTime::fromString(ui->comboBox_start->currentText(), "yyyy-MM-dd")).toString("yyyy-MM-dd hh:mm:ss");
 //   ls_end=n_func::uf_getdatetime("end",  QDateTime::fromString(ui->comboBox_end->currentText(), "yyyy-MM-dd")).toString("yyyy-MM-dd hh:mm:ss");

    ls_begin = ui->comboBox_start->currentText();// .sle_1.text
    ls_end =ui->comboBox_end->currentText();// THIS.sle_2.text
    query.prepare("Select dt_begin from cey_u_day_sumup  where dt_date =:ls_begin");
    query.bindValue(":ls_begin",  ls_begin);
    query.exec();
    if(query.next()){
        ldt_begin=query.record().value("dt_begin").toDateTime();
    } else {
        lds_messagebox::warning(this, tr("消息提示"),tr("指定开始营业日不存在!"));
        return;
    }

    query.prepare("Select dt_end from cey_u_day_sumup  where dt_date =:ls_end");
    query.bindValue(":ls_end",  ls_end);
    query.exec();
    if(query.next()){
        ldt_end=query.record().value("dt_end").toDateTime();
    } else {
        lds_messagebox::warning(this, tr("消息提示"),tr("指定结束营业日不存在!"));
        return;
    }

    if(ls_begin > ls_end){
        lds_messagebox::warning(this, tr("消息提示"),tr("开始营业日不能大于结束营业日!"));
        return;
    }

    //获取营业日起始日
    ll_begin=ui->spinBox_beginday->value();
    if(ll_begin<1 || ll_begin > 31){
        ll_begin = 1;
        ls_flag = "2";
    } else {
        if(ll_begin > 0 && ll_begin < 16){
            ls_flag = "2";
        } else {
            ls_flag = "1";
        }
    }
    ldt_date=QDateTime::fromString(ls_begin, "yyyy-MM-dd");
    if(ldt_date.date().day() < ll_begin){
        ldt_mb=ldt_date.addMonths(-1);
        ldt_mb=ldt_mb.addDays(ll_begin-ldt_mb.date().day());
        ldt_me=ldt_date.addDays(ll_begin-ldt_date.date().day());
    } else {
        ldt_mb=ldt_date.addDays(ll_begin-ldt_date.date().day());
        ldt_me=ldt_date.addMonths(1);
        ldt_me=ldt_me.addDays(ll_begin-ldt_me.date().day());
    }

    query.prepare("Select Min(dt_begin), max(dt_end) from cey_u_day_sumup  where dt_date >= :ldt_mb and dt_date < :ldt_me");
    query.bindValue(":ldt_mb", ldt_mb);
    query.bindValue(":ldt_me", ldt_me);
    query.exec();
    qDebug() << query.next() << ldt_mb << ldt_me;
    ldt_begin_m=query.record().value(0).toDateTime();
    ldt_end_m=query.record().value(1).toDateTime();

    query.prepare("Select count(ch_payno) from cey_u_checkout_master  where(ch_state ='Y')and dt_operdate >= :ldt_begin and dt_operdate <= :ldt_end");
    query.bindValue(":ldt_begin",ldt_begin);
    query.bindValue(":ldt_end",ldt_end);
    query.exec();
    query.next();
    ll_count=query.record().value(0).toLongLong();

    query.prepare("Select count(distinct ch_billno)from cey_u_checkout_master  where(ch_state ='Y')and dt_operdate >= :ldt_begin_m and dt_operdate <= :ldt_end_m");
    query.bindValue(":ldt_begin_m",ldt_begin_m);
    query.bindValue(":ldt_end_m",ldt_end_m);
    query.exec();
    query.next();
    ll_count_month=query.record().value(0).toLongLong();

    QList<QStandardItem *> items;
    standmodel->removeRows(0, standmodel->rowCount());

    //营业分析======================================================================================================================================

    items.clear();
    items << new QStandardItem(itemtextspan(tr("营业分析")));
    items << new QStandardItem(tr("单据数"));
    items << new QStandardItem(QString::number(ll_count));
    items << new QStandardItem(QString::number(ll_count_month));
    items << new QStandardItem(QString().sprintf("%.02f",ll_count_month==0? 0.0: (ll_count*1.0)/(ll_count_month*1.0) * 100) +"%");
    standmodel->appendRow(items);


    query.prepare("Select count(a.ch_payno), IFNULL(sum(a.int_person), 0), ABS(IFNULL(sum(round(TIME_TO_SEC(TIMEDIFF(a.dt_operDate , b.dt_operdate))/60)),0))   from cey_u_table a , cey_u_checkout_master b  where a.ch_payno =b.ch_payno and b.ch_state ='Y' and b.dt_operdate >= :ldt_begin and b.dt_operdate <= :ldt_end");
    query.bindValue(":ldt_begin",ldt_begin);qDebug() << ldt_begin;
    query.bindValue(":ldt_end",ldt_end);qDebug() << ldt_end;
    query.exec();
    query.next();
    ll_table=query.record().value(0).toLongLong();qDebug() << ll_table;
    ll_person=query.record().value(1).toLongLong();qDebug() << ll_person;
    ll_minute=query.record().value(2).toLongLong();qDebug() << ll_minute<< "ll_minute";

    query.prepare("Select count(a.ch_payno), IFNULL(sum(a.int_person), 0),  ABS(IFNULL(sum(round(TIME_TO_SEC(TIMEDIFF(a.dt_operDate , b.dt_operdate))/60)), 0)) from cey_u_table a , cey_u_checkout_master b  where a.ch_payno =b.ch_payno and b.ch_state ='Y' and b.dt_operdate >= :ldt_begin_m and b.dt_operdate <= :ldt_end_m");
    query.bindValue(":ldt_begin_m",ldt_begin_m);qDebug() << ldt_begin_m;
    query.bindValue(":ldt_end_m",ldt_end_m);qDebug() << ldt_end_m;
    query.exec();
    query.next();
    ll_table_m=query.record().value(0).toLongLong();
    ll_person_m=query.record().value(1).toLongLong();
    ll_minute_m=query.record().value(2).toLongLong();

    if(ll_table==0){
        ll_minute=0;
    } else {
        ll_minute=(ll_minute*1.0)/(ll_table*1.0);
    }
    if(ll_table_m==0){
        ll_minute_m=0;
    } else {
        ll_minute_m=(ll_minute_m*1.0)/(ll_table_m*1.0);
    }

    items.clear();
    items << new QStandardItem(itemtextspan(tr("营业分析")));
    items << new QStandardItem(tr("桌位数"));
    items << new QStandardItem(QString::number(ll_table));
    items << new QStandardItem(QString::number(ll_table_m));
    items << new QStandardItem(QString().sprintf("%.02f",ll_table_m==0? 0.0: (ll_table*1.0)/(ll_table_m*1.0) * 100) +"%");
    standmodel->appendRow(items);

    items.clear();
    items << new QStandardItem(itemtextspan(tr("营业分析")));
    items << new QStandardItem(tr("人数"));
    items << new QStandardItem(QString::number(ll_person));
    items << new QStandardItem(QString::number(ll_person_m));
    items << new QStandardItem(QString().sprintf("%.02f",ll_person_m==0? 0.0: (ll_person*1.0)/(ll_person_m*1.0) * 100) +"%");
    standmodel->appendRow(items);

    items.clear();
    items << new QStandardItem(itemtextspan(tr("营业分析")));
    items << new QStandardItem(tr("每桌平均人数"));
    items << new QStandardItem(QString().sprintf("%.02f",ll_table==0? 0.0: (ll_person*1.0)/(ll_table*1.0)));
    items << new QStandardItem(QString().sprintf("%.02f",ll_table_m==0? 0.0: (ll_person_m*1.0)/(ll_table_m*1.0)));
    items << new QStandardItem("");
    standmodel->appendRow(items);

    items.clear();
    items << new QStandardItem(itemtextspan(tr("营业分析")));
    items << new QStandardItem(tr("每桌平均消费时间(分钟)"));
    items << new QStandardItem(QString::number(ll_minute));
    items << new QStandardItem(QString::number(ll_minute_m));
    items << new QStandardItem("");
    standmodel->appendRow(items);

    query.prepare("Select IFNULL(sum(a.num_num - a.num_back), 0)  from cey_u_orderdish a , cey_u_checkout_master b   where a.ch_payno =b.ch_payno and(b.ch_state ='Y')and b.dt_operdate >= :ldt_begin and b.dt_operdate <= :ldt_end");
    query.bindValue(":ldt_begin",ldt_begin);
    query.bindValue(":ldt_end",ldt_end);
    query.exec();
    query.next();
    ldec_num=query.record().value(0).toDouble();

    query.prepare("Select IFNULL(sum(a.num_num - a.num_back), 0) from cey_u_orderdish a , cey_u_checkout_master b   where a.ch_payno =b.ch_payno and(b.ch_state ='Y')and b.dt_operdate >= :ldt_begin and b.dt_operdate <= :ldt_end");
    query.bindValue(":ldt_begin_m",ldt_begin_m);
    query.bindValue(":ldt_end_m",ldt_end_m);
    query.exec();
    query.next();
    ldec_num_m=query.record().value(0).toDouble();

    items.clear();
    items << new QStandardItem(itemtextspan(tr("营业分析")));
    items << new QStandardItem(tr("每桌平均消费菜品数量"));
    items << new QStandardItem(QString().sprintf("%.02f" , ll_table==0? 0.0 : ldec_num / ll_table));
    items << new QStandardItem(QString().sprintf("%.02f" , ll_table_m==0? 0.0 : ldec_num_m / ll_table_m));
    items << new QStandardItem("");
    standmodel->appendRow(items);

    query.prepare("Select IFNULL(sum(num_cost), 0), IFNULL(sum(num_discount), 0), IFNULL(sum(num_present), 0), IFNULL(sum(num_service), 0), IFNULL(sum(num_lowcost), 0) from cey_u_checkout_master  where ch_state ='Y' and dt_operdate >= :ldt_begin and dt_operdate <= :ldt_end");
    query.bindValue(":ldt_begin",ldt_begin);
    query.bindValue(":ldt_end",ldt_end);
    query.exec();
    query.next();
    ldec_cost=query.record().value(0).toDouble();
    ldec_discount=query.record().value(1).toDouble();
    ldec_present=query.record().value(2).toDouble();
    ldec_service=query.record().value(3).toDouble();
    ldec_lowcost=query.record().value(4).toDouble();


    query.prepare("Select IFNULL(sum(num_cost), 0), IFNULL(sum(num_discount), 0), IFNULL(sum(num_present), 0), IFNULL(sum(num_service), 0), IFNULL(sum(num_lowcost), 0)  from cey_u_checkout_master    where ch_state ='Y' and dt_operdate >= :ldt_begin_m and dt_operdate <= :ldt_end_m");
    query.bindValue(":ldt_begin_m",ldt_begin_m);
    query.bindValue(":ldt_end_m",ldt_end_m);
    query.exec();
    query.next();
    ldec_cost_m=query.record().value(0).toDouble();
    ldec_discount_m=query.record().value(1).toDouble();
    ldec_present_m=query.record().value(2).toDouble();
    ldec_service_m=query.record().value(3).toDouble();
    ldec_lowcost_m=query.record().value(4).toDouble();

    ldec_amount = ldec_cost - ldec_present - ldec_discount + ldec_service + ldec_lowcost;
    ldec_amount_m = ldec_cost_m - ldec_present_m - ldec_discount_m + ldec_service_m + ldec_lowcost_m;
    items.clear();
    items << new QStandardItem(itemtextspan(tr("营业分析")));
    items << new QStandardItem(tr("每桌平均消费金额"));
    items << new QStandardItem(QString().sprintf("%.02f" , ll_table==0? 0.0 : ldec_amount / ll_table));
    items << new QStandardItem(QString().sprintf("%.02f" , ll_table_m==0? 0.0 : ldec_amount_m / ll_table_m));
    items << new QStandardItem("");
    standmodel->appendRow(items);

    items.clear();
    items << new QStandardItem(itemtextspan(tr("营业分析")));
    items << new QStandardItem(tr("人均消费金额"));
    items << new QStandardItem(QString().sprintf("%.02f" , ll_person==0? 0.0 : ldec_amount / ll_person));
    items << new QStandardItem(QString().sprintf("%.02f" , ll_person_m==0? 0.0 : ldec_amount_m / ll_person_m));
    items << new QStandardItem("");
    standmodel->appendRow(items);
    //营业统计======================================================================================================================================
    items.clear();
    items << new QStandardItem(itemtextspan(tr("营业统计")));
    items << new QStandardItem(tr("总消费金额"));
    items << new QStandardItem(QString().sprintf("%.02f" , ldec_cost));
    items << new QStandardItem(QString().sprintf("%.02f" , ldec_cost_m));
    items << new QStandardItem(QString().sprintf("%.02f",ldec_cost_m==0? 0.0: (ldec_cost*1.0)/(ldec_cost_m*1.0) * 100) +"%");
    standmodel->appendRow(items);

    query_c_series.exec("Select ch_dish_typeno , vch_dish_typename   from cey_bt_dish_type order by ch_dish_typeno");
    while(query_c_series.next()){//0
        ls_typeno=query_c_series.record().value("ch_dish_typeno").toString();
        ls_typename=query_c_series.record().value("vch_dish_typename").toString();

        query.prepare("Select IFNULL(sum(( a.num_num - a.num_back)*( a.num_price + a.num_price_add)) , 0)  from cey_u_orderdish a , cey_bt_dish b , cey_u_checkout_master c    where a.ch_payno =c.ch_payno and c.ch_state ='Y' and b.ch_dish_typeno =:ls_typeno and a.ch_dishno =b.ch_dishno and c.dt_operdate >= :ldt_begin and c.dt_operdate < :ldt_end");
        query.bindValue(":ls_typeno",ls_typeno);
        query.bindValue(":ldt_begin",ldt_begin);
        query.bindValue(":ldt_end",ldt_end);
        query.exec();
        query.next();
        ldec_type=query.record().value(0).toDouble();

        query.prepare("Select IFNULL(sum(( a.num_num - a.num_back)*( a.num_price + a.num_price_add)) , 0)  from cey_u_orderdish a , cey_bt_dish b , cey_u_checkout_master c    where a.ch_payno =c.ch_payno and c.ch_state ='Y' and b.ch_dish_typeno =:ls_typeno and a.ch_dishno =b.ch_dishno and c.dt_operdate >= :ldt_begin_m and c.dt_operdate < :ldt_end_m");
        query.bindValue(":ls_typeno",ls_typeno);
        query.bindValue(":ldt_begin_m",ldt_begin_m);
        query.bindValue(":ldt_end_m",ldt_end_m);
        query.exec();
        query.next();
        ldec_type_m=query.record().value(0).toDouble();

        if(ldec_type==0 && ldec_type_m==0){

        } else {
            items.clear();
            items << new QStandardItem(itemtextspan(tr("营业统计")));
            items << new QStandardItem(" **"+ls_typename);
            items << new QStandardItem(QString().sprintf("%.02f" , ldec_type));
            items << new QStandardItem(QString().sprintf("%.02f" , ldec_type_m));
            items << new QStandardItem(QString().sprintf("%.02f"  ,ldec_type_m==0? 0.0: (ldec_type*1.0)/(ldec_type_m*1.0) * 100) +"%");
            standmodel->appendRow(items);
        }
    }//0
    query_c_series.clear();


    items.clear();
    items << new QStandardItem(itemtextspan(tr("营业统计")));
    items << new QStandardItem(tr("折扣金额"));
    items << new QStandardItem(QString().sprintf("%.02f" , ldec_discount));
    items << new QStandardItem(QString().sprintf("%.02f" , ldec_discount_m));
    items << new QStandardItem(QString().sprintf("%.02f", ldec_discount_m==0? 0.0: (ldec_discount*1.0)/(ldec_discount_m*1.0) * 100) +"%");
    standmodel->appendRow(items);

    items.clear();
    items << new QStandardItem(itemtextspan(tr("营业统计")));
    items << new QStandardItem(tr("赠送金额"));
    items << new QStandardItem(QString().sprintf("%.02f" , ldec_present));
    items << new QStandardItem(QString().sprintf("%.02f" , ldec_present_m));
    items << new QStandardItem(QString().sprintf("%.02f",ldec_present_m==0? 0.0: (ldec_present*1.0)/(ldec_present_m*1.0) * 100) +"%");
    standmodel->appendRow(items);

    items.clear();
    items << new QStandardItem(itemtextspan(tr("营业统计")));
    items << new QStandardItem(tr("服务费"));
    items << new QStandardItem(QString().sprintf("%.02f" , ldec_service));
    items << new QStandardItem(QString().sprintf("%.02f" , ldec_service_m));
    items << new QStandardItem(QString().sprintf("%.02f",ldec_service_m==0? 0.0: (ldec_service*1.0)/(ldec_service_m*1.0) * 100) +"%");
    standmodel->appendRow(items);

    items.clear();
    items << new QStandardItem(itemtextspan(tr("营业统计")));
    items << new QStandardItem(tr("低消收入"));
    items << new QStandardItem(QString().sprintf("%.02f" , ldec_lowcost));
    items << new QStandardItem(QString().sprintf("%.02f" , ldec_lowcost_m));
    items << new QStandardItem(QString().sprintf("%.02f",ldec_lowcost_m==0? 0.0: (ldec_lowcost*1.0)/(ldec_lowcost_m*1.0) * 100) +"%");
    standmodel->appendRow(items);

    items.clear();
    items << new QStandardItem(itemtextspan(tr("营业统计")));
    items << new QStandardItem(tr("应收金额"));
    items << new QStandardItem(QString().sprintf("%.02f" , ldec_amount));
    items << new QStandardItem(QString().sprintf("%.02f" , ldec_amount_m));
    items << new QStandardItem(QString().sprintf("%.02f",ldec_amount_m==0? 0.0: (ldec_amount*1.0)/(ldec_amount_m*1.0) * 100) +"%");
    standmodel->appendRow(items);

    query_c_pay.prepare("Select a.ch_paymodeno , IFNULL(c.vch_paymodename , a.ch_paymodeno), IFNULL(c.ch_incomeflag , 'Y'), sum(a.num_realamount)   from cey_u_checkout_detail a , cey_u_checkout_master b , cey_bt_paymode c    where a.ch_paymodeno =c.ch_paymodeno and a.ch_payno =b.ch_payno and(b.ch_state ='Y')and b.dt_operdate >= :ldt_begin_m and b.dt_operdate <= :ldt_end_m  group by a.ch_paymodeno , IFNULL(c.vch_paymodename , a.ch_paymodeno), IFNULL(c.ch_incomeflag , 'Y')   having sum(a.num_realamount)<> 0    order by a.ch_paymodeno");
    query_c_pay.bindValue(":ldt_begin_m",ldt_begin_m);
    query_c_pay.bindValue(":ldt_end_m",ldt_end_m);
    query_c_pay.exec();
    while(query_c_pay.next()){//0
        ls_paymodeno=query_c_pay.record().value(0).toString();
        ls_paymodename=query_c_pay.record().value(1).toString();
        ls_incomeflag=query_c_pay.record().value(2).toString();
        ldec_amount_m=query_c_pay.record().value(3).toDouble();

        ldec_amount=0.0;
        query.prepare("Select sum(a.num_realamount)from cey_u_checkout_detail a , cey_u_checkout_master b  where a.ch_payno =b.ch_payno and a.ch_paymodeno =:ls_paymodeno and(b.ch_state ='Y')and b.dt_operdate >= :ldt_begin and b.dt_operdate <= :ldt_end");
        query.bindValue(":ls_paymodeno",ls_paymodeno);
        query.bindValue(":ldt_begin",ldt_begin);
        query.bindValue(":ldt_end",ldt_end);
        query.exec();
        ldec_amount=query.record().value(0).toDouble();
        if(ls_incomeflag=="Y"){
            ldec_total_m += ldec_amount_m;
            ldec_total += ldec_amount;
        }
        ldec_total1 += ldec_amount;
        ldec_total1_m += ldec_amount_m;

        items.clear();
        items << new QStandardItem(itemtextspan(tr("营业统计")));
        items << new QStandardItem("  **" + ls_paymodename);
        items << new QStandardItem(QString().sprintf("%.02f" , ldec_amount));
        items << new QStandardItem(QString().sprintf("%.02f" , ldec_amount_m));
        items << new QStandardItem(QString().sprintf("%.02f",ldec_amount_m==0? 0.0: (ldec_amount*1.0)/(ldec_amount_m*1.0) * 100) +"%");
        standmodel->appendRow(items);
    }//0
    //营业结算======================================================================================================================================
    ll_row=standmodel->rowCount();
    if(ll_row > 0){
        query.prepare("select sum(num_deposit) from t_m_deposit  where ch_deposit_mode <> '4' and dt_operdate >= :ldt_begin and dt_operdate <= :ldt_end");
        query.bindValue(":ldt_begin",ldt_begin);
        query.bindValue(":ldt_end",ldt_end);
        query.exec();
        query.next();
        ldec_deposit=query.record().value(0).toDouble();

        query.prepare("select sum(num_deposit) from t_m_deposit   where ch_deposit_mode <> '4' and dt_operdate >= :ldt_begin_m and dt_operdate <= :ldt_end_m");
        query.bindValue(":ldt_begin_m",ldt_begin_m);
        query.bindValue(":ldt_end_m",ldt_end_m);
        query.exec();
        query.next();
        ldec_deposit_m=query.record().value(0).toDouble();

        if(ldec_deposit > 0 && ldec_deposit_m > 0){
            items.clear();
            items << new QStandardItem(tr("营业结算"));
            items << new QStandardItem(tr("充值卡充值额"));
            items << new QStandardItem(QString().sprintf("%.02f" , ldec_deposit));
            items << new QStandardItem(QString().sprintf("%.02f" , ldec_deposit_m));
            items << new QStandardItem(QString().sprintf("%.02f",ldec_deposit_m==0? 0.0: (ldec_deposit*1.0)/(ldec_deposit_m*1.0) * 100) +"%");
            standmodel->appendRow(items);
        }
    }
    if(ll_row > 0){
        ldec_total1_m = ldec_total1_m + ldec_free_m + ldec_blotout_m;
        ldec_total1 = ldec_total1 + ldec_free + ldec_blotout;

        items.clear();
        items << new QStandardItem(itemtextspan(tr("营业结算")));
        items << new QStandardItem(tr("实收金额"));
        items << new QStandardItem(QString().sprintf("%.02f" , ldec_total1));
        items << new QStandardItem(QString().sprintf("%.02f" , ldec_total1_m));
        items << new QStandardItem(QString().sprintf("%.02f",ldec_total1_m==0? 0.0: (ldec_total1*1.0)/(ldec_total1_m*1.0) * 100) +"%");
        standmodel->appendRow(items);
    }
    if(ll_row > 0){
        ls_rate = "0.00%";
        if(ldec_total_m!=0){
            ls_rate=QString().sprintf("%.02f", ldec_total/ldec_total_m * 100) +"%";
        }

        items.clear();
        items << new QStandardItem(itemtextspan(tr("营业结算")));
        items << new QStandardItem(tr("实际收入"));
        items << new QStandardItem(QString().sprintf("%.02f" , ldec_total));
        items << new QStandardItem(QString().sprintf("%.02f" , ldec_total_m));
        items << new QStandardItem(QString().sprintf("%.02f", ldec_total_m==0? 0.0: (ldec_total*1.0)/(ldec_total_m*1.0) * 100) +"%");
        standmodel->appendRow(items);
    }

    //表头
    standmodel->setHorizontalHeaderLabels(QStringList () << tr("统计类型") << tr("统计项目") << tr("日营业值") << tr("月累计值") << tr("占当月比例"));
    //制表人 和 制表时间
    ui->label_operator->setText(n_func::gs_opername);
    ui->label_operattime->setText(n_func::f_get_sysdatetime().toString("yyyy-MM-dd hh:mm:ss"));
//    ui->label_timefrom->setText(ldt_begin.toString("yyyy-MM-dd hh:mm:ss"));
//    ui->label_timeto->setText(ldt_end.toString("yyyy-MM-dd hh:mm:ss"));
}

void w_rpt_report_daybusinessreprot::toexit()
{
    this->reject();
}

void w_rpt_report_daybusinessreprot::updatecombegindate(int yearindex)
{
    ui->spinBox_beginday->setRange(0, 99);
    switch(yearindex){
    case 0://上月
        ui->spinBox_beginday->setMinimum(16);
        break;
    case 1://号
        ui->spinBox_beginday->setMaximum(15);
        break;
    }
}

void w_rpt_report_daybusinessreprot::savebeginset()
{
    QSettings *conf = new QSettings(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    conf->setValue("w_rpt_report_daybusinessreprot/rpt_month", ui->comboBox_beginmonth->currentIndex());
    conf->setValue("w_rpt_report_daybusinessreprot/rpt_day", ui->spinBox_beginday->value());
    conf->sync();
    delete conf;
}

void w_rpt_report_daybusinessreprot::loadbeginset()
{
    QSettings *conf = new QSettings(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    ui->comboBox_beginmonth->setCurrentIndex(conf->value("w_rpt_report_daybusinessreprot/rpt_month").toInt());
    ui->spinBox_beginday->setValue(conf->value("w_rpt_report_daybusinessreprot/rpt_day").toInt());

    //直接更新range
    updatecombegindate(ui->comboBox_beginmonth->currentIndex());
}

QString w_rpt_report_daybusinessreprot::itemtextspan(const QString &itemtext)
{
    for(int r=0,r_count=standmodel->rowCount(); r<r_count;r++){
        if(standmodel->item(r, 0)->text() == itemtext){
            return "";
        }
    }
    return itemtext;
}
