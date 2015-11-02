#ifndef LDS_MODEL_SQLTABLEMODEL_H
#define LDS_MODEL_SQLTABLEMODEL_H

#include "lds_model_tablemodel.h"
class lds_model_sqltablemodel : public lds_model_tablemodel
{
    Q_OBJECT
public:
    explicit lds_model_sqltablemodel(QObject *parent = 0, QSqlDatabase db = QSqlDatabase());
    Qt::ItemFlags normalflag;
    Qt::ItemFlags flags(const QModelIndex &index) const;

};

#endif // LDS_MODEL_SQLTABLEMODEL_H
