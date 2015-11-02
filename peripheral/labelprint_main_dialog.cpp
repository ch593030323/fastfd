#include "labelprint_main_dialog.h"
#include "ui_labelprint_main_dialog.h"
#include "labelprint_set_dialog.h"
#include "labelprint_set_tagbarcode_dialog.h"
#include "labelprint_design_dialog.h"
#include <QtDebug>
#include "commonheader.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include "labelprint_goodinfo_add_dialog.h"

labelprint_main_dialog::labelprint_main_dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::labelprint_main_dialog)
{
    ui->setupUi(this);setWindowFlags(Qt::Dialog|Qt::FramelessWindowHint);this->setLayoutDirection(CommonHeader::com_widget_dir);
    this->setWindowTitle(tr("标签打印机"));
    setdialog=static_cast<labelprint_set_dialog*>(ui->stackedWidget->widget(1));
    gooddialog=static_cast<labelprint_goodinfo_dialog*>(ui->stackedWidget->widget(2));
    designdialog=static_cast<labelprint_design_dialog*>(ui->stackedWidget->widget(0));
    setdialog->st_dialog=designdialog->setdialog;

    ui->stackedWidget->setCurrentIndex(2);
    designdialog->design->printmodel=printmodel=gooddialog->printmodel;

    connect(ui->pushButton_exit,SIGNAL(clicked()),this,SLOT(toexit()));
    connect(ui->radioButton_overview,SIGNAL(clicked()),this,SLOT(toswitch()));
    connect(ui->radioButton_set,SIGNAL(clicked()),this,SLOT(toswitch()));
    connect(ui->radioButton_goodinfo,SIGNAL(clicked()),this,SLOT(toswitch()));
    connect(ui->pushButton_print,SIGNAL(clicked()),designdialog,SLOT(toprint()));
    connect(gooddialog,SIGNAL(signal_add()),this,SLOT(togoodadd()));

}

labelprint_main_dialog::~labelprint_main_dialog()
{
    delete ui;
}

void labelprint_main_dialog::inputfinished()
{
    QSqlDatabase db_sql=QSqlDatabase::addDatabase("QSQLITE", "labelprint_main_dialog");
    db_sql.setDatabaseName(CommonHeader::com_root_dirpath+"/labelprint_main_dialog.db");
    if(db_sql.open()){
        QSqlQuery query(db_sql);
        query.exec("create table if not exists labelprint_main_dialog_table ("
                   "int_id int primary key,"
                   "vch_billno char(20),"
                   "vch_ip char(20), "
                   "vch_content char(2048) ,"
                   "dt_date datetime NULL,"
                   "vch_type char(20)"
                   ")"
                   );
    }
    gooddialog->isfinishinput=true;
}

void labelprint_main_dialog::toprint()
{
    designdialog->toprint(false, false);
}

void labelprint_main_dialog::goodsadd(const QList<QStringList> &itemslist)
{
    /** temslist
    1 2 3 4 5 6
    1 2 3 4 5 6
    1 2 3 4 5 6
*/
    foreach(QStringList items, itemslist){
        QList<QStandardItem*> items0;
        for(int c=0,ccount=items.count();c<ccount;c++){
            items0.append(new QStandardItem(items[c]));
        }
        printmodel->appendRow(items0);
    }
}

void labelprint_main_dialog::togoodadd()
{
    ///标签打印机导入的表头是什么，生成的itemlist的一行对应的就是什么
    ///重载步骤：
    /// 1调用其他窗口，生成对应的itemlist，
    /// 2调用goodsadd(itemslist);
    //1
    QList<QStringList> itemslist;
    labelprint_goodinfo_add_dialog dialog(this);
    dialog.ui->tableView->setModel(gooddialog->inputallgoods_model);

    if(QDialog::Accepted==dialog.exec()){
        QModelIndexList indexlist=dialog.ui->tableView->selectionModel()->selectedRows();

        foreach(QModelIndex index, indexlist){
            QStringList items;
            for(int c=0,ccount=gooddialog->inputallgoods_model->columnCount();c<ccount;c++){
                items.append(gooddialog->inputallgoods_model->item(index.row(), c)->text());
            }
            itemslist.append(items);
        }
    }
    //2
    goodsadd(itemslist);
}

void labelprint_main_dialog::toexit()
{
    setdialog->tosave();
    designdialog->setdialog->tosave();
    designdialog->tosave();
    this->reject();
}

void labelprint_main_dialog::togoodremove()
{

}

void labelprint_main_dialog::tooverview()
{
    labelprint_design_dialog dialog(this);
}

void labelprint_main_dialog::toset()
{
    labelprint_set_dialog dialog(this);
}

void labelprint_main_dialog::toswitch()
{
    QRadioButton *b=qobject_cast<QRadioButton*>(sender());
    if(b==ui->radioButton_goodinfo){
        ui->stackedWidget->setCurrentIndex(2);
    }
    if(b==ui->radioButton_overview){
        ui->stackedWidget->setCurrentIndex(0);
    }
    if(b==ui->radioButton_set){
        ui->stackedWidget->setCurrentIndex(1);
    }

    setdialog->tosave();
}
