#ifndef W_SCR_DISH_WEIXIN_GETBILL_H
#define W_SCR_DISH_WEIXIN_GETBILL_H

#include <QDialog>
#include "ui_w_scr_dish_weixin_getbill_dialog.h"
class w_scr_dish_weixin_getbill : public QDialog
{
    Q_OBJECT
public:
    explicit w_scr_dish_weixin_getbill(QWidget *parent = 0);
public:
    Ui_w_scr_dish_weixin_getbill_dialog*ui;

};

#endif // W_SCR_DISH_WEIXIN_GETBILL_H
