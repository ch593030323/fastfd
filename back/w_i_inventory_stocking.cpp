#include "w_i_inventory_stocking.h"
//库存盘点
#include"ui_w_i_inventory_goodsinput_dialog.h"
#include "backheader.h"
#include <QDebug>
#include <QSqlQueryModel>
#include <QStandardItemModel>
#include "n_func.h"
#include "lds_messagebox.h"
#include <QSqlError>
#include "lds_tableview_delegate_check.h"


QVariant w_i_inventory_stocking::QueryModel::data(const QModelIndex &item, int role) const
{
    if(role==Qt::DisplayRole){
        if(item.column() == 0){
            return backheader::VIPFINDVALUE(w_i_inventory::MODEL_INVENTORY_STATE_TYPE, QSqlQueryModel::data(item, role).toString());
        }

        if(item.column() == 4){
            return QSqlQueryModel::data(item, role).toDateTime().toString("yyyy-MM-dd hh:mm:ss");
        }
    }
    return QSqlQueryModel::data(item, role);
}


Qt::ItemFlags w_i_inventory_stocking::StandardItemModel::flags(const QModelIndex &index) const
{
    if(index.column()==6){
        return Qt::ItemIsSelectable|Qt::ItemIsEnabled|Qt::ItemIsEditable;
    }
    return Qt::ItemIsSelectable|Qt::ItemIsEnabled;
}



//======================================
w_i_inventory_stocking::w_i_inventory_stocking(QWidget *parent) :
    QDialog(parent),
    ui(new Ui_w_i_inventory_goodsinput_Dialog),
    _type("PD")
{
    ui->setupUi(this);
    ui->frame->setFixedWidth(w_rpt_report::frame_left_fixed_width);
    //时间改成年月日
    ui->dateTimeEdit->setDisplayFormat("yyyy-MM-dd");
    ui->dateTimeEdit_2->setDisplayFormat("yyyy-MM-dd");
    ui->dateTimeEdit->setDate(n_func::f_get_sysdatetime().date());
    ui->dateTimeEdit_2->setDate(n_func::f_get_sysdatetime().date());

    //model
    querymodel=new QueryModel;
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);ui->tableView->setModel(querymodel);
    standmodel_2=new StandardItemModel;
    ui->tableView_2->setEditTriggers(QAbstractItemView::NoEditTriggers);ui->tableView_2->setModel(standmodel_2);
    ui->tableView_2->tableView->setItemDelegate(new lds_tableview_delegate_check(QVector<int>()
                                                                                 << 6
                                                                                 ,
                                                                                 ui->tableView_2->tableView));
    //! toolbar
    ui->widget->setup(QStringList() << tr("新增盘点单") << tr("盘点录入") << tr("刷新") << tr("退出"), Qt::AlignTop);
    connect(ui->widget->index_widget(tr("新增盘点单")),SIGNAL(clicked()),this,SLOT(tonewstock()));
    connect(ui->widget->index_widget(tr("盘点录入")),SIGNAL(clicked()),this,SLOT(toloadstock()));
    connect(ui->widget->index_widget(tr("刷新")),SIGNAL(clicked()),this,SLOT(torefresh()));
    connect(ui->widget->index_widget(tr("退出")),SIGNAL(clicked()),this,SLOT(toexit()));

    connect(ui->radioButton_all, SIGNAL(clicked()),this,SLOT(torefresh()));
    connect(ui->radioButton_no, SIGNAL(clicked()),this,SLOT(torefresh()));
    connect(ui->radioButton_ok, SIGNAL(clicked()),this,SLOT(torefresh()));
    connect(ui->radioButton_bad, SIGNAL(clicked()),this,SLOT(torefresh()));

    connect(ui->tableView,SIGNAL(selectchanged(int)),this,SLOT(toupdatetableview2(int)));
    //! after
    torefresh();

    //    ui->tableView_2->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

}

lds_tableview_sum *w_i_inventory_stocking::getTableview_2()
{
    return ui->tableView_2;
}

w_i_inventory_stocking::StandardItemModel *w_i_inventory_stocking::getStandmodel_2()
{
    return static_cast<StandardItemModel *>(standmodel_2);
}

lds_tableView *w_i_inventory_stocking::getTableview()
{
    return ui->tableView;
}

QSqlQueryModel *w_i_inventory_stocking::getQuerymodel()
{
    return querymodel;
}

