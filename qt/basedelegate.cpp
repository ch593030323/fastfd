#include "basedelegate.h"
#include <QCheckBox>
#include <QApplication>

void BaseDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QItemDelegate::paint(painter,option,index);

    int  cc = index.column();
    if(cc < untill) return;
    int checked;
    QString secs = index.data( Qt::DisplayRole).toString().trimmed();

    secs.toUpper() == "Y"?checked = 1:checked = 0;
    QStyleOptionButton *checkBoxOption = new QStyleOptionButton();
    checkBoxOption->state |= QStyle::State_Enabled;
    /*根据值Y/N判断是否选中*/
    checked?checkBoxOption->state |= QStyle::State_On:
            checkBoxOption->state |= QStyle::State_Off;

    /*返回QCheckBox位置*/
    QRect x = option.rect;
    QSize c = QCheckBox().sizeHint();
    x.setX(x.x() + (x.width()  - c.width()) / 2);
    x.setY(x.y() + (x.height() - c.height())/ 2);
    x.setSize(c);

    checkBoxOption->rect = x;

    /*绘制QCheckBox*/
    QApplication::style()->drawControl(QStyle::CE_CheckBox,checkBoxOption,painter);
}
