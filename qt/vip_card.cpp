#include "vip_card.h"
#include "ui_vip_card.h"
#include "choose_vip.h"
#include <QSqlQueryModel>
#include<QSqlRecord>
#include "lds_messagebox.h"
#include "fast_order_2.h"
#include <QDateTime>
#include "backheader.h"
#include "w_m_member_list.h"
#include "n_func.h"
#include "lds_messagebox.h"
#include "fast_pay.h"
#include "ui_fast_pay.h"

vip_card::vip_card(const QString & yfye, const QString & xf, const QString zk, const QString & zs, const QString & ys, const QString & ml, const QString & ss, const QString & xj,
                   fast_pay *parent) :
    _num(yfye),_xf(xf),_zk(zk),_zs(zs),_ys(ys),_ml(ml),_ss(ss),_xj(xj),QDialog(parent),
    ui(new Ui::vip_card),
    _parent(parent),
    is_yun_member(false)
{
    ui->setupUi(this);
    //    //this->setWindowFlags(Qt::FramelessWindowHint|Qt::Dialog);this->setStyleSheet("QDialog{border:2px solid gray}");
    init_control();
    btn_t_f = false;
    discount = 0.00;
    ui->widget_horizontalLayout_3->hide();
}

vip_card::~vip_card()
{
    delete ui;
}

void vip_card::init_control()
{
    //    QSqlQueryModel model;
    //    model.setQuery(QString("Select vch_value from t_m_parameter where vch_parameter = 'vip_card_type'"));
    //    QString str = model.record(0).value(0).toString();
    //    ui->lineEdit->setEnabled(false);
    //    ui->lineEdit_2->setEnabled(false);
    //    ui->lineEdit_3->setEnabled(false);
    //    ui->lineEdit_6->setEnabled(false);
    //    ui->lineEdit_7->setEnabled(false);
    //    if(str == "磁卡")
    //    {
    //        ui->lineEdit_4->setEnabled(true);
    //        model.clear();
    //        str.clear();
    //        model.setQuery(QString("Select vch_value from t_m_parameter where vch_parameter = 'input_memberno' "));
    //        str = model.record(0).value(0).toString();
    //        if(str == "0")
    //        {
    //            ui->lineEdit_8->setEnabled(false);
    //        }
    //        else
    //        {
    //            ui->lineEdit_8->setEnabled(true);
    //            ui->lineEdit_8->setFocus();
    //        }
    //    }
    //    else
    //    {
    //        ui->lineEdit_4->setEnabled(false);
    //        ui->lineEdit_8->setEnabled(false);
    //    }
}

void vip_card::on_pushButton_clicked()//查卡
{
    choose_vip choose;
    /*choose->exec();*/backheader::static_dialogexec(choose, tr("会员选择"));
    ui->lineEdit_8->setText(choose._text);
    //    if(!ui->lineEdit_8->text().isEmpty()){
    //        on_pushButton_3_clicked();
    //    }
}

