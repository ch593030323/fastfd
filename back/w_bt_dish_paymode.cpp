#include "w_bt_dish_paymode.h"
//收银方式
#include"ui_w_bt_dish_unitset_dialog.h"
#include "lds_model_sqlrelationaltablemodel.h"
#include "lds_tableview_delegate_check.h"
#include "backheader.h"
#include "lds_messagebox.h"
#include <QSqlRecord>
#include <QSqlQuery>
#include <QDebug>
#include <qsqlerror.h>
#include <QSqlRelationalDelegate>
#include "ui_w_bt_dish_paymode_dialog.h"

w_bt_dish_paymode::w_bt_dish_paymode(QWidget *parent) :
    QDialog(parent),
    ui(new Ui_w_bt_dish_paymode_dialog)
{
    ui->setupUi(this);
    //! tableview
    tablemodel=new lds_model_sqlrelationaltablemodel(this);
    tablemodel->setTable("cey_bt_paymode");
    tablemodel->setEditStrategy(QSqlTableModel::OnManualSubmit);tablemodel->setJoinMode(QSqlRelationalTableModel::LeftJoin);
    tablemodel->select();

    ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::SelectedClicked);ui->tableView->setModel(tablemodel->fpmodel);


    tablemodel->save_set_header("ch_paymodeno", tr("代码"));
    tablemodel->save_set_header("vch_paymodename", tr("收银方式"));
    tablemodel->save_set_header("ch_key", tr("快捷键"));
    ui->tableView->rePecifyHeader(tablemodel->save_set_header_saver);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->setColumnWidth(0, 50);

    mapper = new QDataWidgetMapper(this);
    mapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
    mapper->setModel(tablemodel);
    mapper->setItemDelegate(new QSqlRelationalDelegate(mapper));
    mapper->addMapping(ui->lineEdit_ch_key,tablemodel->fieldIndex("ch_key"));
    mapper->addMapping(ui->lineEdit_ch_paymodeno,tablemodel->fieldIndex("ch_paymodeno"));
    mapper->addMapping(ui->lineEdit_vch_paymodename,tablemodel->fieldIndex("vch_paymodename"));
    mapper->addMapping(ui->checkBox_ch_changeflag,tablemodel->fieldIndex("ch_changeflag"));
    mapper->addMapping(ui->checkBox_ch_faceflag,tablemodel->fieldIndex("ch_faceflag"));
    mapper->addMapping(ui->checkBox_ch_idflag,tablemodel->fieldIndex("ch_IDflag"));
    mapper->addMapping(ui->checkBox_ch_incomeflag,tablemodel->fieldIndex("ch_incomeflag"));
    //! toolbar
    ui->widget->setup(QStringList() << tr("新增") << tr("删除") <<  tr("刷新") << tr("保存") << tr("退出"), Qt::AlignRight);
    connect(ui->widget->index_widget(tr("新增")), SIGNAL(clicked()),this,SLOT(tonew()));
    connect(ui->widget->index_widget(tr("删除")), SIGNAL(clicked()),this,SLOT(todel()));
    connect(ui->widget->index_widget(tr("刷新")), SIGNAL(clicked()),this,SLOT(torefresh()));
    connect(ui->widget->index_widget(tr("保存")), SIGNAL(clicked()),this,SLOT(tosave()));
    connect(ui->widget->index_widget(tr("退出")), SIGNAL(clicked()),this,SLOT(toexit()));
    connect(ui->tableView,SIGNAL(selectchanged(int)),this,SLOT(mapper_setcurindex(int)));

    mapper->toFirst();
    ui->tableView->selectRow(0);
}

void w_bt_dish_paymode::tonew()
{
    int rowcount=tablemodel->rowCount();
    tablemodel->insertRow(rowcount);
    tablemodel->setData(tablemodel->index(rowcount, 0), tablemodel->max_column_add_1("ch_paymodeno"));
}

