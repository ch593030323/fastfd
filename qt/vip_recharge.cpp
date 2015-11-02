#include "vip_recharge.h"
#include "ui_vip_recharge.h"
#include "read_magnetic_card.h"
#include "backheader.h"

vip_recharge::vip_recharge(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::vip_recharge)
{
    ui->setupUi(this);
    //this->setWindowFlags(Qt::FramelessWindowHint|Qt::Dialog);this->setStyleSheet("QDialog{border:2px solid gray}");
    init_control();
}

vip_recharge::~vip_recharge()
{
    delete ui;
}

void vip_recharge::init_control()
{
    ui->lineEdit->setEnabled(false);
    ui->lineEdit_2->setEnabled(false);
    ui->lineEdit_3->setEnabled(false);
    ui->lineEdit_4->setEnabled(false);
    ui->lineEdit_5->setEnabled(false);
    ui->lineEdit_6->setEnabled(false);
    ui->lineEdit_7->setEnabled(false);
    ui->lineEdit_8->setEnabled(false);
    ui->lineEdit_9->setEnabled(false);
    ui->comboBox->setEnabled(false);
    ui->checkBox->setChecked(true);

    ui->comboBox->addItems(QStringList() << tr("现金") << tr("银行卡") << tr("支票"));
    ui->comboBox->setCurrentIndex(0);
}

void vip_recharge::on_pushButton_5_clicked()//退出
{
    this->close();
}

void vip_recharge::on_pushButton_7_clicked()//读卡
{
    //要判断后台设置的是磁卡还是IC卡

    read_magnetic_card magnetic_card;
    /*magnetic_card->exec();*/backheader::static_dialogexec(magnetic_card, tr("会员卡号"));
}
