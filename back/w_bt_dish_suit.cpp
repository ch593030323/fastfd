#include "w_bt_dish_suit.h"
//菜品套菜资料
#include "ui_w_bt_dish_suit_dialog.h"
#include <QSqlQueryModel>
#include <QSqlRecord>
#include "lds_messagebox.h"
#include <QSqlQuery>
#include <QSqlError>
#include "lds_tableview_repecify.h"
#include "QStandardItemModel"

#include <QSqlRecord>
#include "backheader.h"
w_bt_dish_suit::RelationalTableModel_2::RelationalTableModel_2(QObject *parent, QSqlDatabase db)
    : lds_model_sqlrelationaltablemodel(parent, db)
{//cey_bt_dish_suit
    backheader::regetvch_dish_typename();
    backheader::regetvch_unitname();

    sql_headerlabels.append(SQL_HEADERLABEL("select  cey_bt_dish.vch_dishname from cey_bt_dish where cey_bt_dish.ch_dishno=cey_bt_dish_suit.ch_dishno" ,
                                            tr("品名")));
    sql_headerlabels.append(SQL_HEADERLABEL("select  cey_bt_dish.ch_dish_typeno from cey_bt_dish where cey_bt_dish.ch_dishno=cey_bt_dish_suit.ch_dishno" ,
                                            tr("菜品类型")));
    sql_headerlabels.append(SQL_HEADERLABEL("select  cey_bt_dish.ch_unitno from cey_bt_dish where cey_bt_dish.ch_dishno=cey_bt_dish_suit.ch_dishno" ,
                                            tr("单位")));
    sql_headerlabels.append(SQL_HEADERLABEL("select  cey_bt_dish.num_price from cey_bt_dish where cey_bt_dish.ch_dishno=cey_bt_dish_suit.ch_dishno" ,
                                            tr("价格")));
    sql_headerlabels.append(SQL_HEADERLABEL("select  cey_bt_dish.num_price*cey_bt_dish_suit.num_num from cey_bt_dish where cey_bt_dish.ch_dishno=cey_bt_dish_suit.ch_dishno" ,
                                            tr("金额")));
}

QVariant w_bt_dish_suit::RelationalTableModel_2::data(const QModelIndex &item, int role) const
{
    QVariant _d=lds_model_sqlrelationaltablemodel::data(item, role);

    if(role==Qt::DisplayRole){
        if(this->headerData(item.column(), Qt::Horizontal,  Qt::DisplayRole).toString().endsWith(tr("菜品类型"))){
            return backheader::getouterdata(backheader::vch_dish_typename, _d.toString());
        }
        if(this->headerData(item.column(), Qt::Horizontal,  Qt::DisplayRole).toString().endsWith(tr("单位"))){
            return backheader::getouterdata(backheader::vch_unitname, _d.toString());
        }
    }
    return _d;
}

//===================
w_bt_dish_suit::RelationalTableModel_3::RelationalTableModel_3(QObject *parent, QSqlDatabase db)
    : lds_model_sqlrelationaltablemodel(parent, db)
{//cey_bt_suit_change

    sql_headerlabels.append(SQL_HEADERLABEL("select  cey_bt_dish.vch_dishname from cey_bt_dish where cey_bt_dish.ch_dishno=cey_bt_suit_change.ch_dishno" ,
                                            tr("品名")));
    sql_headerlabels.append(SQL_HEADERLABEL("select  cey_bt_dish.ch_dish_typeno from cey_bt_dish where cey_bt_dish.ch_dishno=cey_bt_suit_change.ch_dishno" ,
                                            tr("菜品类型")));
    sql_headerlabels.append(SQL_HEADERLABEL("select  cey_bt_dish.ch_unitno from cey_bt_dish where cey_bt_dish.ch_dishno=cey_bt_suit_change.ch_dishno" ,
                                            tr("单位")));
    sql_headerlabels.append(SQL_HEADERLABEL("select  cey_bt_dish.num_price from cey_bt_dish where cey_bt_dish.ch_dishno=cey_bt_suit_change.ch_dishno" ,
                                            tr("价格")));
    sql_headerlabels.append(SQL_HEADERLABEL("select  cey_bt_dish.num_price*cey_bt_suit_change.num_num_change from cey_bt_dish where cey_bt_dish.ch_dishno=cey_bt_suit_change.ch_dishno" ,
                                            tr("金额")));
}

