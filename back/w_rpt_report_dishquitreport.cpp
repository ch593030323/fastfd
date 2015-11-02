#include "w_rpt_report_dishquitreport.h"
//菜品退菜报表
#include "ui_w_rpt_report_cashreport_dialog.h"
#include "n_func.h"
#include "backheader.h"
w_rpt_report_dishquitreport::w_rpt_report_dishquitreport(QWidget *parent) :
    QDialog(parent),ui(new Ui_w_rpt_report_cashreport_Dialog)
{
    //! init
    ui->setupUi(this);
    ui->frame->setFixedWidth(w_rpt_report::frame_left_fixed_width);
    ui->dateTimeEdit->setDateTime(QDateTime(n_func::f_get_sysdatetime().date(), QTime(0,0,0)));
    ui->dateTimeEdit_2->setDateTime(QDateTime(n_func::f_get_sysdatetime().date(), QTime(23,59,59)));

    ui->tableView->tableView->setSortingEnabled(true);

    tab1sqlinit();

    querymodel=new datetimeModel;

    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);ui->tableView->setModel(querymodel);

    ui->comboBox_11->addItems(tab1info.keys());
    //! toolbar
    ui->widget->setup(QStringList()<<tr("查询")<<tr("退出"), Qt::AlignTop);
    connect(ui->widget->index_widget(tr("查询")),SIGNAL(clicked()),this,SLOT(toselect()));
    connect(ui->widget->index_widget(tr("退出")),SIGNAL(clicked()),this,SLOT(toexit()));
    connect(ui->comboBox_11,SIGNAL(currentIndexChanged(QString)),this,SLOT(tocom12(QString)));
    connect(ui->tableView->tableView->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(toselect()));

    //! after
    tocom12(ui->comboBox_11->currentText());

    //tabwidget header
    ui->tabWidget->setTabText(0, tr("菜品退菜报表"));

}