void w_bt_dish_paymode::todel()
{
    //clicked (none) returns long [pbm_bnclicked]
    //long ancestorreturnvalue
    QString ls_paymodeno;
    qlonglong ll_row;
    ll_row=ui->tableView->currentIndex().row();
    if(ll_row<0){
        return;
    }
    QSqlQuery query;
    query.exec(QString("Select count(*)   from cey_u_checkout_detail  where ch_paymodeno ='%1' ").arg(ls_paymodeno));
    query.next();
    if(query.record().value(0).toLongLong() > 0){
        lds_messagebox::warning(this, tr("消息提示"),tr("当前收银方式存在收银记录，不能删除!"));
        return;
    }
    tablemodel->removeRow(ui->tableView->currentIndex().row());
    tablemodel->submitAll();
}

void w_bt_dish_paymode::torefresh()
{
    tablemodel->select();
}

bool w_bt_dish_paymode::tosave()
{
    for(int r=0,r_count=tablemodel->rowCount();r<r_count;r++){
        if(tablemodel->record(r).value("vch_paymodename").toString().isEmpty()){
            lds_messagebox::warning(this, tr("消息提示"),tr("收银方式名称不能为空!"));
            return false;
        }
        if(tablemodel->same_column_exist("vch_paymodename", r)){
            lds_messagebox::warning(this, tr("消息提示"),tr("收银方式名称不能重名!"));
            return false;
        }
        if(tablemodel->record(r).value("ch_changeflag").toString()=="Y"
                &&tablemodel->record(r).value("ch_faceflag").toString()=="Y"){
            lds_messagebox::warning(this, tr("消息提示"),tr("是否找零与是否面额只能选其一!"));
            return false;
        }

        if(tablemodel->record(r).value("ch_key").toString().isEmpty()){
            lds_messagebox::warning(this, tr("消息提示"),tr("快捷键不能为空!"));
            return false;
        }

        if(tablemodel->record(r).value("ch_key").toString().count()!=1
                ||(tablemodel->record(r).value("ch_key").toString().toLocal8Bit()[0]<65
                   &&tablemodel->record(r).value("ch_key").toString().toLocal8Bit()[0]<90)
                ){
            lds_messagebox::warning(this, tr("消息提示"),tr("快捷键只能为A～Z!"));
            return false;
        }
        if(tablemodel->same_column_exist("ch_key", r)){
            lds_messagebox::warning(this, tr("消息提示"),tr("此快捷键已被其它收银方式占用!"));
            return false;
        }
    }
    if(tablemodel->submitAll()){
        lds_messagebox::warning(this, tr("消息提示"),tr("保存成功"));
        return true;
    } else {
        lds_messagebox::warning(this, tr("消息提示"),tr("数据有误，操作失败")+ tablemodel->lastError().text());
        return false;
    }
}

void w_bt_dish_paymode::toexit()
{
    if(tablemodel->model_is_dirty()){
        if(0==lds_messagebox::warning(this, tr("消息提示"),tr("有数据未保存,确认保存?")+tablemodel->dirty_err_string(), tr("确认"),tr("取消"))){
            if(!tosave()){
                return;
            }
        }
    }
    this->reject();
}

void w_bt_dish_paymode::mapper_setcurindex(int index)
{
    QString ls_paymodeno=tablemodel->record(index).value("ch_paymodeno").toString();
    bool isenable=
            !(ls_paymodeno=="00" ||
              ls_paymodeno=="01"||
              ls_paymodeno=="02"||
              ls_paymodeno=="03"||

              ls_paymodeno=="04"||
              ls_paymodeno=="05"||
              ls_paymodeno=="XX"||
              ls_paymodeno=="YY"||
              ls_paymodeno=="ZZ");
//    ui->lineEdit_ch_key->setEnabled(isenable);
    ui->lineEdit_ch_paymodeno->setEnabled(isenable);
    ui->lineEdit_vch_paymodename->setEnabled(isenable);
    ui->checkBox_ch_changeflag->setEnabled(isenable);
    ui->checkBox_ch_faceflag->setEnabled(isenable);
    ui->checkBox_ch_idflag->setEnabled(isenable);
    ui->checkBox_ch_incomeflag->setEnabled(isenable);
    ui->widget->index_widget(tr("删除"))->setEnabled(isenable);

    mapper->setCurrentIndex(index);
}
