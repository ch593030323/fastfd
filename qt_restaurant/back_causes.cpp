#include "back_causes.h"
#include "ui_back_causes.h"
#include <QSqlQueryModel>
#include <QSqlRecord>

back_causes::back_causes(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::back_causes)
{
    ui->setupUi(this);
    //this->setWindowFlags(Qt::FramelessWindowHint|Qt::Dialog);this->setStyleSheet("QDialog{border:2px solid gray}");
    ui->lineEdit->setEnabled(false);
    init_control();
}

back_causes::~back_causes()
{
    delete ui;
}

void back_causes::init_control()
{
    QSqlQueryModel model;

    ui->lineEdit->setFocus();
//    ui->tableView->verticalHeader()->hide();
//    ui->tableView->horizontalHeader()->setStyleSheet("QHeaderView::section{background-color: qlineargradient(spread:reflect, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(255, 255, 74, 255), stop:1 rgba(255, 255,21, 255));color:black;height:35px;border-radius:0px;font-size:14pt;}");
//    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
//    ui->tableView->verticalHeader()->setDefaultSectionSize(50);
//    //ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置属性为只读
//    ui->tableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //ui->tableView->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents );//文字自适应表格

    QStringList labels;
    labels <<"行号"<<"原因说明";
    table_model = new QStandardItemModel();
    table_model->setHorizontalHeaderLabels(labels);
    ui->tableView->setModel(table_model);

    model.setQuery(QString("SELECT vch_reason  FROM cey_bt_reason Where vch_type = '退菜原因'"));
    for(int i = 0; i < model.rowCount();i++)
    {
        int row = table_model->rowCount();
        table_model->setRowCount(row + 1);
        table_model->setItem(row,0,new QStandardItem(QString::number(row + 1))); //行号
        table_model->setItem(row,1,new QStandardItem(model.record(i).value(0).toString()));
    }

//    ui->tableView->setColumnWidth(0,40);
//    ui->tableView->setColumnWidth(1,538);
//    ui->tableView->horizontalHeader()->setResizeMode(0,QHeaderView::Fixed);
//    ui->tableView->horizontalHeader()->setResizeMode(1,QHeaderView::Fixed);

    if(table_model->rowCount() > 0)
    {
        ui->tableView->selectRow(0);
    }
}

void back_causes::on_pushButton_15_clicked()
{
    this->close();
}

void back_causes::on_pushButton_7_clicked()
{
    if(ui->checkBox_ch_curprice_4->isChecked() == true)
    {
        causes = ui->lineEdit->text();
    }
    else
    {
        int row = ui->tableView->currentIndex().row();
        if(row >= 0)
        {
            causes = table_model->item(row,1)->text();
        }
    }
    this->close();
}

void back_causes::on_checkBox_ch_curprice_4_clicked()
{
    if(ui->checkBox_ch_curprice_4->isChecked() == true)
    {
        ui->lineEdit->setEnabled(true);
    }
    else
    {
        ui->lineEdit->setEnabled(false);
    }
}
