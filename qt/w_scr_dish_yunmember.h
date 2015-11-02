#ifndef W_SCR_DISH_YUNMEMBER_H
#define W_SCR_DISH_YUNMEMBER_H

#include <QDialog>
#include "ui_vip_card.h"

class w_scr_dish_yunmember : public QDialog
{
    Q_OBJECT
public:
    explicit w_scr_dish_yunmember(QWidget *parent = 0);

signals:

public slots:
private:
    Ui_vip_card *ui;

};

#endif // W_SCR_DISH_YUNMEMBER_H
