#include "w_sys_manage_operatorright.h"
//操作员权限
#include "ui_w_sys_manage_operatorright_dialog.h"
#include "lds_model_sqlrelationaltablemodel.h"

#include <QSqlQueryModel>
#include <QStandardItemModel>
#include "backheader.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include "lds_messagebox.h"
#include "lds_tableview_delegate_check.h"
#include "ui_w_m_member_pwdchange_dialog.h"
#include "n_func.h"
#include "ui_w_bt_dish_unitset_dialog.h"


w_sys_manage_operatorright::w_sys_manage_operatorright(QWidget *parent) :
    QDialog(parent),
    ui(new Ui_w_sys_manage_operatorright_Dialog)
{
    ui->setupUi(this);
    //! tableview
    querymodel=new QSqlQueryModel;
    standmodel=new QStandardItemModel;

    ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::SelectedClicked);ui->tableView->setModel(querymodel);
    ui->tableView_2->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::SelectedClicked);ui->tableView_2->setModel(standmodel);

    lds_tableview_delegate_check *degate=new lds_tableview_delegate_check(QVector<int>()
                                                                          << 2, ui->tableView);
    lds_tableview_delegate_check *degate_2=new lds_tableview_delegate_check(QVector<int>()
                                                                            << 3, ui->tableView_2);
    degate->setClickable();
    ui->tableView->setItemDelegate(degate);
    degate_2->setClickable();
    ui->tableView_2->setItemDelegate(degate_2);
    //! toolbar
    ui->widget->setup(QStringList() << tr("全选") << tr("全不选") << "|"<<tr("菜品权限")<<tr("权限复制")<< "|"<<tr("修改密码") << tr("保存") <<tr("刷新") << tr("退出"));
    connect(ui->widget->index_widget(tr("全选")), SIGNAL(clicked()),this,SLOT(toselectall()));
    connect(ui->widget->index_widget(tr("全不选")), SIGNAL(clicked()),this,SLOT(toselectno()));
    connect(ui->widget->index_widget(tr("菜品权限")), SIGNAL(clicked()),this,SLOT(todishright()));
    connect(ui->widget->index_widget(tr("权限复制")), SIGNAL(clicked()),this,SLOT(torightcopy()));
    connect(ui->widget->index_widget(tr("修改密码")), SIGNAL(clicked()),this,SLOT(topwdchange()));
    connect(ui->widget->index_widget(tr("保存")), SIGNAL(clicked()),this,SLOT(tosave()));
    connect(ui->widget->index_widget(tr("刷新")), SIGNAL(clicked()),this,SLOT(torefresh()));
    connect(ui->widget->index_widget(tr("退出")), SIGNAL(clicked()),this,SLOT(toexit()));

    connect(ui->tableView,SIGNAL(selectchanged(int)),this,SLOT(rightmodelupdate()));
    //!refresh
    torefresh();
}

void w_sys_manage_operatorright::torefresh()
{
    querymodel->setQuery("select vch_operID, vch_operator, ch_flag from cey_sys_operator");
    backheader::modelSetHeader(querymodel, QStringList() << tr("代码") << tr("姓名") << tr("是否生效"));
    ui->tableView->restore();
    rightmodelupdate();

    ui->tableView_2->restore();

}

void w_sys_manage_operatorright::toselectall()
{
    int row=ui->tableView->currentIndex().row();
    if(row >= 0){
        for(int r=0,r_count=standmodel->rowCount(); r<r_count; r++){//0
            standmodel->item(r,3)->setText("Y");
        }//0
    }
}

void w_sys_manage_operatorright::toselectno()
{
    int row=ui->tableView->currentIndex().row();
    if(row >= 0){
        for(int r=0,r_count=standmodel->rowCount(); r<r_count; r++){//0
            standmodel->item(r,3)->setText("N");
        }//0
    }
}

