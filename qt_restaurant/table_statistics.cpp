#include "table_statistics.h"
#include "ui_table_statistics.h"
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QDateTime>

table_statistics::table_statistics(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::table_statistics)
{
    ui->setupUi(this);
    //this->setWindowFlags(Qt::FramelessWindowHint|Qt::Dialog);this->setStyleSheet("QDialog{border:2px solid gray}");
    init_control();
}

table_statistics::~table_statistics()
{
    delete ui;
}

void table_statistics::init_control()
{
    QSqlQueryModel model;
    model.setQuery(QString("select NOW()"));
    QString str = model.record(0).value(0).toDateTime().toString("yyyy-MM-dd hh:mm:ss");
    ui->label_5->setText(tr("当前时间:") + str);

    QStringList labels;
    labels <<tr("餐桌类型")<<tr("台数")<<tr("空台")<<tr("在用")<<tr("当前消费金额");
    table_model = new QStandardItemModel();
    table_model->setHorizontalHeaderLabels(labels);
    //    ui->tableView->setModel(table_model);
    ui->tableView->setModel(table_model);

    model.clear();
    QSqlQueryModel num;
    model.setQuery(QString("SELECT b.vch_typename , count( a.ch_tableno), count( a.ch_billno)FROM cey_bt_table a,cey_bt_table_type b  where a.ch_typeno=b.ch_typeno group by b.vch_typename order by b.vch_typename"));
    for(int i = 0; i < model.rowCount(); i++)
    {
        int row = table_model->rowCount();
        table_model->setRowCount(row + 1);
        table_model->setItem(row,0,new QStandardItem(model.record(i).value(0).toString())); //餐桌类型
        table_model->setItem(row,1,new QStandardItem(model.record(i).value(1).toString())); //餐桌类型
        int kongtai = model.record(i).value(1).toInt() - model.record(i).value(2).toInt();
        table_model->setItem(row,2,new QStandardItem(QString::number(kongtai))); //空台
        table_model->setItem(row,3,new QStandardItem(QString::number(model.record(i).value(2).toInt()))); //在用
        num.clear();
        num.setQuery(QString("SELECT IFNULL( sum( num_cost), 0)FROM cey_u_table a , cey_bt_table b  where a.ch_billno =b.ch_billno and a.ch_tableno =b.ch_tableno and b.ch_typeno =(select ch_typeno from cey_bt_table_type c where c.vch_typename ='%1')").arg(model.record(i).value(0).toString()));
        table_model->setItem(row,4,new QStandardItem(QString().sprintf("%0.2f",num.record(0).value(0).toFloat()))); //餐桌类型
    }


    QFont font;
    font.setPointSize(14);
    float taishu = 0.00;
    float kong = 0.00;
    float zaiyong = 0.00;
    float jine = 0.00;
    for(int i = 0; i < table_model->rowCount();i++)
    {
        taishu += table_model->item(i,1)->text().toFloat();
        kong += table_model->item(i,2)->text().toFloat();
        zaiyong += table_model->item(i,3)->text().toFloat();
        jine += table_model->item(i,4)->text().toFloat();
    }

    QString str1 = tr("   合计:                     ") + QString().sprintf("%0.2f",taishu) + "                         " + QString().sprintf("%0.2f",kong) + "                         "+ QString().sprintf("%0.2f",zaiyong) + "                         "+ QString().sprintf("%0.2f",jine);
    ui->label_3->setText(str1);

    if(table_model->rowCount() > 0)
    {
        ui->tableView->selectRow(0);
    }

}

void table_statistics::on_pushButton_14_clicked()//退出
{
    this->close();
}

void table_statistics::on_pushButton_7_clicked()//打印
{

}

