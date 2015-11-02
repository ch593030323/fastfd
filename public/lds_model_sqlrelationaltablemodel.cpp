#include "lds_model_sqlrelationaltablemodel.h"
#include <QColor>
#include <QtDebug>
#include <QSqlRecord>
#include <QPixmap>

lds_model_sqlrelationaltablemodel::lds_model_sqlrelationaltablemodel(QObject *parent, QSqlDatabase db) :
    QSqlRelationalTableModel(parent, db)
{
    fpmodel=new FilterProxyModel(this);
    fpmodel->setSourceModel(this);
}

bool lds_model_sqlrelationaltablemodel::model_is_dirty()
{
    dirty_index=QModelIndex();
    bool dirty=false;
    for(int r=0,r_count=this->rowCount();r<r_count;r++){
        for(int c=0,c_count=this->columnCount();c<c_count;c++){
            if(this->isDirty(this->index(r, c))){
                dirty=true;
                dirty_index=this->index(r,c);
                break;
            }
        }
        if(dirty)break;
    }
    return dirty;
}

QString lds_model_sqlrelationaltablemodel::dirty_err_string()
{
    if(dirty_index.isValid()){
        return tr("%1,%2")//数据为脏!")
                .arg(dirty_index.row()+1)
                .arg(dirty_index.column()+1);

    }
    return "";
}

bool lds_model_sqlrelationaltablemodel::same_column_exist(const QString &field_name, const QString &value)
{
    QString cmp_value=value;
    for(int r=0,r_count=this->rowCount();r<r_count;r++){
        if(cmp_value==this->record(r).value(field_name).toString()){
            return true;
        }
    }
    return false;
}

bool lds_model_sqlrelationaltablemodel::same_column_exist(const QString &field_name, int row)
{
    QString cmp_value=this->record(row).value(field_name).toString();
    for(int r=0,r_count=this->rowCount();r<r_count;r++){
        if(r==row)continue;
        if(cmp_value==this->record(r).value(field_name).toString()){
            return true;
        }
    }
    return false;
}

bool lds_model_sqlrelationaltablemodel::same_column_exist(const QString &field_name)
{
    QSet<QString> content_set;
    for(int r=0,r_count=this->rowCount();r<r_count;r++){
        content_set.insert(this->record(r).value(field_name).toString());
    }
    return content_set.count()!=this->rowCount();
}

bool lds_model_sqlrelationaltablemodel::same_column_exist(int column)
{
    QSet<QString> content_set;
    for(int r=0,r_count=this->rowCount();r<r_count;r++){
        content_set.insert(this->index(r, column).data().toString());
    }
    qDebug() << content_set<<(content_set.count()!=this->rowCount());
    return content_set.count()!=this->rowCount();
}

QString lds_model_sqlrelationaltablemodel::max_column_add_1(const QString &field_name)
{
    int value=0;
    int cmp_value;
    for(int r=0,r_count=this->rowCount();r<r_count;r++){
        cmp_value=this->record(r).value(field_name).toInt();
        if(value<cmp_value){
            value=cmp_value;
        }
    }

    return QString::number(value+1);
}

int lds_model_sqlrelationaltablemodel::find_column_exist(const QString &field_name, const QString &cmp_value)
{
    int value=-1;
    for(int r=0,r_count=this->rowCount();r<r_count;r++){
        if(cmp_value==this->record(r).value(field_name).toString()){
            value=r;
            break;
        }
    }
    return value;
}

QString lds_model_sqlrelationaltablemodel::sum_value_column(const QString &field_name)
{
    double value=0.0;
    for(int r=0,r_count=this->rowCount();r<r_count;r++){
        value+=this->record(r).value(field_name).toDouble();
    }
    return QString().sprintf("%.02f", value);
}

void lds_model_sqlrelationaltablemodel::save_set_header(const QString &field_name, const QString &text, bool iseditable)
{
    this->setHeaderData(this->fieldIndex(field_name), Qt::Horizontal,text,Qt::EditRole);
    if(iseditable){
        this->setHeaderData(this->fieldIndex(field_name), Qt::Horizontal,QPixmap(":/image/widgets_pic/model_pen"),Qt::DecorationRole);
    }
    save_set_header_saver<<text;
}

