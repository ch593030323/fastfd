#include "w_bt_dish_base.h"
#include "ui_w_bt_dish_dialog.h"
#include <QSqlQueryModel>
#include "public_sql.h"
#include <QSqlError>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlRecord>
#include "lds_model_sqlrelationaltablemodel.h"
#include "lds_messagebox.h"
#include <QStandardItemModel>
#include "getchinesespell.h"
#include "lds_tableview_delegate_check.h"
#include "backheader.h"
#include <QSettings>
#include "lds_model_sqlrelationaltablemodel.h"
#include <QSqlRelation>
#include "w_bt_dish.h"
#include "w_sys_manage_basicdataexport.h"
#include "lds_combobox_model.h"
#include <QListWidget>

//盘点机：中智 大鹏 -> 好搭档

//菜品资料维护
//【0】重写selectStatement()实现 另加额外列的方法
//【1】排序不计算额外列， 特殊颜色区分
//【2】每个类独立写，不统一父类
//【3】排序允许函数必须设置开始，否则将丢失额外列
//【4】表格需要show之后，才可以重写排写表头，否则无效
//【5】    QTimer::singleShot(0, this,SLOT(to_headerview_repecify())); 调用该函数，来显示额外列
w_bt_dish_base::RelationalTableModel::RelationalTableModel(QObject *parent, QSqlDatabase db)
    : lds_model_sqlrelationaltablemodel(parent, db)
{
}

QVariant w_bt_dish_base::RelationalTableModel::data(const QModelIndex &item, int role) const
{
    QVariant _d=lds_model_sqlrelationaltablemodel::data(item, role);
    if(role==Qt::DisplayRole){
        if(this->headerData(item.column(), Qt::Horizontal,  Qt::DisplayRole).toString().endsWith(tr("税率%"))){
            return   QString().sprintf("%.02f",_d.toFloat()) +"%";
        }
    }
    return _d;
}
w_bt_dish_base::w_bt_dish_base(QWidget *parent) :
    QDialog(parent),
    ui(new Ui_w_bt_dish_Dialog),
    select_dish_image_dialog(0)
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
    tablemodel=new RelationalTableModel(this);

    tablemodel->setTable("cey_bt_dish");
    tablemodel->setEditStrategy(QSqlTableModel::OnManualSubmit);tablemodel->setJoinMode(QSqlRelationalTableModel::LeftJoin);
    tablemodel->setRelation(4/*ch_dish_typeno*/, QSqlRelation("cey_bt_dish_type", "ch_dish_typeno","vch_dish_typename"));
    tablemodel->setRelation(5/*ch_unitno*/, QSqlRelation("cey_bt_unit", "ch_unitno","vch_unitname"));
    tablemodel->setRelation(14/*ch_sub_typeno*/, QSqlRelation("cey_bt_dish_subtype", "ch_sub_typeno","vch_sub_typename"));
    tablemodel->setRelation(28/*vch_pur_unitno*/, QSqlRelation("cey_bt_unit", "ch_unitno","vch_unitname"));

    ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::SelectedClicked);ui->tableView->setModel(tablemodel->fpmodel);

    //! refresh
    torefresh();

    //重新布局表头和隐藏相关表头
    QList<int> virtualheades;
    tablemodel->save_set_header("ch_dishno",tr("品码"));
    tablemodel->save_set_header("vch_dishname",tr("品名"));
    tablemodel->save_set_header("cey_bt_unit_vch_unitname_2",tr("单位"));
    tablemodel->save_set_header("num_price",tr("价格"));
    tablemodel->save_set_header("ch_weight",tr("是否称重"));
    tablemodel->save_set_header("item_flag",tr("直销商品"));

    tablemodel->save_set_header("ch_suitflag",tr("套菜"));
    tablemodel->save_set_header("ch_off",tr("常用菜"));
    tablemodel->save_set_header("ch_discount",tr("折扣"));
    tablemodel->save_set_header("ch_curprice",tr("时价"));
    tablemodel->save_set_header("ch_tempflag",tr("临时菜"));

    tablemodel->save_set_header("ch_stopflag",tr("停用"));
    tablemodel->save_set_header("int_rate",tr("税率%"));
    tablemodel->save_set_header("vch_barcode", tr("条码"));
    virtualheades=ui->tableView->rePecifyHeader(tablemodel->save_set_header_saver);

    ui->tableView->setItemDelegate(
                new lds_tableview_delegate_check(
                    QVector<int>()<< virtualheades[4]
                << virtualheades[5]
            << virtualheades[6]
            << virtualheades[7]
            << virtualheades[8]
            << virtualheades[9]
            << virtualheades[10]<< virtualheades[11]
            ,
            ui->tableView
            ,
            true)
            );

    //! toolbar
    ui->widget->setup(QStringList()
                      <<tr("大类设置")
                      << tr("品类设置") << tr("单位设置") << tr("新增")
                      << tr("修改") << tr("删除") << tr("刷新") << tr("导出")
                      << tr("退出"));
    connect(ui->treeWidget,SIGNAL(pressed(QModelIndex)),this,SLOT(treeselect()));
    connect(ui->lineEdit,SIGNAL(textChanged(QString)),this,SLOT(treeselect()));
    connect(ui->checkBox,SIGNAL(toggled(bool)),this,SLOT(treeselect()));

    connect(ui->widget->index_widget(tr("大类设置")), SIGNAL(clicked()),this,SLOT(to_dish_type()));
    connect(ui->widget->index_widget(tr("品类设置")), SIGNAL(clicked()),this,SLOT(totypeset()));
    connect(ui->widget->index_widget(tr("单位设置")), SIGNAL(clicked()),this,SLOT(tounitset()));
    connect(ui->widget->index_widget(tr("新增")), SIGNAL(clicked()),this,SLOT(tonew()));
    connect(ui->widget->index_widget(tr("修改")), SIGNAL(clicked()),this,SLOT(tochange()));
    connect(ui->widget->index_widget(tr("删除")), SIGNAL(clicked()),this,SLOT(todel()));
    connect(ui->widget->index_widget(tr("刷新")), SIGNAL(clicked()),this,SLOT(torefresh()));
    connect(ui->widget->index_widget(tr("导出")), SIGNAL(clicked()),this,SLOT(toexport()));
    connect(ui->widget->index_widget(tr("退出")), SIGNAL(clicked()),this,SLOT(toexit()));
    //    connect(ui->tableView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(tochange()));
    //! after
    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    ui->checkBox->setChecked(conf.value("w_bt_dish_base/isstop").toBool());
}

w_bt_dish_base::~w_bt_dish_base()
{
}

