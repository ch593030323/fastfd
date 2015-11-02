#include "fast_pay.h"
#include "vip_card.h"
#include "bank_card.h"
#include "discount_dispose.h"
#include "vip_recharge.h"
#include "fast_order_2.h"
#include "fast_order_2.h"
#include <QStandardItemModel>
#include "n_func.h"
#include <QSqlRecord>
#include <QDebug>
#include "lds_messagebox.h"
#include "lds_messagebox.h"
#include "special_dispose.h"
#include "free_view.h"
#include <QSqlQuery>
#include "other_pay_view.h"
#include"sqlquery.h"
#include "back/w_m_member_list.h"
#include <QSettings>
#include <QDesktopWidget>
#include <QApplication>
#include <QSqlError>
#include "backheader.h"
#include "system_setting.h"
#include "w_bt_dish_kitchenprinter.h"
#include "fast_order_2.h"
#include "public_yuninfo_dialog.h"
#include "lds_loadingdialog.h"
#include "labelprint_main_dialog.h"
#include "trans_u2g.h"
#include "w_scr_dish_restaurant_dialog.h"
#include <QSqlQueryModel>

fast_pay::fast_pay(frontViewDialog_virtual *parent, bool on_d_p_b_r) :
    QDialog(0),
    ui(new Ui_fast_pay),
    _parent(parent)
{
    ui->setupUi(this);
    connect(ui->lineEdit_9,SIGNAL(textChanged(QString)),this,SLOT(lineEdit_9_textChanged(QString)));
    init_control();
    init_data(on_d_p_b_r);
    kafujine = 0.000;
    shifouyouhuika = false;
    btn_t_f = false;
    count_dis = 0;
    pay_lowcost = 0;//低消
    pay_servicecost = 0;//服务费
    otherpay=0;
    ui->tableView->setEditTriggers(QTableView::NoEditTriggers);
    if(_parent)
        _parent->emit_signal_display_total(ui->lineEdit_4->text());
    setjiulou_visiable(false);
    QTimer::singleShot(100, ui->lineEdit_9,SLOT(selectAll()));//->selectAll();
}

fast_pay::~fast_pay()
{
    delete ui;
}

QString fast_pay::get_total()
{
    return ui->lineEdit_9->text();
}

void fast_pay::init_control()
{
    ui->lineEdit->setEnabled(false);
    ui->lineEdit_2->setEnabled(false);
    ui->lineEdit_3->setEnabled(false);
    ui->lineEdit_4->setEnabled(false);
    ui->lineEdit_5->setEnabled(false);
    ui->lineEdit_6->setEnabled(false);
    ui->lineEdit_7->setEnabled(false);

    ui->lineEdit_11->setEnabled(false);
    ui->lineEdit_12->setEnabled(false);
    ui->lineEdit_13->setEnabled(false);
    ui->lineEdit_14->setEnabled(false);
    ui->lineEdit_15->setEnabled(false);
    ui->lineEdit_9->setFocus();

    table_model = new QStandardItemModel();
    QStringList list;
    list <<tr("行号")<<tr("付款方式")<<tr("实付现金")<<tr("付款金额")<<tr("凭证号");
    table_model->setHorizontalHeaderLabels(list);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->verticalHeader()->setDefaultSectionSize(40);
    ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::SelectedClicked);ui->tableView->setModel(table_model);
    ui->tableView->verticalHeader()->hide();
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#else
    ui->tableView->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
#endif
}

void fast_pay::init_data(bool on_d_p_b_r)
{
    QSqlQueryModel model_l;
    model_l.setQuery("Select NOW()");
    QString time = model_l.record(0).value(0).toDateTime().toString("yyyy-MM-dd hh:mm:ss");

    QStandardItemModel & model = fast_order_2::modelBase;
    QSqlQueryModel model_query;
    QString discount;//是否折扣
    QString discount_num;//折扣值
    model_query.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='pay_auto_discount'"));
    discount = model_query.record(0).value(0).toString();
    model_query.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='pay_auto_discount_value'"));
    discount_num = model_query.record(0).value(0).toString();

    float xiaofei = 0.000;//消费金额
    float zhekou = 0.000;//折扣金额
    float zengsong = 0.000;//赠送金额
    float moling = 0.000;//抹零金额
    float shuishou = 0.000;//税收金额
    float yingshou = 0.000;//应收金额
    float xianjin = 0.000;//如果系统管理设置中设置了默认为现金付款，则现金付款为应收金额，否则为0.000；
    if(n_func::gs_operid == "0000")//用户名为系统管理员时
    {
        if(discount == "1")//设置了账单自动折扣
        {
            for(int i = 0; i < model.rowCount(); i++)
            {
                if(model.item(i,9)->text() != "Y")//套菜
                {
                    xiaofei += model.item(i,3)->text().toFloat() * model.item(i,4)->text().toFloat() + model.item(i,3)->text().toFloat() * model.item(i,21)->text().toFloat();
                    shuishou += model.item(i,3)->text().toFloat() * model.item(i,4)->text().toFloat() * model.item(i,17)->text().toFloat() * 0.01 ;
                }

                if(model.item(i,9)->text() != "Y" &&  model.item(i,10)->text() != "Y"/*特价*/ && model.item(i,18)->text() == "Y"/*折扣*/&& model.item(i,8)->text() == "N"/*赠送*/)
                {
                    model.item(i,5)->setText(QString().sprintf("%.02f",discount_num.toFloat()));
                    model.item(i,19)->setText(time);
                    float aa = 0.000;
                    aa = model.item(i,3)->text().toFloat() * model.item(i,4)->text().toFloat() * model.item(i,5)->text().toFloat() * 0.01 + model.item(i,3)->text().toFloat() * model.item(i,21)->text().toFloat();
                    model.item(i,6)->setText(QString().sprintf("%.02f",aa));
                    zhekou += model.item(i,3)->text().toFloat() * model.item(i,4)->text().toFloat() * (100 - discount_num.toFloat()) * 0.01;
                }

                if(model.item(i,8)->text() == "Y" && model.item(i,9)->text() != "Y")
                {
                    zengsong += model.item(i,3)->text().toFloat() * model.item(i,4)->text().toFloat();
                }
            }
        }
        else
        {
            for(int i = 0; i < model.rowCount(); i++)
            {
                if(model.item(i,9)->text() != "Y")
                {
                    xiaofei += model.item(i,3)->text().toFloat() * model.item(i,4)->text().toFloat()  + model.item(i,3)->text().toFloat() * model.item(i,21)->text().toFloat();
                    shuishou += model.item(i,3)->text().toFloat() * model.item(i,4)->text().toFloat() * model.item(i,17)->text().toFloat() * 0.01 ;
                }

                if(model.item(i,9)->text() != "Y" &&  model.item(i,10)->text() != "Y" && model.item(i,18)->text() == "Y" && model.item(i,5)->text() != "100" && model.item(i,8)->text() == "N")
                {
                    zhekou += model.item(i,3)->text().toFloat() * model.item(i,4)->text().toFloat() * (100-model.item(i,5)->text().toFloat()) * 0.01;
                }

                if(model.item(i,8)->text() == "Y" && model.item(i,9)->text() != "Y")
                {
                    zengsong += model.item(i,3)->text().toFloat() * model.item(i,4)->text().toFloat();
                }
            }
        }
    }
    else
    {
        if(discount == "1")//设置了账单自动折扣
        {
            QSqlQueryModel quanxian;
            quanxian.setQuery(QString("Select int_discount from cey_sys_operator where vch_operID ='%1' and ch_flag = 'Y' ").arg(n_func::gs_operid));
            float quanxian_zhi = 0.000;
            quanxian_zhi = quanxian.record(0).value(0).toFloat();
            if(quanxian_zhi <= discount_num.toFloat())
            {
                for(int i = 0; i < model.rowCount(); i++)
                {
                    if(model.item(i,9)->text() != "Y")
                    {
                        xiaofei += model.item(i,3)->text().toFloat() * model.item(i,4)->text().toFloat()  + model.item(i,3)->text().toFloat() * model.item(i,21)->text().toFloat();
                        shuishou += model.item(i,3)->text().toFloat() * model.item(i,4)->text().toFloat() * model.item(i,17)->text().toFloat() * 0.01 ;
                    }

                    if(model.item(i,9)->text() != "Y" &&  model.item(i,10)->text() != "Y" && model.item(i,18)->text() == "Y" && model.item(i,8)->text() == "N")
                    {
                        model.item(i,5)->setText(QString().sprintf("%.02f",discount_num.toFloat()));
                        model.item(i,19)->setText(time);
                        float aa = 0.000;
                        aa = model.item(i,3)->text().toFloat() * model.item(i,4)->text().toFloat() * model.item(i,5)->text().toFloat() * 0.01 + model.item(i,3)->text().toFloat() * model.item(i,21)->text().toFloat();
                        model.item(i,6)->setText(QString().sprintf("%.02f",aa));
                        zhekou += model.item(i,3)->text().toFloat() * model.item(i,4)->text().toFloat() * (100 - discount_num.toFloat()) * 0.01;
                    }

                    if(model.item(i,8)->text() == "Y" && model.item(i,9)->text() != "Y")
                    {
                        zengsong += model.item(i,3)->text().toFloat() * model.item(i,4)->text().toFloat();
                    }
                }
            }
            else
            {
                for(int i = 0; i < model.rowCount(); i++)
                {
                    if(model.item(i,9)->text() != "Y")
                    {
                        xiaofei += model.item(i,3)->text().toFloat() * model.item(i,4)->text().toFloat() + model.item(i,3)->text().toFloat() * model.item(i,21)->text().toFloat();
                        shuishou += model.item(i,3)->text().toFloat() * model.item(i,4)->text().toFloat() * model.item(i,17)->text().toFloat() * 0.01 ;
                    }
                    if(model.item(i,9)->text() != "Y" &&  model.item(i,10)->text() != "Y" && model.item(i,18)->text() == "Y" && model.item(i,5)->text() != "100"&& model.item(i,8)->text() == "N")
                    {
                        zhekou += model.item(i,3)->text().toFloat() * model.item(i,4)->text().toFloat() * (100 -model.item(i,5)->text().toFloat()) * 0.01;
                    }

                    if(model.item(i,8)->text() == "Y" && model.item(i,9)->text() != "Y")
                    {
                        zengsong += model.item(i,3)->text().toFloat() * model.item(i,4)->text().toFloat();
                    }
                }
            }
        }
        else
        {
            for(int i = 0; i < model.rowCount(); i++)
            {
                if(model.item(i,9)->text() != "Y")
                {
                    xiaofei += model.item(i,3)->text().toFloat() * model.item(i,4)->text().toFloat() + model.item(i,3)->text().toFloat() * model.item(i,21)->text().toFloat();
                    shuishou += model.item(i,3)->text().toFloat() * model.item(i,4)->text().toFloat() * model.item(i,17)->text().toFloat() * 0.01 ;
                }

                if(model.item(i,9)->text() != "Y" &&  model.item(i,10)->text() != "Y" && model.item(i,18)->text() == "Y" && model.item(i,5)->text() != "100" && model.item(i,8)->text() == "N")
                {
                    zhekou += model.item(i,3)->text().toFloat() * model.item(i,4)->text().toFloat() * (100 -model.item(i,5)->text().toFloat()) * 0.01;
                }

                if(model.item(i,8)->text() == "Y" && model.item(i,9)->text() != "Y")
                {
                    zengsong += model.item(i,3)->text().toFloat() * model.item(i,4)->text().toFloat();
                }
            }
        }
    }

    float shijiyishou = 0.000;
    shijiyishou = xiaofei- zengsong - zhekou;
    float shijiyingshou = 0.000;
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
            float startyingshou = 0.000;
            startyingshou = shijiyishou;
            shijiyingshou = (int)shijiyishou;
            moling = startyingshou -shijiyingshou;
        }
        else if(str == "3")//到十元
        {
            int str;
            float startyingshou = 0.000;
            startyingshou = shijiyishou;
            str = (int)shijiyishou;
            QString str1 = QString::number(str);
            QString str2;
            // qDebug()<< "xxx"<<str1.right(1);
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
            float startyingshou = 0.000;
            startyingshou = shijiyishou;
            str = (int)shijiyishou;
            QString str1 = QString::number(str);
            QString str2;
            if(str < 100)
            {
                shijiyingshou = 0.000;
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
            float startyingshou = 0.000;
            startyingshou = shijiyishou;
            shijiyingshou = (int)((shijiyishou * 10) + 0.5) / 10.0;
            moling = startyingshou -shijiyingshou;
        }
        else if(str == "3")//到元
        {
            float startyingshou = 0.000;
            startyingshou = shijiyishou;
            shijiyingshou = (int)(shijiyishou + 0.5);
            moling = startyingshou -shijiyingshou;
        }
        else if(str == "4")//到十元
        {
            float startyingshou = 0.000;
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
        xianjin = 0.000;
    }
    else
    {
        xianjin = yingshou;
    }

    ui->lineEdit->setText(QString().sprintf("%.02f",xiaofei));

    ui->lineEdit_2->setText(QString().sprintf("%.02f",zhekou));
    ui->lineEdit_3->setText(QString().sprintf("%.02f",zengsong));
    ui->lineEdit_5->setText(QString().sprintf("%.02f",moling));
    ui->lineEdit_6->setText(QString().sprintf("%.02f",shuishou));
    ui->lineEdit_4->setText(QString().sprintf("%.02f",yingshou));
    ui->lineEdit_9->setText(QString().sprintf("%.02f",xianjin));
    ui->label_14->setText(QString().sprintf("%.02f",yingshou));

    if(!on_d_p_b_r){//当没有开启折扣，赠送，抹零，税率是，会之为0
        ui->lineEdit_2->setText("0.00");
        ui->lineEdit_3->setText("0.00");
        ui->lineEdit_5->setText("0.00");
        ui->lineEdit_6->setText("0.00");
        ui->label_14->setText(ui->lineEdit->text());
        ui->lineEdit_4->setText(ui->lineEdit->text());
    }
    update_lowcost_service();
    lineEdit_9_textChanged(ui->lineEdit_9->text());
    emit change();//发送信号到点菜页面
}

QString fast_pay::get_curdandao() const
{
    return bendandanhao;
}


void fast_pay::on_pushButton_21_clicked()//取消
{
    _parent->to_dish_title(_parent->_num, _parent->_total);
    if(table_model->rowCount() > 0)
    {
        if(0==lds_messagebox::warning(this, tr("消息提示"),tr("已有付款记录,是否确定退出?"),tr("确认"),tr("取消")))
        {
            emit change();//发送信号到点菜页面
            this->close();
            btn_t_f = false;
        }
    }
    else
    {
        emit change();//发送信号到点菜页面
        this->close();
        btn_t_f = false;
    }
}

void fast_pay::on_pushButton_15_clicked()//折扣
{
    QString time =n_func::f_get_sysdatetime().toString("yyyy-MM-dd hh:mm:ss");

    ;
    if(table_model->rowCount() > 0)
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("已有付款记录,不可进行打折处理!"));

        return;
    }

    QStandardItemModel & model = fast_order_2::modelBase;
    discount_dispose dispose;
    backheader::static_dialogexec(dispose, tr("折扣处理"));

    float xiaofei = 0.000;
    xiaofei = ui->lineEdit->text().toFloat();
    float zhekou = 0.000;
    float zengsong = ui->lineEdit_3->text().toFloat();
    float moling = 0.000;
    moling = ui->lineEdit_5->text().toFloat();
    float shuishou = 0.000;
    shuishou = ui->lineEdit_6->text().toFloat();
    float yingshou = 0.000;
    yingshou = ui->lineEdit_4->text().toFloat();
    float xianjin = 0.000;
    xianjin = ui->lineEdit_9->text().toFloat();

    if(dispose.btn_t_f == true)
    {
        for(int i = 0; i < model.rowCount(); i++)
        {
            if(model.item(i,9)->text() != "Y" && model.item(i,10)->text() != "Y" && model.item(i,8)->text() == "N" && model.item(i,18)->text() == "Y")
            {
                model.item(i,5)->setText(QString().sprintf("%.02f",dispose.zhekouzhi));
                model.item(i,19)->setText(time);
                float bb = 0.000;
                bb = model.item(i,3)->text().toFloat() * model.item(i,4)->text().toFloat() * dispose.zhekouzhi * 0.01;
                model.item(i,6)->setText(QString().sprintf("%.02f",bb));

                float aa =0.000;
                aa = model.item(i,3)->text().toFloat() * model.item(i,4)->text().toFloat() * (100 - dispose.zhekouzhi) * 0.01;
                zhekou += aa;
            }
        }
    }


    float shijiyishou = 0.000;
    shijiyishou = xiaofei- zengsong - zhekou;
    float shijiyingshou = 0.000;
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
            float startyingshou = 0.000;
            startyingshou = shijiyishou;
            shijiyingshou = (int)shijiyishou;
            moling = startyingshou -shijiyingshou;
        }
        else if(str == "3")//到十元
        {
            int str;
            float startyingshou = 0.000;
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
            float startyingshou = 0.000;
            startyingshou = shijiyishou;
            str = (int)shijiyishou;
            QString str1 = QString::number(str);
            QString str2;
            if(str < 100)
            {
                shijiyingshou = 0.000;
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
            float startyingshou = 0.000;
            startyingshou = shijiyishou;
            shijiyingshou = (int)((shijiyishou * 10) + 0.5) / 10.0;
            moling = startyingshou -shijiyingshou;
        }
        else if(str == "3")//到元
        {
            float startyingshou = 0.000;
            startyingshou = shijiyishou;
            shijiyingshou = (int)(shijiyishou + 0.5);
            moling = startyingshou -shijiyingshou;
        }
        else if(str == "4")//到十元
        {
            float startyingshou = 0.000;
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
        xianjin = 0.000;
    }
    else
    {
        xianjin = yingshou;
    }

    ui->lineEdit->setText(QString().sprintf("%.02f",xiaofei));
    ui->lineEdit_2->setText(QString().sprintf("%.02f",zhekou));
    ui->lineEdit_3->setText(QString().sprintf("%.02f",zengsong));
    ui->lineEdit_4->setText(QString().sprintf("%.02f",yingshou));
    ui->lineEdit_5->setText(QString().sprintf("%.02f",moling));
    ui->lineEdit_6->setText(QString().sprintf("%.02f",shuishou));
    ui->lineEdit_9->setText(QString().sprintf("%.02f",xianjin));
    ui->label_14->setText(QString().sprintf("%.02f",yingshou));

    update_lowcost_service();
    lineEdit_9_textChanged(ui->lineEdit_9->text());
    emit change();

}

void fast_pay::on_pushButton_17_clicked()//会员卡
{
    if(ui->label_14->text().toFloat() == 0)
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("已足额付款无需继续付款!"));
        return;
    }
    if(table_model->rowCount() > 0)
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("已有其他付款，不允许再使用会员卡付款!"));
        return;
    }

    vip_card card(ui->label_14->text(),ui->lineEdit->text(),ui->lineEdit_2->text(),ui->lineEdit_3->text(),ui->lineEdit_4->text(),ui->lineEdit_5->text(),ui->lineEdit_6->text(),ui->lineEdit_9->text(), this);
    QTimer::singleShot(100, &card,SLOT(on_pushButton_3_clicked()));
    backheader::static_dialogexec(card, tr("会员付款"));

    if(card.btn_t_f == true)
    {
        if(card.discount > 0 && card.discount < 100)//有折扣
        {
            if(card.ch_attribute == "1")
            {
                ui->lineEdit->setText(card._xf);
                ui->lineEdit_2->setText(card._zk);
                ui->lineEdit_3->setText(card._zs);
                ui->lineEdit_4->setText(card._ys);
                ui->lineEdit_5->setText(card._ml);
                ui->lineEdit_6->setText(card._ss);
                ui->lineEdit_9->setText(card._num);
                ui->label_14->setText(card._num);
                shifouyouhuika = true;
                youhuikahao = card.fukuankahao;
                timer = n_func::f_get_sysdatetime().toString("yyyy-MM-dd hh:mm:ss");
            }
            else
            {
                QSqlQuery model;
                model.prepare("select ch_paymodeno from cey_bt_paymode where vch_paymodename = :as_mode");
                model.bindValue(":as_mode",tr("充值卡"));
                model.exec();
                QString paytype;
                while(model.next()) paytype = model.value(0).toString();
                int row = table_model->rowCount();
                table_model->setRowCount(row + 1);
                table_model->setItem(row,0,new QStandardItem(QString::number(row+1))); //序号
                table_model->setItem(row,1,new QStandardItem(tr("充值卡")));if(card.is_yun_member) table_model->item(row, 1)->setData(QColor("green"), Qt::TextColorRole);
                table_model->setItem(row,2,new QStandardItem(card._card_pay));
                table_model->setItem(row,3,new QStandardItem(card._card_pay));
                table_model->setItem(row,4,new QStandardItem(card.fukuankahao));
                table_model->setItem(row,5,new QStandardItem(paytype));
                table_model->setItem(row,6,new QStandardItem(card.beizhu));
                table_model->setItem(row,7,new QStandardItem(card.time));
                ui->tableView->selectRow(row);
                ui->tableView->setColumnHidden(5,true);
                ui->tableView->setColumnHidden(6,true);
                ui->tableView->setColumnHidden(7,true);

                ui->lineEdit->setText(card._xf);
                ui->lineEdit_2->setText(card._zk);
                ui->lineEdit_3->setText(card._zs);
                ui->lineEdit_4->setText(card._ys);
                ui->lineEdit_5->setText(card._ml);
                ui->lineEdit_6->setText(card._ss);
                ui->lineEdit_9->setText(card._xj);
                ui->label_14->setText(ui->lineEdit_9->text());
                ui->label_12->setText(card._card_pay);
                shifouyouhuika = false;
                youhuikahao = "";
            }
        }
        else//无折扣
        {
            if(card.ch_attribute == "1")
            {
                ui->lineEdit->setText(card._xf);
                ui->lineEdit_2->setText(card._zk);
                ui->lineEdit_3->setText(card._zs);
                ui->lineEdit_4->setText(card._ys);
                ui->lineEdit_5->setText(card._ml);
                ui->lineEdit_6->setText(card._ss);
                ui->lineEdit_9->setText(card._num);
                ui->label_14->setText(card._num);
                shifouyouhuika = false;
                youhuikahao = card.fukuankahao;
            }
            else
            {
                QSqlQuery model;
                model.prepare("select ch_paymodeno from cey_bt_paymode where vch_paymodename = :as_mode");
                model.bindValue(":as_mode",tr("充值卡"));
                model.exec();
                QString paytype;
                while(model.next()) paytype = model.value(0).toString();
                int row = table_model->rowCount();
                table_model->setRowCount(row + 1);
                table_model->setItem(row,0,new QStandardItem(QString::number(row+1))); //序号
                table_model->setItem(row,1,new QStandardItem(tr("充值卡")));if(card.is_yun_member) table_model->item(row, 1)->setData(QColor("green"), Qt::TextColorRole);
                table_model->setItem(row,2,new QStandardItem(card._card_pay));
                table_model->setItem(row,3,new QStandardItem(card._card_pay));
                table_model->setItem(row,4,new QStandardItem(card.fukuankahao));
                table_model->setItem(row,5,new QStandardItem(paytype));
                table_model->setItem(row,6,new QStandardItem(card.beizhu));
                table_model->setItem(row,7,new QStandardItem(card.time));
                ui->tableView->selectRow(row);
                ui->tableView->setColumnHidden(5,true);
                ui->tableView->setColumnHidden(6,true);
                ui->tableView->setColumnHidden(7,true);

                ui->lineEdit->setText(card._xf);
                ui->lineEdit_2->setText(card._zk);
                ui->lineEdit_3->setText(card._zs);
                ui->lineEdit_4->setText(card._ys);
                ui->lineEdit_5->setText(card._ml);
                ui->lineEdit_6->setText(card._ss);
                ui->lineEdit_9->setText(card._xj);
                ui->label_14->setText(ui->lineEdit_9->text());
                ui->label_12->setText(card._card_pay);
                shifouyouhuika = false;
                youhuikahao = "";
            }
        }
        update_lowcost_service();
        lineEdit_9_textChanged(ui->lineEdit_9->text());
    }//if on
}

