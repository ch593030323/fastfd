#include "other_pay_view.h"
#include "ui_other_pay_view.h"
#include <QSqlQueryModel>
#include <QSqlRecord>

other_pay_view::other_pay_view(const QString& str,QWidget *parent) :
    type(str),QDialog(parent),
    ui(new Ui::other_pay_view)
{
    ui->setupUi(this);
    //this->setWindowFlags(Qt::FramelessWindowHint|Qt::Dialog);this->setStyleSheet("QDialog{border:2px solid gray}");
    init_control();
    btn_t_f = false;
    curindex_id;
}

other_pay_view::~other_pay_view()
{
    delete ui;
}

void other_pay_view::init_control()
{
    QStringList items;
    QSqlQueryModel model;
    model.setQuery(QString("select * from cey_bt_paymode"));
    for(int i = 0;i < model.rowCount(); i++)
    {
        QString str = model.record(i).value(0).toString().trimmed();
        if(type == "1")
        {
            if(str != "00" && str != "02" && str != "03" && str != "04" && str != "05" && str != "XX" && str != "YY" && str != "ZZ" )
            {
                items.append(model.record(i).value(1).toString());
                payname << model.record(i).value(1).toString().trimmed();
                payfanshi << str;
            }
        }
        else
        {
            if(str != "00" && str != "02" && str != "03" && str != "04" && str != "05" && str != "XX" && str != "YY" && str != "ZZ" )
            {
                items.append(model.record(i).value(1).toString());
                payname << model.record(i).value(1).toString().trimmed();
                payfanshi << str;
            }
        }
    }
    ui->comboBox->clear();
    ui->comboBox->addItems(items);
}

void other_pay_view::on_pushButton_4_clicked()//取定
{
    curindex_id = ui->comboBox->currentIndex();
    btn_t_f = true;
    this->close();
}

void other_pay_view::on_pushButton_5_clicked()//取消
{
    btn_t_f = false;
    this->close();
}

