#include "cook_zhuantai_quanbu.h"
#include "ui_cook_zhuantai_quanbu.h"
#include <QSqlQueryModel>
#include <QSqlRecord>
#include "lds_messagebox.h"
#include "backheader.h"

cook_zhuantai_quanbu::cook_zhuantai_quanbu(const QString & str,QWidget *parent) :
    _str(str),QDialog(parent),
    ui(new Ui::cook_zhuantai_quanbu)
{
    ui->setupUi(this);
    //this->setWindowFlags(Qt::FramelessWindowHint|Qt::Dialog);this->setStyleSheet("QDialog{border:2px solid gray}");
    btn_t_f = false;
    init_control();
    ui->lineEdit->setText(_str);
    ui->lineEdit_2->setFocus();
}

cook_zhuantai_quanbu::~cook_zhuantai_quanbu()
{
    delete ui;
}

void cook_zhuantai_quanbu::init_control()
{
    ui->lineEdit->setFocus();
//    ui->tableView->verticalHeader()->hide();
//    ui->tableView->horizontalHeader()->setStyleSheet("QHeaderView::section{background-color: qlineargradient(spread:reflect, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(255, 255, 74, 255), stop:1 rgba(255, 255,21, 255));color:black;height:35px;border-radius:0px;font-size:14pt;}");
//    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
//    ui->tableView->verticalHeader()->setDefaultSectionSize(50);
//    //ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置属性为只读
    // ui->tableView->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents );//文字自适应表格

    QStringList labels;
    labels <<"行号" <<"餐桌号" <<"餐桌名"<<"餐桌类型" <<"当前单号" ;
    table_model = new QStandardItemModel();
    table_model->setHorizontalHeaderLabels(labels);
    ui->tableView->setModel(table_model);
    ui->tableView->setSelectionBehavior(QTableView::SelectRows);
    ui->tableView->setSelectionMode(QTableView::SingleSelection);
    QSqlQueryModel model;
    QSqlQueryModel type;
    model.setQuery(QString("select * from cey_bt_table where ch_billno is null "));
    for(int i = 0; i < model.rowCount(); i++)
    {
        int row = table_model->rowCount();
        table_model->setRowCount(row + 1);

        table_model->setItem(row,0,new QStandardItem(QString::number(row + 1))); //行号
        table_model->setItem(row,1,new QStandardItem(model.record(i).value(0).toString())); //餐桌号
        table_model->setItem(row,2,new QStandardItem(model.record(i).value(1).toString())); //餐桌名
        type.clear();
        type.setQuery(QString("select vch_typename from cey_bt_table_type where ch_typeno ='%1' ").arg(model.record(i).value(9).toString()));
        table_model->setItem(row,3,new QStandardItem(type.record(0).value(0).toString())); //餐桌类型
        table_model->setItem(row,4,new QStandardItem(model.record(i).value(3).toString())); //当前单号
    }

//    ui->tableView->setColumnWidth(0,40);
//    ui->tableView->setColumnWidth(1,80);
//    ui->tableView->setColumnWidth(2,100);
//    ui->tableView->setColumnWidth(3,100);
//    ui->tableView->setColumnWidth(4,150);

//    ui->tableView->horizontalHeader()->setResizeMode(0,QHeaderView::Fixed);
//    ui->tableView->horizontalHeader()->setResizeMode(1,QHeaderView::Fixed);
//    ui->tableView->horizontalHeader()->setResizeMode(2,QHeaderView::Fixed);
//    ui->tableView->horizontalHeader()->setResizeMode(3,QHeaderView::Fixed);
//    ui->tableView->horizontalHeader()->setResizeMode(4,QHeaderView::Fixed);

    if(table_model->rowCount() > 0)
    {
        ui->lineEdit_2->setText(table_model->item(0,1)->text());
        ui->tableView->selectRow(0);
    }

    ui->lineEdit->setEnabled(false);
}

void cook_zhuantai_quanbu::on_pushButton_15_clicked()
{
    btn_t_f= false;
    this->close();
}

void cook_zhuantai_quanbu::on_pushButton_7_clicked()
{
    if(0==lds_messagebox::warning(this, tr("消息提示"),tr("确定进行餐桌转台操作?"),tr("确认"),tr("取消")))
    {
        btn_t_f = true;
        zhuan_zhuohao = ui->lineEdit_2->text();
    }
    else
    {
        btn_t_f = false;
    }
    this->close();
}

void cook_zhuantai_quanbu::on_tableView_clicked(const QModelIndex &index)
{
    int row = ui->tableView->currentIndex().row();
    if(row <= -1)
    {
        return;
    }
    ui->lineEdit_2->setText(table_model->item(row,1)->text());
}