QVariant w_bt_dish_suit::RelationalTableModel_3::data(const QModelIndex &item, int role) const
{
    QVariant _d=lds_model_sqlrelationaltablemodel::data(item, role);
    if(role==Qt::DisplayRole){
        if(this->headerData(item.column(), Qt::Horizontal,  Qt::DisplayRole).toString().endsWith(tr("菜品类型"))){
            return backheader::getouterdata(backheader::vch_dish_typename, _d.toString());
        }
        if(this->headerData(item.column(), Qt::Horizontal,  Qt::DisplayRole).toString().endsWith(tr("单位"))){
            return backheader::getouterdata(backheader::vch_unitname, _d.toString());
        }
    }
    return _d;
}

w_bt_dish_suit::w_bt_dish_suit(QWidget *parent) : QDialog(parent),ui(new Ui_w_bt_dish_suit_Dialog)
{
    ui->setupUi(this);
    //! init
    tablemodel=new lds_model_sqlrelationaltablemodel;
    tablemodel_2=new RelationalTableModel_2;
    tablemodel_3=new RelationalTableModel_3;

    tablemodel->setTable("cey_bt_dish");
    tablemodel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    tablemodel->setFilter("ch_suitflag = 'Y' ");
    tablemodel->save_set_header("ch_dishno", tr("品码"));
    tablemodel->save_set_header("vch_dishname", tr("套菜名称"));
    tablemodel->save_set_header("num_price", tr("价格"));
    ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::SelectedClicked);ui->tableView->setModel(tablemodel->fpmodel);
    ui->tableView->rePecifyHeader(tablemodel->save_set_header_saver);

    tablemodel_2->setTable("cey_bt_dish_suit");
    tablemodel_2->setEditStrategy(QSqlTableModel::OnManualSubmit);
    //ch_suitno = '%1'
    tablemodel_2->save_set_header("ch_dishno", tr("品码"));
    tablemodel_2->save_set_header("num_num", tr("数量"), true);
    ui->tableView_2->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::SelectedClicked);ui->tableView_2->setModel(tablemodel_2->fpmodel);
    ui->tableView_2->tableView->rePecifyHeader(tablemodel_2->save_set_header_saver);

    tablemodel_3->setTable("cey_bt_suit_change");
    tablemodel_3->setEditStrategy(QSqlTableModel::OnManualSubmit);
    //int_flowID = '%1'
    tablemodel_3->save_set_header("ch_dishno", tr("品码"));
    tablemodel_3->save_set_header("num_num_change", tr("替换数量"),true);
    ui->tableView_3->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::SelectedClicked);ui->tableView_3->setModel(tablemodel_3->fpmodel);
    ui->tableView_3->rePecifyHeader(tablemodel_3->save_set_header_saver);
    //! toolbar
    ui->widget->setup(QStringList()<< tr("增加菜品") << tr("删除菜品")<< tr("增加替换菜品") << tr("删除替换菜品")   << tr("保存") << tr("刷新") << tr("退出"));

    connect(ui->tableView,SIGNAL(clicked(QModelIndex)),this,SLOT(t1_t2_t3()));
    connect(this,SIGNAL(tomitt2t3()),this,SLOT(t2_t3()));
    connect(ui->tableView_2->tableView,SIGNAL(clicked(QModelIndex)),this,SLOT(t2_t3()));

    connect(ui->widget->index_widget(tr("增加菜品")), SIGNAL(clicked()),this,SLOT(tonew()));
    connect(ui->widget->index_widget(tr("删除菜品")), SIGNAL(clicked()),this,SLOT(todel()));
    connect(ui->widget->index_widget(tr("增加替换菜品")), SIGNAL(clicked()),this,SLOT(toreplace()));
    connect(ui->widget->index_widget(tr("删除替换菜品")), SIGNAL(clicked()),this,SLOT(toreplace_del()));
    connect(ui->widget->index_widget(tr("保存")), SIGNAL(clicked()),this,SLOT(tosave()));
    connect(ui->widget->index_widget(tr("退出")), SIGNAL(clicked()),this,SLOT(toexit()));
    connect(ui->widget->index_widget(tr("刷新")), SIGNAL(clicked()),this,SLOT(torefresh()));
    //! after
    tablemodel->select();
    ui->tableView->selectRow(0);
    t1_t2_t3();
}

