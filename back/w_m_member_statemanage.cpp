
#include "w_m_member_statemanage.h"
//会员状态管理
#include "ui_w_m_member_statemanage_dialog.h"
#include <QSqlError>
#include <QDebug>
#include "lds_model_sqlrelationaltablemodel.h"
#include <QSqlRecord>
#include "lds_tableview_repecify.h"
#include "w_m_member.h"
#include "w_m_member_list.h"
#include "n_func.h"
#include "lds_messagebox.h"
#include <QSqlQuery>
#include "uo_rw_member.h"
#include "backheader.h"
#include "lds_tableview_repecify.h"
#include "lds_tableview_delegate_check.h"

//【0】重写selectStatement()实现 另加额外列的方法
//【1】排序不计算额外列， 特殊颜色区分
//【2】每个类独立写，不统一父类
//【3】排序允许函数必须设置开始，否则将丢失额外列
//【4】表格需要show之后，才可以重写排写表头，否则无效
//【5】    QTimer::singleShot(0, this,SLOT(to_headerview_repecify())); 调用该函数，来显示额外列
w_m_member_statemanage::RelationalTableModel::RelationalTableModel(QObject *parent, QSqlDatabase db)
    : lds_model_sqlrelationaltablemodel(parent, db)
{
    backheader::regetvch_operid_name();
}

QVariant w_m_member_statemanage::RelationalTableModel::data(const QModelIndex &item, int role) const
{
    QVariant _d=lds_model_sqlrelationaltablemodel::data(item, role);
    if(role==Qt::DisplayRole){
        if(_d.type()==QVariant::DateTime){
            return _d.toDateTime().toString("yyyy-MM-dd");
        }
        if(item.column() ==9){//状态
            return  backheader::VIPFINDVALUE(w_m_member::VIP_MODEL_STATE, _d.toString());
        }
        if(item.column() ==14){//创建人员
            return backheader::getouterdata(backheader::vch_operid_name, _d.toString());
        }
    }
    return _d;
}

Qt::ItemFlags w_m_member_statemanage::RelationalTableModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsEnabled|Qt::ItemIsSelectable;

}

w_m_member_statemanage::w_m_member_statemanage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui_w_m_member_statemanage_Dialog)
{
    ui->setupUi(this);
    //! init
    //! tableview
    tablemodel_2=new RelationalTableModel(this);
    tablemodel_2->setJoinMode(QSqlRelationalTableModel::LeftJoin);
    tablemodel_2->setEditStrategy(QSqlTableModel::OnManualSubmit);tablemodel_2->setJoinMode(QSqlRelationalTableModel::LeftJoin);
    tablemodel_2->setTable("t_m_member");
    tablemodel_2->setRelation(2/*ch_typeno*/, QSqlRelation("t_m_member_type", "ch_typeno","vch_typename"));
    ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::SelectedClicked);ui->tableView->setModel(tablemodel_2->fpmodel);
    tablemodel_2->setHeaderData(0,Qt::Horizontal,lds_tableview_repecify::saveHeader(tr("会员编号"),headerstmp_2));
    tablemodel_2->setHeaderData(1,Qt::Horizontal,lds_tableview_repecify::saveHeader(tr("会员名称"),headerstmp_2));
    tablemodel_2->setHeaderData(2,Qt::Horizontal,lds_tableview_repecify::saveHeader(tr("会员类型"),headerstmp_2));
    tablemodel_2->setHeaderData(4,Qt::Horizontal,lds_tableview_repecify::saveHeader(tr("生日"),headerstmp_2));
    tablemodel_2->setHeaderData(5,Qt::Horizontal,lds_tableview_repecify::saveHeader(tr("手机"),headerstmp_2));
    tablemodel_2->setHeaderData(6,Qt::Horizontal,lds_tableview_repecify::saveHeader(tr("电话"),headerstmp_2));
    tablemodel_2->setHeaderData(9,Qt::Horizontal,lds_tableview_repecify::saveHeader(tr("状态"),headerstmp_2));
    tablemodel_2->setHeaderData(11,Qt::Horizontal,lds_tableview_repecify::saveHeader(tr("持卡"),headerstmp_2));
    tablemodel_2->setHeaderData(8,Qt::Horizontal,lds_tableview_repecify::saveHeader(tr("有效日期"),headerstmp_2));
    tablemodel_2->setHeaderData(14,Qt::Horizontal,lds_tableview_repecify::saveHeader(tr("创建人员"),headerstmp_2));

    QList<int> virtualheades;
    virtualheades = ui->tableView->rePecifyHeader( headerstmp_2);
    ui->tableView->setItemDelegate(new lds_tableview_delegate_check(QVector<int>()
                                                                    << virtualheades[7]
                                   ,
                                   ui->tableView));
    //! refresh
    torefresh();


    //! toolbar
    ui->widget->setup(QStringList()
                      << tr("刷新") << tr("详情")
                      << tr("冻结") << tr("挂失") << tr("作废") << tr("启用")
                      << tr("退现") << tr("取款") << tr("退卡") << tr("补卡")
                      << tr("退出"));
    connect(ui->widget->index_widget(tr("刷新")), SIGNAL(clicked()),this,SLOT(torefresh()));
    connect(ui->widget->index_widget(tr("详情")), SIGNAL(clicked()),this,SLOT(todetail()));
    connect(ui->widget->index_widget(tr("冻结")), SIGNAL(clicked()),this,SLOT(tostop()));
    connect(ui->widget->index_widget(tr("挂失")), SIGNAL(clicked()),this,SLOT(toloss()));
    connect(ui->widget->index_widget(tr("作废")), SIGNAL(clicked()),this,SLOT(toblank()));
    connect(ui->widget->index_widget(tr("启用")), SIGNAL(clicked()),this,SLOT(toopen()));
    connect(ui->widget->index_widget(tr("退现")), SIGNAL(clicked()),this,SLOT(toblackcash()));
    connect(ui->widget->index_widget(tr("取款")), SIGNAL(clicked()),this,SLOT(tofetch()));
    connect(ui->widget->index_widget(tr("退卡")), SIGNAL(clicked()),this,SLOT(toblankcard()));
    connect(ui->widget->index_widget(tr("补卡")), SIGNAL(clicked()),this,SLOT(tonewsend()));
    connect(ui->widget->index_widget(tr("退出")), SIGNAL(clicked()),this,SLOT(toexit()));
    connect(ui->lineEdit,SIGNAL(textChanged(QString)),this,SLOT(torefresh()));
    connect(ui->lineEdit_2,SIGNAL(textChanged(QString)),this,SLOT(torefresh()));

}


