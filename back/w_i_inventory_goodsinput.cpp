#include "w_i_inventory_goodsinput.h"
//商品入库
#include "backheader.h"
#include <QDebug>
#include <QSqlQueryModel>
#include <QStandardItemModel>
#include "n_func.h"
#include "lds_messagebox.h"
#include <QSqlError>
#include "backheader.h"
#include <QEventLoop>

QVariant w_i_inventory_goodsinput::QueryModel::data(const QModelIndex &item, int role) const
{
    if(role==Qt::DisplayRole){
        if(item.column() == 0){
            return backheader::VIPFINDVALUE(w_i_inventory::MODEL_INVENTORY_STATE_TYPE, QSqlQueryModel::data(item, role).toString());
        }

        if(item.column() == 2){
            return backheader::VIPFINDVALUE(w_i_inventory::MODEL_INVENTORY_TYPE, QSqlQueryModel::data(item, role).toString());
        }

        if(item.column() == 5){
            return QSqlQueryModel::data(item, role).toDateTime().toString("yyyy-MM-dd hh:mm:ss");
        }

        if(item.column() == 7){
            return QSqlQueryModel::data(item, role).toDateTime().toString("yyyy-MM-dd hh:mm:ss");
        }
    }
    return QSqlQueryModel::data(item, role);
}

w_i_inventory_goodsinput::w_i_inventory_goodsinput(QWidget *parent, const QString &type) :
    QDialog(parent),
    ui(new Ui_w_i_inventory_goodsinput_Dialog),
    _type(type)
{
    ui->setupUi(this);
    ui->frame->setFixedWidth(w_rpt_report::frame_left_fixed_width);

    //时间改成年月日
    ui->dateTimeEdit->setDisplayFormat("yyyy-MM-dd");
    ui->dateTimeEdit_2->setDisplayFormat("yyyy-MM-dd");
    ui->dateTimeEdit->setDate(n_func::f_get_sysdatetime().date());
    ui->dateTimeEdit_2->setDate(n_func::f_get_sysdatetime().date());

    //model
    querymodel=new QueryModel;
    ui->tableView->setEditTriggers(QTableView::NoEditTriggers);ui->tableView->setModel(querymodel);
    standmodel_2=new QStandardItemModel;
    ui->tableView_2->setEditTriggers(QTableView::NoEditTriggers);ui->tableView_2->setModel(standmodel_2);

    //! toolbar
    ui->widget->setup(QStringList() << tr("新增单据") << tr("查看单据") << tr("刷新") << tr("退出"), Qt::AlignTop);
    connect(ui->widget->index_widget(tr("新增单据")),SIGNAL(clicked()),this,SLOT(tonewbill()));
    connect(ui->widget->index_widget(tr("查看单据")),SIGNAL(clicked()),this,SLOT(tolookbill()));
    connect(ui->widget->index_widget(tr("刷新")),SIGNAL(clicked()),this,SLOT(torefresh()));
    connect(ui->widget->index_widget(tr("退出")),SIGNAL(clicked()),this,SLOT(toexit()));

    connect(ui->radioButton_all, SIGNAL(clicked()),this,SLOT(torefresh()));
    connect(ui->radioButton_no, SIGNAL(clicked()),this,SLOT(torefresh()));
    connect(ui->radioButton_ok, SIGNAL(clicked()),this,SLOT(torefresh()));
    connect(ui->radioButton_bad, SIGNAL(clicked()),this,SLOT(torefresh()));

    connect(ui->tableView,SIGNAL(selectchanged(int)),this,SLOT(toupdatetableview2(int)));
    //! after
    //!refresh
    torefresh();

    //ch_sheettype隐藏
    ui->tableView->setColumnHidden(9,true);
}

lds_tableview_sum *w_i_inventory_goodsinput::getTableview_2()
{
    return ui->tableView_2;
}

QStandardItemModel *w_i_inventory_goodsinput::getStandmodel_2()
{
    return standmodel_2;
}

lds_tableView *w_i_inventory_goodsinput::getTableview()
{
    return ui->tableView;
}

QSqlQueryModel *w_i_inventory_goodsinput::getQuerymodel()
{
    return querymodel;
}

void w_i_inventory_goodsinput::tonewbill()
{
    w_i_inventory_goodsinput_new dialog(this);
    backheader::static_dialogexec(dialog,qobject_cast<QAbstractButton* >(sender())->text());
}

void w_i_inventory_goodsinput::tolookbill()
{
    w_i_inventory_goodsinput_change dialog(this);
    backheader::static_dialogexec(dialog,qobject_cast<QAbstractButton* >(sender())->text());
}

