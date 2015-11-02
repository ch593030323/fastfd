#include "w_m_member_point.h"
//会员积分管理
#include "ui_w_m_member_dialog.h"
#include <QSqlQueryModel>
#include <QSqlError>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlRecord>
#include "lds_model_sqlrelationaltablemodel.h"

#include "getchinesespell.h"
#include "lds_tableview_delegate_check.h"
#include <QSqlRelation>
#include "lds_tableview_repecify.h"
#include "backheader.h"
#include <QTimer>
#include "w_m_member.h"
#include "lds_messagebox.h"
#include "ui_w_m_member_point_recharge_dialog.h"



//【0】重写selectStatement()实现 另加额外列的方法
//【1】排序不计算额外列， 特殊颜色区分
//【2】每个类独立写，不统一父类
//【3】排序允许函数必须设置开始，否则将丢失额外列
//【4】表格需要show之后，才可以重写排写表头，否则无效
//【5】    QTimer::singleShot(0, this,SLOT(to_headerview_repecify())); 调用该函数，来显示额外列
w_m_member_point::RelationalTableModel::RelationalTableModel(QObject *parent, QSqlDatabase db)
    : lds_model_sqlrelationaltablemodel(parent, db)
{
    backheader::regetvch_operid_name();
    sql_headerlabels.append(SQL_HEADERLABEL("ifnull((select num_point from t_m_curamount where t_m_curamount.vch_memberno=t_m_member.vch_memberno), 0)" ,tr("剩余积分")));
}

QVariant w_m_member_point::RelationalTableModel::data(const QModelIndex &item, int role) const
{
    QVariant _d=lds_model_sqlrelationaltablemodel::data(item, role);
    if(role==Qt::DisplayRole){
        if(_d.type()==QVariant::DateTime){
            return _d.toDateTime().toString("yyyy-MM-dd");
        }
        if(this->headerData(item.column(), Qt::Horizontal,  Qt::DisplayRole).toString().endsWith(tr("状态"))){
            return backheader::VIPFINDVALUE(w_m_member::VIP_MODEL_STATE, _d.toString());
        }
        if(this->headerData(item.column(), Qt::Horizontal,  Qt::DisplayRole).toString().endsWith(tr("创建人员"))){
            return backheader::getouterdata(backheader::vch_operid_name, _d.toString());
        }
    }
    return _d;
}

Qt::ItemFlags w_m_member_point::RelationalTableModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsEnabled|Qt::ItemIsSelectable;
}


w_m_member_point::w_m_member_point(QWidget *parent) :
    QDialog(parent),ui(new Ui_w_m_member_Dialog)
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
    tablemodel->setJoinMode(QSqlRelationalTableModel::LeftJoin);
    tablemodel->setEditStrategy(QSqlTableModel::OnManualSubmit);tablemodel->setJoinMode(QSqlRelationalTableModel::LeftJoin);
    tablemodel->setTable("t_m_member");
    tablemodel->setRelation(2/*ch_typeno*/, QSqlRelation("t_m_member_type", "ch_typeno","vch_typename"));
    ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::SelectedClicked);ui->tableView->setModel(tablemodel->fpmodel);

    //! refresh
    torefresh();

    //! toolbar
    ui->widget->setup(QStringList()
                      << tr("详情") << tr("礼品资料")
                      << tr("充积分") << tr("减积分") << tr("换礼品")
                      << tr("刷新") << tr("退出"));    connect(ui->treeWidget,SIGNAL(pressed(QModelIndex)),this,SLOT(treeselect()));
    connect(ui->lineEdit,SIGNAL(textChanged(QString)),this,SLOT(treeselect()));

    connect(ui->widget->index_widget(tr("详情")), SIGNAL(clicked()),this,SLOT(toliping_detail()));
    connect(ui->widget->index_widget(tr("礼品资料")), SIGNAL(clicked()),this,SLOT(toliping_info()));
    connect(ui->widget->index_widget(tr("充积分")), SIGNAL(clicked()),this,SLOT(toadd_point()));
    connect(ui->widget->index_widget(tr("减积分")), SIGNAL(clicked()),this,SLOT(tosub_point()));
    connect(ui->widget->index_widget(tr("换礼品")), SIGNAL(clicked()),this,SLOT(tochange_liping()));
    connect(ui->widget->index_widget(tr("刷新")), SIGNAL(clicked()),this,SLOT(torefresh()));
    connect(ui->widget->index_widget(tr("退出")), SIGNAL(clicked()),this,SLOT(toexit()));

    //! after
    ui->label_2->setVisible(false);
    ui->lineEdit_2->setVisible(false);
    //! after
    QTimer::singleShot(0, this,SLOT(to_headerview_repecify()));
}

