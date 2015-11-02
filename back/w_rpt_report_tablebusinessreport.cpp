#include "w_rpt_report_tablebusinessreport.h"
//餐桌营业数据统计
#include "ui_w_rpt_report_tablebusinessreport_dialog.h"
#include "n_func.h"
#include "backheader.h"
w_rpt_report_tablebusinessreport::w_rpt_report_tablebusinessreport(QWidget *parent) :
    QDialog(parent),
    ui(new Ui_w_rpt_report_tablebusinessreport_Dialog)
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
    //! toolbar
    ui->widget->setup(QStringList()<<tr("查询")<<tr("退出"), Qt::AlignTop);
    connect(ui->widget->index_widget(tr("查询")),SIGNAL(clicked()),this,SLOT(toselect()));
    connect(ui->widget->index_widget(tr("退出")),SIGNAL(clicked()),this,SLOT(toexit()));
    connect(ui->comboBox_11,SIGNAL(currentIndexChanged(int)),this,SLOT(tocom12(int)));
    connect(ui->tableView->tableView->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(toselect()));

    //! after
    ui->tabWidget->setTabText(0, "");
    ui->comboBox_11->addItems(tab1info.keys());
    ui->comboBox_13->addItems(w_rpt_report::querymodelsetfilters("select * from cey_bt_table"));
    //tabwidget header
    ui->tabWidget->setTabText(0, tr("餐桌营业数据统计"));
}

