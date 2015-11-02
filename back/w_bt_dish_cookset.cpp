#include "w_bt_dish_cookset.h"
#include "ui_w_bt_dish_cookset_dialog.h"
//菜品厨打设置
#include<QSqlQuery>
#include <QSqlRecord>
#include "lds_model_sqlrelationaltablemodel.h"

#include "lds_model_sqlrelationaltablemodel.h"
#include "lds_messagebox.h"
#include <QSqlError>
#include "lds_tableview_repecify.h"
#include "backheader.h"
#include "lds_tableview_delegate_check.h"
#include "w_sys_manage_basicdataexport.h"
#include "lds_messagebox.h"

w_bt_dish_cookset::w_bt_dish_cookset(QWidget *parent) :
    QDialog(parent),ui(new Ui_w_bt_dish_cookset_Dialog)
{
    ui->setupUi(this);
    //! treewidget
    ui->treeWidget->header()->hide();
    ui->treeWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->treeWidget->setColumnCount(1);
    ui->treeWidget->rootitem=new lds_treeWidgetItem(QStringList()
                                                    <<tr("全部类型"));
    ui->treeWidget->addTopLevelItem(ui->treeWidget->rootitem);
    //! tableview
    QStringList headerstmp_2;
    QStringList headerstmp_3;

    tablemodel_2=new lds_model_sqlrelationaltablemodel(this);
    tablemodel_2->setTable("cey_bt_dish");
    tablemodel_2->setEditStrategy(QSqlTableModel::OnManualSubmit);
    tablemodel_2->setJoinMode(QSqlRelationalTableModel::LeftJoin);
    tablemodel_2->setRelation(4/*ch_dish_typeno*/, QSqlRelation("cey_bt_dish_type", "ch_dish_typeno","vch_dish_typename"));
    tablemodel_2->setRelation(5/*ch_unitno*/, QSqlRelation("cey_bt_unit", "ch_unitno","vch_unitname"));
    tablemodel_2->setRelation(14/*ch_sub_typeno*/, QSqlRelation("cey_bt_dish_subtype", "ch_sub_typeno","vch_sub_typename"));

    ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::SelectedClicked);ui->tableView->setModel(tablemodel_2->fpmodel);

    QList<int> virtualheaders;
    tablemodel_2->setHeaderData(0,Qt::Horizontal,lds_tableview_repecify::saveHeader(tr("品码"),headerstmp_2));
    tablemodel_2->setHeaderData(1,Qt::Horizontal,lds_tableview_repecify::saveHeader(tr("品名"),headerstmp_2));
    tablemodel_2->setHeaderData(2,Qt::Horizontal,lds_tableview_repecify::saveHeader(tr("拼音简码"),headerstmp_2));
    tablemodel_2->setHeaderData(4,Qt::Horizontal,lds_tableview_repecify::saveHeader(tr("大类"),headerstmp_2));
    tablemodel_2->setHeaderData(14,Qt::Horizontal,lds_tableview_repecify::saveHeader(tr("小类"),headerstmp_2));
    tablemodel_2->setHeaderData(5,Qt::Horizontal,lds_tableview_repecify::saveHeader(tr("单位"),headerstmp_2));
    tablemodel_2->setHeaderData(6,Qt::Horizontal,lds_tableview_repecify::saveHeader(tr("价格"),headerstmp_2));
    tablemodel_2->setHeaderData(27,Qt::Horizontal,lds_tableview_repecify::saveHeader(tr("直销商品"),headerstmp_2));
    tablemodel_2->setHeaderData(7,Qt::Horizontal,lds_tableview_repecify::saveHeader(tr("套菜"),headerstmp_2));
    tablemodel_2->setHeaderData(30,Qt::Horizontal,lds_tableview_repecify::saveHeader(tr("折扣"),headerstmp_2));
    tablemodel_2->setHeaderData(9,Qt::Horizontal,lds_tableview_repecify::saveHeader(tr("时价"),headerstmp_2));
    tablemodel_2->setHeaderData(10,Qt::Horizontal,lds_tableview_repecify::saveHeader(tr("临时菜"),headerstmp_2));
    tablemodel_2->setHeaderData(11,Qt::Horizontal,lds_tableview_repecify::saveHeader(tr("停用"),headerstmp_2));
    virtualheaders=ui->tableView->rePecifyHeader( headerstmp_2);
    ui->tableView->setItemDelegate(new lds_tableview_delegate_check(QVector<int>()
                                                                    << virtualheaders[7]
                                   << virtualheaders[8]
            << virtualheaders[9]
            << virtualheaders[10]
            << virtualheaders[11]
            << virtualheaders[12],
            ui->tableView));
    sql_select_3="SELECT  cey_bt_cp_dish.ch_dishno ,           cey_bt_cp_dish.ch_no ,           cey_bt_cp_memo.vch_cp_memo ,           cey_bt_cp_memo.num_add_price     "
            "FROM cey_bt_cp_dish ,           cey_bt_cp_memo     WHERE(cey_bt_cp_dish.ch_no = cey_bt_cp_memo.ch_no)and         (( cey_bt_cp_dish.ch_dishno = '%1')) ";
    querymodel_3=new QSqlQueryModel;
    querymodel_3->setQuery(sql_select_3.arg("ch_dishno"));
    ui->tableView_2->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::SelectedClicked);ui->tableView_2->setModel(querymodel_3);

    querymodel_3->setHeaderData(1,Qt::Horizontal,lds_tableview_repecify::saveHeader(tr("编号"),headerstmp_3));
    querymodel_3->setHeaderData(2,Qt::Horizontal,lds_tableview_repecify::saveHeader(tr("特殊做法"),headerstmp_3));
    querymodel_3->setHeaderData(3,Qt::Horizontal,lds_tableview_repecify::saveHeader(tr("加价"),headerstmp_3));

    ui->tableView_2->rePecifyHeader(  headerstmp_3);

    //! refresh
    torefresh();
    //! toolbar
    ui->widget->setup(QStringList()
                      << tr("做法类型") << tr("特殊做法")
                      << tr("菜品做法设置") << tr("菜品做法删除")
                      << tr("刷新") << tr("导出")
                      << tr("退出"));
    connect(ui->treeWidget,SIGNAL(pressed(QModelIndex)),this,SLOT(t1_t2_t3()));
    connect(ui->lineEdit,SIGNAL(textChanged(QString)),this,SLOT(t1_t2_t3()));
    connect(ui->checkBox,SIGNAL(toggled(bool)),this,SLOT(t1_t2_t3()));

    connect(ui->widget->index_widget(tr("做法类型")), SIGNAL(clicked()),this,SLOT(tocooktype()));
    connect(ui->widget->index_widget(tr("特殊做法")), SIGNAL(clicked()),this,SLOT(tospecialcook()));
    connect(ui->widget->index_widget(tr("菜品做法删除")), SIGNAL(clicked()),this,SLOT(todel()));
    connect(ui->widget->index_widget(tr("菜品做法设置")), SIGNAL(clicked()),this,SLOT(todishset()));
    //    connect(ui->widget->index_widget(tr("批量设置")), SIGNAL(clicked()),this,SLOT(todishsets()));
    connect(ui->widget->index_widget(tr("刷新")), SIGNAL(clicked()),this,SLOT(torefresh()));
    connect(ui->widget->index_widget(tr("导出")), SIGNAL(clicked()),this,SLOT(toexport()));
    connect(ui->widget->index_widget(tr("退出")), SIGNAL(clicked()),this,SLOT(toexit()));
    connect(ui->tableView,SIGNAL(selectchanged(int)),this,SLOT(t2_t3()));
    ui->checkBox->setVisible(false);
}

