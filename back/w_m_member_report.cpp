#include "w_m_member_report.h"
//会员报表
#include <QtDebug>
#include <QHeaderView>
#include <QTableView>
#include "w_m_member.h"
#include "n_func.h"
#include "backheader.h"
#include <QSqlError>

w_m_member_report::w_m_member_report(QWidget *parent) :
    QDialog(parent),
    ui(new Ui_w_m_member_report_Dialog)
{
    ui->setupUi(this);
    ui->frame->setFixedWidth(w_rpt_report::frame_left_fixed_width);

    ui->dateTimeEdit->setDateTime(QDateTime(n_func::f_get_sysdatetime().date(), QTime(0,0,0)));
    ui->dateTimeEdit_2->setDateTime(QDateTime(n_func::f_get_sysdatetime().date(), QTime(23,59,59)));


    {
        QStandardItemModel *com_member_model=new QStandardItemModel;
        QSqlQuery query;
        query.exec("select ch_typeno, vch_typename from t_m_member_type ");
        QTableView *com_tab;
        com_member_model->appendRow(QList<QStandardItem*>()
                                    << new QStandardItem("") << new QStandardItem(tr("全部")));
        while(query.next()){
            com_member_model->appendRow(QList<QStandardItem*>()
                                        << new QStandardItem(query.record().value("ch_typeno").toString())
                                        << new QStandardItem(query.record().value("vch_typename").toString())
                                        );
        }
        com_member_model->setHorizontalHeaderLabels(QStringList() << tr("编号") << tr("类型"));
        ui->comboBox_membertype->setMaxVisibleItems(5);
        ui->comboBox_membertype->setView(com_tab=new QTableView);com_tab->setColumnWidth(0, 50);//com_tab->setColumnWidth(1, 50);
        ui->comboBox_membertype->setModel(com_member_model);}


    {QStandardItemModel *com_member_model=new QStandardItemModel;
        QTableView *com_tab;
        com_member_model->appendRow(QList<QStandardItem*>()
                                    << new QStandardItem("") << new QStandardItem(tr("全部")));
        for(int row=0,rowcount=w_m_member::VIP_MODEL_STATE.rowCount();row<rowcount;row++){
            com_member_model->appendRow(QList<QStandardItem*>()
                                        << new QStandardItem(w_m_member::VIP_MODEL_STATE.item(row, 0)->text())
                                        << new QStandardItem(w_m_member::VIP_MODEL_STATE.item(row, 1)->text())
                                        );
        }
        com_member_model->setHorizontalHeaderLabels(QStringList() << tr("编号") << tr("状态"));
        ui->comboBox_13->setMaxVisibleItems(5);
        ui->comboBox_13->setView(com_tab=new QTableView);com_tab->setColumnWidth(0, 50);//com_tab->setColumnWidth(1, 50);
        ui->comboBox_13->setModel(com_member_model);}

    //! sqlinit
    tab1sqlinit();

    querymodel=new datetimeModel;
    ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::SelectedClicked);ui->tableView->setModel(querymodel);
    ui->tableView->tableView->setSortingEnabled(true);
    querymodel_1=new datetimeModel;
    ui->tableView_2->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::SelectedClicked);ui->tableView_2->setModel(querymodel_1);
    ui->tableView_2->tableView->setSortingEnabled(true);
    querymodel_2=new datetimeModel;
    ui->tableView_3->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::SelectedClicked);ui->tableView_3->setModel(querymodel_2);
    ui->tableView_3->tableView->setSortingEnabled(true);

    querymodel_3=new datetimeModel;
    ui->tableView_4->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::SelectedClicked);ui->tableView_4->setModel(querymodel_3);
    ui->tableView_4->tableView->setSortingEnabled(true);
    querymodel_4=new datetimeModel;
    ui->tableView_5->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::SelectedClicked);ui->tableView_5->setModel(querymodel_4);
    ui->tableView_5->tableView->setSortingEnabled(true);
    querymodel_5=new datetimeModel;
    ui->tableView_6->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::SelectedClicked);ui->tableView_6->setModel(querymodel_5);
    ui->tableView_6->tableView->setSortingEnabled(true);
    //! toolbar
    ui->widget->setup(QStringList()<<tr("查询")<<tr("退出"), Qt::AlignTop);
    connect(ui->widget->index_widget(tr("查询")),SIGNAL(clicked()),this,SLOT(toselect()));
    connect(ui->widget->index_widget(tr("退出")),SIGNAL(clicked()),this,SLOT(toexit()));
    connect(ui->tableView->tableView->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(toselect()));
    connect(ui->tabWidget,SIGNAL(currentChanged(int)),this,SLOT(toupdate_tabw_list(int)));

}

