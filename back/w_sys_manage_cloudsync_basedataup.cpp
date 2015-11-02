#include "w_sys_manage_cloudsync_basedataup.h"
#include "ui_w_sys_manage_cloudsync_basedataup.h"


w_sys_manage_cloudsync_basedataup::w_sys_manage_cloudsync_basedataup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::w_sys_manage_cloudsync_basedataup)
{  
    ui->setupUi(this);
    my_cloudsync_service_help = new cloudsync_service_help(ui->progressBar);
}

w_sys_manage_cloudsync_basedataup::~w_sys_manage_cloudsync_basedataup()
{
    delete ui;
    delete my_cloudsync_service_help;
}

/**
 * 上传按钮事件
 */
void w_sys_manage_cloudsync_basedataup::on_pushButton_up_clicked()
{
    ui->pushButton_up->setEnabled(false);
    my_cloudsync_service_help->upLoadBaseDate();
    ui->pushButton_up->setEnabled(true);
    this->accept();
}

/**
 * 取消按钮事件
 */
void w_sys_manage_cloudsync_basedataup::on_pushButton_cancel_clicked()
{
    my_cloudsync_service_help->stop();
    this->reject();
}

void w_sys_manage_cloudsync_basedataup::closeEvent(QCloseEvent *e)
{
    e->ignore();
}