void fast_pay::on_pushButton_18_clicked()//银行卡
{
    float label_14 = 0.000;
    label_14 = ui->label_14->text().toFloat();
    if(ui->label_14->text().toFloat() == 0)
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("已足额付款无需继续付款!"));
        return;
    }
    QString timer;
    bank_card bank("[03]银行卡",ui->label_14->text());
    /*bank.exec();*/backheader::static_dialogexec(bank, tr("其他收银操作"));
    if(bank.btn_t_f == true)
    {
        kafujine = bank.shishoujine;
        fukuanpingzheng = bank.pingzhenghao;
        QSqlQueryModel model;
        model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='cash_payall' "));
        QString str = model.record(0).value(0).toString();
        if(str == "1")//默认现金付全款
        {
            if(ui->label_14->text().toFloat() <= bank.shishoujine)
            {
                ui->lineEdit_9->setText("0.000");
                float l_12 = 0.000;
                l_12 = ui->label_14->text().toFloat() + ui->label_12->text().toFloat();
                ui->label_12->setText(QString().sprintf("%.02f",l_12));
                ui->label_14->setText("0.000");
            }
            else
            {
                float yue = 0.000;
                yue = ui->label_14->text().toFloat() - bank.shishoujine;
                ui->lineEdit_9->setText(QString().sprintf("%.02f",yue));
                ui->label_12->setText(QString().sprintf("%.02f",bank.shishoujine + ui->label_12->text().toFloat()));
                ui->label_14->setText(QString().sprintf("%.02f",yue));
            }
        }
        else//不是现金付全款
        {
            if(ui->label_14->text().toFloat() <= bank.shishoujine)
            {
                float l_12 = 0.000;
                l_12 = ui->label_14->text().toFloat() + ui->label_12->text().toFloat();
                ui->label_12->setText(QString().sprintf("%.02f",l_12));
                ui->label_14->setText("0.000");
                ui->lineEdit_9->setText("0.000");
                ui->lineEdit_7->setText("0.000");
            }
            else
            {
                float yue = 0.000;
                yue = ui->label_14->text().toFloat() - bank.shishoujine;
                ui->label_12->setText(QString().sprintf("%.02f",bank.shishoujine + ui->label_12->text().toFloat()));
                ui->label_14->setText(QString().sprintf("%.02f",yue));
                ui->lineEdit_9->setText("0.000");
                ui->lineEdit_7->setText("0.000");
            }
        }
        timer = bank.time;
    }
    else
    {
        return;
    }

    QSqlQuery model;
    model.prepare("select ch_paymodeno from cey_bt_paymode where vch_paymodename = :as_mode");
    model.bindValue(":as_mode",tr("银行卡"));
    model.exec();
    QString paytype;
    while(model.next())
    {
        paytype = model.value(0).toString();
    }

    int row = table_model->rowCount();
    table_model->setRowCount(row + 1);
    float t_shifuxianjin = 0.000;
    float t_fukuanjie = 0.000;
    if(label_14 <= kafujine)//付清
    {
        t_shifuxianjin = label_14;
        t_fukuanjie = kafujine;
    }
    else//未付清
    {
        t_shifuxianjin = kafujine;
        t_fukuanjie = kafujine;
    }
    table_model->setItem(row,0,new QStandardItem(QString::number(row+1))); //序号
    table_model->setItem(row,1,new QStandardItem(tr("银行卡")));
    table_model->setItem(row,2,new QStandardItem(QString().sprintf("%.02f",t_shifuxianjin)));
    table_model->setItem(row,3,new QStandardItem(QString().sprintf("%.02f",t_fukuanjie)));
    table_model->setItem(row,4,new QStandardItem(fukuanpingzheng));
    table_model->setItem(row,5,new QStandardItem(paytype));
    table_model->setItem(row,6,new QStandardItem(""));
    table_model->setItem(row,7,new QStandardItem(timer));
    ui->tableView->selectRow(row);
    update_lowcost_service();


    ui->tableView->setColumnHidden(5,true);
    ui->tableView->setColumnHidden(6,true);
    ui->tableView->setColumnHidden(7,true);
    lineEdit_9_textChanged(ui->lineEdit_9->text());

}

void fast_pay::on_pushButton_19_clicked()//代金券
{
    float label_14 = 0.000;
    label_14 = ui->label_14->text().toFloat();
    if(ui->label_14->text().toFloat() == 0)
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("已足额付款无需继续付款!"));

        return;
    }
    QString timer;
    bank_card bank("[04]代金券",ui->label_14->text());
    /*bank.exec();*/backheader::static_dialogexec(bank, tr("其他收银操作"));
    if(bank.btn_t_f == true)
    {
        kafujine = bank.shishoujine;
        fukuanpingzheng = bank.pingzhenghao;
        QSqlQueryModel model;
        model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='cash_payall' "));
        QString str = model.record(0).value(0).toString();
        if(str == "1")//默认现金付全款
        {
            if(ui->label_14->text().toFloat() <= bank.shishoujine)
            {
                ui->lineEdit_9->setText("0.000");
                float l_12 = 0.000;
                l_12 = ui->label_14->text().toFloat() + ui->label_12->text().toFloat();
                ui->label_12->setText(QString().sprintf("%.02f",l_12));
                ui->label_14->setText("0.000");
            }
            else
            {
                float yue = 0.000;
                yue = ui->label_14->text().toFloat() - bank.shishoujine;
                ui->lineEdit_9->setText(QString().sprintf("%.02f",yue));
                ui->label_12->setText(QString().sprintf("%.02f",bank.shishoujine + ui->label_12->text().toFloat()));
                ui->label_14->setText(QString().sprintf("%.02f",yue));
            }
        }
        else//不是现金付全款
        {
            if(ui->label_14->text().toFloat() <= bank.shishoujine)
            {
                float l_12 = 0.000;
                l_12 = ui->label_14->text().toFloat() + ui->label_12->text().toFloat();
                ui->label_12->setText(QString().sprintf("%.02f",l_12));
                ui->label_14->setText("0.000");
                ui->lineEdit_9->setText("0.000");
                ui->lineEdit_7->setText("0.000");
            }
            else
            {
                float yue = 0.000;
                yue = ui->label_14->text().toFloat() - bank.shishoujine;
                ui->label_12->setText(QString().sprintf("%.02f",bank.shishoujine + ui->label_12->text().toFloat()));
                ui->label_14->setText(QString().sprintf("%.02f",yue));
                ui->lineEdit_9->setText("0.000");
                ui->lineEdit_7->setText("0.000");
            }
        }
        timer = bank.time;
    }
    else
    {
        return;
    }

    QSqlQuery model;
    model.prepare("select ch_paymodeno from cey_bt_paymode where vch_paymodename = :as_mode");
    model.bindValue(":as_mode",tr("代金券"));
    model.exec();
    QString paytype;
    while(model.next())
    {
        paytype = model.value(0).toString();
    }

    int row = table_model->rowCount();
    table_model->setRowCount(row + 1);
    float t_shifuxianjin = 0.000;
    float t_fukuanjie = 0.000;
    if(label_14 <= kafujine)//付清
    {
        t_shifuxianjin = label_14;
        t_fukuanjie = kafujine;
    }
    else//未付清
    {
        t_shifuxianjin = kafujine;
        t_fukuanjie = kafujine;
    }

    table_model->setItem(row,0,new QStandardItem(QString::number(row+1))); //序号
    table_model->setItem(row,1,new QStandardItem(tr("代金券")));
    table_model->setItem(row,2,new QStandardItem(QString().sprintf("%.02f",t_shifuxianjin)));
    table_model->setItem(row,3,new QStandardItem(QString().sprintf("%.02f",t_fukuanjie)));
    table_model->setItem(row,4,new QStandardItem(fukuanpingzheng));
    table_model->setItem(row,5,new QStandardItem(paytype));
    table_model->setItem(row,6,new QStandardItem(""));
    table_model->setItem(row,7,new QStandardItem(timer));
    ui->tableView->selectRow(row);


    ui->tableView->setColumnHidden(5,true);
    ui->tableView->setColumnHidden(6,true);
    ui->tableView->setColumnHidden(7,true);
    update_lowcost_service();
    lineEdit_9_textChanged(ui->lineEdit_9->text());

}

void fast_pay::on_pushButton_20_clicked()//其他付款
{
    float label_14 = 0.000;
    label_14 = ui->label_14->text().toFloat();

    if(ui->label_14->text().toFloat() == 0)
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("已足额付款无需继续付款!"));

        return;
    }

    other_pay_view other_pay("1");
    /*other_pay.exec();*/backheader::static_dialogexec(other_pay, tr("其他付款"));
    if(other_pay.btn_t_f == true && other_pay.payfanshi.count() > 0)
    {
        QString timer;
        QString str = tr("[") + other_pay.payfanshi[other_pay.curindex_id] + tr("]") + other_pay.payname[other_pay.curindex_id];
        bank_card bank(str,ui->label_14->text());
        /*bank.exec();*/backheader::static_dialogexec(bank, tr("其他收银操作"));
        if(bank.btn_t_f == true)
        {
            kafujine = bank.shishoujine;
            fukuanpingzheng = bank.pingzhenghao;
            QSqlQueryModel model;
            model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='cash_payall' "));
            QString str = model.record(0).value(0).toString();
            if(str == "1")//默认现金付全款
            {
                if(ui->label_14->text().toFloat() <= bank.shishoujine)
                {
                    ui->lineEdit_9->setText("0.000");
                    float l_12 = 0.000;
                    l_12 = ui->label_14->text().toFloat() + ui->label_12->text().toFloat();
                    ui->label_12->setText(QString().sprintf("%.02f",l_12));
                    ui->label_14->setText("0.000");
                }
                else
                {
                    float yue = 0.000;
                    yue = ui->label_14->text().toFloat() - bank.shishoujine;
                    ui->lineEdit_9->setText(QString().sprintf("%.02f",yue));
                    ui->label_12->setText(QString().sprintf("%.02f",bank.shishoujine + ui->label_12->text().toFloat()));
                    ui->label_14->setText(QString().sprintf("%.02f",yue));
                }
            }
            else//不是现金付全款
            {
                if(ui->label_14->text().toFloat() <= bank.shishoujine)
                {
                    float l_12 = 0.000;
                    l_12 = ui->label_14->text().toFloat() + ui->label_12->text().toFloat();
                    ui->label_12->setText(QString().sprintf("%.02f",l_12));
                    ui->label_14->setText("0.000");
                    ui->lineEdit_9->setText("0.000");
                    ui->lineEdit_7->setText("0.000");
                }
                else
                {
                    float yue = 0.000;
                    yue = ui->label_14->text().toFloat() - bank.shishoujine;
                    ui->label_12->setText(QString().sprintf("%.02f",bank.shishoujine + ui->label_12->text().toFloat()));
                    ui->label_14->setText(QString().sprintf("%.02f",yue));
                    ui->lineEdit_9->setText("0.000");
                    ui->lineEdit_7->setText("0.000");
                }
            }
            timer = bank.time;
        }
        else
        {
            return;
        }

        QSqlQuery model;
        model.prepare("select ch_paymodeno from cey_bt_paymode where vch_paymodename = :as_mode");
        model.bindValue(":as_mode",other_pay.payname[other_pay.curindex_id]);
        model.exec();
        QString paytype;
        while(model.next())
        {
            paytype = model.value(0).toString();
        }

        int row = table_model->rowCount();
        table_model->setRowCount(row + 1);
        float t_shifuxianjin = 0.000;
        float t_fukuanjie = 0.000;
        if(label_14 <= kafujine)//付清
        {
            t_shifuxianjin = label_14;
            t_fukuanjie = kafujine;
        }
        else//未付清
        {
            t_shifuxianjin = kafujine;
            t_fukuanjie = kafujine;
        }

        table_model->setItem(row,0,new QStandardItem(QString::number(row+1))); //序号
        table_model->setItem(row,1,new QStandardItem(other_pay.payname[other_pay.curindex_id]));
        table_model->setItem(row,2,new QStandardItem(QString().sprintf("%.02f",t_shifuxianjin)));
        table_model->setItem(row,3,new QStandardItem(QString().sprintf("%.02f",t_fukuanjie)));
        table_model->setItem(row,4,new QStandardItem(fukuanpingzheng));
        table_model->setItem(row,5,new QStandardItem(paytype));
        table_model->setItem(row,6,new QStandardItem(""));
        table_model->setItem(row,7,new QStandardItem(timer));
        ui->tableView->selectRow(row);


        ui->tableView->setColumnHidden(5,true);
        ui->tableView->setColumnHidden(6,true);
        ui->tableView->setColumnHidden(7,true);
        update_lowcost_service();
        lineEdit_9_textChanged(ui->lineEdit_9->text());

    }
}

void fast_pay::on_pushButton_22_clicked()//特殊处理
{
    if(ui->label_14->text().toFloat() == 0)
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("已足额付款无需继续付款!"));

        return;
    }

    if(table_model->rowCount() > 0)
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("已有付款记录,不可进行特殊处理!"));

        return;
    }

    special_dispose dispose;
    /*dispose.exec();*/backheader::static_dialogexec(dispose, tr("特殊处理"));
    if(dispose.btn_t_f == true)
    {
        if(dispose.curindex_id == 0)//免单
        {
            free_view freeview(tr("免单操作"),ui->label_14->text());
            /*freeview.exec();*/backheader::static_dialogexec(freeview, tr("免单操作"));
            if(freeview.btn_t_f == true)
            {
                if(freeview.oper_id != "0000")
                {
                    QSqlQueryModel cey_sys;
                    cey_sys.setQuery(QString("select ch_free_flag from cey_sys_operator where vch_operID = '%1' ").arg(freeview.oper_id));
                    QString str = cey_sys.record(0).value(0).toString();
                    if(str == "N")
                    {
                        lds_messagebox::warning(this, tr("消息提示"),tr("sorry,该操作员没有免单权限!"));

                        return;
                    }
                    else
                    {
                        //添加table_model
                        ui->label_12->setText(ui->label_14->text());
                        ui->label_14->setText("0.000");
                        ui->lineEdit_9->setText("0.000");
                        QSqlQuery model;
                        model.prepare("select ch_paymodeno from cey_bt_paymode where vch_paymodename = :as_mode");
                        model.bindValue(":as_mode",tr("免单金额"));
                        model.exec();
                        QString paytype;
                        while(model.next())
                        {
                            paytype = model.value(0).toString();
                        }
                        int row = table_model->rowCount();
                        table_model->setItem(row,0,new QStandardItem(QString::number(row+1))); //序号
                        table_model->setItem(row,1,new QStandardItem(tr("免单")));
                        table_model->setItem(row,2,new QStandardItem(QString().sprintf("%.02f",ui->label_12->text().toFloat())));
                        table_model->setItem(row,3,new QStandardItem(QString().sprintf("%.02f",ui->label_12->text().toFloat())));
                        table_model->setItem(row,4,new QStandardItem(""));
                        table_model->setItem(row,5,new QStandardItem(paytype));
                        table_model->setItem(row,6,new QStandardItem(freeview.beizhu));
                        table_model->setItem(row,7,new QStandardItem(freeview.time));
                        ui->tableView->selectRow(row);

                        ui->tableView->setColumnHidden(5,true);
                        ui->tableView->setColumnHidden(6,true);
                        ui->tableView->setColumnHidden(7,true);
                        lineEdit_9_textChanged("");
                    }
                }
                else//免单为0000
                {
                    ui->label_12->setText(ui->label_14->text());
                    ui->label_14->setText("0.000");
                    ui->lineEdit_9->setText("0.000");
                    QSqlQuery model;
                    model.prepare("select ch_paymodeno from cey_bt_paymode where vch_paymodename = :as_mode");
                    model.bindValue(":as_mode",tr("免单金额"));
                    model.exec();
                    QString paytype;
                    while(model.next())
                    {
                        paytype = model.value(0).toString();
                    }
                    int row = table_model->rowCount();
                    table_model->setItem(row,0,new QStandardItem(QString::number(row+1))); //序号
                    table_model->setItem(row,1,new QStandardItem(tr("免单")));
                    table_model->setItem(row,2,new QStandardItem(QString().sprintf("%.02f",ui->label_12->text().toFloat())));
                    table_model->setItem(row,3,new QStandardItem(QString().sprintf("%.02f",ui->label_12->text().toFloat())));
                    table_model->setItem(row,4,new QStandardItem(""));
                    table_model->setItem(row,5,new QStandardItem(paytype));
                    table_model->setItem(row,6,new QStandardItem(freeview.beizhu));
                    table_model->setItem(row,7,new QStandardItem(freeview.time));
                    ui->tableView->selectRow(row);

                    ui->tableView->setColumnHidden(5,true);
                    ui->tableView->setColumnHidden(6,true);
                    ui->tableView->setColumnHidden(7,true);
                    lineEdit_9_textChanged("");
                }
            }
            else
            {
                return;
            }
        }
        else//抹零
        {
            free_view freeview(tr("手工抹零"),ui->label_14->text());
            /*freeview.exec();*/backheader::static_dialogexec(freeview, tr("手工抹零"));
            if(freeview.btn_t_f == true)
            {
                if(freeview.oper_id != "0000")
                {
                    QSqlQueryModel cey_sys;
                    cey_sys.setQuery(QString("select num_free from cey_sys_operator where vch_operID = '%1' ").arg(freeview.oper_id));
                    float aa = 0.000;
                    aa = cey_sys.record(0).value(0).toFloat();
                    if(freeview.line2_num > aa)
                    {
                        lds_messagebox::warning(this, tr("消息提示"),tr("sorry,您的抹零金额超出了您的最大抹零金额!"));

                        return;
                    }
                    else
                    {
                        if(freeview.line2_num >= ui->label_14->text().toFloat())//全部抹零
                        {
                            //添加table_model
                            float bb;
                            bb = 0.000;
                            bb = ui->lineEdit_5->text().toFloat() + freeview.line2_num;
                            ui->lineEdit_5->setText(QString().sprintf("%.02f",bb));
                            ui->label_12->setText(ui->label_14->text());
                            ui->label_14->setText("0.000");
                            if(ui->lineEdit_9->text().toFloat() != 0)
                            {
                                ui->lineEdit_9->setText("0.000");
                            }
                            QSqlQuery model;
                            model.prepare("select ch_paymodeno from cey_bt_paymode where vch_paymodename = :as_mode");
                            model.bindValue(":as_mode",tr("手工抹零"));
                            model.exec();
                            QString paytype;
                            while(model.next())
                            {
                                paytype = model.value(0).toString();
                            }

                            int row = table_model->rowCount();
                            table_model->setItem(row,0,new QStandardItem(QString::number(row+1))); //序号
                            table_model->setItem(row,1,new QStandardItem(tr("手工抹零")));
                            table_model->setItem(row,2,new QStandardItem(QString().sprintf("%.02f",ui->label_12->text().toFloat())));
                            table_model->setItem(row,3,new QStandardItem(QString().sprintf("%.02f",ui->label_12->text().toFloat())));
                            table_model->setItem(row,4,new QStandardItem(""));
                            table_model->setItem(row,5,new QStandardItem(paytype));
                            table_model->setItem(row,6,new QStandardItem(freeview.beizhu));
                            table_model->setItem(row,7,new QStandardItem(freeview.time));
                            ui->tableView->selectRow(row);

                            ui->tableView->setColumnHidden(5,true);
                            ui->tableView->setColumnHidden(6,true);
                            ui->tableView->setColumnHidden(7,true);
                            lineEdit_9_textChanged("");
                        }
                        else//部分抹零
                        {
                            //添加table_model
                            float aa = 0.000;
                            aa = ui->label_14->text().toFloat() - freeview.line2_num;
                            ui->label_12->setText(QString().sprintf("%.02f",freeview.line2_num));
                            ui->label_14->setText(QString().sprintf("%.02f",aa));
                            if(ui->lineEdit_9->text().toFloat() != 0)
                            {
                                ui->lineEdit_9->setText(QString().sprintf("%.02f",aa));
                            }

                            QSqlQuery model;
                            model.prepare("select ch_paymodeno from cey_bt_paymode where vch_paymodename = :as_mode");
                            model.bindValue(":as_mode",tr("手工抹零"));
                            model.exec();
                            QString paytype;
                            while(model.next())
                            {
                                paytype = model.value(0).toString();
                            }

                            float bb;
                            bb = 0.000;
                            bb = ui->lineEdit_5->text().toFloat() + freeview.line2_num;
                            ui->lineEdit_5->setText(QString().sprintf("%.02f",bb));
                            int row = table_model->rowCount();
                            table_model->setItem(row,0,new QStandardItem(QString::number(row+1))); //序号
                            table_model->setItem(row,1,new QStandardItem(tr("手工抹零")));
                            table_model->setItem(row,2,new QStandardItem(QString().sprintf("%.02f",freeview.line2_num)));
                            table_model->setItem(row,3,new QStandardItem(QString().sprintf("%.02f",freeview.line2_num)));
                            table_model->setItem(row,4,new QStandardItem(""));
                            table_model->setItem(row,5,new QStandardItem(paytype));
                            table_model->setItem(row,6,new QStandardItem(freeview.beizhu));
                            table_model->setItem(row,7,new QStandardItem(freeview.time));
                            ui->tableView->selectRow(row);

                        }
                        ui->tableView->setColumnHidden(5,true);
                        ui->tableView->setColumnHidden(6,true);
                        ui->tableView->setColumnHidden(7,true);
                        lineEdit_9_textChanged("");
                    }
                }
                else//免单为0000
                {
                    if(freeview.line2_num >= ui->label_14->text().toFloat())//全部抹零
                    {
                        //添加table_model
                        ui->label_12->setText(ui->label_14->text());
                        ui->label_14->setText("0.000");
                        if(ui->lineEdit_9->text().toFloat() != 0)
                        {
                            ui->lineEdit_9->setText("0.000");
                        }

                        QSqlQuery model;
                        model.prepare("select ch_paymodeno from cey_bt_paymode where vch_paymodename = :as_mode");
                        model.bindValue(":as_mode",tr("手工抹零"));
                        model.exec();
                        QString paytype;
                        while(model.next())
                        {
                            paytype = model.value(0).toString();
                        }

                        int row = table_model->rowCount();
                        table_model->setItem(row,0,new QStandardItem(QString::number(row+1))); //序号
                        table_model->setItem(row,1,new QStandardItem(tr("手工抹零")));
                        table_model->setItem(row,2,new QStandardItem(QString().sprintf("%.02f",ui->label_12->text().toFloat())));
                        table_model->setItem(row,3,new QStandardItem(QString().sprintf("%.02f",ui->label_12->text().toFloat())));
                        table_model->setItem(row,4,new QStandardItem(""));
                        table_model->setItem(row,5,new QStandardItem(paytype));
                        table_model->setItem(row,6,new QStandardItem(freeview.beizhu));
                        table_model->setItem(row,7,new QStandardItem(freeview.time));
                        ui->tableView->selectRow(row);


                        ui->tableView->setColumnHidden(5,true);
                        ui->tableView->setColumnHidden(6,true);
                        ui->tableView->setColumnHidden(7,true);
                        lineEdit_9_textChanged("");
                    }
                    else//部分抹零
                    {
                        //添加table_model
                        float aa = 0.000;
                        aa = ui->label_14->text().toFloat() - freeview.line2_num;
                        ui->label_12->setText(QString().sprintf("%.02f",freeview.line2_num));
                        ui->label_14->setText(QString().sprintf("%.02f",aa));
                        if(ui->lineEdit_9->text().toFloat() != 0)
                        {
                            ui->lineEdit_9->setText(QString().sprintf("%.02f",aa));
                        }
                        QSqlQuery model;
                        model.prepare("select ch_paymodeno from cey_bt_paymode where vch_paymodename = :as_mode");
                        model.bindValue(":as_mode",tr("手工抹零"));
                        model.exec();
                        QString paytype;
                        while(model.next())
                        {
                            paytype = model.value(0).toString();
                        }

                        int row = table_model->rowCount();
                        table_model->setItem(row,0,new QStandardItem(QString::number(row+1))); //序号
                        table_model->setItem(row,1,new QStandardItem(tr("手工抹零")));
                        table_model->setItem(row,2,new QStandardItem(QString().sprintf("%.02f",freeview.line2_num)));
                        table_model->setItem(row,3,new QStandardItem(QString().sprintf("%.02f",freeview.line2_num)));
                        table_model->setItem(row,4,new QStandardItem(""));
                        table_model->setItem(row,5,new QStandardItem(paytype));
                        table_model->setItem(row,6,new QStandardItem(freeview.beizhu));
                        table_model->setItem(row,7,new QStandardItem(freeview.time));
                        ui->tableView->selectRow(row);


                        ui->tableView->setColumnHidden(5,true);
                        ui->tableView->setColumnHidden(6,true);
                        ui->tableView->setColumnHidden(7,true);
                        lineEdit_9_textChanged("");
                    }
                }
            }
            else
            {
                return;
            }
        }
    }
    update_lowcost_service();
    lineEdit_9_textChanged(ui->lineEdit_9->text());

}

