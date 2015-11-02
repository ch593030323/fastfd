#include "w_m_member.h"
#include "w_m_member_list.h"
//会员资料维护
#include "ui_w_m_member_dialog.h"
#include "lds_model_sqlrelationaltablemodel.h"

#include <QSqlQuery>
#include <QSqlRecord>
#include "n_func.h"
#include "lds_messagebox.h"
#include <QSqlError>
#include <QDebug>
#include "lds_model_sqlrelationaltablemodel.h"
#include "lds_tableview_repecify.h"
#include "backheader.h"
#include "reader_rf.h"
#include <QSettings>
#include "w_sys_manage_basicdataexport.h"
#include "public_cika_read_dialog.h"
bool w_m_member_list::is_yun_member=false;

w_m_member_list::w_m_member_list(QWidget *parent) :
    QDialog(parent), ui(new Ui_w_m_member_Dialog)
{
    ui->setupUi(this);

    //! init
    //! treewidget
    ui->treeWidget->header()->hide();
    ui->treeWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->treeWidget->setColumnCount(1);
    ui->treeWidget->rootitem=new lds_treeWidgetItem(QStringList()
                                                    <<tr("全部类型"));
    ui->treeWidget->addTopLevelItem(ui->treeWidget->rootitem);

    //! tableview
    tablemodel_2=new RelationalTableModel(this);
    tablemodel_2->setJoinMode(QSqlRelationalTableModel::LeftJoin);
    tablemodel_2->setEditStrategy(QSqlTableModel::OnManualSubmit);tablemodel_2->setJoinMode(QSqlRelationalTableModel::LeftJoin);
    tablemodel_2->setTable("t_m_member");
    tablemodel_2->setRelation(2/*ch_typeno*/, QSqlRelation("t_m_member_type", "ch_typeno","vch_typename"));
    ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::SelectedClicked);ui->tableView->setModel(tablemodel_2->fpmodel);
    tablemodel_2->setHeaderData(0,Qt::Horizontal,lds_tableview_repecify::saveHeader(tr("会员编号"),headerstmp_2));
    tablemodel_2->setHeaderData(12,Qt::Horizontal,lds_tableview_repecify::saveHeader(tr("卡面卡号"),headerstmp_2));
    tablemodel_2->setHeaderData(1,Qt::Horizontal,lds_tableview_repecify::saveHeader(tr("会员名称"),headerstmp_2));
    tablemodel_2->setHeaderData(2,Qt::Horizontal,lds_tableview_repecify::saveHeader(tr("会员类型"),headerstmp_2));
    tablemodel_2->setHeaderData(4,Qt::Horizontal,lds_tableview_repecify::saveHeader(tr("生日"),headerstmp_2));
    tablemodel_2->setHeaderData(5,Qt::Horizontal,lds_tableview_repecify::saveHeader(tr("手机"),headerstmp_2));
    tablemodel_2->setHeaderData(6,Qt::Horizontal,lds_tableview_repecify::saveHeader(tr("电话"),headerstmp_2));
    tablemodel_2->setHeaderData(9,Qt::Horizontal,lds_tableview_repecify::saveHeader(tr("状态"),headerstmp_2));
    tablemodel_2->setHeaderData(11,Qt::Horizontal,lds_tableview_repecify::saveHeader(tr("持卡"),headerstmp_2));

    ui->tableView->rePecifyHeader( headerstmp_2);

    //! refresh
    torefresh();

    //! toolbar
    ui->widget->setup(QStringList()
                      << tr("会员类型")<< tr("新增") << tr("删除") << tr("批量新增")
                      << tr("发卡") << tr("充值") << tr("设密码")
                      << tr("刷新") << tr("导出")
                      << tr("退出"));

    connect(ui->treeWidget,SIGNAL(pressed(QModelIndex)),this,SLOT(treeselect()));
    connect(ui->lineEdit,SIGNAL(textChanged(QString)),this,SLOT(treeselect()));
    connect(ui->lineEdit,SIGNAL(textChanged(QString)),this,SLOT(treeselect()));
    connect(ui->lineEdit_2,SIGNAL(textChanged(QString)),this,SLOT(treeselect()));

    connect(ui->widget->index_widget(tr("新增")), SIGNAL(clicked()),this,SLOT(tonew()));
    connect(ui->widget->index_widget(tr("删除")), SIGNAL(clicked()),this,SLOT(todel()));
    connect(ui->widget->index_widget(tr("批量新增")), SIGNAL(clicked()),this,SLOT(tonews()));
    connect(ui->widget->index_widget(tr("会员类型")), SIGNAL(clicked()),this,SLOT(tomembertype()));
    connect(ui->widget->index_widget(tr("发卡")), SIGNAL(clicked()),this,SLOT(tosendcard()));
    connect(ui->widget->index_widget(tr("充值")), SIGNAL(clicked()),this,SLOT(torecharge()));
    connect(ui->widget->index_widget(tr("设密码")), SIGNAL(clicked()),this,SLOT(tosetpwd()));
    connect(ui->widget->index_widget(tr("刷新")), SIGNAL(clicked()),this,SLOT(torefresh()));
    connect(ui->widget->index_widget(tr("导出")), SIGNAL(clicked()),this,SLOT(toexport()));
    connect(ui->widget->index_widget(tr("退出")), SIGNAL(clicked()),this,SLOT(toexit()));

    connect(ui->lineEdit,SIGNAL(textChanged(QString)),this,SLOT(treeselect()));
    connect(ui->tableView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(tosendcard()));
}

lds_tableView *w_m_member_list::getTableview()
{
    return ui->tableView;
}

lds_treeWidget *w_m_member_list::getTreew()
{
    return ui->treeWidget;
}

lds_model_sqlrelationaltablemodel *w_m_member_list::getTablemodel()
{
    return tablemodel_2;
}

QString w_m_member_list::save_submit()
{
    return tablemodel_2->submitAll()?"":tablemodel_2->lastError().text();
}

w_m_member_list::RelationalTableModel::RelationalTableModel(QObject *parent, QSqlDatabase db):lds_model_sqlrelationaltablemodel(parent, db)
{
    w_m_member::VIP_MODEL_INIT();
}

QVariant w_m_member_list::RelationalTableModel::data(const QModelIndex &item, int role) const
{
    QVariant _d=QSqlRelationalTableModel::data(item, role);
    int column=item.column();
    if(role==Qt::DisplayRole){
        if(column==4){//生日
            return _d.toDateTime().toString("yyyy-MM-dd");
        }
        if(column==9){//状态
            return  backheader::VIPFINDVALUE(w_m_member::VIP_MODEL_STATE, _d.toString());
        }

        if(column==11){//持卡
            return  backheader::VIPFINDVALUE(w_m_member::VIP_MODEL_FLAG, _d.toString());
        }
    }

    return _d;
}

void w_m_member_list::torefresh()
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

void w_m_member_list::treeselect()
{
    t2_ch_typeno.clear();
    QString filter;

    if(ui->treeWidget->currentItem()){
        int index=ui->treeWidget->deepofitem(ui->treeWidget->currentItem());
        if(index==1){
            t2_ch_typeno=backheader::getinterdata(ui->treeWidget->currentItem()->text(0));
            filter=QString("t_m_member.ch_typeno='%1'").arg(t2_ch_typeno);
        }
    }

    if(filter.isEmpty()){

    } else {
        filter += " and ";
    }

    filter += QString("(vch_memberno like '%1%' or vch_member like '%1%' )and(ifnull(vch_tel, '') like '%2%' or ifnull(vch_handtel, '') like '%2%') ")
            .arg(ui->lineEdit->text())
            .arg(ui->lineEdit_2->text())
            ;
    qDebug() << filter;
    tablemodel_2->setFilter(filter);
    tablemodel_2->select();
    tablemodel_2->relationModel(2)->select();
    ui->tableView->restore();

    qDebug() << tablemodel_2->query().lastQuery();
}

void w_m_member_list::tonew()
{
    w_m_member_list_new dialog(this, tablemodel_2, ui->tableView);
    backheader::static_dialogexec(dialog,qobject_cast<QAbstractButton* >(sender())->text());
}

void w_m_member_list::todel()
{
    int row=ui->tableView->currentIndex().row();
    if(row<0){
        lds_messagebox::warning(this, tr("消息提示"),tr("没有选中需要删除的会员!"));
        return;
    }
    QString ls_memberno=tablemodel_2->record(row).value("vch_memberno").toString();
    if(tablemodel_2->record(row).value("ch_cardflag").toString()=="Y"){
        lds_messagebox::warning(this, tr("消息提示"),tr("已经发卡会员不能删除!"));
        return;
    }

    QSqlQuery query;
    query.exec(QString("select count(1) from t_m_deposit where vch_memberno='%1'").arg(ls_memberno));
    query.next();
    if(query.record().value(0).toLongLong()>0){
        lds_messagebox::warning(this, tr("消息提示"),tr("会员已经存在金额历史，不能删除!"));
        return;
    }
    query.exec(QString("select count(1) from t_m_pay where vch_memberno='%1'").arg(ls_memberno));
    query.next();
    if(query.record().value(0).toLongLong()>0){
        lds_messagebox::warning(this, tr("消息提示"),tr("会员已经存在消费结算记录，不能删除!"));
        return;
    }

    query.exec(QString("select count(1) "+n_func::f_m_switch_product("check_data_table","1") + " where vch_voucherno ='%1' and ch_paymodeno='01'").arg(ls_memberno));
    query.next();
    if(query.record().value(0).toLongLong()>0){
        lds_messagebox::warning(this, tr("消息提示"),tr("会员已经存在消费结算记录，不能删除!"));
        return;
    }
    if(0==lds_messagebox::warning(this, tr("消息提示"),tr("确认删除该会员?"), tr("确定"), tr("取消"))){
        //! <--transaction begin-->
        QSqlDatabase::database().transaction();
        if(query.exec(QString("delete from  t_m_member where vch_memberno='%1'").arg(ls_memberno))){
            QSqlDatabase::database().commit();
            torefresh();
            lds_messagebox::warning(this, tr("消息提示"),tr("会员资料删除成功!"));
        }else {
            qDebug() << query.lastError().text();
            QSqlDatabase::database().rollback();
            torefresh();
            lds_messagebox::warning(this, tr("消息提示"),tr("会员资料删除失败!"));
        }
        //! <--transaction end-->

    }

}

void w_m_member_list::tonews()
{
    w_m_member_news dialog(this);
    backheader::static_dialogexec(dialog,qobject_cast<QAbstractButton* >(sender())->text());
}

void w_m_member_list::tomembertype()
{
    w_m_member_list_type dialog(this);
    backheader::static_dialogexec(dialog,qobject_cast<QAbstractButton* >(sender())->text());
}

void w_m_member_list::tosendcard()
{
    if(ui->tableView->currentIndex().row() >= 0)
        statictosendcard(this, tablemodel_2, ui->tableView,true);
    else
        lds_messagebox::warning(this, tr("消息提示"),tr("没有选中需要发卡的会员!"));
}

void w_m_member_list::statictosendcard(w_m_member_list *parent, QSqlRelationalTableModel *tablemodel,lds_tableView *tableview, bool isenabled, const QString &title)
{
    int row=tableview->currentIndex().row();
    if(row>=0){
        w_m_member_list_sendcard dialog(parent, tablemodel, tableview, isenabled);
        backheader::static_dialogexec(dialog,title.isEmpty()?tr("会员卡发卡"):title);
    }    else
        lds_messagebox::warning(0, tr("消息提示"),tr("没有选中需要发卡的会员!"));
}

void w_m_member_list::statictorecharge(QWidget *parent, QSqlRelationalTableModel *tablemodel, lds_tableView *tableview, const QString &parm)
{
    if(!parm.isEmpty()
            ||(parm.isEmpty()&&backheader::operid_right_code("0404"))){
        w_m_member_list_recharge dialog(parent, tablemodel, tableview,parm);
        backheader::static_dialogexec(dialog,"");
    }
}