void vip_card::on_pushButton_3_clicked()//读卡
{
    _num=_parent->ui->label_14->text();
    _xf=_parent->ui->lineEdit->text();
    _zk=_parent->ui->lineEdit_2->text();
    _zs=_parent->ui->lineEdit_3->text();

    _ys=_parent->ui->lineEdit_4->text();
    _ml=_parent->ui->lineEdit_5->text();
    _ss=_parent->ui->lineEdit_6->text();
    _xj=_parent->ui->lineEdit_9->text();

    disconnect(ui->lineEdit_4,SIGNAL(textChanged(QString)),this,SLOT(tomember_payall(QString)));

    uo_rw_member uo_member;
    QString cardno;
    QString passwrod;
    //! 开启云会员static_to_memeber_readcard会判断
    while(1){
        //读卡
        QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
        if(!w_m_member_list::static_to_memeber_readcard(cardno, uo_member, true)){
            break;
        }
        is_yun_member=w_m_member_list::is_yun_member;
        //若是云端，则获取云端的密码
        if(w_m_member_list::is_yun_member){
            passwrod=w_sys_manage_cloudsync::get_yun_member_readcard(cardno,
                                                                     conf.value("w_sys_manage_cloudsync_basedataset/cloudUser").toString(),
                                                                     "password",
                                                                     QVariant::String);if(!w_sys_manage_cloudsync::this_action_is_successed) break;
        } else {
            QSqlQuery query;
            query.exec(QString("select ifnull(vch_password, '') from hddpos.t_m_member where vch_memberno='%1'")
                       .arg(cardno));
            query.next();
            passwrod = query.record().value(0).toString();
        }
        //判断密码
        if(!passwrod.isEmpty()){
            lds_dialog_input dialog;
            dialog.set_second_hidden(false);
            dialog.set_key_hidden(true);
            dialog.ui->label->setText(tr("密码"));
            dialog.ui->label_2->setText(tr("再次输入密码"));
            dialog.ui->lineEdit->setEchoMode(QLineEdit::Password);
            dialog.ui->lineEdit_2->setEchoMode(QLineEdit::Password);
            if(!QDialog::Accepted==backheader::static_dialogexec(dialog,tr("请输入密码"))){
                break;
            }
            if(dialog.ui->lineEdit->text()!=dialog.ui->lineEdit_2->text()){
                lds_messagebox::warning(this, tr("消息提示"),tr("两次密码不一致!"));
                break;
            }
            if(passwrod!=dialog.ui->lineEdit->text()){
                lds_messagebox::warning(this, tr("消息提示"),tr("密码错误，操作取消!"));
                break;
            }
        }
        //! 判断密码

        QString text = cardno;
        QSqlQueryModel model;
        if(text.trimmed().isEmpty())
        {
            lds_messagebox::warning(this, tr("消息提示"),tr("无可查询的卡!"));
            
            break;
        }
        //获取开信息
        QString kahao;
        QString m_type;
        QString huiyuanmingcheng;
        QString type;
        ch_attribute;
        if(w_m_member_list::is_yun_member){
            kahao=w_sys_manage_cloudsync::get_yun_member_readcard(cardno, conf.value("w_sys_manage_cloudsync_basedataset/cloudUser").toString(),
                                                                  "vch_memberno", QVariant::String);if(!w_sys_manage_cloudsync::this_action_is_successed) break;
            m_type=w_sys_manage_cloudsync::get_yun_member_readcard(cardno, conf.value("w_sys_manage_cloudsync_basedataset/cloudUser").toString(),
                                                                   "ch_typeno", QVariant::String);if(!w_sys_manage_cloudsync::this_action_is_successed) break;
            huiyuanmingcheng=w_sys_manage_cloudsync::get_yun_member_readcard(cardno, conf.value("w_sys_manage_cloudsync_basedataset/cloudUser").toString(),
                                                                             "vch_member", QVariant::String);if(!w_sys_manage_cloudsync::this_action_is_successed) break;
            type=w_sys_manage_cloudsync::get_yun_member_readcard(cardno, conf.value("w_sys_manage_cloudsync_basedataset/cloudUser").toString(),
                                                                 "ch_state", QVariant::String);if(!w_sys_manage_cloudsync::this_action_is_successed) break;
            ch_attribute=w_sys_manage_cloudsync::get_yun_member_readcard(cardno, conf.value("w_sys_manage_cloudsync_basedataset/cloudUser").toString(),
                                                                         "ch_attribute", QVariant::String);if(!w_sys_manage_cloudsync::this_action_is_successed) break;

        } else {
            model.setQuery(QString("SELECT a.vch_memberno,a.vch_member,a.ch_typeno,a.vch_tel, a.vch_handtel,  a.dt_birthday ,b.ch_attribute  FROM t_m_member a,t_m_member_type b where a.ch_typeno = b.ch_typeno and b.ch_attribute like '%'  and a.vch_memberno = '%1' ").arg(text));
            kahao = model.record(0).value(0).toString();
            m_type = model.record(0).value(2).toString();
            huiyuanmingcheng = model.record(0).value(1).toString();
            ch_attribute=model.record(0).value("ch_attribute").toString();

            model.setQuery(QString("select ch_state from t_m_member where vch_memberno = '%1' ").arg(text));
            type = model.record(0).value(0).toString();
        }

        if(ch_attribute=="1"){
            ui->label_4->setVisible(false);
            ui->label_7->setVisible(false);
            ui->lineEdit_3->setVisible(false);
            ui->lineEdit_6->setVisible(false);
        }  else {
            ui->label_4->setVisible(true);
            ui->label_7->setVisible(true);
            ui->lineEdit_3->setVisible(true);
            ui->lineEdit_6->setVisible(true);
        }
        if(kahao.isEmpty())
        {
            lds_messagebox::warning(this, tr("消息提示"),tr("有误,还未发卡或不存在,请核实!"));
            
            break;
        }

        if(type == "4")
        {
            lds_messagebox::warning(this, tr("消息提示"),tr("此卡已经作废,不能付款!"));
            break;
        }

        if(type == "3")
        {
            lds_messagebox::warning(this, tr("消息提示"),tr("此卡已经挂失,不能付款!"));
            
            break;
        }

        if(type == "2")
        {
            lds_messagebox::warning(this, tr("消息提示"),tr("此卡已经止付,不能付款!"));
            
            break;
        }

        float baobenjine = 0.00;
        double num_amount;
        double num_point;

        if(w_m_member_list::is_yun_member){
            baobenjine=w_sys_manage_cloudsync::get_yun_member_membertype(m_type,
                                                                         "intCardBaseamount", QVariant::Int).toFloat();if(!w_sys_manage_cloudsync::this_action_is_successed) break;
            ch_attribute=w_sys_manage_cloudsync::get_yun_member_membertype(m_type,
                                                                              "chAttribute", QVariant::String);if(!w_sys_manage_cloudsync::this_action_is_successed) break;
            discount=w_sys_manage_cloudsync::get_yun_member_membertype(m_type,
                                                                       "intDiscount", QVariant::Int).toInt();if(!w_sys_manage_cloudsync::this_action_is_successed) break;

            num_amount=w_sys_manage_cloudsync::get_yun_member_readcard(cardno, conf.value("w_sys_manage_cloudsync_basedataset/cloudUser").toString(),
                                                                       "num_amount", QVariant::Double).toDouble();if(!w_sys_manage_cloudsync::this_action_is_successed) break;
            num_point=w_sys_manage_cloudsync::get_yun_member_readcard(cardno, conf.value("w_sys_manage_cloudsync_basedataset/cloudUser").toString(),
                                                                      "num_point", QVariant::Double).toDouble();if(!w_sys_manage_cloudsync::this_action_is_successed) break;
        } else {
            model.setQuery(QString("select * from t_m_member_type where ch_typeno = '%1'").arg(m_type));
            baobenjine = model.record(0).value(12).toFloat();
            ch_attribute = model.record(0).value(2).toString();//1为优惠卡，2为充值卡
            discount = model.record(0).value(5).toFloat();

            model.setQuery(QString("select * from t_m_curamount where vch_memberno = '%1'").arg(text));
            num_amount = model.record(0).value(1).toString().toFloat();//余额
            num_point = model.record(0).value(3).toString().toFloat();//积分
        }
        ui->lineEdit_6->setText(QString().sprintf("%0.2f",baobenjine));
        ui->lineEdit->setText(kahao);
        ui->lineEdit_3->setText(QString().sprintf("%0.2f", num_amount));//余额
        ui->lineEdit_7->setText(QString().sprintf("%0.2f", num_point));//积分
        ui->lineEdit_2->setText(huiyuanmingcheng);
        ui->lineEdit_4->setText(QString().sprintf("%0.2f",_num.toFloat()));
        qDebug() << "member info set lineedit";
        //        if(_num.toFloat() > kazhi.record(0).value(1).toFloat())
        //        {
        //            ui->lineEdit_4->setText(QString().sprintf("%0.2f",kazhi.record(0).value(1).toFloat()));
        //        }
        //        else
        //        {
        //            ui->lineEdit_4->setText(QString().sprintf("%0.2f",_num.toFloat()));
        //        }

        //会员卡充值是自动个折扣
        if(n_func::f_m_get_parameter("m_card_paydiscount")=="0"/*不是自动折扣*/
                &&(discount>0&&discount<100)
                && 1==lds_messagebox::warning(this, tr("消息提示"),tr("会员卡有折扣[%1]，是否折扣?").arg(discount), tr("确认"),tr("取消")/*不允许折扣*/)){
        } else
            if(/*ch_attribute == "2" &&*/ discount > 0 && discount < 100)
            {
                QStandardItemModel & model = fast_order_2::modelBase;
                float xiaofei = 0.00;
                xiaofei = _xf.toFloat();
                float zhekou = 0.00;
                float zengsong = 0.00;
                zengsong = _zs.toFloat();
                float moling = 0.00;
                moling = _ml.toFloat();
                float shuishou = 0.00;
                shuishou = _ss.toFloat();
                float yingshou = 0.00;
                yingshou = _ys.toFloat();
                float xianjin = 0.00;
                xianjin = _xj.toFloat();

                for(int i = 0; i < model.rowCount(); i++)
                {
                    if(model.item(i,9)->text() != "Y" && model.item(i,10)->text() != "Y" && model.item(i,8)->text() == "N" && model.item(i,18)->text() == "Y")
                    {
                        // model.item(i,5)->setText(QString().sprintf("0.2f",discount->zhekouzhi));
                        float bb = 0.00;
                        bb = model.item(i,3)->text().toFloat() * model.item(i,4)->text().toFloat() * discount * 0.01;
                        // model.item(i,6)->setText(QString().sprintf("%0.2f",bb));

                        float aa =0.00;
                        aa = model.item(i,3)->text().toFloat() * model.item(i,4)->text().toFloat() * (100 - discount) * 0.01;
                        zhekou += aa;
                    }
                }


                float shijiyishou = 0.00;
                shijiyishou = xiaofei- zengsong - zhekou;
                float shijiyingshou = 0.00;
                QSqlQueryModel moling_m;
                moling_m.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='blotout_round' "));
                QString type = moling_m.record(0).value(0).toString();
                if(type == "1")//抹零
                {
                    moling_m.clear();
                    moling_m.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='blotout_mode'"));
                    QString str = moling_m.record(0).value(0).toString();
                    if(str == "2")//到元
                    {
                        float startyingshou = 0.00;
                        startyingshou = shijiyishou;
                        shijiyingshou = (int)shijiyishou;
                        moling = startyingshou -shijiyingshou;
                    }
                    else if(str == "3")//到十元
                    {
                        int str;
                        float startyingshou = 0.00;
                        startyingshou = shijiyishou;
                        str = (int)shijiyishou;
                        QString str1 = QString::number(str);
                        QString str2;
                        if(str1.right(1) != 0)
                        {
                            str2 = "0";
                        }
                        shijiyingshou = (str1.left(str1.size()-1) + str2).toFloat();
                        moling = startyingshou -shijiyingshou;
                    }
                    else if(str == "4")//到百元
                    {
                        int str;
                        float startyingshou = 0.00;
                        startyingshou = shijiyishou;
                        str = (int)shijiyishou;
                        QString str1 = QString::number(str);
                        QString str2;
                        if(str < 100)
                        {
                            shijiyingshou = 0.00;
                        }
                        else
                        {
                            str2 = "00";
                            shijiyingshou = (str1.left(str1.size()-2) + str2).toFloat();
                        }

                        moling = startyingshou -shijiyingshou;
                    }
                }
                else//四舍五入
                {
                    moling_m.clear();
                    moling_m.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='round_mode'"));
                    QString str = moling_m.record(0).value(0).toString();
                    if(str == "2")//到角
                    {
                        float startyingshou = 0.00;
                        startyingshou = shijiyishou;
                        shijiyingshou = (int)((shijiyishou * 10) + 0.5) / 10.0;
                        moling = startyingshou -shijiyingshou;
                    }
                    else if(str == "3")//到元
                    {
                        float startyingshou = 0.00;
                        startyingshou = shijiyishou;
                        shijiyingshou = (int)(shijiyishou + 0.5);
                        moling = startyingshou -shijiyingshou;
                    }
                    else if(str == "4")//到十元
                    {
                        float startyingshou = 0.00;
                        startyingshou = shijiyishou;
                        int ying;
                        ying = (int)shijiyishou;
                        shijiyingshou = (ying%10)>=5?(ying-ying%10+10):(ying-ying%10);
                        moling = startyingshou -shijiyingshou;
                    }
                }
                yingshou = xiaofei - zengsong - moling - zhekou  + shuishou;

                moling_m.clear();
                moling_m.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='cash_payall' "));
                QString xianjin_type = moling_m.record(0).value(0).toString();
                if(xianjin_type == "0")
                {
                    xianjin = 0.00;
                }
                else
                {
                    xianjin = yingshou;
                }

                _xf = QString().sprintf("%0.2f",xiaofei);
                _zk = QString().sprintf("%0.2f",zhekou);
                _zs = QString().sprintf("%0.2f",zengsong);
                _ys = QString().sprintf("%0.2f",yingshou);
                _ml = QString().sprintf("%0.2f",moling);
                _ss = QString().sprintf("%0.2f",shuishou);
                _xj = QString().sprintf("%0.2f",yingshou);
                _num = QString().sprintf("%0.2f",yingshou);
                ui->lineEdit_4->setText(_num);
                qDebug() << "member info update";
                //===================================================================================================
            }
        member_payall_save=ui->lineEdit_4->text();
        connect(ui->lineEdit_4,SIGNAL(textChanged(QString)),this,SLOT(tomember_payall(QString)));
        break;
    }//! while(1)
    w_m_member_list::yun_member_close();//! 关闭云会员
}

