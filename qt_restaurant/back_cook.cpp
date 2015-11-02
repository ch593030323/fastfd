#include "back_cook.h"
#include "ui_back_cook.h"
#include <QSqlQueryModel>
#include <QSqlRecord>
#include "back_causes.h"
#include "lds_messagebox.h"
#include "lds_messagebox.h"
#include "backheader.h"
#include "w_scr_dish_restaurant_dialog.h"

back_cook::back_cook(const QString & name,const QString bianma,QWidget *parent) :
    _name(name),_bianma(bianma),QDialog(parent),
    ui(new Ui::back_cook)
{
    ui->setupUi(this);
    //this->setWindowFlags(Qt::FramelessWindowHint|Qt::Dialog);this->setStyleSheet("QDialog{border:2px solid gray}");
    init_control();
    ui->frame->setup(QStringList()<<tr("整单退")<<tr("确定")<<tr("取消"), Qt::AlignRight);
    connect(ui->frame->index_widget(tr("整单退")),SIGNAL(clicked()),this,SLOT(on_pushButton_16_clicked()));
    connect(ui->frame->index_widget(tr("确定")),SIGNAL(clicked()),this,SLOT(on_pushButton_7_clicked()));
    connect(ui->frame->index_widget(tr("取消")),SIGNAL(clicked()),this,SLOT(on_pushButton_15_clicked()));

}

back_cook::~back_cook()
{
    delete ui;
}

void back_cook::init_control()
{
    ui->lineEdit->setEnabled(false);
    ui->lineEdit_2->setEnabled(false);
    ui->lineEdit_3->setEnabled(false);
    ui->lineEdit_4->setEnabled(false);
    ui->lineEdit->setText(_name);
    ui->lineEdit_5->setFocus();

    QSqlQueryModel model;
    model.setQuery(QString("select * from cey_u_orderdish where ch_billno = '%1'and ch_tableno = '%2' and ch_dishno = '%3' ").arg(w_scr_dish_restaurant_dialog::table_danhao).arg(frontViewDialog_virtual::cur_desk_no).arg(_bianma));

    ui->lineEdit_2->setText(QString().sprintf("%0.2f",model.record(0).value(8).toFloat()));
    ui->lineEdit_3->setText(QString().sprintf("%0.2f",model.record(0).value(9).toFloat()));
    float num = 0.00;
    num = model.record(0).value(8).toFloat() - model.record(0).value(9).toFloat();
    ui->lineEdit_4->setText(QString().sprintf("%0.2f",num));
    ui->lineEdit_5->setText(QString().sprintf("%0.2f",num));
}

void back_cook::on_pushButton_15_clicked()
{
    this->close();
    back_all = false;
    bakc_one = false;
}

void back_cook::on_pushButton_16_clicked()
{
    ;
    QSqlQueryModel model;
    model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='orderdish_back' "));
    QString causes = model.record(0).value(0).toString();

    if(causes == "1")
    {
        if(ui->lineEdit_6->text().trimmed().isEmpty())
        {
            lds_messagebox::warning(this, tr("消息提示"),tr("请输入退菜原因!"));
            
            return;
        }
    }
    if(0==lds_messagebox::warning(this, tr("消息提示"),tr("是否整单退菜？"),tr("确认"),tr("取消")))
    {
        cook_causes = ui->lineEdit_6->text();
        back_all = true;
        bakc_one = false;
    }
    else
    {
        back_all = false;
        bakc_one = false;
    }
    this->close();
}

void back_cook::on_pushButton_7_clicked()
{
    QSqlQueryModel model;
    model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='orderdish_back' "));
    QString causes = model.record(0).value(0).toString();

    int i = ui->lineEdit_5->text().count(".");  //返回查找相同字符出现的次数
    int k = ui->lineEdit_5->text().count("-");
    ;
    if(ui->lineEdit_5->text().isEmpty() || i > 1 || k > 1)
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("修改数据格式有误,如23!"));
        
        return;
    }
    if(k == 1 && ui->lineEdit_5->text().left(1) != "-")
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("修改数据格式有误,如23!"));
        
        return;
    }
    if(ui->lineEdit_5->text().toFloat() <= 0)
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("菜品数量必须大于0!"));
        
        return;
    }

    float num = 0.00;
    num = ui->lineEdit_4->text().toFloat() - ui->lineEdit_5->text().toFloat();
    if(num < 0)
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("退菜数量不能大于可退数量!"));
        
        return;
    }

    if(causes == "1")
    {
        if(ui->lineEdit_6->text().trimmed().isEmpty())
        {
            lds_messagebox::warning(this, tr("消息提示"),tr("请输入退菜原因!"));
            
            return;
        }
    }
    if(0==lds_messagebox::warning(this, tr("消息提示"),tr("是否按指定数零退菜？"),tr("确认"),tr("取消")))
    {
        back_num = ui->lineEdit_5->text().toFloat();
        cook_causes = ui->lineEdit_6->text();
        back_all = false;
        bakc_one = true;
    }
    else
    {
        back_all = false;
        bakc_one = false;
    }
    this->close();
}

void back_cook::on_pushButton_8_clicked()
{
    back_causes causes(this);
    backheader::static_dialogexec(causes, tr("退菜原因"));
    ui->lineEdit_6->setText(causes.causes);
}