void w_bt_dish_suit::t1_t2_t3()
{
    t1_t2();
    emit tomitt2t3();
}

void w_bt_dish_suit::t1_t2()
{
    int row=ui->tableView->currentIndex().row();
    if(row>=-1){
        //有脏数据
        if(tablemodel_2->model_is_dirty()){
            tablemodel_2->submitAll();
        }
        tablemodel_2->setFilter(QString("ch_suitno = '%1'")
                                .arg(tablemodel->record(row).value("ch_dishno").toString())
                                );
        tablemodel_2->select();
        ui->tableView_2->tableView->restore();
        ui->tableView_2->updateSum(6, tablemodel_2->sum_value_column(tr("金额")));
    }
}

void w_bt_dish_suit::t2_t3()
{
    int row=ui->tableView_2->tableView->currentIndex().row();
    if(row>=-1){
        //有脏数据
        if(tablemodel_3->model_is_dirty()){
            tablemodel_3->submitAll();
        }
        tablemodel_3->setFilter(QString("int_flowID = '%1'")
                                .arg(tablemodel_2->record(row).value("int_flowID").toString())
                                );
        tablemodel_3->select();
        ui->tableView_3->restore();
    }
}

void w_bt_dish_suit::tonew()
{
    int row=ui->tableView->currentIndex().row();
    if(row>=0){
        w_bt_dish_suit_new dialog(this);
        backheader::static_dialogexec(dialog,qobject_cast<QAbstractButton* >(sender())->text());
    } else {
        lds_messagebox::warning(this, tr("消息提示"),tr("请选择套菜资料!"));
    }
}

void w_bt_dish_suit::todel()
{
    int row=ui->tableView_2->tableView->currentIndex().row();
    if(row>-1){
        if(tablemodel_3->rowCount()>0){
            if(1==lds_messagebox::warning(this,tr("删除套菜明细"),tr("该套菜明细下有可替换菜品，是否继续删除"),tr("继续"),tr("取消"))){
                return;
            }
        }
        tablemodel_2->removeRow(row);
        tablemodel_2->submitAll();
        ui->tableView_2->tableView->restore();
        t2_t3();
    } else {
        lds_messagebox::warning(this, tr("消息提示"),tr("请先选择要删除的套菜明细菜品记录!"));
    }
}

void w_bt_dish_suit::toreplace()
{
    int row=ui->tableView_2->tableView->currentIndex().row();
    if(row>=0){
        w_bt_dish_suit_replace dialog(this);
        backheader::static_dialogexec(dialog,qobject_cast<QAbstractButton* >(sender())->text());
    } else {
        lds_messagebox::warning(this, tr("消息提示"),tr("没有选择添加菜品!"));
    }
}

void w_bt_dish_suit::toreplace_del()
{
    int row=ui->tableView_3->currentIndex().row();
    if(row>-1){
        tablemodel_3->removeRow(row);
        tablemodel_3->submitAll();
        ui->tableView_3->restore();
    } else {
        lds_messagebox::warning(this, tr("消息提示"),tr("请先选择要删除的替换菜品!"));
    }
}

bool w_bt_dish_suit::tosave()
{
    if(!tablemodel_2->submitAll()){
        lds_messagebox::warning(this, tr("消息提示"),tr("数据有误，操作失败"), tablemodel_2->lastError().text());
        return false;
    }

    if(!tablemodel_3->submitAll()){
        lds_messagebox::warning(this, tr("消息提示"),tr("数据有误，操作失败"),tablemodel_3->lastError().text());
        return false;
    }

    lds_messagebox::warning(this, tr("消息提示"),/*tablemodel_2->lastError().text()*/tr("保存成功"));
    return true;

}

void w_bt_dish_suit::toexit()
{
    if(tablemodel_2->model_is_dirty()
            || tablemodel_3->model_is_dirty()){
        if(0==lds_messagebox::warning(this, tr("消息提示"),tr("有数据未保存,确认保存?")+tablemodel->dirty_err_string(), tr("确认"),tr("取消"))){
            if(!tosave()){
                return;
            }
        }
    }
//    if((tablemodel_2->model_is_dirty()||tablemodel_3->model_is_dirty())
//            &&!tosave())return;
    this->reject();
}