void vip_card::on_pushButton_4_clicked()//确定
{
    if(ui->lineEdit->text().trimmed().isEmpty())
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("请先读卡!"));
        return;
    }

    if(ch_attribute=="2"){
        if(ui->lineEdit_4->text().toFloat() > ui->lineEdit_3->text().toFloat()){
            lds_messagebox::warning(this, tr("消息提示"),tr("卡余额不足,付款失败!"));

            return;
        }
        if(ui->lineEdit_4->text().toFloat() > _num.toFloat() && ch_attribute == "2")
        {
            lds_messagebox::warning(this, tr("消息提示"),tr("付款金额大于结帐金额!"));
            return;
        }
    }

    time = n_func::f_get_sysdatetime().toString("yyyy-MM-dd hh:mm:ss");
    beizhu = ui->lineEdit_5->text();

    fukuankahao = ui->lineEdit->text();
    btn_t_f = true;
    _xj = QString().sprintf("%0.2f",_xj.toFloat()-ui->lineEdit_4->text().toFloat());//QString().sprintf("%0.2f",yingshou);
    _card_pay=QString().sprintf("%.02f", ui->lineEdit_4->text().toFloat());
    this->close();
}

void vip_card::on_pushButton_5_clicked()//取消
{
    btn_t_f = false;
    this->reject();
}

void vip_card::tomember_payall(const QString &arg1)
{
    if(is_yun_member)w_m_member_list::yun_member_open();
    while(1){
        if(arg1==member_payall_save) break;
        //判断会员卡是否必须付全款
        if(n_func::f_m_get_parameter("deposit_pay_all")=="0"/*不是付全款*/){
            break;
        }
        lds_messagebox::warning(this, tr("消息提示"),tr("当前设置充值卡付款必须付全款"));
        ui->lineEdit_4->setText(member_payall_save);
        break;
    }
    w_m_member_list::yun_member_close();

}
