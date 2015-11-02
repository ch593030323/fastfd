#include "w_sys_manage_cloudsync_basedatadown.h"
#include "ui_w_sys_manage_cloudsync_basedatadown.h"

w_sys_manage_cloudsync_basedatadown::w_sys_manage_cloudsync_basedatadown(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::w_sys_manage_cloudsync_basedatadown)
{
    ui->setupUi(this);
    my_cloudsync_service_help = new cloudsync_service_help(ui->progressBar);
}

w_sys_manage_cloudsync_basedatadown::~w_sys_manage_cloudsync_basedatadown()
{
    delete ui;
    delete my_cloudsync_service_help;
}

/**
 * 下载按钮点击事件
 */
void w_sys_manage_cloudsync_basedatadown::on_pushButton_down_clicked()
{
    ui->pushButton_down->setEnabled(false);
    my_cloudsync_service_help->downLoadBaseDate();
    ui->pushButton_down->setEnabled(true);
    this->accept();
}

/**
 * 关闭按钮点击事件
 */
void w_sys_manage_cloudsync_basedatadown::on_pushButton_cancel_clicked()
{
    my_cloudsync_service_help->stop();
    this->reject();

}

void w_sys_manage_cloudsync_basedatadown::closeEvent(QCloseEvent *e)
{
    e->ignore();

}