void w_i_inventory_goodsinput::torefresh(const QString &findsheetno)
{
    //sql语句 select
    QString selectsql=
            "SELECT  ch_state,ch_sheetNo,ch_operation,"
            "(select vch_operator  from (Select vch_operID,vch_operator from cey_sys_operator union Select '0000' as vch_operID,'system' as vch_operator )t where t.vch_operID=cey_st_sheet_master.vch_handle), "
            "(select vch_operator  from (Select vch_operID,vch_operator from cey_sys_operator union Select '0000' as vch_operID,'system' as vch_operator )t where t.vch_operID=cey_st_sheet_master.vch_operId), "
            "dt_operdate,"
            "(select vch_operator  from (Select vch_operID,vch_operator from cey_sys_operator union Select '0000' as vch_operID,'system' as vch_operator )t where t.vch_operID=cey_st_sheet_master.vch_audit), "
            "dt_audit,vch_memo,ch_sheetType"
            " FROM cey_st_sheet_master"
            " where ch_sheetType = '%1' "
            ;
    selectsql=selectsql.arg(_type);

    if(ui->radioButton_all->isChecked()){
        selectsql+=
                " and ((ch_state = 'N' and dt_operdate between '%1' and"
                " '%2') or"
                " (ch_state <> 'N' and dt_audit between '%1' and"
                " '%2'))"
                ;
    } else if(ui->radioButton_no->isChecked()){
        selectsql+=
                "and (ch_state='N' and dt_operdate between '%1' and '%2')";
    } else if(ui->radioButton_ok->isChecked()){
        selectsql+=
                "and (ch_state='Y' and dt_audit between '%1' and '%2')";
    } else if(ui->radioButton_bad->isChecked()){
        selectsql+=
                "and (ch_state='O' and dt_audit between '%1' and '%2')";
    }

    querymodel->setQuery(selectsql
                         .arg(QDateTime(ui->dateTimeEdit->date()).toString("yyyy-MM-dd hh:mm:ss"))
                         .arg(QDateTime(ui->dateTimeEdit_2->date(), QTime(23,59,59)).toString("yyyy-MM-dd hh:mm:ss"))
                         );
    //更新表头
    backheader::modelSetHeader(querymodel, QStringList() << tr("状态") << tr("单号") << tr("业务类型") << tr("经手人") << tr("制单人") << tr("制单日期") << tr("审核人") << tr("审核日期")<< tr("备注"));
    //为空则选中第一行，否则遍历选中sheetno所在行
    if(findsheetno.isEmpty()){
        ui->tableView->restore();
    } else {
        for(int r=0,r_count=querymodel->rowCount();r<r_count;r++){
            if(querymodel->record(r).value("ch_sheetNo").toString()==findsheetno){
                ui->tableView->selectRow(r);
                break;
            }
        }
    }
    //更新tableview 2的数据
    toupdatetableview2(ui->tableView->currentIndex().row());
}

void w_i_inventory_goodsinput::toexit()
{
    this->reject();
}

void w_i_inventory_goodsinput::toupdatetableview2(int index)
{
    QString filtersql;
    if(index>=0){
        filtersql=querymodel->record(index).value("ch_sheetNo").toString();
    }

    //sql语句 select
    QString selectsql=
            " select"
            " cey_st_sheet_detail.ch_materialno, "
            " cey_bt_dish.vch_dishname, "
            " (select vch_unitname from cey_bt_unit where cey_bt_unit.ch_unitno=cey_bt_dish.ch_unitno), "
            " cey_st_sheet_detail.num_wrapping, "
            " (select vch_unitname from cey_bt_unit where cey_bt_unit.ch_unitno=cey_bt_dish.vch_pur_unitno), "
            " ifnull(cey_st_sheet_detail.num_num,0), "
            " cey_st_sheet_detail.num_present, "
            " cey_st_sheet_detail.num_price, "
            " round(ifnull(cey_st_sheet_detail.num_num, 0)*cey_st_sheet_detail.num_price, 2), "
            " cey_st_sheet_detail.vch_memo"
            //            " cey_st_sheet_detail.int_id"

            " FROM cey_st_sheet_detail, "
            " cey_bt_dish "
            " WHERE(cey_st_sheet_detail.ch_materialno = cey_bt_dish.ch_dishno)"
            " and(ch_sheetno = '%1') " ;

    backheader::standmodelfilldata(standmodel_2,
                                   selectsql
                                   .arg(filtersql));
    qDebug() << selectsql.arg(filtersql);
    //合计
    ui->tableView_2->updateSum(8);
    //更新表头
    standmodel_2->setHorizontalHeaderLabels(QStringList() << tr("编码") << tr("名称") << tr("库存单位") << tr("数量(辅)") << tr("入库单位") << tr("数量(主)") << tr("赠送数量") << tr("单价")<< tr("小计金额")<< tr("备注"));

    //tableview2 restore
    ui->tableView_2->tableView->restore();
}

//==============================
#include "ui_w_i_inventory_goodsinput_change_dialog.h"
w_i_inventory_goodsinput_virtual::w_i_inventory_goodsinput_virtual(w_i_inventory_goodsinput *parent):
    QDialog(parent),
    ui(new Ui_w_i_inventory_goodsinput_change_Dialog),
    _parent(parent),
    standmodel(0)
{
    ui->setupUi(this);
    ui->frame_2->setObjectName("inventory");

    //导入数据
    ui->comboBox_ch_operation->setModel(&w_i_inventory::MODEL_INVENTORY_TYPE);
    ui->comboBox_ch_operation->setModelColumn(1);
    ui->comboBox_ch_operation->setCurrentIndex(backheader::VIPFINDVALUE(w_i_inventory::MODEL_INVENTORY_TYPE, "001", 0));

    ui->comboBox_vch_handle->addItems(inventory_queryfilter("Select vch_operID,vch_operator from cey_sys_operator union Select '0000' as vch_operID,'system' as vch_operator "));
    //    ui->frame_2->setEnabled(false);
    //! toolbar
    ui->widget_2->setup(QStringList() << tr("行增加") << tr("行删除")<<tr("修改")<< tr("选择物料") << tr("保存")<< tr("审核") << tr("作废"));
    connect(ui->widget_2->index_widget(tr("行增加")),SIGNAL(clicked()),this,SLOT(toadd()));
    connect(ui->widget_2->index_widget(tr("行删除")),SIGNAL(clicked()),this,SLOT(todel()));
    connect(ui->widget_2->index_widget(tr("修改")),SIGNAL(clicked()),this,SLOT(tochange()));
    //    connect(ui->widget_2->index_widget(tr("选择物料")),SIGNAL(clicked()),this,SLOT(togoods()));
    connect(ui->widget_2->index_widget(tr("保存")),SIGNAL(clicked()),this,SLOT(tosave()));
    connect(ui->widget_2->index_widget(tr("审核")),SIGNAL(clicked()),this,SLOT(tosheet()));
    connect(ui->widget_2->index_widget(tr("作废")),SIGNAL(clicked()),this,SLOT(tobad()));

    ui->widget_2->setselectgoodsenable_itemflag("选择物料");
    connect(ui->widget_2,SIGNAL(okstrs(QStringList)),this,SLOT(togoods(QStringList)));


    //! after
    ui->tableView->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->lineEdit_ch_sheetno->setEnabled(false);
    ui->comboBox_ch_operation->setEnabled(false);
    ui->lineEdit_dt_audit->setEnabled(false);
    ui->lineEdit_dt_operdate->setEnabled(false);
    ui->lineEdit_vch_audit->setEnabled(false);
    ui->lineEdit_vch_operid->setEnabled(false);
}

