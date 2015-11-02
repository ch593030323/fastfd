#include "w_scr_dish_data_collection.h"
#include "ui_w_scr_dish_data_collection_dialog.h"

#include"ui_w_bt_dish_unitset_dialog.h"
#include "lds_model_sqlrelationaltablemodel.h"
#include "lds_tableview_delegate_check.h"
#include "backheader.h"
#include "lds_messagebox.h"
#include <QSqlRecord>
#include <QSqlQuery>
#include <QDebug>
#include <qsqlerror.h>
#include <QSqlRelationalDelegate>
w_scr_dish_data_collection::w_scr_dish_data_collection(QWidget *parent) :
    QDialog(parent),
    ui(new Ui_w_scr_dish_data_collection_dialog)
{
    ui->setupUi(this);
    //! tableview
    tablemodel=new lds_model_sqlrelationaltablemodel(this);
    tablemodel->setTable("cey_web_data_collection");
    tablemodel->setEditStrategy(QSqlTableModel::OnManualSubmit);tablemodel->setJoinMode(QSqlRelationalTableModel::LeftJoin);
    tablemodel->select();
    ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::SelectedClicked);
    ui->tableView->setModel(tablemodel->fpmodel);
    tablemodel->save_set_header("service_ip", tr("服务器ip"));
    tablemodel->save_set_header("service_port", tr("服务器port"));
    ui->tableView->rePecifyHeader(tablemodel->save_set_header_saver);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->setColumnWidth(0, 100);

    //2
    mapper = new QDataWidgetMapper(this);
    mapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
    mapper->setModel(tablemodel);
    mapper->setItemDelegate(new QSqlRelationalDelegate(mapper));
    mapper->addMapping(ui->lineEdit_cashier,tablemodel->fieldIndex("cashier"));
    mapper->addMapping(ui->lineEdit_ch_uploadflag,tablemodel->fieldIndex("ch_uploadflag"));
    mapper->addMapping(ui->lineEdit_ch_uploadmemo,tablemodel->fieldIndex("ch_uploadmemo"));
    mapper->addMapping(ui->lineEdit_itemcode,tablemodel->fieldIndex("itemcode"));
    mapper->addMapping(ui->lineEdit_mallid,tablemodel->fieldIndex("mallid"));
    mapper->addMapping(ui->lineEdit_service_ip,tablemodel->fieldIndex("service_ip"));
    mapper->addMapping(ui->lineEdit_service_port,tablemodel->fieldIndex("service_port"));
    mapper->addMapping(ui->lineEdit_service_submit_path,tablemodel->fieldIndex("service_submit_path"));
    mapper->addMapping(ui->lineEdit_storecode,tablemodel->fieldIndex("storecode"));
    mapper->addMapping(ui->lineEdit_txdate_yyyymmdd,tablemodel->fieldIndex("txdate_yyyymmdd"));
    mapper->addMapping(ui->lineEdit_txtime_hhmmss,tablemodel->fieldIndex("txtime_hhmmss"));
    mapper->addMapping(ui->lineEdit_username,tablemodel->fieldIndex("username"));

    //3
    connect(ui->tableView,SIGNAL(selectchanged(int)),this,SLOT(mapper_setcurindex(int)));
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(toexit()));
    connect(ui->pushButton_save, SIGNAL(clicked()),this,SLOT(tosave()));

    mapper->toFirst();
    ui->tableView->selectRow(0);
}

void w_scr_dish_data_collection::toexit()
{
    this->reject();
}

void w_scr_dish_data_collection::tosave()
{
    if(tablemodel->submitAll()){
        lds_messagebox::warning(this, tr("消息提示"),tr("保存成功"));
    } else {
        lds_messagebox::warning(this, tr("消息提示"),tr("数据有误，操作失败")+ tablemodel->lastError().text());
    }
}

void w_scr_dish_data_collection::mapper_setcurindex(int index)
{
    mapper->setCurrentIndex(index);
}
