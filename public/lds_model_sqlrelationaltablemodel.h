#ifndef LDS_MODEL_SQLRELATIONALTABLEMODEL_H
#define LDS_MODEL_SQLRELATIONALTABLEMODEL_H

#include <QSqlRelationalTableModel>
#include <QSqlDatabase>
#include <QStringList>
#include <QPixmap>
#include <QSortFilterProxyModel>

class lds_model_sqlrelationaltablemodel : public QSqlRelationalTableModel
{
    Q_OBJECT
public:
    struct SQL_HEADERLABEL{
        SQL_HEADERLABEL(const QString &_sql, const QString &_headerlabel):
            sql(_sql), headerlabel(_headerlabel){}
        QString sql;
        QString headerlabel;
    };

    class FilterProxyModel:public QSortFilterProxyModel{//过滤model ，因为window下的tablemodel编辑是总是无法正常使用
    public:
        FilterProxyModel(lds_model_sqlrelationaltablemodel *parent = 0):
            QSortFilterProxyModel(parent),
        _parent(parent){}
        lds_model_sqlrelationaltablemodel *_parent;
        Qt::ItemFlags flags(const QModelIndex &index) const;
    };
    FilterProxyModel *fpmodel;

public:
    lds_model_sqlrelationaltablemodel(QObject *parent = 0, QSqlDatabase db = QSqlDatabase());

    bool model_is_dirty();//model是否有脏数据
    QModelIndex dirty_index;//假如有脏，对应的脏数据的index
    QString dirty_err_string();//更具脏数据的index，返回错误信息

    bool same_column_exist(const QString &field_name, const QString &value);//是否有相同的filed_name对应的value存在
    bool same_column_exist(const QString &field_name, int row);
    bool same_column_exist(const QString &field_name);
    bool same_column_exist(int column);

    QString max_column_add_1(const QString &field_name);//返回+1后的str
    int find_column_exist(const QString &field_name, const QString &cmp_value);//return 行数

    QString sum_value_column(const QString &field_name);//某一field_name的一列的总和

    void save_set_header(const QString &field_name, const QString &text, bool iseditable=false);//配合tableview的 rePecifyHeader 函数使用的
    QStringList save_set_header_saver;
    int save_set_header_saver_index(const QString &headername);

    void set_last_data(const QString &field_name, const QString &text);//设置最后一行的数据
    void set_row_data(int row, const QString &field_name, const QString &text);//设置某一行，的数据

    //!=====本model提供新的relation，补充系统的setrelation，
    //!=====由于系统的relation只能对id使用另外的表中的name经行替换，
    //!=====这里无线放大了这种情况，额外的情况，用蓝色表头说明，但是同时丧失了select函数的意思
    QList<SQL_HEADERLABEL> sql_headerlabels;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
protected:
    QString selectStatement() const;
    bool updateRowInTable(int row, const QSqlRecord &values);
    bool insertRowIntoTable(const QSqlRecord &values);
    QString orderByClause() const;
//    example
//    w_m_member_point::RelationalTableModel::RelationalTableModel(QObject *parent, QSqlDatabase db)
//        : lds_model_sqlrelationaltablemodel(parent, db)
//    {
//        backheader::regetvch_operid_name();
//        sql_headerlabels.append(SQL_HEADERLABEL("ifnull((select num_point from t_m_curamount where t_m_curamount.vch_memberno=t_m_member.vch_memberno), 0)" ,tr("剩余积分")));
//    }

//    QVariant w_m_member_point::RelationalTableModel::data(const QModelIndex &item, int role) const
//    {
//        QVariant _d=QSqlRelationalTableModel::data(item, role);
//        if(role==Qt::DisplayRole){
//            if(_d.type()==QVariant::DateTime){
//                return _d.toDateTime().toString("yyyy-MM-dd");
//            }
//            if(this->headerData(item.column(), Qt::Horizontal,  Qt::DisplayRole).toString().endsWith(tr("状态"))){
//                return backheader::VIPFINDVALUE(w_m_member::VIP_MODEL_STATE, _d.toString());
//            }
//            if(this->headerData(item.column(), Qt::Horizontal,  Qt::DisplayRole).toString().endsWith(tr("创建人员"))){
//                return backheader::getouterdata(backheader::vch_operid_name, _d.toString());
//            }
//        }
//        return _d;
//    }

//    Qt::ItemFlags w_m_member_point::RelationalTableModel::flags(const QModelIndex &index) const
//    {
//        return Qt::ItemIsEnabled|Qt::ItemIsSelectable;
//    }
    //!======
};

#endif // LDS_MODEL_SQLRELATIONALTABLEMODEL_H
