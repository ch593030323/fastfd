#include "frontviewdialog_tableview.h"
#include <QPainter>
#include <QScrollBar>
#include <QApplication>
#include <QStaticText>
#include "auto_expand_image.h"
#include "public_sql.h"
#include <QSettings>
#include <QStandardItemModel>
#include <QLabel>
#include "backheader.h"
#include "frontviewdialog_virtual.h"

frontViewDialog_tableview::frontViewDialog_tableview(QWidget *parent) :
    lds_tableView_updown(parent)
{
    this->tableview->setFocusPolicy(Qt::NoFocus);
    this->tableview->setItemDelegate(new delegate(this->tableview, this));
    this->tableview->setFrameShape(QFrame::NoFrame);
}

void frontViewDialog_tableview::setModel(QStandardItemModel *model)
{
    this->tableview->setModel(model);
    this->tableview->setSelectionModel(&frontViewDialog_virtual::modelBase_select);
    this->tableview->rePecifyHeader(QStringList()<<tr("序号")<<tr("品名")<<tr("单价")<<tr("数量")<<tr("总价"));
    this->tableview->verticalHeader()->hide();
    this->tableview->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->tableview->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->tableview->verticalHeader()->setDefaultSectionSize(60);
    this->tableview->setEditTriggers(QTableView::NoEditTriggers);
    this->tableview->setShowGrid(false);
    this->tableview->setColumnWidth(0, 40);
    this->tableview->setColumnWidth(2, 90);
    this->tableview->setColumnWidth(4, 50);
    this->tableview->setColumnWidth(3, 60);
    this->tableview->setColumnWidth(6, 60);
    //清除合计信息
    connect(this->tableview->model(),SIGNAL(dataChanged(QModelIndex,QModelIndex)),this,SLOT(to_clear_payfor_info()));
    //副屏的槽
    connect(this->tableview,SIGNAL(selectchanged(int)),this,SLOT(to_curdishno_change(int)));

    //2 所有调用该类的，都需要同步滚动条和选区
    if(public_sql::save_login->double_screen
            &&public_sql::save_login->double_screen->ui->tableView!=this){
        connect(public_sql::save_login->double_screen->ui->tableView->tableview->verticalScrollBar(),
                SIGNAL(valueChanged(int)),this->tableview->verticalScrollBar(),SLOT(setValue(int)));
        connect(this->tableview->verticalScrollBar(),SIGNAL(valueChanged(int)),
                public_sql::save_login->double_screen->ui->tableView->tableview->verticalScrollBar(),SLOT(setValue(int)));
    }
}

//若是副屏存在则显示副品
void frontViewDialog_tableview::to_curdishno_change(int row)
{
    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    if(row>=0
            &&public_sql::save_login->double_screen){
        auto_expand_image::curdish_imagepath=
                conf.value("system_setting/bofanglujing").toString()+"/"+this->tableview->model()->index(row, 1).data().toString();
        public_sql::save_login->double_screen->to_show_and_hide();
    }
}
void frontViewDialog_tableview::to_payfor_info(const QString &info)
{
    this->tableview->update_paintinfo(info);
}

void frontViewDialog_tableview::to_clear_payfor_info()
{
    to_payfor_info("");
}

/*

QTableView{
     background: white;
     alternate-background-color: #dbe3e9;
 }
QTableView::item:selected {
     color:red;
 }
*/
void frontViewDialog_tableview::delegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const QAbstractItemModel *model0=index.model();
    int row=index.row();
    int column=index.column();
    const QRect &rect=option.rect;
    QString specialflag;
    QFont f;
    //2
    QStyleOptionViewItemV4 opt=option;
    initStyleOption(&opt, index);

    //3奇数行有色值
    if(row%2==1)painter->fillRect(opt.rect, QColor("#dbe3e9"));
    else painter->fillRect(opt.rect, QColor("white"));

    //4选中字体颜色为red
    painter->save();
    if(opt.state&QStyle::State_Selected){
        painter->setPen(QColor("red"));
    }
    painter->drawText(QRect(rect.x(),rect.y(),rect.width(),rect.height()*3.0/4.0), int(Qt::AlignCenter|Qt::TextWrapAnywhere),index.data().toString());
    //5下半行数据可是特价之类的内容
    if("P"==model0->index(row, 9).data().toString())specialflag.append(tr("主  "));
    if("Y"==model0->index(row, 9).data().toString())specialflag.append(tr("子  "));
    if("Y"==model0->index(row, 8).data().toString())specialflag.append(tr("赠  "));
    if("Y"==model0->index(row, 10).data().toString())specialflag.append(tr("特  "));
    specialflag.append(model0->index(row, 7).data().toString());//做法
    specialflag=specialflag.trimmed();
    if(!specialflag.isEmpty()){
        f=painter->font();
        f.setPointSize(8);
        painter->setPen(QColor("purple"));
        painter->setFont(f);
        painter->drawText(QRect(0,rect.y()+rect.height()*3.0/4.0,tableview->viewport()->width(), rect.height()*1.0/4.0), int(Qt::AlignCenter), specialflag);
    }
    painter->restore();
}