void w_bt_dish_base::torefresh()
{
    //treewidget
    ui->treeWidget->deleteAllItem();
    QSqlQuery query;
    query.exec(
                "select nametype from("
                "select  concat(vch_dish_typename,'[',ch_dish_typeno,']')     as 'nametype', ch_dish_typeno  from cey_bt_dish_type"
                ")t  group by nametype order by ch_dish_typeno asc"
                );
    while(query.next()){
        lds_treeWidgetItem *item=new lds_treeWidgetItem(QStringList()
                                                        <<query.record().value("nametype").toString()
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
                                                             <<query_1.record().value("nametype").toString()
                                                             );
            item->addChild(item1);
        }
    }

    ui->treeWidget->restore();
    //tableview
    tablemodel->select();
    tablemodel->relationModel(4)->select();
    tablemodel->relationModel(5)->select();
    tablemodel->relationModel(14)->select();
    tablemodel->relationModel(28)->select();
    ui->tableView->restore();
}

void w_bt_dish_base::treeselect()
{
    QString sqlselect;
    //tree检索
    if(ui->treeWidget->currentItem()){
        int index=ui->treeWidget->deepofitem(ui->treeWidget->currentItem());
        if(index>0){
            sqlselect=backheader::getinterdata(ui->treeWidget->currentItem()->text(0));
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
    sqlselect +=QString("cey_bt_dish.ch_stopflag like '%1' ").arg(ui->checkBox->isChecked()?"%":"N");
    //过滤
    sqlselect +=QString("and (cey_bt_dish.ch_dishno like '%1%' or cey_bt_dish.vch_spell like '%2%' or cey_bt_dish.vch_dishname like '%1%') ")
            .arg(ui->lineEdit->text())
            .arg(ui->lineEdit->text().toUpper())
            ;
    //检索
    tablemodel->setFilter(sqlselect);
    tablemodel->select();
    tablemodel->relationModel(5)->select();
    tablemodel->relationModel(28)->select();
    ui->tableView->restore();

    //更新配置文件
    if(qobject_cast<QCheckBox *>(sender())){
        QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
        conf.setValue("w_bt_dish_base/isstop", qobject_cast<QCheckBox *>(sender())->isChecked());
    }
}

void w_bt_dish_base::to_dish_type()
{
    w_bt_dish_base_dish_type dialog(this);

    backheader::static_dialogexec(dialog,qobject_cast<QAbstractButton* >(sender())->text());
}

void w_bt_dish_base::totypeset()
{
    w_bt_dish_base_typeset dialog(this);

    backheader::static_dialogexec(dialog,qobject_cast<QAbstractButton* >(sender())->text());
}

void w_bt_dish_base::tounitset()
{
    w_bt_dish_base_unitset dialog(this);

    backheader::static_dialogexec(dialog,qobject_cast<QAbstractButton* >(sender())->text());
}

void w_bt_dish_base::tonew()
{
    if(ui->treeWidget->deepofitem(ui->treeWidget->currentItem())==2){
        QString vch_dish_typename=ui->treeWidget->currentItem()->parent()->text(0);
        QString vch_sub_typename=ui->treeWidget->currentItem()->text(0);
        w_bt_dish_base_new dialog(this, vch_dish_typename,vch_sub_typename);
        backheader::static_dialogexec(dialog,qobject_cast<QAbstractButton* >(sender())->text());
    } else {
        lds_messagebox::warning(this, tr("消息提示"),tr("请选择菜品小类！"));
    }
}

void w_bt_dish_base::tochange()
{
    int row=ui->tableView->currentIndex().row();
    if(row<0){
        lds_messagebox::warning(this, tr("消息提示"),tr("请选择将要修改的菜品记录!"));
        return;
    }
    w_bt_dish_base_change dialog(this);
    backheader::static_dialogexec(dialog,qobject_cast<QAbstractButton* >(sender())->text());
}

void w_bt_dish_base::todel()
{
    int row=ui->tableView->currentIndex().row();
    if(row<0){
        lds_messagebox::warning(this, tr("消息提示"),tr("请选择将要删除的菜品记录!"));
        return;
    }

    QString ls_dishno=tablemodel->index(row, 0).data().toString();
    QSqlQuery query;
    query.exec(QString("select count(*) from cey_u_orderdish where ch_dishno='%1'").arg(ls_dishno));
    if(query.next()){
        if(query.record().value(0).toInt()>0){
            lds_messagebox::warning(this, tr("消息提示"),tr("指定菜品记录已经营业操作,不能删除!"));
            return;
        }
    }
    query.exec(QString("select count(*) from cey_st_material_flow where ch_materialno='%1'").arg(ls_dishno));
    if(query.next()){
        if(query.record().value(0).toInt()>0){
            lds_messagebox::warning(this, tr("消息提示"),tr("指定菜品/或商品记录已经营业操作,不能删除!"));
            return;
        }
    }
    if(0==lds_messagebox::warning(this, tr("消息提示"),tr("确认删除指定菜品记录"),tr("确认"),tr("取消"))){
        //! <--transaction begin-->
        QSqlDatabase::database().transaction();
        if(query.exec(QString("delete from cey_bt_dish where ch_dishno='%1' ").arg(ls_dishno))){
            QSqlDatabase::database().commit();
            lds_messagebox::warning(this, tr("消息提示"),tr("删除成功!"));
            torefresh();
            ui->tableView->restore();
        } else {
            QSqlDatabase::database().rollback();
            torefresh();
            lds_messagebox::warning(this, tr("消息提示"),tr("数据有误，操作失败,删除失败!"), query.lastError().text());
        }
    }
    //! <--transaction end-->
}

void w_bt_dish_base::toexport()
{
    w_sys_manage_basicdataexport dialog(this);
    dialog.reset_export_tablenames(QStringList() << tablemodel->tableName());
    dialog.reset_export_confname(QString("%1/path").arg(this->metaObject()->className()));
    backheader::static_dialogexec(dialog,tr("基础数据导出"));

}

void w_bt_dish_base::toexit()
{
    if(tablemodel->model_is_dirty())tablemodel->submitAll();
    this->reject();
}

//============================
#include "ui_w_bt_dish_typeset_dialog.h"
#include "lds_tableview_delegate_check.h"


w_bt_dish_base_typeset::w_bt_dish_base_typeset(w_bt_dish_base *parent):QDialog(parent),ui(new Ui_w_bt_dish_typeset_Dialog),_parent(parent)
{
    ui->setupUi(this);

    //! tableview
    tablemodel=new lds_model_sqlrelationaltablemodel;
    tablemodel->setTable("cey_bt_dish_type");

    ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::SelectedClicked);ui->tableView->setModel(tablemodel->fpmodel);


    tablemodel_2=new lds_model_sqlrelationaltablemodel;
    tablemodel_2->setTable("cey_bt_dish_subtype");
    tablemodel_2->setEditStrategy(QSqlTableModel::OnManualSubmit);
    ui->tableView_2->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::SelectedClicked);ui->tableView_2->setModel(tablemodel_2->fpmodel);
    lds_tableview_delegate_check *delegate_2=new lds_tableview_delegate_check(
                QVector<int>()<<3,
                ui->tableView_2
                );
    delegate_2->setClickable();
    ui->tableView_2->setItemDelegate(delegate_2);
    //! refresh
    torefresh();
    tablemodel->save_set_header("ch_dish_typeno", tr("大类编码"));
    tablemodel->save_set_header("vch_dish_typename", tr("大类名称"));

    tablemodel_2->save_set_header("ch_sub_typeno", tr("小类编码"),true);
    tablemodel_2->save_set_header("vch_sub_typename", tr("小类名称"), true);
    tablemodel_2->save_set_header("ch_dish_typeno", tr("大类编码"));
    tablemodel_2->save_set_header("ch_labelprint_flag", tr("标签打印"), true);

    //! toolbar
    ui->widget_3->setup(QStringList()<<tr("新增")<<tr("删除")<<tr("保存")<<tr("刷新")<<tr("退出"),Qt::AlignRight);
    connect(ui->widget_3->index_widget(tr("新增")),SIGNAL(clicked()),this,SLOT(tonew2()));
    connect(ui->widget_3->index_widget(tr("删除")),SIGNAL(clicked()),this,SLOT(todel2()));
    connect(ui->widget_3->index_widget(tr("保存")),SIGNAL(clicked()),this,SLOT(tosave()));
    connect(ui->widget_3->index_widget(tr("刷新")),SIGNAL(clicked()),this,SLOT(torefresh()));
    connect(ui->widget_3->index_widget(tr("退出")),SIGNAL(clicked()),this,SLOT(toexit()));
    connect(ui->tableView,SIGNAL(clicked(QModelIndex)),this,SLOT(tableview2update(QModelIndex)));

    //! after

    torefresh();

}


