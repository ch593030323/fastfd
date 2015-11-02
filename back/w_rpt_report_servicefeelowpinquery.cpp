#include "w_rpt_report_servicefeelowpinquery.h"
//服务费与低消查询
#include "ui_w_rpt_report_cashreport_dialog.h"
#include "n_func.h"
#include "backheader.h"

w_rpt_report_servicefeelowpinquery::w_rpt_report_servicefeelowpinquery(QWidget *parent) :
    QDialog(parent),ui(new Ui_w_rpt_report_cashreport_Dialog)
{
    ui->setupUi(this);
    ui->frame->setFixedWidth(w_rpt_report::frame_left_fixed_width);
    ui->dateTimeEdit->setDateTime(QDateTime(n_func::f_get_sysdatetime().date(), QTime(0,0,0)));
    ui->dateTimeEdit_2->setDateTime(QDateTime(n_func::f_get_sysdatetime().date(), QTime(23,59,59)));

    ui->tableView->tableView->setSortingEnabled(true);

    tab1sqlinit();

    querymodel=new datetimeModel;

    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);ui->tableView->setModel(querymodel);

    ui->comboBox_11->addItems(tab1info.keys());
    ui->comboBox_12->addItems(w_rpt_report::querymodelsetfilters("select * from cey_bt_table_area"));
    //! toolbar
    ui->widget->setup(QStringList()<<tr("查询")<<tr("退出"), Qt::AlignTop);
    connect(ui->widget->index_widget(tr("查询")),SIGNAL(clicked()),this,SLOT(toselect()));
    connect(ui->widget->index_widget(tr("退出")),SIGNAL(clicked()),this,SLOT(toexit()));

    connect(ui->tableView->tableView->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(toselect()));

    //tabwidget header
    ui->tabWidget->setTabText(0, tr("服务费与低消查询"));
}

w_rpt_report_servicefeelowpinquery::~w_rpt_report_servicefeelowpinquery()
{

}

void w_rpt_report_servicefeelowpinquery::tab1sqlinit()
{
    tab1info.insert(tr("服务费收取情况"),
                    w_rpt_report::TABTYPE(QStringList()<<tr("餐桌类型")<<tr("餐桌")<<tr("付款单号")<<tr("收取金额")<<tr("免收金额")<<tr("操作员")<<tr("时间"),
                                          QString (
                                              "SELECT "

                                              "(SELECT vch_typename FROM cey_bt_table_type where cey_bt_table_type.ch_typeno=table_typeno) as 'tabname0',"

                                              "(SELECT vch_tablename FROM cey_bt_table where cey_bt_table.ch_tableno=tableno) as 'tabname1',"

                                              "payno as 'tabname2',"

                                              "amount1 as 'tabname3',"

                                              "amount2 as 'tabname4',"

                                              "operID as 'tabname5',"

                                              "operdate as 'tabname6'"

                                              " from ("

                                              "SELECT c.ch_typeno as 'table_typeno' ,"

                                              " b.ch_tableno as 'tableno' ,"

                                              " a.ch_payno as 'payno' ,"

                                              " (case a.ch_state"

                                              " when '1' then"

                                              " a.num_amount"

                                              " else"

                                              " 0"

                                              " end) as 'amount1' ,"

                                              " (case a.ch_state"

                                              " when '1' then"

                                              " 0"

                                              " else"

                                              " a.num_amount"

                                              " end) as 'amount2' ,"

                                              " a.vch_operID as 'operID' ,"

                                              " a.dt_operdate as 'operdate'"

                                              " FROM cey_u_service_low a, cey_u_checkout_master b, cey_bt_table c"

                                              " WHERE (a.ch_payno = b.ch_payno)"

                                              " and (b.ch_tableno = c.ch_tableno)"

                                              " and (b.ch_state = 'Y')"

                                              " and (a.ch_type = '1')"

                                              " and DATE_FORMAT(b.dt_operdate, '%Y-%m-%d %h:%i:%s') >= '%1'"

                                              " and DATE_FORMAT(b.dt_operdate, '%Y-%m-%d %h:%i:%s') <= '%2'"

                                              " and DATE_FORMAT(b.dt_operdate, '%Y') >= '0'"

                                              " and DATE_FORMAT(b.dt_operdate, '%Y') <= '3'"

                                              " and (c.ch_typeno like '%3%')"

                                              " order by c.ch_typeno, b.ch_tableno, a.ch_payno"

                                              " )t ORDER BY tabname%4"
                                              )
                                          ));
    tab1info.insert(tr("低消差收取情况"),
                    w_rpt_report::TABTYPE(QStringList()<<tr("餐桌类型")<<tr("餐桌")<<tr("付款单号")<<tr("收取金额")<<tr("免收金额")<<tr("操作员")<<tr("时间"),
                                          QString (
                                              "SELECT "

                                              "(SELECT vch_typename FROM cey_bt_table_type where cey_bt_table_type.ch_typeno=table_typeno) as 'tabname0',"

                                              "(SELECT vch_tablename FROM cey_bt_table where cey_bt_table.ch_tableno=tableno) as 'tabname1',"

                                              "payno as 'tabname2',"

                                              "amount1 as 'tabname3',"

                                              "amount2 as 'tabname4',"

                                              "operID as 'tabname5',"

                                              "operdate as 'tabname6'"

                                              " from ("

                                              "SELECT c.ch_typeno as 'table_typeno' ,"

                                              " b.ch_tableno as 'tableno' ,"

                                              " a.ch_payno as 'payno' ,"

                                              " (case a.ch_state"

                                              " when '1' then"

                                              " a.num_amount"

                                              " else"

                                              " 0"

                                              " end) as 'amount1' ,"

                                              " (case a.ch_state"

                                              " when '1' then"

                                              " 0"

                                              " else"

                                              " a.num_amount"

                                              " end) as 'amount2' ,"

                                              " a.vch_operID as 'operID' ,"

                                              " a.dt_operdate as 'operdate'"

                                              " FROM cey_u_service_low a, cey_u_checkout_master b, cey_bt_table c"

                                              " WHERE (a.ch_payno = b.ch_payno)"

                                              " and (b.ch_tableno = c.ch_tableno)"

                                              " and (b.ch_state = 'Y')"

                                              " and (a.ch_type = '1')"

                                              " and DATE_FORMAT(b.dt_operdate, '%Y-%m-%d %h:%i:%s') >= '%1'"

                                              " and DATE_FORMAT(b.dt_operdate, '%Y-%m-%d %h:%i:%s') <= '%2'"

                                              " and DATE_FORMAT(b.dt_operdate, '%Y') >= '0'"

                                              " and DATE_FORMAT(b.dt_operdate, '%Y') <= '3'"

                                              " and (c.ch_typeno like '%3%')"

                                              " order by c.ch_typeno, b.ch_tableno, a.ch_payno"

                                              " )t ORDER BY tabname%4"
                                              )
                                          ));
}

void w_rpt_report_servicefeelowpinquery::toselect()
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
    if(sumsql.endsWith("DESC")||
            sumsql.endsWith("ASC")){
        //5 6 7  9 10
        QSqlQuery query;

        if(tab1info[ui->comboBox_11->currentText()].except_c.isEmpty()){
            query.exec(QString("select sum(tabname3),sum(tabname4) from (%1)t1 ").arg(sumsql.left(sumsql.lastIndexOf("ORDER BY"))));
            query.next();

            ui->tableView->updateSum(3,query.record().value(0).toString());
            ui->tableView->updateSum(4,query.record().value(1).toString());
        }
    }
}

void w_rpt_report_servicefeelowpinquery::toexit()
{
    this->reject();
}
