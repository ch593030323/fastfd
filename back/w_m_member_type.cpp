#include "w_m_member_type.h"
//会员类型变更
#include "ui_w_m_member_type_dialog.h"
#include "w_m_member_list.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include "lds_messagebox.h"
#include "n_func.h"

w_m_member_type::w_m_member_type(QWidget *parent) :
    QDialog(parent),
    ui(new Ui_w_m_member_type_dialog)
{
    ui->setupUi(this);
    QSqlQueryModel *commodel=new QSqlQueryModel;
    commodel->setQuery(QString("select vch_typename as '%1', ch_typeno as '%2', ch_attribute as '%3' from t_m_member_type;")
                       .arg(tr("会员类型"))
                       .arg(tr("类型编号"))
                       .arg(tr("充值编号"))
                       );
    ui->comboBox_vch_typename->setModel(commodel);
    ui->comboBox_vch_typename->setModelColumn(0);
    ui->comboBox_vch_typename->setCurrentIndex(-1);

    ui->comboBox_typenames->setModel(commodel);
    ui->comboBox_typenames->setMaxVisibleItems(5);
    ui->comboBox_typenames->setView(new QTableView);
    ui->comboBox_typenames->setCurrentIndex(-1);

    ui->frame->setup(QStringList()<<tr("读卡") << tr("确定") << tr("取消"), Qt::AlignRight);
    connect(ui->frame->index_widget(tr("读卡")), SIGNAL(clicked()),this,SLOT(toreadcard()));
    connect(ui->frame->index_widget(tr("确定")), SIGNAL(clicked()),this,SLOT(took()));
    connect(ui->frame->index_widget(tr("取消")), SIGNAL(clicked()),this,SLOT(toexit()));

}

void w_m_member_type::toreadcard()
{
    n_func::st_member_type lst_member;
    QString cardno;
    QString as_memberno;
    uo_rw_member uo_member;

    //读卡
    if(!w_m_member_list::static_to_memeber_readcard(cardno, uo_member)){
        ui->lineEdit_vch_memberno->clear();
        ui->lineEdit_vch_member->clear();
        ui->comboBox_vch_typename->setCurrentIndex(-1);
        ui->lineEdit_num_amount->clear();
        ui->lineEdit_num_point->clear();
        return;
    }

    QSqlQuery query;
    query.exec(QString("select vch_memberno from t_m_member where vch_cardno = '%1' ")
               .arg(cardno)
               );
    query.next();
    as_memberno=query.record().value("vch_memberno").toString();
    if(as_memberno.isEmpty()){
        lds_messagebox::warning(this, tr("消息提示"),tr("对应会员号为空!"));
        return ;
    }
    lst_card=uo_member.of_read_card(as_memberno);
    if(lst_card.flag == false){
        lds_messagebox::warning(this, tr("消息提示"),lst_card.errtext);
        return;
    }

    lst_member=n_func::f_m_get_limit(lst_card.cardid);
    if(lst_member.as_m_state.isEmpty()){
        lds_messagebox::warning(this, tr("消息提示"),tr("非本公司卡或无效卡！"));
        return;
    }
    if(lst_member.as_m_state!="1"){
        lds_messagebox::warning(this, tr("消息提示"),tr("此卡状态非正常,不能操作!"));
        return;
    }

    query.exec(QString("select a.vch_member, b.ch_typeno from t_m_member a, t_m_member_type b where a.ch_typeno=b.ch_typeno and a.vch_memberno='%1' ")
               .arg(lst_card.cardid));
    query.next();

    ui->lineEdit_num_amount->setText(QString().sprintf("%.02f", lst_card.amount));
    ui->lineEdit_num_point->setText(QString().sprintf("%.02f", lst_card.point));
    ui->lineEdit_vch_member->setText(query.record().value("vch_member").toString());
    ui->lineEdit_vch_memberno->setText(lst_card.cardid);
    for(int row=0,rowcount=ui->comboBox_vch_typename->count();row<rowcount;row++){
        if(ui->comboBox_vch_typename->model()->index(row, 1).data().toString()==
                query.record().value("ch_typeno").toString()){ui->comboBox_vch_typename->setCurrentIndex(row);break;}
    }
}

