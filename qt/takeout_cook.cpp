#include "takeout_cook.h"
#include <QSqlQueryModel>
#include "fast_order_2.h"
#include <QSqlRecord>
#include <QDateTime>
#include "n_func.h"
#include "lds_messagebox.h"
#include "operid_view.h"
#include <QSqlQuery>
#include <QDebug>
#include "n_func.h"
#include <QSettings>
#include <QSqlError>
#include <QSqlError>
#include "backheader.h"
#include "system_setting.h"
#include "fast_order_2.h"
#include "labelprint_main_dialog.h"
#include "fast_pay.h"

takeout_cook::takeout_cook(fast_order_2 *parent) :
    QDialog(parent),
    ui(new Ui::takeout_cook),
    _parent(parent)
{
    ui->setupUi(this);
    //this->setWindowFlags(Qt::FramelessWindowHint|Qt::Dialog);this->setStyleSheet("QDialog{border:2px solid gray}");
    init_control();
    init_data();
    btn_t_f = false;
}

takeout_cook::~takeout_cook()
{
    delete ui;
}

void takeout_cook::init_control()
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
}

void takeout_cook::init_data()
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
                    aa = model.item(i,3)->text().toFloat() * model.item(i,4)->text().toFloat() * discount_num.toFloat() * 0.01;
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
                        aa = model.item(i,3)->text().toFloat() * model.item(i,4)->text().toFloat() * discount_num.toFloat() * 0.01;
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


    ui->lineEdit_2->setText(QString().sprintf("%0.2f",xiaofei));//消费
    ui->lineEdit_3->setText(QString().sprintf("%0.2f",zhekou));//折扣
    ui->lineEdit_5->setText(QString().sprintf("%0.2f",zengsong));//赠送
    ui->lineEdit_7->setText(QString().sprintf("%0.2f",yingshou));//应收
    ui->lineEdit_6->setText(QString().sprintf("%0.2f",moling));//抹零
    ui->lineEdit_4->setText(QString().sprintf("%0.2f",shuishou));//税收
    ui->lineEdit_12->setText(n_func::gs_operid);//操作员

    moling_m.clear();
    moling_m.setQuery("Select NOW()");
    QString str = moling_m.record(0).value(0).toDateTime().toString("yyyy-MM-dd hh:mm:ss");
    ui->lineEdit_13->setText(str);//时间
}

