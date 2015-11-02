#ifndef PUBLIC_LOGINDIALOG_H
#define PUBLIC_LOGINDIALOG_H

#include <QDialog>
#include <QDateTime>
#include <QBasicTimer>
#include "fontviewdialog_doublescreen.h"
#include "lds_dialog_drog.h"
class fast_order_2;
class Printer_POS_SERIAL;
class lds_loadingDialog;
class Ui_backviewdialog3_Dialog;
class fontviewDialog_doublescreen;
class w_scr_dish_restaurant_dialog;
class public_loginDialog : public lds_dialog_drog
{
    Q_OBJECT
public:
    explicit public_loginDialog(QWidget *parent = 0, const QString &u="", const QString &p="");
    ~public_loginDialog();
    void setversion(const QString &text);
public slots:
    void goto_back();
    void goto_pre();

    bool pre_is_exist();
    void togoto_back();
    void togoto_pre();
    void goto_close();
    void todbset();//数据库设置
    void hideall_and_back_login(const QString &p, const QString &u);
    void hideall_and_back_login();
signals:
    void signal_w_sys_manage_cloudsync_basedatadown();
    void signal_everytime_to_print_failed_kitchen_bill();
public slots:
    void init_dbconnect();
    void to_init_dbconnect(lds_loadingDialog *loading);
    void showprinterr(const QString &errstring, const QString &title);
    void showmemoinfo();
    QWidget* getcur_validparent();
    void to_sql_heart_beat();
    /*厨打*/
public slots:
    void to_everytime_to_print_failed_kitchen_bill(const QString &int_id=QString(), const QString &vch_ip=QString());//间断时间补打漏打单


private slots:
    void set_num();
    QString totrylogin();//验证登录信息
    void updateinfo(const QString &errinfo);
    void eveyread();

protected:
    void timerEvent(QTimerEvent *e);
public :
    fast_order_2 *dialog_pre;
    fontviewDialog_doublescreen *double_screen;
private:
    Printer_POS_SERIAL *printer;
    Ui_backviewdialog3_Dialog *ui;
    int info_sqr;
    QDialog *dialog_back;
    w_scr_dish_restaurant_dialog *dialog_pre_2;
    QTimer sql_heart_beat;

    QBasicTimer btimer;
    QBasicTimer kitchen_timer;
    bool sql_isopen;
};

#endif // PUBLIC_LOGINDIALOG_H
