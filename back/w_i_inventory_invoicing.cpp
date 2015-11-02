#include "w_i_inventory_invoicing.h"
//进销存汇总报表
#include"ui_w_i_inventory_Invoicing_dialog.h"
#include "backheader.h"
#include <QDebug>
#include <QSqlQueryModel>
#include <QStandardItemModel>
#include "n_func.h"
w_i_inventory_Invoicing::w_i_inventory_Invoicing(QWidget *parent) :
    QDialog(parent),
    ui(new Ui_w_i_inventory_Invoicing_Dialog)
{
    ui->setupUi(this);
    ui->frame->setFixedWidth(w_rpt_report::frame_left_fixed_width);

    //时间改成年月日
    ui->dateTimeEdit->setDisplayFormat("yyyy-MM-dd");
    ui->dateTimeEdit_2->setDisplayFormat("yyyy-MM-dd");
    ui->dateTimeEdit->setDate(n_func::f_get_sysdatetime().date());
    ui->dateTimeEdit_2->setDate(n_func::f_get_sysdatetime().date());

    //菜品大类
    ui->comboBox_series->addItems(w_rpt_report::querymodelsetfilters("select * from cey_bt_dish_type"));

    //model
    standmodel=new QStandardItemModel;
    ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::SelectedClicked);ui->tableView->setModel(standmodel);
    ui->tableView->tableView->setSortingEnabled(true);
    //! toolbar
    ui->widget->setup(QStringList() << tr("查询") << tr("退出"), Qt::AlignTop);
    connect(ui->widget->index_widget(tr("查询")),SIGNAL(clicked()),this,SLOT(toselect()));
    connect(ui->widget->index_widget(tr("退出")),SIGNAL(clicked()),this,SLOT(toexit()));
    connect(ui->tableView->tableView->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(toselect()));

}

