#include "choose_vip.h"
#include "ui_choose_vip.h"
#include <QSqlQueryModel>
#include <QSqlRecord>

choose_vip::choose_vip(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::choose_vip)
{
    ui->setupUi(this);
//    //this->setWindowFlags(Qt::FramelessWindowHint|Qt::Dialog);this->setStyleSheet("QDialog{border:2px solid gray}");
    init_control();
}

choose_vip::~choose_vip()
{
    delete ui;
}

void choose_vip::init_control()
{
    //    ui->tableView->verticalHeader()->hide();
    //    ui->tableView->horizontalHeader()->setStyleSheet("QHeaderView::section{background-color: qlineargradient(spread:reflect, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(132, 132, 132, 255), stop:1 rgba(105, 105,105, 255));color:white;height:35px;border-radius:0px;font-size:14pt;}");
    //    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    //    //    ui->tableView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //    //    ui->tableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //    ui->tableView->verticalHeader()->setDefaultSectionSize(40);
    //    //ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置属性为只读
    // ui->tableView->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents );//文字自适应表格

    QStringList labels;
    labels<<"会员编号"<<"会员名称"<<"会员类型"<<"手机"<<"电话"<<"生日";

    QSqlQueryModel model;
    model.setQuery(QString("SELECT a.vch_memberno,a.vch_member,a.ch_typeno,a.vch_tel, a.vch_handtel,  a.dt_birthday   FROM t_m_member a,t_m_member_type b where a.ch_typeno = b.ch_typeno and b.ch_attribute like '%' and ch_state = '1' and ch_cardflag = 'Y' "));

    model_table = new QStandardItemModel();
    model_table->setHorizontalHeaderLabels(labels);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);ui->tableView->setModel(model_table);

    for(int i = 0; i < model.rowCount(); i++)
    {
        int row = model_table->rowCount();
        model_table->setRowCount(row + 1);
        model_table->setItem(row,0,new QStandardItem(model.record(i).value(0).toString()));
        model_table->setItem(row,1,new QStandardItem(model.record(i).value(1).toString()));
        QSqlQueryModel type;
        type.clear();
        type.setQuery(QString("select vch_typename from t_m_member_type where ch_typeno = '%1' ").arg(model.record(i).value(2).toString()));
        model_table->setItem(row,2,new QStandardItem(type.record(0).value(0).toString()));
        model_table->setItem(row,3,new QStandardItem((model.record(i).value(4).toString())));
        model_table->setItem(row,4,new QStandardItem((model.record(i).value(3).toString())));
        model_table->setItem(row,5,new QStandardItem((model.record(i).value(5).toString())));
    }

    ui->tableView->selectRow(0);
    //    ui->tableView->setColumnWidth(0,100);
    //    ui->tableView->setColumnWidth(1,100);
    //    ui->tableView->setColumnWidth(2,100);
    //    ui->tableView->setColumnWidth(3,100);
    //    ui->tableView->setColumnWidth(4,100);
    //    ui->tableView->setColumnWidth(5,100);

    //    ui->tableView->horizontalHeader()->setResizeMode(0,QHeaderView::Fixed);
    //    ui->tableView->horizontalHeader()->setResizeMode(1,QHeaderView::Fixed);
    //    ui->tableView->horizontalHeader()->setResizeMode(2,QHeaderView::Fixed);
    //    ui->tableView->horizontalHeader()->setResizeMode(3,QHeaderView::Fixed);
    //    ui->tableView->horizontalHeader()->setResizeMode(4,QHeaderView::Fixed);
    //    ui->tableView->horizontalHeader()->setResizeMode(5,QHeaderView::Fixed);
}

void choose_vip::on_pushButton_4_clicked()//确定
{
    _text="";
    int row=ui->tableView->currentIndex().row();
    if(row>=0)
        _text=model_table->item(row, 0)->text();
    this->close();
}

void choose_vip::on_pushButton_5_clicked()//取消
{
    this->close();
}

void choose_vip::on_lineEdit_textChanged(const QString &arg1)//查卡
{
    QString str = ui->lineEdit->text();
    QString linetext = "%" +  str + "%";

    QStringList labels;
    labels<<"会员编号"<<"会员名称"<<"会员类型"<<"手机"<<"电话"<<"生日";

    QSqlQueryModel model;
    model.setQuery(QString("SELECT a.vch_memberno,a.vch_member,a.ch_typeno,a.vch_tel, a.vch_handtel,  a.dt_birthday   FROM t_m_member a,t_m_member_type b where a.ch_typeno = b.ch_typeno and b.ch_attribute like '%' and ch_state = '1' and ch_cardflag = 'Y' and(a.vch_memberno like '%1' or a.vch_handtel like '%2' or a.vch_tel like '%3')").arg(linetext).arg(linetext).arg(linetext));

    model_table = new QStandardItemModel();
    model_table->setHorizontalHeaderLabels(labels);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);ui->tableView->setModel(model_table);

    for(int i = 0; i < model.rowCount(); i++)
    {
        int row = model_table->rowCount();
        model_table->setRowCount(row + 1);
        model_table->setItem(row,0,new QStandardItem(model.record(i).value(0).toString()));
        model_table->setItem(row,1,new QStandardItem(model.record(i).value(1).toString()));
        QSqlQueryModel type;
        type.clear();
        type.setQuery(QString("select vch_typename from t_m_member_type where ch_typeno = '%1' ").arg(model.record(i).value(2).toString()));
        model_table->setItem(row,2,new QStandardItem(type.record(0).value(0).toString()));
        model_table->setItem(row,3,new QStandardItem((model.record(i).value(4).toString())));
        model_table->setItem(row,4,new QStandardItem((model.record(i).value(3).toString())));
        model_table->setItem(row,5,new QStandardItem((model.record(i).value(5).toString())));
    }

    ui->tableView->selectRow(0);
    ui->tableView->setColumnWidth(0,100);
    ui->tableView->setColumnWidth(1,100);
    ui->tableView->setColumnWidth(2,100);
    ui->tableView->setColumnWidth(3,100);
    ui->tableView->setColumnWidth(4,100);
    ui->tableView->setColumnWidth(5,100);

    //    ui->tableView->horizontalHeader()->setResizeMode(0,QHeaderView::Fixed);
    //    ui->tableView->horizontalHeader()->setResizeMode(1,QHeaderView::Fixed);
    //    ui->tableView->horizontalHeader()->setResizeMode(2,QHeaderView::Fixed);
    //    ui->tableView->horizontalHeader()->setResizeMode(3,QHeaderView::Fixed);
    //    ui->tableView->horizontalHeader()->setResizeMode(4,QHeaderView::Fixed);
    //    ui->tableView->horizontalHeader()->setResizeMode(5,QHeaderView::Fixed);
}
