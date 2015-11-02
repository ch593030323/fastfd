#include "frontviewdialog_zhongcan_diancai.h"
#include "ui_frontViewDialog_list_item_dialog.h"
#include "lds_menu.h"
#include <QWidgetAction>

frontViewDialog_zhongcan_diancai::frontViewDialog_zhongcan_diancai(QWidget *parent) : QDialog(parent)
{
    ui=new Ui_frontviewdialog_zhongcan_diancai_Dialog;
    ui->setupUi(this);

    //menu
    lds_menu *menu=new lds_menu;
    QWidgetAction *widgetaction=new QWidgetAction(ui->pushButton_other);
    widgetaction->setDefaultWidget(ui->widget_other_menu);
    menu->addAction(widgetaction);
    ui->pushButton_other->setMenu(menu);

    ui->label_deskno->setText("002");
    ui->label_deskoperid->setText("XX");
    ui->label_deskperson->setText("XXX");
    ui->label_opemtime->setText("2015-05-25 14:23:58");
    ui->label_openbill->setText("A00820150524");
    ui->label_openperson->setText("2");

    ui->widget->horizontal_count=5;
    ui->widget->vertical_count=1;
   // ui->widget->tool_size=QSize(75,40);
    ui->widget->add_item("0", tr("中餐"), true, QPixmap(":/example/zhongcan"));
    ui->widget->add_item("1", tr("西餐"), true, QPixmap(":/example/xican"));
    ui->widget->add_item("2", tr("糖类"), true, QPixmap(":/example/tanglei"));
    ui->widget->add_item("3", tr("甜品"), true, QPixmap(":/example/tianpin"));
    ui->widget->add_item("4", tr("酒水"), true, QPixmap(":/example/zhongcan"));
    ui->widget->add_item("5", tr("烟酒"), true, QPixmap(":/example/zhongcan"));
    ui->widget->generat_from_items();

    ui->widget_2->horizontal_count=5;
    ui->widget_2->vertical_count=1;
    //ui->widget_2->tool_size=QSize(75,40);
    ui->widget_2->add_item("0", tr("牛排"));
    ui->widget_2->add_item("1", tr("意大利面"));
    ui->widget_2->add_item("2", tr("烧烤"));
    ui->widget_2->add_item("3", tr("沙拉"));
    ui->widget_2->add_item("4", tr("烩菜"));
    ui->widget_2->add_item("5", tr("浓汤"));
    ui->widget_2->add_item("6", tr("面包"));
    ui->widget_2->generat_from_items();

    ui->widget_3->horizontal_count=5;
    ui->widget_3->vertical_count=4;
   // ui->widget_3->tool_size=QSize(75,75);
    ui->widget_3->add_item("0", tr("西冷\n牛排"), true, QPixmap(":/example/zhongcan"));
    ui->widget_3->add_item("1", tr("意大\n利面"), true, QPixmap(":/example/zhongcan"));
    ui->widget_3->add_item("2", tr("菲力\n牛排"), true, QPixmap(":/example/zhongcan"));
    ui->widget_3->add_item("3", tr("丁骨\n牛排"), true, QPixmap(":/example/zhongcan"));
    ui->widget_3->add_item("4", tr("皇家\n牛排"), true, QPixmap(":/example/zhongcan"));
    ui->widget_3->add_item("5", tr("沙湖\n牛排"), true, QPixmap(":/example/zhongcan"));
    ui->widget_3->generat_from_items();

    ui->frame_title->setStyleSheet("background: rgb(92, 86, 70);color:white;");
    //    ui->listWidget->test();

    ui->pushButton_alldesk_cuicai->setObjectName("frontview");connect(ui->pushButton_alldesk_cuicai,SIGNAL(clicked()),this,SLOT(to_btn_text()));
    ui->pushButton_cancel_desk->setObjectName("frontview");connect(ui->pushButton_cancel_desk,SIGNAL(clicked()),this,SLOT(to_btn_text()));
    ui->pushButton_changedish->setObjectName("frontview");connect(ui->pushButton_changedish,SIGNAL(clicked()),this,SLOT(to_btn_text()));
    ui->pushButton_changyong->setObjectName("frontview");connect(ui->pushButton_changyong,SIGNAL(clicked()),this,SLOT(to_btn_text()));
    ui->pushButton_cook->setObjectName("frontview");connect(ui->pushButton_cook,SIGNAL(clicked()),this,SLOT(to_btn_text()));

    ui->pushButton_cuicai->setObjectName("frontview");connect(ui->pushButton_cuicai,SIGNAL(clicked()),this,SLOT(to_btn_text()));
    ui->pushButton_del->setObjectName("frontview");connect(ui->pushButton_del,SIGNAL(clicked()),this,SLOT(to_btn_text()));
    ui->pushButton_desk_info->setObjectName("frontview");connect(ui->pushButton_desk_info,SIGNAL(clicked()),this,SLOT(to_btn_text()));
    ui->pushButton_discount->setObjectName("frontview");connect(ui->pushButton_discount,SIGNAL(clicked()),this,SLOT(to_btn_text()));
    ui->pushButton_dish_switch_desk->setObjectName("frontview");connect(ui->pushButton_dish_switch_desk,SIGNAL(clicked()),this,SLOT(to_btn_text()));

    ui->pushButton_jia->setObjectName("frontview");connect(ui->pushButton_jia,SIGNAL(clicked()),this,SLOT(to_btn_text()));
    ui->pushButton_jian->setObjectName("frontview");connect(ui->pushButton_jian,SIGNAL(clicked()),this,SLOT(to_btn_text()));
    ui->pushButton_present->setObjectName("frontview");connect(ui->pushButton_present,SIGNAL(clicked()),this,SLOT(to_btn_text()));
    ui->pushButton_shuliang->setObjectName("frontview");connect(ui->pushButton_shuliang,SIGNAL(clicked()),this,SLOT(to_btn_text()));
    ui->pushButton_switch_desk->setObjectName("frontview");connect(ui->pushButton_switch_desk,SIGNAL(clicked()),this,SLOT(to_btn_text()));

    ui->pushButton_timedish->setObjectName("frontview");connect(ui->pushButton_timedish,SIGNAL(clicked()),this,SLOT(to_btn_text()));
    ui->pushButton_other->setObjectName("frontview");
    ui->pushButton_yuding->setObjectName("backview_bottom");connect(ui->pushButton_yuding,SIGNAL(clicked()),this,SLOT(to_btn_text()));
    ui->pushButton_bianma->setObjectName("backview_bottom");connect(ui->pushButton_bianma,SIGNAL(clicked()),this,SLOT(to_btn_text()));
    ui->pushButton_tuichu->setObjectName("backview_bottom");connect(ui->pushButton_tuichu,SIGNAL(clicked()),this,SLOT(to_btn_text()));
    ui->pushButton_jiesuan->setObjectName("backview_bottom");connect(ui->pushButton_jiesuan,SIGNAL(clicked()),this,SLOT(to_btn_text()));
    ui->pushButton_luodan->setObjectName("backview_bottom");connect(ui->pushButton_luodan,SIGNAL(clicked()),this,SLOT(to_btn_text()));


    this->setStyleSheet(
                "QPushButton#frontview{background-color: rgb(251, 6, 116);border:1px solid rgb(251, 6, 116); border-radius:0;}"
                "QPushButton#frontview:default{background-color: transparent;}"

                "QPushButton#autoexpand{background-color: rgb(225, 221, 201);color:rgb(225, 221, 201);border:0px;border-radius:10;}"
                "QPushButton#autoexpand:pressed{background-color: rgb(255, 139, 8);color:rgb(255, 139, 8);border:0px;}"

                "QPushButton#backview_bottom{background-color: rgb(92, 86, 70);border:0px; color:white;border-radius:10;}"
                "QPushButton#backview_bottom:pressed{background-color: rgb(251, 6, 116);border:0px; color:white;}"
                "QPushButton#backview_bottom:default{background-color: rgb(251, 6, 116);border:0px; color:white;}"
                );






    ui->listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    connect(ui->widget,SIGNAL(signal_text(QString)),this,SIGNAL(signal_grade0(QString)));
    connect(ui->widget_2,SIGNAL(signal_text(QString)),this,SIGNAL(signal_grade1(QString)));
    connect(ui->widget_3,SIGNAL(signal_text(QString)),this,SIGNAL(signal_grade2(QString)));

}

