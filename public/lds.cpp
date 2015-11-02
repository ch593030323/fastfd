#include "lds.h"
#include <QApplication>
#include <QDesktopWidget>
lds::lds()
{

}

QPoint lds::getPopPoint(QWidget *parent, QWidget *popwidget)
{
    QPoint pos = (parent->layoutDirection() == Qt::RightToLeft) ? parent->rect().bottomRight() : parent->rect().bottomLeft();
    QPoint pos2 = (parent->layoutDirection() == Qt::RightToLeft) ? parent->rect().topRight() : parent->rect().topLeft();
    pos = parent->mapToGlobal(pos);
    pos2 = parent->mapToGlobal(pos2);
    QSize size = popwidget->sizeHint();
    QRect screen = QApplication::desktop()->availableGeometry(pos);
    if (parent->layoutDirection() == Qt::RightToLeft) {
        pos.setX(pos.x()-size.width());
        pos2.setX(pos2.x()-size.width());
        if (pos.x() < screen.left())
            pos.setX(qMax(pos.x(), screen.left()));
        else if (pos.x()+size.width() > screen.right())
            pos.setX(qMax(pos.x()-size.width(), screen.right()-size.width()));
    } else {
        if (pos.x()+size.width() > screen.right())
            pos.setX(screen.right()-size.width());
        pos.setX(qMax(pos.x(), screen.left()));
    }
    if (pos.y() + size.height() > screen.bottom())
        pos.setY(pos2.y() - size.height());
    else if (pos.y() < screen.top())
        pos.setY(screen.top());
    if (pos.y() < screen.top())
        pos.setY(screen.top());
    if (pos.y()+size.height() > screen.bottom())
        pos.setY(screen.bottom()-size.height());
    return pos;
}

