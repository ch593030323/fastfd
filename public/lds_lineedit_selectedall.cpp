#include "lds_lineedit_selectedall.h"

lds_lineedit_selectedall::lds_lineedit_selectedall(QWidget *parent) :
    QLineEdit(parent)
{
    connect(this,SIGNAL(getfocus()),this,SLOT(selectAll()),Qt::QueuedConnection);
}

void lds_lineedit_selectedall::focusInEvent(QFocusEvent *e)
{
    QLineEdit::focusInEvent(e);
    emit getfocus();
}