void table_statistics::on_radioButton_clicked()//餐桌类型统计
{
    ui->label_3->show();
    table_model->removeRows(0,table_model->rowCount());
    ui->tableView->setModel(table_model);

    QSqlQueryModel model;
    model.clear();
    QSqlQueryModel num;
    model.setQuery(QString("SELECT b.vch_typename , count( a.ch_tableno), count( a.ch_billno)FROM cey_bt_table a,cey_bt_table_type b  where a.ch_typeno=b.ch_typeno group by b.vch_typename order by b.vch_typename"));
    for(int i = 0; i < model.rowCount(); i++)
    {
        int row = table_model->rowCount();
        table_model->setRowCount(row + 1);
        table_model->setItem(row,0,new QStandardItem(model.record(i).value(0).toString())); //餐桌类型
        table_model->setItem(row,1,new QStandardItem(model.record(i).value(1).toString())); //餐桌类型
        int kongtai = model.record(i).value(1).toInt() - model.record(i).value(2).toInt();
        table_model->setItem(row,2,new QStandardItem(QString::number(kongtai))); //空台
        table_model->setItem(row,3,new QStandardItem(QString::number(model.record(i).value(2).toInt()))); //在用
        num.clear();
        num.setQuery(QString("SELECT IFNULL( sum( num_cost), 0)FROM cey_u_table a , cey_bt_table b  where a.ch_billno =b.ch_billno and a.ch_tableno =b.ch_tableno and b.ch_typeno =(select ch_typeno from cey_bt_table_type c where c.vch_typename ='%1')").arg(model.record(i).value(0).toString()));
        table_model->setItem(row,4,new QStandardItem(QString().sprintf("%0.2f",num.record(0).value(0).toFloat()))); //餐桌类型
    }

    QFont font;
    font.setPointSize(14);
    float taishu = 0.00;
    float kong = 0.00;
    float zaiyong = 0.00;
    float jine = 0.00;
    for(int i = 0; i < table_model->rowCount();i++)
    {

        taishu += table_model->item(i,1)->text().toFloat();
        kong += table_model->item(i,2)->text().toFloat();
        zaiyong += table_model->item(i,3)->text().toFloat();
        jine += table_model->item(i,4)->text().toFloat();
    }

    QString str = tr("   合计:                     ") + QString().sprintf("%0.2f",taishu) + "                         " + QString().sprintf("%0.2f",kong) + "                         "+ QString().sprintf("%0.2f",zaiyong) + "                         "+ QString().sprintf("%0.2f",jine);
    ui->label_3->setText(str);
    if(table_model->rowCount() > 0)
    {
        ui->tableView->selectRow(0);
    }
}

void table_statistics::on_radioButton_2_clicked()//餐桌明细统计
{
    ui->label_3->show();
    QStringList labels;
    labels <<tr("餐桌类型")<<tr("餐桌号")<<tr("餐桌名")<<tr("当前状态")<<tr("当前消费金额");
    table_model->clear();
    table_model->setHorizontalHeaderLabels(labels);
    //    ui->tableView->setModel(table_model);
    ui->tableView->setModel(table_model);

    QSqlQueryModel model;
    model.clear();
    QSqlQueryModel num;
    model.setQuery(QString("Select b.vch_typename,a.ch_tableno ,a.vch_tablename,a.ch_billno from cey_bt_table a,cey_bt_table_type b where a.ch_typeno=b.ch_typeno order by b.vch_typename , a.ch_tableno"));
    for(int i = 0; i < model.rowCount(); i++)
    {
        int row = table_model->rowCount();
        table_model->setRowCount(row + 1);
        table_model->setItem(row,0,new QStandardItem(model.record(i).value(0).toString())); //餐桌类型
        table_model->setItem(row,1,new QStandardItem(model.record(i).value(1).toString())); //餐桌号
        table_model->setItem(row,2,new QStandardItem(model.record(i).value(2).toString()));//餐桌名
        QString state;
        if(model.record(i).value(3).toString().isEmpty())
        {
            state = tr("空闲");
        }
        else
        {
            state = tr("在用");
        }
        table_model->setItem(row,3,new QStandardItem(state));//当前状态
        num.clear();
        num.setQuery(QString("Select IFNULL(num_cost , 0)from cey_u_table where ch_billno ='%1' and ch_tableno ='%2'").arg(model.record(i).value(3).toString()).arg(model.record(i).value(1).toString()));
        table_model->setItem(row,4,new QStandardItem(QString().sprintf("%0.2f",num.record(0).value(0).toFloat())));//餐桌名
    }

    QStringList type;
    QFont font;
    font.setPointSize(14);
    for(int i = 0; i < table_model->rowCount();i++)
    {
        if(!type.contains(table_model->item(i,0)->text()))
        {
            type << table_model->item(i,0)->text();
        }
    }


    QString label_text;
    for(int i = 0; i < type.count(); i++)
    {
        int taishu = 0;
        int kontai = 0;
        int zaiyong = 0;
        float jine = 0.00;
        QString str1 = tr("   ") + type[i] + "     台数:";
        for(int k = 0; k < table_model->rowCount(); k++)
        {
            if(table_model->item(k,0)->text() == type[i])
            {
                taishu += 1;
                if(table_model->item(k,3)->text() == tr("空闲"))
                {
                    kontai += 1;
                }
                else
                {
                    zaiyong += 1;
                }
                jine += table_model->item(k,4)->text().toFloat();
            }
        }

        QString str2;
        str2 = QString::number(taishu);
        if(str2.length() < 3)
        {
            int m = 8;
            int h = m - str2.length();
            for(int a = 0; a < h; a++)
            {
                str2.append(" ");
            }
        }
        str1.append(str2);
        str1.append(tr("空台:"));

        QString str3;
        str3 = QString::number(kontai);
        if(str3.length() < 3)
        {
            int m = 8;
            int h = m - str3.length();
            for(int a = 0; a < h; a++)
            {
                str3.append(" ");
            }
        }
        str1.append(str3);
        str1.append(tr("在用:"));

        QString str4;
        str4 = QString::number(zaiyong);
        if(str4.length() < 3)
        {
            int m = 8;
            int h = m - str4.length();
            for(int a = 0; a < h; a++)
            {
                str4.append(" ");
            }
        }
        str1.append(str4);
        str1.append(tr("当前消费金额:"));

        QString str5 = QString().sprintf("%0.2f",jine);
        str1.append(str5);
        if(i < type.count() - 1)
        {
            str1.append("\n");
        }

        label_text.append(str1);
    }
    ui->label_3->setText(label_text);

    if(table_model->rowCount() > 0)
    {
        ui->tableView->selectRow(0);
    }
}