void w_bt_dish_suit::torefresh()
{
    tablemodel->select();ui->tableView->restore();
    tablemodel_2->select();ui->tableView_2->tableView->restore();
    tablemodel_3->select();ui->tableView_3->restore();

    ui->tableView_2->updateSum(6, tablemodel_2->sum_value_column(tr("金额")));
}

//=======================
#include "ui_w_bt_dish_suit_new_dialog.h"
#include "lds_model_sqlrelationaltablemodel.h"
#include <QSqlQuery>

w_bt_dish_suit_new::w_bt_dish_suit_new(w_bt_dish_suit *parent):
    QDialog(parent),
    ui(new Ui_w_bt_dish_suit_new_Dialog),
    _parent(parent)
{
    ui->setupUi(this);
    ui->label_7->setText(tr("套菜:%1")
                         .arg(_parent->tablemodel->record(_parent->ui->tableView->currentIndex().row()).value("ch_dishno").toString())
                         );
    //! tableview
    tablemodel=new lds_model_sqlrelationaltablemodel(this);
    tablemodel->setTable("cey_bt_dish");
    tablemodel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    tablemodel->setJoinMode(QSqlRelationalTableModel::LeftJoin);
    tablemodel->setRelation(4/*ch_dish_typeno*/, QSqlRelation("cey_bt_dish_type", "ch_dish_typeno","vch_dish_typename"));
    tablemodel->setRelation(5/*ch_unitno*/, QSqlRelation("cey_bt_unit", "ch_unitno","vch_unitname"));

    tablemodel->save_set_header("ch_dishno", tr("品码"));
    tablemodel->save_set_header("vch_dishname", tr("品名"));
    tablemodel->save_set_header("vch_spell", tr("拼音简码"));
    tablemodel->save_set_header("ch_dish_typeno", tr("品类"));
    tablemodel->save_set_header("ch_unitno", tr("单位"));
    tablemodel->save_set_header("num_price", tr("价格"));
    ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::SelectedClicked);ui->tableView->setModel(tablemodel->fpmodel);
    ui->tableView->rePecifyHeader(tablemodel->save_set_header_saver);

    ui->tableView->setSelectionMode(QAbstractItemView::MultiSelection);

    QStandardItemModel commodel;
    {
        QList<QStandardItem *> items;
        items << new QStandardItem("-1");
        items << new QStandardItem(tr("全部"));
        commodel.appendRow(items);
    }
    QSqlQuery query;
    query.exec("select *from cey_bt_dish_type");
    while(query.next()){
        QList<QStandardItem *> items;
        items << new QStandardItem(query.record().value(0).toString());
        items << new QStandardItem(query.record().value(1).toString());
        commodel.appendRow(items);
    }
    backheader::lds_com_setdatas(&commodel, ui->comboBox_dishtype);

    //! connect
    ui->widget->setup(QStringList()<<tr("检索")<<tr("重置"),Qt::AlignTop);
    ui->widget_2->setup(QStringList()<<tr("应用")<<tr("确定")<<tr("退出"),Qt::AlignRight);

    connect(ui->widget->index_widget(tr("检索")),SIGNAL(clicked()),this,SLOT(torefresh()));
    connect(ui->widget->index_widget(tr("重置")),SIGNAL(clicked()),this,SLOT(toreset()));
    connect(ui->widget_2->index_widget(tr("应用")),SIGNAL(clicked()),this,SLOT(toapp()));
    connect(ui->widget_2->index_widget(tr("确定")),SIGNAL(clicked()),this,SLOT(took()));
    connect(ui->widget_2->index_widget(tr("退出")),SIGNAL(clicked()),this,SLOT(toexit()));

    QTimer::singleShot(100, this,SLOT(torefresh()));
}

