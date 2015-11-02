#include "lds_combobox_model.h"

lds_combobox_model::lds_combobox_model(QObject *parent) : QStandardItemModel(parent)
{

}

lds_combobox_model::~lds_combobox_model()
{

}

QVariant lds_combobox_model::data(const QModelIndex &index, int role) const
{
    QVariant _d=QStandardItemModel::data(index, role);

    if(role==Qt::SizeHintRole){
        return QSize(0,40);
    }
    return _d;
}