void w_m_member_list::statictomembertype(w_m_member_list *parent)
{
    w_m_member_list_type dialog(parent);
    backheader::static_dialogexec(dialog,tr("会员类型"));

}

bool w_m_member_list::static_to_memeber_readcard(QString &cardid, uo_rw_member &uo_member, bool isyun)
{
    QString cardno;
    QString cardno_tmp1;
    QString cardno_tmp2;
    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    if(isyun
            &&conf.value("w_sys_manage_cloudsync_basedataset/yun_member").toBool()
            &&0==lds_messagebox::warning(0, tr("消息提示"), tr("检测到您已经启用云会员"),tr("云会员"), tr("本地会员"))){
        yun_member_open();//打开云会员
    }
    uo_member.of_init("4");
    QString errtext;
    //<loop1 begin>
    while(1){
        if(uo_member.is_cardtype == "1"){//ic卡
            QString icpasswd=n_func::f_m_get_parameter("vip_ic_password");
            Reader_RF reader;
            if(reader.setDefPwd(icpasswd=="FFFFFF"?"FFFFFFFFFFFF":icpasswd.toLocal8Bit().toHex())){
                if(reader.readcard(cardno, cardno_tmp1, cardno_tmp2)){
                    qDebug() << "cardno"<<cardno;
                    break;
                }
            }
            errtext=tr("读卡失败")+reader.lastError();
            break;
        }  else if(uo_member.is_cardtype=="2"){//磁卡
            public_cika_read_Dialog dialog(0);
            if(backheader::static_dialogexec(dialog, tr("读卡"))==QDialog::Accepted){
                cardno=dialog.getcardno();
                break;
            }
            //reject
            return false;
        } else {
            errtext=tr("发卡类型为 无会员卡，读卡失败!");
        }
        break;
    }
    //<loop1 end>
    if(errtext.isEmpty()){
        cardid=cardno.trimmed();
        return true;
    } else {
        lds_messagebox::warning(0, tr("消息提示"),errtext);
        return false;
    }
}

void w_m_member_list::yun_member_open()
{
    qDebug() << "------------------------------------yun_member_open";
    is_yun_member=true;
}

void w_m_member_list::yun_member_close()
{
    w_sys_manage_cloudsync::json_free();
    qDebug() << "------------------------------------yun_member_close";
    is_yun_member=false;
}

void w_m_member_list::torecharge()
{
    w_m_member_list::statictorecharge(this, tablemodel_2, ui->tableView,"");
}

void w_m_member_list::tosetpwd()
{
    if(ui->tableView->currentIndex().row() < 0){
        lds_messagebox::warning(this, tr("消息提示"),tr("没有选中需要设置密码的会员!"));
        return;
    }
    w_m_member_pwdchange dialog(this);
    QString error;
    if((error=dialog.check()).isEmpty()){
        backheader::static_dialogexec(dialog,qobject_cast<QAbstractButton* >(sender())->text());
    } else {
        lds_messagebox::warning(this, tr("消息提示"),error);
    }
}

void w_m_member_list::toexport()
{
    w_sys_manage_basicdataexport dialog(this);
    dialog.reset_export_tablenames(QStringList() << tablemodel_2->tableName());
    dialog.reset_export_confname(QString("%1/path").arg(this->metaObject()->className()));
    backheader::static_dialogexec(dialog,tr("基础数据导出"));
}

void w_m_member_list::toexit()
{
    this->reject();
}

//====================================
#include "ui_w_m_member_sendcard_dialog.h"
#include <QDataWidgetMapper>
#include <QSqlRelationalDelegate>
#include "lds_model_sqlrelationaltablemodel.h"
#include <QTimer>
w_m_member_list_sendcard::w_m_member_list_sendcard(w_m_member_list *parent, QSqlRelationalTableModel *tablemodel, lds_tableView *tableview, bool isenable):
    QDialog(parent),
    ui(new Ui_w_m_member_sendcard_Dialog),    _tablemodel(tablemodel),
    _tableview(tableview),
    _parent(parent)
{
    ui->setupUi(this);
    //! datawidget
    mapper = new QDataWidgetMapper(this);
    mapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
    mapper->setModel(_tablemodel);
    mapper->setItemDelegate(new QSqlRelationalDelegate(mapper));
    backheader::lds_com_setdatas(&w_m_member::VIP_MODEL_STATE, ui->comboBox_ch_state);
    backheader::lds_com_setdatas(&w_m_member::VIP_MODEL_FLAG, ui->comboBox_ch_cardflag);
    ui->comboBox_ch_typeno->setModel(_tablemodel->relationModel(2));
    ui->comboBox_ch_typeno->setModelColumn(1);
    mapper->addMapping(ui->comboBox_ch_cardflag, _tablemodel->fieldIndex("ch_cardflag"),"comusrdata");
    mapper->addMapping(ui->comboBox_ch_state, _tablemodel->fieldIndex("ch_state"), "comusrdata");
    mapper->addMapping(ui->lineEdit_vch_address, _tablemodel->fieldIndex("vch_address"));
    mapper->addMapping(ui->lineEdit_vch_cardno, _tablemodel->fieldIndex("vch_cardno"));
    mapper->addMapping(ui->lineEdit_vch_handtel, _tablemodel->fieldIndex("vch_handtel"));
    mapper->addMapping(ui->lineEdit_vch_id, _tablemodel->fieldIndex("vch_id"));
    mapper->addMapping(ui->lineEdit_vch_member, _tablemodel->fieldIndex("vch_member"));
    mapper->addMapping(ui->lineEdit_vch_memberno, _tablemodel->fieldIndex("vch_memberno"));
    mapper->addMapping(ui->lineEdit_vch_operid, _tablemodel->fieldIndex("vch_operid"));
    mapper->addMapping(ui->lineEdit_vch_tel, _tablemodel->fieldIndex("vch_tel"));
    mapper->addMapping(ui->dateTimeEdit_dt_birthday_show,_tablemodel->fieldIndex("dt_birthday"));
    mapper->addMapping(ui->dateTimeEdit_dt_limit_show,_tablemodel->fieldIndex("dt_limit"));
    mapper->addMapping(ui->dateTimeEdit_dt_operdate_show,_tablemodel->fieldIndex("dt_operdate"));
    mapper->addMapping(ui->comboBox_ch_typeno, _tablemodel->fieldIndex("vch_typename"));
    //! tableview_other
    tablemodel_2=new lds_model_sqlrelationaltablemodel;
    tablemodel_2->setEditStrategy(QSqlTableModel::OnManualSubmit);
    tablemodel_2->setTable("t_m_member_other");
    ui->tableView_other->setModel(tablemodel_2->fpmodel);
    ui->tableView_other->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::SelectedClicked);
    tablemodel_2->save_set_header("vch_item", tr("项目"), true);
    tablemodel_2->save_set_header("vch_memo", tr("内容"), true);
    ui->tableView_other->rePecifyHeader(tablemodel_2->save_set_header_saver);

    //! tableview_log
    tablemodel_3=new lds_model_sqlrelationaltablemodel;
    tablemodel_3->setTable("t_m_member_log");
    ui->tableView_log->setModel(tablemodel_3->fpmodel);
    tablemodel_3->save_set_header("vch_memberno", tr("会员编号"));
    tablemodel_3->save_set_header("vch_matter", tr("项目"));
    tablemodel_3->save_set_header("vch_memo", tr("内容"));
    tablemodel_3->save_set_header("vch_operID", tr("操作人员"));
    tablemodel_3->save_set_header("dt_operdate", tr("操作时间"));
    ui->tableView_log->rePecifyHeader(tablemodel_3->save_set_header_saver);
    //! toolbar
    ui->widget_2->setup(QStringList()<<tr("新增")<<tr("删除"), Qt::AlignTop);
    ui->widget->setup(QStringList()<<tr("发卡")<<tr("上一行")<<tr("下一行")<<tr("保存")<<tr("退出"), Qt::AlignRight);
    connect(ui->widget_2->index_widget(tr("新增")),SIGNAL(clicked()),this,SLOT(tonew()));
    connect(ui->widget_2->index_widget(tr("删除")),SIGNAL(clicked()),this,SLOT(todel()));
    connect(ui->widget->index_widget(tr("发卡")),SIGNAL(clicked()),this,SLOT(tosendcard()));
    connect(ui->widget->index_widget(tr("上一行")),SIGNAL(clicked()),this,SLOT(toup()));
    connect(ui->widget->index_widget(tr("下一行")),SIGNAL(clicked()),this,SLOT(todown()));
    connect(ui->widget->index_widget(tr("保存")),SIGNAL(clicked()),this,SLOT(tosave()));
    connect(ui->widget->index_widget(tr("退出")),SIGNAL(clicked()),this,SLOT(toexit()));

    //    connect(ui->lineEdit_ch_cardflag_hide,SIGNAL(textChanged(QString)),this,SLOT(tocomboBox_ch_cardflag(QString)));
    //    connect(ui->comboBox_ch_cardflag,SIGNAL(currentIndexChanged(QString)),this,SLOT(tolineEdit_ch_cardflag_hide(QString)));
    //    connect(ui->lineEdit_ch_state_hide,SIGNAL(textChanged(QString)),this,SLOT(tocomboBox_ch_state(QString)));
    //    connect(ui->comboBox_ch_state,SIGNAL(currentIndexChanged(QString)),this,SLOT(tolineEdit_ch_state_hide(QString)));
    //! after
    //获取is_cardtype的内容
    QString ls_value=n_func::f_m_get_parameter("vip_card_type", tr("无会员卡"));
    if(ls_value==tr("IC卡")){
        is_cardtype="1";
    } else if(ls_value==tr("磁卡")){
        is_cardtype="2";
    } else {//无会员
        is_cardtype="4";
    }
    //只是查看详细信息的
    if(!isenable){
        ui->frame_baseinfo->setEnabled(isenable);
        ui->widget_2->setEnabled(isenable);
        ui->widget->index_widget(tr("发卡"))->setVisible(isenable);
        ui->widget->index_widget(tr("保存"))->setVisible(isenable);
    }
    //初始化数据
    cb5=new uo_rw_member;
    cb5->of_init("2");//2是磁卡
    ui->comboBox_ch_cardflag->setEnabled(false);
    ui->comboBox_ch_state->setEnabled(false);
    ui->lineEdit_vch_operid->setEnabled(false);
    ui->dateTimeEdit_dt_operdate_show->setEnabled(false);

    ui->lineEdit_vch_memberno->setEnabled(false);
    ui->comboBox_ch_typeno->setEnabled(false);
    //更新当前index
    ui->label_record->setText(QString("%1/%2").arg(_tableview->currentIndex().row()+1).arg(_tableview->verticalHeader()->count()));

    //设置datetimelineedit为只有日期
    ui->dateTimeEdit_dt_birthday_show->setDisplayFormat("yyyy-MM-dd");
    ui->dateTimeEdit_dt_limit_show->setDisplayFormat("yyyy-MM-dd");
    ui->dateTimeEdit_dt_operdate_show->setDisplayFormat("yyyy-MM-dd");
    //! TIMESLOT
    QTimer::singleShot(0, this, SLOT(torefresh()));//信号槽的重写是需要这的写的，实现subclass 同时继承

}

void w_m_member_list_sendcard::tonew()
{
    int rowcount=tablemodel_2->rowCount();
    tablemodel_2->insertRow(rowcount);
    tablemodel_2->setData(tablemodel_2->index(rowcount, tablemodel_2->fieldIndex("vch_memberno")), ui->lineEdit_vch_memberno->text());
    tablemodel_2->setData(tablemodel_2->index(rowcount, tablemodel_2->fieldIndex("vch_operid")), n_func::gs_operid);
}

