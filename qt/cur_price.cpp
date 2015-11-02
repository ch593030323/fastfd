#include "cur_price.h"
#include "ui_cur_price.h"
#include "lds_messagebox.h"
#include <QDebug>
#include "backheader.h"

cur_price::cur_price(const QString & str,QWidget *parent) :
    linetext(str),QDialog(parent),
    ui(new Ui::cur_price)
{
    ui->setupUi(this);
    //this->setWindowFlags(Qt::FramelessWindowHint|Qt::Dialog);this->setStyleSheet("QDialog{border:2px solid gray}");
    btn_t_f = false;
    ui->lineEdit->setText(linetext);
    ui->lineEdit->setFocus();
}

cur_price::~cur_price()
{
    delete ui;
}

void cur_price::on_pushButton_clicked()//1
{
    ui->lineEdit->insert("1");
}

void cur_price::on_pushButton_2_clicked()//2
{
    ui->lineEdit->insert("2");
}

void cur_price::on_pushButton_3_clicked()//3
{
    ui->lineEdit->insert("3");
}

void cur_price::on_pushButton_4_clicked()//4
{
    ui->lineEdit->insert("4");
}

void cur_price::on_pushButton_8_clicked()//5
{
    ui->lineEdit->insert("5");
}

void cur_price::on_pushButton_6_clicked()//6
{
    ui->lineEdit->insert("6");
}

void cur_price::on_pushButton_5_clicked()//7
{
    ui->lineEdit->insert("7");
}

void cur_price::on_pushButton_7_clicked()//8
{
    ui->lineEdit->insert("8");
}

void cur_price::on_pushButton_12_clicked()//9
{
    ui->lineEdit->insert("9");
}

void cur_price::on_pushButton_10_clicked()//0
{
    ui->lineEdit->insert("0");
}

void cur_price::on_pushButton_9_clicked()//00
{
    ui->lineEdit->insert("00");
}

void cur_price::on_pushButton_11_clicked()//.
{
    ui->lineEdit->insert(".");
}

void cur_price::on_pushButton_13_clicked()//-
{
    ui->lineEdit->insert("-");
}

void cur_price::on_pushButton_14_clicked()//d
{
    ui->lineEdit->backspace();
}

void cur_price::on_pushButton_15_clicked()//确定
{
    int i = ui->lineEdit->text().count(".");  //返回查找相同字符出现的次数
    int k = ui->lineEdit->text().count("-");
    ;
    if(ui->lineEdit->text().isEmpty() || i > 1 || k > 1)
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("修改数据格式有误,如23!"));

        return;
    }
    if(k == 1 && ui->lineEdit->text().left(1) != "-")
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("修改数据格式有误,如23!"));

        return;
    }
    if(ui->lineEdit->text().toFloat() <= 0)
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("菜品价格必须大于0!"));

        return;
    }
    price = ui->lineEdit->text();
    btn_t_f = true;
    this->close();
}

void cur_price::on_pushButton_16_clicked()//取消
{
    btn_t_f = false;
    this->close();
}