void w_sys_manage_operatorright::todishright()
{
    QDialog dialog(this);
    Ui_w_bt_dish_unitset_Dialog dui;
    QStandardItemModel _tablemodel;
    QSqlQuery query;
    QString curoperid;
    QString sqlerr;
    int row;
    if((row=ui->tableView->currentIndex().row())>=0){
        //2
        dui.setupUi(&dialog);
        dui.widget->setup(QStringList() << tr("确定") << tr("取消"), Qt::AlignRight);
        connect(dui.widget->index_widget(tr("确定")),SIGNAL(clicked()),&dialog,SLOT(accept()));
        connect(dui.widget->index_widget(tr("取消")),SIGNAL(clicked()),&dialog,SLOT(reject()));
        curoperid=querymodel->record(row).value("vch_operID").toString();
        query.exec(QString("select * from cey_bt_dish_type a left join cey_bt_dish_type_oper b on a.ch_dish_typeno=b.ch_dish_typeno and b.vch_operid='%1';")
                   .arg(curoperid));
        while(query.next()){
            _tablemodel.appendRow(QList<QStandardItem*>()
                                  << new QStandardItem(query.record().value("ch_dish_typeno").toString())
                                  << new QStandardItem(query.record().value("vch_dish_typename").toString())
                                  << new QStandardItem(query.record().value("ch_state").toString()=="Y"?"Y":"N")
                                  );
        }
        //3
        _tablemodel.setHorizontalHeaderLabels(QStringList() << tr("菜品大类编码") << tr("菜品大类")<< tr("权限"));
        dui.tableView->setModel(&_tablemodel);
        lds_tableview_delegate_check *delegate=new lds_tableview_delegate_check(
                    QVector<int>()<<2,
                    dui.tableView);
        delegate->setClickable();
        dui.tableView->setItemDelegate(delegate);
        dui.tableView->setEditTriggers(QTableView::NoEditTriggers);
        if(QDialog::Accepted==backheader::static_dialogexec(dialog, tr("菜品权限"))){
            QSqlDatabase::database().transaction();
            {
                query.exec("delete from cey_bt_dish_type_oper where ch_dish_typeno not in (select ch_dish_typeno from cey_bt_dish_type)");
                query.exec("delete from cey_bt_dish_type_oper where vch_operid not in (select vch_operid from cey_sys_operator)");
                for(int row=0,rowcount=_tablemodel.rowCount();row<rowcount;row++){
                    query.exec(QString("select * from cey_bt_dish_type_oper where vch_operid='%1' and ch_dish_typeno='%2' ")
                               .arg(curoperid)
                               .arg(_tablemodel.item(row, 0)->text())
                               );
                    if(query.next()){//存在
                        if(query.record().value("ch_state").toString()!=_tablemodel.item(row, 2)->text()){//状态不一样,更新状态
                            query.exec(QString("update cey_bt_dish_type_oper set vch_operid='%1', ch_dish_typeno='%2', ch_state='%3' where vch_operid='%1' and ch_dish_typeno='%2' ")
                                       .arg(curoperid)
                                       .arg(_tablemodel.item(row, 0)->text())
                                       .arg(_tablemodel.item(row, 2)->text())
                                       );
                        }
                    } else {//不存在,插入
                        query.exec(QString("insert into cey_bt_dish_type_oper(vch_operid, ch_dish_typeno, ch_state)values('%1','%2','%3' ) ")
                                   .arg(curoperid)
                                   .arg(_tablemodel.item(row, 0)->text())
                                   .arg(_tablemodel.item(row, 2)->text())
                                   );
                    }
                    sqlerr=query.lastError().text().trimmed();
                    if(sqlerr.isEmpty()==false){
                        break;
                    }
                }
            }
            if(sqlerr.isEmpty()==false){
                QSqlDatabase::database().rollback();
                lds_messagebox::warning(this, tr("菜品权限"), tr("数据库错误")+"\n"+sqlerr);
                return;
            }
            QSqlDatabase::database().commit();
        }
    }
}

void w_sys_manage_operatorright::torightcopy()
{
    w_sys_manage_operatorright_rightcopy dialog(this);
    backheader::static_dialogexec(dialog,qobject_cast<QAbstractButton* >(sender())->text());
}

void w_sys_manage_operatorright::topwdchange()
{
    w_sys_manage_operatorright_pwdchange dialog(this);

    //默认选择是登录账号
    dialog.ui->lineEdit_cur_operator->setText(n_func::gs_operid);
    dialog.ui->lineEdit_login_operator->setText(n_func::gs_operid);
    //有选中 则修改账号
    int row=ui->tableView->currentIndex().row();
    if(row>=0){
        dialog.ui->lineEdit_cur_operator->setText(querymodel->record(row).value("vch_operID").toString());
    }

    //如果admin 来修改非admin 密码， 则旧密码不需要填写
    if(n_func::gs_operid=="0000" && dialog.ui->lineEdit_cur_operator->text() !="0000"){
        dialog.ui->lineEdit_oldpwd->setEnabled(false);
    }
    dialog.ui->lineEdit_login_operator->setEnabled(false);
    backheader::static_dialogexec(dialog,qobject_cast<QAbstractButton* >(sender())->text());
}

