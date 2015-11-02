#include "w_rpt_report_dishsalereport.h"
//菜品消费报表
#include "ui_w_rpt_report_dishsalereport_dialog.h"
#include <QSqlQueryModel>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlRecord>
#include "n_func.h"
#include "lds_tableview.h"
#include <QSqlError>
#include "backheader.h"

w_rpt_report_dishsalereport::w_rpt_report_dishsalereport(QWidget *parent) :
    QDialog(parent),ui(new Ui_w_rpt_report_dishsalereport_Dialog)
{
    ui->setupUi(this);
    ui->frame->setFixedWidth(w_rpt_report::frame_left_fixed_width);
    //! init
    ui->dateTimeEdit->setDateTime(QDateTime(n_func::f_get_sysdatetime().date(), QTime(0,0,0)));
    ui->dateTimeEdit_2->setDateTime(QDateTime(n_func::f_get_sysdatetime().date(), QTime(23,59,59)));

    ui->tableView->tableView->setSortingEnabled(true);
    ui->tableView_2->tableView->setSortingEnabled(true);

    tab1sqlinit();
    tab2sqlinit();

    querymodel=new datetimeModel;
    querymodel_2=new datetimeModel;

    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);ui->tableView->setModel(querymodel);
    ui->tableView_2->setEditTriggers(QAbstractItemView::NoEditTriggers);ui->tableView_2->setModel(querymodel_2);

    ui->comboBox_11->addItems(tab1info.keys());
    ui->comboBox_21->addItems(tab1info.keys());
    //! toolbar
    ui->widget->setup(QStringList()<<tr("查询")<<tr("退出"), Qt::AlignTop);
    connect(ui->widget->index_widget(tr("查询")),SIGNAL(clicked()),this,SLOT(toselect()));
    connect(ui->widget->index_widget(tr("退出")),SIGNAL(clicked()),this,SLOT(toexit()));
    connect(ui->comboBox_11,SIGNAL(currentIndexChanged(QString)),this,SLOT(tocom12(QString)));
    connect(ui->tableView->tableView->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(toselect()));
    connect(ui->tableView_2->tableView->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(toselect()));

    //! after
    tocom12(ui->comboBox_11->currentText());

    //tabwidget header
    ui->tabWidget->setTabText(0, tr("菜品消费报表明细"));
    ui->tabWidget->setTabText(1, tr("菜品消费汇总报表"));
}

void w_rpt_report_dishsalereport::toselect()
{
    QString ls_begin;
    QString ls_end;
    ls_begin=n_func::uf_getdatetime("begin", ui->dateTimeEdit->dateTime()).toString("yyyy-MM-dd hh:mm");
    ls_end=n_func::uf_getdatetime("end", ui->dateTimeEdit_2->dateTime()).toString("yyyy-MM-dd hh:mm");

    switch(ui->tabWidget->currentIndex()){
    case 0:{
        //tabwidget header
        ui->tabWidget->setTabText(0, tr("菜品消费报表明细"));
        //model select
        querymodel->setQuery(tab1info[ui->comboBox_11->currentText()].sql
                .arg(ls_begin.isEmpty()?ui->dateTimeEdit->dateTime().toString("yyyy-MM-dd hh:mm:ss"):ls_begin)
                .arg(ls_end.isEmpty()?ui->dateTimeEdit_2->dateTime().toString("yyyy-MM-dd hh:mm:ss"):ls_end)
                .arg(backheader::getinterdata(ui->comboBox_12->currentText()))//获取过滤信息
                .arg(w_rpt_report::querymodelordersort(ui->tableView->tableView))
                );
        qDebug() << querymodel->query().lastQuery();
        //更新表头
        backheader::modelSetHeader(querymodel, tab1info[ui->comboBox_11->currentText()].headers);
        //制表人 和 制表时间
        ui->label_operator->setText(n_func::gs_opername);
        ui->label_operattime->setText(n_func::f_get_sysdatetime().toString("yyyy-MM-dd hh:mm:ss"));
        ui->label_timefrom->setText(ls_begin);
        ui->label_timeto->setText(ls_end);

        //更新合计
        QString sumsql=querymodel->query().lastQuery().trimmed();
        if(sumsql.endsWith("DESC")||sumsql.endsWith("ASC")){
            //5 6 7  9 10
            QSqlQuery query;
            query.exec(QString("select sum(tabname5),sum(tabname6),sum(tabname7),sum(tabname9),sum(tabname10),sum(tabname11) from (%1)t1 ").arg(sumsql.left(sumsql.lastIndexOf("ORDER BY"))));
            query.next();
            ui->tableView->updateSum(5,query.record().value(0).toString());
            ui->tableView->updateSum(6,query.record().value(1).toString());
            ui->tableView->updateSum(7,query.record().value(2).toString());
            ui->tableView->updateSum(9,query.record().value(3).toString());
            ui->tableView->updateSum(10,query.record().value(4).toString());
            ui->tableView->updateSum(11,query.record().value(5).toString());
        }
    }break;
    case 1:{
        //tabwidget header
        ui->tabWidget->setTabText(1, tr("菜品消费汇总报表"));
        //model select
        querymodel_2->setQuery(tab2info[ui->comboBox_21->currentText()].sql
                .arg(ls_begin.isEmpty()?ui->dateTimeEdit->dateTime().toString("yyyy-MM-dd hh:mm:ss"):ls_begin)
                .arg(ls_end.isEmpty()?ui->dateTimeEdit_2->dateTime().toString("yyyy-MM-dd hh:mm:ss"):ls_end)
                .arg(w_rpt_report::querymodelordersort(ui->tableView_2->tableView))
                );
        //更新表头
        backheader::modelSetHeader(querymodel_2, tab2info[ui->comboBox_21->currentText()].headers);
        //制表人 和 制表时间
        ui->label_operator->setText(n_func::gs_opername);
        ui->label_operattime->setText(n_func::f_get_sysdatetime().toString("yyyy-MM-dd hh:mm:ss"));
        //更新合计
        QString sumsql=querymodel_2->query().lastQuery().trimmed();
        qDebug() << sumsql;
        if(sumsql.endsWith("DESC")||sumsql.endsWith("ASC")){
            //5 6 7  9 10
            QSqlQuery query;
            query.exec(QString("select sum(tabname1),sum(tabname2),sum(tabname3),sum(tabname5),sum(tabname7) from (%1)t1 ").arg(sumsql.left(sumsql.lastIndexOf("ORDER BY"))));
            query.next();
            ui->tableView_2->updateSum(1,query.record().value(0).toString());
            ui->tableView_2->updateSum(2,query.record().value(1).toString());
            ui->tableView_2->updateSum(3,query.record().value(2).toString());
            ui->tableView_2->updateSum(5,query.record().value(3).toString());
            ui->tableView_2->updateSum(7,query.record().value(4).toString());
        }
    }break;
    }
}