lds_tableView *w_m_member_statemanage::getTableview()
{
    return ui->tableView;
}

lds_model_sqlrelationaltablemodel *w_m_member_statemanage::getTablemodel()
{
    return tablemodel_2;
}

void w_m_member_statemanage::torefresh()
{
    tablemodel_2->setFilter(QString("(vch_memberno like '%1%' or vch_member like '%1%' )and(ifnull(vch_tel, '') like '%2%' or ifnull(vch_handtel, '') like '%2%') ")
                            .arg(ui->lineEdit->text())
                            .arg(ui->lineEdit_2->text())
                            );
    tablemodel_2->select();
    tablemodel_2->relationModel(2)->select();
    ui->tableView->restore();

}

void w_m_member_statemanage::todetail()
{
    w_m_member_list::statictosendcard(0, tablemodel_2, ui->tableView, false, tr("会员卡详情"));
}

void w_m_member_statemanage::tostop()
{
    //ue_stop (none) returns (none)
    long ll_row;
    QString ls_memberno;
    QString ls_state;
    QString ls_memo;
    QString ls_err;
    QString ls_save_hq;
    QDateTime ldt;
    QDateTime ldt_old;


    ll_row = ui->tableView->currentIndex().row();
    if(ll_row<0){
        lds_messagebox::warning(this, tr("消息提示"),tr("没有选中需要止付的会员!"));
        return;
    }
    ls_memberno=tablemodel_2->record(ll_row).value("vch_memberno").toString();
    ls_state=tablemodel_2->record(ll_row).value("ch_state").toString();
    //    items.clear();items<<new QStandardItem("1")<<new QStandardItem(tr("正常"));VIP_MODEL_STATE.appendRow(items);
    //    items.clear();items<<new QStandardItem("2")<<new QStandardItem(tr("止付"));VIP_MODEL_STATE.appendRow(items);
    //    items.clear();items<<new QStandardItem("3")<<new QStandardItem(tr("挂失"));VIP_MODEL_STATE.appendRow(items);
    //    items.clear();items<<new QStandardItem("4")<<new QStandardItem(tr("作废"));VIP_MODEL_STATE.appendRow(items);
    if(ls_state=="2"){
        lds_messagebox::warning(this, tr("消息提示"),tr("指定会员状态已为止付!"));
        return;

    } else if(ls_state=="4"){
        lds_messagebox::warning(this, tr("消息提示"),tr("指定会员状态为作废，不能止付!"));
        return;
    }
    w_m_member_statemanage_state dialog(this);
    dialog.setTitile(tr("会员号:")+ls_memberno+tr(",处理状态:止付"));
    dialog.textkey=tr("止付");
    if(    backheader::static_dialogexec(dialog,qobject_cast<QAbstractButton* >(sender())->text())==QDialog::Rejected) return;
    ls_memo=dialog.getExplain();

    qDebug() << ls_memo;
    if(!ls_memo.isEmpty()){
    }
    if(ls_memo=="OK")ls_memo="";
    ldt = n_func::f_get_sysdatetime();
    ldt_old=tablemodel_2->record(ll_row).value("dt_operdate").toDateTime();
    if(ldt_old.isValid()  &&  ldt_old > ldt){
        lds_messagebox::warning(this, tr("消息提示"),tr("系统时间小于上次保存时间，请确认系统时间是否正确！"));
        return;

    }
    //! <--transaction begin-->
    QSqlDatabase::database().transaction();
    QSqlQuery query;
    switch(0){
    case 0:
        ls_err=n_func::f_m_insert_log(ls_memberno,tr("止付"),ls_memo,0);
        if(!ls_err.isEmpty()){
            break;
        }
        query.prepare("update t_m_member set ch_state=:ls_state, dt_operdate=:ldt where  vch_memberno =:ls_memberno");
        query.bindValue(":ls_state", "2");
        query.bindValue(":ldt", ldt);
        query.bindValue(":ls_memberno", ls_memberno);
        ls_err=query.exec()?"":query.lastError().text();
        if(ls_err.isEmpty()){

        } else {
            break;
        }
    }
    if(ls_err.isEmpty()){
        QSqlDatabase::database().commit();
        ls_err=tr("状态变更成功！");

    } else {
        qDebug() << query.lastError().text();
        QSqlDatabase::database().rollback();
        ls_err=tr("状态变更失败!");

    }
    //! <--transaction end-->
    torefresh();
    lds_messagebox::warning(this, tr("消息提示"),ls_err);

}

