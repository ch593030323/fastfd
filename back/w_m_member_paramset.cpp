#include "w_m_member_paramset.h"
//会员参数设置
#include "n_func.h"
#include "ui_w_m_member_paramset_dialog.h"
#include "lds_messagebox.h"
#include <QDebug>
#include <QSqlRecord>
#include "w_m_member.h"

w_m_member_paramset::w_m_member_paramset(QWidget *parent) :
    QDialog(parent),ui(new Ui_w_m_member_paramset_Dialog)
{
    ui->setupUi(this);
    ui->comboBox_vip_card_type->addItems(QStringList()<<tr("IC卡")<<tr("磁卡")<<tr("无会员卡"));
    ui->comboBox_vip_ic_machine->addItems(QStringList()<<tr("明化RF"));
    ui->comboBox_vip_ic_port->addItems(QStringList() <<tr("USB"));
    ui->comboBox_vip_card_type->setCurrentIndex(-1);
    //! conncet
    ui->widget->setup(QStringList()<<tr("确定")<<tr("取消"),Qt::AlignRight);
    connect(ui->widget->index_widget(tr("确定")),SIGNAL(clicked()),this,SLOT(took()));
    connect(ui->widget->index_widget(tr("取消")),SIGNAL(clicked()),this,SLOT(tocancel()));
    connect(ui->comboBox_vip_card_type,SIGNAL(currentIndexChanged(QString)),this,SLOT(inputmemberno(QString)));

    //! after
    ue_get1();
    ue_get2();
    ui->checkBox_discount->setEnabled(false);
    ui->tabWidget->setObjectName("tab");
    connect(ui->checkBox_save,SIGNAL(clicked(bool)),this,SLOT(toisrecharge(bool)));
}

void w_m_member_paramset::ue_get1()
{
    QString ls_value=n_func::f_m_get_parameter("member_ATT_mode", "123");
    ui->checkBox_discount->setChecked(ls_value.contains("1"));
    ui->checkBox_save->setChecked(ls_value.contains("2"));
    //  if(ls_value.contains("1"))ui->checkBox_discount->setChecked(true);

    ls_value=n_func::f_m_get_parameter("member_bank_print","0");
    ui->checkBox_member_bank->setChecked((ls_value=="1"));

    //    ls_value=n_func::f_m_get_parameter("member_print","0");
    //    ui->checkBox_member_print->setChecked((ls_value=="1"));

    ls_value=n_func::f_m_get_parameter("m_deposit_negative","0");
    ui->checkBox_m_deposit_negative->setChecked((ls_value=="1"));

    ls_value=n_func::f_m_get_parameter("m_card_paydiscount","0");
    ui->checkBox_m_card_paydiscount->setChecked(ls_value=="1");

    //    ls_value=n_func::f_m_get_parameter("m_hang_paydiscount","0");
    //    ui->checkBox_m_hang_paydiscount->setChecked(ls_value=="1");

    ls_value=n_func::f_m_get_parameter("m_max_deposit","30000");
    ui->lineEdit_max_deposit->setText(ls_value);

    ls_value=n_func::f_m_get_parameter("discount_view","0");
    ui->checkBox_discount_view->setChecked(ls_value=="1");

    ls_value=n_func::f_m_get_parameter("deposit_pay_all","0");
    ui->checkBox_deposit_pay_all->setChecked(ls_value=="1");

    ls_value=n_func::f_m_get_parameter("local_member_select","0");
    ui->checkBox_local_select->setChecked(ls_value=="1");

    ls_value=n_func::f_m_get_parameter("m_fetch_base","0");
    ui->checkBox_m_fetch_base->setChecked(ls_value=="1");

    ls_value=n_func::f_m_get_parameter("m_member_pay","0");
    ui->checkBox_member_pay->setChecked(ls_value=="1");

    toisrecharge(ui->checkBox_save->isChecked());
//    if(!ui->checkBox_save->isChecked()){
//        ui->checkBox_m_deposit_negative->setEnabled(false);
//        ui->lineEdit_max_deposit->setEnabled(false);
//        //        ui->checkBox_member_print->setEnabled(false);
//        ui->checkBox_deposit_pay_all->setEnabled(false);
//        ui->checkBox_m_card_paydiscount->setEnabled(false);

//        ui->checkBox_m_deposit_negative->setChecked(false);
//        ui->lineEdit_max_deposit->clear();
//        //        ui->checkBox_member_print->checked(false);
//        ui->checkBox_deposit_pay_all->setCheckable(false);
//        ui->checkBox_m_card_paydiscount->setChecked(false);

//    }
}