void w_rpt_report_dishsalereport::toexit()
{
    this->reject();
}

void w_rpt_report_dishsalereport::tab1sqlinit()
{
    //按菜品类型
    tab1info.insert(tr("按菜品类型"),
                    w_rpt_report::TABTYPE(QStringList()<<tr("品类")<<tr("品码")<<tr("品名")<<tr("单位")<<tr("均价")<<tr("数量")<<tr("金额")<<tr("折扣额")<<tr("折扣率")<<tr("赠送额")<<tr("赠送率")<<tr("实际金额"),
                            QString(" select"
                                      " (select vch_dish_typename from cey_bt_dish_type where cey_bt_dish_type.ch_dish_typeno=cey_bt_dish.ch_dish_typeno) as 'tabname0',"
                                      " cey_bt_dish.ch_dishno as 'tabname1',"
                                      " cey_bt_dish.vch_dishname as 'tabname2',"
                                      " (select vch_unitname from cey_bt_unit where cey_bt_unit.ch_unitno=cey_bt_dish.ch_unitno) as 'tabname3',"
                                      " (case t.num when 0 then 0 else t.amount/t.num end) as 'tabname4',"
                                      " t.num as 'tabname5',"
                                      " t.amount as 'tabname6',"
                                      " t.discount as 'tabname7',"
                                      " (case t.amount when 0 then 0 else t.discount/t.amount end) as 'tabname8',"
                                      " t.present as 'tabname9',"
                                      " (case t.amount when 0 then 0 else t.present/t.amount end) as 'tabname10',"
                                      " t.amount-t.discount-t.present as 'tabname11'"
                                      " from"
                                      " cey_bt_dish,"
                                      " ("
                                      " SELECT"
                                      " a.ch_dishno,"
                                      " sum(a.num_num - a.num_back) as 'num',"
                                      " sum((a.num_num - a.num_back) * (a.num_price + a.num_price_add))  as 'amount',  "
                                      " Round(sum((a.num_num - a.num_back) * (a.num_price + a.num_price_add) * (1 - a.int_discount * 0.01)),2) as 'discount', "
                                      " Round(sum(case a.ch_presentflag when 'Y' then ((a.num_num - a.num_back) * (a.num_price + a.num_price_add) * a.int_discount * 0.01) else 0.00 end),2) as 'present' "
                                      " FROM cey_u_orderdish a,cey_u_checkout_master b,cey_bt_dish c  "
                                      " WHERE (a.ch_payno = b.ch_payno)"
                                      " and (b.ch_state = 'Y')"
                                      " and (a.ch_dishno = c.ch_dishno)"
                                      "  and(a.num_num - a.num_back <> 0 )"
                                      " and b.dt_operdate>'%1'"
                                      " and b.dt_operdate<= '%2'"
                                      " and (c.ch_dish_typeno like '%3%' )"
                                      " Group by a.ch_dishno)"
                                      " t  where cey_bt_dish.ch_dishno=t.ch_dishno  ORDER BY tabname%4"),
                            w_rpt_report::querymodelsetfilters("select * from cey_bt_dish_type")
                            ));

    tab1info.insert(tr("按餐桌"),
                    w_rpt_report::TABTYPE(QStringList()<<tr("餐桌")<<tr("品码")<<tr("品名")<<tr("单位")<<tr("均价")<<tr("数量")<<tr("金额")<<tr("折扣额")<<tr("折扣率")<<tr("赠送额")<<tr("赠送率")<<tr("实际金额"),
                            QString (
                                " select "
                                " t.ch_tableno as 'tabname0',"
                                " t.ch_dishno as 'tabname1',"
                                " t.vch_dishname as 'tabname2',"
                                " (select vch_unitname from cey_bt_unit where cey_bt_unit.ch_unitno=t.ch_unitno) as 'tabname3',"
                                " (case t.amount when 0 then 0 else t.amount/t.num end) as 'tabname4',"
                                " t.num as 'tabname5',"
                                " t.amount as 'tabname6',"
                                " t.discount as 'tabname7',"
                                " (case t.amount when 0 then 0 else t.discount/t.amount end) as 'tabname8',"
                                " t.present as 'tabname9',"
                                " (case t.amount when 0 then 0 else t.present/t.amount end) as 'tabname10',"
                                " t.amount-t.discount-t.present as 'tabname11' "
                                " from ("
                                " SELECT a.ch_tableno, a.ch_dishno,c.vch_dishname, c.ch_unitno,  sum(a.num_num - a.num_back) as 'num', sum((a.num_num - a.num_back) * (a.num_price + a.num_price_add)) as 'amount',Round(sum((a.num_num - a.num_back) * (a.num_price + a.num_price_add) * (1 - a.int_discount * 0.01)),2) as 'discount',Round(sum(case a.ch_presentflag when 'Y' then ((a.num_num - a.num_back) * (a.num_price + a.num_price_add) * a.int_discount * 0.01) else 0.00 end),2) as 'present'  FROM cey_u_orderdish a,cey_u_checkout_master b,cey_bt_dish c  WHERE (a.ch_payno = b.ch_payno) and (b.ch_state = 'Y') and (a.ch_dishno = c.ch_dishno) and(a.num_num - a.num_back <> 0)and b.dt_operdate > '%1' and b.dt_operdate <='%2' and (a.ch_tableno like '%3%')Group by a.ch_tableno,a.ch_dishno,c.vch_dishname,c.ch_unitno"
                                " )t ORDER BY tabname%4"
                                ),
                            w_rpt_report::querymodelsetfilters("select * from cey_bt_table")
                            ));

    tab1info.insert(tr("按餐桌类型"),
                    w_rpt_report::TABTYPE(QStringList()<<tr("餐桌类型")<<tr("品码")<<tr("品名")<<tr("单位")<<tr("均价")<<tr("数量")<<tr("金额")<<tr("折扣额")<<tr("折扣率")<<tr("赠送额")<<tr("赠送率")<<tr("实际金额"),
                            QString (
                                "select"
                                " t.ch_typeno as 'tabname0',"
                                " t.ch_dishno as 'tabname1',"
                                " t.vch_dishname as 'tabname2',"
                                " (select vch_unitname from cey_bt_unit where cey_bt_unit.ch_unitno=t.ch_unitno) as 'tabname3',"
                                " (case t.amount when 0 then 0 else t.amount/t.num end) as 'tabname4',"
                                " t.num as 'tabname5',"
                                " t.amount as 'tabname6',"
                                " t.discount as 'tabname7',"
                                " (case t.amount when 0 then 0 else t.discount/t.amount end) as 'tabname8',"
                                " t.present as 'tabname9',"
                                " (case t.amount when 0 then 0 else t.present/t.amount end) as 'tabname10',"
                                " t.amount-t.discount-t.present as 'tabname11' "
                                " from ("
                                " SELECT d.ch_typeno,a.ch_dishno,c.vch_dishname,  c.ch_unitno,  sum(a.num_num - a.num_back) as 'num', sum((a.num_num - a.num_back) * (a.num_price + a.num_price_add)) as 'amount',Round(sum((a.num_num - a.num_back) * (a.num_price + a.num_price_add) * (1 - a.int_discount * 0.01)),2) as 'discount', Round(sum(case a.ch_presentflag when 'Y' then ((a.num_num - a.num_back) * (a.num_price + a.num_price_add) * a.int_discount * 0.01) else 0.00 end),2) as 'present'  FROM cey_u_orderdish a,cey_u_checkout_master b,cey_bt_dish c  ,(select ch_typeno,ch_tableno from cey_bt_table union all select 'Q0' as ch_typeno,'Q000' as ch_tableno) d WHERE (a.ch_tableno = d.ch_tableno) and (a.ch_payno = b.ch_payno) and (b.ch_state = 'Y') and (a.ch_dishno = c.ch_dishno) and(a.num_num - a.num_back <> 0)and b.dt_operdate > '%1' and b.dt_operdate <='%2' and (d.ch_typeno like '%3%') Group by d.ch_typeno,a.ch_dishno,c.vch_dishname,c.ch_unitno"
                                ") t ORDER BY tabname%4"
                                ),
                            w_rpt_report::querymodelsetfilters("select * from cey_bt_table_area")
                            ));
    tab1info.insert(tr("按操作员"),
                    w_rpt_report::TABTYPE(QStringList()<<tr("操作员")<<tr("品码")<<tr("品名")<<tr("单位")<<tr("均价")<<tr("数量")<<tr("金额")<<tr("折扣额")<<tr("折扣率")<<tr("赠送额")<<tr("赠送率")<<tr("实际金额"),
                            QString (
                                "select"
                                " t.vch_operID as 'tabname0',"
                                " t.ch_dishno as 'tabname1',"
                                " t.vch_dishname as 'tabname2',"
                                " (select vch_unitname from cey_bt_unit where cey_bt_unit.ch_unitno=t.ch_unitno) as 'tabname3',"
                                " (case t.amount when 0 then 0 else t.amount/t.num end) as 'tabname4',"
                                " t.num as 'tabname5',"
                                " t.amount as 'tabname6',"
                                " t.discount as 'tabname7',"
                                " (case t.amount when 0 then 0 else t.discount/t.amount end) as 'tabname8',"
                                " t.present as 'tabname9',"
                                " (case t.amount when 0 then 0 else t.present/t.amount end) as 'tabname10',"
                                " t.amount-t.discount-t.present as 'tabname11'"
                                " from("
                                " SELECT a.vch_operID, a.ch_dishno,c.vch_dishname, c.ch_unitno,  sum(a.num_num - a.num_back) as 'num', sum((a.num_num - a.num_back) * (a.num_price + a.num_price_add)) as 'amount',Round(sum((a.num_num - a.num_back) * (a.num_price + a.num_price_add) * (1 - a.int_discount * 0.01)),2) as 'discount',Round(sum(case a.ch_presentflag when 'Y' then ((a.num_num - a.num_back) * (a.num_price + a.num_price_add) * a.int_discount * 0.01) else 0.00 end),2) as 'present'  FROM cey_u_orderdish a,cey_u_checkout_master b,cey_bt_dish c  WHERE (a.ch_payno = b.ch_payno) and (b.ch_state = 'Y') and (a.ch_dishno = c.ch_dishno) and(a.num_num - a.num_back <> 0)and b.dt_operdate > '%1' and b.dt_operdate <='%2' and (a.vch_operID like '%3%')Group by a.vch_operID,a.ch_dishno,c.vch_dishname,c.ch_unitno"
                                ")t ORDER BY tabname%4"
                                ),
                            w_rpt_report::querymodelsetfilters("Select vch_operID,vch_operator from cey_sys_operator union Select '0000' as 'vch_operID', 'system' as 'vch_operator' ")
                            ));

    tab1info.insert(tr("按付款单号"),
                    w_rpt_report::TABTYPE(QStringList()<<tr("付款单号")<<tr("品码")<<tr("品名")<<tr("单位")<<tr("均价")<<tr("数量")<<tr("金额")<<tr("折扣额")<<tr("折扣率")<<tr("赠送额")<<tr("赠送率")<<tr("实际金额")<<tr("收款人")<<tr("收款时间")<<tr("点菜人")<<tr("点菜时间"),
                            QString (
                                "select"
                                " t.ch_payno as 'tabname0',"
                                " t.ch_dishno as 'tabname1',"
                                " t.vch_dishname as 'tabname2',"
                                " (select vch_unitname from cey_bt_unit where cey_bt_unit.ch_unitno=t.ch_unitno) as 'tabname3',"
                                " (case t.amount when 0 then 0 else t.amount/t.num end) as 'tabname4',"
                                " t.num as 'tabname5',"
                                " t.amount as 'tabname6',"
                                " t.discount as 'tabname7',"
                                " (case t.amount when 0 then 0 else t.discount/t.amount end) as 'tabname8',"
                                " t.present as 'tabname9',"
                                " (case t.amount when 0 then 0 else t.present/t.amount end) as 'tabname10',"
                                " t.amount-t.discount-t.present as 'tabname11',"
                                " t.vch_operID as 'tabname12',"
                                " t.dt_operdate as 'tabname13',"
                                " t.vch_order as 'tabname14',"
                                " t.dt_order as 'tabname15'"
                                " from("
                                " SELECT b.ch_payno,"
                                " a.ch_dishno,"
                                " c.vch_dishname,  "
                                " c.ch_unitno,  "
                                "  (a.num_num - a.num_back) as 'num',"
                                "  (a.num_num - a.num_back) * (a.num_price + a.num_price_add) as 'amount',"
                                "  (a.num_num - a.num_back) * (a.num_price + a.num_price_add) * (1 - a.int_discount * 0.01) as 'discount',"
                                "  case a.ch_presentflag when 'Y' then ((a.num_num - a.num_back) * (a.num_price + a.num_price_add) * a.int_discount * 0.01) else 0.00 end as 'present' ,"
                                " b.vch_operID as 'vch_operID',"
                                " c.ch_dish_typeno as 'typeno',"
                                " b.dt_operdate as 'dt_operdate',"
                                " a.vch_operID as 'vch_order',"
                                " a.dt_operdate as 'dt_order'"
                                " FROM cey_u_orderdish a,cey_u_checkout_master b,cey_bt_dish c"
                                " WHERE(a.ch_payno = b.ch_payno)and"
                                "(b.ch_state = 'Y')and"
                                "(a.ch_dishno = c.ch_dishno)and"
                                "(a.num_num - a.num_back <> 0)and"
                                "(b.dt_operdate > '%1' and b.dt_operdate <='%2' and '%3'='%3')"
                                ")t ORDER BY tabname%4"
                                ),
                            w_rpt_report::querymodelsetfilters("")
                            ));
    tab1info.insert(tr("按班次营业时段"),
                    w_rpt_report::TABTYPE(QStringList()<<tr("班次营业时段")<<tr("品码")<<tr("品名")<<tr("单位")<<tr("均价")<<tr("数量")<<tr("金额")<<tr("折扣额")<<tr("折扣率")<<tr("赠送额")<<tr("赠送率")<<tr("实际金额")<<tr("收款人")<<tr("收款时间")<<tr("点菜人")<<tr("点菜时间"),
                            QString (
                                "select"
                                " t.ch_dish_typeno as 'tabname0',"
                                " t.ch_dishno as 'tabname1',"
                                " t.vch_dishname as 'tabname2',"
                                " (select vch_unitname from cey_bt_unit where cey_bt_unit.ch_unitno=t.ch_unitno) as 'tabname3',"
                                " (case t.amount when 0 then 0 else t.amount/t.num end) as 'tabname4',"
                                " t.num as 'tabname5',"
                                " t.amount as 'tabname6',"
                                " t.discount as 'tabname7',"
                                " (case t.amount when 0 then 0 else t.discount/t.amount end) as 'tabname8',"
                                " t.present as 'tabname9',"
                                " (case t.amount when 0 then 0 else t.present/t.amount end) as 'tabname10',"
                                " t.amount-t.discount-t.present as tabname11"
                                " from ("
                                " Select BB.ch_dish_typeno, BB.ch_dishno, BB.vch_dishname, BB.ch_unitno, sum(BB.num) as 'num', sum(BB.amount) as 'amount', sum(BB.discount) as 'discount', sum(BB.present) as 'present' "
                                " From ("
                                " SELECT  ifnull(Case When DATE_FORMAT(b.dt_operdate, '%Y-%m-%d') < '' Then '' Else ("

                                " Select  ch_classno"
                                " From cey_bt_class where DATE_FORMAT(dt_begin, '%Y-%m-%d') < DATE_FORMAT(b.dt_operdate, '%Y-%m-%d')  Order By dt_begin Desc  LIMIT 1) End,'') as 'ch_dish_typeno' ,"

                                " a.ch_dishno , c.vch_dishname, c.ch_unitno,  sum(a.num_num - a.num_back) as 'num', sum((a.num_num - a.num_back) * (a.num_price + a.num_price_add)) as 'amount',Round(sum((a.num_num - a.num_back) * (a.num_price + a.num_price_add) * (1 - a.int_discount * 0.01)),2) as 'discount', Round(sum(case a.ch_presentflag when 'Y' then ((a.num_num - a.num_back) * (a.num_price + a.num_price_add) * a.int_discount * 0.01) else 0.00 end),2) as 'present' "
                                " FROM cey_u_orderdish a,cey_u_checkout_master b,cey_bt_dish c  WHERE (a.ch_payno = b.ch_payno) and (b.ch_state = 'Y') and (a.ch_dishno = c.ch_dishno) and(a.num_num - a.num_back <> 0)and b.dt_operdate > '%1' and b.dt_operdate <='%2' and '%3'='%3' Group by b.dt_operdate,a.ch_dishno,c.vch_dishname,c.ch_unitno,c.ch_dish_typeno  )BB  Where BB.ch_dish_typeno Like '%3%' Group by BB.ch_dish_typeno, BB.ch_dishno, BB.vch_dishname, BB.ch_unitno"
                                ")t ORDER BY tabname%4"
                                ),
                            w_rpt_report::querymodelsetfilters("select ch_classno, vch_classname from cey_bt_class")
                            ));
}

