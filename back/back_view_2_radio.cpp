#include "back_view_2_radio.h"
#include <QApplication>
#include <QDesktopWidget>
#include"backheader.h"

QList<QRadioButton *> back_view_2_radio::rbtnlist;
back_view_2_radio::back_view_2_radio(QWidget *parent) :
    QRadioButton(parent)
{
    rbtnlist.append(this);

}