void w_i_inventory_stocking::tonewstock()
{
    w_i_inventory_stocking_new dialog(this);
    backheader::static_dialogexec(dialog,qobject_cast<QAbstractButton* >(sender())->text());
}

void w_i_inventory_stocking::toloadstock()
{
    w_i_inventory_stocking_change dialog(this);
    backheader::static_dialogexec(dialog,qobject_cast<QAbstractButton* >(sender())->text());
}

void w_i_inventory_stocking::toexit()
{
    this->reject();
}

void w_i_inventory_stocking::toupdatetableview2(int index)
{
    QString filtersql;
    if(index>=0){
        filtersql=querymodel->record(index).value("ch_sheetNo").toString();
    }

    //sql语句 select
    QString selectsql=

            "SELECT  "
            " cey_st_check_detail.ch_materialno ,"
            "  cey_bt_dish.vch_dishname ,"
            " (select vch_unitname from cey_bt_unit where cey_bt_unit.ch_unitno=cey_bt_dish.ch_unitno), "
            "  cey_st_check_detail.num_sys ,"
            "  cey_st_check_detail.num_check ,"
            "  cey_st_check_detail.num_pro_loss ,"
            "  cey_st_check_detail.chg_flag "

            //            " cey_st_check_detail.ch_sheetno ,"
            //            "   cey_st_check_detail.cost_price ,"
            //            "  cey_bt_dish.ch_unitno    "
            " FROM cey_st_check_detail ,"
            "  cey_bt_dish    "
            "   WHERE(cey_st_check_detail.ch_materialno = cey_bt_dish.ch_dishno )"
            "  and         (( cey_st_check_detail.ch_sheetno = '%1'))  "
            ;

    backheader::standmodelfilldata(standmodel_2,
                                   selectsql
                                   .arg(filtersql));
    qDebug() << selectsql.arg(filtersql);
    //更新表头
    standmodel_2->setHorizontalHeaderLabels(QStringList() << tr("编码") << tr("名称") << tr("单位") << tr("系统库存") << tr("盘点数量")<< tr("盈亏数量")<< tr("修改库存"));

    //tableview2 restore
    ui->tableView_2->tableView->restore();
}

void w_i_inventory_stocking::torefresh(const QString &findsheetno)
{
    //sql语句 select
    QString selectsql=
            "SELECT  "
            "ch_state,"
            "ch_sheetNo,"
            "(select vch_operator  from (Select vch_operID,vch_operator from cey_sys_operator union Select '0000' as vch_operID,'system' as vch_operator )t where t.vch_operID=cey_st_check_master.vch_handle), "
            "(select vch_operator  from (Select vch_operID,vch_operator from cey_sys_operator union Select '0000' as vch_operID,'system' as vch_operator )t where t.vch_operID=cey_st_check_master.vch_operId), "
            "dt_operdate,"
            "(select vch_operator  from (Select vch_operID,vch_operator from cey_sys_operator union Select '0000' as vch_operID,'system' as vch_operator )t where t.vch_operID=cey_st_check_master.vch_audit), "
            "dt_audit,"
            "vch_memo"
            " FROM cey_st_check_master "
            ;
    if(ui->radioButton_all->isChecked()){
        selectsql+=
                " where  ((ch_state = 'N' and dt_operdate between '%1' and"
                " '%2') or"
                " (ch_state <> 'N' and dt_audit between '%1' and"
                " '%2'))"
                ;
    } else if(ui->radioButton_no->isChecked()){
        selectsql+=
                "where (ch_state='N' and dt_operdate between '%1' and '%2')";
    } else if(ui->radioButton_ok->isChecked()){
        selectsql+=
                "where (ch_state='Y' and dt_audit between '%1' and '%2')";
    } else if(ui->radioButton_bad->isChecked()){
        selectsql+=
                "where (ch_state='O' and dt_audit between '%1' and '%2')";
    }
    querymodel->setQuery(selectsql
                         .arg(QDateTime(ui->dateTimeEdit->date()).toString("yyyy-MM-dd hh:mm:ss"))
                         .arg(QDateTime(ui->dateTimeEdit_2->date(), QTime(23,59,59)).toString("yyyy-MM-dd hh:mm:ss"))
                         );
    qDebug() << querymodel->query().lastQuery();
    //更新表头
    backheader::modelSetHeader(querymodel, QStringList() << tr("状态") << tr("盘点单号") << tr("盘点人") << tr("制单人") << tr("制单日期") << tr("审核人") << tr("审核日期")<< tr("备注"));
    //为空则选中第一行，否则遍历选中sheetno所在行
    if(findsheetno.isEmpty()){
        ui->tableView->restore();
    } else {
        for(int r=0,r_count=querymodel->rowCount();r<r_count;r++){
            if(querymodel->record(r).value("ch_sheetNo").toString()==findsheetno){
                ui->tableView->selectRow(r);
                break;
            }
        }
    }
    //更新tableview 2的数据
    toupdatetableview2(ui->tableView->currentIndex().row());
}

