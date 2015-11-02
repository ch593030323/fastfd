#ifndef W_BT_DISH_H
#define W_BT_DISH_H

#include <QDialog>
#include "lds_tableview_repecify.h"
#include <QStandardItemModel>
#include "backheader.h"

class QComboBox;
class w_bt_dish : public QDialog
{
    Q_OBJECT
public:
    explicit w_bt_dish(QWidget *parent = 0);
    //基础资料
    static QStandardItemModel MODEL_DISH_DEDUCTTYPE;//t提成方式
    //基础资料
    static QStandardItemModel MODEL_DISH_SERVICE_MODE;//服务费模式
    //基础资料
    static QStandardItemModel MODEL_DISH_LOW_MODE;//低消方式

    static   QString comcurrentvalue_0(QComboBox*com);// 格式：no name 返回值 currentindex no


    static int MODEL_DISH_INIT();
private:
};

#endif // W_BT_DISH_H
