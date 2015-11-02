#include "w_rpt_report_dishsalerank.h"
//菜品销售排行统计
#include "ui_w_rpt_report_directsalegrossprofitreport_dialog.h"
#include <QSqlQueryModel>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlRecord>
#include "n_func.h"
#include "lds_tableview.h"
#include <QSqlError>
#include "backheader.h"


int w_rpt_report_dishsalerank::RankQueryModel::columnCount(const QModelIndex &parent) const
{
    return QSqlQueryModel::columnCount() + 1;
}

QVariant w_rpt_report_dishsalerank::RankQueryModel::data(const QModelIndex &item, int role) const
{
    qlonglong c_count=RankQueryModel::columnCount();
    qlonglong c_count1=QSqlQueryModel::columnCount();
    if(role==Qt::DisplayRole){
        if(item.column() >= c_count1 && item.column() < c_count){//总列数之外
            if(QSqlQueryModel::data(item, role).toString().isEmpty()){//为空
                double _ldex_itemamount=this->record(item.row()).value("amount").toDouble();
                if(_ldec_cost > 0.0001)//除数不为0
                    return QString().sprintf("%.02f", (_ldex_itemamount * _ldec_income) /_ldec_cost);
                else
                    return "";
            }
        }
    }
    return QSqlQueryModel::data(item, role);
}


w_rpt_report_dishsalerank::w_rpt_report_dishsalerank(QWidget *parent) :
    QDialog(parent),
    ui(new Ui_w_rpt_report_directsalegrossprofitreport_Dialog)
{
    ui->setupUi(this);
    ui->frame->setFixedWidth(w_rpt_report::frame_left_fixed_width);
    //! init
    ui->dateTimeEdit->setDateTime(QDateTime(n_func::f_get_sysdatetime().date(), QTime(0,0,0)));
    ui->dateTimeEdit_2->setDateTime(QDateTime(n_func::f_get_sysdatetime().date(), QTime(23,59,59)));

    tab1sqlinit();

    querymodel=new RankQueryModel;

    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);ui->tableView->setModel(querymodel);

    ui->comboBox_series->addItems(tab1info.keys());
    //! toolbar
    ui->widget->setup(QStringList()<<tr("查询")<<tr("退出"), Qt::AlignTop);
    connect(ui->widget->index_widget(tr("查询")),SIGNAL(clicked()),this,SLOT(toselect()));
    connect(ui->widget->index_widget(tr("退出")),SIGNAL(clicked()),this,SLOT(toexit()));
    //    connect(ui->tableView->tableView->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(toselect()));

    //! after
    ui->tabWidget->setTabText(0, "");

    //tabwidget header
    ui->tabWidget->setTabText(0, tr("菜品销售排行统计"));
}