int lds_model_sqlrelationaltablemodel::save_set_header_saver_index(const QString &headername)
{
    for(int index=0,count=save_set_header_saver.count();index<count;index++){
        if(save_set_header_saver[index]==headername){
            return index;
        }
    }
    return -1;
}

void lds_model_sqlrelationaltablemodel::set_last_data(const QString &field_name, const QString &text)
{
    if(this->rowCount() > 0){
        this->setData(this->index(this->rowCount()-1, this->fieldIndex(field_name)), text);
        qDebug() << field_name << text;
    } else {
        qDebug() << "max row is not exist!";
    }
}

void lds_model_sqlrelationaltablemodel::set_row_data(int row, const QString &field_name, const QString &text)
{
    if(this->rowCount() > row){
        this->setData(this->index(row, this->fieldIndex(field_name)), text);
        qDebug() << field_name << text;
    } else {
        qDebug() << "max row is not exist!";
    }
}

QVariant lds_model_sqlrelationaltablemodel::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVariant _dtmp=QSqlRelationalTableModel::headerData(section, orientation, Qt::DisplayRole);
    QVariant _d=QSqlRelationalTableModel::headerData(section, orientation, role);

    if(orientation==Qt::Horizontal){
        if(role==Qt::TextColorRole){
            foreach(SQL_HEADERLABEL sql_label, sql_headerlabels){
                if(_dtmp.toString().contains(sql_label.headerlabel)){
                    return QColor("blue");
                }
            }
        }
    }
    if(orientation==Qt::Vertical){
        if(role==Qt::DisplayRole){
            if(_d.toString()=="*"){
                return "+";
            }
            if(_d.toString()=="!"){
                return "-";
            }
        }
        if(role==Qt::TextColorRole){
            if(_dtmp.toString()=="!"
                    ||_dtmp.toString()=="*"){
                return QColor("red");
            }
        }
    }
    return _d;
}


QString lds_model_sqlrelationaltablemodel::selectStatement() const
{
    QString sqlselect = QSqlRelationalTableModel::selectStatement();
    QStringList from01 =  QSqlRelationalTableModel::selectStatement().split("FROM");
    if(from01.size()==2){
        sqlselect=from01[0];

        //删除第一个额外列的 前一个,之后的所有数据，可能会和下面的又重复
        if(sql_headerlabels.count()>0){
            int index;
            if((index=sqlselect.indexOf(sql_headerlabels.first().headerlabel)) != -1){
                sqlselect.truncate(sqlselect.lastIndexOf(",", index));
            }
        }

        //获取拼接好的数据
        foreach(SQL_HEADERLABEL sql_label, sql_headerlabels){
            sqlselect += ",(" +sql_label.sql +") as "+sql_label.headerlabel;
        }
        sqlselect += " FROM " + from01[1];
    }

    return sqlselect;
}

bool lds_model_sqlrelationaltablemodel::updateRowInTable(int row, const QSqlRecord &values)
{
    return QSqlRelationalTableModel::updateRowInTable(row, values);
}

bool lds_model_sqlrelationaltablemodel::insertRowIntoTable(const QSqlRecord &values)
{
    return QSqlRelationalTableModel::insertRowIntoTable(values);
}

QString lds_model_sqlrelationaltablemodel::orderByClause() const
{
    QString _r=QSqlRelationalTableModel::orderByClause();
    foreach(SQL_HEADERLABEL sql_label, sql_headerlabels){
        if(_r.contains(sql_label.headerlabel)){
            return "";
        }
    }
    return _r;
}


Qt::ItemFlags lds_model_sqlrelationaltablemodel::FilterProxyModel::flags(const QModelIndex &index) const
{
    //新增加的都不可以编辑
    QString _r=this->headerData(index.column(), Qt::Horizontal,  Qt::DisplayRole).toString();
    foreach(SQL_HEADERLABEL sql_label, _parent->sql_headerlabels){
        if(_r.contains(sql_label.headerlabel)){
            return Qt::ItemIsEnabled|Qt::ItemIsSelectable;
        }
    }
    //headerdata有icon的可以编辑
    if(this->headerData(index.column(), Qt::Horizontal,Qt::DecorationRole).isValid()){
        return Qt::ItemIsEnabled|Qt::ItemIsSelectable|Qt::ItemIsEditable;
    }
    //默认都不可以编辑
    return Qt::ItemIsEnabled|Qt::ItemIsSelectable;
}
