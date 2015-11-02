#ifndef W_BT_DISH_BASE_CHANGE_SELECT_DISH_IMAGE_H
#define W_BT_DISH_BASE_CHANGE_SELECT_DISH_IMAGE_H

#include <QDialog>

class Ui_w_bt_dish_base_change_select_dish_image_Dialog;

class w_bt_dish_base_change_select_dish_image : public QDialog
{
    Q_OBJECT
public:
    explicit w_bt_dish_base_change_select_dish_image(QWidget *parent = 0);

    QString select_text;
public slots:
    void took();
    void toexit();
private:
    Ui_w_bt_dish_base_change_select_dish_image_Dialog *ui;

};

#endif // W_BT_DISH_BASE_CHANGE_SELECT_DISH_IMAGE_H