void fast_pay::on_pushButton_23_clicked()//会员充值
{
    w_m_member_list::statictorecharge(this, 0, 0,"");
    //    int ret=lds_messagebox::warning(this,tr("会员充值/取款"), tr("会员充值/取款"),tr("充值"),tr("取款"));
    //    if(ret==0){
    //        w_m_member_list::statictorecharge(this, 0, 0,"");
    //    }
    //    if(ret==1){
    //        w_m_member_list::statictorecharge(this, 0, 0,"C");
    //    }
}

void fast_pay::on_pushButton_clicked()//7
{
    count_dis++;
    ui->lineEdit_9->insert("7");
}

void fast_pay::on_pushButton_2_clicked()//8
{
    count_dis++;
    ui->lineEdit_9->insert("8");
}

void fast_pay::on_pushButton_3_clicked()//9
{
    count_dis++;
    ui->lineEdit_9->insert("9");
}

void fast_pay::on_pushButton_4_clicked()//50
{
    count_dis++;
    ui->lineEdit_9->clear();
    ui->lineEdit_9->insert(tr("50"));
}

void fast_pay::on_pushButton_6_clicked()//4
{
    count_dis++;
    ui->lineEdit_9->insert("4");
}

void fast_pay::on_pushButton_8_clicked()//5
{
    count_dis++;
    ui->lineEdit_9->insert("5");
}

void fast_pay::on_pushButton_5_clicked()//6
{
    count_dis++;
    ui->lineEdit_9->insert("6");
}

void fast_pay::on_pushButton_7_clicked()//100
{
    count_dis++;
    ui->lineEdit_9->clear();
    ui->lineEdit_9->insert(tr("100"));
}

void fast_pay::on_pushButton_10_clicked()//1
{
    count_dis++;
    ui->lineEdit_9->insert("1");
}

void fast_pay::on_pushButton_12_clicked()//2
{
    count_dis++;
    ui->lineEdit_9->insert("2");
}

void fast_pay::on_pushButton_9_clicked()//3
{
    count_dis++;
    ui->lineEdit_9->insert("3");
}

void fast_pay::on_pushButton_11_clicked()//d
{
    ui->lineEdit_9->backspace();
}

void fast_pay::on_pushButton_14_clicked()//0
{
    count_dis++;
    ui->lineEdit_9->insert("0");
}

void fast_pay::on_pushButton_16_clicked()//.
{
    count_dis++;
    ui->lineEdit_9->insert(tr("."));
}

void fast_pay::on_pushButton_13_clicked()//买单
{
    //开始付款
    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    to_pay(conf.value("system_setting/fukuanshishurushoupai").toBool());
    if(pay_finished){//付款成功
        if(_parent)
            _parent->to_dish_title_other("", tr("找零")+ui->lineEdit_7->text());
        this->accept();
    } else {
        lds_messagebox::warning(this,tr("付款"),tr("付款失败"));
    }
}

