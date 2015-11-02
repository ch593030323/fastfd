#include "uinormal_commbox.h"
#include <QtDebug>
#include <QStandardItemModel>

uinormal_commbox::uinormal_commbox(QWidget *parent) :
    QComboBox(parent)
{
    setFixedHeight(30);
}

void uinormal_commbox::addItems(const QStringList &texts)
{
    QStandardItemModel *commodel=new QStandardItemModel(this);
    foreach(QString text,texts){
        commodel->appendRow(new QStandardItem(text));
        commodel->setData(commodel->index(commodel->rowCount()-1, 0), QSize(100,30), Qt::SizeHintRole);
    }
    this->setModel(commodel);
}