void w_m_member_statemanage::toloss()
{
    //ue_loss (none) returns (none)
    long ll_row;
    QString ls_memberno;
    QString ls_state;
    QString ls_err;
    QString ls_memo;
    QDateTime ldt;
    QDateTime ldt_old;
    ll_row = ui->tableView->currentIndex().row();
    if(ll_row<0){
        lds_messagebox::warning(this, tr("消息提示"),tr("没有选中需要挂失会员!"));
        return;
    }
    ls_memberno=tablemodel_2->record(ll_row).value("vch_memberno").toString();
    ls_state=tablemodel_2->record(ll_row).value("ch_state").toString();
    //    items.clear();items<<new QStandardItem("1")<<new QStandardItem(tr("正常"));VIP_MODEL_STATE.appendRow(items);
    //    items.clear();items<<new QStandardItem("2")<<new QStandardItem(tr("止付"));VIP_MODEL_STATE.appendRow(items);
    //    items.clear();items<<new QStandardItem("3")<<new QStandardItem(tr("挂失"));VIP_MODEL_STATE.appendRow(items);
    //    items.clear();items<<new QStandardItem("4")<<new QStandardItem(tr("作废"));VIP_MODEL_STATE.appendRow(items);
    if(ls_state=="3"){
        lds_messagebox::warning(this, tr("消息提示"),tr("指定会员状态已为挂失!"));
        return;

    } else if(ls_state=="4"){
        lds_messagebox::warning(this, tr("消息提示"),tr("指定会员状态为作废，不能止付!"));
        return;
    }
    w_m_member_statemanage_state dialog(this);
    dialog.setTitile(tr("会员号:")+ls_memberno+tr(",处理状态:挂失"));
    dialog.textkey=tr("挂失");
    if(    backheader::static_dialogexec(dialog,qobject_cast<QAbstractButton* >(sender())->text())==QDialog::Rejected) return;
    ls_memo=dialog.getExplain();

    if(!ls_memo.isEmpty()){
    }
    if(ls_memo=="OK")ls_memo="";
    ldt = n_func::f_get_sysdatetime();
    ldt_old=tablemodel_2->record(ll_row).value("dt_operdate").toDateTime();
    if(ldt_old.isValid()  &&  ldt_old > ldt){
        lds_messagebox::warning(this, tr("消息提示"),tr("系统时间小于上次保存时间，请确认系统时间是否正确！"));
        return;
    }

    //! <--transaction begin-->
    QSqlDatabase::database().transaction();
    QSqlQuery query;
    switch(0){
    case 0:
        ls_err=n_func::f_m_insert_log(ls_memberno,tr("挂失"),ls_memo,0);
        if(!ls_err.isEmpty()){
            break;
        }
        query.prepare("update t_m_member set ch_state=:ls_state, dt_operdate=:ldt where  vch_memberno =:ls_memberno");
        query.bindValue(":ls_state", "3");
        query.bindValue(":ldt", ldt);
        query.bindValue(":ls_memberno", ls_memberno);
        ls_err=query.exec()?"":query.lastError().text();
        if(ls_err.isEmpty()){

        } else {
            break;
        }
    }
    if(ls_err.isEmpty()){
        QSqlDatabase::database().commit();
        ls_err=tr("状态变更成功！");

    } else {
        qDebug() << query.lastError().text();
        QSqlDatabase::database().rollback();
        ls_err=tr("状态变更失败!");

    }
    //! <--transaction end-->
    torefresh();
    lds_messagebox::warning(this, tr("消息提示"),ls_err);
}

void w_m_member_statemanage::toblank()
{
    //ue_blank (none) returns (none)
    long ll_row;
    long li_rtn;
    QString ls_memberno;
    QString ls_err;
    //    st_member lst_member;
    uo_rw_member::read_card_type lst_card;
    uo_rw_member uo_member;
    QDateTime ldt;
    QDateTime ldt_old;
    double ldec_remain;
    QString ls_value;
    QString ls_type;
    QString ls_bute;
    QString ls_cardflag;
    QString ls_state;
    QString ls_memo;
    double ldec_curamount;
    double ldec_base=0;

    ll_row = ui->tableView->currentIndex().row();
    if(ll_row<0){
        lds_messagebox::warning(this, tr("消息提示"),tr("没有选中需要作废会员!"));
        return;
    }
    ls_memberno=tablemodel_2->record(ll_row).value("vch_memberno").toString();
    ls_state=tablemodel_2->record(ll_row).value("ch_state").toString();
    ls_cardflag=tablemodel_2->record(ll_row).value("ch_cardflag").toString();
    //    items.clear();items<<new QStandardItem("1")<<new QStandardItem(tr("正常"));VIP_MODEL_STATE.appendRow(items);
    //    items.clear();items<<new QStandardItem("2")<<new QStandardItem(tr("止付"));VIP_MODEL_STATE.appendRow(items);
    //    items.clear();items<<new QStandardItem("3")<<new QStandardItem(tr("挂失"));VIP_MODEL_STATE.appendRow(items);
    //    items.clear();items<<new QStandardItem("4")<<new QStandardItem(tr("作废"));VIP_MODEL_STATE.appendRow(items);
    if(ls_state=="4"){
        lds_messagebox::warning(this, tr("消息提示"),tr("指定会员状态已为作废!"));
        return;

    } else if(ls_state=="2"){
        lds_messagebox::warning(this, tr("消息提示"),tr("止付卡不能作废!"));
        return;
    }
    if(ls_cardflag=="N"){
        lds_messagebox::warning(this, tr("消息提示"),tr("该会员卡还未制卡，不需要作废!"));
        return;
    }

    //获取会员详细数据
    uo_member.of_init("4");
    lst_card=uo_member.of_read_card(ls_memberno);
    if(lst_card.flag == false){
        lds_messagebox::warning(this, tr("消息提示"),lst_card.errtext);
        return;
    }
    QSqlQuery query;
    query.exec(QString("select int_basedata,ch_typeno from t_m_member where vch_memberno='%1'").arg(ls_memberno));
    if(query.next()){

    } else {
        lds_messagebox::warning(this, tr("消息提示"),tr("查询会员保本金额失败!"));
        return;

    }
    ldec_base=query.record().value("int_basedata").toDouble();
    ls_type=query.record().value("ch_typeno").toString();

    query.prepare("select ch_attribute from t_m_member_type where ch_typeno =:ls_type");
    query.bindValue(":ls_type",ls_type);
    query.exec();
    if(query.next()){
        ls_bute=query.record().value("ch_attribute").toString();
    } else {
        lds_messagebox::warning(this, tr("消息提示"),tr("查询会员类型失败!"));
        return;
    }
    if(ls_bute!="1"){
        if(n_func::f_m_get_parameter("m_fetch_base","0")=="1"){
            if(lst_card.amount>0){
                lds_messagebox::warning(this, tr("消息提示"),tr("卡余额(包含保本金额,保本金额可取)不为零,不能作废,~r~n可进行取款操作，待余额为零再作废!"));
                return;
            }
            ldec_base=0;
        } else {
            if(lst_card.amount>ldec_base){
                lds_messagebox::warning(this, tr("消息提示"),tr("卡余额(不包含保本金额,保本金额不可取)不为零,不能作废,~r~n可进行取款操作，待余额为零再作废!"));
                return;
            }
        }
    }
    w_m_member_statemanage_state dialog(this);
    dialog.setTitile(tr("会员号:")+ls_memberno+tr(",处理状态:作废"));
    dialog.textkey=tr("作废");
    if(    backheader::static_dialogexec(dialog,qobject_cast<QAbstractButton* >(sender())->text())==QDialog::Rejected) return;
    ls_memo=dialog.getExplain();
    if(!ls_memo.isEmpty()){
    }
    if(ls_memo=="OK")ls_memo="";
    ldt = n_func::f_get_sysdatetime();
    ldt_old=tablemodel_2->record(ll_row).value("dt_operdate").toDateTime();
    if(ldt_old.isValid()  &&  ldt_old > ldt){
        lds_messagebox::warning(this, tr("消息提示"),tr("系统时间小于上次保存时间，请确认系统时间是否正确！"));
        return;
    }

    //! <--transaction begin-->
    QSqlDatabase::database().transaction();
    switch(0){
    case 0:
        ls_err=n_func::f_m_insert_log(ls_memberno,tr("作废"),ls_memo,0);
        if(!ls_err.isEmpty()){
            break;
        }
        query.prepare("update t_m_member set ch_state=:ls_state, dt_operdate=:ldt where  vch_memberno =:ls_memberno");
        query.bindValue(":ls_state", "4");
        query.bindValue(":ldt", ldt);
        query.bindValue(":ls_memberno", ls_memberno);
        ls_err=query.exec()?"":query.lastError().text();
        if(ls_err.isEmpty()){

        } else {
            break;
        }
    }
    if(ls_err.isEmpty()){
        QSqlDatabase::database().commit();
        ls_err=tr("状态变更成功！");

    } else {
        qDebug() << query.lastError().text();
        QSqlDatabase::database().rollback();
        ls_err=tr("状态变更失败!");

    }
    //! <--transaction end-->
    torefresh();
    lds_messagebox::warning(this, tr("消息提示"),ls_err);

}

