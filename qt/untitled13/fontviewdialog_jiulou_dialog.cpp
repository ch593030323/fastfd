#include "fontviewdialog_jiulou_dialog.h"
#include "ui_fontviewdialog_jiulou_dialog.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include "frontviewdialog_autoexpand.h"
#include "backheader.h"
#include "fontviewdialog_jiulou_diancan.h"
#include "lds_menu.h"
#include <QWidgetAction>

fontviewDialog_jiulou_Dialog::fontviewDialog_jiulou_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::fontviewDialog_jiulou_Dialog)
{
    ui->setupUi(this);
    {//系统设置
        lds_menu *menu=new lds_menu(this);
        mview=new manager_view;
        QWidgetAction *widgetaction=new QWidgetAction(ui->pushButton_set);
        widgetaction->setDefaultWidget(mview);
        menu->setStyleSheet("QMenu{border:0px;}");
        menu->addAction(widgetaction);
        ui->pushButton_set->setMenu(menu);
    }

    int horizontal_count;
    int vertical_count;
    if(backheader::mainwindowsize.width()==800){
        horizontal_count=7;
        vertical_count=4;
    } else {
        horizontal_count=10;
        vertical_count=6;
    }

    ui->frame_main->horizontal_count=5;
    ui->frame_main->vertical_count=4;
    ui->frame_main->set_btn_property("pushbutton_jiulou");
    ui->frame_main->tool_size=QSize(110, 110);
    QSqlQuery query;
    query.exec(QString("select * from cey_bt_table "));
    while(query.next()){
        ui->frame_main->add_item(query.record().value("ch_tableno").toString(),
                                 query.record().value("vch_tablename").toString(),
                                 false,
                                 QPixmap(),
                                 query.record().value("ch_billno").toString().isEmpty()?frontViewDialog_toolbtn::KONGXIAN : frontViewDialog_toolbtn::XUANZHONG
                                                                                        );
    }
    ui->frame_main->generat_from_items();

    ui->frame_list->horizontal_count=1;
    ui->frame_list->vertical_count=5;
    ui->frame_list->set_btn_property("pushbutton_jiuloulist");
    ui->frame_list->tool_size=QSize(100, 60);


//    connect(ui->radioButton_table_all,SIGNAL(toggled(bool)),this,SLOT(filter_content()));
//    connect(ui->radioButton_table_use,SIGNAL(toggled(bool)),this,SLOT(filter_content()));
//    connect(ui->radioButton_table_order,SIGNAL(toggled(bool)),this,SLOT(filter_content()));
//    connect(ui->radioButton_table_nouse,SIGNAL(toggled(bool)),this,SLOT(filter_content()));
    //    connect(ui->lineEdit,SIGNAL(textChanged(QString)),this,SLOT(filter_content()));
    //        connect(ui->tabWidget,SIGNAL(currentChanged(int)),this,SLOT(filter_content()));
    //    //
    //    connect(ui->pushButton_set,SIGNAL(clicked()),this,SIGNAL(signal_jinliguanli()));
    ////    connect(ui->pushButton_set,SIGNAL(clicked()),this,SIGNAL(signal_shezhi()));
    connect(ui->pushButton_order, SIGNAL(clicked()),this,SIGNAL(signal_yuding()));
    connect(ui->pushButton_refresh,SIGNAL(clicked()),this,SIGNAL(signal_shuaxin()));
//    connect(ui->pushButton_bill,SIGNAL(clicked()),this,SIGNAL(signal_yingyeshuju()));

    connect(ui->pushButton_switch,SIGNAL(clicked()),this,SIGNAL(signal_jiaoban()));
    ////    connect(ui->pushButton_set,SIGNAL(clicked()),this,SIGNAL(signal_shoudongshuru()));
    connect(ui->pushButton_open,SIGNAL(clicked()),this,SIGNAL(signal_kaiqianxiang()));
    connect(ui->pushButton_quit,SIGNAL(clicked()),this,SIGNAL(signal_tuichu()));
    ui->pushButton_other->hide();
    QTimer *timer=new QTimer;
    connect(timer,SIGNAL(timeout()),this,SLOT(print_geometry()));
    timer->start(1000);
}

fontviewDialog_jiulou_Dialog::~fontviewDialog_jiulou_Dialog()
{
    delete ui;
}

void fontviewDialog_jiulou_Dialog::update_desk_info(int void_num, int use_num, const QString &oper, const QString &dt)
{
//    query.exec(QString("SELECT concat(vch_typename,'[',ch_typeno,']') as nametype,ch_typeno  FROM hddpos.cey_bt_table_type union select '"+tr("所有桌台")+"' as vch_typename,'' as ch_typeno ORDER BY ch_typeno ASC;"));
//    while(query.next()){
//        ui->frame_list->add_item(query.record().value("ch_typeno").toString(),
//                                 query.record().value("nametype").toString(),
//                                 false);
//    }
//    ui->frame_list->generat_from_items();
}

void fontviewDialog_jiulou_Dialog::to_tabe_clicked(const QString &text)
{
    emit signal_sub_desk(text);
}

void fontviewDialog_jiulou_Dialog::filter_content()
{
}

void fontviewDialog_jiulou_Dialog::print_geometry()
{
    qDebug() << this->geometry();

}
