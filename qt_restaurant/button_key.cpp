#include "button_key.h"
#include "ui_button_key.h"

button_key::button_key(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::button_key)
{
    ui->setupUi(this);
    //this->setWindowFlags(Qt::FramelessWindowHint|Qt::Dialog);this->setStyleSheet("QDialog{border:2px solid gray}");
    btn_t_f = false;
}

button_key::~button_key()
{
    delete ui;
}

void button_key::on_pushButton_clicked()//1
{
    emit btn1();
}

void button_key::on_pushButton_2_clicked()//2
{
    emit btn2();
}

void button_key::on_pushButton_3_clicked()//3
{
    emit btn3();
}

void button_key::on_pushButton_4_clicked()//4
{
    emit btn4();
}

void button_key::on_pushButton_5_clicked()//5
{
    emit btn5();
}

void button_key::on_pushButton_6_clicked()//6
{
    emit btn6();
}

void button_key::on_pushButton_7_clicked()//7
{
    emit btn7();
}

void button_key::on_pushButton_8_clicked()//8
{
    emit btn8();
}

void button_key::on_pushButton_9_clicked()//9
{
    emit btn9();
}

void button_key::on_pushButton_10_clicked()//0
{
    emit btn0();
}

void button_key::on_pushButton_11_clicked()//00
{
    emit btn00();
}

void button_key::on_pushButton_12_clicked()//t
{
    btn_t_f = true;
    this->close();
}

void button_key::on_pushButton_13_clicked()//d
{
    emit btnd();
}

void button_key::on_pushButton_14_clicked()//-
{
    emit btnxian();
}

void button_key::on_pushButton_15_clicked()//.
{
    emit btndian();
}

void button_key::on_pushButton_16_clicked()//f
{
    btn_t_f = false;
    emit quxiao();
    this->close();
}