void w_m_member_point::torefresh()
{
    //treewidget
    ui->treeWidget->deleteAllItem();
    QSqlQuery query;
    query.exec(
                "select nametype from("
                "select concat(vch_typename,'[',ch_typeno,']') as 'nametype',ch_typeno  from t_m_member_type "
                " )t  group by nametype order by ch_typeno asc"
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
}

void w_m_member_point::treeselect()
{
    QString sqlselect;
    //tree检索
    if(ui->treeWidget->currentItem()){
        int index=ui->treeWidget->deepofitem(ui->treeWidget->currentItem());
        if(index==1){
            sqlselect=backheader::getinterdata(ui->treeWidget->currentItem()->text(0));
            sqlselect=QString("t_m_member.ch_typeno='%1'").arg(sqlselect);
        }
    }

    if(sqlselect.isEmpty()){

    } else {
        sqlselect +=" and ";
    }
    //过滤
    sqlselect +=QString(" (t_m_member.vch_memberno like '%1%' or t_m_member.vch_member like '%2%') ")
            .arg(ui->lineEdit->text())
            .arg(ui->lineEdit->text());
    //检索
    tablemodel->setFilter(sqlselect);
    tablemodel->select();
    tablemodel->relationModel(2)->select();
    ui->tableView->restore();

}

void w_m_member_point::toliping_detail()
{
    w_m_member_point_detail dialog(this);
    backheader::static_dialogexec(dialog,qobject_cast<QAbstractButton* >(sender())->text());

}

void w_m_member_point::toliping_info()
{
    w_m_member_point_liping_info dialog(this);
    backheader::static_dialogexec(dialog,qobject_cast<QAbstractButton* >(sender())->text());

}

void w_m_member_point::toadd_point()
{
    w_m_member_point_recharge dialog("5",this);
    if(dialog._ini_error.isEmpty()){
        backheader::static_dialogexec(dialog,qobject_cast<QAbstractButton* >(sender())->text());

    } else {
        lds_messagebox::warning(this, tr("消息提示"),dialog._ini_error);
    }
}

void w_m_member_point::tosub_point()
{   
    w_m_member_point_recharge dialog("6",this);
    if(dialog._ini_error.isEmpty()){
        backheader::static_dialogexec(dialog,qobject_cast<QAbstractButton* >(sender())->text());

    } else {
        lds_messagebox::warning(this, tr("消息提示"),dialog._ini_error);
    }
}

void w_m_member_point::tochange_liping()
{
    w_m_member_point_recharge dialog("4",this);
    QSqlQueryModel *querymodel=new QSqlQueryModel;
    querymodel->setQuery(QString("select vch_giftname as '%1', ch_giftno as '%2' , num_point as '%3' from t_m_gift ")
                         .arg(tr("礼品名称"))
                         .arg(tr("礼品编码"))
                         .arg(tr("积分值")));
    qDebug() << querymodel->query().lastQuery();
    dialog.ui->comboBox_vch_memo->setMaxVisibleItems(5);
    dialog.ui->comboBox_vch_memo->setView(new QTableView);
    dialog.ui->comboBox_vch_memo->setModel(querymodel);
    dialog.ui->comboBox_vch_memo->setEditable(false);
    dialog.ui->comboBox_vch_memo->setCurrentIndex(-1);
    connect(dialog.ui->comboBox_vch_memo,SIGNAL(currentIndexChanged2(QString)),dialog.ui->lineEdit_num_point,SLOT(setText(QString)));

    if(dialog._ini_error.isEmpty()){
        backheader::static_dialogexec(dialog,qobject_cast<QAbstractButton* >(sender())->text());

    } else {
        lds_messagebox::warning(this, tr("消息提示"),dialog._ini_error);
    }
}

void w_m_member_point::toexit()
{
    this->reject();
}

void w_m_member_point::to_headerview_repecify()
{

    QStringList headerstmp;
    tablemodel->setHeaderData(0,Qt::Horizontal,lds_tableview_repecify::saveHeader(tr("会员编号"),headerstmp));
    tablemodel->setHeaderData(1,Qt::Horizontal,lds_tableview_repecify::saveHeader(tr("会员名称"),headerstmp));
    tablemodel->setHeaderData(2,Qt::Horizontal,lds_tableview_repecify::saveHeader(tr("会员类型"),headerstmp));
    headerstmp.append(tr("剩余积分"));
    tablemodel->setHeaderData(4,Qt::Horizontal,lds_tableview_repecify::saveHeader(tr("生日"),headerstmp));
    tablemodel->setHeaderData(5,Qt::Horizontal,lds_tableview_repecify::saveHeader(tr("手机"),headerstmp));
    tablemodel->setHeaderData(6,Qt::Horizontal,lds_tableview_repecify::saveHeader(tr("电话"),headerstmp));
    tablemodel->setHeaderData(9,Qt::Horizontal,lds_tableview_repecify::saveHeader(tr("状态"),headerstmp));
    tablemodel->setHeaderData(11,Qt::Horizontal,lds_tableview_repecify::saveHeader(tr("持卡"),headerstmp));
    tablemodel->setHeaderData(8,Qt::Horizontal,lds_tableview_repecify::saveHeader(tr("有效期限"),headerstmp));
    tablemodel->setHeaderData(14,Qt::Horizontal,lds_tableview_repecify::saveHeader(tr("创建人员"),headerstmp));

    QList<int> virtualheades;
    virtualheades=  ui->tableView->rePecifyHeader( headerstmp);
    ui->tableView->setItemDelegate(new lds_tableview_delegate_check(QVector<int>()
                                                                    << virtualheades[8]
                                   ,
                                   ui->tableView));

}

//============================
#include "ui_w_bt_dish_unitset_dialog.h"
//【0】重写selectStatement()实现 另加额外列的方法
//【1】排序不计算额外列， 特殊颜色区分
//【2】每个类独立写，不统一父类
//【3】排序允许函数必须设置开始，否则将丢失额外列
//【4】表格需要show之后，才可以重写排写表头，否则无效
//【5】    QTimer::singleShot(0, this,SLOT(to_headerview_repecify())); 调用该函数，来显示额外列
w_m_member_point_detail::RelationalTableModel::RelationalTableModel(QObject *parent, QSqlDatabase db):
    lds_model_sqlrelationaltablemodel(parent, db)
{
    backheader::regetvch_operid_name();
}

QVariant w_m_member_point_detail::RelationalTableModel::data(const QModelIndex &item, int role) const
{
    QVariant _d=lds_model_sqlrelationaltablemodel::data(item, role);

    if(role==Qt::DisplayRole){
        if(this->headerData(item.column(), Qt::Horizontal,  Qt::DisplayRole).toString().endsWith(tr("积分类型"))){
            return backheader::VIPFINDVALUE(w_m_member::VIP_MODEL_POINT_TYPE, _d.toString());
        }
        if(this->headerData(item.column(), Qt::Horizontal,  Qt::DisplayRole).toString().endsWith(tr("操作员"))){
            return backheader::getouterdata(backheader::vch_operid_name, _d.toString());
        }
        if(this->headerData(item.column(), Qt::Horizontal,  Qt::DisplayRole).toString().endsWith(tr("操作时间"))){
            return _d.toDateTime().toString("yyyy-MM-dd");
        }
    }
    return lds_model_sqlrelationaltablemodel::data(item, role);
}

QVariant w_m_member_point_detail::RelationalTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return QSqlRelationalTableModel::headerData(section, orientation, role);
}