void w_m_member_statemanage::toopen()
{
    //ue_loss (none) returns (none)
    long ll_row;
    QString ls_memberno;
    QString ls_state;
    QString ls_err;
    QString ls_memo;
    QDateTime ldt;
    QDateTime ldt_old;
    ll_row = ui->tableView->currentIndex().row();
    if(ll_row<0){
        lds_messagebox::warning(this, tr("消息提示"),tr("没有选中需要启用会员!"));
        return;
    }
    ls_memberno=tablemodel_2->record(ll_row).value("vch_memberno").toString();
    ls_state=tablemodel_2->record(ll_row).value("ch_state").toString();
    //    items.clear();items<<new QStandardItem("1")<<new QStandardItem(tr("正常"));VIP_MODEL_STATE.appendRow(items);
    //    items.clear();items<<new QStandardItem("2")<<new QStandardItem(tr("止付"));VIP_MODEL_STATE.appendRow(items);
    //    items.clear();items<<new QStandardItem("3")<<new QStandardItem(tr("挂失"));VIP_MODEL_STATE.appendRow(items);
    //    items.clear();items<<new QStandardItem("4")<<new QStandardItem(tr("作废"));VIP_MODEL_STATE.appendRow(items);
    if(ls_state=="1"){
        lds_messagebox::warning(this, tr("消息提示"),tr("指定会员状态已为正常!"));
        return;

    } else if(ls_state=="4"){
        lds_messagebox::warning(this, tr("消息提示"),tr("指定会员状态为作废，不能启用!"));
        return;
    }
    w_m_member_statemanage_state dialog(this);
    dialog.setTitile(tr("会员号:")+ls_memberno+tr(",处理状态:启用"));
    dialog.textkey=tr("启用");
    if(    backheader::static_dialogexec(dialog,qobject_cast<QAbstractButton* >(sender())->text())==QDialog::Rejected) return;
    ls_memo=dialog.getExplain();

    if(!ls_memo.isEmpty()){
    }
    if(ls_memo=="OK")ls_memo="";
    ldt = n_func::f_get_sysdatetime();
    ldt_old=tablemodel_2->record(ll_row).value("dt_operdate").toDateTime();
    if(ldt_old.isValid()  &&  ldt_old > ldt){
        lds_messagebox::warning(this, tr("消息提示"),tr("系统时间小于上次保存时间，请确认系统时间是否正确！"));
        return;
    }

    //! <--transaction begin-->
    QSqlDatabase::database().transaction();
    QSqlQuery query;
    switch(0){
    case 0:
        ls_err=n_func::f_m_insert_log(ls_memberno,tr("启用"),ls_memo,0);
        if(!ls_err.isEmpty()){
            break;
        }
        query.prepare("update t_m_member set ch_state=:ls_state, dt_operdate=:ldt where  vch_memberno =:ls_memberno");
        query.bindValue(":ls_state", "1");
        query.bindValue(":ldt", ldt);
        query.bindValue(":ls_memberno", ls_memberno);
        ls_err=query.exec()?"":query.lastError().text();
        if(ls_err.isEmpty()){

        } else {
            break;
        }
    }
    if(ls_err.isEmpty()){
        QSqlDatabase::database().commit();
        ls_err=tr("状态变更成功！");

    } else {
        qDebug() << query.lastError().text();
        QSqlDatabase::database().rollback();
        ls_err=tr("状态变更失败!");

    }
    //! <--transaction end-->
    torefresh();
    lds_messagebox::warning(this, tr("消息提示"),ls_err);
}

