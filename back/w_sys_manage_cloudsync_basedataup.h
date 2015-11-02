#ifndef W_SYS_MANAGE_CLOUDSYNC_BASEDATAUP_H
#define W_SYS_MANAGE_CLOUDSYNC_BASEDATAUP_H

#include <QDialog>
#include <QDesktopWidget>
#include "cloudsync_service_help.h"
#include <QCloseEvent>

namespace Ui {
class w_sys_manage_cloudsync_basedataup;
}

class w_sys_manage_cloudsync_basedataup : public QDialog
{
    Q_OBJECT

public:
    explicit w_sys_manage_cloudsync_basedataup(QWidget *parent = 0);
    ~w_sys_manage_cloudsync_basedataup();

private slots:
    // 上传按钮事件
    void on_pushButton_up_clicked();
    // 取消按钮事件
    void on_pushButton_cancel_clicked();

protected:
    void closeEvent(QCloseEvent *e);
public:
    Ui::w_sys_manage_cloudsync_basedataup *ui;
    cloudsync_service_help *my_cloudsync_service_help;
};

#endif // W_SYS_MANAGE_CLOUDSYNC_BASEDATAUP_H
