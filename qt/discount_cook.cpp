#include "discount_cook.h"
#include "ui_discount_cook.h"
#include "lds_messagebox.h"
#include "backheader.h"

discount_cook::discount_cook(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::discount_cook)
{
    ui->setupUi(this);
    //this->setWindowFlags(Qt::FramelessWindowHint|Qt::Dialog);this->setStyleSheet("QDialog{border:2px solid gray}");
    discount= 0.00;
    btn_t_f = false;
    ui->lineEdit->setFocus();
}

discount_cook::~discount_cook()
{
    delete ui;
}

void discount_cook::on_pushButton_clicked()//1
{
    ui->lineEdit->insert("1");
}

void discount_cook::on_pushButton_2_clicked()//2
{
    ui->lineEdit->insert("2");
}

void discount_cook::on_pushButton_3_clicked()//3
{
    ui->lineEdit->insert("3");
}

void discount_cook::on_pushButton_4_clicked()//4
{
    ui->lineEdit->insert("4");
}

void discount_cook::on_pushButton_8_clicked()//5
{
    ui->lineEdit->insert("5");
}

void discount_cook::on_pushButton_6_clicked()//6
{
    ui->lineEdit->insert("6");
}

void discount_cook::on_pushButton_5_clicked()//7
{
    ui->lineEdit->insert("7");
}

void discount_cook::on_pushButton_7_clicked()//8
{
    ui->lineEdit->insert("8");
}

void discount_cook::on_pushButton_12_clicked()//9
{
    ui->lineEdit->insert("9");
}

void discount_cook::on_pushButton_10_clicked()//0
{
    ui->lineEdit->insert("0");
}

void discount_cook::on_pushButton_9_clicked()//00
{
    ui->lineEdit->insert(tr("00"));
}

void discount_cook::on_pushButton_11_clicked()//.
{
    ui->lineEdit->insert(tr("."));
}

void discount_cook::on_pushButton_13_clicked()//-
{
    ui->lineEdit->insert(tr("-"));
}

void discount_cook::on_pushButton_14_clicked()//d
{
    ui->lineEdit->backspace();
}

void discount_cook::on_pushButton_15_clicked()//t
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
    if(ui->lineEdit->text().toFloat()  < 0  || ui->lineEdit->text().toFloat() > 100)
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("折扣值必须为0到100之间的数值,如90!"));
        
        return;
    }

    discount = ui->lineEdit->text().toFloat();
    btn_t_f = true;
    this->close();
}

void discount_cook::on_pushButton_16_clicked()//f
{
    btn_t_f = false;
    this->close();
}