bool w_sys_manage_operatorright::tosave()
{
    int row=ui->tableView->currentIndex().row();
    QString errsql;
    QSqlQuery query;
    //! <--transaction begin-->
    QSqlDatabase::database().transaction();
    if(row >= 0){
        QString ls_operid=querymodel->record(ui->tableView->currentIndex().row()).value("vch_operID").toString();
        if(query.exec(QString("delete from cey_sys_oper_purview where vch_operID = '%1'")
                      .arg(ls_operid)
                      )){
            for(int r=0,r_count=standmodel->rowCount(); r<r_count; r++){//0
                if(standmodel->item(r,3)->text().toUpper() =="Y"){
                    if(query.exec(QString("insert into cey_sys_oper_purview(vch_operID, ch_purviewno)values( '%1', '%2' )")
                                  .arg(ls_operid)
                                  .arg(standmodel->item(r, 1)->text())
                                  )){
                        qDebug() << query.lastQuery();
                        continue;
                    }
                    errsql=query.lastError().text();
                    break;
                }
            }//0
        } else {
            errsql=query.lastError().text();
        }
    }

    if(errsql.isEmpty()){
        QSqlDatabase::database().commit();
        torefresh();
        lds_messagebox::warning(this, tr("消息提示"),tr("保存成功!"));
    } else {
        QSqlDatabase::database().rollback();
        torefresh();
        lds_messagebox::warning(this, tr("消息提示"),tr("数据异常，保存失败!"), errsql);
    }
}

void w_sys_manage_operatorright::toexit()
{
    this->reject();
}

void w_sys_manage_operatorright::rightmodelupdate()
{
    standmodel->removeRows(0, standmodel->rowCount());

    QSqlQuery query;
    query.exec(QString("SELECT  vch_purview_typename ,ch_purviewno, vch_purviewname , (case when ch_purviewno in(select ch_purviewno from cey_sys_oper_purview where vch_operid = '%1') then 'Y' else 'N' end)  FROM cey_sys_purview_list")
               .arg(querymodel->record(ui->tableView->currentIndex().row()).value("vch_operID").toString())
               );
    while(query.next()){
        QList<QStandardItem *> items;
        items << new QStandardItem(query.record().value(0).toString());items.last()->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
        items << new QStandardItem(query.record().value(1).toString());items.last()->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
        items << new QStandardItem(query.record().value(2).toString());items.last()->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
        items << new QStandardItem(query.record().value(3).toString());

        standmodel->appendRow(items);
    }
    backheader::modelSetHeader(standmodel, QStringList() << tr("模块类型") << tr("模块编号") << tr("模块名称") << tr("权限"));
}

//========================================
#include "ui_w_sys_manage_operatorright_rightcopy_dialog.h"
w_sys_manage_operatorright_rightcopy::w_sys_manage_operatorright_rightcopy(w_sys_manage_operatorright *parent):
    QDialog(parent),
    ui(new Ui_w_sys_manage_operatorright_rightcopy_Dialog),
    _parent(parent)
{
    ui->setupUi(this);
    //! tableview
    querymodel=new QSqlQueryModel;
    QString vch_operator;
    if(_parent->ui->tableView->currentIndex().row()>=0){
        vch_operidfrom=_parent->querymodel->record(_parent->ui->tableView->currentIndex().row()).value("vch_operID").toString();
        vch_operator=_parent->querymodel->record(_parent->ui->tableView->currentIndex().row()).value("vch_operator").toString();
    }

    querymodel->setQuery(QString("select vch_operID, vch_operator  from cey_sys_operator where vch_operID <> '%1' ")
                         .arg(vch_operidfrom)
                         );
    ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::SelectedClicked);ui->tableView->setModel(querymodel);
    ui->label_2->setText(vch_operator+"("+vch_operidfrom+")");

    //! toolbar
    ui->widget->setup(QStringList() << tr("权限copy") << tr("退出"), Qt::AlignRight);
    connect(ui->widget->index_widget(tr("权限copy")), SIGNAL(clicked()),this,SLOT(tocopy()));
    connect(ui->widget->index_widget(tr("退出")), SIGNAL(clicked()),this,SLOT(toexit()));

}

void w_sys_manage_operatorright_rightcopy::tocopy()
{
    if(ui->tableView->currentIndex().row()>=0){
        QString vch_operidto=querymodel->record(ui->tableView->currentIndex().row()).value("vch_operID").toString();
        QSqlQuery query;
        QString errsql;
        //! <--transaction begin-->
        QSqlDatabase::database().transaction();
        // 删除
        while(1){
            //1
            query.exec(QString("delete from cey_sys_oper_purview where vch_operID = '%1'")
                       .arg(vch_operidto));
            errsql=query.lastError().text().trimmed().isEmpty();
            if(false==errsql.isEmpty())break;
            //2
            query.exec(QString(" Insert into cey_sys_oper_purview(vch_operID , ch_purviewno) select '%2' as vch_operID, ch_purviewno from cey_sys_oper_purview where vch_operID='%1' ")
                       .arg(vch_operidfrom)
                       .arg(vch_operidto));
            errsql=query.lastError().text().trimmed().isEmpty();
            if(false==errsql.isEmpty())break;
            break;
        }
        if(errsql.isEmpty()){
            QSqlDatabase::database().commit();
            _parent->torefresh();
            lds_messagebox::warning(this, tr("消息提示"),tr("保存成功!"));
        } else {
            QSqlDatabase::database().rollback();
            _parent->torefresh();
            lds_messagebox::warning(this, tr("消息提示"),tr("数据异常，保存失败!"), errsql);
        }
        //! <--transaction end-->
    }
}

