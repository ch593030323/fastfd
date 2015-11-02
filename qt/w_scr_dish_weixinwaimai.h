#ifndef W_SCR_DISH_WEIXINWAIMAI_H
#define W_SCR_DISH_WEIXINWAIMAI_H

#include <QDialog>
#include <QStandardItemModel>
#include "fast_order_2.h"

class lds_model_sqlrelationaltablemodel;
class Ui_w_scr_dish_weixinwaimai_dialog;
class w_scr_dish_weixinwaimai:public QDialog{
    Q_OBJECT
public:
    w_scr_dish_weixinwaimai(fast_order_2 *parent=0);
private slots:
    void torefuse();//拒绝订单
    void totake_waimai();
    void toexit();
    void tosearch(int row=-1);
    void torefresh();
private:
    Ui_w_scr_dish_weixinwaimai_dialog *ui;
    lds_model_sqlrelationaltablemodel *tablemodel;
    lds_model_sqlrelationaltablemodel *tablemodel_2;

    fast_order_2 *_parent;
};

#endif // W_SCR_DISH_WEIXINWAIMAI_H
