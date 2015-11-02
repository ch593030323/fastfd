#include "w_i_inventory_query.h"
//库存查询
#include"ui_w_i_inventory_query_dialog.h"
#include "backheader.h"
#include <QDebug>
#include <QSqlQueryModel>
#include "n_func.h"
#include "lds_dialog_input.h"
#include <QSqlError>
#include <QSqlRecord>
#include "lds_messagebox.h"

w_i_inventory_query::w_i_inventory_query(QWidget *parent) :
    QDialog(parent),ui(new Ui_w_i_inventory_query_Dialog)
{
    ui->setupUi(this);
    ui->frame->setFixedWidth(w_rpt_report::frame_left_fixed_width);
    //菜品大类
    ui->comboBox_series->addItems(w_rpt_report::querymodelsetfilters("select * from cey_bt_dish_type"));

    querymodel=new QSqlQueryModel;
    ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::SelectedClicked);ui->tableView->setModel(querymodel);
    ui->tableView->tableView->setSortingEnabled(true);
    //! toolbar
    ui->widget->setup(QStringList() << tr("查询") /*<< tr("修改库存")*/ << tr("退出"), Qt::AlignTop);
    connect(ui->widget->index_widget(tr("查询")),SIGNAL(clicked()),this,SLOT(toselect()));
//    connect(ui->widget->index_widget(tr("修改库存")),SIGNAL(clicked()),this,SLOT(tochange()));
    connect(ui->widget->index_widget(tr("退出")),SIGNAL(clicked()),this,SLOT(toexit()));
    connect(ui->tableView->tableView->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(toselect()));

}

void w_i_inventory_query::tab1sqlinit()
{

}

void w_i_inventory_query::toselect()
{
    //查询时间
    ui->label_querytime->setText(n_func::f_get_sysdatetime().toString("yyyy-MM-dd hh:mm:ss"));
    //数据库sql
    querymodel->setQuery(
                QString(
                    "select ch_dishno as tabname0,"

                    " vch_dishname as tabname1,"

                    " (select vch_unitname"

                    " from cey_bt_unit"

                    " where cey_bt_unit.ch_unitno = t.ch_unitno) as tabname2,"

                    " num_store as tabname3,"

                    " num_cost as tabname4,"

                    " Round(num_store * num_cost, 2) as tabname5,"

                    " (select vch_dish_typename"

                    " from cey_bt_dish_type"

                    " where cey_bt_dish_type.ch_dish_typeno = t.ch_dish_typeno) as tabname6,"

                    " (select vch_sub_typename"

                    " from cey_bt_dish_subtype"

                    " where cey_bt_dish_subtype.ch_sub_typeno = t.ch_sub_typeno) as tabname7"

                    " from (SELECT a.ch_dishno,"

                    " a.vch_dishname,"

                    " a.int_unit_rate,"

                    " a.ch_unitno,"

                    " a.vch_pur_unitno,"

                    " a.ch_dish_typeno,"

                    " a.ch_sub_typeno,"

                    " IFNULL(b.num_num, 0) as num_store,"

                    " IFNULL(b.cost_price, 0) as num_cost"

                    " FROM cey_bt_dish a"

                    " LEFT OUTER JOIN cey_st_material_num b"

                    " ON a.ch_dishno = b.ch_materialno"

                    " where IFNULL(a.item_flag, 'N') = 'Y'"

                    " and (a.ch_sub_typeNo like '%1%' and a.ch_dishno like '%2%')) t"

                    " ORDER BY tabname%3"
                    )
                .arg(backheader::getinterdata(ui->comboBox_series->currentText()))
                .arg(ui->selectgoods->gettext())
                .arg(w_rpt_report::querymodelordersort(ui->tableView->tableView))
                );

    qDebug() <<querymodel->query().lastQuery();
    //更新表头
    backheader::modelSetHeader(querymodel, QStringList() << tr("编码")<< tr("商品名称")<< tr("单位")<< tr("库存数量")<< tr("库存单价")<< tr("库存金额")<< tr("大类")<< tr("小类"));

    //更新tabwidget text
    ui->tabWidget->setTabText(0, this->windowTitle());
    //更新合计
    QString sumsql=querymodel->query().lastQuery().trimmed();
    if(sumsql.endsWith("DESC")||sumsql.endsWith("ASC")){
        //5 6 7  9 10
        QSqlQuery query;
        query.exec(QString("select sum(tabname3),sum(tabname5)  from (%1)t1 ").arg(sumsql.left(sumsql.lastIndexOf("ORDER BY"))));
        query.next();

        ui->tableView->updateSum(3,query.record().value(0).toString());
        ui->tableView->updateSum(5,query.record().value(1).toString());
    }
}