void w_sys_manage_operatorright_rightcopy::toexit()
{
    this->reject();
}

//=============================
w_sys_manage_operatorright_pwdchange::w_sys_manage_operatorright_pwdchange(w_sys_manage_operatorright *parent):
    QDialog(parent),
    ui(new Ui_w_m_member_pwdchange_Dialog)
{
    ui->setupUi(this);
    //! toolbar
    ui->widget->setup(QStringList() << tr("确定") << tr("退出"), Qt::AlignRight);
    connect(ui->widget->index_widget(tr("确定")), SIGNAL(clicked()),this,SLOT(took()));
    connect(ui->widget->index_widget(tr("退出")), SIGNAL(clicked()),this,SLOT(toexit()));

    //! after
    ui->lineEdit_cur_operator->setEnabled(false);

}

void w_sys_manage_operatorright_pwdchange::took()
{
    //clicked (none) returns long [pbm_bnclicked]
    //long ancestorreturnvalue
    QString ls_operid;
    QString ls_old;
    QString ls_new;
    QString ls_confirm;
    QString ls_err;
    qlonglong ll_exist;
    QSqlQuery query;

    ls_operid=ui->lineEdit_cur_operator->text();
    ls_old=ui->lineEdit_oldpwd->text();
    ls_new=ui->lineEdit_newpwd->text();
    ls_confirm=ui->lineEdit_newpwd2->text();

    //如果是自己修改自己的密码， 则必须输入旧密码
    if(n_func::gs_operid==ls_operid){
        if(ls_old.trimmed().isEmpty()){
            lds_messagebox::warning(this, tr("消息提示"),tr("请输入旧密码错误!"));
            return;
        }
    }

    //新密码不为空
    if(ls_new.trimmed().isEmpty()){
        lds_messagebox::warning(this, tr("消息提示"),tr("新密码不能为空!"));
        return;
    }
    //新旧密码一致
    if(ls_new != ls_confirm){
        lds_messagebox::warning(this, tr("消息提示"),tr("新密码与确认新密码不一样!"));
        return;
    }

    //如果是 修改 admin的话，需要匹配旧密码
    if(ls_operid == "0000"){//0
        if(!n_func::f_cmp_syspwd(ls_old)){// != n_func::f_get_sysparm("system_pass", "0000")){
            lds_messagebox::warning(this, tr("消息提示"),tr("旧密码错误!"));
            return;
        }
    }   else {
        //如果是admin 来修改 非admin密码的话
        if(n_func::gs_operid=="0000"){
        } else {
            //如果是非admin 来修改 非admin密码的话
            query.prepare("Select Count(*)  from cey_sys_operator  where vch_operID =:ls_operid and ifnull(vch_password , '') =:ls_old");
            query.bindValue(":ls_operid", ls_operid);
            query.bindValue(":ls_old", ls_old);
            query.exec();
            query.next();
            if(query.record().value(0).toLongLong()==0){
                lds_messagebox::warning(this, tr("消息提示"),tr("旧密码错误!"));qDebug() << query.lastQuery() << query.boundValues();
                return;
            }
        }
    }//0
    //! <--transaction begin-->
    QSqlDatabase::database().transaction();
    if(ls_operid=="0000"){
        if((ls_err=n_func::f_set_sysparm("system_pass", ls_new, tr("系统管理员密码"))).isEmpty()){

        }
    } else {
        query.prepare("update cey_sys_operator  set vch_password =:ls_new     where vch_operID =:ls_operid");
        query.bindValue(":ls_new", ls_new);
        query.bindValue(":ls_operid", ls_operid);
        if(query.exec()){

        } else {
            ls_err=query.lastError().text();
        }
    }
    if(ls_err.isEmpty()){
        QSqlDatabase::database().commit();
        lds_messagebox::warning(this, tr("消息提示"),tr("密码修改成功，请记住新密码!"));
        this->accept();
    } else {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("出现异常，密码修改失败!"));
    }
    //! <--transaction end-->

}

void w_sys_manage_operatorright_pwdchange::toexit()
{
    this->reject();
}
