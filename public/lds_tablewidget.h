#ifndef LDS_TABLEWIDGET_H
#define LDS_TABLEWIDGET_H

#include <QTableWidget>
#include <QPainter>
#include <QStyledItemDelegate>
#include <QDebug>

class lds_tableWidget : public QTableWidget
{
    Q_OBJECT
public:
    class ItemDelegate:public QStyledItemDelegate{
    public:
        explicit ItemDelegate(QWidget *parent = 0);
    protected:

        int borderwidth;//border 宽度
        QString focusbordercolor;//border 色
        QString notfocusbordercolor;//border 色

        int margin;//icon 和 text 的margin

        int gridwidth;//grid 宽度
        QString gridcolor;//grid 色

        void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
        QTableWidget *_parent;
        friend class lds_tableWidget;
    };
public:
    explicit lds_tableWidget(QWidget *parent = 0);
    ItemDelegate *delegate;
};

#endif // LDS_TABLEWIDGET_H