Qt::ItemFlags w_m_member_point_detail::RelationalTableModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsEnabled|Qt::ItemIsSelectable;
}

QString w_m_member_point_detail::RelationalTableModel::selectStatement() const
{
    return QSqlRelationalTableModel::selectStatement();
}

bool w_m_member_point_detail::RelationalTableModel::updateRowInTable(int row, const QSqlRecord &values)
{
    return QSqlRelationalTableModel::updateRowInTable(row, values);
}

bool w_m_member_point_detail::RelationalTableModel::insertRowIntoTable(const QSqlRecord &values)
{
    return QSqlRelationalTableModel::insertRowIntoTable(values);
}

QString w_m_member_point_detail::RelationalTableModel::orderByClause() const
{
    return QSqlRelationalTableModel::orderByClause();
}

w_m_member_point_detail::w_m_member_point_detail(w_m_member_point *parent)
    :QDialog(parent),
      ui(new Ui_w_bt_dish_unitset_Dialog),
      _parent(parent)
{
    ui->setupUi(this);

    //! tableview
    tablemodel=new RelationalTableModel(this);
    tablemodel->setJoinMode(QSqlRelationalTableModel::LeftJoin);
    tablemodel->setEditStrategy(QSqlTableModel::OnManualSubmit);tablemodel->setJoinMode(QSqlRelationalTableModel::LeftJoin);
    tablemodel->setTable("t_m_point");
    ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::SelectedClicked);ui->tableView->setModel(tablemodel->fpmodel);

    QStringList headerstmp;
    tablemodel->setHeaderData(tablemodel->fieldIndex("ch_type"), Qt::Horizontal, lds_tableview_repecify::saveHeader(tr("积分类型"),headerstmp));
    tablemodel->setHeaderData(tablemodel->fieldIndex("ch_payno"), Qt::Horizontal, lds_tableview_repecify::saveHeader(tr("凭证编号"),headerstmp));
    tablemodel->setHeaderData(tablemodel->fieldIndex("num_point"), Qt::Horizontal, lds_tableview_repecify::saveHeader(tr("积分值"),headerstmp));
    tablemodel->setHeaderData(tablemodel->fieldIndex("vch_memo"), Qt::Horizontal, lds_tableview_repecify::saveHeader(tr("备注"),headerstmp));
    tablemodel->setHeaderData(tablemodel->fieldIndex("vch_operid"), Qt::Horizontal, lds_tableview_repecify::saveHeader(tr("操作员"),headerstmp));
    tablemodel->setHeaderData(tablemodel->fieldIndex("dt_operdate"), Qt::Horizontal, lds_tableview_repecify::saveHeader(tr("操作时间"),headerstmp));
    QList<int> virtualheades=  ui->tableView->rePecifyHeader( headerstmp);

    if(_parent->ui->tableView->currentIndex().row()>=0){
        tablemodel->setFilter(QString("vch_memberno='%1' ")
                              .arg(_parent->tablemodel->record(_parent->ui->tableView->currentIndex().row()).value("vch_memberno").toString())
                              );
    }
    tablemodel->select();

    //! toolbar
    ui->widget->setup(QStringList()
                      << tr("打印") << tr("退出"), Qt::AlignRight);

    connect(ui->widget->index_widget(tr("打印")), SIGNAL(clicked()),this,SLOT(setPrintCount()));
    connect(ui->widget->index_widget(tr("退出")), SIGNAL(clicked()),this,SLOT(toexit()));
    //! after
    ui->widget->index_widget(tr("打印"))->setVisible(false);

}

