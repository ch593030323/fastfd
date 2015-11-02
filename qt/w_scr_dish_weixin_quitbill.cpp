#include "w_scr_dish_weixin_quitbill.h"
#include "ui_w_scr_dish_weixin_quitbill_dialog.h"
#include "n_func.h"
#include "w_rpt_report.h"
#include"lds_dialog_input.h"
#include "lds_messagebox.h"
#include "lds_messagebox.h"


w_scr_dish_weixin_quitbill::w_scr_dish_weixin_quitbill(QWidget *parent) :
    QDialog(parent),
    ui(new Ui_w_scr_dish_weixin_quitbill_dialog)
{
    ui->setupUi(this);
    ui->frame->setFixedWidth(w_rpt_report::frame_left_fixed_width);

    querymodel=new QSqlQueryModel;
    ui->tableView->setModel(querymodel);
    ui->comboBox->addItems(QStringList()<< tr("订桌退单查询") << tr("外卖退单查询") );//1代表外卖;2代表订桌
    //! init
    ui->dateTimeEdit->setDateTime(QDateTime(n_func::f_get_sysdatetime().date(), QTime(0,0,0)));
    ui->dateTimeEdit_2->setDateTime(QDateTime(n_func::f_get_sysdatetime().date(), QTime(23,59,59)));
    //! toolbar
    ui->widget->setup(QStringList()<<tr("拒绝退单")<<tr("确认退单") << tr("查询")<<tr("退出"), Qt::AlignTop);
    connect(ui->widget->index_widget(tr("查询")),SIGNAL(clicked()),this,SLOT(toselect()));
    connect(ui->widget->index_widget(tr("退出")),SIGNAL(clicked()),this,SLOT(toexit()));
    connect(ui->widget->index_widget(tr("拒绝退单")),SIGNAL(clicked()),this,SLOT(torefuse()));
    connect(ui->widget->index_widget(tr("确认退单")),SIGNAL(clicked()),this,SLOT(toconfirm()));


}

void w_scr_dish_weixin_quitbill::toselect()
{
    querymodel->setQuery(QString("select sn as "+tr("订单编号")+", eatTime as "+tr("就餐时间")+", eatPels as "+tr("就餐人数")+", eatTableNo as "+tr("餐桌编号")+", tableAreaNo as "+tr("餐区编号")+",tel as "+tr("手机号码")+",orderer as "+tr("预订人")+",(case sex  when '1' then '"+tr("男")+"' else '"+tr("女")+"' end) as "+tr("性别")+",note as "+tr("订单备注")+",address as "+tr("地址")+",sendTime as "+tr("送达时间")+
                                 " from cey_web_order_info where (orderType='%3') and orderState='02' and date >='%1' and date <='%2' ")
                         .arg(ui->dateTimeEdit->dateTime().toString("yyyy-MM-dd hh:mm:ss"))
                         .arg(ui->dateTimeEdit_2->dateTime().toString("yyyy-MM-dd hh:mm:ss"))
                         .arg(ui->comboBox->currentIndex())
                         );
    qDebug() << querymodel->query().lastQuery();
}

void w_scr_dish_weixin_quitbill::toexit()
{
    this->reject();
}

void w_scr_dish_weixin_quitbill::torefuse()
{
    if(ui->tableView->currentIndex().row()>=0){
        if(1==lds_messagebox::warning(this, tr("消息提示"),tr("确认拒绝?"), tr("确认"), tr("'取消"))){
            return;
        }
        lds_dialog_input inputdialog;
        inputdialog.set_key_hidden(true);
        inputdialog.set_pop_enable(false);
        inputdialog.ui->label->setText(tr("请输入拒绝理由"));
        if(QDialog::Accepted==backheader::static_dialogexec(inputdialog, tr("提示"))){
            if(inputdialog._text.isEmpty()){
                lds_messagebox::warning(this, tr("消息提示"),tr("拒绝理由不能为空!"));
                return;
            }

            //    02->23
            if(w_sys_manage_cloudsync::up_weixin_change_takkeoutstate(
                        querymodel->record(ui->tableView->currentIndex().row()).value(tr("订单编号")).toString(),
                        "23",//交易结束-退订失败
                        inputdialog._text
                        )){
                lds_messagebox::warning(this, tr("消息提示"),tr("退订已拒绝!"));
            }
        }
    }
}

void w_scr_dish_weixin_quitbill::toconfirm()
{
    if(ui->tableView->currentIndex().row()>=0){
        //    02->24
        if(1==lds_messagebox::warning(this, tr("消息提示"),tr("确认退单?"), tr("确认"), tr("'取消"))){
            return;
        }
        //!1===========begin
        QSqlDatabase::database().transaction();
        QSqlQuery query;
        if(query.exec(QString("update cey_u_togo set ch_state ='P' ,vch_operID ='%1',dt_operdate='%2' where ch_togono like '%%3' ")
                      .arg(n_func::gs_operid)
                      .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))
                      .arg(querymodel->record(ui->tableView->currentIndex().row()).value(tr("订单编号")).toString())
                      ))
        {
            qDebug() << query.lastQuery();
        }
        else
        {
            QSqlDatabase::database().rollback();
            lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_u_togo数据失败!"));
            return;
        }

        if(query.prepare(QString("update cey_u_orderdish set num_back =num_num,num_num =0,vch_back_operID ='%1',dt_back_operdate='%2' where ch_togono like'%%3';")
                         .arg(n_func::gs_operid)
                         .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))
                         .arg(querymodel->record(ui->tableView->currentIndex().row()).value(tr("订单编号")).toString())
                         ))
        {
            qDebug() << query.lastQuery();
        }
        else
        {
            QSqlDatabase::database().rollback();
            lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_u_orderdish数据失败!"));
            return;
        }
        //!1===========end


        //    02->24
        if(w_sys_manage_cloudsync::up_weixin_change_takkeoutstate(
                    querymodel->record(ui->tableView->currentIndex().row()).value(tr("订单编号")).toString(),
                    "24"//交易结束-退订成功
                    )){
            QSqlDatabase::database().commit();
            lds_messagebox::warning(this, tr("消息提示"),tr("退订成功!"));
            return;
        }
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("退订失败!"));
    }
}
