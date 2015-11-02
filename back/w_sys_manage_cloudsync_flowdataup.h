#ifndef W_SYS_MANAGE_CLOUDSYNC_FLOWDATAUP_H
#define W_SYS_MANAGE_CLOUDSYNC_FLOWDATAUP_H

#include <QDialog>
#include "cloudsync_service_help.h"
#include "ui_w_sys_manage_cloudsync_flowdataup.h"
#include <QCloseEvent>

namespace Ui {
class w_sys_manage_cloudsync_flowdataup;
}

class w_sys_manage_cloudsync_flowdataup : public QDialog
{
    Q_OBJECT

public:
    explicit w_sys_manage_cloudsync_flowdataup(QWidget *parent = 0);
    ~w_sys_manage_cloudsync_flowdataup();


public slots:
    void on_pushButton_cancel_clicked();

    void on_pushButton_up_clicked();

    void tocancel();
protected:
    void closeEvent(QCloseEvent *e);
public:
    Ui::w_sys_manage_cloudsync_flowdataup *ui;
    cloudsync_service_help *my_cloudsync_service_help;
};

#endif // W_SYS_MANAGE_CLOUDSYNC_FLOWDATAUP_H
