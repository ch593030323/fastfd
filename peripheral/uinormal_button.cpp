#include "uinormal_button.h"

uinormal_button::uinormal_button(QWidget *parent) :
    QPushButton(parent)
{
}
QSize uinormal_button::sizeHint() const
{
    QSize size=QPushButton::sizeHint();
    size.setWidth(qMax(60, size.width()) +20);

    return size;
}
