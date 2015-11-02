#include "w_scr_dish_weixin_getbill.h"

w_scr_dish_weixin_getbill::w_scr_dish_weixin_getbill(QWidget *parent) :
    QDialog(parent),
    ui(new Ui_w_scr_dish_weixin_getbill_dialog)
{
    ui->setupUi(this);
    ui->widget->setup(QStringList() << tr("确定")<< tr("取消"), Qt::AlignRight);
    connect(ui->widget->index_widget(tr("确定")),SIGNAL(clicked()),this,SLOT(accept()));
    connect(ui->widget->index_widget(tr("取消")),SIGNAL(clicked()),this,SLOT(reject()));
}