void w_bt_dish_cookset::torefresh()
{
    //treewidget
    ui->treeWidget->deleteAllItem();
    QSqlQuery query;
    query.exec(
                "select nametype from("
                "select concat(vch_dish_typename,'[',ch_dish_typeno,']') as 'nametype', ch_dish_typeno  from cey_bt_dish_type"
                ")t  group by nametype order by ch_dish_typeno asc"
                );
    while(query.next()){
        lds_treeWidgetItem *item=new lds_treeWidgetItem(QStringList()
                                                        <<QString("%1")
                                                        .arg(query.record().value("nametype").toString())
                                                        );
        ui->treeWidget->rootitem->addChild(item);
        //获取[]的内容
        QSqlQuery query_1;
        QString no=query.record().value("nametype").toString();
        no=no.split("[")[1];
        no.chop(1);
        query_1.exec(QString(
                         "select concat(vch_sub_typename,'[',ch_sub_typeno,']') as 'nametype' from cey_bt_dish_subtype where ch_dish_typeno = '%1' order by ch_sub_typeno asc;"
                         )
                     .arg(no)
                     );
        while(query_1.next()){
            lds_treeWidgetItem *item1=new lds_treeWidgetItem(QStringList()
                                                             <<QString("%1")
                                                             .arg(query_1.record().value("nametype").toString())
                                                             );
            item->addChild(item1);
        }
    }
    ui->treeWidget->restore();
    //tableview
    t1_t2_t3();
}

