#ifndef LDS_MODEL_TABLEMODEL_H
#define LDS_MODEL_TABLEMODEL_H


#include <QSqlTableModel>
#include <QSqlDatabase>
#include <QStringList>
#include <QPixmap>
#include <QSortFilterProxyModel>

class lds_model_tablemodel : public QSqlTableModel
{
    Q_OBJECT
public:
    class FilterProxyModel:public QSortFilterProxyModel{
    public:
        FilterProxyModel(QObject *parent = 0):
            QSortFilterProxyModel(parent){}

        Qt::ItemFlags flags(const QModelIndex &index) const;
    };
    FilterProxyModel *fpmodel;

public:
    explicit lds_model_tablemodel(QObject *parent = 0,  QSqlDatabase db = QSqlDatabase());

    bool model_is_dirty();
    QModelIndex dirty_index;
    QString dirty_err_string();

    bool same_column_exist(const QString &field_name, int row);
    QString max_column_add_1(const QString &field_name);//返回+1后的str
    int find_column_exist(const QString &field_name, const QString &cmp_value);//return 行数

    QString sum_value_column(const QString &field_name);

    void save_set_header(const QString &field_name, const QString &text, bool iseditable=false);
    QStringList save_set_header_saver;

    void set_last_data(const QString &field_name, const QString &text);

};

#endif // LDS_MODEL_TABLEMODEL_H