void w_bt_dish_suit_new::torefresh()
{
    QString dishname=ui->lineEdit_dishname->text();
    QString dishno= ui->lineEdit_dishno->text();
    QString spell= ui->lineEdit_spell->text().toUpper();
    QString dishtype= ui->comboBox_dishtype->comusrdata();
    dishtype=dishtype=="-1"?"":dishtype;

    QString selectfilter=QString("cey_bt_dish.ch_dishno like '%1%' and cey_bt_dish.vch_spell like '%2%' and cey_bt_dish.ch_dish_typeno like '%3%' and vch_dishname like '%4%' and ch_suitflag='N' ")
            .arg(dishno)
            .arg(spell)
            .arg(dishtype)
            .arg(dishname)

            ;//  orand (-1,ch_dish_typeno) like '%%3%'
    tablemodel->setFilter(selectfilter);
    tablemodel->select();
    ui->tableView->restore_rows();
}

void w_bt_dish_suit_new::toreset()
{
    ui->lineEdit_dishname->clear();
    ui->lineEdit_dishno->clear();
    ui->lineEdit_spell->clear();
    ui->comboBox_dishtype->setCurrentIndex(0);
    ui->tableView->clearSelection();
}

bool w_bt_dish_suit_new::toapp()
{
    QSet<int> currows = ui->tableView->getcurrows();
    int index;
    foreach(int r, currows){
        QString ch_dishno=tablemodel->record(r).value("ch_dishno").toString();
        if((index=_parent->tablemodel_2->find_column_exist("ch_dishno", ch_dishno)) != -1){
            continue;
        }

        int rowcount=_parent->tablemodel_2->rowCount();
        _parent->tablemodel_2->insertRow(rowcount);

        _parent->tablemodel_2->set_last_data("ch_suitno", _parent->tablemodel->record(_parent->ui->tableView->currentIndex().row()).value("ch_dishno").toString());
        _parent->tablemodel_2->set_last_data("ch_dishno", tablemodel->record(r).value("ch_dishno").toString());
        _parent->tablemodel_2->set_last_data("num_num", "1");
    }
    _parent->tablemodel_2->submitAll();
}

void w_bt_dish_suit_new::took()
{
    toapp();
    this->accept();
}

void w_bt_dish_suit_new::toexit()
{
    this->reject();
}

//=================================
//#include "ui_w_bt_dish_replace_dialog.h"
w_bt_dish_suit_replace::w_bt_dish_suit_replace(w_bt_dish_suit *parent):
    w_bt_dish_suit_new(parent)
{
    ui->label_7->setText(tr("套菜:%1;菜品明细:%2")
                         .arg(_parent->tablemodel_2->record(_parent->ui->tableView_2->tableView->currentIndex().row()).value("ch_suitno").toString())
                         .arg(_parent->tablemodel_2->record(_parent->ui->tableView_2->tableView->currentIndex().row()).value("ch_dishno").toString())
                         );
}

void w_bt_dish_suit_replace::torefresh()
{
    w_bt_dish_suit_new::torefresh();
}


bool w_bt_dish_suit_replace::toapp()
{
    QSet<int> currows = ui->tableView->getcurrows();
    int index;
    QString ch_dishno;
    QString _ch_dishno=_parent->tablemodel_2->record(_parent->ui->tableView_2->tableView->currentIndex().row()).value("ch_dishno").toString();
    foreach(int r, currows){
        ch_dishno=tablemodel->record(r).value("ch_dishno").toString();
        if((index=_parent->tablemodel_3->find_column_exist("ch_dishno", ch_dishno)) != -1){
            continue;
        }
        if(_ch_dishno==ch_dishno){
            lds_messagebox::warning(this, tr("消息提示"),tr("所选菜品与套菜配置菜品一样!"));
            _parent->tablemodel_3->select();
            return false;
        }

        int rowcount=_parent->tablemodel_3->rowCount();
        _parent->tablemodel_3->insertRow(rowcount);
        _parent->tablemodel_3->set_last_data("int_flowid", _parent->tablemodel_2->record(_parent->ui->tableView_2->tableView->currentIndex().row()).value("int_flowid").toString());
        _parent->tablemodel_3->set_last_data("ch_dishno", tablemodel->record(r).value("ch_dishno").toString());
        _parent->tablemodel_3->set_last_data("num_num_change", "1");
    }

    _parent->tablemodel_3->submitAll();
    return true;
}

void w_bt_dish_suit_replace::took()
{
    if(toapp())
        this->accept();
}

void w_bt_dish_suit_replace::toexit()
{
    this->reject();
}
