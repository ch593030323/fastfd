#ifndef BASEDELEGATE_H
#define BASEDELEGATE_H

#include <QItemDelegate>

class BaseDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit BaseDelegate(int end, QObject *parent = 0) : QItemDelegate(parent),untill(end){}
    virtual void paint(QPainter * painter,const QStyleOptionViewItem & option, const QModelIndex      & index) const;

private:
    int untill;
};

#endif // BASEDELEGATE_H
