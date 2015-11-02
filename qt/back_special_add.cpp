#include "back_special_add.h"
#include "ui_back_special_add.h"
#include "lds_tableview_delegate_check.h"
#include "lds_tableview_repecify.h"
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QDebug>

back_special_add::back_special_add(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::back_special_add)
{
    ui->setupUi(this);
    //this->setWindowFlags(Qt::FramelessWindowHint|Qt::Dialog);this->setStyleSheet("QDialog{border:2px solid gray}");
    init_control();
    btn_t_f = false;
}

back_special_add::~back_special_add()
{
    delete ui;
}

void back_special_add::init_control()
{
    QSqlQueryModel type;
    type.setQuery(QString("select * from cey_bt_dish_type"));
    QPixmap pixmap(1,50);
    pixmap.fill(Qt::transparent);
    QIcon icon(pixmap);

    QStringList strlist;
    strlist <<"       ";
    for(int i = 0; i < type.rowCount(); i++)
    {
        strlist << type.record(i).value(1).toString();
    }

    for(int i = 0; i < strlist.count(); i++)
    {
        ui->comboBox->addItem(icon, strlist[i]);
    }
    ui->comboBox->setCurrentIndex(0);

//    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置属性为只读
    QStringList labels;
    labels<<tr("是否选中")<<tr("行号")<<tr("品码")<<tr("品名")<<tr("拼音简码")<<tr("品类")<<tr("单位")<<tr("价格");
    table_model = new QStandardItemModel();
    table_model->setHorizontalHeaderLabels(labels);
    //    ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::SelectedClicked);ui->tableView->setModel(table_model);


    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);ui->tableView->setModel(table_model);
    lds_tableview_delegate_check *delegate=new lds_tableview_delegate_check(QVector<int>() << 0 ,ui->tableView);
    delegate->setClickable();
    ui->tableView->setItemDelegate(delegate) ;


    QSqlQueryModel model;
    model.setQuery(QString("select distinct b.ch_dishno,b.vch_dishname,b.vch_spell,IFNULL((select c.vch_dish_typename from cey_bt_dish_type c where b.ch_dish_typeno=c.ch_dish_typeno),b.ch_dish_typeno),IFNULL((select d.vch_unitname from cey_bt_unit d where d.ch_unitno=b.ch_unitno),b.ch_unitno),b.num_price from cey_bt_dish b where b.ch_stopflag = 'N'"));
    for(int i = 0; i < model.rowCount(); i++)
    {
        int row = table_model->rowCount();
        table_model->setRowCount(row + 1);

        table_model->setItem(row,0,new QStandardItem("N")); //是否选中
        table_model->setItem(row,1,new QStandardItem(QString::number(row+1))); //序号
        table_model->setItem(row,2,new QStandardItem(model.record(i).value(0).toString()));//品码
        table_model->setItem(row,3,new QStandardItem(model.record(i).value(1).toString()));
        table_model->setItem(row,4,new QStandardItem(model.record(i).value(2).toString()));
        table_model->setItem(row,5,new QStandardItem(model.record(i).value(3).toString()));
        table_model->setItem(row,6,new QStandardItem(model.record(i).value(4).toString()));
        table_model->setItem(row,7,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(5).toFloat())));
    }

    QFont font;
    font.setPointSize(14);


    if(table_model->rowCount() > 0)
    {
        ui->tableView->selectRow(0);
    }
}

void back_special_add::on_pushButton_14_clicked()
{
    btn_t_f = true;
    this->close();
}

void back_special_add::on_pushButton_15_clicked()//全选/全消
{
    if(ui->pushButton_15->text() == tr("全选"))
    {
        for(int i = 0; i < table_model->rowCount(); i++)
        {
            table_model->item(i,0)->setText(tr("Y"));
        }
        ui->pushButton_15->setText(tr("全消"));
    }
    else
    {
        for(int i = 0; i < table_model->rowCount(); i++)
        {
            table_model->item(i,0)->setText("N");
        }
        ui->pushButton_15->setText(tr("全选"));
    }
}


