#ifndef LDS_TABLEVIEW_SUM_H
#define LDS_TABLEVIEW_SUM_H
#include <QWidget>
#include <QStandardItemModel>
#include <QDebug>
#include <QItemSelection>
#include <QItemDelegate>
#include <QPainter>
#include <QtGlobal>
#include <QAbstractItemView>



//有汇总的tableview





class QTableView;
class lds_tableView;
class QLineEdit;
class QAbstractItemModel;
class QScrollBar;
class QHeaderView;

class lds_tableview_sum : public QWidget
{
    Q_OBJECT
public:
    explicit lds_tableview_sum(QWidget *parent = 0);
    void setModel(QAbstractItemModel *model);
    lds_tableView *tableView;//tableview 本身的函数在这里操作
    QTableView *lineView;

    QString updateSum(int column);//用的是普通model，即model和linemodel的列数一致
    QString updateSum(int column, const QString &value);//用的是普通model，即model和linemodel的列数一致

    void setlineAlignment(Qt::AlignmentFlag flag);//设置总计line的对齐方式，会立刻更新
    Qt::AlignmentFlag LineAlignment();//返回line的对齐方式

    void setEditTriggers(QAbstractItemView::EditTriggers triggers);
private slots:
    void update_tableview_bar(int min, int max);
    void update_lineview_header(int logicalIndex, int oldSize, int newSize);
    void update_lineview_selection(const QItemSelection &selected, const QItemSelection &deselected);
    void update_tableview_verticalheader();
    void linerefresh();
    void scrollbarinfo(int step);
private:
    class RedModel:public QStandardItemModel{
    public:
        RedModel(QObject *parent = 0):
            QStandardItemModel(parent),
            lineflag(Qt::AlignLeft){}
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
        Qt::AlignmentFlag lineflag;
    };
    class ItemDelegate : public QItemDelegate
    {
    public:
        explicit ItemDelegate(QObject *parent = 0):QItemDelegate(parent){}
        virtual void paint(QPainter * painter,const QStyleOptionViewItem & option, const QModelIndex & index) const;
    };

    RedModel *linemodel;
    QAbstractItemModel *parentmodel;
};
#endif // LDS_TABLEVIEW_SUM_H
