#include "w_rpt_report_cashfreequery.h"
//收银免单情况查询
#include "ui_w_rpt_report_cashfreequery_dialog.h"
#include "n_func.h"
#include "backheader.h"
w_rpt_report_cashfreequery::w_rpt_report_cashfreequery(QWidget *parent) :
    QDialog(parent),ui(new Ui_w_rpt_report_cashfreequery_Dialog)
{
    ui->setupUi(this);
    ui->frame->setFixedWidth(w_rpt_report::frame_left_fixed_width);
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

    //tabwidget header
    ui->tabWidget->setTabText(0, tr("收银免单情况查询"));
}

void w_rpt_report_cashfreequery::tab1sqlinit()
{
    tab1info.insert(tr("收银免单情况查询"),
                    w_rpt_report::TABTYPE(QStringList()<<tr("付款单号")<<tr("餐桌号")<<tr("结算金额")<<tr("免单金额")<<tr("手工金额")<<tr("免服务费")<<tr("免低消")<<tr("百分比")<<tr("免单人")<<tr("操作时间"),
                            QString (
                                "SELECT payno as 'tabname0',"

                                "tableno as 'tabname1',"

                                "amount - blotout as 'tabname2',"

                                "free as 'tabname3',"

                                "free2 as 'tabname4',"

                                "t.server as 'tabname5',"

                                "t.lower as 'tabname6',"

                                "if(amount + server + lower = 0 , 1 , (free + free2 + server + lower)/(amount + server + lower)) as 'tabname7',"

                                "operid as 'tabname8',"

                                "operdate as 'tabname9'"

                                " FROM("

                                "SELECT a.ch_payno as 'payno', "

                                " a.ch_tableno as 'tableno', "

                                " a.num_cost - a.num_discount - a.num_present +"

                                " a.num_service + a.num_lowcost as 'amount', "

                                " sum(case b.ch_paymodeno"

                                " when 'YY' then"

                                " b.num_realamount"

                                " else"

                                " 0"

                                " end) as 'blotout', "

                                " sum(case b.ch_paymodeno"

                                " when 'XX' then"

                                " b.num_realamount"

                                " else"

                                " 0"

                                " end) as 'free', "

                                " sum(case b.ch_paymodeno"

                                " when 'ZZ' then"

                                " b.num_realamount"

                                " else"

                                " 0"

                                " end) as 'free2', "

                                " ifnull((select sum(num_amount)"

                                " from cey_u_service_low"

                                " where ch_payno = a.ch_payno"

                                " and ch_type = '1'"

                                " and ch_state = '0'),"

                                " 0) as 'server', "

                                " ifnull((select sum(num_amount)"

                                " from cey_u_service_low"

                                " where ch_payno = a.ch_payno"

                                " and ch_type = '2'"

                                " and ch_state = '0'),"

                                " 0) as 'lower', "

                                " b.vch_operId as 'operID', "

                                " b.dt_operdate as 'operdate'"

                                " FROM cey_u_checkout_master a, cey_u_checkout_detail b"

                                " WHERE a.ch_payno = b.ch_payno"

                                " and a.ch_state = 'Y'"

                                " and (b.ch_paymodeno in ('XX', 'ZZ') or exists"

                                " (select 1"

                                " from cey_u_service_low"

                                " where ch_payno = a.ch_payno"

                                " and ch_state = '0'))"

                                " and DATE_FORMAT(b.dt_operdate, '%Y-%m-%d %h:%i:%s') >= '%1'"

                                " and DATE_FORMAT(b.dt_operdate, '%Y-%m-%d %h:%i:%s') <= '%2'"

                                " and DATE_FORMAT(b.dt_operdate, '%Y') >= '0'"

                                " and DATE_FORMAT(b.dt_operdate, '%Y') <= '3'"

                                ""

                                " group by a.ch_payno,"

                                " a.ch_tableno,"

                                " a.num_cost,"

                                " a.num_discount,"

                                " a.num_present,"

                                " a.num_service,"

                                " a.num_lowcost,"

                                " b.vch_operId,"

                                " b.dt_operdate"

                                " )t ORDER BY tabname%3"
                                )
                            )
                    );

}

void w_rpt_report_cashfreequery::toselect()
{
    QString ls_begin;
    QString ls_end;
    ls_begin=n_func::uf_getdatetime("begin", ui->dateTimeEdit->dateTime()).toString("yyyy-MM-dd hh:mm");
    ls_end=n_func::uf_getdatetime("end", ui->dateTimeEdit_2->dateTime()).toString("yyyy-MM-dd hh:mm");
    //model select
    querymodel->setQuery(tab1info[tr("收银免单情况查询")].sql
            .arg(ls_begin.isEmpty()?ui->dateTimeEdit->dateTime().toString("yyyy-MM-dd hh:mm:ss"):ls_begin)
            .arg(ls_end.isEmpty()?ui->dateTimeEdit_2->dateTime().toString("yyyy-MM-dd hh:mm:ss"):ls_end)
            .arg(w_rpt_report::querymodelordersort(ui->tableView->tableView))
            );
    //更新表头
    backheader::modelSetHeader(querymodel, tab1info[tr("收银免单情况查询")].headers);
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

        if(tab1info[tr("收银免单情况查询")].except_c.isEmpty()){
            query.exec(QString("select sum(tabname2),sum(tabname3),sum(tabname4),sum(tabname5),sum(tabname6),sum(tabname7),sum(tabname8) from (%1)t1 ").arg(sumsql.left(sumsql.lastIndexOf("ORDER BY"))));
            query.next();

            ui->tableView->updateSum(2,query.record().value(0).toString());
            ui->tableView->updateSum(3,query.record().value(1).toString());
            ui->tableView->updateSum(4,query.record().value(2).toString());
            ui->tableView->updateSum(5,query.record().value(3).toString());
            ui->tableView->updateSum(6,query.record().value(4).toString());
            ui->tableView->updateSum(7,query.record().value(5).toString());
            ui->tableView->updateSum(8,query.record().value(6).toString());
        }
    }
}

void w_rpt_report_cashfreequery::toexit()
{
    this->reject();
}