//==========================================
#include "ui_w_i_inventory_stocking_change_dialog.h"
#include "ui_w_i_inventory_stocking_change_change_dialog.h"
#include <QTimer>
w_i_inventory_stocking_change::w_i_inventory_stocking_change(w_i_inventory_stocking *parent):
    QDialog(parent),
    ui(new Ui_w_i_inventory_stocking_change_Dialog),
    _parent(parent),
    standmodel(0),
    parentrow(-1)
{
    ui->setupUi(this);
    ui->frame_2->setObjectName("inventory");

    //导入数据
    ui->comboBox_vch_handle->addItems(inventory_queryfilter("Select vch_operID,vch_operator from cey_sys_operator union Select '0000' as vch_operID,'system' as vch_operator "));
    //    ui->frame_2->setEnabled(false);
    //! toolbar
    ui->widget->setup(QStringList() << tr("退出"), Qt::AlignTop);
    connect(ui->widget->index_widget(tr("退出")),SIGNAL(clicked()),this,SLOT(toexit()));

    //! after
    ui->tableView->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->lineEdit_ch_sheetno->setEnabled(false);
    ui->lineEdit_dt_audit->setEnabled(false);
    ui->lineEdit_dt_operdate->setEnabled(false);
    ui->lineEdit_vch_audit->setEnabled(false);
    ui->lineEdit_vch_operid->setEnabled(false);
    QTimer::singleShot(0, this, SLOT(infoupdate()));//盘点信息update
    QTimer::singleShot(0, this, SLOT(typewindow()));//查看盘点信号槽

}

void w_i_inventory_stocking_change::toexit()
{
    _parent->torefresh();
    this->reject();
}
//QString selectsql=
//        "SELECT  "
//        "ch_state,"
//        "ch_sheetNo,"
//        "(select vch_operator  from (Select vch_operID,vch_operator from cey_sys_operator union Select '0000' as vch_operID,'system' as vch_operator )t where t.vch_operID=cey_st_check_master.vch_handle), "
//        "(select vch_operator  from (Select vch_operID,vch_operator from cey_sys_operator union Select '0000' as vch_operID,'system' as vch_operator )t where t.vch_operID=cey_st_check_master.vch_operId), "
//        "dt_operdate,"
//        "(select vch_operator  from (Select vch_operID,vch_operator from cey_sys_operator union Select '0000' as vch_operID,'system' as vch_operator )t where t.vch_operID=cey_st_check_master.vch_audit), "
//        "dt_audit,"
//        "vch_memo"
//        " FROM cey_st_check_master "
void w_i_inventory_stocking_change::tobillsave()
{
    QString ls_sheetno = ui->lineEdit_ch_sheetno->text();
    if(ui->comboBox_vch_handle->currentText().isEmpty()){
        lds_messagebox::warning(this, tr("消息提示"),tr("盘点责任人不能为空，请选择后再保存!"));
        return;
    }
    //! <--transaction begin-->
    QSqlQuery query;
    QSqlDatabase::database().transaction();
    QString errsql;
    //保存责任人
    //保存备注信息
    if(query.exec(QString("update cey_st_check_master set vch_handle='%1' ,vch_memo='%3' where ch_sheetNo='%2' ")
                  .arg(backheader::getinterdata(ui->comboBox_vch_handle->currentText()))
                  .arg(ui->lineEdit_ch_sheetno->text())
                  .arg(ui->lineEdit_vch_memo->text())
                  )){
        //保存 model
        for(int r=0,r_count=standmodel->rowCount();r<r_count;r++){//0
            if(query.exec(QString("update cey_st_check_detail set chg_flag='%1', num_check ='%4' where ch_sheetNo='%2' and ch_materialno='%3' ")
                          .arg(standmodel->item(r, 6)->text())
                          .arg(ui->lineEdit_ch_sheetno->text())
                          .arg(standmodel->item(r, 0)->text())
                          .arg(standmodel->item(r, 4)->text())

                          )){
            } else {
                qDebug() << query.lastQuery();
                errsql=query.lastError().text();
                break;
            }
        }
    } else {
        errsql=query.lastError().text();
    }
    if(errsql.isEmpty()){
        QSqlDatabase::database().commit();
        _parent->torefresh(ls_sheetno);
        ui->lineEdit_ch_sheetno->setText(ls_sheetno);
        lds_messagebox::warning(this, tr("消息提示"),tr("保存成功!"));
    } else {
        QSqlDatabase::database().rollback();
        _parent->torefresh();
        lds_messagebox::warning(this, tr("消息提示"),tr("数据有误，操作失败!\n")+errsql);
    }
    ui->tableView->tableView->restore();
    //! <--transaction end-->

}

