#include "w_rpt_report_directsalegrossprofitreport.h"
//直销商品毛利情况表
#include "ui_w_rpt_report_directsalegrossprofitreport_dialog.h"
#include <QSqlQueryModel>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlRecord>
#include "n_func.h"
#include "lds_tableview.h"
#include <QSqlError>
#include "backheader.h"
w_rpt_report_directsalegrossprofitreport::w_rpt_report_directsalegrossprofitreport(QWidget *parent) :
    QDialog(parent),
    ui(new Ui_w_rpt_report_directsalegrossprofitreport_Dialog)
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

    //! toolbar
    ui->widget->setup(QStringList()<<tr("查询")<<tr("退出"), Qt::AlignTop);
    connect(ui->widget->index_widget(tr("查询")),SIGNAL(clicked()),this,SLOT(toselect()));
    connect(ui->widget->index_widget(tr("退出")),SIGNAL(clicked()),this,SLOT(toexit()));
    connect(ui->tableView->tableView->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(toselect()));

    //! after
    ui->comboBox_series->addItems(w_rpt_report::querymodelsetfilters("select * from cey_bt_dish_type"));
    //tabwidget header
    ui->tabWidget->setTabText(0, tr("直销商品毛利情况表"));

}

w_rpt_report_directsalegrossprofitreport::~w_rpt_report_directsalegrossprofitreport()
{

}

void w_rpt_report_directsalegrossprofitreport::tab1sqlinit()
{
    tab1info.insert(tr("直销商品毛利情况表"),
                    w_rpt_report::TABTYPE(QStringList()<<tr("餐桌")<<tr("品码")<<tr("品名")<<tr("单位")<<tr("数量")<<tr("消费金额")<<tr("成本金额")<<tr("毛利额")<<tr("毛利率"),
                            QString (

                                "select (select cey_bt_dish_type.vch_dish_typename from cey_bt_dish_type where cey_bt_dish_type.ch_dish_typeno=t.ch_dish_typeno) as 'tabname0' ,"

                                " ch_dishno as 'tabname1' ,"

                                " vch_dishname as 'tabname2' ,"

                                " (select vch_unitname from cey_bt_unit where cey_bt_unit.ch_unitno=t.ch_unitno) as 'tabname3' ,"

                                " num as 'tabname4' ,"

                                " num_amt as 'tabname5' ,"

                                " num_ck_amt as 'tabname6' ,"

                                " num_amt - num_ck_amt as 'tabname7' ,"

                                " (num_amt - num_ck_amt) / num_amt as 'tabname8' "

                                " from (select C.ch_dish_typeno,"

                                " B.ch_dishno,"

                                " C.vch_dishname,"

                                " c.ch_unitno,"

                                " sum(B.num_num - B.num_back) num,"

                                " sum((B.num_num - B.num_back) *"

                                " (B.num_price + B.num_price_add) * int_discount * 0.01) num_amt,"

                                " sum(num_kc_amt) num_ck_amt"

                                " from (select ch_sheetno,"

                                " sum(-num_num * int_flag) num_kc,"

                                " sum(-num_num * int_flag * cost_price) num_kc_amt"

                                " from cey_st_material_flow"

                                " group by ch_sheetno) A,"

                                " cey_u_orderdish B,"

                                " cey_bt_dish C,"

                                " cey_u_checkout_master D"

                                " where LEFT(A.ch_sheetno,60) = LEFT(B.int_flowid,60)"

                                " and B.ch_dishno = C.ch_dishno"

                                " and D.ch_payno = B.ch_payno"

                                " and ch_state = 'Y'"

                                " and DATE_FORMAT(D.dt_operdate, '%Y-%m-%d %h:%i:%s') >= '%1'"

                                " and DATE_FORMAT(D.dt_operdate, '%Y-%m-%d %h:%i:%s') <= '%2'"

                                " and C.ch_dish_typeno like '%3%'"

                                " group by C.ch_dish_typeno, B.ch_dishno, C.vch_dishname, c.ch_unitno) t ORDER BY tabname%4"
                                ),
                            w_rpt_report::querymodelsetfilters("")
                            ));
}

void w_rpt_report_directsalegrossprofitreport::toselect()
{
    QString ls_begin;
    QString ls_end;
    ls_begin=n_func::uf_getdatetime("begin", ui->dateTimeEdit->dateTime()).toString("yyyy-MM-dd hh:mm");
    ls_end=n_func::uf_getdatetime("end", ui->dateTimeEdit_2->dateTime()).toString("yyyy-MM-dd hh:mm");

    //model select
    querymodel->setQuery(tab1info[ui->tabWidget->tabText(0)].sql
            .arg(ls_begin.isEmpty()?ui->dateTimeEdit->dateTime().toString("yyyy-MM-dd hh:mm:ss"):ls_begin)
            .arg(ls_end.isEmpty()?ui->dateTimeEdit_2->dateTime().toString("yyyy-MM-dd hh:mm:ss"):ls_end)
            .arg(backheader::getinterdata(ui->comboBox_series->currentText()))//获取过滤信息
            .arg(w_rpt_report::querymodelordersort(ui->tableView->tableView))
            );
    //更新表头
    backheader::modelSetHeader(querymodel, tab1info[tr("直销商品毛利情况表")].headers);
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
        query.exec(QString("select sum(tabname5),sum(tabname6),sum(tabname7),sum(tabname8)  from (%1)t1 ").arg(sumsql.left(sumsql.lastIndexOf("ORDER BY"))));
        query.next();
        qDebug() <<query.lastQuery();

        ui->tableView->updateSum(5,query.record().value(0).toString());
        ui->tableView->updateSum(6,query.record().value(1).toString());
        ui->tableView->updateSum(7,query.record().value(2).toString());
        ui->tableView->updateSum(8,query.record().value(3).toString());

    }
}

void w_rpt_report_directsalegrossprofitreport::toexit()
{
    this->reject();
}

void w_rpt_report_directsalegrossprofitreport::tocom12(const QString &text)
{

}