void w_i_inventory_query::toexit()
{
    this->reject();
}

void w_i_inventory_query::tochange()
{
    if(ui->tableView->tableView->currentIndex().row()>=0){
        QString ch_dishno=querymodel->record(ui->tableView->tableView->currentIndex().row()).value("tabname0").toString();
        QString num=querymodel->record(ui->tableView->tableView->currentIndex().row()).value("tabname3").toString();
        lds_dialog_input inputdialog;
        QString sqlerr;
        inputdialog.ui->label->setText(tr("当前库存:"));
        inputdialog.ui->lineEdit->setText(num);
        inputdialog.ui->lineEdit->setEnabled(false);
        inputdialog.set_second_hidden(false);
        inputdialog.ui->label_2->setText(tr("修改后:"));
        if(QDialog::Accepted==backheader::static_dialogexec(inputdialog, tr("修改库存"))){

            QSqlQuery query;
            //cey_st_sheet_master
            query.exec(QString("update cey_st_material_num set num_num='%1' where ch_materialno='%2' ")
                       .arg(inputdialog.ui->lineEdit_2->text())
                       .arg(ch_dishno));
            qDebug() << query.lastQuery();
            if((sqlerr=query.lastError().text().trimmed()).isEmpty()){
                toselect();
                lds_messagebox::warning(this, tr("库存修改"), tr("修改成功"));
            } else {
                lds_messagebox::warning(this, tr("库存修改"), tr("数据库错误")+"\n"+sqlerr);
            }
        }
    }
}
//BEGIN
//    DECLARE m_de int default 0;
//    DECLARE m_done int default 0;
//    DECLARE sheetNo  varchar(12);
//    DECLARE sheetType varchar(2);
//    DECLARE operation varchar(3);
//    DECLARE dtaudit datetime;
//    DECLARE materialno varchar(7);
//    DECLARE numnum numeric(16,4);
//    DECLARE numpresent numeric(15,4);
//    DECLARE numprice numeric(16,4);
//    DECLARE costprice  numeric(16,4);
//    DECLARE unitrate numeric(16,2);
//    DECLARE intflowId integer;
//    DECLARE initqty   numeric(16,4);
//    DECLARE  initprice numeric(16,4);
//    DECLARE remainqty numeric(16,4);
//    DECLARE remainprice numeric(16,4);
//    DECLARE num_amount numeric(16,2);
//    DECLARE cur_rows cursor for
//      select b.ch_materialno, b.num_wrapping, b.num_present,  b.num_price, ifnull(c.int_unit_rate,1)
//       from  cey_st_sheet_detail b, cey_bt_dish c
//         where NEW.ch_sheetNo = b.ch_sheetno and b.ch_materialno = c.ch_dishno;

//   DECLARE continue handler for not found set m_done = 1;

//   open cur_rows;

// WHILE (m_done = 0) DO

//    fetch cur_rows into  materialno, numnum, numpresent, numprice, unitrate;

//      IF(NEW.ch_state = 'Y')THEN

//        set sheetNo = NEW.ch_sheetNo ;
//        set sheetType = NEW.ch_sheetType ;
//        set operation = NEW.ch_operation ;
//        set dtaudit = NEW.dt_audit ;

//        set intflowId = (select ifnull(max(int_flowId),0) from cey_st_material_flow  where ch_materialno = materialno);

//        IF (intflowId = 0) THEN
//          set initqty =  0 ;
//        ELSE
//          set initqty = (select remain_qty from cey_st_material_flow where int_flowId = intflowId);
//        END IF;

//        IF (intflowId = 0) THEN
//          set initprice =  0 ;
//        ELSE
//          set initprice = (select remain_price from cey_st_material_flow where int_flowId = intflowId);
//        END IF;

//        IF (sheetType = 'RK')  THEN

//             set remainqty = initqty + numnum;

//             IF (remainqty = 0) THEN
//                set remainprice = 0;
//             ELSE
//                set remainprice = (abs(((numnum / unitrate) * numprice  + initqty * initprice) / remainqty));
//             END IF;

//             insert cey_st_material_flow (ch_materialno, dt_operdate, sheettype, int_flag,
//                       num_num, sheet_price, cost_price, chg_amount, remain_qty, remain_price, ch_sheetno)
//               values (materialno, now(), sheetType, 1, numnum, numprice / unitrate, remainqty, 0, remainqty, remainprice, sheetNo);

//             IF (numpresent > 0) THEN

//                  set initqty = remainqty;
//                  set initprice = remainprice;
//                  set remainqty = initqty + numpresent;

