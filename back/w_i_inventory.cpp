#include "w_i_inventory.h"
#include <QDebug>

QStandardItemModel w_i_inventory::MODEL_INVENTORY_TYPE;
QStandardItemModel w_i_inventory::MODEL_INVENTORY_STATE_TYPE;

w_i_inventory::w_i_inventory(QObject *parent) :
    QObject(parent)
{
}

int w_i_inventory::MODEL_INVENTORY_INIT()
{
    qDebug() << "MODEL_INVENTORY_INIT...";
    //! init
    QList<QStandardItem *> items;
    if(MODEL_INVENTORY_TYPE.rowCount() == 0){
        items.clear();items<<new QStandardItem("001")<<new QStandardItem(tr("采购入库"));MODEL_INVENTORY_TYPE.appendRow(items);
        items.clear();items<<new QStandardItem("002")<<new QStandardItem(tr("退货出货"));MODEL_INVENTORY_TYPE.appendRow(items);
        items.clear();items<<new QStandardItem("004")<<new QStandardItem(tr("物料报损"));MODEL_INVENTORY_TYPE.appendRow(items);

    }
    if(MODEL_INVENTORY_STATE_TYPE.rowCount() == 0){
        items.clear();items<<new QStandardItem("N")<<new QStandardItem(tr("未审核"));MODEL_INVENTORY_STATE_TYPE.appendRow(items);
        items.clear();items<<new QStandardItem("Y")<<new QStandardItem(tr("已审核"));MODEL_INVENTORY_STATE_TYPE.appendRow(items);
        items.clear();items<<new QStandardItem("O")<<new QStandardItem(tr("已作废"));MODEL_INVENTORY_STATE_TYPE.appendRow(items);

    }
}
