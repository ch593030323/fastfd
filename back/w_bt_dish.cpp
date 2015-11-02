
#include "w_bt_dish.h"
//盘点机：中智 大鹏 -> 好搭档
#include <QComboBox>
QStandardItemModel w_bt_dish::MODEL_DISH_DEDUCTTYPE;
QStandardItemModel w_bt_dish::MODEL_DISH_SERVICE_MODE;
QStandardItemModel w_bt_dish::MODEL_DISH_LOW_MODE;

//菜品资料维护
w_bt_dish::w_bt_dish(QWidget *parent) : QDialog(parent)
{
}

QString w_bt_dish::comcurrentvalue_0(QComboBox*com)
{
    QString ret;
    if(com->model()){
        if(com->currentIndex()>-1){
            ret=com->model()->index(com->currentIndex(),0).data().toString();
        }
    }
    return  ret;
}

int w_bt_dish::MODEL_DISH_INIT()
{
    qDebug() << "MODEL_INVENTORY_INIT...";
    //! init
    QList<QStandardItem *> items;
    if(MODEL_DISH_DEDUCTTYPE.rowCount() == 0){
        items.clear();items<<new QStandardItem("1")<<new QStandardItem(tr("比例提成"));MODEL_DISH_DEDUCTTYPE.appendRow(items);
        items.clear();items<<new QStandardItem("2")<<new QStandardItem(tr("定额提成"));MODEL_DISH_DEDUCTTYPE.appendRow(items);
    }
    if(MODEL_DISH_SERVICE_MODE.rowCount() == 0){
        items.clear();items<<new QStandardItem("0")<<new QStandardItem(tr("不处理"));MODEL_DISH_SERVICE_MODE.appendRow(items);
        items.clear();items<<new QStandardItem("1")<<new QStandardItem(tr("时间单位"));MODEL_DISH_SERVICE_MODE.appendRow(items);
        items.clear();items<<new QStandardItem("2")<<new QStandardItem(tr("消费额服务费率"));MODEL_DISH_SERVICE_MODE.appendRow(items);
        items.clear();items<<new QStandardItem("3")<<new QStandardItem(tr("餐桌定额"));MODEL_DISH_SERVICE_MODE.appendRow(items);
        items.clear();items<<new QStandardItem("4")<<new QStandardItem(tr("菜品消费服务率"));MODEL_DISH_SERVICE_MODE.appendRow(items);
    }
    if(MODEL_DISH_LOW_MODE.rowCount() == 0){
        items.clear();items<<new QStandardItem("1")<<new QStandardItem(tr("餐桌低消"));MODEL_DISH_LOW_MODE.appendRow(items);
        items.clear();items<<new QStandardItem("2")<<new QStandardItem(tr("人均低消"));MODEL_DISH_LOW_MODE.appendRow(items);
    }
}

