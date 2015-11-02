#include "w_bt_dish_table.h"
//餐桌资料
#include "ui_w_bt_dish_dialog.h"
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlRecord>
#include "lds_model_sqlrelationaltablemodel.h"
#include "lds_messagebox.h"
#include "backheader.h"
#include "w_bt_dish.h"
#include <QSqlError>
#include "lds_treewidgetitem.h"
#include "w_sys_manage_basicdataexport.h"

w_bt_dish_table::w_bt_dish_table(QWidget *parent) :
    QDialog(parent),ui(new Ui_w_bt_dish_Dialog)
{
    ui->setupUi(this);
    ui->label->setVisible(false);
    ui->lineEdit->setVisible(false);
    ui->checkBox->setVisible(false);
    //! toolbar
    ui->widget->setup(QStringList()
                      << tr("餐区设置") << tr("桌类设置") << tr("新增") << tr("批量新增")
                      << tr("修改") << tr("删除") <<  tr("刷新")
                      << tr("导出") << tr("退出"));
    //! treewidget
    ui->treeWidget->header()->hide();
    ui->treeWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->treeWidget->setColumnCount(1);
    ui->treeWidget->rootitem=new lds_treeWidgetItem(QStringList()
                                                    <<tr("全部类型"));
    ui->treeWidget->addTopLevelItem(ui->treeWidget->rootitem);
    //! tableview
    tablemodel=new lds_model_sqlrelationaltablemodel(this);
    tablemodel->setTable("cey_bt_table");
    tablemodel->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);tablemodel->setJoinMode(QSqlRelationalTableModel::LeftJoin);
    tablemodel->setRelation(2/*ch_typeno*/, QSqlRelation("cey_bt_table_type", "ch_typeno","vch_typename"));
    tablemodel->setRelation(9/*ch_areano*/, QSqlRelation("cey_bt_table_area", "ch_areano","vch_areaname"));
    ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::SelectedClicked);ui->tableView->setModel(tablemodel->fpmodel);

    QStringList headerstmp;
    QList<int> virtualheades;
    tablemodel->setHeaderData(0,Qt::Horizontal,lds_tableview_repecify::saveHeader(tr("餐桌号"),headerstmp));
    tablemodel->setHeaderData(1,Qt::Horizontal,lds_tableview_repecify::saveHeader(tr("餐桌名"),headerstmp));
    tablemodel->setHeaderData(2,Qt::Horizontal,lds_tableview_repecify::saveHeader(tr("餐桌类型"),headerstmp));
    tablemodel->setHeaderData(5,Qt::Horizontal,lds_tableview_repecify::saveHeader(tr("人数"),headerstmp));
    tablemodel->setHeaderData(6,Qt::Horizontal,lds_tableview_repecify::saveHeader(tr("备注"),headerstmp));
    tablemodel->setHeaderData(9,Qt::Horizontal,lds_tableview_repecify::saveHeader(tr("所属区域"),headerstmp));
    virtualheades=ui->tableView->rePecifyHeader(headerstmp);
    //! refresh
    torefresh();

    //! connect
    connect(ui->treeWidget,SIGNAL(pressed(QModelIndex)),this,SLOT(treeselect()));

    connect(ui->widget->index_widget(tr("新增")), SIGNAL(clicked()),this,SLOT(tonew()));
    connect(ui->widget->index_widget(tr("修改")), SIGNAL(clicked()),this,SLOT(tochange()));
    connect(ui->widget->index_widget(tr("删除")), SIGNAL(clicked()),this,SLOT(todel()));
    connect(ui->widget->index_widget(tr("批量新增")), SIGNAL(clicked()),this,SLOT(tonews()));
    connect(ui->widget->index_widget(tr("桌类设置")), SIGNAL(clicked()),this,SLOT(totable()));
    connect(ui->widget->index_widget(tr("餐区设置")), SIGNAL(clicked()),this,SLOT(toarea()));
    connect(ui->widget->index_widget(tr("刷新")), SIGNAL(clicked()),this,SLOT(torefresh()));
    connect(ui->widget->index_widget(tr("导出")), SIGNAL(clicked()),this,SLOT(toexport()));
    connect(ui->widget->index_widget(tr("退出")), SIGNAL(clicked()),this,SLOT(toexit()));

}

void w_bt_dish_table::torefresh()
{
    //treewidget
    ui->treeWidget->deleteAllItem();
    QSqlQuery query;
    query.exec("select concat(vch_typename,'[',ch_typeno,']') as 'nametype'  from cey_bt_table_type order by cey_bt_table_type.ch_typeno asc");
    while(query.next()){
        lds_treeWidgetItem *item=new lds_treeWidgetItem(QStringList()
                                                        <<QString("%1")
                                                        .arg(query.record().value("nametype").toString())
                                                        );
        ui->treeWidget->rootitem->addChild(item);
    }
    ui->treeWidget->restore();
    //tableview
    tablemodel->select();
    tablemodel->relationModel(2/*ch_typeno*/)->select();
    tablemodel->relationModel(9/*ch_areano*/)->select();

    ui->tableView->restore();
}

lds_treeWidget *w_bt_dish_table::getTreew()
{
    return ui->treeWidget;
}

lds_tableView *w_bt_dish_table::getTableview()
{
    return ui->tableView;
}

lds_model_sqlrelationaltablemodel *w_bt_dish_table::getTablemodel()
{
    return tablemodel;
}