void w_bt_dish_base_typeset::tonew2()
{
    //tableview 中没有选中行 则return
    QModelIndex rowindex=ui->tableView->currentIndex();
    if(rowindex.row()<0) return;

    //获取大类编码
    QString ls_typeno=tablemodel->record(rowindex.row()).value(0).toString();

    //获取小类编码 1-99 这儿没有通过数据获取，是因为需要多次新增
    int int_subtypeno=0;
    for(int r=0,r_count=tablemodel_2->rowCount();r<r_count;r++){
        int tmp=tablemodel_2->record(r).value(0).toString().right(2).toInt();
        if(int_subtypeno < tmp){
            int_subtypeno=tmp;
        }
    }
    int_subtypeno++;
    if(int_subtypeno>99){
        lds_messagebox::warning(this, tr("消息提示"),tr("小类编码后两位不能大于99"));
        return;
    }
    QString ls_subtypeno=QString().sprintf("%02d",int_subtypeno);
    ls_subtypeno=ls_typeno+ls_subtypeno;

    //新增初始化
    int rowcount=tablemodel_2->rowCount();
    tablemodel_2->insertRow(rowcount);
    tablemodel_2->setData(tablemodel_2->index(rowcount,0),ls_subtypeno);
    tablemodel_2->setData(tablemodel_2->index(rowcount,2),ls_typeno);

}

void w_bt_dish_base_typeset::todel2()
{
    QSqlQuery query;
    query.exec(QString("select count(*) from cey_bt_dish where ch_sub_typeno = '%1' ")
               .arg(tablemodel_2->record(ui->tableView_2->currentIndex().row()).value("ch_sub_typeno").toString()));
    query.next();
    if(query.record().value(0).toLongLong() > 0){
        lds_messagebox::warning(this, tr("消息提示"),tr("该小类下已有菜品，不能删除!"));
        return;
    }

    tablemodel_2->removeRow(ui->tableView_2->currentIndex().row());
}

bool w_bt_dish_base_typeset::tosave()
{
    if(tablemodel->submitAll()){
        if(tablemodel_2->submitAll()){
            lds_messagebox::warning(this, tr("消息提示"),/*tablemodel_2->lastError().text()*/tr("保存成功"));
            return true;
        }
    }
    lds_messagebox::warning(this, tr("消息提示"),/*tablemodel->lastError().text()*/tr("数据有误，操作失败"));
    return false;
}

void w_bt_dish_base_typeset::torefresh()
{
    tablemodel->select();
    tablemodel_2->select();

    ui->tableView->restore();
}

void w_bt_dish_base_typeset::toexit()
{
    if(tablemodel_2->model_is_dirty()||tablemodel->model_is_dirty()){
        if(0==lds_messagebox::warning(this, tr("消息提示"),tr("有数据未保存,确认保存?")+tablemodel_2->dirty_err_string(), tr("确认"),tr("取消"))){
            if(!tosave()){
                return;
            }
        }
    }
    //    if(tablemodel->model_is_dirty()
    //            &&!tablemodel->submitAll()){
    //        lds_messagebox::warning(this, tr("消息提示"),tablemodel->lastError().text());
    //        return;
    //    }
    if(tablemodel_2->model_is_dirty()
            &&!tablemodel_2->submitAll()){
        lds_messagebox::warning(this, tr("消息提示"),tablemodel->lastError().text());
        return;
    }
    _parent->torefresh();
    this->reject();
}

void w_bt_dish_base_typeset::tableview2update(const QModelIndex &index)
{
    if(tablemodel_2->model_is_dirty()){
        lds_messagebox::warning(this, tr("消息提示"),tr("有数据未保存!")+tablemodel_2->dirty_err_string());
        ui->tableView->select_beforerow();;
        return;
    }

    //    if(tablemodel_2->model_is_dirty()
    //            &&!tablemodel_2->submitAll()){
    //        ui->tableView->select_beforerow();
    //        lds_messagebox::warning(this, tr("消息提示"),tablemodel_2->lastError().text());
    //        return;
    //    }
    tablemodel_2->setFilter(QString("ch_dish_typeno='%1'").arg(tablemodel->record(index.row()).value(0).toString()));
    tablemodel_2->select();
}

//========================
#include "ui_w_bt_dish_unitset_dialog.h"
#include "lds_tableview_delegate_com.h"

w_bt_dish_base_unitset::w_bt_dish_base_unitset(QWidget *parent):QDialog(parent),ui(new Ui_w_bt_dish_unitset_Dialog)
{
    ui->setupUi(this);

    //! tableview
    tablemodel=new lds_model_sqlrelationaltablemodel_tmp;
    tablemodel->setTable("cey_bt_unit");
    tablemodel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    tablemodel->save_set_header("ch_unitno",tr("编码"), true);
    tablemodel->save_set_header("vch_unitname",tr("单位"), true);
    tablemodel->save_set_header("ch_numtype",tr("数量类型"), true);
    tablemodel->save_set_header("num_default",tr("默认数量"), true);

    //获取com的数据代理
    QMap<int, QStringList> intlistmap;
    intlistmap.insert(2, QStringList()<<tr("整数")<<tr("小数"));
    lds_tableview_delegate_com *delegate=new lds_tableview_delegate_com(intlistmap, ui->tableView);
    ui->tableView->setItemDelegate(delegate);

    ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::SelectedClicked);ui->tableView->setModel(tablemodel->fpmodel);

    //! toolbar
    ui->widget->setup(QStringList()<<tr("新增")<<tr("删除")<<tr("刷新")<<tr("保存")<<tr("退出"),Qt::AlignRight);
    connect(ui->widget->index_widget(tr("新增")),SIGNAL(clicked()),this,SLOT(tonew()));
    connect(ui->widget->index_widget(tr("删除")),SIGNAL(clicked()),this,SLOT(todel()));
    connect(ui->widget->index_widget(tr("刷新")),SIGNAL(clicked()),this,SLOT(torefresh()));
    connect(ui->widget->index_widget(tr("保存")),SIGNAL(clicked()),this,SLOT(tosave()));
    connect(ui->widget->index_widget(tr("退出")),SIGNAL(clicked()),this,SLOT(toexit()));

    QTimer::singleShot(100, this, SLOT(torefresh()));

}