void w_m_member_type::took()
{
    //clicked (none) returns long [pbm_bnclicked]
    //long ancestorreturnvalue
    QString ls_attribute;
    QString ls_typeno;
    QString ls_memberno;
    QString ls_attribute_new;
    QString ls_typeno_new;
    QString ls_err;
    double ldec_amount;


    ls_memberno = ui->lineEdit_vch_memberno->text();
    ls_attribute = ui->comboBox_vch_typename->model()->index(ui->comboBox_vch_typename->currentIndex(), 2).data().toString();
    ls_typeno = ui->comboBox_vch_typename->model()->index(ui->comboBox_vch_typename->currentIndex(), 1).data().toString();
    ls_attribute_new = ui->comboBox_typenames->model()->index(ui->comboBox_typenames->currentIndex(), 2).data().toString();
    ls_typeno_new = ui->comboBox_typenames->model()->index(ui->comboBox_typenames->currentIndex(), 1).data().toString();
    ldec_amount = ui->lineEdit_num_amount->text().toDouble();
    if(ls_memberno.trimmed().isEmpty()){
        lds_messagebox::warning(this, tr("消息提示"),tr("请先读卡!"));
        return;
    }
    if(ls_typeno_new.isEmpty()){
        lds_messagebox::warning(this, tr("消息提示"),tr("请先选择新类型!"));
        return;
    }
    if(ls_typeno==ls_typeno_new){
        lds_messagebox::warning(this, tr("消息提示"),tr("会员类型相同!"));
        return;
    }
    switch(ls_attribute.toInt()){
    case 1:
    case 2:
        if(ls_attribute_new!="2"){
            if(ldec_amount!=0){
                lds_messagebox::warning(this, tr("消息提示"),tr("卡余额不为零，充值卡不能转其它类型卡!"));
                return;
            }
        }
        break;
    case 3:
        if(ls_attribute_new!="3"){
            if(ldec_amount!=0){
                lds_messagebox::warning(this, tr("消息提示"),tr("未结算金额不为零，借记卡不能转其它类型卡!"));
                return;
            }
        }
        break;
    }
    if(0==lds_messagebox::warning(this, tr("消息提示"), tr("确定当前会员转指定类型?"),tr("确定"),tr("取消"))){
        //!----------------transaction
        QSqlDatabase::database().transaction();
        ls_err.clear();
        QSqlQuery query;
        if(query.exec(QString("update t_m_member set ch_typeno ='%1' where vch_memberno ='%2' ")
                      .arg(ls_typeno_new)
                      .arg(ls_memberno))){
            if((ls_err=n_func::f_m_insert_log(ls_memberno,tr("转类型"),ui->comboBox_vch_typename->currentText()+ "转为" + ui->comboBox_typenames->currentText(),0)).isEmpty()){
                ui->lineEdit_vch_memberno->clear();
                ui->lineEdit_vch_member->clear();
                ui->comboBox_vch_typename->setCurrentIndex(-1);
                ui->lineEdit_num_amount->clear();
                ui->lineEdit_num_point->clear();
            }
        } else {
            ls_err=query.lastError().text();
        }
        //!----------------transaction result
        if(ls_err.isEmpty()){
            QSqlDatabase::database().commit();
            lds_messagebox::warning(this, tr("消息提示"),tr("会员转类型成功!"));
        } else {
            QSqlDatabase::database().rollback();
            lds_messagebox::warning(this, tr("消息提示"),tr("转类型失败\n")+ls_err);
        }
    }
}

void w_m_member_type::toexit()
{
    this->reject();
}
