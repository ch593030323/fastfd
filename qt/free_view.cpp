#include "free_view.h"
#include "ui_free_view.h"
#include <QSqlQueryModel>
#include "lds_messagebox.h"
#include <QSqlRecord>
#include <QDateTime>
#include <QDebug>
#include"backheader.h"

free_view::free_view(const QString &title,const QString & str,QWidget *parent) :
    _title(title),num(str),QDialog(parent),
    ui(new Ui::free_view)
{
    ui->setupUi(this);
    btn_t_f = false;
    line2_num = 0.00;
    init_control();
}

free_view::~free_view()
{
    delete ui;
}

void free_view::init_control()
{
    ui->lineEdit->setEnabled(false);
    ui->lineEdit->setText(num);
    ui->lineEdit_2->setText(num);
    ui->lineEdit_4->setEchoMode(QLineEdit::Password);
    if(_title == tr("免单操作"))
    {
        ui->lineEdit_2->setEnabled(false);
    }
}

void free_view::on_pushButton_4_clicked()//确定
{
    //先判断权限
    QSqlQueryModel model;
    model.setQuery(QString("Select vch_operID,vch_password from cey_sys_operator where ch_flag = 'Y' "));
    ;
    if(ui->lineEdit_3->text().isEmpty())
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("操作员代码不能为空!"));
        
        return;
    }

    if(ui->lineEdit_3->text() != "0000")
    {
        int k = 0;
        for(int i = 0; i < model.rowCount(); i++)
        {
            if(model.record(i).value(0).toString() == ui->lineEdit_4->text().trimmed())
            {
                k++;
                if(ui->lineEdit_4->text().trimmed() != model.record(i).value(1).toString())
                {
                    lds_messagebox::warning(this, tr("消息提示"),tr("操作密码错误!"));
                    
                    return;
                }
            }
        }

        if(k == 0)
        {
            lds_messagebox::warning(this, tr("消息提示"),tr("操作员代码不存在!"));
            
            return;
        }
    }
    else
    {
        if(ui->lineEdit_4->text().trimmed() != "0000")
        {
            lds_messagebox::warning(this, tr("消息提示"),tr("操作密码错误!"));
            
            return;
        }
    }

    if(ui->lineEdit_2->text().toFloat() > ui->lineEdit->text().toFloat())
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("抹零金额不能大于结算金额!"));
        
        return;
    }

    oper_id = ui->lineEdit_3->text().trimmed();
    line2_num = ui->lineEdit_2->text().toFloat();
    beizhu = ui->lineEdit_5->text();
    QSqlQueryModel model1;
    model1.setQuery("Select NOW()");
    time = model1.record(0).value(0).toDateTime().toString("yyyy-MM-dd hh:mm:ss");

    btn_t_f = true;
    this->close();
}

void free_view::on_pushButton_5_clicked()//取消
{
    btn_t_f = false;
    this->close();
}