void w_bt_dish_base_unitset::tonew()
{
    int rowcount=tablemodel->rowCount();
    tablemodel->insertRow(rowcount);
    tablemodel->setData(tablemodel->index(rowcount,0), tablemodel->max_column_add_1("ch_unitno"));
    tablemodel->setData(tablemodel->index(rowcount,2), 1);
    tablemodel->setData(tablemodel->index(rowcount,3),1);
}

void w_bt_dish_base_unitset::todel()
{
    int row=ui->tableView->currentIndex().row();
    if(row<0){
        //请先选择删除记录
        lds_messagebox::warning(this, tr("消息提示"),tr("请先选择删除记录!"));
        return;
    }
    QString ls_typeno=tablemodel->record(row).value(0).toString();
    QSqlQuery query;
    query.exec(QString("select count(*) from cey_bt_dish where ch_unitno='%1'").arg(ls_typeno));
    query.next();
    if(query.record().value(0).toLongLong()>0){
        //指定单位已存在菜品记录，不能删除!
        lds_messagebox::warning(this, tr("消息提示"),tr("指定单位已存在菜品记录，不能删除!"));
        return;
    }
    tablemodel->removeRow(row);
    ui->tableView->gonext();
}

bool w_bt_dish_base_unitset::tosave()
{
    //有的话，则保存
    QString ls_unitno;
    QString ls_unitname;
    for(int r=0,r_count=tablemodel->rowCount();r<r_count;r++){
        ls_unitno=tablemodel->record(r).value(0).toString();
        ls_unitname=tablemodel->record(r).value(1).toString();

        if(ls_unitno.trimmed().isEmpty()){
            lds_messagebox::warning(this, tr("消息提示"),tr("第%1行，编码为空，不能保存!").arg(r+1));
            return false;
        }
        if(ls_unitname.trimmed().isEmpty()){
            lds_messagebox::warning(this, tr("消息提示"),tr("第%1行，单位为空，不能保存!").arg(r+1));
            return false;
        }

        if(findunitno(ls_unitno, r)!=-1){
            lds_messagebox::warning(this, tr("消息提示"),tr("编码有重复，请核实!"));
            return false;
        }
    }

    if(tablemodel->submitAll()){
        static_cast<w_bt_dish_base *>(this->parent())->torefresh();
        lds_messagebox::warning(this, tr("消息提示"),/*tablemodel_2->lastError().text()*/tr("保存成功"));
        torefresh();
    } else {
        lds_messagebox::warning(this, tr("消息提示"),/*tablemodel->lastError().text()*/tr("数据有误，操作失败"));
        return false;
    }
    return true;
}

void w_bt_dish_base_unitset::torefresh()
{
    tablemodel->select();
}

void w_bt_dish_base_unitset::toexit()
{
    if(tablemodel->model_is_dirty()){
        if(0==lds_messagebox::warning(this, tr("消息提示"),tr("有数据未保存,确认保存?")+tablemodel->dirty_err_string(), tr("确认"),tr("取消"))){
            if(!tosave()){
                return;
            }
        }
    }

    //    if(tablemodel->model_is_dirty()
    //            &&tablemodel->submitAll()){
    //        lds_messagebox::warning(this, tr("消息提示"),tablemodel->lastError().text());
    //        return;
    //    }
    this->reject();
}

int w_bt_dish_base_unitset::findunitno(const QString &unitno, int exceptrow)
{
    for(int r=0,r_count=tablemodel->rowCount();r<r_count;r++){
        if(r!=exceptrow
                &&tablemodel->record(r).value(0).toString() == unitno)
            return r;
    }
    return -1;
}


QVariant w_bt_dish_base_unitset::lds_model_sqlrelationaltablemodel_tmp::data(const QModelIndex &idx, int role) const
{
    QVariant _d=lds_model_sqlrelationaltablemodel::data(idx, role);
    if(idx.column()==2&&role==Qt::DisplayRole){
        if(_d.toString()=="1"){
            return tr("整数");
        } else if(_d.toString()=="2"){
            return tr("小数");
        }
    }
    return _d;
}
//======================================
#include "ui_w_bt_dish_new_dialog.h"
#include <QDataWidgetMapper>
#include <QSqlRelationalDelegate>
#include <QTimer>

