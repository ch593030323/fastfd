#include "lds_combobox.h"
#include <QAbstractItemView>
#include <QTableView>
#include <QStandardItemModel>
#include <qdebug.h>

lds_ComboBox::lds_ComboBox(QWidget *parent) :
    QComboBox(parent)
{
    connect(this,SIGNAL(currentIndexChanged(int)),this,SLOT(to_currentchange(int)));
}

void lds_ComboBox::addItems(const QStringList &texts)
{
    QStandardItemModel *commodel=new QStandardItemModel(this);
    foreach(QString text,texts){
        commodel->appendRow(new QStandardItem(text));
        commodel->setData(commodel->index(commodel->rowCount()-1, 0), QSize(100,35), Qt::SizeHintRole);
    }
    this->setModel(commodel);
}

void lds_ComboBox::addItems(const QList<lds_ComboBox::PAIRLIST> &texts)
{
    QStandardItemModel *commodel=new QStandardItemModel(this);
    foreach(PAIRLIST text,texts){
        commodel->appendRow(new QStandardItem(text.key));
        commodel->setData(commodel->index(commodel->rowCount()-1, 0), QSize(100,35), Qt::SizeHintRole);
        commodel->setData(commodel->index(commodel->rowCount()-1, 0), text.usrvalue, Qt::UserRole);
    }
    this->setModel(commodel);
}

void lds_ComboBox::setindex_c1(const QString c1_text)
{
    for(int r=0,r_count=this->count(); r<r_count;r++){
        if(this->itemText(r).endsWith(QString("(%1)").arg(c1_text))){
            setCurrentIndex(r);
            return;
        }
    }
    setCurrentIndex(-1);
}

void lds_ComboBox::setcomusrdata(const QString usrtext)
{
    qDebug() << __FILE__<<__FUNCTION__<<__LINE__<< usrtext;
    setCurrentIndex(-1);
    QAbstractItemModel *model=this->model();
    if(model){
        for(int r=0,r_count=model->rowCount();r<r_count;r++){
            if(model->data(model->index(r, 0), Qt::UserRole).toString()==usrtext){
                setCurrentIndex(r);
                break;
            }
        }
    }
}

//void lds_ComboBox::setcolumn2index1(const QString &index)
//{
//    setCurrentIndex(-1);
//    QAbstractItemModel *model=this->model();
//    if(model
//            &&model->columnCount()==2){
//        for(int r=0,r_count=model->rowCount();r<r_count;r++){
//            if(model->data(model->index(r, 0)).toString()==index){
//                setCurrentIndex(r);
//                break;
//            }
//        }
//    }
//}

QString lds_ComboBox::column2index1()
{
    int currindex=this->currentIndex();
    QAbstractItemModel *model=this->model();
    if(model
            &&model->columnCount()>=2
            &&currindex>=0){
        return model->data(model->index(currindex, 0)).toString();
    }

    return "";
}

QString lds_ComboBox::comusrdata()
{
    qDebug() << "there is begin";
    QAbstractItemModel *model=this->model();
    if(model
            &&model->rowCount()>0){
        qDebug() << "there is end2"<<model->data(model->index(this->currentIndex(), 0), Qt::UserRole).toString();
        return model->data(model->index(this->currentIndex(), 0), Qt::UserRole).toString();
    }
    qDebug() << "there is end2";
    return "";
}

void lds_ComboBox::to_currentchange(int index)
{
    QAbstractItemModel *model=this->model();
    if(!model) return;

    if(model->columnCount()>=2){
        emit currentIndexChanged1(model->index(index, 1).data().toString());
    }
    if(model->columnCount()>=3){
        emit currentIndexChanged2(model->index(index, 2).data().toString());
    }
    if(model->columnCount()>=4){
        emit currentIndexChanged3(model->index(index, 3).data().toString());
    }
    if(model->columnCount()>=5){
        emit currentIndexChanged4(model->index(index, 4).data().toString());
    }
}
