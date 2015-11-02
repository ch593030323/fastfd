#include "lds_tableview_delegate_com.h"
#include <QDebug>
#include <QDateTimeEdit>
#include "lds_combobox.h"

lds_tableview_delegate_com::lds_tableview_delegate_com(QMap<int, QStringList> &composmap, QObject *parent) :
    QItemDelegate(parent),_composmap(composmap)
{
}

QWidget *lds_tableview_delegate_com::createEditor(QWidget *aParent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (_composmap.keys().contains(index.column())) {
        lds_ComboBox *editor = new lds_ComboBox(aParent);
        editor->addItems(_composmap[index.column()]);
        editor->setCurrentIndex(qMax(editor->findText(index.data().toString()),0));
        return editor;
    }

    return QItemDelegate::createEditor(aParent, option, index);
}

void lds_tableview_delegate_com::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    lds_ComboBox *dateEditor = qobject_cast<lds_ComboBox *>(editor);
    if(dateEditor){
        model->setData(index, dateEditor->currentIndex()+1);

    } else {
        QItemDelegate::setModelData(editor, model, index);
    }
}

void lds_tableview_delegate_com::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    lds_ComboBox *dateEditor = qobject_cast<lds_ComboBox *>(editor);
    if(dateEditor){

    } else {
        QItemDelegate::setEditorData(editor, index);
    }
}