//                  IF (remainqty = 0) THEN
//                     set remainprice = 0;
//                  ELSEIF (initqty < 0 and remainqty > 0) THEN
//                         set remainprice = numprice / unitrate;
//                  ELSE
//                        set remainprice = abs((initqty * initprice) / remainqty);
//                  END IF;

//                  insert cey_st_material_flow (ch_materialno, dt_operdate, sheettype, int_flag,
//                         num_num, sheet_price, cost_price, chg_amount, remain_qty, remain_price, ch_sheetno)
//                  values (materialno, now(), sheetType, 1, numpresent, 0, remainprice, 0, remainqty, remainprice, sheetNo);
//             END IF;


//                 IF exists ( Select * from cey_sys_parameter where vch_parameter = 'sys_stock_price' and vch_value = '1') THEN
//                update cey_bt_dish set num_sale_price = numprice where ch_dishno = materialno;
//             END IF;

//             IF exists (select 1 from cey_st_material_num where ch_materialno = materialno) THEN
//              update cey_st_material_num set num_num = remainqty, cost_price = remainprice
//               where ch_materialno = materialno;
//             ELSE
//               insert into cey_st_material_num (ch_materialno, num_num, cost_price)
//                 values (materialno, remainqty, remainprice);
//             END IF;

//             fetch cur_rows into  materialno, numnum, numpresent, numprice, unitrate;

//        ELSEIF (sheetType = 'TH')  THEN
//             set remainqty = initqty - numnum;
//             IF (remainqty = 0) THEN
//                set remainprice = 0;
//             ELSE
//                set remainprice = (abs((initqty * initprice - numnum / unitrate * numprice   ) / remainqty));
//             END IF;

//             insert cey_st_material_flow (ch_materialno, dt_operdate, sheettype, int_flag,
//                       num_num, sheet_price, cost_price, chg_amount, remain_qty, remain_price, ch_sheetno)
//               values (materialno, now(), sheetType, 1, numnum, numprice / unitrate, remainprice, 0, remainqty, remainprice, sheetNo);


//             IF (numpresent > 0) THEN
//                  set initqty   = remainqty;
//                  set initprice = remainprice;
//                  set remainqty = initqty - numpresent;

//                  IF (remainqty = 0) THEN
//                     set remainprice = 0;
//                  ELSE
//                       set remainprice = abs((initqty * initprice) / remainqty);
//                  END IF;

//                  insert cey_st_material_flow (ch_materialno, dt_operdate, sheettype, int_flag,
//                         num_num, sheet_price, cost_price, chg_amount, remain_qty, remain_price, ch_sheetno)
//                  values (materialno, now(), sheetType, 1, numpresent, 0, remainprice, 0, remainqty, remainprice, sheetNo);
//              END IF;

//             IF exists (select 1 from cey_st_material_num where ch_materialno = materialno) THEN
//              update cey_st_material_num set num_num = remainqty, cost_price = remainprice
//               where ch_materialno = materialno;
//             ELSE
//               insert into cey_st_material_num (ch_materialno, num_num, cost_price)
//                 values (materialno, remainqty, remainprice);
//             END IF;

//             fetch cur_rows into  materialno, numnum, numpresent, numprice, unitrate;

//        ELSEIF (sheetType = 'CK') THEN
//             set remainqty = initqty - numnum;
//             IF (remainqty = 0) THEN
//                set remainprice = 0;
//             ELSEIF (initqty < 0 and remainqty > 0) THEN
//                set m_de = 1;
//             ELSE
//                set remainprice = abs((initqty * initprice - numnum / unitrate * numprice   ) / remainqty);
//              END IF;

//             insert cey_st_material_flow (ch_materialno, dt_operdate, sheettype, int_flag,
//                       num_num, sheet_price, cost_price, chg_amount, remain_qty, remain_price, ch_sheetno)
//               values (materialno, now(), sheetType, 1, numnum, numprice/unitrate, remainprice, 0, remainqty, remainprice, sheetNo);

//             IF exists (select 1 from cey_st_material_num where ch_materialno = materialno) THEN
//              update cey_st_material_num set num_num = remainqty, cost_price = remainprice
//               where ch_materialno = materialno;
//             ELSE
//               insert into cey_st_material_num (ch_materialno, num_num, cost_price)
//                 values (materialno, remainqty, remainprice);
//             END IF;

//             fetch cur_rows into  materialno, numnum, numpresent, numprice, unitrate;

//        ELSE
//            fetch cur_rows into  materialno, numnum, numpresent, numprice, unitrate;
//        END IF;

//      END IF;

// END WHILE;

//  close cur_rows;

//END