void w_bt_dish_cookset::t1_t2_t3()
{
    t1_t2();
    t2_t3();
}

void w_bt_dish_cookset::t1_t2()
{
    t2_ch_dish_typeno.clear();
    QString sqlselect;
    //tree检索
    if(ui->treeWidget->currentItem()){
        int index=ui->treeWidget->deepofitem(ui->treeWidget->currentItem());
        if(index>0){
            QString text=ui->treeWidget->currentItem()->text(0);
            sqlselect=text.split("[")[1];
            sqlselect.chop(1);
            if(index==1){
                sqlselect=QString("cey_bt_dish.ch_dish_typeno='%1'").arg(sqlselect);
            } else if(index==2){
                sqlselect=QString("cey_bt_dish.ch_sub_typeno='%1'").arg(sqlselect);
            }
        }
    }
    if(sqlselect.isEmpty()){

    } else {
        sqlselect +=" and ";
    }
    //是否停用
    sqlselect +=QString("cey_bt_dish.ch_stopflag like '%1' ").arg("N");
    //过滤
    sqlselect += QString(" and (cey_bt_dish.ch_dishno like '%1%' or cey_bt_dish.vch_dishname like '%1%' or cey_bt_dish.vch_spell like '%1%')")
            .arg(ui->lineEdit->text());
    tablemodel_2->setFilter(sqlselect);
    tablemodel_2->select();
    ui->tableView->restore();

}

void w_bt_dish_cookset::t2_t3()
{
    t3_ch_dishno.clear();
    int row=ui->tableView->currentIndex().row();
    if(row>=0){
        t3_ch_dishno=tablemodel_2->record(row).value("ch_dishno").toString();
    }
    querymodel_3->setQuery(sql_select_3.arg(t3_ch_dishno));
    ui->tableView_2->restore();
}

void w_bt_dish_cookset::tocooktype()
{
    w_bt_dish_cookset_cooktype dialog(this);

    backheader::static_dialogexec(dialog,qobject_cast<QAbstractButton* >(sender())->text());
}

void w_bt_dish_cookset::tospecialcook()
{
    w_bt_dish_cookset_specialcook dialog(this);

    backheader::static_dialogexec(dialog,qobject_cast<QAbstractButton* >(sender())->text());
}