void w_m_member_statemanage::toblackcash()
{
    //ue_blank_cash (none) returns (none)
    long ll_row;
    QString ls_memberno;
    QString ls_err;
    n_func::st_member_type  lst_member;
    uo_rw_member::read_card_type lst_card;
    uo_rw_member uo_member;
    QDateTime ldt;
    QDateTime ldt_old;
    double ldec_bcash;

    ll_row = ui->tableView->currentIndex().row();
    if(ll_row<0){
        lds_messagebox::warning(this, tr("消息提示"),tr("没有选中需要退现会员!"));
        return;
    }
    ls_memberno=tablemodel_2->record(ll_row).value("vch_memberno").toString();
    lst_member = n_func::f_m_get_limit(ls_memberno);
    //    items.clear();items<<new QStandardItem("1")<<new QStandardItem(tr("正常"));VIP_MODEL_STATE.appendRow(items);
    //    items.clear();items<<new QStandardItem("2")<<new QStandardItem(tr("止付"));VIP_MODEL_STATE.appendRow(items);
    //    items.clear();items<<new QStandardItem("3")<<new QStandardItem(tr("挂失"));VIP_MODEL_STATE.appendRow(items);
    //    items.clear();items<<new QStandardItem("4")<<new QStandardItem(tr("作废"));VIP_MODEL_STATE.appendRow(items);


    if(lst_member.as_m_state != "3"){
        lds_messagebox::warning(this, tr("消息提示"),tr("退现功能只针对挂失状态会员，操作中止!"));
        return;
    }
    if(lst_member.as_m_att != "2"){
        lds_messagebox::warning(this, tr("消息提示"),tr("退现功能只针对充值类会员，操作中止!"));
        return;
    }

    ldt = n_func::f_get_sysdatetime();
    ldt_old=tablemodel_2->record(ll_row).value("dt_operdate").toDateTime();
    if(ldt_old.isValid()  &&  ldt_old > ldt){
        lds_messagebox::warning(this, tr("消息提示"),tr("系统时间小于上次保存时间，请确认系统时间是否正确！"));
        return;
    }

    //    uo_member = CREATE uo_rw_member
    uo_member.of_init("4");
    lst_card = uo_member.of_read_card(ls_memberno);
    if(lst_card.flag == false){
        lds_messagebox::warning(this, tr("消息提示"),lst_card.errtext);
        return;
    }
    if(lst_card.flag){
        if(lst_card.amount==0){
            lds_messagebox::warning(this, tr("消息提示"),tr("会员当前余额为零，无需退现!"));
            return;
        }
        ldec_bcash = lst_card.amount - lst_member.adec_m_limitdata;
        if(ldec_bcash <= 0){
            lds_messagebox::warning(this, tr("消息提示"),tr("会员当前无保本外可退现金额，操作中止!"));
            return;
        }


        if(0==lds_messagebox::warning(this, tr("消息提示"), QString("%1[%2]%3[%4]%5")
                                      .arg(tr("确定要将会员"))
                                      .arg(lst_member.as_member)
                                      .arg(tr("账户余额"))
                                      .arg(QString().sprintf("%.02f", ldec_bcash))
                                      .arg(tr("作退现处理吗？")),
                                      tr("确定"), tr("取消"))){
            //! <--transaction begin-->
            QSqlDatabase::database().transaction();
            ls_err= n_func::f_m_insert_log(ls_memberno,
                                           tr("退现"),
                                           tr("金额:") + QString().sprintf("%.02f", ldec_bcash),
                                           0);
            if(ls_err.isEmpty()){
                ls_err=n_func::f_m_deposit(ls_memberno, - ldec_bcash, - ldec_bcash,lst_member.adec_m_limitdata,"5","5",tr("挂失退现转结"));
                if(ls_err.isEmpty()){
                    QSqlDatabase::database().commit();
                }
            }
            QSqlDatabase::database().rollback();
            //! <--transaction end-->
            torefresh();
            if(ls_err.isEmpty()){
                lds_messagebox::warning(this, tr("消息提示"),tr("退现成功!"));

            } else {
                qDebug() << ls_err;
                lds_messagebox::warning(this, tr("消息提示"),tr("读取会员帐务数据失败，操作中止!"));
            }
        }
    } else {
        lds_messagebox::warning(this, tr("消息提示"),lst_card.errtext);
    }
}

void w_m_member_statemanage::tofetch()
{
    //ue_fetch (none) returns (none)
    w_m_member_list::statictorecharge(this, tablemodel_2, ui->tableView,"C");
    torefresh();
}

void w_m_member_statemanage::toblankcard()
{
    //ue_blank_card (none) returns (none)
    w_m_member_list::statictorecharge(this, tablemodel_2, ui->tableView,"D");
    torefresh();
}

