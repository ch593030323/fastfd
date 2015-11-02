#include "w_sys_manage_cloudsync_flowdataup.h"
#include "n_func.h"
#include "lds_messagebox.h"


w_sys_manage_cloudsync_flowdataup::w_sys_manage_cloudsync_flowdataup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::w_sys_manage_cloudsync_flowdataup)
{
    ui->setupUi(this);

    ui->dateTimeEdit_begin->setDisplayFormat("yyyy-MM-dd");
    ui->dateTimeEdit_end->setDisplayFormat("yyyy-MM-dd");
    ui->dateTimeEdit_begin->setDate(n_func::f_get_sysdatetime().date());
    ui->dateTimeEdit_end->setDate(n_func::f_get_sysdatetime().date());
    my_cloudsync_service_help = new cloudsync_service_help(ui->progressBar);
    connect(ui->pushButton_cancel,SIGNAL(clicked()),this,SLOT(tocancel()));
}

w_sys_manage_cloudsync_flowdataup::~w_sys_manage_cloudsync_flowdataup()
{
    delete ui;
    delete my_cloudsync_service_help;
}

/**
 * 关闭按钮点击事件
 */
void w_sys_manage_cloudsync_flowdataup::on_pushButton_cancel_clicked()
{
//    my_cloudsync_service_help->stop();
//    this->reject();
}

void w_sys_manage_cloudsync_flowdataup::on_pushButton_up_clicked()
{
    ui->pushButton_up->setEnabled(false);
    my_cloudsync_service_help->upLoadFlowDate(ui->dateTimeEdit_begin->date().toString("yyyy-MM-dd"),ui->dateTimeEdit_end->date().toString("yyyy-MM-dd"));
    ui->pushButton_up->setEnabled(true);
    lds_messagebox::warning(this, tr("消息提示"),tr("上传成功!"), true);
    this->accept();
}

void w_sys_manage_cloudsync_flowdataup::tocancel()
{
    my_cloudsync_service_help->stop();
    this->reject();
}

void w_sys_manage_cloudsync_flowdataup::closeEvent(QCloseEvent *e)
{
    e->ignore();
}