w_bt_dish_base_change::w_bt_dish_base_change(w_bt_dish_base *parent):
    QDialog(parent),
    ui(new Ui_w_bt_dish_new_Dialog),
    _parent(parent)
{
    ui->setupUi(this);
    //! datewidet
    lds_model_sqlrelationaltablemodel* tablemodel=_parent->tablemodel;

    backheader::lds_com_setitems(tablemodel->relationModel(5), ui->comboBox_ch_unitno);
    backheader::lds_com_setitems(tablemodel->relationModel(5), ui->comboBox_vch_pur_unitno);
    backheader::lds_com_setdatas(&w_bt_dish::MODEL_DISH_DEDUCTTYPE,ui->comboBox_ch_deducttype);

    mapper = new QDataWidgetMapper(this);
    mapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
    mapper->setModel(tablemodel);
    mapper->setItemDelegate(new QSqlRelationalDelegate(mapper));
    mapper->addMapping(ui->lineEdit_ch_dishno, tablemodel->fieldIndex("ch_dishno"));
    mapper->addMapping(ui->lineEdit_vch_dish_typename, tablemodel->fieldIndex("vch_dish_typename"));
    mapper->addMapping(ui->lineEdit_vch_sub_typename, tablemodel->fieldIndex("vch_sub_typename"));
    mapper->addMapping(ui->lineEdit_int_deduct, tablemodel->fieldIndex("int_deduct"));
    mapper->addMapping(ui->lineEdit_num_deduct, tablemodel->fieldIndex("num_deduct"));
    mapper->addMapping(ui->lineEdit_num_m_price, tablemodel->fieldIndex("num_m_price"));
    mapper->addMapping(ui->lineEdit_num_price, tablemodel->fieldIndex("num_price"));
    mapper->addMapping(ui->lineEdit_num_sale_price, tablemodel->fieldIndex("num_sale_price"));
    mapper->addMapping(ui->lineEdit_vch_dishname, tablemodel->fieldIndex("vch_dishname"));
    mapper->addMapping(ui->lineEdit_vch_spell, tablemodel->fieldIndex("vch_spell"));
    mapper->addMapping(ui->lineEdit_vch_subno, tablemodel->fieldIndex("vch_subno"));
    mapper->addMapping(ui->comboBox_ch_deducttype, 20, "comusrdata");//根据需要将index-1
    mapper->addMapping(ui->comboBox_ch_unitno,5);
    mapper->addMapping(ui->comboBox_vch_pur_unitno,28);
    mapper->addMapping(ui->checkBox_ch_curprice,tablemodel->fieldIndex("ch_curprice"),"checked1");
    mapper->addMapping(ui->checkBox_ch_deductflag,tablemodel->fieldIndex("ch_deductflag"),"checked1");
    mapper->addMapping(ui->checkBox_ch_discount,tablemodel->fieldIndex("ch_discount"),"checked1");
    mapper->addMapping(ui->checkBox_ch_lowflag,tablemodel->fieldIndex("ch_lowflag"),"checked1");
    mapper->addMapping(ui->checkBox_ch_off,tablemodel->fieldIndex("ch_off"),"checked1");
    mapper->addMapping(ui->checkBox_ch_serviceflag,tablemodel->fieldIndex("ch_serviceflag"),"checked1");
    mapper->addMapping(ui->checkBox_ch_suitflag,tablemodel->fieldIndex("ch_suitflag"),"checked1");
    mapper->addMapping(ui->checkBox_item_flag,tablemodel->fieldIndex("item_flag"),"checked1");
    mapper->addMapping(ui->checkBox_ch_stopflag,tablemodel->fieldIndex("ch_stopflag"),"checked1");
    mapper->addMapping(ui->checkBox_ch_tempflag,tablemodel->fieldIndex("ch_tempflag"),"checked1");
    mapper->addMapping(ui->checkBox_ch_weight,tablemodel->fieldIndex(("ch_weight")));
    mapper->addMapping(ui->lineEdit_vch_barcode,tablemodel->fieldIndex(("vch_barcode")));
    mapper->addMapping(ui->spinBox_int_unit_rate,tablemodel->fieldIndex("int_unit_rate"));
    mapper->addMapping(ui->spinBox_int_rate,tablemodel->fieldIndex(("int_rate")));

    //! init
    ui->frame->setEnabled(ui->checkBox_item_flag->isChecked());
    ui->frame_2->setEnabled(ui->checkBox_ch_deductflag->isChecked());

    //! toolbar
    ui->widget->setup(QStringList()<<tr("上一行")<<tr("下一行")<<tr("保存")<<tr("继续")<<tr("退出"), Qt::AlignRight);
    connect(ui->widget->index_widget(tr("保存")),SIGNAL(clicked()),this,SLOT(tosave()));
    connect(ui->widget->index_widget(tr("继续")),SIGNAL(clicked()),this,SLOT(tocontinue()));
    connect(ui->widget->index_widget(tr("上一行")),SIGNAL(clicked()),this,SLOT(toup()));
    connect(ui->widget->index_widget(tr("下一行")),SIGNAL(clicked()),this,SLOT(todown()));
    connect(ui->widget->index_widget(tr("退出")),SIGNAL(clicked()),this,SLOT(toexit()));

    connect(ui->lineEdit_vch_dishname,SIGNAL(textChanged(QString)),this,SLOT(spellupdate(QString)));
    connect(ui->comboBox_ch_deducttype,SIGNAL(currentIndexChanged(QString)),this,SLOT(update_ch_deducttype(QString)));

    //! after
    //    ui->widget->index_widget(tr("上一行"))->setVisible(false);
    //    ui->widget->index_widget(tr("下一行"))->setVisible(false);

    ui->widget->index_widget(tr("继续"))->setVisible(false);
    ui->label_record->setText(QString("%1/%2").arg(_parent->ui->tableView->currentIndex().row()+1).arg(_parent->ui->tableView->verticalHeader()->count()));

    ui->label_8->setVisible(false);
    ui->lineEdit_num_m_price->setVisible(false);
    ui->lineEdit_ch_dishno->setEnabled(false);
    ui->lineEdit_vch_dish_typename->setEnabled(false);
    ui->lineEdit_vch_sub_typename->setEnabled(false);
    static_cast<QPushButton*>(ui->widget->index_widget(tr("上一行")))->setDefault(false);static_cast<QPushButton*>(ui->widget->index_widget(tr("上一行")))->setAutoDefault(false);
    static_cast<QPushButton*>(ui->widget->index_widget(tr("下一行")))->setDefault(false);static_cast<QPushButton*>(ui->widget->index_widget(tr("下一行")))->setAutoDefault(false);
    static_cast<QPushButton*>(ui->widget->index_widget(tr("保存")))->setDefault(false);static_cast<QPushButton*>(ui->widget->index_widget(tr("保存")))->setAutoDefault(false);
    static_cast<QPushButton*>(ui->widget->index_widget(tr("继续")))->setDefault(false);static_cast<QPushButton*>(ui->widget->index_widget(tr("继续")))->setAutoDefault(false);
    static_cast<QPushButton*>(ui->widget->index_widget(tr("退出")))->setDefault(false);static_cast<QPushButton*>(ui->widget->index_widget(tr("退出")))->setAutoDefault(false);

    //! TIMESLOT
    QTimer::singleShot(0, this, SLOT(mapperupdate()));//信号槽的重写是需要这的写的，实现subclass 同时继承
}

bool w_bt_dish_base_change::tosave()
{
    QSqlQuery query;
    if(!ui->lineEdit_vch_barcode->text().isEmpty()){
        query.exec(QString("select count(*) from cey_bt_dish where vch_barcode = '%1' and ch_dishno <> '%2' ")
                   .arg(ui->lineEdit_vch_barcode->text())
                   .arg(ui->lineEdit_ch_dishno->text())
                   );
        query.next();
        if(query.record().value(0).toLongLong() > 0) {
            lds_messagebox::warning(this, tr("消息提示"),tr("条码不能重复!"));
            return false;
        }
    }
    //父类 model 提交
    if(_parent->tablemodel->submitAll()){
        //父类刷新，包括 父类 model 刷新
        _parent->torefresh();
        QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
        conf.setValue("w_bt_dish_base_image/"+ui->lineEdit_ch_dishno->text(), image_dish_path);
        //成功信息
        lds_messagebox::warning(this, tr("消息提示"),tr("保存成功"));
        return true;
    }
    //错误提示
    lds_messagebox::warning(this, tr("消息提示"),tr("数据有误，操作失败\n")+_parent->tablemodel->lastError().text());
    return false;
}

void w_bt_dish_base_change::tocontinue()
{


}

