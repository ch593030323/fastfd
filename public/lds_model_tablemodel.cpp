#include "lds_model_tablemodel.h"
#include <QColor>
#include <QtDebug>
#include <QSqlRecord>
#include <QPixmap>
#include <QSqlRecord>
#include <QSqlQuery>

lds_model_tablemodel::lds_model_tablemodel(QObject *parent, QSqlDatabase db) :
    QSqlTableModel(parent, db)
{
    fpmodel=new FilterProxyModel(this);
    fpmodel->setSourceModel(this);
}

bool lds_model_tablemodel::model_is_dirty()
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

QString lds_model_tablemodel::dirty_err_string()
{
    if(dirty_index.isValid()){
        return tr("%1,%2")//数据为脏!")
                .arg(dirty_index.row()+1)
                .arg(dirty_index.column()+1);

    }
    return "";
}

bool lds_model_tablemodel::same_column_exist(const QString &field_name, int row)
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

QString lds_model_tablemodel::max_column_add_1(const QString &field_name)
{
    int value=0;
    int cmp_value;
    if(this->rowCount()<=1){
        QSqlQuery query;
        query.exec(QString("select max(%1) from %2")
                   .arg(field_name)
                   .arg(this->tableName())
                   );
        query.next();
        value=query.record().value(0).toInt();
    } else {
        for(int r=0,r_count=this->rowCount();r<r_count;r++){
            cmp_value=this->record(r).value(field_name).toInt();
            if(value<cmp_value){
                value=cmp_value;
            }
        }
    }
    return QString::number(value+1);
}

int lds_model_tablemodel::find_column_exist(const QString &field_name, const QString &cmp_value)
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

QString lds_model_tablemodel::sum_value_column(const QString &field_name)
{
    double value=0.0;
    for(int r=0,r_count=this->rowCount();r<r_count;r++){
        value+=this->record(r).value(field_name).toDouble();
    }
    return QString().sprintf("%.02f", value);
}

void lds_model_tablemodel::save_set_header(const QString &field_name, const QString &text, bool iseditable)
{
    this->setHeaderData(this->fieldIndex(field_name), Qt::Horizontal,text,Qt::EditRole);
    if(iseditable){
        this->setHeaderData(this->fieldIndex(field_name), Qt::Horizontal,QPixmap(":/image/widgets_pic/model_pen"),Qt::DecorationRole);
    }
    save_set_header_saver<<text;
}

void lds_model_tablemodel::set_last_data(const QString &field_name, const QString &text)
{
    if(this->rowCount() > 0){
        this->setData(this->index(this->rowCount()-1, this->fieldIndex(field_name)), text);
        qDebug() << field_name << text;
    } else {
        qDebug() << "max row is not exist!";
    }
}

Qt::ItemFlags lds_model_tablemodel::FilterProxyModel::flags(const QModelIndex &index) const
{
    //headerdata有icon的可以编辑
    if(this->headerData(index.column(), Qt::Horizontal,Qt::DecorationRole).isValid()){
        return Qt::ItemIsEnabled|Qt::ItemIsSelectable|Qt::ItemIsEditable;
    }
    //默认都不可以编辑
    return Qt::ItemIsEnabled|Qt::ItemIsSelectable;
}
