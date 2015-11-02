#include "lds_treewidgetitem.h"

lds_treeWidgetItem::lds_treeWidgetItem(const QStringList &strings, int type):
    QTreeWidgetItem(strings, type)
{
   setSizeHint(0, QSize(60,30));
}