void w_bt_dish_base_change::toexit()
{
    if(_parent->tablemodel->model_is_dirty()){
        if(0==lds_messagebox::warning(this, tr("消息提示"),tr("有数据未保存,确认保存?"), tr("确定"),tr("取消"))){
            if(!tosave()) return;
        } else {
            _parent->torefresh();
        }
    }
    this->reject();
}

void w_bt_dish_base_change::toup()
{
    if(_parent->tablemodel->model_is_dirty()){
        if(0==lds_messagebox::warning(this, tr("消息提示"),tr("有数据未保存,确认保存?"), tr("确定"),tr("取消"))){
            if(!tosave()) return;
        } else {
            _parent->torefresh();
        }
    }
    _parent->ui->tableView->goprevious();
    mapperupdate();
    ui->label_record->setText(QString("%1/%2").arg(_parent->ui->tableView->currentIndex().row()+1).arg(_parent->ui->tableView->verticalHeader()->count()));
}

void w_bt_dish_base_change::todown()
{
    if(_parent->tablemodel->model_is_dirty()){
        if(0==lds_messagebox::warning(this, tr("消息提示"),tr("有数据未保存,确认保存?"), tr("确定"),tr("取消"))){
            if(!tosave()) return;
        } else {
            _parent->torefresh();
        }
    }
    _parent->ui->tableView->gonext();
    mapperupdate();
    ui->label_record->setText(QString("%1/%2").arg(_parent->ui->tableView->currentIndex().row()+1).arg(_parent->ui->tableView->verticalHeader()->count()));
}

void w_bt_dish_base_change::spellupdate(const QString &text)
{
    QString spell=getChineseSpell::ChineseSpell(text);
    spell=spell.toUpper();
    ui->lineEdit_vch_spell->setText(spell);
    ui->lineEdit_vch_subno->setText(spell);
}

void w_bt_dish_base_change::mapperupdate()
{
    mapper->setCurrentIndex(_parent->ui->tableView->currentIndex().row());
}

void w_bt_dish_base_change::update_ch_deducttype(const QString &type)
{
    if(type==tr("比例提成")){
        ui->lineEdit_int_deduct->setEnabled(true);
        ui->lineEdit_num_deduct->setEnabled(false);
    } else if(type==tr("定额提成")){
        ui->lineEdit_int_deduct->setEnabled(false);
        ui->lineEdit_num_deduct->setEnabled(true);
    }
}

void w_bt_dish_base_change::to_select_dishpic()
{
    image_dish_path.clear();
    if(_parent->select_dish_image_dialog==0){
        _parent->select_dish_image_dialog=new w_bt_dish_base_change_select_dish_image(this);
        _parent->select_dish_image_dialog->generate_image();
    } else {
        _parent->select_dish_image_dialog->update_match_info();
    }

    if(QDialog::Accepted==
            backheader::static_dialogexec(*_parent->select_dish_image_dialog,qobject_cast<QAbstractButton* >(sender())->text())
            ){
        image_dish_path=public_sql::image_dish_dir+"/"+_parent->select_dish_image_dialog->select_text;

    }
}

//=====================================
w_bt_dish_base_new::w_bt_dish_base_new(w_bt_dish_base *parent, const QString &vch_dish_typename, const QString &vch_sub_typename):
    w_bt_dish_base_change(parent),
    _vch_dish_typename(vch_dish_typename),
    _vch_sub_typename(vch_sub_typename)
{
    //! init
    ch_dish_typeno=backheader::getinterdata(_vch_dish_typename);
    ch_sub_typeno=backheader::getinterdata(_vch_sub_typename);
    tocontinue();

    //! after
    ui->widget->index_widget(tr("下一行"))->setVisible(false);
    ui->widget->index_widget(tr("上一行"))->setVisible(false);
    ui->widget->index_widget(tr("继续"))->setVisible(false);
    ui->label_record->setVisible(false);
    ui->lineEdit_ch_dishno->setEnabled(true);
}

