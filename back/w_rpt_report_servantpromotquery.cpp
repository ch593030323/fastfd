#include "w_rpt_report_servantpromotquery.h"
//服务员提成查询
#include "ui_w_rpt_report_servantpromotquery_dialog.h"
#include "n_func.h"
#include "backheader.h"

w_rpt_report_servantpromotquery::w_rpt_report_servantpromotquery(QWidget *parent) :
    QDialog(parent),ui(new Ui_w_rpt_report_servantpromotquery_Dialog)
{
    ui->setupUi(this);
    ui->frame->setFixedWidth(w_rpt_report::frame_left_fixed_width);
    //! init
    //开始结束时间
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

    //! com_opertor
    ui->comboBox_operator->addItems(w_rpt_report::querymodelsetfilters("Select vch_operID,vch_operator from cey_sys_operator union Select 'vch_operID' = '0000','vch_operator' = 'system'  "));

    //! sqlinit
    tab1sqlinit();
    //! tableView
    querymodel=new datetimeModel;
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);ui->tableView->setModel(querymodel);

    querymodel_2=new datetimeModel;
    ui->tableView_2->setEditTriggers(QAbstractItemView::NoEditTriggers);ui->tableView_2->setModel(querymodel_2);

    ui->tableView->tableView->setSortingEnabled(true);
    ui->tableView_2->tableView->setSortingEnabled(true);

    //! toolbar
    ui->widget->setup(QStringList()<<tr("查询")<<tr("退出"), Qt::AlignTop);
    connect(ui->widget->index_widget(tr("查询")),SIGNAL(clicked()),this,SLOT(toselect()));
    connect(ui->widget->index_widget(tr("退出")),SIGNAL(clicked()),this,SLOT(toexit()));
    connect(ui->tableView->tableView->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(toselect()));
    connect(ui->tableView_2->tableView->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(toselect()));
    //! after
    //tabwidget header
    ui->tabWidget->setTabText(0, tr("服务员提成明细查询"));
    ui->tabWidget->setTabText(1, tr("服务员提成收银查询"));

}

w_rpt_report_servantpromotquery::~w_rpt_report_servantpromotquery()
{

}

void w_rpt_report_servantpromotquery::tab1sqlinit()
{
    tab1info.insert(tr("服务员提成明细查询"),
                    w_rpt_report::TABTYPE(QStringList() << tr("服务员") << tr("付款单号") << tr("品名") << tr("单位") << tr("点菜数量") << tr("销售金额") << tr("定额金额") << tr("比率提成") << tr("提成小计") << tr("提成率") << tr("操作时间"),
                            QString (
                                " SELECT item as 'tabname0',"

                                " payno as 'tabname1',"

                                " dishname as 'tabname2',"

                                " (select vch_unitname from cey_bt_unit where ch_unitno=unit) as 'tabname3',"

                                " num as 'tabname4',"

                                " amount as 'tabname5',"

                                " fdeduct as 'tabname6',"

                                " rdeduct as 'tabname7',"

                                " fdeduct + rdeduct as 'tabname8',"

                                "(fdeduct + rdeduct)/ amount as 'tabname9', "

                                " operdate as 'tabname10' "

                                "  FROM ("

                                " SELECT b.vch_waiter as 'item' ,"

                                "  a.ch_payno as 'payno' ,"

                                "  d.ch_dishno as 'dishno' ,"

                                "  d.vch_dishname as 'dishname' ,"

                                "  d.ch_unitno as 'unit' ,"

                                "  d.ch_dish_typeno as 'seriesno' ,"

                                "  d.ch_sub_typeno as 'typeno' ,"

                                "  (num_num - num_back) as 'num' ,"

                                "  (num_num - num_back) * (num_price_org + num_price_add) *"

                                "  int_discount * 0.01 as 'amount' ,"

                                "  (case d.ch_deducttype"

                                "  when '2' then"

                                "  d.num_deduct * (num_num - num_back)"

                                "  else"

                                "  0"

                                "  end) as 'fdeduct' ,"

                                "  '' ="

                                "  (case d.ch_deducttype"

                                "  when '1' then"

                                "  (num_num - num_back) * (num_price_org + num_price_add) *"

                                "  int_discount * 0.01 * d.int_deduct * 0.01"

                                "  else"

                                "  0"

                                "  end) as 'rdeduct' ,"

                                "  b.dt_operdate as 'operdate'"

                                "  FROM cey_u_orderdish a,"

                                "  cey_u_table b,"

                                "  cey_u_checkout_master c,"

                                "  cey_bt_dish d"

                                "  WHERE a.ch_tableno = b.ch_tableno"

                                "  and a.ch_payno = b.ch_payno"

                                "  and b.vch_waiter is not null"

                                "  and a.ch_payno = c.ch_payno"

                                "  and c.ch_state = 'Y'"

                                "  and a.ch_suitflag <> 'Y'"

                                "  and a.ch_dishno = d.ch_dishno"

                                "  and (a.ch_presentflag <> 'Y')"

                                "  and (d.ch_deductflag = 'Y')"

                                "  and b.vch_waiter like '%3%'"

                                "  and DATE_FORMAT(c.dt_operdate, '%Y-%m-%d %h:%i:%s') >= '%1'"

                                "  and DATE_FORMAT(c.dt_operdate, '%Y-%m-%d %h:%i:%s') <= '%2'"

                                ")t ORDER BY tabname%4"
                                ),
                            w_rpt_report::querymodelsetfilters("")
                            ));

    tab1info.insert(tr("服务员提成收银查询"),
                    w_rpt_report::TABTYPE(QStringList() << tr("服务员") << tr("付款单号") << tr("餐桌") << tr("收银方式") << tr("收入款金额") << tr("时间"),
                            QString (
                                "SELECT item as 'tabname0',"

                                "payno as 'tabname1',"

                                "tableno as 'tabname2',"

                                "(select vch_paymodename from cey_bt_paymode where ch_paymodeno = paymode) as 'tabname3',"

                                "realamount as 'tabname4',"

                                "operdate as 'tabname5'"

                                " FROM("

                                "SELECT b.vch_waiter as 'item' ,"

                                " a.ch_payno as 'payno' ,"

                                " b.ch_tableno as 'tableno' ,"

                                " a.ch_paymodeno as 'paymode' ,"

                                " a.num_realamount as 'realamount' ,"

                                " a.dt_operdate as 'operdate' "

                                " FROM cey_u_checkout_detail a,"

                                " cey_u_table b,"

                                " cey_u_checkout_master c,"

                                " cey_bt_paymode d"

                                " WHERE a.ch_payno = c.ch_payno"

                                " and a.ch_paymodeno = d.ch_paymodeno"

                                " and c.ch_state = 'Y'"

                                " and c.ch_payno = b.ch_payno"

                                " and c.ch_tableno = b.ch_tableno"

                                " and LENGTH(Rtrim(Ltrim(IFNULL(b.vch_waiter, '')))) <> 0"

                                " and b.vch_waiter like '%3%'"

                                " and DATE_FORMAT(c.dt_operdate, '%Y-%m-%d %h:%i:%s') >= '%1'"

                                " and DATE_FORMAT(c.dt_operdate, '%Y-%m-%d %h:%i:%s') <= '%2'"

                                ")t ORDER BY tabname%4"
                                ),
                            w_rpt_report::querymodelsetfilters("")
                            ));
}

