#ifndef LDS_TABLEVIEW_DELEGATE_DATAEDIT_H
#define LDS_TABLEVIEW_DELEGATE_DATAEDIT_H

#include <QItemDelegate>
#include <QMap>

class lds_tableview_delegate_dataedit : public QItemDelegate
{
    Q_OBJECT
public:
    explicit lds_tableview_delegate_dataedit(const QString &format="yyyy-MM-dd hh:mm:ss" , QObject *parent = 0);
protected:
    QWidget *createEditor(QWidget *aParent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
private:
    QString _format;
};

#endif // LDS_TABLEVIEW_DELEGATE_DATAEDIT_H
