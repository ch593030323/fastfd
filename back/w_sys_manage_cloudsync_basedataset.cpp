#include "w_sys_manage_cloudsync_basedataset.h"
#include "qsettings.h"
#include "public_sql.h"
#include <QDebug>
#include "wei_xin_service_help.h"
#include "lds_messagebox.h"

w_sys_manage_cloudsync_basedataset::w_sys_manage_cloudsync_basedataset(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::w_sys_manage_cloudsync_basedataset)
{
    ui->setupUi(this);

    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    ui->yun_server_ip->setText(conf.value("w_sys_manage_cloudsync_basedataset/yun_server_ip","").toString());
    ui->cloudUser->setText(conf.value("w_sys_manage_cloudsync_basedataset/cloudUser","").toString());
    ui->cloudPass->setText(conf.value("w_sys_manage_cloudsync_basedataset/cloudPass","").toString());
    ui->orgCode->setText(conf.value("w_sys_manage_cloudsync_basedataset/orgCode","").toString());
    ui->membership_data_flag->setChecked(conf.value("w_sys_manage_cloudsync_basedataset/membership_data_flag",true).toBool());
    ui->sales_data_flag->setChecked(conf.value("w_sys_manage_cloudsync_basedataset/sales_data_flag",true).toBool());
    ui->first_boot_upload_data->setChecked(conf.value("w_sys_manage_cloudsync_basedataset/first_boot_upload_data",false).toBool());
    ui->first_boot_download_data->setChecked(conf.value("w_sys_manage_cloudsync_basedataset/first_boot_download_data").toBool());

    ui->checkBox_yun_member->setChecked(conf.value("w_sys_manage_cloudsync_basedataset/yun_member").toBool());
    ui->checkBox_yun_start->setChecked(conf.value("w_sys_manage_cloudsync_basedataset/yun_start").toBool());
}

w_sys_manage_cloudsync_basedataset::~w_sys_manage_cloudsync_basedataset()
{
    delete ui;
}

/**
 * 确认按钮
 */
void w_sys_manage_cloudsync_basedataset::on_pushButton_submit_clicked()
{
    save_ini();
    lds_messagebox::warning(this, tr("消息提示"),tr("保存成功"));
    this->accept();
}

/**
 * 关闭按钮
 */
void w_sys_manage_cloudsync_basedataset::on_pushButton_close_clicked()
{
    this->reject();
}

void w_sys_manage_cloudsync_basedataset::on_pushButton_usrcheck_clicked()
{
    ui->pushButton_usrcheck->setEnabled(false);
    wei_xin_service_help wxHelp(this);
    wxHelp.checkUser();
    ui->pushButton_usrcheck->setEnabled(true);
}

void w_sys_manage_cloudsync_basedataset::save_ini()
{
    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);

    conf.setValue("w_sys_manage_cloudsync_basedataset/yun_server_ip",ui->yun_server_ip->text());
    conf.setValue("w_sys_manage_cloudsync_basedataset/cloudUser",ui->cloudUser->text());
    conf.setValue("w_sys_manage_cloudsync_basedataset/cloudPass",ui->cloudPass->text());
    conf.setValue("w_sys_manage_cloudsync_basedataset/orgCode",ui->orgCode->text());
    conf.setValue("w_sys_manage_cloudsync_basedataset/membership_data_flag",ui->membership_data_flag->isChecked());
    conf.setValue("w_sys_manage_cloudsync_basedataset/sales_data_flag",ui->sales_data_flag->isChecked());
    conf.setValue("w_sys_manage_cloudsync_basedataset/first_boot_upload_data",ui->first_boot_upload_data->isChecked());
    conf.setValue("w_sys_manage_cloudsync_basedataset/first_boot_download_data",ui->first_boot_download_data->isChecked());

    conf.setValue("w_sys_manage_cloudsync_basedataset/yun_member",ui->checkBox_yun_member->isChecked());
    conf.setValue("w_sys_manage_cloudsync_basedataset/yun_start",ui->checkBox_yun_start->isChecked());
}
