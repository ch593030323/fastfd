#ifndef LDS_COMBOBOX_MODEL_H
#define LDS_COMBOBOX_MODEL_H

#include <QStandardItemModel>

class lds_combobox_model : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit lds_combobox_model(QObject *parent = 0);
    ~lds_combobox_model();

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
};

#endif // LDS_COMBOBOX_MODEL_H
