#ifndef W_SCR_DISH_SPECIAL_COOK_H
#define W_SCR_DISH_SPECIAL_COOK_H

#include <QDialog>
#include "fast_order_2.h"
#include "ui_w_scr_dish_special_cook.h"

namespace Ui {
class w_scr_dish_special_cook;
}

class w_scr_dish_special_cook : public QDialog
{
    Q_OBJECT

public:
    explicit w_scr_dish_special_cook(const QString & dishno,const QString & neirong,const QString & price,QWidget *parent = 0);
    ~w_scr_dish_special_cook();
    bool btn_t_f;
    QStringList list;
    float add_price;
    QString zuofaneirong;
private slots:
    void to_showalldish();
    void to_ok();
    void to_exit();
    void to_update_labelreturn(const QString &dishno);
private:
    Ui::w_scr_dish_special_cook *ui;
};

#endif // W_SCR_DISH_SPECIAL_COOK_H
