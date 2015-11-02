#include "w_i_inventory_paremset.h"
//库存参数设置
#include "ui_w_i_inventory_paremset_dialog.h"
#include "n_func.h"
#include "lds_messagebox.h"

w_i_inventory_paremset::w_i_inventory_paremset(QWidget *parent) :
    QDialog(parent),
    ui(new Ui_w_i_inventory_paremset_Dialog)
{
    ui->setupUi(this);

    ui->radioButton->setChecked(
                n_func::f_get_sysparm("store_print_total", "page")=="page");
    ui->radioButton_2->setChecked(
                n_func::f_get_sysparm("store_print_total", "page")=="table");

    ui->checkBox->setChecked(n_func::f_get_sysparm("sys_stock_price", "0")=="1");

    //! toolbar
    ui->widget->setup(QStringList() << tr("确定") << tr("取消"), Qt::AlignRight);
    connect(ui->widget->index_widget(tr("确定")),SIGNAL(clicked()),this,SLOT(took()));
    connect(ui->widget->index_widget(tr("取消")),SIGNAL(clicked()),this,SLOT(toexit()));

}

void w_i_inventory_paremset::took()
{
    QString errsql;
    errsql=n_func::f_set_sysparm("store_print_total", ui->radioButton->isChecked()?"page":"table", tr("库存单据打印合计金额位置"));
    errsql=n_func::f_set_sysparm("sys_stock_price", ui->checkBox->isChecked()?"1":"0", tr("用入库价更新采购价"));
    if(errsql.isEmpty())
        this->accept();
    else
        lds_messagebox::warning(this, tr("消息提示"),tr("数据库错误!"));
}

void w_i_inventory_paremset::toexit()
{
    this->reject();
}