void w_i_inventory_stocking_change::tobillcheck()
{
    //ue_sheet_audit (none) returns (none)
    QString ls_sheetno;
    QString ls_errtext;
    QString ls_find;
    qlonglong ll_found;
    QDateTime ldt_datetime;

    ls_sheetno=ui->lineEdit_ch_sheetno->text();
    if(_parent->getQuerymodel()->record(parentrow).value("ch_state").toString()!="N"){
        return;
    }
    if(0==lds_messagebox::warning(this, tr("消息提示"),tr("确认要审核当前盘点单？[%1]")
                                  .arg(ls_sheetno), tr("确认"),tr("取消")
                                  )){
        ldt_datetime = n_func::f_get_sysdatetime();
        //! <--transaction begin-->
        QSqlQuery query;
        QSqlDatabase::database().transaction();
        QString errsql;
        query.prepare("update cey_st_check_master  set ch_state ='Y' , vch_audit =:gs_operid , dt_audit =:ldt_datetime  where ch_sheetNo =:ls_sheetno and ch_state ='N'");
        query.bindValue(":gs_operid", n_func::gs_operid);
        query.bindValue(":ldt_datetime", ldt_datetime);
        query.bindValue(":ls_sheetno", ls_sheetno);
        if(query.exec()){
            QSqlDatabase::database().commit();
            ui->tableView->tableView->restore();
            infoupdate(ls_sheetno);
            lds_messagebox::warning(this, tr("消息提示"),tr("单据[%1]已经审核!").arg(ls_sheetno));
        } else {
            QSqlDatabase::database().rollback();
            _parent->torefresh();
            ui->tableView->tableView->restore();
            lds_messagebox::warning(this, tr("消息提示"),tr("单据[%1]的状态已被更改，审核失败!").arg(ls_sheetno));
        }
        //! <--transaction end-->
    }
}

void w_i_inventory_stocking_change::tobillbad()
{
    //ue_sheet_blankout (none) returns (none)
    qlonglong ll_found;
    QString ls_sheetno;
    QString ls_errtext;
    QDateTime ldt_datetime;

    ls_sheetno=ui->lineEdit_ch_sheetno->text();

    if(_parent->getQuerymodel()->record(parentrow).value("ch_state").toString()!="N"){
        return;
    }
    if(0==lds_messagebox::warning(this, tr("消息提示"),tr("确认要作废当前盘点？[%1]")
                                  .arg(ls_sheetno), tr("确认"),tr("取消")
                                  )){
        ldt_datetime = n_func::f_get_sysdatetime();
        //! <--transaction begin-->
        QSqlQuery query;
        QSqlDatabase::database().transaction();
        QString errsql;
        query.prepare("update cey_st_check_master   set ch_state ='O' , vch_audit =:gs_operid , dt_audit =:ldt_datetime  where ch_sheetNo =:ls_sheetno and ch_state ='N' ");
        query.bindValue(":gs_operid", n_func::gs_operid);
        query.bindValue(":ldt_datetime", ldt_datetime);
        query.bindValue(":ls_sheetno", ls_sheetno);
        if(query.exec()){
            QSqlDatabase::database().commit();
            ui->tableView->tableView->restore();
            infoupdate(ls_sheetno);
            lds_messagebox::warning(this, tr("消息提示"),tr("单据[%1]已经作废!").arg(ls_sheetno));
        } else {
            QSqlDatabase::database().rollback();
            _parent->torefresh();
            ui->tableView->tableView->restore();
            lds_messagebox::warning(this, tr("消息提示"),tr("单据[%1]的状态已被更改，作废失败!").arg(ls_sheetno));
        }
        //! <--transaction end-->
    }
}