void w_m_member_list_sendcard::todel()
{
    int row=ui->tableView_other->currentIndex().row();
    if(row<0){
        lds_messagebox::warning(this, tr("消息提示"),tr("请选择将要删除的记录!"));
        return;
    }
    tablemodel_2->removeRow(row);
    if(tablemodel_2->submitAll()){
        torefresh();
        lds_messagebox::warning(this, tr("消息提示"),tr("删除成功!"));
        return;
    }
    lds_messagebox::warning(this, tr("消息提示"),tr("数据库错误!"), tablemodel_2->lastError().text());
}

void w_m_member_list_sendcard::tosendcard()
{
    //clicked (none) returns long [pbm_bnclicked]
    //long ancestorreturnvalue
    QString ls_memberno;
    QString ls_cardflag;
    QString ls_state;
    QString ls_cardno;
    QString ls_err;
    long ll_exist;
    QSqlQuery query;
    ls_memberno = ui->lineEdit_vch_memberno->text();//PARENT.idw_main.object.vch_memberno[1]
    ls_cardflag = w_m_member::VIP_MODEL_FLAG.item(ui->comboBox_ch_cardflag->currentIndex(), 0)->text();//ui->lineEdit_ch_cardflag_hide->text();//PARENT.idw_main.object.ch_cardflag[1]
    ls_state = w_m_member::VIP_MODEL_STATE.item(ui->comboBox_ch_state->currentIndex(), 0)->text();//ui->lineEdit_ch_state_hide->text();//PARENT.idw_main.object.ch_state[1]
    ls_cardno = ui->lineEdit_vch_cardno->text();//PARENT.idw_main.object.vch_cardno[1]

    if(ls_cardno.trimmed().isEmpty()){
        lds_messagebox::warning(this, tr("消息提示"),tr("卡面卡号不能为空！"));
        return;
    }
    query.prepare("Select Count(*) from t_m_member  where vch_cardno =:ls_cardno and vch_memberno <> :ls_memberno");
    query.bindValue(":ls_cardno", ls_cardno);
    query.bindValue(":ls_memberno", ls_memberno);
    query.exec();
    query.next();
    ll_exist=query.record().value(0).toLongLong();
    if(ll_exist > 0){
        lds_messagebox::warning(this, tr("消息提示"),tr("输入的卡面卡号系统已存在，不能保存!"));
        return;
    }

    //! <--transaction begin-->
    QSqlDatabase::database().transaction();
    switch(0){
    case 0:
        query.prepare("update t_m_member set vch_cardno =:ls_cardno where vch_memberno =:ls_memberno");
        query.bindValue(":ls_cardno", ls_cardno);
        query.bindValue(":ls_memberno", ls_memberno);
        if(!query.exec()){
            qDebug() << query.lastError();
            ls_err=tr("会员") + ls_memberno + tr("发卡失败");
            break;
        }
        if(ls_state=="4"){
            ls_err=tr("此会员已是作废状态，不能进行相应操作!");
            break;
        } else if(ls_cardflag=="Y"){
            ls_err=tr("此会员已发卡~r~nIC卡可清除卡芯片再重发，非IC卡只能进行卡状态维护!");
            break;
        } else {
            if((ls_err=cb5->of_send_card(ls_memberno)).isEmpty()){
                if(!(ls_err=n_func::f_m_insert_log(ls_memberno,tr("发卡"),"",0)).isEmpty()){
                    break;
                }
            }
        }
    }
    if(ls_err.isEmpty()){
        QSqlDatabase::database().commit();
        torefresh();
        lds_messagebox::warning(this, tr("消息提示"),tr("发卡成功!"));
    } else {
        QSqlDatabase::database().rollback();
        torefresh();
        lds_messagebox::warning(this, tr("消息提示"),ls_err);
    }
    //! <--transaction end-->
}

void w_m_member_list_sendcard::toup()
{
    _tableview->goprevious();
    mapper->setCurrentIndex(_tableview->currentIndex().row());
    torefresh();
    ui->label_record->setText(QString("%1/%2").arg(_tableview->currentIndex().row()+1).arg(_tableview->verticalHeader()->count()));
}

void w_m_member_list_sendcard::todown()
{
    _tableview->gonext();
    mapper->setCurrentIndex(_tableview->currentIndex().row());
    torefresh();
    ui->label_record->setText(QString("%1/%2").arg(_tableview->currentIndex().row()+1).arg(_tableview->verticalHeader()->count()));
}

bool w_m_member_list_sendcard::tosave()
{
    QDateTime ldt;
    QDateTime ldt_old;
    QDateTime ldt_limit;
    QDateTime ldt_birth;
    QString ls_errtext;
    long ll_exist;
    QString ls_memberno;
    QString ls_membername;
    QString ls_typeno;
    QString ls_attribute;
    QString ls_cardno;
    QSqlQuery query;

    ls_memberno=ui->lineEdit_vch_memberno->text();
    ls_membername=ui->lineEdit_vch_member->text();
    ls_typeno=ui->comboBox_ch_typeno->column2index1();
    ls_cardno=ui->lineEdit_vch_cardno->text();
    if(ls_memberno.trimmed().isEmpty()){
        ls_errtext=tr("会员编号");
    } else if(ls_membername.trimmed().isEmpty()){
        ls_errtext=tr("会员名称");
    } else if(ls_typeno.trimmed().isEmpty()){
        ls_errtext=tr("会员类型");
    } else if(ls_cardno.trimmed().isEmpty()){
        if(is_cardtype=="1"){
            ls_errtext="";
        } else {
            ls_errtext=tr("卡面卡号");
        }
    }

    switch(0){//switch
    case 0://0
        if(!ls_errtext.isEmpty()){
            ls_errtext=ls_errtext+tr("不能为空!");
            break;
        }//0
    case 1://1
        ldt_limit=ui->dateTimeEdit_dt_limit_show->dateTime();
        if(ldt_limit < QDateTime::currentDateTime()){
            ls_errtext=ls_errtext+tr("有效期限不能小于当前日期!");
            break;
        }//1
    case 2://2
        ldt_birth=ui->dateTimeEdit_dt_birthday_show->dateTime();
        if(ldt_birth < QDateTime::fromString("1800-01-01", "yyyy-MM-dd")){
            ls_errtext=ls_errtext+tr("系统不支持'超年龄'会员，请核实会员生日必须在1800年后!");
            break;
        }//2
    case 3:{//3
        ll_exist=0;
        query.exec(QString("select count(*) from t_m_member where vch_cardno='%1' and vch_memberno <>'%2'").arg(ls_cardno).arg(ls_memberno));
        query.next();

        if(query.record().value(0).toLongLong() > 0){
            ls_errtext=ls_errtext+tr("输入卡面卡号系统已存在，不能保存!");
            break;
        }

    }//3
    case 4://4
        ldt=n_func::f_get_sysdatetime();
        ldt=ui->dateTimeEdit_dt_operdate_show->dateTime();
        if(ldt_old.isValid() && ldt_old > ldt){
            ls_errtext=ls_errtext+tr("系统时间小于上次保存时间,请确认系统时间是否正确!");
            break;
        }
        ui->dateTimeEdit_dt_operdate_show->setDateTime(ldt);
        //4
    case 5://5
        if(!tablemodel_2->submitAll()){
            ls_errtext=tr("数据库失败!");
            break;
        }

        if(!tablemodel_3->submitAll()){
            ls_errtext=tr("数据库失败!");
            break;
        }
        //5
    case 6:
        ls_errtext.clear();
        if(!(ls_errtext=_tablemodel->submitAll()?"":_tablemodel->lastError().text()).isEmpty()){
            break;
        }
        query.exec(QString("select ch_attribute from t_m_member_type where ch_typeno='%1'").arg(ls_typeno));
        query.next();
        ls_attribute=query.record().value(0).toString();
        if(ls_attribute=="3"){
            ls_errtext=n_func::f_m_hang_curamount(ls_memberno);
            if(!ls_errtext.isEmpty()){
                break;
            }
        }
        if(!tablemodel_2->submitAll()){
            ls_attribute=tablemodel_2->lastError().text();
            break;
        }
        if(!tablemodel_3->submitAll()){
            ls_attribute=tablemodel_3->lastError().text();
            break;
        }
    case 7:break;
    }//switch

    //刷新
    torefresh();
    //错误信息
    if(ls_errtext.isEmpty()){//! success
        lds_messagebox::warning(this, tr("消息提示"),tr("会员资料保存成功!"));
    } else {//! failure
        lds_messagebox::warning(this, tr("消息提示"),ls_errtext);
    }
}

void w_m_member_list_sendcard::toexit()
{
    this->reject();
}

void w_m_member_list_sendcard::torefresh()
{
    qDebug() << "void w_m_member_list_sendcard::torefresh()";

    //_parent->refresh();
    _tablemodel->select();
    _tablemodel->relationModel(2)->select();
    _tableview->restore();

    //mapper
    mapper->setCurrentIndex(_tableview->currentIndex().row());

    //tableView_other
    QString memberno=_tablemodel->record(_tableview->currentIndex().row()).value("vch_memberno").toString();
    tablemodel_2->setFilter(QString("t_m_member_other.vch_memberno='%1'").arg(memberno));
    tablemodel_2->select();
    ui->tableView_other->restore();

    //tableView_log
    tablemodel_3->select();
    tablemodel_3->setFilter(QString("t_m_member_log.vch_memberno='%1'").arg(memberno));
    ui->tableView_log->restore();
}

//===================================
w_m_member_list_new::w_m_member_list_new(w_m_member_list *parent,QSqlRelationalTableModel *tablemodel,lds_tableView *tableview, bool isnew):
    w_m_member_list_sendcard(parent, tablemodel, tableview, isnew)
{
    ui->tabWidget->setObjectName("tab");
    ui->widget->index_widget(tr("发卡"))->setVisible(false);
    ui->widget->index_widget(tr("上一行"))->setVisible(false);
    ui->widget->index_widget(tr("下一行"))->setVisible(false);
    ui->lineEdit_vch_memberno->setEnabled(true);
    ui->comboBox_ch_typeno->setEnabled(true);
    ui->label_record->setVisible(false);
    //    Removes the tab at position index from this stack of widgets. The page widget itself is not deleted.
    ui->tabWidget->removeTab(1);
    ui->tabWidget->removeTab(1);
    ui->dateTimeEdit_dt_limit_show->setDateTime(QDateTime::currentDateTime().addYears(1));
    ui->dateTimeEdit_dt_operdate_show->setDateTime(QDateTime::currentDateTime());
    ui->lineEdit_vch_operid->setText(n_func::gs_operid);
}