void w_bt_dish_cookset::todel()
{
    int row=ui->tableView_2->currentIndex().row();
    if(row<0){
        lds_messagebox::warning(this, tr("消息提示"),tr("请选择需要删除的做法记录!"));
        return;
    }

    QString ch_no=querymodel_3->record(row).value("ch_no").toString();
    QString ch_dishno=querymodel_3->record(row).value("ch_dishno").toString();
#ifdef QT_DEBUG
#else
    if(0==lds_messagebox::warning(this, tr("消息提示"),tr("确认删除指定做法记录"),tr("确认"),tr("取消"))){
#endif
        QSqlQuery query;
        QSqlDatabase::database().transaction();
        qDebug() << ch_dishno<<ch_no;
        if(query.exec(QString("delete from cey_bt_cp_dish where ch_dishno ='%1' and ch_no='%2'").arg(ch_dishno,ch_no))){
            QSqlDatabase::database().commit();
        } else {
            QSqlDatabase::database().rollback();
            lds_messagebox::warning(this, tr("消息提示"),tr("数据库错误!"));
        }
        torefresh();
#ifdef QT_DEBUG
#else
    }
#endif
}

void w_bt_dish_cookset::todishset()
{
    w_bt_dish_cookset_dishset dialog(this);

    backheader::static_dialogexec(dialog,qobject_cast<QAbstractButton* >(sender())->text());
}

void w_bt_dish_cookset::todishsets()
{
}

void w_bt_dish_cookset::toexport()
{
    w_sys_manage_basicdataexport dialog(this);
    dialog.reset_export_tablenames(QStringList() << tablemodel_2->tableName());
    dialog.reset_export_confname(QString("%1/path").arg(this->metaObject()->className()));
    backheader::static_dialogexec(dialog,tr("基础数据导出"));
}

void w_bt_dish_cookset::toexit()
{
    this->reject();
}

//=====================================
#include "ui_w_bt_dish_unitset_dialog.h"
#include "lds_model_sqlrelationaltablemodel.h"
w_bt_dish_cookset_cooktype::w_bt_dish_cookset_cooktype(QWidget *parent):QDialog(parent),
    ui(new Ui_w_bt_dish_unitset_Dialog)
{
    ui->setupUi(this);
    tablemodel=new lds_model_sqlrelationaltablemodel;
    tablemodel->setTable("cey_bt_cp_type");
    tablemodel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::SelectedClicked);ui->tableView->setModel(tablemodel->fpmodel);
    tablemodel->save_set_header("ch_typeno", tr("编号"), true);
    tablemodel->save_set_header("vch_typename", tr("特殊做法类型"), true);


    //! toolbar
    ui->widget->setup(QStringList()<<tr("新增")<<tr("删除")<<tr("刷新")<<tr("保存")<<tr("退出"), Qt::AlignRight);
    connect(ui->widget->index_widget(tr("新增")),SIGNAL(clicked()),this,SLOT(tonew()));
    connect(ui->widget->index_widget(tr("删除")),SIGNAL(clicked()),this,SLOT(todel()));
    connect(ui->widget->index_widget(tr("刷新")),SIGNAL(clicked()),this,SLOT(torefresh()));
    connect(ui->widget->index_widget(tr("保存")),SIGNAL(clicked()),this,SLOT(tosave()));
    connect(ui->widget->index_widget(tr("退出")),SIGNAL(clicked()),this,SLOT(toexit()));

    //! refresh
    QTimer::singleShot(100, this, SLOT(torefresh()));

}

void w_bt_dish_cookset_cooktype::tonew()
{
    int rowcount=tablemodel->rowCount();
    tablemodel->insertRow(rowcount);
    tablemodel->setData(tablemodel->index(rowcount, 0), tablemodel->max_column_add_1("ch_typeno"));
}

void w_bt_dish_cookset_cooktype::todel()
{
    int row=ui->tableView->currentIndex().row();
    if(row<0){
        lds_messagebox::warning(this, tr("消息提示"),tr("请先选择删除记录!"));
        return;
    }
    QString ls_typeno=tablemodel->record(row).value(0).toString();
    QSqlQuery query;
    query.exec(QString("select count(*) from cey_bt_cp_memo where ch_typeno='%1'").arg(ls_typeno));
    query.next();
    if(query.record().value(0).toLongLong()>0){
        lds_messagebox::warning(this, tr("消息提示"),tr("指定类型已存在厨打说明，不能删除!"));
        return;
    }
    tablemodel->removeRow(row);
}