void table_statistics::on_radioButton_3_clicked()//消费明细统计
{
    ui->label_3->hide();
    QStringList labels;
    labels <<tr("餐桌类型")<<tr("餐桌号")<<tr("餐桌名")<<tr("菜名")<<tr("数量")<<tr("金额");
    table_model->clear();
    table_model->setHorizontalHeaderLabels(labels);
    //    ui->tableView->setModel(table_model);
    ui->tableView->setModel(table_model);

    QSqlQueryModel model;
    model.clear();

    model.setQuery(QString("Select d.vch_typename , a.ch_tableno , a.vch_tablename , c.vch_dishname , sum( b.num_num - b.num_back), sum( case when b.ch_presentflag ='Y' then 0 else(( b.num_num - b.num_back)*( b.num_price + IFNULL( b.num_price_add , 0)))*b.int_discount *0.01 end)from cey_bt_table a , cey_u_orderdish b , cey_bt_dish c,cey_bt_table_type d where a.ch_billno =b.ch_billno and a.ch_tableno =b.ch_tableno and b.ch_dishno =c.ch_dishno and d.ch_typeno=a.ch_typeno Group by d.vch_typename , a.ch_tableno,a.ch_typeno , a.vch_tablename , c.vch_dishname having sum( b.num_num - b.num_back)<> 0 order by d.vch_typename , a.ch_tableno, a.vch_tablename , c.vch_dishname"));
    for(int i = 0; i < model.rowCount(); i++)
    {
        int row = table_model->rowCount();
        table_model->setRowCount(row + 1);
        table_model->setItem(row,0,new QStandardItem(model.record(i).value(0).toString())); //餐桌类型
        table_model->setItem(row,1,new QStandardItem(model.record(i).value(1).toString())); //餐桌号
        table_model->setItem(row,2,new QStandardItem(model.record(i).value(2).toString()));//餐桌名
        table_model->setItem(row,3,new QStandardItem(model.record(i).value(3).toString()));//菜名
        table_model->setItem(row,4,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(4).toFloat())));//数量
        table_model->setItem(row,5,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(5).toFloat())));//金额
    }

    //

    if(table_model->rowCount() > 0)
    {
        ui->tableView->selectRow(0);
    }
}