bool w_bt_dish_base_new::tosave()
{
    QSqlQuery query;
    if(!ui->lineEdit_vch_barcode->text().isEmpty()){
        query.exec(QString("select count(*) from cey_bt_dish where vch_barcode = '%1' ").arg(ui->lineEdit_vch_barcode->text()));
        query.next();
        if(query.record().value(0).toLongLong() > 0) {
            lds_messagebox::warning(this, tr("消息提示"),tr("条码不能重复!"));
            return false;
        }
    }
    //开头不能为0
    QString ch_dishno=ui->lineEdit_ch_dishno->text();
    if(ch_dishno.startsWith("0")){
        lds_messagebox::warning(this, tr("消息提示"),tr("编号开头不能为0!"));
        return false;
    }
    if(ui->lineEdit_ch_dishno->text().isEmpty()){
        lds_messagebox::warning(this, tr("消息提示"),tr("菜品编码不能为空!"));
        return false;
    }
    if(ui->lineEdit_vch_dishname->text().isEmpty()){
        lds_messagebox::warning(this, tr("消息提示"),tr("菜品名称不能为空!"));
        return false;
    }
    if(ui->comboBox_ch_unitno->currentText().isEmpty()){
        lds_messagebox::warning(this, tr("消息提示"),tr("菜品单位不能为空!"));
        return false;
    }
    query.exec(QString("select ch_dishno from cey_bt_dish where ch_dishno='%1' ")
               .arg(ui->lineEdit_ch_dishno->text())
               );
    if(query.next()){
        lds_messagebox::warning(this, tr("消息提示"),tr("菜品编码有重复!"));
        return false;
    }

    //手动插入表格
    QString /*ch_dishno,*/vch_dishname,vch_spell,vch_subno,num_price,ch_unitno,item_flag,ch_suitflag,ch_curprice,ch_tempflag,ch_off,
            ch_discount,ch_stopflag,/*ch_dish_typeno,*//*ch_sub_typeno,*/vch_pur_unitno,int_unit_rate,num_sale_price,ch_serviceflag,ch_lowflag,
            ch_deductflag,ch_deducttype,int_deduct,num_deduct,int_rate,ch_weight,vch_barcode;
    //ch_dishno=ui->lineEdit_ch_dishno->text();
    vch_dishname=ui->lineEdit_vch_dishname->text();
    vch_spell=ui->lineEdit_vch_spell->text();
    vch_subno=ui->lineEdit_vch_subno->text();
    num_price=ui->lineEdit_num_price->text();

    ch_unitno=w_bt_dish::comcurrentvalue_0(ui->comboBox_ch_unitno);
    item_flag=ui->checkBox_item_flag->isChecked1();
    ch_suitflag=ui->checkBox_ch_suitflag->isChecked1();
    ch_curprice=ui->checkBox_ch_curprice->isChecked1();
    ch_tempflag=ui->checkBox_ch_tempflag->isChecked1();

    ch_off=ui->checkBox_ch_off->isChecked1();
    ch_discount=ui->checkBox_ch_discount->isChecked1();
    ch_stopflag=ui->checkBox_ch_stopflag->isChecked1();
    //ch_dish_typeno=ui->lineEdit_vch_dish_typename->text();//! 123
    //ch_sub_typeno=ui->lineEdit_vch_sub_typename->text();//! 23

    vch_pur_unitno=w_bt_dish::comcurrentvalue_0(ui->comboBox_vch_pur_unitno);//! 45
    int_unit_rate=QString::number(ui->spinBox_int_unit_rate->value());
    num_sale_price=ui->lineEdit_num_sale_price->text();
    ch_serviceflag=ui->checkBox_ch_serviceflag->isChecked1();
    ch_lowflag=ui->checkBox_ch_lowflag->isChecked1();

    ch_deductflag=ui->checkBox_ch_deductflag->isChecked1();
    ch_deducttype=w_bt_dish::comcurrentvalue_0(ui->comboBox_ch_deducttype);
    int_deduct=ui->lineEdit_int_deduct->text();
    num_deduct=ui->lineEdit_num_deduct->text();
    int_rate=QString::number(ui->spinBox_int_rate->value());
    ch_weight=ui->checkBox_ch_weight->isChecked1();
    vch_barcode=ui->lineEdit_vch_barcode->text();
    //! <--transaction begin-->
    QSqlDatabase::database().transaction();
    query.prepare(QString("insert into cey_bt_dish (ch_dishno,vch_dishname,vch_spell,vch_subno,num_price,ch_unitno,item_flag,ch_suitflag,ch_curprice,ch_tempflag,ch_off,ch_discount,ch_stopflag,ch_dish_typeno,ch_sub_typeno,vch_pur_unitno,int_unit_rate,num_sale_price,ch_serviceflag,ch_lowflag,ch_deductflag,ch_deducttype,int_deduct,num_deduct,int_rate,ch_weight,vch_barcode)values(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)"));
    query.addBindValue(ch_dishno);
    query.addBindValue(vch_dishname);
    query.addBindValue(vch_spell);
    query.addBindValue(vch_subno);
    query.addBindValue(num_price);

    query.addBindValue(ch_unitno);
    query.addBindValue(item_flag);
    query.addBindValue(ch_suitflag);
    query.addBindValue(ch_curprice);
    query.addBindValue(ch_tempflag);

    query.addBindValue(ch_off);
    query.addBindValue(ch_discount);
    query.addBindValue(ch_stopflag);
    query.addBindValue(ch_dish_typeno);
    query.addBindValue(ch_sub_typeno);

    query.addBindValue(vch_pur_unitno);
    query.addBindValue(int_unit_rate);
    query.addBindValue(num_sale_price);
    query.addBindValue(ch_serviceflag);
    query.addBindValue(ch_lowflag);

    query.addBindValue(ch_deductflag);
    query.addBindValue(ch_deducttype);
    query.addBindValue(int_deduct);
    query.addBindValue(num_deduct);
    query.addBindValue(int_rate);
    query.addBindValue(ch_weight);
    query.addBindValue(vch_barcode);

    if(query.exec()){
        QSqlDatabase::database().commit();
        _parent->torefresh();
        lds_messagebox::warning(this, tr("消息提示"),/*tablemodel_2->lastError().text()*/tr("保存成功"));
        QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
        conf.setValue("w_bt_dish_base_image/"+ui->lineEdit_ch_dishno->text(), image_dish_path);
        tocontinue();
        return true;
    } else {
        QSqlDatabase::database().rollback();
        _parent->torefresh();
        lds_messagebox::warning(this, tr("消息提示"),/*query.lastError().text()*/tr("数据有误，操作失败"), query.lastError().text());
        return false;
    }

    //! <--transaction end-->
}

void w_bt_dish_base_new::tocontinue()
{
    //更新mapper的com数据
    _parent->tablemodel->select();
    _parent->ui->tableView->restore();

    QString nextno=ch_dish_typeno+"000";
    QString preno=ch_dish_typeno;

    QSqlQuery query;
    query.exec(QString("select max(ch_dishno) from cey_bt_dish where ch_dish_typeno='%1' ")//and ch_sub_typeno='%2'
               .arg(ch_dish_typeno)
               //               .arg(ch_sub_typeno)
               );
    if(query.next()){
        QString tmpno=query.value(0).toString();
        preno=tmpno.left(2);
        nextno=(preno.isEmpty()?QString().sprintf("%02d",ch_dish_typeno.toInt()):preno)
                +QString().sprintf("%03d",tmpno.right(3).toInt()+1);
    }

    ui->lineEdit_ch_dishno->setText(nextno);
    ui->lineEdit_vch_sub_typename->setText(_vch_dish_typename.split("[")[0]);
    ui->lineEdit_vch_dish_typename->setText(_vch_sub_typename.split("[")[0]);
    ui->comboBox_ch_deducttype->setCurrentIndex(0);
    ui->comboBox_ch_unitno->setCurrentIndex(0);
    ui->comboBox_vch_pur_unitno->setCurrentIndex(0);
    ui->lineEdit_vch_dishname->clear();
    ui->lineEdit_vch_spell->clear();
    ui->spinBox_int_unit_rate->setValue(1);
    ui->checkBox_ch_discount->setChecked(1);
}

void w_bt_dish_base_new::toexit()
{
    _parent->torefresh();
    this->reject();
}

void w_bt_dish_base_new::mapperupdate()
{
    mapper->setCurrentIndex(-1);
    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    image_dish_path=conf.value("w_bt_dish_base_image/"+ui->lineEdit_ch_dishno->text(), ":/image/logo1.png").toString();

}
//=========================
w_bt_dish_base_dish_type::w_bt_dish_base_dish_type(w_bt_dish_base *parent):
    QDialog(parent),
    ui(new Ui_w_bt_dish_unitset_Dialog),
    _parent(parent)
{
    ui->setupUi(this);
    tablemodel=new lds_model_sqlrelationaltablemodel;
    tablemodel->setTable("cey_bt_dish_type");
    tablemodel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::SelectedClicked);ui->tableView->setModel(tablemodel);

    tablemodel->save_set_header("ch_dish_typeno", tr("大类编码"),true);
    tablemodel->save_set_header("vch_dish_typename", tr("大类名称"), true);

    //! toolbar
    ui->widget->setup(QStringList()<<tr("新增")<<tr("删除")<<tr("刷新")<<tr("保存")<<tr("退出"),Qt::AlignRight);
    connect(ui->widget->index_widget(tr("新增")),SIGNAL(clicked()),this,SLOT(tonew()));
    connect(ui->widget->index_widget(tr("删除")),SIGNAL(clicked()),this,SLOT(todel()));
    connect(ui->widget->index_widget(tr("刷新")),SIGNAL(clicked()),this,SLOT(torefresh()));
    connect(ui->widget->index_widget(tr("保存")),SIGNAL(clicked()),this,SLOT(tosave()));
    connect(ui->widget->index_widget(tr("退出")),SIGNAL(clicked()),this,SLOT(toexit()));

    QTimer::singleShot(100, this, SLOT(torefresh()));

}

