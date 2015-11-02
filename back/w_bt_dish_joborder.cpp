#include "w_bt_dish_joborder.h"
//营业班次
#include"ui_w_bt_dish_unitset_dialog.h"
#include "lds_model_sqlrelationaltablemodel.h"
#include "lds_tableview_delegate_check.h"
#include "backheader.h"
#include "lds_messagebox.h"
#include <QSqlRecord>
#include <QSqlQuery>
#include <QDebug>
#include <qsqlerror.h>
#include "lds_tableview_delegate_time.h"


QVariant w_bt_dish_joborder::RelationalTableModel::data(const QModelIndex &item, int role) const
{
    QVariant _d=lds_model_sqlrelationaltablemodel::data(item, role);
    if(role==Qt::DisplayRole||role==Qt::EditRole){
        if(_d.type()==QVariant::DateTime){
            return _d.toDateTime().toString("hh:mm");
        }
    }
    return _d;
}

w_bt_dish_joborder::w_bt_dish_joborder(QWidget *parent) :
    QDialog(parent),
    ui(new Ui_w_bt_dish_unitset_Dialog)
{
    ui->setupUi(this);
    //! tableview
    tablemodel=new RelationalTableModel(this);
    tablemodel->setTable("cey_bt_class");
    tablemodel->setEditStrategy(QSqlTableModel::OnManualSubmit);tablemodel->setJoinMode(QSqlRelationalTableModel::LeftJoin);

    ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::SelectedClicked);ui->tableView->setModel(tablemodel->fpmodel);
    ui->tableView->setItemDelegate(new lds_tableview_delegate_time("hh:mm", ui->tableView));
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#else
    ui->tableView->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
#endif
    tablemodel->save_set_header("ch_classno", tr("编码"), true);
    tablemodel->save_set_header("vch_classname", tr("班次名称"), true);
    tablemodel->save_set_header("dt_begin", tr("开始时间"), true);
    //! toolbar
    ui->widget->setup(QStringList() << tr("新增") << tr("删除") <<  tr("刷新") << tr("保存") << tr("退出"), Qt::AlignRight);
    connect(ui->widget->index_widget(tr("新增")), SIGNAL(clicked()),this,SLOT(tonew()));
    connect(ui->widget->index_widget(tr("删除")), SIGNAL(clicked()),this,SLOT(todel()));
    connect(ui->widget->index_widget(tr("刷新")), SIGNAL(clicked()),this,SLOT(torefresh()));
    connect(ui->widget->index_widget(tr("保存")), SIGNAL(clicked()),this,SLOT(tosave()));
    connect(ui->widget->index_widget(tr("退出")), SIGNAL(clicked()),this,SLOT(toexit()));

    QTimer::singleShot(100, this, SLOT(torefresh()));
}

void w_bt_dish_joborder::tonew()
{
    int rowcount=tablemodel->rowCount();
    tablemodel->insertRow(rowcount);
    tablemodel->setData(tablemodel->index(rowcount, 0), tablemodel->max_column_add_1("ch_classno"));
    tablemodel->setData(tablemodel->index(rowcount, 2), QDateTime::fromString("00:00", "hh:mm"));
}

void w_bt_dish_joborder::todel()
{
    tablemodel->removeRow(ui->tableView->currentIndex().row());
}

void w_bt_dish_joborder::torefresh()
{
    tablemodel->select();
}

bool w_bt_dish_joborder::tosave()
{
    for(int r=0,r_count=tablemodel->rowCount();r<r_count;r++){
        if(tablemodel->record(r).value("vch_classname").toString().isEmpty()){
            lds_messagebox::warning(this, tr("消息提示"),tr("班次名称不能为空!"));
            return false;
        }
        if(tablemodel->same_column_exist("dt_begin", r)){
            lds_messagebox::warning(this, tr("消息提示"),tr("开始时间有重复!"));
            return false;
        }
    }
    if(tablemodel->submitAll()){
        lds_messagebox::warning(this, tr("消息提示"),tr("保存成功"));
        return true;
    } else {
        lds_messagebox::warning(this, tr("消息提示"),tr("数据有误，操作失败"), tablemodel->lastError().text());
        return false;
    }
}

void w_bt_dish_joborder::toexit()
{
    if(tablemodel->model_is_dirty()){
        if(0==lds_messagebox::warning(this, tr("消息提示"),tr("有数据未保存,确认保存?")+tablemodel->dirty_err_string(), tr("确认"),tr("取消"))){
            if(!tosave()){
                return;
            }
        }
    }
//    if(tablemodel->model_is_dirty()){
//        if(!tosave()) return;
//    }
    this->reject();
}