void w_rpt_report_dishsalerank::tab1sqlinit()
{
    tab1info.insert(tr("按菜品明细"),
                    w_rpt_report::TABTYPE(QStringList()<<tr("菜品编码")<<tr("菜品名称")<<tr("数量")<<tr("消费金额")<<tr("收入金额"),
                                          QString (
                                              "Select a.ch_dishno as 'itemno',"

                                              " c.vch_dishname as 'itemname',"

                                              " IFNULL(sum(a.num_num - a.num_back), 0) as 'num',"

                                              " IFNULL(sum(case when a.ch_presentflag = 'Y' then"

                                              " 0"

                                              " else"

                                              " (a.num_num - a.num_back) * (a.num_price + a.num_price_add) *"

                                              " a.int_discount * 0.01"

                                              " end),"

                                              " 0) as 'amount'"

                                              " from cey_u_orderdish a, cey_u_checkout_master b, cey_bt_dish c"

                                              " where (a.ch_dishno = c.ch_dishno)"

                                              " and (a.ch_payno = b.ch_payno)"

                                              " and (b.ch_state = 'Y')"

                                              " and b.dt_operdate >= '%1'"

                                              " and b.dt_operdate <= '%2'"

                                              " Group by a.ch_dishno, c.vch_dishname"
                                              )
                                          )
                    );
    tab1info.insert(tr("按菜品类型"),
                    w_rpt_report::TABTYPE(QStringList()<<tr("菜品编码")<<tr("菜品名称")<<tr("数量")<<tr("消费金额")<<tr("收入金额"),
                                          QString (

                                              "Select c.ch_dish_typeno as 'itemno',"

                                              " d.vch_dish_typename as 'itemname',"

                                              " IFNULL(sum(a.num_num - a.num_back), 0) as 'num',"

                                              " IFNULL(sum(case when a.ch_presentflag = 'Y' then"

                                              " 0"

                                              " else"

                                              " (a.num_num - a.num_back) * (a.num_price + a.num_price_add) "

                                              "*"

                                              " a.int_discount * 0.01"

                                              " end),"

                                              " 0) as 'amount'"

                                              " from cey_u_orderdish a,"

                                              " cey_u_checkout_master b,"

                                              " cey_bt_dish c,"

                                              " cey_bt_dish_type d"

                                              " where (a.ch_dishno = c.ch_dishno)"

                                              " and (c.ch_dish_typeno = d.ch_dish_typeno)"

                                              " and (a.ch_payno = b.ch_payno)"

                                              " and (b.ch_state = 'Y')"

                                              " and b.dt_operdate >= '%1'"

                                              " and b.dt_operdate <= '%2'"

                                              " Group by c.ch_dish_typeno, d.vch_dish_typename"

                                              " order by c.ch_dish_typeno"
                                              )
                                          )
                    );
    tab1info.insert(tr("按餐桌号"),
                    w_rpt_report::TABTYPE(QStringList()<<tr("桌类编号")<<tr("桌类名称")<<tr("数量")<<tr("消费金额")<<tr("收入金额"),
                                          QString (
                                              "Select a.ch_tableno as 'itemno',"

                                              " c.vch_tablename as 'itemname',"

                                              " IFNULL(sum(a.num_num - a.num_back), 0) as 'num',"

                                              " IFNULL(sum(case when a.ch_presentflag = 'Y' then"

                                              " 0"

                                              " else"

                                              " (a.num_num - a.num_back) * (a.num_price + a.num_price_add) *"

                                              " a.int_discount * 0.01"

                                              " end),"

                                              " 0) as 'amount'"

                                              " from cey_u_orderdish a,"

                                              " cey_u_checkout_master b,"

                                              " (select ch_tableno, vch_tablename"

                                              " from cey_bt_table"

                                              " union"

                                              " select 'Q000' as 'ch_tableno', 'Q000' as 'vch_tablename') as c"

                                              " where (a.ch_tableno = c.ch_tableno)"

                                              " and (a.ch_payno = b.ch_payno)"

                                              " and (b.ch_state = 'Y')"

                                              " and b.dt_operdate >= '%1'"

                                              " and b.dt_operdate <= '%2'"

                                              " Group by a.ch_tableno, c.vch_tablename"
                                              )
                                          )
                    );
    tab1info.insert(tr("按餐桌类型"),
                    w_rpt_report::TABTYPE(QStringList()<<tr("桌类编码")<<tr("桌类名称")<<tr("数量")<<tr("消费金额")<<tr("收入金额"),
                                          QString (
                                              "Select c.ch_typeno as 'itemno',"

                                              " d.vch_typename as 'itemname',"

                                              " IFNULL(sum(a.num_num - a.num_back), 0) as 'num',"

                                              " IFNULL(sum(case"

                                              " when a.ch_presentflag = 'Y' then"

                                              " 0"

                                              " else"

                                              " (a.num_num - a.num_back) * (a.num_price + a.num_price_add) *"

                                              " a.int_discount * 0.01"

                                              " end),"

                                              " 0) as 'amount'"

                                              " from cey_u_orderdish a,"

                                              " cey_u_checkout_master b,"

                                              " (select ch_tableno, vch_tablename, ch_typeno"

                                              " from cey_bt_table"

                                              " union"

                                              " select 'Q000' as 'ch_tableno',"

                                              " '"+tr("快餐")+"Q000' as 'vch_tablename',"

                                              " 'Q0' as 'ch_typeno') as c,"

                                              " (select ch_typeno, vch_typename"

                                              " from cey_bt_table_type"

                                              " union"

                                              " select 'Q0' as 'ch_typeno', '"+tr("快餐")+"Q000' as 'vch_typename') as d"

                                              " where (a.ch_tableno = c.ch_tableno)"

                                              " and (c.ch_typeno = d.ch_typeno)"

                                              " and (a.ch_payno = b.ch_payno)"

                                              " and (b.ch_state = 'Y')"

                                              " and b.dt_operdate >= '%1'"

                                              " and b.dt_operdate <= '%2'"

                                              " Group by c.ch_typeno, d.vch_typename"
                                              )
                                          )
                    );
    tab1info.insert(tr("按班次营业时段"),
                    w_rpt_report::TABTYPE(QStringList()<<tr("班次编码")<<tr("班次名称")<<tr("数量")<<tr("消费金额")<<tr("收入金额"),
                                          QString (
                                              "Select BB.itemno AS 'itemno',"

                                              " AA.vch_classname AS 'itemname',"

                                              " sum(BB.num) AS 'num',"

                                              " sum(BB.amount) AS 'amount'"

                                              " From (Select IFNULL((Case"

                                              " When DATE_FORMAT(b.dt_operdate, '%Y-%m-%d') < '09:33:00' Then"

                                              " '11'"

                                              " Else"

                                              " (Select ch_classno"

                                              " From cey_bt_class"

                                              " where DATE_FORMAT(dt_begin, '%Y-%m-%d') < DATE_FORMAT(b.dt_operdate, '%Y-%m-%d')"

                                              " Order By dt_begin Desc LIMIT 1)"

                                              " End),"

                                              " '11') AS 'itemno',"

                                              " IFNULL(sum(a.num_num - a.num_back), 0) AS 'num',"

                                              " IFNULL(sum(case"

                                              " when a.ch_presentflag = 'Y' then"

                                              " 0"

                                              " else"

                                              " (a.num_num - a.num_back) * (a.num_price + a.num_price_add) *"

                                              " a.int_discount * 0.01"

                                              " end),"

                                              " 0) AS 'amount'"

                                              " from cey_u_orderdish a, cey_u_checkout_master b"

                                              " Where (a.ch_payno = b.ch_payno)"

                                              " and (b.ch_state = 'Y')"

                                              " and b.dt_operdate >= '%1'"

                                              " and b.dt_operdate <= '%2'"

                                              " Group by b.dt_operdate) BB,"

                                              " cey_bt_class AA"

                                              " Where BB.itemno = AA.ch_classno"

                                              " Group By BB.itemno, AA.vch_classname"
                                              )
                                          )
                    );

}