void fast_pay::to_pay(bool is_manul_input_serialno, bool isprint)
{
    //判断是云会员付款
    pay_finished=false;
    float idec_service=ui->lineEdit_14->text().toFloat();
    float idec_lowcost=ui->lineEdit_11->text().toFloat();
    float ldec_backcash = 0.0;
    float ldec_remain_amt=0.0;
    QString ls_err;
    QString yun_memberno;
    member_up_yun muy;
    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    for(int i = 0; i < table_model->rowCount(); i++)
    {
        if(table_model->item(i,5)->text().trimmed() == "02"){
            if(table_model->item(i,1)->data(Qt::TextColorRole) == QColor("green")){
                yun_memberno=table_model->item(i, 4)->text();//卡号
                break;
            }
        }
    }
    if(!yun_memberno.isEmpty()){
        w_m_member_list::yun_member_open();
        muy.isrun=true;
    }
    yun_pre_memberno=yun_memberno;
    //!判断是云会员付款

    int loopcount=2;
    while(loopcount){
        if(muy.isrun){
            //若是实际是付款成功的，则直接退出，打印相关小票
            if(!yun_pre_billno.isEmpty()
                    &&w_sys_manage_cloudsync::get_yun_member_checkbillnopay(yun_pre_billno, yun_pre_memberno)){
                pay_finished=true;
                goto GOTO_FAST_PAY_PAY_FINISHED_END;
            }
        }
        //发送找零到客显
        if(_parent)
            _parent->emit_signal_display_change(ui->lineEdit_7->text());
        if(fast_order_2::modelBase.rowCount() == 0)
        {
            goto GOTO_FAST_PAY_PAY_FINISHED_END;
        }

        if(ui->lineEdit_8->text() != frontViewDialog_virtual::cur_desk_no)
        {
            lds_messagebox::warning(this, tr("消息提示"),tr("输入台号有误,请核实!"));
            goto GOTO_FAST_PAY_PAY_FINISHED_END;
        }

        if(ui->lineEdit_9->text().toFloat() + ui->label_12->text().toFloat() < ui->lineEdit_4->text().toFloat())
        {
            lds_messagebox::warning(this, tr("消息提示"),tr("对不起,您的付款金额不够!"));

            goto GOTO_FAST_PAY_PAY_FINISHED_END;
        }

        if(ui->label_14->text().toFloat() > 0 && ui->lineEdit_7->text().toFloat() >= 100)
        {
            lds_messagebox::warning(this, tr("消息提示"),tr("您输入的金额已超出最大找零金额100元,请核实!"));

            goto GOTO_FAST_PAY_PAY_FINISHED_END;
        }

        QSqlQueryModel m_billno;
        QString liushuihao;
        QString danhao;

        m_billno.setQuery("Select NOW()");
        pay_timer = m_billno.record(0).value(0).toDateTime().toString("yyyy-MM-dd hh:mm:ss");
        QDateTime ldt_datetime;
        ldt_datetime.setTime(m_billno.record(0).value(0).toTime());
        ldt_datetime.setDate(m_billno.record(0).value(0).toDate());
        QString time = m_billno.record(0).value(0).toDateTime().toString("yyyy-MM-dd hh:mm:ss");
        QString currentDate = QString(muy.isrun?"K":"Q")+ ldt_datetime.toString("yyMMdd");
        QString guqingdate = ldt_datetime.toString("yyyy-MM-dd");

        if(muy.isrun){
            danhao=w_sys_manage_cloudsync::get_yun_member_maxorderno("maxOrderNo", QVariant::String);if(!w_sys_manage_cloudsync::this_action_is_successed) goto GOTO_FAST_PAY_PAY_FINISHED_END;
        } else {
            m_billno.setQuery(QString("select Max(ch_billno) from cey_u_master where Left(ch_billno,7) = '%1' ").arg(currentDate));
            danhao = m_billno.record(0).value(0).toString().trimmed();
        }


        //Q111111 00001
        if(danhao.trimmed().isEmpty())
        {
            bendandanhao=danhao = currentDate + QString().sprintf("%05d",1);
        }
        else
        {
            QString str = danhao.right(5);
            int aa = str.toInt() + 1;
            bendandanhao =  danhao = currentDate + QString().sprintf("%05d",aa);
        }
        yun_pre_billno=danhao;
        /*若是，第二次进入是，最大单号会按照云端的最好单号算*/
        liushuihao_p=system_setting::get_max_serialno_add1(danhao);
        //是否输入手牌号
        liushuihao_p_is_shoudog.clear();
        if(is_manul_input_serialno){
            lds_dialog_input inputdialog;
            inputdialog.ui->label->setText(tr("手牌号:"));
            inputdialog.ui->lineEdit->setMaxLength(3);
            inputdialog.ui->lineEdit->setText(liushuihao_p);
            if(QDialog::Accepted==backheader::static_dialogexec(inputdialog, tr("请输入手牌号"))){
                liushuihao_p_is_shoudog=inputdialog._text;
            } else {
                goto GOTO_FAST_PAY_PAY_FINISHED_END;
            }
        }
        m_billno.clear();
        QString liushi = (muy.isrun?"F":"P") + ldt_datetime.toString("yyMMdd");
        if(muy.isrun){
            liushuihao=w_sys_manage_cloudsync::get_yun_member_maxmasterno("maxMasterNo", QVariant::String);if(!w_sys_manage_cloudsync::this_action_is_successed) goto GOTO_FAST_PAY_PAY_FINISHED_END;
        } else {
            m_billno.setQuery(QString("select Max(ch_payno) from cey_u_checkout_master where Left(ch_payno,7) = '%1' ").arg(liushi));
            liushuihao = m_billno.record(0).value(0).toString().trimmed();
        }
        if(liushuihao.trimmed().isEmpty())
        {
            liushuihao = liushi + QString().sprintf("%05d",1);
            wdanhao = liushi + QString().sprintf("%05d",1);
        }
        else
        {
            QString str = liushuihao.right(liushuihao.length() - 7);
            int aa = str.toInt() + 1;
            liushuihao = liushi + QString().sprintf("%05d",aa);
            wdanhao = liushi + QString().sprintf("%05d",aa);
        }
        QSqlQuery query;
        QSqlDatabase::database().transaction();
        QString zhuohao = frontViewDialog_virtual::cur_desk_no;

        if(query.exec(QString("insert into cey_u_master( ch_billno , ch_state , vch_operID , dt_operdate)values('%1','%2','%3','%4')")
                      .arg(danhao).arg("1").arg(n_func::gs_operid).arg(time)))
        {

            if(muy.isrun){
                orderMaster o;
                o.omChBillNo=danhao;
                o.omChState="1";
                o.omDtOperDate=time;
                o.omVchOperId=n_func::gs_operid;
                muy.appitem(o);
            }
        }
        else
        {
            QSqlDatabase::database().rollback();
            qDebug() << query.lastError();  lds_messagebox::warning(this, tr("消息提示"),tr("插入into cey_u_master失败!"));

            goto GOTO_FAST_PAY_PAY_FINISHED_END;
        }

        //付款明细
        if(query.exec(QString("insert into cey_u_checkout_master( ch_payno , ch_billno , ch_tableno , num_cost , num_discount , num_present , num_service , num_lowcost , num_blotout ,  num_run , vch_run_operID , ch_state , vch_operID , dt_operdate , vch_operID2 , dt_operdate2 ,num_rate) values('%1','%2','%3','%4','%5','%6','%7','%8','%9','%10', null,'%11','%12','%13',null,null,'%14')")
                      .arg(liushuihao).arg(danhao).arg(zhuohao).arg(ui->lineEdit->text()).arg(ui->lineEdit_2->text()).
                      arg(ui->lineEdit_3->text()).arg(ui->lineEdit_14->text()).arg(ui->lineEdit_13->text()).arg(ui->lineEdit_5->text()).arg("0").arg("Y").arg(n_func::gs_operid).
                      arg(time).arg(ui->lineEdit_6->text())))
        {
            if(muy.isrun){
                checkOutMaster o;
                o.comChPayNo=liushuihao;
                o.comChBillNo=danhao;
                o.comChTableNo=zhuohao;
                o.comNumCost=ui->lineEdit->text().toFloat();
                o.comNumDiscount=ui->lineEdit_2->text().toFloat();

                o.comNumPresent=ui->lineEdit_3->text().toFloat();
                o.comNumService=ui->lineEdit_14->text().toFloat();
                o.comNumLowCost=ui->lineEdit_13->text().toFloat();
                o.comNumBlotout=ui->lineEdit_5->text().toFloat();
                o.comNumRun=0;
                //o.comVchRunOperId="Y";

                o.comChState=="Y";
                o.comVchOperId=n_func::gs_operid;
                o.comDtOperDate=time;
                o.comNumRate=ui->lineEdit_6->text().toFloat();

                muy.appitem(o);
            }
        }else
        {
            qDebug() << query.lastQuery();
            QSqlDatabase::database().rollback();
            qDebug() << query.lastError();  lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_u_checkout_master失败!"));

            goto GOTO_FAST_PAY_PAY_FINISHED_END;
        }

        //服务非
        //        FOR ll_row = 1 TO dw_bill.rowcount() //157
        //            dw_bill.object.ch_payno[ll_row] =ls_payno
        //        NEXT //157
        //        IF THIS.idec_service <> 0 THEN //-162
        //        ELSE //-162
        //            GOTO LABEL_KENSHU_188
        //        END IF //-162
        //        IF THIS.ib_service_free THEN //164
        //            INSERT INTO cey_u_service_low ( ch_payno , ch_type , num_amount , vch_operID , dt_operdate , ch_state , vch_memo )
        //            VALUES ( :ls_payno , '1' , :THIS.idec_service , :THIS.is_service_operid , :ldt_datetime , '0' , null )
        //            USING sqlca;
        //        ELSE //164
        //            INSERT INTO cey_u_service_low ( ch_payno , ch_type , num_amount , vch_operID , dt_operdate , ch_state , vch_memo )
        //            VALUES ( :ls_payno , '1' , :THIS.idec_service , :gvar.gs_operid , :ldt_datetime , '1' , null )
        //            USING sqlca;
        //        END IF //164
        //        IF sqlca.sqlcode <> 0 THEN //-169
        //        ELSE //-169
        //            GOTO LABEL_KENSHU_188
        //        END IF //-169
        //        ls_err = sqlca.sqlerrtext
        //        GOTO LABEL_KENSHU_183
        //餐桌
        if(query.exec(QString("insert into cey_u_table( ch_billno , ch_tableno , int_person , vch_memo ,  ch_state , ch_payno , vch_operID , dt_operdate , ch_order_state , num_cost)values('%1','%2','%3',null,'%4','%5','%6','%7','%8','%9')")
                      .arg(danhao).arg(zhuohao).arg("1").arg("1").arg(liushuihao).arg(n_func::gs_operid).arg(time).
                      arg("4").arg(ui->lineEdit->text())))
        {
            if(muy.isrun){
                tableRecordsConsumption o;
                o.chBillno=danhao;
                o.chTableno=zhuohao;
                o.intPerson=1*1.0;
                //o.vchMemo="1";
                o.chState="1";
                o.chPayno=liushuihao;
                o.vchOperId=n_func::gs_operid;
                o.dtOperdate=time;
                o.chOrderState="4";
                o.numCost=ui->lineEdit_4->text().toFloat();
                muy.appitem(o);
            }
        }
        else
        {
            QSqlDatabase::database().rollback();
            qDebug() << query.lastError();  lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_u_table失败!"));

            goto GOTO_FAST_PAY_PAY_FINISHED_END;
        }

        if(zhuohao=="Q000"//若是中餐默认的餐桌号
                ||query.exec(QString("update cey_bt_table set ch_billno = null where ch_tableno='%1' ").arg(zhuohao))){

        } else {
            qDebug() << query.lastQuery();
            QSqlDatabase::database().rollback();
            lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_bt_table失败!"));
            goto GOTO_FAST_PAY_PAY_FINISHED_END;
        }


        //==============================================================================
        //现金付款
        if(ui->label_14->text().toFloat() != 0)
        {
            float aa = 0.000;
            aa = ui->lineEdit_9->text().toFloat() - ui->lineEdit_7->text().toFloat();
            if(query.exec(QString("insert into cey_u_checkout_detail(ch_payno , ch_paymodeno ,  num_payamount , num_change , num_realamount , num_face ,vch_operID ,  dt_operdate ,num_card_remain,vch_voucherno, num_face_Num  )values('%1','%2','%3','%4','%5','%6','%7','%8','%9','null','%10')")
                          .arg(liushuihao).arg("00").arg(ui->lineEdit_9->text()).arg(ui->lineEdit_7->text())
                          .arg(QString().sprintf("%.02f",aa)).arg("0.0")
                          .arg(n_func::gs_operid).arg(time).arg("0.000").arg("0.0")))
            {
                if(muy.isrun){
                    checkOutDetail o;
                    o.codChPayNo=liushuihao;
                    o.codChPayModeNo="00";
                    o.codNumPayAmount=ui->lineEdit_9->text().toFloat();
                    o.codNumChange=ui->lineEdit_7->text().toFloat();
                    o.codNumRealAmount=aa;

                    o.codNumFace=0;
                    o.codVchOperId=n_func::gs_operid;
                    o.codDtOperDate=time;
                    o.codNumCardRemain=0;
                    //                    o.codVchVoucherNo;
                    o.codNumFaceNum=0;
                    muy.appitem(o);
                }
            }
            else
            {
                QSqlDatabase::database().rollback();
                lds_messagebox::warning(this, tr("消息提示"),tr("cey_u_checkout_detail失败!"));

                goto GOTO_FAST_PAY_PAY_FINISHED_END;
            }
        }

        if(ui->lineEdit_5->text().toFloat() != 0)//存在自动抹零
        {
            QSqlQuery model;
            model.prepare("select ch_paymodeno from cey_bt_paymode where vch_paymodename = :as_mode");
            model.bindValue(":as_mode",tr("自动抹零"));
            model.exec();
            QString paytype;
            while(model.next())
            {
                paytype = model.record().value(0).toString();
            }
            if(query.exec(QString("insert into cey_u_checkout_detail(ch_payno , ch_paymodeno ,num_payamount , num_change , num_realamount , num_face ,vch_operID , dt_operdate ,num_card_remain, vch_voucherno, num_face_Num )values('%1','%2','%3','%4','%5' ,'%6','%7','%8','%9','null','%10')")
                          .arg(liushuihao).arg(paytype).arg(ui->lineEdit_5->text()).arg("0.0").arg(ui->lineEdit_5->text()).
                          arg("0.0")
                          .arg(n_func::gs_operid).arg(time).arg("0.000").arg("0.0")))
            {
                if(muy.isrun){
                    checkOutDetail o;
                    o.codChPayNo=liushuihao;
                    o.codChPayModeNo=paytype;
                    o.codNumPayAmount=ui->lineEdit_5->text().toFloat();
                    o.codNumChange=0;
                    o.codNumRealAmount=ui->lineEdit_5->text().toFloat();

                    o.codNumFace=0;
                    o.codVchOperId=n_func::gs_operid;
                    o.codDtOperDate=time;
                    muy.appitem(o);
                }
            }
            else
            {
                QSqlDatabase::database().rollback();
                lds_messagebox::warning(this, tr("消息提示"),tr("cey_u_checkout_detail失败!"));

                goto GOTO_FAST_PAY_PAY_FINISHED_END;
            }
        }

        if(shifouyouhuika == true)
        {
            //添加优惠卡付款
            float jinfen = 0.000;
            QString fen;
            QSqlQueryModel vip;

            if(w_m_member_list::is_yun_member){
                QString m_type=w_sys_manage_cloudsync::get_yun_member_readcard(youhuikahao, conf.value("w_sys_manage_cloudsync_basedataset/cloudUser").toString(),
                                                                               "ch_typeno", QVariant::String);if(!w_sys_manage_cloudsync::this_action_is_successed) goto GOTO_FAST_PAY_PAY_FINISHED_END;
                QString chCalcType=w_sys_manage_cloudsync::get_yun_member_membertype(m_type,
                                                                                     "chCalcType", QVariant::String);if(!w_sys_manage_cloudsync::this_action_is_successed) goto GOTO_FAST_PAY_PAY_FINISHED_END;
                int intCalcUnit=w_sys_manage_cloudsync::get_yun_member_membertype(m_type,
                                                                                  "intCalcUnit", QVariant::Int).toInt();if(!w_sys_manage_cloudsync::this_action_is_successed) goto GOTO_FAST_PAY_PAY_FINISHED_END;
                jinfen = ui->lineEdit->text().toFloat() / intCalcUnit;
                if(chCalcType=="0"){
                    fen = QString::number(int(jinfen));
                }
                else
                {
                    fen = QString().sprintf("%.02f",jinfen);
                }
            } else {
                vip.setQuery(QString("select ch_typeno from t_m_member where vch_memberno = '%1' ").arg(youhuikahao));
                QString str = vip.record(0).value(0).toString();//得到类型编号
                vip.clear();
                vip.setQuery(QString("select * from t_m_member_type where ch_typeno = '%1' ").arg(str));

                jinfen = ui->lineEdit->text().toFloat() / vip.record(0).value(9).toFloat();
                if(vip.record(0).value(8).toString() == "0")
                {
                    fen = QString::number(int(jinfen));
                }
                else
                {
                    fen = QString().sprintf("%.02f",jinfen);
                }
            }

            if(query.exec(QString("insert into t_m_point(vch_memberno ,ch_type, ch_payno , num_point ,vch_memo , vch_operID , dt_operdate)values ('%1','%2','%3','%4',null,'%5','%6')")
                          .arg((muy.isrun?"Y":"")+youhuikahao).arg("3").arg(liushuihao).arg(fen).arg(n_func::gs_operid).arg(timer)))
            {
                if(muy.isrun){
                    memberPointList o;
                    o.mpVchMemberNo=youhuikahao;
                    o.mpChType="3";
                    o.mpChPayNo=liushuihao;
                    o.mpNumPoint= QString().sprintf("%.02f", fen.toFloat()).toFloat(); qDebug() << fen.toFloat();
                    //o.mpVchMemo;
                    o.mpVchOperId=n_func::gs_operid;

                    o.mpDtOperDate=timer;
                    muy.appitem(o);
                }
            }
            else
            {
                QSqlDatabase::database().rollback();
                lds_messagebox::warning(this, tr("消息提示"),tr("t_m_point失败!"));

                goto GOTO_FAST_PAY_PAY_FINISHED_END;
            }

            vip.clear();
            float aa= 0.000;
            if(w_m_member_list::is_yun_member){
                aa=w_sys_manage_cloudsync::get_yun_member_readcard(youhuikahao, conf.value("w_sys_manage_cloudsync_basedataset/cloudUser").toString(),
                                                                   "num_point", QVariant::Double).toDouble() + jinfen;if(!w_sys_manage_cloudsync::this_action_is_successed) goto GOTO_FAST_PAY_PAY_FINISHED_END;
            } else {
                vip.setQuery(QString("select * from t_m_curamount where vch_memberno = '' ").arg(youhuikahao));
                aa = vip.record(0).value(3).toFloat() + jinfen;
            }
            if(query.exec(QString("update t_m_curamount set vch_operID='%1' , dt_operdate='%2' ,num_point ='%3' where vch_memberno ='%4'")
                          .arg(n_func::gs_operid).arg(timer).arg(QString().sprintf("%.02f",aa)).arg((muy.isrun?"Y":"")+youhuikahao)))
            {
                //update ?更新没问题
                if(muy.isrun){
                    memberCuramount o;
                    o.vchOperId=n_func::gs_operid;
                    o.dtOperdate=timer;
                    o.numPoint=jinfen;//aa;云端是新增的积分,本地是新增+之前的
                    o.vchMemberno=youhuikahao;

                    muy.appitem(o);
                }
            }
            else
            {
                QSqlDatabase::database().rollback();
                lds_messagebox::warning(this, tr("消息提示"),tr("t_m_curamount失败!"));

                goto GOTO_FAST_PAY_PAY_FINISHED_END;
            }
        }

        for(int i = 0; i < table_model->rowCount(); i++)
        {
            QString pingzheng = table_model->item(i,4)->text();
            if(pingzheng.trimmed().isEmpty())
            {
                pingzheng = "null";
            }

            if(table_model->item(i,5)->text().trimmed() == "02")//会员卡付款
            {
                //积分
                QString jifen;
                QString ch_typeno;
                QString ch_calc;
                QString ch_calc_type;
                int int_calc_uint;
                QString shijijifen;
                float bencijinfen = 0.000;
                if(w_m_member_list::is_yun_member){
                    ch_typeno=w_sys_manage_cloudsync::get_yun_member_readcard(table_model->item(i,4)->text(), conf.value("w_sys_manage_cloudsync_basedataset/cloudUser").toString(),
                                                                              "ch_typeno", QVariant::String);if(!w_sys_manage_cloudsync::this_action_is_successed) goto GOTO_FAST_PAY_PAY_FINISHED_END;
                    ch_calc=w_sys_manage_cloudsync::get_yun_member_membertype(ch_typeno,
                                                                              "chCalc", QVariant::String);if(!w_sys_manage_cloudsync::this_action_is_successed) goto GOTO_FAST_PAY_PAY_FINISHED_END;
                    ch_calc_type=w_sys_manage_cloudsync::get_yun_member_membertype(ch_typeno,
                                                                                   "chCalcType", QVariant::String);if(!w_sys_manage_cloudsync::this_action_is_successed) goto GOTO_FAST_PAY_PAY_FINISHED_END;
                    int_calc_uint=w_sys_manage_cloudsync::get_yun_member_membertype(ch_typeno,
                                                                                    "intCalcUnit", QVariant::Int).toInt();if(!w_sys_manage_cloudsync::this_action_is_successed) goto GOTO_FAST_PAY_PAY_FINISHED_END;
                } else {
                    QSqlQueryModel vip;
                    vip.setQuery(QString("select ch_typeno from t_m_member where vch_memberno = '%1' ").arg(table_model->item(i,4)->text()));
                    ch_typeno = vip.record(0).value(0).toString();//得到类型编号
                    vip.clear();
                    vip.setQuery(QString("select * from t_m_member_type where ch_typeno = '%1' ").arg(ch_typeno));
                    ch_calc=vip.record(0).value(7).toString();
                    ch_calc_type=vip.record(0).value(8).toString();
                    int_calc_uint=vip.record(0).value(9).toInt();

                }
                if(ch_calc == "Y")//可以积分
                {
                    float yuanjiyifen = 0.000;
                    yuanjiyifen = int_calc_uint;
                    bencijinfen = table_model->item(i,3)->text().toFloat() / yuanjiyifen;
                    if(ch_calc_type  == "0")//积分取整数
                    {
                        QString fen = QString::number(int(bencijinfen));
                        shijijifen = QString::number(int(bencijinfen));
                        if(query.exec(QString("insert into t_m_point(vch_memberno ,ch_type, ch_payno , num_point , vch_memo , vch_operID , dt_operdate)values ('%1','%2','%3','%4',null,'%5','%6')")
                                      .arg((muy.isrun?"Y":"")+table_model->item(i,4)->text()).arg("3").arg(liushuihao).arg(fen).arg(n_func::gs_operid).arg(table_model->item(i,7)->text())))
                        {
                            if(muy.isrun){
                                memberPointList o;
                                o.mpVchMemberNo=table_model->item(i,4)->text();
                                o.mpChType="3";
                                o.mpChPayNo=liushuihao;
                                o.mpNumPoint=fen.toFloat();qDebug() << "o.mpNumPoint" << o.mpNumPoint;
                                //o.mpVchMemo;
                                o.mpVchOperId=n_func::gs_operid;

                                o.mpDtOperDate=table_model->item(i,7)->text();
                                muy.appitem(o);
                            }
                        }
                        else
                        {
                            QSqlDatabase::database().rollback();
                            lds_messagebox::warning(this, tr("消息提示"),tr("t_m_point失败!"));

                            goto GOTO_FAST_PAY_PAY_FINISHED_END;
                        }
                    }
                    else//小数
                    {
                        QString fen = QString().sprintf("%.02f",bencijinfen);
                        shijijifen = QString().sprintf("%.02f",bencijinfen);
                        if(query.exec(QString("insert into t_m_point(vch_memberno ,ch_type, ch_payno , num_point , vch_memo , vch_operID , dt_operdate)values ('%1','%2','%3','%4',null,'%5','%6')")
                                      .arg((muy.isrun?"Y":"")+table_model->item(i,4)->text()).arg("3").arg(liushuihao).arg(fen).arg(n_func::gs_operid).arg(table_model->item(i,7)->text())))
                        {
                            if(muy.isrun){
                                memberPointList o;
                                o.mpVchMemberNo=table_model->item(i,4)->text();
                                o.mpChType="3";
                                o.mpChPayNo=liushuihao;
                                o.mpNumPoint=fen.toFloat();qDebug() << "o.mpNumPoint" << o.mpNumPoint;
                                //o.mpVchMemo;
                                o.mpVchOperId=n_func::gs_operid;

                                o.mpDtOperDate=table_model->item(i,7)->text();
                                muy.appitem(o);
                            }
                        }
                        else
                        {
                            QSqlDatabase::database().rollback();
                            lds_messagebox::warning(this, tr("消息提示"),tr("t_m_point失败!"));

                            goto GOTO_FAST_PAY_PAY_FINISHED_END;
                        }
                    }
                }
                else//不积分
                {
                    bencijinfen = 0.000;
                    shijijifen = "0.000";
                }

                if(query.exec(QString("insert into t_m_pay(vch_memberno , ch_payno , ch_paymodeno ,  num_pay , vch_operID , dt_operdate , int_checkflow) values ('%1','%2','%3','%4','%5','%6','%7')")
                              .arg((muy.isrun?"Y":"")+table_model->item(i,4)->text()).arg(liushuihao).arg(table_model->item(i,5)->text()).
                              arg(table_model->item(i,3)->text()).arg(n_func::gs_operid).
                              arg(table_model->item(i,7)->text()).arg("0")))
                {
                    if(muy.isrun){
                        memberPay o;
                        o.mpVchMemberNo=table_model->item(i,4)->text();
                        o.mpChPayNo=liushuihao;
                        o.mpChPayModeNo=table_model->item(i,5)->text();
                        o.mpNumPay=table_model->item(i,3)->text().toFloat();
                        o.mpVchOperId=n_func::gs_operid;
                        o.mpDtOperDate=table_model->item(i,7)->text();
                        o.mpIntCheckFlow=0;
                        muy.appitem(o);
                    }
                }
                else
                {
                    QSqlDatabase::database().rollback();
                    lds_messagebox::warning(this, tr("消息提示"),tr("t_m_pay失败!"));

                    goto GOTO_FAST_PAY_PAY_FINISHED_END;
                }

                QString num_amount;
                QString num_point;
                if(w_m_member_list::is_yun_member){
                    num_amount=w_sys_manage_cloudsync::get_yun_member_readcard(table_model->item(i,4)->text(), conf.value("w_sys_manage_cloudsync_basedataset/cloudUser").toString(),
                                                                               "num_amount", QVariant::Double);if(!w_sys_manage_cloudsync::this_action_is_successed) goto GOTO_FAST_PAY_PAY_FINISHED_END;
                    num_point=w_sys_manage_cloudsync::get_yun_member_readcard(table_model->item(i,4)->text(), conf.value("w_sys_manage_cloudsync_basedataset/cloudUser").toString(),
                                                                              "num_point", QVariant::Double);if(!w_sys_manage_cloudsync::this_action_is_successed) goto GOTO_FAST_PAY_PAY_FINISHED_END;

                } else {
                    QSqlQueryModel vip_model;
                    vip_model.setQuery(QString("select * from t_m_curamount where vch_memberno = '%1' ").arg(table_model->item(i,4)->text()));
                    num_amount=vip_model.record(0).value(1).toString();
                    num_point=vip_model.record(0).value(3).toString();
                }

                if(num_amount.toFloat() < table_model->item(i,3)->text().toFloat())
                {
                    lds_messagebox::warning(this, tr("消息提示"),tr("会员卡余额已不够本次支付金额,请核实"));

                    goto GOTO_FAST_PAY_PAY_FINISHED_END;
                }
                else
                {
                    n_func::st_member_type lst_member = n_func::f_m_get_item(table_model->item(i,4)->text());
                    float ldec_payamt = table_model->item(i,3)->text().toFloat();
                    lst_member.ai_backcash_rate;
                    ldec_remain_amt = num_amount.toFloat();
                    ldec_remain_amt -= ldec_payamt;
                    ldec_backcash = QString::number(ldec_payamt * (lst_member.ai_backcash_rate * 0.01), 'f', 2).toFloat();
                    if(ldec_backcash > 0){
                        ldec_remain_amt += ldec_backcash;
                        ls_err = n_func::f_m_deposit(table_model->item(i, 4)->text(),ldec_backcash, 0, ldec_remain_amt, "3", "5", "");
                        if(!ls_err.isEmpty()){
                            lds_messagebox::warning(this,  tr("消息提示"), ls_err);
                            goto GOTO_FAST_PAY_PAY_FINISHED_END;
                        }
                    }

                    huyuankayue = QString().sprintf("%.02f",ldec_remain_amt);
                    float bb = 0.000;
                    bb = num_point.toFloat() + shijijifen.toFloat();

                    if(query.exec(QString("update t_m_curamount set num_amount = '%1' , vch_operID='%2' , dt_operdate='%3' , num_point ='%4' where vch_memberno ='%5'")
                                  .arg(QString().sprintf("%.02f",ldec_remain_amt)).arg(n_func::gs_operid).arg(table_model->item(i,7)->text()).arg(QString().sprintf("%.02f",bb))
                                  .arg((muy.isrun?"Y":"")+table_model->item(i,4)->text())))
                    {
                        if(muy.isrun){
                            memberCuramount o;
                            o.numAmount=ldec_remain_amt;
                            o.vchOperId=n_func::gs_operid;
                            o.dtOperdate=table_model->item(i,7)->text();
                            o.numPoint=shijijifen.toFloat();//bb;云端之插入增加的部分;本地还是按之前+本地插入本地
                            o.vchMemberno=table_model->item(i,4)->text();
                            muy.appitem(o);
                        }
                    }
                    else
                    {
                        QSqlDatabase::database().rollback();
                        lds_messagebox::warning(this, tr("消息提示"),tr("t_m_curamount失败!"));

                        goto GOTO_FAST_PAY_PAY_FINISHED_END;
                    }

                    if(query.exec(QString("update cey_u_checkout_master set vch_memberno='%1' where ch_payno = '%2' ")
                                  .arg(table_model->item(i, 4)->text())
                                  .arg(liushuihao))){
                        if(muy.checkOutMasterList.count() > 0){
                            muy.checkOutMasterList[0].comVchMemberNo=table_model->item(i, 4)->text();
                        }
                    } else {
                        QSqlDatabase::database().rollback();
                        lds_messagebox::warning(this, tr("消息提示"),tr("t_m_curamount失败!"));

                        goto GOTO_FAST_PAY_PAY_FINISHED_END;
                    }

                }
            }

            if(query.exec(QString("insert into cey_u_checkout_detail(ch_payno , ch_paymodeno , num_payamount , num_change , num_realamount , num_face ,vch_operID , dt_operdate ,num_card_remain,vch_voucherno, num_face_Num )values('%1','%2','%3','%4','%5','%6','%7','%8','%9','%10','%11')")
                          .arg(liushuihao).arg(table_model->item(i,5)->text()).arg(table_model->item(i,2)->text()).arg("0.0").arg(table_model->item(i,3)->text())
                          .arg("0.0").arg(n_func::gs_operid).arg(table_model->item(i,7)->text()).arg("0.0").arg(pingzheng).arg("0.0")))
            {
                if(muy.isrun){
                    checkOutDetail o;
                    o.codChPayNo=liushuihao;
                    o.codChPayModeNo=table_model->item(i,5)->text();
                    o.codNumPayAmount=table_model->item(i,2)->text().toFloat();
                    o.codNumChange=0;
                    o.codNumRealAmount=table_model->item(i,3)->text().toFloat();

                    o.codNumFace=0;
                    o.codVchOperId=n_func::gs_operid;
                    o.codDtOperDate=table_model->item(i,7)->text();
                    o.codNumCardRemain=0;
                    o.codVchVoucherNo=pingzheng;
                    o.codNumFaceNum=0;
                    muy.appitem(o);
                }
            }
            else
            {
                QSqlDatabase::database().rollback();
                lds_messagebox::warning(this, tr("消息提示"),tr("cey_u_checkout_detail失败!"));

                goto GOTO_FAST_PAY_PAY_FINISHED_END;
            }
        }

        //============================================================================================

        //菜品消费明细表
        QStandardItemModel & model = fast_order_2::modelBase;
        QSqlQueryModel guqing;
        for(int i = 0; i < model.rowCount(); i++)
        {
            guqing.clear();
            guqing.setQuery(QString("Select ifnull(num_num - num_sale , 0)from cey_bt_dish_warn where ch_dishno ='%1' and ch_state ='Y' and DATE_FORMAT(dt_date, '%Y-%m-%d') ='%2' ").arg(model.item(i,1)->text()).arg(guqingdate));
            if(guqing.rowCount() > 0 && model.item(i,9)->text() != tr("Y"))
            {
                query.prepare(QString("update cey_bt_dish_warn set num_sale = num_sale + :as_num where ch_state ='Y' and ch_dishno = :as_dishno and DATE_FORMAT(dt_date, '%Y-%m-%d') = :as_date "));
                query.bindValue(":as_num",model.item(i,3)->text());
                query.bindValue(":as_dishno",model.item(i,1)->text());
                query.bindValue(":as_date",guqingdate);
                if(query.exec())
                {
                    if(muy.isrun){

                        //                        muy.appitem(o);
                    }
                }
                else
                {
                    QSqlDatabase::database().rollback();
                    lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_bt_dish_warn菜品估清失败!"));

                    goto GOTO_FAST_PAY_PAY_FINISHED_END;
                }
            }

            float add_price;
            add_price = 0.000;
            add_price = model.item(i,3)->text().toFloat() * model.item(i,21)->text().toFloat();
            if(model.item(i,8)->text() == "Y")//赠送菜品
            {
                if(model.item(i,7)->text().isEmpty())
                {
                    if(query.exec(QString("insert into cey_u_orderdish(ch_billno,ch_tableno,int_id,ch_dishno,num_price,num_price_org, num_price_add,num_num,num_back,vch_back_operID,dt_back_operdate,int_discount,vch_dis_operID, dt_dis_operdate,vch_memberno,ch_consult,ch_suitflag,ch_suitno,ch_specialflag,ch_presentflag,vch_pre_operID,dt_pre_operdate,vch_operID,dt_operdate,ch_togono,ch_payno,vch_print_memo) values ('%1','%2','%3','%4','%5','%6','%7','%8','%9',null,null,'%10',null,null,null,'%11','%12', null,'%13','%14','%15','%16','%17','%18',null,'%19',null)")
                                  .arg(danhao).arg(zhuohao).arg(QString::number(i)).arg(model.item(i,1)->text()).arg(model.item(i,4)->text()).
                                  arg(model.item(i,4)->text())
                                  .arg(QString().sprintf("%.02f",add_price)).arg(model.item(i,3)->text()).arg("0.000").arg(model.item(i,5)->text()).
                                  arg("0").arg(model.item(i,9)->text())
                                  .arg((muy.isrun?"Y":"")+model.item(i,10)->text()).arg(model.item(i,8)->text()).arg(n_func::gs_operid).arg(model.item(i,20)->text()).
                                  arg(n_func::gs_operid)
                                  .arg(time).arg(liushuihao)))
                    {
                        if(muy.isrun){
                            orderDish o;
                            o.bdChBillNo=danhao;
                            o.bdChTableNo=zhuohao;
                            o.bdIntId=i*1.0;
                            o.bdChDishNo=model.item(i,1)->text();
                            o.bdNumPrice=model.item(i,4)->text().toFloat();

                            o.bdNumPriceOrg=model.item(i,4)->text().toFloat();
                            o.bdNumPriceAdd=add_price;
                            o.bdNumNum=model.item(i, 3)->text().toFloat();
                            o.bdNumBack=0;
                            //                    o.bdVchOperId=model.item(i,9)->text();

                            //                    o.bdDtBackOperDate=model.item(i,10)->text();
                            o.bdIntDiscount=model.item(i,9)->text().toInt();
                            //                    o.bdVchDisOperId;
                            //                    o.bdDtDisOperDate;
                            o.btVchMemberNo=model.item(i,10)->text();

                            o.bdChConsult=model.item(i,8)->text();
                            //                    o.bdch_suitflag;
                            //                    o.ch_suitno;
                            o.bdChSpecialFlag;
                            //                    o.bdchpresent;

                            o.bdVchPreOperId=n_func::gs_operid;
                            o.bdDtPreOperDate=model.item(i,20)->text();
                            o.bdVchOperId=n_func::gs_operid;
                            o.bdDtOperDate=time;
                            //                    o.bdChTogoNo=

                            o.bdChPayNo=liushuihao;
                            //                    o.bdVchPrintMemo;
                            muy.appitem(o);
                        }
                    }
                    else
                    {
                        QSqlDatabase::database().rollback();
                        qDebug() << query.lastError();  lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_u_orderdish菜品消费明细表失败!"));
                        goto GOTO_FAST_PAY_PAY_FINISHED_END;
                    }
                }
                else
                {
                    query.prepare(QString("insert into cey_u_orderdish(ch_billno,ch_tableno,int_id,ch_dishno,num_price,num_price_org,num_price_add,num_num,num_back,vch_back_operID,dt_back_operdate,int_discount,vch_dis_operID,dt_dis_operdate,vch_memberno,ch_consult,ch_suitflag,ch_suitno,ch_specialflag,ch_presentflag,vch_pre_operID,dt_pre_operdate,vch_operID,dt_operdate,ch_togono,ch_payno,vch_print_memo) values(:str,:str1,:str2,:str3,:str4,:str5,:str6,:str7,:str8,null,null,:str9,null,null,null,:str10,:str11,null,:str12,:str13,:str14,:str15,:str16,:str17,null,:str18,:str19)"));
                    query.bindValue(":str",danhao);
                    query.bindValue(":str1",zhuohao);
                    query.bindValue(":str2",QString::number(i));
                    query.bindValue(":str3",model.item(i,1)->text());
                    query.bindValue(":str4",model.item(i,4)->text());
                    query.bindValue(":str5",model.item(i,4)->text());
                    query.bindValue(":str6",QString().sprintf("%.02f",add_price));
                    query.bindValue(":str7",model.item(i,3)->text());
                    query.bindValue(":str8","0.000");
                    query.bindValue(":str9",model.item(i,5)->text());
                    query.bindValue(":str10","0");
                    query.bindValue(":str11",model.item(i,9)->text());
                    query.bindValue(":str12",model.item(i,10)->text());
                    query.bindValue(":str13",model.item(i,8)->text());
                    query.bindValue(":str14",n_func::gs_operid);
                    query.bindValue(":str15",model.item(i,20)->text());
                    query.bindValue(":str16",n_func::gs_operid);
                    query.bindValue(":str17",time);
                    query.bindValue(":str18",liushuihao);
                    query.bindValue(":str19",model.item(i,7)->text());

                    if(query.exec())
                    {
                        if(muy.isrun){
                            orderDish o; qDebug() << "---------------------------" << o.bdIntFlowId;
                            o.bdChBillNo=danhao;
                            o.bdChTableNo=zhuohao;
                            o.bdIntId=i*1.0;
                            o.bdChDishNo=model.item(i,1)->text();
                            o.bdNumPrice=model.item(i,4)->text().toFloat();

                            o.bdNumPriceOrg=model.item(i,4)->text().toFloat();
                            o.bdNumPriceAdd=add_price;
                            o.bdNumNum=model.item(i, 3)->text().toFloat();
                            o.bdNumBack=0;
                            //                    o.bdVchOperId=model.item(i,9)->text();

                            //                    o.bdDtBackOperDate=model.item(i,10)->text();
                            o.bdIntDiscount=model.item(i,5)->text().toInt();
                            //                    o.bdVchDisOperId;
                            //                    o.bdDtDisOperDate;
                            //                                        o.btVchMemberNo=model.item(i,10)->text();

                            o.bdChConsult="0";
                            //                    o.bdch_suitflag;
                            //                    o.ch_suitno;
                            //                                        o.bdChSpecialFlag;
                            //                    o.bdchpresent;

                            o.bdVchPreOperId=n_func::gs_operid;
                            o.bdDtPreOperDate=model.item(i,20)->text();
                            o.bdVchOperId=n_func::gs_operid;
                            o.bdDtOperDate=time;
                            //                    o.bdChTogoNo=

                            o.bdChPayNo=liushuihao;
                            o.bdVchPrintMemo=model.item(i,7)->text();
                            muy.appitem(o);
                        }
                    }
                    else
                    {
                        QSqlDatabase::database().rollback();
                        qDebug() << query.lastError();  lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_u_orderdish失败菜品消费明细表!"));

                        goto GOTO_FAST_PAY_PAY_FINISHED_END;
                    }
                }
            }
            else if(model.item(i,5)->text().toFloat() != 100)//折扣菜品
            {
                if(model.item(i,7)->text().isEmpty())
                {
                    if(query.exec(QString("insert into cey_u_orderdish(ch_billno,ch_tableno,int_id,ch_dishno,num_price,num_price_org,num_price_add,num_num,num_back,vch_back_operID,dt_back_operdate,int_discount,vch_dis_operID,dt_dis_operdate,vch_memberno,ch_consult,ch_suitflag,ch_suitno,ch_specialflag,ch_presentflag,vch_pre_operID,dt_pre_operdate,vch_operID,dt_operdate,ch_togono,ch_payno,vch_print_memo) values('%1','%2','%3','%4','%5','%6','%7','%8','%9','%10','%11','%12','%13','%14',null,'%15','%16',null,'%17','%18',null,null,'%19','%20',null,'%21',null)")
                                  .arg(danhao).arg(zhuohao).arg(QString::number(i)).arg(model.item(i,1)->text()).arg(model.item(i,4)->text()).arg(model.item(i,4)->text())
                                  .arg(QString().sprintf("%.02f",add_price)).arg(model.item(i,3)->text()).arg("0.000").arg(n_func::gs_operid).arg(model.item(i,19)->text()).arg(QString::number(int(model.item(i,5)->text().toFloat()))).arg(n_func::gs_operid)
                                  .arg(model.item(i,19)->text()).arg("1").arg("N").arg("N").arg("N").arg(n_func::gs_operid).arg(time).arg(liushuihao)))
                    {
                        qDebug() << "insert into cey_u_orderdish" << query.lastQuery();
                        if(muy.isrun){
                            orderDish o;
                            o.bdChBillNo=danhao;
                            o.bdChTableNo=zhuohao;
                            o.bdIntId=i*1.0;
                            o.bdChDishNo=model.item(i,1)->text();
                            o.bdNumPrice=model.item(i,4)->text().toFloat();

                            o.bdNumPriceOrg=model.item(i,4)->text().toFloat();
                            o.bdNumPriceAdd=add_price;
                            o.bdNumNum=model.item(i, 3)->text().toFloat();
                            o.bdNumBack=0;
                            o.bdVchOperId=n_func::gs_operid;

                            o.bdDtBackOperDate=model.item(i,19)->text();
                            o.bdIntDiscount=model.item(i,5)->text().toFloat();
                            o.bdVchDisOperId=n_func::gs_operid;
                            o.bdDtDisOperDate=model.item(i,19)->text();
                            //                    o.btVchMemberNo=model.item(i,10)->text();

                            o.bdChConsult="1";
                            //                    o.bdch_suitflag;
                            //                    o.ch_suitno;
                            o.bdChSpecialFlag="N";
                            //                    o.bdchpresent;

                            //                    o.bdVchPreOperId=n_func::gs_operid;
                            //                    o.bdDtPreOperDate=model.item(i,20)->text();
                            o.bdVchOperId=n_func::gs_operid;
                            o.bdDtOperDate=time;
                            //                    o.bdChTogoNo=

                            o.bdChPayNo=liushuihao;
                            //                    o.bdVchPrintMemo;
                            muy.appitem(o);
                        }
                    }
                    else
                    {
                        QSqlDatabase::database().rollback();
                        qDebug() << query.lastError();  lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_u_orderdish失败菜品消费明细表!"));

                        goto GOTO_FAST_PAY_PAY_FINISHED_END;
                    }
                }
                else
                {
                    query.prepare(QString("insert into cey_u_orderdish(ch_billno,ch_tableno,int_id,ch_dishno,num_price,num_price_org,num_price_add,num_num,num_back,vch_back_operID,dt_back_operdate,int_discount,vch_dis_operID,dt_dis_operdate,vch_memberno,ch_consult,ch_suitflag,ch_suitno,ch_specialflag,ch_presentflag,vch_pre_operID,dt_pre_operdate,vch_operID,dt_operdate,ch_togono,ch_payno,vch_print_memo) values(:str,:str1,:str2,:str3,:str4,:str5,:str6,:str7,:str8,null,null,:str9,null,null,null,:str10,:str11,null,:str12,:str13,:str14,:str15,:str16,:str17,null,:str18,:str19)"));
                    query.bindValue(":str",danhao);
                    query.bindValue(":str1",zhuohao);
                    query.bindValue(":str2",QString::number(i));
                    query.bindValue(":str3",model.item(i,1)->text());
                    query.bindValue(":str4",model.item(i,4)->text());
                    query.bindValue(":str5",model.item(i,4)->text());
                    query.bindValue(":str6",QString().sprintf("%.02f",add_price));
                    query.bindValue(":str7",model.item(i,3)->text());
                    query.bindValue(":str8","0.000");
                    query.bindValue(":zhekouren",n_func::gs_operid);
                    query.bindValue(":zhekoutime",model.item(i,19)->text());
                    query.bindValue(":str9",QString::number(int(model.item(i,5)->text().toFloat())));
                    query.bindValue(":str10",n_func::gs_operid);
                    query.bindValue(":str11",model.item(i,19)->text());
                    query.bindValue(":str12","1");
                    query.bindValue(":str13","N");
                    query.bindValue(":str14","N");
                    query.bindValue(":str15","N");
                    query.bindValue(":str16",n_func::gs_operid);
                    query.bindValue(":str17",time);
                    query.bindValue(":str18",liushuihao);
                    query.bindValue(":str19",model.item(i,7)->text());

                    if(query.exec())
                    {
                        if(muy.isrun){
                            orderDish o;
                            o.bdChBillNo=danhao;
                            o.bdChTableNo=zhuohao;
                            o.bdIntId=i*1.0;  qDebug() << i;
                            o.bdChDishNo=model.item(i,1)->text();
                            o.bdNumPrice=model.item(i,4)->text().toFloat();

                            o.bdNumPriceOrg=model.item(i,4)->text().toFloat();
                            o.bdNumPriceAdd=add_price;
                            o.bdNumNum=model.item(i, 3)->text().toFloat();
                            o.bdNumBack=0;
                            o.bdVchOperId=n_func::gs_operid;

                            o.bdDtBackOperDate=model.item(i,19)->text();
                            o.bdIntDiscount=model.item(i,5)->text().toFloat();
                            o.bdVchDisOperId=n_func::gs_operid;
                            o.bdDtDisOperDate=model.item(i,19)->text();
                            //                    o.btVchMemberNo=model.item(i,10)->text();

                            o.bdChConsult="1";
                            //                    o.bdch_suitflag;
                            //                    o.ch_suitno;
                            o.bdChSpecialFlag="N";
                            //                    o.bdchpresent;

                            //                    o.bdVchPreOperId=n_func::gs_operid;
                            //                    o.bdDtPreOperDate=model.item(i,20)->text();
                            o.bdVchOperId=n_func::gs_operid;
                            o.bdDtOperDate=time;
                            //                    o.bdChTogoNo=

                            o.bdChPayNo=liushuihao;
                            o.bdVchPrintMemo=model.item(i,7)->text();
                            muy.appitem(o);
                        }
                    }
                    else
                    {
                        QSqlDatabase::database().rollback();
                        qDebug() << query.lastError();  lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_u_orderdish菜品消费明细表折扣菜品失败!"));

                        goto GOTO_FAST_PAY_PAY_FINISHED_END;
                    }
                }
            }
            else//普通菜品
            {
                if(model.item(i,7)->text().isEmpty())//无做法
                {
                    if(query.exec(QString("insert into cey_u_orderdish(ch_billno,ch_tableno,int_id,ch_dishno,num_price,num_price_org,num_price_add,num_num,num_back,vch_back_operID,dt_back_operdate,int_discount,vch_dis_operID,dt_dis_operdate,vch_memberno,ch_consult,ch_suitflag,ch_suitno,ch_specialflag,ch_presentflag,vch_pre_operID,dt_pre_operdate,vch_operID,dt_operdate,ch_togono,ch_payno,vch_print_memo) values('%1','%2','%3','%4','%5','%6','%7','%8','%9',null,null,'%10',null,null,null,'%11','%12',null,'%13','%14',null,null,'%15','%16',null,'%17',null)")
                                  .arg(danhao).arg(zhuohao).arg(QString::number(i)).arg(model.item(i,1)->text()).arg(model.item(i,4)->text()).arg(model.item(i,4)->text())
                                  .arg(QString().sprintf("%.02f",add_price)).arg(model.item(i,3)->text()).arg("0.000").arg(model.item(i,5)->text()).arg("0").arg(model.item(i,9)->text())
                                  .arg(model.item(i,10)->text()).arg(model.item(i,8)->text()).arg(n_func::gs_operid)
                                  .arg(time).arg(liushuihao)))
                    {
                        qDebug() << query.lastQuery();
                        if(muy.isrun){
                            orderDish o;
                            o.bdChBillNo=danhao;
                            o.bdChTableNo=zhuohao;
                            o.bdIntId=i*1.0;
                            o.bdChDishNo=model.item(i,1)->text();
                            o.bdNumPrice=model.item(i,4)->text().toFloat();

                            o.bdNumPriceOrg=model.item(i,4)->text().toFloat();
                            o.bdNumPriceAdd=add_price;
                            o.bdNumNum=model.item(i, 3)->text().toFloat();
                            o.bdNumBack=0;
                            //                        o.bdVchOperId=n_func::gs_operid;

                            //                        o.bdDtBackOperDate=model.item(i,19)->text();
                            o.bdIntDiscount=model.item(i,5)->text().toFloat();
                            //                        o.bdVchDisOperId=n_func::gs_operid;
                            //                        o.bdDtDisOperDate=model.item(i,19)->text();
                            //                    o.btVchMemberNo=model.item(i,10)->text();

                            o.bdChConsult="0";
                            //                    o.bdch_suitflag;
                            //                    o.ch_suitno;
                            o.bdChSpecialFlag=model.item(i,10)->text();
                            //                    o.bdchpresent;

                            //                    o.bdVchPreOperId=n_func::gs_operid;
                            //                    o.bdDtPreOperDate=model.item(i,20)->text();
                            o.bdVchOperId=n_func::gs_operid;
                            o.bdDtOperDate=time;
                            //                    o.bdChTogoNo=

                            o.bdChPayNo=liushuihao;
                            //                    o.bdVchPrintMemo;
                            muy.appitem(o);
                        }
                    }
                    else
                    {
                        QSqlDatabase::database().rollback();
                        qDebug() << query.lastError();  lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_u_orderdish菜品消费明细表失败!"));

                        goto GOTO_FAST_PAY_PAY_FINISHED_END;
                    }

                    qDebug() << query.lastQuery();
                }
                else
                {
                    qDebug() << "insert into cey_u_orderdish" << danhao;
                    query.prepare(QString("insert into cey_u_orderdish(ch_billno,ch_tableno,int_id,ch_dishno,num_price,num_price_org,num_price_add,num_num,num_back,vch_back_operID,dt_back_operdate,int_discount,vch_dis_operID,dt_dis_operdate,vch_memberno,ch_consult,ch_suitflag,ch_suitno,ch_specialflag,ch_presentflag,vch_pre_operID,dt_pre_operdate,vch_operID,dt_operdate,ch_togono,ch_payno,vch_print_memo) values(:str,:str1,:str2,:str3,:str4,:str5,:str6,:str7,:str8,null,null,:str9,null,null,null,:str10,:str11,null,:str12,:str13,null,null,:str14,:str15,null,:str16,:str17)"));
                    query.bindValue(":str",danhao);
                    query.bindValue(":str1",zhuohao);
                    query.bindValue(":str2",QString::number(i));
                    query.bindValue(":str3",model.item(i,1)->text());
                    query.bindValue(":str4",model.item(i,4)->text());
                    query.bindValue(":str5",model.item(i,4)->text());
                    query.bindValue(":str6",QString().sprintf("%.02f",add_price));
                    query.bindValue(":str7",model.item(i,3)->text());
                    query.bindValue(":str8","0.000");
                    query.bindValue(":str9",model.item(i,5)->text());
                    query.bindValue(":str10","0");
                    query.bindValue(":str11",model.item(i,9)->text());
                    query.bindValue(":str12",model.item(i,10)->text());
                    query.bindValue(":str13",model.item(i,8)->text());
                    query.bindValue(":str14",n_func::gs_operid);
                    query.bindValue(":str15",time);
                    query.bindValue(":str16",liushuihao);
                    query.bindValue(":str17",model.item(i,7)->text());

                    if(query.exec())
                    {
                        if(muy.isrun){
                            orderDish o;
                            o.bdChBillNo=danhao;
                            o.bdChTableNo=zhuohao;
                            o.bdIntId=i*1.0;
                            o.bdChDishNo=model.item(i,1)->text();
                            o.bdNumPrice=model.item(i,4)->text().toFloat();

                            o.bdNumPriceOrg=model.item(i,4)->text().toFloat();
                            o.bdNumPriceAdd=add_price;
                            o.bdNumNum=model.item(i, 3)->text().toFloat();
                            o.bdNumBack=0;
                            //                    o.bdVchOperId=model.item(i,9)->text();

                            //                    o.bdDtBackOperDate=model.item(i,10)->text();
                            o.bdIntDiscount=model.item(i,5)->text().toInt();
                            //                    o.bdVchDisOperId;
                            //                    o.bdDtDisOperDate;
                            //                                        o.btVchMemberNo=model.item(i,10)->text();

                            o.bdChConsult="0";
                            //                    o.bdch_suitflag;
                            //                    o.ch_suitno;
                            o.bdChSpecialFlag=model.item(i,10)->text();
                            //                    o.bdchpresent;

                            o.bdVchPreOperId=n_func::gs_operid;
                            o.bdDtPreOperDate=model.item(i,20)->text();
                            o.bdVchOperId=n_func::gs_operid;
                            o.bdDtOperDate=time;
                            //                    o.bdChTogoNo=

                            o.bdChPayNo=liushuihao;
                            o.bdVchPrintMemo=model.item(i,7)->text();
                            muy.appitem(o);
                        }
                    }
                    else
                    {
                        QSqlDatabase::database().rollback();
                        qDebug() << query.lastError();  lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_u_orderdish菜品消费明细表普通菜品失败!"));

                        goto GOTO_FAST_PAY_PAY_FINISHED_END;
                    }
                }
            }
        }
        if(muy.isrun){
            //若是数据库插入没问题，则
            //若是最大单号没有冲突
            if("ok" != w_sys_manage_cloudsync::upload_yun_pay_info(muy)){
                QSqlDatabase::database().rollback();//云会员失败则回滚
                goto GOTO_FAST_PAY_PAY_FINISHED_END;
            }
            //            lds_messagebox::warning(this, tr("消息提示"),tr("云会员付款成功!"));
        }
        QSqlDatabase::database().commit();
        pay_finished=true;
        goto GOTO_FAST_PAY_PAY_FINISHED_END;
    }//! while

GOTO_FAST_PAY_PAY_FINISHED_END:
    w_m_member_list::yun_member_close();
    if(pay_finished) {
        if(conf.value("system_setting/njgj_use", false).toBool()){///采集数据
            //现金付款
            QList<w_scr_dish::TECH_TRANS_NJGJ_tendercode> tendercodes;
            if(ui->label_14->text().toFloat() != 0){
                tendercodes.append(w_scr_dish::TECH_TRANS_NJGJ_tendercode("CH", ui->label_14->text()));
            }
            for(int row=0,rowcount=table_model->rowCount();row<rowcount;row++){
                if(table_model->item(row, 5)->text()=="03"){
                    tendercodes.append(w_scr_dish::TECH_TRANS_NJGJ_tendercode("CI",table_model->item(row, 3)->text()));
                }
            }
            w_scr_dish::TECH_TRANS_NJGJ_sql(
                        QDate::currentDate().toString("yyyyMMdd"),
                        QTime::currentTime().toString("hhmmss"),
                        "01",
                        bendandanhao,
                        n_func::gs_operid,
                        n_func::gs_opername,
                        QString::number(_parent->get_cur_num()),
                        ui->lineEdit_4->text(),
                        tendercodes
                        );

        }///采集数据
        if(isprint)
            fast_pay_print(bendandanhao);
        btn_t_f  = true;
    }
}

