#ifndef W_BT_DISH_PAYMODE_H
#define W_BT_DISH_PAYMODE_H

#include <QDialog>
#include "w_bt_dish.h"

class Ui_w_bt_dish_paymode_dialog;
class lds_model_sqlrelationaltablemodel;
class lds_tableView;
class QDataWidgetMapper;
class w_bt_dish_paymode : public QDialog
{
    Q_OBJECT
public:
    explicit w_bt_dish_paymode(QWidget *parent = 0);

private slots:
    void tonew();
    void todel();
    void torefresh();
    bool tosave();
    void toexit();

    void mapper_setcurindex(int index);
private:
    Ui_w_bt_dish_paymode_dialog *ui;
    lds_model_sqlrelationaltablemodel *tablemodel;
    QDataWidgetMapper *mapper;

};

#endif // W_BT_DISH_PAYMODE_H