void w_m_member_point_detail::setPrintCount()
{

}

void w_m_member_point_detail::toexit()
{
    this->reject();
}

//=======================================
#include "lds_messagebox.h"
w_m_member_point_liping_info::w_m_member_point_liping_info(w_m_member_point *parent):
    QDialog(parent),
    ui(new Ui_w_bt_dish_unitset_Dialog)
{
    ui->setupUi(this);
    //! tableview
    tablemodel=new lds_model_sqlrelationaltablemodel(this);
    tablemodel->setTable("t_m_gift");
    tablemodel->setEditStrategy(QSqlTableModel::OnManualSubmit);tablemodel->setJoinMode(QSqlRelationalTableModel::LeftJoin);

    ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::SelectedClicked);ui->tableView->setModel(tablemodel->fpmodel);

    tablemodel->save_set_header("ch_giftno", tr("礼品编码"), true);
    tablemodel->save_set_header("vch_giftname", tr("礼品名称"), true);
    tablemodel->save_set_header("num_point", tr("积分值"), true);
    ui->tableView->rePecifyHeader(tablemodel->save_set_header_saver);

    //! toolbar
    ui->widget->setup(QStringList() << tr("新增") << tr("删除") <<  tr("刷新") << tr("保存") << tr("退出"), Qt::AlignRight);
    connect(ui->widget->index_widget(tr("新增")), SIGNAL(clicked()),this,SLOT(tonew()));
    connect(ui->widget->index_widget(tr("删除")), SIGNAL(clicked()),this,SLOT(todel()));
    connect(ui->widget->index_widget(tr("刷新")), SIGNAL(clicked()),this,SLOT(torefresh()));
    connect(ui->widget->index_widget(tr("保存")), SIGNAL(clicked()),this,SLOT(tosave()));
    connect(ui->widget->index_widget(tr("退出")), SIGNAL(clicked()),this,SLOT(toexit()));


    QTimer::singleShot(100, this, SLOT(torefresh()));

}