void w_m_member_report::tab1sqlinit()
{

    tab1info.insert(tr("会员充值付款汇总报表"),
                    w_rpt_report::TABTYPE(QStringList()<<tr("会员编号")<<tr("会员名称")<<tr("卡面卡号")<<tr("卡类型")<<tr("充值金额")<<tr("实收金额")<<tr("差额")<<tr("优惠")<<tr("付款金额"),
                                          QString("select vch_memberno as tabname0,"
                                                  "vch_member as tabname1,"
                                                  "vch_cardno as tabname2,"
                                                  "vch_typename as tabname3,"
                                                  "num_deposit as tabname4,"
                                                  "num_realamount as tabname5,"
                                                  "ce as tabname6,"
                                                  "yh as tabname7,"
                                                  "num_pay as tabname8  from (%1)t where (%4 or  t.num_deposit <> 0) ORDER BY tabname%5"
                                                  )
                                          .arg(QString(
                                                   "SELECT t_m_member.vch_memberno as vch_memberno,"

                                                   " t_m_member.vch_member as vch_member,"

                                                   " t_m_member.vch_cardno as vch_cardno,"

                                                   " t_m_member_type.vch_typename as vch_typename,"

                                                   " (select ifnull(sum(case when t_m_deposit.ch_deposit_mode = '4' then 0 else t_m_deposit.num_deposit end), 0)"

                                                   " from t_m_deposit where t_m_deposit.vch_memberno = t_m_member.vch_memberno"

                                                   " and DATE_FORMAT(t_m_deposit.dt_operdate, '%Y-%m-%d %h:%i:%s') between '%1' and '%2') "
                                                   "as num_deposit,"

                                                   " (select ifnull(sum(t_m_deposit.num_realamount), 0) from t_m_deposit  where t_m_deposit.vch_memberno = t_m_member.vch_memberno"

                                                   " and DATE_FORMAT(t_m_deposit.dt_operdate, '%Y-%m-%d %h:%i:%s') between '%1' and '%2') as num_realamount, "

                                                   "(select sum(-case when"

                                                   " ifnull(case  when t_m_deposit.ch_deposit_mode = '4' then  0  else  t_m_deposit.num_deposit  end,"

                                                   " 0) - ifnull(t_m_deposit.num_realamount, 0) > 0 then 0 else"

                                                   " ifnull(case"

                                                   " when t_m_deposit.ch_deposit_mode = '4' then"

                                                   " 0"

                                                   " else"

                                                   " t_m_deposit.num_deposit"

                                                   " end,"

                                                   " 0) - ifnull(t_m_deposit.num_realamount, 0) end)"

                                                   " from t_m_deposit"

                                                   " where t_m_deposit.vch_memberno = t_m_member.vch_memberno"

                                                   " and DATE_FORMAT(t_m_deposit.dt_operdate, '%Y-%m-%d %h:%i:%s') between"

                                                   " '%1' and '%2') as ce ,"

                                                   " t_m_member.ch_state as ch_state,"

                                                   " (select sum(case"

                                                   " when ifnull(case"

                                                   " when t_m_deposit.ch_deposit_mode = '4' then"

                                                   " 0"

                                                   " else"

                                                   " t_m_deposit.num_deposit"

                                                   " end,"

                                                   " 0) - ifnull(t_m_deposit.num_realamount, 0) > 0 then"

                                                   " ifnull(case"

                                                   " when t_m_deposit.ch_deposit_mode = '4' then"

                                                   " 0"

                                                   " else"

                                                   " t_m_deposit.num_deposit"

                                                   " end,"

                                                   " 0) - ifnull(t_m_deposit.num_realamount, 0)"

                                                   " else"

                                                   " 0"

                                                   " end)"

                                                   " from t_m_deposit"

                                                   " where t_m_deposit.vch_memberno = t_m_member.vch_memberno"

                                                   " and DATE_FORMAT(t_m_deposit.dt_operdate, '%Y-%m-%d %h:%i:%s') between"

                                                   " '%1' and '%2') as yh,"

                                                   " (select ifnull(sum(num_pay), 0)"

                                                   " from t_m_pay"

                                                   " where t_m_pay.vch_memberno = t_m_member.vch_memberno"

                                                   " and t_m_pay.dt_operdate between '%1' and"

                                                   " '%2') as num_pay"

                                                   " FROM t_m_member, t_m_member_type"

                                                   " WHERE (t_m_member.ch_typeno = t_m_member_type.ch_typeno)"

                                                   " and (t_m_member_type.ch_attribute = '2')"

                                                   " AND (t_m_member.vch_memberno like '%3%' or t_m_member.vch_member like '%3%')"

                                                   " and t_m_member.ch_state like '%6%' "
                                                   )))
                    );
    tab1info.insert(tr("会员余额积分情况"),
                    w_rpt_report::TABTYPE(QStringList()<<tr("会员编号")<<tr("会员类型")<<tr("会员种类")<<tr("联系电话")<<tr("余额/可借记额")<<tr("积分值")<<tr("最近业务时间"),
                                          QString("select vch_memberno as tabname0,"
                                                  "vch_typename as tabname1,"
                                                  "vch_att as tabname2,"
                                                  "vch_telno as tabname3,"
                                                  "num_amount as tabname4,"
                                                  "num_point as tabname5,"
                                                  "dt_operdate as tabname6  from (%1)t  ORDER BY tabname%4"
                                                  )
                                          .arg(QString(
                                                   "Select	a.vch_memberno as vch_memberno,"
                                                   "a.vch_member as vch_member,"
                                                   "b.vch_typename as vch_typename,"
                                                   " (case b.ch_attribute when '2' then '"+tr("充值卡")+"' when '1' then '"+tr("优惠卡")+"' else '' end) as vch_att,"
                                                   "(case length(ifnull(a.vch_tel,''))+length(ifnull(a.vch_handtel,''))"
                                                   "when length(ifnull(a.vch_tel,'')) then a.vch_tel when length(ifnull(a.vch_handtel,'')) then a.vch_handtel"
                                                   " when 0 then '' else a.vch_tel +','+ a.vch_handtel end) as vch_telno,"
                                                   " ifnull(c.num_amount,0.00) as num_amount,"
                                                   "  ifnull(c.num_point,0.00) as num_point,"
                                                   " c.dt_operdate as dt_operdate,"
                                                   " a.ch_state as ch_state"
                                                   " From t_m_member a, t_m_member_type b, t_m_curamount c"
                                                   " Where a.ch_typeno = b.ch_typeno and a.vch_memberno = c.vch_memberno"
                                                   " and a.ch_typeno like '%1%' and a.vch_memberno like '%2%'"
                                                   " and a.ch_state like '%3%' "
                                                   )))
                    );
    tab1info.insert(tr("会员菜品消费汇总"),
                    w_rpt_report::TABTYPE(QStringList()<<tr("品码")<<tr("菜品名称")<<tr("单位")<<tr("均价")<<tr("数量")<<tr("金额")<<tr("折扣额")<<tr("折扣率")<<tr("赠送额")<<tr("赠送率")<<tr("实际金额"),
                                          QString("select dishno as tabname0,"
                                                  "dishname as tabname1,"
                                                  "(select vch_unitname from cey_bt_unit where ch_unitno=dish_unitno) as tabname2,"
                                                  "(amount/if(num=0,1,num))as tabname3,"
                                                  "num as tabname4,"
                                                  "amount as tabname5,"
                                                  "discount as tabname6,"
                                                  "concat((100*(amount-discount)/if(amount=0,1,amount)), '%')as tabname7,"
                                                  "present as tabname8,"
                                                  "concat((100*(present)/if(amount=0,1,amount)), '%')as tabname9,"
                                                  "amount-discount-present as tabname10 from (%1)t  ORDER BY tabname%4"
                                                  )
                                          .arg(QString(
                                                   "SELECT a.ch_dishno as dishno,"
                                                   "c.vch_dishname as dishname,"
                                                   "c.ch_unitno as dish_unitno,"
                                                   " sum(a.num_num - a.num_back) as num,"
                                                   " sum((a.num_num - a.num_back) * (a.num_price + a.num_price_add)) as amount,"
                                                   "Round(sum((a.num_num - a.num_back) * (a.num_price + a.num_price_add) * (1 - a.int_discount * 0.01)),2) as discount,"
                                                   "Round(sum(case a.ch_presentflag when 'Y' then ((a.num_num - a.num_back) * (a.num_price + a.num_price_add) * a.int_discount * 0.01) else 0.00 end),2) as present,"
                                                   "c.ch_dish_typeno as typeno"
                                                   " FROM cey_u_orderdish a,cey_u_checkout_master b,cey_bt_dish c ,t_m_member_type t,t_m_member m"
                                                   "  WHERE"
                                                   " ( a.ch_payno = b.ch_payno ) and"
                                                   "  ( b.ch_state = 'Y' ) and"
                                                   " ( a.ch_dishno = c.ch_dishno ) and"
                                                   " ( a.num_num - a.num_back <> 0 )  and"
                                                   "  m.vch_memberno = b.vch_memberno"
                                                   "  and  m.ch_typeno = t.ch_typeno"
                                                   " and b.dt_operdate >= '%1'"
                                                   "  and b.dt_operdate <= '%2'"
                                                   "  and ( m.vch_memberno like '%3%' or m.vch_member like '%3%' )"
                                                   "  Group by a.ch_dishno,c.vch_dishname,c.ch_unitno,c.ch_dish_typeno"
                                                   )))
                    );

    tab1info.insert(tr("会员菜品消费明细"),
                    w_rpt_report::TABTYPE(QStringList()<<tr("结账单号")<<tr("会员编号")<<tr("会员类型")<<tr("时间")<<tr("菜品编码")<<tr("菜品名称")<<tr("单价")<<tr("数量")<<tr("金额")<<tr("折扣金额")<<tr("赠送金额")<<tr("实际金额"),
                                          QString(
                                              "select ch_payno as tabname0,"
                                              "memberno as tabname1,"
                                              "membertype as tabname2,"
                                              "dt_operdate as tabname3,"
                                              " dishno as tabname4,"
                                              "dishname as tabname5,"
                                              "price as tabname6,"
                                              "num_num as tabname7,"
                                              "amount as tabname8,"
                                              "discount as tabname9,"
                                              "present as tabname10,"
                                              "amount-discount-present as tabname11 from (%1)t  ORDER BY tabname%4"
                                              )
                                          .arg(QString(
                                                   "Select a.ch_payno as ch_payno,"
                                                   " a.dt_operdate as dt_operdate,"
                                                   "b.vch_memberno as memberno,"
                                                   " t.vch_typename as membertype,"
                                                   " a.ch_dishno as dishno,"
                                                   " a.num_price as price,"
                                                   "c.vch_dishname as dishname,"
                                                   " a.num_price + a.num_price_add as num_price,"
                                                   " a.num_num - a.num_back as num_num,"
                                                   " ((a.num_num - a.num_back) * (a.num_price + a.num_price_add)) as amount,"
                                                   "Round(((a.num_num - a.num_back) * (a.num_price + a.num_price_add) * (1 - a.int_discount * 0.01)),2) as discount,"
                                                   "Round((case a.ch_presentflag when 'Y' then ((a.num_num - a.num_back) * (a.num_price + a.num_price_add) * a.int_discount * 0.01) else 0.00 end),2) as present"
                                                   " FROM cey_u_orderdish a,cey_u_checkout_master b,cey_bt_dish c ,t_m_member_type t,t_m_member m "
                                                   " WHERE ( a.ch_payno = b.ch_payno ) and "
                                                   " ( b.ch_state = 'Y' ) and"
                                                   "  ( a.ch_dishno = c.ch_dishno ) and"
                                                   " ( a.num_num - a.num_back <> 0 )  and "
                                                   " m.vch_memberno = b.vch_memberno "
                                                   " and  m.ch_typeno = t.ch_typeno	"
                                                   " and b.dt_operdate >= '%1'"
                                                   "  and b.dt_operdate <= '%2'"
                                                   " and ( m.vch_memberno like '%3%' or m.vch_member like '%3%' )"
                                                   )))
                    );

    tab1info.insert(tr("会员卡量统计"),
                    w_rpt_report::TABTYPE(QStringList()<<tr("会员类型")<<tr("卡总量")<<tr("正常未发卡量")<<tr("正常已发卡量")<<tr("止付卡量")<<tr("挂失卡量")<<tr("作废卡量"),
                                          QString(
                                              "  select  typeno as tabname0,"
                                              " totalnum as tabname1,"
                                              " normalnum as tabname2,"
                                              " rmalnum as tabname3,"
                                              " stopnum as tabname4,"
                                              "  lossnum as tabname5,"
                                              " cancelnum as tabname6 from (%1)t  ORDER BY tabname%2"
                                              )
                                          .arg(QString(
                                                   " SELECT ch_typeno as typeno,"
                                                   " count(vch_memberno) as totalnum,"
                                                   " sum(case ch_state when '1'  then case when ch_cardflag = 'N' then 1 else 0 end else 0 end ) as normalnum,"
                                                   " sum(case ch_state when '1'  then case when ch_cardflag  = 'Y' then 1 else 0 end else 0 end) as rmalnum,"
                                                   " sum(case ch_state when '2' then 1 else 0 end) as stopnum,"
                                                   " sum(case ch_state when '3' then 1 else 0 end) as lossnum,"
                                                   " sum(case ch_state when '4' then 1 else 0 end) as  cancelnum"
                                                   " FROM t_m_member"
                                                   " where ch_typeno like '%1%'   "
                                                   " GROUP by ch_typeno"
                                                   )))
                    );
    tab1info.insert(tr("会员保本金额"),
                    w_rpt_report::TABTYPE(QStringList()<<tr("会员编号")<<tr("会员姓名")<<tr("会员类型")<<tr("会员种类")<<tr("联系电话")<<tr("状态")<<tr("持卡")<<tr("保本金额")<<tr("发卡时间"),
                                          QString(
                                              "select memberno as tabname0,"
                                              "member as tabname1,"
                                              "typename as tabname2,"
                                              "vch_att as tabname3,"
                                              "vch_telno as tabname4,"
                                              " (case ch_state when '1' then '"+tr("正常")+"'when '2' then '"+tr("冻结")+"'when '3' then '"+tr("挂失")+"'when '4' then '"+tr("作废")+"'when '5' then '"+tr("已退卡")+"' end) as tabname5,"
                                              "  (case ch_cardflag when 'Y' then '"+tr("是")+"' else '"+tr("否")+"' end) as tabname6,"
                                              " int_basedata as tabname7,"
                                              " dt_operdate as tabname8 from (%1)t  ORDER BY tabname%3"
                                              )
                                          .arg(QString(
                                                   " Select	a.vch_memberno as memberno,"
                                                   " a.vch_member as member,"
                                                   " b.vch_typename as typename,"
                                                   " (case b.ch_attribute when '2' then '"+tr("充值卡")+"' when '3' then '"+tr("挂帐卡")+"' else '' end) as vch_att,"
                                                   " (case length(ifnull(a.vch_tel,''))+length(ifnull(a.vch_handtel,''))"
                                                   " when length(ifnull(a.vch_tel,'')) then a.vch_tel when length(ifnull(a.vch_handtel,'')) then a.vch_handtel"
                                                   " when 0 then '' else a.vch_tel +','+ a.vch_handtel end) as vch_telno,"
                                                   "  a.ch_state as ch_state,"
                                                   "   a.ch_cardflag as ch_cardflag,"
                                                   "   a.int_basedata as int_basedata,"
                                                   "  a.dt_sendtime as dt_operdate"
                                                   "  From		t_m_member a, t_m_member_type b"
                                                   " Where		a.ch_typeno = b.ch_typeno and b.ch_attribute <> '1' and"
                                                   " a.ch_typeno like '%1%' and a.ch_state like '%2%' "
                                                   " union   Select	a.vch_memberno as vch_memberno,"
                                                   "   a.vch_member as vch_member,"
                                                   " b.vch_typename as vch_typename,"
                                                   " '"+tr("充值卡")+"' as vch_att,"
                                                   " '' as vch_telno,"
                                                   " '5'as ch_state,"
                                                   " 'N'as ch_cardflag,"
                                                   " num_bace as int_basedata,"
                                                   "  dt_send_date as dt_operdate"
                                                   "  From		t_m_member_log a,  t_m_member_type b"
                                                   "  Where		a.ch_typeno = b.ch_typeno  and"
                                                   " a.vch_matter = '"+tr("退卡")+"' and"
                                                   " a.ch_typeno like '%1%'"
                                                   )))
                    );
}

