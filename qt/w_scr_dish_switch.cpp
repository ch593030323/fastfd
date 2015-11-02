#include "w_scr_dish_switch.h"

w_scr_dish_switch::w_scr_dish_switch(QWidget *parent) :
    lds_dialog_input(parent)
{
    set_second_hidden(false);
    ui->label->setText(tr("接班人"));
    ui->label_2->setText(tr("密码"));
    ui->lineEdit_2->setEchoMode(QLineEdit::Password);
}