frontViewDialog_zhongcan_diancai::~frontViewDialog_zhongcan_diancai()
{

}

void frontViewDialog_zhongcan_diancai::dish_detail_clear()
{
    ui->listWidget->clear();
}

int frontViewDialog_zhongcan_diancai::dish_detail_currow()
{
    return ui->listWidget->currentRow();
}

QString frontViewDialog_zhongcan_diancai::get_btn_del_text()
{
    return ui->pushButton_del->text();
}

void frontViewDialog_zhongcan_diancai::set_btn_del_text(const QString &text)
{
    ui->pushButton_del->setText(text);
}


void frontViewDialog_zhongcan_diancai::to_grade0(const QList<frontViewDialog_autoexpand::content_item_icon> &item_icon_list)
{
    ui->widget->clearall_item();
    foreach(frontViewDialog_autoexpand::content_item_icon item_icon, item_icon_list){
        ui->widget->add_item(item_icon.index, item_icon.text, item_icon.is_use_pixmap, item_icon.icon);
    }
    ui->widget->generat_from_items();
}

void frontViewDialog_zhongcan_diancai::to_grade1(const QList<frontViewDialog_autoexpand::content_item_icon> &item_icon_list)
{
    ui->widget_2->clearall_item();
    foreach(frontViewDialog_autoexpand::content_item_icon item_icon, item_icon_list){
        ui->widget_2->add_item(item_icon.index, item_icon.text, item_icon.is_use_pixmap, item_icon.icon);
    }
    ui->widget_2->generat_from_items();

}

