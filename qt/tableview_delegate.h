#ifndef TABLEVIEW_DELEGATE_H
#define TABLEVIEW_DELEGATE_H

#include <QItemDelegate>

class Tableview_delegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit Tableview_delegate(QObject *parent = 0) : QItemDelegate(parent){}
    virtual void paint(QPainter * painter,const QStyleOptionViewItem & option, const QModelIndex      & index) const;

private:
    int untill;
};

#endif // TABLEVIEW_DELEGATE_H
