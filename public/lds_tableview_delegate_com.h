#ifndef LDS_TABLEVIEW_DELEGATE_COM_H
#define LDS_TABLEVIEW_DELEGATE_COM_H

#include <QItemDelegate>
#include <QMap>

class lds_tableview_delegate_com : public QItemDelegate
{
    Q_OBJECT
public:
    explicit lds_tableview_delegate_com(QMap<int, QStringList> &composmap/*第几列，该列的comitems*/, QObject *parent = 0);


    QWidget *createEditor(QWidget *aParent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;

private:
    QMap<int, QStringList> _composmap;

};

#endif // LDS_TABLEVIEW_DELEGATE_COM_H
