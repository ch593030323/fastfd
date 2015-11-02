#include "cook_operation.h"
#include "ui_cook_operation.h"
#include <QSqlQueryModel>
#include <QSqlRecord>
#include "lds_tableview_delegate_check.h"
#include "lds_messagebox.h"
#include "lds_messagebox.h"
#include "backheader.h"
#include "w_scr_dish_restaurant_dialog.h"

cook_operation::cook_operation(const QString &str,QWidget *parent) :
    _str(str),QDialog(parent),
    ui(new Ui::cook_operation)
{
    ui->setupUi(this);
    //this->setWindowFlags(Qt::FramelessWindowHint|Qt::Dialog);this->setStyleSheet("QDialog{border:2px solid gray}");
    btn_t_f = false;
    init_control();
    print = false;
}

cook_operation::~cook_operation()
{
    delete ui;
}

void cook_operation::init_control()
{
    ui->lineEdit->setFocus();
//    ui->tableView->verticalHeader()->hide();
//    ui->tableView->horizontalHeader()->setStyleSheet("QHeaderView::section{background-color: qlineargradient(spread:reflect, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(255, 255, 74, 255), stop:1 rgba(255, 255,21, 255));color:black;height:35px;border-radius:0px;font-size:14pt;}");
//    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
//    ui->tableView->verticalHeader()->setDefaultSectionSize(50);
//    //ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置属性为只读
   // ui->tableView->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents );//文字自适应表格

    QStringList labels;
    labels <<"行号"<<"是否锁定" <<"餐桌号" <<"餐桌名"<<"餐桌类型" <<"当前单号" ;
    table_model = new QStandardItemModel();
    table_model->setHorizontalHeaderLabels(labels);
    //    ui->tableView->setModel(table_model);


    ui->tableView->setModel(table_model);
    lds_tableview_delegate_check *delegate=new lds_tableview_delegate_check(QVector<int>() << 1 ,ui->tableView);
    //delegate->setClickable();
    ui->tableView->setItemDelegate(delegate);

    QSqlQueryModel model;
    QSqlQueryModel type;
    model.setQuery(QString("select * from cey_bt_table where ch_billno <> '' and ch_tableno <> '%1' ").arg(frontViewDialog_virtual::cur_desk_no));
    for(int i = 0; i < model.rowCount(); i++)
    {
        int row = table_model->rowCount();
        table_model->setRowCount(row + 1);

        table_model->setItem(row,0,new QStandardItem(QString::number(row + 1))); //行号
        table_model->setItem(row,1,new QStandardItem(model.record(i).value(4).toString())); //是否锁定
        table_model->setItem(row,2,new QStandardItem(model.record(i).value(0).toString())); //餐桌号
        table_model->setItem(row,3,new QStandardItem(model.record(i).value(1).toString())); //餐桌名
        type.clear();
        type.setQuery(QString("select vch_typename from cey_bt_table_type where ch_typeno ='%1' ").arg(model.record(i).value(9).toString()));
        table_model->setItem(row,4,new QStandardItem(type.record(0).value(0).toString())); //餐桌类型
        table_model->setItem(row,5,new QStandardItem(model.record(i).value(3).toString())); //当前单号
    }

//    ui->tableView->setColumnWidth(0,40);
//    ui->tableView->setColumnWidth(1,80);
//    ui->tableView->setColumnWidth(2,100);
//    ui->tableView->setColumnWidth(3,100);
//    ui->tableView->setColumnWidth(4,100);
//    ui->tableView->setColumnWidth(5,150);

//    ui->tableView->horizontalHeader()->setResizeMode(0,QHeaderView::Fixed);
//    ui->tableView->horizontalHeader()->setResizeMode(1,QHeaderView::Fixed);
//    ui->tableView->horizontalHeader()->setResizeMode(2,QHeaderView::Fixed);
//    ui->tableView->horizontalHeader()->setResizeMode(3,QHeaderView::Fixed);
//    ui->tableView->horizontalHeader()->setResizeMode(4,QHeaderView::Fixed);
//    ui->tableView->horizontalHeader()->setResizeMode(5,QHeaderView::Fixed);


    if(table_model->rowCount() > 0)
    {
        ui->lineEdit->setText(table_model->item(0,2)->text());
        ui->tableView->selectRow(0);
        zhuan_danhao = table_model->item(0,5)->text();
        zhuan_zhuohao = table_model->item(0,2)->text();
    }
}

void cook_operation::on_pushButton_15_clicked()
{
    btn_t_f = false;
    this->close();
}

void cook_operation::on_tableView_clicked(const QModelIndex &index)
{
    int row = ui->tableView->currentIndex().row();
    if(row <= -1)
    {
        return;
    }
    ui->lineEdit->setText(table_model->item(row,2)->text());
    zhuan_danhao = table_model->item(row,5)->text();
    zhuan_zhuohao = table_model->item(row,2)->text();
}

void cook_operation::on_pushButton_7_clicked()//确定
{
    QSqlQueryModel model;
    ;

    model.setQuery(QString("select ch_lockflag from cey_bt_table where ch_tableno = '%1' ").arg(ui->lineEdit->text()));
    if((model.record(0).value(0).toString() == "Y"))
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("该餐桌已锁台,请确定!"));
        
        return;
    }
    if(lds_messagebox::warning(this, tr("消息提示"),tr("是否将指定菜转入指定餐桌?"),tr("确认"),tr("取消")))
    {
         btn_t_f = true;
    }
    else
    {
        btn_t_f = false;
    }
    this->close();
}

void cook_operation::on_checkBox_clicked()
{
    if(ui->checkBox->isChecked() == true)
    {
        print = true;
    }
    else
    {
        print = false;
    }
}