void w_rpt_report_dishsalerank::toselect()
{
    //获取计算应收金额的 两个param
    QSqlQuery query;
    query.exec(
                QString(
                    "Select IFNULL(sum(a.num_realamount), 0)"

                    " from cey_u_checkout_detail a, cey_u_checkout_master b, cey_bt_paymode c"

                    " where a.ch_payno = b.ch_payno"

                    " and a.ch_paymodeno = c.ch_paymodeno"

                    " and a.ch_paymodeno <> 'YY'"

                    " and c.ch_incomeflag = 'Y'"

                    " and b.ch_state = 'Y'"

                    " and b.dt_operdate >= '%1'"

                    " and b.dt_operdate <= '%2' "
                    )
                .arg(ui->dateTimeEdit->dateTime().toString("yyyy-MM-dd hh:mm:ss"))
                .arg(ui->dateTimeEdit_2->dateTime().toString("yyyy-MM-dd hh:mm:ss"))
                );
    query.next();
    querymodel->_ldec_income=query.record().value(0).toDouble();
    query.exec(
                QString(
                    "Select IFNull(sum((a.num_num - a.num_back) *"

                    " (a.num_price + a.num_price_add) * a.int_discount * 0.01),"

                    " 0)"

                    " from cey_u_orderdish a, cey_u_checkout_master b"

                    " where a.ch_payno = b.ch_payno"

                    " and b.ch_state = 'Y'"

                    " and (a.ch_presentflag = 'N')"

                    " and b.dt_operdate >= '%1'"

                    " and b.dt_operdate <= '%2' "
                    )
                .arg(ui->dateTimeEdit->dateTime().toString("yyyy-MM-dd hh:mm:ss"))
                .arg(ui->dateTimeEdit_2->dateTime().toString("yyyy-MM-dd hh:mm:ss"))
                );
    query.next();
    querymodel->_ldec_cost=query.record().value(0).toDouble();
    //获取日营业时间段
    QString ls_begin;
    QString ls_end;
    ls_begin=n_func::uf_getdatetime("begin", ui->dateTimeEdit->dateTime()).toString("yyyy-MM-dd hh:mm");
    ls_end=n_func::uf_getdatetime("end", ui->dateTimeEdit_2->dateTime()).toString("yyyy-MM-dd hh:mm");

    //model select
    querymodel->setQuery(tab1info[ui->comboBox_series->currentText()].sql
            .arg(ls_begin.isEmpty()?ui->dateTimeEdit->dateTime().toString("yyyy-MM-dd hh:mm:ss"):ls_begin)
            .arg(ls_end.isEmpty()?ui->dateTimeEdit_2->dateTime().toString("yyyy-MM-dd hh:mm:ss"):ls_end)
            //            .arg(w_rpt_report::querymodelordersort(ui->tableView->tableView))
            );
    //更新表头
    backheader::modelSetHeader(querymodel, tab1info[ui->comboBox_series->currentText()].headers);
    //制表人 和 制表时间
    ui->label_operator->setText(n_func::gs_opername);
    ui->label_operattime->setText(n_func::f_get_sysdatetime().toString("yyyy-MM-dd hh:mm:ss"));
    ui->label_timefrom->setText(ls_begin);
    ui->label_timeto->setText(ls_end);
    //更新合计
    ui->tableView->updateSum(2);
    ui->tableView->updateSum(3);
    ui->tableView->updateSum(4);
}

void w_rpt_report_dishsalerank::toexit()
{
    this->reject();
}

void w_rpt_report_dishsalerank::tocom12(const QString &text)
{

}

