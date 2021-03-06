#ifndef LDS_TABLEVIEW_REPECIFY_H
#define LDS_TABLEVIEW_REPECIFY_H

#include <QObject>
#include <QHeaderView>
#include <QTableView>
#include <QDebug>

class lds_tableview_repecify{
public:
    lds_tableview_repecify();
    static QList<int> rePecifyHeader(QTableView *tableview, const QStringList &tos);

    static QString saveHeader(const QString &text, QStringList &ret);
};



#endif // LDS_TABLEVIEW_REPECIFY_H