void frontViewDialog_zhongcan_diancai::to_grade2(const QList<frontViewDialog_autoexpand::content_item_icon> &item_icon_list)
{
    ui->widget_3->clearall_item();
    foreach(frontViewDialog_autoexpand::content_item_icon item_icon, item_icon_list){
        ui->widget_3->add_item(item_icon.index, item_icon.text, item_icon.is_use_pixmap, item_icon.icon);
    }
    ui->widget_3->generat_from_items();

}

void frontViewDialog_zhongcan_diancai::to_dish_detail(const QList<frontViewDialog_list::list_item> &list_item_list)
{
    foreach(frontViewDialog_list::list_item list_item, list_item_list){
        QListWidgetItem *listitem=new QListWidgetItem(ui->listWidget);
        listitem->setBackground(Qt::transparent);
        ui->listWidget->addItem(listitem);
        frontviewdialog_frame_list_item *w=new frontviewdialog_frame_list_item(listitem);
        w->setFixedHeight(60);
        listitem->setSizeHint(QSize(100, w->height()));
        ui->listWidget->setItemWidget(listitem, w);
        w->fui->label_p->setText(list_item._no);
        w->fui->label_p0->setText(list_item._dishno);
        w->fui->label_p1->setText(list_item._dishname);
        w->fui->label_p2->setText(list_item._price);
        w->fui->label_p3->setText(list_item._num);
        w->fui->label_p4->setText(list_item._total);

        w->p5_clear();
        if(list_item._suit=="P"){
            w->p5_append(tr("主"));
        } else if(list_item._suit=="Y"){
            w->p5_append(tr("子"));
        }
        if(list_item._present=="Y"){
            w->p5_append(tr("赠"));
        }
        if(list_item._special=="Y"){
            w->p5_append(tr("特"));
        }
        w->p5_append(list_item._cook);
    }
    ui->listWidget->setCurrentRow(ui->listWidget->count()-1);
}

void frontViewDialog_zhongcan_diancai::to_desk_detail(const QStringList &list_item)
{
    if(list_item.count() == 6){
        ui->label_deskno->setText(list_item[0]);
        ui->label_deskoperid->setText(list_item[1]);
        ui->label_deskperson->setText(list_item[2]);
        ui->label_opemtime->setText(list_item[3]);
        ui->label_openbill->setText(list_item[4]);
        ui->label_openperson->setText(list_item[5]);
    }
}

void frontViewDialog_zhongcan_diancai::to_add_dish_detail(const QList<frontViewDialog_list::list_item> &list_item_list)
{
    foreach(frontViewDialog_list::list_item list_item, list_item_list){
        QListWidgetItem *listitem=new QListWidgetItem(ui->listWidget);
        listitem->setBackground(Qt::transparent);
        ui->listWidget->addItem(listitem);
        frontviewdialog_frame_list_item *w=new frontviewdialog_frame_list_item(listitem);
        w->setFixedHeight(60);
        listitem->setSizeHint(QSize(100, w->height()));
        ui->listWidget->setItemWidget(listitem, w);
        w->fui->label_p->setText(list_item._no);
        w->fui->label_p0->setText(list_item._dishno);
        w->fui->label_p1->setText(list_item._dishname);
        w->fui->label_p2->setText(list_item._price);
        w->fui->label_p3->setText(list_item._num);
        w->fui->label_p4->setText(list_item._total);

        w->p5_clear();
        if(list_item._suit=="P"){
            w->p5_append(tr("主"));
        } else if(list_item._suit=="Y"){
            w->p5_append(tr("子"));
        }
        if(list_item._present=="Y"){
            w->p5_append(tr("赠"));
        }
        if(list_item._special=="Y"){
            w->p5_append(tr("特"));
        }
        w->p5_append(list_item._cook);

    }
    ui->listWidget->setCurrentRow(ui->listWidget->count()-1);
}