bool w_bt_dish_cookset_cooktype::tosave()
{
    QString ls_unitno;
    QString ls_unitname;
    for(int r=0,r_count=tablemodel->rowCount();r<r_count;r++){
        if(tablemodel->record(r).value("ch_typeno").toString().isEmpty()){
            lds_messagebox::warning(this, tr("消息提示"),tr("编码为空，不能保存!"));
            return false;
        }
        if(tablemodel->record(r).value("vch_typename").toString().isEmpty()){
            lds_messagebox::warning(this, tr("消息提示"),tr("特殊做法类型为空，不能保存!"));
            return false;
        }
        if(tablemodel->same_column_exist("ch_typeno", r)){
            lds_messagebox::warning(this, tr("消息提示"),tr("编码有重复，请核实!"));
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

void w_bt_dish_cookset_cooktype::torefresh()
{
    tablemodel->select();
}

void w_bt_dish_cookset_cooktype::toexit()
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

//================================
#include "ui_w_bt_dish_cookset_specialcook_dialog.h"
w_bt_dish_cookset_specialcook::w_bt_dish_cookset_specialcook(QWidget *parent):
    QDialog(parent),ui(new Ui_w_bt_dish_cookset_specialcook__Dialog)
{
    ui->setupUi(this);
    //! tableview

    tablemodel=new lds_model_sqlrelationaltablemodel;
    tablemodel->setTable("cey_bt_cp_type");
    tablemodel->setEditStrategy(QSqlTableModel::OnManualSubmit);

    tablemodel_2=new lds_model_sqlrelationaltablemodel;
    tablemodel_2->setTable("cey_bt_cp_memo");
    tablemodel_2->setEditStrategy(QSqlTableModel::OnManualSubmit);


    ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::SelectedClicked);ui->tableView->setModel(tablemodel->fpmodel);
    ui->tableView_2->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::SelectedClicked);ui->tableView_2->setModel(tablemodel_2->fpmodel);

    tablemodel->save_set_header("ch_typeno", tr("编号"));
    tablemodel->save_set_header("vch_typename", tr("特殊做法类型"));

    tablemodel_2->save_set_header("ch_no", tr("编号"), true);
    tablemodel_2->save_set_header("vch_cp_memo", tr("特殊做法"), true);
    tablemodel_2->save_set_header("ch_typeno", tr("特殊做法类型"));
    tablemodel_2->save_set_header("num_add_price", tr("加价"), true);
    //! toolbar

    ui->widget->setup(QStringList()<<tr("新增")<<tr("删除")<<tr("刷新")<<tr("保存")<<tr("退出"),Qt::AlignRight);
    connect(ui->widget->index_widget(tr("新增")),SIGNAL(clicked()),this,SLOT(tonew()));
    connect(ui->widget->index_widget(tr("删除")),SIGNAL(clicked()),this,SLOT(todel()));
    connect(ui->widget->index_widget(tr("刷新")),SIGNAL(clicked()),this,SLOT(torefresh()));
    connect(ui->widget->index_widget(tr("保存")),SIGNAL(clicked()),this,SLOT(tosave()));
    connect(ui->widget->index_widget(tr("退出")),SIGNAL(clicked()),this,SLOT(toexit()));
    connect(ui->tableView,SIGNAL(clicked(QModelIndex)),this,SLOT(totable_2()));

    //! refresh
    QTimer::singleShot(100, this, SLOT(torefresh()));

}

void w_bt_dish_cookset_specialcook::tonew()
{
    if(ui->tableView->currentIndex().row() < 0) {
        lds_messagebox::warning(this, tr("消息提示"),tr("特殊做法类型没有选中行!"));
        return;
    }
    int rowcount=tablemodel_2->rowCount();
    tablemodel_2->insertRow(rowcount);
    QSqlQuery query;
    query.exec("select max(ch_no)+1 as ch_no_1 from cey_bt_cp_memo");
    query.next();
    tablemodel_2->setData(tablemodel_2->index(rowcount,0),QString::number(qMax(query.record().value("ch_no_1").toInt(), tablemodel_2->max_column_add_1("ch_no").toInt())).rightJustified(3, '0', true));
    tablemodel_2->setData(tablemodel_2->index(rowcount,2),tablemodel->record(ui->tableView->currentIndex().row()).value("ch_typeno").toString());
    tablemodel_2->setData(tablemodel_2->index(rowcount,3), "0");
}

void w_bt_dish_cookset_specialcook::todel()
{
    tablemodel_2->removeRow(ui->tableView_2->currentIndex().row());
}

bool w_bt_dish_cookset_specialcook::tosave()
{
    for(int r=0,r_count=tablemodel_2->rowCount();r<r_count;r++){
        if(tablemodel_2->record(r).value("ch_no").toString().isEmpty()){
            lds_messagebox::warning(this, tr("消息提示"),tr("编号为空，不能保存!"));
            return false;
        }
        if(tablemodel_2->record(r).value("vch_cp_memo").toString().isEmpty()){
            lds_messagebox::warning(this, tr("消息提示"),tr("特殊做法为空，不能保存!"));
            return false;
        }
        if(tablemodel_2->same_column_exist("ch_no", r)){
            lds_messagebox::warning(this, tr("消息提示"),tr("编码有重复，请核实!"));
            return false;
        }
    }

    if(tablemodel_2->submitAll()){
        lds_messagebox::warning(this, tr("消息提示"),/*tablemodel_2->lastError().text()*/tr("保存成功"));
        return true;
    } else {
        lds_messagebox::warning(this, tr("消息提示"),/*tablemodel->lastError().text()*/tr("数据有误，操作失败"), tablemodel_2->lastError().text());
        return false;
    }
}

void w_bt_dish_cookset_specialcook::torefresh()
{
    tablemodel->select();
    tablemodel_2->select();

    ui->tableView->restore();
    //    ui->tableView_2->restore();
}

void w_bt_dish_cookset_specialcook::toexit()
{
    if(tablemodel_2->model_is_dirty()){
        if(0==lds_messagebox::warning(this, tr("消息提示"),tr("有数据未保存,确认保存?")+tablemodel_2->dirty_err_string(), tr("确认"),tr("取消"))){
            if(!tosave()){
                return;
            }
        }
    }
    //    if(tablemodel_2->model_is_dirty()){
    //        if(!tosave()) return;
    //    }
    this->reject();
}

void w_bt_dish_cookset_specialcook::totable_2()
{
    if(tablemodel_2->model_is_dirty()){
        if(0==lds_messagebox::warning(this, tr("消息提示"),tr("有数据未保存,确认保存?")+tablemodel_2->dirty_err_string(), tr("确认"),tr("取消"))){
            if(!tosave()){
                ui->tableView->select_beforerow();
                return;
            }
        }
    }
    //    if(tablemodel_2->model_is_dirty()){
    //        if(!tosave()) return;
    //    }
    tablemodel_2->setFilter(QString("ch_typeno='%1'")
                            .arg(tablemodel->record(ui->tableView->currentIndex().row()).value("ch_typeno").toString())
                            );
    tablemodel_2->select();
    //    ui->tableView_2->restore();
}


//=============================
#include "ui_w_bt_dish_cookset_dishset_dialog.h"
w_bt_dish_cookset_dishset::w_bt_dish_cookset_dishset(QWidget *parent):
    QDialog(parent),ui(new Ui_w_bt_dish_cookset_dishset_Dialog),_parent(static_cast<w_bt_dish_cookset *>(parent))
{
    ui->setupUi(this);
    //! toolbar

    ui->widget->setup(QStringList()<</*tr("全选")<<tr("全不选")<<*/tr("应用到所有") << tr("应用")<<tr("确认")<<tr("退出"),Qt::AlignRight);
    //! treewidget
    ui->treeWidget->header()->hide();
    ui->treeWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->treeWidget->setColumnCount(1);
    ui->treeWidget->rootitem=new lds_treeWidgetItem(QStringList()
                                                    <<tr("全部类型"));
    ui->treeWidget->addTopLevelItem(ui->treeWidget->rootitem);

    //! tableview
    tablemodel_2=new lds_model_sqlrelationaltablemodel;
    tablemodel_2->setTable("cey_bt_cp_memo");
    tablemodel_2->setEditStrategy(QSqlTableModel::OnManualSubmit);

    ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::SelectedClicked);ui->tableView->setModel(tablemodel_2->fpmodel);
    ui->tableView->setSelectionMode(QAbstractItemView::MultiSelection);


    //! refresh
    torefresh();
    QStringList headerstmp;
    QList<int> virtualheades;
    tablemodel_2->setHeaderData(0,Qt::Horizontal,lds_tableview_repecify::saveHeader(tr("编号"),headerstmp));
    tablemodel_2->setHeaderData(1,Qt::Horizontal,lds_tableview_repecify::saveHeader(tr("特殊做法"),headerstmp));
    tablemodel_2->setHeaderData(3,Qt::Horizontal,lds_tableview_repecify::saveHeader(tr("加价"),headerstmp));
    virtualheades=ui->tableView->rePecifyHeader( headerstmp);
    //! connect

    //    connect(ui->widget->index_widget(tr("全选")),SIGNAL(clicked()),this,SLOT(toselectall()));
    //    connect(ui->widget->index_widget(tr("全不选")),SIGNAL(clicked()),this,SLOT(toselectno()));
    connect(ui->widget->index_widget(tr("应用到所有")), SIGNAL(clicked()),this,SLOT(toappall()));
    connect(ui->widget->index_widget(tr("应用")),SIGNAL(clicked()),this,SLOT(toapp()));
    connect(ui->widget->index_widget(tr("确认")),SIGNAL(clicked()),this,SLOT(took()));
    connect(ui->widget->index_widget(tr("退出")),SIGNAL(clicked()),this,SLOT(toexit()));
    connect(ui->treeWidget,SIGNAL(pressed(QModelIndex)),this,SLOT(treeselect()));
}

