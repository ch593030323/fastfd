#ifndef FRONTVIEWDIALOG_TABLEVIEW_H
#define FRONTVIEWDIALOG_TABLEVIEW_H

#include <QTableView>
#include"lds_tableview_updown.h"
 #include <QStyledItemDelegate>
#include <QStandardItemModel>
class QItemSelectionModel;

class frontViewDialog_tableview : public lds_tableView_updown
{
    Q_OBJECT
public:
    explicit frontViewDialog_tableview(QWidget *parent = 0);
    virtual void setModel(QStandardItemModel *model);
signals:
    void curdishno_change(const QString &dishno);
public slots:
    void to_curdishno_change(int row);
    void to_payfor_info(const QString &info);//当model有数据change时，结账信息会自动消失的
    void to_clear_payfor_info();
private:
    QString paintinfo;

    class delegate:public QStyledItemDelegate{
    public:
        delegate(QTableView *t, QObject *parent = 0):QStyledItemDelegate(parent),tableview(t){}
        // painting
        void paint(QPainter *painter,
                   const QStyleOptionViewItem &option,
                   const QModelIndex &index) const;
        QTableView *tableview;
    };

};

#endif // FRONTVIEWDIALOG_TABLEVIEW_H
