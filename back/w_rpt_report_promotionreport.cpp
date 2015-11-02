#include "w_rpt_report_promotionreport.h"
//菜品促销报表
#include "ui_w_rpt_report_dishsalereport_dialog.h"
#include <QSqlQueryModel>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlRecord>
#include "n_func.h"
#include "lds_tableview.h"
#include <QSqlError>
#include "backheader.h"
w_rpt_report_promotionreport::w_rpt_report_promotionreport(QWidget *parent) :
    QDialog(parent),ui(new Ui_w_rpt_report_dishsalereport_Dialog)
{
    ui->setupUi(this);
    ui->frame->setFixedWidth(w_rpt_report::frame_left_fixed_width);
    //! init
    ui->dateTimeEdit->setDateTime(QDateTime(n_func::f_get_sysdatetime().date(), QTime(0,0,0)));
    ui->dateTimeEdit_2->setDateTime(QDateTime(n_func::f_get_sysdatetime().date(), QTime(23,59,59)));

    ui->tableView->tableView->setSortingEnabled(true);
    tab1sqlinit();
    querymodel=new datetimeModel;
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);ui->tableView->setModel(querymodel);
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
    ui->tabWidget->setCurrentIndex(0);
    ui->tabWidget->removeTab(1);
    ui->stackedWidget->setCurrentIndex(0);

    //tabwidget header
    ui->tabWidget->setTabText(0, tr("菜品促销报表"));
}

w_rpt_report_promotionreport::~w_rpt_report_promotionreport()
{

}

void w_rpt_report_promotionreport::tab1sqlinit()
{
    tab1info.insert(tr("套菜消费汇总报表"),
                    w_rpt_report::TABTYPE(QStringList()<<tr("套菜编码")<<tr("品名")<<tr("单位")<<tr("均价")<<tr("数量")<<tr("金额")<<tr("折扣额")<<tr("折扣率")<<tr("赠送额")<<tr("赠送率")<<tr("实际金额"),
                            QString (
                                " select "
                                " t.dishno as 'tabname0',"
                                " t.dishname as 'tabname1',"
                                " (select vch_unitname from cey_bt_unit where cey_bt_unit.ch_unitno=t.dish_unitno) as 'tabname2',"
                                " (case t.amount when 0 then 0 else t.amount/t.num end) as 'tabname3',"
                                " t.num as 'tabname4',"
                                " t.amount as 'tabname5',"
                                " t.discount as 'tabname6',"
                                " (case t.amount when 0 then 0 else t.discount/t.amount end) as 'tabname7',"
                                " t.present as 'tabname8',"
                                " (case t.amount when 0 then 0 else t.present/t.amount end) as 'tabname9',"
                                " t.amount-t.discount-t.present as 'tabname10' "
                                " from (%1)t  ORDER BY tabname%4").arg(
                                QString (

                                    "SELECT d.ch_dishno as 'dishno' ,"

                                    " d.vch_dishname as 'dishname' ,"

                                    " d.ch_unitno as 'dish_unitno' ,"

                                    " sum((num_num - num_back)) as 'num' ,"

                                    " sum((num_num - num_back) * (a.num_price + num_price_add)) as 'amount' ,"

                                    " sum((((num_num - num_back) * (a.num_price + num_price_add)) *"

                                    " (100 - int_discount) * 0.01)) as 'discount' ,"

                                    " sum((case ch_presentflag"

                                    " when 'Y' then"

                                    " ((num_num - num_back) *"

                                    " (a.num_price + num_price_add)) * int_discount * 0.01"

                                    " else"

                                    " 0.00"

                                    " end)) as 'present' ,"

                                    " replace(d.ch_dishno, ' ', '') as 'suitno' ,"

                                    " '0' as 'orders' "

                                    " FROM cey_u_orderdish a, cey_u_checkout_master c, cey_bt_dish d"

                                    " WHERE a.ch_payno = c.ch_payno"

                                    " and c.ch_state = 'Y'"

                                    " and (a.ch_suitflag = 'P')"

                                    " and a.ch_dishno = d.ch_dishno"

                                    " and (num_num - num_back) <> 0"

                                    " and (DATE_FORMAT(C.dt_operdate, '%Y-%m-%d %h:%i:%s') >='%1')"

                                    " and (DATE_FORMAT(C.dt_operdate, '%Y-%m-%d %h:%i:%s') <='%2')"

                                    " and d.ch_dishno like '%3%'"

                                    " and (DATE_FORMAT(c.dt_operdate, '%Y') >='0')"

                                    " and (DATE_FORMAT(c.dt_operdate, '%Y') <='3')"

                                    " group by d.ch_dishno, d.vch_dishname, d.ch_unitno"

                                    " union"

                                    " SELECT d.ch_dishno as 'dishno' ,"

                                    " space(5) + d.vch_dishname as 'dishname' ,"

                                    " d.ch_unitno as 'unit' ,"

                                    " sum((num_num - num_back)) as 'num' ,"

                                    " sum(num_price_add) as 'amount' ,"

                                    " sum(((num_price_add) * (100 - int_discount) * 0.01)) as 'discount' ,"

                                    " sum((case ch_presentflag"

                                    " when 'Y' then"

                                    " (num_price_add) * int_discount * 0.01"

                                    " else"

                                    " 0.00"

                                    " end)) as 'present' ,"

                                    " replace(a.ch_suitno, ' ', '') as 'suitno' ,"

                                    " '1' as 'orders'"

                                    " FROM cey_u_orderdish a, cey_u_checkout_master c, cey_bt_dish d"

                                    " WHERE a.ch_payno = c.ch_payno"

                                    " and c.ch_state = 'Y'"

                                    " and (a.ch_suitflag = 'Y')"

                                    " and a.ch_dishno = d.ch_dishno"

                                    " and (DATE_FORMAT(C.dt_operdate, '%Y-%m-%d %h:%i:%s') >='%1')"

                                    " and (DATE_FORMAT(C.dt_operdate, '%Y-%m-%d %h:%i:%s') <='%2')"

                                    " and a.ch_suitno like '%3%'"

                                    " and (DATE_FORMAT(c.dt_operdate, '%Y') >='0')"

                                    " and (DATE_FORMAT(c.dt_operdate, '%Y') <='3')"

                                    " group by d.ch_dishno, d.vch_dishname, d.ch_unitno, a.ch_suitno"
                                    )),
                            w_rpt_report::querymodelsetfilters("")
                            ));


}

