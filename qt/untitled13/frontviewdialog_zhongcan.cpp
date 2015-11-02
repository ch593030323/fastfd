#include "frontviewdialog_zhongcan.h"
#include "ui_frontViewDialog_zhongcan_dialog.h"

frontViewDialog_zhongcan::frontViewDialog_zhongcan(QWidget *parent) :
    QDialog(parent)
{
    ui = new Ui_frontViewDialog_zhongcan_Dialog;
    ui->setupUi(this);

    ui->widget->horizontal_count=3;
    ui->widget->vertical_count=1;
    //ui->widget->tool_size=QSize(150,50);
    ui->widget->add_item("0", tr("所有桌台(20/50)"), true, QPixmap(":/example/suoyou"));
    ui->widget->add_item("1", tr("大厅(10/35)"), true, QPixmap(":/example/dating"));
    ui->widget->add_item("2", tr("包厢(5/15)"), true, QPixmap(":/example/baoxiang"));
    //    ui->widget->generat_from_items();

    ui->widget_2->horizontal_count=1;
    ui->widget_2->vertical_count=6;
   // ui->widget_2->tool_size=QSize(100,50);
   // ui->widget_2->icon_size=QSize(30,30);
    ui->widget_2->add_item("0", tr("经理管理"), true, QPixmap(":/example/guanli"));
//    ui->widget_2->add_item("1", tr("设置"), true, QPixmap(":/example/shezhi"));
    ui->widget_2->add_item("2", tr("预定"), true, QPixmap(":/example/yuding"));
    ui->widget_2->add_item("3", tr("刷新"), true, QPixmap(":/example/shuaxin"));
    ui->widget_2->add_item("4", tr("营业数据"), true, QPixmap(":/example/danju"));
    ui->widget_2->add_item("5", tr("交班操作"), true, QPixmap(":/example/jiaoban"));
//    ui->widget_2->add_item("6", tr("手动输入"), true, QPixmap(":/example/shoudongshuru"));
    ui->widget_2->add_item("7", tr("开钱箱"), true, QPixmap(":/example/shoudongshuru"));
    ui->widget_2->generat_from_items();


    ui->widget_3->horizontal_count=6;
    ui->widget_3->vertical_count=4;
    //ui->widget_3->tool_size=QSize(100,100);
//    ui->widget_3->objectname="autoexpand_radius_0";
    //    ui->widget_3->is_can_released=true;
//    ui->widget_3->is_like_tab=false;

    for(int index=0;index<50;index++){
        ui->widget_3->add_item(QString::number(index), "005" +QString::number(index), true, QPixmap());
    }
    //    ui->widget_3->generat_from_items();

    this->setStyleSheet(
                "QPushButton#autoexpand{background-color: rgb(225, 221, 201);border:2px solid rgb(225, 221, 201);border-radius:10;font-weight:bold;}"
                "QPushButton#autoexpand:pressed{background-color:  rgb(251, 30, 114);}"

                "QPushButton#autoexpand_radius_0{background-color: rgb(225, 221, 201);border:0px;border-radius:0;}"
                "QPushButton#autoexpand_radius_0:pressed{background-color: rgb(249, 253, 167);border:0px;}"
                );

    connect(ui->widget,SIGNAL(signal_text(QString)),this,SIGNAL(signal_desk(QString)));
    connect(ui->widget_3,SIGNAL(signal_text(QString)),this,SIGNAL(signal_sub_desk(QString)));
    connect(ui->widget_2,SIGNAL(signal_text(QString)),this,SLOT(to_desk_sys_send(QString)));
    connect(ui->pushButton_tuichu,SIGNAL(clicked()),this,SIGNAL(signal_tuichu()));
}

void frontViewDialog_zhongcan::update_desk_info(const QString &void_num, const QString &use_num , const QString &oper, const QString &dt)
{
    if(!void_num.isEmpty())ui->label_void->setText(void_num);
    if(!use_num.isEmpty())ui->label_use->setText(use_num);
    if(!oper.isEmpty())ui->label_oper->setText(oper);
    if(!dt.isEmpty())ui->label_dt->setText(dt);
}

void frontViewDialog_zhongcan::to_desk(const QList<frontViewDialog_autoexpand::content_item_icon> &item_icon_list)
{
    ui->widget->clearall_item();
    foreach(frontViewDialog_autoexpand::content_item_icon item_icon, item_icon_list){
        ui->widget->add_item(item_icon.index, item_icon.text, item_icon.is_use_pixmap, item_icon.icon);
    }
    ui->widget->generat_from_items();
}

void frontViewDialog_zhongcan::to_sub_desk(const QList<frontViewDialog_autoexpand::content_item_icon> &item_icon_list)
{
    ui->widget_3->clearall_item();
    foreach(frontViewDialog_autoexpand::content_item_icon item_icon, item_icon_list){
        ui->widget_3->add_item(item_icon.index, item_icon.text, item_icon.is_use_pixmap, item_icon.icon);
    }
    ui->widget_3->generat_from_items();

}

void frontViewDialog_zhongcan::to_desk_sys(const QList<frontViewDialog_autoexpand::content_item_icon> &item_icon_list)
{
    ui->widget_2->clearall_item();
    foreach(frontViewDialog_autoexpand::content_item_icon item_icon, item_icon_list){
        ui->widget_2->add_item(item_icon.index, item_icon.text, item_icon.is_use_pixmap, item_icon.icon);
    }
    ui->widget_2->generat_from_items();

}

void frontViewDialog_zhongcan::to_desk_sys_send(const QString &text)
{
    if(text=="0"){
        emit signal_jinliguanli();
    } else if(text=="1"){
        emit signal_shezhi();
    } else if(text=="2"){
        emit signal_yuding();
    } else if(text=="3"){
        emit signal_shuaxin();
    } else if(text=="4"){
        emit signal_yingyeshuju();
    }
    else if(text=="5"){
        emit signal_jiaoban();
    } else if(text=="6"){
        emit signal_shoudongshuru();
    } else if(text=="7"){
        emit signal_kaiqianxiang();
    }
}
