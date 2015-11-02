#ifndef W_SCR_DISH_RESTAURANT_DIALOG_H
#define W_SCR_DISH_RESTAURANT_DIALOG_H

#include <QDialog>
#include "public_logindialog.h"
#include "frontviewdialog_virtual.h"

//厨打 出品(服务员)  预打单(结账单,可能有二维码结账)  结账单 消费单(客人)
namespace Ui {
class w_scr_dish_restaurant_dialog;
}
class w_scr_dish_restaurant_dialog : public QDialog
{
    Q_OBJECT
public:
    explicit w_scr_dish_restaurant_dialog(public_loginDialog *parent = 0);
    ~w_scr_dish_restaurant_dialog();
    static QString dalei;
//    static QString table_lock;
    static QString table_danhao;
//    static QString table_no;
public slots:
    void torefresh();
private slots:
    void tocanceldeak();
    void tolockstate();
    void toopen();
    void toorder();//预订
    void toother();
    void toquit();
    void toset();
    void toshift();
    void toswitch();

    void totablemessage(const QString &tableno);
    void totablesave(const QString &tableno);
    void toupdatetable(const QString &tabletype);
    void toswitchback();
private:
    Ui::w_scr_dish_restaurant_dialog *ui;
    public_loginDialog *_parent;
    QString _curtypeno;
};

#endif // W_SCR_DISH_RESTAURANT_DIALOG_H