void w_bt_dish_table::tonew()
{
    if(ui->treeWidget->deepofitem(ui->treeWidget->currentItem())==1){
        w_bt_dish_table_new dialog(this);

        backheader::static_dialogexec(dialog,qobject_cast<QAbstractButton* >(sender())->text());

    } else {
        lds_messagebox::warning(this, tr("消息提示"),tr("请先选择餐桌类型！"));
    }
}

void w_bt_dish_table::tochange()
{
    int row=ui->tableView->currentIndex().row();
    if(row>=0){
        w_bt_dish_table_change dialog(this);
        backheader::static_dialogexec(dialog,qobject_cast<QAbstractButton* >(sender())->text());
    }
}

void w_bt_dish_table::todel()
{
    int row=ui->tableView->currentIndex().row();
    if(row<0){
        lds_messagebox::warning(this, tr("消息提示"),tr("请选择将要删除的餐桌记录!"));
        return;
    }

    QString ls_dishno=tablemodel->index(row, 0).data().toString();
    QSqlQuery query;
    query.exec(QString("select count(*) from cey_u_table where ch_tableno='%1'").arg(ls_dishno));
    if(query.next()){
        if(query.record().value(0).toInt()>0){
            lds_messagebox::warning(this, tr("消息提示"),tr("指定餐桌记录已经营业操作,不能删除!"));
            return;
        }
    }
    if(0==lds_messagebox::warning(this, tr("消息提示"),tr("确认删除指定餐桌记录"),tr("确认"),tr("取消"))){
        //! <--transaction begin-->
        QSqlDatabase::database().transaction();
        if(query.exec(QString("delete from cey_bt_table where ch_tableno='%1' ").arg(ls_dishno))){
            QSqlDatabase::database().commit();
            treeselect();
            lds_messagebox::warning(this, tr("消息提示"),tr("删除成功!"));
        } else {
            QSqlDatabase::database().rollback();
            lds_messagebox::warning(this, tr("消息提示"),tr("数据有误，操作失败,删除失败!"));
        }
        //! <--transaction end-->
    }
}

void w_bt_dish_table::tonews()
{
    w_bt_dish_table_news dialog(this);
    backheader::static_dialogexec(dialog,qobject_cast<QAbstractButton* >(sender())->text());
}

void w_bt_dish_table::totable()
{
    w_bt_dish_table_tableset dialog(this);
    backheader::static_dialogexec(dialog,qobject_cast<QAbstractButton* >(sender())->text());
}

void w_bt_dish_table::toarea()
{
    w_bt_dish_table_areaset dialog(this);
    backheader::static_dialogexec(dialog,qobject_cast<QAbstractButton* >(sender())->text());
}

void w_bt_dish_table::toexport()
{
    w_sys_manage_basicdataexport dialog(this);
    dialog.reset_export_tablenames(QStringList() << tablemodel->tableName());
    dialog.reset_export_confname(QString("%1/path").arg(this->metaObject()->className()));
    backheader::static_dialogexec(dialog,tr("基础数据导出"));

}

void w_bt_dish_table::toexit()
{
    this->reject();
}

void w_bt_dish_table::treeselect()
{
    QString sqlselect;
    if(ui->treeWidget->currentItem()){
        int index=ui->treeWidget->deepofitem(ui->treeWidget->currentItem());
        if(index==1){
            sqlselect=backheader::getinterdata(ui->treeWidget->currentItem()->text(0));
        }

        tablemodel->setFilter(QString("cey_bt_table.ch_typeno like '%1%'").arg(sqlselect));
        tablemodel->select();
        tablemodel->relationModel(2/*ch_typeno*/)->select();
        tablemodel->relationModel(9/*ch_areano*/)->select();

        ui->tableView->restore();
    }
}

//==============================
#include "ui_w_bt_dish_table_new_dialog.h"
#include "lds_model_sqlrelationaltablemodel.h"
#include <QDataWidgetMapper>
#include <QSqlRelationalDelegate>
#include "mainwindow.h"