void w_i_inventory_goodsinput_virtual::infoupdate(int tableview_row)
{
    if(tableview_row>=0){
        //判断是 未审核； 已审核； 已作废
        if(_parent->getQuerymodel()->record(tableview_row).value("ch_state").toString()=="N"){//未审核；
            ui->frame_2->drawPixmap(QPixmap());
        } else  if(_parent->getQuerymodel()->record(tableview_row).value("ch_state").toString()=="Y"){//已审核；
            ui->frame_2->drawPixmap(QPixmap(":/image/widgets_pic/haschecked"));
        } else  if(_parent->getQuerymodel()->record(tableview_row).value("ch_state").toString()=="O"){//已作废
            ui->frame_2->drawPixmap(QPixmap(":/image/widgets_pic/hasbaded"));
        }

        QSqlQuery query;
        query.exec(QString("select *from cey_st_sheet_master where ch_sheetNo='%1' ")
                   .arg(_parent->getQuerymodel()->record(tableview_row).value("ch_sheetNo").toString())
                   );
        query.next();
        ui->lineEdit_ch_sheetno->setText(query.record().value("ch_sheetNo").toString());
        ui->comboBox_vch_handle->setindex_c1(query.record().value("vch_handle").toString());
        ui->lineEdit_vch_memo->setText(query.record().value("vch_memo").toString());
        ui->lineEdit_vch_operid->setText(inventory_getopername(query.record().value("vch_operid").toString()));
        ui->lineEdit_dt_operdate->setText(query.record().value("dt_operdate").toDateTime().toString("yyyy-MM-dd hh:mm:ss"));
        ui->lineEdit_vch_audit->setText(query.record().value("vch_audit").toString());
        ui->lineEdit_dt_audit->setText(query.record().value("dt_audit").toDateTime().toString("yyyy-MM-dd hh:mm:ss"));

        if(standmodel==0){
            standmodel=_parent->getStandmodel_2();
            ui->tableView->setEditTriggers(QTableView::NoEditTriggers);ui->tableView->setModel(standmodel);
        }
    }

}

void w_i_inventory_goodsinput_virtual::infoupdate(const QString &ls_sheetno)
{
    QSqlQuery query;
    QString ls_value;
    query.exec(QString("select ch_state from cey_st_sheet_master where ch_sheetNo='%1' ").arg(ls_sheetno));
    query.next();
    ls_value=query.record().value("ch_state").toString();
    if(ls_value=="N"){
        ui->frame_2->drawPixmap(QPixmap());
    } else if(ls_value=="Y"){
        ui->frame_2->drawPixmap(QPixmap(":/image/widgets_pic/haschecked"));
    } else if(ls_value=="O"){
        ui->frame_2->drawPixmap(QPixmap(":/image/widgets_pic/hasbaded"));
    }
}

void w_i_inventory_goodsinput_virtual::tosheet()//审核
{
    //ue_sheet_audit (none) returns (none)
    QString ls_sheetno;
    QString ls_errtext;
    QString ls_find;
    qlonglong ll_found;
    QDateTime ldt_datetime;

    if(ui->lineEdit_ch_sheetno->text().isEmpty()){
        lds_messagebox::warning(this, tr("消息提示"),tr("单据号码不能为空，请先保存!"));
        return;
    }
    ls_sheetno=ui->lineEdit_ch_sheetno->text();
    if(_parent->getStandmodel_2()->rowCount()<=0){
        lds_messagebox::warning(this, tr("消息提示"),tr("空单据不允许审核!"));
        return;
    }

    //判断该单据中的int_id 是1 - max 排序的
    QSqlQuery query;
    query.exec(QString("select min(int_id) from cey_st_sheet_detail where cey_st_sheet_detail.ch_sheetno='%1' ")
               .arg(ui->lineEdit_ch_sheetno->text())
               );
    query.next();
    if(query.record().value(0).toLongLong()!=1){
        lds_messagebox::warning(this, tr("消息提示"),tr("未保存的单据不允许直接审核!"));
        return;
    }
    query.exec(QString("select max(int_id), count(int_id) from cey_st_sheet_detail where cey_st_sheet_detail.ch_sheetno='%1' ")
               .arg(ui->lineEdit_ch_sheetno->text())
               );
    query.next();
    if(query.record().value(0).toLongLong()!=query.record().value(1).toLongLong()){
        lds_messagebox::warning(this, tr("消息提示"),tr("未保存的单据不允许直接审核!"));
        return;
    }

    if(_parent->getQuerymodel()->record(_parent->getTableview()->currentIndex().row()).value("ch_state").toString()!="N"){
        lds_messagebox::warning(this, tr("消息提示"),tr("单据已经审核!"));
        return;
    }

    if(_parent->_type=="RK"
            || _parent->_type=="TH"){//-1
        for(int r=0,r_count=_parent->getStandmodel_2()->rowCount();r<r_count;r++){//0
            if(
                    (
                        _parent->getStandmodel_2()->item(r, 5)->text().toDouble() < 0//数量
                        || _parent->getStandmodel_2()->item(r, 6)->text().toDouble() < 0//赠送数量
                        )
                    ||(
                        _parent->getStandmodel_2()->item(r, 5)->text().toDouble() == 0//数量
                        && _parent->getStandmodel_2()->item(r, 6)->text().toDouble() == 0//赠送数量
                        )
                    ){
                lds_messagebox::warning(this, tr("消息提示"),tr("数量(主)必须大于零，请修改后再审核!"));
                return;
            }
        }//0
    } else {//-1
        for(int r=0,r_count=_parent->getStandmodel_2()->rowCount();r<r_count;r++){//0
            if(_parent->getStandmodel_2()->item(r, 5)->text().toDouble() <= 0//数量
                    ){
                lds_messagebox::warning(this, tr("消息提示"),tr("数量(主)必须大于零，请修改后再审核!"));
                return;
            }
        }//0
    }//-1

    if(0==lds_messagebox::warning(this, tr("消息提示"),tr("确认要审核单据[%1]").arg(ls_sheetno), tr("确认"),tr("取消")
                                  )){
        ldt_datetime = n_func::f_get_sysdatetime();
        //! <--transaction begin-->
        QSqlQuery query;
        QSqlDatabase::database().transaction();
        QString errsql;

        query.prepare("update cey_st_sheet_master  set ch_state ='Y' , vch_audit =:gs_operid , dt_audit =:ldt_datetime  where ch_sheetNo =:ls_sheetno and ch_state ='N'");
        query.bindValue(":gs_operid", n_func::gs_operid);
        query.bindValue(":ldt_datetime", ldt_datetime);
        query.bindValue(":ls_sheetno", ls_sheetno);
        if(query.exec()){
            QSqlDatabase::database().commit();
            _parent->torefresh();
            ui->tableView->tableView->restore();
            w_i_inventory_goodsinput_virtual::infoupdate(ls_sheetno);
            lds_messagebox::warning(this, tr("消息提示"),tr("单据[%1]已经审核!").arg(ls_sheetno));

        } else {
            QSqlDatabase::database().rollback();
            _parent->torefresh();
            ui->tableView->tableView->restore();
            lds_messagebox::warning(this, tr("消息提示"),tr("单据[%1]的状态已被更改，审核失败!").arg(ls_sheetno));
        }
        //! <--transaction end-->
    }
}

