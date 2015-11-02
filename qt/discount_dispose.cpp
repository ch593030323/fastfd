#include "discount_dispose.h"
#include "ui_discount_dispose.h"
#include <QSqlQueryModel>
#include "n_func.h"
#include <QSqlRecord>
#include "lds_messagebox.h"
#include "lds_messagebox.h"
#include "power_oper.h"
#include <QDebug>
#include "backheader.h"


discount_dispose::discount_dispose(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::discount_dispose)
{
    ui->setupUi(this);
    //this->setWindowFlags(Qt::FramelessWindowHint|Qt::Dialog);this->setStyleSheet("QDialog{border:2px solid gray}");
    ui->lineEdit->setFocus();
    ui->lineEdit->setText("100");
    zhekouzhi = 0.00;
    btn_t_f = false;
    ui->lineEdit->setFocus();
    ui->lineEdit->selectAll();
}

discount_dispose::~discount_dispose()
{
    delete ui;
}

void discount_dispose::on_pushButton_13_clicked()//确定
{
    this->close();
    if(n_func::gs_operid != "0000")
    {
        QSqlQueryModel model;
        model.setQuery(QString("Select int_discount from cey_sys_operator where vch_operID ='%1' and ch_flag = 'Y' ").arg(n_func::gs_operid));
        int operdis = model.record(0).value(0).toInt();
        if(operdis > ui->lineEdit->text().toInt())
        {
            if(0==lds_messagebox::warning(this, tr("消息提示"),tr("指定操作员没有该折扣权限!"),tr("确认"),tr("取消")))
            {
                power_oper oper;
                /*oper.exec();*/backheader::static_dialogexec(oper, tr("权限核实"));
                if(oper.btn_t_f == true)
                {
                    if(oper.operid != "0000")
                    {
                        model.clear();
                        model.setQuery(QString("Select int_discount from cey_sys_operator where vch_operID ='%1' and ch_flag = 'Y' ").arg(oper.operid));
                        int zhe = model.record(0).value(0).toInt();
                        if(zhe > ui->lineEdit->text().toInt())
                        {
                            ;
                            lds_messagebox::warning(this, tr("消息提示"),tr("指定操作员没有该折扣权限!"));
                            
                            return;
                        }
                    }
                }
                else
                {
                    return;
                }
            }
            else
            {
                return;
            }
        }
    }
    zhekouzhi = ui->lineEdit->text().toFloat();
    btn_t_f = true;
}

void discount_dispose::on_pushButton_10_clicked()//取消
{
    btn_t_f = false;
    this->close();
}

void discount_dispose::on_pushButton_clicked()//1
{
    ui->lineEdit->insert("1");
}

void discount_dispose::on_pushButton_2_clicked()//2
{
    ui->lineEdit->insert("2");
}

void discount_dispose::on_pushButton_3_clicked()//3
{
    ui->lineEdit->insert("3");
}

void discount_dispose::on_pushButton_6_clicked()//4
{
    ui->lineEdit->insert("4");
}

void discount_dispose::on_pushButton_5_clicked()//5
{
    ui->lineEdit->insert("5");
}

void discount_dispose::on_pushButton_4_clicked()//6
{
    ui->lineEdit->insert("6");
}

void discount_dispose::on_pushButton_9_clicked()//7
{
    ui->lineEdit->insert("7");
}

void discount_dispose::on_pushButton_8_clicked()//8
{
    ui->lineEdit->insert("8");
}

void discount_dispose::on_pushButton_7_clicked()//9
{
    ui->lineEdit->insert("9");
}

void discount_dispose::on_pushButton_12_clicked()//0
{
    ui->lineEdit->insert("0");
}

void discount_dispose::on_pushButton_11_clicked()//d
{
    ui->lineEdit->backspace();
}

void discount_dispose::on_pushButton_14_clicked()
{
    ui->lineEdit->insert(".");
}