bool w_m_member_list_new::tosave()
{

    QDateTime ldt;
    QDateTime ldt_old;
    QDateTime ldt_limit;
    QDateTime ldt_birth;
    QString ls_errtext;
    long ll_exist;
    QString ls_memberno;
    QString ls_membername;
    QString ls_typeno;
    QString ls_attribute;
    QString ls_cardno;
    QSqlQuery query;

    ls_memberno=ui->lineEdit_vch_memberno->text();
    ls_membername=ui->lineEdit_vch_member->text();
    ls_typeno=ui->comboBox_ch_typeno->column2index1();
    ls_cardno=ui->lineEdit_vch_cardno->text();
    if(ls_memberno.trimmed().isEmpty()){
        ls_errtext=tr("会员编号");
    } else if(ls_membername.trimmed().isEmpty()){
        ls_errtext=tr("会员名称");
    } else if(ls_typeno.trimmed().isEmpty()){
        ls_errtext=tr("会员类型");
    } else if(ls_cardno.trimmed().isEmpty()){
        if(is_cardtype=="1"){
            ls_errtext="";
        } else {
            ls_errtext=tr("卡面卡号");
        }
    }

    switch(0){//switch
    case 0://0
        query.exec(QString("select count(1) from t_m_member where vch_memberno='%1'").arg(ls_memberno));
        query.next();
        if(query.record().value(0).toLongLong() > 0) {
            ls_errtext=tr("编号已经存在，请输入新的会员编号!");
            break;
        }
        if(!ls_errtext.isEmpty()){
            ls_errtext=ls_errtext+tr("不能为空!");
            break;
        }//0
    case 1://1
        ldt_limit=ui->dateTimeEdit_dt_limit_show->dateTime();
        if(ldt_limit < QDateTime::currentDateTime()){
            ls_errtext=ls_errtext+tr("有效期限不能小于当前日期!");
            break;
        }//1
    case 2://2
        ldt_birth=ui->dateTimeEdit_dt_birthday_show->dateTime();
        if(ldt_birth < QDateTime::fromString("1800-01-01", "yyyy-MM-dd")){
            ls_errtext=ls_errtext+tr("系统不支持'超年龄'会员，请核实会员生日必须在1800年后!");
            break;
        }//2
    case 3:{//3
        ll_exist=0;
        query.exec(QString("select count(*) from t_m_member where vch_cardno='%1' and vch_memberno <>'%2'").arg(ls_cardno).arg(ls_memberno));
        query.next();

        if(query.record().value(0).toLongLong() > 0){
            ls_errtext=ls_errtext+tr("输入卡面卡号系统已存在，不能保存!");
            break;
        }

    }//3
    case 4://4
        //4
    case 5://5
        if(!tablemodel_2->submitAll()){
            ls_errtext=tr("数据库失败!");
            qDebug() << 50;

            break;
        }
        if(!tablemodel_3->submitAll()){
            ls_errtext=tr("数据库失败!");
            qDebug() << 51;

            break;
        }
        //5
    case 6:
        //! <--transaction begin-->
        QSqlDatabase::database().transaction();
        ls_errtext.clear();
        query.prepare(QString("INSERT INTO t_m_member(vch_memberno, vch_member, ch_typeno, dt_limit, num_limit, vch_operID, dt_operdate, vch_cardno)VALUES(?,?,?,?,?,?,?,?)"));
        query.addBindValue(ls_memberno);
        query.addBindValue(ls_membername);
        query.addBindValue(ls_typeno);
        query.addBindValue(ldt_limit);
        query.addBindValue(0/*num_limit*/);
        query.addBindValue(n_func::gs_operid);
        query.addBindValue(ui->dateTimeEdit_dt_operdate_show->dateTime());
        query.addBindValue(ls_cardno);
        if(!query.exec()){
            ls_errtext=query.lastError().text();
            qDebug() << 6;
            QSqlDatabase::database().rollback();
            break;
        }
        query.exec(QString("select ch_attribute from t_m_member_type where ch_typeno='%1'").arg(ls_typeno));
        query.next();
        ls_attribute=query.record().value(0).toString();
        if(ls_attribute=="3"){
            ls_errtext=n_func::f_m_hang_curamount(ls_memberno);
            if(!ls_errtext.isEmpty()){
                QSqlDatabase::database().rollback();
                break;
            }
        }
        //! success
        QSqlDatabase::database().commit();
        //! <--transaction end-->
    case 7:break;
    }//switch

    //刷新
    torefresh();
    //错误信息
    if(ls_errtext.isEmpty()){//! success
        lds_messagebox::warning(this, tr("消息提示"),tr("会员资料保存成功!"));
        this->accept();
    } else {//! failure
        lds_messagebox::warning(this, tr("消息提示"),ls_errtext);
    }
}

void w_m_member_list_new::torefresh()
{
    //_parent->refresh();
    _tablemodel->select();
    _tablemodel->relationModel(2)->select();
    _tableview->restore();

    //mapper
    mapper->setCurrentIndex(-1);
    ui->comboBox_ch_typeno->setCurrentIndex(_parent->getTreew()->get_0_Index());
    //tableView_other
    QString memberno=_tablemodel->record(_tableview->currentIndex().row()).value("vch_memberno").toString();
    tablemodel_2->setFilter(QString("t_m_member_other.vch_memberno='%1'").arg(memberno));
    tablemodel_2->select();
    ui->tableView_other->restore();

    //tableView_log
    tablemodel_3->select();
    ui->tableView_log->restore();

}


//====================================
#include "ui_w_bt_dish_unitset_dialog.h"
#include "lds_model_sqlrelationaltablemodel.h"
#include "lds_tableview_repecify.h"
#include <QtDebug>



QVariant w_m_member_list_type::TableModel::data(const QModelIndex &idx, int role) const
{
    QVariant _d = lds_model_sqlrelationaltablemodel::data(idx, role);
    if(role == Qt::DisplayRole){
        qDebug() << idx.column() << _d.toString();

        if(idx.column() == 2){//会员卡种类
            return backheader::VIPFINDVALUE(w_m_member::VIP_MODEL_ATTRI, _d.toString());
        }
        if(idx.column() == 3){//优惠方式
            return backheader::VIPFINDVALUE(w_m_member::VIP_MODEL_FAVOUR, _d.toString());

        }
        if(idx.column() == 7){//是否积分
            return backheader::VIPFINDVALUE(w_m_member::VIP_MODEL_CALC, _d.toString());
        }
    }

    return _d;
}

Qt::ItemFlags w_m_member_list_type::TableModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

w_m_member_list_type::w_m_member_list_type(w_m_member_list *parent):
    QDialog(parent),ui(new Ui_w_bt_dish_unitset_Dialog),
    _parent(parent)
{
    ui->setupUi(this);
    //! tableview
    tablemodel=new TableModel;
    tablemodel->setTable("t_m_member_type");
    tablemodel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::SelectedClicked);ui->tableView->setModel(tablemodel->fpmodel);

    //!refresh
    torefresh();

    //! toolbar
    ui->widget->setup(QStringList()<<tr("新增")<<tr("修改")<<tr("删除")<<tr("刷新")<<tr("退出"), Qt::AlignRight);
    connect(ui->widget->index_widget(tr("新增")),SIGNAL(clicked()),this,SLOT(tonew()));
    connect(ui->widget->index_widget(tr("修改")),SIGNAL(clicked()),this,SLOT(tochange()));
    connect(ui->widget->index_widget(tr("删除")),SIGNAL(clicked()),this,SLOT(todel()));
    connect(ui->widget->index_widget(tr("刷新")),SIGNAL(clicked()),this,SLOT(torefresh()));
    connect(ui->widget->index_widget(tr("退出")),SIGNAL(clicked()),this,SLOT(toexit()));
    //    connect(ui->tableView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(tochange()));

    //! after
    tablemodel->setHeaderData(0,Qt::Horizontal,lds_tableview_repecify::saveHeader(tr("类型编码"),headerstmp));
    tablemodel->setHeaderData(1,Qt::Horizontal,lds_tableview_repecify::saveHeader(tr("类型名称"),headerstmp));
    tablemodel->setHeaderData(2,Qt::Horizontal,lds_tableview_repecify::saveHeader(tr("会员卡种类"),headerstmp));
    tablemodel->setHeaderData(3,Qt::Horizontal,lds_tableview_repecify::saveHeader(tr("优惠方式"),headerstmp));
    tablemodel->setHeaderData(7,Qt::Horizontal,lds_tableview_repecify::saveHeader(tr("是否积分"),headerstmp));

    ui->tableView->rePecifyHeader( headerstmp);

}

lds_model_sqlrelationaltablemodel *w_m_member_list_type::getTablemodel()
{
    return tablemodel;
}

lds_tableView *w_m_member_list_type::getTableView()
{
    return ui->tableView;
}

void w_m_member_list_type::tonew()
{
    w_m_member_list_type_new dialog(this);
    backheader::static_dialogexec(dialog,qobject_cast<QAbstractButton* >(sender())->text());
}

void w_m_member_list_type::todel()
{
    int row=ui->tableView->currentIndex().row();
    if(row>=0){
        if(0==lds_messagebox::warning(this, tr("消息提示"),tr("确认删除?"), tr("确认"),tr("取消"))){
            QSqlQuery query;
            query.exec(QString("select count(*) from t_m_member where ch_typeno='%1'").arg(tablemodel->record(row).value("ch_typeno").toString()));
            query.next();
            if(query.record().value(0).toLongLong() > 0){
                lds_messagebox::warning(this, tr("消息提示"),tr("会员类型已经使用,不能删除!"));
                return;
            }
            tablemodel->removeRow(row);
            if(tablemodel->submitAll()){
                ui->tableView->restore();
            } else {
                lds_messagebox::warning(this, tr("消息提示"),tr("数据库错误!"));
            }
        }
    } else {
        lds_messagebox::warning(this, tr("消息提示"),tr("没有所选行!"));
    }
}

bool w_m_member_list_type::tosave()
{
}

void w_m_member_list_type::torefresh()
{
    tablemodel->select();
    ui->tableView->restore();
}

void w_m_member_list_type::toexit()
{
    if(_parent)_parent->torefresh();
    this->reject();
}

void w_m_member_list_type::tochange()
{
    w_m_member_list_type_change dialog(this);
    backheader::static_dialogexec(dialog,qobject_cast<QAbstractButton* >(sender())->text());
}
//===================================
#include "ui_w_m_member_list_type_new_dialog.h"
w_m_member_list_type_change::w_m_member_list_type_change(w_m_member_list_type *parent):
    QDialog(parent),
    ui(new Ui_w_m_member_list_type_new_Dialog),
    _parent(parent)
{
    ui->setupUi(this);
    backheader::lds_com_setdatas(&w_m_member::VIP_MODEL_AREA, ui->comboBox_ch_area_flag);
    backheader::lds_com_setdatas(&w_m_member::VIP_MODEL_ATTRI, ui->comboBox_ch_attribute);
    backheader::lds_com_setdatas(&w_m_member::VIP_MODEL_CALC, ui->comboBox_ch_calc);
    backheader::lds_com_setdatas(&w_m_member::VIP_MODEL_CALCTYPE, ui->comboBox_ch_calc_type);
    backheader::lds_com_setdatas(&w_m_member::VIP_MODEL_FAVOUR, ui->comboBox_ch_favourable);

    connect(ui->comboBox_ch_attribute,SIGNAL(currentIndexChanged(QString)),this,SLOT(is_recharge(QString)));
    connect(ui->comboBox_ch_calc,SIGNAL(currentIndexChanged(QString)),this,SLOT(is_point(QString)));


    mapper = new QDataWidgetMapper(this);
    mapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
    mapper->setModel(_parent->tablemodel);
    mapper->setItemDelegate(new mappdelegate(mapper));

    mapper->addMapping(ui->lineEdit_int_backcash_rate, _parent->tablemodel->fieldIndex("int_backcash_rate"));
    mapper->addMapping(ui->lineEdit_ch_typeno, _parent->tablemodel->fieldIndex("ch_typeno"));
    mapper->addMapping(ui->lineEdit_int_calc_unit, _parent->tablemodel->fieldIndex("int_calc_unit"));
    mapper->addMapping(ui->lineEdit_int_card_baseamount, _parent->tablemodel->fieldIndex("int_card_baseamount"));
    mapper->addMapping(ui->lineEdit_int_discount, _parent->tablemodel->fieldIndex("int_discount"));
    mapper->addMapping(ui->lineEdit_vch_typename, _parent->tablemodel->fieldIndex("vch_typename"));

    mapper->addMapping(ui->comboBox_ch_area_flag, _parent->tablemodel->fieldIndex("ch_area_flag"), "comusrdata");
    mapper->addMapping(ui->comboBox_ch_attribute, _parent->tablemodel->fieldIndex("ch_attribute"), "comusrdata");
    mapper->addMapping(ui->comboBox_ch_calc, _parent->tablemodel->fieldIndex("ch_calc"), "comusrdata");
    mapper->addMapping(ui->comboBox_ch_calc_type, _parent->tablemodel->fieldIndex("ch_calc_type"), "comusrdata");
    mapper->addMapping(ui->comboBox_ch_favourable, _parent->tablemodel->fieldIndex("ch_favourable"), "comusrdata");

    //! toolbar
    ui->widget->setup(QStringList()<<tr("保存")<<tr("退出"),Qt::AlignRight);
    connect(ui->widget->index_widget(tr("保存")), SIGNAL(clicked()),this,SLOT(tosave()));
    connect(ui->widget->index_widget(tr("退出")), SIGNAL(clicked()),this,SLOT(toexit()));

    ui->lineEdit_ch_typeno->setEnabled(false);
    ui->comboBox_ch_attribute->setEnabled(false);
    QTimer::singleShot(0, this,SLOT(mapperinit()));
}