void w_i_inventory_goodsinput_virtual::tobad()
{
    //ue_sheet_blankout (none) returns (none)
    qlonglong ll_found;
    QString ls_sheetno;
    QString ls_errtext;
    QDateTime ldt_datetime;


    if(ui->lineEdit_ch_sheetno->text().isEmpty()){
        lds_messagebox::warning(this, tr("消息提示"),tr("单据号码不能为空，请先保存!"));
        return;
    }
    if(_parent->getStandmodel_2()->rowCount()<=0){
        lds_messagebox::warning(this, tr("消息提示"),tr("空单据不允许作废!"));
        return;
    }
    //判断该单据中的int_id 是1 - max 排序的
    QSqlQuery query;
    query.exec(QString("select min(int_id) from cey_st_sheet_detail where cey_st_sheet_detail.ch_sheetno='%1' ")
               .arg(ui->lineEdit_ch_sheetno->text())
               );
    query.next();
    if(query.record().value(0).toLongLong()!=1){
        lds_messagebox::warning(this, tr("消息提示"),tr("未保存的单据不允许直接作废!"));
        return;
    }
    query.exec(QString("select max(int_id), count(int_id) from cey_st_sheet_detail where cey_st_sheet_detail.ch_sheetno='%1' ")
               .arg(ui->lineEdit_ch_sheetno->text())
               );
    query.next();
    if(query.record().value(0).toLongLong()!=query.record().value(1).toLongLong()){
        lds_messagebox::warning(this, tr("消息提示"),tr("未保存的单据不允许直接作废!"));
        return;
    }

    if(_parent->getQuerymodel()->record(_parent->getTableview()->currentIndex().row()).value("ch_state").toString()!="N"){
        return;
    }

    ls_sheetno=ui->lineEdit_ch_sheetno->text();
    if(0==lds_messagebox::warning(this, tr("消息提示"),tr("确认要作废单据[%1]")
                                  .arg(ls_sheetno), tr("确认"),tr("取消")
                                  )){
        ldt_datetime = n_func::f_get_sysdatetime();
        //! <--transaction begin-->
        QSqlQuery query;
        QSqlDatabase::database().transaction();
        QString errsql;

        query.prepare("update cey_st_sheet_master   set ch_state ='O' , vch_audit =:gs_operid , dt_audit =:ldt_datetime  where ch_sheetNo =:ls_sheetno and ch_state ='N' ");
        query.bindValue(":gs_operid", n_func::gs_operid);
        query.bindValue(":ldt_datetime", ldt_datetime);
        query.bindValue(":ls_sheetno", ls_sheetno);
        if(query.exec()){
            QSqlDatabase::database().commit();
            _parent->torefresh(ls_sheetno);
            ui->tableView->tableView->restore();
            w_i_inventory_goodsinput_virtual::infoupdate(ls_sheetno);
            lds_messagebox::warning(this, tr("消息提示"),tr("单据[%1]已经作废!").arg(ls_sheetno));

        } else {
            QSqlDatabase::database().rollback();
            _parent->torefresh();
            ui->tableView->tableView->restore();
            lds_messagebox::warning(this, tr("消息提示"),tr("单据[%1]的状态已被更改，作废失败!").arg(ls_sheetno));
        }
        //! <--transaction end-->
    }
}


void w_i_inventory_goodsinput_virtual::toup()
{
    _parent->getTableview()->goprevious();
    infoupdate(_parent->getTableview()->currentIndex().row());
}

void w_i_inventory_goodsinput_virtual::todown()
{
    _parent->getTableview()->gonext();
    infoupdate(_parent->getTableview()->currentIndex().row());

}

void w_i_inventory_goodsinput_virtual::toexit()
{
    this->reject();
}

//==============================

w_i_inventory_goodsinput_change::w_i_inventory_goodsinput_change(w_i_inventory_goodsinput *parent):
    w_i_inventory_goodsinput_virtual(parent)
{
    typewindow();
    QTimer::singleShot(100, this,SLOT(infoupdate()));
}

