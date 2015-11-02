#include "public_cika_read_dialog.h"
#include "n_func.h"
#include "w_m_member_list.h"

public_cika_read_Dialog::public_cika_read_Dialog(QWidget *parent) :
    lds_dialog_input(parent)
{
    ui->label->setText(tr("请输入卡号"));
    ui->lineEdit->set_ignore_keyevent(n_func::f_m_get_parameter("input_memberno", "0")=="1");
    set_key_hidden(true);
}

public_cika_read_Dialog::~public_cika_read_Dialog()
{
}

QString public_cika_read_Dialog::getcardno()
{
    return ui->lineEdit->text();
}
