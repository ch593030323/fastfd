#ifndef W_SCR_DISH_WEIXIN_QUITBILL_H
#define W_SCR_DISH_WEIXIN_QUITBILL_H

#include <QDialog>
#include <QSqlQueryModel>

class Ui_w_scr_dish_weixin_quitbill_dialog;
class w_scr_dish_weixin_quitbill : public QDialog
{
    Q_OBJECT
public:
    explicit w_scr_dish_weixin_quitbill(QWidget *parent = 0);

private:
    Ui_w_scr_dish_weixin_quitbill_dialog *ui;
private slots:
    void toselect();
    void toexit();
    void torefuse();
    void toconfirm();
private:
    QSqlQueryModel *querymodel;
};

#endif // W_SCR_DISH_WEIXIN_QUITBILL_H