void w_i_inventory_goodsinput_change::toadd()
{
    w_i_inventory_goodsinput_change_change dialog(this);
    while(backheader::static_dialogexec(dialog,qobject_cast<QAbstractButton* >(sender())->text()) == QDialog::Accepted){

        if(dialog.ui->widget_ch_dishno->gettext().isEmpty()
                || dialog.ui->lineEdit_subnum->text().isEmpty()
                || dialog.ui->lineEdit_mainnum->text().isEmpty()
                || dialog.ui->lineEdit_presentnum->text().isEmpty()
                || dialog.ui->lineEdit_price->text().isEmpty()
                ){
            lds_messagebox::warning(this, tr("消息提示"),tr("有数据为空!"));
            continue;
        }

        QSqlQuery query;
        //! <--transaction begin-->
        QSqlDatabase::database().transaction();
        query.exec(QString("select max(int_id) from cey_st_sheet_detail where ch_sheetno='%1' ")
                   .arg(ui->lineEdit_ch_sheetno->text())
                   );
        query.next();
        qlonglong int_idmax=query.record().value(0).toLongLong();
        int_idmax++;
        if(query.exec(QString("insert into cey_st_sheet_detail(ch_sheetno, ch_materialno, num_wrapping, num_num, num_present ,num_price,vch_memo, int_id )values('%1','%2','%3','%4','%5','%6','%7' , '%8' )")
                      .arg(ui->lineEdit_ch_sheetno->text())
                      .arg(dialog.ui->widget_ch_dishno->gettext())
                      .arg(dialog.ui->lineEdit_subnum->text())
                      .arg(dialog.ui->lineEdit_mainnum->text())
                      .arg(dialog.ui->lineEdit_presentnum->text())
                      .arg(dialog.ui->lineEdit_price->text())
                      .arg(dialog.ui->lineEdit_vch_memo->text())
                      .arg(int_idmax)

                      )){
            QSqlDatabase::database().commit();
            _parent->torefresh();
        } else {
            QSqlDatabase::database().rollback();
            _parent->torefresh();
            lds_messagebox::warning(this, tr("消息提示"),tr("数据有误，操作失败!"), query.lastError().text());
            qDebug() << query.lastQuery();
        }
        ui->tableView->tableView->restore();
        //! <--transaction end-->
        break;
    }
}

void w_i_inventory_goodsinput_change::todel()
{
    int row=ui->tableView->tableView->currentIndex().row();
    if(row>=0){
        QSqlQuery query;
        //! <--transaction begin-->
        QSqlDatabase::database().transaction();

        if(query.exec(QString("delete from  cey_st_sheet_detail  where cey_st_sheet_detail.ch_sheetno='%1' and cey_st_sheet_detail.int_id='%2' ")
                      .arg(ui->lineEdit_ch_sheetno->text())
                      .arg(ui->tableView->tableView->model()->index(row, 10).data().toString())

                      )){
            qDebug() << query.lastQuery();
            QSqlDatabase::database().commit();
            _parent->torefresh();
        } else {
            QSqlDatabase::database().rollback();
            _parent->torefresh();
            lds_messagebox::warning(this, tr("消息提示"),tr("数据有误，操作失败!"), query.lastError().text());
        }
        ui->tableView->tableView->restore();
        //! <--transaction end-->
    }
}

void w_i_inventory_goodsinput_change::tochange()
{
    int row=ui->tableView->tableView->currentIndex().row();
    if(row>=0){
        w_i_inventory_goodsinput_change_change dialog(this);
        dialog.ui->widget_ch_dishno->setEnabled(false);
        dialog.ui->widget_ch_dishno->settext(ui->tableView->tableView->model()->index(row, 0).data().toString());
        dialog.ui->lineEdit_vch_dishname->setText(ui->tableView->tableView->model()->index(row, 1).data().toString());
        dialog.ui->lineEdit_subnum->setText(ui->tableView->tableView->model()->index(row, 3).data().toString());
        dialog.ui->lineEdit_mainnum->setText(ui->tableView->tableView->model()->index(row, 5).data().toString());
        dialog.ui->lineEdit_presentnum->setText(ui->tableView->tableView->model()->index(row, 6).data().toString());
        dialog.ui->lineEdit_price->setText(ui->tableView->tableView->model()->index(row, 7).data().toString());
        dialog.ui->lineEdit_vch_memo->setText(ui->tableView->tableView->model()->index(row, 9).data().toString());
        if(backheader::static_dialogexec(dialog,qobject_cast<QAbstractButton* >(sender())->text()) == QDialog::Accepted){
            QSqlQuery query;
            //! <--transaction begin-->
            QSqlDatabase::database().transaction();

            if(query.exec(QString("update cey_st_sheet_detail set num_num='%1',num_present='%2',num_price='%3',num_wrapping='%6',vch_memo='%7' where cey_st_sheet_detail.ch_sheetno='%4' and ch_materialno='%5' ")
                          .arg(dialog.ui->lineEdit_mainnum->text())
                          .arg(dialog.ui->lineEdit_presentnum->text())
                          .arg(dialog.ui->lineEdit_price->text())
                          .arg(ui->lineEdit_ch_sheetno->text())
                          .arg(ui->tableView->tableView->model()->index(row, 0).data().toString())
                          .arg(dialog.ui->lineEdit_subnum->text())
                          .arg(dialog.ui->lineEdit_vch_memo->text())

                          )){
                QSqlDatabase::database().commit();
                _parent->torefresh();
            } else {
                QSqlDatabase::database().rollback();
                _parent->torefresh();
                lds_messagebox::warning(this, tr("消息提示"),tr("数据有误，操作失败!"), query.lastError().text());
            }
            ui->tableView->tableView->restore();
            //! <--transaction end-->
        }
    }
}

void w_i_inventory_goodsinput_change::togoods(const QStringList &texts)
{
    //传过来的是 no
    QSqlQuery query;
    //! <--transaction begin-->
    QSqlDatabase::database().transaction();
    QString errsql;
    foreach(QString text, texts){//foreach
        query.exec(QString("select max(int_id) from cey_st_sheet_detail where ch_sheetno='%1' ")
                   .arg(ui->lineEdit_ch_sheetno->text())
                   );
        query.next();
        qlonglong int_idmax=query.record().value(0).toLongLong();
        int_idmax++;
        if(query.exec(QString("insert into cey_st_sheet_detail(ch_sheetno, ch_materialno, int_id )values('%1','%2','%3' )")
                      .arg(ui->lineEdit_ch_sheetno->text())
                      .arg(text)
                      .arg(int_idmax)
                      )){
        } else {
            errsql=query.lastError().text();
            break;
        }
    }//foreach
    if(errsql.isEmpty()){
        QSqlDatabase::database().commit();
        _parent->torefresh();
    } else {
        QSqlDatabase::database().rollback();
        _parent->torefresh();
        lds_messagebox::warning(this, tr("消息提示"),tr("数据有误，操作失败!"), errsql);
    }
    ui->tableView->tableView->restore();
    //! <--transaction end-->
}

