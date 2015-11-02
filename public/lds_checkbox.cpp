#include "lds_checkbox.h"
#include <QDebug>

lds_CheckBox::lds_CheckBox(QWidget *parent) :
    QCheckBox(parent)
{
}

void lds_CheckBox::setChecked1(QString c)
{
    setChecked(c=="Y");
}

QString lds_CheckBox::isChecked1()
{
    return isChecked()? "Y":"N";
}