w_m_member_report::~w_m_member_report()
{
    delete ui;
}

void w_m_member_report::toselect()
{
    QString sumsql;
    QString ls_begin;
    QString ls_end;
    ls_begin=ui->dateTimeEdit->dateTime().toString("yyyy-MM-dd hh:mm:ss");
    ls_end=ui->dateTimeEdit_2->dateTime().toString("yyyy-MM-dd hh:mm:ss");
    switch(ui->tabWidget->currentIndex()){
    case 0://会员充值付款汇总报表
        querymodel->setQuery(tab1info[ui->tabWidget->tabText(ui->tabWidget->currentIndex())].sql
                .arg(ls_begin)
                .arg(ls_end)
                .arg(ui->widget_2->gettext())//获取过滤信息
                .arg(ui->checkBox->isChecked()?"true":"false")
                .arg(w_rpt_report::querymodelordersort(ui->tableView->tableView))
                .arg(ui->comboBox_13->comusrdata())
                );
        qDebug() << querymodel->query().lastQuery();
        //更新表头
        backheader::modelSetHeader(querymodel, tab1info[ui->tabWidget->tabText(ui->tabWidget->currentIndex())].headers);
        //制表人 和 制表时间
        ui->label_operator->setText(n_func::gs_opername);
        ui->label_operattime->setText(n_func::f_get_sysdatetime().toString("yyyy-MM-dd hh:mm:ss"));
        //更新合计
        sumsql=querymodel->query().lastQuery().trimmed();
        if(sumsql.endsWith("DESC")||sumsql.endsWith("ASC")){
            //5 6 7  9 10
            QSqlQuery query;
            query.exec(QString("select sum(tabname4),sum(tabname5),sum(tabname6),sum(tabname7),sum(tabname8)  from (%1)t1 ").arg(sumsql.left(sumsql.lastIndexOf("ORDER BY"))));
            query.next();

            ui->tableView->updateSum(4,query.record().value(0).toString());
            ui->tableView->updateSum(5,query.record().value(1).toString());
            ui->tableView->updateSum(6,query.record().value(2).toString());
            ui->tableView->updateSum(7,query.record().value(3).toString());
            ui->tableView->updateSum(8,query.record().value(4).toString());

        }
        break;

    case 1://会员余额积分情况
        querymodel_1->setQuery(tab1info[ui->tabWidget->tabText(ui->tabWidget->currentIndex())].sql
                .arg(ui->comboBox_membertype->currentText())//会员类型
                .arg(ui->widget_2->gettext())//获取过滤信息
                .arg(ui->comboBox_13->comusrdata())
                .arg(w_rpt_report::querymodelordersort(ui->tableView_2->tableView))
                );
        //更新表头
        backheader::modelSetHeader(querymodel_1, tab1info[ui->tabWidget->tabText(ui->tabWidget->currentIndex())].headers);
        //制表人 和 制表时间
        ui->label_operator->setText(n_func::gs_opername);
        ui->label_operattime->setText(n_func::f_get_sysdatetime().toString("yyyy-MM-dd hh:mm:ss"));
        //更新合计
        sumsql=querymodel_1->query().lastQuery().trimmed();
        if(sumsql.endsWith("DESC")||sumsql.endsWith("ASC")){
            //5 6 7  9 10
            QSqlQuery query;
            query.exec(QString("select sum(tabname4),sum(tabname5) from (%1)t1 ").arg(sumsql.left(sumsql.lastIndexOf("ORDER BY"))));
            query.next();
            ui->tableView_2->updateSum(4,query.record().value(0).toString());
            ui->tableView_2->updateSum(5,query.record().value(1).toString());
        }
        break;

    case 2://会员菜品消费汇总
    {
        querymodel_2->setQuery(tab1info[ui->tabWidget->tabText(ui->tabWidget->currentIndex())].sql
                .arg(ls_begin)
                .arg(ls_end)
                .arg(ui->widget_2->gettext())//获取过滤信息
                .arg(w_rpt_report::querymodelordersort(ui->tableView_3->tableView))
                );
        //更新表头
        backheader::modelSetHeader(querymodel_2, tab1info[ui->tabWidget->tabText(ui->tabWidget->currentIndex())].headers);
        //制表人 和 制表时间
        ui->label_operator->setText(n_func::gs_opername);
        ui->label_operattime->setText(n_func::f_get_sysdatetime().toString("yyyy-MM-dd hh:mm:ss"));
        qDebug() << querymodel_2->query().lastQuery();
        qDebug() << querymodel_2->lastError().text();
        //更新合计
        sumsql=querymodel_2->query().lastQuery().trimmed();
        if(sumsql.endsWith("DESC")||sumsql.endsWith("ASC")){
            //5 6 7  9 10
            QSqlQuery query;
            query.exec(QString("select sum(tabname4),sum(tabname5),sum(tabname6),sum(tabname8),sum(tabname10) from (%1)t1 ").arg(sumsql.left(sumsql.lastIndexOf("ORDER BY"))));
            query.next();
            ui->tableView_3->updateSum(4,query.record().value(0).toString());
            ui->tableView_3->updateSum(5,query.record().value(1).toString());
            ui->tableView_3->updateSum(6,query.record().value(2).toString());
            ui->tableView_3->updateSum(8,query.record().value(3).toString());
            ui->tableView_3->updateSum(10,query.record().value(4).toString());
        }
    }
        break;
    case 3://会员菜品消费明细
    {
        querymodel_3->setQuery(tab1info[ui->tabWidget->tabText(ui->tabWidget->currentIndex())].sql
                .arg(ls_begin)
                .arg(ls_end)
                .arg(ui->widget_2->gettext())//获取过滤信息
                .arg(w_rpt_report::querymodelordersort(ui->tableView_4->tableView))
                );
        //更新表头
        backheader::modelSetHeader(querymodel_3, tab1info[ui->tabWidget->tabText(ui->tabWidget->currentIndex())].headers);
        //制表人 和 制表时间
        ui->label_operator->setText(n_func::gs_opername);
        ui->label_operattime->setText(n_func::f_get_sysdatetime().toString("yyyy-MM-dd hh:mm:ss"));
        qDebug() << querymodel_3->query().lastQuery();
        qDebug() << querymodel_3->lastError().text();
        //更新合计
        sumsql=querymodel_3->query().lastQuery().trimmed();
        if(sumsql.endsWith("DESC")||sumsql.endsWith("ASC")){
            //5 6 7  9 10
            QSqlQuery query;
            query.exec(QString("select sum(tabname8),sum(tabname11) from (%1)t1 ").arg(sumsql.left(sumsql.lastIndexOf("ORDER BY"))));
            query.next();
            ui->tableView_4->updateSum(8,query.record().value(0).toString());
            ui->tableView_4->updateSum(11,query.record().value(1).toString());
        }
    }
        break;
    case 4://会员卡量统计
    {
        querymodel_4->setQuery(tab1info[ui->tabWidget->tabText(ui->tabWidget->currentIndex())].sql
                .arg(ui->comboBox_membertype->currentText())//获取过滤信息
                .arg(w_rpt_report::querymodelordersort(ui->tableView_5->tableView))
                );
        //更新表头
        backheader::modelSetHeader(querymodel_4, tab1info[ui->tabWidget->tabText(ui->tabWidget->currentIndex())].headers);
        //制表人 和 制表时间
        ui->label_operator->setText(n_func::gs_opername);
        ui->label_operattime->setText(n_func::f_get_sysdatetime().toString("yyyy-MM-dd hh:mm:ss"));
        qDebug() << querymodel_4->query().lastQuery();
        qDebug() << querymodel_4->lastError().text();
        //更新合计
        sumsql=querymodel_4->query().lastQuery().trimmed();
        if(sumsql.endsWith("DESC")||sumsql.endsWith("ASC")){
            //5 6 7  9 10
            QSqlQuery query;
            query.exec(QString("select sum(tabname1),sum(tabname2),sum(tabname3),sum(tabname4),sum(tabname5),sum(tabname6)  from (%1)t1 ").arg(sumsql.left(sumsql.lastIndexOf("ORDER BY"))));
            query.next();
            ui->tableView_5->updateSum(1,query.record().value(0).toString());
            ui->tableView_5->updateSum(2,query.record().value(1).toString());
            ui->tableView_5->updateSum(3,query.record().value(2).toString());
            ui->tableView_5->updateSum(4,query.record().value(3).toString());
            ui->tableView_5->updateSum(5,query.record().value(4).toString());
            ui->tableView_5->updateSum(6,query.record().value(5).toString());
        }
    }
        break;
    case 5://会员保本金额
    {
        querymodel_5->setQuery(tab1info[ui->tabWidget->tabText(ui->tabWidget->currentIndex())].sql
                .arg(ui->comboBox_membertype->currentText())//获取过滤信息
                .arg(ui->comboBox_13->currentText())
                .arg(w_rpt_report::querymodelordersort(ui->tableView_6->tableView))
                );
        //更新表头
        backheader::modelSetHeader(querymodel_5, tab1info[ui->tabWidget->tabText(ui->tabWidget->currentIndex())].headers);
        //制表人 和 制表时间
        ui->label_operator->setText(n_func::gs_opername);
        ui->label_operattime->setText(n_func::f_get_sysdatetime().toString("yyyy-MM-dd hh:mm:ss"));
        qDebug() << querymodel_5->query().lastQuery();
        qDebug() << querymodel_5->lastError().text();
        //更新合计
        sumsql=querymodel_5->query().lastQuery().trimmed();
        if(sumsql.endsWith("DESC")||sumsql.endsWith("ASC")){
            //5 6 7  9 10
            QSqlQuery query;
            query.exec(QString("select sum(tabname7) from (%1)t1 ").arg(sumsql.left(sumsql.lastIndexOf("ORDER BY"))));
            query.next();
            ui->tableView_6->updateSum(7,query.record().value(0).toString());
        }
    }
        break;
    }
}