bool w_m_member_list_type_change::tosave()
{
    if(_parent->tablemodel->submitAll()){
        lds_messagebox::warning(this, tr("消息提示"),tr("保存成功"));
        _parent->ui->tableView->restore();
        this->accept();
        return true;
    } else {
        lds_messagebox::warning(this, tr("消息提示"),tr("数据有误，操作失败"), _parent->tablemodel->lastError().text());
        return false;
    }
}

void w_m_member_list_type_change::toexit()
{
    _parent->tablemodel->select();
    _parent->ui->tableView->restore();
    this->reject();
}

void w_m_member_list_type_change::is_recharge(const QString &text)
{
    ui->frame_1->setEnabled(text==tr("充值卡"));
}

void w_m_member_list_type_change::is_point(const QString &text)
{
    ui->frame_3->setEnabled(text==tr("积分"));
}

void w_m_member_list_type_change::mapperinit()
{
    mapper->setCurrentIndex(_parent->ui->tableView->currentIndex().row());
}

//===================================
w_m_member_list_type_new::w_m_member_list_type_new(w_m_member_list_type *parent):
    w_m_member_list_type_change(parent)
{
    ui->lineEdit_ch_typeno->setEnabled(true);
    ui->comboBox_ch_attribute->setEnabled(true);

}

bool w_m_member_list_type_new::tosave()
{
    QString ls_memeber_typeno=ui->lineEdit_ch_typeno->text();
    QString ls_member_name=ui->lineEdit_vch_typename->text();
    QString ls_mess;
    QString ls_attribute=ui->comboBox_ch_attribute->currentText();
    switch(0){
    case 0:
        if(ls_memeber_typeno.isEmpty()){
            ls_mess=tr("类型编号不能为空!");
            qDebug() << ls_memeber_typeno;
            break;
        }
        if(ls_member_name.isEmpty()){
            ls_mess=tr("会员卡类型名称不能为空!");
            qDebug() << ls_member_name;
            break;
        }
        if(ls_attribute.isEmpty()){
            ls_mess=tr("会员种类不能为空!");
            qDebug() << ls_attribute;
            break;
        }
    case 1:
        if(ui->lineEdit_int_backcash_rate->text().toInt() < 0){
            ls_mess=tr("充值卡付款还现比率不能小于0！");
            break;
        }
        if(ui->lineEdit_int_backcash_rate->text().toInt() >= 100){
            ls_mess=tr("充值卡付款还现比率不能大于等于100！");
            break;
        }
        if(ui->lineEdit_int_card_baseamount->text().toInt() < 0){
            ls_mess=tr("充值卡保本金额不能小于0!");
            break;
        }
        if(ui->lineEdit_int_discount->text().toInt() < 0){
            ls_mess=tr("固定折扣不能小于0!");
            break;
        }
        if(ui->lineEdit_int_discount->text().toInt() >100){
            ls_mess=tr("固定折扣不能大于100!");
            break;
        }
    case 2:{
        QSqlQuery query;
        query.exec(QString("select count(*) from t_m_member_type where ch_typeno = '%1' ")
                   .arg(ls_memeber_typeno));
        query.next();
        if(query.record().value(0).toLongLong() > 0){
            ls_mess=tr("编号已经存在,请输入新的会员类型编号!");
            break;
        }
        //! <--transaction begin-->
        QSqlDatabase::database().transaction();
        query.prepare(QString("insert into  t_m_member_type  (ch_typeno, vch_typename, ch_attribute, int_discount,   ch_calc, ch_calc_type, int_calc_unit, ch_pricekind,ch_area_flag,ch_favourable, int_card_baseamount,int_backcash_rate  )values(?,?,?,?,?,?,?,?,?,?,?,?)"));
        query.addBindValue(ui->lineEdit_ch_typeno->text());//会员类型号
        query.addBindValue(ui->lineEdit_vch_typename->text());//会员类型名
        query.addBindValue(ui->comboBox_ch_attribute->comusrdata());//会员类型
        query.addBindValue(ui->lineEdit_int_discount->text());//固定折扣比值
        query.addBindValue(ui->comboBox_ch_calc->comusrdata());//是否积分
        query.addBindValue(ui->comboBox_ch_calc_type->comusrdata());//积分是否取整
        query.addBindValue(ui->lineEdit_int_calc_unit->text());//积分单元
        query.addBindValue(1/*ch_pricekind*/);
        query.addBindValue(ui->comboBox_ch_area_flag->comusrdata());//消费积分范围
        query.addBindValue(ui->comboBox_ch_favourable->comusrdata());//优惠方式
        query.addBindValue(ui->lineEdit_int_card_baseamount->text());//保本金额
        query.addBindValue(ui->lineEdit_int_backcash_rate->text());//充值卡付款还现
        qDebug() << query.boundValues();
        if(query.exec()){
            //! <--transaction commit-->
            QSqlDatabase::database().commit();
            _parent->tablemodel->select();
            lds_messagebox::warning(this, tr("消息提示"),tr("保存成功!"));
            this->accept();
            return true;
        }
        ls_mess=query.lastError().text();
    }
        break;
    }
    //! <--transaction commit-->
    QSqlDatabase::database().rollback();
    _parent->tablemodel->select();
    lds_messagebox::warning(this, tr("消息提示"),ls_mess);
    return false;
}

void w_m_member_list_type_new::toexit()
{
    this->reject();
}

void w_m_member_list_type_new::mapperinit()
{
    is_recharge(ui->comboBox_ch_attribute->currentText());
    is_point(ui->comboBox_ch_calc->currentText());
}

//===================================
#include "ui_w_m_member_list_recharge_dialog.h"
w_m_member_list_recharge::w_m_member_list_recharge(QWidget *parent, QSqlRelationalTableModel *tablemodel, lds_tableView *tableview, const QString &parm):
    QDialog(parent),
    ui(new Ui_w_m_member_list_recharge_Dialog),
    _isready(false),
    _tablemodel(tablemodel),
    _tableview(tableview),
    _parm(parm)

{
    ui->setupUi(this);

    bckey=tr("充值");
    //! init
    this->setWindowTitle(tr("会员卡充值"));
    if(_parm=="C"){
        ui->lineEdit_num_realamount->setVisible(false);
        ui->label_num_realamount_t->setVisible(false);
        ui->label_num_deposit_t->setText(tr("取款金额"));
        ui->label_amount_t->setText(tr("取后面额"));
        ui->label_operdate_t->setText(tr("取款时间"));
        bckey=tr("取款");
        this->setWindowTitle(tr("会员卡取款"));
    } else if(_parm=="D"){
        ui->lineEdit_num_realamount->setVisible(false);
        ui->label_num_realamount_t->setVisible(false);
        ui->label_num_deposit_t->setText(tr("退卡金额"));
        ui->label_amount_t->setText(tr("退后面额"));
        ui->label_operdate_t->setText(tr("退款时间"));
        bckey=tr("退卡");
        this->setWindowTitle(tr("会员卡退卡"));
    }
    backheader::lds_com_setdatas(&w_m_member::RECHARGE_MODEL_PAYMODE, ui->comboBox_ch_pay_mode);

    //! toolbar
    ui->widget->setup(QStringList()<<tr("余额打印")<<tr("读卡")<<bckey<<tr("退出"),Qt::AlignRight);
    connect(ui->widget->index_widget(tr("余额打印")), SIGNAL(clicked()),this,SLOT(toremainprint()));
    connect(ui->widget->index_widget(tr("读卡")), SIGNAL(clicked()),this,SLOT(toreadcard()));
    connect(ui->widget->index_widget(bckey), SIGNAL(clicked()),this,SLOT(torecharge()));
    connect(ui->widget->index_widget(tr("退出")), SIGNAL(clicked()),this,SLOT(toexit()));

    //    lineEdit_num_realamount  lineEdit_num_deposit lineEdit_remain lineEdit_amount
    connect(ui->lineEdit_num_realamount,SIGNAL(textChanged(QString)),ui->lineEdit_num_deposit,SLOT(setText(QString)));
    connect(ui->lineEdit_num_deposit,SIGNAL(textChanged(QString)),this,SLOT(TtolineEdit_amount()));
    connect(ui->checkBox,SIGNAL(toggled(bool)),this,SLOT(tochecksave()));

    //! after
    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    ui->checkBox->setChecked(conf.value("w_m_member_list_recharge/isprint").toBool());
    ui->lineEdit_vch_memberno->setEnabled(false);
    ui->lineEdit_vch_member->setEnabled(false);
    ui->lineEdit_amount->setEnabled(false);
    ui->lineEdit_remain->setEnabled(false);
    ui->lineEdit_vch_operid->setEnabled(false);
    ui->lineEdit_operdate->setEnabled(false);
}

