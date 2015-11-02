#include "bianma_cook.h"
#include "ui_bianma_cook.h"

bianma_cook::bianma_cook(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::bianma_cook)
{
    ui->setupUi(this);
    //    //this->setWindowFlags(Qt::FramelessWindowHint|Qt::Dialog);this->setStyleSheet("QDialog{border:2px solid gray}");
}

bianma_cook::~bianma_cook()
{
    delete ui;
}

void bianma_cook::on_pushButton_clicked()//Q
{
    ui->lineEdit->insert("Q");
}

void bianma_cook::on_pushButton_2_clicked()//W
{
    ui->lineEdit->insert(tr("W"));
}

void bianma_cook::on_pushButton_3_clicked()//E
{
    ui->lineEdit->insert(tr("E"));
}

void bianma_cook::on_pushButton_4_clicked()//R
{
    ui->lineEdit->insert(tr("R"));
}

void bianma_cook::on_pushButton_5_clicked()//T
{
    ui->lineEdit->insert(tr("T"));
}

void bianma_cook::on_pushButton_6_clicked()//Y
{
    ui->lineEdit->insert(tr("Y"));
}

void bianma_cook::on_pushButton_7_clicked()//U
{
    ui->lineEdit->insert(tr("U"));
}

void bianma_cook::on_pushButton_8_clicked()//I
{
    ui->lineEdit->insert(tr("I"));
}

void bianma_cook::on_pushButton_9_clicked()//O
{
    ui->lineEdit->insert(tr("O"));
}

void bianma_cook::on_pushButton_28_clicked()//P
{
    ui->lineEdit->insert("P");
}

void bianma_cook::on_pushButton_15_clicked()//A
{
    ui->lineEdit->insert(tr("A"));
}

void bianma_cook::on_pushButton_13_clicked()//S
{
    ui->lineEdit->insert(tr("S"));
}

void bianma_cook::on_pushButton_10_clicked()//D
{
    ui->lineEdit->insert(tr("D"));
}

void bianma_cook::on_pushButton_17_clicked()//F
{
    ui->lineEdit->insert(tr("F"));
}

void bianma_cook::on_pushButton_11_clicked()//G
{
    ui->lineEdit->insert(tr("G"));
}

void bianma_cook::on_pushButton_14_clicked()//H
{
    ui->lineEdit->insert(tr("H"));
}

void bianma_cook::on_pushButton_12_clicked()//J
{
    ui->lineEdit->insert(tr("J"));
}

void bianma_cook::on_pushButton_16_clicked()//K
{
    ui->lineEdit->insert(tr("K"));
}

void bianma_cook::on_pushButton_18_clicked()//L
{
    ui->lineEdit->insert(tr("L"));
}

void bianma_cook::on_pushButton_29_clicked()//.
{
    ui->lineEdit->insert(tr("."));
}

void bianma_cook::on_pushButton_24_clicked()//Z
{
    ui->lineEdit->insert(tr("Z"));
}

void bianma_cook::on_pushButton_22_clicked()//X
{
    ui->lineEdit->insert(tr("X"));
}

void bianma_cook::on_pushButton_19_clicked()//C
{
    ui->lineEdit->insert(tr("C"));
}

void bianma_cook::on_pushButton_26_clicked()//V
{
    ui->lineEdit->insert(tr("V"));
}

void bianma_cook::on_pushButton_20_clicked()//B
{
    ui->lineEdit->insert(tr("B"));
}

void bianma_cook::on_pushButton_23_clicked()//N
{
    ui->lineEdit->insert("N");
}

void bianma_cook::on_pushButton_21_clicked()//M
{
    ui->lineEdit->insert(tr("M"));
}

void bianma_cook::on_pushButton_25_clicked()//DELETE
{
    ui->lineEdit->backspace();
}

void bianma_cook::on_pushButton_27_clicked()
{
    ui->lineEdit->clear();
}

void bianma_cook::on_pushButton_30_clicked()//0
{
    ui->lineEdit->insert("0");
}

void bianma_cook::on_pushButton_31_clicked()//1
{
    ui->lineEdit->insert("1");
}

void bianma_cook::on_pushButton_32_clicked()//2
{
    ui->lineEdit->insert("2");
}

void bianma_cook::on_pushButton_33_clicked()//3
{
    ui->lineEdit->insert("3");
}

void bianma_cook::on_pushButton_34_clicked()//4
{
    ui->lineEdit->insert("4");
}

void bianma_cook::on_pushButton_35_clicked()//5
{
    ui->lineEdit->insert("5");
}

void bianma_cook::on_pushButton_36_clicked()//6
{
    ui->lineEdit->insert("6");
}

void bianma_cook::on_pushButton_37_clicked()//7
{
    ui->lineEdit->insert("7");
}

void bianma_cook::on_pushButton_38_clicked()//8
{
    ui->lineEdit->insert("8");
}

void bianma_cook::on_pushButton_39_clicked()//9
{
    ui->lineEdit->insert("9");
}


void bianma_cook::on_lineEdit_textChanged(const QString &arg1)
{
    emit edit_text(arg1);
}

void bianma_cook::on_pushButton_enter_clicked()
{
    emit edit_text("enter");
}