w_rpt_report_dishsalereport::~w_rpt_report_dishsalereport()
{
    /*list释放*/
}

void w_rpt_report_dishsalereport::tocom12(const QString &text)
{
    ui->comboBox_12->clear();
    ui->comboBox_12->addItems(tab1info[text].filters);
}

void w_rpt_report_dishsalereport::tab2sqlinit()
{
    tab2info.insert(tr("按菜品类型"),
                    w_rpt_report::TABTYPE(QStringList()<<tr("品类")<<tr("数量")<<tr("金额")<<tr("折扣额")<<tr("折扣率")<<tr("赠送额")<<tr("赠送率")<<tr("实际金额"),
                            QString (
                                "select"
                                " (select vch_dish_typename from cey_bt_dish_type where cey_bt_dish_type.ch_dish_typeno = t.ch_dish_typeno) as 'tabname0',"
                                " t.num as 'tabname1',"
                                " t.amount as 'tabname2',"
                                " t.discount as 'tabname3',"
                                " (case t.amount when 0 then 0 else t.discount/t.amount end) as 'tabname4',"
                                " t.present as 'tabname5',"
                                " (case t.amount when 0 then 0 else t.present/t.amount end) as 'tabname6',"
                                " t.amount-t.discount-t.present as 'tabname7' "
                                " from("
                                " SELECT c.ch_dish_typeno,sum(a.num_num - a.num_back) as 'num',sum((a.num_num - a.num_back) * (a.num_price + a.num_price_add)) as 'amount',Round(sum((a.num_num - a.num_back) * (a.num_price + a.num_price_add) * (1 - a.int_discount * 0.01)),2) as 'discount',Round(sum(case a.ch_presentflag when 'Y' then ((a.num_num - a.num_back) * (a.num_price + a.num_price_add) * a.int_discount * 0.01) else 0.00 end),2)  as 'present' FROM cey_u_orderdish a,cey_u_checkout_master b,cey_bt_dish c  WHERE (a.ch_payno = b.ch_payno) and (b.ch_state = 'Y') and (a.ch_dishno = c.ch_dishno) and(a.num_num - a.num_back <> 0)and b.dt_operdate > '%1' and b.dt_operdate <='%2' and (c.ch_dish_typeno like '%')Group by c.ch_dish_typeno"
                                ") t ORDER BY tabname%3")
                            ,
                            w_rpt_report::querymodelsetfilters("")
                            ));

    tab2info.insert(tr("按餐桌"),
                    w_rpt_report::TABTYPE(QStringList()<<tr("餐桌")<<tr("数量")<<tr("金额")<<tr("折扣额")<<tr("折扣率")<<tr("赠送额")<<tr("赠送率")<<tr("实际金额"),
                            QString (
                                "select"
                                " t.ch_tableno as 'tabname0',"
                                " t.num as 'tabname1',"
                                " t.amount as 'tabname2',"
                                " t.discount as 'tabname3',"
                                " (case t.amount when 0 then 0 else t.discount/t.amount end) as 'tabname4',"
                                " t.present as 'tabname5',"
                                " (case t.amount when 0 then 0 else t.present/t.amount end) as 'tabname6',"
                                " t.amount-t.discount-t.present as 'tabname7'"
                                " from("
                                " SELECT a.ch_tableno, sum(a.num_num - a.num_back) as 'num',sum((a.num_num - a.num_back) * (a.num_price + a.num_price_add)) as 'amount',Round(sum((a.num_num - a.num_back) * (a.num_price + a.num_price_add) * (1 - a.int_discount * 0.01)),2) as 'discount',Round(sum(case a.ch_presentflag when 'Y' then ((a.num_num - a.num_back) * (a.num_price + a.num_price_add) * a.int_discount * 0.01) else 0.00 end),2) as 'present'  FROM cey_u_orderdish a,cey_u_checkout_master b,cey_bt_dish c  WHERE (a.ch_payno = b.ch_payno) and (b.ch_state = 'Y') and (a.ch_dishno = c.ch_dishno) and(a.num_num - a.num_back <> 0)and b.dt_operdate > '%1' and b.dt_operdate <='%2' and (a.ch_tableno like '%')Group by a.ch_tableno"
                                ") t ORDER BY tabname%3")
                            ,
                            w_rpt_report::querymodelsetfilters("")
                            ));

    tab2info.insert(tr("按餐桌类型"),
                    w_rpt_report::TABTYPE(QStringList()<<tr("餐桌类型")<<tr("数量")<<tr("金额")<<tr("折扣额")<<tr("折扣率")<<tr("赠送额")<<tr("赠送率")<<tr("实际金额"),
                            QString (
                                "select"
                                " t.ch_typeno as 'tabname0',"
                                " t.num as 'tabname1',"
                                " t.amount as 'tabname2',"
                                " t.discount as 'tabname3',"
                                " (case t.amount when 0 then 0 else t.discount/t.amount end) as 'tabname4',"
                                " t.present as 'tabname5',"
                                " (case t.amount when 0 then 0 else t.present/t.amount end) as 'tabname6',"
                                " t.amount-t.discount-t.present as 'tabname7'"
                                " from("
                                " SELECT d.ch_typeno, sum(a.num_num - a.num_back) as 'num',sum((a.num_num - a.num_back) * (a.num_price + a.num_price_add)) as 'amount', Round(sum((a.num_num - a.num_back) * (a.num_price + a.num_price_add) * (1 - a.int_discount * 0.01)),2) as 'discount', Round(sum(case a.ch_presentflag when 'Y' then ((a.num_num - a.num_back) * (a.num_price + a.num_price_add) * a.int_discount * 0.01) else 0.00 end),2) as 'present'  FROM cey_u_orderdish a,cey_u_checkout_master b,cey_bt_dish c  ,(select ch_typeno,ch_tableno from cey_bt_table union all select 'Q0' as ch_typeno,'Q000' as ch_tableno) d WHERE (a.ch_tableno = d.ch_tableno) and (a.ch_payno = b.ch_payno) and (b.ch_state = 'Y') and (a.ch_dishno = c.ch_dishno) and(a.num_num - a.num_back <> 0)and b.dt_operdate > '%1' and b.dt_operdate <='%2' and (d.ch_typeno like '%')Group by d.ch_typeno"
                                ") t ORDER BY tabname%3")
                            ,
                            w_rpt_report::querymodelsetfilters("")
                            ));
    tab2info.insert(tr("按操作员"),
                    w_rpt_report::TABTYPE(QStringList()<<tr("操作员")<<tr("数量")<<tr("金额")<<tr("折扣额")<<tr("折扣率")<<tr("赠送额")<<tr("赠送率")<<tr("实际金额"),
                            QString (
                                "select"
                                " t.vch_operID as 'tabname0',"
                                " t.num as 'tabname1',"
                                " t.amount as 'tabname2',"
                                " t.discount as 'tabname3',"
                                " (case t.amount when 0 then 0 else t.discount/t.amount end) as 'tabname4',"
                                " t.present as 'tabname5',"
                                " (case t.amount when 0 then 0 else t.present/t.amount end) as 'tabname6',"
                                " t.amount-t.discount-t.present as 'tabname7' "
                                " from("
                                " SELECT a.vch_operID,sum(a.num_num - a.num_back) as 'num', sum((a.num_num - a.num_back) * (a.num_price + a.num_price_add)) as 'amount',Round(sum((a.num_num - a.num_back) * (a.num_price + a.num_price_add) * (1 - a.int_discount * 0.01)),2) as 'discount',Round(sum(case a.ch_presentflag when 'Y' then ((a.num_num - a.num_back) * (a.num_price + a.num_price_add) * a.int_discount * 0.01) else 0.00 end),2) as 'present'  FROM cey_u_orderdish a,cey_u_checkout_master b,cey_bt_dish c  WHERE (a.ch_payno = b.ch_payno) and (b.ch_state = 'Y') and (a.ch_dishno = c.ch_dishno) and(a.num_num - a.num_back <> 0)and b.dt_operdate > '%1' and b.dt_operdate <='%2' and (a.vch_operID like '%')Group by a.vch_operID"
                                ") t ORDER BY tabname%3")
                            ,
                            w_rpt_report::querymodelsetfilters("")
                            ));
    tab2info.insert(tr("按付款单号"),
                    w_rpt_report::TABTYPE(QStringList()<<tr("付款单号")<<tr("数量")<<tr("金额")<<tr("折扣额")<<tr("折扣率")<<tr("赠送额")<<tr("赠送率")<<tr("实际金额")<<tr("收款人")<<tr("收款时间"),
                            QString (
                                "select"
                                " t.ch_payno as 'tabname0',"
                                " t.num as 'tabname1',"
                                " t.amount as 'tabname2',"
                                " t.discount as 'tabname3',"
                                " (case t.amount when 0 then 0 else t.discount/t.amount end) as 'tabname4',"
                                " t.present as 'tabname5',"
                                " (case t.amount when 0 then 0 else t.present/t.amount end) as 'tabname6',"
                                " t.amount-t.discount-t.present as 'tabname7',"
                                " t.vch_operID as 'tabname8',"
                                " t.dt_operdate as 'tabname9'"
                                " from("
                                " SELECT b.ch_payno,"
                                " sum(a.num_num - a.num_back) as 'num',"
                                " sum((a.num_num - a.num_back) * (a.num_price + a.num_price_add)) as 'amount',"
                                " sum((a.num_num - a.num_back) * (a.num_price + a.num_price_add) * (1 - a.int_discount * 0.01)) as 'discount',"
                                " sum(case a.ch_presentflag when 'Y' then ((a.num_num - a.num_back) * (a.num_price + a.num_price_add) * a.int_discount * 0.01) else 0.00 end) as 'present',"
                                " b.vch_operID as 'vch_operID',"
                                " b.dt_operdate as 'dt_operdate'"
                                " FROM cey_u_orderdish a,cey_u_checkout_master b,cey_bt_dish c"
                                " WHERE(a.ch_payno = b.ch_payno)and"
                                "(b.ch_state = 'Y')and"
                                "(a.ch_dishno = c.ch_dishno)and"
                                "(a.num_num - a.num_back <> 0)and"
                                "(b.dt_operdate > '%1' and b.dt_operdate <='%2')"
                                " group by b.ch_payno, b.vch_operID, b.dt_operdate"
                                ")t ORDER BY tabname%3"
                                ),
                            w_rpt_report::querymodelsetfilters("")
                            ));
    tab2info.insert(tr("按班次营业时段"),
                    w_rpt_report::TABTYPE(QStringList()<<tr("班次")<<tr("数量")<<tr("金额")<<tr("折扣额")<<tr("折扣率")<<tr("赠送额")<<tr("赠送率")<<tr("实际金额"),
                            QString (
                                "select"
                                " t.dish_typeno as 'tabname0',"
                                " t.num as 'tabname1',"
                                " t.amount as 'tabname2',"
                                " t.discount as 'tabname3',"
                                "  (case t.amount when 0 then 0 else t.discount/t.amount end) as 'tabname4',"
                                " t.present as 'tabname5',"
                                " (case t.amount when 0 then 0 else t.present/t.amount end) as 'tabname6',"
                                " t.amount-t.discount-t.present as 'tabname7'"
                                " from ("
                                " Select BB.dish_typeno, sum(BB.num) as 'num',  sum(BB.amount) as 'amount',  sum(BB.discount) as 'discount', sum(BB.present) as 'present'  From (SELECT ifnull(Case When DATE_FORMAT(b.dt_operdate, '%Y-%m-%d')  < '' Then '' Else (Select ch_classno From cey_bt_class where DATE_FORMAT(dt_begin, '%Y-%m-%d')  < DATE_FORMAT(b.dt_operdate, '%Y-%m-%d')  Order By dt_begin Desc LIMIT 1) End,'')  as 'dish_typeno',sum(a.num_num - a.num_back) as 'amount', sum((a.num_num - a.num_back) * (a.num_price + a.num_price_add)) as 'num', Round(sum((a.num_num - a.num_back) * (a.num_price + a.num_price_add) * (1 - a.int_discount * 0.01)),2) as 'discount',Round(sum(case a.ch_presentflag when 'Y' then ((a.num_num - a.num_back) * (a.num_price + a.num_price_add) * a.int_discount * 0.01) else 0.00 end),2) as 'present'  FROM cey_u_orderdish a,cey_u_checkout_master b,cey_bt_dish c  WHERE (a.ch_payno = b.ch_payno) and (b.ch_state = 'Y') and (a.ch_dishno = c.ch_dishno) and(a.num_num - a.num_back <> 0)and b.dt_operdate > '%1' and b.dt_operdate <='%2' Group by b.dt_operdate,a.ch_dishno,c.vch_dishname,c.ch_unitno,c.ch_dish_typeno  )BB  Where BB.dish_typeno Like '%' Group by BB.dish_typeno"
                                ")t ORDER BY tabname%3"
                                ),
                            w_rpt_report::querymodelsetfilters("select ch_classno, vch_classname from cey_bt_class")
                            ));


}