void w_bt_dish_cookset_dishset::toselectall()
{
    ui->tableView->selectAll();
}

void w_bt_dish_cookset_dishset::toselectno()
{
    ui->tableView->clearSelection();
}

void w_bt_dish_cookset_dishset::toapp()
{
    //! <--transaction begin-->
    QString errtext;
    QSqlDatabase::database().transaction();
    QSqlQuery query;
    QSet<int>currows= ui->tableView->getcurrows();
    foreach(int r, currows){
        QString ch_dishno=tablemodel_2->record(r).value("ch_no").toString();
        bool isfind=false;
        for(int rr=0;rr<_parent->querymodel_3->rowCount();rr++){
            if(ch_dishno==_parent->querymodel_3->record(rr).value("ch_no").toString()){
                isfind=true;
                break;
            }
        }
        if(isfind) continue;
        query.prepare(QString("insert into cey_bt_cp_dish(ch_dishno, ch_no) values(?,?)"));
        query.addBindValue(_parent->t3_ch_dishno);
        query.addBindValue(tablemodel_2->record(r).value("ch_no").toString());
        if(query.exec()){
            continue;
        }
        errtext=query.lastError().text();
        break;
    }
    if(errtext.isEmpty()){
        QSqlDatabase::database().commit();
        _parent->torefresh();
        torefresh();
    } else {
        QSqlDatabase::database().rollback();
        _parent->torefresh();
        torefresh();
        lds_messagebox::warning(this, tr("消息提示"),/*tr("数据有误，操作失败!")*/errtext);
        qDebug() << errtext;
    }
    //! <--transaction end-->
}