void w_m_member_statemanage::tonewsend()
{
    //ue_new_send (none) returns (none)
    long ll_row;
    QString ls_memberno;
    QString ls_err;
    QString ls_value;
    n_func::st_member_type  lst_member;
    uo_rw_member::read_card_type lst_card;
    uo_rw_member uo_member;
    QDateTime ldt;
    QDateTime ldt_old;
    double ldec_bcash;

    uo_member.of_init("4");
    ll_row = ui->tableView->currentIndex().row();
    if(ll_row<0){
        lds_messagebox::warning(this, tr("消息提示"),tr("没有选中需要补卡会员!"));
        return;
    }
    ls_memberno=tablemodel_2->record(ll_row).value("vch_memberno").toString();
    lst_member = n_func::f_m_get_limit(ls_memberno);

    if(lst_member.as_m_state != "3"){
        lds_messagebox::warning(this, tr("消息提示"),tr("补卡功能只针对挂失状态会员，操作中止!"));
        return;
    }
    if(lst_member.as_m_att != "2"){
        if(lst_member.as_m_att == "3"){
            lds_messagebox::warning(this, tr("消息提示"),tr("挂帐会员请结清帐务后，创建新会员并发卡!"));
            return;
        } else {
            lds_messagebox::warning(this, tr("消息提示"),tr("补卡功能只针对充值类会员，操作中止!"));
            return;
        }
    }

    if(lst_member.as_m_cardflag!="Y"){
        ls_value = n_func::f_m_get_parameter("vip_card_type",tr("无会员卡"));
        if(ls_value == tr("IC卡") || ls_value == tr("磁卡")){
            lds_messagebox::warning(this, tr("消息提示"),tr("会员卡%1还未发卡,不能补卡!").arg(ls_memberno));
            return;
        }
    }

    ldt = n_func::f_get_sysdatetime();
    ldt_old = tablemodel_2->record(ll_row).value("dt_operdate").toDateTime();
    if(ldt_old.isValid() && ldt_old > ldt){
        lds_messagebox::warning(this, tr("消息提示"),tr("系统时间小于上次保存时间，请确认系统时间是否正确!"));
        return;
    }
    lst_card = uo_member.of_read_card(ls_memberno);
    if(lst_card.flag == false){
        lds_messagebox::warning(this, tr("消息提示"),lst_card.errtext);
        return;
    }
    if(lst_card.flag){
        if(lst_card.amount==0){
            lds_messagebox::warning(this, tr("消息提示"),tr("会员当前余额为零，无需补卡，请直接创建新会员并发卡!"));
            return;
        }
        ldec_bcash = lst_card.amount - lst_member.adec_m_limitdata;
        if(ldec_bcash <= 0){
            lds_messagebox::warning(this, tr("消息提示"),tr("会员当前无保本外可用金额，不能补卡!"));
            return;
        }
        /*开始补卡*/
        w_m_member_statemanage_trans dialog(this);
        if(dialog.initData1()){
            backheader::static_dialogexec(dialog,qobject_cast<QAbstractButton* >(sender())->text());
            torefresh();
        }
        return;
    }
    lds_messagebox::warning(this, tr("消息提示"),tr("读取会员帐务数据失败，操作中止!"));
    return;
}

void w_m_member_statemanage::toexit()
{
    this->reject();
}

//===============================
#include "ui_w_m_member_state_dialog.h"
w_m_member_statemanage_state::w_m_member_statemanage_state(QWidget *parent):
    QDialog(parent),ui(new Ui_w_m_member_state_Dialog)
{
    ui->setupUi(this);
    ui->widget->setup(QStringList()<<tr("确定")<<tr("取消"), Qt::AlignRight);
    connect(ui->widget->index_widget(tr("确定")),SIGNAL(clicked()),this,SLOT(took()));
    connect(ui->widget->index_widget(tr("取消")),SIGNAL(clicked()),this,SLOT(reject()));

}

void w_m_member_statemanage_state::setTitile(const QString &text)
{
    ui->label->setText(text);
}

QString w_m_member_statemanage_state::getExplain()
{
    return ui->lineEdit->text();
}

void w_m_member_statemanage_state::took()
{
    if(ui->lineEdit->text().isEmpty()){
        //        lds_messagebox::warning(this, tr("消息提示"),tr("附加说明不能为空!"));
        this->accept();
    } else  if(0==lds_messagebox::warning(this, tr("消息提示"),tr("确定指定会员")+textkey+"?", tr("确定"), tr("取消"))){
        this->accept();
    } else {
        this->reject();
    }
}

void w_m_member_statemanage_state::tocancel()
{
}

//===================================
#include "ui_w_m_member_trans_dialog.h"
w_m_member_statemanage_trans::w_m_member_statemanage_trans(QWidget *parent):
    QDialog(parent),
    ui(new Ui_w_m_member_trans_Dialog),
    _parent(static_cast<w_m_member_statemanage *>(parent))
{

    ui->setupUi(this);
    cb_read1=new uo_rw_member;
    cb_read2=new uo_rw_member;

    cb_read1->of_init("1");
    cb_read2->of_init("1");

    is_type=tr("补卡");
    is_lmemberno=_parent->getTablemodel()->record(_parent->getTableview()->currentIndex().row()).value("vch_memberno").toString();
    //! toolbar
    ui->widget->setup(QStringList() << tr("补卡") << tr("退出"), Qt::AlignRight);
    connect(ui->widget->index_widget(tr("补卡")), SIGNAL(clicked()),this,SLOT(tonewcard()));
    connect(ui->widget->index_widget(tr("退出")), SIGNAL(clicked()),this,SLOT(toexit()));

}
w_m_member_statemanage_trans::~w_m_member_statemanage_trans()
{
    delete cb_read1;
    delete cb_read2;
}


