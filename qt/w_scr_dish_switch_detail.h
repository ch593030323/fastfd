#ifndef W_SCR_DISH_SWITCH_DETAIL_H
#define W_SCR_DISH_SWITCH_DETAIL_H

#include <QDialog>
#include "commonheader.h"

namespace Ui {
class w_scr_dish_switch_detail;
}

class w_scr_dish_switch_detail : public QDialog
{
    Q_OBJECT

public:
    explicit w_scr_dish_switch_detail(QWidget *parent = 0);
    ~w_scr_dish_switch_detail();
    QString swtich_oper;
private slots:
    void tosaveini();
    void toswitch();
    void toexit();
private:
    Ui::w_scr_dish_switch_detail *ui;
    blockIns printins;
};

#endif // W_SCR_DISH_SWITCH_DETAIL_H
