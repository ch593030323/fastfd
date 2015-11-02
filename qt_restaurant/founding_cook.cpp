#include "founding_cook.h"
#include "ui_founding_cook.h"
#include "operid_view.h"
#include "table_order_2.h"
#include "backheader.h"

founding_cook::founding_cook(const QString &danhao,const QString &zhuohao,const QString &renshu,const QString &kaitairen,const QString &time,const QString & str,QWidget *parent) :
    _danhao(danhao),_zhuohao(zhuohao),_renshu(renshu),_kaitairen(kaitairen),_time(time),_str(str),QDialog(parent),
    ui(new Ui::founding_cook)
{
    ui->setupUi(this);
    //this->setWindowFlags(Qt::FramelessWindowHint|Qt::Dialog);this->setStyleSheet("QDialog{border:2px solid gray}");
    ui->lineEdit->setEnabled(false);
    ui->lineEdit->setText(_danhao);
    ui->lineEdit_2->setEnabled(false);
    ui->lineEdit_2->setText(_zhuohao);
    ui->lineEdit_3->setText(_renshu);
    ui->lineEdit_4->setEnabled(false);
    ui->lineEdit_6->setEnabled(false);
    ui->lineEdit_6->setText(_kaitairen);
    ui->lineEdit_7->setEnabled(false);
    ui->lineEdit_7->setText(_time);

    if(_str == "2")
    {
        ui->pushButton_6->hide();
        ui->pushButton_13->hide();
        ui->lineEdit_4->setText(table_order_2::k_fuwuyuan);
        ui->lineEdit_5->setText(table_order_2::k_beizhu);
    }
}

founding_cook::~founding_cook()
{
    delete ui;
}

void founding_cook::on_pushButton_14_clicked()//取消
{
    wangluo = false;
    dianccai = false;
    btn_t_f = false;
    this->close();
}

void founding_cook::on_pushButton_9_clicked()//选择
{
    operid_view oper(this);
    /*oper.exec();*/backheader::static_dialogexec(oper, tr("操作员"));
    if(oper.btn_t_f == true)
    {
        ui->lineEdit_4->setText(oper.operid);
    }
}

void founding_cook::on_pushButton_6_clicked()//网络订单
{
    songcanren = ui->lineEdit_4->text();
    beizhu = ui->lineEdit_5->text();
    wangluo = true;
    dianccai = false;
    btn_t_f = false;
    this->close();
}

void founding_cook::on_pushButton_13_clicked()//点菜
{
    _renshu = ui->lineEdit_3->text();
    songcanren = ui->lineEdit_4->text();
    beizhu = ui->lineEdit_5->text();
    wangluo = false;
    dianccai = true;
    btn_t_f = false;
    this->close();
}

void founding_cook::on_pushButton_7_clicked()//确定
{
    _renshu = ui->lineEdit_3->text();
    songcanren = ui->lineEdit_4->text();
    beizhu = ui->lineEdit_5->text();
    wangluo = false;
    dianccai = false;
    btn_t_f = true;
    this->close();
}