void w_bt_dish_cookset_dishset::toappall()
{
    if(ui->tableView->selectionModel()->selectedRows().isEmpty()){
        lds_messagebox::warning(this, tr("消息提示"),tr("没有选中任何做法"));
        return;
    }

    //应用到全部，存在即更新，不存在insert
    if(1==lds_messagebox::warning(this, tr("消息提示"),tr("应用到所有将会覆盖以前的做法设置,是否继续"), tr("确认"),tr("取消"))){
        return;
    }

    //! <--transaction begin-->
    QString errtext;
    QSqlDatabase::database().transaction();
    QSqlQuery query;
    QSqlQuery query_insert;
    QSet<int>currows= ui->tableView->getcurrows();
    //delete所有菜品做法
    if(query.exec("delete from cey_bt_cp_dish")){//delete all
        QStringList ch_nos;
        foreach(int r, currows){
            ch_nos.append(tablemodel_2->record(r).value("ch_no").toString());
        }
        query.exec("select ch_dishno from cey_bt_dish where cey_bt_dish.ch_stopflag like 'N' ");
        while(query.next()){//1
            foreach(QString ch_no, ch_nos){//0
                query_insert.prepare("insert into cey_bt_cp_dish(ch_dishno, ch_no) values(?,?)");
                query_insert.addBindValue(query.record().value("ch_dishno").toString());
                query_insert.addBindValue(ch_no);
                if(query_insert.exec()){
                    continue;
                }
                errtext=query_insert.lastError().text();
                break;
            }//0
            if(errtext.isEmpty()){
                continue;
            } else {
                break;
            }
        }//1
    } else {//delete all
        errtext=query.lastError().text();
    }//delete all
    if(errtext.isEmpty()){
        QSqlDatabase::database().commit();
        _parent->torefresh();
        torefresh();
    } else {
        QSqlDatabase::database().rollback();
        _parent->torefresh();
        torefresh();
        lds_messagebox::warning(this, tr("消息提示"),/*tr("数据有误，操作失败!")*/errtext);
        qDebug() << errtext;
    }
    //! <--transaction end-->

    lds_messagebox::warning(this, tr("消息提示"),tr("应用完成"));
}