w_m_member_list_recharge::~w_m_member_list_recharge()
{
    w_m_member_list::yun_member_close();
}
void w_m_member_list_recharge::ue_fetch()
{
    //ue_fetch (none) returns integer
    QString ls_memberno;
    QString ls_value;
    QString ls_member;
    QString ls_deposit_mode;
    QString ls_pay_mode;
    QString ls_memo;
    QString ls_err;
    QString ls_machine;
    double ldec_deposit;
    double ldec_realamount;
    double ldec_remain;
    double ldec_amount;
    double ldec_max_deposit;
    double ldc_limit;
    long ll_row;
    long ll_id;
    long ll_exist;
    bool lb_it;
    QDateTime ldt;
    QDateTime ldt_old;

    n_func::st_member_type  lst_member;

    ls_deposit_mode = "8";

    if(_tableview)ll_row = _tableview->currentIndex().row();
    if(_tablemodel)_tablemodel->record(ll_row).value("remain").toDouble();
    ls_memberno=ui->lineEdit_vch_memberno->text();
    ldec_deposit=0-ui->lineEdit_num_deposit->text().toDouble();
    ldec_realamount = ldec_deposit;
    ls_pay_mode = ui->comboBox_ch_pay_mode->comusrdata();
    ls_memo = ui->lineEdit_vch_memo->text();
    ldec_remain = ui->lineEdit_remain->text().toDouble();
    ldec_amount = ldec_deposit + ldec_remain;

    if(ls_memberno.trimmed().isEmpty()){
        lds_messagebox::warning(this, tr("消息提示"),tr("请先读卡/查卡,再充值!"));
        return;
    }

    if(ldec_deposit>=0){
        lds_messagebox::warning(this, tr("消息提示"),tr("取款金额不能为非正数!"));
        return;
    }

    lst_member = n_func::f_m_get_limit(ls_memberno);
    //2

    if(lst_member.as_m_state != "1"){
        lds_messagebox::warning(this, tr("消息提示"),tr("取款功能只针对正常状态会员，操作中止!"));
        return;
    }
    if(lst_member.as_m_att != "2"){
        lds_messagebox::warning(this, tr("消息提示"),tr("取款功能只针对充值类会员，操作中止!"));
        return;
    }
    ldt = QDateTime::currentDateTime();
    ldt_old = lst_member.dt_operdate;
    if(ldt_old.isValid() && ldt_old > ldt){
        lds_messagebox::warning(this, tr("消息提示"),tr("系统时间小于上次保存时间，请确认系统时间是否正确!"));
        return;
    }
    //3
    if(n_func::f_m_get_parameter("m_fetch_base", "0")=="1"){
        if(ldec_amount < 0){
            lds_messagebox::warning(this, tr("消息提示"),tr("取款金额超过可用金额!"));
            return;
        }
    }else {
        if((ldec_amount - lst_member.adec_m_limitdata) < 0){
            lds_messagebox::warning(this, tr("消息提示"),tr("取款金额超过保本外可用金额!"));
            return;
        }
    }

    //    END IF //19
    //    END IF //14
    //    ls_machine = f_m_get_parameter("vip_ic_machine","明华澳汉")
    //    IF (ls_machine = "STAR-TCP300/400" OR ls_machine = "日科创想IT卡") THEN //23
    //    ls_memberno = PARENT.cb_r.of_set_deposit_start(ldec_amount)
    //    IF (ifnull(ls_memberno) OR trim(ls_memberno) = "") THEN //25
    //    messagebox(gvar.gs_appname,"卡号不存在，请先读卡或查卡!")
    //    RETURN 1
    //    END IF //25
    //    lst_member = f_m_get_limit(ls_memberno)
    //    IF lst_member.as_m_state = "" THEN //29
    //    messagebox(gvar.gs_appname,"非本公司卡或无效卡！")
    //    RETURN 1
    //    ELSEIF lst_member.as_m_state <> "1" THEN //29
    //    messagebox(gvar.gs_appname,"此卡状态非正常，不能取款！")
    //    RETURN 1
    //    END IF //29
    //    IF lst_member.adt_m_limitdate <= gfunc.f_get_sysdatetime() THEN //35
    //    messagebox(gvar.gs_appname,"此卡已经超过有效期！")
    //    RETURN 1
    //    END IF //35
    //    IF lst_member.as_m_att <> "2" THEN //38
    //    messagebox(gvar.gs_appname,"非充值卡，不能取款！")
    //    RETURN 1
    //    END IF //38
    //    PARENT.lst_card = PARENT.cb_r.of_read_card(ls_memberno)
    //    dw_1.object.vch_memberno[1] =ls_memberno
    //    dw_1.object.t_3.text = lst_member.as_member
    //    dw_1.object.remain[1] =PARENT.lst_card.amount
    //    ldec_remain = PARENT.lst_card.amount
    //    ldec_amount = ldec_deposit + ldec_remain
    if(ls_memberno.trimmed().isEmpty()){
        lds_messagebox::warning(this, tr("消息提示"),tr("卡号不存在，请先读卡或查卡!"));
        return;
    }
    if(!_isready){
        lds_messagebox::warning(this, tr("消息提示"),tr("当前取款记录已取款,请重新读卡或查卡!"));
        return;
    } else {
        if(1==lds_messagebox::warning(this, tr("消息提示"),tr("确定数据正确无误，是否取款"), tr("确认"), tr("取消"))){
            ls_err=tr("手动退出!");
            return;
        }
    }
    //! <--transaction begin-->
    QSqlDatabase::database().transaction();
    switch(0){
    case 0:
        if(!(ls_err=n_func::f_m_deposit(ls_memberno,ldec_deposit,ldec_realamount,ldec_amount,ls_deposit_mode,ls_pay_mode,ls_memo, _parm)).isEmpty()){
            break;
        }
        if(w_m_member_list::is_yun_member) break;///=若是云会员付款,或退卡则到此结束
    case 1:
        if(n_func::f_m_get_parameter("vip_card_type", tr("IC卡"))==tr("IC卡")){
            //对ic卡做进一步处理
        }
        if(!(ls_err=n_func::f_m_insert_log(ls_memberno,tr("取款"),tr("取款金额:") + QString().sprintf("%.02f", qAbs(ldec_deposit)), 0)).isEmpty()){
            break;
        }
    }

    if(ls_err.isEmpty()){
        ui->lineEdit_amount->setText(QString().sprintf("%.02f", ldec_amount));
        _isready=false;
        QSqlDatabase::database().commit();
    } else {
        QSqlDatabase::database().rollback();
        qDebug() << ls_err;
    }
    //! <--transaction end-->
    if(_tablemodel)_tablemodel->select();
    if(_tableview)_tableview->restore();
    lds_messagebox::warning(this, tr("消息提示"),ls_err.isEmpty()?tr("取款成功!"):tr("数据异常，取款数据保存失败!"));
    if(ls_err.isEmpty()){
        if(ui->checkBox->isChecked())rechargeprint();
    }
}

void w_m_member_list_recharge::ue_blankcard()
{
    //ue_back_card (none) returns (none)
    long ll_row;
    long li_rtn;
    QString ls_err;
    QString ls_memberno;
    QString ls_memo;
    uo_rw_member uo_member;
    n_func::st_member_type  lst_member;
    QDateTime ldt;
    QDateTime ldt_old;
    double ldec_remain;
    double ldec_deposit;
    double ldec_realamount;
    double ldec_amount;
    QString ls_value;
    double ldec_base;
    double ldec_point=0;
    QString ls_type;
    QString ls_bute;

    uo_member.of_init("4");
    ll_row = _tableview->currentIndex().row();
    ls_memberno=ui->lineEdit_vch_memberno->text();
    ldec_deposit=0-ui->lineEdit_num_deposit->text().toDouble();
    ldec_realamount = ldec_deposit;
    ls_memo = ui->lineEdit_vch_memo->text();
    ldec_remain = ui->lineEdit_remain->text().toDouble();
    //    if(_tablemodel)_tablemodel->record(ll_row).value("remain").toDouble();
    ldec_amount = ldec_deposit + ldec_remain;

    if(ls_memberno.trimmed().isEmpty()){
        lds_messagebox::warning(this, tr("消息提示"),tr("请先读卡/查卡,再充值!"));
        return;
    }
    if(ldec_deposit>=0){
        lds_messagebox::warning(this, tr("消息提示"),tr("退款金额不能为非正数!"));
        return;
    }
    lst_member = n_func::f_m_get_limit(ls_memberno);
    //2
    if(lst_member.as_m_state == "4"){
        lds_messagebox::warning(this, tr("消息提示"),tr("此会员卡已作废，操作中止!"));
        return;
    }
    if(lst_member.as_m_state == "3"){
        lds_messagebox::warning(this, tr("消息提示"),tr("此会员卡已挂失，无法退卡!"));
        return;
    }
    if(lst_member.as_m_state == "2"){
        lds_messagebox::warning(this, tr("消息提示"),tr("止付卡不能退卡，只能启用!"));
        return;
    }

    if(lst_member.as_m_cardflag != "Y"){
        ls_value = n_func::f_m_get_parameter("vip_card_type",tr("无会员卡"));
        if(ls_value == tr("IC卡") || ls_value == tr("磁卡")){
            lds_messagebox::warning(this, tr("消息提示"),tr("会员卡%1,还未发卡,不能退卡!").arg(ls_memberno));
            return;
        }
    }
    ldt = QDateTime::currentDateTime();
    ldt_old = lst_member.dt_operdate;

    if(ldt_old.isValid() && ldt_old > ldt){
        lds_messagebox::warning(this, tr("消息提示"),tr("系统时间小于上次保存时间，请确认系统时间是否正确!"));
        return;
    }
    //3
    if(n_func::f_m_get_parameter("m_fetch_base", "0")=="1"){
        if(ldec_amount < 0){
            lds_messagebox::warning(this, tr("消息提示"),tr("退款金额超过可用金额!"));
            return;
        }
        if(ldec_amount>0){
            lds_messagebox::warning(this, tr("消息提示"),tr("卡余额(包含保本金额,保本金额可取)不为零,不能退卡,~r~n可进行取款操作，待余额为零再退卡!"));
            return;
        }
    }else {
        if((ldec_amount - lst_member.adec_m_limitdata) < 0){
            lds_messagebox::warning(this, tr("消息提示"),tr("退款金额超过保本外可用金额!"));
            return;
        }
        if((ldec_amount - lst_member.adec_m_limitdata) > 0){
            lds_messagebox::warning(this, tr("消息提示"),tr("卡余额(不包含保本金额,保本金额不可取)不为零,不能退卡,~r~n可进行取款操作，待余额为零再退卡!"));
            return;
        }
        ldec_base = lst_member.adec_m_limitdata;
    }


    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    QSqlQuery query;
    //! <--transaction begin-->
    QSqlDatabase::database().transaction();
    switch(0){
    case 0:
        if(w_m_member_list::is_yun_member){
            ldec_point=w_sys_manage_cloudsync::get_yun_member_readcard(ls_memberno, conf.value("w_sys_manage_cloudsync_basedataset/cloudUser").toString(),
                                                                       "num_point", QVariant::Double).toDouble();if(!w_sys_manage_cloudsync::this_action_is_successed) break;
        } else {
            query.prepare("select num_point from t_m_curamount  where vch_memberno =:ls_memberno");
            query.bindValue(":ls_memberno", ls_memberno);
            query.exec();
            query.next();
            ldec_point=query.record().value("num_point").toDouble();
        }
        if(ldec_point > 0) {
            if(1==lds_messagebox::warning(this, tr("消息提示"),tr("卡积分不为零,是要执相应的积分操作,还是系统自动冲减?点YSE按系统自动冲减,点NO自己执行相关操作!"), tr("确认"), tr("取消"))){
                lds_messagebox::warning(this, tr("消息提示"),tr("自己执行积分操作,取消退卡!"));
                return;
            }
        }

        if(!lst_card.flag){
            ls_err=lst_card.errtext;
            break;
        }

        if(lst_card.amount>0){
            ls_err=n_func::f_m_deposit(ls_memberno,ldec_deposit,ldec_deposit,0,"5","6",tr("退卡还现"), _parm);
            if(!ls_err.isEmpty()){
                qDebug() <<ls_err;
                ls_err=tr("读取会员帐务数据失败，操作终止!");
                break;
            }
        }
        if(w_m_member_list::is_yun_member) break;///=若是云会员付款,或退卡则到此结束
    case 1:
        query.prepare("update t_m_member set ch_cardflag ='N' , ch_state ='1' , int_basedata =0  where vch_memberno =:ls_memberno");
        query.bindValue(":ls_memberno", ls_memberno);
        ls_err=query.exec()?"":query.lastError().text();
        if(!ls_err.isEmpty()){
            qDebug() <<ls_err;
            ls_err=tr("修改会员发卡标志失败!");
            break;
        }

        query.prepare("update t_m_curamount set num_amount =0 , num_point =0 where vch_memberno =:ls_memberno");
        query.bindValue(":ls_memberno", ls_memberno);
        ls_err=query.exec()?"":query.lastError().text();
        if(!ls_err.isEmpty()){
            qDebug() <<ls_err;
            ls_err=tr("会员余额恢复!");
            break;
        }

        ls_err=n_func::f_m_insert_log(lst_card.cardid,tr("退卡"),tr("金额:") +QString().sprintf("%.02f",qAbs(ldec_deposit)),ldec_base);
        if(!ls_err.isEmpty()){
            ls_err=tr("读取会员帐务数据失败，操作终止!");
            break;
        }

        //无法清除卡蕊片,操作终止
        //            IF  NOT (uo_member.of_clear_card(f_m_get_parameter("vip_ic_password",""))) THEN //58
        //            ls_err = "无法清除卡蕊片,操作终止!"
        if(uo_member.is_cardtype == "1"){
            QString icpasswd=n_func::f_m_get_parameter("vip_ic_password");
            Reader_RF reader;
            if(reader.setDefPwd(icpasswd=="FFFFFF"?"FFFFFFFFFFFF":icpasswd.toLocal8Bit().toHex())){
                if(reader.writecard("","","")){
                    if(reader.pwdChange(icpasswd=="FFFFFF"?"FFFFFFFFFFFF":icpasswd.toLocal8Bit().toHex(), "FFFFFFFFFFFF")){
                        //清除成功
                    }
                }
            }
            ls_err=reader.lastError();
        }
        if(!ls_err.isEmpty()){
            ls_err=tr("读取会员帐务数据失败，操作终止!");
            break;
        }
    }
    if(ls_err.isEmpty()){
        QSqlDatabase::database().commit();
        ls_err=tr("退卡成功!");
        lds_messagebox::warning(this, tr("消息提示"),ls_err);
        if(ui->checkBox->isChecked())rechargeprint();
    } else {
        QSqlDatabase::database().rollback();
        ls_err=tr("退卡失败!");
        lds_messagebox::warning(this, tr("消息提示"),ls_err);
    }
    //! <--transaction end-->
}

