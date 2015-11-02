#include "lds_toolbar_button.h"
#include <QVariant>

lds_toolbar_button::lds_toolbar_button(QWidget *parent):QPushButton(parent)
{
    this->setFixedHeight(40);
}

QSize lds_toolbar_button::sizeHint() const
{
    int menu_width=0;
    if(this->menu()){
        menu_width=20;
    }
    QSize size=QPushButton::sizeHint();
    size.setWidth(qMax(menu_width+60, size.width()) +20);

    return size;
}

void lds_toolbar_button::updateText(const QString &text)
{
    setText(text);
}