bool w_i_inventory_goodsinput_change::tosave()
{
    //ue_sheet_save (none) returns integer
    //integer ancestorreturnvalue
    QString ls_sheetno;
    qlonglong ll_row;
    qlonglong ll_found;

    ll_row=_parent->getTableview()->currentIndex().row();
    if(ll_row>=0){
        if(_parent->getQuerymodel()->record(ll_row).value("ch_sheetType")=="CK"){
            if(ui->comboBox_ch_operation->currentText().isEmpty()){
                lds_messagebox::warning(this, tr("消息提示"),tr("出库类型不能为空!"));
                return false;
            }
        }
    }
    if(ui->comboBox_vch_handle->currentText().isEmpty()){
        lds_messagebox::warning(this, tr("消息提示"),tr("经手人不能为空，请选择后再保存!"));
        return false;
    }

    if(_parent->getStandmodel_2()->rowCount() <=0){
        lds_messagebox::warning(this, tr("消息提示"),tr("无物料记录，不允保存!"));
        return false;
    }

    for(int r=0,r_count=_parent->getStandmodel_2()->rowCount();r<r_count;r++){
        if(_parent->getStandmodel_2()->item(r, 7)->text().toDouble() <= 0.0 ){
            lds_messagebox::warning(this, tr("消息提示"),tr("单价必须大于零，请修改!"));
            return false;
        }
    }

    ls_sheetno=ui->lineEdit_ch_sheetno->text();

    //! <--transaction begin-->
    QSqlQuery query;
    QSqlDatabase::database().transaction();
    QString errsql;

    if(query.exec(QString("update cey_st_sheet_master set vch_handle='%1' , vch_memo='%2' where ch_sheetNo='%3' ")
                  .arg(backheader::getinterdata(ui->comboBox_vch_handle->currentText()))
                  .arg(ui->lineEdit_vch_memo->text())
                  .arg(ui->lineEdit_ch_sheetno->text())
                  )){//1
        if(query.exec(QString("delete from  cey_st_sheet_detail  where cey_st_sheet_detail.ch_sheetno='%1'  ")
                      .arg(ui->lineEdit_ch_sheetno->text())
                      )){
            for(int r=0,r_count=_parent->getStandmodel_2()->rowCount();r<r_count;r++){//0
                if(query.exec(QString("insert into cey_st_sheet_detail(ch_sheetno, ch_materialno, num_wrapping, num_num, num_present ,num_price,vch_memo, int_id )values('%1','%2','%3','%4','%5','%6','%7' , '%8' )")
                              .arg(ui->lineEdit_ch_sheetno->text())
                              .arg(_parent->getStandmodel_2()->item(r, 0)->text())//编码
                              .arg(_parent->getStandmodel_2()->item(r,3)->text())//数量辅
                              .arg(_parent->getStandmodel_2()->item(r,5)->text())//数量主
                              .arg(_parent->getStandmodel_2()->item(r,6)->text())//数量赠送
                              .arg(_parent->getStandmodel_2()->item(r,7)->text())//价格
                              .arg(_parent->getStandmodel_2()->item(r,9)->text())//备注
                              .arg(r+1)//int_d
                              )){
                } else {
                    errsql=query.lastError().text();
                    break;
                }
            }//0
        } else {
            errsql=query.lastError().text();
        }
    } else {
        errsql=query.lastError().text();
    }//1


    if(errsql.isEmpty()){
        QSqlDatabase::database().commit();
        _parent->torefresh();
        ui->tableView->tableView->restore();
        lds_messagebox::warning(this, tr("消息提示"),tr("保存成功!"));
        return true;
    } else {
        QSqlDatabase::database().rollback();
        _parent->torefresh();
        ui->tableView->tableView->restore();
        lds_messagebox::warning(this, tr("消息提示"),tr("数据有误，操作失败!"), errsql);
        return false;
    }
    //! <--transaction end-->
}

void w_i_inventory_goodsinput_change::infoupdate()
{
    w_i_inventory_goodsinput_virtual::infoupdate(_parent->getTableview()->currentIndex().row());
}

void w_i_inventory_goodsinput_change::typewindow()
{
    ui->widget->setup(QStringList() << tr("上一页") << tr("下一页") << tr("退出"), Qt::AlignTop);
    connect(ui->widget->index_widget(tr("上一页")),SIGNAL(clicked()),this,SLOT(toup()));
    connect(ui->widget->index_widget(tr("下一页")),SIGNAL(clicked()),this,SLOT(todown()));
    connect(ui->widget->index_widget(tr("退出")),SIGNAL(clicked()),this,SLOT(toexit()));
}

//============================
w_i_inventory_goodsinput_change_change::w_i_inventory_goodsinput_change_change(QWidget *parent):
    QDialog(parent),
    ui(new Ui_w_i_inventory_goodsinput_subchange_Dialog)
{
    ui->setupUi(this);
    //! toolbar
    ui->widget->setup(QStringList() << tr("确定") << tr("取消"), Qt::AlignRight);
    connect(ui->widget->index_widget(tr("确定")),SIGNAL(clicked()),this,SLOT(accept()));
    connect(ui->widget->index_widget(tr("取消")),SIGNAL(clicked()),this,SLOT(reject()));
}


w_i_inventory_goodsinput_new::w_i_inventory_goodsinput_new(w_i_inventory_goodsinput *parent):
    w_i_inventory_goodsinput_virtual(parent)
{
    typewindow();
    QTimer::singleShot(100, this,SLOT(infoupdate()));
}