w_bt_dish_table_change::w_bt_dish_table_change(QWidget *parent, bool ischange):
    QDialog(parent),
    ui(new Ui_w_bt_dish_table_new_Dialog),
    _parent(static_cast<w_bt_dish_table* >(parent)),
    _ischange(ischange)
{
    ui->setupUi(this);
    //! datawidget
    ui->comboBox_vch_typename->setModel(_parent->tablemodel->relationModel(2));
    ui->comboBox_vch_typename->setModelColumn(1);
    ui->comboBox_vch_areaname->setModel(_parent->tablemodel->relationModel(9));
    ui->comboBox_vch_areaname->setModelColumn(1);

    mapper = new QDataWidgetMapper(this);
    mapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
    mapper->setModel(_parent->tablemodel);
    mapper->setItemDelegate(new QSqlRelationalDelegate(mapper));
    mapper->addMapping(ui->lineEdit_ch_tableno, _parent->tablemodel->fieldIndex("ch_tableno"));
    mapper->addMapping(ui->lineEdit_int_person, _parent->tablemodel->fieldIndex("int_person"));
    mapper->addMapping(ui->lineEdit_vch_memo, _parent->tablemodel->fieldIndex("vch_memo"));
    mapper->addMapping(ui->lineEdit_vch_tablename, _parent->tablemodel->fieldIndex("vch_tablename"));
    mapper->addMapping(ui->comboBox_vch_typename,2);//, "currentIndex");
    mapper->addMapping(ui->comboBox_vch_areaname, 9);//, "currentIndex");

    //! toolbar
    ui->widget->setup(QStringList()<<tr("上一行")<<tr("下一行")<<tr("保存")<<tr("继续")<<tr("退出"), Qt::AlignRight);
    connect(ui->widget->index_widget(tr("上一行")),SIGNAL(clicked()),this,SLOT(toup()));
    connect(ui->widget->index_widget(tr("下一行")),SIGNAL(clicked()),this,SLOT(todown()));
    connect(ui->widget->index_widget(tr("保存")),SIGNAL(clicked()),this,SLOT(tosave()));
    connect(ui->widget->index_widget(tr("继续")),SIGNAL(clicked()),this,SLOT(tocontinue()));
    connect(ui->widget->index_widget(tr("退出")),SIGNAL(clicked()),this,SLOT(toexit()));
    //! after
    //    ui->widget->index_widget(tr("上一行"))->setVisible(false);
    //    ui->widget->index_widget(tr("下一行"))->setVisible(false);

    ui->widget->index_widget(tr("继续"))->setVisible(false);
    ui->lineEdit_ch_tableno->setEnabled(false);
    ui->comboBox_vch_typename->setEnabled(false);
    ui->label_record->setText(QString("%1/%2").arg(_parent->getTableview()->currentIndex().row()+1).arg(_parent->getTableview()->verticalHeader()->count()));
    //这样做可以屏蔽pushbutton的enter时间,当有lineeidt需要输入条码是,会很顺畅
    static_cast<QPushButton*>(ui->widget->index_widget(tr("上一行")))->setDefault(false);static_cast<QPushButton*>(ui->widget->index_widget(tr("上一行")))->setAutoDefault(false);
    static_cast<QPushButton*>(ui->widget->index_widget(tr("下一行")))->setDefault(false);static_cast<QPushButton*>(ui->widget->index_widget(tr("下一行")))->setAutoDefault(false);
    static_cast<QPushButton*>(ui->widget->index_widget(tr("保存")))->setDefault(false);static_cast<QPushButton*>(ui->widget->index_widget(tr("保存")))->setAutoDefault(false);
    static_cast<QPushButton*>(ui->widget->index_widget(tr("继续")))->setDefault(false);static_cast<QPushButton*>(ui->widget->index_widget(tr("继续")))->setAutoDefault(false);
    static_cast<QPushButton*>(ui->widget->index_widget(tr("退出")))->setDefault(false);static_cast<QPushButton*>(ui->widget->index_widget(tr("退出")))->setAutoDefault(false);

    //! TIMESLOT
    QTimer::singleShot(0, this, SLOT(mapperindexupdate()));//信号槽的重写是需要这的写的，实现subclass 同时继承
}

bool w_bt_dish_table_change::tosave()
{
    if(_parent->tablemodel->submitAll()){
        lds_messagebox::warning(this, tr("消息提示"),tr("保存成功"));
    } else {
        lds_messagebox::warning(this, tr("消息提示"),/*tablemodel->lastError().text()*/tr("数据有误，操作失败"));
    }
    mapperindexupdate_select();
}

void w_bt_dish_table_change::toexit()
{
    mapperindexupdate_select();
    this->reject();
}

void w_bt_dish_table_change::toup()
{
    if(_parent->tablemodel->model_is_dirty()){
        if(0==lds_messagebox::warning(this, tr("消息提示"),tr("有数据未保存,确认保存?"), tr("确定"),tr("取消"))){
            if(!tosave()) return;
        } else {
            _parent->torefresh();
        }
    }
    _parent->ui->tableView->goprevious();
    mapper->setCurrentIndex(_parent->ui->tableView->currentIndex().row());
    ui->label_record->setText(QString("%1/%2").arg(_parent->ui->tableView->currentIndex().row()+1).arg(_parent->ui->tableView->verticalHeader()->count()));
}

void w_bt_dish_table_change::todown()
{
    if(_parent->tablemodel->model_is_dirty()){
        if(0==lds_messagebox::warning(this, tr("消息提示"),tr("有数据未保存,确认保存?"), tr("确定"),tr("取消"))){
            if(!tosave()) return;
        } else {
            _parent->torefresh();
        }
    }
    _parent->ui->tableView->gonext();
    mapper->setCurrentIndex(_parent->ui->tableView->currentIndex().row());
    ui->label_record->setText(QString("%1/%2").arg(_parent->ui->tableView->currentIndex().row()+1).arg(_parent->ui->tableView->verticalHeader()->count()));
}

void w_bt_dish_table_change::tocontinue()
{

}

void w_bt_dish_table_change::tomapperindexupdate()
{

    mapper->setCurrentIndex(_parent->ui->tableView->currentIndex().row());
}

void w_bt_dish_table_change::mapperindexupdate()
{
    if(_ischange)
        mapper->setCurrentIndex(_parent->ui->tableView->currentIndex().row());
    else
        mapper->setCurrentIndex(-1);
}

void w_bt_dish_table_change::mapperindexupdate_select()
{
    _parent->torefresh();
    lds_model_sqlrelationaltablemodel* tablemodel=_parent->getTablemodel();
    tablemodel->relationModel(2)->select();
    tablemodel->relationModel(9)->select();

    mapperindexupdate();
}

//===============================
w_bt_dish_table_new::w_bt_dish_table_new(QWidget *parent):
    w_bt_dish_table_change(parent, false)
{
    ui->widget->index_widget(tr("上一行"))->setVisible(false);
    ui->widget->index_widget(tr("下一行"))->setVisible(false);
    ui->widget->index_widget(tr("继续"))->setVisible(false);
    tocontinue();

    ui->label_record->setVisible(false);
    ui->comboBox_vch_typename->setEnabled(false);
    ui->lineEdit_ch_tableno->setEnabled(true);

}

