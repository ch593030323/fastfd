
#include "w_bt_dish_quit.h"
//退菜原因
#include"ui_w_bt_dish_unitset_dialog.h"
#include "lds_model_sqlrelationaltablemodel.h"
#include "lds_tableview_delegate_check.h"
#include "backheader.h"
#include "lds_messagebox.h"
#include <QSqlRecord>
#include <QSqlQuery>
#include <QDebug>
#include "backheader.h"
#include "w_bt_dish.h"
#include "qsqlerror.h"

w_bt_dish_quit::w_bt_dish_quit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui_w_bt_dish_unitset_Dialog)
{
    ui->setupUi(this);
    //! tableview
    tablemodel=new lds_model_sqlrelationaltablemodel(this);
    tablemodel->setTable("cey_bt_reason");
    tablemodel->setEditStrategy(QSqlTableModel::OnManualSubmit);tablemodel->setJoinMode(QSqlRelationalTableModel::LeftJoin);

    tablemodel->save_set_header("vch_reason", tr("原因说明"), true);
    tablemodel->save_set_header("vch_type", tr("退菜原因"));
    ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::SelectedClicked);ui->tableView->setModel(tablemodel->fpmodel);
    ui->tableView->rePecifyHeader(tablemodel->save_set_header_saver);

    //! toolbar
    ui->widget->setup(QStringList()<<tr("新增")<<tr("删除")<<tr("刷新")<<tr("保存")<<tr("退出"),Qt::AlignRight);
    connect(ui->widget->index_widget(tr("新增")),SIGNAL(clicked()),this,SLOT(tonew()));
    connect(ui->widget->index_widget(tr("删除")),SIGNAL(clicked()),this,SLOT(todel()));
    connect(ui->widget->index_widget(tr("刷新")),SIGNAL(clicked()),this,SLOT(torefresh()));
    connect(ui->widget->index_widget(tr("保存")),SIGNAL(clicked()),this,SLOT(tosave()));
    connect(ui->widget->index_widget(tr("退出")),SIGNAL(clicked()),this,SLOT(toexit()));

    //!refresh
    QTimer::singleShot(100, this, SLOT(torefresh()));
}

void w_bt_dish_quit::tonew()
{
    int rowcount=tablemodel->rowCount();
    tablemodel->insertRow(rowcount);
    tablemodel->setData(tablemodel->index(rowcount, 2), tr("退菜原因"));
}

void w_bt_dish_quit::todel()
{
    tablemodel->removeRow(ui->tableView->currentIndex().row());
}

bool w_bt_dish_quit::tosave()
{
    for(int r=0,r_count=tablemodel->rowCount();r<r_count;r++){
        if(tablemodel->record(r).value("vch_reason").toString().isEmpty()){
            lds_messagebox::warning(this, tr("消息提示"),tr("原因不能为空!"));
            return false;
        }
    }
    if(tablemodel->submitAll()){
        lds_messagebox::warning(this, tr("消息提示"),/*tablemodel_2->lastError().text()*/tr("保存成功"));
        return true;
    } else {
        lds_messagebox::warning(this, tr("消息提示"),/*tablemodel->lastError().text()*/tr("数据有误，操作失败"),tablemodel->lastError().text());
        return false;
    }
}

void w_bt_dish_quit::torefresh()
{
    tablemodel->select();
}

void w_bt_dish_quit::toexit()
{
    if(tablemodel->model_is_dirty()){
        if(0==lds_messagebox::warning(this, tr("消息提示"),tr("有数据未保存,确认保存?")+tablemodel->dirty_err_string(), tr("确认"),tr("取消"))){
            if(!tosave()){
                return;
            }
        }
    }
//    if(tablemodel->model_is_dirty()
//            &&!tosave())return;
    this->reject();
}
