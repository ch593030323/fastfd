#include "table_pay.h"
#include "ui_table_pay.h"
#include "vip_card.h"
#include "bank_card.h"
#include "discount_dispose.h"
#include "vip_recharge.h"
#include <QStandardItemModel>
#include "n_func.h"
#include <QSqlRecord>
#include <QDebug>
#include "lds_messagebox.h"
#include "lds_messagebox.h"
#include "res_special_dispose.h"
#include "free_view.h"
#include <QSqlQuery>
#include "other_pay_view.h"
#include"sqlquery.h"
#include "back/w_m_member_list.h"
#include <QSettings>
#include "table_order_2.h"
#include "w_scr_dish_restaurant_dialog.h"
#include "n_func.h"
#include <QSqlError>
#include "backheader.h"
#include "system_setting.h"

table_pay::table_pay(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::table_pay)
{
    ui->setupUi(this);
    init_control();
    init_data();
    kafujine = 0.00;
    shifouyouhuika = false;
    btn_t_f = false;
    count_dis = 0;
}

table_pay::~table_pay()
{
    delete ui;
}

void table_pay::init_control()
{
    ui->lineEdit->setEnabled(false);
    ui->lineEdit_2->setEnabled(false);
    ui->lineEdit_3->setEnabled(false);
    ui->lineEdit_4->setEnabled(false);
    ui->lineEdit_5->setEnabled(false);
    ui->lineEdit_6->setEnabled(false);
    ui->lineEdit_7->setEnabled(false);
    //ui->lineEdit_9->setFocus();

    QStringList list;
    list<<tr("行号")<<tr("付款方式")<<tr("实付现金")<<tr("付款金额")<<tr("凭证号");
    table_model.setHorizontalHeaderLabels(list);
    //    ui->tableView->horizontalHeader()->setStyleSheet("QHeaderView::section{background-color: qlineargradient(spread:reflect, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(115, 185, 185, 255), stop:1 rgba(63, 124, 124, 255));color:black;height:40px;border-radius:0px;font-size:14pt;}");
    //    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    //    ui->tableView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //    ui->tableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //    ui->tableView->verticalHeader()->setDefaultSectionSize(40);
    //    //ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置属性为只读//351
    //    ui->tableView->verticalHeader()->hide();
    ui->tableView->setModel(&table_model);
    //    ui->tableView->setColumnWidth(0,40);
    //    ui->tableView->setColumnWidth(1,90);
    //    ui->tableView->setColumnWidth(2,90);
    //    ui->tableView->setColumnWidth(3,125);
    //    ui->tableView->setColumnWidth(4,156);

    //    ui->tableView->horizontalHeader()->setResizeMode(0,QHeaderView::Fixed);
    //    ui->tableView->horizontalHeader()->setResizeMode(1,QHeaderView::Fixed);
    //    ui->tableView->horizontalHeader()->setResizeMode(2,QHeaderView::Fixed);
    //    ui->tableView->horizontalHeader()->setResizeMode(3,QHeaderView::Fixed);
    //    ui->tableView->horizontalHeader()->setResizeMode(4,QHeaderView::Fixed);

}

void table_pay::init_data()
{
    QSqlQueryModel person;
    person.setQuery(QString("select int_person,vch_memo from cey_u_table where ch_billno= '%1' ").arg(w_scr_dish_restaurant_dialog::table_danhao));
    QString table_renshu = person.record(0).value(0).toString();

    ui->lineEdit_12->setText(w_scr_dish_restaurant_dialog::table_danhao);
    ui->lineEdit_10->setText(table_renshu);
    ui->lineEdit_13->setText(person.record(0).value(1).toString());

    QSqlQueryModel model_l;
    model_l.setQuery("Select NOW()");
    QString time = model_l.record(0).value(0).toDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QDateTime pay_time = model_l.record(0).value(0).toDateTime();

    QStandardItemModel & model = table_order_2::modelBase;
    QSqlQueryModel model_query;
    QString discount;//是否折扣
    QString discount_num;//折扣值
    model_query.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='pay_auto_discount'"));
    discount = model_query.record(0).value(0).toString();
    model_query.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='pay_auto_discount_value'"));
    discount_num = model_query.record(0).value(0).toString();

    float xiaofei = 0.00;
    float zhekou = 0.00;
    float zengsong = 0.00;
    float moling = 0.00;
    float shuishou = 0.00;
    float yingshou = 0.00;
    float xianjin = 0.00;
    if(n_func::gs_operid == "0000")//特价，子套菜不能折扣//?时价菜能折扣?
    {
        if(discount == "1")//设置了账单自动折扣
        {
            for(int i = 0; i < model.rowCount(); i++)
            {
                if(model.item(i,9)->text() != "Y")
                {
                    xiaofei += model.item(i,3)->text().toFloat() * model.item(i,4)->text().toFloat() + model.item(i,3)->text().toFloat() * model.item(i,21)->text().toFloat();
                    shuishou += model.item(i,3)->text().toFloat() * model.item(i,4)->text().toFloat() * model.item(i,17)->text().toFloat() * 0.01 ;
                }

                if(model.item(i,9)->text() != "Y" &&  model.item(i,10)->text() != "Y" && model.item(i,18)->text() == "Y"&& model.item(i,8)->text() == "N")
                {
                    model.item(i,5)->setText(QString().sprintf("%0.2f",discount_num.toFloat()));
                    model.item(i,19)->setText(time);
                    float aa = 0.00;
                    aa = model.item(i,3)->text().toFloat() * model.item(i,4)->text().toFloat() * model.item(i,5)->text().toFloat() * 0.01 + model.item(i,3)->text().toFloat() * model.item(i,21)->text().toFloat();
                    model.item(i,6)->setText(QString().sprintf("%0.2f",aa));
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
            float quanxian_zhi = 0.00;
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
                        model.item(i,5)->setText(QString().sprintf("%0.2f",discount_num.toFloat()));
                        model.item(i,19)->setText(time);
                        float aa = 0.00;
                        aa = model.item(i,3)->text().toFloat() * model.item(i,4)->text().toFloat() * model.item(i,5)->text().toFloat() * 0.01 + model.item(i,3)->text().toFloat() * model.item(i,21)->text().toFloat();
                        model.item(i,6)->setText(QString().sprintf("%0.2f",aa));
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

    ui->lineEdit->setText(QString().sprintf("%0.2f",xiaofei));
    ui->lineEdit_2->setText(QString().sprintf("%0.2f",zhekou));
    ui->lineEdit_3->setText(QString().sprintf("%0.2f",zengsong));
    ui->lineEdit_4->setText(QString().sprintf("%0.2f",yingshou));
    ui->lineEdit_5->setText(QString().sprintf("%0.2f",moling));
    ui->lineEdit_6->setText(QString().sprintf("%0.2f",shuishou));
    //ui->lineEdit_9->setText(QString().sprintf("%0.2f",xianjin));
    //ui->label_14->setText(QString().sprintf("%0.2f",yingshou));

    //系统设置相关
    QSqlQueryModel canzhuo;
    canzhuo.setQuery(QString("select ch_areaNo from cey_bt_table where ch_tableno = '%1' ").arg(frontViewDialog_virtual::cur_desk_no));
    QString areano = canzhuo.record(0).value(0).toString();
    canzhuo.clear();
    canzhuo.setQuery(QString("select * from cey_bt_table_type where ch_typeno = '%1' ").arg(areano));
    int server_type = 0;
    server_type = canzhuo.record(0).value(3).toInt();
    QString fuwufei;
    if(server_type == 0)//不处理
    {
        fuwufei = "0.00";
    }
    else if(server_type == 1)//时间单位
    {
        int time_lenth = 0;
        time_lenth = table_order_2::cook_time.msecsTo(pay_time)/60000;

        float shouzhong = 0.00;
        float start = 0.00;
        float chao_num = 0.00;
        start = canzhuo.record(0).value(6).toFloat();

        int chaozhong = 0;
        if(canzhuo.record(0).value(5).toInt() >=  canzhuo.record(0).value(15).toInt())
        {
            chaozhong = time_lenth - canzhuo.record(0).value(5).toInt();
        }
        else
        {
            chaozhong = time_lenth - canzhuo.record(0).value(6).toInt();
        }

        if(chaozhong > 0)
        {
            int chao_time = canzhuo.record(0).value(7).toInt() + canzhuo.record(0).value(9).toInt();
            int cishu = 0;
            if(chaozhong % chao_time == 0)
            {
                cishu = chaozhong / chao_time;
            }
            else
            {
                cishu = chaozhong / chao_time + 1;
            }
            chao_num = cishu * canzhuo.record(0).value(8).toFloat();
        }
        shouzhong = start + chao_num;
        fuwufei = QString().sprintf("%0.2f",shouzhong);
    }
    else if(server_type == 2)//消费额服务费率
    {
        float server_num = 0.00;
        float zongjia = 0.00;
        if(canzhuo.record(0).value(4).toString() == "Y")//折前服务费
        {
            for(int i = 0; i < table_order_2::modelBase.rowCount(); i++)
            {
                if(table_order_2::modelBase.item(i,9)->text() != "Y")
                {
                    zongjia += table_order_2::modelBase.item(i,3)->text().toFloat() * table_order_2::modelBase.item(i,4)->text().toFloat();
                }
            }

            server_num = zongjia * canzhuo.record(0).value(11).toFloat() * 0.01;
        }
        else
        {
            for(int i = 0; i < table_order_2::modelBase.rowCount(); i++)
            {
                if(table_order_2::modelBase.item(i,9)->text() != "Y")
                {
                    zongjia += table_order_2::modelBase.item(i,3)->text().toFloat() * table_order_2::modelBase.item(i,4)->text().toFloat()* table_order_2::modelBase.item(i,5)->text().toFloat() * 0.01;
                }
            }

            server_num = zongjia * canzhuo.record(0).value(11).toFloat() * 0.01;
        }

        fuwufei = QString().sprintf("%0.2f",server_num);
    }
    else if(server_type == 3)//餐桌定额
    {
        fuwufei = canzhuo.record(0).value(10).toString();
    }
    else //菜品消费服务率
    {
        float server_num = 0.00;
        QSqlQueryModel cook;
        if(canzhuo.record(0).value(4).toString() == "Y")//折前服务费
        {
            for(int i = 0; i  < table_order_2::modelBase.rowCount(); i++)
            {
                if(table_order_2::modelBase.item(i,9)->text() != "Y")
                {
                    cook.clear();
                    cook.setQuery(QString("select ch_werviceflag,ch_lowflag from cey_bt_dish where ch_dishno = '%1' ").arg(table_order_2::modelBase.item(i,1)->text()));
                    if(cook.record(0).value(0).toString() == "Y" && cook.record(0).value(0).toString() == "N")//收服务费且不计入低消
                    {
                        server_num = table_order_2::modelBase.item(i,3)->text().toFloat() * table_order_2::modelBase.item(i,4)->text().toFloat() * canzhuo.record(0).value(11).toFloat() * 0.01;
                    }
                }
            }
        }
        else
        {
            for(int i = 0; i  < table_order_2::modelBase.rowCount(); i++)
            {
                if(table_order_2::modelBase.item(i,9)->text() != "Y")
                {
                    cook.clear();
                    cook.setQuery(QString("select ch_werviceflag,ch_lowflag from cey_bt_dish where ch_dishno = '%1' ").arg(table_order_2::modelBase.item(i,1)->text()));
                    if(cook.record(0).value(0).toString() == "Y" && cook.record(0).value(0).toString() == "N")//收服务费且不计入低消
                    {
                        server_num = table_order_2::modelBase.item(i,3)->text().toFloat() * table_order_2::modelBase.item(i,4)->text().toFloat() * table_order_2::modelBase.item(i,5)->text().toFloat() * 0.01 * canzhuo.record(0).value(11).toFloat() * 0.01;
                    }
                }
            }
        }

        fuwufei = QString().sprintf("%0.2f",server_num);
    }

    QString dixiao_type;
    dixiao_type = canzhuo.record(0).value(13).toString();
    float dixiao_num;
    if(dixiao_type == "1")//餐桌低消
    {
        dixiao_num = canzhuo.record(0).value(14).toFloat();
    }
    else//人均低消
    {
        QSqlQueryModel person;
        person.setQuery(QString("select int_person from cey_u_table where ch_billno= '%1' ").arg(w_scr_dish_restaurant_dialog::table_danhao));
        dixiao_num = person.record(0).value(0).toFloat() * canzhuo.record(0).value(14).toFloat();
    }

    QSqlQueryModel model_this;
    QString str;
    model_this.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='service_handwork' "));//手工处理服务费
    str = model_this.record(0).value(0).toString();
    if(str != "0")
    {
        fuwufei = "0.00";
    }


    //服务费计入低消费
    model_this.clear();
    model_this.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='low_service' "));
    QString dixiao = model_this.record(0).value(0).toString();
    if(dixiao != "0")
    {
        fuwufei = "0.00";
    }

    //低消
    if(dixiao_num < yingshou)
    {
        dixiao_num = 0.00;
    }
    else
    {
        dixiao_num = dixiao_num - yingshou;
    }

    ui->lineEdit_14->setText(fuwufei);
    ui->lineEdit_11->setText(QString().sprintf("%0.2f",dixiao_num));


    yingshou = yingshou + fuwufei.toFloat() + dixiao_num;
    ui->lineEdit_4->setText(QString().sprintf("%0.2f",yingshou));
    ui->lineEdit_9->setText(QString().sprintf("%0.2f",yingshou));
    ui->lineEdit_9->setFocus();
    ui->label_14->setText(QString().sprintf("%0.2f",yingshou));
    ui->lineEdit_7->setText("0.00");
    emit change();
}


void table_pay::on_pushButton_21_clicked()//取消
{
    if(table_model.rowCount() > 0)
    {
        if(0==lds_messagebox::warning(this, tr("消息提示"),tr("已有付款记录,是否确定退出?"),tr("确认"),tr("取消")))
        {
            emit change();
            this->close();
            btn_t_f = false;
        }
    }
    else
    {
        emit change();
        this->close();
        btn_t_f = false;
    }
}

void table_pay::on_pushButton_15_clicked()//折扣
{
    QSqlQueryModel model_l;
    model_l.setQuery("Select NOW()");
    QString time = model_l.record(0).value(0).toDateTime().toString("yyyy-MM-dd hh:mm:ss");

    ;
    if(table_model.rowCount() > 0)
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("已有付款记录,不可进行打折处理!"));
        
        return;
    }

    QStandardItemModel & model = table_order_2::modelBase;
    discount_dispose dispose(this);
    /*dispose.exec();*/backheader::static_dialogexec(dispose, tr("折扣处理"));

    float xiaofei = 0.00;
    xiaofei = ui->lineEdit->text().toFloat();
    float zhekou = 0.00;
    float zengsong = 0.00;
    zengsong = ui->lineEdit_3->text().toFloat();
    float moling = 0.00;
    moling = ui->lineEdit_5->text().toFloat();
    float shuishou = 0.00;
    shuishou = ui->lineEdit_6->text().toFloat();
    float yingshou = 0.00;
    yingshou = ui->lineEdit_4->text().toFloat();
    float xianjin = 0.00;
    xianjin = ui->lineEdit_9->text().toFloat();

    if(dispose.btn_t_f == true)
    {
        for(int i = 0; i < model.rowCount(); i++)
        {
            if(model.item(i,9)->text() != "Y" && model.item(i,10)->text() != "Y" && model.item(i,8)->text() == "N" && model.item(i,18)->text() == "Y")
            {
                model.item(i,5)->setText(QString().sprintf("%0.2f",dispose.zhekouzhi));
                model.item(i,19)->setText(time);
                float bb = 0.00;
                bb = model.item(i,3)->text().toFloat() * model.item(i,4)->text().toFloat() * dispose.zhekouzhi * 0.01;
                model.item(i,6)->setText(QString().sprintf("%0.2f",bb));

                float aa =0.00;
                aa = model.item(i,3)->text().toFloat() * model.item(i,4)->text().toFloat() * (100 - dispose.zhekouzhi) * 0.01;
                zhekou += aa;
            }
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

    ui->lineEdit->setText(QString().sprintf("%0.2f",xiaofei));
    ui->lineEdit_2->setText(QString().sprintf("%0.2f",zhekou));
    ui->lineEdit_3->setText(QString().sprintf("%0.2f",zengsong));
    ui->lineEdit_4->setText(QString().sprintf("%0.2f",yingshou));
    ui->lineEdit_5->setText(QString().sprintf("%0.2f",moling));
    ui->lineEdit_6->setText(QString().sprintf("%0.2f",shuishou));
    ui->lineEdit_9->setText(QString().sprintf("%0.2f",xianjin));
    ui->label_14->setText(QString().sprintf("%0.2f",yingshou));
    emit change();

}

void table_pay::on_pushButton_17_clicked()//会员卡
{
    ;
    if(ui->label_14->text().toFloat() == 0)
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("已足额付款无需继续付款!"));
        
        return;
    }

    if(table_model.rowCount() > 0)
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("已有其他付款，不允许再使用会员卡付款!"));
        
        return;
    }

    vip_card card(ui->label_14->text(),ui->lineEdit->text(),ui->lineEdit_2->text(),ui->lineEdit_3->text(),ui->lineEdit_4->text(),ui->lineEdit_5->text(),ui->lineEdit_6->text(),ui->lineEdit_9->text(), 0);
    /*card.exec();*/backheader::static_dialogexec(card, "");
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
                ui->lineEdit_9->setText(card._xj);
                ui->label_14->setText(card._num);
                shifouyouhuika = true;
                youhuikahao = card.fukuankahao;
                QSqlQueryModel model1;
                model1.setQuery("Select NOW()");
                timer = model1.record(0).value(0).toDateTime().toString("yyyy-MM-dd hh:mm:ss");
            }
            else
            {
                ui->lineEdit->setText(card._xf);
                ui->lineEdit_2->setText(card._zk);
                ui->lineEdit_3->setText(card._zs);
                ui->lineEdit_4->setText(card._ys);
                ui->lineEdit_5->setText(card._ml);
                ui->lineEdit_6->setText(card._ss);
                ui->lineEdit_9->setText(card._xj);
                ui->label_14->setText(tr("0.00"));
                ui->label_12->setText(card._ys);
                shifouyouhuika = false;
                youhuikahao = "";

                QSqlQuery model;
                model.prepare("select ch_paymodeno from cey_bt_paymode where vch_paymodename = :as_mode");
                model.bindValue(":as_mode",tr("充值卡"));
                model.exec();
                QString paytype;
                while(model.next())
                {
                    paytype = model.value(0).toString();
                }
                int row = table_model.rowCount();
                table_model.setRowCount(row + 1);
                table_model.setItem(row,0,new QStandardItem(QString::number(row+1))); //序号
                table_model.setItem(row,1,new QStandardItem(tr("充值卡")));
                table_model.setItem(row,2,new QStandardItem(card._ys));
                table_model.setItem(row,3,new QStandardItem(card._ys));
                table_model.setItem(row,4,new QStandardItem(card.fukuankahao));
                table_model.setItem(row,5,new QStandardItem(paytype));
                table_model.setItem(row,6,new QStandardItem(card.beizhu));
                table_model.setItem(row,7,new QStandardItem(card.time));
                ui->tableView->selectRow(row);

                ui->tableView->setColumnHidden(5,true);
                ui->tableView->setColumnHidden(6,true);
                ui->tableView->setColumnHidden(7,true);
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
                ui->lineEdit_9->setText(card._xj);
                ui->label_14->setText(card._num);
                shifouyouhuika = false;
                youhuikahao = card.fukuankahao;
            }
            else
            {
                ui->lineEdit->setText(card._xf);
                ui->lineEdit_2->setText(card._zk);
                ui->lineEdit_3->setText(card._zs);
                ui->lineEdit_4->setText(card._ys);
                ui->lineEdit_5->setText(card._ml);
                ui->lineEdit_6->setText(card._ss);
                ui->lineEdit_9->setText(card._xj);
                ui->label_14->setText(tr("0.00"));
                ui->label_12->setText(card._ys);
                shifouyouhuika = false;
                youhuikahao = "";
                QSqlQuery model;
                model.prepare("select ch_paymodeno from cey_bt_paymode where vch_paymodename = :as_mode");
                model.bindValue(":as_mode",tr("充值卡"));
                model.exec();
                QString paytype;
                while(model.next())
                {
                    paytype = model.value(0).toString();
                }
                int row = table_model.rowCount();
                table_model.setRowCount(row + 1);
                table_model.setItem(row,0,new QStandardItem(QString::number(row+1))); //序号
                table_model.setItem(row,1,new QStandardItem(tr("充值卡")));
                table_model.setItem(row,2,new QStandardItem(card._ys));
                table_model.setItem(row,3,new QStandardItem(card._ys));
                table_model.setItem(row,4,new QStandardItem(card.fukuankahao));
                table_model.setItem(row,5,new QStandardItem(paytype));
                table_model.setItem(row,6,new QStandardItem(card.beizhu));
                table_model.setItem(row,7,new QStandardItem(card.time));

                ui->tableView->selectRow(row);

                ui->tableView->setColumnHidden(5,true);
                ui->tableView->setColumnHidden(6,true);
                ui->tableView->setColumnHidden(7,true);
            }
        }
    }

}