bool w_bt_dish_table_new::tosave()
{
    //开头不能为0
    QString ch_tableno=ui->lineEdit_ch_tableno->text();
    if(ch_tableno.startsWith("0")){
        lds_messagebox::warning(this, tr("消息提示"),tr("编号开头不能为0!"));
        return false;
    }
    if(ui->lineEdit_ch_tableno->text().isEmpty()){
        lds_messagebox::warning(this, tr("消息提示"),tr("餐桌号不能为空!"));
        return false;
    }
    if(ui->lineEdit_vch_tablename->text().isEmpty()){
        lds_messagebox::warning(this, tr("消息提示"),tr("餐桌名不能为空!"));
        return false;
    }
    if(ui->comboBox_vch_typename->currentText().isEmpty()){
        lds_messagebox::warning(this, tr("消息提示"),tr("餐桌类型不能为空!"));
        return false;
    }
    if(ui->comboBox_vch_areaname->currentText().isEmpty()){
        lds_messagebox::warning(this, tr("消息提示"),tr("餐区不能为空!"));
        return false;
    }
    if(_parent->tablemodel->same_column_exist(_parent->tablemodel->fieldIndex("ch_tableno"))){
        lds_messagebox::warning(this, tr("消息提示"),tr("餐桌号有重复!"));
        return false;
    }
    if(_parent->tablemodel->same_column_exist(_parent->tablemodel->fieldIndex("vch_tablename"))){
        lds_messagebox::warning(this, tr("消息提示"),tr("餐桌名有重复!"));
        return false;
    }
    //手动插入表格
    QSqlQuery query;
    QString /*ch_tableno*/vch_tablename,ch_typeno,ch_areano,vch_memo,int_person;

    vch_tablename=ui->lineEdit_vch_tablename->text();
    ch_typeno=ui->comboBox_vch_typename->model()->index(ui->comboBox_vch_typename->currentIndex(), 0).data().toString();
    ch_areano=ui->comboBox_vch_areaname->model()->index(ui->comboBox_vch_areaname->currentIndex(), 0).data().toString();
    vch_memo=ui->lineEdit_vch_memo->text();
    int_person=ui->lineEdit_int_person->text();

    //! <--transaction begin-->
    QSqlDatabase::database().transaction();
    query.prepare(QString("insert into cey_bt_table (ch_tableno,vch_tablename,ch_typeno,ch_areano,vch_memo,int_person)values(?,?,?,?,?,?)"));
    query.addBindValue(ch_tableno);
    query.addBindValue(vch_tablename);
    query.addBindValue(ch_typeno);
    query.addBindValue(ch_areano);
    query.addBindValue(vch_memo);
    query.addBindValue(int_person);
    if(query.exec()){
        QSqlDatabase::database().commit();
        _parent->torefresh();
        lds_messagebox::warning(this, tr("消息提示"),tr("保存成功"));
        tocontinue();
        return true;
    } else {
        QSqlDatabase::database().rollback();
        _parent->torefresh();
        lds_messagebox::warning(this, tr("消息提示"),query.lastError().text());
    }
    //! <--transaction end-->

    return false;
}

void w_bt_dish_table_new::tocontinue()
{
    //更新mapper的com数据
    mapperindexupdate_select();

    QString tabletype=backheader::getinterdata(_parent->getTreew()->currentItem()->text(0));
    QString nextno=tabletype+"00";
    QSqlQuery query;
    query.exec(QString("select max(ch_tableno) from cey_bt_table where ch_typeno='%1' and left(ch_tableno, 2)='%1'")
               .arg(tabletype));
    if(query.next()){
        QString tmpno=query.value(0).toString();
        nextno=tabletype+QString().sprintf("%02d",tmpno.right(2).toInt()+1);
    }

    //初始化餐桌类型
    ui->lineEdit_ch_tableno->setText(nextno);
    ui->comboBox_vch_typename->setCurrentIndex(_parent->getTreew()->currentIndex().row());
}


//==============================
#include "ui_w_bt_dish_table_news_dialog.h"
w_bt_dish_table_news::w_bt_dish_table_news(QWidget *parent):
    QDialog(parent),
    ui(new Ui_w_bt_table_news_Dialog),
    _parent(static_cast<w_bt_dish_table *>(parent))
{
    ui->setupUi(this);
    //! toolbar
    ui->widget->setup(QStringList() << tr("生成") << tr("退出"), Qt::AlignRight);

    //! area type
    QSqlQueryModel querymodel;
    querymodel.setQuery("select ch_areano, vch_areaname from cey_bt_table_area");
    backheader::lds_com_setdatas(&querymodel, ui->comboBox_ch_areano);
    querymodel.setQuery("select ch_typeno, vch_typename from cey_bt_table_type");
    backheader::lds_com_setdatas(&querymodel, ui->comboBox_ch_typeno);
    ui->comboBox_ch_typeno->setCurrentIndex(_parent->getTreew()->get_0_Index());

    //! connect
    connect(ui->widget->index_widget(tr("生成")), SIGNAL(clicked()),this,SLOT(togenrate()));
    connect(ui->widget->index_widget(tr("退出")), SIGNAL(clicked()),this,SLOT(toexit()));

    ui->lineEdit_int_len->setEnabled(false);

}

