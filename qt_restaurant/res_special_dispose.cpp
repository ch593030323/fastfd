#include "res_special_dispose.h"
#include "ui_res_special_dispose.h"

res_special_dispose::res_special_dispose(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::res_special_dispose)
{
    ui->setupUi(this);  //this->setWindowFlags(Qt::FramelessWindowHint|Qt::Dialog);this->setStyleSheet("QDialog{border:2px solid gray}");
    init_control();
    btn_t_f = false;
    curindex_id;
}

res_special_dispose::~res_special_dispose()
{
    delete ui;
}

void res_special_dispose::init_control()
{
    QPixmap pixmap(1, 60);
    pixmap.fill(Qt::transparent);
    QIcon icon(pixmap);
    ui->comboBox->setIconSize(QSize(1, 60));
    ui->comboBox->addItem(icon,tr("免低消费"));
    ui->comboBox->addItem(icon,tr("免服务费"));
    ui->comboBox->addItem(icon,tr("免单"));
    ui->comboBox->addItem(icon,tr("手工抹零"));
}

void res_special_dispose::on_pushButton_4_clicked()//确定
{
    curindex_id = ui->comboBox->currentIndex();
    btn_t_f = true;
    this->close();
}

void res_special_dispose::on_pushButton_5_clicked()//取消
{
    btn_t_f = false;
    this->close();
}