void back_special_add::on_pushButton_10_clicked()//检索
{
    QString bianma =  tr("%") + ui->lineEdit->text() +  tr("%");
    QString pyjm =  tr("%") + ui->lineEdit_2->text() +  tr("%");

    QString pinlei = ui->comboBox->currentText().trimmed();
    QSqlQueryModel type;
    type.setQuery(QString("select ch_dish_typeno from cey_bt_dish_type where vch_dish_typename = '%1' " ).arg(pinlei));
    QString caipin_type =  tr("%") + type.record(0).value(0).toString().trimmed() +  tr("%");
    QString pinming = tr("%") + ui->lineEdit_3->text() +  tr("%");
    table_model->clear();

    QStringList labels;
    labels<<tr("是否选中")<<tr("行号")<<tr("品码")<<tr("品名")<<tr("拼音简码")<<tr("品类")<<tr("单位")<<tr("价格");
    table_model = new QStandardItemModel();
    table_model->setHorizontalHeaderLabels(labels);
    //    ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::SelectedClicked);ui->tableView->setModel(table_model);


    ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::SelectedClicked);ui->tableView->setModel(table_model);
    //    lds_tableview_delegate_check *delegate=new lds_tableview_delegate_check(QVector<int>() << 0 ,ui->tableView);
    //    delegate->setClickable();
    //    ui->tableView->setItemDelegate(delegate) ;

    QSqlQueryModel model;
    model.setQuery(QString("select distinct b.ch_dishno,b.vch_dishname,b.vch_spell,IFNULL((select c.vch_dish_typename from cey_bt_dish_type c where b.ch_dish_typeno=c.ch_dish_typeno),b.ch_dish_typeno),IFNULL((select d.vch_unitname from cey_bt_unit d where d.ch_unitno=b.ch_unitno),b.ch_unitno),b.num_price from cey_bt_dish b where  b.ch_dishno like '%1' and b.vch_spell like'%2' and b.vch_dishname like '%3' and b.ch_dish_typeno like'%4' and b.ch_stopflag='N'").arg(bianma).arg(pyjm).arg(pinming).arg(caipin_type));
    //qDebug() << QString("select distinct b.ch_dishno,b.vch_dishname,b.vch_spell,IFNULL((select c.vch_dish_typename from cey_bt_dish_type c where b.ch_dish_typeno=c.ch_dish_typeno),b.ch_dish_typeno),IFNULL((select d.vch_unitname from cey_bt_unit d where d.ch_unitno=b.ch_unitno),b.ch_unitno),b.num_price from cey_bt_dish  b.ch_dishno like '%1' and b.vch_spell like'%2' and b.vch_dishname like '%3' and b.ch_dish_typeno like'%4' and b.ch_stopflag='N'").arg(bianma).arg(pyjm).arg(pinming).arg(caipin_type);
    for(int i = 0; i < model.rowCount(); i++)
    {
        int row = table_model->rowCount();
        table_model->setRowCount(row + 1);

        table_model->setItem(row,0,new QStandardItem("N")); //是否选中
        table_model->setItem(row,1,new QStandardItem(QString::number(row+1))); //序号
        table_model->setItem(row,2,new QStandardItem(model.record(i).value(0).toString()));//品码
        table_model->setItem(row,3,new QStandardItem(model.record(i).value(1).toString()));
        table_model->setItem(row,4,new QStandardItem(model.record(i).value(2).toString()));
        table_model->setItem(row,5,new QStandardItem(model.record(i).value(3).toString()));
        table_model->setItem(row,6,new QStandardItem(model.record(i).value(4).toString()));
        table_model->setItem(row,7,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(5).toFloat())));
    }

    QFont font;
    font.setPointSize(14);


    if(table_model->rowCount() > 0)
    {
        ui->tableView->selectRow(0);
    }
}

void back_special_add::on_pushButton_11_clicked()//重置
{
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->comboBox->setCurrentIndex(0);
}

void back_special_add::on_pushButton_12_clicked()//应用
{
    for(int i = 0; i < table_model->rowCount(); i++)
    {
        if(table_model->item(i,0)->text() == tr("Y"))
        {
            if(!yingyong.contains(table_model->item(i,2)->text()))
            {
                yingyong << table_model->item(i,2)->text();
            }
        }
    }
}

void back_special_add::on_pushButton_13_clicked()//确定
{
    btn_t_f = true;
    for(int i = 0; i < table_model->rowCount(); i++)
    {
        if(table_model->item(i,0)->text() == tr("Y"))
        {
            if(!truelist.contains(table_model->item(i,2)->text()))
            {
                truelist << table_model->item(i,2)->text();
            }
        }
    }
    this->close();
}