void w_bt_dish_table_news::togenrate()
{
    //clicked (none) returns long [pbm_bnclicked]
    //long ancestorreturnvalue
    QString ls_sql;
    QString ls_code_start;
    QString ls_code_end;
    QString ls_ch_typeno;
    QString ls_ch_areano;
    QString ls_msg;
    QString ls_ch_tableno;
    QString ls_ch_tablename;
    QString ls_filter;
    QString ls_qz;
    QString ls_ch_temp;
    QString errtext;
    qlonglong ll_i;
    qlonglong ll_int_id;
    qlonglong ll_code_start;
    qlonglong ll_code_end;
    qlonglong ll_int_person;
    qlonglong ll_exist;
    int li_ret;
    int li_len;


    //    IF PARENT.dw_set.accepttext() = -1 THEN RETURN
    ls_qz =ui->lineEdit_code_qz->text();//= PARENT.dw_set.getitemstring(1,"code_qz")
    ll_code_start =ui->lineEdit_code_start->text().toLongLong();// PARENT.dw_set.getitemnumber(1,"code_start")
    ll_code_end =ui->lineEdit_code_end->text().toLongLong();// PARENT.dw_set.getitemnumber(1,"code_end")
    ls_ch_typeno =ui->comboBox_ch_typeno->comusrdata();
    ls_ch_areano =ui->comboBox_ch_areano->comusrdata();
    ll_int_person =ui->lineEdit_int_person->text().toLongLong();// PARENT.dw_set.getitemnumber(1,"int_person")
    ls_code_start = QString::number(ll_code_start);
    ls_code_end = QString::number(ll_code_end);
    li_len = ui->lineEdit_int_len->text().toInt();//PARENT.dw_set.getitemnumber(1,"int_len")
    ls_ch_temp = "N";
    ll_int_id = 0;
    ls_filter = ui->lineEdit_vch_filter->text().trimmed();//trim(PARENT.dw_set.object.vch_filter[1])
    if(ls_filter.isEmpty()){

    }
    qDebug() << ls_ch_typeno.isEmpty() << ls_code_start;
    if(ui->comboBox_ch_typeno->currentText().isEmpty()){
        ls_msg=tr("类型不能为空!");
    } else if(ui->comboBox_ch_areano->currentText().isEmpty()){
        ls_msg=tr("区域不能为空!");
    } else if(ui->lineEdit_code_start->text().isEmpty()){
        ls_msg=tr("起始编号不能为空!");
    } else if(ui->lineEdit_code_end->text().isEmpty()){
        ls_msg=tr("结束编号不能为空!");
    } else if(ls_code_start.length()>4||ls_code_end.length()>4){
        ls_msg=tr("编号位数不能为空且不能大于4位!");
    }else if(li_len==0){
        ls_msg=tr("编号长度不能为空!");
    } else if(ll_code_start > ll_code_end){
        ls_msg=tr("起始编号不能大于结束编号!");
    } else if(qAbs(ll_code_end - ll_code_start) > 1000){
        ls_msg=tr("一次批量创建不超过1千条!");
    } else if(ls_qz.length()>2){
        ls_msg=tr("前缀不能超过2位!");
    } else if((ls_qz.length() + ls_code_end.length())>4){
        ls_msg=tr("前缀位数加编号位数不能超过4位!");
    }

    if(ls_msg.isEmpty()){

    } else {
        lds_messagebox::warning(this, tr("消息提示"),ls_msg);
        return;
    }

    //! <--transaction begin-->
    ls_filter.replace(",", "");
    ls_filter="["+ls_filter+"]";
    QSqlDatabase::database().transaction();
    QSqlQuery query;
    for(long ll_i=ll_code_start; ll_i<=ll_code_end;ll_i++){//0
        ls_ch_tableno=ls_qz + QString::number(ll_i).rightJustified(li_len-ls_qz.length(), '0').right(li_len-ls_qz.length());
        ls_ch_tablename=tr("桌")+ls_ch_tableno;

        if(!ls_filter.isEmpty() && ls_ch_tableno.contains(QRegExp(ls_filter))){
            continue;
        }

        query.exec(QString("Select count(*)  from cey_bt_table   where ch_tableno ='%1'").arg(ls_ch_tableno));
        query.next();
        if(query.record().value(0).toLongLong() > 0){
            errtext=tr("批量范围中的编号已经存在，例如：")+ls_ch_tableno+tr("，本次操作无效！");
            break;
        }

        query.prepare("Insert into cey_bt_table (ch_tableno,vch_tablename,ch_typeno,ch_areano,int_person) Values(?,?,?,?,?)");
        query.addBindValue(ls_ch_tableno);
        query.addBindValue(ls_ch_tablename);
        query.addBindValue(ls_ch_typeno);
        query.addBindValue(ls_ch_areano);
        query.addBindValue(ll_int_person);
        if(query.exec()){
            query.prepare("update cey_bt_table set ch_lockflag ='N' , ch_state ='N' where ch_tableno =?");
            query.addBindValue(ls_ch_tableno);
            if(query.exec()){
                continue;
            }
        }
        errtext=query.lastError().text();
        break;
    }//0
    if(errtext.isEmpty()){
        QSqlDatabase::database().commit();
        _parent->torefresh();
    } else {
        QSqlDatabase::database().rollback();
        _parent->torefresh();
        lds_messagebox::warning(this, tr("消息提示"),/*tr("数据有误，操作失败!")*/errtext);
        qDebug() << errtext;
    }
    //! <--transaction end-->
}

void w_bt_dish_table_news::toexit()
{
    this->reject();
}

//=================================
#include"ui_w_bt_dish_table_tableset_dialog.h"
#include <QStandardItemModel>

