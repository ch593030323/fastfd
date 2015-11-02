#include "modify_view.h"
#include "ui_modify_view.h"
#include "lds_messagebox.h"


modify_view::modify_view(const QString &str,QWidget *parent) :
    _str(str),QDialog(parent),
    ui(new Ui::modify_view)
{
    ui->setupUi(this);
    //this->setWindowFlags(Qt::FramelessWindowHint|Qt::Dialog);this->setStyleSheet("QDialog{border:2px solid gray}");
    ui->lineEdit->setText(_str);
    ui->lineEdit->setFocus();
    btn_t_f = false;
}

modify_view::~modify_view()
{
    delete ui;
}

void modify_view::on_pushButton_4_clicked()
{
    ;
    if(ui->lineEdit->text().trimmed().isEmpty())
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("价格不能为空,请输入!"));

        return;
    }

    int i = ui->lineEdit->text().count(".");  //返回查找相同字符出现的次数
    int k = ui->lineEdit->text().count("-");
    if(ui->lineEdit->text().isEmpty() || i > 1 || k > 1)
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("修改数据格式有误,如23!"));

        return;
    }
    if(k == 1 && ui->lineEdit->text().left(1) != "-")
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("修改数据格式有误,如23!"));

        return;
    }
    jiage_m = ui->lineEdit->text();
    btn_t_f = true;
    this->close();
}

void modify_view::on_pushButton_5_clicked()
{
    btn_t_f = false;
    this->close();
}