void fast_pay::lineEdit_9_textChanged(const QString &arg1)
{
    //找零
    ui->lineEdit_7->setText(QString().sprintf("%.02f",
                                              otherpay+ui->lineEdit_9->text().toFloat()-ui->lineEdit_autualpay->text().toFloat()));
}

void fast_pay::print_lost(const QString & str)
{
    lds_messagebox::warning(this, tr("消息提示"),str);
}

void fast_pay::fast_pay_print(const QString &_dayindanhao,const QString &title0, int printchoose)
{
    QString title=title0+"\n";
    QSettings configIniRead(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    QSqlQueryModel model;

    //单号
    model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_payno' "));
    QString danhao = model.record(0).value(0).toString();
    model.clear();
    model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_payno_len' "));
    QString danlen = model.record(0).value(0).toString();
    model.clear();
    model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_payno_row' "));
    int danrow = model.record(0).value(0).toInt();

    //工号
    model.clear();
    model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_operid' "));
    QString gonghao = model.record(0).value(0).toString();
    model.clear();
    model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_operid_len' "));
    QString gonglen = model.record(0).value(0).toString();
    model.clear();
    model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_operid_row' "));
    int gongrow = model.record(0).value(0).toInt();

    //餐桌
    model.clear();
    model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_table' "));
    QString table = model.record(0).value(0).toString();
    model.clear();
    model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_table_len' "));
    QString tablelen = model.record(0).value(0).toString();
    model.clear();
    model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_table_row' "));
    int tablerow = model.record(0).value(0).toInt();

    //人数
    model.clear();
    model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_person' "));
    QString renshu = model.record(0).value(0).toString();
    model.clear();
    model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_person_len' "));
    QString renshulen = model.record(0).value(0).toString();
    model.clear();
    model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_person_row' "));
    int renshurow = model.record(0).value(0).toInt();

    //时间
    model.clear();
    model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_time' "));
    QString timer = model.record(0).value(0).toString();
    model.clear();
    model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_time_len' "));
    QString timerlen = model.record(0).value(0).toString();
    model.clear();
    model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_time_row' "));
    int timerrow = model.record(0).value(0).toInt();

    //流水号
    model.clear();
    model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='serial_no' "));
    QString serial = model.record(0).value(0).toString();
    model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='serial_no_len' "));
    QString seriallen = model.record(0).value(0).toString();


    {
        blockIns b_pre;
        QString content;//标题内容

        //第一行=====================================================================================
        //单号
        if(danhao == "1" && !wdanhao.isEmpty())
        {
            setprintsize(b_pre, danlen, tr("付款号:") + wdanhao+"\n");
        }

        //工号
        if(gonghao == "1")
        {
            setprintsize(b_pre, gonglen, tr("工号:") + n_func::gs_operid+"\n");
        }

        //餐桌
        if(table == "1")
        {
            setprintsize(b_pre, tablelen,  tr("餐桌:")+frontViewDialog_virtual::cur_desk_no+"\n");
        }

        //人数
        if(renshu == "1")
        {
            setprintsize(b_pre, renshulen, tr("人数:1")+"\n");
        }

        //时间
        if(timer == "1")
        {
            setprintsize(b_pre, timerlen, tr("时间:") +pay_timer+"\n");
        }
        //流水号
        if(serial == "1")
        {
            QString liushuihaoma =(liushuihao_p_is_shoudog.isEmpty()?liushuihao_p:liushuihao_p_is_shoudog);
            if(!liushuihaoma.isEmpty())
                setprintsize(b_pre, seriallen, tr("流水号:")+liushuihaoma+"\n");
        }
        //打印消费内容

        QString shifoumingxi;
        shifoumingxi = configIniRead.value("system_setting/maidanmingxi").toString();
        if(shifoumingxi == "1")
        {
            //表头
            QString tou = configIniRead.value("system_setting/biaoneirong").toString();
            //tou = tr("项目               数量           金额");
            content.append(n_func::gs_print_line+"\n");
            content.append(tou+"\n");
            content.append(n_func::gs_print_line+"\n");
        }
        else
        {
            content.append(n_func::gs_print_line+"\n");
        }

        model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_series_group' "));
        QString fenzu = model.record(0).value(0).toString();
        //本单序号
        model.clear();
        model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_rowid' "));
        QString bxuhao = model.record(0).value(0).toString();

        //赠送标识
        model.clear();
        model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_presentflag' "));
        QString bzensong = model.record(0).value(0).toString();

        //特价标识
        model.clear();
        model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_specialflag' "));
        QString btejia = model.record(0).value(0).toString();

        //超出字符串长度自动截断
        model.clear();
        model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_overstep_cut' "));
        QString bjieduan = model.record(0).value(0).toString();

        //品名
        model.clear();
        model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_dishname' "));
        QString pinming = model.record(0).value(0).toString();

        model.clear();
        model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_dishname_len' "));
        int pinminglen = model.record(0).value(0).toInt();

        model.clear();
        model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_dishname_row' "));
        int pinmingrow = model.record(0).value(0).toInt();

        //单位
        model.clear();
        model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_unit' "));
        QString danwei = model.record(0).value(0).toString();

        model.clear();
        model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_unit_len' "));
        int danweilen = model.record(0).value(0).toInt();

        model.clear();
        model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_unit_row' "));
        int danweirow = model.record(0).value(0).toInt();

        //数量
        model.clear();
        model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_num' "));
        QString shuliang = model.record(0).value(0).toString();

        model.clear();
        model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_num_len' "));
        int shulianglen = model.record(0).value(0).toInt();

        model.clear();
        model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_num_row' "));
        int shuliangrow = model.record(0).value(0).toInt();

        model.clear();
        model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_num_dec' "));
        int shuliangxiaoshu = model.record(0).value(0).toInt();

        //价格
        model.clear();
        model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_price' "));
        QString jiage = model.record(0).value(0).toString();

        model.clear();
        model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_price_len' "));
        int jiagelen = model.record(0).value(0).toInt();

        model.clear();
        model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_price_row' "));
        int jiagerow = model.record(0).value(0).toInt();

        model.clear();
        model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_price_dec' "));
        int jiagexiaoshu = model.record(0).value(0).toInt();

        //金额
        model.clear();
        model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_amount' "));
        QString jine = model.record(0).value(0).toString();

        model.clear();
        model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_amount_len' "));
        int jinelen = model.record(0).value(0).toInt();

        model.clear();
        model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_amount_row' "));
        int jinerow = model.record(0).value(0).toInt();

        model.clear();
        model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_amount_dec' "));
        int jinexiaoshu = model.record(0).value(0).toInt();

        //折扣
        model.clear();
        model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_disdetail' "));
        QString zhekou = model.record(0).value(0).toString();

        model.clear();
        model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_disdetail_len' "));
        int zhekoulen = model.record(0).value(0).toInt();

        model.clear();
        model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_disdetail_row' "));
        int zhekourow = model.record(0).value(0).toInt();

        //做法
        model.clear();
        model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_printmemo' "));
        QString zuofa = model.record(0).value(0).toString();

        if(shifoumingxi == "1")
        {
            if(fenzu == "1")//菜品按大类分组
            {
                QSqlQueryModel dalei;
                dalei.setQuery(QString("select ch_dish_typeno from cey_bt_dish_type"));
                for(int k = 0; k < dalei.rowCount(); k++)
                {
                    QString neirong;
                    float zongjia = 0.000;
                    for(int i = 0; i < fast_order_2::modelBase.rowCount(); i++)
                    {
                        zongjia += fast_order_2::modelBase.item(i,3)->text().toFloat() * fast_order_2::modelBase.item(i,4)->text().toFloat();
                        QString hanghao = QString::number(i+1) + tr(".");
                        if(hanghao.length() < 3)
                        {
                            setprintsize(hanghao, 3,Qt::AlignLeft);
                        }
                        if(bxuhao=="1")neirong.append(hanghao);
                        if(bzensong == "1")
                        {
                            if(fast_order_2::modelBase.item(i,8)->text() == tr("Y"))
                            {
                                neirong.append(tr("赠)"));
                            }
                        }

                        if(btejia == "1")
                        {
                            if(fast_order_2::modelBase.item(i,10)->text() == tr("Y"))
                            {
                                neirong.append(tr("特)"));
                            }
                        }

                        if(fast_order_2::modelBase.item(i,9)->text() == "P")
                        {
                            neirong.append(tr("套)"));
                        }

                        if(fast_order_2::modelBase.item(i,9)->text() == tr("Y"))
                        {
                            neirong.append(tr("细)"));
                        }
                        // content.append(neirong);
                        //第一行=========================================================================
                        QString neirong1;
                        //品名
                        if(pinming == "1")
                        {
                            QString pin = fast_order_2::modelBase.item(i,2)->text();
                            if(bjieduan == "1")
                            {
                                if(pin.length() > 5)
                                {
                                    pin = pin.left(5);
                                }
                            }

                            if(pin.length() >= pinminglen)
                            {
                                pin = pin.left(pinminglen);
                            }
                            else
                            {
                                setprintsize(pin, pinminglen,"pin");
                            }

                            if(pinmingrow == 1)
                            {
                                neirong1.append(pin);
                            }
                        }

                        //单位
                        if(danwei == "1")
                        {
                            QString dan = fast_order_2::modelBase.item(i,12)->text();
                            w_scr_dish::setprintsize(dan, danweilen,"dan");

                            if(danweirow == 1)
                            {
                                neirong1.append(dan);
                            }
                        }

                        //数量
                        if(shuliang == "1")
                        {
                            QString shu = fast_order_2::modelBase.item(i,3)->text();
                            if(shuliangxiaoshu == 0)
                            {
                                shu = QString::number(int(shu.toInt()));
                            }
                            else if(shuliangxiaoshu == 1)
                            {
                                shu = QString().sprintf("%.03f",shu.toFloat());
                            }
                            else
                            {
                                shu = QString().sprintf("%.03f",shu.toFloat());
                            }

                            if(shu.length() >= shulianglen)
                            {
                                shu = shu.left(shulianglen);
                            }
                            else
                            {
                                w_scr_dish::setprintsize(shu, shulianglen,"shu");
                            }

                            if(shuliangrow == 1)
                            {
                                neirong1.append(shu);
                            }
                        }

                        //价格
                        if(jiage == "1")
                        {
                            QString jia = fast_order_2::modelBase.item(i,4)->text();
                            if(jiagexiaoshu == 0)
                            {
                                if(fast_order_2::modelBase.item(i,9)->text() == "Y")
                                {
                                    jia = "0";
                                }
                                else
                                {
                                    jia = QString::number(int(jia.toInt()));
                                }
                            }
                            else if(jiagexiaoshu == 1)
                            {
                                if(fast_order_2::modelBase.item(i,9)->text() == "Y")
                                {
                                    jia = tr("0.0");
                                }
                                else
                                {
                                    jia = QString().sprintf("%.01f",jia.toFloat());
                                }
                            }
                            else
                            {
                                if(fast_order_2::modelBase.item(i,9)->text() == "Y")
                                {
                                    jia = "0.000";
                                }
                                else
                                {
                                    jia = QString().sprintf("%.02f",jia.toFloat());
                                }
                            }

                            if(jia.length() >= jiagelen)
                            {
                                jia = jia.left(jiagelen);
                            }
                            else
                            {
                                w_scr_dish::setprintsize(jia, jiagelen,"jia");
                            }

                            if(jiagerow == 1)
                            {
                                neirong1.append(jia);
                            }
                        }

                        //金额
                        if(jine == "1")
                        {
                            float aa;
                            aa = 0.000;
                            aa = fast_order_2::modelBase.item(i,3)->text().toFloat() * fast_order_2::modelBase.item(i,4)->text().toFloat();
                            QString jin = QString().sprintf("%.02f",aa);

                            if(jinexiaoshu == 0)
                            {
                                if(fast_order_2::modelBase.item(i,9)->text() == "Y")
                                {
                                    jin = "0";
                                }
                                else
                                {
                                    jin = QString::number(int(jin.toInt()));
                                }
                            }
                            else if(jinexiaoshu == 1)
                            {
                                if(fast_order_2::modelBase.item(i,9)->text() == "Y")
                                {
                                    jin = tr("0.0");
                                }
                                else
                                {
                                    jin = QString().sprintf("%.01f",jin.toFloat());
                                }
                            }
                            else
                            {
                                if(fast_order_2::modelBase.item(i,9)->text() == "Y")
                                {
                                    jin = "0.000";
                                }
                                else
                                {
                                    jin = QString().sprintf("%.02f",jin.toFloat());
                                }
                            }


                            if(jin.length() >= jinelen)
                            {
                                jin = jin.left(jinelen);
                            }
                            else
                            {
                                w_scr_dish::setprintsize(jin, jinelen,"jin");
                            }

                            if(jinerow == 1)
                            {
                                neirong1.append(jin);
                            }
                        }

                        //折扣
                        if(zhekou == "1")
                        {
                            float aa = fast_order_2::modelBase.item(i,3)->text().toFloat() * fast_order_2::modelBase.item(i,4)->text().toFloat() * (100 - fast_order_2::modelBase.item(i,5)->text().toInt() )*0.01;
                            QString zhe = QString().sprintf("%.02f",aa);
                            if(zhe.length() >= zhekoulen)
                            {
                                zhe = zhe.left(zhekoulen);
                            }
                            else
                            {
                                w_scr_dish::setprintsize(zhe, zhekoulen,"zhe");
                            }

                            if(zhekourow == 1)
                            {
                                neirong1.append(zhe);
                            }
                        }
                        if(!neirong1.isEmpty())
                        {
                            neirong1.append("\n");
                            neirong.append(neirong1);
                        }
                        //第一行=========================================================================

                        //第2行=========================================================================
                        QString neirong2;
                        //品名
                        if(pinming == "1")
                        {
                            QString pin = fast_order_2::modelBase.item(i,2)->text();
                            if(bjieduan == "1")
                            {
                                if(pin.length() > 5)
                                {
                                    pin = pin.left(5);
                                }

                            }

                            if(pin.length() >= pinminglen)
                            {
                                pin = pin.left(pinminglen);
                            }
                            else
                            {
                                setprintsize(pin, pinminglen,"pin");
                            }

                            if(pinmingrow == 2)
                            {
                                neirong2.append(pin);
                            }
                        }

                        //单位
                        if(danwei == "1")
                        {
                            QString dan = fast_order_2::modelBase.item(i,12)->text();
                            if(dan.length() >= danweilen)
                            {
                                dan = dan.left(danweilen);
                            }
                            else
                            {
                                w_scr_dish::setprintsize(dan, danweilen,"dan");
                            }

                            if(danweirow == 2)
                            {
                                neirong2.append(dan);
                            }
                        }

                        //数量
                        if(shuliang == "1")
                        {
                            QString shu = fast_order_2::modelBase.item(i,3)->text();
                            if(shuliangxiaoshu == 0)
                            {
                                shu = QString::number(int(shu.toInt()));
                            }
                            else if(shuliangxiaoshu == 1)
                            {
                                shu = QString().sprintf("%.03f",shu.toFloat());
                            }
                            else
                            {
                                shu = QString().sprintf("%.03f",shu.toFloat());
                            }

                            if(shu.length() >= shulianglen)
                            {
                                shu = shu.left(shulianglen);
                            }
                            else
                            {
                                w_scr_dish::setprintsize(shu, shulianglen,"shu");
                            }

                            if(shuliangrow == 2)
                            {
                                neirong2.append(shu);
                            }
                        }

                        //价格
                        if(jiage == "1")
                        {
                            QString jia = fast_order_2::modelBase.item(i,4)->text();
                            if(jiagexiaoshu == 0)
                            {
                                if(fast_order_2::modelBase.item(i,9)->text() == "Y")
                                {
                                    jia = "0";
                                }
                                else
                                {
                                    jia = QString::number(int(jia.toInt()));
                                }
                            }
                            else if(jiagexiaoshu == 1)
                            {
                                if(fast_order_2::modelBase.item(i,9)->text() == "Y")
                                {
                                    jia = tr("0.0");
                                }
                                else
                                {
                                    jia = QString().sprintf("%.01f",jia.toFloat());
                                }
                            }
                            else
                            {
                                if(fast_order_2::modelBase.item(i,9)->text() == "Y")
                                {
                                    jia = "0.000";
                                }
                                else
                                {
                                    jia = QString().sprintf("%.02f",jia.toFloat());
                                }
                            }

                            if(jia.length() >= jiagelen)
                            {
                                jia = jia.left(jiagelen);
                            }
                            else
                            {
                                w_scr_dish::setprintsize(jia, jiagelen,"jia");
                            }

                            if(jiagerow == 2)
                            {
                                neirong2.append(jia);
                            }
                        }

                        //金额
                        if(jine == "1")
                        {
                            float aa;
                            aa = 0.000;
                            aa = fast_order_2::modelBase.item(i,3)->text().toFloat() * fast_order_2::modelBase.item(i,4)->text().toFloat();
                            QString jin = QString().sprintf("%.02f",aa);

                            if(jinexiaoshu == 0)
                            {
                                if(fast_order_2::modelBase.item(i,9)->text() == "Y")
                                {
                                    jin = "0";
                                }
                                else
                                {
                                    jin = QString::number(int(jin.toInt()));
                                }
                            }
                            else if(jinexiaoshu == 1)
                            {
                                if(fast_order_2::modelBase.item(i,9)->text() == "Y")
                                {
                                    jin = tr("0.0");
                                }
                                else
                                {
                                    jin = QString().sprintf("%.01f",jin.toFloat());
                                }
                            }
                            else
                            {
                                if(fast_order_2::modelBase.item(i,9)->text() == "Y")
                                {
                                    jin = "0.000";
                                }
                                else
                                {
                                    jin = QString().sprintf("%.02f",jin.toFloat());
                                }
                            }


                            if(jin.length() >= jinelen)
                            {
                                jin = jin.left(jinelen);
                            }
                            else
                            {
                                w_scr_dish::setprintsize(jin, jinelen,"jin");
                            }

                            if(jinerow == 2)
                            {
                                neirong2.append(jin);
                            }
                        }

                        //折扣
                        if(zhekou == "1")
                        {
                            float aa = fast_order_2::modelBase.item(i,3)->text().toFloat() * fast_order_2::modelBase.item(i,4)->text().toFloat() * (100 - fast_order_2::modelBase.item(i,5)->text().toInt() )*0.01;
                            QString zhe = QString().sprintf("%.02f",aa);
                            if(zhe.length() >= zhekoulen)
                            {
                                zhe = zhe.left(zhekoulen);
                            }
                            else
                            {
                                w_scr_dish::setprintsize(zhe, zhekoulen,"zhe");
                            }

                            if(zhekourow == 2)
                            {
                                neirong2.append(zhe);
                            }
                        }
                        if(!neirong2.isEmpty())
                        {
                            neirong2.append("\n");
                            neirong.append(neirong2);
                        }
                        //第2行=========================================================================
                        if(zuofa == "1")
                        {
                            if(!fast_order_2::modelBase.item(i,7)->text().isEmpty())
                            {
                                neirong.append(fast_order_2::modelBase.item(i,7)->text());
                                neirong.append("\n");
                            }
                        }

                        //content
                    }
                    if(fast_order_2::modelBase.rowCount() > 0)
                    {
                        content.append(neirong);
                        QSqlQueryModel thismodel;
                        thismodel.setQuery(QString("select vch_dish_typename from cey_bt_dish_type where ch_dish_typeno = '%1' ").arg(fast_order_2::modelBase.item(0,22)->text()));
                        QString strlei = tr("      ") + thismodel.record(0).value(0).toString() + tr("------------") + QString().sprintf("%.02f",zongjia) + "\n";
                        content.append(strlei);
                        content.append(n_func::gs_print_line+"\n");
                    }
                }
            }
            else
            {
                QStandardItemModel & cook = fast_order_2::modelBase;
                QString neirong;
                for(int i = 0; i < fast_order_2::modelBase.rowCount(); i++)
                {
                    QString hanghao = QString::number(i+1) + tr(".");
                    if(hanghao.length() < 3)
                    {
                        hanghao.append(" ");
                    }
                    if(bxuhao=="1")neirong.append(hanghao);
                    if(bzensong == "1")
                    {
                        if(fast_order_2::modelBase.item(i,8)->text() == tr("Y"))
                        {
                            neirong.append(tr("赠)"));
                        }
                    }

                    if(btejia == "1")
                    {
                        if(fast_order_2::modelBase.item(i,10)->text() == tr("Y"))
                        {
                            neirong.append(tr("特)"));
                        }
                    }

                    if(fast_order_2::modelBase.item(i,9)->text() == "P")
                    {
                        neirong.append(tr("套)"));
                    }

                    if(fast_order_2::modelBase.item(i,9)->text() == tr("Y"))
                    {
                        neirong.append(tr("细)"));
                    }

                    //第一行=========================================================================
                    QString neirong1;
                    //品名
                    if(pinming == "1")
                    {
                        QString pin = fast_order_2::modelBase.item(i,2)->text();
                        if(bjieduan == "1")
                        {
                            if(pin.length() > 5)
                            {
                                pin = pin.left(5);
                            }

                        }

                        if(pin.length() >= pinminglen)
                        {
                            pin = pin.left(pinminglen);
                        }
                        else
                        {
                            setprintsize(pin, pinminglen,"pin");
                        }

                        if(pinmingrow == 1)
                        {
                            neirong1.append(pin);
                        }
                    }

                    //单位
                    if(danwei == "1")
                    {
                        QString dan = fast_order_2::modelBase.item(i,12)->text();
                        if(dan.length() >= danweilen)
                        {
                            dan = dan.left(danweilen);
                        }
                        else
                        {
                            w_scr_dish::setprintsize(dan, danweilen,"dan");

                        }

                        if(danweirow == 1)
                        {
                            neirong1.append(dan);
                        }
                    }

                    //数量
                    if(shuliang == "1")
                    {
                        QString shu = fast_order_2::modelBase.item(i,3)->text();
                        if(shuliangxiaoshu == 0)
                        {
                            shu = QString::number(int(shu.toInt()));
                        }
                        else if(shuliangxiaoshu == 1)
                        {
                            shu = QString().sprintf("%.03f",shu.toFloat());
                        }
                        else
                        {
                            shu = QString().sprintf("%.03f",shu.toFloat());
                        }

                        if(shu.length() >= shulianglen)
                        {
                            shu = shu.left(shulianglen);
                        }
                        else
                        {
                            w_scr_dish::setprintsize(shu, shulianglen,"shu");
                        }

                        if(shuliangrow == 1)
                        {
                            neirong1.append(shu);
                        }
                    }

                    //价格
                    if(jiage == "1")
                    {
                        QString jia = fast_order_2::modelBase.item(i,4)->text();
                        if(jiagexiaoshu == 0)
                        {
                            if(fast_order_2::modelBase.item(i,9)->text() == "Y")
                            {
                                jia = "0";
                            }
                            else
                            {
                                jia = QString::number(int(jia.toInt()));
                            }
                        }
                        else if(jiagexiaoshu == 1)
                        {
                            if(fast_order_2::modelBase.item(i,9)->text() == "Y")
                            {
                                jia = tr("0.0");
                            }
                            else
                            {
                                jia = QString().sprintf("%.01f",jia.toFloat());
                            }
                        }
                        else
                        {
                            if(fast_order_2::modelBase.item(i,9)->text() == "Y")
                            {
                                jia = "0.000";
                            }
                            else
                            {
                                jia = QString().sprintf("%.02f",jia.toFloat());
                            }
                        }

                        if(jia.length() >= jiagelen)
                        {
                            jia = jia.left(jiagelen);
                        }
                        else
                        {
                            w_scr_dish::setprintsize(jia, jiagelen,"jia");
                        }

                        if(jiagerow == 1)
                        {
                            neirong1.append(jia);
                        }
                    }

                    //金额
                    if(jine == "1")
                    {
                        float aa;
                        aa = 0.000;
                        aa = fast_order_2::modelBase.item(i,3)->text().toFloat() * fast_order_2::modelBase.item(i,4)->text().toFloat();
                        QString jin = QString().sprintf("%.02f",aa);

                        if(jinexiaoshu == 0)
                        {
                            if(fast_order_2::modelBase.item(i,9)->text() == "Y")
                            {
                                jin = "0";
                            }
                            else
                            {
                                jin = QString::number(int(jin.toInt()));
                            }
                        }
                        else if(jinexiaoshu == 1)
                        {
                            if(fast_order_2::modelBase.item(i,9)->text() == "Y")
                            {
                                jin = tr("0.0");
                            }
                            else
                            {
                                jin = QString().sprintf("%.01f",jin.toFloat());
                            }
                        }
                        else
                        {
                            if(fast_order_2::modelBase.item(i,9)->text() == "Y")
                            {
                                jin = "0.000";
                            }
                            else
                            {
                                jin = QString().sprintf("%.02f",jin.toFloat());
                            }
                        }


                        if(jin.length() >= jinelen)
                        {
                            jin = jin.left(jinelen);
                        }
                        else
                        {
                            w_scr_dish::setprintsize(jin, jinelen,"jin");
                        }

                        if(jinerow == 1)
                        {
                            neirong1.append(jin);
                        }
                    }

                    //折扣
                    if(zhekou == "1")
                    {
                        float aa = fast_order_2::modelBase.item(i,3)->text().toFloat() * fast_order_2::modelBase.item(i,4)->text().toFloat() * (100 - fast_order_2::modelBase.item(i,5)->text().toInt() )*0.01;
                        QString zhe = QString().sprintf("%.02f",aa);
                        if(zhe.length() >= zhekoulen)
                        {
                            zhe = zhe.left(zhekoulen);
                        }
                        else
                        {
                            w_scr_dish::setprintsize(zhe, zhekoulen,"zhe");
                        }

                        if(zhekourow == 1)
                        {
                            neirong1.append(zhe);
                        }
                    }
                    if(!neirong1.isEmpty())
                    {
                        neirong1.append("\n");
                        neirong.append(neirong1);
                    }
                    //第一行=========================================================================

                    //第2行=========================================================================
                    QString neirong2;
                    //品名
                    if(pinming == "1")
                    {
                        QString pin = fast_order_2::modelBase.item(i,2)->text();
                        if(bjieduan == "1")
                        {
                            if(pin.length() > 5)
                            {
                                pin = pin.left(5);
                            }

                        }

                        if(pin.length() >= pinminglen)
                        {
                            pin = pin.left(pinminglen);
                        }
                        else
                        {
                            setprintsize(pin, pinminglen,"pin");
                        }

                        if(pinmingrow == 2)
                        {
                            neirong2.append(pin);
                        }
                    }

                    //单位
                    if(danwei == "1")
                    {
                        QString dan = fast_order_2::modelBase.item(i,12)->text();
                        if(dan.length() >= danweilen)
                        {
                            dan = dan.left(danweilen);
                        }
                        else
                        {
                            w_scr_dish::setprintsize(dan, danweilen,"dan");
                        }

                        if(danweirow == 2)
                        {
                            neirong2.append(dan);
                        }
                    }

                    //数量
                    if(shuliang == "1")
                    {
                        QString shu = fast_order_2::modelBase.item(i,3)->text();
                        if(shuliangxiaoshu == 0)
                        {
                            shu = QString::number(int(shu.toInt()));
                        }
                        else if(shuliangxiaoshu == 1)
                        {
                            shu = QString().sprintf("%.03f",shu.toFloat());
                        }
                        else
                        {
                            shu = QString().sprintf("%.03f",shu.toFloat());
                        }

                        if(shu.length() >= shulianglen)
                        {
                            shu = shu.left(shulianglen);
                        }
                        else
                        {
                            w_scr_dish::setprintsize(shu, shulianglen,"shu");
                        }

                        if(shuliangrow == 2)
                        {
                            neirong2.append(shu);
                        }
                    }

                    //价格
                    if(jiage == "1")
                    {
                        QString jia = fast_order_2::modelBase.item(i,4)->text();
                        if(jiagexiaoshu == 0)
                        {
                            if(fast_order_2::modelBase.item(i,9)->text() == "Y")
                            {
                                jia = "0";
                            }
                            else
                            {
                                jia = QString::number(int(jia.toInt()));
                            }
                        }
                        else if(jiagexiaoshu == 1)
                        {
                            if(fast_order_2::modelBase.item(i,9)->text() == "Y")
                            {
                                jia = tr("0.0");
                            }
                            else
                            {
                                jia = QString().sprintf("%.01f",jia.toFloat());
                            }
                        }
                        else
                        {
                            if(fast_order_2::modelBase.item(i,9)->text() == "Y")
                            {
                                jia = "0.000";
                            }
                            else
                            {
                                jia = QString().sprintf("%.02f",jia.toFloat());
                            }
                        }

                        if(jia.length() >= jiagelen)
                        {
                            jia = jia.left(jiagelen);
                        }
                        else
                        {
                            w_scr_dish::setprintsize(jia, jiagelen,"jia");
                        }

                        if(jiagerow == 2)
                        {
                            neirong2.append(jia);
                        }
                    }

                    //金额
                    if(jine == "1")
                    {
                        float aa;
                        aa = 0.000;
                        aa = fast_order_2::modelBase.item(i,3)->text().toFloat() * fast_order_2::modelBase.item(i,4)->text().toFloat();
                        QString jin = QString().sprintf("%.02f",aa);

                        if(jinexiaoshu == 0)
                        {
                            if(fast_order_2::modelBase.item(i,9)->text() == "Y")
                            {
                                jin = "0";
                            }
                            else
                            {
                                jin = QString::number(int(jin.toInt()));
                            }
                        }
                        else if(jinexiaoshu == 1)
                        {
                            if(fast_order_2::modelBase.item(i,9)->text() == "Y")
                            {
                                jin = tr("0.0");
                            }
                            else
                            {
                                jin = QString().sprintf("%.01f",jin.toFloat());
                            }
                        }
                        else
                        {
                            if(fast_order_2::modelBase.item(i,9)->text() == "Y")
                            {
                                jin = "0.000";
                            }
                            else
                            {
                                jin = QString().sprintf("%.02f",jin.toFloat());
                            }
                        }


                        if(jin.length() >= jinelen)
                        {
                            jin = jin.left(jinelen);
                        }
                        else
                        {
                            w_scr_dish::setprintsize(jin, jinelen,"jin");
                        }

                        if(jinerow == 2)
                        {
                            neirong2.append(jin);
                        }
                    }

                    //折扣
                    if(zhekou == "1")
                    {
                        float aa = fast_order_2::modelBase.item(i,3)->text().toFloat() * fast_order_2::modelBase.item(i,4)->text().toFloat() * (100 - fast_order_2::modelBase.item(i,5)->text().toInt() )*0.01;
                        QString zhe = QString().sprintf("%.02f",aa);
                        if(zhe.length() >= zhekoulen)
                        {
                            zhe = zhe.left(zhekoulen);
                        }
                        else
                        {
                            w_scr_dish::setprintsize(zhe, zhekoulen,"zhe");
                        }

                        if(zhekourow == 2)
                        {
                            neirong2.append(zhe);
                        }
                    }
                    if(!neirong2.isEmpty())
                    {
                        neirong2.append("\n");
                        neirong.append(neirong2);
                    }
                    //第2行=========================================================================
                    if(zuofa == "1")
                    {
                        if(!fast_order_2::modelBase.item(i,7)->text().isEmpty())
                        {
                            neirong.append(fast_order_2::modelBase.item(i,7)->text());
                            neirong.append("\n");
                        }
                    }

                }
                content.append(neirong);
                content.append(n_func::gs_print_line+"\n");
            }
        }
        //表尾格式
        //每行打印列数
        model.clear();
        model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_offcols' "));
        int linecount = model.record(0).value(0).toInt();
        //        linecount = 2;
        int line_num = 0;
        //付款详情 现金
        QString xiangqing = configIniRead.value("system_setting/fukuanxiangqing").toString();
        if(xiangqing == "1")
        {
            if(ui->label_14->text().toFloat() > 0)
            {
                line_num++;
                QString str = tr("现金:") + ui->lineEdit_9->text();
                if(str.length() < 12)
                {
                    int a = 12 - str.length();
                    for(int m = 0; m < a; m++)
                    {
                        str.append(" ");
                    }
                }
                if(line_num == linecount)
                {
                    str.append("\n");
                    content.append(str);
                    line_num = 0;
                }
                else
                {
                    content.append(str);
                }
            }

            for(int i = 0; i < table_model->rowCount(); i++)
            {
                line_num++;
                QString str = table_model->item(i,1)->text() + tr(":") + table_model->item(i,3)->text();
                if(str.length() < 12)
                {
                    int a = 12 - str.length();
                    for(int m = 0; m < a; m++)
                    {
                        str.append(" ");
                    }
                }
                if(line_num == linecount)
                {
                    str.append("\n");
                    content.append(str);
                    line_num = 0;
                }
                else
                {
                    content.append(str);
                }
            }
        }

        //消费
        model.clear();
        model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_cost' "));
        if(model.record(0).value(0).toInt() == 1)
        {
            line_num++;
            QString str = tr("消费:") + ui->lineEdit->text();
            if(str.length() < 12)
            {
                int a = 12 - str.length();
                for(int m = 0; m < a; m++)
                {
                    str.append(" ");
                }
            }
            if(line_num == linecount)
            {
                str.append("\n");
                content.append(str);
                line_num = 0;
            }
            else
            {
                content.append(str);
            }
        }

        //折扣
        model.clear();
        model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_discount' "));
        if(model.record(0).value(0).toInt() == 1&&!(printchoose&PRINT_ONLY_COSUME))
        {
            if(ui->lineEdit_2->text().toFloat() > 0)
            {
                line_num++;
                QString str = tr("折扣:") + ui->lineEdit_2->text();
                if(str.length() < 12)
                {
                    int a = 12 - str.length();
                    for(int m = 0; m < a; m++)
                    {
                        str.append(" ");
                    }
                }
                if(line_num == linecount)
                {
                    str.append("\n");
                    content.append(str);
                    line_num = 0;
                }
                else
                {
                    content.append(str);
                }
            }
        }

        //赠送
        model.clear();
        model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_present' "));
        if(model.record(0).value(0).toInt() == 1&&!(printchoose&PRINT_ONLY_COSUME))
        {
            if(ui->lineEdit_3->text().toFloat() > 0)
            {
                line_num++;
                QString str = tr("赠送:") + ui->lineEdit_3->text();
                if(str.length() < 12)
                {
                    int a = 12 - str.length();
                    for(int m = 0; m < a; m++)
                    {
                        str.append(" ");
                    }
                }
                if(line_num == linecount)
                {
                    str.append("\n");
                    content.append(str);
                    line_num = 0;
                }
                else
                {
                    content.append(str);
                }
            }
        }

        //抹零
        model.clear();
        model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_blotout' "));
        if(model.record(0).value(0).toInt() == 1&&!(printchoose&PRINT_ONLY_COSUME))
        {
            if(ui->lineEdit_5->text().toFloat() != 0)
            {
                line_num++;
                QString str = tr("抹零:") + ui->lineEdit_5->text();
                if(str.length() < 12)
                {
                    int a = 12 - str.length();
                    for(int m = 0; m < a; m++)
                    {
                        str.append(" ");
                    }
                }
                if(line_num == linecount)
                {
                    str.append("\n");
                    content.append(str);
                    line_num = 0;
                }
                else
                {
                    content.append(str);
                }
            }
        }

        //税收
        QString shuishou = configIniRead.value("system_setting/shuishoujine").toString();
        //shuishou = "1";
        if(shuishou.toInt() == 1&&!(printchoose&PRINT_ONLY_COSUME))
        {
            if(ui->lineEdit_6->text().toFloat() != 0)
            {
                line_num++;
                QString str = tr("税收:") + ui->lineEdit_6->text();
                if(str.length() < 12)
                {
                    int a = 12 - str.length();
                    for(int m = 0; m < a; m++)
                    {
                        str.append(" ");
                    }
                }
                if(line_num == linecount)
                {
                    str.append("\n");
                    content.append(str);
                    line_num = 0;
                }
                else
                {
                    content.append(str);
                }
            }
        }

        //应收
        model.clear();
        model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_receive' "));
        if(model.record(0).value(0).toInt() == 1&&!(printchoose&PRINT_ONLY_COSUME))
        {
            line_num++;
            QString str = tr("应收:") + ui->lineEdit_4->text();
            if(str.length() < 12)
            {
                int a = 12 - str.length();
                for(int m = 0; m < a; m++)
                {
                    str.append(" ");
                }
            }
            if(line_num == linecount)
            {
                str.append("\n");
                content.append(str);
                line_num = 0;
            }
            else
            {
                content.append(str);
            }
        }

        model.clear();
        model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_cash' "));
        if(model.record(0).value(0).toInt() == 1&&!(printchoose&PRINT_ONLY_COSUME))
        {
            line_num++;
            QString str = tr("实收:") + QString().sprintf("%.02f", ui->label_12->text().toFloat() + ui->lineEdit_9->text().toFloat());
            /*ui->lineEdit_4->text();*///是应收的金额？所手的全部金额
            if(str.length() < 12)
            {
                int a = 12 - str.length();
                for(int m = 0; m < a; m++)
                {
                    str.append(" ");
                }
            }
            if(line_num == linecount)
            {
                str.append("\n");
                content.append(str);
                line_num = 0;
            }
            else
            {
                content.append(str);
            }
        }

        //找零
        if(configIniRead.value("system_setting/zhaolingjine").toInt() == 1&&!(printchoose&PRINT_ONLY_COSUME)){
            QString str = tr("找零:") + ui->lineEdit_7->text();
            if(str.length() < 12)
            {
                int a = 12 - str.length();
                for(int m = 0; m < a; m++)
                {
                    str.append(" ");
                }
            }
            if(line_num == linecount)
            {
                str.append("\n");
                content.append(str);
                line_num = 0;
            }
            else
            {
                str.append("\n");
                content.append(str);
            }
        }

        for(int i = 0; i < table_model->rowCount(); i++)
        {
            if(table_model->item(i,1)->text() == tr("充值卡"))
            {
                QSqlQueryModel model_ka;
                model_ka.setQuery(QString("SELECT  b.vch_memberno ,b.vch_cardno,b.vch_member ,ifnull((select d.vch_typename from t_m_member_type d where d.ch_typeno = b.ch_typeno ),b.ch_typeno) ,b.vch_tel ,b.vch_handtel ,b.ch_state ,b.ch_cardflag ,    DATE_FORMAT(b.dt_birthday, '%Y-%m-%d %h:%i:%s'),    b.vch_operid ,   DATE_FORMAT(b.dt_limit, '%Y-%m-%d %h:%i:%s'),ifnull( (select num_point from t_m_curamount where vch_memberno = b.vch_memberno),0) as point  FROM t_m_member_type a  ,t_m_member b  where a.ch_typeno = b.ch_typeno and b.vch_memberno = '%1'").arg(table_model->item(i,4)->text()));

                //会员名称
                model.clear();
                model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_membername' "));
                if(model.record(0).value(0).toInt() == 1)
                {
                    line_num++;
                    QString str = tr("会员名称:") + table_model->item(i,4)->text();
                    if(str.length() < 12)
                    {
                        int a = 12 - str.length();
                        for(int m = 0; m < a; m++)
                        {
                            str.append(" ");
                        }
                    }
                    if(line_num == linecount)
                    {
                        str.append("\n");
                        content.append(str);
                        line_num = 0;
                    }
                    else
                    {
                        content.append(str);
                    }
                }

                //会员类型
                model.clear();
                model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_membertype' "));
                if(model.record(0).value(0).toInt() == 1)
                {
                    line_num++;
                    QString str = tr("会员类型:") + model_ka.record(0).value(3).toString();
                    if(str.length() < 12)
                    {
                        int a = 12 - str.length();
                        for(int m = 0; m < a; m++)
                        {
                            str.append(" ");
                        }
                    }
                    if(line_num == linecount)
                    {
                        str.append("\n");
                        content.append(str);
                        line_num = 0;
                    }
                    else
                    {
                        content.append(str);
                    }
                }

                //会员积分
                model.clear();
                model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='print_vip_point' "));
                if(model.record(0).value(0).toInt() == 1)
                {
                    line_num++;
                    QString str = tr("会员积分:") + QString().sprintf("%.02f",model_ka.record(0).value(11).toFloat());
                    if(str.length() < 12)
                    {
                        int a = 12 - str.length();
                        for(int m = 0; m < a; m++)
                        {
                            str.append(" ");
                        }
                    }
                    if(line_num == linecount)
                    {
                        str.append("\n");
                        content.append(str);
                        line_num = 0;
                    }
                    else
                    {
                        content.append(str);
                    }
                }

                //会员信誉额
                model.clear();
                model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='print_ye' "));
                if(model.record(0).value(0).toInt() == 1)
                {
                    line_num++;
                    QString str = tr("会员余额:") + huyuankayue;
                    if(str.length() < 12)
                    {
                        int a = 12 - str.length();
                        for(int m = 0; m < a; m++)
                        {
                            str.append(" ");
                        }
                    }
                    if(line_num == linecount)
                    {
                        str.append("\n");
                        content.append(str);
                        line_num = 0;
                    }
                    else
                    {
                        content.append(str);
                    }
                }
            }
        }
        content.append("\n"+n_func::gs_print_line+"\n");
        //签名
        model.clear();
        model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_underwrite' "));
        if(model.record(0).value(0).toInt() == 1)
        {
            QString str = tr("签名:\n");
            content.append(str);
        }

        //尾语
        {
            QString tail=configIniRead.value("system_setting/jieweiyu").toString();
            if(!tail.isEmpty())
                content.append(tail);
        }

        //打印份数
        model.clear();
        model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_count' "));
        int print_count = model.record(0).value(0).toInt();

        //!小票打印开始
        blockIns b;
        //图片打印
        QString logo = configIniRead.value("system_setting/dayinlogo").toString();
        if(logo == "1"
                &&printchoose&PRINT_PIC)
        {
            QString str = configIniRead.value("system_setting/dayinlogoneirong").toString();//图片路径
            b+= backheader::printer->drawLogo("1B40 1B6101", str);//清除缓存 居中 pixmap路径
        }
        //1表头
        content.append("\n");
        if(configIniRead.value("system_setting/biaotoushezhi").toBool()){
            b.append(backheader::printer->drawText("1B6100 1D2100"));//左对齐,正常字体
            b.append(backheader::printer->drawText("1B6101 1D2111",configIniRead.value("system_setting/zhangdanbiaoti").toString()+"\n"));//居中不放大
        }
        if(!configIniRead.value("system_setting/not_show_bill_title").toBool()){
            b.append(backheader::printer->drawText("1B6100 1D2100"));//左对齐,正常字体
            b.append(backheader::printer->drawText("1B6101 1D2111",title));//居中不放大
        }
        //付款信息
        b+=backheader::printer->drawText("1B40 1C5700", "");//放大
        b.append(backheader::printer->drawText("","\n"));
        b.append(b_pre);
        //付款内容
        QStringList contentlist=content.split("\n");
        QString content_len=n_func::f_get_sysparm("bp_content_len");
        foreach(QString ct, contentlist){
            setprintsize(b, content_len, ct+"\n");
        }
        //打印一维码
        QString yiweima = configIniRead.value("system_setting/yiweima").toString();
        if(yiweima == "1"&&printchoose&PRINT_BARTYPE)
        {
            QString str = configIniRead.value("system_setting/yiweimaneirong").toString();//一维码内容
            //str = "12345678";
            b+= backheader::printer->drawoneBarcode("1B40 1B6101",str);//清除缓存 居中 pixmap路径
        }

        //打印二维码
        QString erweima = configIniRead.value("system_setting/erweima").toString();
        if(erweima == "1"&&printchoose&PRINT_BARTYPE)
        {
            QString str = configIniRead.value("system_setting/erweimaneirong").toString();
            b+= backheader::printer->drawQR_Code("", str);//清除缓存 居中 pixmap路径
        }

        b += backheader::printer->drawText("1B6405 1D5601", "");//走纸5x， 切纸
        b=backheader::printer->setPrintCount(b, print_count);
        //是否开钱箱
        if(configIniRead.value("system_setting/qianxiang").toBool()
                &&printchoose&PRINT_DRAWER){
            blockIns bins;
            bins.append(pieceIns(configIniRead.value("system_setting/qianxiangzhiling").toString().toLocal8Bit(), ""));
            b+=bins;
        }

        //保存小票
        {
            QByteArray byte;
            QDataStream in(&byte, QIODevice::WriteOnly);
            foreach(pieceIns ins, b){
                in<<ins.first;
                in<<ins.second;
            }
            QFile file(public_sql::fastpay_printdata_dir+"/"+wdanhao);
            file.open(QFile::WriteOnly);
            file.write(byte);
            file.close();
        }
        if(configIniRead.value("system_setting/maidanzhangdan").toString() == "1"
                &&printchoose&PRINT_BILL){
            //打印小票
            backheader::asyn_dataprint(b);
        }
        if(_parent)
            _parent->emit_signal_payfor_info(b);
        //打印标签，奶茶模式
        if(printchoose&PRINT_LABEL)
            tolabelprint();
    }
    if(printchoose&PRINT_KITCHEN)
        tokitchenprint("", _dayindanhao,
                       (liushuihao_p_is_shoudog.isEmpty()?liushuihao_p:liushuihao_p_is_shoudog));
    if(printchoose&PRINT_KITCHEN_OUT)
        tokitchenprint("out_", _dayindanhao,
                       (liushuihao_p_is_shoudog.isEmpty()?liushuihao_p:liushuihao_p_is_shoudog));

    //出品打印结束=============================
}

void fast_pay::tokitchenprint(const QString &key, const QString &_dayindanhao, const QString &serial_no)
{
    kitchen_or_outprint(key, _dayindanhao,serial_no);
}

void fast_pay::tolabelprint()
{
    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    QSqlQuery query;
    if(conf.value("system_setting/naichamoshi").toBool()){///if
        labelprint_main_dialog dialog;
        dialog.printmodel->setHorizontalHeaderLabels(QStringList()
                                                     << tr("打印次数")<< tr("单号")<< tr("价格")
                                                     << tr("品名") <<  tr("品码") <<tr("时间") << tr("做法")
                                                     );
        for(int r=0,r_count=fast_order_2::modelBase.rowCount();r<r_count;r++){
            query.exec(QString("select ch_labelprint_flag from cey_bt_dish_subtype where ch_sub_typeno='%1' ")
                       .arg(fast_order_2::modelBase.item(r, 23)->text()));
            query.next();
            if(query.record().value("ch_labelprint_flag").toString() == "Y"){
                for(int pr=0,pcount=fast_order_2::modelBase.item(r, 3)->text().toFloat();pr<pcount;pr++){
                    dialog.printmodel->appendRow(QList<QStandardItem*>()
                                                 <<new QStandardItem("1")
                                                 <<new QStandardItem(tr("流水号:")+(liushuihao_p_is_shoudog.isEmpty()?liushuihao_p:liushuihao_p_is_shoudog))
                                                 <<new QStandardItem(tr("价格:")+fast_order_2::modelBase.item(r, 4)->text())
                                                 <<new QStandardItem(tr("品名:")+fast_order_2::modelBase.item(r, 2)->text())
                                                 <<new QStandardItem(/*tr("品码:")+*/fast_order_2::modelBase.item(r, 1)->text())
                                                 <<new QStandardItem(/*tr("时间:")+*/n_func::f_get_sysdatetime().toString("yyyy-MM-dd hh:mm:ss"))
                                                 <<new QStandardItem(/*tr("做法:")+*/fast_order_2::modelBase.item(r, 7)->text())
                                                 );
                }//for
            }//if
        }//for
        dialog.toprint();
    }///if
}

void fast_pay::kitchen_or_outprint(const QString &key, const QString &_dayindanhao, const QString &serial_no)
{
    /*cJSON*/cJSON *root=cJSON_CreateObject();
    QString liuhuihaoma = serial_no;

    //厨房打印机
    QSettings *kitchenprinter = new  QSettings(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    //流水号
    K_DATA_CONTENT kdcontent;
    QList<BINS_IP> bins_ip_list;
    int printcount=kitchenprinter->value("w_bt_dish_Kitchenprinter_param/"+key+"kitchen_num_spin").toInt();//打印次数

    kdcontent.about_title.append(K_TITLE(tr("流水号"),  liuhuihaoma.toLocal8Bit()));/*cJSON*/cJSON_AddItemToObject(root, kdcontent.about_title.last().pre.toLocal8Bit().data(), cJSON_CreateString(kdcontent.about_title.last().content.data()));
    kdcontent.about_title.append(K_TITLE(tr("开台单号"), _dayindanhao.toLocal8Bit()));/*cJSON*/cJSON_AddItemToObject(root, kdcontent.about_title.last().pre.toLocal8Bit().data(), cJSON_CreateString(kdcontent.about_title.last().content.data()));
    kdcontent.about_title.append(K_TITLE(tr("餐桌名称"), frontViewDialog_virtual::cur_desk_no.toLocal8Bit()));/*cJSON*/cJSON_AddItemToObject(root, kdcontent.about_title.last().pre.toLocal8Bit().data(), cJSON_CreateString(kdcontent.about_title.last().content.data()));
    kdcontent.about_title.append(K_TITLE(tr("点菜人"), n_func::gs_operid.toLocal8Bit()));/*cJSON*/cJSON_AddItemToObject(root, kdcontent.about_title.last().pre.toLocal8Bit().data(), cJSON_CreateString(kdcontent.about_title.last().content.data()));
    kdcontent.about_title.append(K_TITLE(tr("点菜时间"), fast_order_2::diancaitime.toLocal8Bit()));/*cJSON*/cJSON_AddItemToObject(root, kdcontent.about_title.last().pre.toLocal8Bit().data(), cJSON_CreateString(kdcontent.about_title.last().content.data()));
    kdcontent.about_title.append(K_TITLE(tr("打印时间"), n_func::f_get_sysdatetime().toString("yyyy-MM-dd hh:mm:ss").toLocal8Bit()));/*cJSON*/cJSON_AddItemToObject(root, kdcontent.about_title.last().pre.toLocal8Bit().data(), cJSON_CreateString(kdcontent.about_title.last().content.data()));
    kdcontent.about_title.append(K_TITLE(tr("人数"), "1"));/*cJSON*/cJSON_AddItemToObject(root, kdcontent.about_title.last().pre.toLocal8Bit().data(), cJSON_CreateString(kdcontent.about_title.last().content.data()));

    kdcontent.about_content.content.append(
                INDEX_DATA_CONTENT(tr(""), "", 0.0,QVector<DATA_CONTENT>()
                                   << DATA_CONTENT(tr("品名"),tr("品名"))
                                   << DATA_CONTENT(tr("单位"),tr("单位"))
                                   << DATA_CONTENT(tr("品码"),tr("品码"))
                                   << DATA_CONTENT(tr("数量"),tr("数量"))
                                   << DATA_CONTENT(tr("价格"),tr("价格"))
                                   )
                );

    //一菜一单
    //打印到一起，打印套菜，打印套菜明细
    //厨打份数
    //一一菜一单是，打印菜品小类
    QSqlQuery query;
    QString pre;//例如套，细
    QStringList index_value;//打印内容
    switch(kitchenprinter->value("w_bt_dish_Kitchenprinter_param/"+key+"suit_printtype_com").toInt()){
    case 0://只打印主套菜       [不打印套菜明细]
        index_value.append("Y");/*cJSON*/cJSON_AddItemToObject(root, tr("厨打方式").toLocal8Bit().data(), cJSON_CreateString(tr("只打印主套菜").toLocal8Bit().data()));
        break;
    case 1://只打印套菜明细    [不打印主套菜]
        index_value.append("P");/*cJSON*/cJSON_AddItemToObject(root, tr("厨打方式").toLocal8Bit().data(), cJSON_CreateString(tr("只打印套菜明细").toLocal8Bit().data()));
        break;
    case 2://打印在一起
        /*cJSON*/cJSON_AddItemToObject(root, tr("厨打方式").toLocal8Bit().data(), cJSON_CreateString(tr("打印在一起").toLocal8Bit().data()));
        break;
    }
    if(kitchenprinter->value("w_bt_dish_Kitchenprinter_param/"+key+"dish_printtype_com").toInt() == 0){////一菜一单
        if(kitchenprinter->value("w_bt_dish_Kitchenprinter_param/"+key+"dish_bill_printtype_check").toString() == "1"){//打印菜品小类
            /*cJSON*/cJSON_AddItemToObject(root, tr("打印方式").toLocal8Bit().data(), cJSON_CreateString(tr("一菜一单").toLocal8Bit().data()));
            cJSON*array=0;
            cJSON_AddItemToObject(root, tr("打印内容").toLocal8Bit().data(), array=cJSON_CreateArray());

            for(int row = 0; row < fast_order_2::modelBase.rowCount(); row++){
                while(kdcontent.about_content.content.count() >1){
                    kdcontent.about_content.content.pop_back();
                }
                //屏蔽一些打印内容
                if(index_value.contains(fast_order_2::modelBase.item(row, 9)->text())){
                    continue;
                }
                pre="";
                if(fast_order_2::modelBase.item(row, 9)->text()=="P")pre=tr("套)");
                if(fast_order_2::modelBase.item(row, 9)->text()=="Y")pre=tr("细)");
                //!若是中餐的话，已经落单的不需要厨打-begin
                if(fast_order_2::modelBase.columnCount()>24){
                    if(fast_order_2::modelBase.item(row, 24)->text()=="Y")
                        continue;
                }
                //!若是中餐的话，已经落单的不需要厨打-end

                query.exec(QString(key.isEmpty()?
                                       "select ch_printflag,vch_printer from cey_bt_dish where ch_dishno = '%1'":
                                       "select ch_outflag,vch_outprint from cey_bt_dish where ch_dishno = '%1'").arg(fast_order_2::modelBase.item(row,1)->text()));
                query.next();if(query.record().value(0).toString()!="Y"){continue;}

                kdcontent.about_content.content.append(
                            INDEX_DATA_CONTENT(tr(""), fast_order_2::modelBase.item(row, 1)->text(), fast_order_2::modelBase.item(row, 4)->text().toFloat(),
                                               QVector<DATA_CONTENT>()
                                               << DATA_CONTENT(tr("品名"), pre+fast_order_2::modelBase.item(row, 2)->text())//品名
                                               << DATA_CONTENT(tr("单位"), fast_order_2::modelBase.item(row, 12)->text())//单位
                                               << DATA_CONTENT(tr("品码"), fast_order_2::modelBase.item(row, 1)->text())//品码
                                               << DATA_CONTENT(tr("数量"), fast_order_2::modelBase.item(row, 3)->text())//数量
                                               << DATA_CONTENT(tr("价格"), fast_order_2::modelBase.item(row, 4)->text())//价格
                                               )
                            );

                cJSON*subarray=0;
                cJSON_AddItemToObject(array, "", subarray=cJSON_CreateObject());
                cJSON_AddStringToObject(subarray,tr("品名").toLocal8Bit().data(),     (pre+fast_order_2::modelBase.item(row, 2)->text()).toLocal8Bit().data());
                cJSON_AddStringToObject(subarray,tr("单位").toLocal8Bit().data(),     (fast_order_2::modelBase.item(row, 12)->text()).toLocal8Bit().data());
                cJSON_AddStringToObject(subarray,tr("品码").toLocal8Bit().data(),     (fast_order_2::modelBase.item(row, 1)->text()).toLocal8Bit().data());
                cJSON_AddStringToObject(subarray,tr("数量").toLocal8Bit().data(),     (fast_order_2::modelBase.item(row, 3)->text()).toLocal8Bit().data());
                cJSON_AddStringToObject(subarray,tr("价格").toLocal8Bit().data(),     (fast_order_2::modelBase.item(row, 4)->text()).toLocal8Bit().data());

                if(!fast_order_2::modelBase.item(row,  7)->text().isEmpty()){
                    kdcontent.about_content.content.append(
                                INDEX_DATA_CONTENT(tr(""), fast_order_2::modelBase.item(row, 1)->text(), 0.0, QVector<DATA_CONTENT>()
                                                   << DATA_CONTENT(tr("做法"), tr("做法:")+fast_order_2::modelBase.item(row, 7)->text())//做法
                                                   )
                                );
                    cJSON_AddStringToObject(subarray,tr("做法").toLocal8Bit().data(),     (fast_order_2::modelBase.item(row, 7)->text()).toLocal8Bit().data());
                }
                for(int x=0;x<printcount;x++){
                    bins_ip_list.append(
                                w_bt_dish_Kitchenprinter::k_data_print(kdcontent, key, _dayindanhao)
                                );
                }
            }
        }
    }////一菜一单

    //一桌一单
    //打印到一起，打印套菜，打印套菜明细
    //厨打份数
    //一桌一单是，打印菜品大类，打印菜品小类，全不选是全不一单
    int table_check=0;//0没有勾选；1大类打印；2小类打印；
    QString pre_typeno;//上一个大类用于子套菜时处理
    QString cur_type;//当前大类
    QString about_type;//大类
    if(kitchenprinter->value("w_bt_dish_Kitchenprinter_param/"+key+"table_bill_printseries_check").toString() == "1"){//打印菜品大类
        table_check+=1;
    }
    if(kitchenprinter->value("w_bt_dish_Kitchenprinter_param/"+key+"table_bill_printtype_check").toString() == "1"){//打印菜品大类
        table_check+=2;
    }
    if(kitchenprinter->value("w_bt_dish_Kitchenprinter_param/"+key+"dish_printtype_com").toInt() == 1){////一桌一单
        QMap<QString/*大类或者小类*/, DATACPQ/*具体内容*/> str_dcq_map;
        /*cJSON*/cJSON_AddItemToObject(root, tr("打印方式").toLocal8Bit().data(), cJSON_CreateString(tr("一桌一单").toLocal8Bit().data()));
        cJSON*subroot=0;
        cJSON_AddItemToObject(root, tr("打印内容").toLocal8Bit().data(), subroot=cJSON_CreateObject());
        switch(table_check){
        case 0:{
            /*cJSON*/cJSON_AddItemToObject(root, tr("一桌一单方式").toLocal8Bit().data(), cJSON_CreateString(tr("全部打印").toLocal8Bit().data()));
            for(int row = 0; row < fast_order_2::modelBase.rowCount(); row++){////for
                pre="";
                if(fast_order_2::modelBase.item(row, 9)->text()=="P")pre=tr("套)");
                if(fast_order_2::modelBase.item(row, 9)->text()=="Y")pre=tr("细)");
                //!若是中餐的话，已经落单的不需要厨打-begin
                if(fast_order_2::modelBase.columnCount()>24){
                    if(fast_order_2::modelBase.item(row, 24)->text()=="Y")
                        continue;
                }
                //!若是中餐的话，已经落单的不需要厨打-end

                about_type="";
                if(str_dcq_map.find(about_type)==str_dcq_map.end()){
                    str_dcq_map.insert(about_type,DATACPQ());
                }
                pre_typeno=about_type;
                cur_type="";
                query.exec(QString(key.isEmpty()?
                                       "select ch_printflag,vch_printer from cey_bt_dish where ch_dishno = '%1'":
                                       "select ch_outflag,vch_outprint from cey_bt_dish where ch_dishno = '%1'").arg(fast_order_2::modelBase.item(row,1)->text()));
                query.next();if(query.record().value(0).toString()!="Y"){continue;}

                str_dcq_map[pre_typeno].append(
                            INDEX_DATA_CONTENT(cur_type,fast_order_2::modelBase.item(row,1)->text(),fast_order_2::modelBase.item(row,4)->text().toFloat()*fast_order_2::modelBase.item(row,3)->text().toFloat(),
                                               QVector<DATA_CONTENT>()
                                               << DATA_CONTENT(tr("品名"), pre+fast_order_2::modelBase.item(row,2)->text())//品名
                                               << DATA_CONTENT(tr("单位"), fast_order_2::modelBase.item(row,12)->text())//单位
                                               << DATA_CONTENT(tr("品码"), fast_order_2::modelBase.item(row,1)->text())//品码
                                               << DATA_CONTENT(tr("数量"), fast_order_2::modelBase.item(row,3)->text())//数量
                                               << DATA_CONTENT(tr("价格"), fast_order_2::modelBase.item(row,4)->text())//价格
                                               )
                            );
                if(!fast_order_2::modelBase.item(row, 7)->text().isEmpty()){
                    str_dcq_map[pre_typeno].append(
                                INDEX_DATA_CONTENT(tr(""),fast_order_2::modelBase.item(row,1)->text(),0.0,
                                                   QVector<DATA_CONTENT>()
                                                   << DATA_CONTENT(tr("做法"), tr("做法:")+fast_order_2::modelBase.item(row, 7)->text())//做法
                                                   )
                                );
                }
            }////for
        }
            break;
        case 1://打印菜品大类
            /*cJSON*/cJSON_AddItemToObject(root, tr("一桌一单方式").toLocal8Bit().data(), cJSON_CreateString(tr("打印菜品大类").toLocal8Bit().data()));
            for(int row = 0; row < fast_order_2::modelBase.rowCount(); row++){////for
                pre="";
                if(fast_order_2::modelBase.item(row, 9)->text()=="P")pre=tr("套)");
                if(fast_order_2::modelBase.item(row, 9)->text()=="Y")pre=tr("细)");
                //!若是中餐的话，已经落单的不需要厨打-begin
                if(fast_order_2::modelBase.columnCount()>24){
                    if(fast_order_2::modelBase.item(row, 24)->text()=="Y")
                        continue;
                }
                //!若是中餐的话，已经落单的不需要厨打-end

                query.exec(QString("SELECT vch_dish_typename FROM hddpos.cey_bt_dish_type where ch_dish_typeno = '%1'")
                           .arg(fast_order_2::modelBase.item(row, 22)->text())
                           );
                query.next();
                about_type=query.record().value("vch_dish_typename").toString();
                if(str_dcq_map.find(about_type)==str_dcq_map.end()){
                    str_dcq_map.insert(about_type,DATACPQ());
                }

                if(fast_order_2::modelBase.item(row, 9)->text()=="Y"){//是套菜
                } else {
                    pre_typeno=about_type;
                }
                cur_type="";
                if(str_dcq_map[pre_typeno].isEmpty()){
                    cur_type=about_type;
                }
                query.exec(QString(key.isEmpty()?
                                       "select ch_printflag,vch_printer from cey_bt_dish where ch_dishno = '%1'":
                                       "select ch_outflag,vch_outprint from cey_bt_dish where ch_dishno = '%1'").arg(fast_order_2::modelBase.item(row,1)->text()));
                query.next();if(query.record().value(0).toString()!="Y"){continue;}
                str_dcq_map[pre_typeno].append(
                            INDEX_DATA_CONTENT(cur_type,fast_order_2::modelBase.item(row,1)->text(),fast_order_2::modelBase.item(row,4)->text().toFloat()*fast_order_2::modelBase.item(row,3)->text().toFloat(),
                                               QVector<DATA_CONTENT>()
                                               << DATA_CONTENT(tr("品名"),pre+fast_order_2::modelBase.item(row,2)->text())//品名
                                               << DATA_CONTENT(tr("单位"),fast_order_2::modelBase.item(row,12)->text())//单位
                                               << DATA_CONTENT(tr("品码"),fast_order_2::modelBase.item(row,1)->text())//品码
                                               << DATA_CONTENT(tr("数量"),fast_order_2::modelBase.item(row,3)->text())//数量
                                               << DATA_CONTENT(tr("价格"),fast_order_2::modelBase.item(row,4)->text())//价格
                                               )
                            );
                if(!fast_order_2::modelBase.item(row, 7)->text().isEmpty()){
                    str_dcq_map[pre_typeno].append(
                                INDEX_DATA_CONTENT(tr(""),fast_order_2::modelBase.item(row,1)->text(),0.0,
                                                   QVector<DATA_CONTENT>()
                                                   << DATA_CONTENT(tr("做法"), tr("做法:")+fast_order_2::modelBase.item(row, 7)->text())//做法
                                                   )
                                );
                }

            }////for
            break;
        case 2://打印菜品小类
            /*cJSON*/cJSON_AddItemToObject(root, tr("一桌一单方式").toLocal8Bit().data(), cJSON_CreateString(tr("打印菜品小类").toLocal8Bit().data()));
            for(int row = 0; row < fast_order_2::modelBase.rowCount(); row++){////for
                pre="";
                if(fast_order_2::modelBase.item(row, 9)->text()=="P")pre=tr("套)");
                if(fast_order_2::modelBase.item(row, 9)->text()=="Y")pre=tr("细)");
                //!若是中餐的话，已经落单的不需要厨打-begin
                if(fast_order_2::modelBase.columnCount()>24){
                    if(fast_order_2::modelBase.item(row, 24)->text()=="Y")
                        continue;
                }
                //!若是中餐的话，已经落单的不需要厨打-end

                query.exec(QString("SELECT vch_sub_typename FROM cey_bt_dish_subtype where ch_sub_typeno = '%1';")
                           .arg(fast_order_2::modelBase.item(row, 23)->text())
                           );
                query.next();
                about_type=query.record().value("vch_sub_typename").toString();
                if(str_dcq_map.find(about_type)==str_dcq_map.end()){
                    str_dcq_map.insert(about_type,DATACPQ());
                }
                if(fast_order_2::modelBase.item(row, 9)->text()=="Y"){//是套菜
                } else {
                    pre_typeno=about_type;
                }
                cur_type="";
                if(str_dcq_map[pre_typeno].isEmpty()){
                    cur_type=about_type;
                }
                query.exec(QString(key.isEmpty()?
                                       "select ch_printflag,vch_printer from cey_bt_dish where ch_dishno = '%1'":
                                       "select ch_outflag,vch_outprint from cey_bt_dish where ch_dishno = '%1'").arg(fast_order_2::modelBase.item(row,1)->text()));
                query.next();if(query.record().value(0).toString()!="Y"){continue;}
                str_dcq_map[pre_typeno].append(
                            INDEX_DATA_CONTENT(cur_type,fast_order_2::modelBase.item(row,1)->text(),fast_order_2::modelBase.item(row,4)->text().toFloat()*fast_order_2::modelBase.item(row,3)->text().toFloat(),
                                               QVector<DATA_CONTENT>()
                                               << DATA_CONTENT(tr("品名"),pre+fast_order_2::modelBase.item(row,2)->text())//品名
                                               << DATA_CONTENT(tr("单位"),fast_order_2::modelBase.item(row,12)->text())//单位
                                               << DATA_CONTENT(tr("品码"),fast_order_2::modelBase.item(row,1)->text())//品码
                                               << DATA_CONTENT(tr("数量"),fast_order_2::modelBase.item(row,3)->text())//数量
                                               << DATA_CONTENT(tr("价格"),fast_order_2::modelBase.item(row,4)->text())//价格
                                               )
                            );
                if(!fast_order_2::modelBase.item(row, 7)->text().isEmpty()){
                    str_dcq_map[pre_typeno].append(
                                INDEX_DATA_CONTENT(tr(""),fast_order_2::modelBase.item(row,1)->text(),0.0,
                                                   QVector<DATA_CONTENT>()
                                                   << DATA_CONTENT(tr("做法"),tr("做法:")+fast_order_2::modelBase.item(row, 7)->text())//做法
                                                   )
                                );
                }
            }////for
            break;
        }

        //汇总
        for(QMap<QString/*大类或者小类*/, DATACPQ/*具体内容*/>::iterator str_dcq_map_itr=str_dcq_map.begin();
            str_dcq_map_itr != str_dcq_map.end();
            str_dcq_map_itr++){
            kdcontent.about_tail.clear();
            while(kdcontent.about_content.content.count() >1){
                kdcontent.about_content.content.pop_back();
            }
            kdcontent.about_content.content.append(str_dcq_map_itr.value());

            //!-----------------------begin
            cJSON*array=0;cJSON*subarray=0;
            cJSON_AddItemToObject(subroot, str_dcq_map_itr.key().toLocal8Bit().data(), array=cJSON_CreateArray());
            foreach(INDEX_DATA_CONTENT idc, str_dcq_map_itr.value()){
                cJSON_AddItemToObject(array, "", subarray=cJSON_CreateObject());
                foreach(DATA_CONTENT dc, idc.datas){
                    cJSON_AddStringToObject(subarray,dc.pre.toLocal8Bit().data(), dc.vch_dishname.toLocal8Bit().data());
                }
            }
            cJSON_AddStringToObject(array, tr("菜品金额合计").toLocal8Bit().data(), "0.0");
            //!-----------------------end

            kdcontent.about_tail.append(K_TAIL(tr("菜品金额合计"), "0.0"));
            for(int x=0;x<printcount;x++){
                bins_ip_list.append(
                            w_bt_dish_Kitchenprinter::k_data_print(kdcontent, key, _dayindanhao)
                            );
            }
        }
    }////一桌一单

    w_bt_dish_Kitchenprinter::one_kdata_stragety_print(bins_ip_list);
    w_sys_manage_cloudsync::print_yun(root);
    cJSON_Delete(root);

}

void fast_pay::update_lowcost_service()
{
    //低消
    ui->lineEdit_14->setText(QString().sprintf("%0.2f",pay_servicecost));
    ui->lineEdit_13->setText(QString().sprintf("%0.2f",qMax(float(0.0), pay_lowcost-ui->lineEdit_4->text().toFloat())));
    //应收 消费+低消差
    ui->lineEdit_autualpay->setText(QString().sprintf("%.02f", ui->lineEdit_4->text().toFloat()+ui->lineEdit_13->text().toFloat()+ui->lineEdit_14->text().toFloat()));

    otherpay=0;
    for(int row=0,rowcount=table_model->rowCount();row<rowcount;row++){
        otherpay+=table_model->item(row, 2)->text().toFloat();
    }
    //应收金额
    ui->label_14->setText(QString().sprintf("%.02f", ui->lineEdit_autualpay->text().toFloat()-otherpay));
    ui->lineEdit_9->setText(ui->label_14->text());
    //找零
}

void fast_pay::setjiulou_visiable(bool isvisibled)
{
    ui->label_15->setVisible(isvisibled);
    ui->lineEdit_15->setVisible(isvisibled);
    ui->label_20->setVisible(isvisibled);
    ui->lineEdit_12->setVisible(isvisibled);
    ui->label_18->setVisible(isvisibled);
    ui->lineEdit_13->setVisible(isvisibled);
    ui->label_19->setVisible(isvisibled);
    ui->lineEdit_14->setVisible(isvisibled);
    ui->label_17->setVisible(isvisibled);
    ui->lineEdit_11->setVisible(isvisibled);
    ui->label_autualpay->setVisible(isvisibled);
    ui->lineEdit_autualpay->setVisible(isvisibled);

    ui->label_5->setVisible(!isvisibled);
    ui->lineEdit_4->setVisible(!isvisibled);
}

void fast_pay::on_lineEdit_4_textChanged(const QString &arg1)
{
    if(_parent)
        _parent->to_dish_title_other("", tr("应收")+ui->lineEdit_4->text());
}

void fast_pay::on_lineEdit_7_textChanged(const QString &arg1)
{
}

void fast_pay::setprintsize(blockIns &b, const QString &fontlen, const QString &content)
{
    //若是下划线的
    if(content.startsWith(n_func::gs_print_line)){b.append(backheader::printer->drawText("",content));return;}

    //否则遵循放大
    QString fontsize=fontlen;
    if(fontsize.count()!=2){
        fontsize="00";
    }
    b.append(backheader::printer->drawText(
                 QByteArray("1D21").append(fontsize),
                 content)
             );
    b.append(backheader::printer->drawText("1D2100"));
}

void fast_pay::setprintsize(QString &content, int len, const QString &flag)
{
    //    pin dan shu jia jin zhe

    if(flag=="pin")setprintsize(content, len, Qt::AlignLeft);
    if(flag=="dan")setprintsize(content, len, Qt::AlignLeft);
    if(flag=="shu")setprintsize(content, len, Qt::AlignLeft);
    if(flag=="jia")setprintsize(content, len, Qt::AlignLeft);
    if(flag=="jin")setprintsize(content, len, Qt::AlignLeft);
    if(flag=="zhe")setprintsize(content, len, Qt::AlignLeft);

}

void fast_pay::setprintsize(QString &content, int len, Qt::AlignmentFlag flag)
{
    switch(flag){
    case Qt::AlignLeft:
        content.append(QString(len-size_g_of_u(content), ' '));
        break;
    case Qt::AlignRight:
        content.prepend(QString(len-size_g_of_u(content), ' '));
        break;
    }
}

