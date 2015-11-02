#include "frontviewdialog_frame_list_item.h"
#include <qdebug.h>
frontviewdialog_frame_list_item::frontviewdialog_frame_list_item(QListWidgetItem *parentitem, QWidget *parent) :
    QRadioButton(parent),
    _item(parentitem)
{
//    init();
}

frontviewdialog_frame_list_item::frontviewdialog_frame_list_item(QWidget *parent):QRadioButton(parent)
{
//    init();
}

void frontviewdialog_frame_list_item::init()
{
//    fui=new Ui_frontViewDialog_list_item_Dialog;
//    fui->setupUi(this);
//    this->setFocusPolicy(Qt::NoFocus);
//    this->setStyleSheet(
//                "QRadioButton{background:transparent;min-height:40px;}"
//                "QRadioButton:indicator{ width:0px;}"
//                "QLabel{font:12pt;}"
//                );
//    is_close_press_color_red=false;
//    fui->label_p->setFixedWidth(50);//序号
//    fui->label_p0->hide();fui->label_p0->setFixedWidth(50);//品码
//    fui->label_p1->setFixedWidth(100);//品名
//    fui->label_p2->setFixedWidth(50);//单价
//    fui->label_p3->setFixedWidth(50);//数量
//    fui->label_p4->setFixedWidth(50);//小计
//    fui->label_p5->setFixedWidth(100);//做法
//    fui->label_p5->hide();

//    connect(this,SIGNAL(toggled(bool)),this,SLOT(update_fui(bool)));
}

frontviewdialog_frame_list_item::~frontviewdialog_frame_list_item()
{
//    delete fui;
}

void frontviewdialog_frame_list_item::p5_append(const QString &text)
{
//    if(text.isEmpty()) return;
//    if(fui->label_p5->isHidden())fui->label_p5->show();
//    fui->label_p5->setText(fui->label_p5->text()+text);
}

void frontviewdialog_frame_list_item::p5_clear()
{
//    fui->label_p5->clear();
}

void frontviewdialog_frame_list_item::close_press_color_red()
{
//    is_close_press_color_red=true;
//    this->setStyleSheet(
//                "QRadioButton{background:transparent;min-height:40px;}"
//                "QRadioButton:indicator{width:0px;}"
//                );
//    this->setEnabled(false);
}

void frontviewdialog_frame_list_item::update_fui(bool flag)
{
//    QListWidget *listwidget=0;
//    if(_item)
//        listwidget=qobject_cast<QListWidget*>(_item->listWidget());
//    if(listwidget)
//        listwidget->setCurrentItem(_item);
//    if(!is_close_press_color_red){
//        if(flag) {this->setStyleSheet(
//                        "QRadioButton{background:transparent;min-height:40px;}"
//                        "QRadioButton:indicator{ width:0px;}"
//                        "QRadioButton:checked{color:red;background:transparent;}"
//                        "QRadioButton:unchecked{background:transparent;}"
//                        "QLabel{color:red;font:12pt;}"
//                        );
//            fui->label_p3->setStyleSheet("color:purple;");
//        } else {
//            this->setStyleSheet(
//                        "QRadioButton{background:transparent;min-height:40px;}"
//                        "QRadioButton:indicator{ width:0px;}"
//                        "QRadioButton:checked{color:red;background:transparent;}"
//                        "QRadioButton:unchecked{background:transparent;}"
//                        "QLabel{font:12pt;}"
//                        );
//            fui->label_p3->setStyleSheet("");
//        }
//    }
}