void takeout_cook::print()
{
    QSettings *configIniRead = new QSettings(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    QSqlQueryModel model;
    QString str_t= configIniRead->value("system_setting/zhangdanbiaoti").toString();
    if(!str_t.isEmpty())
    {
        str_t.append("\n");
    }
    QString title = str_t +  tr("外卖/外带单\n");//需居中

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

    //外卖地址
    model.clear();
    model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_address' "));
    QString address = model.record(0).value(0).toString();
    model.clear();
    model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_address_len' "));
    QString addressrlen = model.record(0).value(0).toString();
    model.clear();
    model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_address_row' "));
    int addressrow = model.record(0).value(0).toInt();

    //外卖人
    model.clear();
    model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_linkman' "));
    QString waimairen = model.record(0).value(0).toString();
    model.clear();
    model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_linkman_len' "));
    QString waimairenlen = model.record(0).value(0).toString();
    model.clear();
    model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_linkman_row' "));
    int waimairenrow = model.record(0).value(0).toInt();

    //外卖电话
    model.clear();
    model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_tel' "));
    QString tel = model.record(0).value(0).toString();
    model.clear();
    model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_tel_len' "));
    QString tellen = model.record(0).value(0).toString();
    model.clear();
    model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_tel_row' "));
    int telrow = model.record(0).value(0).toInt();

    //服务员
    model.clear();
    model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_waiter' "));
    QString waiter = model.record(0).value(0).toString();
    model.clear();
    model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_waiter_len' "));
    QString waiterlen = model.record(0).value(0).toString();
    model.clear();
    model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_waiter_row' "));
    int waiterrow = model.record(0).value(0).toInt();

    //服务员名称
    model.clear();
    model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_waitername' "));
    QString wname = model.record(0).value(0).toString();
    model.clear();
    model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_waitername_len' "));
    QString wnamelen = model.record(0).value(0).toString();
    model.clear();
    model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_waitername_row' "));
    int wnamerow = model.record(0).value(0).toInt();

    //流水号
    model.clear();
    model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='serial_no' "));
    QString serial = model.record(0).value(0).toString();
    model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='serial_no_len' "));
    QString seriallen = model.record(0).value(0).toString();

    QString shifoudayi = configIniRead->value("system_setting/maidanzhangdan").toString();
    //shifoudayi = "1";
    if(shifoudayi == "1")
    {
        blockIns b;
        QByteArray content;//标题内容
        //第一行=====================================================================================
        QByteArray content1;
        //单号
        if(danhao == "1")
            w_scr_dish::setprintsize(b, danlen, tr("单号:") + wdanhao+"\n");
        //工号
        if(gonghao == "1")
            w_scr_dish::setprintsize(b, gonglen, tr("工号:") + n_func::gs_operid+"\n");
        //餐桌
        if(table == "1")
            w_scr_dish::setprintsize(b, tablelen, tr("餐桌:Q000") +"\n");
        //人数
        if(renshu == "1")
            w_scr_dish::setprintsize(b, renshulen, tr("人数:1") +"\n");
        //时间
        if(timer == "1")
            w_scr_dish::setprintsize(b, timerlen, tr("时间:") +ui->lineEdit_13->text()+"\n");
        //外卖地址
        if(address == "1")
            w_scr_dish::setprintsize(b, addressrlen, tr("外卖地址:") +ui->lineEdit_10->text()+"\n");
        //外卖人
        if(waimairen == "1")
            w_scr_dish::setprintsize(b, waimairenlen, tr("外卖人:") +ui->lineEdit_9->text()+"\n");
        //电话
        if(tel == "1")
            w_scr_dish::setprintsize(b, tellen, tr("电话:") +ui->lineEdit_8->text()+"\n");
        //服务员
        if(waiter == "1"){
            QSqlQuery  query;
            query.exec(QString("select vch_operID from cey_sys_operator where vch_operator = '%1' ").arg(ui->lineEdit_11->text()));
            query.next();
            w_scr_dish::setprintsize(b, waiterlen, tr("服务员:") +query.record().value("vch_operID").toString()+"\n");
        }
        //服务员名称
        if(wname == "1")
            w_scr_dish::setprintsize(b, wnamelen, tr("服务员名称:") + ui->lineEdit_11->text()+"\n");
        //流水号
        if(serial == "1")
            w_scr_dish::setprintsize(b, seriallen, tr("流水号:")+liushuihao+"\n");
        //打印消费内容

        //表头
        QString shifoumingxi;
        shifoumingxi = configIniRead->value("system_setting/maidanmingxi").toString();
        if(shifoumingxi == "1")
        {
            QString tou = configIniRead->value("system_setting/biaoneirong").toString();
            // tou = tr("项目               数量           金额\n");
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
                    QStandardItemModel cook;
                    for(int m = 0; m < fast_order_2::modelBase.rowCount(); m++)
                    {
                        if(fast_order_2::modelBase.item(m,22)->text() == dalei.record(k).value(0).toString())
                        {
                            int row = cook.rowCount();
                            cook.setRowCount(row + 1);
                            cook.setItem(row,0,new QStandardItem(fast_order_2::modelBase.item(m,0)->text()));
                            cook.setItem(row,1,new QStandardItem(fast_order_2::modelBase.item(m,1)->text()));
                            cook.setItem(row,2,new QStandardItem(fast_order_2::modelBase.item(m,2)->text()));
                            cook.setItem(row,3,new QStandardItem(fast_order_2::modelBase.item(m,3)->text()));
                            cook.setItem(row,4,new QStandardItem(fast_order_2::modelBase.item(m,4)->text()));
                            cook.setItem(row,5,new QStandardItem(fast_order_2::modelBase.item(m,5)->text()));
                            cook.setItem(row,6,new QStandardItem(fast_order_2::modelBase.item(m,6)->text()));
                            cook.setItem(row,7,new QStandardItem(fast_order_2::modelBase.item(m,7)->text()));
                            cook.setItem(row,8,new QStandardItem(fast_order_2::modelBase.item(m,8)->text()));
                            cook.setItem(row,9,new QStandardItem(fast_order_2::modelBase.item(m,9)->text()));
                            cook.setItem(row,10,new QStandardItem(fast_order_2::modelBase.item(m,10)->text()));
                            cook.setItem(row,11,new QStandardItem(fast_order_2::modelBase.item(m,11)->text()));
                            cook.setItem(row,12,new QStandardItem(fast_order_2::modelBase.item(m,12)->text()));
                            cook.setItem(row,13,new QStandardItem(fast_order_2::modelBase.item(m,13)->text()));
                            cook.setItem(row,14,new QStandardItem(fast_order_2::modelBase.item(m,14)->text()));
                            cook.setItem(row,15,new QStandardItem(fast_order_2::modelBase.item(m,15)->text()));
                            cook.setItem(row,16,new QStandardItem(fast_order_2::modelBase.item(m,16)->text()));
                            cook.setItem(row,17,new QStandardItem(fast_order_2::modelBase.item(m,17)->text()));
                            cook.setItem(row,18,new QStandardItem(fast_order_2::modelBase.item(m,18)->text()));
                            cook.setItem(row,19,new QStandardItem(fast_order_2::modelBase.item(m,19)->text()));
                            cook.setItem(row,20,new QStandardItem(fast_order_2::modelBase.item(m,20)->text()));
                            cook.setItem(row,21,new QStandardItem(fast_order_2::modelBase.item(m,21)->text()));
                            cook.setItem(row,22,new QStandardItem(fast_order_2::modelBase.item(m,22)->text()));
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
                QStandardItemModel & cook = fast_order_2::modelBase;
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
        // linecount = 2;
        int line_num = 0;
        //付款详情
        QString xiangqing = configIniRead->value("system_setting/fukuanxiangqing").toString();
//        if(xiangqing == "1")
//        {
//            line_num++;
//            QString str = tr("现金:") + ui->lineEdit_7->text();
//            if(str.length() < 12)
//            {
//                int a = 12 - str.length();
//                for(int m = 0; m < a; m++)
//                {
//                    str.append(" ");
//                }
//            }
//            if(line_num == linecount)
//            {
//                str.append("\n");
//                content.append(str);
//                line_num = 0;
//            }
//            else
//            {
//                content.append(str);
//            }
//        }

        //消费
        model.clear();
        model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_cost' "));
        if(model.record(0).value(0).toInt() == 1)
        {
            line_num++;
            QString str = tr("消费:") + ui->lineEdit_2->text();
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
            if(ui->lineEdit_3->text().toFloat() > 0)
            {
                line_num++;
                QString str = tr("折扣:") + ui->lineEdit_3->text();
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
            if(ui->lineEdit_5->text().toFloat() > 0)
            {
                line_num++;
                QString str = tr("赠送:") + ui->lineEdit_5->text();
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
            if(ui->lineEdit_6->text().toFloat() != 0)
            {
                line_num++;
                QString str = tr("抹零:") + ui->lineEdit_6->text();
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
        // shuishou = "1";
        if(shuishou.toInt() == 1)
        {
            if(ui->lineEdit_4->text().toFloat() != 0)
            {
                line_num++;
                QString str = tr("税收:") + ui->lineEdit_4->text();
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
            QString str = tr("应收:") + ui->lineEdit_7->text();
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
        content.append("\n");
        content.append(n_func::gs_print_line+"\n");

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
        //图片打印
        QString logo = configIniRead->value("system_setting/dayinlogo").toString();
        if(logo == "1")
        {
            QString str = configIniRead->value("system_setting/dayinlogoneirong").toString();//图片路径
            b+= backheader::printer->drawLogo("1B40 1B6101", str);//清除缓存 居中 pixmap路径
        }

        content.append("\n");
        if(configIniRead->value("system_setting/biaotoushezhi").toBool()){
            b.push_front(backheader::printer->drawText("1B6100 1D2100"));//左对齐,正常字体
            b.push_front(backheader::printer->drawText("1B6101 1D2111",configIniRead->value("system_setting/zhangdanbiaoti").toString()+"\n"));//居中不放大
        }
        if(!configIniRead->value("system_setting/not_show_bill_title").toBool()){
            b.push_front(backheader::printer->drawText("1B6100 1D2100"));//左对齐,正常字体
            b.push_front(backheader::printer->drawText("1B6101 1D2111",title));//居中不放大
        }

        b+=backheader::printer->drawText("1B40 1C5700", "");//放大
        QList<QByteArray> contentlist=content.split('\n');
        QString content_len=n_func::f_get_sysparm("bp_content_len");
        foreach(QByteArray ct, contentlist){
            w_scr_dish::setprintsize(b, content_len, ct+"\n");
        }

        //打印二维码
        QString erweima = configIniRead->value("system_setting/erweima").toString();
        if(erweima == "1")
        {
            QString str = configIniRead->value("system_setting/erweimaneirong").toString();//二维码内容
            b+= backheader::printer->drawQR_Code("", str);//清除缓存 居中 pixmap路径
        }
        b += backheader::printer->drawText("1B6405 1D5601", "");//走纸5x， 切纸
        b=backheader::printer->setPrintCount(b, print_count);
        //打印小票
        backheader::asyn_dataprint(b);

        //付款打印
        _parent->to_payfor_info(b);


        //打印标签，奶茶模式
        QSqlQuery query;
        if(configIniRead->value("system_setting/naichamoshi").toBool()){///if
            labelprint_main_dialog dialog;
            dialog.printmodel->setHorizontalHeaderLabels(QStringList()
                                                         << tr("打印次数")<< tr("单号")<< tr("价格")
                                                         << tr("品名") << tr("时间") << tr("做法")
                                                         );
            for(int r=0,r_count=fast_order_2::modelBase.rowCount();r<r_count;r++){
                query.exec(QString("select ch_labelprint_flag from cey_bt_dish_subtype where ch_sub_typeno='%1' ")
                           .arg(fast_order_2::modelBase.item(r, 23)->text()));
                query.next();
                if(query.record().value("ch_labelprint_flag").toString() == "Y"){
                    for(int pr=0,pcount=fast_order_2::modelBase.item(r, 3)->text().toFloat();pr<pcount;pr++){
                        dialog.printmodel->appendRow(QList<QStandardItem*>()
                                                     <<new QStandardItem("1")
                                                     <<new QStandardItem(tr("单号:")+wdanhao)
                                                     <<new QStandardItem(tr("价格:")+fast_order_2::modelBase.item(r, 4)->text())
                                                     <<new QStandardItem(tr("品名:")+fast_order_2::modelBase.item(r, 2)->text())
                                                     <<new QStandardItem(tr("时间:")+n_func::f_get_sysdatetime().toString("yyyy-MM-dd hh:mm:ss"))
                                                     <<new QStandardItem(tr("做法:")+fast_order_2::modelBase.item(r, 7)->text())
                                                     );
                    }//for
                }//if
            }//for
            dialog.toprint();
        }///if


        //厨打
        fast_pay::kitchen_or_outprint("", wdanhao,liushuihao);
        fast_pay::kitchen_or_outprint("out_", wdanhao,liushuihao);
    }
}

void takeout_cook::on_pushButton_3_clicked()//取消
{
    btn_t_f = false;
    this->reject();
}

void takeout_cook::on_pushButton_2_clicked()//确定
{
    bool isyun_waimai=false;
    QString weixin_sn;
    //01->12
    if(this->windowTitle().startsWith(tr("微信外卖"))){
        isyun_waimai=true;
        weixin_sn=this->windowTitle().split("-").value(1);
    }

    ;
    if(ui->lineEdit_8->text().trimmed().isEmpty() ||ui->lineEdit_9->text().trimmed().isEmpty() ||ui->lineEdit_10->text().trimmed().isEmpty() ||ui->lineEdit_11->text().trimmed().isEmpty() ||ui->lineEdit_3->text().trimmed().isEmpty())
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("信息不完整,请核实!"));

        return;
    }
    if(1==lds_messagebox::warning(this, tr("消息提示"),tr("确认派送?"), tr("确认"), tr("取消"))){
        return;
    }

    QSqlQuery query;
    QSqlDatabase::database().transaction();
    QString zhuohao = "Q000";

    QSqlQueryModel modelthis;
    modelthis.setQuery("Select NOW()");
    QDateTime ldt_datetime;
    ldt_datetime.setTime(modelthis.record(0).value(0).toTime());
    ldt_datetime.setDate(modelthis.record(0).value(0).toDate());
    QString currentDate = "C" + ldt_datetime.toString("yyMMdd");
    QString currentDate1 = "Q" + ldt_datetime.toString("yyMMdd");
    QString currentDate2 = (isyun_waimai?"T":"W") + ldt_datetime.toString("yyMMdd");

    modelthis.clear();
    modelthis.setQuery(QString("select Max(right(ch_guestid, 4)) from cey_u_togo_guest where Left(ch_guestid,7) = '%1' ").arg(currentDate));
    QString guestid = modelthis.record(0).value(0).toString().trimmed();

    if(guestid.isEmpty())
    {
        guestid = currentDate + QString().sprintf("%04d",1);
    }
    else
    {
        QString str = guestid.right(guestid.length() - 8);
        int aa = str.toInt() + 1;
        guestid = currentDate + tr("-") + QString().sprintf("%04d",aa);
    }

    qDebug() <<"guestid"<< guestid;
    query.prepare(QString("INSERT INTO cey_u_togo_guest(ch_guestid , vch_guestname , vch_tel , vch_address)VALUES (:guset,:xingming,:tel,:dizhi)"));
    query.bindValue(":guset",guestid);
    query.bindValue(":xingming",ui->lineEdit_9->text());
    query.bindValue(":tel",ui->lineEdit_8->text());
    query.bindValue(":dizhi",ui->lineEdit_10->text());
    if(query.exec())
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        qDebug() << query.lastError();  lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_u_togo_guest失败!"));

        return;
    }

    modelthis.clear();
    modelthis.setQuery(QString("select Max(ch_billno) from cey_u_master where Left(ch_billno,7) = '%1' ").arg(currentDate1));
    QString danhao = modelthis.record(0).value(0).toString().trimmed();
    if(danhao.trimmed().isEmpty())
    {
        danhao = currentDate1 + QString().sprintf("%05d",1);
    }
    else
    {
        QString str = danhao.right(danhao.length() - 7);
        int aa = str.toInt() + 1;
        danhao = currentDate1 + QString().sprintf("%05d",aa);
    }

    query.prepare(QString("INSERT INTO cey_u_master(ch_billno , ch_state , vch_operID , dt_operdate)VALUES (:danhao,:state,:oper,:time)"));
    query.bindValue(":danhao",danhao);
    query.bindValue(":state","1");
    query.bindValue(":oper",n_func::gs_operid);
    query.bindValue(":time",ui->lineEdit_13->text());
    if(query.exec())
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        qDebug() << query.lastError();  lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_u_master失败!"));

        return;
    }

    query.prepare(QString("INSERT cey_u_table(ch_billno , ch_tableno , int_person , vch_memo , ch_state , ch_payno , vch_operID , dt_operdate , ch_order_state , num_cost)VALUES (:danhao,:zhuohao,:person,null,:state,null,:oper,:time,:orderstate,:numcost)"));
    query.bindValue(":danhao",danhao);
    query.bindValue(":zhuohao",zhuohao);
    query.bindValue(":person","1");
    query.bindValue(":state","1");
    query.bindValue(":oper",n_func::gs_operid);
    query.bindValue(":time",ui->lineEdit_13->text());
    query.bindValue(":orderstate","1");
    query.bindValue(":numcost",ui->lineEdit_7->text());
    if(query.exec())
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        qDebug() << query.lastError();  lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_u_table失败!"));

        return;
    }

    modelthis.clear();
    modelthis.setQuery(QString("select Max(ch_togono) from cey_u_togo where Left(ch_togono,7) = '%1' ").arg(currentDate2));
    QString whao = modelthis.record(0).value(0).toString().trimmed();
    int whao_no=1;
    if(whao.trimmed().isEmpty()){

    } else {
        QString str = whao.right(whao.length() - 8);
        whao_no = str.toInt() + 1;
    }
    whao = currentDate2 + tr("-") + QString().sprintf("%05d",whao_no);
    if(isyun_waimai)whao = currentDate2 + tr("-") + QString().sprintf("%05d",weixin_sn.toInt());
    wdanhao = whao;

    liushuihao=system_setting::get_max_serialno_add1(wdanhao);

    query.prepare(QString("INSERT cey_u_togo (ch_togono,ch_billno,ch_payno,num_cost,num_discount,num_present,num_blotout,num_rate,vch_waiter,vch_address,vch_linkman,vch_tel,ch_state,vch_operID,dt_operdate) values(:guest,:danhao,'null',:yingshou,:zhkou,:zengsong,:moling,:shuishou,:songcanren,:dizhi,:dingcanren,:tel,'N',:operid,:time)"));
    query.bindValue(":guest",whao);
    query.bindValue(":danhao",danhao);
    query.bindValue(":yingshou",ui->lineEdit_2->text());
    query.bindValue(":zhkou",ui->lineEdit_3->text());
    query.bindValue(":zengsong",ui->lineEdit_5->text());
    query.bindValue(":moling",ui->lineEdit_6->text());
    query.bindValue(":shuishou",ui->lineEdit_4->text());
    query.bindValue(":songcanren",ui->lineEdit_11->text());
    query.bindValue(":dizhi",ui->lineEdit_10->text());
    query.bindValue(":dingcanre",ui->lineEdit_9->text());
    query.bindValue(":tel",ui->lineEdit_8->text());
    query.bindValue(":operid",n_func::gs_operid);
    query.bindValue(":time",ui->lineEdit_13->text());

    if(query.exec())
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        qDebug() << query.lastError();  lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_u_togo失败!"));

        return;
    }

    QStandardItemModel & model = fast_order_2::modelBase;
    for(int i = 0; i < model.rowCount(); i++)
    {
        float add_price;
        add_price = 0.00;
        add_price = model.item(i,3)->text().toFloat() * model.item(i,21)->text().toFloat();
        if(model.item(i,8)->text() == "Y")//赠送菜品
        {
            if(model.item(i,7)->text().isEmpty()) //没有特殊做法
            {
                query.prepare(QString("insert into cey_u_orderdish(ch_billno,ch_tableno,int_id,ch_dishno,num_price,num_price_org,num_price_add,num_num,num_back,vch_back_operID,dt_back_operdate,int_discount,vch_dis_operID,dt_dis_operdate,vch_memberno,ch_consult,ch_suitflag,ch_suitno,ch_specialflag,ch_presentflag,vch_pre_operID,dt_pre_operdate,vch_operID,dt_operdate,ch_togono,ch_payno,vch_print_memo) values(:danhao,:zhuohao,:hanghao,:pinma,:danjia,:danjiad1,:jiajia,:shuliang,:backnum,null,null,:zhekou,null,null,null,:zhconsult,:taocai,null,:tejia,:zengsong,:zensongren,:zengsongtime,:caozuoyuan,:caozuotime,:guest,null,null)"));
                query.bindValue(":danhao",danhao);
                query.bindValue(":zhuohao",zhuohao);
                query.bindValue(":hanghao",QString::number(i+1));
                query.bindValue(":pinma",model.item(i,1)->text());
                query.bindValue(":danjia",model.item(i,4)->text());
                query.bindValue(":danjiad1",model.item(i,4)->text());
                query.bindValue(":jiajia",QString().sprintf("%0.2f",add_price));
                query.bindValue(":shuliang",model.item(i,3)->text());
                query.bindValue(":backnum",tr("0.00"));
                query.bindValue(":zhekou",model.item(i,5)->text());
                query.bindValue(":zhconsult","0");
                query.bindValue(":taocai",model.item(i,9)->text());
                query.bindValue(":tejia",model.item(i,10)->text());
                query.bindValue(":zengsong",model.item(i,8)->text());
                query.bindValue(":zensongren",n_func::gs_operid);
                query.bindValue(":zengsongtime",model.item(i,20)->text());
                query.bindValue(":caozuoyuan",n_func::gs_operid);
                query.bindValue(":caozuotime",ui->lineEdit_13->text());
                query.bindValue(":guest",whao);
                if(query.exec())
                {

                }
                else
                {
                    QSqlDatabase::database().rollback();
                    lds_messagebox::warning(this, tr("消息提示"),tr("cey_u_orderdish失败!"));

                    return;
                }
            }
            else
            {
                query.prepare(QString("insert into cey_u_orderdish(ch_billno,ch_tableno,int_id,ch_dishno,num_price,num_price_org,num_price_add,num_num,num_back,vch_back_operID,dt_back_operdate,int_discount,vch_dis_operID,dt_dis_operdate,vch_memberno,ch_consult,ch_suitflag,ch_suitno,ch_specialflag,ch_presentflag,vch_pre_operID,dt_pre_operdate,vch_operID,dt_operdate,ch_togono,ch_payno,vch_print_memo) values(:danhao,:zhuohao,:hanghao,:pinma,:danjia,:danjiad1,:jiajia,:shuliang,:backnum,null,null,:zhekou,null,null,null,:zhconsult,:taocai,null,:tejia,:zengsong,:zensongren,:zengsongtime,:caozuoyuan,:caozuotime,:guest,null,:zuofa)"));
                query.bindValue(":danhao",danhao);
                query.bindValue(":zhuohao",zhuohao);
                query.bindValue(":hanghao",QString::number(i+1));
                query.bindValue(":pinma",model.item(i,1)->text());
                query.bindValue(":danjia",model.item(i,4)->text());
                query.bindValue(":danjiad1",model.item(i,4)->text());
                query.bindValue(":jiajia",QString().sprintf("%0.2f",add_price));
                query.bindValue(":shuliang",model.item(i,3)->text());
                query.bindValue(":backnum",tr("0.00"));
                query.bindValue(":zhekou",model.item(i,5)->text());
                query.bindValue(":zhconsult","0");
                query.bindValue(":taocai",model.item(i,9)->text());
                query.bindValue(":tejia",model.item(i,10)->text());
                query.bindValue(":zengsong",model.item(i,8)->text());
                query.bindValue(":zensongren",n_func::gs_operid);
                query.bindValue(":zengsongtime",model.item(i,20)->text());
                query.bindValue(":caozuoyuan",n_func::gs_operid);
                query.bindValue(":caozuotime",ui->lineEdit_13->text());
                query.bindValue(":guest",whao);
                query.bindValue(":zuofa",model.item(i,7)->text());
                if(query.exec())
                {

                }
                else
                {
                    QSqlDatabase::database().rollback();
                    lds_messagebox::warning(this, tr("消息提示"),tr("cey_u_orderdish失败!"));

                    return;
                }
            }
        }
        else if(model.item(i,5)->text().toFloat() != 100)//折扣菜品
        {
            QString zhekou = QString::number(int(model.item(i,5)->text().toFloat()));
            if(model.item(i,7)->text().isEmpty())
            {
                query.prepare(QString("insert into cey_u_orderdish(ch_billno,ch_tableno,int_id,ch_dishno,num_price,num_price_org,num_price_add,num_num,num_back,vch_back_operID,dt_back_operdate,int_discount,vch_dis_operID,dt_dis_operdate,vch_memberno,ch_consult,ch_suitflag,ch_suitno,ch_specialflag,ch_presentflag,vch_pre_operID,dt_pre_operdate,vch_operID,dt_operdate,ch_togono,ch_payno,vch_print_memo) values(:danhao,:zhuohao,:hanghao,:pinma,:danjia,:danjiad1,:jiajia,:shuliang,:backnum,null,null,:zhekou,:zhekouyuan,:zhekoutime,null,:zhconsult,:taocai,null,:tejia,:zengsong,null,null,:caozuoyuan,:caozuotime,:guest,null,null);"));
                query.bindValue(":danhao",danhao);
                query.bindValue(":zhuohao",zhuohao);
                query.bindValue(":hanghao",QString::number(i+1));
                query.bindValue(":pinma",model.item(i,1)->text());
                query.bindValue(":danjia",model.item(i,4)->text());
                query.bindValue(":danjiad1",model.item(i,4)->text());
                query.bindValue(":jiajia",QString().sprintf("%0.2f",add_price));
                query.bindValue(":shuliang",model.item(i,3)->text());
                query.bindValue(":backnum",tr("0.00"));
                query.bindValue(":zhekou",zhekou);
                query.bindValue(":zhekouyuan",n_func::gs_operid);
                query.bindValue(":zhekoutime",model.item(i,19)->text());
                query.bindValue(":zhconsult","1");
                query.bindValue(":taocai",model.item(i,9)->text());
                query.bindValue(":tejia",model.item(i,10)->text());
                query.bindValue(":zengsong",model.item(i,8)->text());
                query.bindValue(":caozuoyuan",n_func::gs_operid);
                query.bindValue(":caozuotime",ui->lineEdit_13->text());
                query.bindValue(":guest",whao);
                if(query.exec())
                {

                }
                else
                {
                    QSqlDatabase::database().rollback();
                    lds_messagebox::warning(this, tr("消息提示"),tr("cey_u_orderdish失败!"));

                    return;
                }
            }
            else
            {
                query.prepare(QString("insert into cey_u_orderdish(ch_billno,ch_tableno,int_id,ch_dishno,num_price,num_price_org,num_price_add,num_num,num_back,vch_back_operID,dt_back_operdate,int_discount,vch_dis_operID,dt_dis_operdate,vch_memberno,ch_consult,ch_suitflag,ch_suitno,ch_specialflag,ch_presentflag,vch_pre_operID,dt_pre_operdate,vch_operID,dt_operdate,ch_togono,ch_payno,vch_print_memo) values(:danhao,:zhuohao,:hanghao,:pinma,:danjia,:danjiad1,:jiajia,:shuliang,:backnum,null,null,:zhekou,:zhekouyuan,:zhekoutime,null,:zhconsult,:taocai,null,:tejia,:zengsong,null,null,:caozuoyuan,:caozuotime,:guest,null,:zuofa);"));
                query.bindValue(":danhao",danhao);
                query.bindValue(":zhuohao",zhuohao);
                query.bindValue(":hanghao",QString::number(i+1));
                query.bindValue(":pinma",model.item(i,1)->text());
                query.bindValue(":danjia",model.item(i,4)->text());
                query.bindValue(":danjia1",model.item(i,4)->text());
                query.bindValue(":jiajia",QString().sprintf("%0.2f",add_price));
                query.bindValue(":shuliang",model.item(i,3)->text());
                query.bindValue(":backnum",tr("0.00"));
                query.bindValue(":zhekou",zhekou);
                query.bindValue(":zhekouyuan",n_func::gs_operid);
                query.bindValue(":zhekoutime",model.item(i,19)->text());
                query.bindValue(":zhconsult","1");
                query.bindValue(":taocai",model.item(i,9)->text());
                query.bindValue(":tejia",model.item(i,10)->text());
                query.bindValue(":zengsong",model.item(i,8)->text());
                query.bindValue(":caozuoyuan",n_func::gs_operid);
                query.bindValue(":caozuotime",ui->lineEdit_13->text());
                query.bindValue(":guest",whao);
                query.bindValue(":zuofa",model.item(i,7)->text());
                if(query.exec())
                {

                }
                else
                {
                    QSqlDatabase::database().rollback();
                    lds_messagebox::warning(this, tr("消息提示"),tr("cey_u_orderdish失败!"));

                    return;
                }
            }
        }
        else//普通菜
        {
            if(model.item(i,7)->text().isEmpty())
            {
                query.prepare(QString("insert into cey_u_orderdish(ch_billno,ch_tableno,int_id,ch_dishno,num_price,num_price_org,num_price_add,num_num,num_back,vch_back_operID,dt_back_operdate,int_discount,vch_dis_operID,dt_dis_operdate,vch_memberno,ch_consult,ch_suitflag,ch_suitno,ch_specialflag,ch_presentflag,vch_pre_operID,dt_pre_operdate,vch_operID,dt_operdate,ch_togono,ch_payno,vch_print_memo) values(:danhao,:zhuohao,:hanghao,:pinma,:danjia,:danjiad1,:jiajia,:shuliang,:backnum,null,null,:zhekou,null,null,null,:zhconsult,:taocai,null,:tejia,:zengsong,null,null,:caozuoyuan,:caozuotime,:guest,null,null)"));
                query.bindValue(":danhao",danhao);
                query.bindValue(":zhuohao",zhuohao);
                query.bindValue(":hanghao",QString::number(i+1));
                query.bindValue(":pinma",model.item(i,1)->text());
                query.bindValue(":danjia",model.item(i,4)->text());
                query.bindValue(":danjia1",model.item(i,4)->text());
                query.bindValue(":jiajia",QString().sprintf("%0.2f",add_price));
                query.bindValue(":shuliang",model.item(i,3)->text());
                query.bindValue(":backnum",tr("0.00"));
                query.bindValue(":zhekou",model.item(i,5)->text());
                query.bindValue(":zhconsult","0");
                query.bindValue(":taocai",model.item(i,9)->text());
                query.bindValue(":tejia",model.item(i,10)->text());
                query.bindValue(":zengsong",model.item(i,8)->text());
                query.bindValue(":caozuoyuan",n_func::gs_operid);
                query.bindValue(":caozuotime",ui->lineEdit_13->text());
                query.bindValue(":guest",whao);
                if(query.exec())
                {

                }
                else
                {
                    QSqlDatabase::database().rollback();
                    lds_messagebox::warning(this, tr("消息提示"),tr("cey_u_orderdish失败!"));

                    return;
                }
            }
            else
            {
                query.prepare(QString("insert into cey_u_orderdish(ch_billno,ch_tableno,int_id,ch_dishno,num_price,num_price_org,num_price_add,num_num,num_back,vch_back_operID,dt_back_operdate,int_discount,vch_dis_operID,dt_dis_operdate,vch_memberno,ch_consult,ch_suitflag,ch_suitno,ch_specialflag,ch_presentflag,vch_pre_operID,dt_pre_operdate,vch_operID,dt_operdate,ch_togono,ch_payno,vch_print_memo) values(:danhao,:zhuohao,:hanghao,:pinma,:danjia,:danjiad1,:jiajia,:shuliang,:backnum,null,null,:zhekou,null,null,null,:zhconsult,:taocai,null,:tejia,:zengsong,null,null,:caozuoyuan,:caozuotime,:guest,null,:zuofa)"));
                query.bindValue(":danhao",danhao);
                query.bindValue(":zhuohao",zhuohao);
                query.bindValue(":hanghao",QString::number(i+1));
                query.bindValue(":pinma",model.item(i,1)->text());
                query.bindValue(":danjia",model.item(i,4)->text());
                query.bindValue(":danjia1",model.item(i,4)->text());
                query.bindValue(":jiajia",QString().sprintf("%0.2f",add_price));
                query.bindValue(":shuliang",model.item(i,3)->text());
                query.bindValue(":backnum",tr("0.00"));
                query.bindValue(":zhekou",model.item(i,5)->text());
                query.bindValue(":zhconsult","0");
                query.bindValue(":taocai",model.item(i,9)->text());
                query.bindValue(":tejia",model.item(i,10)->text());
                query.bindValue(":zengsong",model.item(i,8)->text());
                query.bindValue(":caozuoyuan",n_func::gs_operid);
                query.bindValue(":caozuotime",ui->lineEdit_13->text());
                query.bindValue(":guest",whao);
                query.bindValue(":zuofa",model.item(i,7)->text());
                if(query.exec())
                {

                }
                else
                {
                    QSqlDatabase::database().rollback();
                    lds_messagebox::warning(this, tr("消息提示"),tr("cey_u_orderdish失败!"));

                    return;
                }
            }
        }
    }

    //01->12
    QString weixinwaimai_info;
    if(!isyun_waimai
            || (isyun_waimai
                &&w_sys_manage_cloudsync::up_weixin_change_takkeoutstate(
                    weixin_sn,
                    "12"//已受理-已制作派送
                    ))){
        if(isyun_waimai) weixinwaimai_info=tr("[微信外卖]");
        QSqlDatabase::database().commit();
        btn_t_f = true;
        print();
        lds_messagebox::warning(this, tr("消息提示"),tr("外卖已制作派送!")+weixinwaimai_info);
        this->accept();
        return;
    }
    QSqlDatabase::database().rollback();
    lds_messagebox::warning(this, tr("消息提示"),tr("外卖制作派送失败!"));
}

void takeout_cook::on_pushButton_clicked()//送餐员
{
    operid_view oper;
    /*oper.exec();*/backheader::static_dialogexec(oper, tr("操作员"));
    if(oper.btn_t_f == true)
    {
        ui->lineEdit_11->setText(oper.operid);
    }
}

void takeout_cook::print_lost(const QString &str)
{
    ;
    lds_messagebox::warning(this, tr("消息提示"),str);

}
