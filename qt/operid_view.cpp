#include "operid_view.h"
#include "ui_operid_view.h"
#include <QSqlRecord>
#include <QScrollBar>

operid_view::operid_view(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::operid_view)
{
    ui->setupUi(this);
    //this->setWindowFlags(Qt::FramelessWindowHint|Qt::Dialog);this->setStyleSheet("QDialog{border:2px solid gray}");
    btn_t_f = false;
    init_data();
}

operid_view::~operid_view()
{
    delete ui;
}

void operid_view::init_data()
{
    model = new QSqlQueryModel;
    QGridLayout *layout = new QGridLayout();

    model->setQuery(QString("select * from cey_sys_operator where ch_flag = 'Y' "));
    for(int i = 0; i < model->rowCount(); i++)
    {
        QString str_index;
        str_index =  model->record(i).value(0).toString() + "\n\n" + model->record(i).value(1).toString();
        list_info =  new lsit_label_info(i/5,i%5,str_index,"background-color:rgb(90,90,90);color:white;","background-color:rgb(255,0,0);color:white;");
        layout->addWidget(list_info,i/5,i%5,1,1,Qt::AlignTop);
        connect(list_info,SIGNAL(toType(int)),this,SLOT(show_label_num(int)));
    }

    layout->setAlignment(Qt::AlignTop);
    layout->setSpacing(10);
    layout->setContentsMargins(5,5,5,5);
    ui->scrollAreaWidgetContents->setLayout(layout);
    int count = model->rowCount();
    if(count > 15 && count%5 == 0)
    {
        ui->scrollAreaWidgetContents->resize(ui->scrollAreaWidgetContents->width(),count/5 * 90 + count/5 * 2);
    }
    else if(count > 15 && count%5 != 0)
    {
        ui->scrollAreaWidgetContents->resize( ui->scrollAreaWidgetContents->width(),count/5 * 90 + count/5 * 2 + 95);
    }
    operid = model->record(0).value(1).toString();
}

void operid_view::on_pushButton_11_clicked()//上一页
{
    ui->scrollArea_3->verticalScrollBar()->setValue(ui->scrollArea_3->verticalScrollBar()->value() - ui->scrollArea_3->height());
}

void operid_view::on_pushButton_12_clicked()//下一页
{
    ui->scrollArea_3->verticalScrollBar()->setValue(ui->scrollArea_3->verticalScrollBar()->value() + ui->scrollArea_3->height());
}

void operid_view::on_pushButton_13_clicked()//确定
{
    btn_t_f = true;
    this->close();
}

void operid_view::on_pushButton_14_clicked()//取消
{
    btn_t_f = false;
    this->close();
}

void operid_view::show_label_num(int num)
{
    operid = model->record(num).value(1).toString();
}
