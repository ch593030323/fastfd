#include "w_scr_dish_weixinwaimai.h"
#include "ui_w_scr_dish_weixinwaimai_dialog.h"
#include "lds_model_sqlrelationaltablemodel.h"
#include <QSqlRecord>
#include "takeout_cook.h"
#include "backheader.h"
#include "lds_messagebox.h"
#include "lds_dialog_input.h"

w_scr_dish_weixinwaimai::w_scr_dish_weixinwaimai(fast_order_2 *parent):
    QDialog(parent),
    ui(new Ui_w_scr_dish_weixinwaimai_dialog),
    _parent(parent)
{
    ui->setupUi(this);
    //init
    tablemodel=new lds_model_sqlrelationaltablemodel;
    ui->tableView->setModel(tablemodel);
    tablemodel_2=new lds_model_sqlrelationaltablemodel;
    ui->tableView_2->setModel(tablemodel_2);

    tablemodel->setTable("cey_web_order_info");
    tablemodel_2->setTable("cey_web_order_dish");

    tablemodel->save_set_header("sn",tr("订单编号"));
    tablemodel->save_set_header("tel",tr("手机号码"));
    ui->tableView->rePecifyHeader(tablemodel->save_set_header_saver);
    tablemodel_2->save_set_header("dishNo",tr("菜品编号"));
    tablemodel_2->save_set_header("dishName",tr("菜品名称"));
    tablemodel_2->save_set_header("number",tr("菜品数量"));
    tablemodel_2->save_set_header("price",tr("价格"));
    tablemodel_2->save_set_header("suitFlag",tr("套菜标志"));
    tablemodel_2->save_set_header("note",tr("备注"));
    ui->tableView_2->rePecifyHeader(tablemodel_2->save_set_header_saver);

    //    connect(ui->tableView,SIGNAL(selectchanged(int)),this,SLOT(torefresh()));
    //!after
    ui->comboBox->addItem(tr("未受理"),0);
    ui->comboBox->addItem(tr("已受理"),1);
    ui->comboBox->addItem(tr("交易结束"),2);
    ui->dateTimeEdit->setDateTime(QDateTime(QDate::currentDate(),QTime(0,0,0)));
    ui->dateTimeEdit_2->setDateTime(QDateTime(QDate::currentDate(),QTime(23,59,59)));
    ui->tableView->setEditTriggers(QTableView::NoEditTriggers);
    ui->tableView_2->setEditTriggers(QTableView::NoEditTriggers);
    ui->widget->setup(QStringList() << tr("拒绝")<<tr("下单") << tr("取消"), Qt::AlignRight);

    connect(ui->widget->index_widget(tr("拒绝")),SIGNAL(clicked()),this,SLOT(torefuse()));
    connect(ui->widget->index_widget(tr("下单")),SIGNAL(clicked()),this,SLOT(totake_waimai()));
    connect(ui->widget->index_widget(tr("取消")),SIGNAL(clicked()),this,SLOT(toexit()));
    connect(ui->pushButton_search,SIGNAL(clicked()),this,SLOT(tosearch()));
    connect(ui->tableView,SIGNAL(clicked(QModelIndex)),this,SLOT(torefresh()));
    tosearch(0);
}

void w_scr_dish_weixinwaimai::torefuse()
{
    if(ui->tableView->currentIndex().row()<0) {
        lds_messagebox::warning(this, tr("消息提示"),tr("没有选中行"));
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
        if(1==lds_messagebox::warning(this, tr("消息提示"),tr("确认拒绝?"), tr("确认"), tr("'取消"))){
            return;
        }
        //    01->22
        if(w_sys_manage_cloudsync::up_weixin_change_takkeoutstate(
                    tablemodel->record(ui->tableView->currentIndex().row()).value("sn").toString(),
                    "22",//交易结束-交易拒绝
                    inputdialog._text
                    )){
            tablemodel->removeRow(ui->tableView->currentIndex().row());
            tablemodel->submitAll();
            lds_messagebox::warning(this, tr("消息提示"),tr("外卖拒绝成功!"));
        }
    }
}

void w_scr_dish_weixinwaimai::totake_waimai()
{
    //前台有点菜信息
    if(_parent->modelBase.rowCount() > 0){
        lds_messagebox::warning(this, tr("消息提示"),tr("前台有点菜信息,操作取消"));
        return;
    }
    //获取订单信息
    if(tablemodel_2->rowCount() <=0){
        lds_messagebox::warning(this, tr("消息提示"),tr("没有菜品信息"));
        return;
    }
    if(1==lds_messagebox::warning(this, tr("消息提示"),tr("确认下单?"), tr("确认"), tr("'取消"))){
        return;
    }
    for(int row=0,rowcount=tablemodel_2->rowCount();row<rowcount; row++){
        int num=tablemodel_2->record(row).value("number").toInt();
        while(num--)_parent->addGood(tablemodel_2->record(row).value("dishNo").toString(), false);
    }
    //弹出对应的窗口
    takeout_cook takedialog(_parent);
    takedialog.ui->lineEdit_8->setText(tablemodel->record(ui->tableView->currentIndex().row()).value("tel").toString());
    takedialog.ui->lineEdit_9->setText(tablemodel->record(ui->tableView->currentIndex().row()).value("orderer").toString());
    takedialog.ui->lineEdit_10->setText(tablemodel->record(ui->tableView->currentIndex().row()).value("address").toString());
    QTimer::singleShot(100, &takedialog, SLOT(on_pushButton_clicked()));//送餐员
    if(QDialog::Accepted==backheader::static_dialogexec(takedialog, tr("微信外卖-")+
                                                        tablemodel->record(ui->tableView->currentIndex().row()).value("sn").toString())){
        tablemodel->removeRow(ui->tableView->currentIndex().row());
        tablemodel->submitAll();
    }
    _parent->list_clear();
}