void w_i_inventory_goodsinput_new::infoupdate()
{
    if(standmodel)
        standmodel->removeRows(0, standmodel->rowCount());
    if(standmodel==0){
        standmodel = new QStandardItemModel;
        standmodel->setHorizontalHeaderLabels(QStringList() << tr("编码") << tr("名称") << tr("库存单位") << tr("数量(辅)") << tr("入库单位") << tr("数量(主)") << tr("赠送数量") << tr("单价")<< tr("小计金额")<< tr("备注"));

        ui->tableView->setEditTriggers(QTableView::NoEditTriggers);ui->tableView->setModel(standmodel);
    }

    ui->frame_2->setEnabled(true);
    ui->lineEdit_ch_sheetno->clear();
    ui->lineEdit_dt_audit->clear();
    ui->lineEdit_dt_operdate->clear();
    ui->lineEdit_vch_audit->clear();
    ui->lineEdit_vch_memo->clear();
    ui->lineEdit_vch_operid->clear();

    ui->lineEdit_vch_operid->setText(n_func::gs_opername);
    ui->lineEdit_dt_operdate->setText(n_func::f_get_sysdatetime().toString("yyyy-MM-dd hh:mm:ss"));
}

void w_i_inventory_goodsinput_new::typewindow()
{
    ui->widget->setup(QStringList() << tr("新增单据") << tr("退出"), Qt::AlignTop);
    connect(ui->widget->index_widget(tr("新增单据")),SIGNAL(clicked()),this,SLOT(infoupdate()));
    connect(ui->widget->index_widget(tr("退出")),SIGNAL(clicked()),this,SLOT(toexit()));
}

void w_i_inventory_goodsinput_new::toadd()
{
    w_i_inventory_goodsinput_change_change dialog(this);
    while(backheader::static_dialogexec(dialog,qobject_cast<QAbstractButton* >(sender())->text()) == QDialog::Accepted){

        if(dialog.ui->widget_ch_dishno->gettext().isEmpty()
                || dialog.ui->lineEdit_subnum->text().isEmpty()
                || dialog.ui->lineEdit_mainnum->text().isEmpty()
                || dialog.ui->lineEdit_presentnum->text().isEmpty()
                || dialog.ui->lineEdit_price->text().isEmpty()
                ){
            lds_messagebox::warning(this, tr("消息提示"),tr("有数据为空!"));
            continue;
        }
        QSqlQuery query;
        query.exec(QString("select "
                           " cey_bt_dish.vch_dishname, "
                           " (select vch_unitname from cey_bt_unit where cey_bt_unit.ch_unitno=cey_bt_dish.ch_unitno), "
                           " (select vch_unitname from cey_bt_unit where cey_bt_unit.ch_unitno=cey_bt_dish.vch_pur_unitno) "
                           " FROM cey_bt_dish "
                           " where cey_bt_dish.ch_dishno='%1 '"
                           ).arg(dialog.ui->widget_ch_dishno->gettext()));
        query.next();

        QList<QStandardItem* > items;
        items<<new QStandardItem(dialog.ui->widget_ch_dishno->gettext());

        items<<new QStandardItem(query.record().value(0).toString());
        items<<new QStandardItem(query.record().value(1).toString());

        items<<new QStandardItem(dialog.ui->lineEdit_subnum->text());

        items<<new QStandardItem(query.record().value(2).toString());

        items<<new QStandardItem(dialog.ui->lineEdit_mainnum->text());
        items<<new QStandardItem(dialog.ui->lineEdit_presentnum->text());
        items<<new QStandardItem(dialog.ui->lineEdit_price->text());

        items<<new QStandardItem(QString().sprintf("%.02f", dialog.ui->lineEdit_price->text().toDouble() * dialog.ui->lineEdit_mainnum->text().toDouble()));

        items<<new QStandardItem(dialog.ui->lineEdit_vch_memo->text());

        standmodel->appendRow(items);
        break;
    }
}

void w_i_inventory_goodsinput_new::todel()
{
    if(ui->tableView->tableView->currentIndex().row()>=0){
        standmodel->removeRow(ui->tableView->tableView->currentIndex().row());
    } else {
        lds_messagebox::warning(this, tr("消息提示"),tr("没有数据被选中!"));
    }
}

void w_i_inventory_goodsinput_new::tochange()
{
    int row=ui->tableView->tableView->currentIndex().row();
    if(row>=0){
        w_i_inventory_goodsinput_change_change dialog(this);
        dialog.ui->widget_ch_dishno->setEnabled(false);
        dialog.ui->widget_ch_dishno->settext(standmodel->index(row, 0).data().toString());
        dialog.ui->lineEdit_vch_dishname->setText(standmodel->index(row, 1).data().toString());
        dialog.ui->lineEdit_subnum->setText(standmodel->index(row, 3).data().toString());
        dialog.ui->lineEdit_mainnum->setText(standmodel->index(row, 5).data().toString());
        dialog.ui->lineEdit_presentnum->setText(standmodel->index(row, 6).data().toString());
        dialog.ui->lineEdit_price->setText(standmodel->index(row, 7).data().toString());
        dialog.ui->lineEdit_vch_memo->setText(standmodel->index(row, 9).data().toString());
        if(backheader::static_dialogexec(dialog,qobject_cast<QAbstractButton* >(sender())->text()) == QDialog::Accepted){
            standmodel->item(row, 3)->setText(dialog.ui->lineEdit_subnum->text());
            standmodel->item(row, 5)->setText(dialog.ui->lineEdit_mainnum->text());
            standmodel->item(row, 6)->setText(dialog.ui->lineEdit_presentnum->text());
            standmodel->item(row, 7)->setText(dialog.ui->lineEdit_price->text());
            standmodel->item(row, 8)->setText(QString().sprintf("%.02f", dialog.ui->lineEdit_price->text().toDouble() * dialog.ui->lineEdit_mainnum->text().toDouble()));

            standmodel->item(row, 9)->setText(dialog.ui->lineEdit_vch_memo->text());
        }
    }
}

