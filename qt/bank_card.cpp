#include "bank_card.h"
#include "ui_bank_card.h"
#include "lds_messagebox.h"
#include <QDateTime>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include"backheader.h"

bank_card::bank_card(const QString & type,const QString & num,QWidget *parent) :
    _type(type),_num(num),QDialog(parent),
    ui(new Ui::bank_card)
{
    ui->setupUi(this);
    //this->setWindowFlags(Qt::FramelessWindowHint|Qt::Dialog);this->setStyleSheet("QDialog{border:2px solid gray}");
    init_control();
    btn_t_f = false;
    shishoujine = 0.00;
    shifujine = 0.00;
    ui->lineEdit->setText(_type);
    ui->lineEdit_3->setText(_num);
    ui->lineEdit_4->setText(_num);
    ui->lineEdit_8->setText(_num);
}

bank_card::~bank_card()
{
    delete ui;
}

void bank_card::init_control()
{
    ui->lineEdit->setEnabled(false);
    ui->lineEdit_3->setEnabled(false);
    ui->lineEdit_4->setEnabled(false);
    ui->lineEdit_5->setEnabled(false);
    ui->lineEdit_6->setEnabled(false);
    ui->lineEdit_7->setEnabled(false);
    ui->lineEdit_8->setFocus();
}

void bank_card::on_pushButton_4_clicked()//确定
{
    ;
    int i = ui->lineEdit_8->text().count(".");  //返回查找相同字符出现的次数
    int k = ui->lineEdit_8->text().count("-");

    if(ui->lineEdit_8->text().isEmpty() || i > 1 || k > 1)
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("修改数据格式有误,如23!"));
        
        return;
    }
    if(k == 1 && ui->lineEdit_8->text().left(1) != "-")
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("修改数据格式有误,如23!"));
        
        return;
    }
    pingzhenghao = ui->lineEdit_2->text();
    shishoujine = ui->lineEdit_8->text().toFloat();
    shifujine = ui->lineEdit_4->text().toFloat();
    btn_t_f = true;
    QSqlQueryModel model1;
    model1.setQuery("Select NOW()");
    time = model1.record(0).value(0).toDateTime().toString("yyyy-MM-dd hh:mm:ss");
    this->close();
}

void bank_card::on_pushButton_5_clicked()//取消
{
    btn_t_f = false;
    this->close();
}

void bank_card::on_lineEdit_8_textChanged(const QString &arg1)
{
    ui->lineEdit_4->setText(ui->lineEdit_8->text());
}
