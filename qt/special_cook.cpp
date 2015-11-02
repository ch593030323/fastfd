#include "special_cook.h"
#include "ui_special_cook.h"
#include <QSqlRecord>
#include <QScrollBar>
#include <QDebug>

special_cook::special_cook(const QString & str,const QString & neirong,const QString & price,QWidget *parent) :
    zuofa(str),_neirong(neirong),_price(price),QDialog(parent),
    ui(new Ui::special_cook)
{
    ui->setupUi(this);
    //this->setWindowFlags(Qt::FramelessWindowHint|Qt::Dialog);this->setStyleSheet("QDialog{border:2px solid gray}");
    btn_t_f = false;
    ui->lineEdit->setFocus();
    ui->lineEdit->setText(_neirong);
    init_data();
    if(!_neirong.isEmpty())
    {
        list = _neirong.split(";");
    }
   add_price = _price.toFloat();
}

special_cook::~special_cook()
{
    delete ui;
}

void special_cook::init_data()
{
    model = new QSqlQueryModel;
    QGridLayout *layout = new QGridLayout();

    model->setQuery(QString("SELECT a.ch_no,a.vch_cp_memo,a.ch_typeno,a.num_add_price FROM cey_bt_cp_memo a,cey_bt_cp_dish b WHERE(a.ch_no = b.ch_no)and( b.ch_dishno = '%1')").arg(zuofa));
    for(int i = 0; i < model->rowCount(); i++)
    {
        QString str_index;
        str_index =  model->record(i).value(1).toString() + "\n" + QString().sprintf("%0.2f",model->record(i).value(3).toFloat());
        list_info =  new lsit_label_info(i/5,i%5,str_index,
                                         "background-color:rgb(90,90,90);color:white;",
                                         "background-color:rgb(255,0,0);color:white;",
                                         false);
        layout->addWidget(list_info,i/5,i%5,1,1,Qt::AlignTop);
        connect(list_info,SIGNAL(toType(int)),this,SLOT(show_label_num(int)));
    }

    layout->setAlignment(Qt::AlignTop);
    layout->setSpacing(10);
    layout->setContentsMargins(5,5,5,5);
    ui->scrollAreaWidgetContents_2->setLayout(layout);
    int count = model->rowCount();
    if(count > 15 && count%5 == 0)
    {
        ui->scrollAreaWidgetContents_2->resize(ui->scrollAreaWidgetContents_2->width(),count/5 * 90 + count/5 * 2);
    }
    else if(count > 15 && count%5 != 0)
    {
        ui->scrollAreaWidgetContents_2->resize( ui->scrollAreaWidgetContents_2->width(),count/5 * 90 + count/5 * 2 + 95);
    }
}

void special_cook::show_label_num(int num)
{
    if(!list.contains(model->record(num).value(1).toString().split("\n").last()))
    {
        list << model->record(num).value(1).toString().split("\n").last();
        add_price = add_price + model->record(num).value(3).toFloat();
    }
    else
    {
        list.removeOne(model->record(num).value(1).toString().split("\n").last());
        add_price = add_price - model->record(num).value(3).toFloat();
    }
    ui->lineEdit->setText(list.join(";"));
}

void special_cook::on_pushButton_clicked()//上一页
{
    ui->scrollArea->verticalScrollBar()->setValue(ui->scrollArea->verticalScrollBar()->value() - ui->scrollArea->height());
}

void special_cook::on_pushButton_2_clicked()//下一页
{
    ui->scrollArea->verticalScrollBar()->setValue(ui->scrollArea->verticalScrollBar()->value() + ui->scrollArea->height());
}

void special_cook::on_pushButton_3_clicked()//所有做法
{
    lsit_label_info::flag = QPoint(0,0);
    lsit_label_info::lable1 = NULL;
    qDeleteAll(ui->scrollAreaWidgetContents_2->children());
    QGridLayout *layout = new QGridLayout();
    model->clear();
    model->setQuery(QString("select * from cey_bt_cp_memo"));
    for(int i = 0; i < model->rowCount(); i++)
    {
        QString str_index;
        str_index =  model->record(i).value(1).toString() + "\n" + QString().sprintf("%0.2f",model->record(i).value(3).toFloat());
        list_info =  new lsit_label_info(i/5,i%5,str_index,"background-color:rgb(90,90,90);color:white;","background-color:rgb(255,0,0);color:white;");
        layout->addWidget(list_info,i/5,i%5,1,1,Qt::AlignTop);
        connect(list_info,SIGNAL(toType(int)),this,SLOT(show_label_num(int)));
    }

    layout->setAlignment(Qt::AlignTop);
    layout->setSpacing(10);
    layout->setContentsMargins(5,5,5,5);
    ui->scrollAreaWidgetContents_2->setLayout(layout);
    int count = model->rowCount();
    if(count > 15 && count%5 == 0)
    {
        ui->scrollAreaWidgetContents_2->resize(ui->scrollAreaWidgetContents_2->width(),count/5 * 90 + count/5 * 2);
    }
    else if(count > 15 && count%5 != 0)
    {
        ui->scrollAreaWidgetContents_2->resize( ui->scrollAreaWidgetContents_2->width(),count/5 * 90 + count/5 * 2 + 95);
    }
}

void special_cook::on_pushButton_4_clicked()//确定
{
    zuofaneirong = ui->lineEdit->text();
    btn_t_f = true;
    this->close();
}


void special_cook::on_pushButton_5_clicked()//取消
{
    btn_t_f = false;
    this->close();
}