void w_i_inventory_goodsinput_new::togoods(const QStringList &texts)
{
    //传过来的是 no
    QSqlQuery query;
    foreach(QString text, texts){//foreach
        query.exec(QString("select "
                           " cey_bt_dish.vch_dishname, "
                           " (select vch_unitname from cey_bt_unit where cey_bt_unit.ch_unitno=cey_bt_dish.ch_unitno), "
                           " (select vch_unitname from cey_bt_unit where cey_bt_unit.ch_unitno=cey_bt_dish.vch_pur_unitno) "
                           " FROM cey_bt_dish "
                           " where cey_bt_dish.ch_dishno='%1 '"
                           ).arg(text));
        query.next();

        QList<QStandardItem* > items;
        items<<new QStandardItem(text);

        items<<new QStandardItem(query.record().value(0).toString());
        items<<new QStandardItem(query.record().value(1).toString());

        items<<new QStandardItem("0");

        items<<new QStandardItem(query.record().value(2).toString());

        items<<new QStandardItem("0");
        items<<new QStandardItem("0");
        items<<new QStandardItem("0");

        items<<new QStandardItem("0");

        items<<new QStandardItem("");

        standmodel->appendRow(items);
    }
    ui->tableView->tableView->restore();
}

bool w_i_inventory_goodsinput_new::tosave()
{
    //ue_sheet_save (none) returns integer
    //integer ancestorreturnvalue
    QString ls_sheetno;
    qlonglong ll_row;
    qlonglong ll_found;


    if(ui->comboBox_vch_handle->currentText().isEmpty()){
        lds_messagebox::warning(this, tr("消息提示"),tr("经手人不能为空，请选择后再保存!"));
        return false;
    }

    if(standmodel->rowCount() <=0){
        lds_messagebox::warning(this, tr("消息提示"),tr("无物料记录，不允保存!"));
        return false;
    }

    for(int r=0,r_count=standmodel->rowCount();r<r_count;r++){
        if(standmodel->item(r, 7)->text().toDouble() <= 0.0 ){
            lds_messagebox::warning(this, tr("消息提示"),tr("单价必须大于零，请修改!"));
            return false;
        }
    }

    ls_sheetno=getnewsheetno();


    //! <--transaction begin-->
    QSqlQuery query;
    QSqlDatabase::database().transaction();
    QString errsql;

    //增加cey_st_sheet_master
    if(query.exec(QString("INSERT INTO cey_st_sheet_master(ch_sheetNo, ch_sheetType, ch_operation, vch_handle, vch_operId, dt_operdate, ch_state)VALUES('%1', '%5', '001', '%2', '%3', '%4', 'N' )")
                  .arg(ls_sheetno)
                  .arg(backheader::getinterdata(ui->comboBox_vch_handle->currentText()))
                  .arg(n_func::gs_operid)
                  .arg(n_func::f_get_sysdatetime().toString("yyyy-MM-dd hh:mm:ss"))
                  .arg(_parent->_type)

                  )
            ){//1
        //删除cey_st_sheet_detail
        if(query.exec(QString("delete from  cey_st_sheet_detail  where cey_st_sheet_detail.ch_sheetno='%1'  ")
                      .arg(ls_sheetno)
                      )){
            //增加cey_st_sheet_detail
            for(int r=0,r_count=standmodel->rowCount();r<r_count;r++){//0
                if(query.exec(QString("insert into cey_st_sheet_detail(ch_sheetno, ch_materialno, num_wrapping, num_num, num_present ,num_price,vch_memo, int_id )values('%1','%2','%3','%4','%5','%6','%7' , '%8' )")
                              .arg(ls_sheetno)
                              .arg(standmodel->item(r, 0)->text())//编码
                              .arg(standmodel->item(r,3)->text())//数量辅
                              .arg(standmodel->item(r,5)->text())//数量主
                              .arg(standmodel->item(r,6)->text())//数量赠送
                              .arg(standmodel->item(r,7)->text())//价格
                              .arg(standmodel->item(r,9)->text())//备注
                              .arg(r+1)//int_d
                              )){
                    qDebug() << query.lastQuery();
                } else {
                    errsql=query.lastError().text();
                    break;
                }
            }//0
        } else {
            errsql=query.lastError().text();
        }
    } else {
        errsql=query.lastError().text();
    }//1


    if(errsql.isEmpty()){
        QSqlDatabase::database().commit();
        _parent->torefresh(ls_sheetno);
        ui->tableView->tableView->restore();
        ui->lineEdit_ch_sheetno->setText(ls_sheetno);
        lds_messagebox::warning(this, tr("消息提示"),tr("保存成功!"));
        return true;
    } else {
        QSqlDatabase::database().rollback();
        _parent->torefresh();
        ui->tableView->tableView->restore();
        lds_messagebox::warning(this, tr("消息提示"),tr("数据有误，操作失败!"), errsql);
        return false;
    }
    //! <--transaction end-->
}

//void w_i_inventory_goodsinput_new::tosheet()
//{

//}

//void w_i_inventory_goodsinput_new::tobad()
//{

//}

QString w_i_inventory_goodsinput_new::getnewsheetno()
{
    QDateTime dt=n_func::f_get_sysdatetime();
    QString new_sheetno=_parent->_type+dt.toString("yyyy").right(2)+dt.toString("MMdd")+"0000";
    QSqlQuery query;
    query.exec(QString("select max(ch_sheetNO) from cey_st_sheet_master  where  LEFT(ch_sheetNo,8)='%1' ")
               .arg(new_sheetno.left(8))
               );
    if(query.next()){
        if(query.record().value(0).toString().isEmpty()){

        } else {
            new_sheetno=query.record().value(0).toString();
        }
    }
    new_sheetno=new_sheetno.left(8)+QString().sprintf("%04d", (new_sheetno.right(4).toInt()+1)).right(4);
    return new_sheetno;
}