void w_m_member_report::toexit()
{
    this->reject();
}

void w_m_member_report::toupdate_tabw_list(int index)
{
    ui->widget_filter0->setVisible(false);//开始时间
    ui->widget_filter1->setVisible(false);//结束时间
    ui->widget_filter2->setVisible(false);//会员编号
    ui->widget_filter3->setVisible(false);//会员类型
    ui->widget_filter4->setVisible(false);//会员状态
    ui->checkBox->setVisible(false);//是否显示0充值
    switch(index){
    case 0://会员充值付款汇总报表
        ui->widget_filter0->setVisible(true);//开始时间
        ui->widget_filter1->setVisible(true);//结束时间
        ui->widget_filter2->setVisible(true);//会员编号
        ui->widget_filter4->setVisible(true);//会员状态
        ui->checkBox->setVisible(true);//是否显示0充值
        break;
    case 1://会员余额积分情况
        ui->widget_filter3->setVisible(true);//会员类型
        ui->widget_filter2->setVisible(true);//会员编号
        ui->widget_filter4->setVisible(true);//会员状态
        break;
    case 2://会员菜品消费汇总
        ui->widget_filter0->setVisible(true);//开始时间
        ui->widget_filter1->setVisible(true);//结束时间
        ui->widget_filter2->setVisible(true);//会员编号
        break;
    case 3: //会员菜品消费明细
        ui->widget_filter0->setVisible(true);//开始时间
        ui->widget_filter1->setVisible(true);//结束时间
        ui->widget_filter2->setVisible(true);//会员编号
        break;
    case 4: //会员卡量统计
        ui->widget_filter3->setVisible(true);//会员类型
        break;
    case 5://会员保本金额
        ui->widget_filter3->setVisible(true);//会员类型
        ui->widget_filter4->setVisible(true);//会员状态
        break;
    }
}