void table_pay::on_pushButton_18_clicked()//银行卡
{
    float label_14 = 0.00;
    label_14 = ui->label_14->text().toFloat();
    ;
    if(ui->label_14->text().toFloat() == 0)
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("已足额付款无需继续付款!"));
        
        return;
    }
    QString timer;
    bank_card *bank = new bank_card("[03]银行卡",ui->label_14->text(), this);
    /*bank->exec();*/backheader::static_dialogexec(*bank, "");
    if(bank->btn_t_f == true)
    {
        kafujine = bank->shishoujine;
        fukuanpingzheng = bank->pingzhenghao;
        QSqlQueryModel model;
        model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='cash_payall' "));
        QString str = model.record(0).value(0).toString();
        if(str == "1")//默认现金付全款
        {
            if(ui->label_14->text().toFloat() <= bank->shishoujine)
            {
                ui->lineEdit_9->setText("0.00");
                float l_12 = 0.00;
                l_12 = ui->label_14->text().toFloat() + ui->label_12->text().toFloat();
                ui->label_12->setText(QString().sprintf("%0.2f",l_12));
                ui->label_14->setText("0.00");
            }
            else
            {
                float yue = 0.00;
                yue = ui->label_14->text().toFloat() - bank->shishoujine;
                ui->lineEdit_9->setText(QString().sprintf("%0.2f",yue));
                ui->label_12->setText(QString().sprintf("%0.2f",bank->shishoujine + ui->label_12->text().toFloat()));
                ui->label_14->setText(QString().sprintf("%0.2f",yue));
            }
        }
        else//不是现金付全款
        {
            if(ui->label_14->text().toFloat() <= bank->shishoujine)
            {
                float l_12 = 0.00;
                l_12 = ui->label_14->text().toFloat() + ui->label_12->text().toFloat();
                ui->label_12->setText(QString().sprintf("%0.2f",l_12));
                ui->label_14->setText("0.00");
                ui->lineEdit_9->setText("0.00");
                ui->lineEdit_7->setText("0.00");
            }
            else
            {
                float yue = 0.00;
                yue = ui->label_14->text().toFloat() - bank->shishoujine;
                ui->label_12->setText(QString().sprintf("%0.2f",bank->shishoujine + ui->label_12->text().toFloat()));
                ui->label_14->setText(QString().sprintf("%0.2f",yue));
                ui->lineEdit_9->setText("0.00");
                ui->lineEdit_7->setText("0.00");
            }
        }
        timer = bank->time;
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

    int row = table_model.rowCount();
    table_model.setRowCount(row + 1);
    float t_shifuxianjin = 0.00;
    float t_fukuanjie = 0.00;
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
    table_model.setItem(row,0,new QStandardItem(QString::number(row+1))); //序号
    table_model.setItem(row,1,new QStandardItem(tr("银行卡")));
    table_model.setItem(row,2,new QStandardItem(QString().sprintf("%0.2f",t_shifuxianjin)));
    table_model.setItem(row,3,new QStandardItem(QString().sprintf("%0.2f",t_fukuanjie)));
    table_model.setItem(row,4,new QStandardItem(fukuanpingzheng));
    table_model.setItem(row,5,new QStandardItem(paytype));
    table_model.setItem(row,6,new QStandardItem(""));
    table_model.setItem(row,7,new QStandardItem(timer));
    ui->tableView->selectRow(row);


    ui->tableView->setColumnHidden(5,true);
    ui->tableView->setColumnHidden(6,true);
    ui->tableView->setColumnHidden(7,true);
}

void table_pay::on_pushButton_19_clicked()//代金券
{
    float label_14 = 0.00;
    label_14 = ui->label_14->text().toFloat();
    ;
    if(ui->label_14->text().toFloat() == 0)
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("已足额付款无需继续付款!"));
        
        return;
    }
    QString timer;
    bank_card *bank = new bank_card("[04]代金券",ui->label_14->text(), this);
    /*bank->exec();*/backheader::static_dialogexec(*bank, "");
    if(bank->btn_t_f == true)
    {
        kafujine = bank->shishoujine;
        fukuanpingzheng = bank->pingzhenghao;
        QSqlQueryModel model;
        model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='cash_payall' "));
        QString str = model.record(0).value(0).toString();
        if(str == "1")//默认现金付全款
        {
            if(ui->label_14->text().toFloat() <= bank->shishoujine)
            {
                ui->lineEdit_9->setText("0.00");
                float l_12 = 0.00;
                l_12 = ui->label_14->text().toFloat() + ui->label_12->text().toFloat();
                ui->label_12->setText(QString().sprintf("%0.2f",l_12));
                ui->label_14->setText("0.00");
            }
            else
            {
                float yue = 0.00;
                yue = ui->label_14->text().toFloat() - bank->shishoujine;
                ui->lineEdit_9->setText(QString().sprintf("%0.2f",yue));
                ui->label_12->setText(QString().sprintf("%0.2f",bank->shishoujine + ui->label_12->text().toFloat()));
                ui->label_14->setText(QString().sprintf("%0.2f",yue));
            }
        }
        else//不是现金付全款
        {
            if(ui->label_14->text().toFloat() <= bank->shishoujine)
            {
                float l_12 = 0.00;
                l_12 = ui->label_14->text().toFloat() + ui->label_12->text().toFloat();
                ui->label_12->setText(QString().sprintf("%0.2f",l_12));
                ui->label_14->setText("0.00");
                ui->lineEdit_9->setText("0.00");
                ui->lineEdit_7->setText("0.00");
            }
            else
            {
                float yue = 0.00;
                yue = ui->label_14->text().toFloat() - bank->shishoujine;
                ui->label_12->setText(QString().sprintf("%0.2f",bank->shishoujine + ui->label_12->text().toFloat()));
                ui->label_14->setText(QString().sprintf("%0.2f",yue));
                ui->lineEdit_9->setText("0.00");
                ui->lineEdit_7->setText("0.00");
            }
        }
        timer = bank->time;
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

    int row = table_model.rowCount();
    table_model.setRowCount(row + 1);
    float t_shifuxianjin = 0.00;
    float t_fukuanjie = 0.00;
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

    table_model.setItem(row,0,new QStandardItem(QString::number(row+1))); //序号
    table_model.setItem(row,1,new QStandardItem(tr("代金券")));
    table_model.setItem(row,2,new QStandardItem(QString().sprintf("%0.2f",t_shifuxianjin)));
    table_model.setItem(row,3,new QStandardItem(QString().sprintf("%0.2f",t_fukuanjie)));
    table_model.setItem(row,4,new QStandardItem(fukuanpingzheng));
    table_model.setItem(row,5,new QStandardItem(paytype));
    table_model.setItem(row,6,new QStandardItem(""));
    table_model.setItem(row,7,new QStandardItem(timer));
    ui->tableView->selectRow(row);


    ui->tableView->setColumnHidden(5,true);
    ui->tableView->setColumnHidden(6,true);
    ui->tableView->setColumnHidden(7,true);
}

void table_pay::on_pushButton_20_clicked()//其他付款
{
    float label_14 = 0.00;
    label_14 = ui->label_14->text().toFloat();

    ;
    if(ui->label_14->text().toFloat() == 0)
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("已足额付款无需继续付款!"));
        
        return;
    }
    other_pay_view other_pay("1");
    /*other_pay.exec();*/backheader::static_dialogexec(other_pay, tr("其他付款"));

    if(other_pay.btn_t_f == true)
    {
        QString timer;
        QString str = tr("[") + other_pay.payfanshi[other_pay.curindex_id] + tr("]") + other_pay.payname[other_pay.curindex_id];
        bank_card *bank = new bank_card(str,ui->label_14->text(),this);
        /*bank->exec();*/backheader::static_dialogexec(*bank, "");
        if(bank->btn_t_f == true)
        {
            kafujine = bank->shishoujine;
            fukuanpingzheng = bank->pingzhenghao;
            QSqlQueryModel model;
            model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='cash_payall' "));
            QString str = model.record(0).value(0).toString();
            if(str == "1")//默认现金付全款
            {
                if(ui->label_14->text().toFloat() <= bank->shishoujine)
                {
                    ui->lineEdit_9->setText("0.00");
                    float l_12 = 0.00;
                    l_12 = ui->label_14->text().toFloat() + ui->label_12->text().toFloat();
                    ui->label_12->setText(QString().sprintf("%0.2f",l_12));
                    ui->label_14->setText("0.00");
                }
                else
                {
                    float yue = 0.00;
                    yue = ui->label_14->text().toFloat() - bank->shishoujine;
                    ui->lineEdit_9->setText(QString().sprintf("%0.2f",yue));
                    ui->label_12->setText(QString().sprintf("%0.2f",bank->shishoujine + ui->label_12->text().toFloat()));
                    ui->label_14->setText(QString().sprintf("%0.2f",yue));
                }
            }
            else//不是现金付全款
            {
                if(ui->label_14->text().toFloat() <= bank->shishoujine)
                {
                    float l_12 = 0.00;
                    l_12 = ui->label_14->text().toFloat() + ui->label_12->text().toFloat();
                    ui->label_12->setText(QString().sprintf("%0.2f",l_12));
                    ui->label_14->setText("0.00");
                    ui->lineEdit_9->setText("0.00");
                    ui->lineEdit_7->setText("0.00");
                }
                else
                {
                    float yue = 0.00;
                    yue = ui->label_14->text().toFloat() - bank->shishoujine;
                    ui->label_12->setText(QString().sprintf("%0.2f",bank->shishoujine + ui->label_12->text().toFloat()));
                    ui->label_14->setText(QString().sprintf("%0.2f",yue));
                    ui->lineEdit_9->setText("0.00");
                    ui->lineEdit_7->setText("0.00");
                }
            }
            timer = bank->time;
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

        int row = table_model.rowCount();
        table_model.setRowCount(row + 1);
        float t_shifuxianjin = 0.00;
        float t_fukuanjie = 0.00;
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

        table_model.setItem(row,0,new QStandardItem(QString::number(row+1))); //序号
        table_model.setItem(row,1,new QStandardItem(other_pay.payname[other_pay.curindex_id]));
        table_model.setItem(row,2,new QStandardItem(QString().sprintf("%0.2f",t_shifuxianjin)));
        table_model.setItem(row,3,new QStandardItem(QString().sprintf("%0.2f",t_fukuanjie)));
        table_model.setItem(row,4,new QStandardItem(fukuanpingzheng));
        table_model.setItem(row,5,new QStandardItem(paytype));
        table_model.setItem(row,6,new QStandardItem(""));
        table_model.setItem(row,7,new QStandardItem(timer));
        ui->tableView->selectRow(row);


        ui->tableView->setColumnHidden(5,true);
        ui->tableView->setColumnHidden(6,true);
        ui->tableView->setColumnHidden(7,true);
    }
}