void w_i_inventory_Invoicing::toselect()
{
    //ue_query (none) returns (none)
    QString ls_deptno;
    QString ls_materialno;
    QString ls_typeno;
    QString ls_seriesno;
    QString ls_filter;
    QString ls_sql;
    QString ls_where;
    QString ls_err;
    QString ls_syntax;
    QString ls_dept;
    double ldec_initnum;
    double ldec_innum;
    double ldec_inamount;
    double ldec_outnum;
    double ldec_outamount;
    double ldec_alteramount;
    double ldec_endnum;
    double ldec_initprice;
    double ldec_endprice;
    qlonglong ll_count;
    qlonglong ll_row;
    qlonglong ll_irow;
    qlonglong ll_index;
    qlonglong ll_flowid;
    QDateTime ldt_begin;
    QDateTime ldt_end;
    //    datastore lds;
    double ldec_inchange;
    double ldec_outchange;
    double ldec_outchange_1;

    QSqlQuery query_2;
    QSqlQuery query;
    standmodel->removeRows(0, standmodel->rowCount());
    ls_sql=
            " Select bb.ch_dishno ch_materialno, bb.vch_dishname vch_materialname, bb.ch_unitno ch_material_unitno  "
            " from cey_bt_dish bb where exists (select * from cey_st_material_num aa where aa.ch_materialno = bb.ch_dishno)"
            " and bb.ch_sub_typeno like '%1%'  and bb.ch_dishno like '%2%'  order by ch_dishno";
    query.exec(ls_sql
               .arg(backheader::getinterdata(ui->comboBox_series->currentText()))
               .arg(ui->selectgoods->gettext())
               );
    qDebug() << query.lastQuery();
    ldt_begin =QDateTime(ui->dateTimeEdit->date());//= datetime(date(uo_begin.of_getdatetime()),time(0))
    ldt_end = QDateTime(ui->dateTimeEdit_2->date(), QTime(23,59,59));//datetime(date(uo_end.of_getdatetime()),time("23:59:59:950"))
    while(query.next()){//query.next
        ls_materialno=query.record().value("ch_materialno").toString();
        ldec_initprice = 0;
        ldec_initnum = 0;
        ll_flowid = 0;

        query_2.prepare("select ifnull(max(int_flowId), 0) from cey_st_material_flow    where dt_operdate <= :ldt_begin and ch_materialno =:ls_materialno ");
        query_2.bindValue(":ldt_begin", ldt_begin);
        query_2.bindValue(":ls_materialno",ls_materialno );
        query_2.exec();
        query_2.next();
        ll_flowid=query_2.record().value(0).toLongLong();

        if(ll_flowid==0){
            ldec_initprice = 0;
        } else {
            query_2.prepare("select remain_price , remain_qty  from cey_st_material_flow   where int_flowId =:ll_flowid");
            query_2.bindValue(":ll_flowid", ll_flowid);
            query_2.exec();
            query_2.next();
            ldec_initprice=query_2.record().value(0).toDouble();
            ldec_initnum=query_2.record().value(1).toDouble();
        }
        ll_flowid = 0;
        ldec_endnum = 0;
        ldec_endprice = 0;

        query_2.prepare("select ifnull(max(int_flowId), 0) from cey_st_material_flow   where dt_operdate <= :ldt_end and ch_materialno =:ls_materialno");
        query_2.bindValue(":ldt_end", ldt_end);
        query_2.bindValue(":ls_materialno", ls_materialno);
        query_2.exec();
        query_2.next();
        ll_flowid=query_2.record().value(0).toLongLong();

        if(ll_flowid==0){
            ldec_endnum = 0;
            ldec_endprice = 0;
        }else {
            query_2.prepare("select remain_price , remain_qty  from cey_st_material_flow   where int_flowId =:ll_flowid");
            query_2.bindValue(":ll_flowid", ll_flowid);
            query_2.exec();
            query_2.next();
            ldec_endprice=query_2.record().value(0).toDouble();
            ldec_endnum=query_2.record().value(1).toDouble();
        }

        ldec_innum = 0;
        ldec_inamount = 0;

        query_2.prepare("Select ifnull(sum(num_num), 0), ifnull(sum(num_num *sheet_price), 0) from cey_st_material_flow  where dt_operdate >= :ldt_begin and dt_operdate <= :ldt_end and ch_materialno =:ls_materialno and sheettype ='RK' ");
        query_2.bindValue(":ldt_begin", ldt_begin);
        query_2.bindValue(":ldt_end", ldt_end);
        query_2.bindValue(":ls_materialno", ls_materialno);
        query_2.exec();
        query_2.next();
        ldec_innum=query_2.record().value(0).toDouble();
        ldec_inamount=query_2.record().value(1).toDouble();

        query_2.prepare("Select :ldec_innum + ifnull(sum(num_num), 0), :ldec_inamount + ifnull(sum(num_num *cost_price), 0)  from cey_st_material_flow   where dt_operdate >= :ldt_begin and dt_operdate <= :ldt_end and ch_materialno =:ls_materialno and(sheettype ='PD' and int_flag =1 )");
        query_2.bindValue(":ldec_innum", ldec_innum);
        query_2.bindValue(":ldec_inamount", ldec_inamount);
        query_2.bindValue(":ldt_begin", ldt_begin);
        query_2.bindValue(":ldt_end", ldt_end);
        query_2.bindValue(":ls_materialno", ls_materialno);
        query_2.exec();
        query_2.next();
        ldec_innum=query_2.record().value(0).toDouble();
        ldec_inamount=query_2.record().value(1).toDouble();

        ldec_outnum = 0;
        ldec_outamount = 0;

        //        query_2.prepare("Select ifnull(sum(num_num), 0), ifnull(sum(num_num *sheet_price), 0)from cey_st_material_flow  where dt_operdate >= :ldt_begin and dt_operdate <= :ldt_end and ch_materialno =:ls_materialno and(sheettype ='CK' or sheettype ='QT' )");
        //        query_2.bindValue(":ldt_begin", ldt_begin);
        //        query_2.bindValue(":ldt_end", ldt_end);
        //        query_2.bindValue(":ls_materialno", ls_materialno);
        //        query_2.exec();
        //        query_2.next();
        //        ldec_outnum=query_2.record().value(0).toDouble();
        //        ldec_outamount=query_2.record().value(1).toDouble();

        query_2.prepare("Select ifnull(sum(num_num), 0), ifnull(sum(num_num *cost_price), 0)from cey_st_material_flow where dt_operdate >= :ldt_begin and dt_operdate <= :ldt_end and ch_materialno =:ls_materialno and(sheettype ='QT' )");
        query_2.bindValue(":ldt_begin", ldt_begin);
        query_2.bindValue(":ldt_end", ldt_end);
        query_2.bindValue(":ls_materialno", ls_materialno);
        query_2.exec();
        query_2.next();
        ldec_outnum=query_2.record().value(0).toDouble();
        ldec_outamount=query_2.record().value(1).toDouble();

        query_2.prepare("Select :ldec_outnum + ifnull(sum(num_num), 0), :ldec_outamount + ifnull(sum(num_num *sheet_price), 0) from cey_st_material_flow  where dt_operdate >= :ldt_begin and dt_operdate <= :ldt_end and ch_materialno =:ls_materialno and sheettype ='TH' ");
        query_2.bindValue(":ldec_outnum", ldec_outnum);
        query_2.bindValue(":ldec_outamount", ldec_outamount);
        query_2.bindValue(":ldt_begin", ldt_begin);
        query_2.bindValue(":ldt_end", ldt_end);
        query_2.bindValue(":ls_materialno", ls_materialno);
        query_2.exec();
        query_2.next();
        ldec_outnum=query_2.record().value(0).toDouble();
        ldec_outamount=query_2.record().value(1).toDouble();

        query_2.prepare("Select :ldec_outnum + ifnull(sum(num_num), 0), :ldec_outamount + ifnull(sum(num_num *cost_price), 0) from cey_st_material_flow  where dt_operdate >= :ldt_begin and dt_operdate <= :ldt_end and ch_materialno =:ls_materialno and(sheettype ='PD' and int_flag =- 1)");
        query_2.bindValue(":ldec_outnum", ldec_outnum);
        query_2.bindValue(":ldec_outamount", ldec_outamount);
        query_2.bindValue(":ldt_begin", ldt_begin);
        query_2.bindValue(":ldt_end", ldt_end);
        query_2.bindValue(":ls_materialno", ls_materialno);
        query_2.exec();
        query_2.next();
        ldec_outnum=query_2.record().value(0).toDouble();
        ldec_outamount=query_2.record().value(1).toDouble();

        ldec_alteramount = ldec_initnum * ldec_initprice + ldec_inamount - ldec_outamount - ldec_endnum * ldec_endprice;
        if((qAbs(ldec_initnum) + qAbs(ldec_innum) + qAbs(ldec_innum) + qAbs(ldec_outnum) + qAbs(ldec_endnum)) == 0){
            if(!ui->checkBox->isChecked()){
                continue;
            }
        }
        QList<QStandardItem *> items;
        items << new QStandardItem(ls_materialno);
        items << new QStandardItem(query.record().value("vch_materialname").toString());
        items << new QStandardItem(query.record().value("ch_material_unitno").toString());

        items << new QStandardItem(QString::number(ldec_initnum));
        items << new QStandardItem(QString::number(ldec_initprice));//4
        items << new QStandardItem(QString().sprintf("%.02f", ldec_initnum*ldec_initprice));


        items << new QStandardItem(QString::number(ldec_innum));
        items << new QStandardItem(QString::number(ldec_inamount));

        items << new QStandardItem(QString::number(ldec_outnum));
        items << new QStandardItem(QString::number(ldec_outamount));

        items << new QStandardItem(QString::number(ldec_alteramount));

        items << new QStandardItem(QString::number(ldec_endprice));
        items << new QStandardItem(QString::number(ldec_endnum));
        items << new QStandardItem(QString().sprintf("%.02f", ldec_endprice*ldec_endnum));


        standmodel->appendRow(items);
    }//query.next
    //model 表头
    standmodel->setHorizontalHeaderLabels(QStringList() << tr("编码") << tr("商品名称") << tr("库存单位") << tr("期初价")<< tr("期初量") << tr("期初额")   << tr("入库量") << tr("入库额") << tr("出库量")<< tr("出库额")  << tr("成本调价") << tr("期末价") << tr("期末量") << tr("期末额"));

    //总计
    ui->tableView->updateSum(4);
    ui->tableView->updateSum(5);
    ui->tableView->updateSum(6);
    ui->tableView->updateSum(7);
    ui->tableView->updateSum(8);
    ui->tableView->updateSum(9);
    ui->tableView->updateSum(10);
    ui->tableView->updateSum(12);
    ui->tableView->updateSum(13);
}

void w_i_inventory_Invoicing::toexit()
{
    this->reject();
}