void w_m_member_statemanage_trans::tonewcard()
{
    //clicked (none) returns long [pbm_bnclicked]
    //long ancestorreturnvalue
    uo_rw_member::read_card_type lst_card1;
    uo_rw_member::read_card_type lst_card2;
    uo_rw_member::read_card_type lst_card;
    n_func::st_member_type lst_member1;
    uo_rw_member uo_member;
    QString ls_memberno1;
    QString ls_memberno2;
    QString ls_err;
    QString ls_memo;
    QString ls_hq_save;
    QString ls_cardno;
    double ldec_trans;
    QDateTime ldt;


    uo_member.of_init("4");
    ls_memberno1 = ui->lineEdit_oldvipno->text();
    ls_memberno2 = ui->lineEdit_newvipno->text();
    ls_cardno = ui->lineEdit_newcardno->text();
    if(ls_memberno1.isEmpty() || ls_memberno2.isEmpty()){
        lds_messagebox::warning(this, tr("消息提示"),tr("请先输入新卡卡号!"));
        return;
    }
    if(ls_cardno.trimmed().isEmpty()){
        lds_messagebox::warning(this, tr("消息提示"),tr("请先输入新卡卡面卡号!"));
        return;
    }

    ldec_trans = ui->lineEdit_tonewnum->text().toDouble();
    if(ldec_trans <= 0){
        lds_messagebox::warning(this, tr("消息提示"),tr("转入新卡金额为零!"));
        return;
    }
    if(1==lds_messagebox::warning(this, tr("消息提示"),tr("确定要执行此%1操作吗?").arg(this->is_type), tr("确定"), tr("取消"))){
        return;
    }

    //! <--transaction begin-->
    QSqlDatabase::database().transaction();
    QSqlQuery query;
    switch(0){//4
    case 0:
        if(ls_memberno1==ls_memberno2){
            ls_err=tr("转入库会员卡号不能与转出卡号相同!");
            break;
        }

        if((ls_err=this->wf_addsendcard()).isEmpty()){//3
            lst_card1 = uo_member.of_read_card(ls_memberno1);
            if(lst_card1.flag == false){
                ls_err=lst_card1.errtext;
                break;
            }
            lst_card2 = uo_member.of_read_card(ls_memberno2);
            if(lst_card2.flag == false){
                ls_err=lst_card2.errtext;
                break;
            }
            lst_member1 = n_func::f_m_get_limit(ls_memberno1);

            if(ui->checkBox_istonewyajin->isChecked()){
                if(lst_card1.amount<ldec_trans){
                    ls_err=tr("原挂失会员余额不足以完成本次补卡操作!");
                    break;
                }
            } else {
                if((lst_card1.amount - lst_member1.adec_m_limitdata) < ldec_trans){
                    ls_err=tr("转出会员余额") + QString().sprintf("%.02f", lst_card1.amount) + tr("除保本金额") + QString().sprintf("%.02f", lst_member1.adec_m_limitdata) + tr("外不足以完成本次转帐操作!");
                    break;
                }
            }
            ls_memo=tr("挂失补卡转帐");
            if((ls_err=n_func::f_m_deposit(ls_memberno1, - ldec_trans,0,lst_card1.amount - ldec_trans,"5","5",ls_memo)).isEmpty()){//3
                if((ls_err=n_func::f_m_deposit(ls_memberno2,ldec_trans,0,lst_card2.amount + ldec_trans,"5","5","")).isEmpty()){//3
                    if(lst_card1.point!=0){//2
                        query.prepare("update t_m_curamount   set num_point =:lst_card1_point  where vch_memberno =:ls_memberno2");
                        query.bindValue(":lst_card1_point", lst_card1.point);
                        query.bindValue(":ls_memberno2", ls_memberno2);
                        if(!query.exec()) {
                            ls_err=query.lastError().text();
                            break;
                        }
                        query.prepare("iNSERT t_m_point(vch_memberno , ch_type , num_point , vch_operid , dt_operdate , vch_memo) values(:ls_memberno2 , '5' , :lst_card1_point , :sys , now() , :buka )");
                        query.bindValue(":ls_memberno2", ls_memberno2);
                        query.bindValue(":lst_card1_point", lst_card1.point);
                        query.bindValue(":sys", tr("系统"));
                        query.bindValue(":buka", tr("补卡"));
                        if(!query.exec()) {
                            ls_err=query.lastError().text();
                            break;
                        }
                    }//2
                    if((ls_err=cb_read2->of_send_card(ls_memberno2)).isEmpty()){//1
                        if((ls_err=n_func::f_m_insert_log(ls_memberno1,tr("补卡"),tr("目标卡号:") + ls_memberno2 + tr("转出金额:") + QString().sprintf("%.02f", ldec_trans),0)).isEmpty()){//1
                            if((ls_err=n_func::f_m_insert_log(ls_memberno2,tr("补卡"),tr("原卡号:") + ls_memberno1 + tr("转入金额:") + QString().sprintf("%.02f", ldec_trans),0)).isEmpty()){//1
                                /*success*/
                                break;
                            }//1
                        }//1
                    }//1
                    break;
                }//3
            }//3
        }//3
    }//4

    if(ls_err.isEmpty()) {
        QSqlDatabase::database().commit();
        ls_err=tr("%1成功").arg(this->is_type);
    } else {
        QSqlDatabase::database().rollback();
    }
    //! <--transaction end-->

    _parent->torefresh();
    lds_messagebox::warning(this, tr("消息提示"),ls_err);
}

//    update t_m_curamount
//    set num_point =:lst_card1.point
//    where vch_memberno =:ls_memberno2
//    USING sqlca;
//    IF sqlca.sqlcode < 0 THEN //50
//    GOTO LABEL_KENSHU_74
//    END IF //50
//    iNSERT t_m_point(vch_memberno , ch_type , num_point , vch_operid , dt_operdate , vch_memo )
//    values(:ls_memberno2 , '5' , :lst_card1.point , '系统' , getdate() , '补卡' )
//    USING sqlca;
//    IF sqlca.sqlcode < 0 THEN //53
//    GOTO LABEL_KENSHU_74
//    END IF //53
//    END IF //48
//    IF  NOT (PARENT.cb_read2.of_send_card(ls_memberno2)) THEN //55
//    GOTO LABEL_KENSHU_74
//    END IF //55
//    f_m_insert_log(ls_memberno1,"补卡","目标卡号:" + ls_memberno2 + "转出金额:" + string(ldec_trans,"0.00"),0)
//    f_m_insert_log(ls_memberno2,"补卡","原卡号:" + ls_memberno1 + "转入金额:" + string(ldec_trans,"0.00"),0)
//    ELSE //47
//    f_m_insert_log(ls_memberno1,"转帐","目标卡号:" + ls_memberno2 + "转出金额:" + string(ldec_trans,"0.00"),0)
//    f_m_insert_log(ls_memberno2,"转帐","原卡号:" + ls_memberno1 + "转入金额:" + string(ldec_trans,"0.00"),0)
//    END IF //47
//    COMMIT USING sqlca;
//    PARENT.sle_1.text = "";PARENT.sle_2.text = ""
//    PARENT.sle_3.text = "";PARENT.sle_4.text = ""
//    PARENT.sle_5.text = "";PARENT.sle_6.text = ""
//    PARENT.sle_7.text = "";PARENT.sle_8.text = ""
//    PARENT.sle_amount.text = "0.00"
//    messagebox(gvar.gs_appname,PARENT.is_type + "成功!")
//    IF PARENT.is_type = "补卡" THEN //69
//    PARENT.cb_4.EVENT clicked()
//    RETURN
//    END IF //69
//    GOTO LABEL_KENSHU_77
//    RETURN
//    END IF //45
//    END IF //43
//    LABEL_KENSHU_74:
//    IF ls_err = "" THEN ls_err = sqlca.sqlerrtext
//    ROLLBACK USING sqlca;
//    messagebox(gvar.gs_appname,PARENT.is_type + "操作失败 " + ls_err)
//    END IF //19
//    END IF //6
//    LABEL_KENSHU_77:
//    DESTROY uo_member
//    PARENT.sle_amount.setfocus()
//    IF PARENT.is_type = "补卡" THEN //79
//    PARENT.sle_amount.selecttext(1,len(PARENT.sle_amount.text))
//    END IF //79
//    RETURN