void table_pay::on_pushButton_22_clicked()//特殊处理
{
    ;
    if(ui->label_14->text().toFloat() == 0)
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("已足额付款无需继续付款!"));
        
        return;
    }

    if(table_model.rowCount() > 0)
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("已有付款记录,不可进行特殊处理!"));
        
        return;
    }

    res_special_dispose dispose(this);
    /*dispose.exec();*/backheader::static_dialogexec(dispose, tr("特殊处理"));
    if(dispose.btn_t_f == true)
    {
        if(dispose.curindex_id == 0)//免低消费
        {
            if(n_func::gs_operid != "0000")
            {
                QSqlQueryModel cey_sys;
                cey_sys.setQuery(QString("select count(*)  from cey_sys_oper_purview  where ch_purviewno = '%1' and vch_operID ='%2' ").arg("0220").arg(n_func::gs_operid));
                QString str = cey_sys.record(0).value(0).toString();
                if(str.toInt() == 0)
                {
                    lds_messagebox::warning(this, tr("消息提示"),tr("sorry,该操作员没有免服务费权限!"));
                    
                    return;
                }
                else
                {
                    ui->lineEdit_11->setText(tr("0.00"));
                }
            }
            else
            {
                ui->lineEdit_11->setText(tr("0.00"));
            }
        }
        else if(dispose.curindex_id == 1)//免服务费
        {
            if(n_func::gs_operid != "0000")
            {
                QSqlQueryModel cey_sys;
                cey_sys.setQuery(QString("select count(*)  from cey_sys_oper_purview  where ch_purviewno = '%1' and vch_operID ='%2' ").arg("0219").arg(n_func::gs_operid));
                QString str = cey_sys.record(0).value(0).toString();
                if(str.toInt() == 0)
                {
                    lds_messagebox::warning(this, tr("消息提示"),tr("sorry,该操作员没有免服务费权限!"));
                    
                    return;
                }
                else
                {
                    ui->lineEdit_14->setText(tr("0.00"));
                }
            }
            else
            {
                ui->lineEdit_14->setText(tr("0.00"));
            }
        }
        else if(dispose.curindex_id == 2)//免单
        {
            free_view *freeview = new free_view(tr("免单操作"),ui->label_14->text(), this);
            /*freeview->exec();*/backheader::static_dialogexec(*freeview, "");
            if(freeview->btn_t_f == true)
            {
                if(freeview->oper_id != "0000")
                {
                    QSqlQueryModel cey_sys;
                    cey_sys.setQuery(QString("select ch_free_flag from cey_sys_operator where vch_operID = '%1' ").arg(freeview->oper_id));
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
                        ui->label_14->setText(tr("0.00"));
                        ui->lineEdit_9->setText(tr("0.00"));
                        QSqlQuery model;
                        model.prepare("select ch_paymodeno from cey_bt_paymode where vch_paymodename = :as_mode");
                        model.bindValue(":as_mode",tr("免单金额"));
                        model.exec();
                        QString paytype;
                        while(model.next())
                        {
                            paytype = model.value(0).toString();
                        }
                        int row = table_model.rowCount();
                        table_model.setItem(row,0,new QStandardItem(QString::number(row+1))); //序号
                        table_model.setItem(row,1,new QStandardItem(tr("免单")));
                        table_model.setItem(row,2,new QStandardItem(QString().sprintf("%0.2f",ui->label_12->text().toFloat())));
                        table_model.setItem(row,3,new QStandardItem(QString().sprintf("%0.2f",ui->label_12->text().toFloat())));
                        table_model.setItem(row,4,new QStandardItem(""));
                        table_model.setItem(row,5,new QStandardItem(paytype));
                        table_model.setItem(row,6,new QStandardItem(freeview->beizhu));
                        table_model.setItem(row,7,new QStandardItem(freeview->time));
                        ui->tableView->selectRow(row);

                        ui->tableView->setColumnHidden(5,true);
                        ui->tableView->setColumnHidden(6,true);
                        ui->tableView->setColumnHidden(7,true);
                    }
                }
                else//免单为0000
                {
                    ui->label_12->setText(ui->label_14->text());
                    ui->label_14->setText(tr("0.00"));
                    ui->lineEdit_9->setText(tr("0.00"));
                    QSqlQuery model;
                    model.prepare("select ch_paymodeno from cey_bt_paymode where vch_paymodename = :as_mode");
                    model.bindValue(":as_mode",tr("免单金额"));
                    model.exec();
                    QString paytype;
                    while(model.next())
                    {
                        paytype = model.value(0).toString();
                    }
                    int row = table_model.rowCount();
                    table_model.setItem(row,0,new QStandardItem(QString::number(row+1))); //序号
                    table_model.setItem(row,1,new QStandardItem(tr("免单")));
                    table_model.setItem(row,2,new QStandardItem(QString().sprintf("%0.2f",ui->label_12->text().toFloat())));
                    table_model.setItem(row,3,new QStandardItem(QString().sprintf("%0.2f",ui->label_12->text().toFloat())));
                    table_model.setItem(row,4,new QStandardItem(""));
                    table_model.setItem(row,5,new QStandardItem(paytype));
                    table_model.setItem(row,6,new QStandardItem(freeview->beizhu));
                    table_model.setItem(row,7,new QStandardItem(freeview->time));
                    ui->tableView->selectRow(row);


                    ui->tableView->setColumnHidden(5,true);
                    ui->tableView->setColumnHidden(6,true);
                    ui->tableView->setColumnHidden(7,true);
                }
            }
            else
            {
                return;
            }
        }
        else//抹零
        {
            free_view *freeview = new free_view(tr("手工抹零"),ui->label_14->text(), this);
            /*freeview->exec();*/backheader::static_dialogexec(*freeview, "");
            if(freeview->btn_t_f == true)
            {
                if(freeview->oper_id != "0000")
                {
                    QSqlQueryModel cey_sys;
                    cey_sys.setQuery(QString("select num_free from cey_sys_operator where vch_operID = '%1' ").arg(freeview->oper_id));
                    float aa = 0.00;
                    aa = cey_sys.record(0).value(0).toFloat();
                    if(freeview->line2_num > aa)
                    {
                        lds_messagebox::warning(this, tr("消息提示"),tr("sorry,您的抹零金额超出了您的最大抹零金额!"));
                        
                        return;
                    }
                    else
                    {
                        if(freeview->line2_num >= ui->label_14->text().toFloat())//全部抹零
                        {
                            //添加table_model
                            float bb;
                            bb = 0.00;
                            bb = ui->lineEdit_5->text().toFloat() + freeview->line2_num;
                            ui->lineEdit_5->setText(QString().sprintf("%0.2f",bb));
                            ui->label_12->setText(ui->label_14->text());
                            ui->label_14->setText(tr("0.00"));
                            if(ui->lineEdit_9->text().toFloat() != 0)
                            {
                                ui->lineEdit_9->setText(tr("0.00"));
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

                            int row = table_model.rowCount();
                            table_model.setItem(row,0,new QStandardItem(QString::number(row+1))); //序号
                            table_model.setItem(row,1,new QStandardItem(tr("手工抹零")));
                            table_model.setItem(row,2,new QStandardItem(QString().sprintf("%0.2f",ui->label_12->text().toFloat())));
                            table_model.setItem(row,3,new QStandardItem(QString().sprintf("%0.2f",ui->label_12->text().toFloat())));
                            table_model.setItem(row,4,new QStandardItem(""));
                            table_model.setItem(row,5,new QStandardItem(paytype));
                            table_model.setItem(row,6,new QStandardItem(freeview->beizhu));
                            table_model.setItem(row,7,new QStandardItem(freeview->time));
                            ui->tableView->selectRow(row);


                            ui->tableView->setColumnHidden(5,true);
                            ui->tableView->setColumnHidden(6,true);
                            ui->tableView->setColumnHidden(7,true);
                        }
                        else//部分抹零
                        {
                            //添加table_model
                            float aa = 0.00;
                            aa = ui->label_14->text().toFloat() - freeview->line2_num;
                            ui->label_12->setText(QString().sprintf("%0.2f",freeview->line2_num));
                            ui->label_14->setText(QString().sprintf("%0.2f",aa));
                            if(ui->lineEdit_9->text().toFloat() != 0)
                            {
                                ui->lineEdit_9->setText(QString().sprintf("%0.2f",aa));
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
                            bb = 0.00;
                            bb = ui->lineEdit_5->text().toFloat() + freeview->line2_num;
                            ui->lineEdit_5->setText(QString().sprintf("%0.2f",bb));
                            int row = table_model.rowCount();
                            table_model.setItem(row,0,new QStandardItem(QString::number(row+1))); //序号
                            table_model.setItem(row,1,new QStandardItem(tr("手工抹零")));
                            table_model.setItem(row,2,new QStandardItem(QString().sprintf("%0.2f",freeview->line2_num)));
                            table_model.setItem(row,3,new QStandardItem(QString().sprintf("%0.2f",freeview->line2_num)));
                            table_model.setItem(row,4,new QStandardItem(""));
                            table_model.setItem(row,5,new QStandardItem(paytype));
                            table_model.setItem(row,6,new QStandardItem(freeview->beizhu));
                            table_model.setItem(row,7,new QStandardItem(freeview->time));
                            ui->tableView->selectRow(row);


                        }
                        ui->tableView->setColumnHidden(5,true);
                        ui->tableView->setColumnHidden(6,true);
                        ui->tableView->setColumnHidden(7,true);
                    }
                }
                else//免单为0000
                {
                    if(freeview->line2_num >= ui->label_14->text().toFloat())//全部抹零
                    {
                        //添加table_model
                        ui->label_12->setText(ui->label_14->text());
                        ui->label_14->setText(tr("0.00"));
                        if(ui->lineEdit_9->text().toFloat() != 0)
                        {
                            ui->lineEdit_9->setText(tr("0.00"));
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

                        int row = table_model.rowCount();
                        table_model.setItem(row,0,new QStandardItem(QString::number(row+1))); //序号
                        table_model.setItem(row,1,new QStandardItem(tr("手工抹零")));
                        table_model.setItem(row,2,new QStandardItem(QString().sprintf("%0.2f",ui->label_12->text().toFloat())));
                        table_model.setItem(row,3,new QStandardItem(QString().sprintf("%0.2f",ui->label_12->text().toFloat())));
                        table_model.setItem(row,4,new QStandardItem(""));
                        table_model.setItem(row,5,new QStandardItem(paytype));
                        table_model.setItem(row,6,new QStandardItem(freeview->beizhu));
                        table_model.setItem(row,7,new QStandardItem(freeview->time));
                        ui->tableView->selectRow(row);


                        ui->tableView->setColumnHidden(5,true);
                        ui->tableView->setColumnHidden(6,true);
                        ui->tableView->setColumnHidden(7,true);
                    }
                    else//部分抹零
                    {
                        //添加table_model
                        float aa = 0.00;
                        aa = ui->label_14->text().toFloat() - freeview->line2_num;
                        ui->label_12->setText(QString().sprintf("%0.2f",freeview->line2_num));
                        ui->label_14->setText(QString().sprintf("%0.2f",aa));
                        if(ui->lineEdit_9->text().toFloat() != 0)
                        {
                            ui->lineEdit_9->setText(QString().sprintf("%0.2f",aa));
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

                        int row = table_model.rowCount();
                        table_model.setItem(row,0,new QStandardItem(QString::number(row+1))); //序号
                        table_model.setItem(row,1,new QStandardItem(tr("手工抹零")));
                        table_model.setItem(row,2,new QStandardItem(QString().sprintf("%0.2f",freeview->line2_num)));
                        table_model.setItem(row,3,new QStandardItem(QString().sprintf("%0.2f",freeview->line2_num)));
                        table_model.setItem(row,4,new QStandardItem(""));
                        table_model.setItem(row,5,new QStandardItem(paytype));
                        table_model.setItem(row,6,new QStandardItem(freeview->beizhu));
                        table_model.setItem(row,7,new QStandardItem(freeview->time));
                        ui->tableView->selectRow(row);


                        ui->tableView->setColumnHidden(5,true);
                        ui->tableView->setColumnHidden(6,true);
                        ui->tableView->setColumnHidden(7,true);
                    }
                }
            }
            else
            {
                return;
            }
        }
    }
}

void table_pay::on_pushButton_23_clicked()//会员充值
{
    w_m_member_list::statictorecharge(this, 0, 0,"");
}

void table_pay::on_pushButton_clicked()//7
{
    count_dis++;
    ui->lineEdit_9->insert("7");
}

void table_pay::on_pushButton_2_clicked()//8
{
    count_dis++;
    ui->lineEdit_9->insert("8");
}

void table_pay::on_pushButton_3_clicked()//9
{
    count_dis++;
    ui->lineEdit_9->insert("9");
}

void table_pay::on_pushButton_4_clicked()//50
{
    count_dis++;
    ui->lineEdit_9->clear();
    ui->lineEdit_9->insert(tr("50"));
}

void table_pay::on_pushButton_6_clicked()//4
{
    count_dis++;
    ui->lineEdit_9->insert("4");
}

void table_pay::on_pushButton_8_clicked()//5
{
    count_dis++;
    ui->lineEdit_9->insert("5");
}

void table_pay::on_pushButton_5_clicked()//6
{
    count_dis++;
    ui->lineEdit_9->insert("6");
}

void table_pay::on_pushButton_7_clicked()//100
{
    count_dis++;
    ui->lineEdit_9->clear();
    ui->lineEdit_9->insert(tr("100"));
}

void table_pay::on_pushButton_10_clicked()//1
{
    count_dis++;
    ui->lineEdit_9->insert("1");
}

void table_pay::on_pushButton_12_clicked()//2
{
    count_dis++;
    ui->lineEdit_9->insert("2");
}

void table_pay::on_pushButton_9_clicked()//3
{
    count_dis++;
    ui->lineEdit_9->insert("3");
}

void table_pay::on_pushButton_11_clicked()//d
{
    ui->lineEdit_9->backspace();
}

void table_pay::on_pushButton_14_clicked()//0
{
    count_dis++;
    ui->lineEdit_9->insert("0");
}

void table_pay::on_pushButton_16_clicked()//.
{
    count_dis++;
    ui->lineEdit_9->insert(tr("."));
}

void table_pay::on_pushButton_13_clicked()//买单
{
    if(table_order_2::modelBase.rowCount() == 0)
    {
        return;
    }

    if(ui->lineEdit_8->text().trimmed().isEmpty())
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("台号不能为空,请核实!"));
        
        return;
    }

    if(ui->lineEdit_9->text().toFloat() + ui->label_12->text().toFloat() < ui->lineEdit_4->text().toFloat())
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("对不起,您的付款金额不够!"));
        
        return;
    }

    if(ui->label_14->text().toFloat() > 0 && ui->lineEdit_7->text().toFloat() >= 100)
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("您输入的金额已超出最大找零金额100元,请核实!"));
        
        return;
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
    QString currentDate = "Q" + ldt_datetime.toString("yyMMdd");
    QString guqingdate = ldt_datetime.toString("yyyy-MM-dd");

    //    m_billno.clear();
    //    m_billno.setQuery(QString("select Max(ch_billno) from cey_u_master where Left(ch_billno,7) = '%1' ").arg(currentDate));
    //    danhao = m_billno.record(0).value(0).toString().trimmed();
    //    if(danhao.trimmed().isEmpty())
    //    {
    //        danhao = currentDate + QString().sprintf("%05d",1);
    //        bendandanhao = currentDate + QString().sprintf("%05d",1);
    //    }
    //    else
    //    {
    //        QString str = danhao.right(danhao.length() - 7);
    //        int aa = str.toInt() + 1;
    //        danhao = currentDate + QString().sprintf("%05d",aa);
    //        bendandanhao = currentDate + QString().sprintf("%05d",aa);
    //    }
    danhao = w_scr_dish_restaurant_dialog::table_danhao;
    bendandanhao = w_scr_dish_restaurant_dialog::table_danhao;
    //    int a = 0;
    //    a = danhao.right(4).toInt()% 100;
    //    if(a < 10)
    //    {
    //        liushuihao_p = "0" + QString::number(a);
    //    }
    //    else
    //    {
    //        liushuihao_p = QString::number(a);
    //    }
    liushuihao_p=system_setting::get_max_serialno_add1(danhao);
    m_billno.clear();
    QString liushi = "P" + ldt_datetime.toString("yyMMdd");
    m_billno.setQuery(QString("select Max(ch_payno) from cey_u_checkout_master where Left(ch_payno,7) = '%1' ").arg(liushi));
    liushuihao = m_billno.record(0).value(0).toString().trimmed();
    if(liushuihao.isEmpty())
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

    query.prepare(QString("Insert into cey_u_service_low(ch_payno , ch_type , num_amount , vch_operID , dt_operdate , ch_state , vch_memo)values(:liushui,:dixiao,:num,:oper,:time,:state,null)"));
    query.bindValue(":liushui",liushuihao);
    query.bindValue(":dixiao","2");
    query.bindValue(":num",ui->lineEdit_11->text());
    query.bindValue(":oper",n_func::gs_operid);
    query.bindValue(":time",time);
    query.bindValue(":state","0");
    if(query.exec())
    {
    }
    else
    {
        QSqlDatabase::database().rollback();
        qDebug() << query.lastError();  lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_u_service_low低消失败!"));
        
        return;
    }

    query.prepare(QString("Insert into cey_u_service_low(ch_payno , ch_type , num_amount , vch_operID , dt_operdate , ch_state , vch_memo)values(:liushui,:dixiao,:num,:oper,:time,:state,null)"));
    query.bindValue(":liushui",liushuihao);
    query.bindValue(":dixiao","1");
    query.bindValue(":num",ui->lineEdit_14->text());
    query.bindValue(":oper",n_func::gs_operid);
    query.bindValue(":time",time);
    query.bindValue(":state","1");
    if(query.exec())
    {
    }
    else
    {
        QSqlDatabase::database().rollback();
        qDebug() << query.lastError();  lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_u_service_low服务费失败!"));
        
        return;
    }


    //    if(query.exec(QString("insert into cey_u_master( ch_billno , ch_state , vch_operID , dt_operdate)values('%1','%2','%3','%4')")
    //                  .arg(danhao).arg("1").arg(n_func::gs_operid).arg(time)))
    //    {

    //    }
    //    else
    //    {
    //        QSqlDatabase::database().rollback();
    //                   qDebug() << query.lastError();  lds_messagebox::warning(this, tr("消息提示"),tr("插入into cey_u_master失败!"));
    //
    //        return;
    //    }

    //付款明细
    if(query.exec(QString("insert into cey_u_checkout_master( ch_payno , ch_billno , ch_tableno , num_cost , num_discount , num_present , num_service , num_lowcost , num_blotout , num_run , vch_run_operID , ch_state , vch_operID , dt_operdate , vch_operID2 , dt_operdate2 ,num_rate) values('%1','%2','%3','%4','%5','%6','%7','%8','%9','%10',null,'%11','%12','%13',null,null,'%14')")
                  .arg(liushuihao).arg(danhao).arg(zhuohao).arg(ui->lineEdit->text()).arg(ui->lineEdit_2->text()).arg(ui->lineEdit_3->text())
                  .arg("0").arg("0").arg(ui->lineEdit_5->text()).arg("0").arg("Y").arg(n_func::gs_operid).arg(time).arg(ui->lineEdit_6->text())))
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        qDebug() << query.lastError();  lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_u_checkout_master失败!"));
        
        return;
    }

    //    if(query.exec(QString("insert into cey_u_table( ch_billno , ch_tableno , int_person , vch_memo , ch_state , ch_payno , vch_operID , dt_operdate , ch_order_state , num_cost)values('%1','%2','%3',null,'%4','%5','%6','%7','%8','%9')")
    //                  .arg(danhao).arg(zhuohao).arg("1").arg("1").arg(liushuihao).arg(n_func::gs_operid).arg(time).arg("4").arg(ui->lineEdit->text())))
    //    {

    //    }
    //    else
    //    {
    //        QSqlDatabase::database().rollback();
    //                   qDebug() << query.lastError();  lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_u_table失败!"));
    //
    //        return;
    //    }

    //============================================================================================
    //现金付款
    if(ui->label_14->text().toFloat() != 0)
    {
        float aa = 0.00;
        aa = ui->lineEdit_9->text().toFloat() - ui->lineEdit_7->text().toFloat();qDebug() << ui->lineEdit_7->text() << aa;
        // qDebug() << QString("insert into cey_u_checkout_detail(ch_payno , ch_paymodeno , num_payamount , num_change , num_realamount , num_face ,vch_operID , dt_operdate ,num_card_remain,vch_voucherno, num_face_Num )values('%1','%2','%3','%4','%5','%6','%7','%8','%9','null','%10')")
        // .arg(liushuihao).arg("00").arg(ui->lineEdit_9->text()).arg(ui->lineEdit_7->text()).arg(QString().sprintf("%0.2f",aa)).arg("0.0")
        // .arg(n_func::gs_operid).arg(time).arg("0.00").arg("0.0");
        if(query.exec(QString("insert into cey_u_checkout_detail(ch_payno , ch_paymodeno , num_payamount , num_change , num_realamount , num_face ,vch_operID , dt_operdate ,num_card_remain,vch_voucherno, num_face_Num )values('%1','%2','%3','%4','%5','%6','%7','%8','%9','null','%10')")
                      .arg(liushuihao).arg("00").arg(ui->lineEdit_9->text()).arg(ui->lineEdit_7->text()).arg(QString().sprintf("%0.2f",aa)).arg("0.0")
                      .arg(n_func::gs_operid).arg(time).arg("0.00").arg("0.0")))
        {

        }
        else
        {
            QSqlDatabase::database().rollback();
            lds_messagebox::warning(this, tr("消息提示"),tr("cey_u_checkout_detail失败!"));
            
            return;
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
        if(query.exec(QString("insert into cey_u_checkout_detail(ch_payno , ch_paymodeno , num_payamount , num_change , num_realamount , num_face ,vch_operID , dt_operdate ,num_card_remain,vch_voucherno, num_face_Num )values('%1','%2','%3','%4','%5','%6','%7','%8','%9','null','%10')")
                      .arg(liushuihao).arg(paytype).arg(ui->lineEdit_5->text()).arg("0.0").arg(ui->lineEdit_5->text()).arg("0.0")
                      .arg(n_func::gs_operid).arg(time).arg("0.00").arg("0.0")))
        {

        }
        else
        {
            QSqlDatabase::database().rollback();
            lds_messagebox::warning(this, tr("消息提示"),tr("cey_u_checkout_detail失败!"));
            
            return;
        }
    }

    if(shifouyouhuika == true)
    {
        //添加优惠卡付款
        float jinfen = 0.00;
        QString fen;
        QSqlQueryModel vip;
        vip.setQuery(QString("select ch_typeno from t_m_member where vch_memberno = '%1' ").arg(youhuikahao));
        QString str = vip.record(0).value(0).toString();//得到类型编号
        vip.clear();
        vip.setQuery(QString("select * from t_m_member_type where ch_typeno = '%1' ").arg(str));

        jinfen = ui->lineEdit->text().toFloat() / vip.record(0).value(9).toFloat();
        if(vip.record(8).value(0).toString() == "0")
        {
            fen = QString::number(int(jinfen));
        }
        else
        {
            fen = QString().sprintf("%0.2f",jinfen);
        }

        if(query.exec(QString("insert into t_m_point(vch_memberno ,ch_type, ch_payno , num_point , vch_memo , vch_operID , dt_operdate)values ('%1','%2','%3','%4',null,'%5','%6')")
                      .arg(youhuikahao).arg("3").arg(liushuihao).arg(fen).arg(n_func::gs_operid).arg(timer)))
        {

        }
        else
        {
            QSqlDatabase::database().rollback();
            lds_messagebox::warning(this, tr("消息提示"),tr("t_m_point失败!"));
            
            return;
        }

        vip.clear();
        vip.setQuery(QString("select * from t_m_curamount where vch_memberno = '' ").arg(youhuikahao));
        float aa= 0.00;
        aa = vip.record(0).value(3).toFloat() + jinfen;

        if(query.exec(QString("update t_m_curamount set vch_operID='%1' , dt_operdate='%2' , num_point ='%3' where vch_memberno ='%4'")
                      .arg(n_func::gs_operid).arg(timer).arg(QString().sprintf("%0.2f",aa)).arg(youhuikahao)))
        {

        }
        else
        {
            QSqlDatabase::database().rollback();
            lds_messagebox::warning(this, tr("消息提示"),tr("t_m_curamount失败!"));
            
            return;
        }
    }

    for(int i = 0; i < table_model.rowCount(); i++)
    {
        QString pingzheng = table_model.item(i,4)->text();
        if(pingzheng.trimmed().isEmpty())
        {
            pingzheng = "null";
        }

        if(table_model.item(i,5)->text().trimmed() == "02")//会员卡付款
        {
            //积分
            QString jifen;
            QSqlQueryModel vip;
            vip.setQuery(QString("select ch_typeno from t_m_member where vch_memberno = '%1' ").arg(table_model.item(i,4)->text()));
            QString str = vip.record(0).value(0).toString();//得到类型编号
            vip.clear();
            vip.setQuery(QString("select * from t_m_member_type where ch_typeno = '%1' ").arg(str));
            QString shijijifen;
            float bencijinfen = 0.00;
            if(vip.record(0).value(7).toString() == "Y")//可以积分
            {
                float yuanjiyifen = 0.00;
                yuanjiyifen = vip.record(0).value(9).toFloat();
                bencijinfen = table_model.item(i,3)->text().toFloat() / yuanjiyifen;
                if(vip.record(0).value(8).toString()  == "0")//积分取整数
                {
                    QString fen = QString::number(int(bencijinfen));
                    shijijifen = QString::number(int(bencijinfen));
                    if(query.exec(QString("insert into t_m_point(vch_memberno ,ch_type, ch_payno , num_point , vch_memo , vch_operID , dt_operdate)values ('%1','%2','%3','%4',null,'%5','%6')")
                                  .arg(table_model.item(i,4)->text()).arg("3").arg(liushuihao).arg(fen).arg(n_func::gs_operid).arg(table_model.item(i,7)->text())))
                    {

                    }
                    else
                    {
                        QSqlDatabase::database().rollback();
                        lds_messagebox::warning(this, tr("消息提示"),tr("t_m_point失败!"));
                        
                        return;
                    }
                }
                else//小数
                {
                    QString fen = QString().sprintf("%0.2f",bencijinfen);
                    shijijifen = QString().sprintf("%0.2f",bencijinfen);
                    if(query.exec(QString("insert into t_m_point(vch_memberno ,ch_type, ch_payno , num_point , vch_memo , vch_operID , dt_operdate)values ('%1','%2','%3','%4',null,'%5','%6')")
                                  .arg(table_model.item(i,4)->text()).arg("3").arg(liushuihao).arg(fen).arg(n_func::gs_operid).arg(table_model.item(i,7)->text())))
                    {

                    }
                    else
                    {
                        QSqlDatabase::database().rollback();
                        lds_messagebox::warning(this, tr("消息提示"),tr("t_m_point失败!"));
                        
                        return;
                    }
                }
            }
            else//不积分
            {
                bencijinfen = 0.00;
                shijijifen = tr("0.00");
            }

            if(query.exec(QString("insert into t_m_pay(vch_memberno , ch_payno , ch_paymodeno , num_pay , vch_operID , dt_operdate , int_checkflow) values ('%1','%2','%3','%4','%5','%6','%7')")
                          .arg(table_model.item(i,4)->text()).arg(liushuihao).arg(table_model.item(i,5)->text()).arg(table_model.item(i,3)->text()).arg(n_func::gs_operid).arg(table_model.item(i,7)->text()).arg("0")))
            {

            }
            else
            {
                QSqlDatabase::database().rollback();
                lds_messagebox::warning(this, tr("消息提示"),tr("t_m_pay失败!"));
                
                return;
            }

            QSqlQueryModel vip_model;
            vip_model.setQuery(QString("select * from t_m_curamount where vch_memberno = '%1' ").arg(table_model.item(i,4)->text()));
            if(vip_model.record(0).value(1).toFloat() < table_model.item(i,3)->text().toFloat())
            {
                lds_messagebox::warning(this, tr("消息提示"),tr("会员卡余额已不够本次支付金额,请核实"));
                
                return;
            }
            else
            {
                float aa = 0.00;
                aa = vip_model.record(0).value(1).toFloat() - table_model.item(i,3)->text().toFloat();
                huyuankayue = QString().sprintf("%0.2f",aa);
                float bb = 0.00;
                bb = vip_model.record(0).value(3).toFloat() + shijijifen.toFloat();

                if(query.exec(QString("update t_m_curamount set num_amount = '%1' , vch_operID='%2' , dt_operdate='%3' , num_point ='%4' where vch_memberno ='%5'")
                              .arg(QString().sprintf("%0.2f",aa)).arg(n_func::gs_operid).arg(table_model.item(i,7)->text()).arg(QString().sprintf("%0.2f",bb))
                              .arg(table_model.item(i,4)->text())))
                {

                }
                else
                {
                    QSqlDatabase::database().rollback();
                    lds_messagebox::warning(this, tr("消息提示"),tr("t_m_curamount失败!"));
                    
                    return;
                }
            }

            //            if(query.exec(QString("insert into t_m_point(vch_memberno ,ch_type, ch_payno , num_point , vch_memo , vch_operID , dt_operdate)values ('%1','%2','%3','%4',null,'%5','%6')")
            //                          .arg(table_model.item(i,4)->text()).arg("3").arg(liushuihao).arg(jifen).arg(n_func::gs_operid).arg(table_model.item(i,7)->text())))
            //            {

            //            }
            //            else
            //            {
            //                QSqlDatabase::database().rollback();
            //                lds_messagebox::warning(this, tr("消息提示"),tr("t_m_point失败!"));
            //
            //                return;
            //            }
        }

        if(query.exec(QString("insert into cey_u_checkout_detail(ch_payno , ch_paymodeno , num_payamount , num_change , num_realamount , num_face ,vch_operID , dt_operdate ,num_card_remain,vch_voucherno, num_face_Num )values('%1','%2','%3','%4','%5','%6','%7','%8','%9','%10','%11')")
                      .arg(liushuihao).arg(table_model.item(i,5)->text()).arg(table_model.item(i,2)->text()).arg("0.0").arg(table_model.item(i,3)->text())
                      .arg("0.0").arg(n_func::gs_operid).arg(table_model.item(i,7)->text()).arg("0.0").arg(pingzheng).arg("0.0")))
        {

        }
        else
        {
            QSqlDatabase::database().rollback();
            lds_messagebox::warning(this, tr("消息提示"),tr("cey_u_checkout_detail失败!"));
            
            return;
        }
    }

    //============================================================================================

    //菜品消费明细表
    QStandardItemModel & model = table_order_2::modelBase;
    for(int i = 0; i < model.rowCount(); i++)
    {
        query.prepare(QString("update cey_u_orderdish Set int_discount =:zhekou , ch_consult ='0' ,vch_dis_operID=:oper ,dt_dis_operdate=:time  where ch_billno =:danhao and ch_dishno =:pinming"));
        query.bindValue(":zhekou",model.item(i,5)->text().toInt());
        query.bindValue(":oper",model.item(i,14)->text());
        query.bindValue(":dt_dis_operdate",model.item(i,19)->text());
        query.bindValue("danhao",w_scr_dish_restaurant_dialog::table_danhao);
        query.bindValue(":pinming",model.item(i,1)->text());
        if(query.exec())
        {
        }
        else
        {
            QSqlDatabase::database().rollback();
            lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_u_orderdish失败!"));
            
            return;
        }
    }

    query.prepare(QString("update cey_u_orderdish set ch_payno  = :liushui where ch_billno = :danhao"));
    query.bindValue(":liushui",liushuihao);
    query.bindValue(":danhao",danhao);
    if(query.exec())
    {
    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_u_orderdish失败!"));
        
        return;
    }

    query.prepare(QString("update cey_u_table set ch_payno  = :liushui where ch_tableno =:tableno and ch_billno  =:danhao"));
    query.bindValue(":liushui",liushuihao);
    query.bindValue(":tableno",zhuohao);
    query.bindValue(":danhao",danhao);
    if(query.exec())
    {
    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_u_table失败!"));
        
        return;
    }

    query.prepare(QString("update cey_bt_table set ch_billno  = null where ch_tableno =:tableno"));
    query.bindValue(":tableno",zhuohao);
    if(query.exec())
    {
    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_u_table失败!"));
        
        return;
    }

    query.prepare(QString("update cey_bt_table set ch_lockflag ='N' where ch_tableno =:tableno"));
    query.bindValue(":tableno",zhuohao);
    if(query.exec())
    {
    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_u_table失败!"));
        
        return;
    }

    query.prepare(QString("update cey_u_table set num_cost =:num where ch_billno =:danhao"));
    query.bindValue(":num",QString().sprintf("%0.2f",yingshou_jine));
    query.bindValue(":danhao",danhao);
    if(query.exec())
    {
    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_u_table失败!"));
        
        return;
    }

    dayindanhao = danhao;
    QSqlDatabase::database().commit();
    print();
    btn_t_f  = true;
    this->close();
}


