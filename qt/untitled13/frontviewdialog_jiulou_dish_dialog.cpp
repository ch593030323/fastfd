#include "frontviewdialog_jiulou_dish_dialog.h"
#include "backheader.h"
#include "lds_menu.h"
#include <QWidgetAction>
#include "bianma_cook.h"

frontViewDialog_jiulou_dish_dialog::frontViewDialog_jiulou_dish_dialog(QWidget *parent) :
    frontViewDialog_virtual(parent),
    ui(new Ui_frontViewDialog_jiulou_dish_dialog)
{
    ui->setupUi(this);
    {
        lds_menu *menu=new lds_menu;
        QWidgetAction *widgetaction=new QWidgetAction(ui->pushButton_other);
        widgetaction->setDefaultWidget(ui->frame_other_menu);
        menu->addAction(widgetaction);
        ui->pushButton_other->setMenu(menu);
    }
    {
        lds_menu *menu=new lds_menu;
        QWidgetAction *widgetaction=new QWidgetAction(ui->pushButton_bianma);
        bianma_cook *bianma = new bianma_cook(menu);
        bianma->setFixedSize(500,300);
        widgetaction->setDefaultWidget(bianma);
        menu->addAction(widgetaction);
        ui->pushButton_bianma->setMenu(menu);
        connect(bianma,SIGNAL(edit_text(QString)),this,SIGNAL(signal_bianma_diancai(QString)));
    }

//    connect(ui->widget_1,SIGNAL(signal_text(QString)),this,SIGNAL(signal_grade0(QString)));
//    connect(ui->widget_2,SIGNAL(signal_text(QString)),this,SIGNAL(signal_grade1(QString)));
//    connect(ui->widget_3,SIGNAL(signal_text(QString)),this,SIGNAL(signal_grade2(QString)));


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
    connect(ui->lineEdit_scan,SIGNAL(returnPressed()),this,SLOT(to_scanbar()));

}

frontViewDialog_jiulou_dish_dialog::~frontViewDialog_jiulou_dish_dialog()
{
    delete ui;
}
void frontViewDialog_jiulou_dish_dialog::dish_detail_clear()
{

}

int frontViewDialog_jiulou_dish_dialog::dish_detail_currow()
{

}

void frontViewDialog_jiulou_dish_dialog::set_btn_del_text(const QString &text)
{
    ui->pushButton_quitdish->setText(text);
}

QString frontViewDialog_jiulou_dish_dialog::get_btn_del_text()
{
    return ui->pushButton_quitdish->text();
}

void frontViewDialog_jiulou_dish_dialog::pushbutton_other_hide()
{
    ui->pushButton_other->menu()->hide();
}

void frontViewDialog_jiulou_dish_dialog::listpayforclear()
{
}

float frontViewDialog_jiulou_dish_dialog::get_cur_num()
{
    return _num;
}

void frontViewDialog_jiulou_dish_dialog::to_scanbar()
{
    if(ui->lineEdit_scan->text().isEmpty()) return;
    emit signal_scanbar(ui->lineEdit_scan->text());
    ui->lineEdit_scan->clear();
}

void frontViewDialog_jiulou_dish_dialog::to_grade0(const QList<frontViewDialog_autoexpand::content_item_icon> &item_icon_list)
{
}

void frontViewDialog_jiulou_dish_dialog::to_grade1(const QList<frontViewDialog_autoexpand::content_item_icon> &item_icon_list)
{

}

void frontViewDialog_jiulou_dish_dialog::to_grade2(const QList<frontViewDialog_autoexpand::content_item_icon> &item_icon_list)
{
}

void frontViewDialog_jiulou_dish_dialog::to_add_dish_detail(const QList<frontViewDialog_list::list_item> &list_item_list)
{


}
void frontViewDialog_jiulou_dish_dialog::to_del_dish_detail(int row)
{

}

void frontViewDialog_jiulou_dish_dialog::to_update_dish_detail(const frontViewDialog_list::list_item &list_item, int row)
{

}

void frontViewDialog_jiulou_dish_dialog::to_dish_title(double num, double total)
{
    if(num>=0){ui->label_num->setText(tr("数量")+QString().sprintf("%.03f", num));_num=num;}
    if(total>=0)ui->label_total->setText(tr("金额")+QString().sprintf("%.02f", total));
}

void frontViewDialog_jiulou_dish_dialog::to_dish_title_other(const QString &num, const QString &total)
{
    if(!num.isEmpty())ui->label_num->setText(num);
    if(!total.isEmpty())ui->label_total->setText(total);
}