void w_m_member_statemanage_trans::toexit()
{
    this->reject();
}

bool w_m_member_statemanage_trans::initData1()
{
    //clicked (none) returns long [pbm_bnclicked]
    //long ancestorreturnvalue
    uo_rw_member::read_card_type lst_card;
    n_func::st_member_type  lst_member;
    QString ls_state;
    QString ls_name;
    double ldec_amount;
    double ldtc_point;

    lst_card = cb_read1->of_read_card(is_lmemberno);
    if(lst_card.flag == false){
        lds_messagebox::warning(this, tr("消息提示"),lst_card.errtext);
        return false;
    }
    if(lst_card.flag){
        QString m_state=backheader::VIPFINDVALUE(w_m_member::VIP_MODEL_STATE, n_func::f_m_get_state(is_lmemberno));
        if(m_state!=tr("挂失")){
            lds_messagebox::warning(this, tr("消息提示"),tr("此卡状态为%1，不能操作!").arg(m_state));
            return false;
        }

        lst_member=n_func::f_m_get_item(lst_card.cardid);
        if(lst_member.as_m_att != "2"){
            qDebug() << lst_member.as_m_att;
            lds_messagebox::warning(this, tr("消息提示"),tr("非充值卡会员，不能操作!").arg(m_state));
            return false;
        }

        if(!lst_card.cardid.isEmpty() && lst_card.cardid==ui->lineEdit_newvipno->text()){
            lds_messagebox::warning(this, tr("消息提示"),tr("原挂失会员与新发卡会员不能为同一会员！"));
            return false;
        }
        ui->lineEdit_oldvipno->setText(lst_card.cardid);
        ui->lineEdit_oldvipname->setText(lst_member.as_member);
        lst_member=n_func::f_m_get_limit(is_lmemberno);
        ii_basedata=int(lst_member.adec_m_limitdata);
        ui->lineEdit_tonewnum->setText(QString().sprintf("%.02f", (lst_card.amount-lst_member.adec_m_limitdata)));

        ui->lineEdit_oldamount->setText(QString().sprintf("%.02f", lst_card.amount));
        ui->lineEdit_oldpoint->setText(QString().sprintf("%.02f", lst_card.point));

        ui->lineEdit_newvipname->setText(ui->lineEdit_oldvipname->text());
        ui->lineEdit_newamount->setText(ui->lineEdit_oldamount->text());
        ui->lineEdit_newpoint->setText(ui->lineEdit_oldpoint->text());

        ui->lineEdit_tonewnum->setFocus();
        ui->lineEdit_tonewnum->selectAll();
    } else {
        lds_messagebox::warning(this, tr("消息提示"),lst_card.errtext);
        return false;

    }
    return true;
}

QString w_m_member_statemanage_trans::wf_addsendcard()
{
    //Public function wf_addsendcard (none) returns integer
    QString ls_oldmemberno;
    QString ls_newmemberno;
    QString ls_errtext;
    QString ls_cardno;
    QString ls_sql;
    int li_exists;
    int li_basedata=0;

    if(!ui->checkBox_istonewyajin->isChecked()){
        li_basedata=this->ii_basedata;
    }

    ls_oldmemberno = ui->lineEdit_oldvipno->text();
    ls_newmemberno = ui->lineEdit_newvipno->text();
    ls_cardno = ui->lineEdit_newcardno->text();
    ls_sql = " insert into t_m_member(vch_memberno,vch_member,ch_typeno,vch_id,dt_birthday,";
    ls_sql = ls_sql + " vch_tel,vch_handtel,vch_address,dt_limit,ch_state,num_limit,ch_cardflag,";
    ls_sql = ls_sql + " vch_password,vch_operID,dt_operdate,vch_cardno) ";
    ls_sql = ls_sql + " select '" + ls_newmemberno + "',vch_member,ch_typeno,vch_id,dt_birthday,vch_tel,vch_handtel,";
    ls_sql = ls_sql + " vch_address,dt_limit,'1',num_limit,'N',vch_password,'" + n_func::gs_operid + "',now(), " + ls_cardno;
    ls_sql = ls_sql + " from t_m_member where vch_memberno = '" + ls_oldmemberno + "'";

    QSqlQuery query;
    query.exec(QString("select count(1) from t_m_member where vch_member ='%1'").arg(ls_newmemberno));
    query.next();
    li_exists=query.record().value(0).toInt();
    if(li_exists > 0){
        return tr("此会员卡号[") + ls_newmemberno + tr("]已被使用，请重新输入!");
    }
    li_exists = 0;
    query.exec(QString("select count(1) from t_m_member where vch_cardno = '%1'").arg(ls_cardno));
    query.next();
    li_exists=query.record().value(0).toInt();
    if(li_exists > 0){
        return tr("此会员卡面卡号[") + ls_newmemberno + tr("]已被使用，请重新输入!");
    }

    qDebug() << ls_sql;
    if(query.exec(ls_sql)){
        query.prepare("update t_m_member set ch_state ='4' , vch_cardno =null , int_basedata =:li_basedata where vch_memberno =:ls_oldmemberno");
        query.bindValue(":li_basedata", li_basedata);
        query.bindValue(":ls_oldmemberno", ls_oldmemberno);
        if(query.exec()){
            return "";
        }
    }
    return tr("修改原卡状态失败:")+query.lastError().text();
}