void w_bt_dish_cookset_dishset::took()
{
    toapp();
    this->accept();
}

void w_bt_dish_cookset_dishset::toexit()
{
    this->reject();
}

void w_bt_dish_cookset_dishset::torefresh()
{
    //treewidget
    ui->treeWidget->deleteAllItem();
    QSqlQuery query;
    query.exec(
                "select nametype from("
                "select concat(vch_typename,'[',ch_typeno,']') as 'nametype' from cey_bt_cp_type"
                ")t  group by nametype "
                );
    while(query.next()){
        lds_treeWidgetItem *item=new lds_treeWidgetItem(QStringList()
                                                        <<QString("%1")
                                                        .arg(query.record().value("nametype").toString())
                                                        );
        ui->treeWidget->rootitem->addChild(item);
    }
    ui->treeWidget->restore();
    //tableview
    treeselect();
    ui->tableView->restore();
}

void w_bt_dish_cookset_dishset::treeselect()
{
    t2_ch_typeno.clear();
    QString filter;
    if(ui->treeWidget->currentItem()){
        int index=ui->treeWidget->deepofitem(ui->treeWidget->currentItem());
        if(index==1){
            t2_ch_typeno=ui->treeWidget->currentItem()->text(0);
            t2_ch_typeno=t2_ch_typeno.split("[")[1];
            t2_ch_typeno.chop(1);
            filter=QString("ch_typeno='%1'").arg(t2_ch_typeno);
        }
    }
    tablemodel_2->setFilter(filter);
    tablemodel_2->select();
    ui->tableView->restore();
}

//==============================
//#include "ui_w_bt_dish_cookset_secialcook_dialog.h"
//w_bt_dish_cookset_specialcook::w_bt_dish_cookset_specialcook(QWidget *parent)：
//        QDialog(parent),ui(new Ui_w_bt_dish_specialcook_Dialog)
//{
//    ui->setupUi(this);
//}