void w_m_member_list_recharge::rechargeprint()
{
    //打印内容
    //title 会员卡余额
    // 会员卡号
    //会员姓名
    //会员类型
    //会员余额
    //操作员
    //操作日期

    if(ui->lineEdit_vch_memberno->text().isEmpty()){
        lds_messagebox::warning(this, tr("消息提示"),tr("会员编号为空，请先读卡!"));
    } else {
        blockIns printdata;
        printdata += backheader::printer->drawText("1B40 1B6101 1C5701", tr("会员卡")+bckey+"\n");//清除缓存 居中 2x2x
        printdata += backheader::printer->drawText("1B40", "");//清除缓存

        printdata += backheader::printer->drawText("", ui->label_vch_memberno_t->text()+":"+ui->lineEdit_vch_memberno->text() + "\n");//
        printdata += backheader::printer->drawText("", ui->label_vch_member_t->text()+":"+ui->lineEdit_vch_member->text()+ "\n");//
        printdata += backheader::printer->drawText("", tr("会员类型")+":"+lst_card.card_type + "\n");//
        printdata += backheader::printer->drawText("", ui->label_num_realamount_t->text()+":"+ui->lineEdit_num_realamount->text()+ "\n");//
        printdata += backheader::printer->drawText("", ui->label_num_deposit_t->text()+":"+ui->lineEdit_num_deposit->text()+ "\n");//
        printdata += backheader::printer->drawText("", ui->label_amount_t->text()+":"+ui->lineEdit_amount->text()+ "\n");//
        printdata += backheader::printer->drawText("", tr("操作员")+":"+n_func::gs_opername + "\n");//
        printdata += backheader::printer->drawText("", tr("操作日期")+":"+n_func::f_get_sysdatetime().toString("yyyy-MM-dd hh:mm:ss") + "\n");

        printdata += backheader::printer->drawText("1B6405 1D5601", "");//走纸5x， 切纸
        printdata =   backheader::printer->setPrintCount(printdata, 1);

        backheader::asyn_dataprint(printdata);
    }
}

void w_m_member_list_recharge::toremainprint()
{
    //打印内容
    //title 会员卡余额
    // 会员卡号
    //会员姓名
    //会员类型
    //会员余额
    //操作员
    //操作日期

    if(ui->lineEdit_vch_memberno->text().isEmpty()){
        lds_messagebox::warning(this, tr("消息提示"),tr("会员编号为空，请先读卡!"));
    } else {
        blockIns printdata;

        printdata += backheader::printer->drawText("1B40 1B6101 1C5701", tr("会员余额")+"\n");//清除缓存 居中 2x2x
        printdata += backheader::printer->drawText("1B40", "");//清除缓存

        printdata += backheader::printer->drawText("", ui->label_vch_memberno_t->text()+":"+lst_card.cardid + "\n");//
        printdata += backheader::printer->drawText("", ui->label_vch_member_t->text()+":"+lst_card.cardname + "\n");//
        printdata += backheader::printer->drawText("", tr("会员类型")+":"+lst_card.card_type + "\n");//
        printdata += backheader::printer->drawText("",ui->label_remain_t->text()+":"+QString().sprintf("%.02f", lst_card.amount) + "\n");//
        printdata += backheader::printer->drawText("", tr("操作员")+":"+n_func::gs_opername + "\n");//
        printdata += backheader::printer->drawText("", tr("操作日期")+":"+n_func::f_get_sysdatetime().toString("yyyy-MM-dd hh:mm:ss") + "\n");

        printdata += backheader::printer->drawText("1B6405 1D5601", "");//走纸5x， 切纸
        printdata =   backheader::printer->setPrintCount(printdata, 1);

        backheader::asyn_dataprint(printdata);
    }
}

void w_m_member_list_recharge::toreadcard()
{
    n_func::st_member_type lst_member;
    QString cardno;
    QString as_memberno;
    uo_rw_member uo_member;
    QString ls_value;
    _isready=false;
    bool isyun=_parm.isEmpty()//充值
            ||_parm=="D"//退卡
            ||_parm=="C"//取款
            ;
    //读卡
    w_m_member_list::yun_member_close();
    if(!w_m_member_list::static_to_memeber_readcard(cardno, uo_member, isyun)){
        ui->lineEdit_vch_memberno->clear();
        ui->lineEdit_vch_member->clear();
        ui->lineEdit_remain->clear();
        ui->lineEdit_amount->clear();
        return;
    }
    qDebug() << cardno;
    as_memberno=cardno;
    if(as_memberno.isEmpty()){
        lds_messagebox::warning(this, tr("消息提示"),tr("对应会员号为空!"));
        return ;
    }

    lst_card=uo_member.of_read_card(as_memberno);
    if(lst_card.flag == false){
        lds_messagebox::warning(this, tr("消息提示"),lst_card.errtext);
        return;
    }
    if(lst_card.flag){
        lst_member=n_func::f_m_get_limit(lst_card.cardid);
        if(lst_member.as_m_state.isEmpty()){
            lds_messagebox::warning(this, tr("消息提示"),tr("非本公司卡或无效卡！"));
            return;
        } else if(lst_member.as_m_state!="1"){
            if(this->_parm!="D"){
                lds_messagebox::warning(this, tr("消息提示"),tr("此卡状态非正常,不能充值!"));
                return;

            }
        }

        if(lst_member.adt_m_limitdate <= n_func::f_get_sysdatetime()){
            lds_messagebox::warning(this, tr("消息提示"),tr("此卡已经超过有效期！"));
            return;
        }

        if(this->_parm!="D"){
            if(lst_member.as_m_att!="2"){
                lds_messagebox::warning(this, tr("消息提示"),tr("非充值卡,不能充值"));
                return;

            }
        }

        if(lst_member.as_m_cardflag !="Y"){
            ls_value=n_func::f_m_get_parameter("vip_card_type", tr("无会员卡"));
            if(ls_value==tr("IC卡") || ls_value==tr("磁卡")){
                if(this->_parm=="C"){
                    lds_messagebox::warning(this, tr("消息提示"),tr("会员卡")+as_memberno+tr(",还未发卡,不能取款!"));
                    return;
                } else {
                    lds_messagebox::warning(this, tr("消息提示"),tr("会员卡")+cardno+tr(",还未发卡,不能充值!"));
                    return;
                }
            }
        }
    } else {
        lds_messagebox::warning(this, tr("消息提示"),lst_card.errtext);
        return;
    }
    //==
    ui->lineEdit_vch_memberno->setText(lst_card.cardid);
    ui->lineEdit_vch_member->setText(lst_member.as_member);
    ui->lineEdit_num_realamount->setText("0");
    ui->lineEdit_num_deposit->setText("0");
    ui->lineEdit_remain->setText(QString().sprintf("%.02f", lst_card.amount));
    ui->lineEdit_amount->setText("0");
    ui->lineEdit_vch_operid->setText(n_func::gs_operid);
    ui->lineEdit_operdate->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm"));
    _isready=true;

    if(_isready)TtolineEdit_amount();
}

void w_m_member_list_recharge::torecharge()
{

    if(this->_parm=="C"){
        ue_fetch();
        return;
    }
    if(this->_parm=="D"){
        ue_blankcard();
        return;
    }
    QString ls_memberno=ui->lineEdit_vch_memberno->text();
    double ldec_deposit=ui->lineEdit_num_deposit->text().toDouble();
    double ldec_realamount=ui->lineEdit_num_realamount->text().toDouble();
    double ldec_remaint=ui->lineEdit_remain->text().toDouble();
    double ldec_amount=ldec_deposit+ldec_remaint;
    QString ls_value;
    QString ls_error;
    QString ls_deposit_mode="1";
    QString ls_pay_mode=w_m_member::RECHARGE_MODEL_PAYMODE.item(ui->comboBox_ch_pay_mode->currentIndex(), 0)->text();
    QString ls_memo=ui->lineEdit_vch_memo->text();
    //100，000.00
    double ldec_max_deposit=n_func::f_m_get_parameter("m_max_deposit", "999999").remove(",").toDouble();
    n_func::st_member_type  lst_member;

    switch(0){
    case 0:
        if(ls_memberno.trimmed().isEmpty()){
            ls_error=(tr("请先读卡/查卡,再充值!"));
            break;
        }
        if(ldec_deposit < 0 || ldec_realamount < 0){
            ls_error=(tr("充值金额与实收金额不能为负数!"));
            break;
        }
        if(ldec_deposit>ldec_max_deposit){
            ls_error=(tr("充值金额不能大于最大充值金额，你可已在会员参数设置中更改该值!"));
            break;
        }
        if(n_func::f_m_get_parameter("m_deposit_negative", "0")=="1" && ldec_deposit != ldec_realamount){
            ls_error=(tr("充值金额与实收金额不等,不能充值,你可以在会员参数设置中修改此项!"));
            break;
        }
        if ((ldec_deposit > 0 || ldec_realamount > 0) || ldec_deposit < 0 && ldec_realamount < 0){
            if (ldec_deposit < 0 && ldec_realamount != ldec_deposit){
                ls_error=(tr("负数充值时，充值金额与实收金额必须相等!"));
                break;
            }
        }  else {
            if(ldec_deposit == 0 && ldec_realamount == 0){
                ls_error=(tr("充值金额与实收金额不能全部为0，不能充值!"));
                break;
            } else {
                ls_error=(tr("充值金额与实收金额正负不一致，不能充值!"));
                break;
            }
        }

        if(!_isready){
            ls_error=(tr("当前充值记录已充值,请重新读卡或查卡!"));
            break;
        } else {
            if(1==lds_messagebox::warning(this, tr("消息提示"),tr("确定数据正确无误，是否充值！"), tr("确认"), tr("取消"))){
                ls_error=tr("手动退出!");
                break;
            }
        }
    case 1:
        //! <--transaction begin-->
        QSqlDatabase::database().transaction();
        if((ls_error=n_func::f_m_deposit(ls_memberno,ldec_deposit,ldec_realamount,ldec_amount,ls_deposit_mode,ls_pay_mode,ls_memo,_parm)).isEmpty()){
            if(n_func::f_m_get_parameter("vip_card_type", tr("IC卡"))==tr("IC卡")){

            }
        } else {
            qDebug() << 10 << ls_error;
            ls_error=tr("数据库错误!");
            QSqlDatabase::database().rollback();
            break;
        }


        if(!(ls_error=n_func::f_m_insert_log(ls_memberno,tr("充值"),tr("充值金额:") + QString::number(ldec_deposit),0)).isEmpty()){
            ls_error=(tr("数据库错误!"));
            QSqlDatabase::database().rollback();
            break;
        }
        QSqlDatabase::database().commit();
        if(_tablemodel)_tablemodel->select();
        if(_tableview)_tableview->restore();
        _isready=false;
        //! <--transaction end-->
        break;
    }

    if(ls_error.isEmpty()){
        lds_messagebox::warning(this, tr("消息提示"),tr("充值成功!"));
        if(ui->checkBox->isChecked())rechargeprint();
        return;

    } else {
        lds_messagebox::warning(this, tr("消息提示"),ls_error);
        return;
    }
}

void w_m_member_list_recharge::toexit()
{
    this->reject();
}