void w_rpt_report_promotionreport::tab2sqlinit()
{

}

void w_rpt_report_promotionreport::tab3sqlinit()
{

}

void w_rpt_report_promotionreport::tab4sqlinit()
{

}

void w_rpt_report_promotionreport::tab5sqlinit()
{

}

void w_rpt_report_promotionreport::toselect()
{
    QString ls_begin;
    QString ls_end;
    ls_begin=n_func::uf_getdatetime("begin", ui->dateTimeEdit->dateTime()).toString("yyyy-MM-dd hh:mm");
    ls_end=n_func::uf_getdatetime("end", ui->dateTimeEdit_2->dateTime()).toString("yyyy-MM-dd hh:mm");

    //model select
    querymodel->setQuery(tab1info[ui->comboBox_11->currentText()].sql
            .arg(ls_begin.isEmpty()?ui->dateTimeEdit->dateTime().toString("yyyy-MM-dd hh:mm:ss"):ls_begin)
            .arg(ls_end.isEmpty()?ui->dateTimeEdit_2->dateTime().toString("yyyy-MM-dd hh:mm:ss"):ls_end)
            .arg(backheader::getinterdata(ui->comboBox_12->currentText()))//获取过滤信息
            .arg(w_rpt_report::querymodelordersort(ui->tableView->tableView))
            );
    //更新表头
    backheader::modelSetHeader(querymodel, tab1info[ui->comboBox_11->currentText()].headers);
    //更新合计    //制表人 和 制表时间
    ui->label_operator->setText(n_func::gs_opername);
    ui->label_operattime->setText(n_func::f_get_sysdatetime().toString("yyyy-MM-dd hh:mm:ss"));
    ui->label_timefrom->setText(ls_begin);
    ui->label_timeto->setText(ls_end);

    QString sumsql=querymodel->query().lastQuery().trimmed();
    qDebug() << sumsql;
    if(sumsql.endsWith("DESC")||sumsql.endsWith("ASC")){
        //5 6 7  9 10
        QSqlQuery query;

        if(tab1info[ui->comboBox_11->currentText()].except_c.isEmpty()){
            query.exec(QString("select sum(tabname3),sum(tabname4),sum(tabname5),sum(tabname6),sum(tabname7),sum(tabname8),sum(tabname9) from (%1)t1 ").arg(sumsql.left(sumsql.lastIndexOf("ORDER BY"))));
            query.next();

            ui->tableView->updateSum(3,query.record().value(0).toString());
            ui->tableView->updateSum(4,query.record().value(1).toString());
            ui->tableView->updateSum(5,query.record().value(2).toString());
            ui->tableView->updateSum(6,query.record().value(3).toString());
            ui->tableView->updateSum(7,query.record().value(4).toString());
            ui->tableView->updateSum(8,query.record().value(5).toString());
            ui->tableView->updateSum(9,query.record().value(6).toString());

        } else {
            query.exec(QString("select sum(tabname3),sum(tabname4),sum(tabname5),sum(tabname6),sum(tabname7) from (%1)t1 ").arg(sumsql.left(sumsql.lastIndexOf("ORDER BY"))));
            query.next();

            ui->tableView->updateSum(3,query.record().value(0).toString());
            ui->tableView->updateSum(4,query.record().value(1).toString());
            ui->tableView->updateSum(5,query.record().value(2).toString());
            ui->tableView->updateSum(6,query.record().value(3).toString());
            ui->tableView->updateSum(7,query.record().value(4).toString());
        }
        qDebug() << query.lastQuery();
    }
}

void w_rpt_report_promotionreport::toexit()
{
    this->reject();
}

void w_rpt_report_promotionreport::tocom12(const QString &text)
{

}
