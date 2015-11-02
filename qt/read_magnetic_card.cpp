#include "read_magnetic_card.h"
#include "ui_read_magnetic_card.h"

read_magnetic_card::read_magnetic_card(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::read_magnetic_card)
{
    ui->setupUi(this);
    //this->setWindowFlags(Qt::FramelessWindowHint|Qt::Dialog);this->setStyleSheet("QDialog{border:2px solid gray}");
    ui->lineEdit->setFocus();
}

read_magnetic_card::~read_magnetic_card()
{
    delete ui;
}

void read_magnetic_card::on_pushButton_15_clicked()//取消
{
    this->close();
}

void read_magnetic_card::on_pushButton_clicked()//1
{
    ui->lineEdit->insert("1");
}

void read_magnetic_card::on_pushButton_2_clicked()//2
{
    ui->lineEdit->insert("2");
}

void read_magnetic_card::on_pushButton_3_clicked()//3
{
    ui->lineEdit->insert("3");
}

void read_magnetic_card::on_pushButton_6_clicked()//4
{
    ui->lineEdit->insert("4");
}

void read_magnetic_card::on_pushButton_5_clicked()//5
{
    ui->lineEdit->insert("5");
}

void read_magnetic_card::on_pushButton_8_clicked()//6
{
    ui->lineEdit->insert("6");
}

void read_magnetic_card::on_pushButton_4_clicked()//7
{
    ui->lineEdit->insert("7");
}

void read_magnetic_card::on_pushButton_7_clicked()//8
{
    ui->lineEdit->insert("8");
}

void read_magnetic_card::on_pushButton_10_clicked()//9
{
    ui->lineEdit->insert("9");
}

void read_magnetic_card::on_pushButton_12_clicked()//0
{
    ui->lineEdit->insert("0");
}

void read_magnetic_card::on_pushButton_9_clicked()//00
{
    ui->lineEdit->insert(tr("00"));
}

void read_magnetic_card::on_pushButton_11_clicked()//d
{
    ui->lineEdit->backspace();
}