QString w_m_member_paramset::ue_set1()
{
    QString error;
    QString ls_value="1";
    if(ui->checkBox_save->isChecked()) ls_value+="2";
    if(!(error=n_func::f_m_set_parameter("member_ATT_mode", ls_value, tr("会员需要使用类型"))).isEmpty()){
        return error;
    }

    ls_value=ui->checkBox_member_bank->isChecked()?"1":"0";
    if(!(error=n_func::f_m_set_parameter("member_bank_print", ls_value, tr("会员作废的时候打印账单"))).isEmpty()){
        return error;
    }

    //    ls_value=ui->checkBox_member_print->isChecked()?"1":"0";
    //    if(!(error=n_func::f_m_set_parameter("member_print", ls_value, tr("会员充值卡付款打印"))).isEmpty()){
    //        return error;
    //    }

    ls_value=ui->checkBox_m_deposit_negative->isChecked()?"1":"0";
    if(!(error=n_func::f_m_set_parameter("m_deposit_negative", ls_value, tr("充值与实收金额必须相等"))).isEmpty()){
        return error;
    }

    ls_value=ui->checkBox_m_card_paydiscount->isChecked()?"1":"0";
    if(!(error=n_func::f_m_set_parameter("m_card_paydiscount", ls_value, tr("充值卡会员付款时自动折扣"))).isEmpty()){
        return error;
    }

    //    ls_value=ui->checkBox_m_hang_paydiscount->isChecked()?"1":"0";
    //    if(!(error=n_func::f_m_set_parameter("m_hang_paydiscount", ls_value, tr("挂帐卡会员付款时自动折扣"))).isEmpty()){
    //        return error;
    //    }
    ls_value=ui->lineEdit_max_deposit->text();
    if(!(error=n_func::f_m_set_parameter("m_max_deposit", ls_value, tr("最大充值金额"))).isEmpty()){
        return error;
    }

    ls_value=ui->checkBox_discount_view->isChecked()?"1":"0";
    if(!(error=n_func::f_m_set_parameter("discount_view", ls_value, tr("折扣时显示会员折扣详情"))).isEmpty()){
        return error;
    }

    ls_value=ui->checkBox_deposit_pay_all->isChecked()?"1":"0";
    if(!(error=n_func::f_m_set_parameter("deposit_pay_all", ls_value, tr("充值卡付款需全单满付"))).isEmpty()){
        return error;
    }

    ls_value=ui->checkBox_local_select->isChecked()?"1":"0";
    if(!(error=n_func::f_m_set_parameter("local_member_select", ls_value, tr("启动本地会员营业时查卡"))).isEmpty()){
        return error;
    }

    ls_value=ui->checkBox_m_fetch_base->isChecked()?"1":"0";
    if(!(error=n_func::f_m_set_parameter("m_fetch_base", ls_value, tr("可取保本金额"))).isEmpty()){
        return error;
    }

    ls_value=ui->checkBox_member_pay->isChecked()?"1":"0";
    if(!(error=n_func::f_m_set_parameter("m_member_pay", ls_value, tr("充值会员付款允许负数"))).isEmpty()){
        return error;
    }
    return "";
}

void w_m_member_paramset::ue_get2()
{
    QString ls_value;
    long li_index;

    ui->lineEdit_vip_ic_password->setText(n_func::f_m_get_parameter("vip_ic_password","FFFFFF"));
    ls_value=n_func::f_m_get_parameter("vip_card_type", tr("无会员卡"));
//    ui->comboBox_vip_card_type->setCurrentIndex(-1);
    if(ls_value==tr("IC卡")){
        ui->comboBox_vip_card_type->setCurrentIndex(0);
    } else if(ls_value==tr("磁卡")){
        ui->comboBox_vip_card_type->setCurrentIndex(1);
        ls_value=n_func::f_m_get_parameter("input_memberno","0");
        ui->checkBox_input_memberno->setChecked(ls_value=="1");
    } else if(ls_value==tr("无会员卡")){
        ui->comboBox_vip_card_type->setCurrentIndex(2);
    }

    //若已经发卡，则，发开类型不能修改
    QSqlQuery query;
    query.exec("select *from t_m_member where ch_cardflag = 'Y' ");
    if(query.next()){
        ui->comboBox_vip_card_type->setEnabled(false);
    }
}

QString w_m_member_paramset::ue_set2()
{
    QString ls_value;
    QString error;
    ls_value=ui->comboBox_vip_card_type->currentText();
    if(!(error=n_func::f_m_set_parameter("vip_card_type", ls_value, tr("卡类型"))).isEmpty()){
        return error;
    }

    ls_value=ui->lineEdit_vip_ic_password->text();
    if(!(error=n_func::f_m_set_parameter("vip_ic_password", ls_value, tr("IC卡读系密码"))).isEmpty()){
        return error;
    }

    ls_value=ui->checkBox_input_memberno->isChecked()?"1":"0";
    if(!(error=n_func::f_m_set_parameter("input_memberno", ls_value, tr("手动输入会员卡号"))).isEmpty()){
        return error;
    }
    return "";
}

void w_m_member_paramset::toisrecharge(bool flag)
{
    qDebug() << flag;
    if(flag){
        ui->groupBox_2->setEnabled(true);
    } else {
        QSqlQuery query;
        query.exec("select count(*) from t_m_member_type where ch_attribute='2' ");
        query.next();
        if(query.record().value(0).toLongLong() > 0){
            lds_messagebox::warning(this, tr("消息提示"),tr("会员类型中已经有充值卡会员类型，不能取消!"));
            ui->checkBox_save->setChecked(true);
            return;
        }

        ui->groupBox_2->setEnabled(false);
        ui->lineEdit_max_deposit->setText("");
    }
}

void w_m_member_paramset::inputmemberno(const QString &text)
{
    ui->frame_cika->setVisible(text==tr("磁卡"));
    ui->frame_ic->setVisible(text==tr("IC卡"));
}

void w_m_member_paramset::took()
{
    QString errtext;
    if((errtext=ue_set1()).isEmpty()){
        if((errtext=ue_set2()).isEmpty()){
            w_m_member::VIP_MODEL_INIT();
            lds_messagebox::warning(this, tr("消息提示"),tr("保存成功!"));
            this->accept();
            return;
        }
    }
    qDebug() << errtext;
    lds_messagebox::warning(this, tr("消息提示"),tr("数据库错误!"));
}

void w_m_member_paramset::tocancel()
{
    this->reject();
}
