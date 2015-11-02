#include "lds_tableview_delegate_time.h"

#include <QDebug>
#include <QDateTimeEdit>
#include <QTimeEdit>
#include "lds_datetimeedit.h"

lds_tableview_delegate_time::lds_tableview_delegate_time(const QString &format, QObject *parent)
    :QItemDelegate(parent),
      _format(format)
{

}

QWidget *lds_tableview_delegate_time::createEditor(QWidget *aParent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(!QTime::fromString(index.data().toString(), _format).isNull()){
        lds_datetimeEdit *editor=new lds_datetimeEdit(aParent);
        if(editor){
            editor->setDisplayFormat(_format);
            return editor;
        }
    }
    return QItemDelegate::createEditor(aParent, option, index);
}

void lds_tableview_delegate_time::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if(!QTime::fromString(index.data().toString(), _format).isNull()){
        lds_datetimeEdit *editor0=qobject_cast<lds_datetimeEdit *>(editor);
        if(editor0){
            model->setData(index, editor0->dateTime());
            return;
        }
    }
    QItemDelegate::setModelData(editor, model, index);

}

void lds_tableview_delegate_time::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if(!QTime::fromString(index.data().toString(), _format).isNull()){
        lds_datetimeEdit *editor0=qobject_cast<lds_datetimeEdit *>(editor);
        if(editor0){
            editor0->setTime(index.data().toTime());
        }
    }
    QItemDelegate::setEditorData(editor, index);
}
