#ifndef LDS_TREEWIDGETITEM_H
#define LDS_TREEWIDGETITEM_H

#include <QTreeWidgetItem>

class lds_treeWidgetItem : public QTreeWidgetItem
{
public:
    explicit  lds_treeWidgetItem(const QStringList &strings, int type = Type);

};

#endif // LDS_TREEWIDGETITEM_H