void w_scr_dish_weixinwaimai::toexit()
{
    this->reject();
}

void w_scr_dish_weixinwaimai::tosearch(int row)
{
    tablemodel->setFilter(
                "eatTime<='"+ui->dateTimeEdit_2->dateTime().toString("yyyy-MM-dd hh:mm:ss")+"'  and "
                "eatTime>='"+ui->dateTimeEdit->dateTime().toString("yyyy-MM-dd hh:mm:ss")+"'  and "
                "orderState like '"+ui->comboBox->itemData(ui->comboBox->currentIndex()).toString()+"%' and "
                "orderType='1' ");
    tablemodel->select();
    if(row==-1)
        ui->tableView->restore();
    else{
        ui->tableView->selectRow(row);
        ui->tableView->resizeColumnsToContents();
    }
    torefresh();
}

void w_scr_dish_weixinwaimai::torefresh()
{
    tablemodel_2->setFilter("orderSn='"+tablemodel->record(ui->tableView->currentIndex().row()).value("sn").toString()+"' ");
    tablemodel_2->select();
    ui->tableView_2->restore();

    ui->lineEdit_orderer->setText(tablemodel->record(ui->tableView->currentIndex().row()).value("orderer").toString());
    ui->lineEdit_eatTime->setText(tablemodel->record(ui->tableView->currentIndex().row()).value("eatTime").toDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    ui->lineEdit_tabelAreaNo->setText(tablemodel->record(ui->tableView->currentIndex().row()).value("tableAreaNo").toString());
    ui->lineEdit_eatTableNo->setText(tablemodel->record(ui->tableView->currentIndex().row()).value("eatTableNo").toString());
    ui->lineEdit_eatPels->setText(tablemodel->record(ui->tableView->currentIndex().row()).value("eatPels").toString());

    ui->lineEdit_payType->setText(tablemodel->record(ui->tableView->currentIndex().row()).value("payType").toString()=="0"?tr("当面支付"):tr("在线支付"));
    ui->lineEdit_payState->setText(tablemodel->record(ui->tableView->currentIndex().row()).value("payState").toString()=="0"?tr("未支付"):tr("已支付"));
    ui->lineEdit_orderType->setText(tablemodel->record(ui->tableView->currentIndex().row()).value("orderType").toString());
    switch(ui->lineEdit_orderType->text().toInt()){
    case 0:ui->lineEdit_orderType->setText(tr("普通订单"));break;
    case 1:ui->lineEdit_orderType->setText(tr("外卖订单"));break;
    case 2:ui->lineEdit_orderType->setText(tr("订桌订单"));break;
    }

    ui->lineEdit_orderState->setText(tablemodel->record(ui->tableView->currentIndex().row()).value("orderState").toString());

    ui->widget->index_widget(tr("下单"))->setEnabled(false);

    ui->widget->index_widget(tr("拒绝"))->setEnabled(false);

    switch(ui->lineEdit_orderState->text().toInt()){
    case 1:ui->lineEdit_orderState->setText(tr("未受理-订单"));
        ui->widget->index_widget("下单")->setEnabled(true);
        ui->widget->index_widget("拒绝")->setEnabled(true);
        break;
    case 2:ui->lineEdit_orderState->setText(tr("未受理-退单"));break;
    case 11:ui->lineEdit_orderState->setText(tr("已受理-订单"));break;
    case 12:ui->lineEdit_orderState->setText(tr("已受理-已制作派送"));break;
    case 21:ui->lineEdit_orderState->setText(tr("交易结束-交易成功"));break;
    case 22:ui->lineEdit_orderState->setText(tr("交易结束-交易拒绝"));break;
    case 23:ui->lineEdit_orderState->setText(tr("交易结束-退订失败"));break;
    case 24:ui->lineEdit_orderState->setText(tr("交易结束-退订成功"));break;
    case 25:ui->lineEdit_orderState->setText(tr("交易结束-交易响应超时"));break;
    }
    qDebug() << "-------------------------------------------";
    ui->lineEdit_address->setText(tablemodel->record(ui->tableView->currentIndex().row()).value("address").toString());
}