void w_rpt_report_servantpromotquery::toselect()
{
    QString ls_begin;
    QString ls_end;
    ls_begin=n_func::uf_getdatetime("begin", QDateTime::fromString(ui->comboBox_start->currentText(), "yyyy-MM-dd")).toString("yyyy-MM-dd hh:mm:ss");
    ls_end=n_func::uf_getdatetime("end",  QDateTime::fromString(ui->comboBox_end->currentText(), "yyyy-MM-dd")).toString("yyyy-MM-dd hh:mm:ss");

    switch(ui->tabWidget->currentIndex()){
    case 0: {
        //tabwidget header
        ui->tabWidget->setTabText(0, ui->tabWidget->tabText(0));
        //model select
        querymodel->setQuery(tab1info[tr("服务员提成明细查询")].sql
                .arg(ls_begin)
                .arg(ls_end)
                .arg(backheader::getinterdata(ui->comboBox_operator->currentText()))//获取过滤信息
                .arg(w_rpt_report::querymodelordersort(ui->tableView->tableView))
                );
        qDebug() << querymodel->query().lastQuery().trimmed();
        //更新表头
        backheader::modelSetHeader(querymodel, tab1info[tr("服务员提成明细查询")].headers);
        //制表人 和 制表时间
        ui->label_operator->setText(n_func::gs_opername);
        ui->label_operattime->setText(n_func::f_get_sysdatetime().toString("yyyy-MM-dd hh:mm:ss"));
//        ui->label_timefrom->setText(ls_begin);
//        ui->label_timeto->setText(ls_end);
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
        break;
    case 1:{
        //tabwidget header
        ui->tabWidget->setTabText(1, ui->tabWidget->tabText(1));
        //model select
        querymodel_2->setQuery(tab1info[tr("服务员提成收银查询")].sql
                .arg(ui->comboBox_start->currentText())
                .arg(ui->comboBox_end->currentText())
                .arg(backheader::getinterdata(ui->comboBox_operator->currentText()))//获取过滤信息
                .arg(w_rpt_report::querymodelordersort(ui->tableView_2->tableView))
                );
        qDebug() << querymodel_2->query().lastQuery().trimmed();
        //更新表头
        backheader::modelSetHeader(querymodel_2, tab1info[tr("服务员提成收银查询")].headers);
        //制表人 和 制表时间
        ui->label_operator->setText(n_func::gs_opername);
        ui->label_operattime->setText(n_func::f_get_sysdatetime().toString("yyyy-MM-dd hh:mm:ss"));
        //更新合计
        QString sumsql=querymodel_2->query().lastQuery().trimmed();
        if(sumsql.endsWith("DESC")||sumsql.endsWith("ASC")){
            //5 6 7  9 10
            QSqlQuery query;
            query.exec(QString("select sum(tabname4)  from (%1)t1 ").arg(sumsql.left(sumsql.lastIndexOf("ORDER BY"))));
            query.next();
            qDebug() <<query.lastQuery();

            ui->tableView_2->updateSum(4,query.record().value(0).toString());
        }
    }
        break;
    }


}

void w_rpt_report_servantpromotquery::toexit()
{
    this->reject();
}

void w_rpt_report_servantpromotquery::tocomtype(const QString &text)
{

}