void w_rpt_report_tablebusinessreport::tab1sqlinit()
{
    tab1info.insert(tr("区域统计"),
                    w_rpt_report::TABTYPE(QStringList()<<tr("楼层")<<tr("桌号")<<tr("桌名")<<tr("开台次数")<<tr("比率")<<tr("人数")<<tr("比率")<<tr("实际金额")<<tr("比率")<<tr("人均"),
                            QString ("SELECT (select vch_areaname from cey_bt_table_area where ch_areano = typeno) as 'tabname0',"

                                     "       tableno as 'tabname1',"

                                     "       tablename as 'tabname2',"

                                     "       opentable as 'tabname3',"

                                     "       opentable /"

                                     "       (Select count(a.ch_tableno)"

                                     "          from cey_u_table a, cey_u_checkout_master b"

                                     "         where a.ch_payno = b.ch_payno"

                                     "           and b.ch_state = 'Y'"

                                     "           and DATE_FORMAT(b.dt_operdate, '%Y-%m-%d %h:%i:%s') >="

                                     "               '%1'"

                                     "           and DATE_FORMAT(b.dt_operdate, '%Y-%m-%d %h:%i:%s') <="

                                     "               '%2'"

                                     "           and DATE_FORMAT(b.dt_operdate, '%Y') >= '0'"

                                     "           and DATE_FORMAT(b.dt_operdate, '%Y') <= '3') as 'tabname4',"

                                     "       person as 'tabname5',"

                                     "       person / (Select IFNULL(sum(a.int_person), 0)"

                                     "                   from cey_u_table a, cey_u_checkout_master b"

                                     "                  where a.ch_payno = b.ch_payno"

                                     "                    and b.ch_state = 'Y'"

                                     "                    and DATE_FORMAT(b.dt_operdate, '%Y-%m-%d %h:%i:%s') >="

                                     "                        '%1'"

                                     "                    and DATE_FORMAT(b.dt_operdate, '%Y-%m-%d %h:%i:%s') <="

                                     "                        '%2'"

                                     "                    and DATE_FORMAT(b.dt_operdate, '%Y') >= '0'"

                                     "                    and DATE_FORMAT(b.dt_operdate, '%Y') <= '3') as 'tabname6',"

                                     "       amount as 'tabname7',"

                                     "       amount / ("

                                     "                 "

                                     "                 Select IFNULL(sum(aa.num_realamount), 0)"

                                     "                   from cey_u_checkout_detail aa,"

                                     "                         cey_u_checkout_master bb,"

                                     "                         cey_bt_paymode        cc"

                                     "                  where aa.ch_payno = bb.ch_payno"

                                     "                    and bb.ch_state = 'Y'"

                                     "                    and aa.ch_paymodeno = cc.ch_paymodeno"

                                     "                    and cc.ch_incomeflag = 'Y'"

                                     "                    and DATE_FORMAT(bb.dt_operdate, '%Y-%m-%d %h:%i:%s') >="

                                     "                        '%1'"

                                     "                    and DATE_FORMAT(bb.dt_operdate, '%Y-%m-%d %h:%i:%s') <="

                                     "                        '%2'"

                                     "                    and DATE_FORMAT(bb.dt_operdate, '%Y') >= '0'"

                                     "                    and DATE_FORMAT(bb.dt_operdate, '%Y') <= '3'"

                                     "                 "

                                     "                )as 'tabname8',"

                                     "       if(ifnull(amount / person), 0, (amount / person)) as 'tabname9'"

                                     "  FROM (SELECT ch_areano as 'typeno',"

                                     "               ch_tableno as 'tableno',"

                                     "               vch_tablename as 'tablename',"

                                     "               IFNULL((Select count(a.ch_tableno)"

                                     "                        from cey_u_table a, cey_u_checkout_master b"

                                     "                       where a.ch_payno = b.ch_payno"

                                     "                         and b.ch_state = 'Y'"

                                     "                         and DATE_FORMAT(b.dt_operdate, '%Y-%m-%d %h:%i:%s') >="

                                     "                             '%1'"

                                     "                         and DATE_FORMAT(b.dt_operdate, '%Y-%m-%d %h:%i:%s') <="

                                     "                             '%2'"

                                     "                         and DATE_FORMAT(b.dt_operdate, '%Y') >= '0'"

                                     "                         and DATE_FORMAT(b.dt_operdate, '%Y') <= '3'"

                                     "                         and zz.ch_tableno = a.ch_tableno),"

                                     "                      0) as 'opentable',"

                                     "               IFNULL((Select IFNULL(sum(a.int_person), 0)"

                                     "                        from cey_u_table a, cey_u_checkout_master b"

                                     "                       where a.ch_payno = b.ch_payno"

                                     "                         and b.ch_state = 'Y'"

                                     "                         and DATE_FORMAT(b.dt_operdate, '%Y-%m-%d %h:%i:%s') >="

                                     "                             '%1'"

                                     "                         and DATE_FORMAT(b.dt_operdate, '%Y-%m-%d %h:%i:%s') <="

                                     "                             '%2'"

                                     "                         and DATE_FORMAT(b.dt_operdate, '%Y') >= '0'"

                                     "                         and DATE_FORMAT(b.dt_operdate, '%Y') <= '3'"

                                     "                         and zz.ch_tableno = a.ch_tableno),"

                                     "                      0) as 'person',"

                                     "               IFNULL((Select IFNULL(sum(aa.num_realamount), 0)"

                                     "                        from cey_u_checkout_detail aa,"

                                     "                             cey_u_checkout_master bb,"

                                     "                             cey_bt_paymode        cc"

                                     "                       where aa.ch_payno = bb.ch_payno"

                                     "                         and bb.ch_state = 'Y'"

                                     "                         and bb.ch_tableno = ZZ.ch_tableno"

                                     "                         and aa.ch_paymodeno = cc.ch_paymodeno"

                                     "                         and cc.ch_incomeflag = 'Y'"

                                     "                         and DATE_FORMAT(bb.dt_operdate, '%Y-%m-%d %h:%i:%s') >="

                                     "                             '%1'"

                                     "                         and DATE_FORMAT(bb.dt_operdate, '%Y-%m-%d %h:%i:%s') <="

                                     "                             '%2'"

                                     "                         and DATE_FORMAT(bb.dt_operdate, '%Y') >= '0'"

                                     "                         and DATE_FORMAT(bb.dt_operdate, '%Y') <= '3'),"

                                     "                      0) as 'amount'"

                                     "          FROM (select ch_areano, ch_tableno, vch_tablename"

                                     "                  from cey_bt_table"

                                     "                union"

                                     "                select 'Q0', 'Q000', 'Q000') ZZ"

                                     "         where ZZ.ch_areano like '%3%'"

                                     "           and ZZ.ch_tableno like '%4%'"

                                     "         group by ch_areano, ch_tableno, vch_tablename) t"

                                     " ORDER BY tabname%5"
                                     )
                            )
                    );
    tab1info.insert(tr("桌类统计"),
                    w_rpt_report::TABTYPE(QStringList()<<tr("桌类")<<tr("桌号")<<tr("桌名")<<tr("开台次数")<<tr("比率")<<tr("人数")<<tr("比率")<<tr("实际金额")<<tr("比率")<<tr("人均"),
                            QString ("SELECT (select vch_typename from cey_bt_table_type where ch_typeno = typeno) as 'tabname0',"

                                     "       tableno as 'tabname1',"

                                     "       tablename as 'tabname2',"

                                     "       opentable as 'tabname3',"

                                     "       opentable / (SELECT sum(1)"

                                     "                        from cey_u_table aa, cey_u_checkout_master bb"

                                     "                       where aa.ch_payno = bb.ch_payno"

                                     "                         and bb.ch_state = 'Y'"

                                     "                         and (DATE_FORMAT(bb.dt_operdate,"

                                     "                                          '%Y-%m-%d %h:%i:%s') >="

                                     "                             '%1')"

                                     "                         and (DATE_FORMAT(bb.dt_operdate,"

                                     "                                          '%Y-%m-%d %h:%i:%s') <="

                                     "                             '%2')"

                                     "                         and (DATE_FORMAT(bb.dt_operdate, '%Y') >= '0')"

                                     "                         and (DATE_FORMAT(bb.dt_operdate, '%Y') <= '3')) as 'tabname4',"

                                     "       person as 'tabname5',"

                                     "       person / (SELECT sum(cc.int_person)"

                                     "                        from cey_u_table cc, cey_u_checkout_master dd"

                                     "                       where cc.ch_payno = dd.ch_payno"

                                     "                         and dd.ch_state = 'Y'"

                                     "                         and (DATE_FORMAT(dd.dt_operdate,"

                                     "                                          '%Y-%m-%d %h:%i:%s') >="

                                     "                             '%1')"

                                     "                         and (DATE_FORMAT(dd.dt_operdate,"

                                     "                                          '%Y-%m-%d %h:%i:%s') <="

                                     "                             '%2')"

                                     "                         and (DATE_FORMAT(dd.dt_operdate, '%Y') >= '0')"

                                     "                         and (DATE_FORMAT(dd.dt_operdate, '%Y') <= '3')) as 'tabname6',"

                                     "       amount as 'tabname7',"

                                     "       amount / (Select IFNULL(sum(aa.num_realamount), 0)"

                                     "                        from cey_u_checkout_detail aa,"

                                     "                             cey_u_checkout_master bb,"

                                     "                             cey_bt_paymode        cc"

                                     "                       where aa.ch_payno = bb.ch_payno"

                                     "                         and bb.ch_state = 'Y'"

                                     "                         and aa.ch_paymodeno = cc.ch_paymodeno"

                                     "                         and cc.ch_incomeflag = 'Y'"

                                     "                         and (DATE_FORMAT(bb.dt_operdate,"

                                     "                                          '%Y-%m-%d %h:%i:%s') >="

                                     "                             '%1')"

                                     "                         and (DATE_FORMAT(bb.dt_operdate,"

                                     "                                          '%Y-%m-%d %h:%i:%s') <="

                                     "                             '%2')"

                                     "                         and (DATE_FORMAT(bb.dt_operdate, '%Y') >= '0')"

                                     "                         and (DATE_FORMAT(bb.dt_operdate, '%Y') <= '3')) as 'tabname8',"

                                     "       if(ifnull(amount / person), 0, (amount / person)) as 'tabname9'"

                                     "  FROM (SELECT ch_typeno as 'typeno',"

                                     "               ch_tableno as 'tableno',"

                                     "               vch_tablename as 'tablename',"

                                     "               IFNULL((SELECT sum(1)"

                                     "                        from cey_u_table aa, cey_u_checkout_master bb"

                                     "                       where aa.ch_payno = bb.ch_payno"

                                     "                         and aa.ch_tableno = A.ch_tableno"

                                     "                         and bb.ch_state = 'Y'"

                                     "                         and (DATE_FORMAT(bb.dt_operdate,"

                                     "                                          '%Y-%m-%d %h:%i:%s') >="

                                     "                             '%1')"

                                     "                         and (DATE_FORMAT(bb.dt_operdate,"

                                     "                                          '%Y-%m-%d %h:%i:%s') <="

                                     "                             '%2')"

                                     "                         and (DATE_FORMAT(bb.dt_operdate, '%Y') >= '0')"

                                     "                         and (DATE_FORMAT(bb.dt_operdate, '%Y') <= '3')),"

                                     "                      0) as 'opentable',"

                                     "               IFNULL((SELECT sum(cc.int_person)"

                                     "                        from cey_u_table cc, cey_u_checkout_master dd"

                                     "                       where cc.ch_payno = dd.ch_payno"

                                     "                         and cc.ch_tableno = A.ch_tableno"

                                     "                         and dd.ch_state = 'Y'"

                                     "                         and (DATE_FORMAT(dd.dt_operdate,"

                                     "                                          '%Y-%m-%d %h:%i:%s') >="

                                     "                             '%1')"

                                     "                         and (DATE_FORMAT(dd.dt_operdate,"

                                     "                                          '%Y-%m-%d %h:%i:%s') <="

                                     "                             '%2')"

                                     "                         and (DATE_FORMAT(dd.dt_operdate, '%Y') >= '0')"

                                     "                         and (DATE_FORMAT(dd.dt_operdate, '%Y') <= '3')),"

                                     "                      0) as 'person',"

                                     "               IFNULL((Select IFNULL(sum(aa.num_realamount), 0)"

                                     "                        from cey_u_checkout_detail aa,"

                                     "                             cey_u_checkout_master bb,"

                                     "                             cey_bt_paymode        cc"

                                     "                       where aa.ch_payno = bb.ch_payno"

                                     "                         and bb.ch_state = 'Y'"

                                     "                         and bb.ch_tableno = A.ch_tableno"

                                     "                         and aa.ch_paymodeno = cc.ch_paymodeno"

                                     "                         and cc.ch_incomeflag = 'Y'"

                                     "                         and (DATE_FORMAT(bb.dt_operdate,"

                                     "                                          '%Y-%m-%d %h:%i:%s') >="

                                     "                             '%1')"

                                     "                         and (DATE_FORMAT(bb.dt_operdate,"

                                     "                                          '%Y-%m-%d %h:%i:%s') <="

                                     "                             '%2')"

                                     "                         and (DATE_FORMAT(bb.dt_operdate, '%Y') >= '0')"

                                     "                         and (DATE_FORMAT(bb.dt_operdate, '%Y') <= '3')),"

                                     "                      0) as 'amount'"

                                     "          FROM (select ch_typeno, ch_areano, ch_tableno, vch_tablename"

                                     "                  from cey_bt_table"

                                     "                union"

                                     "                select 'Q0', 'Q0', 'Q000', 'Q000') A"

                                     "         where A.ch_typeno like '%3%'"

                                     "           and A.ch_tableno like '%4%'"

                                     "         group by ch_typeno, ch_tableno, vch_tablename) t"

                                     " ORDER BY tabname%5"
                                     )
                            )
                    );
}