void w_rpt_report_dishquitreport::tab1sqlinit()
{
    tab1info.insert(tr("按菜品类型"),
                    w_rpt_report::TABTYPE(QStringList()<<tr("菜品类型")<<tr("品码")<<tr("品名")<<tr("单位")<<tr("均价")<<tr("数量")<<tr("金额")<<tr("折扣额")<<tr("赠送额")<<tr("实际金额"),
                            QString("select "
                                    " (select vch_dish_typename from cey_bt_dish_type where cey_bt_dish_type.ch_dish_typeno=dish_typeno) as 'tabname0',"
                                    " dishno as 'tabname1',"
                                    " dishname as 'tabname2',"
                                    " (select vch_unitname from cey_bt_unit where cey_bt_unit.ch_unitno=dish_unitno) as 'tabname3',"
                                    " (case t.num when 0 then 0 else t.amount/t.num end) as 'tabname4',"
                                    " t.num as 'tabname5',"
                                    " t.amount as 'tabname6',"
                                    " t.discount as 'tabname7',"
                                    " t.present as 'tabname8',"
                                    " t.amount-t.discount-t.present as 'tabname9'"
                                    " from (%1)t  ORDER BY tabname%4").arg(
                                QString (
                                    "Select dish_typeno,"

                                    " dish_typeno as 'dish_typeno1',"

                                    " dishno,"

                                    " dishname,"

                                    " dish_unitno,"

                                    " sum(num) as 'num',"

                                    " sum(amount) as 'amount',"

                                    " sum(discount) as 'discount',"

                                    " sum(present) as 'present',"

                                    " typeno"

                                    " from (SELECT c.ch_dish_typeno as 'dish_typeno',"

                                    " a.ch_dishno as 'dishno',"

                                    " c.vch_dishname as 'dishname',"

                                    " c.ch_unitno as 'dish_unitno',"

                                    " a.num_back as 'num',"

                                    " a.num_back * (a.num_price + a.num_price_add) as 'amount',"

                                    " a.num_back * (a.num_price + a.num_price_add) *"

                                    " (100 - a.int_discount) * 0.01 as 'discount',"

                                    " (case a.ch_presentflag"

                                    " when 'Y' then"

                                    " a.num_back * (a.num_price + a.num_price_add) *"

                                    " a.int_discount * 0.01"

                                    " else"

                                    " 0"

                                    " end) as 'present',"

                                    " c.ch_dish_typeno as 'typeno'"

                                    " FROM cey_u_orderdish a, cey_u_checkout_master b, cey_bt_dish c"

                                    " where (a.ch_payno = b.ch_payno)"

                                    " and (b.ch_state = 'Y')"

                                    " and (a.num_back <> 0)"

                                    " and (a.ch_dishno = c.ch_dishno)"

                                    " and (c.ch_dish_typeno like '%3%')"

                                    " and (DATE_FORMAT(b.dt_operdate,'%Y-%m-%d %h:%i:%s') >= '%1')"

                                    " and (DATE_FORMAT(b.dt_operdate,'%Y-%m-%d %h:%i:%s') <= '%2')"

                                    ""

                                    " union all"

                                    " SELECT c.ch_dish_typeno as 'dish_typeno',"

                                    " a.ch_dishno as 'dishno',"

                                    " c.vch_dishname as 'dishname',"

                                    " c.ch_unitno as 'dish_unitno',"

                                    " a.num_num - a.num_back as 'num',"

                                    " '' ="

                                    " (a.num_num - a.num_back) * (a.num_price + a.num_price_add) as 'amount',"

                                    " '' ="

                                    " (a.num_num - a.num_back) * (a.num_price + a.num_price_add) *"

                                    " (100 - a.int_discount) * 0.01 as 'discount',"

                                    " '' ="

                                    " (case a.ch_presentflag"

                                    " when 'Y' then"

                                    " (a.num_num - a.num_back) * (a.num_price + a.num_price_add) *"

                                    " a.int_discount * 0.01"

                                    " else"

                                    " 0"

                                    " end) as 'present',"

                                    " c.ch_dish_typeno as 'typeno'"

                                    " FROM cey_u_orderdish a, cey_u_checkout_master b, cey_bt_dish c"

                                    " where (a.ch_payno = b.ch_payno)"

                                    " and (b.ch_state = 'P')"

                                    " and (a.num_num - a.num_back <> 0)"

                                    " and (a.ch_dishno = c.ch_dishno)"

                                    " and (c.ch_dish_typeno like '%3%')"

                                    " and (DATE_FORMAT(b.dt_operdate2,'%Y-%m-%d %h:%i:%s') >= '%1')"

                                    " and (DATE_FORMAT(b.dt_operdate2,'%Y-%m-%d %h:%i:%s') <= '%2')"

                                    ""

                                    " union all"

                                    " SELECT c.ch_dish_typeno as 'dish_typeno',"

                                    " a.ch_dishno as 'dishno',"

                                    " c.vch_dishname as 'dishname',"

                                    " c.ch_unitno as 'dish_unitno',"

                                    " a.num_num - a.num_back as 'num',"

                                    " '' ="

                                    " (a.num_num - a.num_back) * (a.num_price + a.num_price_add) as 'amount',"

                                    " '' ="

                                    " (a.num_num - a.num_back) * (a.num_price + a.num_price_add) *"

                                    " (100 - a.int_discount) * 0.01 as 'discount',"

                                    " '' ="

                                    " (case a.ch_presentflag"

                                    " when 'Y' then"

                                    " (a.num_num - a.num_back) * (a.num_price + a.num_price_add) *"

                                    " a.int_discount * 0.01"

                                    " else"

                                    " 0"

                                    " end) as 'present',"

                                    " c.ch_dish_typeno as 'typeno'"

                                    " FROM cey_u_orderdish a, cey_u_togo b, cey_bt_dish c"

                                    " where (a.ch_togono = b.ch_togono)"

                                    " and (b.ch_state = 'P')"

                                    " and (a.num_num - a.num_back <> 0)"

                                    " and (a.ch_dishno = c.ch_dishno)"

                                    " and (c.ch_dish_typeno like '%3%')"

                                    " and (DATE_FORMAT(b.dt_operdate,'%Y-%m-%d %h:%i:%s') >= '%1')"

                                    " and (DATE_FORMAT(b.dt_operdate,'%Y-%m-%d %h:%i:%s') <= '%2')"

                                    ")cc"

                                    " Group by dish_typeno, dishno, dishname, dish_unitno, typeno"
                                    )),
                            w_rpt_report::querymodelsetfilters("select * from cey_bt_dish_type")
                            ));

    tab1info.insert(tr("按餐桌"),
                    w_rpt_report::TABTYPE(QStringList()<<tr("餐桌")<<tr("品码")<<tr("品名")<<tr("单位")<<tr("均价")<<tr("数量")<<tr("金额")<<tr("折扣额")<<tr("赠送额")<<tr("实际金额"),

                            QString("select "
                                    " (select vch_tablename  from cey_bt_table where cey_bt_table.ch_tableno=dish_typeno) as 'tabname0',"
                                    " dishno as 'tabname1',"
                                    " dishname as 'tabname2',"
                                    " (select vch_unitname from cey_bt_unit where cey_bt_unit.ch_unitno=dish_unitno) as 'tabname3',"
                                    " (case t.num when 0 then 0 else t.amount/t.num end) as 'tabname4',"
                                    " t.num as 'tabname5',"
                                    " t.amount as 'tabname6',"
                                    " t.discount as 'tabname7',"
                                    " t.present as 'tabname8',"
                                    " t.amount-t.discount-t.present as 'tabname9'"
                                    " from (%1)t  ORDER BY tabname%4").arg(
                                QString (
                                    "Select dish_typeno,"

                                    " dish_typeno as 'dish_typeno1',"

                                    " dishno,"

                                    " dishname,"

                                    " dish_unitno,"

                                    " sum(num) as 'num',"

                                    " sum(amount) as 'amount',"

                                    " sum(discount) as 'discount',"

                                    " sum(present) as 'present',"

                                    " typeno"

                                    " from (SELECT a.ch_tableno as 'dish_typeno',"

                                    " a.ch_dishno as 'dishno',"

                                    " c.vch_dishname as 'dishname',"

                                    " c.ch_unitno as 'dish_unitno',"

                                    " a.num_back as 'num',"

                                    " a.num_back * (a.num_price + a.num_price_add) as 'amount',"

                                    " a.num_back * (a.num_price + a.num_price_add) *"

                                    " (100 - a.int_discount) * 0.01 as 'discount',"

                                    " (case a.ch_presentflag"

                                    " when 'Y' then"

                                    " a.num_back * (a.num_price + a.num_price_add) *"

                                    " a.int_discount * 0.01"

                                    " else"

                                    " 0"

                                    " end) as 'present',"

                                    " c.ch_dish_typeno as 'typeno'"

                                    " FROM cey_u_orderdish a, cey_u_checkout_master b, cey_bt_dish c"

                                    " where (a.ch_payno = b.ch_payno)"

                                    " and (b.ch_state = 'Y')"

                                    " and (a.num_back <> 0)"

                                    " and (a.ch_dishno = c.ch_dishno)"

                                    " and (a.ch_tableno like '%3%')"

                                    " and (DATE_FORMAT(b.dt_operdate,'%Y-%m-%d %h:%i:%s') >= '%1')"

                                    " and (DATE_FORMAT(b.dt_operdate,'%Y-%m-%d %h:%i:%s') <= '%2')"

                                    " and (DATE_FORMAT(b.dt_operdate,'%Y-%m-%d') >= '0')"

                                    " and (DATE_FORMAT(b.dt_operdate,'%Y-%m-%d') <= '3')"

                                    ""

                                    " union all"

                                    " SELECT a.ch_tableno as 'dish_typeno',"

                                    " a.ch_dishno as 'dishno',"

                                    " c.vch_dishname as 'dishname',"

                                    " c.ch_unitno as 'dish_unitno',"

                                    " a.num_num - a.num_back as 'num',"

                                    " '' ="

                                    " (a.num_num - a.num_back) * (a.num_price + a.num_price_add) as 'amount',"

                                    " '' ="

                                    " (a.num_num - a.num_back) * (a.num_price + a.num_price_add) *"

                                    " (100 - a.int_discount) * 0.01 as 'discount',"

                                    " '' ="

                                    " (case a.ch_presentflag"

                                    " when 'Y' then"

                                    " (a.num_num - a.num_back) * (a.num_price + a.num_price_add) *"

                                    " a.int_discount * 0.01"

                                    " else"

                                    " 0"

                                    " end) as 'present',"

                                    " c.ch_dish_typeno as 'typeno'"

                                    " FROM cey_u_orderdish a, cey_u_checkout_master b, cey_bt_dish c"

                                    " where (a.ch_payno = b.ch_payno)"

                                    " and (b.ch_state = 'P')"

                                    " and (a.num_num - a.num_back <> 0)"

                                    " and (a.ch_dishno = c.ch_dishno)"

                                    " and (a.ch_tableno like '%3%')"

                                    " and (DATE_FORMAT(b.dt_operdate,'%Y-%m-%d %h:%i:%s') >= '%1')"

                                    " and (DATE_FORMAT(b.dt_operdate,'%Y-%m-%d %h:%i:%s') <= '%2')"

                                    " and (DATE_FORMAT(b.dt_operdate,'%Y-%m-%d') >= '0')"

                                    " and (DATE_FORMAT(b.dt_operdate,'%Y-%m-%d') <= '3')"

                                    " union all"

                                    " SELECT a.ch_tableno as 'dish_typeno',"

                                    " a.ch_dishno as 'dishno',"

                                    " c.vch_dishname as 'dishname',"

                                    " c.ch_unitno as 'dish_unitno',"

                                    " a.num_num - a.num_back as 'num',"

                                    " '' ="

                                    " (a.num_num - a.num_back) * (a.num_price + a.num_price_add) as 'amount',"

                                    " '' ="

                                    " (a.num_num - a.num_back) * (a.num_price + a.num_price_add) *"

                                    " (100 - a.int_discount) * 0.01 as 'discount',"

                                    " '' ="

                                    " (case a.ch_presentflag"

                                    " when 'Y' then"

                                    " (a.num_num - a.num_back) * (a.num_price + a.num_price_add) *"

                                    " a.int_discount * 0.01"

                                    " else"

                                    " 0"

                                    " end) as 'present',"

                                    " c.ch_dish_typeno as 'typeno'"

                                    " FROM cey_u_orderdish a, cey_u_togo b, cey_bt_dish c"

                                    " where (a.ch_togono = b.ch_togono)"

                                    " and (b.ch_state = 'P')"

                                    " and (a.num_num - a.num_back <> 0)"

                                    " and (a.ch_dishno = c.ch_dishno)"

                                    " and (a.ch_tableno like '%3%')"

                                    " and (DATE_FORMAT(b.dt_operdate,'%Y-%m-%d %h:%i:%s') >= '%1')"

                                    " and (DATE_FORMAT(b.dt_operdate,'%Y-%m-%d %h:%i:%s') <= '%2')"

                                    " and (DATE_FORMAT(b.dt_operdate,'%Y-%m-%d') >= '0')"

                                    " and (DATE_FORMAT(b.dt_operdate,'%Y-%m-%d') <= '3')) cc"

                                    " Group by dish_typeno, dishno, dishname, dish_unitno, typeno"
                                    )),

                            w_rpt_report::querymodelsetfilters("select * from cey_bt_table")
                            ));

    tab1info.insert(tr("按餐桌类型"),
                    w_rpt_report::TABTYPE(QStringList()<<tr("餐桌类型")<<tr("品码")<<tr("品名")<<tr("单位")<<tr("均价")<<tr("数量")<<tr("金额")<<tr("折扣额")<<tr("赠送额")<<tr("实际金额"),
                            QString("select "
                                    " (select vch_typename  from cey_bt_table_type where cey_bt_table_type.ch_typeno=dish_typeno) as 'tabname0',"
                                    " dishno as 'tabname1',"
                                    " dishname as 'tabname2',"
                                    " (select vch_unitname from cey_bt_unit where cey_bt_unit.ch_unitno=dish_unitno) as 'tabname3',"
                                    " (case t.num when 0 then 0 else t.amount/t.num end) as 'tabname4',"
                                    " t.num as 'tabname5',"
                                    " t.amount as 'tabname6',"
                                    " t.discount as 'tabname7',"
                                    " t.present as 'tabname8',"
                                    " t.amount-t.discount-t.present as 'tabname9'"
                                    " from (%1)t  ORDER BY tabname%4").arg(
                                QString (
                                    "Select dish_typeno,"

                                    " dish_typeno as 'dish_typeno1',"

                                    " dishno,"

                                    " dishname,"

                                    " dish_unitno,"

                                    " sum(num) as 'num',"

                                    " sum(amount) as 'amount',"

                                    " sum(discount) as 'discount',"

                                    " sum(present) as 'present',"

                                    " typeno"

                                    " from (SELECT d.ch_typeno as 'dish_typeno',"

                                    " a.ch_dishno as 'dishno',"

                                    " c.vch_dishname as 'dishname',"

                                    " c.ch_unitno as 'dish_unitno',"

                                    " a.num_back as 'num',"

                                    " a.num_back * (a.num_price + a.num_price_add) as 'amount',"

                                    " a.num_back * (a.num_price + a.num_price_add) *"

                                    " (100 - a.int_discount) * 0.01 as 'discount',"

                                    " (case a.ch_presentflag"

                                    " when 'Y' then"

                                    " a.num_back * (a.num_price + a.num_price_add) *"

                                    " a.int_discount * 0.01"

                                    " else"

                                    " 0"

                                    " end) as 'present',"

                                    " c.ch_dish_typeno as 'typeno'"

                                    " FROM cey_u_orderdish a,"

                                    " cey_u_checkout_master b,"

                                    " cey_bt_dish c,"

                                    " (select ch_tableno, ch_typeno"

                                    " from cey_bt_table"

                                    " union"

                                    " select 'Q000', 'Q0') d"

                                    " where a.ch_tableno = d.ch_tableno"

                                    " and (a.ch_payno = b.ch_payno)"

                                    " and (b.ch_state = 'Y')"

                                    " and (a.num_back <> 0)"

                                    " and (a.ch_dishno = c.ch_dishno)"

                                    " and (d.ch_typeno like '%3%')"

                                    " and (DATE_FORMAT(b.dt_operdate,'%Y-%m-%d %h:%i:%s') >= '%1')"

                                    " and (DATE_FORMAT(b.dt_operdate,'%Y-%m-%d %h:%i:%s') <= '%2')"

                                    " and (DATE_FORMAT(b.dt_operdate, '%Y-%m-%d') >= '0')"

                                    " and (DATE_FORMAT(b.dt_operdate, '%Y-%m-%d') <= '3')"

                                    " union all"

                                    " SELECT d.ch_typeno as 'dish_typeno',"

                                    " a.ch_dishno as 'dishno',"

                                    " c.vch_dishname as 'dishname',"

                                    " c.ch_unitno as 'dish_unitno',"

                                    " a.num_num - a.num_back as 'num',"

                                    " '' ="

                                    " (a.num_num - a.num_back) * (a.num_price + a.num_price_add) as 'amount',"

                                    " '' ="

                                    " (a.num_num - a.num_back) * (a.num_price + a.num_price_add) *"

                                    " (100 - a.int_discount) * 0.01 as 'discount',"

                                    " '' = (case a.ch_presentflag"

                                    " when 'Y' then"

                                    " (a.num_num - a.num_back) *"

                                    " (a.num_price + a.num_price_add) * a.int_discount * 0.01"

                                    " else"

                                    " 0"

                                    " end) as 'present',"

                                    " c.ch_dish_typeno as 'typeno'"

                                    " FROM cey_u_orderdish a,"

                                    " cey_u_checkout_master b,"

                                    " cey_bt_dish c,"

                                    " (select ch_tableno, ch_typeno"

                                    " from cey_bt_table"

                                    " union"

                                    " select 'Q000', 'Q0') d"

                                    " where a.ch_tableno = d.ch_tableno"

                                    " and (a.ch_payno = b.ch_payno)"

                                    " and (b.ch_state = 'P')"

                                    " and (a.num_num - a.num_back <> 0)"

                                    " and (a.ch_dishno = c.ch_dishno)"

                                    " and (d.ch_typeno like '%3%')"

                                    " and (DATE_FORMAT(b.dt_operdate,'%Y-%m-%d %h:%i:%s') >= '%1')"

                                    " and (DATE_FORMAT(b.dt_operdate,'%Y-%m-%d %h:%i:%s') <= '%2')"

                                    " and (DATE_FORMAT(b.dt_operdate, '%Y-%m-%d') >= '0')"

                                    " and (DATE_FORMAT(b.dt_operdate, '%Y-%m-%d') <= '3')"

                                    " union all"

                                    " SELECT d.ch_typeno as 'dish_typeno',"

                                    " a.ch_dishno as 'dishno',"

                                    " c.vch_dishname as 'dishname',"

                                    " c.ch_unitno as 'dish_unitno',"

                                    " a.num_num - a.num_back as 'num',"

                                    " '' ="

                                    " (a.num_num - a.num_back) * (a.num_price + a.num_price_add) as 'amount',"

                                    " '' ="

                                    " (a.num_num - a.num_back) * (a.num_price + a.num_price_add) *"

                                    " (100 - a.int_discount) * 0.01 as 'discount',"

                                    " '' = (case a.ch_presentflag"

                                    " when 'Y' then"

                                    " (a.num_num - a.num_back) *"

                                    " (a.num_price + a.num_price_add) * a.int_discount * 0.01"

                                    " else"

                                    " 0"

                                    " end) as 'present',"

                                    " c.ch_dish_typeno as 'typeno'"

                                    " FROM cey_u_orderdish a,"

                                    " cey_u_togo b,"

                                    " cey_bt_dish c,"

                                    " (select ch_tableno, ch_typeno"

                                    " from cey_bt_table"

                                    " union"

                                    " select 'Q000', 'Q0') d"

                                    " where a.ch_tableno = d.ch_tableno"

                                    " and (a.ch_togono = b.ch_togono)"

                                    " and (b.ch_state = 'P')"

                                    " and (a.num_num - a.num_back <> 0)"

                                    " and (a.ch_dishno = c.ch_dishno)"

                                    " and (d.ch_typeno like '%3%')"

                                    " and (DATE_FORMAT(b.dt_operdate,'%Y-%m-%d %h:%i:%s') >= '%1')"

                                    " and (DATE_FORMAT(b.dt_operdate,'%Y-%m-%d %h:%i:%s') <= '%2')"

                                    " and (DATE_FORMAT(b.dt_operdate, '%Y-%m-%d') >= '0')"

                                    " and (DATE_FORMAT(b.dt_operdate, '%Y-%m-%d') <= '3')) cc"

                                    " Group by dish_typeno, dishno, dishname, dish_unitno, typeno"
                                    )),
                            w_rpt_report::querymodelsetfilters("select * from cey_bt_table_area")
                            ));

    tab1info.insert(tr("按退菜人"),
                    w_rpt_report::TABTYPE(QStringList()<<tr("退菜人")<<tr("品码")<<tr("品名")<<tr("单位")<<tr("均价")<<tr("数量")<<tr("金额")<<tr("折扣额")<<tr("赠送额")<<tr("实际金额"),
                            QString("select "
                                    " dish_typeno as 'tabname0',"
                                    " dishno as 'tabname1',"
                                    " dishname as 'tabname2',"
                                    " (select vch_unitname from cey_bt_unit where cey_bt_unit.ch_unitno=dish_unitno) as 'tabname3',"
                                    " (case t.num when 0 then 0 else t.amount/t.num end) as 'tabname4',"
                                    " t.num as 'tabname5',"
                                    " t.amount as 'tabname6',"
                                    " t.discount as 'tabname7',"
                                    " t.present as 'tabname8',"
                                    " t.amount-t.discount-t.present as 'tabname9'"
                                    " from (%1)t  ORDER BY tabname%4").arg(
                                QString (
                                    "Select dish_typeno,"

                                    " dish_typeno as 'dish_typeno1',"

                                    " dishno,"

                                    " dishname,"

                                    " dish_unitno,"

                                    " sum(num) as 'num',"

                                    " sum(amount) as 'amount',"

                                    " sum(discount) as 'discount',"

                                    " sum(present) as 'present',"

                                    " typeno"

                                    " from (SELECT ifnull(a.vch_back_operID, '') as 'dish_typeno',"

                                    " a.ch_dishno as 'dishno',"

                                    " c.vch_dishname as 'dishname',"

                                    " c.ch_unitno as 'dish_unitno',"

                                    " a.num_back as 'num',"

                                    " a.num_back * (a.num_price + a.num_price_add) as 'amount',"

                                    " a.num_back * (a.num_price + a.num_price_add) *"

                                    " (100 - a.int_discount) * 0.01 as 'discount',"

                                    " (case a.ch_presentflag"

                                    " when 'Y' then"

                                    " a.num_back * (a.num_price + a.num_price_add) *"

                                    " a.int_discount * 0.01"

                                    " else"

                                    " 0"

                                    " end) as 'present',"

                                    " c.ch_dish_typeno as 'typeno'"

                                    " FROM cey_u_orderdish a, cey_u_checkout_master b, cey_bt_dish c"

                                    " where (a.ch_payno = b.ch_payno)"

                                    " and (b.ch_state = 'Y')"

                                    " and (a.num_back <> 0)"

                                    " and (a.ch_dishno = c.ch_dishno)"

                                    " and (ifnull(a.vch_back_operID, '') like '%3%')"

                                    " and (DATE_FORMAT(b.dt_operdate,'%Y-%m-%d %h:%i:%s') >= '%1')"

                                    " and (DATE_FORMAT(b.dt_operdate,'%Y-%m-%d %h:%i:%s') <= '%2')"

                                    " and (DATE_FORMAT(b.dt_operdate, '%Y-%m-%d') >= '0')"

                                    " and (DATE_FORMAT(b.dt_operdate, '%Y-%m-%d') <= '3')"

                                    " union all"

                                    " SELECT ifnull(b.vch_operID2, '') as 'dish_typeno',"

                                    " a.ch_dishno as 'dishno',"

                                    " c.vch_dishname as 'dishname',"

                                    " c.ch_unitno as 'dish_unitno',"

                                    " a.num_num - a.num_back as 'num',"

                                    " '' ="

                                    " (a.num_num - a.num_back) * (a.num_price + a.num_price_add) as 'amount',"

                                    " '' ="

                                    " (a.num_num - a.num_back) * (a.num_price + a.num_price_add) *"

                                    " (100 - a.int_discount) * 0.01 as 'discount',"

                                    " '' ="

                                    " (case a.ch_presentflag"

                                    " when 'Y' then"

                                    " (a.num_num - a.num_back) * (a.num_price + a.num_price_add) *"

                                    " a.int_discount * 0.01"

                                    " else"

                                    " 0"

                                    " end) as 'present',"

                                    " c.ch_dish_typeno as 'typeno'"

                                    " FROM cey_u_orderdish a, cey_u_checkout_master b, cey_bt_dish c"

                                    " where (a.ch_payno = b.ch_payno)"

                                    " and (b.ch_state = 'P')"

                                    " and (a.num_num - a.num_back <> 0)"

                                    " and (a.ch_dishno = c.ch_dishno)"

                                    " and (ifnull(b.vch_operID2, '') like '%3%')"

                                    " and (DATE_FORMAT(b.dt_operdate,'%Y-%m-%d %h:%i:%s') >= '%1')"

                                    " and (DATE_FORMAT(b.dt_operdate,'%Y-%m-%d %h:%i:%s') <= '%2')"

                                    " and (DATE_FORMAT(b.dt_operdate, '%Y-%m-%d') >= '0')"

                                    " and (DATE_FORMAT(b.dt_operdate, '%Y-%m-%d') <= '3')"

                                    " union all"

                                    " SELECT ifnull(b.vch_operID, '') as 'dish_typeno',"

                                    " a.ch_dishno as 'dishno',"

                                    " c.vch_dishname as 'dishname',"

                                    " c.ch_unitno as 'dish_unitno',"

                                    " a.num_num - a.num_back as 'num',"

                                    " '' ="

                                    " (a.num_num - a.num_back) * (a.num_price + a.num_price_add) as 'amount',"

                                    " '' ="

                                    " (a.num_num - a.num_back) * (a.num_price + a.num_price_add) *"

                                    " (100 - a.int_discount) * 0.01 as 'discount',"

                                    " '' ="

                                    " (case a.ch_presentflag"

                                    " when 'Y' then"

                                    " (a.num_num - a.num_back) * (a.num_price + a.num_price_add) *"

                                    " a.int_discount * 0.01"

                                    " else"

                                    " 0"

                                    " end) as 'present',"

                                    " c.ch_dish_typeno as 'typeno'"

                                    " FROM cey_u_orderdish a, cey_u_togo b, cey_bt_dish c"

                                    " where (a.ch_togono = b.ch_togono)"

                                    " and (b.ch_state = 'P')"

                                    " and (a.num_num - a.num_back <> 0)"

                                    " and (a.ch_dishno = c.ch_dishno)"

                                    " and (ifnull(b.vch_operID, '') like '%3%')"

                                    " and (DATE_FORMAT(b.dt_operdate,'%Y-%m-%d %h:%i:%s') >= '%1')"

                                    " and (DATE_FORMAT(b.dt_operdate,'%Y-%m-%d %h:%i:%s') <= '%2')"

                                    " and (DATE_FORMAT(b.dt_operdate, '%Y-%m-%d') >= '0')"

                                    " and (DATE_FORMAT(b.dt_operdate, '%Y-%m-%d') <= '3')) cc"

                                    " Group by dish_typeno, dishno, dishname, dish_unitno, typeno"

                                    )),
                            w_rpt_report::querymodelsetfilters("Select vch_operID,vch_operator from cey_sys_operator union Select '0000' as 'vch_operID', 'system' as 'vch_operator' ")
                            ));

    tab1info.insert(tr("按退菜明细查询"),
                    w_rpt_report::TABTYPE(QStringList()<<tr("退菜类型")<<tr("品码")<<tr("品名")<<tr("单位")<<tr("均价")<<tr("数量")<<tr("金额")<<tr("折扣额")<<tr("赠送额")<<tr("实际金额")<<tr("退菜原因")<<tr("退菜人")<<tr("退菜时间"),
                            QString (
                                "SELECT '点单退菜' as 'dish_typeno',"

                                " '点单退菜' as 'dish_typeno1',"

                                " a.ch_dishno as 'dishno',"

                                " c.vch_dishname as 'dishname',"

                                " c.ch_unitno as 'dish_unitno',"

                                " a.num_back as 'num',"

                                " a.num_back * (a.num_price + a.num_price_add) as 'amount',"

                                " a.num_back * (a.num_price + a.num_price_add) *"

                                " (100 - a.int_discount) * 0.01 as 'discount',"

                                " (case a.ch_presentflag"

                                " when 'Y' then"

                                " a.num_back * (a.num_price + a.num_price_add) * a.int_discount * 0.01"

                                " else"

                                " 0"

                                " end) as 'present',"

                                " c.ch_dish_typeno as 'typeno',"

                                " a.vch_back_operID as 'operID',"

                                " a.dt_back_operdate as 'operdate',"

                                " (select vch_reason"

                                " from cey_u_orderdish_log"

                                " where a.int_flowid = int_orderflow"

                                " and ch_type = '1' LIMIT 1) as 'reason'"

                                " FROM cey_u_orderdish a, cey_u_checkout_master b, cey_bt_dish c"

                                " WHERE (a.ch_payno = b.ch_payno)"

                                " and (b.ch_state = 'Y')"

                                " and (a.num_back <> 0)"

                                " and (a.ch_dishno = c.ch_dishno)"

                                " and (DATE_FORMAT(b.dt_operdate,'%Y-%m-%d %h:%i:%s') >= '%1')"

                                " and (DATE_FORMAT(b.dt_operdate,'%Y-%m-%d %h:%i:%s') <= '%2')"

                                " and (DATE_FORMAT(b.dt_operdate, '%Y-%m-%d') >= '0')"

                                " and (DATE_FORMAT(b.dt_operdate, '%Y-%m-%d') <= '3')"

                                "union all"

                                ""

                                " SELECT '快餐退单' as 'dish_typeno',"

                                " '快餐退单' as 'dish_typeno1',"

                                " a.ch_dishno as 'dishno',"

                                " c.vch_dishname as 'dishname',"

                                " c.ch_unitno as 'dish_unitno',"

                                " a.num_num - a.num_back as 'num',"

                                " (a.num_num - a.num_back) * (a.num_price + a.num_price_add) as 'amount',"

                                " (a.num_num - a.num_back) * (a.num_price + a.num_price_add) *"

                                " (100 - a.int_discount) * 0.01 as 'discount',"

                                " (case a.ch_presentflag"

                                " when 'Y' then"

                                " (a.num_num - a.num_back) *"

                                " (a.num_price + a.num_price_add) * a.int_discount * 0.01"

                                " else"

                                " 0"

                                " end) as 'present',"

                                " c.ch_dish_typeno as 'typeno',"

                                " b.vch_operID2 as 'operID',"

                                " b.dt_operdate2 as 'operdate',"

                                " (select vch_reason"

                                " from cey_u_orderdish_log"

                                " where a.int_flowid = int_orderflow"

                                " and ch_type = '1' LIMIT 1) as 'reason'"

                                " FROM cey_u_orderdish a, cey_u_checkout_master b, cey_bt_dish c"

                                " WHERE (a.ch_payno = b.ch_payno)"

                                " and (b.ch_state = 'P')"

                                " and (a.num_num - a.num_back <> 0)"

                                " and (a.ch_dishno = c.ch_dishno)"

                                " and (DATE_FORMAT(b.dt_operdate,'%Y-%m-%d %h:%i:%s') >= '%1')"

                                " and (DATE_FORMAT(b.dt_operdate,'%Y-%m-%d %h:%i:%s') <= '%2')"

                                " and (DATE_FORMAT(b.dt_operdate, '%Y-%m-%d') >= '0')"

                                " and (DATE_FORMAT(b.dt_operdate, '%Y-%m-%d') <= '3')"

                                " union all"

                                " SELECT '外卖退单' as 'dish_typeno',"

                                " '外卖退单' as 'dish_typeno1',"

                                " a.ch_dishno as 'dishno',"

                                " c.vch_dishname as 'dishname',"

                                " c.ch_unitno as 'dish_unitno',"

                                " a.num_num - a.num_back as 'num',"

                                " (a.num_num - a.num_back) * (a.num_price + a.num_price_add) as 'amount',"

                                " (a.num_num - a.num_back) * (a.num_price + a.num_price_add) *"

                                " (100 - a.int_discount) * 0.01 as 'discount',"

                                " (case a.ch_presentflag"

                                " when 'Y' then"

                                " (a.num_num - a.num_back) *"

                                " (a.num_price + a.num_price_add) * a.int_discount * 0.01"

                                " else"

                                " 0"

                                " end) as 'present',"

                                " c.ch_dish_typeno as 'typeno',"

                                " b.vch_operID as 'operID',"

                                " b.dt_operdate as 'operdate',"

                                " (select vch_reason"

                                " from cey_u_orderdish_log"

                                " where a.int_flowid = int_orderflow"

                                " and ch_type = '1' LIMIT 1) as 'reason'"

                                " FROM cey_u_orderdish a, cey_u_togo b, cey_bt_dish c"

                                " WHERE (a.ch_togono = b.ch_togono)"

                                " and (b.ch_state = 'P')"

                                " and (a.num_num - a.num_back <> 0)"

                                " and (a.ch_dishno = c.ch_dishno)"

                                " and (DATE_FORMAT(b.dt_operdate,'%Y-%m-%d %h:%i:%s') >= '%1')"

                                " and (DATE_FORMAT(b.dt_operdate,'%Y-%m-%d %h:%i:%s') <= '%2')"

                                " and (DATE_FORMAT(b.dt_operdate, '%Y-%m-%d') >= '0')"

                                " and (DATE_FORMAT(b.dt_operdate, '%Y-%m-%d') <= '3')"
                                ),
                            w_rpt_report::querymodelsetfilters("")
                            ));
}

void w_rpt_report_dishquitreport::toselect()
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
    //制表人 和 制表时间
    ui->label_operator->setText(n_func::gs_opername);
    ui->label_operattime->setText(n_func::f_get_sysdatetime().toString("yyyy-MM-dd hh:mm:ss"));
    ui->label_timefrom->setText(ls_begin);
    ui->label_timeto->setText(ls_end);
    //更新合计
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

void w_rpt_report_dishquitreport::toexit()
{
    this->reject();
}

void w_rpt_report_dishquitreport::tocom12(const QString &text)
{
    ui->comboBox_12->clear();
    ui->comboBox_12->addItems(tab1info[text].filters);
}