void w_i_inventory_stocking_change::infoupdate(const QString &ls_sheetno)
{
    QSqlQuery query;
    QString ls_value;
    query.exec(QString("select ch_state from cey_st_check_master where ch_sheetNo='%1' ").arg(ls_sheetno));
    query.next();
    ls_value=query.record().value("ch_state").toString();
    if(ls_value=="N"){
        ui->frame_2->drawPixmap(QPixmap());
    } else if(ls_value=="Y"){
        ui->frame_2->drawPixmap(QPixmap(":/image/widgets_pic/haschecked"));
    } else if(ls_value=="O"){
        ui->frame_2->drawPixmap(QPixmap(":/image/widgets_pic/hasbaded"));
    }

}

void w_i_inventory_stocking_change::infoupdate(int tableview_row)
{
    if(tableview_row>=0){
        //判断是 未审核； 已审核； 已作废
        if(_parent->getQuerymodel()->record(tableview_row).value("ch_state").toString()=="N"){//未审核；
            //            ui->frame_2->setEnabled(true);

        } else  if(_parent->getQuerymodel()->record(tableview_row).value("ch_state").toString()=="Y"){//已审核
            ui->frame_2->drawPixmap(QPixmap(":/image/widgets_pic/haschecked"));
            //            ui->frame_2->setEnabled(false);
        } else  if(_parent->getQuerymodel()->record(tableview_row).value("ch_state").toString()=="O"){//已作废
            ui->frame_2->drawPixmap(QPixmap(":/image/widgets_pic/hasbaded"));
            //            ui->frame_2->setEnabled(false);
        }
        if(ui->frame_2->isEnabled()){

        } else {
        }
        QSqlQuery query;
        query.exec(QString("select *from cey_st_check_master where ch_sheetNo='%1' ")
                   .arg(_parent->getQuerymodel()->record(tableview_row).value("ch_sheetNo").toString())
                   );
        query.next();
        ui->lineEdit_ch_sheetno->setText(query.record().value("ch_sheetNo").toString());
        ui->comboBox_vch_handle->setindex_c1(query.record().value("vch_handle").toString());
        ui->lineEdit_vch_memo->setText(query.record().value("vch_memo").toString());
        ui->lineEdit_vch_operid->setText(inventory_getopername(query.record().value("vch_operid").toString()));
        ui->lineEdit_dt_operdate->setText(query.record().value("dt_operdate").toDateTime().toString("yyyy-MM-dd hh:mm:ss"));
        ui->lineEdit_vch_audit->setText(query.record().value("vch_audit").toString());
        ui->lineEdit_dt_audit->setText(query.record().value("dt_audit").toDateTime().toString("yyyy-MM-dd hh:mm:ss"));

        if(standmodel==0){
            standmodel=_parent->getStandmodel_2();
            ui->tableView->setEditTriggers(QTableView::NoEditTriggers);ui->tableView->setModel(standmodel);
            lds_tableview_delegate_check *degate=new lds_tableview_delegate_check(QVector<int>()
                                                                                  << 6
                                                                                  ,
                                                                                  ui->tableView->tableView);
            //            degate->setClickable();
            ui->tableView->tableView->setItemDelegate(degate);
        }
    }
}

void w_i_inventory_stocking_change::infoupdate()
{
    infoupdate(parentrow=_parent->getTableview()->currentIndex().row());
}

