#ifndef W_SYS_MANAGE_CLOUDSYNC_BASEDATADOWN_H
#define W_SYS_MANAGE_CLOUDSYNC_BASEDATADOWN_H

#include <QDialog>
#include "cloudsync_service_help.h"
#include <QCloseEvent>

namespace Ui {
class w_sys_manage_cloudsync_basedatadown;
}

class w_sys_manage_cloudsync_basedatadown : public QDialog
{
    Q_OBJECT

public:
    explicit w_sys_manage_cloudsync_basedatadown(QWidget *parent = 0);
    ~w_sys_manage_cloudsync_basedatadown();

public slots:
    // 下载按钮事件
    void on_pushButton_down_clicked();
    // 关闭按钮事件
    void on_pushButton_cancel_clicked();
protected:
    void closeEvent(QCloseEvent *e);

private:
    cloudsync_service_help *my_cloudsync_service_help;
    Ui::w_sys_manage_cloudsync_basedatadown *ui;
};

#endif // W_SYS_MANAGE_CLOUDSYNC_BASEDATADOWN_H