void w_m_member_point_liping_info::tonew()
{
    int rowcount=tablemodel->rowCount();
    tablemodel->insertRow(rowcount);
    tablemodel->setData(tablemodel->index(rowcount, tablemodel->fieldIndex("ch_giftno")), tablemodel->max_column_add_1("ch_giftno"));
    tablemodel->setData(tablemodel->index(rowcount, tablemodel->fieldIndex("num_point")), "0");

}

void w_m_member_point_liping_info::todel()
{
    tablemodel->removeRow(ui->tableView->currentIndex().row());
}

void w_m_member_point_liping_info::torefresh()
{
    tablemodel->select();
}

bool w_m_member_point_liping_info::tosave()
{
    for(int r=0,r_count=tablemodel->rowCount();r<r_count;r++){
        if(tablemodel->record(r).value("ch_giftno").toString().isEmpty()){
            lds_messagebox::warning(this, tr("消息提示"),tr("礼品编码不能为空!"));
            return false;
        }
        if(tablemodel->same_column_exist("ch_giftno", r)){
            lds_messagebox::warning(this, tr("消息提示"),tr("礼品编码有重复!"));
            return false;
        }
        if(tablemodel->record(r).value("num_point").toString().isEmpty()){
            lds_messagebox::warning(this, tr("消息提示"),tr("积分值不能为空!"));
            return false;
        }
        if(tablemodel->record(r).value("vch_giftname").toString().isEmpty()){
            lds_messagebox::warning(this, tr("消息提示"),tr("礼品名称不能为空!"));
            return false;
        }
    }

    if(tablemodel->submitAll()){
        lds_messagebox::warning(this, tr("消息提示"),tr("保存成功"));
        return true;
    } else {
        lds_messagebox::warning(this, tr("消息提示"),tr("数据有误，操作失败\n")+ tablemodel->lastError().text());
        return false;
    }
}

void w_m_member_point_liping_info::toexit()
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

//=============================
#include "n_func.h"