w_bt_dish_table_tableset::w_bt_dish_table_tableset(QWidget *parent):
    QDialog(parent),
    ui(new Ui_w_bt_dish_table_tableset_Dialog),
    _parent(static_cast<w_bt_dish_table* >(parent))
{
    ui->setupUi(this);
    //! toolbar
    ui->widget->setup(QStringList()<<tr("上一行")<<tr("下一行")<<tr("新增")<<tr("删除")<<tr("保存")<<tr("退出"), Qt::AlignRight);
    //! tableview
    backheader::lds_com_setdatas(&w_bt_dish::MODEL_DISH_LOW_MODE, ui->comboBox_ch_low_mode);
    backheader::lds_com_setdatas(&w_bt_dish::MODEL_DISH_SERVICE_MODE, ui->comboBox_ch_service_mode);
    //几个写死的com 和radiobuton 的内容
    //! init
    tablemodel=new lds_model_sqlrelationaltablemodel(this);
    tablemodel->setTable("cey_bt_table_type");
    tablemodel->setEditStrategy(QSqlTableModel::OnManualSubmit);tablemodel->setJoinMode(QSqlRelationalTableModel::LeftJoin);
    tablemodel->select();

    mapper = new QDataWidgetMapper(this);
    mapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
    mapper->setModel(tablemodel);
    mapper->setItemDelegate(new QSqlRelationalDelegate(mapper));
    mapper->addMapping(ui->comboBox_ch_low_mode, tablemodel->fieldIndex("ch_low_mode"), "comusrdata");
    mapper->addMapping(ui->checkBox_ch_serviceflag, tablemodel->fieldIndex("ch_serviceflag"), "checked1");//将bool 转换为Y N
    mapper->addMapping(ui->comboBox_ch_service_mode, tablemodel->fieldIndex("ch_service_mode"), "comusrdata");
    mapper->addMapping(ui->lineEdit_ch_typeno, tablemodel->fieldIndex("ch_typeno"));
    mapper->addMapping(ui->lineEdit_int_begin_time, tablemodel->fieldIndex("int_begin_time"));
    mapper->addMapping(ui->lineEdit_int_over, tablemodel->fieldIndex("int_over"));
    mapper->addMapping(ui->lineEdit_int_rate, tablemodel->fieldIndex("int_rate"));
    mapper->addMapping(ui->lineEdit_int_start_minute, tablemodel->fieldIndex("int_start_minute"));
    mapper->addMapping(ui->lineEdit_int_unit_minute, tablemodel->fieldIndex("int_unit_minute"));
    mapper->addMapping(ui->lineEdit_num_lowcost, tablemodel->fieldIndex("num_lowcost"));
    mapper->addMapping(ui->lineEdit_num_max_service, tablemodel->fieldIndex("num_max_service"));
    mapper->addMapping(ui->lineEdit_num_ration, tablemodel->fieldIndex("num_ration"));
    mapper->addMapping(ui->lineEdit_num_start_amount, tablemodel->fieldIndex("num_start_amount"));
    mapper->addMapping(ui->lineEdit_num_unit_amount, tablemodel->fieldIndex("num_unit_amount"));
    mapper->addMapping(ui->lineEdit_vch_typename, tablemodel->fieldIndex("vch_typename"));

    //! connect
    connect(ui->widget->index_widget(tr("上一行")),SIGNAL(clicked()),this,SLOT(toup()));
    connect(ui->widget->index_widget(tr("下一行")),SIGNAL(clicked()),this,SLOT(todown()));
    connect(ui->widget->index_widget(tr("新增")),SIGNAL(clicked()),this,SLOT(tonew()));
    connect(ui->widget->index_widget(tr("删除")),SIGNAL(clicked()),this,SLOT(todel()));
    connect(ui->widget->index_widget(tr("保存")),SIGNAL(clicked()),this,SLOT(tosave()));
    connect(ui->widget->index_widget(tr("退出")),SIGNAL(clicked()),this,SLOT(toexit()));
    connect(ui->checkBox_ch_serviceflag,SIGNAL(toggled(bool)),this,SLOT(toupdate_ch_serviceflagtext(bool)));
    connect(ui->comboBox_ch_service_mode,SIGNAL(currentIndexChanged(int)),this,SLOT(update_enabled(int)));
    //! after
    mapper->setCurrentIndex(qMax(0, _parent->ui->treeWidget->get_0_Index()));
    if(mapper->currentIndex()<0){tonew();mapper->setCurrentIndex(0);}

    mapperindexupdate();
    ui->label_record->setText(QString("%1/%2").arg(mapper->currentIndex()+1).arg(tablemodel->rowCount()));

    //    QTableView *tableview=new QTableView;
    //    tableview->setModel(tablemodel);
    //    tableview->show();
}

void w_bt_dish_table_tableset::toup()
{
    mapper->toPrevious();
    mapperindexupdate();
}

void w_bt_dish_table_tableset::todown()
{
    mapper->toNext();
    mapperindexupdate();
}

void w_bt_dish_table_tableset::tonew()
{
    //若是未保存的 则不继续
    int rowcount=tablemodel->rowCount();
    if(rowcount>0
            &&tablemodel->isDirty(tablemodel->index(rowcount-1, 0)))return;

    QString typeno="00";

    tablemodel->insertRow(rowcount);
    QSqlQuery query;
    query.exec(QString("select max(ch_typeno) from cey_bt_table_type"));
    if(query.next()){
        typeno=QString().sprintf("%02d",(query.record().value(0).toInt()+1));
    }
    tablemodel->setData(tablemodel->index(rowcount,0),typeno);

    mapper->setCurrentIndex(rowcount);
    mapperindexupdate();
}

