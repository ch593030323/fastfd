#include "lds_menu.h"
//#include "qmenu_p.h"
#include <qdebug.h>

lds_menu::lds_menu(QWidget *parent) : QMenu(parent)
{
    this->setWindowFlags(Qt::Popup);
//    this->setTearOffEnabled(true);
}

lds_menu::~lds_menu()
{

}

void lds_menu::popup(const QPoint &pos, QAction *at)
{
    QMenu::popup(pos, at);
}

QAction *lds_menu::exec(const QPoint &pos, QAction *at)
{
    QMenu::exec(pos, at);
}

void lds_menu::changeEvent(QEvent *e)
{
    QMenu::changeEvent(e);
}

void lds_menu::mouseReleaseEvent(QMouseEvent *e)
{
        QMenu::mouseReleaseEvent(e);
}

void lds_menu::mousePressEvent(QMouseEvent *e)
{
    QMenu::mousePressEvent(e);
}

void lds_menu::mouseMoveEvent(QMouseEvent *e)
{
    return QMenu::mouseMoveEvent(e);
}

