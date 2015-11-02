#include "lds_button_tiptext.h"
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QToolButton>
#include <QSqlQuery>
#include <QSqlRecord>
#include "backheader.h"
#include <QMenu>
#include <QWidgetAction>
#include <QLabel>
#include "lds_menu.h"

lds_button_tiptext::lds_button_tiptext(QWidget *parent) :
    QPushButton(parent)
{
    //menu
    lds_menu *menu=new lds_menu;
    QWidgetAction *widgetaction=new QWidgetAction(this);
    action=new QLabel(menu);
    widgetaction->setDefaultWidget(action);
    menu->addAction(widgetaction);
    this->setMenu(menu);

    action->setStyleSheet("color:blue;");
    action->setWordWrap(true);
}

void lds_button_tiptext::setLabelText(const QString &text)
{
    action->setText(text);
}