void w_bt_dish_table_tableset::todel()
{
    int row=mapper->currentIndex();
    if(row<0){
        //请先选择删除记录
        lds_messagebox::warning(this, tr("消息提示"),tr("请先选择删除记录!"));
        return;
    }
    QString ls_typeno=tablemodel->record(row).value(0).toString();
    QSqlQuery query;
    query.exec(QString("select count(*) from cey_bt_table where ch_typeno='%1'").arg(ls_typeno));
    query.next();
    if(query.record().value(0).toLongLong()>0){
        //指定单位已存在菜品记录，不能删除!
        lds_messagebox::warning(this, tr("消息提示"),tr("当前桌类存在餐桌记录，不能删除!"));
        return;
    }
    if(0==lds_messagebox::warning(this, tr("消息提示"),tr("确认删除指定餐桌记录"),tr("确认"),tr("取消"))){
        tablemodel->removeRow(mapper->currentIndex());
        if(tablemodel->submitAll()){
            lds_messagebox::warning(this, tr("消息提示"),tr("删除成功!"));
            static_cast<w_bt_dish_table *>(this->parent())->torefresh();
            tablemodel->select();
            todown();
        } else {
            lds_messagebox::warning(this, tr("消息提示"),tr("数据有误，操作失败,删除失败!"));
        }
    }
}

bool w_bt_dish_table_tableset::tosave()
{
    mapper->submit();
    int premapperindex=mapper->currentIndex();
    for(int row=0,rowcount=tablemodel->rowCount();row<rowcount;row++){
        if(tablemodel->data(tablemodel->index(row, tablemodel->fieldIndex("ch_typeno"))).toString().isEmpty()){
            lds_messagebox::warning(this, tr("消息提示"),tr("%1行 编码不能为空!").arg(row+1));
            return false;
        }
        if(tablemodel->data(tablemodel->index(row, tablemodel->fieldIndex("vch_typename"))).toString().isEmpty()){
            lds_messagebox::warning(this, tr("消息提示"),tr("%1行 桌类名称不能为空!").arg(row+1));
            return false;
        }

        if(tablemodel->data(tablemodel->index(row, tablemodel->fieldIndex("ch_service_mode"))).toString().isEmpty()){
            lds_messagebox::warning(this, tr("消息提示"),tr("%1行 服务费模式不能为空!").arg(row+1));
            return false;
        }

        if(tablemodel->data(tablemodel->index(row, tablemodel->fieldIndex("ch_low_mode"))).toString().isEmpty()){
            lds_messagebox::warning(this, tr("消息提示"),tr("%1行 低消方式不能为空!").arg(row+1));
            return false;
        }

        //============
        if(ui->frame_2->isEnabled()
                &&tablemodel->data(tablemodel->index(row, tablemodel->fieldIndex("int_unit_minute"))).toInt()<=0){
            lds_messagebox::warning(this, tr("消息提示"),tr("%1行 超钟收费单位不能小于等于0!").arg(row+1));
            return false;
        }
        if(ui->frame_4->isEnabled()
                &&tablemodel->data(tablemodel->index(row, tablemodel->fieldIndex("num_max_service"))).toInt()<
                tablemodel->data(tablemodel->index(row, tablemodel->fieldIndex("num_ration"))).toInt()
                ){
            lds_messagebox::warning(this, tr("消息提示"),tr("%1行 定额服务费不能高于最高服务费!").arg(row+1));
            return false;
        }

        //===============
    }
    if(tablemodel->submitAll()){
        lds_messagebox::warning(this, tr("消息提示"),/*tablemodel->lastError().text()*/tr("保存成功"));
        _parent->torefresh();
        tablemodel->select();
        mapper->setCurrentIndex(premapperindex);
        todown();
        return true;
    } else {
        lds_messagebox::warning(this, tr("消息提示"),/*tablemodel->lastError().text()*/tr("数据有误，操作失败"));
        return false;
    }
}

void w_bt_dish_table_tableset::toexit()
{
    this->reject();
}

void w_bt_dish_table_tableset::toupdate_ch_serviceflagtext(bool f)
{
    ui->checkBox_ch_serviceflag->setText(f?tr("折前服务率"):tr("折后服务率"));
}

void w_bt_dish_table_tableset::update_enabled(int index)
{
    switch(index){
    case 0:
        ui->frame_2->setEnabled(false);
        ui->frame_3->setEnabled(false);
        ui->frame_4->setEnabled(false);
        break;
    case 1:
        ui->frame_2->setEnabled(true);
        ui->frame_3->setEnabled(false);
        ui->frame_4->setEnabled(false);
        break;
    case 2:
    case 4:
        ui->frame_2->setEnabled(false);
        ui->frame_3->setEnabled(true);
        ui->frame_4->setEnabled(false);
        break;
    case 3:
        ui->frame_2->setEnabled(false);
        ui->frame_3->setEnabled(false);
        ui->frame_4->setEnabled(true);
        break;
    }
}

void w_bt_dish_table_tableset::mapperrefresh()
{
    int curindex=mapper->currentIndex();

    tablemodel->select();

    mapper->setCurrentIndex(qMax(0,curindex));
}

void w_bt_dish_table_tableset::isnewq(int r)
{
    if(tablemodel->model_is_dirty()){
        this->setStyleSheet("QLabel{color:red;}");
        ui->lineEdit_vch_typename->setEnabled(true);
        ui->lineEdit_ch_typeno->setEnabled(true);
    } else {
        this->setStyleSheet("");
        ui->lineEdit_vch_typename->setEnabled(false);
        ui->lineEdit_ch_typeno->setEnabled(false);
    }
    //static_cast<w_bt_dish_table *>(this->parent())->getTreew()->setcurdeep(QVector<int>()<<0<<r);

}

