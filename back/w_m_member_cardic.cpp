#include "w_m_member_cardic.h"
//卡芯片清除
#include "ui_w_m_member_cardic_dialog.h"
#include "public_cika_read_dialog.h"
#include "reader_rf.h"
#include "backheader.h"
#include "lds_messagebox.h"
#include "w_m_member.h"
#include "backheader.h"
#include <QSqlError>
#include "w_m_member_list.h"

w_m_member_cardic::w_m_member_cardic(QWidget *parent) :
    QDialog(parent),
    ui(new Ui_w_m_member_cardic_Dialog)
{
    ui->setupUi(this);
    ui->widget->setup(QStringList() /*<<tr("直接清除") */<< tr("读卡") << tr("清除") << tr("退出"), Qt::AlignRight);
    connect(ui->widget->index_widget(tr("读卡")), SIGNAL(clicked()),this,SLOT(to_read_card()));
    connect(ui->widget->index_widget(tr("清除")), SIGNAL(clicked()),this,SLOT(to_clear()));
    connect(ui->widget->index_widget(tr("退出")), SIGNAL(clicked()),this,SLOT(to_exit()));
//    connect(ui->widget->index_widget(tr("直接清除")), SIGNAL(clicked()),this,SLOT(to_dynamic_clear()));

    ui->lineEdit_ch_state->setEnabled(false);
    ui->lineEdit_memberno->setEnabled(false);
    ui->lineEdit_num_amount->setEnabled(false);
    ui->lineEdit_point->setEnabled(false);
}

void w_m_member_cardic::to_read_card()
{
    QString ls_value;
    QString cardno;
    QString cardno_tmp1;
    QString cardno_tmp2;
    QString as_memberno;

    if(!w_m_member_list::static_to_memeber_readcard(cardno, uo_member)){
        ui->lineEdit_ch_state->clear();
        ui->lineEdit_memberno->clear();
        ui->lineEdit_num_amount->clear();
        ui->lineEdit_point->clear();
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

    ui->lineEdit_ch_state->setText(backheader::VIPFINDVALUE(w_m_member::VIP_MODEL_STATE, lst_member.as_m_state));
    ui->lineEdit_memberno->setText(lst_card.cardid);
    ui->lineEdit_num_amount->setText(QString().sprintf("%.02f", lst_card.amount));
    ui->lineEdit_point->setText(QString().sprintf("%.02f", lst_card.point));
}

void w_m_member_cardic::to_clear()
{
    QString errtext;

    if(ui->lineEdit_memberno->text().trimmed().isEmpty()){
        lds_messagebox::warning(this, tr("消息提示"),tr("卡号为空请先读卡!"));
        return;
    }
    if(lst_member.as_m_state=="1" &&  lst_card.amount > 0){//4
        if(0==lds_messagebox::warning(this, tr("消息提示"),tr("此会员状态正常且尚有余额,你确定要初始化这张卡么?"), tr("确定"), tr("取消"))){//3
        } else {
            return;
        }
    }
    //! <--transaction begin-->
    QSqlDatabase::database().transaction();
    QSqlQuery query;
    if(query.exec(QString("update t_m_member set ch_cardflag = 'N', vch_cardno='', int_basedata=0  where vch_memberno = '%1' ")
                  .arg(lst_card.cardid))){//2
        if((errtext=n_func::f_m_insert_log(lst_card.cardid, tr("清除芯片"), lst_card.cardid, 0)).isEmpty()){//1

            //清除ic卡
            if(uo_member.is_cardtype == "1"){//0
                QString icpasswd=n_func::f_m_get_parameter("vip_ic_password");
                Reader_RF reader;
                if(reader.setDefPwd(icpasswd=="FFFFFF"?"FFFFFFFFFFFF":icpasswd.toLocal8Bit().toHex())){
                    if(reader.writecard("","","")){
                        if(reader.pwdChange(icpasswd=="FFFFFF"?"FFFFFFFFFFFF":icpasswd.toLocal8Bit().toHex()
                                            , "FFFFFFFFFFFF")){
                            //清除成功
                            //! <--transaction commit-->
                            QSqlDatabase::database().commit();
                            lds_messagebox::warning(this, tr("消息提示"),tr("清除成功!"));
                            ui->lineEdit_ch_state->clear();
                            ui->lineEdit_memberno->clear();
                            ui->lineEdit_num_amount->clear();
                            ui->lineEdit_point->clear();
                            return;
                        }
                    }
                }
                errtext=reader.lastError();
            }//0
        }//1
        qDebug() << "1";
    }//2
    qDebug() << "2" << query.lastQuery();
    errtext=query.lastError().text();
    //! <--transaction rollback-->
    QSqlDatabase::database().rollback();
    lds_messagebox::warning(this, tr("消息提示"),tr("清除失败!")+"\n"+query.lastError().text());
    ui->lineEdit_ch_state->clear();
    ui->lineEdit_memberno->clear();
    ui->lineEdit_num_amount->clear();
    ui->lineEdit_point->clear();
}

void w_m_member_cardic::to_exit()
{
    this->reject();
}

void w_m_member_cardic::to_dynamic_clear()
{
    QString icpasswd=n_func::f_m_get_parameter("vip_ic_password");
    //    icpasswd="ffffffffffff";
    Reader_RF reader;
    if(reader.setDefPwd(icpasswd=="FFFFFF"?"FFFFFFFFFFFF":icpasswd.toLocal8Bit().toHex())){
        if(reader.writecard("","","")){
            if(reader.pwdChange(icpasswd=="FFFFFF"?"FFFFFFFFFFFF":icpasswd.toLocal8Bit().toHex(), "FFFFFFFFFFFF")){
                //清除成功
                qDebug() << "success";
                return;
            }
        }
    }
    qDebug() << "failure";
}