void w_bt_dish_base_dish_type::tonew()
{
    tablemodel->insertRow(tablemodel->rowCount());
    QString value=tablemodel->max_column_add_1("ch_dish_typeno");
    if(value.toInt()<10){//开头不为零，并且为两位
        value=QString::number(10+value.toInt());
    }
    tablemodel->set_last_data("ch_dish_typeno", value);

}

void w_bt_dish_base_dish_type::todel()
{
    QSqlQuery query;
    query.exec(QString("select count(*) from cey_bt_dish_subtype where ch_dish_typeno = '%1' ")
               .arg(tablemodel->record(ui->tableView->currentIndex().row()).value("ch_dish_typeno").toString()));
    query.next();
    if(query.record().value(0).toLongLong() > 0){
        lds_messagebox::warning(this, tr("消息提示"),tr("该大类下已有小类，不能删除!"));
        return;
    }
    tablemodel->removeRow(ui->tableView->currentIndex().row());
}

bool w_bt_dish_base_dish_type::tosave()
{
    if(tablemodel->submitAll()){
        lds_messagebox::warning(this, tr("消息提示"),/*tablemodel_2->lastError().text()*/tr("保存成功"));
        return true;
    } else {
        lds_messagebox::warning(this, tr("消息提示"),/*tablemodel->lastError().text()*/tr("数据有误，操作失败"));
        return false;
    }
}

void w_bt_dish_base_dish_type::torefresh()
{
    tablemodel->select();
}

void w_bt_dish_base_dish_type::toexit()
{
    if(tablemodel->model_is_dirty()){
        if(0==lds_messagebox::warning(this, tr("消息提示"),tr("有数据未保存,确认保存?")+tablemodel->dirty_err_string(), tr("确认"),tr("取消"))){
            if(!tosave()){
                return;
            }
        }
    }
    //    if(tablemodel->model_is_dirty()
    //            &&!tosave()){
    //        return;
    //    }
    _parent->torefresh();
    this->reject();
}
//==========================================
#include "ui_w_bt_dish_base_change_select_dish_image_dialog.h"
#include <QListWidgetItem>
#include "w_sys_manage.h"
w_bt_dish_base_change_select_dish_image::w_bt_dish_base_change_select_dish_image(QWidget *parent) :
    QDialog(parent),
    ui(new Ui_w_bt_dish_base_change_select_dish_image_Dialog)
{
    ui->setupUi(this);

    ui->listWidget->setIconSize(QSize(100,100));
    ui->listWidget->setViewMode(QListWidget::IconMode);
    //! toolbar
    ui->widget->setup(QStringList() << tr("确定") << tr("退出"), Qt::AlignRight);
    connect(ui->widget->index_widget(tr("确定")), SIGNAL(clicked()),this,SLOT(took()));
    connect(ui->widget->index_widget(tr("退出")), SIGNAL(clicked()),this,SLOT(toexit()));
    connect(ui->lineEdit,SIGNAL(textChanged(QString)),this,SLOT(to_where_value()));
    connect(ui->checkBox,SIGNAL(toggled(bool)),this,SLOT(to_where_value()));
    connect(ui->checkBox,SIGNAL(toggled(bool)),this,SLOT(to_save_check(bool)));

    ui->widget_2->setEnabled(false);
    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    ui->checkBox->setChecked(conf.value("w_bt_dish_base_change_select_dish_image/check").toBool());

}

void w_bt_dish_base_change_select_dish_image::generate_image()
{
    one_by_one_list=w_sys_manage::get_flilename_path(public_sql::image_dish_dir);
    one_by_one_index=0;
    QTimer::singleShot(10, this, SLOT(one_by_one_import()));
}

void w_bt_dish_base_change_select_dish_image::update_match_info()
{
    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    conf.beginGroup("w_bt_dish_base_image");
    QStringList keys = conf.childKeys();
    //    settings.setValue("fridge/color", Qt::white);
    //    settings.setValue("fridge/size", QSize(32, 96));
    //    settings.beginGroup("fridge);
    //    childKeys: ["color", "size"]

    //遍历text_item_map
    QMap<QString, QListWidgetItem*>::iterator begin=text_item_map.begin();
    while(begin!=text_item_map.end()){
        QFont font=this->ui->listWidget->font();
        QColor color=Qt::black;

        //需找begin 在keys中寻找有匹配的
        foreach(QString key, keys){
            if(QFileInfo(conf.value(key).toString()).fileName()==begin.key()){
                //QFileInfo(conf.value(key).toString()) 获取image 的路径
                color=Qt::red;
                font.setBold(true);
                font.setStrikeOut(true);
                break;
            }
        }
        begin.value()->setTextColor(color);
        begin.value()->setFont(font);
        begin++;
    }

    //对菜品过滤
    to_where_value();
}

void w_bt_dish_base_change_select_dish_image::one_by_one_import()
{
    if(one_by_one_index>=one_by_one_list.count()) {
        update_match_info();
        ui->widget_2->setEnabled(true);
        return;
    }
    QListWidgetItem *item = new QListWidgetItem(ui->listWidget);
    item->setIcon(QIcon(public_sql::image_dish_dir+"/"+one_by_one_list[one_by_one_index]));
    item->setText(one_by_one_list[one_by_one_index]);
    item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
    ui->listWidget->addItem(item);

    one_by_one_index++;
    text_item_map.insert(item->text(), item);
    ui->label_num->setText(QString("%1/%2").arg(one_by_one_index).arg(one_by_one_list.count()));
    QTimer::singleShot(10, this, SLOT(one_by_one_import()));
}

void w_bt_dish_base_change_select_dish_image::took()
{
    if(ui->listWidget->currentItem())
        select_text=ui->lineEdit_image_filename->text();
    this->accept();
}

void w_bt_dish_base_change_select_dish_image::toexit()
{
    this->reject();
}

void w_bt_dish_base_change_select_dish_image::to_where_value()
{

    QMap<QString, QListWidgetItem*>::iterator begin=text_item_map.begin();
    bool flag;
    while(begin!=text_item_map.end()){
        flag=begin.key().startsWith(ui->lineEdit->text())
                &&!(!ui->checkBox->isChecked() && begin.value()->font().strikeOut())
                ;
        begin.value()->setHidden(!flag);
        begin++;
    }
}

void w_bt_dish_base_change_select_dish_image::to_save_check(bool flag)
{
    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    conf.setValue("w_bt_dish_base_change_select_dish_image/check", flag);
}

void w_bt_dish_base_change_select_dish_image::resizeEvent(QResizeEvent *e)
{
    QDialog::resizeEvent(e);
    ui->listWidget->reset();
}

void w_bt_dish_base_change_select_dish_image::closeEvent(QCloseEvent *e)
{
    e->ignore();
}