void w_i_inventory_stocking_change::typewindow()
{
    ui->widget_2->setup(QStringList() << tr("保存")<< tr("盘点审核") << tr("盘点作废")<<tr("修改"));
    connect(ui->widget_2->index_widget(tr("保存")),SIGNAL(clicked()),this,SLOT(tobillsave()));
    connect(ui->widget_2->index_widget(tr("盘点审核")),SIGNAL(clicked()),this,SLOT(tobillcheck()));
    connect(ui->widget_2->index_widget(tr("盘点作废")),SIGNAL(clicked()),this,SLOT(tobillbad()));
    connect(ui->widget_2->index_widget(tr("修改")),SIGNAL(clicked()),this,SLOT(tochange()));

}
void w_i_inventory_stocking_change::tochange()
{
    if(ui->tableView->tableView->currentIndex().row()>=0){
        QDialog dialog(this);
        Ui_w_i_inventory_stocking_change_change_dialog dui;
        dui.setupUi(&dialog);
        dui.lineEdit->setText(standmodel->item(ui->tableView->tableView->currentIndex().row(), 4)->text());
        dui.checkBox->setChecked1(standmodel->item(ui->tableView->tableView->currentIndex().row(), 6)->text());
        if(QDialog::Accepted==backheader::static_dialogexec(dialog, tr("修改"))){
            standmodel->item(ui->tableView->tableView->currentIndex().row(), 4)->setText(dui.lineEdit->text());
            standmodel->item(ui->tableView->tableView->currentIndex().row(), 6)->setText(dui.checkBox->isChecked1());
        }
    }
}


//======================
w_i_inventory_stocking_new::w_i_inventory_stocking_new(w_i_inventory_stocking *parent):
    w_i_inventory_stocking_change(parent)
{

}

void w_i_inventory_stocking_new::togoods(const QStringList & texts)
{
    QSqlQuery query;

    foreach(QString text, texts){//1
        //若是该编码已经存在，则跳过
        bool isbreak=false;
        for(int r=0,r_count=standmodel->rowCount();r<r_count;r++){//0
            if(standmodel->item(r, 0)->text() == text){
                isbreak=true;
                break;
            }
        }//0
        if(isbreak) continue;


        query.exec(QString("select vch_dishname , "
                           "ch_unitno , "
                           "vch_pur_unitno ,"
                           " (select vch_unitname from cey_bt_unit where cey_bt_unit.ch_unitno=cey_bt_dish.vch_pur_unitno) as vch_pur_unitname, "
                           " int_unit_rate , ifnull(num_sale_price , 0) from cey_bt_dish where ch_dishno ='%1' ")
                   .arg(text)
                   );
        query.next();
        QList<QStandardItem *> items;
        items << new QStandardItem(text);
        items << new QStandardItem(query.record().value("vch_dishname").toString());
        items << new QStandardItem(query.record().value("vch_pur_unitname").toString());

        query.exec(QString("select num_num from cey_st_material_num where ch_materialno ='%1' ")
                   .arg(text)
                   );
        query.next();
        items << new QStandardItem(query.record().value("num_num").toString());

        standmodel->appendRow(items);
    }//1
}

void w_i_inventory_stocking_new::todel()
{
    if(ui->tableView->tableView->currentIndex().row()>=0){
        standmodel->removeRow(ui->tableView->tableView->currentIndex().row());
    }
}