void w_m_member_list_recharge::TtolineEdit_amount()
{
    if(_parm=="C"){//取款
        ui->lineEdit_amount->setText(QString().sprintf("%.02f", (ui->lineEdit_remain->text().toFloat() - ui->lineEdit_num_deposit->text().toFloat())));
    } else if(_parm=="D"){//退卡
        ui->lineEdit_amount->setText(QString().sprintf("%.02f", (ui->lineEdit_remain->text().toFloat() - ui->lineEdit_num_deposit->text().toFloat())));
    } else {//充值
        if(n_func::f_m_get_parameter("m_deposit_negative", "0")=="1"){
            ui->lineEdit_num_deposit->setText(ui->lineEdit_num_realamount->text());
        }
        ui->lineEdit_amount->setText(QString().sprintf("%.02f", (ui->lineEdit_num_deposit->text().toFloat() + ui->lineEdit_remain->text().toFloat())));
    }
    //    if(_parm!="C"){
    //        if(n_func::f_m_get_parameter("m_deposit_negative", "0")=="1"){
    //            ui->lineEdit_num_deposit->setText(ui->lineEdit_num_realamount->text());
    //        }
    //        ui->lineEdit_amount->setText(QString().sprintf("%.02f", (ui->lineEdit_num_deposit->text().toFloat() + ui->lineEdit_remain->text().toFloat())));

    //    } else {
    //        ui->lineEdit_amount->setText(QString().sprintf("%.02f", (ui->lineEdit_remain->text().toFloat() - ui->lineEdit_num_deposit->text().toFloat())));

    //    }
}

void w_m_member_list_recharge::tochecksave()
{
    //更新配置文件
    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    conf.setValue("w_m_member_list_recharge/isprint", ui->checkBox->isChecked());
}

//==========================
#include "ui_w_m_member_pwdchange_dialog.h"
w_m_member_pwdchange::w_m_member_pwdchange(QWidget *parent):
    QDialog(parent),
    ui(new Ui_w_m_member_pwdchange_Dialog),
    _parent(static_cast<w_m_member_list* >(parent))

{
    ui->setupUi(this);
    //! toolbar
    ui->widget->setup(QStringList()<<tr("确定")<<tr("取消"),Qt::AlignRight);
    connect(ui->widget->index_widget(tr("确定")), SIGNAL(clicked()),this,SLOT(took()));
    connect(ui->widget->index_widget(tr("取消")), SIGNAL(clicked()),this,SLOT(tocancel()));

    ui->label_4->hide();
    ui->lineEdit_cur_operator->hide();
    ui->label_5->hide();
    ui->lineEdit_login_operator->hide();
}

void w_m_member_pwdchange::took()
{
    QString ls_password_old;
    QString ls_password;
    QString ls_password1;
    QString ls_err;


    ls_password_old = ui->lineEdit_oldpwd->text();
    ls_password = ui->lineEdit_newpwd->text();
    ls_password1 = ui->lineEdit_newpwd2->text();
    if(is_password.isEmpty()){

    } else {
        qDebug() << is_password;
        if(ls_password_old==is_password){

        } else {
            lds_messagebox::warning(this, tr("消息提示"),tr("旧密码录入错误，不能修改密码!"));
            return;
        }
    }
    if(ls_password==ls_password1){

    } else {
        lds_messagebox::warning(this, tr("消息提示"),tr("新密码与确认密码不一样，修改密码中止!"));
        return;
    }

    if(0==lds_messagebox::warning(this, tr("消息提示"),tr("确定按设置修改密码"),tr("确认"),tr("取消"))){
        //! <--transaction begin-->
        QSqlDatabase::database().transaction();
        QSqlQuery query;
        query.prepare("update t_m_member set vch_password =:ls_password where vch_memberno =:is_memberno");
        query.bindValue(":ls_password", ls_password);
        query.bindValue(":is_memberno", is_memberno);
        if(query.exec()){
            QSqlDatabase::database().commit();
            ls_err=tr("修改密码成功，请记住新密码!");
            _parent->torefresh();
            lds_messagebox::warning(this, tr("消息提示"),ls_err);
            this->accept();
        } else {
            QSqlDatabase::database().rollback();
            ls_err=tr("修改密码失败!");
            _parent->torefresh();
            lds_messagebox::warning(this, tr("消息提示"),ls_err);

        }
        //! <--transaction end-->
    }
}

void w_m_member_pwdchange::tocancel()
{
    this->reject();
}

QString w_m_member_pwdchange::check()
{
    is_memberno=_parent->getTablemodel()->record(_parent->getTableview()->currentIndex().row()).value("vch_memberno").toString();
    QSqlQuery query;
    query.exec(QString("select ifnull(vch_password, '') from t_m_member where vch_memberno='%1'").arg(is_memberno));
    if(query.next()){
        is_password=query.record().value(0).toString();
        if(is_password.isEmpty()){
            ui->lineEdit_oldpwd->setEnabled(false);
            ui->lineEdit_newpwd->setFocus();
        } else {
            ui->lineEdit_oldpwd->setFocus();
        }
        qDebug() << "old pwd" << is_password;
        return "";
    } else {
        return tr("指定会员号不存在,请刷新数据,核实!");
    }
}

//=======================
#include "ui_w_m_member_news_dialog.h"
w_m_member_news::w_m_member_news(w_m_member_list *parent):
    QDialog(parent),
    _parent(parent),
    ui(new Ui_w_m_member_news_Dialog)
{
    ui->setupUi(this);

    //! init
    ui->comboBox_ch_typeno->addItems( w_m_member::get_viptypeno_name("select ch_typeno, vch_typename from t_m_member_type"));
    ui->comboBox_ch_typeno->setCurrentIndex(qMax(_parent->getTreew()->get_0_Index(), 0));
    ui->dateTimeEdit_dt_limit->setDateTime(n_func::f_get_sysdatetime().addYears(1));
    ui->dateTimeEdit_dt_limit->setDisplayFormat("yyyy-MM-dd hh:mm:ss");
    //! toolbar
    ui->widget->setup(QStringList() << tr("生成") << tr("退出"), Qt::AlignRight);
    connect(ui->widget->index_widget(tr("生成")), SIGNAL(clicked()),this,SLOT(took()));
    connect(ui->widget->index_widget(tr("退出")), SIGNAL(clicked()),this,SLOT(tocancel()));

}

void w_m_member_news::took()
{
    //clicked (none) returns long [pbm_bnclicked]
    QString ls_limit_tab;
    QString ls_sql;
    QString ls_memberno;
    QString ls_membername;
    QString ls_cardtype;
    QString ls_rand;
    QString ls_rand1;
    QString ls_rand2;
    QString ls_mess;
    QString ls_start;
    QString ls_begin;
    QString ls_end;
    QString ls_type;
    QString ls_cardno;
    qlonglong ll_begin;
    qlonglong ll_end;
    qlonglong ll_limit;
    qlonglong ll_query;
    qlonglong i;
    qlonglong ll_exist;
    qlonglong ll_flownum;
    qlonglong ll_left;
    double ldec_limit;
    double ldec_flownum;
    QDateTime ldt_limit;
    QDateTime ldt_today;


    //    PARENT.dw_1.accepttext()
    ls_start = ui->lineEdit_code_start->text();//trim(PARENT.dw_1.object.code_start[1])
    ll_begin = ui->lineEdit_code_begin->text().toLongLong();//PARENT.dw_1.object.code_begin[1]
    ls_begin = ui->lineEdit_code_begin->text();//string(ll_begin)
    ll_end = ui->lineEdit_code_end->text().toLongLong();//PARENT.dw_1.object.code_end[1]
    ls_end = ui->lineEdit_code_end->text();//string(ll_end)
    ll_limit = ui->lineEdit_code_limit->text().toLongLong();//PARENT.dw_1.object.code_limit[1]
    ls_type = backheader::getinterdata(ui->comboBox_ch_typeno->currentText());//PARENT.dw_1.object.ch_typeno[1]
    ldt_limit = ui->dateTimeEdit_dt_limit->dateTime();//PARENT.dw_1.object.dt_limit[1]

    if(ll_limit==0){
        lds_messagebox::warning(this, tr("消息提示"),tr("编号位数不能为零!"));
        return;
    }
    if(ldt_limit < n_func::f_get_sysdatetime()){
        lds_messagebox::warning(this, tr("消息提示"),tr("会员有效期限应该大于当前日期!"));
        return;
    }
    if(ldt_limit.isNull()){
        lds_messagebox::warning(this, tr("消息提示"),tr("有效期限不能为空!"));
        return;
    }
    if(ui->comboBox_ch_typeno->currentText().isEmpty()){
        lds_messagebox::warning(this, tr("消息提示"),tr("会员类型不能为空!"));
        return;
    }
    if(ll_limit==0 || (ll_limit+ls_start.length()) > 20){
        lds_messagebox::warning(this, tr("消息提示"),tr("编号位数不能为空且不能大于%1位!")
                                .arg(QString::number(20-ls_start.length()))
                                );
        return;
    }
    if(ll_end-ll_begin >= 10000){
        lds_messagebox::warning(this, tr("消息提示"),tr("一次能批量创建的会员不超过1万条!"));
        return;
    }
    if(ll_limit < ls_end.trimmed().length()){
        lds_messagebox::warning(this, tr("消息提示"),tr("编号部分位数不能小于编号到(结束)实际位数!"));
        return;
    }
    if(ll_begin > ll_end){
        lds_messagebox::warning(this, tr("消息提示"),tr("编号从(起始)不能大于编号到(结束)!"));
        return;
    }
    if(ls_end.isEmpty()){
        lds_messagebox::warning(this, tr("消息提示"),tr("编号到(结束)不能为空!"));
        return;
    }
    if(ls_begin.isEmpty()){
        lds_messagebox::warning(this, tr("消息提示"),tr("编号从(起始)不能为空!"));
        return;
    }

    if(ls_start.isEmpty()){
        if(1==lds_messagebox::warning(this, tr("消息提示"),tr("编号前缀为空，(前缀可以固定生成在每个会员编号前面)，你还要继续吗？"), tr("继续"),tr("取消"))){
            return;
        }
    }

    //! <--transaction begin-->
    QSqlDatabase::database().transaction();
    QString errtext;
    QSqlQuery query;
    for(qlonglong ll_i=ll_begin; ll_i <= ll_end; ll_i++){
        ls_memberno = ls_start +QString::number(ll_i).rightJustified(ll_limit, '0').right(ll_limit);// fill("0",ll_limit - len(string(i))) + string(i)
        ls_membername = "批量" + QString::number(ll_i);
        ls_cardno=ls_memberno;

        query.prepare("Select count(*)    from t_m_member   where vch_memberno =:ls_memberno");
        query.bindValue(":ls_memberno", ls_memberno);
        query.exec();
        query.next();
        if(query.record().value(0).toLongLong() > 0){
            errtext=tr("批量范围中的会员编号系统已经存在，例如:%1，本次操作无效!")
                    .arg(ls_memberno);
            break;
        }
        ldt_today=n_func::f_get_sysdatetime();

        query.prepare("Insert into t_m_member (vch_memberno,vch_cardno,vch_member,ch_typeno,num_limit,dt_limit,vch_operID,dt_operdate) Values(?,?,?,?,?,?,?,?)");
        query.addBindValue(ls_cardno);
        query.addBindValue(ls_memberno);
        query.addBindValue(ls_membername);
        query.addBindValue(ls_type);
        query.addBindValue(ldec_limit);
        query.addBindValue(ldt_limit);
        query.addBindValue(n_func::gs_operid);
        query.addBindValue(ldt_today);

        if(query.exec()){
        } else {
            errtext=query.lastError().text();
            break;
        }
    }

    if(errtext.isEmpty()){
        QSqlDatabase::database().commit();
        _parent->torefresh();
        lds_messagebox::warning(this, tr("消息提示"),tr("批量生成会员完成！"));
    } else {
        QSqlDatabase::database().rollback();
        _parent->torefresh();
        lds_messagebox::warning(this, tr("消息提示"),tr("批量生成会员失败！")+"\n"+errtext, errtext);

    }
    //! <--transaction end-->
}

void w_m_member_news::tocancel()
{
    this->reject();
}

