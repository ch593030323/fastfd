#ifndef LDS_TABLEVIEW_DELEGATE_TIME_H
#define LDS_TABLEVIEW_DELEGATE_TIME_H
#include <QItemDelegate>


//    tablemodel->setData(tablemodel->index(rowcount, 2), "00:00"); 时间需要手动指定
class lds_tableview_delegate_time:public QItemDelegate
{
    Q_OBJECT
public:
    explicit lds_tableview_delegate_time(const QString &format="hh:mm:ss" , QObject *parent = 0);
    void setDisplayFormat(const QString &format);
protected:
    QWidget *createEditor(QWidget *aParent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
private:
    QString _format;
};

#endif // LDS_TABLEVIEW_DELEGATE_TIME_H
