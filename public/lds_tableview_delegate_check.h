#ifndef LDS_TABLEVIEW_DELEGATE_CHECK_H
#define LDS_TABLEVIEW_DELEGATE_CHECK_H
#include <QItemDelegate>




//tableview 的代理，实现checkbox的显示






class lds_tableview_delegate_check : public QItemDelegate
{
    Q_OBJECT
public:
    explicit lds_tableview_delegate_check(QVector<int> &check_pos, QObject *parent, bool ischecked=false);
    lds_tableview_delegate_check(int aftercolumn, QObject *parent, bool ischecked=false);//aftercolumn后的列都为checkbox
    ~lds_tableview_delegate_check();
    virtual void paint(QPainter * painter,const QStyleOptionViewItem & option, const QModelIndex & index) const;
    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const;
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void setClickable();
    bool clickable();
signals:
    void checkclick(const QModelIndex& index);
private slots:
    void inter_clicked(const QModelIndex& index);
private:
    QVector<int> _check_pos;
    bool ischeckable;
};

#endif // LDS_TABLEVIEW_DELEGATE_CHECK_H
