#include "special_dispose.h"
#include "ui_special_dispose.h"

special_dispose::special_dispose(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::special_dispose)
{
    ui->setupUi(this);
    init_control();
    btn_t_f = false;
    curindex_id;
}

special_dispose::~special_dispose()
{
    delete ui;
}

void special_dispose::init_control()
{
    ui->comboBox->addItems(QStringList()<<tr("免单")<<tr("手工抹零"));
}

void special_dispose::on_pushButton_4_clicked()//确定
{
    curindex_id = ui->comboBox->currentIndex();
    btn_t_f = true;
    this->close();
}

void special_dispose::on_pushButton_5_clicked()//取消
{
    btn_t_f = false;
    this->close();
}
