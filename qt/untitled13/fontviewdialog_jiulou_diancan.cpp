#include "fontviewdialog_jiulou_diancan.h"
#include "backheader.h"
#include <QSqlQuery>
#include <QHostInfo>
#include "n_func.h"
#include <QSqlRecord>
#include "ui_fontviewDialog_jiulou_bottom_dialog.h"
#include "lds_menu.h"
#include <QWidgetAction>
#include "bianma_cook.h"

fontviewDialog_jiulou_diancan::fontviewDialog_jiulou_diancan(QWidget *parent) :
    frontViewDialog_jiulou_dish_dialog(parent)
{
    QSqlQuery query;
    query.exec("select vch_company from cey_sys_company");
    query.next();
    QString info;
    info.append("<p style=\" margin:0px;\">"+tr("机号:")+query.record().value(0).toString()+"    "+"</p>");
    info.append("<p style=\" margin:0px;\">"+tr("登录用户:")+n_func::gs_opername+"    "+tr("门店:")+QHostInfo::localHostName()+"    "+"</p>");

    connect(ui->pushButton_allcuicai,SIGNAL(clicked()),this,SIGNAL(signal_alldesk_cuicai()));
    connect(ui->pushButton_canceltable,SIGNAL(clicked()),this,SIGNAL(signal_cancel_desk()));
    connect(ui->pushButton_change,SIGNAL(clicked()),this,SIGNAL(signal_changedish()));
    connect(ui->pushButton_changyong,SIGNAL(clicked()),this,SIGNAL(signal_changyong()));
    connect(ui->pushButton_cook,SIGNAL(clicked()),this,SIGNAL(signal_cook()));

    connect(ui->pushButton_cuicai,SIGNAL(clicked()),this,SIGNAL(signal_cuicai()));
    connect(ui->pushButton_quitdish,SIGNAL(clicked()),this,SIGNAL(signal_del()));
    //    connect(ui->pushButton_,SIGNAL(clicked()),this,SIGNAL(signal_desk_info()));
    connect(ui->pushButton_discount,SIGNAL(clicked()),this,SIGNAL(signal_discount()));
    connect(ui->pushButton_dishswitch,SIGNAL(clicked()),this,SIGNAL(signal_dish_switch_desk()));

    connect(ui->pushButton_add,SIGNAL(clicked()),this,SIGNAL(signal_jia()));
    connect(ui->pushButton_sub,SIGNAL(clicked()),this,SIGNAL(signal_jian()));
    connect(ui->pushButton_present,SIGNAL(clicked()),this,SIGNAL(signal_present()));
    connect(ui->pushButton_num,SIGNAL(clicked()),this,SIGNAL(signal_shuliang()));
    connect(ui->pushButton_tableswitch,SIGNAL(clicked()),this,SIGNAL(signal_switch_desk()));

    connect(ui->pushButton_dishtmp,SIGNAL(clicked()),this,SIGNAL(signal_timedish()));
    connect(ui->pushButton_prebill,SIGNAL(clicked()),this,SIGNAL(signal_yuding()));
    //    connect(ui->pushButton_,SIGNAL(clicked()),this,SIGNAL(signal_bianma()));
    connect(ui->pushButton_okbill,SIGNAL(clicked()),this,SIGNAL(signal_luodan()));
    connect(ui->pushButton_paybill,SIGNAL(clicked()),this,SIGNAL(signal_jisuan()));

    connect(ui->pushButton_quit,SIGNAL(clicked()),this,SIGNAL(signal_tuichu()));



    connect(ui->widget_1,SIGNAL(signal_text(QString)),this,SIGNAL(signal_grade0(QString)));
    connect(ui->widget_2,SIGNAL(signal_text(QString)),this,SIGNAL(signal_grade1(QString)));
    connect(ui->widget_3,SIGNAL(signal_text(QString)),this,SIGNAL(signal_grade2(QString)));

}

void fontviewDialog_jiulou_diancan::set_btn_del_text(const QString &text)
{
    ui->pushButton_quitdish->setText(text);
}

QString fontviewDialog_jiulou_diancan::get_btn_del_text()
{
    return ui->pushButton_quitdish->text();
}

void fontviewDialog_jiulou_diancan::pushbutton_other_hide()
{
    ui->pushButton_other->menu()->hide();
}

