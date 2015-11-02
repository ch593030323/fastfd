#include "w_rpt_report_categorysalesanalysis.h"
//品类销售营业分析
#include "ui_w_rpt_report_categorysalesanalysis_dialog.h"
#include <QSqlQueryModel>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlRecord>
#include "n_func.h"
#include "lds_tableview.h"
#include <QSqlError>
#include "backheader.h"
#include <QDebug>
#include "lds_chartdialog.h"
#include "lds_chart_nightcharts.h"

w_rpt_report_categorysalesanalysis::w_rpt_report_categorysalesanalysis(QWidget *parent) :
    QDialog(parent),ui(new Ui_w_rpt_report_categorysalesanalysis_Dialog)
{
    ui->setupUi(this);
    ui->frame->setFixedWidth(w_rpt_report::frame_left_fixed_width);
    //! init
    ui->dateTimeEdit->setDateTime(QDateTime(n_func::f_get_sysdatetime().date(), QTime(0,0,0)));
    ui->dateTimeEdit_2->setDateTime(QDateTime(n_func::f_get_sysdatetime().date(), QTime(23,59,59)));

    tab1sqlinit();

    standmodel=new datetimestandModel;

    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);ui->tableView->setModel(standmodel);
    ui->tableView->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->comboBox_charttype->addItems(QStringList() << tr("销售数量") << tr("销售金额") << tr("折扣金额") << tr("赠送金额") << tr("实际金额"));
    //! toolbar
    //    connect(ui->tableView->tableView->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(toselect()));
    ui->widget->setup(QStringList()<< tr("图表分析")<<tr("查询") <<tr("退出"), Qt::AlignTop);
    connect(ui->widget->index_widget(tr("查询")),SIGNAL(clicked()),this,SLOT(toselect()));
    connect(ui->widget->index_widget(tr("图表分析")),SIGNAL(clicked()),this,SLOT(tochart()));
    connect(ui->widget->index_widget(tr("退出")),SIGNAL(clicked()),this,SLOT(toexit()));
    //! after
    //    ui->comboBox_series->addItems(w_rpt_report::querymodelsetfilters("select * from cey_bt_dish_type"));
    ui->tabWidget->setTabText(0, "");

    //tabwidget header
    ui->tabWidget->setTabText(0, tr("品类销售营业分析"));

}

void w_rpt_report_categorysalesanalysis::tab1sqlinit()
{
    tab1info.insert(tr("品类销售营业分析"),
                    w_rpt_report::TABTYPE(QStringList()<<tr("菜品类型")<<tr("编码")<<tr("销售数量")<<tr("比例")<<tr("销售金额")<<tr("比例")<<tr("折扣金额")<<tr("比例")<<tr("赠送金额")<<tr("比例")<<tr("实际金额")<<tr("比例"),
                                          QString (
                                              "SELECT d.ch_dish_typeno as 'ch_seriesno',"

                                              " d.vch_dish_typename as 'ch_seriesname',"

                                              " sum(a.num_num - a.num_back) as 'num',"

                                              " sum((a.num_num - a.num_back) * (a.num_price + a.num_price_add)) as 'amount',"

                                              " sum((a.num_num - a.num_back) * (a.num_price + a.num_price_add) *"

                                              " (1 - a.int_discount * 0.01)) as 'discount',"

                                              " sum((case a.ch_presentflag"

                                              " when 'Y' then"

                                              " (a.num_num - a.num_back)"

                                              " else"

                                              " 0"

                                              " end) * (a.num_price + a.num_price_add) * a.int_discount * 0.01) as 'present'"

                                              " FROM cey_u_orderdish a,"

                                              " cey_u_checkout_master b,"

                                              " cey_bt_dish c,"

                                              " cey_bt_dish_type d"

                                              " WHERE a.ch_payno = b.ch_payno"

                                              " and b.ch_state = 'Y'"

                                              " and a.ch_dishno = c.ch_dishno"

                                              " and c.ch_dish_typeno = d.ch_dish_typeno"

                                              " and b.dt_operdate >= '%1' "

                                              " and b.dt_operdate <= '%2' "

                                              " GROUP BY d.ch_dish_typeno, d.vch_dish_typename"

                                              " ORDER BY d.ch_dish_typeno"
                                              ),
                                          w_rpt_report::querymodelsetfilters("")
                                          ));
}