void w_rpt_report_tablebusinessreport::toselect()
{
    QString ls_begin;
    QString ls_end;
    ls_begin=n_func::uf_getdatetime("begin", ui->dateTimeEdit->dateTime()).toString("yyyy-MM-dd hh:mm");
    ls_end=n_func::uf_getdatetime("end", ui->dateTimeEdit_2->dateTime()).toString("yyyy-MM-dd hh:mm");

    //model select
    querymodel->setQuery(tab1info[ui->comboBox_11->currentText()].sql
            .arg(ls_begin.isEmpty()?ui->dateTimeEdit->dateTime().toString("yyyy-MM-dd hh:mm:ss"):ls_begin)
            .arg(ls_end.isEmpty()?ui->dateTimeEdit_2->dateTime().toString("yyyy-MM-dd hh:mm:ss"):ls_end)
            .arg(backheader::getinterdata(ui->comboBox_12->currentText()))
            .arg(backheader::getinterdata(ui->comboBox_13->currentText()))
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

    if(sumsql.endsWith("DESC")||sumsql.endsWith("ASC")){
        //5 6 7  9 10
        QSqlQuery query;

        if(tab1info[ui->comboBox_11->currentText()].except_c.isEmpty()){
            query.exec(QString("select sum(tabname3),sum(tabname5),sum(tabname7),sum(tabname9) from (%1)t1 ").arg(sumsql.left(sumsql.lastIndexOf("ORDER BY"))));
            query.next();

            ui->tableView->updateSum(3,query.record().value(0).toString());

            ui->tableView->updateSum(5,query.record().value(1).toString());

            ui->tableView->updateSum(7,query.record().value(2).toString());

            ui->tableView->updateSum(9,query.record().value(3).toString());

        }
    }
}

void w_rpt_report_tablebusinessreport::toexit()
{
    this->reject();
}

void w_rpt_report_tablebusinessreport::tocom12(int index)
{
    ui->comboBox_12->clear();
    switch(index){
    case 0:
        ui->comboBox_12->addItems(w_rpt_report::querymodelsetfilters("select * from cey_bt_table_area"));
        break;
    case 1:
        ui->comboBox_12->addItems(w_rpt_report::querymodelsetfilters("select * from cey_bt_table_type"));
        break;
    }
}