void table_pay::on_lineEdit_9_textChanged(const QString &arg1)
{
    if(count_dis != 0)
    {
        float xianjin = 0.00;
        xianjin = ui->lineEdit_9->text().toFloat();
        float yingshou = 0.00;
        yingshou = ui->label_14->text().toFloat();
        if(xianjin > yingshou)
        {
            ui->lineEdit_7->setText(QString().sprintf("%0.2f",xianjin - yingshou));
        }
        else
        {
            ui->lineEdit_7->setText(tr("0.00"));
        }
    }
    else
    {
        ui->lineEdit_7->setText(tr("0.00"));
    }
}

void table_pay::print_lost(const QString & str)
{
    ;
    lds_messagebox::warning(this, tr("消息提示"),str);
    
}

void table_pay::print()
{
    QSettings *configIniRead = new QSettings(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    QSqlQueryModel model;
    QString title = tr("付款账单\n");//需居中

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
    model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='serial_no_len' "));
    QString seriallen = model.record(0).value(0).toString();


    {
        blockIns b;
        QString content;//标题内容
        QString fontsize;

        //第一行=====================================================================================
        //单号
        if(danhao == "1")
        {
            w_scr_dish::setprintsize(b, danlen, tr("付款号:") + wdanhao+"\n");
        }

        //工号
        if(gonghao == "1")
        {
            w_scr_dish::setprintsize(b, gonglen, tr("工号:") + n_func::gs_operid+"\n");
        }

        //餐桌
        if(table == "1")
        {
            w_scr_dish::setprintsize(b, tablelen,  tr("餐桌:Q000")+"\n");
        }

        //人数
        if(renshu == "1")
        {
            w_scr_dish::setprintsize(b, renshulen, tr("人数:1")+"\n");
        }

        //时间
        if(timer == "1")
        {
            w_scr_dish::setprintsize(b, timerlen, tr("时间:") +pay_timer+"\n");
        }
        //流水号
        if(timer == "1")
        {
            QString liushuihaoma = tr("流水号:") +
                    (/*liushuihao_p_is_shoudog.isEmpty()?*/liushuihao_p/*:liushuihao_p_is_shoudog*/) +
                    "\n";
            w_scr_dish::setprintsize(b, seriallen, liushuihaoma);
        }
        //打印消费内容

        QString shifoumingxi;
        shifoumingxi = configIniRead->value("system_setting/maidanmingxi").toString();
        if(shifoumingxi == "1")
        {
            //表头
            QString tou = configIniRead->value("system_setting/biaoneirong").toString();
            //tou = tr("项目               数量           金额\n");
            content.append(n_func::gs_print_line+"\n");
            content.append(tou+"\n");
            content.append(tr("\n---------------------------\n"));
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
                    QStandardItemModel  cook;
                    for(int m = 0; m < table_order_2::modelBase.rowCount(); m++)
                    {
                        if(table_order_2::modelBase.item(m,22)->text() == dalei.record(k).value(0).toString())
                        {
                            int row = cook.rowCount();
                            cook.setRowCount(row + 1);
                            cook.setItem(row,0,new QStandardItem(table_order_2::modelBase.item(m,0)->text()));
                            cook.setItem(row,1,new QStandardItem(table_order_2::modelBase.item(m,1)->text()));
                            cook.setItem(row,2,new QStandardItem(table_order_2::modelBase.item(m,2)->text()));
                            cook.setItem(row,3,new QStandardItem(table_order_2::modelBase.item(m,3)->text()));
                            cook.setItem(row,4,new QStandardItem(table_order_2::modelBase.item(m,4)->text()));
                            cook.setItem(row,5,new QStandardItem(table_order_2::modelBase.item(m,5)->text()));
                            cook.setItem(row,6,new QStandardItem(table_order_2::modelBase.item(m,6)->text()));
                            cook.setItem(row,7,new QStandardItem(table_order_2::modelBase.item(m,7)->text()));
                            cook.setItem(row,8,new QStandardItem(table_order_2::modelBase.item(m,8)->text()));
                            cook.setItem(row,9,new QStandardItem(table_order_2::modelBase.item(m,9)->text()));
                            cook.setItem(row,10,new QStandardItem(table_order_2::modelBase.item(m,10)->text()));
                            cook.setItem(row,11,new QStandardItem(table_order_2::modelBase.item(m,11)->text()));
                            cook.setItem(row,12,new QStandardItem(table_order_2::modelBase.item(m,12)->text()));
                            cook.setItem(row,13,new QStandardItem(table_order_2::modelBase.item(m,13)->text()));
                            cook.setItem(row,14,new QStandardItem(table_order_2::modelBase.item(m,14)->text()));
                            cook.setItem(row,15,new QStandardItem(table_order_2::modelBase.item(m,15)->text()));
                            cook.setItem(row,16,new QStandardItem(table_order_2::modelBase.item(m,16)->text()));
                            cook.setItem(row,17,new QStandardItem(table_order_2::modelBase.item(m,17)->text()));
                            cook.setItem(row,18,new QStandardItem(table_order_2::modelBase.item(m,18)->text()));
                            cook.setItem(row,19,new QStandardItem(table_order_2::modelBase.item(m,19)->text()));
                            cook.setItem(row,20,new QStandardItem(table_order_2::modelBase.item(m,20)->text()));
                            cook.setItem(row,21,new QStandardItem(table_order_2::modelBase.item(m,21)->text()));
                            cook.setItem(row,22,new QStandardItem(table_order_2::modelBase.item(m,22)->text()));
                        }
                    }
                    QByteArray neirong;
                    float zongjia = 0.00;
                    for(int i = 0; i < cook.rowCount(); i++)
                    {
                        zongjia += cook.item(i,3)->text().toFloat() * cook.item(i,4)->text().toFloat();
                        QString hanghao = QString::number(i+1) + tr(".");
                        if(hanghao.length() < 3)
                        {
                            hanghao.append(" ");
                        }
                        if(bxuhao=="1")neirong.append(hanghao);
                        if(bzensong == "1")
                        {
                            if(cook.item(i,8)->text() == tr("Y"))
                            {
                                neirong.append(tr("赠)"));
                            }
                        }

                        if(btejia == "1")
                        {
                            if(cook.item(i,10)->text() == tr("Y"))
                            {
                                neirong.append(tr("特)"));
                            }
                        }

                        if(cook.item(i,9)->text() == "P")
                        {
                            neirong.append(tr("套)"));
                        }

                        if(cook.item(i,9)->text() == tr("Y"))
                        {
                            neirong.append(tr("细)"));
                        }
                        // content.append(neirong);
                        //第一行=========================================================================
                        QByteArray neirong1;
                        //品名
                        if(pinming == "1")
                        {
                            QString pin = cook.item(i,2)->text();
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
                                w_scr_dish::setprintsize(pin, pinminglen,"pin");
                            }

                            if(pinmingrow == 1)
                            {
                                neirong1.append(pin);
                            }
                        }

                        //单位
                        if(danwei == "1")
                        {
                            QString dan = cook.item(i,12)->text();
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
                            QString shu = cook.item(i,3)->text();
                            if(shuliangxiaoshu == 0)
                            {
                                shu = QString::number(int(shu.toInt()));
                            }
                            else if(shuliangxiaoshu == 1)
                            {
                                shu = QString().sprintf("%0.1f",shu.toFloat());
                            }
                            else
                            {
                                shu = QString().sprintf("%0.2f",shu.toFloat());
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
                            QString jia = cook.item(i,4)->text();
                            if(jiagexiaoshu == 0)
                            {
                                if(cook.item(i,9)->text() == "Y")
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
                                if(cook.item(i,9)->text() == "Y")
                                {
                                    jia = tr("0.0");
                                }
                                else
                                {
                                    jia = QString().sprintf("%0.1f",jia.toFloat());
                                }
                            }
                            else
                            {
                                if(cook.item(i,9)->text() == "Y")
                                {
                                    jia = tr("0.00");
                                }
                                else
                                {
                                    jia = QString().sprintf("%0.2f",jia.toFloat());
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
                            aa = 0.00;
                            aa = cook.item(i,3)->text().toFloat() * cook.item(i,4)->text().toFloat();
                            QString jin = QString().sprintf("%0.2f",aa);

                            if(jinexiaoshu == 0)
                            {
                                if(cook.item(i,9)->text() == "Y")
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
                                if(cook.item(i,9)->text() == "Y")
                                {
                                    jin = tr("0.0");
                                }
                                else
                                {
                                    jin = QString().sprintf("%0.1f",jin.toFloat());
                                }
                            }
                            else
                            {
                                if(cook.item(i,9)->text() == "Y")
                                {
                                    jin = tr("0.00");
                                }
                                else
                                {
                                    jin = QString().sprintf("%0.2f",jin.toFloat());
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
                            float aa = cook.item(i,3)->text().toFloat() * cook.item(i,4)->text().toFloat() * (100 - cook.item(i,5)->text().toInt() )*0.01;
                            QString zhe = QString().sprintf("%0.2f",aa);
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
                            QString pin = cook.item(i,2)->text();
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
                                w_scr_dish::setprintsize(pin, pinminglen,"pin");
                            }

                            if(pinmingrow == 2)
                            {
                                neirong2.append(pin);
                            }
                        }

                        //单位
                        if(danwei == "1")
                        {
                            QString dan = cook.item(i,12)->text();
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
                            QString shu = cook.item(i,3)->text();
                            if(shuliangxiaoshu == 0)
                            {
                                shu = QString::number(int(shu.toInt()));
                            }
                            else if(shuliangxiaoshu == 1)
                            {
                                shu = QString().sprintf("%0.1f",shu.toFloat());
                            }
                            else
                            {
                                shu = QString().sprintf("%0.2f",shu.toFloat());
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
                            QString jia = cook.item(i,4)->text();
                            if(jiagexiaoshu == 0)
                            {
                                if(cook.item(i,9)->text() == "Y")
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
                                if(cook.item(i,9)->text() == "Y")
                                {
                                    jia = tr("0.0");
                                }
                                else
                                {
                                    jia = QString().sprintf("%0.1f",jia.toFloat());
                                }
                            }
                            else
                            {
                                if(cook.item(i,9)->text() == "Y")
                                {
                                    jia = tr("0.00");
                                }
                                else
                                {
                                    jia = QString().sprintf("%0.2f",jia.toFloat());
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
                            aa = 0.00;
                            aa = cook.item(i,3)->text().toFloat() * cook.item(i,4)->text().toFloat();
                            QString jin = QString().sprintf("%0.2f",aa);

                            if(jinexiaoshu == 0)
                            {
                                if(cook.item(i,9)->text() == "Y")
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
                                if(cook.item(i,9)->text() == "Y")
                                {
                                    jin = tr("0.0");
                                }
                                else
                                {
                                    jin = QString().sprintf("%0.1f",jin.toFloat());
                                }
                            }
                            else
                            {
                                if(cook.item(i,9)->text() == "Y")
                                {
                                    jin = tr("0.00");
                                }
                                else
                                {
                                    jin = QString().sprintf("%0.2f",jin.toFloat());
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
                            float aa = cook.item(i,3)->text().toFloat() * cook.item(i,4)->text().toFloat() * (100 - cook.item(i,5)->text().toInt() )*0.01;
                            QString zhe = QString().sprintf("%0.2f",aa);
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
                            if(!cook.item(i,7)->text().isEmpty())
                            {
                                neirong.append(cook.item(i,7)->text());
                                neirong.append("\n");
                            }
                        }

                        //content
                    }
                    if(cook.rowCount() > 0)
                    {
                        content.append(neirong);
                        QSqlQueryModel thismodel;
                        thismodel.setQuery(QString("select vch_dish_typename from cey_bt_dish_type where ch_dish_typeno = '%1' ").arg(cook.item(0,22)->text()));
                        QString strlei = tr("      ") + thismodel.record(0).value(0).toString() + tr("------------") + QString().sprintf("%0.2f",zongjia) + "\n";
                        content.append(strlei);
                        content.append(n_func::gs_print_line+"\n");
                    }
                }
            }
            else
            {
                QStandardItemModel & cook = table_order_2::modelBase;
                QByteArray neirong;
                for(int i = 0; i < cook.rowCount(); i++)
                {
                    QString hanghao = QString::number(i+1) + tr(".");
                    if(hanghao.length() < 3)
                    {
                        hanghao.append(" ");
                    }
                    if(bxuhao=="1")neirong.append(hanghao);
                    if(bzensong == "1")
                    {
                        if(cook.item(i,8)->text() == tr("Y"))
                        {
                            neirong.append(tr("赠)"));
                        }
                    }

                    if(btejia == "1")
                    {
                        if(cook.item(i,10)->text() == tr("Y"))
                        {
                            neirong.append(tr("特)"));
                        }
                    }

                    if(cook.item(i,9)->text() == "P")
                    {
                        neirong.append(tr("套)"));
                    }

                    if(cook.item(i,9)->text() == tr("Y"))
                    {
                        neirong.append(tr("细)"));
                    }

                    //第一行=========================================================================
                    QByteArray neirong1;
                    //品名
                    if(pinming == "1")
                    {
                        QString pin = cook.item(i,2)->text();
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
                            w_scr_dish::setprintsize(pin, pinminglen,"pin");
                        }

                        if(pinmingrow == 1)
                        {
                            neirong1.append(pin);
                        }
                    }

                    //单位
                    if(danwei == "1")
                    {
                        QString dan = cook.item(i,12)->text();
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
                        QString shu = cook.item(i,3)->text();
                        if(shuliangxiaoshu == 0)
                        {
                            shu = QString::number(int(shu.toInt()));
                        }
                        else if(shuliangxiaoshu == 1)
                        {
                            shu = QString().sprintf("%0.1f",shu.toFloat());
                        }
                        else
                        {
                            shu = QString().sprintf("%0.2f",shu.toFloat());
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
                        QString jia = cook.item(i,4)->text();
                        if(jiagexiaoshu == 0)
                        {
                            if(cook.item(i,9)->text() == "Y")
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
                            if(cook.item(i,9)->text() == "Y")
                            {
                                jia = tr("0.0");
                            }
                            else
                            {
                                jia = QString().sprintf("%0.1f",jia.toFloat());
                            }
                        }
                        else
                        {
                            if(cook.item(i,9)->text() == "Y")
                            {
                                jia = tr("0.00");
                            }
                            else
                            {
                                jia = QString().sprintf("%0.2f",jia.toFloat());
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
                        aa = 0.00;
                        aa = cook.item(i,3)->text().toFloat() * cook.item(i,4)->text().toFloat();
                        QString jin = QString().sprintf("%0.2f",aa);

                        if(jinexiaoshu == 0)
                        {
                            if(cook.item(i,9)->text() == "Y")
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
                            if(cook.item(i,9)->text() == "Y")
                            {
                                jin = tr("0.0");
                            }
                            else
                            {
                                jin = QString().sprintf("%0.1f",jin.toFloat());
                            }
                        }
                        else
                        {
                            if(cook.item(i,9)->text() == "Y")
                            {
                                jin = tr("0.00");
                            }
                            else
                            {
                                jin = QString().sprintf("%0.2f",jin.toFloat());
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
                        float aa = cook.item(i,3)->text().toFloat() * cook.item(i,4)->text().toFloat() * (100 - cook.item(i,5)->text().toInt() )*0.01;
                        QString zhe = QString().sprintf("%0.2f",aa);
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
                        QString pin = cook.item(i,2)->text();
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
                            w_scr_dish::setprintsize(pin, pinminglen,"pin");
                        }

                        if(pinmingrow == 2)
                        {
                            neirong2.append(pin);
                        }
                    }

                    //单位
                    if(danwei == "1")
                    {
                        QString dan = cook.item(i,12)->text();
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
                        QString shu = cook.item(i,3)->text();
                        if(shuliangxiaoshu == 0)
                        {
                            shu = QString::number(int(shu.toInt()));
                        }
                        else if(shuliangxiaoshu == 1)
                        {
                            shu = QString().sprintf("%0.1f",shu.toFloat());
                        }
                        else
                        {
                            shu = QString().sprintf("%0.2f",shu.toFloat());
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
                        QString jia = cook.item(i,4)->text();
                        if(jiagexiaoshu == 0)
                        {
                            if(cook.item(i,9)->text() == "Y")
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
                            if(cook.item(i,9)->text() == "Y")
                            {
                                jia = tr("0.0");
                            }
                            else
                            {
                                jia = QString().sprintf("%0.1f",jia.toFloat());
                            }
                        }
                        else
                        {
                            if(cook.item(i,9)->text() == "Y")
                            {
                                jia = tr("0.00");
                            }
                            else
                            {
                                jia = QString().sprintf("%0.2f",jia.toFloat());
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
                        aa = 0.00;
                        aa = cook.item(i,3)->text().toFloat() * cook.item(i,4)->text().toFloat();
                        QString jin = QString().sprintf("%0.2f",aa);

                        if(jinexiaoshu == 0)
                        {
                            if(cook.item(i,9)->text() == "Y")
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
                            if(cook.item(i,9)->text() == "Y")
                            {
                                jin = tr("0.0");
                            }
                            else
                            {
                                jin = QString().sprintf("%0.1f",jin.toFloat());
                            }
                        }
                        else
                        {
                            if(cook.item(i,9)->text() == "Y")
                            {
                                jin = tr("0.00");
                            }
                            else
                            {
                                jin = QString().sprintf("%0.2f",jin.toFloat());
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
                        float aa = cook.item(i,3)->text().toFloat() * cook.item(i,4)->text().toFloat() * (100 - cook.item(i,5)->text().toInt() )*0.01;
                        QString zhe = QString().sprintf("%0.2f",aa);
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
                        if(!cook.item(i,7)->text().isEmpty())
                        {
                            neirong.append(cook.item(i,7)->text());
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
        linecount = 2;
        int line_num = 0;
        //付款详情
        QString xiangqing = configIniRead->value("system_setting/fukuanxiangqing").toString();
        // xiangqing = "1";
        if(xiangqing == "1")
        {
            if(ui->label_14->text().toFloat() > 0)
            {
                line_num++;
                QString str = tr("现金:") + ui->label_14->text();
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

            for(int i = 0; i < table_model.rowCount(); i++)
            {
                line_num++;
                QString str = table_model.item(i,1)->text() + tr(":") + table_model.item(i,3)->text();
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
        if(model.record(0).value(0).toInt() == 1)
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
        if(model.record(0).value(0).toInt() == 1)
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
        if(model.record(0).value(0).toInt() == 1)
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
        QString shuishou = configIniRead->value("system_setting/shuishoujine").toString();
        //shuishou = "1";
        if(shuishou.toInt() == 1)
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
        if(model.record(0).value(0).toInt() == 1)
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
        if(model.record(0).value(0).toInt() == 1)
        {
            line_num++;
            QString str = tr("实收:") + ui->lineEdit_4->text();//是应收的金额？所手的全部金额
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

        for(int i = 0; i < table_model.rowCount(); i++)
        {
            if(table_model.item(i,1)->text() == tr("充值卡"))
            {
                QSqlQueryModel model_ka;
                model_ka.setQuery(QString("SELECT  b.vch_memberno ,b.vch_cardno,b.vch_member ,ifnull((select d.vch_typename from t_m_member_type d where d.ch_typeno = b.ch_typeno ),b.ch_typeno) ,b.vch_tel ,b.vch_handtel ,b.ch_state ,b.ch_cardflag ,   DATE_FORMAT(b.dt_birthday, '%Y-%m-%d %h:%i:%s') ,    b.vch_operid ,     DATE_FORMAT(b.dt_limit, '%Y-%m-%d %h:%i:%s') ,'point' = ifnull( (select num_point from t_m_curamount where vch_memberno = b.vch_memberno),0)   FROM t_m_member_type a  ,t_m_member b  where a.ch_typeno = b.ch_typeno and b.vch_memberno = '%1'").arg(table_model.item(i,4)->text()));
                //会员名称
                model.clear();
                model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_membername' "));
                if(model.record(0).value(0).toInt() == 1)
                {
                    line_num++;
                    QString str = tr("会员名称:") + table_model.item(i,4)->text();
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
                    QString str = tr("会员积分:") + QString().sprintf("%0.2f",model_ka.record(0).value(11).toFloat());
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


        if(content.right(2) != "\n")
        {
            content.append("\n");
        }

        //签名
        model.clear();
        model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_underwrite' "));
        if(model.record(0).value(0).toInt() == 1)
        {
            QString str = tr("签名:\n");
            content.append(str);
        }

        //尾语
        content.append(configIniRead->value("system_setting/jieweiyu").toString());
        //打印份数
        model.clear();
        model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_togo_count' "));
        int print_count = model.record(0).value(0).toInt();
        //print_count = 2;
        //        printer = new public_mainprinter();
        QString path;
        QString type = configIniRead->value("system_setting/dyjjiekouleixing").toString();
        if(type == tr("串口打印") || type == tr("USB打印"))
        {
            path = configIniRead->value("system_setting/dayinjiduankou").toString();
        }
        else
        {
            path = configIniRead->value("system_setting/dayinjiip").toString() + ":9100";
        }
        // path = "192.168.1.127:9100";
        //图片打印
        QString logo = configIniRead->value("system_setting/dayinlogo").toString();
        // logo = "1";
        if(logo == "1")
        {
            QString str = configIniRead->value("system_setting/dayinlogoneirong").toString();//图片路径
            // str = tr("/home/gyq/pic/logo.png");
            //printer->asynchPrint(printer->setPrintCount(printer->drawLogo("1B40 1B6101", str), 1),"/dev/ttyS0" );//清除缓存 居中 pixmap路径
            b+= backheader::printer->drawLogo("1B40 1B6101", str);//清除缓存 居中 pixmap路径
        }
        content.append("\n");
        QString liuhuihaoma = tr("流水号:") + liushuihao_p + "\n";

        type = configIniRead->value("system_setting/dyjjiekouleixing").toString();
        if(type == tr("串口打印") || type == tr("USB打印"))
        {
            path = configIniRead->value("system_setting/dayinjiduankou").toString();
        }
        else
        {
            path = configIniRead->value("system_setting/dayinjiip").toString() + ":9100";
        }
        //        path = "192.168.1.172:9100";
        b+=backheader::printer->drawText("1B40 1B6101 1C5700",title);//居中不放大
        b+=backheader::printer->drawText("1B40,1C5701",liuhuihaoma,Qt::AlignLeft);//放大
        b += backheader::printer->drawText("1B40,1C5700", content, Qt::AlignLeft);

        //打印一维码
        QString yiweima = configIniRead->value("system_setting/yiweima").toString();
        //yiweima  = "1";
        if(yiweima == "1")
        {
            QString str = configIniRead->value("system_setting/yiweimaneirong").toString();//一维码内容
            //str = "12345678";
            b+= backheader::printer->drawoneBarcode("1B40 1B6101",str);//清除缓存 居中 pixmap路径
        }

        //打印二维码
        QString erweima = configIniRead->value("system_setting/erweima").toString();
        //erweima = "1";
        if(erweima == "1")
        {
            QString str = configIniRead->value("system_setting/erweimaneirong").toString();//图片路径
            // str = tr("www.baidu.com");
            //printer->asynchPrint(printer->setPrintCount(printer->drawLogo("1B40 1B6101", str), 1),"/dev/ttyS0" );//清除缓存 居中 pixmap路径
            b+= backheader::printer->drawQR_Code("", str);//清除缓存 居中 pixmap路径
        }

        b += backheader::printer->drawText("1B6405 1D5601", "");//走纸5x， 切纸
        b=backheader::printer->setPrintCount(b, print_count);
        backheader::asyn_dataprint(b, path);


    }

    //出品打印机
    {
        //出品打印开始=============================
        QSettings *kitchenprinter = new  QSettings(public_sql::settings_ini_dir_file, QSettings::IniFormat);
        QSqlQueryModel printflag;
        QStandardItemModel & print_flag = table_order_2::modelBase;
        QByteArray content_kitchen;

        QString str;
        str =  kitchenprinter->value("w_bt_dish_Kitchenprinter_param/printer_type_com").toString();
        QString str_type = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/printer_type_com").toString();
        //菜品打印方式
        str.clear();
        str =  kitchenprinter->value("w_bt_dish_Kitchenprinter_param/out_dish_printtype_com").toString();
        if(str == "0")//一菜一单
        {
            int hanghao = 0;
            for(int i = 0; i < print_flag.rowCount(); i++)
            {
                content_kitchen.clear();
                printflag.clear();
                printflag.setQuery(QString("select ch_printflag,vch_printer,ch_outflag,vch_outprint from cey_bt_dish where ch_dishno = '%1'").arg(print_flag.item(i,1)->text()));
                if(printflag.record(0).value(0).toString() == tr("Y"))//后台进行了厨打设置
                {
                    hanghao++;
                    QSqlQueryModel thism;
                    thism.setQuery("Select NOW()");
                    QString print_timer = thism.record(0).value(0).toDateTime().toString("yyyy-MM-dd hh:mm:ss");
                    //流水号
                    QString liushuhao;
                    str.clear();
                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/serialno_check").toString();
                    if(str == "1")
                    {
                        //                        int a = bendandanhao.right(4).toInt()%100;
                        //                        if(a == 0)
                        //                        {
                        //                            liushuhao = tr("流水号:01\n");
                        //                        }
                        //                        else if(a < 10)
                        //                        {
                        //                            liushuhao = tr("流水号:0") + QString::number(a) + "\n";
                        //                        }
                        //                        else
                        //                        {
                        //                            liushuhao = tr("流水号:") + QString::number(a)+ "\n";
                        //                        }
                        liushuhao=system_setting::get_max_serialno_add1(bendandanhao);
                        content_kitchen.append(liushuhao);
                    }

                    //开始单号
                    str.clear();
                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/openbillno_check").toString();
                    if(str == "1")
                    {
                        content_kitchen.append(tr("开始单号:") + bendandanhao + "\n");
                    }

                    //餐桌名
                    str.clear();
                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/tablename_check").toString();
                    if(str == "1")
                    {
                        content_kitchen.append(tr("餐桌名:") + tr("Q000\n"));
                    }

                    //点菜人
                    str.clear();
                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/dishopertor_check").toString();
                    if(str == "1")
                    {
                        content_kitchen.append(tr("点菜人:") + n_func::gs_operid + "\n");
                    }

                    //点菜时间
                    str.clear();
                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/dishtime_check").toString();
                    if(str == "1")
                    {
                        content_kitchen.append(tr("点菜时间:") + table_order_2::diancaitime + "\n");
                    }

                    //打印时间
                    str.clear();
                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/printtime_check").toString();
                    if(str == "1")
                    {
                        content_kitchen.append(tr("打印时间:") + print_timer + "\n");
                    }

                    //人数
                    str.clear();
                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/person_check").toString();
                    if(str == "1")
                    {
                        content_kitchen.append(tr("人数:1\n"));
                    }

                    //表内容
                    if(str_type == "0")//80打印机
                    {
                        content_kitchen.append("----------------------------\n");
                    }
                    else
                    {
                        content_kitchen.append("--------------------------------\n");
                    }

                    QString biaotou;
                    //品名
                    str.clear();
                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/ch_dishno_check").toString();
                    if(str == "1")
                    {
                        biaotou.append(tr("品名             "));
                    }

                    //单位
                    str.clear();
                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/vch_unit_check").toString();
                    if(str == "1")
                    {
                        biaotou.append(tr("单位   "));
                    }

                    //品码
                    str.clear();
                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/vch_dishname_check").toString();
                    if(str == "1")
                    {
                        biaotou.append(tr("品码     "));
                    }

                    //数量
                    str.clear();
                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_num_check").toString();
                    if(str == "1")
                    {
                        biaotou.append(tr("数量     "));
                    }

                    //价格
                    str.clear();
                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_price_check").toString();
                    if(str == "1")
                    {
                        biaotou.append(tr("价格"));
                    }

                    content_kitchen.append(biaotou + "\n");
                    if(str_type == "0")//80打印机
                    {
                        content_kitchen.append("----------------------------\n");
                    }
                    else
                    {
                        content_kitchen.append("--------------------------------\n");
                    }

                    //一菜一单打印菜品小类
                    str.clear();
                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/out_dish_bill_printtype_check").toString();
                    if(str == "1")
                    {
                        QString daleimingcheng;
                        QSqlQueryModel mymodel;
                        mymodel.setQuery(QString("select vch_sub_typename from cey_bt_dish_subtype where ch_sub_typeno = '%1' ").arg(print_flag.item(i,23)->text()));
                        if(str_type == "0")//80打印机
                        {
                            daleimingcheng = tr("            ---------") + mymodel.record(0).value(0).toString() + tr("---------\n");
                        }
                        else
                        {
                            daleimingcheng = tr("    ---------") + mymodel.record(0).value(0).toString() + tr("---------\n");

                        }
                        content_kitchen.append(daleimingcheng);
                    }


                    //本单序号
                    str.clear();
                    str =  kitchenprinter->value("w_bt_dish_Kitchenprinter_param/selfbillno_check").toString();
                    if(str == "1")
                    {
                        QString xuhao = QString::number(hanghao) + tr(".");
                        if(xuhao.length() < 3)
                        {
                            xuhao.append(" ");
                        }

                        content_kitchen.append(xuhao);
                    };

                    //赠送标志
                    str.clear();
                    str =  kitchenprinter->value("w_bt_dish_Kitchenprinter_param/presentflag_check").toString();
                    if(str == "1")
                    {
                        if(print_flag.item(i,8)->text() == "Y")
                        {
                            QString zb =("赠)");
                            content_kitchen.append(zb);
                        }
                    };

                    if(print_flag.item(i,9)->text() == "Y")
                    {
                        QString taobiao = tr("套)");
                        content_kitchen.append(taobiao);
                    }

                    //厨打第一行==================================================

                    //品名
                    str.clear();
                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/ch_dishno_check").toString();
                    QString hangneirong;
                    if(str == "1")
                    {
                        //品名宽度
                        str.clear();
                        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/ch_dishno_size_spin").toString();
                        int a = 0;
                        a = str.toInt();
                        QString pinming = print_flag.item(i,2)->text();

                        if(pinming.length() >= a)
                        {
                            pinming = pinming.left(a);
                        }

                        if(pinming.length() < a)
                        {
                            int h = a * 2 - pinming.length() * 2;
                            for(int k = 0; k < h; k++)
                            {
                                pinming.append(" ");
                            }
                        }

                        //品名所在行
                        str.clear();
                        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/ch_dishno_hang_spin").toString();
                        if(str == "1")
                        {
                            hangneirong.append(pinming);
                        }
                    }

                    //单位
                    str.clear();
                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/vch_unit_check").toString();
                    if(str == "1")
                    {
                        //单位宽度
                        str.clear();
                        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/vch_unit_size_spin").toString();
                        int a = 0;
                        a = str.toInt();
                        QString danw = print_flag.item(i,12)->text();

                        if(danw.length() >= a)
                        {
                            danw = danw.left(a);
                        }

                        if(danw.length() < a)
                        {
                            int h = a * 2 - danw.length() * 2;
                            for(int k = 0; k < h; k++)
                            {
                                danw.append(" ");
                            }
                        }

                        //单位所在行
                        str.clear();
                        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/vch_unit_hang_spin").toString();
                        if(str == "1")
                        {
                            hangneirong.append(danw);
                        }
                    }

                    //品码
                    str.clear();
                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/vch_dishname_check").toString();
                    if(str == "1")
                    {
                        //品码宽度
                        str.clear();
                        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/vch_dishname_size_spin").toString();
                        int a = 0;
                        a = str.toInt();
                        QString pinma = print_flag.item(i,1)->text();

                        if(pinma.length() >= a)
                        {
                            pinma = pinma.left(a);
                        }

                        if(pinma.length() < a)
                        {
                            int h = a - pinma.length();
                            for(int k = 0; k < h; k++)
                            {
                                pinma.append(" ");
                            }
                        }

                        //单位所在行
                        str.clear();
                        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/vch_dishname_hang_spin").toString();
                        if(str == "1")
                        {
                            hangneirong.append(pinma);
                        }
                    }

                    //数量
                    str.clear();
                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_num_check").toString();
                    if(str == "1")
                    {
                        QString shuliang;
                        str.clear();
                        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_num_xiaoshu_spin").toString();
                        //数量保留小数位数
                        if(str == "0")
                        {
                            shuliang = QString::number(int(print_flag.item(i,3)->text().toFloat()));
                        }
                        else if(str == "1")
                        {
                            shuliang = QString().sprintf("%0.1f",print_flag.item(i,3)->text().toFloat());
                        }
                        else
                        {
                            shuliang = QString().sprintf("%0.2f",print_flag.item(i,3)->text().toFloat());
                        }

                        //数量宽度
                        str.clear();
                        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_num_size_spin").toString();
                        int a = 0;
                        a = str.toInt();

                        if(shuliang.length() >= a)
                        {
                            shuliang = shuliang.left(a);
                        }

                        if(shuliang.length() < a)
                        {
                            int h = a - shuliang.length();
                            for(int k = 0; k < h; k++)
                            {
                                shuliang.append(" ");
                            }
                        }

                        //数量所在行
                        str.clear();
                        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_num_hang_spin").toString();
                        if(str == "1")
                        {
                            hangneirong.append(shuliang);
                        }
                    }


                    //价格
                    str.clear();
                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_price_check").toString();
                    if(str == "1")
                    {
                        QString jiage;
                        str.clear();
                        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_price_xiaoshu_spin").toString();
                        //价格保留小数位数
                        if(str == "0")
                        {
                            jiage = QString::number(int(print_flag.item(i,4)->text().toFloat()));
                        }
                        else if(str == "1")
                        {
                            jiage = QString().sprintf("%0.1f",print_flag.item(i,4)->text().toFloat());
                        }
                        else
                        {
                            jiage = QString().sprintf("%0.2f",print_flag.item(i,4)->text().toFloat());
                        }

                        //价格宽度
                        str.clear();
                        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_price_size_spin").toString();
                        int a = 0;
                        a = str.toInt();

                        if(jiage.length() >= a)
                        {
                            jiage = jiage.left(a);
                        }

                        if(jiage.length() < a)
                        {
                            int h = a - jiage.length();
                            for(int k = 0; k < h; k++)
                            {
                                jiage.append(" ");
                            }
                        }

                        //价格所在行
                        str.clear();
                        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_price_hang_spin").toString();
                        if(str == "1")
                        {
                            hangneirong.append(jiage);
                        }
                    }

                    if(!hangneirong.isEmpty())
                    {
                        content_kitchen.append(hangneirong);
                        content_kitchen.append("\n");
                    }
                    //厨打第一行==================================================

                    //厨打第二行==================================================
                    //品名
                    str.clear();
                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/ch_dishno_check").toString();
                    QString hangneirong1;
                    if(str == "1")
                    {
                        //品名宽度
                        str.clear();
                        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/ch_dishno_size_spin").toString();
                        int a = 0;
                        a = str.toInt();
                        QString pinming = print_flag.item(i,2)->text();

                        if(pinming.length() >= a)
                        {
                            pinming = pinming.left(a);
                        }


                        if(pinming.length() < a)
                        {
                            int h = a * 2 - pinming.length() * 2;
                            for(int k = 0; k < h; k++)
                            {
                                pinming.append(" ");
                            }
                        }

                        //品名所在行
                        str.clear();
                        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/ch_dishno_hang_spin").toString();
                        if(str == "2")
                        {
                            hangneirong1.append(pinming);
                        }
                    }

                    //单位
                    str.clear();
                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/vch_unit_check").toString();
                    if(str == "1")
                    {
                        //单位宽度
                        str.clear();
                        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/vch_unit_size_spin").toString();
                        int a = 0;
                        a = str.toInt();
                        QString danw = print_flag.item(i,12)->text();

                        if(danw.length() >= a)
                        {
                            danw = danw.left(a);
                        }

                        if(danw.length() < a)
                        {
                            int h = a * 2 - danw.length() * 2;
                            for(int k = 0; k < h; k++)
                            {
                                danw.append(" ");
                            }
                        }

                        //单位所在行
                        str.clear();
                        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/vch_unit_hang_spin").toString();
                        if(str == "2")
                        {
                            hangneirong1.append(danw);
                        }
                    }

                    //品码
                    str.clear();
                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/vch_dishname_check").toString();
                    if(str == "1")
                    {
                        //品码宽度
                        str.clear();
                        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/vch_dishname_size_spin").toString();
                        int a = 0;
                        a = str.toInt();
                        QString pinma = print_flag.item(i,1)->text();

                        if(pinma.length() >= a)
                        {
                            pinma = pinma.left(a);
                        }

                        if(pinma.length() < a)
                        {
                            int h = a - pinma.length();
                            for(int k = 0; k < h; k++)
                            {
                                pinma.append(" ");
                            }
                        }

                        //单位所在行
                        str.clear();
                        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/vch_dishname_hang_spin").toString();
                        if(str == "2")
                        {
                            hangneirong1.append(pinma);
                        }
                    }

                    //数量
                    str.clear();
                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_num_check").toString();
                    if(str == "1")
                    {
                        QString shuliang;
                        str.clear();
                        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_num_xiaoshu_spin").toString();
                        //数量保留小数位数
                        if(str == "0")
                        {
                            shuliang = QString::number(int(print_flag.item(i,3)->text().toFloat()));
                        }
                        else if(str == "1")
                        {
                            shuliang = QString().sprintf("%0.1f",print_flag.item(i,3)->text().toFloat());
                        }
                        else
                        {
                            shuliang = QString().sprintf("%0.2f",print_flag.item(i,3)->text().toFloat());
                        }

                        //数量宽度
                        str.clear();
                        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_num_size_spin").toString();
                        int a = 0;
                        a = str.toInt();

                        if(shuliang.length() >= a)
                        {
                            shuliang = shuliang.left(a);
                        }

                        if(shuliang.length() < a)
                        {
                            int h = a - shuliang.length();
                            for(int k = 0; k < h; k++)
                            {
                                shuliang.append(" ");
                            }
                        }

                        //数量所在行
                        str.clear();
                        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_num_hang_spin").toString();
                        if(str == "2")
                        {
                            hangneirong1.append(shuliang);
                        }
                    }


                    //价格
                    str.clear();
                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_price_check").toString();
                    if(str == "1")
                    {
                        QString jiage;
                        str.clear();
                        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_price_xiaoshu_spin").toString();
                        //价格保留小数位数
                        if(str == "0")
                        {
                            jiage = QString::number(int(print_flag.item(i,4)->text().toFloat()));
                        }
                        else if(str == "1")
                        {
                            jiage = QString().sprintf("%0.1f",print_flag.item(i,4)->text().toFloat());
                        }
                        else
                        {
                            jiage = QString().sprintf("%0.2f",print_flag.item(i,4)->text().toFloat());
                        }

                        //价格宽度
                        str.clear();
                        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_price_size_spin").toString();
                        int a = 0;
                        a = str.toInt();

                        if(jiage.length() >= a)
                        {
                            jiage = jiage.left(a);
                        }

                        if(jiage.length() < a)
                        {
                            int h = a - jiage.length();
                            for(int k = 0; k < h; k++)
                            {
                                jiage.append(" ");
                            }
                        }

                        //价格所在行
                        str.clear();
                        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_price_hang_spin").toString();
                        if(str == "2")
                        {
                            hangneirong1.append(jiage);
                        }
                    }

                    if(!hangneirong1.isEmpty())
                    {
                        content_kitchen.append(hangneirong1);
                        content_kitchen.append("\n");
                    }

                    //厨打第二行==================================================

                    //菜品金额合计
                    str.clear();
                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/dishsum_numprice_check").toString();
                    if(str == "1")
                    {
                        if(str_type == "0")//80打印机
                        {
                            content_kitchen.append("----------------------------\n");
                        }
                        else
                        {
                            content_kitchen.append("--------------------------------\n");
                        }
                        content_kitchen.append(tr("菜品合计金额:") + QString().sprintf("%0.2f",print_flag.item(i,6)->text().toFloat()));
                    }
                    //打印份数
                    str.clear();
                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/kitchen_num_spin").toString();
                    int print_count = str.toInt();
                    blockIns b;
                    QString path = printflag.record(0).value(1).toString() + ":9100";

                    //                    public_mainprinter *chuda_print = new public_mainprinter();
                    str.clear();
                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/printer_fontsize_com").toString();
                    if(str == "1")//普通字体
                    {
                        b+=backheader::printer->drawText("1B40","");
                        b+=backheader::printer->drawText("1B40 1B6101 1C5700",tr("出品打印单\n"));//居中放大
                        b += backheader::printer->drawText("1B40 1C5700", content_kitchen, Qt::AlignLeft);

                    }
                    else
                    {
                        b+=backheader::printer->drawText("1B40","");
                        b+=backheader::printer->drawText("1B40 1B6101 1D2101",tr("出品打印单\n"));//居中放大
                        b += backheader::printer->drawText("1B40 1D2101", content_kitchen, Qt::AlignLeft);
                    }
                    b += backheader::printer->drawText("1B6405 1D5601", "");//走纸5x， 切纸
                    b=backheader::printer->setPrintCount(b, print_count);
                    // printer->asynchPrint(b, path);
                    if(backheader::syn_dataprint(b,path) == true)//成功,更新数据库打印菜品大类
                    {
                        QSqlQuery query;
                        QSqlDatabase::database().transaction();

                        query.prepare(QString("update cey_u_orderdish set ch_outflag ='Y' where ch_billno =:danhao and ch_dishno = :bianma"));
                        query.bindValue(":danhao",dayindanhao);
                        query.bindValue(":bianma",print_flag.item(i,1)->text());
                        if(query.exec())
                        {

                        }
                        else
                        {
                            QSqlDatabase::database().rollback();
                            ;
                            lds_messagebox::warning(this, tr("消息提示"),tr("更新出品打印失败!"));

                        }

                        QSqlDatabase::database().commit();
                    }
                    else//失败
                    {
                        kitchenprinte_lost();
                    }

                }//如果后台进行了厨打设置
            }
        }
        else //一桌一单===============================
        {
            int hanghao = 0;
            //IP轮寻
            QStringList iplist;
            for(int i = 0; i < print_flag.rowCount();i++)
            {
                printflag.clear();
                printflag.setQuery(QString("select ch_outflag,vch_outprint from cey_bt_dish where ch_dishno = '%1' ").arg(print_flag.item(i,1)->text()));
                if(printflag.record(0).value(0).toString() == "Y")
                {
                    if(!iplist.contains(printflag.record(0).value(1).toString()))
                    {
                        iplist << printflag.record(0).value(1).toString();
                    }
                }
            }

            float zongji = 0.00;
            QString capinxiaolei = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/out_table_bill_printtype_check").toString();
            QString caipindalei = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/out_table_bill_printseries_check").toString();
            for(int k = 0; k < iplist.count(); k++)
            {
                //只有IP的个数大于0，才厨打
                if(iplist.count() > 0)
                {
                    content_kitchen.clear();
                    QSqlQueryModel thism;
                    thism.setQuery("Select NOW()");
                    QString print_timer = thism.record(0).value(0).toDateTime().toString("yyyy-MM-dd hh:mm:ss");
                    //流水号
                    QString liushuhao;
                    str.clear();
                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/serialno_check").toString();
                    if(str == "1")
                    {
                        int a = bendandanhao.right(4).toInt()%100;
                        if(a == 0)
                        {
                            liushuhao = tr("流水号:01\n");
                        }
                        else if(a < 10)
                        {
                            liushuhao = tr("流水号:0") + QString::number(a) + "\n";
                        }
                        else
                        {
                            liushuhao = tr("流水号:") + QString::number(a)+ "\n";
                        }
                        content_kitchen.append(liushuhao);
                    }

                    //开始单号
                    str.clear();
                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/openbillno_check").toString();
                    if(str == "1")
                    {
                        content_kitchen.append(tr("开始单号:") + bendandanhao + "\n");
                    }

                    //餐桌名
                    str.clear();
                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/tablename_check").toString();
                    if(str == "1")
                    {
                        content_kitchen.append(tr("餐桌名:") + tr("Q000\n"));
                    }

                    //点菜人
                    str.clear();
                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/dishopertor_check").toString();
                    if(str == "1")
                    {
                        content_kitchen.append(tr("点菜人:") + n_func::gs_operid + "\n");
                    }

                    //点菜时间
                    str.clear();
                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/dishtime_check").toString();
                    if(str == "1")
                    {
                        content_kitchen.append(tr("点菜时间:") + table_order_2::diancaitime + "\n");
                    }

                    //打印时间
                    str.clear();
                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/printtime_check").toString();
                    if(str == "1")
                    {
                        content_kitchen.append(tr("打印时间:") + print_timer + "\n");
                    }

                    //人数
                    str.clear();
                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/person_check").toString();
                    if(str == "1")
                    {
                        content_kitchen.append(tr("人数:1\n"));
                    }

                    //表内容
                    if(str_type == "0")//80打印机
                    {
                        content_kitchen.append("----------------------------\n");
                    }
                    else
                    {
                        content_kitchen.append("--------------------------------\n");
                    }

                    QString biaotou;
                    //品名
                    str.clear();
                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/ch_dishno_check").toString();
                    if(str == "1")
                    {
                        biaotou.append(tr("品名             "));
                    }

                    //单位
                    str.clear();
                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/vch_unit_check").toString();
                    if(str == "1")
                    {
                        biaotou.append(tr("单位   "));
                    }

                    //品码
                    str.clear();
                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/vch_dishname_check").toString();
                    if(str == "1")
                    {
                        biaotou.append(tr("品码     "));
                    }

                    //数量
                    str.clear();
                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_num_check").toString();
                    if(str == "1")
                    {
                        biaotou.append(tr("数量     "));
                    }

                    //价格
                    str.clear();
                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_price_check").toString();
                    if(str == "1")
                    {
                        biaotou.append(tr("价格"));
                    }

                    content_kitchen.append(biaotou + "\n");
                    if(str_type == "0")//80打印机
                    {
                        content_kitchen.append("----------------------------\n");
                    }
                    else
                    {
                        content_kitchen.append("--------------------------------\n");
                    }

                }



                QSqlQueryModel dalei;
                if(caipindalei == "1")
                {
                    dalei.setQuery(QString("select ch_dish_typeno from cey_bt_dish_type"));
                }
                else if(capinxiaolei  == "1")
                {
                    dalei.setQuery(QString("select ch_sub_typeno  from cey_bt_dish_subtype"));
                }
                QStandardItemModel print_model ;
                for(int h = 0; h < dalei.rowCount(); h++)
                {
                    QStandardItemModel cook;
                    for(int m = 0; m < table_order_2::modelBase.rowCount(); m++)
                    {
                        printflag.clear();
                        printflag.setQuery(QString("select ch_outflag,vch_outprint from cey_bt_dish where ch_dishno = '%1' ").arg(table_order_2::modelBase.item(m,1)->text()));
                        if(table_order_2::modelBase.item(m,22)->text() == dalei.record(h).value(0).toString() && printflag.record(0).value(0).toString() == "Y" && printflag.record(0).value(1).toString() == iplist[k])
                        {
                            int row = cook.rowCount();
                            cook.setRowCount(row + 1);
                            cook.setItem(row,0,new QStandardItem(table_order_2::modelBase.item(m,0)->text()));
                            cook.setItem(row,1,new QStandardItem(table_order_2::modelBase.item(m,1)->text()));
                            cook.setItem(row,2,new QStandardItem(table_order_2::modelBase.item(m,2)->text()));
                            cook.setItem(row,3,new QStandardItem(table_order_2::modelBase.item(m,3)->text()));
                            cook.setItem(row,4,new QStandardItem(table_order_2::modelBase.item(m,4)->text()));
                            cook.setItem(row,5,new QStandardItem(table_order_2::modelBase.item(m,5)->text()));
                            cook.setItem(row,6,new QStandardItem(table_order_2::modelBase.item(m,6)->text()));
                            cook.setItem(row,7,new QStandardItem(table_order_2::modelBase.item(m,7)->text()));
                            cook.setItem(row,8,new QStandardItem(table_order_2::modelBase.item(m,8)->text()));
                            cook.setItem(row,9,new QStandardItem(table_order_2::modelBase.item(m,9)->text()));
                            cook.setItem(row,10,new QStandardItem(table_order_2::modelBase.item(m,10)->text()));
                            cook.setItem(row,11,new QStandardItem(table_order_2::modelBase.item(m,11)->text()));
                            cook.setItem(row,12,new QStandardItem(table_order_2::modelBase.item(m,12)->text()));
                            cook.setItem(row,13,new QStandardItem(table_order_2::modelBase.item(m,13)->text()));
                            cook.setItem(row,14,new QStandardItem(table_order_2::modelBase.item(m,14)->text()));
                            cook.setItem(row,15,new QStandardItem(table_order_2::modelBase.item(m,15)->text()));
                            cook.setItem(row,16,new QStandardItem(table_order_2::modelBase.item(m,16)->text()));
                            cook.setItem(row,17,new QStandardItem(table_order_2::modelBase.item(m,17)->text()));
                            cook.setItem(row,18,new QStandardItem(table_order_2::modelBase.item(m,18)->text()));
                            cook.setItem(row,19,new QStandardItem(table_order_2::modelBase.item(m,19)->text()));
                            cook.setItem(row,20,new QStandardItem(table_order_2::modelBase.item(m,20)->text()));
                            cook.setItem(row,21,new QStandardItem(table_order_2::modelBase.item(m,21)->text()));
                            cook.setItem(row,22,new QStandardItem(table_order_2::modelBase.item(m,22)->text()));

                            int row1 = print_model.rowCount();
                            print_model.setRowCount(row1 + 1);
                            print_model.setItem(row1,0,new QStandardItem(table_order_2::modelBase.item(m,0)->text()));
                            print_model.setItem(row1,1,new QStandardItem(table_order_2::modelBase.item(m,1)->text()));
                        }
                    }

                    //寻找大类
                    QString daleimingcheng;
                    QSqlQueryModel mymodel;
                    mymodel.setQuery(QString("select vch_dish_typename from cey_bt_dish_type where ch_dish_typeno = '%1' ").arg(dalei.record(h).value(0).toString()));
                    QSqlQueryModel mod_xiao;
                    mod_xiao.setQuery(QString("select vch_sub_typename from cey_bt_dish_subtype where ch_sub_typeno = '%1' ").arg(dalei.record(h).value(0).toString()));
                    if(caipindalei == "1")
                    {
                        if(str_type == "0")//80打印机
                        {
                            daleimingcheng = tr("            ---------") + mymodel.record(0).value(0).toString() + tr("---------\n");
                        }
                        else
                        {
                            daleimingcheng = tr("    ---------") + mymodel.record(0).value(0).toString() + tr("---------\n");

                        }
                        if(cook.rowCount() > 0)
                        {
                            content_kitchen.append(daleimingcheng);
                        }
                    }
                    else if(capinxiaolei == "1")
                    {
                        if(str_type == "0")//80打印机
                        {
                            daleimingcheng = tr("            ---------") + mod_xiao.record(0).value(0).toString() + tr("---------\n");
                        }
                        else
                        {
                            daleimingcheng = tr("    ---------") + mod_xiao.record(0).value(0).toString() + tr("---------\n");

                        }
                        if(cook.rowCount() > 0)
                        {
                            content_kitchen.append(daleimingcheng);
                        }
                    }

                    for(int i = 0; i < cook.rowCount(); i++)
                    {
                        //大类开始=============================================

                        if(cook.item(i,9)->text() != "Y")
                        {
                            zongji += cook.item(i,3)->text().toFloat() * cook.item(i,4)->text().toFloat() * cook.item(i,5)->text().toFloat() * 0.01 + cook.item(i,3)->text().toFloat() * cook.item(i,21)->text().toFloat() + cook.item(i,3)->text().toFloat() * cook.item(i,17)->text().toFloat() * cook.item(i,4)->text().toFloat() * 0.01;
                            //qDebug() << cook.item(i,3)->text().toFloat() * cook.item(i,4)->text().toFloat() * cook.item(i,5)->text().toFloat() * 0.01 + cook.item(i,3)->text().toFloat() * cook.item(i,21)->text().toFloat() + cook.item(i,3)->text().toFloat() * cook.item(i,17)->text().toFloat() * 0.01;
                        }
                        //本单序号
                        //要判断是否套菜
                        if(cook.item(i,9)->text() == "N")//不是套菜
                        {
                            hanghao++;
                            str.clear();
                            str =  kitchenprinter->value("w_bt_dish_Kitchenprinter_param/selfbillno_check").toString();
                            if(str == "1")
                            {
                                QString xuhao = QString::number(hanghao) + tr(".");
                                if(xuhao.length() < 3)
                                {
                                    xuhao.append(" ");
                                }

                                content_kitchen.append(xuhao);
                            };

                            //赠送标志
                            str.clear();
                            str =  kitchenprinter->value("w_bt_dish_Kitchenprinter_param/presentflag_check").toString();
                            if(str == "1")
                            {
                                if(cook.item(i,8)->text() == "Y")
                                {
                                    QString zb =("赠)");
                                    content_kitchen.append(zb);
                                }
                            };
                        }
                        else
                        {
                            str.clear();
                            str =  kitchenprinter->value("w_bt_dish_Kitchenprinter_param/out_suit_printtype_com").toString();
                            if(str == "0")//主套菜
                            {
                                if(cook.item(i,9)->text() == "P")
                                {
                                    hanghao++;
                                    str.clear();
                                    str =  kitchenprinter->value("w_bt_dish_Kitchenprinter_param/selfbillno_check").toString();
                                    if(str == "1")
                                    {
                                        QString xuhao = QString::number(hanghao) + tr(".");
                                        if(xuhao.length() < 3)
                                        {
                                            xuhao.append(" ");
                                        }

                                        content_kitchen.append(xuhao);
                                    };

                                    //赠送标志
                                    str.clear();
                                    str =  kitchenprinter->value("w_bt_dish_Kitchenprinter_param/presentflag_check").toString();
                                    if(str == "1")
                                    {
                                        if(cook.item(i,8)->text() == "Y")
                                        {
                                            QString zb =("赠)");
                                            content_kitchen.append(zb);
                                        }
                                    };
                                }
                            }
                            else if(str == "1")//套菜明细
                            {
                                if(cook.item(i,9)->text() == tr("Y"))
                                {
                                    hanghao++;
                                    str.clear();
                                    str =  kitchenprinter->value("w_bt_dish_Kitchenprinter_param/selfbillno_check").toString();
                                    if(str == "1")
                                    {
                                        QString xuhao = QString::number(hanghao) + tr(".");
                                        if(xuhao.length() < 3)
                                        {
                                            xuhao.append(" ");
                                        }

                                        content_kitchen.append(xuhao);
                                    };

                                    //赠送标志
                                    str.clear();
                                    str =  kitchenprinter->value("w_bt_dish_Kitchenprinter_param/presentflag_check").toString();
                                    if(str == "1")
                                    {
                                        if(cook.item(i,8)->text() == "Y")
                                        {
                                            QString zb =("赠)");
                                            content_kitchen.append(zb);
                                        }
                                    };
                                }
                            }
                            else//全部打印
                            {
                                hanghao++;
                                str.clear();
                                str =  kitchenprinter->value("w_bt_dish_Kitchenprinter_param/selfbillno_check").toString();
                                if(str == "1")
                                {
                                    QString xuhao = QString::number(hanghao) + tr(".");
                                    if(xuhao.length() < 3)
                                    {
                                        xuhao.append(" ");
                                    }

                                    content_kitchen.append(xuhao);
                                    if(cook.item(i,9)->text() == "Y")
                                    {
                                        content_kitchen.append(tr("套)"));
                                    }
                                };

                                //赠送标志
                                str.clear();
                                str =  kitchenprinter->value("w_bt_dish_Kitchenprinter_param/presentflag_check").toString();
                                if(str == "1")
                                {
                                    if(cook.item(i,8)->text() == "Y")
                                    {
                                        QString zb =("赠)");
                                        content_kitchen.append(zb);
                                    }
                                };
                            }
                        }

                        if(cook.item(i,9)->text() != "N")//为套菜
                        {
                            //套菜打印方式
                            str.clear();
                            str =  kitchenprinter->value("w_bt_dish_Kitchenprinter_param/out_suit_printtype_com").toString();
                            if(str == "0")//主套菜
                            {
                                if(cook.item(i,9)->text() == "P")
                                {
                                    //厨打第一行==================================================

                                    //品名
                                    str.clear();
                                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/ch_dishno_check").toString();
                                    QString hangneirong;
                                    if(str == "1")
                                    {
                                        //品名宽度
                                        str.clear();
                                        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/ch_dishno_size_spin").toString();
                                        int a = 0;
                                        a = str.toInt();
                                        QString pinming = cook.item(i,2)->text();

                                        if(pinming.length() >= a)
                                        {
                                            pinming = pinming.left(a);
                                        }

                                        if(pinming.length() < a)
                                        {
                                            int h = a * 2 - pinming.length() * 2;
                                            for(int k = 0; k < h; k++)
                                            {
                                                pinming.append(" ");
                                            }
                                        }

                                        //品名所在行
                                        str.clear();
                                        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/ch_dishno_hang_spin").toString();
                                        if(str == "1")
                                        {
                                            hangneirong.append(pinming);
                                        }
                                    }

                                    //单位
                                    str.clear();
                                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/vch_unit_check").toString();
                                    if(str == "1")
                                    {
                                        //单位宽度
                                        str.clear();
                                        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/vch_unit_size_spin").toString();
                                        int a = 0;
                                        a = str.toInt();
                                        QString danw = cook.item(i,12)->text();

                                        if(danw.length() >= a)
                                        {
                                            danw = danw.left(a);
                                        }

                                        if(danw.length() < a)
                                        {
                                            int h = a * 2 - danw.length() * 2;
                                            for(int k = 0; k < h; k++)
                                            {
                                                danw.append(" ");
                                            }
                                        }

                                        //单位所在行
                                        str.clear();
                                        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/vch_unit_hang_spin").toString();
                                        if(str == "1")
                                        {
                                            hangneirong.append(danw);
                                        }
                                    }

                                    //品码
                                    str.clear();
                                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/vch_dishname_check").toString();
                                    if(str == "1")
                                    {
                                        //品码宽度
                                        str.clear();
                                        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/vch_dishname_size_spin").toString();
                                        int a = 0;
                                        a = str.toInt();
                                        QString pinma = cook.item(i,1)->text();

                                        if(pinma.length() >= a)
                                        {
                                            pinma = pinma.left(a);
                                        }

                                        if(pinma.length() < a)
                                        {
                                            int h = a - pinma.length();
                                            for(int k = 0; k < h; k++)
                                            {
                                                pinma.append(" ");
                                            }
                                        }

                                        //单位所在行
                                        str.clear();
                                        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/vch_dishname_hang_spin").toString();
                                        if(str == "1")
                                        {
                                            hangneirong.append(pinma);
                                        }
                                    }

                                    //数量
                                    str.clear();
                                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_num_check").toString();
                                    if(str == "1")
                                    {
                                        QString shuliang;
                                        str.clear();
                                        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_num_xiaoshu_spin").toString();
                                        //数量保留小数位数
                                        if(str == "0")
                                        {
                                            shuliang = QString::number(int(cook.item(i,3)->text().toFloat()));
                                        }
                                        else if(str == "1")
                                        {
                                            shuliang = QString().sprintf("%0.1f",cook.item(i,3)->text().toFloat());
                                        }
                                        else
                                        {
                                            shuliang = QString().sprintf("%0.2f",cook.item(i,3)->text().toFloat());
                                        }

                                        //数量宽度
                                        str.clear();
                                        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_num_size_spin").toString();
                                        int a = 0;
                                        a = str.toInt();

                                        if(shuliang.length() >= a)
                                        {
                                            shuliang = shuliang.left(a);
                                        }

                                        if(shuliang.length() < a)
                                        {
                                            int h = a - shuliang.length();
                                            for(int k = 0; k < h; k++)
                                            {
                                                shuliang.append(" ");
                                            }
                                        }

                                        //数量所在行
                                        str.clear();
                                        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_num_hang_spin").toString();
                                        if(str == "1")
                                        {
                                            hangneirong.append(shuliang);
                                        }
                                    }


                                    //价格
                                    str.clear();
                                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_price_check").toString();
                                    if(str == "1")
                                    {
                                        QString jiage;
                                        str.clear();
                                        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_price_xiaoshu_spin").toString();
                                        //价格保留小数位数
                                        if(str == "0")
                                        {
                                            jiage = QString::number(int(cook.item(i,4)->text().toFloat()));
                                        }
                                        else if(str == "1")
                                        {
                                            jiage = QString().sprintf("%0.1f",cook.item(i,4)->text().toFloat());
                                        }
                                        else
                                        {
                                            jiage = QString().sprintf("%0.2f",cook.item(i,4)->text().toFloat());
                                        }

                                        //价格宽度
                                        str.clear();
                                        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_price_size_spin").toString();
                                        int a = 0;
                                        a = str.toInt();

                                        if(jiage.length() >= a)
                                        {
                                            jiage = jiage.left(a);
                                        }

                                        if(jiage.length() < a)
                                        {
                                            int h = a - jiage.length();
                                            for(int k = 0; k < h; k++)
                                            {
                                                jiage.append(" ");
                                            }
                                        }

                                        //价格所在行
                                        str.clear();
                                        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_price_hang_spin").toString();
                                        if(str == "1")
                                        {
                                            hangneirong.append(jiage);
                                        }
                                    }

                                    if(!hangneirong.isEmpty())
                                    {
                                        content_kitchen.append(hangneirong);
                                        content_kitchen.append("\n");
                                    }
                                    //厨打第一行==================================================

                                    //厨打第二行==================================================
                                    //品名
                                    str.clear();
                                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/ch_dishno_check").toString();
                                    QString hangneirong1;
                                    if(str == "1")
                                    {
                                        //品名宽度
                                        str.clear();
                                        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/ch_dishno_size_spin").toString();
                                        int a = 0;
                                        a = str.toInt();
                                        QString pinming = cook.item(i,2)->text();

                                        if(pinming.length() >= a)
                                        {
                                            pinming = pinming.left(a);
                                        }


                                        if(pinming.length() < a)
                                        {
                                            int h = a * 2 - pinming.length() * 2;
                                            for(int k = 0; k < h; k++)
                                            {
                                                pinming.append(" ");
                                            }
                                        }

                                        //品名所在行
                                        str.clear();
                                        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/ch_dishno_hang_spin").toString();
                                        if(str == "2")
                                        {
                                            hangneirong1.append(pinming);
                                        }
                                    }

                                    //单位
                                    str.clear();
                                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/vch_unit_check").toString();
                                    if(str == "1")
                                    {
                                        //单位宽度
                                        str.clear();
                                        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/vch_unit_size_spin").toString();
                                        int a = 0;
                                        a = str.toInt();
                                        QString danw = cook.item(i,12)->text();

                                        if(danw.length() >= a)
                                        {
                                            danw = danw.left(a);
                                        }

                                        if(danw.length() < a)
                                        {
                                            int h = a * 2 - danw.length() * 2;
                                            for(int k = 0; k < h; k++)
                                            {
                                                danw.append(" ");
                                            }
                                        }

                                        //单位所在行
                                        str.clear();
                                        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/vch_unit_hang_spin").toString();
                                        if(str == "2")
                                        {
                                            hangneirong1.append(danw);
                                        }
                                    }

                                    //品码
                                    str.clear();
                                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/vch_dishname_check").toString();
                                    if(str == "1")
                                    {
                                        //品码宽度
                                        str.clear();
                                        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/vch_dishname_size_spin").toString();
                                        int a = 0;
                                        a = str.toInt();
                                        QString pinma = cook.item(i,1)->text();

                                        if(pinma.length() >= a)
                                        {
                                            pinma = pinma.left(a);
                                        }

                                        if(pinma.length() < a)
                                        {
                                            int h = a - pinma.length();
                                            for(int k = 0; k < h; k++)
                                            {
                                                pinma.append(" ");
                                            }
                                        }

                                        //单位所在行
                                        str.clear();
                                        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/vch_dishname_hang_spin").toString();
                                        if(str == "2")
                                        {
                                            hangneirong1.append(pinma);
                                        }
                                    }

                                    //数量
                                    str.clear();
                                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_num_check").toString();
                                    if(str == "1")
                                    {
                                        QString shuliang;
                                        str.clear();
                                        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_num_xiaoshu_spin").toString();
                                        //数量保留小数位数
                                        if(str == "0")
                                        {
                                            shuliang = QString::number(int(cook.item(i,3)->text().toFloat()));
                                        }
                                        else if(str == "1")
                                        {
                                            shuliang = QString().sprintf("%0.1f",cook.item(i,3)->text().toFloat());
                                        }
                                        else
                                        {
                                            shuliang = QString().sprintf("%0.2f",cook.item(i,3)->text().toFloat());
                                        }

                                        //数量宽度
                                        str.clear();
                                        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_num_size_spin").toString();
                                        int a = 0;
                                        a = str.toInt();

                                        if(shuliang.length() >= a)
                                        {
                                            shuliang = shuliang.left(a);
                                        }

                                        if(shuliang.length() < a)
                                        {
                                            int h = a - shuliang.length();
                                            for(int k = 0; k < h; k++)
                                            {
                                                shuliang.append(" ");
                                            }
                                        }

                                        //数量所在行
                                        str.clear();
                                        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_num_hang_spin").toString();
                                        if(str == "2")
                                        {
                                            hangneirong1.append(shuliang);
                                        }
                                    }


                                    //价格
                                    str.clear();
                                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_price_check").toString();
                                    if(str == "1")
                                    {
                                        QString jiage;
                                        str.clear();
                                        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_price_xiaoshu_spin").toString();
                                        //价格保留小数位数
                                        if(str == "0")
                                        {
                                            jiage = QString::number(int(cook.item(i,4)->text().toFloat()));
                                        }
                                        else if(str == "1")
                                        {
                                            jiage = QString().sprintf("%0.1f",cook.item(i,4)->text().toFloat());
                                        }
                                        else
                                        {
                                            jiage = QString().sprintf("%0.2f",cook.item(i,4)->text().toFloat());
                                        }

                                        //价格宽度
                                        str.clear();
                                        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_price_size_spin").toString();
                                        int a = 0;
                                        a = str.toInt();

                                        if(jiage.length() >= a)
                                        {
                                            jiage = jiage.left(a);
                                        }

                                        if(jiage.length() < a)
                                        {
                                            int h = a - jiage.length();
                                            for(int k = 0; k < h; k++)
                                            {
                                                jiage.append(" ");
                                            }
                                        }

                                        //价格所在行
                                        str.clear();
                                        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_price_hang_spin").toString();
                                        if(str == "2")
                                        {
                                            hangneirong1.append(jiage);
                                        }
                                    }

                                    if(!hangneirong1.isEmpty())
                                    {
                                        content_kitchen.append(hangneirong1);
                                        content_kitchen.append("\n");
                                    }

                                    //厨打第二行==================================================
                                }
                            }
                            else if(str == "1")//套菜明细
                            {
                                if(cook.item(i,9)->text() == "Y")
                                {
                                    QString taobiao = tr("套)");
                                    content_kitchen.append(taobiao);
                                    //厨打第一行==================================================

                                    //品名
                                    str.clear();
                                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/ch_dishno_check").toString();
                                    QString hangneirong;
                                    if(str == "1")
                                    {
                                        //品名宽度
                                        str.clear();
                                        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/ch_dishno_size_spin").toString();
                                        int a = 0;
                                        a = str.toInt();
                                        QString pinming = cook.item(i,2)->text();

                                        if(pinming.length() >= a)
                                        {
                                            pinming = pinming.left(a);
                                        }

                                        if(pinming.length() < a)
                                        {
                                            int h = a * 2 - pinming.length() * 2;
                                            for(int k = 0; k < h; k++)
                                            {
                                                pinming.append(" ");
                                            }
                                        }

                                        //品名所在行
                                        str.clear();
                                        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/ch_dishno_hang_spin").toString();
                                        if(str == "1")
                                        {
                                            hangneirong.append(pinming);
                                        }
                                    }

                                    //单位
                                    str.clear();
                                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/vch_unit_check").toString();
                                    if(str == "1")
                                    {
                                        //单位宽度
                                        str.clear();
                                        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/vch_unit_size_spin").toString();
                                        int a = 0;
                                        a = str.toInt();
                                        QString danw = cook.item(i,12)->text();

                                        if(danw.length() >= a)
                                        {
                                            danw = danw.left(a);
                                        }

                                        if(danw.length() < a)
                                        {
                                            int h = a * 2 - danw.length() * 2;
                                            for(int k = 0; k < h; k++)
                                            {
                                                danw.append(" ");
                                            }
                                        }

                                        //单位所在行
                                        str.clear();
                                        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/vch_unit_hang_spin").toString();
                                        if(str == "1")
                                        {
                                            hangneirong.append(danw);
                                        }
                                    }

                                    //品码
                                    str.clear();
                                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/vch_dishname_check").toString();
                                    if(str == "1")
                                    {
                                        //品码宽度
                                        str.clear();
                                        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/vch_dishname_size_spin").toString();
                                        int a = 0;
                                        a = str.toInt();
                                        QString pinma = cook.item(i,1)->text();

                                        if(pinma.length() >= a)
                                        {
                                            pinma = pinma.left(a);
                                        }

                                        if(pinma.length() < a)
                                        {
                                            int h = a - pinma.length();
                                            for(int k = 0; k < h; k++)
                                            {
                                                pinma.append(" ");
                                            }
                                        }

                                        //单位所在行
                                        str.clear();
                                        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/vch_dishname_hang_spin").toString();
                                        if(str == "1")
                                        {
                                            hangneirong.append(pinma);
                                        }
                                    }

                                    //数量
                                    str.clear();
                                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_num_check").toString();
                                    if(str == "1")
                                    {
                                        QString shuliang;
                                        str.clear();
                                        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_num_xiaoshu_spin").toString();
                                        //数量保留小数位数
                                        if(str == "0")
                                        {
                                            shuliang = QString::number(int(cook.item(i,3)->text().toFloat()));
                                        }
                                        else if(str == "1")
                                        {
                                            shuliang = QString().sprintf("%0.1f",cook.item(i,3)->text().toFloat());
                                        }
                                        else
                                        {
                                            shuliang = QString().sprintf("%0.2f",cook.item(i,3)->text().toFloat());
                                        }

                                        //数量宽度
                                        str.clear();
                                        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_num_size_spin").toString();
                                        int a = 0;
                                        a = str.toInt();

                                        if(shuliang.length() >= a)
                                        {
                                            shuliang = shuliang.left(a);
                                        }

                                        if(shuliang.length() < a)
                                        {
                                            int h = a - shuliang.length();
                                            for(int k = 0; k < h; k++)
                                            {
                                                shuliang.append(" ");
                                            }
                                        }

                                        //数量所在行
                                        str.clear();
                                        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_num_hang_spin").toString();
                                        if(str == "1")
                                        {
                                            hangneirong.append(shuliang);
                                        }
                                    }


                                    //价格
                                    str.clear();
                                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_price_check").toString();
                                    if(str == "1")
                                    {
                                        QString jiage;
                                        str.clear();
                                        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_price_xiaoshu_spin").toString();
                                        //价格保留小数位数
                                        if(str == "0")
                                        {
                                            jiage = QString::number(int(cook.item(i,4)->text().toFloat()));
                                        }
                                        else if(str == "1")
                                        {
                                            jiage = QString().sprintf("%0.1f",cook.item(i,4)->text().toFloat());
                                        }
                                        else
                                        {
                                            jiage = QString().sprintf("%0.2f",cook.item(i,4)->text().toFloat());
                                        }

                                        //价格宽度
                                        str.clear();
                                        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_price_size_spin").toString();
                                        int a = 0;
                                        a = str.toInt();

                                        if(jiage.length() >= a)
                                        {
                                            jiage = jiage.left(a);
                                        }

                                        if(jiage.length() < a)
                                        {
                                            int h = a - jiage.length();
                                            for(int k = 0; k < h; k++)
                                            {
                                                jiage.append(" ");
                                            }
                                        }

                                        //价格所在行
                                        str.clear();
                                        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_price_hang_spin").toString();
                                        if(str == "1")
                                        {
                                            hangneirong.append(jiage);
                                        }
                                    }

                                    if(!hangneirong.isEmpty())
                                    {
                                        content_kitchen.append(hangneirong);
                                        content_kitchen.append("\n");
                                    }
                                    //厨打第一行==================================================

                                    //厨打第二行==================================================
                                    //品名
                                    str.clear();
                                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/ch_dishno_check").toString();
                                    QString hangneirong1;
                                    if(str == "1")
                                    {
                                        //品名宽度
                                        str.clear();
                                        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/ch_dishno_size_spin").toString();
                                        int a = 0;
                                        a = str.toInt();
                                        QString pinming = cook.item(i,2)->text();

                                        if(pinming.length() >= a)
                                        {
                                            pinming = pinming.left(a);
                                        }


                                        if(pinming.length() < a)
                                        {
                                            int h = a * 2 - pinming.length() * 2;
                                            for(int k = 0; k < h; k++)
                                            {
                                                pinming.append(" ");
                                            }
                                        }

                                        //品名所在行
                                        str.clear();
                                        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/ch_dishno_hang_spin").toString();
                                        if(str == "2")
                                        {
                                            hangneirong1.append(pinming);
                                        }
                                    }

                                    //单位
                                    str.clear();
                                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/vch_unit_check").toString();
                                    if(str == "1")
                                    {
                                        //单位宽度
                                        str.clear();
                                        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/vch_unit_size_spin").toString();
                                        int a = 0;
                                        a = str.toInt();
                                        QString danw = cook.item(i,12)->text();

                                        if(danw.length() >= a)
                                        {
                                            danw = danw.left(a);
                                        }

                                        if(danw.length() < a)
                                        {
                                            int h = a * 2 - danw.length() * 2;
                                            for(int k = 0; k < h; k++)
                                            {
                                                danw.append(" ");
                                            }
                                        }

                                        //单位所在行
                                        str.clear();
                                        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/vch_unit_hang_spin").toString();
                                        if(str == "2")
                                        {
                                            hangneirong1.append(danw);
                                        }
                                    }

                                    //品码
                                    str.clear();
                                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/vch_dishname_check").toString();
                                    if(str == "1")
                                    {
                                        //品码宽度
                                        str.clear();
                                        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/vch_dishname_size_spin").toString();
                                        int a = 0;
                                        a = str.toInt();
                                        QString pinma = cook.item(i,1)->text();

                                        if(pinma.length() >= a)
                                        {
                                            pinma = pinma.left(a);
                                        }

                                        if(pinma.length() < a)
                                        {
                                            int h = a - pinma.length();
                                            for(int k = 0; k < h; k++)
                                            {
                                                pinma.append(" ");
                                            }
                                        }

                                        //单位所在行
                                        str.clear();
                                        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/vch_dishname_hang_spin").toString();
                                        if(str == "2")
                                        {
                                            hangneirong1.append(pinma);
                                        }
                                    }

                                    //数量
                                    str.clear();
                                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_num_check").toString();
                                    if(str == "1")
                                    {
                                        QString shuliang;
                                        str.clear();
                                        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_num_xiaoshu_spin").toString();
                                        //数量保留小数位数
                                        if(str == "0")
                                        {
                                            shuliang = QString::number(int(cook.item(i,3)->text().toFloat()));
                                        }
                                        else if(str == "1")
                                        {
                                            shuliang = QString().sprintf("%0.1f",cook.item(i,3)->text().toFloat());
                                        }
                                        else
                                        {
                                            shuliang = QString().sprintf("%0.2f",cook.item(i,3)->text().toFloat());
                                        }

                                        //数量宽度
                                        str.clear();
                                        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_num_size_spin").toString();
                                        int a = 0;
                                        a = str.toInt();

                                        if(shuliang.length() >= a)
                                        {
                                            shuliang = shuliang.left(a);
                                        }

                                        if(shuliang.length() < a)
                                        {
                                            int h = a - shuliang.length();
                                            for(int k = 0; k < h; k++)
                                            {
                                                shuliang.append(" ");
                                            }
                                        }

                                        //数量所在行
                                        str.clear();
                                        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_num_hang_spin").toString();
                                        if(str == "2")
                                        {
                                            hangneirong1.append(shuliang);
                                        }
                                    }


                                    //价格
                                    str.clear();
                                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_price_check").toString();
                                    if(str == "1")
                                    {
                                        QString jiage;
                                        str.clear();
                                        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_price_xiaoshu_spin").toString();
                                        //价格保留小数位数
                                        if(str == "0")
                                        {
                                            jiage = QString::number(int(cook.item(i,4)->text().toFloat()));
                                        }
                                        else if(str == "1")
                                        {
                                            jiage = QString().sprintf("%0.1f",cook.item(i,4)->text().toFloat());
                                        }
                                        else
                                        {
                                            jiage = QString().sprintf("%0.2f",cook.item(i,4)->text().toFloat());
                                        }

                                        //价格宽度
                                        str.clear();
                                        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_price_size_spin").toString();
                                        int a = 0;
                                        a = str.toInt();

                                        if(jiage.length() >= a)
                                        {
                                            jiage = jiage.left(a);
                                        }

                                        if(jiage.length() < a)
                                        {
                                            int h = a - jiage.length();
                                            for(int k = 0; k < h; k++)
                                            {
                                                jiage.append(" ");
                                            }
                                        }

                                        //价格所在行
                                        str.clear();
                                        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_price_hang_spin").toString();
                                        if(str == "2")
                                        {
                                            hangneirong1.append(jiage);
                                        }
                                    }

                                    if(!hangneirong1.isEmpty())
                                    {
                                        content_kitchen.append(hangneirong1);
                                        content_kitchen.append("\n");
                                    }

                                    //厨打第二行==================================================
                                }
                            }
                            else //全部打印
                            {
                                //厨打第一行==================================================

                                //品名
                                str.clear();
                                str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/ch_dishno_check").toString();
                                QString hangneirong;
                                if(str == "1")
                                {
                                    //品名宽度
                                    str.clear();
                                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/ch_dishno_size_spin").toString();
                                    int a = 0;
                                    a = str.toInt();
                                    QString pinming = cook.item(i,2)->text();

                                    if(pinming.length() >= a)
                                    {
                                        pinming = pinming.left(a);
                                    }

                                    if(pinming.length() < a)
                                    {
                                        int h = a * 2 - pinming.length() * 2;
                                        for(int k = 0; k < h; k++)
                                        {
                                            pinming.append(" ");
                                        }
                                    }

                                    //品名所在行
                                    str.clear();
                                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/ch_dishno_hang_spin").toString();
                                    if(str == "1")
                                    {
                                        hangneirong.append(pinming);
                                    }
                                }

                                //单位
                                str.clear();
                                str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/vch_unit_check").toString();
                                if(str == "1")
                                {
                                    //单位宽度
                                    str.clear();
                                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/vch_unit_size_spin").toString();
                                    int a = 0;
                                    a = str.toInt();
                                    QString danw = cook.item(i,12)->text();

                                    if(danw.length() >= a)
                                    {
                                        danw = danw.left(a);
                                    }

                                    if(danw.length() < a)
                                    {
                                        int h = a * 2 - danw.length() * 2;
                                        for(int k = 0; k < h; k++)
                                        {
                                            danw.append(" ");
                                        }
                                    }

                                    //单位所在行
                                    str.clear();
                                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/vch_unit_hang_spin").toString();
                                    if(str == "1")
                                    {
                                        hangneirong.append(danw);
                                    }
                                }

                                //品码
                                str.clear();
                                str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/vch_dishname_check").toString();
                                if(str == "1")
                                {
                                    //品码宽度
                                    str.clear();
                                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/vch_dishname_size_spin").toString();
                                    int a = 0;
                                    a = str.toInt();
                                    QString pinma = cook.item(i,1)->text();

                                    if(pinma.length() >= a)
                                    {
                                        pinma = pinma.left(a);
                                    }

                                    if(pinma.length() < a)
                                    {
                                        int h = a - pinma.length();
                                        for(int k = 0; k < h; k++)
                                        {
                                            pinma.append(" ");
                                        }
                                    }

                                    //单位所在行
                                    str.clear();
                                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/vch_dishname_hang_spin").toString();
                                    if(str == "1")
                                    {
                                        hangneirong.append(pinma);
                                    }
                                }

                                //数量
                                str.clear();
                                str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_num_check").toString();
                                if(str == "1")
                                {
                                    QString shuliang;
                                    str.clear();
                                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_num_xiaoshu_spin").toString();
                                    //数量保留小数位数
                                    if(str == "0")
                                    {
                                        shuliang = QString::number(int(cook.item(i,3)->text().toFloat()));
                                    }
                                    else if(str == "1")
                                    {
                                        shuliang = QString().sprintf("%0.1f",cook.item(i,3)->text().toFloat());
                                    }
                                    else
                                    {
                                        shuliang = QString().sprintf("%0.2f",cook.item(i,3)->text().toFloat());
                                    }

                                    //数量宽度
                                    str.clear();
                                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_num_size_spin").toString();
                                    int a = 0;
                                    a = str.toInt();

                                    if(shuliang.length() >= a)
                                    {
                                        shuliang = shuliang.left(a);
                                    }

                                    if(shuliang.length() < a)
                                    {
                                        int h = a - shuliang.length();
                                        for(int k = 0; k < h; k++)
                                        {
                                            shuliang.append(" ");
                                        }
                                    }

                                    //数量所在行
                                    str.clear();
                                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_num_hang_spin").toString();
                                    if(str == "1")
                                    {
                                        hangneirong.append(shuliang);
                                    }
                                }


                                //价格
                                str.clear();
                                str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_price_check").toString();
                                if(str == "1")
                                {
                                    QString jiage;
                                    str.clear();
                                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_price_xiaoshu_spin").toString();
                                    //价格保留小数位数
                                    if(str == "0")
                                    {
                                        jiage = QString::number(int(cook.item(i,4)->text().toFloat()));
                                    }
                                    else if(str == "1")
                                    {
                                        jiage = QString().sprintf("%0.1f",cook.item(i,4)->text().toFloat());
                                    }
                                    else
                                    {
                                        jiage = QString().sprintf("%0.2f",cook.item(i,4)->text().toFloat());
                                    }

                                    //价格宽度
                                    str.clear();
                                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_price_size_spin").toString();
                                    int a = 0;
                                    a = str.toInt();

                                    if(jiage.length() >= a)
                                    {
                                        jiage = jiage.left(a);
                                    }

                                    if(jiage.length() < a)
                                    {
                                        int h = a - jiage.length();
                                        for(int k = 0; k < h; k++)
                                        {
                                            jiage.append(" ");
                                        }
                                    }

                                    //价格所在行
                                    str.clear();
                                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_price_hang_spin").toString();
                                    if(str == "1")
                                    {
                                        hangneirong.append(jiage);
                                    }
                                }

                                if(!hangneirong.isEmpty())
                                {
                                    content_kitchen.append(hangneirong);
                                    content_kitchen.append("\n");
                                }
                                //厨打第一行==================================================

                                //厨打第二行==================================================
                                //品名
                                str.clear();
                                str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/ch_dishno_check").toString();
                                QString hangneirong1;
                                if(str == "1")
                                {
                                    //品名宽度
                                    str.clear();
                                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/ch_dishno_size_spin").toString();
                                    int a = 0;
                                    a = str.toInt();
                                    QString pinming = cook.item(i,2)->text();

                                    if(pinming.length() >= a)
                                    {
                                        pinming = pinming.left(a);
                                    }


                                    if(pinming.length() < a)
                                    {
                                        int h = a * 2 - pinming.length() * 2;
                                        for(int k = 0; k < h; k++)
                                        {
                                            pinming.append(" ");
                                        }
                                    }

                                    //品名所在行
                                    str.clear();
                                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/ch_dishno_hang_spin").toString();
                                    if(str == "2")
                                    {
                                        hangneirong1.append(pinming);
                                    }
                                }

                                //单位
                                str.clear();
                                str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/vch_unit_check").toString();
                                if(str == "1")
                                {
                                    //单位宽度
                                    str.clear();
                                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/vch_unit_size_spin").toString();
                                    int a = 0;
                                    a = str.toInt();
                                    QString danw = cook.item(i,12)->text();

                                    if(danw.length() >= a)
                                    {
                                        danw = danw.left(a);
                                    }

                                    if(danw.length() < a)
                                    {
                                        int h = a * 2 - danw.length() * 2;
                                        for(int k = 0; k < h; k++)
                                        {
                                            danw.append(" ");
                                        }
                                    }

                                    //单位所在行
                                    str.clear();
                                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/vch_unit_hang_spin").toString();
                                    if(str == "2")
                                    {
                                        hangneirong1.append(danw);
                                    }
                                }

                                //品码
                                str.clear();
                                str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/vch_dishname_check").toString();
                                if(str == "1")
                                {
                                    //品码宽度
                                    str.clear();
                                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/vch_dishname_size_spin").toString();
                                    int a = 0;
                                    a = str.toInt();
                                    QString pinma = cook.item(i,1)->text();

                                    if(pinma.length() >= a)
                                    {
                                        pinma = pinma.left(a);
                                    }

                                    if(pinma.length() < a)
                                    {
                                        int h = a - pinma.length();
                                        for(int k = 0; k < h; k++)
                                        {
                                            pinma.append(" ");
                                        }
                                    }

                                    //单位所在行
                                    str.clear();
                                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/vch_dishname_hang_spin").toString();
                                    if(str == "2")
                                    {
                                        hangneirong1.append(pinma);
                                    }
                                }

                                //数量
                                str.clear();
                                str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_num_check").toString();
                                if(str == "1")
                                {
                                    QString shuliang;
                                    str.clear();
                                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_num_xiaoshu_spin").toString();
                                    //数量保留小数位数
                                    if(str == "0")
                                    {
                                        shuliang = QString::number(int(cook.item(i,3)->text().toFloat()));
                                    }
                                    else if(str == "1")
                                    {
                                        shuliang = QString().sprintf("%0.1f",cook.item(i,3)->text().toFloat());
                                    }
                                    else
                                    {
                                        shuliang = QString().sprintf("%0.2f",cook.item(i,3)->text().toFloat());
                                    }

                                    //数量宽度
                                    str.clear();
                                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_num_size_spin").toString();
                                    int a = 0;
                                    a = str.toInt();

                                    if(shuliang.length() >= a)
                                    {
                                        shuliang = shuliang.left(a);
                                    }

                                    if(shuliang.length() < a)
                                    {
                                        int h = a - shuliang.length();
                                        for(int k = 0; k < h; k++)
                                        {
                                            shuliang.append(" ");
                                        }
                                    }

                                    //数量所在行
                                    str.clear();
                                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_num_hang_spin").toString();
                                    if(str == "2")
                                    {
                                        hangneirong1.append(shuliang);
                                    }
                                }


                                //价格
                                str.clear();
                                str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_price_check").toString();
                                if(str == "1")
                                {
                                    QString jiage;
                                    str.clear();
                                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_price_xiaoshu_spin").toString();
                                    //价格保留小数位数
                                    if(str == "0")
                                    {
                                        jiage = QString::number(int(cook.item(i,4)->text().toFloat()));
                                    }
                                    else if(str == "1")
                                    {
                                        jiage = QString().sprintf("%0.1f",cook.item(i,4)->text().toFloat());
                                    }
                                    else
                                    {
                                        jiage = QString().sprintf("%0.2f",cook.item(i,4)->text().toFloat());
                                    }

                                    //价格宽度
                                    str.clear();
                                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_price_size_spin").toString();
                                    int a = 0;
                                    a = str.toInt();

                                    if(jiage.length() >= a)
                                    {
                                        jiage = jiage.left(a);
                                    }

                                    if(jiage.length() < a)
                                    {
                                        int h = a - jiage.length();
                                        for(int k = 0; k < h; k++)
                                        {
                                            jiage.append(" ");
                                        }
                                    }

                                    //价格所在行
                                    str.clear();
                                    str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_price_hang_spin").toString();
                                    if(str == "2")
                                    {
                                        hangneirong1.append(jiage);
                                    }
                                }

                                if(!hangneirong1.isEmpty())
                                {
                                    content_kitchen.append(hangneirong1);
                                    content_kitchen.append("\n");
                                }

                                //厨打第二行==================================================
                            }
                        }
                        else//普通菜
                        {
                            //厨打第一行==================================================

                            //品名
                            str.clear();
                            str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/ch_dishno_check").toString();
                            QString hangneirong;
                            if(str == "1")
                            {
                                //品名宽度
                                str.clear();
                                str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/ch_dishno_size_spin").toString();
                                int a = 0;
                                a = str.toInt();
                                QString pinming = cook.item(i,2)->text();

                                if(pinming.length() >= a)
                                {
                                    pinming = pinming.left(a);
                                }

                                if(pinming.length() < a)
                                {
                                    int h = a * 2 - pinming.length() * 2;
                                    for(int k = 0; k < h; k++)
                                    {
                                        pinming.append(" ");
                                    }
                                }

                                //品名所在行
                                str.clear();
                                str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/ch_dishno_hang_spin").toString();
                                if(str == "1")
                                {
                                    hangneirong.append(pinming);
                                }
                            }

                            //单位
                            str.clear();
                            str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/vch_unit_check").toString();
                            if(str == "1")
                            {
                                //单位宽度
                                str.clear();
                                str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/vch_unit_size_spin").toString();
                                int a = 0;
                                a = str.toInt();
                                QString danw = cook.item(i,12)->text();

                                if(danw.length() >= a)
                                {
                                    danw = danw.left(a);
                                }

                                if(danw.length() < a)
                                {
                                    int h = a * 2 - danw.length() * 2;
                                    for(int k = 0; k < h; k++)
                                    {
                                        danw.append(" ");
                                    }
                                }

                                //单位所在行
                                str.clear();
                                str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/vch_unit_hang_spin").toString();
                                if(str == "1")
                                {
                                    hangneirong.append(danw);
                                }
                            }

                            //品码
                            str.clear();
                            str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/vch_dishname_check").toString();
                            if(str == "1")
                            {
                                //品码宽度
                                str.clear();
                                str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/vch_dishname_size_spin").toString();
                                int a = 0;
                                a = str.toInt();
                                QString pinma = cook.item(i,1)->text();

                                if(pinma.length() >= a)
                                {
                                    pinma = pinma.left(a);
                                }

                                if(pinma.length() < a)
                                {
                                    int h = a - pinma.length();
                                    for(int k = 0; k < h; k++)
                                    {
                                        pinma.append(" ");
                                    }
                                }

                                //单位所在行
                                str.clear();
                                str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/vch_dishname_hang_spin").toString();
                                if(str == "1")
                                {
                                    hangneirong.append(pinma);
                                }
                            }

                            //数量
                            str.clear();
                            str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_num_check").toString();
                            if(str == "1")
                            {
                                QString shuliang;
                                str.clear();
                                str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_num_xiaoshu_spin").toString();
                                //数量保留小数位数
                                if(str == "0")
                                {
                                    shuliang = QString::number(int(cook.item(i,3)->text().toFloat()));
                                }
                                else if(str == "1")
                                {
                                    shuliang = QString().sprintf("%0.1f",cook.item(i,3)->text().toFloat());
                                }
                                else
                                {
                                    shuliang = QString().sprintf("%0.2f",cook.item(i,3)->text().toFloat());
                                }

                                //数量宽度
                                str.clear();
                                str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_num_size_spin").toString();
                                int a = 0;
                                a = str.toInt();

                                if(shuliang.length() >= a)
                                {
                                    shuliang = shuliang.left(a);
                                }

                                if(shuliang.length() < a)
                                {
                                    int h = a - shuliang.length();
                                    for(int k = 0; k < h; k++)
                                    {
                                        shuliang.append(" ");
                                    }
                                }

                                //数量所在行
                                str.clear();
                                str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_num_hang_spin").toString();
                                if(str == "1")
                                {
                                    hangneirong.append(shuliang);
                                }
                            }


                            //价格
                            str.clear();
                            str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_price_check").toString();
                            if(str == "1")
                            {
                                QString jiage;
                                str.clear();
                                str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_price_xiaoshu_spin").toString();
                                //价格保留小数位数
                                if(str == "0")
                                {
                                    jiage = QString::number(int(cook.item(i,4)->text().toFloat()));
                                }
                                else if(str == "1")
                                {
                                    jiage = QString().sprintf("%0.1f",cook.item(i,4)->text().toFloat());
                                }
                                else
                                {
                                    jiage = QString().sprintf("%0.2f",cook.item(i,4)->text().toFloat());
                                }

                                //价格宽度
                                str.clear();
                                str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_price_size_spin").toString();
                                int a = 0;
                                a = str.toInt();

                                if(jiage.length() >= a)
                                {
                                    jiage = jiage.left(a);
                                }

                                if(jiage.length() < a)
                                {
                                    int h = a - jiage.length();
                                    for(int k = 0; k < h; k++)
                                    {
                                        jiage.append(" ");
                                    }
                                }

                                //价格所在行
                                str.clear();
                                str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_price_hang_spin").toString();
                                if(str == "1")
                                {
                                    hangneirong.append(jiage);
                                }
                            }

                            if(!hangneirong.isEmpty())
                            {
                                content_kitchen.append(hangneirong);
                                content_kitchen.append("\n");
                            }
                            //厨打第一行==================================================

                            //厨打第二行==================================================
                            //品名
                            str.clear();
                            str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/ch_dishno_check").toString();
                            QString hangneirong1;
                            if(str == "1")
                            {
                                //品名宽度
                                str.clear();
                                str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/ch_dishno_size_spin").toString();
                                int a = 0;
                                a = str.toInt();
                                QString pinming = cook.item(i,2)->text();

                                if(pinming.length() >= a)
                                {
                                    pinming = pinming.left(a);
                                }


                                if(pinming.length() < a)
                                {
                                    int h = a * 2 - pinming.length() * 2;
                                    for(int k = 0; k < h; k++)
                                    {
                                        pinming.append(" ");
                                    }
                                }

                                //品名所在行
                                str.clear();
                                str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/ch_dishno_hang_spin").toString();
                                if(str == "2")
                                {
                                    hangneirong1.append(pinming);
                                }
                            }

                            //单位
                            str.clear();
                            str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/vch_unit_check").toString();
                            if(str == "1")
                            {
                                //单位宽度
                                str.clear();
                                str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/vch_unit_size_spin").toString();
                                int a = 0;
                                a = str.toInt();
                                QString danw = cook.item(i,12)->text();

                                if(danw.length() >= a)
                                {
                                    danw = danw.left(a);
                                }

                                if(danw.length() < a)
                                {
                                    int h = a * 2 - danw.length() * 2;
                                    for(int k = 0; k < h; k++)
                                    {
                                        danw.append(" ");
                                    }
                                }

                                //单位所在行
                                str.clear();
                                str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/vch_unit_hang_spin").toString();
                                if(str == "2")
                                {
                                    hangneirong1.append(danw);
                                }
                            }

                            //品码
                            str.clear();
                            str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/vch_dishname_check").toString();
                            if(str == "1")
                            {
                                //品码宽度
                                str.clear();
                                str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/vch_dishname_size_spin").toString();
                                int a = 0;
                                a = str.toInt();
                                QString pinma = cook.item(i,1)->text();

                                if(pinma.length() >= a)
                                {
                                    pinma = pinma.left(a);
                                }

                                if(pinma.length() < a)
                                {
                                    int h = a - pinma.length();
                                    for(int k = 0; k < h; k++)
                                    {
                                        pinma.append(" ");
                                    }
                                }

                                //单位所在行
                                str.clear();
                                str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/vch_dishname_hang_spin").toString();
                                if(str == "2")
                                {
                                    hangneirong1.append(pinma);
                                }
                            }

                            //数量
                            str.clear();
                            str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_num_check").toString();
                            if(str == "1")
                            {
                                QString shuliang;
                                str.clear();
                                str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_num_xiaoshu_spin").toString();
                                //数量保留小数位数
                                if(str == "0")
                                {
                                    shuliang = QString::number(int(cook.item(i,3)->text().toFloat()));
                                }
                                else if(str == "1")
                                {
                                    shuliang = QString().sprintf("%0.1f",cook.item(i,3)->text().toFloat());
                                }
                                else
                                {
                                    shuliang = QString().sprintf("%0.2f",cook.item(i,3)->text().toFloat());
                                }

                                //数量宽度
                                str.clear();
                                str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_num_size_spin").toString();
                                int a = 0;
                                a = str.toInt();

                                if(shuliang.length() >= a)
                                {
                                    shuliang = shuliang.left(a);
                                }

                                if(shuliang.length() < a)
                                {
                                    int h = a - shuliang.length();
                                    for(int k = 0; k < h; k++)
                                    {
                                        shuliang.append(" ");
                                    }
                                }

                                //数量所在行
                                str.clear();
                                str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_num_hang_spin").toString();
                                if(str == "2")
                                {
                                    hangneirong1.append(shuliang);
                                }
                            }


                            //价格
                            str.clear();
                            str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_price_check").toString();
                            if(str == "1")
                            {
                                QString jiage;
                                str.clear();
                                str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_price_xiaoshu_spin").toString();
                                //价格保留小数位数
                                if(str == "0")
                                {
                                    jiage = QString::number(int(cook.item(i,4)->text().toFloat()));
                                }
                                else if(str == "1")
                                {
                                    jiage = QString().sprintf("%0.1f",cook.item(i,4)->text().toFloat());
                                }
                                else
                                {
                                    jiage = QString().sprintf("%0.2f",cook.item(i,4)->text().toFloat());
                                }

                                //价格宽度
                                str.clear();
                                str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_price_size_spin").toString();
                                int a = 0;
                                a = str.toInt();

                                if(jiage.length() >= a)
                                {
                                    jiage = jiage.left(a);
                                }

                                if(jiage.length() < a)
                                {
                                    int h = a - jiage.length();
                                    for(int k = 0; k < h; k++)
                                    {
                                        jiage.append(" ");
                                    }
                                }

                                //价格所在行
                                str.clear();
                                str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/num_price_hang_spin").toString();
                                if(str == "2")
                                {
                                    hangneirong1.append(jiage);
                                }
                            }

                            if(!hangneirong1.isEmpty())
                            {
                                content_kitchen.append(hangneirong1);
                                content_kitchen.append("\n");
                            }

                            //厨打第二行==================================================
                        }

                        //大类结束============================================
                    }
                }

                //打印大类内容======================

                //菜品金额合计
                str.clear();
                str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/dishsum_numprice_check").toString();
                if(str == "1")
                {
                    if(str_type == "0")//80打印机
                    {
                        content_kitchen.append("----------------------------\n");
                    }
                    else
                    {
                        content_kitchen.append("--------------------------------\n");
                    }

                    content_kitchen.append(tr("菜品合计金额:") + QString().sprintf("%0.2f",zongji));
                }
                //打印份数
                str.clear();
                str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/kitchen_num_spin").toString();
                int print_count = str.toInt();
                blockIns b;
                QString path = iplist[k] + ":9100";
                louda_ip = iplist[k];

                //                public_mainprinter *chuda_print_yizhuo = new public_mainprinter();
                louda_neirong = content_kitchen;
                louda_title = tr("出品打印单");
                str.clear();
                str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/printer_fontsize_com").toString();
                if(str == "1")//普通字体
                {
                    b+=backheader::printer->drawText("1B40","");
                    b+=backheader::printer->drawText("1B40 1B6101 1C5700",tr("出品打印单\n"));//居中放大
                    b += backheader::printer->drawText("1B40 1C5700", content_kitchen, Qt::AlignLeft);

                }
                else
                {
                    b+=backheader::printer->drawText("1B40","");
                    b+=backheader::printer->drawText("1B40 1B6101 1D2101",tr("出品打印单\n"));//居中放大
                    b += backheader::printer->drawText("1B40 1D2101", content_kitchen, Qt::AlignLeft);
                }
                b += backheader::printer->drawText("1B6405 1D5601", "");//走纸5x， 切纸
                b=backheader::printer->setPrintCount(b, print_count);
                // chuda_print_yizhuo->asynchPrint(b, path);
                if(backheader::syn_dataprint(b,path) == true)//成功,更新数据库打印菜品大类
                {
                    QSqlQuery query;
                    QSqlDatabase::database().transaction();
                    for(int ii = 0; ii < print_model.rowCount(); ii++)
                    {
                        query.prepare(QString("update cey_u_orderdish set ch_outflag ='Y' where ch_billno =:danhao and ch_dishno = :bianma"));
                        query.bindValue(":danhao",dayindanhao);
                        query.bindValue(":bianma",print_model.item(ii,1)->text());
                        if(query.exec())
                        {

                        }
                        else
                        {
                            QSqlDatabase::database().rollback();
                            ;
                            lds_messagebox::warning(this, tr("消息提示"),tr("更新出品打印失败!"));

                        }
                    }
                    QSqlDatabase::database().commit();
                }
                else//失败
                {
                    kitchenprinte_lost();
                }
                // connect(chuda_print_yizhuo,SIGNAL(errorString(QString)),this,SLOT(kitchenprinte_lost(QString)));
                content_kitchen.clear();
                zongji= 0.00;
                hanghao = 0;

                //打印大类内容======================

            }//大类IP轮寻结束

        }

        //出品打印结束==========================
    }
}

void table_pay::kitchenprinte_lost()
{
    QSqlQueryModel model;
    model.setQuery(QString("select Max(id) from chudaloudan"),QSqlDatabase::database("loss_form"));

    QSqlQueryModel model_l;
    model_l.setQuery("Select NOW()");
    QString date = model_l.record(0).value(0).toDateTime().toString("yyyy-MM-dd");

    int maxid = model.record(0).value(0).toInt();

    if(maxid <= 0)
    {
        maxid = 1;
    }
    else
    {
        maxid++;
    }

    QSqlQuery query(QSqlDatabase::database("loss_form"));
    bool bsuccess = query.exec("create table if not exists chudaloudan (id int primary key, "
                               "vch_ip varchar(20) ,vch_content varchar(2048),vch_danaho varchar(20),vch_title varchar(30),vch_printflag varchar(5),dt_date varchar(20))");   //创建一个表
    if(!bsuccess)
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("创建本地数据库chudaloudan失败!"));

        return;
    }
    query.prepare(QString("insert into chudaloudan(id,vch_ip,vch_content,vch_danaho,vch_title,vch_printflag,dt_date) values(:id, :ip,:content,:vch_danaho,:title,:print,:date)"));
    query.bindValue(":id",QString::number(maxid));
    query.bindValue(":ip",louda_ip);
    query.bindValue(":content",louda_neirong);
    query.bindValue(":vch_danaho",dayindanhao);
    query.bindValue(":title",louda_title);
    query.bindValue(":print","N");
    query.bindValue(":date",date);
    if(query.exec())
    {

    }
    else
    {
        QSqlDatabase::database("loss_form").rollback();
        lds_messagebox::warning(this, tr("消息提示"),("插入漏单失败!"));

        return;
    }
    //    model.clear();
    //    model.setQuery(QString("select * from chudaloudan"),QSqlDatabase::database("loss_form"));
    //    for(int i = 0; i< model.rowCount(); i++)
    //    {
    //        qDebug() << model.record(i).value(0).toString() << model.record(i).value(1).toString() << model.record(i).value(2).toString() << model.record(i).value(3).toString()
    //                    << model.record(i).value(4).toString() << model.record(i).value(5).toString();
    //    }
}

void table_pay::on_pushButton_24_clicked()//预打单
{
    emit yudadan();
}
