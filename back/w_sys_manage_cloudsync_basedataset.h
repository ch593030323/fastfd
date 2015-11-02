#ifndef W_SYS_MANAGE_CLOUDSYNC_BASEDATASET_H
#define W_SYS_MANAGE_CLOUDSYNC_BASEDATASET_H

#include <QDialog>
#include "ui_w_sys_manage_cloudsync_basedataset_dialog.h"

namespace Ui {
class w_sys_manage_cloudsync_basedataset;
}

class w_sys_manage_cloudsync_basedataset : public QDialog
{
    Q_OBJECT

public:
    explicit w_sys_manage_cloudsync_basedataset(QWidget *parent = 0);
    ~w_sys_manage_cloudsync_basedataset();

    Ui::w_sys_manage_cloudsync_basedataset *ui;
private slots:
    // 确认按钮事件
    void on_pushButton_submit_clicked();
    // 关闭按钮事件
    void on_pushButton_close_clicked();
    // 验证用户
    void on_pushButton_usrcheck_clicked();

    //配置文件保存
    void save_ini();
private:
};

#endif // W_SYS_MANAGE_CLOUDSYNC_BASEDATASET_H