void frontViewDialog_zhongcan_diancai::to_del_dish_detail(int row)
{
    delete ui->listWidget->takeItem(row);
}

void frontViewDialog_zhongcan_diancai::to_update_dish_detail(const frontViewDialog_list::list_item &list_item, int row)
{
    frontviewdialog_frame_list_item *w=qobject_cast<frontviewdialog_frame_list_item *>(ui->listWidget->itemWidget(ui->listWidget->item(row)));
    if(w){
        if(!list_item._no.isEmpty())w->fui->label_p->setText(list_item._no);
        if(!list_item._dishno.isEmpty())w->fui->label_p0->setText(list_item._dishno);
        if(!list_item._dishname.isEmpty())w->fui->label_p1->setText(list_item._dishname);
        if(!list_item._price.isEmpty())w->fui->label_p2->setText(list_item._price);
        if(!list_item._num.isEmpty())w->fui->label_p3->setText(list_item._num);
        if(!list_item._total.isEmpty())w->fui->label_p4->setText(list_item._total);

        w->p5_clear();
        if(list_item._suit=="P"){
            w->p5_append(tr("主"));
        } else if(list_item._suit=="Y"){
            w->p5_append(tr("子"));
        }
        if(list_item._present=="Y"){
            w->p5_append(tr("赠"));
        }
        if(list_item._special=="Y"){
            w->p5_append(tr("特"));
        }
        w->p5_append(list_item._cook);
    }
}


void frontViewDialog_zhongcan_diancai::to_update_desk_info(const QString &label_deskno, const QString &label_deskoperid, const QString &label_deskperson, const QString &label_opemtime, const QString &label_openbill, const QString &label_openperson)
{
    ui->label_deskno->setText(label_deskno);//餐桌号
    ui->label_deskoperid->setText(label_deskoperid);//服务员
    ui->label_deskperson->setText(label_deskperson);//人数
    ui->label_opemtime->setText(label_opemtime);//开台时间
    ui->label_openbill->setText(label_openbill);//开台单号
    ui->label_openperson->setText(label_openperson);//开台人
}

void frontViewDialog_zhongcan_diancai::to_dish_title(const QString &num, const QString &total)
{
    if(!num.isEmpty())ui->label_num->setText(num);
    if(!total.isEmpty())ui->label_total->setText(total);
}

void frontViewDialog_zhongcan_diancai::to_btn_text()
{
    QPushButton *btn=qobject_cast<QPushButton *>(this->sender());
    if(btn==ui->pushButton_alldesk_cuicai){
        emit signal_alldesk_cuicai();
        return ;
    }
    if(btn==ui->pushButton_cancel_desk){
        emit signal_cancel_desk();
        return ;
    }
    if(btn==ui->pushButton_changedish){
        emit signal_changedish();
        return ;
    }
    if(btn==ui->pushButton_changyong){
        emit signal_changyong();
        return ;
    }
    if(btn==ui->pushButton_cook){
        emit signal_cook();
        return ;
    }

    if(btn==ui->pushButton_cuicai){
        emit signal_cuicai();
        return ;
    }
    if(btn==ui->pushButton_del){
        emit signal_del();
        return ;
    }
    if(btn==ui->pushButton_desk_info){
        emit signal_desk_info();
        return ;
    }
    if(btn==ui->pushButton_discount){
        emit signal_discount();
        return ;
    }
    if(btn==ui->pushButton_dish_switch_desk){
        emit signal_dish_switch_desk();
        return ;
    }

    if(btn==ui->pushButton_jia){
        emit signal_jia();
        return ;
    }
    if(btn==ui->pushButton_jian){
        emit signal_jian();
        return ;
    }
    if(btn==ui->pushButton_present){
        emit signal_present();
        return ;
    }
    if(btn==ui->pushButton_shuliang){
        emit signal_shuliang();
        return ;
    }
    if(btn==ui->pushButton_switch_desk){
        emit signal_switch_desk();
        return ;
    }

    if(btn==ui->pushButton_switch_desk){
        emit signal_switch_desk();
        return ;
    }

    if(btn==ui->pushButton_yuding){
        emit signal_yuding();
        return;
    }

    if(btn==ui->pushButton_bianma){
        emit signal_bianma();
        return;
    }

    if(btn==ui->pushButton_luodan){
        emit signal_luodan();
        return;
    }

    if(btn==ui->pushButton_jiesuan){
        emit signal_jisuan();
        return;
    }

    if(btn==ui->pushButton_tuichu){
        emit signal_tuichu();
        return;
    }

    if(btn=ui->pushButton_timedish){
        emit signal_timedish();
    }
}