void w_i_inventory_stocking_new::tobillmaker()
{
    //ue_sheet_save (none) returns integer
    //integer ancestorreturnvalue
    QString ls_sheetno;
    QString ls_materialno;
    qlonglong ll_row;
    qlonglong ll_found;
    double ldec_num;
    double ldec_cost;
    QDateTime ldt_datetime;

    ls_sheetno=ui->lineEdit_ch_sheetno->text();
    if(standmodel->rowCount()<=0){
        lds_messagebox::warning(this, tr("消息提示"),tr("未选择盘点物料，不允保存!"));
        return;
    }

    if(ui->comboBox_vch_handle->currentText().isEmpty()){
        lds_messagebox::warning(this, tr("消息提示"),tr("盘点责任人不能为空，请选择后再保存!"));
        return;
    }

    ls_sheetno = getnewsheetno();
    if(0==lds_messagebox::warning(this, tr("消息提示"),tr("是否确定: 盘点物料选择已完成，并生成盘点单[%1]")
                                  .arg(ls_sheetno), tr("确认"),tr("取消")
                                  )== false)
        return;
    if(ls_sheetno.isEmpty())
        return;
    ldt_datetime = n_func::f_get_sysdatetime();

    //! <--transaction begin-->
    QSqlQuery query;
    QSqlDatabase::database().transaction();
    QString errsql;

    if(query.exec(QString("INSERT INTO cey_st_check_master(ch_sheetno, vch_handle, vch_operId, dt_operdate, ch_state)VALUES('%1', '%2', '%3', '%4', '%5' )")
                  .arg(ls_sheetno)
                  .arg(backheader::getinterdata(ui->comboBox_vch_handle->currentText()))
                  .arg(n_func::gs_operid)
                  .arg(ldt_datetime.toString("yyyy-MM-dd hh:mm:ss"))
                  .arg("N")
                  )){
        for(int r=0,r_count=standmodel->rowCount();r<r_count;r++){//0
            query.exec(QString("select num_num , cost_price  from cey_st_material_num   where ch_materialno ='%1' ")
                       .arg(standmodel->item(r, 0)->text())//编码
                       );
            query.next();
            query.record().value("num_num").toString();//num_sys
            query.record().value("cost_price").toString();//cost_price

            if(query.exec(QString("INSERT INTO cey_st_check_detail(ch_sheetno, ch_materialno, num_sys, num_check, num_pro_loss, cost_price, chg_flag)VALUES('%1', '%2', %3, %4, %5, %6, '%7' )")
                          .arg(ls_sheetno)//ch_sheetno
                          .arg(standmodel->item(r, 0)->text())//ch_materialno
                          .arg(query.record().value("num_num").toString())//num_sys
                          .arg(query.record().value("num_num").toString())//num_check   ?
                          .arg("0.0")//num_pro_loss     ?
                          .arg(query.record().value("cost_price").toString())//cost_price
                          .arg("Y")//chg_flag
                          )){
            } else {
                errsql=query.lastError().text();
                break;
            }
        }//0
    } else {
        errsql=query.lastError().text();
    }
    if(errsql.isEmpty()){
        QSqlDatabase::database().commit();
        _parent->torefresh(ls_sheetno);
        ui->lineEdit_ch_sheetno->setText(ls_sheetno);
        lds_messagebox::warning(this, tr("消息提示"),tr("保存成功!"));
        this->accept();
    } else {
        QSqlDatabase::database().rollback();
        _parent->torefresh();
        lds_messagebox::warning(this, tr("消息提示"),tr("数据有误，操作失败!"), errsql);
    }
    ui->tableView->tableView->restore();
    //! <--transaction end-->
}

void w_i_inventory_stocking_new::infoupdate()
{
    if(standmodel)
        standmodel->removeRows(0, standmodel->rowCount());
    if(standmodel==0){
        standmodel = new QStandardItemModel;
        standmodel->setHorizontalHeaderLabels(QStringList() << tr("编码") << tr("名称") << tr("单位") << tr("系统库存"));

        ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);ui->tableView->setModel(standmodel);
    }
    ui->lineEdit_vch_operid->setText(n_func::gs_opername);
    ui->lineEdit_dt_operdate->setText(n_func::f_get_sysdatetime().toString("yyyy-MM-dd hh:mm:ss"));

}

void w_i_inventory_stocking_new::typewindow()
{
    ui->widget_2->setup(QStringList() << tr("选择物料")<< tr("删除物料") << tr("盘点单生成"));
    //    connect(ui->widget_2->index_widget(tr("选择物料")),SIGNAL(clicked()),this,SLOT(togoods()));
    connect(ui->widget_2->index_widget(tr("删除物料")),SIGNAL(clicked()),this,SLOT(todel()));
    connect(ui->widget_2->index_widget(tr("盘点单生成")),SIGNAL(clicked()),this,SLOT(tobillmaker()));

    ui->widget_2->setselectgoodsenable_check_sheetno(tr("选择物料"));
    connect(ui->widget_2,SIGNAL(okstrs(QStringList)),this,SLOT(togoods(QStringList)));

}

QString w_i_inventory_stocking_new::getnewsheetno()
{
    QDateTime dt=n_func::f_get_sysdatetime();
    QString new_sheetno=_parent->_type+dt.toString("yyyy").right(2)+dt.toString("MMdd")+"0000";
    QSqlQuery query;
    query.exec(QString("select max(ch_sheetNO) from cey_st_check_master  where  LEFT(ch_sheetNo,8)='%1' ")
               .arg(new_sheetno.left(8))
               );
    if(query.next()){
        if(query.record().value(0).toString().isEmpty()){

        } else {
            new_sheetno=query.record().value(0).toString();
        }
    }
    new_sheetno=new_sheetno.left(8)+QString().sprintf("%04d", (new_sheetno.right(4).toInt()+1)).right(4);
    return new_sheetno;
}
