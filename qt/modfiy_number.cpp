#include "modfiy_number.h"
#include "ui_modfiy_number.h"
#include "lds_messagebox.h"
#include "backheader.h"

modfiy_number::modfiy_number(const QString & num,QWidget *parent) :
    yuan_num(num),QDialog(parent),
    ui(new Ui::modfiy_number)
{
    ui->setupUi(this);
    btn_t_f = false;
    ui->lineEdit->setText(yuan_num);
    ui->lineEdit->selectAll();
}

modfiy_number::~modfiy_number()
{
    delete ui;
}

void modfiy_number::on_pushButton_clicked()//1
{
    ui->lineEdit->insert("1");
}

void modfiy_number::on_pushButton_2_clicked()//2
{
    ui->lineEdit->insert("2");
}

void modfiy_number::on_pushButton_3_clicked()//3
{
    ui->lineEdit->insert("3");
}

void modfiy_number::on_pushButton_4_clicked()//4
{
    ui->lineEdit->insert("4");
}

void modfiy_number::on_pushButton_8_clicked()//5
{
    ui->lineEdit->insert("5");
}

void modfiy_number::on_pushButton_6_clicked()//6
{
    ui->lineEdit->insert("6");
}

void modfiy_number::on_pushButton_5_clicked()//7
{
    ui->lineEdit->insert("7");
}

void modfiy_number::on_pushButton_7_clicked()//8
{
    ui->lineEdit->insert("8");
}

void modfiy_number::on_pushButton_12_clicked()//9
{
    ui->lineEdit->insert("9");
}

void modfiy_number::on_pushButton_10_clicked()//0
{
    ui->lineEdit->insert("0");
}

void modfiy_number::on_pushButton_9_clicked()//00
{
    ui->lineEdit->insert(tr("00"));
}

void modfiy_number::on_pushButton_13_clicked()//-
{
    ui->lineEdit->insert(tr("-"));
}

void modfiy_number::on_pushButton_14_clicked()//d
{
    ui->lineEdit->backspace();
}

void modfiy_number::on_pushButton_15_clicked()//t
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
        lds_messagebox::warning(this, tr("消息提示"),tr("菜品数量必须大于0!"));
        
        return;
    }
    modfiy_num = ui->lineEdit->text().toFloat();
    btn_t_f = true;
    this->close();
}

void modfiy_number::on_pushButton_16_clicked()//f
{
    btn_t_f = false;
    this->close();
}

void modfiy_number::on_pushButton_11_clicked()//.
{
    ui->lineEdit->insert(tr("."));
}