void w_bt_dish_table_tableset::mapperindexupdate()
{
    _parent->getTreew()->set_0_Index(mapper->currentIndex());
    ui->label_record->setText(QString("%1/%2").arg(mapper->currentIndex()+1).arg(tablemodel->rowCount()));

    if(tablemodel->isDirty(tablemodel->index(mapper->currentIndex(), 0))){
        this->setStyleSheet("QLabel{color:red;}");
    } else {
        this->setStyleSheet("");
    }
}
//===========================
#include "ui_w_bt_dish_unitset_dialog.h"
#include "lds_model_sqlrelationaltablemodel.h"

w_bt_dish_table_areaset::w_bt_dish_table_areaset(w_bt_dish_table *parent):
    QDialog(parent),
    ui(new Ui_w_bt_dish_unitset_Dialog),
    _parent(parent)
{
    ui->setupUi(this);
    tablemodel=new lds_model_sqlrelationaltablemodel(this);
    tablemodel->setTable("cey_bt_table_area");
    tablemodel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    tablemodel->save_set_header("ch_areano",tr("编码"),true);
    tablemodel->save_set_header("vch_areaname",tr("区域名称"), true);

    mapper = new QDataWidgetMapper(this);
    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    mapper->setModel(tablemodel);
    mapper->setItemDelegate(new QSqlRelationalDelegate(mapper));
    ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::SelectedClicked);ui->tableView->setModel(tablemodel->fpmodel);


    //! toolbar
    ui->widget->setup(QStringList()<<tr("新增")<<tr("删除")<<tr("刷新")<<tr("保存")<<tr("退出"), Qt::AlignRight);
    connect(ui->widget->index_widget(tr("新增")),SIGNAL(clicked()),this,SLOT(tonew()));
    connect(ui->widget->index_widget(tr("删除")),SIGNAL(clicked()),this,SLOT(todel()));
    connect(ui->widget->index_widget(tr("刷新")),SIGNAL(clicked()),this,SLOT(torefresh()));
    connect(ui->widget->index_widget(tr("保存")),SIGNAL(clicked()),this,SLOT(tosave()));
    connect(ui->widget->index_widget(tr("退出")),SIGNAL(clicked()),this,SLOT(toexit()));

    QTimer::singleShot(100, this, SLOT(torefresh()));

}

void w_bt_dish_table_areaset::tonew()
{
    int rowcount=tablemodel->rowCount();
    tablemodel->insertRow(rowcount);

}

void w_bt_dish_table_areaset::todel()
{
    int row=ui->tableView->currentIndex().row();
    if(row<0){
        //请先选择删除记录
        lds_messagebox::warning(this, tr("消息提示"),tr("请先选择删除记录!"));
        return;
    }
    QString ls_typeno=tablemodel->record(row).value(0).toString();
    QSqlQuery query;
    query.exec(QString("select count(*) from cey_bt_table where ch_areano='%1'").arg(ls_typeno));
    query.next();
    if(query.record().value(0).toLongLong()>0){
        //指定单位已存在菜品记录，不能删除!
        lds_messagebox::warning(this, tr("消息提示"),tr("指定区域已存在餐桌记录，不能删除!"));
        return;
    }
    tablemodel->removeRow(row);
}

void w_bt_dish_table_areaset::torefresh()
{
    tablemodel->select();
}

bool w_bt_dish_table_areaset::tosave()
{
    QString ls_areano;
    QString ls_areaname;
    for(int r=0,r_count=tablemodel->rowCount();r<r_count;r++){
        ls_areano=tablemodel->record(r).value(0).toString();
        ls_areaname=tablemodel->record(r).value(1).toString();
        if(ls_areano.trimmed().isEmpty()){
            lds_messagebox::warning(this, tr("消息提示"),tr("第%1行，区域代码为空，不能保存!").arg(r+1));
            return false;
        }
        if(ls_areaname.trimmed().isEmpty()){
            lds_messagebox::warning(this, tr("消息提示"),tr("第%1行，区域名称为空，不能保存!").arg(r+1));
            return false;
        }

        if(findareano(ls_areano, r)!=-1){
            lds_messagebox::warning(this, tr("消息提示"),tr("区域代码有重复，请核实!"));
            return false;
        }
    }

    if(tablemodel->submitAll()){
        lds_messagebox::warning(this, tr("消息提示"),tr("数据保存成功"));
        return true;
    } else {
        lds_messagebox::warning(this, tr("消息提示"),/*tablemodel->lastError().text()*/tr("数据有误，操作失败"));
        return false;
    }
}

void w_bt_dish_table_areaset::toexit()
{
    if(tablemodel->model_is_dirty()){
        if(0==lds_messagebox::warning(this, tr("消息提示"),tr("有数据未保存,确认保存?")+tablemodel->dirty_err_string(), tr("确认"),tr("取消"))){
            if(!tosave()){
                return;
            }
        }
    }
    //    if(tablemodel->model_is_dirty()
    //            &&!tosave()) return;

    _parent->torefresh();
    this->reject();
}

int w_bt_dish_table_areaset::findareano(const QString &areano, int exceptrow)
{
    for(int r=0,r_count=tablemodel->rowCount();r<r_count;r++){
        if(r!=exceptrow
                &&tablemodel->record(r).value(0).toString() == areano)
            return r;
    }
    return -1;
}


