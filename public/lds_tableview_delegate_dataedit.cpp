#include "lds_tableview_delegate_dataedit.h"
#include <QDebug>
#include <QDateTimeEdit>
#include "lds_datetimeedit.h"


lds_tableview_delegate_dataedit::lds_tableview_delegate_dataedit(const QString &format, QObject *parent)
    :QItemDelegate(parent),
    _format(format)
{

}

QWidget *lds_tableview_delegate_dataedit::createEditor(QWidget *aParent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(!QDateTime::fromString(index.data().toString(), _format).isNull()){
        lds_datetimeEdit *editor=new lds_datetimeEdit(aParent);
        if(editor)editor->setDisplayFormat(_format);
        return editor;
    }
    return QItemDelegate::createEditor(aParent, option, index);
}

void lds_tableview_delegate_dataedit::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if(!QDateTime::fromString(index.data().toString(), _format).isNull()){
        lds_datetimeEdit *editor0=qobject_cast<lds_datetimeEdit *>(editor);
        if(editor0)model->setData(index, editor0->time().toString(_format));
    } else {
        QItemDelegate::setModelData(editor, model, index);
    }

}

void lds_tableview_delegate_dataedit::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if(!QDateTime::fromString(index.data().toString(), _format).isNull()){
        lds_datetimeEdit *editor0=qobject_cast<lds_datetimeEdit *>(editor);
        if(editor0)editor0->setDateTime(index.data().toDateTime());
    } else {
        QItemDelegate::setEditorData(editor, index);
    }
}
