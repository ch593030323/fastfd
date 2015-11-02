#include "w_rpt_report_cashreport.h"
//营业收银报表
#include "ui_w_rpt_report_cashreport_dialog.h"
#include "n_func.h"
#include "backheader.h"

w_rpt_report_cashreport::w_rpt_report_cashreport(QWidget *parent) :
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
    //! toolbar
    ui->widget->setup(QStringList()<<tr("查询")<<tr("退出"), Qt::AlignTop);
    connect(ui->widget->index_widget(tr("查询")),SIGNAL(clicked()),this,SLOT(toselect()));
    connect(ui->widget->index_widget(tr("退出")),SIGNAL(clicked()),this,SLOT(toexit()));
    connect(ui->comboBox_11,SIGNAL(currentIndexChanged(QString)),this,SLOT(tocom12(QString)));
    connect(ui->tableView->tableView->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(toselect()));

    //! after
    tocom12(ui->comboBox_11->currentText());

    //tabwidget header
    ui->tabWidget->setTabText(0, tr("营业收银报表"));

}

void w_rpt_report_cashreport::tab1sqlinit()
{
    tab1info.insert(tr("按餐桌"),
                    w_rpt_report::TABTYPE(QStringList()<<tr("餐桌")<<tr("收银方式")<<tr("收银次数")<<tr("收银金额")<<tr("面额差")<<tr("面额")<<tr("收入")<<tr("非收入")<<tr("收银百分比"),
                            QString (
                                " select "
                                " t.ch_tableno as 'tabname0',"
                                " (select vch_paymodename from cey_bt_paymode where cey_bt_paymode.ch_paymodeno=t.ch_paymodeno) as 'tabname1',"
                                " t.count as 'tabname2',"
                                " t.amount as 'tabname3',"
                                " t.face as 'tabname4',"
                                " t.diff as 'tabname5',"
                                " t.income as 'tabname6',"
                                " t.amount-t.income as 'tabname7',"
                                " t.amount/t.sumamount as 'tabname8' "//, sumamount as 'tabname8' "
                                " from ("
                                " Select b.ch_tableno,a.ch_paymodeno, count(a.ch_payno) as 'count', sum(a.num_realamount) as 'amount', sum(num_face) as 'face',sum(case when num_face <> 0 then num_face - num_realamount else 0 end) as 'diff', sum(case c.ch_incomeflag when 'Y' then num_realamount else 0 end) as 'income' ,"
                                " ("
                                " select sum(a.num_realamount)"
                                " from cey_u_checkout_detail a,cey_u_checkout_master b,cey_bt_paymode c  where (a.ch_payno = b.ch_payno) and (b.ch_state = 'Y') and (a.ch_paymodeno = c.ch_paymodeno) and b.dt_operdate > '%1' and b.dt_operdate <='%2' and (b.ch_tableno like '%3%' )"
                                ")as 'sumamount' "
                                " from cey_u_checkout_detail a,cey_u_checkout_master b,cey_bt_paymode c  where (a.ch_payno = b.ch_payno) and (b.ch_state = 'Y') and (a.ch_paymodeno = c.ch_paymodeno) and b.dt_operdate > '%1' and b.dt_operdate <='%2' and (b.ch_tableno like '%3%')group by b.ch_tableno,a.ch_paymodeno"
                                " )t ORDER BY tabname%4"
                                ),
                            w_rpt_report::querymodelsetfilters("select * from cey_bt_table")
                            ));

    tab1info.insert(tr("按餐桌类型"),
                    w_rpt_report::TABTYPE(QStringList()<<tr("餐桌类型")<<tr("收银方式")<<tr("收银次数")<<tr("收银金额")<<tr("面额差")<<tr("面额")<<tr("收入")<<tr("非收入")<<tr("收银百分比"),
                            QString (
                                " select "
                                " t.ch_typeno as 'tabname0',"
                                " (select vch_paymodename from cey_bt_paymode where cey_bt_paymode.ch_paymodeno=t.ch_paymodeno) as 'tabname1',"
                                " t.count as 'tabname2',"
                                " t.amount as 'tabname3',"
                                " t.face as 'tabname4',"
                                " t.diff as 'tabname5',"
                                " t.income as 'tabname6',"
                                " t.amount-t.income as 'tabname7',"
                                " t.amount/t.sumamount as 'tabname8' "//, sumamount as 'tabname8' "
                                " from ("
                                " Select d.ch_typeno,a.ch_paymodeno, count(a.ch_payno) as 'count', sum(a.num_realamount) as 'amount', sum(num_face) as 'face', sum(case when num_face <> 0 then num_face - num_realamount else 0 end)  as 'diff',  sum(case c.ch_incomeflag when 'Y' then num_realamount else 0 end)  as 'income' ,"
                                " ("
                                " select sum(a.num_realamount)"
                                " from cey_u_checkout_detail a,cey_u_checkout_master b,cey_bt_paymode c ,(select ch_tableno,ch_typeno from cey_bt_table union select 'Q000' as 'ch_tableno','Q0' as 'ch_typeno') as d where b.ch_tableno = d.ch_tableno and (a.ch_payno = b.ch_payno) and (b.ch_state = 'Y') and (a.ch_paymodeno = c.ch_paymodeno) and b.dt_operdate > '%1' and b.dt_operdate <='%2' and (d.ch_typeno like '%3%' )"
                                ")as 'sumamount'"
                                " from cey_u_checkout_detail a,cey_u_checkout_master b,cey_bt_paymode c ,(select ch_tableno,ch_typeno from cey_bt_table union select 'Q000' as 'ch_tableno','Q0' as 'ch_typeno') as d where b.ch_tableno = d.ch_tableno and (a.ch_payno = b.ch_payno) and (b.ch_state = 'Y') and (a.ch_paymodeno = c.ch_paymodeno) and b.dt_operdate > '%1' and b.dt_operdate <='%2' and (d.ch_typeno like '%3%')group by d.ch_typeno,a.ch_paymodeno"
                                " )t ORDER BY tabname%4"
                                ),
                            w_rpt_report::querymodelsetfilters("select * from cey_bt_table_area")
                            ));

    tab1info.insert(tr("按收银员"),
                    w_rpt_report::TABTYPE(QStringList()<<tr("收银员")<<tr("收银方式")<<tr("收银次数")<<tr("收银金额")<<tr("面额差")<<tr("面额")<<tr("收入")<<tr("非收入")<<tr("收银百分比"),
                            QString (
                                " select "
                                " vch_operID as 'tabname0',"
                                " (select vch_paymodename from cey_bt_paymode where cey_bt_paymode.ch_paymodeno=t.ch_paymodeno) as 'tabname1',"
                                " t.count as 'tabname2',"
                                " t.amount as 'tabname3',"
                                " t.face as 'tabname4',"
                                " t.diff as 'tabname5',"
                                " t.income as 'tabname6',"
                                " t.amount-t.income as 'tabname7',"
                                " t.amount/t.sumamount as 'tabname8' "//, sumamount as 'tabname8' "
                                " from ("
                                " Select a.vch_operID,a.ch_paymodeno, count(a.ch_payno) as 'count',  sum(a.num_realamount) as 'amount',sum(num_face) as 'face',sum(case when num_face <> 0 then num_face - num_realamount else 0 end) as 'diff', sum(case c.ch_incomeflag when 'Y' then num_realamount else 0 end) as 'income',  "
                                " ("
                                " select sum(a.num_realamount)"
                                " from cey_u_checkout_detail a,cey_u_checkout_master b,cey_bt_paymode c  where (a.ch_payno = b.ch_payno) and (b.ch_state = 'Y') and (a.ch_paymodeno = c.ch_paymodeno) and b.dt_operdate > '%1' and b.dt_operdate <='%2' and (a.vch_operID like '%3%' )"
                                " )as 'sumamount'"
                                " from cey_u_checkout_detail a,cey_u_checkout_master b,cey_bt_paymode c  where (a.ch_payno = b.ch_payno) and (b.ch_state = 'Y') and (a.ch_paymodeno = c.ch_paymodeno) and b.dt_operdate > '%1' and b.dt_operdate <='%2' and (a.vch_operID like '%3%')group by a.vch_operID,a.ch_paymodeno"
                                " )t ORDER BY tabname%4"
                                ),
                            w_rpt_report::querymodelsetfilters("Select vch_operID,vch_operator from cey_sys_operator union Select '0000' as vch_operID,'system' as vch_operator ")
                            ));

    tab1info.insert(tr("按收银明细"),
                    w_rpt_report::TABTYPE(QStringList()<<tr("付款号")<<tr("餐桌")<<tr("收银方式")<<tr("付款金额")<<tr("找零面额")<<tr("实收(收银)面额")<<tr("面额")<<tr("面额差")<<tr("凭证号")<<tr("收银员")<<tr("收银时间"),
                            QString (
                                " Select "
                                " a.ch_payno as 'tabname0',"
                                "  b.ch_tableno as 'tabname1',"
                                " (select vch_paymodename from cey_bt_paymode where cey_bt_paymode.ch_paymodeno=c.ch_paymodeno) as 'tabname2',"
                                "  a.num_payamount as 'tabname3',"
                                "  a.num_change as 'tabname4',"
                                "  a.num_realamount as 'tabname5',"
                                "  a.num_face as 'tabname6',"
                                "  (case when a.num_face <> 0 then a.num_face-a.num_realamount else 0 end) as 'tabname7',"
                                " a.vch_voucherno as 'tabname8',"
                                " b.vch_operID as 'tabname9',"
                                "  b.dt_operdate as 'tabname10'"
                                " from cey_u_checkout_detail a,"
                                " cey_u_checkout_master b,"
                                " cey_bt_paymode c"
                                " WHERE a.ch_payno = b.ch_payno"
                                " and b.ch_state = 'Y'"
                                " and a.ch_paymodeno = c.ch_paymodeno"
                                " and b.dt_operdate > '%1' and b.dt_operdate <='%2'"
                                " and '%3'='%3'"
                                " ORDER BY tabname%4"

                                ),
                            w_rpt_report::querymodelsetfilters(""),
                            QSet<int>()<<2<<8//去除2 和 8
                            ));

    tab1info.insert(tr("按班次营业时段"),
                    w_rpt_report::TABTYPE(QStringList()<<tr("班次")<<tr("收银方式")<<tr("收银次数")<<tr("收银金额")<<tr("面额差")<<tr("面额")<<tr("收入")<<tr("非收入")<<tr("收银百分比"),
                            QString (
                                " select "
                                " t.typeno as 'tabname0',"
                                " (select vch_paymodename from cey_bt_paymode where cey_bt_paymode.ch_paymodeno =t.ch_paymodeno) as 'tabname1',"
                                " t.count as 'tabname2',"
                                " t.amount as 'tabname3',"
                                " t.face as 'tabname4',"
                                " t.diff as 'tabname5',"
                                " t.income as 'tabname6',"
                                " t.amount-t.income as 'tabname7',"
                                " t.amount/t.sumamount as 'tabname8' "//, sumamount as 'tabname8' "
                                " from("
                                " Select BB.typeno, ch_paymodeno, sum(BB.count) as 'count',  sum(BB.amount) as 'amount', sum(BB.face) as 'face',  sum(BB.diff) as 'diff',  sum(BB.income)  as 'income', "
                                " ("
                                " select sum(BB.amount)"
                                " From( Select Case When  DATE_FORMAT(b.dt_operdate, '%Y-%m-%d') <'' Then '' Else (Select  ch_classno From cey_bt_class where   DATE_FORMAT(dt_begin, '%Y-%m-%d')  < DATE_FORMAT(b.dt_operdate, '%Y-%m-%d')    Order By dt_begin Desc LIMIT 1) End as 'typeno', a.ch_paymodeno,count(a.ch_payno) as 'count',sum(a.num_realamount) as 'amount',sum(num_face) as 'face',sum(case when num_face <> 0 then num_face - num_realamount else 0 end) as 'diff',sum(case c.ch_incomeflag when 'Y' then num_realamount else 0 end) as 'income'  from cey_u_checkout_detail a,cey_u_checkout_master b,cey_bt_paymode c  where (a.ch_payno = b.ch_payno) and (b.ch_state = 'Y') and (a.ch_paymodeno = c.ch_paymodeno) and b.dt_operdate > '%1' and b.dt_operdate <='%2' group by b.dt_operdate, a.ch_paymodeno) BB Where BB.typeno like '%3%'"
                                ")as'sumamount'"
                                " From( Select Case When  DATE_FORMAT(b.dt_operdate, '%Y-%m-%d') < '' Then '' Else (Select  ch_classno From cey_bt_class where  DATE_FORMAT(dt_begin, '%Y-%m-%d')  < DATE_FORMAT(b.dt_operdate, '%Y-%m-%d')    Order By dt_begin Desc LIMIT 1) End as 'typeno', a.ch_paymodeno,count(a.ch_payno) as 'count',sum(a.num_realamount) as 'amount',sum(num_face) as 'face',sum(case when num_face <> 0 then num_face - num_realamount else 0 end) as 'diff',sum(case c.ch_incomeflag when 'Y' then num_realamount else 0 end) as 'income' from cey_u_checkout_detail a,cey_u_checkout_master b,cey_bt_paymode c  where (a.ch_payno = b.ch_payno) and (b.ch_state = 'Y') and (a.ch_paymodeno = c.ch_paymodeno) and b.dt_operdate > '%1' and b.dt_operdate <='%2' group by b.dt_operdate, a.ch_paymodeno) BB Where BB.typeno like '%3%'"
                                " group by BB.typeno, ch_paymodeno "
                                " )t ORDER BY tabname%4"
                                ),
                            w_rpt_report::querymodelsetfilters("select ch_classno, vch_classname from cey_bt_class")
                            ));
}

void w_rpt_report_cashreport::toselect()
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
            query.exec(QString("select sum(tabname2),sum(tabname3),sum(tabname4),sum(tabname5),sum(tabname6),sum(tabname7),sum(tabname8) from (%1)t1 ").arg(sumsql.left(sumsql.lastIndexOf("ORDER BY"))));
            query.next();

            ui->tableView->updateSum(2,query.record().value(0).toString());
            ui->tableView->updateSum(3,query.record().value(1).toString());
            ui->tableView->updateSum(4,query.record().value(2).toString());
            ui->tableView->updateSum(5,query.record().value(3).toString());
            ui->tableView->updateSum(6,query.record().value(4).toString());
            ui->tableView->updateSum(7,query.record().value(5).toString());
            ui->tableView->updateSum(8,query.record().value(6).toString());
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

void w_rpt_report_cashreport::toexit()
{
    this->reject();
}

void w_rpt_report_cashreport::tocom12(const QString &text)
{
    ui->comboBox_12->clear();
    ui->comboBox_12->addItems(tab1info[text].filters);
}
