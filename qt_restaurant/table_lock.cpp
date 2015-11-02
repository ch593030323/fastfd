#include "table_lock.h"
#include "ui_table_lock.h"
#include <QSqlQueryModel>
#include <QSqlRecord>
#include "lds_tableview_delegate_check.h"
#include <QSqlQuery>
#include <QDebug>
#include "lds_messagebox.h"

table_lock::table_lock(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::table_lock)
{
    ui->setupUi(this);
    init_control();
}

table_lock::~table_lock()
{
    delete ui;
}

void table_lock::init_control()
{
    QStringList labels;
    labels <<tr("是否锁定")<<tr("餐桌号")<<tr("餐桌名")<<tr("餐桌类型")<<tr("当前单号");
    table_model = new QStandardItemModel();
    table_model->setHorizontalHeaderLabels(labels);
    ui->tableView->setModel(table_model);
    ui->tableView->setSelectionBehavior(QTableView::SelectRows);
    ui->tableView->setSelectionMode(QTableView::SingleSelection);

    lds_tableview_delegate_check *delegate=new lds_tableview_delegate_check(QVector<int>() << 0 ,ui->tableView);
    delegate->setClickable();
    ui->tableView->setItemDelegate(delegate) ;


    QSqlQueryModel model;
    model.setQuery(QString("select * from cey_bt_table "));
    QSqlQueryModel type;
    for(int i = 0; i < model.rowCount(); i++)
    {
        int row = table_model->rowCount();
        table_model->setRowCount(row + 1);

        table_model->setItem(row,0,new QStandardItem(model.record(i).value(4).toString())); //是否选中
        table_model->setItem(row,1,new QStandardItem(model.record(i).value(0).toString()));//餐桌号
        table_model->setItem(row,2,new QStandardItem(model.record(i).value(1).toString()));//餐桌名
        type.clear();
        type.setQuery(QString("select vch_typename from cey_bt_table_type where ch_typeno = '%1' ").arg(model.record(i).value(9).toString()));
        table_model->setItem(row,3,new QStandardItem(type.record(0).value(0).toString()));//餐桌类型
        table_model->setItem(row,4,new QStandardItem(model.record(i).value(3).toString()));//当前单号
    }
    if(table_model->rowCount() > 0)
    {
        ui->tableView->selectRow(0);
    }
}

void table_lock::on_pushButton_7_clicked()//确定
{
    QSqlQuery query;
    QSqlDatabase::database().transaction();
    for(int i = 0; i < table_model->rowCount(); i++)
    {
        query.prepare(QString("update cey_bt_table set  ch_lockflag = :lock  where ch_tableno = :zhuohao"));
        query.bindValue(":lock",table_model->item(i,0)->text());
        query.bindValue(":zhuohao",table_model->item(i,1)->text());
        if(query.exec())
        {
        }
        else
        {

            QSqlDatabase::database().rollback();
            lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_bt_table失败"));
            
            return;
        }
    }

    QSqlDatabase::database().commit();
    this->close();
}

void table_lock::on_pushButton_14_clicked()//取消
{
    this->close();
}