w_m_member_point_recharge::w_m_member_point_recharge(const QString &type, w_m_member_point *parent):
    QDialog(parent),
    ui(new Ui_w_m_member_point_recharge_Dialog),
    _parent(parent),
    _type(type)
{
    ui->setupUi(this);

    uo_rw_member  uo_member;
    uo_rw_member::read_card_type  lst_card;
    QString ls_memberno;
    uo_member.of_init("4");

    if(_parent->ui->tableView->currentIndex().row()>=0){//-1
        if(_parent->tablemodel->record(_parent->ui->tableView->currentIndex().row()).value("ch_state").toString() =="1"){//0

            ls_memberno=_parent->tablemodel->record(_parent->ui->tableView->currentIndex().row()).value("vch_memberno").toString();
            lst_card=uo_member.of_read_card(ls_memberno);
            if(lst_card.flag){
                ui->lineEdit_vch_memberno->setText(ls_memberno);
                ui->lineEdit_ch_type->setText(backheader::VIPFINDVALUE(w_m_member::VIP_MODEL_POINT_TYPE, _type));//5指的是手工充积分
                ui->lineEdit_num_point_org->setText(QString::number(lst_card.point));
                ui->lineEdit_vch_operid->setText(n_func::gs_operid);
                ui->lineEdit_dt_operdate->setText(n_func::f_get_sysdatetime().toString("yyyy-MM-dd hh:mm:ss"));
            }
        } else {//0
            _ini_error = tr("该会员状态不正常，不允许手动充积分!");
        }//0
    } else {//-1
        _ini_error = tr("没有数据被选中!");
    }

    //! toolbar
    ui->widget->setup(QStringList() << tr("确定") << tr("取消"), Qt::AlignRight);
    connect(ui->widget->index_widget(tr("确定")), SIGNAL(clicked()),this,SLOT(took()));
    connect(ui->widget->index_widget(tr("取消")), SIGNAL(clicked()),this,SLOT(toexit()));
    if(_type=="4"){

    }
    //! after
    ui->lineEdit_vch_memberno->setEnabled(false);
    ui->lineEdit_ch_type->setEnabled(false);
    ui->lineEdit_num_point_org->setEnabled(false);
    ui->lineEdit_vch_operid->setEnabled(false);
    ui->lineEdit_dt_operdate->setEnabled(false);

}

void w_m_member_point_recharge::took()
{
    //clicked (none) returns long [pbm_bnclicked]
    QString ls_memberno;
    QString ls_type;
    QString ls_memo;
    QString ls_err;
    double ldec_point;
    double ldec_point_org;


    ls_memberno=ui->lineEdit_vch_memberno->text();
    ls_memo=ui->comboBox_vch_memo->currentText();
    ldec_point=ui->lineEdit_num_point->text().toDouble();
    ldec_point_org=ui->lineEdit_num_point_org->text().toDouble();
    if(ldec_point == 0){
        lds_messagebox::warning(this, tr("消息提示"),tr("积分数值无效！"));
        return;
    }
    if(_type=="5"){//积分充积分
        ldec_point=qAbs(ldec_point);

    } else if(_type=="6"
              ||_type=="4"){//手工减积分
        ldec_point=-qAbs(ldec_point);
        if(ldec_point_org < qAbs(ldec_point)){
            lds_messagebox::warning(this, tr("消息提示"),tr("原积分值不足以完成本次操作！"));
            return;
        }
        if(_type=="4"){
            ls_memo=tr("礼品:")+ui->comboBox_vch_memo->currentText();
        }
    }

    //! <--transaction begin-->
    QSqlDatabase::database().transaction();
    if((ls_err=n_func::f_m_point(ls_memberno, "", ldec_point, _type, 0, ls_memo)).isEmpty()){
        QSqlDatabase::database().commit();
        _parent->torefresh();
        lds_messagebox::warning(this, tr("消息提示"),tr("操作成功!"));
        this->accept();
    } else {
        QSqlDatabase::database().rollback();
        _parent->torefresh();
        lds_messagebox::warning(this, tr("消息提示"),tr("操作失败!\n")+ ls_err);
    }
    //! <--transaction end-->

}

void w_m_member_point_recharge::toexit()
{
    this->reject();
}