void w_rpt_report_categorysalesanalysis::toselect()
{
    QString ls_begin;
    QString ls_end;
    ls_begin=n_func::uf_getdatetime("begin", ui->dateTimeEdit->dateTime()).toString("yyyy-MM-dd hh:mm");
    ls_end=n_func::uf_getdatetime("end", ui->dateTimeEdit_2->dateTime()).toString("yyyy-MM-dd hh:mm");

    //model select
    standmodel->removeRows(0, standmodel->rowCount());
    QSqlQuery query;
    query.exec(tab1info[tr("品类销售营业分析")].sql
            .arg(ls_begin.isEmpty()?ui->dateTimeEdit->dateTime().toString("yyyy-MM-dd hh:mm:ss"):ls_begin)
            .arg(ls_end.isEmpty()?ui->dateTimeEdit_2->dateTime().toString("yyyy-MM-dd hh:mm:ss"):ls_end)
            );
    qDebug() << tab1info[tr("品类销售营业分析")].sql;
    double num_total=0.0;
    double amount_total=0.0;
    double discount_total=0.0;
    double present_total=0.0;

    double realamount=0.0;
    double realamount_total=0.0;

    while(query.next()){
        QList<QStandardItem *> items;
        //菜品类型
        items << new QStandardItem(query.record().value("ch_seriesname").toString());
        //编码
        items << new QStandardItem(query.record().value("ch_seriesno").toString());

        //销售数量
        items << new QStandardItem(query.record().value("num").toString());num_total+=items.last()->text().toDouble();
        items << new QStandardItem("");//比例3

        //销售金额
        items << new QStandardItem(query.record().value("amount").toString());amount_total+=items.last()->text().toDouble();
        items << new QStandardItem("");//比例5

        //折扣金额
        items << new QStandardItem(query.record().value("discount").toString());discount_total+=items.last()->text().toDouble();
        items << new QStandardItem("");//比例7

        //赠送金额
        items << new QStandardItem(query.record().value("present").toString());present_total+=items.last()->text().toDouble();
        items << new QStandardItem("");//比例9

        //实际金额
        realamount=query.record().value("amount").toDouble()-query.record().value("discount").toDouble()-query.record().value("present").toDouble();
        items << new QStandardItem(QString().sprintf("%.02f", realamount));realamount_total+=items.last()->text().toDouble();
        items << new QStandardItem("");//比例11
        standmodel->appendRow(items);
    }

    for(int r=0,r_count=standmodel->rowCount(); r<r_count;r++){
        for(int c=0,c_count=standmodel->columnCount(); c<c_count;c++){
            standmodel->item(r,3)->setText(QString().sprintf("%.02f",num_total==0? 0.0: (standmodel->item(r, 2)->text().toDouble() / num_total) * 100) +"%");
            standmodel->item(r, 5)->setText(QString().sprintf("%.02f",amount_total==0? 0.0: (standmodel->item(r, 4)->text().toDouble() / amount_total) * 100) +"%");
            standmodel->item(r, 7)->setText(QString().sprintf("%.02f",discount_total==0? 0.0: (standmodel->item(r, 6)->text().toDouble() / discount_total) * 100) +"%");
            standmodel->item(r, 9)->setText(QString().sprintf("%.02f",present_total==0? 0.0: (standmodel->item(r, 8)->text().toDouble() / present_total) * 100) +"%");
            standmodel->item(r, 11)->setText(QString().sprintf("%.02f",realamount_total==0? 0.0: (standmodel->item(r, 10)->text().toDouble() / realamount_total) * 100) +"%");
        }
    }
    //更新表头
    standmodel->setHorizontalHeaderLabels(tab1info[ui->tabWidget->tabText(0)].headers);
    //制表人 和 制表时间
    ui->label_operator->setText(n_func::gs_opername);
    ui->label_operattime->setText(n_func::f_get_sysdatetime().toString("yyyy-MM-dd hh:mm:ss"));
    ui->label_timefrom->setText(ls_begin);
    ui->label_timeto->setText(ls_end);
    //更新合计
    ui->tableView->updateSum(2);
    ui->tableView->updateSum(4);
    ui->tableView->updateSum(6);
    ui->tableView->updateSum(8);
    ui->tableView->updateSum(10);

    //    querymodel->setQuery(tab1info[tr("品类销售营业分析")].sql
    //            .arg(ui->dateTimeEdit->dateTime().toString("yyyy-MM-dd hh:mm:ss"))
    //            .arg(ui->dateTimeEdit_2->dateTime().toString("yyyy-MM-dd hh:mm:ss"))
    //            .arg(backheader::getinterdata(tr("品类销售营业分析")))//获取过滤信息
    //            .arg(w_rpt_report::querymodelordersort(ui->tableView->tableView))
    //            );
    //    //更新表头
    //    backheader::modelSetHeader(querymodel, tab1info[ui->tabWidget->tabText(0)].headers);
    //    //制表人 和 制表时间
    //    ui->label_operator->setText(n_func::gs_opername);
    //    ui->label_operattime->setText(n_func::f_get_sysdatetime().toString("yyyy-MM-dd hh:mm:ss"));
    //    //更新合计
    //    QString sumsql=querymodel->query().lastQuery().trimmed();
    //    if(sumsql.endsWith("DESC")||sumsql.endsWith("ASC")){
    //        //5 6 7  9 10
    //        QSqlQuery query;
    //        query.exec(QString("select sum(tabname5),sum(tabname6),sum(tabname7),sum(tabname8)  from (%1)t1 ").arg(sumsql.left(sumsql.lastIndexOf("ORDER BY"))));
    //        query.next();
    //        qDebug() <<query.lastQuery();

    //        ui->tableView->updateSum(5,query.record().value(0).toString());
    //        ui->tableView->updateSum(6,query.record().value(1).toString());
    //        ui->tableView->updateSum(7,query.record().value(2).toString());
    //        ui->tableView->updateSum(8,query.record().value(3).toString());

    //    }
}

void w_rpt_report_categorysalesanalysis::tochart()
{
    lds_chart_nightcharts dialog(this);
    int color_h=240;
    int color_s=240;
    int color_v=240;

    dialog.chart->chart_items.clear();
    for(int r=0,
        r_count=standmodel->rowCount(),
        c=ui->comboBox_charttype->currentIndex()*2+2,
        c_count=standmodel->columnCount();

        r<r_count && c<c_count;

        r++){
        float cur_value=0.0;
        cur_value=standmodel->item(r, c)->text().toDouble();
        dialog.chart->chart_items.append(
                    lds_chart_nightcharts_content::CHART_ITEMS(standmodel->item(r, 0)->text(),
                                                               QColor::fromHsv(
                                                                   color_h*((r+1)*1.0)/r_count,
                                                                   color_s,
                                                                   color_v),
                                                               cur_value
                                                               ));
    }

    dialog.chart->chart_items_after();
    dialog.resize(backheader::mainwindowsize);
    backheader::static_dialogexec(dialog, tr("统计图"));
}

void w_rpt_report_categorysalesanalysis::toexit()
{
    this->reject();
}

void w_rpt_report_categorysalesanalysis::tocom12(const QString &text)
{

}
