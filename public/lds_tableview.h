#ifndef LDS_TABLEVIEW_H
#define LDS_TABLEVIEW_H

#include <QTableView>
#include <QStack>
#include "lds_tableview_repecify.h"

class lds_tableView : public QTableView
{
    Q_OBJECT
public:
    explicit lds_tableView(QWidget *parent = 0);
    void restore();
    void restore_rows();

    int getcurrow();
    QSet<int> getcurrows();

    void select_beforerow();
    int get_beforerow();
    virtual void setModel(QAbstractItemModel *model);
    virtual void setSelectionModel(QItemSelectionModel *selectionModel);

    QList<int> rePecifyHeader(const QStringList &tos);
    void update_paintinfo(const QString &value);
public slots:
    void clearSelection();
    int gonext();
    int goprevious();
signals:
    void selectchanged(int crow);
    void selectschanged(QSet<int> rows);

private slots:
    void saverow(const QItemSelection &selected, const QItemSelection &deselected);
private:
    int currow;
    QStack<int> stack_row;
    QSet<int> currows;
    
    //可拖动---------------begin
public:
    bool ismoving;

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *e);

private:
    QPoint pressp;
    int pressh,pressv;
    QString paintinfo;
    //可拖动---------------end

};

#endif // LDS_TABLEVIEW_H
