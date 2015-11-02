#include "fast_order_2.h"
#include "fast_order_2.h"
//#include "ui_fast_order.h"
#include <QSqlRecord>
#include <QDebug>
#include "public_sql.h"
#include "lds_messagebox.h"
#include "cur_price.h"
//#include "tableview_delegate.h"
#include "n_func.h"
#include "lds_messagebox.h"
#include "discount_cook.h"
#include "power_oper.h"
#include "dish_change.h"
#include "modfiy_number.h"
#include "special_cook.h"
#include "takeout_cook.h"
#include "take_cashier.h"
#include "take_weight.h"
#include "function_selection.h"
#include "fast_pay.h"
#include <QScrollBar>
#include "public_logindialog.h"
#include <QSqlQuery>
#include <QSettings>
#include "fast_qudan.h"
#include <QDesktopWidget>
#include <QApplication>
#include "backheader.h"
#include <QSqlError>
#include "lds_menu.h"
#include <QHostInfo>
#include "back_view_dialog1.h"
#include "lds_messagebox.h"
#include "system_setting.h"
#include "w_scr_dish_weixinwaimai.h"
#include "lds_loadingdialog.h"
#include "w_scr_dish_weixin_quitbill.h"
#include "w_scr_dish_weixin_getbill.h"
#include "w_scr_dish_special_cook.h"
#include "trans_u2g.h"
#include "pre_autoexpand.h"


fast_order_2::fast_order_2(public_loginDialog *parent) :
    frontViewDialog_virtual(0),
    _parent(parent),
    ui(new Ui_frontViewDialog_fast_2)
{
    ui->setupUi(this);
    frontViewDialog_virtual::cur_desk_no=("Q000");//快餐默认是
    da_xiao = false;
    yingye_model.clear();
    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    yingye_model = conf.value("system_setting/yingyemoshi").toString();

    leiji = 0;
    row_n = -1;
    num_da = 0;
    num_xiao = 0;
    num_cai = 0;
    changyongcook = false;
    bianma_daicai_type = false;
    currentGoodItem_zilei = NULL;

    init_model();
    init_control();
    init_bedi_db();
    print_pr = new print_prompt(this);
    print_pr->setWindowTitle("");
    print_pr->setGeometry(0,0,print_pr->width(),print_pr->height());

    thread = new print_threqd;
    //thread->start();
    connect(thread,SIGNAL(print_lost_thread()),this,SLOT(thread_signal()));
    print_timer = new QTimer();
    connect(print_timer, SIGNAL(timeout()), this, SLOT(print_louda()));
    print_timer->setInterval(1000);
    //    print_timer->start();
    functions=new function_selection;
    functions->hide();
    ui->tableView->setModel(&modelBase);
    /*--------------------------------begin*/

    {//其他f
        lds_menu *menu=new lds_menu(this);
        QWidgetAction *widgetaction=new QWidgetAction(ui->pushButton_other);
        widgetaction->setDefaultWidget(ui->frame);
        menu->setStyleSheet("QMenu{border:0px;}");
        menu->addAction(widgetaction);
        ui->pushButton_other->setMenu(menu);
    }
    {//编码
        lds_menu *menu=new lds_menu(this);
        QWidgetAction *widgetaction=new QWidgetAction(ui->pushButton_bianma);
        bianma_cook *bianma = new bianma_cook(menu);
        bianma->setFixedSize(500,300);
        widgetaction->setDefaultWidget(bianma);
        menu->addAction(widgetaction);
        ui->pushButton_bianma->setMenu(menu);
        connect(bianma,SIGNAL(edit_text(QString)),this,SLOT(bianma_diancai(QString)));
    }
    {//微信订单
        lds_menu *menu=new lds_menu(this);
        QWidgetAction *widgetaction=new QWidgetAction(ui->pushButton_weixinfuction);
        widgetaction->setDefaultWidget(ui->frame_3);
        menu->setStyleSheet("QMenu{border:0px;}");
        menu->addAction(widgetaction);
        ui->pushButton_weixinfuction->setMenu(menu);
    }
    {
        connect(ui->pushButton_allbillcancel,SIGNAL(clicked()),this,SLOT(to_allbillcancel()));
        connect(ui->pushButton_change,SIGNAL(clicked()),this,SLOT(to_change()));
        connect(ui->pushButton_del,SIGNAL(clicked()),this,SLOT(to_del()));
        connect(ui->pushButton_discount,SIGNAL(clicked()),this,SLOT(to_discount()));
        connect(ui->pushButton_exitbill,SIGNAL(clicked()),this,SLOT(to_exitbill()));

        connect(ui->pushButton_num,SIGNAL(clicked()),this,SLOT(to_num()));
        connect(ui->pushButton_present,SIGNAL(clicked()),this,SLOT(to_present()));
        connect(ui->pushButton_specialcook,SIGNAL(clicked()),this,SLOT(to_specialcook()));
        connect(ui->pushButton_takebill,SIGNAL(clicked()),this,SLOT(to_takebill()));
        connect(ui->pushButton_takeweight,SIGNAL(clicked()),this,SLOT(to_takeweight()));

        connect(ui->pushButton_waimai,SIGNAL(clicked()),this,SLOT(to_waimai()));
        //        connect(ui->pushButton_waimai_2,SIGNAL(clicked()),this,SLOT(signal_waimai()));
        connect(ui->pushButton_waimaipay,SIGNAL(clicked()),this,SLOT(to_waimaipay()));
        connect(ui->pushButton_pay,SIGNAL(clicked()),this,SLOT(to_pay()));
        connect(ui->pushButton_bianma,SIGNAL(clicked()),this,SLOT(to_bianma()));
        connect(ui->pushButton_tuichu,SIGNAL(clicked()),this,SLOT(to_tuichu()));
        connect(ui->lineEdit_scan,SIGNAL(returnPressed()),this,SLOT(to_scanbar()));
        //        connect(ui->pushButton_member,SIGNAL(clicked()),this,SLOT(signal_member()));
        connect(ui->pushButton_weixinwaimai,SIGNAL(clicked()),this,SLOT(to_weixinwaimai()));
        connect(ui->pushButton_weixinquitbill,SIGNAL(clicked()),this,SLOT(to_weixinquitbill()));
        connect(ui->pushButton_weixinbill_import,SIGNAL(clicked()),this,SLOT(to_weixinbill_import()));

    }

    {
        connect(ui->pushButton_sys_guqing,SIGNAL(clicked()),functions,SLOT(to_discount()));
        connect(ui->pushButton_sys_tejia,SIGNAL(clicked()),functions,SLOT(to_present()));

        connect(ui->pushButton_sys_yingyedanju,SIGNAL(clicked()),functions,SLOT(to_takebill()));
        connect(ui->pushButton_sys_yingyeqingkuang,SIGNAL(clicked()),functions,SLOT(to_change()));

        connect(ui->pushButton_sys_jiaoban,SIGNAL(clicked()),functions,SLOT(to_exitbill()));
        connect(ui->pushButton_sys_chongzhi,SIGNAL(clicked()),functions,SLOT(to_num()));
        connect(ui->pushButton_sys_qukuan,SIGNAL(clicked()),functions,SLOT(to_member_qukuan()));

        //        connect(ui->pushButton_sys_xitongcanshu,SIGNAL(clicked()),functions,SIGNAL(signal_sys_xitongcanshu()));
        connect(ui->pushButton_sys_xiugaimima,SIGNAL(clicked()),functions,SLOT(to_sys_xiugaimima()));
        connect(ui->pushButton_sys_jiuloumoshi,SIGNAL(clicked()),this,SLOT(toswitchback()));
        //        connect(ui->pushButton_sys_jiuloumoshi,SIGNAL(clicked()),functions,SIGNAL(signal_sys_jiuloumoshi()));

        connect(ui->pushButton_sys_chufangdaying,SIGNAL(clicked()),functions,SLOT(to_sys_chufangdaying()));

        connect(ui->pushButton_sys_kaiqianxiang,SIGNAL(clicked()),functions,SLOT(to_allbillcancel()));
        connect(ui->pushButton_sys_changyongcai,SIGNAL(clicked()),this,SLOT(to_usual_dish()));
        connect(ui->pushButton_sys_suoping,SIGNAL(clicked()),functions,SLOT(to_suopint()));
    }

    connect(ui->widget,SIGNAL(signal1_text(QString)),this,SLOT(init_xiaolei(QString)));
    connect(ui->widget,SIGNAL(signal2_text(QString)),this,SLOT(init_cook(QString)));
    connect(ui->widget,SIGNAL(signal3_text(QString)),this,SLOT(addGood(QString)));
    connect(this,SIGNAL(signal_display_change(QString)),this,SLOT(change_label(QString)));
    connect(this,SIGNAL(signal_payfor_info(blockIns)),this,SLOT(to_payfor_info(blockIns)));
    connect(ui->widget,SIGNAL(signal3_refresh()),this,SLOT(init_cooks()));

    ui->frame_7->setFixedHeight(ui->widget->getframe2_h());
    ui->pushButton->setIcon(QPixmap(public_sql::get_logo_path()));
    /*--------------------------------end*/
}

fast_order_2::~fast_order_2()
{
    delete ui;
}

void fast_order_2::init_model()
{

}

void fast_order_2::init_control()
{
}

QStringList fast_order_2::init_dalei()
{
    QStringList daleis;
    QList<frontViewDialog_autoexpand::content_item_icon> item_icon_list;
    for(int i = 0; i < model_dalei.rowCount(); i++)
    {
        daleis.append(model_dalei.record(i).value("ch_dish_typeno").toString());
        item_icon_list.append(frontViewDialog_autoexpand::content_item_icon(model_dalei.record(i).value("ch_dish_typeno").toString(), model_dalei.record(i).value("vch_dish_typename").toString()));
    }
    this->to_grade0(item_icon_list);
    return daleis;
}

void fast_order_2::list_clear()
{
    modelBase.removeRows(0, modelBase.rowCount());
}

void fast_order_2::init_xiaolei(const QString &daleino)
{

    dalei= daleino;
    model_xiaolei.clear();
    model_xiaolei.setQuery(QString("SELECT ch_sub_typeno,vch_sub_typename FROM cey_bt_dish_subtype where ch_dish_typeno like '%1%' ").arg(daleino));
    QList<frontViewDialog_autoexpand::content_item_icon> item_icon_list;

    for(int i = 0; i < model_xiaolei.rowCount(); i++)
    {
        item_icon_list.append(frontViewDialog_autoexpand::content_item_icon(model_xiaolei.record(i).value("ch_sub_typeno").toString(), model_xiaolei.record(i).value("vch_sub_typename").toString()));
    }
    this->to_grade1(item_icon_list);
    init_cook("");
}

QStringList fast_order_2::init_xiaolei(const QStringList &daleinos)
{
    QStringList xiaoleis;
    QList<frontViewDialog_autoexpand::content_item_icon> item_icon_list;
    QSqlQuery query;
    foreach(QString dalei, daleinos){
        query.exec(QString("SELECT ch_sub_typeno,vch_sub_typename FROM cey_bt_dish_subtype where ch_dish_typeno='%1' ")
                   .arg(dalei));
        while(query.next()){
            xiaoleis.append(query.record().value("ch_sub_typeno").toString());
            item_icon_list.append(frontViewDialog_autoexpand::content_item_icon(query.record().value("ch_sub_typeno").toString(), query.record().value("vch_sub_typename").toString()));
        }
    }
    this->to_grade1(item_icon_list);
    return xiaoleis;
}

void fast_order_2::init_cook(const QString &xiaoleino, const QString &filter)
{
    xiaolei=xiaoleino;
    model_cook.clear();
    if(changyongcook){
        ui->widget->widget3_setsql(QString("select ch_dishno,concat(vch_dishname,'\n',num_price) from cey_bt_dish where ch_off = 'Y' and (ch_stopflag = null or ch_stopflag = 'N')  "));
        changyongcook=false;
    } else {
        ui->widget->widget3_setsql(QString("SELECT ch_dishno,concat(vch_dishname,'\n',num_price) FROM cey_bt_dish where ch_dish_typeno like '%1%' and ch_sub_typeno like'%2%' and (ch_stopflag = null or ch_stopflag = 'N') and (ch_dishno like '%3%' or vch_spell like '%3%' )")
                                   .arg(dalei)
                                   .arg(xiaolei)
                                   .arg(filter));
    }
    ui->widget->widget3_start();
}

void fast_order_2::init_cooks()
{
    ui->widget->widget3_tofresh();
    ui->widget->widget3_setsql("select ch_dishno,concat(vch_dishname,'\n',num_price) from cey_bt_dish where (ch_stopflag = null or ch_stopflag = 'N') ");
    ui->widget->widget3_start();
}

void fast_order_2::torefresh()
{
    frontViewDialog_virtual::cur_desk_no=("Q000");//快餐默认是
    init_pay_model();
    QStringList daleis=init_dalei();
    QStringList xiaoleis=init_xiaolei(daleis);
    init_cooks();
}

void fast_order_2::init_pay_model()
{
    model_dalei.setQuery(QString("select ch_dish_typeno, vch_dish_typename from cey_bt_dish_type where ch_dish_typeno in(select ch_dish_typeno from cey_bt_dish_type_oper b where b.vch_operid='%1' and b.ch_state ='Y') or '0000'='%1'").arg(n_func::gs_operid));
    for(int i = 0; i < model_dalei.rowCount(); i++)
    {
        if(i == num_da)
        {
            dalei = model_dalei.record(i).value("ch_dish_type").toString();
        }
    }

    model_xiaolei.setQuery(QString("SELECT ch_sub_typeno,vch_sub_typename FROM cey_bt_dish_subtype where ch_dish_typeno = '%1' ").arg(dalei));
    for(int i = 0; i < model_xiaolei.rowCount(); i++)
    {
        if(i == num_xiao)
        {
            xiaolei = model_xiaolei.record(i).value("ch_sub_typeno").toString();
        }
    }

    if(changyongcook == false)
    {
        model_cook.setQuery(QString("SELECT ch_dishno,vch_dishname,num_price FROM cey_bt_dish where ch_dish_typeno = '%1' and ch_sub_typeno ='%2' and (ch_stopflag = null or ch_stopflag = 'N')  ").arg(dalei).arg(xiaolei));
    }
    else
    {
        model_cook.setQuery(QString("select ch_dishno,vch_dishname,num_price from cey_bt_dish where ch_off = 'Y' and (ch_stopflag = null or ch_stopflag = 'N')   "));
    }
}

void fast_order_2::init_tuidan_data()
{
    QString time = n_func::f_get_sysdatetime().toString("yyyy-MM-dd hh:mm:ss");
    QString discount;//是否折扣
    QString discount_num;//折扣值
    float xiaofei = 0.00;
    float zhekou = 0.00;
    float zengsong = 0.00;
    float moling = 0.00;
    float shuishou = 0.00;
    float yingshou = 0.00;
    float xianjin = 0.00;
    {
        QSqlQueryModel model_query;
        model_query.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='pay_auto_discount'"));
        discount = model_query.record(0).value(0).toString();
        model_query.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='pay_auto_discount_value'"));
        discount_num = model_query.record(0).value(0).toString();
    }
    if(n_func::gs_operid == "0000")//特价，子套菜不能折扣//?时价菜能折扣?
    {
        if(discount == "1")//设置了账单自动折扣
        {
            for(int i = 0; i < modelBase.rowCount(); i++)
            {
                if(modelBase.item(i,9)->text() != "Y")
                {
                    xiaofei += modelBase.item(i,3)->text().toFloat() * modelBase.item(i,4)->text().toFloat() + modelBase.item(i,3)->text().toFloat() * modelBase.item(i,21)->text().toFloat();
                    shuishou += modelBase.item(i,3)->text().toFloat() * modelBase.item(i,4)->text().toFloat() * modelBase.item(i,17)->text().toFloat() * 0.01 ;
                }

                if(modelBase.item(i,9)->text() != "Y" &&  modelBase.item(i,10)->text() != "Y" && modelBase.item(i,18)->text() == "Y"&& modelBase.item(i,8)->text() == "N")
                {
                    modelBase.item(i,5)->setText(QString().sprintf("%0.2f",discount_num.toFloat()));
                    modelBase.item(i,19)->setText(time);
                    float aa = 0.00;
                    aa = modelBase.item(i,3)->text().toFloat() * modelBase.item(i,4)->text().toFloat() * modelBase.item(i,5)->text().toFloat() * 0.01 + modelBase.item(i,3)->text().toFloat() * modelBase.item(i,21)->text().toFloat();
                    modelBase.item(i,6)->setText(QString().sprintf("%0.2f",aa));
                    zhekou += modelBase.item(i,3)->text().toFloat() * modelBase.item(i,4)->text().toFloat() * (100 - discount_num.toFloat()) * 0.01;
                }

                if(modelBase.item(i,8)->text() == "Y" && modelBase.item(i,9)->text() != "Y")
                {
                    zengsong += modelBase.item(i,3)->text().toFloat() * modelBase.item(i,4)->text().toFloat();
                }
            }
        }
        else
        {
            for(int i = 0; i < modelBase.rowCount(); i++)
            {
                if(modelBase.item(i,9)->text() != "Y")
                {
                    xiaofei += modelBase.item(i,3)->text().toFloat() * modelBase.item(i,4)->text().toFloat()  + modelBase.item(i,3)->text().toFloat() * modelBase.item(i,21)->text().toFloat();
                    shuishou += modelBase.item(i,3)->text().toFloat() * modelBase.item(i,4)->text().toFloat() * modelBase.item(i,17)->text().toFloat() * 0.01 ;
                }

                if(modelBase.item(i,9)->text() != "Y" &&  modelBase.item(i,10)->text() != "Y" && modelBase.item(i,18)->text() == "Y" && modelBase.item(i,5)->text() != "100" && modelBase.item(i,8)->text() == "N")
                {
                    zhekou += modelBase.item(i,3)->text().toFloat() * modelBase.item(i,4)->text().toFloat() * (100-modelBase.item(i,5)->text().toFloat()) * 0.01;
                }

                if(modelBase.item(i,8)->text() == "Y" && modelBase.item(i,9)->text() != "Y")
                {
                    zengsong += modelBase.item(i,3)->text().toFloat() * modelBase.item(i,4)->text().toFloat();
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
                for(int i = 0; i < modelBase.rowCount(); i++)
                {
                    if(modelBase.item(i,9)->text() != "Y")
                    {
                        xiaofei += modelBase.item(i,3)->text().toFloat() * modelBase.item(i,4)->text().toFloat()  + modelBase.item(i,3)->text().toFloat() * modelBase.item(i,21)->text().toFloat();
                        shuishou += modelBase.item(i,3)->text().toFloat() * modelBase.item(i,4)->text().toFloat() * modelBase.item(i,17)->text().toFloat() * 0.01 ;
                    }

                    if(modelBase.item(i,9)->text() != "Y" &&  modelBase.item(i,10)->text() != "Y" && modelBase.item(i,18)->text() == "Y" && modelBase.item(i,8)->text() == "N")
                    {
                        modelBase.item(i,5)->setText(QString().sprintf("%0.2f",discount_num.toFloat()));
                        modelBase.item(i,19)->setText(time);
                        float aa = 0.00;
                        aa = modelBase.item(i,3)->text().toFloat() * modelBase.item(i,4)->text().toFloat() * modelBase.item(i,5)->text().toFloat() * 0.01 + modelBase.item(i,3)->text().toFloat() * modelBase.item(i,21)->text().toFloat();
                        modelBase.item(i,6)->setText(QString().sprintf("%0.2f",aa));
                        zhekou += modelBase.item(i,3)->text().toFloat() * modelBase.item(i,4)->text().toFloat() * (100 - discount_num.toFloat()) * 0.01;
                    }

                    if(modelBase.item(i,8)->text() == "Y" && modelBase.item(i,9)->text() != "Y")
                    {
                        zengsong += modelBase.item(i,3)->text().toFloat() * modelBase.item(i,4)->text().toFloat();
                    }
                }
            }
            else
            {
                for(int i = 0; i < modelBase.rowCount(); i++)
                {
                    if(modelBase.item(i,9)->text() != "Y")
                    {
                        xiaofei += modelBase.item(i,3)->text().toFloat() * modelBase.item(i,4)->text().toFloat() + modelBase.item(i,3)->text().toFloat() * modelBase.item(i,21)->text().toFloat();
                        shuishou += modelBase.item(i,3)->text().toFloat() * modelBase.item(i,4)->text().toFloat() * modelBase.item(i,17)->text().toFloat() * 0.01 ;
                    }
                    if(modelBase.item(i,9)->text() != "Y" &&  modelBase.item(i,10)->text() != "Y" && modelBase.item(i,18)->text() == "Y" && modelBase.item(i,5)->text() != "100"&& modelBase.item(i,8)->text() == "N")
                    {
                        zhekou += modelBase.item(i,3)->text().toFloat() * modelBase.item(i,4)->text().toFloat() * (100 -modelBase.item(i,5)->text().toFloat()) * 0.01;
                    }

                    if(modelBase.item(i,8)->text() == "Y" && modelBase.item(i,9)->text() != "Y")
                    {
                        zengsong += modelBase.item(i,3)->text().toFloat() * modelBase.item(i,4)->text().toFloat();
                    }
                }
            }
        }
        else
        {
            for(int i = 0; i < modelBase.rowCount(); i++)
            {
                if(modelBase.item(i,9)->text() != "Y")
                {
                    xiaofei += modelBase.item(i,3)->text().toFloat() * modelBase.item(i,4)->text().toFloat() + modelBase.item(i,3)->text().toFloat() * modelBase.item(i,21)->text().toFloat();
                    shuishou += modelBase.item(i,3)->text().toFloat() * modelBase.item(i,4)->text().toFloat() * modelBase.item(i,17)->text().toFloat() * 0.01 ;
                }

                if(modelBase.item(i,9)->text() != "Y" &&  modelBase.item(i,10)->text() != "Y" && modelBase.item(i,18)->text() == "Y" && modelBase.item(i,5)->text() != "100" && modelBase.item(i,8)->text() == "N")
                {
                    zhekou += modelBase.item(i,3)->text().toFloat() * modelBase.item(i,4)->text().toFloat() * (100 -modelBase.item(i,5)->text().toFloat()) * 0.01;
                }

                if(modelBase.item(i,8)->text() == "Y" && modelBase.item(i,9)->text() != "Y")
                {
                    zengsong += modelBase.item(i,3)->text().toFloat() * modelBase.item(i,4)->text().toFloat();
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

    lineEdit = (QString().sprintf("%0.2f",xiaofei));
    lineEdit_2 = (QString().sprintf("%0.2f",zhekou));
    lineEdit_3 = (QString().sprintf("%0.2f",zengsong));
    lineEdit_4 = (QString().sprintf("%0.2f",yingshou));
    lineEdit_5 = (QString().sprintf("%0.2f",moling));
    lineEdit_6 = (QString().sprintf("%0.2f",shuishou));
    lineEdit_9 = (QString().sprintf("%0.2f",xianjin));
    label_14 = (QString().sprintf("%0.2f",yingshou));

}

void fast_order_2::print()
{
    QSettings *configIniRead = new QSettings(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    QSqlQueryModel model;
    QString str_t= configIniRead->value("system_setting/zhangdanbiaoti").toString();
    if(!str_t.isEmpty())
    {
        str_t.append("\n");
    }
    QString title = str_t +  tr("退菜单\n");//需居中
    QByteArray content;

    QSqlQueryModel m_billno;
    m_billno.setQuery("Select NOW()");
    QDateTime ldt_datetime;
    ldt_datetime.setTime(m_billno.record(0).value(0).toTime());
    ldt_datetime.setDate(m_billno.record(0).value(0).toDate());
    m_billno.clear();
    QString liushi = "P" + ldt_datetime.toString("yyMMdd");
    m_billno.setQuery(QString("select Max(ch_payno) from cey_u_checkout_master where Left(ch_payno,7) = '%1' ").arg(liushi));
    QString liushuihao = m_billno.record(0).value(0).toString().trimmed();
    if(liushuihao.trimmed().isEmpty())
    {
        liushuihao = liushi + QString().sprintf("%05d",1);
    }
    else
    {
        QString str = liushuihao.right(liushuihao.length() - 7);
        int aa = str.toInt() + 1;
        liushuihao = liushi + QString().sprintf("%05d",aa);
    }

    content.append(tr("付款号:") + liushuihao + "\n");
    content.append(tr("工号:") + n_func::gs_operid + "\n");
    //打印消费内容

    //表头
    QString shifoumingxi;
    shifoumingxi = configIniRead->value("system_setting/maidanmingxi").toString();
    if(shifoumingxi == "1")
    {
        QString tou = configIniRead->value("system_setting/biaoneirong").toString();
        //QString tou = tr("品吗       数量     价格    金额\n");
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
                QStandardItemModel standmodel_cook;
                for(int m = 0; m < modelBase.rowCount(); m++)
                {
                    if(modelBase.item(m,22)->text() == dalei.record(k).value(0).toString())
                    {
                        int row = standmodel_cook.rowCount();
                        standmodel_cook.setRowCount(row + 1);
                        standmodel_cook.setItem(row,0,new QStandardItem(modelBase.item(m,0)->text()));
                        standmodel_cook.setItem(row,1,new QStandardItem(modelBase.item(m,1)->text()));
                        standmodel_cook.setItem(row,2,new QStandardItem(modelBase.item(m,2)->text()));
                        standmodel_cook.setItem(row,3,new QStandardItem(modelBase.item(m,3)->text()));
                        standmodel_cook.setItem(row,4,new QStandardItem(modelBase.item(m,4)->text()));
                        standmodel_cook.setItem(row,5,new QStandardItem(modelBase.item(m,5)->text()));
                        standmodel_cook.setItem(row,6,new QStandardItem(modelBase.item(m,6)->text()));
                        standmodel_cook.setItem(row,7,new QStandardItem(modelBase.item(m,7)->text()));
                        standmodel_cook.setItem(row,8,new QStandardItem(modelBase.item(m,8)->text()));
                        standmodel_cook.setItem(row,9,new QStandardItem(modelBase.item(m,9)->text()));
                        standmodel_cook.setItem(row,10,new QStandardItem(modelBase.item(m,10)->text()));
                        standmodel_cook.setItem(row,11,new QStandardItem(modelBase.item(m,11)->text()));
                        standmodel_cook.setItem(row,12,new QStandardItem(modelBase.item(m,12)->text()));
                        standmodel_cook.setItem(row,13,new QStandardItem(modelBase.item(m,13)->text()));
                        standmodel_cook.setItem(row,14,new QStandardItem(modelBase.item(m,14)->text()));
                        standmodel_cook.setItem(row,15,new QStandardItem(modelBase.item(m,15)->text()));
                        standmodel_cook.setItem(row,16,new QStandardItem(modelBase.item(m,16)->text()));
                        standmodel_cook.setItem(row,17,new QStandardItem(modelBase.item(m,17)->text()));
                        standmodel_cook.setItem(row,18,new QStandardItem(modelBase.item(m,18)->text()));
                        standmodel_cook.setItem(row,19,new QStandardItem(modelBase.item(m,19)->text()));
                        standmodel_cook.setItem(row,20,new QStandardItem(modelBase.item(m,20)->text()));
                        standmodel_cook.setItem(row,21,new QStandardItem(modelBase.item(m,21)->text()));
                        standmodel_cook.setItem(row,22,new QStandardItem(modelBase.item(m,22)->text()));
                    }
                }
                QByteArray neirong;
                float zongjia = 0.00;
                for(int i = 0; i < standmodel_cook.rowCount(); i++)
                {
                    zongjia += standmodel_cook.item(i,3)->text().toFloat() * standmodel_cook.item(i,4)->text().toFloat() * (int)standmodel_cook.item(i,5)->text().toFloat() * 0.01 + standmodel_cook.item(i,3)->text().toFloat() * standmodel_cook.item(i,21)->text().toFloat();
                    QString hanghao = QString::number(i+1) + tr(".");
                    if(hanghao.length() < 3)
                    {
                        hanghao.append(" ");
                    }
                    if(bxuhao=="1")neirong.append(hanghao);
                    if(bzensong == "1")
                    {
                        if(standmodel_cook.item(i,8)->text() == tr("Y"))
                        {
                            neirong.append(tr("赠)"));
                        }
                    }

                    if(btejia == "1")
                    {
                        if(standmodel_cook.item(i,10)->text() == tr("Y"))
                        {
                            neirong.append(tr("特)"));
                        }
                    }

                    if(standmodel_cook.item(i,9)->text() == "P")
                    {
                        neirong.append(tr("套)"));
                    }

                    if(standmodel_cook.item(i,9)->text() == tr("Y"))
                    {
                        neirong.append(tr("细)"));
                    }
                    // content.append(neirong);
                    //第一行=========================================================================
                    QByteArray neirong1;
                    //品名
                    if(pinming == "1")
                    {
                        QString pin = standmodel_cook.item(i,2)->text();
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
                        QString dan = standmodel_cook.item(i,12)->text();
                        if(dan.length() >= danweilen)
                        {
                            dan = dan.left(danweilen);
                        }
                        else
                        {
                            int chang = danweilen - dan.length();
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
                        QString shu = standmodel_cook.item(i,3)->text();
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
                        QString jia = standmodel_cook.item(i,4)->text();
                        if(jiagexiaoshu == 0)
                        {
                            if(standmodel_cook.item(i,9)->text() == "Y")
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
                            if(standmodel_cook.item(i,9)->text() == "Y")
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
                            if(standmodel_cook.item(i,9)->text() == "Y")
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
                        aa = standmodel_cook.item(i,3)->text().toFloat() * standmodel_cook.item(i,4)->text().toFloat() * ((int)(standmodel_cook.item(i,5)->text().toFloat())) * 0.01 + standmodel_cook.item(i,3)->text().toFloat() * standmodel_cook.item(i,21)->text().toFloat();
                        QString jin = QString().sprintf("%0.2f",aa);

                        if(jinexiaoshu == 0)
                        {
                            if(standmodel_cook.item(i,9)->text() == "Y")
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
                            if(standmodel_cook.item(i,9)->text() == "Y")
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
                            if(standmodel_cook.item(i,9)->text() == "Y")
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
                        float aa = standmodel_cook.item(i,3)->text().toFloat() * standmodel_cook.item(i,4)->text().toFloat() * (100 - standmodel_cook.item(i,5)->text().toInt() )*0.01;
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
                        QString pin = standmodel_cook.item(i,2)->text();
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
                        QString dan = standmodel_cook.item(i,12)->text();
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
                        QString shu = standmodel_cook.item(i,3)->text();
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
                        QString jia = standmodel_cook.item(i,4)->text();
                        if(jiagexiaoshu == 0)
                        {
                            if(standmodel_cook.item(i,9)->text() == "Y")
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
                            if(standmodel_cook.item(i,9)->text() == "Y")
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
                            if(standmodel_cook.item(i,9)->text() == "Y")
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
                        aa = standmodel_cook.item(i,3)->text().toFloat() * standmodel_cook.item(i,4)->text().toFloat() * ((int)(standmodel_cook.item(i,5)->text().toFloat())) * 0.01 + standmodel_cook.item(i,3)->text().toFloat() * standmodel_cook.item(i,21)->text().toFloat();
                        QString jin = QString().sprintf("%0.2f",aa);

                        if(jinexiaoshu == 0)
                        {
                            if(standmodel_cook.item(i,9)->text() == "Y")
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
                            if(standmodel_cook.item(i,9)->text() == "Y")
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
                            if(standmodel_cook.item(i,9)->text() == "Y")
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
                        float aa = standmodel_cook.item(i,3)->text().toFloat() * standmodel_cook.item(i,4)->text().toFloat() * (100 - standmodel_cook.item(i,5)->text().toInt() )*0.01;
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
                        if(!standmodel_cook.item(i,7)->text().isEmpty())
                        {
                            neirong.append(standmodel_cook.item(i,7)->text());
                            neirong.append("\n");
                        }
                    }

                    //content
                }
                if(standmodel_cook.rowCount() > 0)
                {
                    content.append(neirong);
                    QSqlQueryModel thismodel;
                    thismodel.setQuery(QString("select vch_dish_typename from cey_bt_dish_type where ch_dish_typeno = '%1' ").arg(standmodel_cook.item(0,22)->text()));
                    QString strlei = tr("      ") + thismodel.record(0).value(0).toString() + tr("------------") + QString().sprintf("%0.2f",zongjia) + "\n";
                    content.append(strlei);
                    content.append(n_func::gs_print_line+"\n");
                }
            }
        }
        else
        {
            QStandardItemModel & cook = modelBase;
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
                    aa = cook.item(i,3)->text().toFloat() * cook.item(i,4)->text().toFloat() * ((int)(cook.item(i,5)->text().toFloat())) * 0.01 + cook.item(i,3)->text().toFloat() * cook.item(i,21)->text().toFloat();
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
                    aa = cook.item(i,3)->text().toFloat() * cook.item(i,4)->text().toFloat() * ((int)(cook.item(i,5)->text().toFloat())) * 0.01 + cook.item(i,3)->text().toFloat() * cook.item(i,21)->text().toFloat();
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
    content.append(tr("应收:") + lineEdit_4 + "\n");
    content.append(n_func::gs_print_line+"\n");
    content.append(tr("现金:") + lineEdit_4 + "\n");

    //表尾格式
    //每行打印列数

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
    int print_count = 1;//model.record(0).value(0).toInt();
    blockIns b;
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
    QString liuhuihaoma = tr("流水号:") + liushuihao_p + "\n";

    //图片打印
    QString logo = configIniRead->value("system_setting/dayinlogo").toString();
    if(logo == "1")
    {
        QString str = configIniRead->value("system_setting/dayinlogoneirong").toString();//图片路径
        b+= backheader::printer->drawLogo("1B40 1B6101", str);//清除缓存 居中 pixmap路径
    }

    //path = "192.168.1.172:9100";
    content.append("\n");
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
    if(erweima == "1")
    {
        QString str = configIniRead->value("system_setting/erweimaneirong").toString();//图片路径
        b+= backheader::printer->drawQR_Code("", str);//清除缓存 居中 pixmap路径
    }
    b += backheader::printer->drawText("1B6405 1D5601", "");//走纸5x， 切纸
    b=backheader::printer->setPrintCount(b, print_count);
    backheader::asyn_dataprint(b);
}

void fast_order_2::print_chufangdayin()
{
    //厨房打印机
    QSettings kitchenprinter(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    QSqlQueryModel printflag;
    QStandardItemModel & print_flag = modelBase;
    QByteArray content_kitchen;

    QString str;
    str =  kitchenprinter.value("w_bt_dish_Kitchenprinter_param/printer_type_com").toString();
    QString str_type = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/printer_type_com").toString();//菜品打印方式
    str.clear();
    str =  kitchenprinter.value("w_bt_dish_Kitchenprinter_param/dish_printtype_com").toString();
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
                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/serialno_check").toString();
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
                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/openbillno_check").toString();
                if(str == "1")
                {
                    content_kitchen.append(tr("开始单号:") + bendandanhao + "\n");
                }

                //餐桌名
                str.clear();
                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/tablename_check").toString();
                if(str == "1")
                {
                    content_kitchen.append(tr("餐桌名:") + tr("Q000\n"));
                }

                //点菜人
                str.clear();
                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/dishopertor_check").toString();
                if(str == "1")
                {
                    content_kitchen.append(tr("点菜人:") + n_func::gs_operid + "\n");
                }

                //点菜时间
                str.clear();
                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/dishtime_check").toString();
                if(str == "1")
                {
                    content_kitchen.append(tr("点菜时间:") + fast_order_2::diancaitime + "\n");
                }

                //打印时间
                str.clear();
                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/printtime_check").toString();
                if(str == "1")
                {
                    content_kitchen.append(tr("打印时间:") + print_timer + "\n");
                }

                //人数
                str.clear();
                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/person_check").toString();
                if(str == "1")
                {
                    content_kitchen.append(tr("人数:1\n"));
                }

                //表内容
                if(str_type == "0")
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
                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/ch_dishno_check").toString();
                if(str == "1")
                {
                    biaotou.append(tr("品名             "));
                }

                //单位
                str.clear();
                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/vch_unit_check").toString();
                if(str == "1")
                {
                    biaotou.append(tr("单位   "));
                }

                //品码
                str.clear();
                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/vch_dishname_check").toString();
                if(str == "1")
                {
                    biaotou.append(tr("品码     "));
                }

                //数量
                str.clear();
                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_num_check").toString();
                if(str == "1")
                {
                    biaotou.append(tr("数量     "));
                }

                //价格
                str.clear();
                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_price_check").toString();
                if(str == "1")
                {
                    biaotou.append(tr("价格"));
                }

                content_kitchen.append(biaotou + "\n");
                if(str_type == "0")
                {
                    content_kitchen.append("----------------------------\n");
                }
                else
                {
                    content_kitchen.append("--------------------------------\n");
                }
                //一菜一单打印菜品小类
                str.clear();
                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/dish_bill_printtype_check").toString();
                if(str == "1")
                {
                    QString daleimingcheng;
                    QSqlQueryModel mymodel;
                    mymodel.setQuery(QString("select vch_sub_typename from cey_bt_dish_subtype where ch_sub_typeno = '%1' ").arg(print_flag.item(i,23)->text()));
                    if(str_type == "0")
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
                str =  kitchenprinter.value("w_bt_dish_Kitchenprinter_param/selfbillno_check").toString();
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
                str =  kitchenprinter.value("w_bt_dish_Kitchenprinter_param/presentflag_check").toString();
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
                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/ch_dishno_check").toString();
                QString hangneirong;
                if(str == "1")
                {
                    //品名宽度
                    str.clear();
                    str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/ch_dishno_size_spin").toString();
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
                    str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/ch_dishno_hang_spin").toString();
                    if(str == "1")
                    {
                        hangneirong.append(pinming);
                    }
                }

                //单位
                str.clear();
                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/vch_unit_check").toString();
                if(str == "1")
                {
                    //单位宽度
                    str.clear();
                    str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/vch_unit_size_spin").toString();
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
                    str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/vch_unit_hang_spin").toString();
                    if(str == "1")
                    {
                        hangneirong.append(danw);
                    }
                }

                //品码
                str.clear();
                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/vch_dishname_check").toString();
                if(str == "1")
                {
                    //品码宽度
                    str.clear();
                    str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/vch_dishname_size_spin").toString();
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
                    str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/vch_dishname_hang_spin").toString();
                    if(str == "1")
                    {
                        hangneirong.append(pinma);
                    }
                }

                //数量
                str.clear();
                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_num_check").toString();
                if(str == "1")
                {
                    QString shuliang;
                    str.clear();
                    str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_num_xiaoshu_spin").toString();
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
                    str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_num_size_spin").toString();
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
                    str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_num_hang_spin").toString();
                    if(str == "1")
                    {
                        hangneirong.append(shuliang);
                    }
                }


                //价格
                str.clear();
                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_price_check").toString();
                if(str == "1")
                {
                    QString jiage;
                    str.clear();
                    str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_price_xiaoshu_spin").toString();
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
                    str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_price_size_spin").toString();
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
                    str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_price_hang_spin").toString();
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
                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/ch_dishno_check").toString();
                QString hangneirong1;
                if(str == "1")
                {
                    //品名宽度
                    str.clear();
                    str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/ch_dishno_size_spin").toString();
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
                    str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/ch_dishno_hang_spin").toString();
                    if(str == "2")
                    {
                        hangneirong1.append(pinming);
                    }
                }

                //单位
                str.clear();
                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/vch_unit_check").toString();
                if(str == "1")
                {
                    //单位宽度
                    str.clear();
                    str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/vch_unit_size_spin").toString();
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
                    str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/vch_unit_hang_spin").toString();
                    if(str == "2")
                    {
                        hangneirong1.append(danw);
                    }
                }

                //品码
                str.clear();
                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/vch_dishname_check").toString();
                if(str == "1")
                {
                    //品码宽度
                    str.clear();
                    str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/vch_dishname_size_spin").toString();
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
                    str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/vch_dishname_hang_spin").toString();
                    if(str == "2")
                    {
                        hangneirong1.append(pinma);
                    }
                }

                //数量
                str.clear();
                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_num_check").toString();
                if(str == "1")
                {
                    QString shuliang;
                    str.clear();
                    str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_num_xiaoshu_spin").toString();
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
                    str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_num_size_spin").toString();
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
                    str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_num_hang_spin").toString();
                    if(str == "2")
                    {
                        hangneirong1.append(shuliang);
                    }
                }


                //价格
                str.clear();
                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_price_check").toString();
                if(str == "1")
                {
                    QString jiage;
                    str.clear();
                    str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_price_xiaoshu_spin").toString();
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
                    str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_price_size_spin").toString();
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
                    str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_price_hang_spin").toString();
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
                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/dishsum_numprice_check").toString();
                if(str == "1")
                {
                    if(str_type == "0")
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
                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/kitchen_num_spin").toString();
                int print_count = str.toInt();
                blockIns b;
                QString path = printflag.record(0).value(1).toString() + ":9100";

                //                public_mainprinter *chuda_print = new public_mainprinter();
                str.clear();
                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/printer_fontsize_com").toString();
                if(str == "1")//普通字体
                {
                    b+=backheader::printer->drawText("1B40","");
                    b+=backheader::printer->drawText("1B40 1B6101 1C5700",tr("菜品退菜单\n"));//居中放大
                    b += backheader::printer->drawText("1B40 1C5700", content_kitchen, Qt::AlignLeft);

                }
                else
                {
                    b+=backheader::printer->drawText("1B40","");
                    b+=backheader::printer->drawText("1B40 1B6101 1D2101",tr("菜品退菜单\n"));//居中放大
                    b += backheader::printer->drawText("1B40 1D2101", content_kitchen, Qt::AlignLeft);
                }
                b += backheader::printer->drawText("1B6405 1D5601", "");//走纸5x， 切纸
                b=backheader::printer->setPrintCount(b, print_count);
                // printer->asynchPrint(b, path);
                if(backheader::syn_dataprint(b,path) == true)//成功,更新数据库打印菜品大类
                {
                    QSqlQuery query;
                    QSqlDatabase::database().transaction();

                    query.prepare(QString("update cey_u_orderdish set ch_printflag ='Y' where ch_billno =:danhao and ch_dishno = :bianma"));
                    query.bindValue(":danhao",dayindanhao);
                    query.bindValue(":bianma",print_flag.item(i,1)->text());
                    if(query.exec())
                    {

                    }
                    else
                    {
                        QSqlDatabase::database().rollback();
                        ;
                        lds_messagebox::warning(this, tr("消息提示"),tr("厨房打印更新数据库失败!"));

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
            printflag.setQuery(QString("select ch_printflag,vch_printer from cey_bt_dish where ch_dishno = '%1'").arg(print_flag.item(i,1)->text()));
            if(printflag.record(0).value(0).toString() == "Y")
            {
                if(!iplist.contains(printflag.record(0).value(1).toString()))
                {
                    iplist << printflag.record(0).value(1).toString();
                }
            }
        }

        float zongji = 0.00;
        str.clear();
        str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/table_bill_printseries_check").toString();
        QString capinxiaolei = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/table_bill_printtype_check").toString();
        QString caipindalei = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/table_bill_printseries_check").toString();
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
                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/serialno_check").toString();
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
                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/openbillno_check").toString();
                if(str == "1")
                {
                    content_kitchen.append(tr("开始单号:") + bendandanhao + "\n");
                }

                //餐桌名
                str.clear();
                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/tablename_check").toString();
                if(str == "1")
                {
                    content_kitchen.append(tr("餐桌名:") + tr("Q000\n"));
                }

                //点菜人
                str.clear();
                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/dishopertor_check").toString();
                if(str == "1")
                {
                    content_kitchen.append(tr("点菜人:") + n_func::gs_operid + "\n");
                }

                //点菜时间
                str.clear();
                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/dishtime_check").toString();
                if(str == "1")
                {
                    content_kitchen.append(tr("点菜时间:") + fast_order_2::diancaitime + "\n");
                }

                //打印时间
                str.clear();
                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/printtime_check").toString();
                if(str == "1")
                {
                    content_kitchen.append(tr("打印时间:") + print_timer + "\n");
                }

                //人数
                str.clear();
                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/person_check").toString();
                if(str == "1")
                {
                    content_kitchen.append(tr("人数:1\n"));
                }

                //表内容
                if(str_type == "0")
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
                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/ch_dishno_check").toString();
                if(str == "1")
                {
                    biaotou.append(tr("品名             "));
                }

                //单位
                str.clear();
                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/vch_unit_check").toString();
                if(str == "1")
                {
                    biaotou.append(tr("单位   "));
                }

                //品码
                str.clear();
                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/vch_dishname_check").toString();
                if(str == "1")
                {
                    biaotou.append(tr("品码     "));
                }

                //数量
                str.clear();
                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_num_check").toString();
                if(str == "1")
                {
                    biaotou.append(tr("数量     "));
                }

                //价格
                str.clear();
                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_price_check").toString();
                if(str == "1")
                {
                    biaotou.append(tr("价格"));
                }

                content_kitchen.append(biaotou + "\n");
                if(str_type == "0")
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

            QStandardItemModel print_model;
            for(int h = 0; h < dalei.rowCount(); h++)
            {
                QStandardItemModel cook;
                for(int m = 0; m < modelBase.rowCount(); m++)
                {
                    printflag.clear();
                    printflag.setQuery(QString("select ch_printflag,vch_printer from cey_bt_dish where ch_dishno = '%1'").arg(modelBase.item(m,1)->text()));
                    if(modelBase.item(m,22)->text() == dalei.record(h).value(0).toString() && printflag.record(0).value(0).toString() == "Y" && printflag.record(0).value(1).toString() == iplist[k])
                    {
                        int row = cook.rowCount();
                        cook.setRowCount(row + 1);
                        cook.setItem(row,0,new QStandardItem(modelBase.item(m,0)->text()));
                        cook.setItem(row,1,new QStandardItem(modelBase.item(m,1)->text()));
                        cook.setItem(row,2,new QStandardItem(modelBase.item(m,2)->text()));
                        cook.setItem(row,3,new QStandardItem(modelBase.item(m,3)->text()));
                        cook.setItem(row,4,new QStandardItem(modelBase.item(m,4)->text()));
                        cook.setItem(row,5,new QStandardItem(modelBase.item(m,5)->text()));
                        cook.setItem(row,6,new QStandardItem(modelBase.item(m,6)->text()));
                        cook.setItem(row,7,new QStandardItem(modelBase.item(m,7)->text()));
                        cook.setItem(row,8,new QStandardItem(modelBase.item(m,8)->text()));
                        cook.setItem(row,9,new QStandardItem(modelBase.item(m,9)->text()));
                        cook.setItem(row,10,new QStandardItem(modelBase.item(m,10)->text()));
                        cook.setItem(row,11,new QStandardItem(modelBase.item(m,11)->text()));
                        cook.setItem(row,12,new QStandardItem(modelBase.item(m,12)->text()));
                        cook.setItem(row,13,new QStandardItem(modelBase.item(m,13)->text()));
                        cook.setItem(row,14,new QStandardItem(modelBase.item(m,14)->text()));
                        cook.setItem(row,15,new QStandardItem(modelBase.item(m,15)->text()));
                        cook.setItem(row,16,new QStandardItem(modelBase.item(m,16)->text()));
                        cook.setItem(row,17,new QStandardItem(modelBase.item(m,17)->text()));
                        cook.setItem(row,18,new QStandardItem(modelBase.item(m,18)->text()));
                        cook.setItem(row,19,new QStandardItem(modelBase.item(m,19)->text()));
                        cook.setItem(row,20,new QStandardItem(modelBase.item(m,20)->text()));
                        cook.setItem(row,21,new QStandardItem(modelBase.item(m,21)->text()));
                        cook.setItem(row,22,new QStandardItem(modelBase.item(m,22)->text()));

                        int row1 = print_model.rowCount();
                        print_model.setRowCount(row1 + 1);
                        print_model.setItem(row1,0,new QStandardItem(modelBase.item(m,0)->text()));
                        print_model.setItem(row1,1,new QStandardItem(modelBase.item(m,1)->text()));
                    }
                }

                //寻找大类
                QString daleimingcheng;
                QSqlQueryModel mymodel;
                mymodel.setQuery(QString("select vch_dish_typename from cey_bt_dish_type where ch_dish_typeno = '%1' ").arg(dalei.record(h).value(0).toString()));
                QSqlQueryModel mod_xiao;
                mod_xiao.setQuery(QString("select vch_sub_typename from cey_bt_dish_subtype where ch_sub_typeno = '%1' ").arg(dalei.record(h).value(0).toString()));
                if(caipindalei  == "1")
                {
                    if(str_type == "0")
                    {
                        daleimingcheng = tr("            ---------") + mymodel.record(0).value(0).toString() + tr("---------\n");
                    }
                    else
                    {
                        daleimingcheng = tr("      ---------") + mymodel.record(0).value(0).toString() + tr("---------\n");
                    }

                    if(cook.rowCount() > 0)
                    {
                        content_kitchen.append(daleimingcheng);
                    }
                }
                else if(capinxiaolei == "1")
                {
                    if(str_type == "0")
                    {
                        daleimingcheng = tr("            ---------") + mod_xiao.record(0).value(0).toString() + tr("---------\n");
                    }
                    else
                    {
                        daleimingcheng = tr("      ---------") + mod_xiao.record(0).value(0).toString() + tr("---------\n");
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
                        zongji += cook.item(i,3)->text().toFloat() * cook.item(i,4)->text().toFloat() * cook.item(i,5)->text().toFloat() * 0.01 + cook.item(i,3)->text().toFloat() * cook.item(i,21)->text().toFloat();
                        //qDebug() << cook.item(i,3)->text().toFloat() * cook.item(i,4)->text().toFloat() * cook.item(i,5)->text().toFloat() * 0.01 + cook.item(i,3)->text().toFloat() * cook.item(i,21)->text().toFloat() + cook.item(i,3)->text().toFloat() * cook.item(i,17)->text().toFloat() * 0.01;
                    }
                    //本单序号
                    //要判断是否套菜
                    if(cook.item(i,9)->text() == "N")//不是套菜
                    {
                        hanghao++;
                        str.clear();
                        str =  kitchenprinter.value("w_bt_dish_Kitchenprinter_param/selfbillno_check").toString();
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
                        str =  kitchenprinter.value("w_bt_dish_Kitchenprinter_param/presentflag_check").toString();
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
                        str =  kitchenprinter.value("w_bt_dish_Kitchenprinter_param/suit_printtype_com").toString();
                        if(str == "0")//主套菜
                        {
                            if(cook.item(i,9)->text() == "P")
                            {
                                hanghao++;
                                str.clear();
                                str =  kitchenprinter.value("w_bt_dish_Kitchenprinter_param/selfbillno_check").toString();
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
                                str =  kitchenprinter.value("w_bt_dish_Kitchenprinter_param/presentflag_check").toString();
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
                                str =  kitchenprinter.value("w_bt_dish_Kitchenprinter_param/selfbillno_check").toString();
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
                                str =  kitchenprinter.value("w_bt_dish_Kitchenprinter_param/presentflag_check").toString();
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
                            str =  kitchenprinter.value("w_bt_dish_Kitchenprinter_param/selfbillno_check").toString();
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
                            str =  kitchenprinter.value("w_bt_dish_Kitchenprinter_param/presentflag_check").toString();
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
                        str =  kitchenprinter.value("w_bt_dish_Kitchenprinter_param/suit_printtype_com").toString();
                        if(str == "0")//主套菜
                        {
                            if(cook.item(i,9)->text() == "P")
                            {
                                //厨打第一行==================================================

                                //品名
                                str.clear();
                                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/ch_dishno_check").toString();
                                QString hangneirong;
                                if(str == "1")
                                {
                                    //品名宽度
                                    str.clear();
                                    str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/ch_dishno_size_spin").toString();
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
                                    str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/ch_dishno_hang_spin").toString();
                                    if(str == "1")
                                    {
                                        hangneirong.append(pinming);
                                    }
                                }

                                //单位
                                str.clear();
                                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/vch_unit_check").toString();
                                if(str == "1")
                                {
                                    //单位宽度
                                    str.clear();
                                    str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/vch_unit_size_spin").toString();
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
                                    str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/vch_unit_hang_spin").toString();
                                    if(str == "1")
                                    {
                                        hangneirong.append(danw);
                                    }
                                }

                                //品码
                                str.clear();
                                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/vch_dishname_check").toString();
                                if(str == "1")
                                {
                                    //品码宽度
                                    str.clear();
                                    str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/vch_dishname_size_spin").toString();
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
                                    str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/vch_dishname_hang_spin").toString();
                                    if(str == "1")
                                    {
                                        hangneirong.append(pinma);
                                    }
                                }

                                //数量
                                str.clear();
                                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_num_check").toString();
                                if(str == "1")
                                {
                                    QString shuliang;
                                    str.clear();
                                    str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_num_xiaoshu_spin").toString();
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
                                    str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_num_size_spin").toString();
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
                                    str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_num_hang_spin").toString();
                                    if(str == "1")
                                    {
                                        hangneirong.append(shuliang);
                                    }
                                }


                                //价格
                                str.clear();
                                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_price_check").toString();
                                if(str == "1")
                                {
                                    QString jiage;
                                    str.clear();
                                    str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_price_xiaoshu_spin").toString();
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
                                    str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_price_size_spin").toString();
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
                                    str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_price_hang_spin").toString();
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
                                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/ch_dishno_check").toString();
                                QString hangneirong1;
                                if(str == "1")
                                {
                                    //品名宽度
                                    str.clear();
                                    str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/ch_dishno_size_spin").toString();
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
                                    str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/ch_dishno_hang_spin").toString();
                                    if(str == "2")
                                    {
                                        hangneirong1.append(pinming);
                                    }
                                }

                                //单位
                                str.clear();
                                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/vch_unit_check").toString();
                                if(str == "1")
                                {
                                    //单位宽度
                                    str.clear();
                                    str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/vch_unit_size_spin").toString();
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
                                    str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/vch_unit_hang_spin").toString();
                                    if(str == "2")
                                    {
                                        hangneirong1.append(danw);
                                    }
                                }

                                //品码
                                str.clear();
                                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/vch_dishname_check").toString();
                                if(str == "1")
                                {
                                    //品码宽度
                                    str.clear();
                                    str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/vch_dishname_size_spin").toString();
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
                                    str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/vch_dishname_hang_spin").toString();
                                    if(str == "2")
                                    {
                                        hangneirong1.append(pinma);
                                    }
                                }

                                //数量
                                str.clear();
                                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_num_check").toString();
                                if(str == "1")
                                {
                                    QString shuliang;
                                    str.clear();
                                    str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_num_xiaoshu_spin").toString();
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
                                    str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_num_size_spin").toString();
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
                                    str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_num_hang_spin").toString();
                                    if(str == "2")
                                    {
                                        hangneirong1.append(shuliang);
                                    }
                                }


                                //价格
                                str.clear();
                                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_price_check").toString();
                                if(str == "1")
                                {
                                    QString jiage;
                                    str.clear();
                                    str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_price_xiaoshu_spin").toString();
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
                                    str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_price_size_spin").toString();
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
                                    str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_price_hang_spin").toString();
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
                                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/ch_dishno_check").toString();
                                QString hangneirong;
                                if(str == "1")
                                {
                                    //品名宽度
                                    str.clear();
                                    str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/ch_dishno_size_spin").toString();
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
                                    str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/ch_dishno_hang_spin").toString();
                                    if(str == "1")
                                    {
                                        hangneirong.append(pinming);
                                    }
                                }

                                //单位
                                str.clear();
                                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/vch_unit_check").toString();
                                if(str == "1")
                                {
                                    //单位宽度
                                    str.clear();
                                    str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/vch_unit_size_spin").toString();
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
                                    str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/vch_unit_hang_spin").toString();
                                    if(str == "1")
                                    {
                                        hangneirong.append(danw);
                                    }
                                }

                                //品码
                                str.clear();
                                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/vch_dishname_check").toString();
                                if(str == "1")
                                {
                                    //品码宽度
                                    str.clear();
                                    str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/vch_dishname_size_spin").toString();
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
                                    str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/vch_dishname_hang_spin").toString();
                                    if(str == "1")
                                    {
                                        hangneirong.append(pinma);
                                    }
                                }

                                //数量
                                str.clear();
                                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_num_check").toString();
                                if(str == "1")
                                {
                                    QString shuliang;
                                    str.clear();
                                    str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_num_xiaoshu_spin").toString();
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
                                    str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_num_size_spin").toString();
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
                                    str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_num_hang_spin").toString();
                                    if(str == "1")
                                    {
                                        hangneirong.append(shuliang);
                                    }
                                }


                                //价格
                                str.clear();
                                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_price_check").toString();
                                if(str == "1")
                                {
                                    QString jiage;
                                    str.clear();
                                    str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_price_xiaoshu_spin").toString();
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
                                    str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_price_size_spin").toString();
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
                                    str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_price_hang_spin").toString();
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
                                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/ch_dishno_check").toString();
                                QString hangneirong1;
                                if(str == "1")
                                {
                                    //品名宽度
                                    str.clear();
                                    str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/ch_dishno_size_spin").toString();
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
                                    str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/ch_dishno_hang_spin").toString();
                                    if(str == "2")
                                    {
                                        hangneirong1.append(pinming);
                                    }
                                }

                                //单位
                                str.clear();
                                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/vch_unit_check").toString();
                                if(str == "1")
                                {
                                    //单位宽度
                                    str.clear();
                                    str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/vch_unit_size_spin").toString();
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
                                    str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/vch_unit_hang_spin").toString();
                                    if(str == "2")
                                    {
                                        hangneirong1.append(danw);
                                    }
                                }

                                //品码
                                str.clear();
                                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/vch_dishname_check").toString();
                                if(str == "1")
                                {
                                    //品码宽度
                                    str.clear();
                                    str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/vch_dishname_size_spin").toString();
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
                                    str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/vch_dishname_hang_spin").toString();
                                    if(str == "2")
                                    {
                                        hangneirong1.append(pinma);
                                    }
                                }

                                //数量
                                str.clear();
                                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_num_check").toString();
                                if(str == "1")
                                {
                                    QString shuliang;
                                    str.clear();
                                    str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_num_xiaoshu_spin").toString();
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
                                    str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_num_size_spin").toString();
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
                                    str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_num_hang_spin").toString();
                                    if(str == "2")
                                    {
                                        hangneirong1.append(shuliang);
                                    }
                                }


                                //价格
                                str.clear();
                                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_price_check").toString();
                                if(str == "1")
                                {
                                    QString jiage;
                                    str.clear();
                                    str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_price_xiaoshu_spin").toString();
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
                                    str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_price_size_spin").toString();
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
                                    str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_price_hang_spin").toString();
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
                            str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/ch_dishno_check").toString();
                            QString hangneirong;
                            if(str == "1")
                            {
                                //品名宽度
                                str.clear();
                                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/ch_dishno_size_spin").toString();
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
                                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/ch_dishno_hang_spin").toString();
                                if(str == "1")
                                {
                                    hangneirong.append(pinming);
                                }
                            }

                            //单位
                            str.clear();
                            str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/vch_unit_check").toString();
                            if(str == "1")
                            {
                                //单位宽度
                                str.clear();
                                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/vch_unit_size_spin").toString();
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
                                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/vch_unit_hang_spin").toString();
                                if(str == "1")
                                {
                                    hangneirong.append(danw);
                                }
                            }

                            //品码
                            str.clear();
                            str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/vch_dishname_check").toString();
                            if(str == "1")
                            {
                                //品码宽度
                                str.clear();
                                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/vch_dishname_size_spin").toString();
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
                                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/vch_dishname_hang_spin").toString();
                                if(str == "1")
                                {
                                    hangneirong.append(pinma);
                                }
                            }

                            //数量
                            str.clear();
                            str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_num_check").toString();
                            if(str == "1")
                            {
                                QString shuliang;
                                str.clear();
                                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_num_xiaoshu_spin").toString();
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
                                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_num_size_spin").toString();
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
                                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_num_hang_spin").toString();
                                if(str == "1")
                                {
                                    hangneirong.append(shuliang);
                                }
                            }


                            //价格
                            str.clear();
                            str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_price_check").toString();
                            if(str == "1")
                            {
                                QString jiage;
                                str.clear();
                                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_price_xiaoshu_spin").toString();
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
                                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_price_size_spin").toString();
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
                                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_price_hang_spin").toString();
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
                            str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/ch_dishno_check").toString();
                            QString hangneirong1;
                            if(str == "1")
                            {
                                //品名宽度
                                str.clear();
                                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/ch_dishno_size_spin").toString();
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
                                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/ch_dishno_hang_spin").toString();
                                if(str == "2")
                                {
                                    hangneirong1.append(pinming);
                                }
                            }

                            //单位
                            str.clear();
                            str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/vch_unit_check").toString();
                            if(str == "1")
                            {
                                //单位宽度
                                str.clear();
                                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/vch_unit_size_spin").toString();
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
                                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/vch_unit_hang_spin").toString();
                                if(str == "2")
                                {
                                    hangneirong1.append(danw);
                                }
                            }

                            //品码
                            str.clear();
                            str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/vch_dishname_check").toString();
                            if(str == "1")
                            {
                                //品码宽度
                                str.clear();
                                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/vch_dishname_size_spin").toString();
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
                                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/vch_dishname_hang_spin").toString();
                                if(str == "2")
                                {
                                    hangneirong1.append(pinma);
                                }
                            }

                            //数量
                            str.clear();
                            str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_num_check").toString();
                            if(str == "1")
                            {
                                QString shuliang;
                                str.clear();
                                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_num_xiaoshu_spin").toString();
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
                                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_num_size_spin").toString();
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
                                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_num_hang_spin").toString();
                                if(str == "2")
                                {
                                    hangneirong1.append(shuliang);
                                }
                            }


                            //价格
                            str.clear();
                            str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_price_check").toString();
                            if(str == "1")
                            {
                                QString jiage;
                                str.clear();
                                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_price_xiaoshu_spin").toString();
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
                                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_price_size_spin").toString();
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
                                str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_price_hang_spin").toString();
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
                        str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/ch_dishno_check").toString();
                        QString hangneirong;
                        if(str == "1")
                        {
                            //品名宽度
                            str.clear();
                            str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/ch_dishno_size_spin").toString();
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
                            str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/ch_dishno_hang_spin").toString();
                            if(str == "1")
                            {
                                hangneirong.append(pinming);
                            }
                        }

                        //单位
                        str.clear();
                        str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/vch_unit_check").toString();
                        if(str == "1")
                        {
                            //单位宽度
                            str.clear();
                            str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/vch_unit_size_spin").toString();
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
                            str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/vch_unit_hang_spin").toString();
                            if(str == "1")
                            {
                                hangneirong.append(danw);
                            }
                        }

                        //品码
                        str.clear();
                        str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/vch_dishname_check").toString();
                        if(str == "1")
                        {
                            //品码宽度
                            str.clear();
                            str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/vch_dishname_size_spin").toString();
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
                            str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/vch_dishname_hang_spin").toString();
                            if(str == "1")
                            {
                                hangneirong.append(pinma);
                            }
                        }

                        //数量
                        str.clear();
                        str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_num_check").toString();
                        if(str == "1")
                        {
                            QString shuliang;
                            str.clear();
                            str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_num_xiaoshu_spin").toString();
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
                            str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_num_size_spin").toString();
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
                            str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_num_hang_spin").toString();
                            if(str == "1")
                            {
                                hangneirong.append(shuliang);
                            }
                        }


                        //价格
                        str.clear();
                        str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_price_check").toString();
                        if(str == "1")
                        {
                            QString jiage;
                            str.clear();
                            str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_price_xiaoshu_spin").toString();
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
                            str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_price_size_spin").toString();
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
                            str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_price_hang_spin").toString();
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
                        str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/ch_dishno_check").toString();
                        QString hangneirong1;
                        if(str == "1")
                        {
                            //品名宽度
                            str.clear();
                            str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/ch_dishno_size_spin").toString();
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
                            str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/ch_dishno_hang_spin").toString();
                            if(str == "2")
                            {
                                hangneirong1.append(pinming);
                            }
                        }

                        //单位
                        str.clear();
                        str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/vch_unit_check").toString();
                        if(str == "1")
                        {
                            //单位宽度
                            str.clear();
                            str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/vch_unit_size_spin").toString();
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
                            str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/vch_unit_hang_spin").toString();
                            if(str == "2")
                            {
                                hangneirong1.append(danw);
                            }
                        }

                        //品码
                        str.clear();
                        str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/vch_dishname_check").toString();
                        if(str == "1")
                        {
                            //品码宽度
                            str.clear();
                            str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/vch_dishname_size_spin").toString();
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
                            str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/vch_dishname_hang_spin").toString();
                            if(str == "2")
                            {
                                hangneirong1.append(pinma);
                            }
                        }

                        //数量
                        str.clear();
                        str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_num_check").toString();
                        if(str == "1")
                        {
                            QString shuliang;
                            str.clear();
                            str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_num_xiaoshu_spin").toString();
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
                            str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_num_size_spin").toString();
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
                            str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_num_hang_spin").toString();
                            if(str == "2")
                            {
                                hangneirong1.append(shuliang);
                            }
                        }


                        //价格
                        str.clear();
                        str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_price_check").toString();
                        if(str == "1")
                        {
                            QString jiage;
                            str.clear();
                            str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_price_xiaoshu_spin").toString();
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
                            str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_price_size_spin").toString();
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
                            str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/num_price_hang_spin").toString();
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
            str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/dishsum_numprice_check").toString();
            if(str == "1")
            {
                if(str_type == "0")
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
            str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/kitchen_num_spin").toString();
            int print_count = str.toInt();
            blockIns b;
            QString path = iplist[k] + ":9100";
            louda_ip = iplist[k];

            //            public_mainprinter *chuda_print_yizhuo = new public_mainprinter();
            louda_neirong = content_kitchen;
            louda_title = tr("菜品退菜单");
            str.clear();
            str = kitchenprinter.value("w_bt_dish_Kitchenprinter_param/printer_fontsize_com").toString();
            if(str == "1")//普通字体
            {
                b+=backheader::printer->drawText("1B40","");
                b+=backheader::printer->drawText("1B40 1B6101 1C5700",tr("菜品退菜单\n"));//居中放大
                b += backheader::printer->drawText("1B40 1C5700", content_kitchen, Qt::AlignLeft);

            }
            else
            {
                b+=backheader::printer->drawText("1B40","");
                b+=backheader::printer->drawText("1B40 1B6101 1D2101",tr("菜品退菜单\n"));//居中放大
                b += backheader::printer->drawText("1B40 1D2101", content_kitchen, Qt::AlignLeft);
            }
            b += backheader::printer->drawText("1B6405 1D5601", "");//走纸5x， 切纸
            b=backheader::printer->setPrintCount(b, print_count);

            if(backheader::syn_dataprint(b,path) == true)//成功,更新数据库打印菜品大类
            {
                QSqlQuery query;
                QSqlDatabase::database().transaction();
                for(int ii = 0; ii < print_model.rowCount(); ii++)
                {
                    query.prepare(QString("update cey_u_orderdish set ch_printflag ='Y' where ch_billno =:danhao and ch_dishno = :bianma"));
                    query.bindValue(":danhao",dayindanhao);
                    query.bindValue(":bianma",print_model.item(ii,1)->text());
                    if(query.exec())
                    {

                    }
                    else
                    {
                        QSqlDatabase::database().rollback();
                        ;
                        lds_messagebox::warning(this, tr("消息提示"),tr("厨房打印更新数据库失败!"));

                    }
                }
                QSqlDatabase::database().commit();
            }
            else//失败
            {
                kitchenprinte_lost();
            }

            content_kitchen.clear();
            zongji= 0.00;
            hanghao = 0;

            //打印大类内容======================

        }//大类IP轮寻结

    } //一桌一单===============================

}

void fast_order_2::kitchenprinte_lost()
{
    //    ;

    //    QSqlQueryModel model;
    //    model.setQuery(QString("select Max(id) from chudaloudan"),db);

    //    QSqlQueryModel model_l;
    //    model_l.setQuery("Select NOW()");
    //    QString date = model_l.record(0).value(0).toDateTime().toString("yyyy-MM-dd");

    //    int maxid = model.record(0).value(0).toInt();

    //    if(maxid <= 0)
    //    {
    //        maxid = 1;
    //    }
    //    else
    //    {
    //        maxid++;
    //    }

    //    QSqlQuery query(db);
    //    bool bsuccess = query.exec("create table if not exists chudaloudan (id int primary key, "
    //                               "vch_ip varchar(20) ,vch_content varchar(2048),vch_danaho varchar(20),vch_title varchar(30),vch_printflag varchar(5),dt_date varchar(20))");   //创建一个表
    //    if(!bsuccess)
    //    {
    //        lds_messagebox::warning(this, tr("消息提示"),tr("创建本地数据库chudaloudan失败!"));

    //        return;
    //    }
    //    query.prepare(QString("insert into chudaloudan(id,vch_ip,vch_content,vch_danaho,vch_title,vch_printflag,dt_date) values(:id, :ip,:content,:vch_danaho,:title,:print,:date)"));
    //    query.bindValue(":id",QString::number(maxid));
    //    query.bindValue(":ip",louda_ip);
    //    query.bindValue(":content",louda_neirong);
    //    query.bindValue(":vch_danaho",dayindanhao);
    //    query.bindValue(":title",louda_title);
    //    query.bindValue(":print","N");
    //    query.bindValue(":date",date);
    //    if(query.exec())
    //    {

    //    }
    //    else
    //    {
    //        db.rollback();
    //        lds_messagebox::warning(this, tr("消息提示"),("插入漏单失败!"));

    //        return;
    //    }
}

void fast_order_2::re_model()
{
    model_dalei.setQuery(QString("select ch_dish_typeno, vch_dish_typename from cey_bt_dish_type where ch_dish_typeno in(select ch_dish_typeno from cey_bt_dish_type_oper b where b.vch_operid='%1' and b.ch_state ='Y') or '0000'='%1'").arg(n_func::gs_operid));
    for(int i = 0; i < model_dalei.rowCount(); i++)
    {
        if(i == num_da)
        {
            dalei = model_dalei.record(i).value("ch_dish_typeno").toString();
        }
    }

    model_xiaolei.setQuery(QString("SELECT ch_sub_typeno,vch_sub_typename FROM cey_bt_dish_subtype where ch_dish_typeno = '%1' ").arg(dalei));
    for(int i = 0; i < model_xiaolei.rowCount(); i++)
    {
        if(i == num_xiao)
        {
            xiaolei = model_xiaolei.record(0).value(0).toString();
        }
    }
    if(changyongcook == false)
    {
        model_cook.setQuery(QString("SELECT ch_dishno,vch_dishname,num_price FROM cey_bt_dish where ch_dish_typeno = '%1' and ch_sub_typeno ='%2' and (ch_stopflag = null or ch_stopflag = 'N') ").arg(dalei).arg(xiaolei));
    }
    else
    {
        model_cook.setQuery(QString("select ch_dishno,vch_dishname,num_price from cey_bt_dish where ch_off = 'Y' and (ch_stopflag = null or ch_stopflag = 'N') "));
    }
}

void fast_order_2::init_bedi_db()
{
    //    db = QSqlDatabase::database("loss_form");//tr("出打漏单表")
    //    db.setDatabaseName("./chudaloudan.db");  //chudaloudan.db的文件
    //    ;

    //    if(!db.open())
    //    {
    //        lds_messagebox::warning(this, tr("消息提示"),tr("打开数库失败!"));

    //        return;
    //    }
    //    else
    //    {
    //        db.open();
    //    }
}

void fast_order_2::init_color()
{
    //    currentSkin.sortOffColor = QColor(70,255,117); //玫瑰红
    //    currentSkin.sortOffFontColor = QColor(Qt::white);
    //    currentSkin.sortOnColor = QColor(70,255,117);//菜品子类
    //    currentSkin.sortOnFontColor = QColor(Qt::white);
    //    currentSkin.sortRounded = true;
    //    currentSkin.sortGradientWay = 1;

    //    currentSkin.itemOffColor = QColor(/*170,170,170*/248,51,16);     //灰灰白
    //    currentSkin.itemOffFontColor = QColor(Qt::black);//QColor(76, 86, 108);
    //    currentSkin.itemOnColor = QColor(70,70,70);
    //    currentSkin.itemOnFontColor = QColor(Qt::white);
    //    currentSkin.itemRounded = true;
    //    currentSkin.itemGradientWay = 1;
}


void fast_order_2::show_tableview_click(const QModelIndex &index)
{
    //row_n = //ui->tableView->tableview->currentIndex().row();
    //ui->tableView->tableview->viewport()->update();
}

void fast_order_2::change_label(const QString &text)
{
    ui->label_total->setText(tr("找零")+text);
}

void fast_order_2::to_payfor_info(const blockIns &bins)
{
    QString printins;
    foreach(pieceIns p, bins){
        printins.append(p.second+"\n");
    }

    ui->tableView->to_payfor_info(_fromlast_cutbetween(printins, n_func::gs_print_line));
}

void fast_order_2::showxiaolei(const QString &str)
{
    if(str.isEmpty())
    {
        return;
    }
    changyongcook = false;
    dalei = str;
    qDeleteAll(itemList_zilei.begin(),itemList_zilei.end());
    itemList_zilei.clear();
    //qDeleteAll(//ui->frame->children());
    layout_zilei = new QGridLayout();
    for(int i = 0; i < item_count_zilei; ++i)
    {
        goodItem_zilei *item = new goodItem_zilei("","");
        itemList_zilei.append(item);
        connect(item,SIGNAL(touch_zilei(QString)),this,SLOT(dis_xiaolei_str(QString)));
        layout_zilei->addWidget(item,i/item_count_row_zilei,i%item_count_row_zilei);
    }
    layout_zilei->setContentsMargins(0,6,0,0);
    layout_zilei->setSpacing(2);
    //ui->frame->setLayout(layout_zilei);

    model_xiaolei.clear();
    model_xiaolei.setQuery(QString("SELECT ch_sub_typeno,vch_sub_typename FROM cey_bt_dish_subtype where ch_dish_typeno = '%1' ").arg(dalei));

    xiaolei = model_xiaolei.record(0).value(0).toString();
    QString kind = model_xiaolei.record(0).value(0).toString();
    showGoods_zilei(kind);
}

bool fast_order_2::showGoods(QString kindId, int index)
{
    //changyongcook = false;
    if(kindId.trimmed().isEmpty())
    {
        return false;
    }
    goodIndex = index;
    goodKind = kindId;
    int preCount;//之前的商品数量

    if(0 == index)
    {
        preCount = 0;
    }
    else
    {
        preCount = item_count-1 + (index-1)*(item_count-2);
    }

    if(bianma_daicai_type == false && changyongcook == false)
    {
        model_cook.clear();
        model_cook.setQuery(QString("SELECT ch_dishno,vch_dishname,num_price FROM cey_bt_dish where ch_dish_typeno = '%1' and ch_sub_typeno ='%2' and (ch_stopflag = null or ch_stopflag = 'N') limit %3,%4 ").arg(dalei).arg(xiaolei).arg(preCount).arg(item_count+1));
        //qDebug() << QString("SELECT ch_dishno,vch_dishname,num_price FROM cey_bt_dish where ch_dish_typeno = '%1' and ch_sub_typeno ='%2' and (ch_stopflag = null or ch_stopflag = 'N') limit %3,%4 ").arg(dalei).arg(xiaolei).arg(preCount).arg(item_count+1);
    }

    if(bianma_daicai_type == true  && changyongcook == false)
    {
        QString edit_bianma = ("%") + bianma_str + ("%");
        model_cook.clear();
        model_cook.setQuery(QString("SELECT ch_dishno,vch_dishname,num_price FROM cey_bt_dish where (ch_stopflag = null or ch_stopflag = 'N') and (ch_dishno like '%1' or vch_spell like '%2')   limit %3,%4")
                            .arg(edit_bianma,
                                 edit_bianma,
                                 QString::number(preCount),
                                 QString::number(item_count+1)));
    }

    if(bianma_daicai_type == false && changyongcook == true)
    {
        model_cook.clear();
        model_cook.setQuery(QString("select ch_dishno,vch_dishname,num_price from cey_bt_dish where ch_off ='Y' and (ch_stopflag = null or ch_stopflag = 'N')  limit %1,%2").arg(preCount).arg(item_count+1));
    }
    int count = 0;
    if(0 == index)       //如果是第一页
    {
        for(int i = 0; i < model_cook.rowCount(); i++)
        {
            if(count < item_count)      //填如所有商品到列表//子商品
            {
                itemList.value(count)->_barcode = model_cook.record(i).value(0).toString();
                itemList.value(count)->_isSelect = false;
                itemList.value(count)->setName(model_cook.record(i).value(1).toString() + "\n" + QString().sprintf("%0.2f",model_cook.record(i).value(6).toFloat()));
            }
            ++count;
        }
        if(count < item_count)
        {
            //如果商品不足列表则填满
            for(;count < item_count;++count)
            {
                itemList.value(count)->_barcode = "";
                itemList.value(count)->_isSelect = false;
                itemList.value(count)->setName("");
            }
        }
        else if(count == item_count)
        {
            //如果等于列表数量则完全填满
        }
        else if(count > item_count)
        {
            //如果大于则代表有下一页，最后一个商品设置为下一页
            itemList.value(itemList.count()-1)->_barcode = "next";
            itemList.value(itemList.count()-1)->_isSelect = false;
            itemList.value(itemList.count()-1)->setName(">>>");
        }
    }
    else if(index > 0)   //不是第一页
    {
        //添加上一页
        itemList.value(0)->_barcode = "pre";
        itemList.value(0)->_isSelect = false;
        itemList.value(0)->setName("<<<");

        for(int i = 0; i < model_cook.rowCount(); i++)
        {
            if(count < item_count-1)      //填如之后所有商品到列表
            {
                itemList.value(count+1)->_barcode = model_cook.record(i).value(0).toString();
                itemList.value(count+1)->_isSelect = false;
                itemList.value(count+1)->setName(model_cook.record(i).value(1).toString() + "\n" + QString().sprintf("%0.2f",model_cook.record(i).value(6).toFloat()));
            }
            ++count;
        }

        if(count < item_count - 1)  //如果商品不足剩下的
        {
            //如果商品不足列表则填满
            for(;count < item_count-1;++count)
            {
                itemList.value(count+1)->_barcode = "";
                itemList.value(count+1)->_isSelect = false;
                itemList.value(count+1)->setName("");
            }
        }
        else if(count == item_count - 1)
        {
            //如果刚好后下面的则满上
        }
        else if(count > item_count - 1)
        {
            //如果大于则代表有下一页，最后一个商品设置为下一页
            itemList.value(itemList.count()-1)->_barcode = "next";
            itemList.value(itemList.count()-1)->_isSelect = false;
            itemList.value(itemList.count()-1)->setName(">>>");
        }
    }
    return true;
}

bool fast_order_2::showGoods_zilei(QString kindId, int index)
{
    if(kindId.isEmpty())
        return false;
    //保存当前页的信息
    goodIndex_xiaolei = index;
    goodKind_xiaolei = kindId;
    int preCount;//之前的商品数量
    if(0 == index)
    {
        preCount = 0;
    }
    else
    {
        preCount = item_count_zilei-1 + (index-1)*(item_count_zilei-2);
    }
    model_xiaolei.clear();
    model_xiaolei.setQuery(QString("SELECT ch_sub_typeno,vch_sub_typename FROM cey_bt_dish_subtype where ch_dish_typeno = '%1' limit %2,%3 ").arg(dalei).arg(preCount).arg(item_count_zilei+1));

    int count = 0;
    if(0 == index)       //如果是第一页
    {
        for(int i = 0; i < model_xiaolei.rowCount(); i++)
        {
            if(count < item_count_zilei)      //填如所有商品到列表
            {
                itemList_zilei.value(count)->_barcode = model_xiaolei.record(i).value(0).toString();
                itemList_zilei.value(count)->_isSelect = false;
                itemList_zilei.value(count)->setName(model_xiaolei.record(i).value(1).toString());
            }
            ++count;
        }
        if(count < item_count_zilei)
        {
            //如果商品不足列表则填满
            for(;count < item_count_zilei;++count)
            {
                itemList_zilei.value(count)->_barcode = "";
                itemList_zilei.value(count)->_isSelect = false;
                itemList_zilei.value(count)->setName("");
            }
        }
        else if(count == item_count_zilei)
        {
            //如果等于列表数量则完全填满
        }
        else if(count > item_count_zilei)
        {
            //如果大于则代表有下一页，最后一个商品设置为下一页
            itemList_zilei.value(itemList_zilei.count()-1)->_barcode = "next";
            itemList_zilei.value(itemList_zilei.count()-1)->_isSelect = false;
            itemList_zilei.value(itemList_zilei.count()-1)->setName(tr("下一页"));
        }
    }
    else if(index > 0)   //不是第一页
    {
        //添加上一页
        itemList_zilei.value(0)->_barcode = "pre";
        itemList_zilei.value(0)->_isSelect = false;
        itemList_zilei.value(0)->setName(tr("上一页"));

        for(int i = 0; i < model_xiaolei.rowCount(); i++)
        {
            if(count < item_count_zilei-1)      //填入之后所有商品到列表
            {
                itemList_zilei.value(count+1)->_barcode = model_xiaolei.record(i).value(0).toString();
                itemList_zilei.value(count+1)->_isSelect = false;
                itemList_zilei.value(count+1)->setName(model_xiaolei.record(i).value(1).toString());
            }
            ++count;
        }

        if(count < item_count_zilei - 1)  //如果商品不足剩下的
        {
            //如果商品不足列表则填满
            for(;count < item_count_zilei-1;++count)
            {
                itemList_zilei.value(count+1)->_barcode = "";
                itemList_zilei.value(count+1)->_isSelect = false;
                itemList_zilei.value(count+1)->setName("");
            }
        }
        else if(count == item_count_zilei - 1)
        {
            //如果刚好后下面的则满上
        }
        else if(count > item_count_zilei - 1)
        {
            //如果大于则代表有下一页，最后一个商品设置为下一页
            itemList_zilei.value(itemList_zilei.count()-1)->_barcode = "next";
            itemList_zilei.value(itemList_zilei.count()-1)->_isSelect = false;
            itemList_zilei.value(itemList_zilei.count()-1)->setName(tr("下一页"));
        }
    }
    return true;
}

void fast_order_2::addGood(const QString &barcode, bool manual_weight)
{
    bool barscaleflag=false;//是条码秤的格式
    bool isweight_flag=false;
    ui->widget->addGoods(barcode,
                         bianma_daicai_type,
                         changyongcook,
                         bianma_str,
                         this->sender()?this->sender()->metaObject()->className():"",
                         barscaleflag,
                         isweight_flag,
                         pay_barcode,
                         row_n);


    ui->tableView->tableview->selectRow(row_n);
    diancaitime=n_func::f_get_sysdatetime().toString("yyyy-MM-dd hh:mm:ss");
    caculate_dishtitle();
    ui->pushButton_takebill->setText(tr("挂单"));
    if(!barscaleflag/*不是条码商品*/
            &&isweight_flag/*是称重商品*/
            &&manual_weight/*手动可以称重*/){
        to_takeweight();
    }
    frontViewDialog_virtual::addGood(row_n);
}

void fast_order_2::dis_xiaolei_str(const QString &barcode)
{
    if(barcode.isEmpty())
        return;
    changyongcook = false;
    if(barcode != "pre" && barcode != "next")
    {
        xiaolei = barcode;
    }
    if(barcode == "pre")
    {
        if(goodIndex_xiaolei <= 0)
            return;

        --goodIndex_xiaolei;
        showGoods_zilei(goodKind_xiaolei,goodIndex_xiaolei);
        return;
    }
    else if(barcode == "next")
    {
        ++goodIndex_xiaolei;
        if(!showGoods_zilei(goodKind_xiaolei,goodIndex_xiaolei))
        {
            --goodIndex_xiaolei;
        }
        return;
    }
    if(!currentGoodItem_zilei.isNull() && currentGoodItem_zilei != sender())
        currentGoodItem_zilei->setSelect(false);

    if(sender() != NULL)
        currentGoodItem_zilei = static_cast<goodItem_zilei*>(sender());

    bianma_daicai_type = false;
    itemList.clear();
    qDeleteAll(itemList.begin(),itemList.end());
    //qDeleteAll(//ui->widget_3->children());
    layout_cook = new QGridLayout;
    for(int i = 0; i < item_count; ++i)
    {
        GoodItem *item = new GoodItem("","");
        itemList.append(item);
        connect(item,SIGNAL(touch(QString)),this,SLOT(addGood(QString)));
        layout_cook->addWidget(item,i/item_count_row,i%item_count_row);
    }
    layout_cook->setContentsMargins(0,0,0,0);
    layout_cook->setSpacing(2);
    //ui->widget_3->setLayout(layout_cook);
    model_cook.clear();
    model_cook.setQuery(QString("select ch_dishno,vch_dishname,num_price from cey_bt_dish where ch_off ='Y' and (ch_stopflag = null or ch_stopflag = 'N')  "));
    QString kind = model_xiaolei.record(0).value(0).toString();
    showGoods(kind);
}

void fast_order_2::toswitchback()
{
    _parent->goto_back();
}

void fast_order_2::to_usual_dish()//常用菜
{
    changyongcook=true;
    init_cook();
}

void fast_order_2::to_present()//赠送
{

    float label_jine = 0.00;
    QSqlQueryModel model;
    QString time = n_func::f_get_sysdatetime().toString("yyyy-MM-dd hh:mm:ss");

    int row = ui->tableView->tableview->currentIndex().row();//ui->tableView->tableview->currentIndex().row();

    if(row <= -1)
    {
        lds_messagebox::warning(this, tr("消息提示"),"请选择需要赠送的菜品记录!");
        return;
    }

    if(modelBase.item(row,9)->text() == "Y")
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("套菜明细,不能赠送!"));
        return;
    }


    if(n_func::gs_operid == "0000")
    {
        if(modelBase.item(row,8)->text() == "N")
        {
            if(0==lds_messagebox::warning(this, tr("消息提示"),tr("是否确定将此菜品赠送?"), tr("确认"), tr("取消")))
            {
                modelBase.item(row,8)->setText("Y");
                modelBase.item(row,20)->setText(time);
                for(int i = 0; i < modelBase.rowCount(); i++)
                {
                    if(modelBase.item(i,8)->text() == "N" && modelBase.item(i,9)->text() != "Y" )
                    {
                        label_jine += modelBase.item(i,6)->text().toFloat();
                    }
                }
                //ui->label_3->setText("￥:" + QString().sprintf("%0.2f",label_jine));
            }
        }
        else
        {
            if(0==lds_messagebox::warning(this, tr("消息提示"),tr("此菜品已经被赠送,是否取消赠送?"), tr("确认"), tr("取消")))
            {
                modelBase.item(row,8)->setText("N");
                modelBase.item(row,20)->setText("");
                //float label_jine = 0.00;
                for(int i = 0; i < modelBase.rowCount(); i++)
                {
                    if(modelBase.item(i,8)->text() == "N" && modelBase.item(i,9)->text() != "Y" )
                    {
                        label_jine += modelBase.item(i,6)->text().toFloat();
                    }
                }
                //ui->label_3->setText("￥:" + QString().sprintf("%0.2f",label_jine));
            }
        }
    }
    else//不是管理员赠送
    {
        model.clear();
        model.setQuery(QString("select num_present from cey_sys_operator where vch_operID = '%1' and ch_waiter = 'Y' ").arg(n_func::gs_operid));
        float present = 0.00;
        present = model.record(0).value(0).toFloat();
        float zengsongjine = 0.00;
        zengsongjine = modelBase.item(row,3)->text().toFloat() * modelBase.item(row,4)->text().toFloat();

        if(modelBase.item(row,8)->text() == "N")//1
        {
            if(zengsongjine > present)
            {
                if(0==lds_messagebox::warning(this, tr("消息提示"),tr("超出赠送金额,请另指定赠送人!"),tr("确认"),tr("取消")))
                {
                    power_oper power;
                    /*power.exec();*/backheader::static_dialogexec(power, "");
                    if(power.btn_t_f == true)
                    {
                        QSqlQueryModel model_l;
                        model_l.setQuery(QString("select num_present from cey_sys_operator where vch_operID = '%1' and ch_waiter = 'Y' ").arg(power.operid));
                        float present_0 = 0.00;
                        present_0 = model_l.record(0).value(0).toFloat();
                        if(power.operid == "0000")
                        {
                            if(0==lds_messagebox::warning(this, tr("消息提示"),tr("是否确定将此菜品赠送?"),tr("确认"),tr("取消")))
                            {
                                modelBase.item(row,8)->setText("Y");
                                modelBase.item(row,20)->setText(time);
                                //float label_jine = 0.00;
                                for(int i = 0; i < modelBase.rowCount(); i++)
                                {
                                    if(modelBase.item(i,8)->text() == "N" && modelBase.item(i,9)->text() != "Y" )
                                    {
                                        label_jine += modelBase.item(i,6)->text().toFloat();
                                    }
                                }
                                //ui->label_3->setText("￥:" + QString().sprintf("%0.2f",label_jine));
                            }
                        }
                        else//所选权限人不为0000
                        {
                            float aa = modelBase.item(row,3)->text().toFloat() * modelBase.item(row,4)->text().toFloat();
                            if(aa > present_0)
                            {
                                lds_messagebox::warning(this, tr("消息提示"),tr("超出赠送金额,请与系统管理员联系!"));
                                return;
                            }
                            else
                            {
                                if(0==lds_messagebox::warning(this, tr("消息提示"),tr("是否确定将此菜品赠送?"),tr("确认"),tr("取消")))
                                {
                                    modelBase.item(row,8)->setText("Y");
                                    modelBase.item(row,20)->setText(time);
                                    //float label_jine = 0.00;
                                    for(int i = 0; i < modelBase.rowCount(); i++)
                                    {
                                        if(modelBase.item(i,8)->text() == "N" && modelBase.item(i,9)->text() != "Y" )
                                        {
                                            label_jine += modelBase.item(i,6)->text().toFloat();
                                        }
                                    }
                                    //ui->label_3->setText("￥:" + QString().sprintf("%0.2f",label_jine));
                                }
                            }
                        }
                    }
                    else
                    {
                        return;
                    }
                }
            }//1
            else
            {//1
                if(0==lds_messagebox::warning(this, tr("消息提示"),tr("是否确定将此菜品赠送?"),tr("确认"),tr("取消")))
                {
                    modelBase.item(row,8)->setText("Y");
                    modelBase.item(row,20)->setText(time);
                    //float label_jine = 0.00;
                    for(int i = 0; i < modelBase.rowCount(); i++)
                    {
                        if(modelBase.item(i,8)->text() == "N" && modelBase.item(i,9)->text() != "Y" )
                        {
                            label_jine += modelBase.item(i,6)->text().toFloat();
                        }
                    }
                    //ui->label_3->setText("￥:" + QString().sprintf("%0.2f",label_jine));
                }
            }
        }
        else
        {
            if(0==lds_messagebox::warning(this, tr("消息提示"),tr("此菜品已经被赠送,是否取消赠送?"), tr("确认"),tr("取消")))
            {
                modelBase.item(row,8)->setText("N");
                modelBase.item(row,20)->setText("");
                //float label_jine = 0.00;
                for(int i = 0; i < modelBase.rowCount(); i++)
                {
                    if(modelBase.item(i,8)->text() == "N" && modelBase.item(i,9)->text() != "Y" )
                    {
                        label_jine += modelBase.item(i,6)->text().toFloat();
                    }
                }
            }
        }//1
    }

    this->to_dish_title( -1, label_jine);

}

void fast_order_2::to_del()//删菜
{
    int row = ui->tableView->tableview->currentIndex().row();//ui->tableView->tableview->currentIndex().row();
    if(row <= -1)
    {
        lds_messagebox::warning(this, tr("消息提示"),"请选择需要删除的菜品记录!");
        return;
    }

    if(modelBase.item(row,9)->text() == "Y")
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("套菜明细不能单独删除!"));
        return;
    }

    if(modelBase.item(row,9)->text() == "P")
    {
        QString bianma = modelBase.item(row,1)->text();
        modelBase.removeRow(row);
        QSqlQueryModel tao;
        tao.setQuery(QString("select * from cey_bt_dish_suit where ch_suitno ='%1'").arg(bianma));
        for(int i = 0; i < tao.rowCount(); i++)
        {
            modelBase.removeRow(row);
        }
    }
    else
    {
        modelBase.removeRow(row);
    }

    caculate_dishtitle();
    ui->tableView->tableview->selectRow(qMin(modelBase.rowCount()-1,row));
    if(modelBase.rowCount() == 0)
    {
        ui->pushButton_takebill->setText(tr("取单"));
    }
    frontViewDialog_virtual::toDel();
}

bool fast_order_2::to_allbillcancel()//整单取消
{
    if(modelBase.rowCount() == 0)
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("您还未进行点单,请确认!"));
        return false;
    }

    if(0==lds_messagebox::warning(this, tr("消息提示"),tr("是否确定整单取消?"),tr("确认"),tr("取消")))
    {
        modelBase.removeRows(0, modelBase.rowCount());
        this->dish_detail_clear();
        this->to_dish_title( 0, 0);
        ui->pushButton_takebill->setText(tr("取单"));
        return true;
    }
    return false;
}

void fast_order_2::to_discount()//折扣
{
    QString time = n_func::f_get_sysdatetime().toString("yyyy-MM-dd hh:mm:ss");
    QSqlQuery query;

    int row = ui->tableView->tableview->currentIndex().row();//ui->tableView->tableview->currentIndex().row();
    if(row <= -1)
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("请选择需要折扣的菜品!"));
        return;
    }

    if(modelBase.item(row,15)->text() == "Y")
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("时价菜不允许折扣!"));
        return;
    }

    if(modelBase.item(row,9)->text() == "Y")
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("套菜明细不允许折扣!"));
        return;
    }
    if(modelBase.item(row,8)->text() == "Y")
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("当前菜品已作赠送，不能再进行折扣处理!"));
        return;
    }
    query.exec(QString("select ch_discount from cey_bt_dish where ch_dishno='%1' ").arg(modelBase.item(row,1)->text()));
    query.next();
    if(query.record().value(0).toString()!="Y"){
        lds_messagebox::warning(this, tr("消息提示"),tr("当前菜品属性中未勾选打折，不能再进行折扣处理!"));
        return;
    }
    if(modelBase.item(row,10)->text() == "Y")
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("当前菜品已是特价,不允许再进行折扣处理!"));
        return;
    }

    if(n_func::gs_operid != "0000" && modelBase.item(row,18)->text() != "Y")
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("当前菜品属性中未勾选打折\n不能进行折扣处理!"));
        return;
    }

    if(modelBase.item(row,5)->text() !=  "100"&& n_func::gs_operid == modelBase.item(row,14)->text())
    {
        if(0==lds_messagebox::warning(this, tr("消息提示"),tr("指定菜品已有折扣,是否取消原有折扣?"),tr("确认"),tr("取消")))
        {
            modelBase.item(row,5)->setText(tr("100"));
            modelBase.item(row,14)->setText("");
            float jine = 0.00;
            jine = modelBase.item(row,3)->text().toFloat() * modelBase.item(row,4)->text().toFloat() * modelBase.item(row,5)->text().toFloat() * 0.01 + modelBase.item(row,3)->text().toFloat() * modelBase.item(row,21)->text().toFloat();
            modelBase.item(row,6)->setText(QString().sprintf("%0.2f",jine));
            modelBase.item(row,19)->setText("");
        }
        else
        {
            return;
        }
    }
    else  if(modelBase.item(row,5)->text() != "100" && n_func::gs_operid != modelBase.item(row,14)->text())
    {
        if(0==lds_messagebox::warning(this, tr("消息提示"),tr("此菜品已经进行折扣处理\n取消折扣必须是同一人!"),tr("确认"),tr("取消")))
        {
            power_oper power;
            /*power.exec();*/backheader::static_dialogexec(power, "");
            if(power.btn_t_f == true)
            {
                if(power.operid == modelBase.item(row,14)->text())
                {
                    modelBase.item(row,5)->setText(tr("100"));
                    modelBase.item(row,14)->setText("");
                    float jine = 0.00;
                    jine = modelBase.item(row,3)->text().toFloat() *
                            modelBase.item(row,4)->text().toFloat() *
                            modelBase.item(row,5)->text().toFloat() * 0.01 +
                            modelBase.item(row,3)->text().toFloat() *
                            modelBase.item(row,21)->text().toFloat();
                    modelBase.item(row,6)->setText(QString().sprintf("%0.2f",jine));
                    modelBase.item(row,19)->setText("");
                }
                else
                {
                    QString str = tr("取消折扣操作人:") + power.operid + tr("和折扣处理人:") + modelBase.item(row,14)->text() + "\n"+tr("不一样,操作终止!");
                    lds_messagebox::warning(this, tr("消息提示"),str);
                    return;
                }
            }
            else
            {
                return;
            }
        }
        else
        {
            return;
        }
    }
    else
    {
        discount_cook  discount_c;
        backheader::static_dialogexec(discount_c, tr("菜品定价或折扣"));
        if(discount_c.btn_t_f == true)
        {
            if(n_func::gs_operid != "0000")
            {
                QSqlQueryModel model;
                model.setQuery(QString("Select int_discount from cey_sys_operator where vch_operID ='%1' and ch_flag = 'Y' ").arg(n_func::gs_operid));
                float quanxian_zhi = 0.00;
                quanxian_zhi = model.record(0).value(0).toFloat();
                float discount_num = 0.00;
                discount_num = discount_c.discount;
                if(discount_num < quanxian_zhi)
                {
                    if(0==lds_messagebox::warning(this, tr("消息提示"),tr("指定操作员没有该折扣操作权限!"),tr("确认"),tr("取消")))
                    {
                        power_oper power;
                        /*power.exec();*/backheader::static_dialogexec(power, "");
                        if(power.btn_t_f == true)
                        {
                            QSqlQueryModel model;
                            model.setQuery(QString("Select int_discount from cey_sys_operator where ch_flag = 'Y' and vch_operID ='%1' ").arg(power.operid));
                            if(discount_num < model.record(0).value(0).toFloat())
                            {
                                lds_messagebox::warning(this, tr("消息提示"),tr("指定操作员没有该折扣操作权限!"));

                                return;
                            }

                            modelBase.item(row,5)->setText(QString().sprintf("%0.0f",discount_num));
                            modelBase.item(row,14)->setText(power.operid);
                            float jine = 0.00;
                            jine = modelBase.item(row,3)->text().toFloat() * modelBase.item(row,4)->text().toFloat() * modelBase.item(row,5)->text().toFloat() * 0.01 + modelBase.item(row,3)->text().toFloat() * modelBase.item(row,21)->text().toFloat();;
                            modelBase.item(row,6)->setText(QString().sprintf("%0.2f",jine));
                            modelBase.item(row,19)->setText(time);
                        }
                        else
                        {
                            return;
                        }
                    }
                    else
                    {
                        return;
                    }
                }
                else
                {
                    modelBase.item(row,5)->setText(QString().sprintf("%0.0f",discount_num));
                    modelBase.item(row,14)->setText(n_func::gs_operid);
                    float jine = 0.00;
                    jine = modelBase.item(row,3)->text().toFloat() * modelBase.item(row,4)->text().toFloat() * modelBase.item(row,5)->text().toFloat() * 0.01 + modelBase.item(row,3)->text().toFloat() * modelBase.item(row,21)->text().toFloat();;
                    modelBase.item(row,6)->setText(QString().sprintf("%0.2f",jine));
                    modelBase.item(row,19)->setText(time);
                }

            }
            else
            {
                modelBase.item(row,5)->setText(QString().sprintf("%0.0f",discount_c.discount));
                modelBase.item(row,14)->setText(n_func::gs_operid);
                float jine = 0.00;
                jine = modelBase.item(row,3)->text().toFloat() * modelBase.item(row,4)->text().toFloat() * modelBase.item(row,5)->text().toFloat() * 0.01 + modelBase.item(row,3)->text().toFloat() * modelBase.item(row,21)->text().toFloat();;
                modelBase.item(row,6)->setText(QString().sprintf("%0.2f",jine));
                modelBase.item(row,19)->setText(time);
            }
        }
        else
        {
            return;
        }
    }

    float label_shuliang = 0.00;
    float label_jine = 0.00;
    for(int i = 0; i < modelBase.rowCount(); i++)
    {
        modelBase.item(i,0)->setText(QString::number(i + 1));
        label_shuliang += modelBase.item(i,3)->text().toFloat();
        if(modelBase.item(i,8)->text() == "N" && modelBase.item(i,9)->text() != "Y" )
        {
            label_jine += modelBase.item(i,6)->text().toFloat();
        }
    }
    this->to_dish_title( label_shuliang, label_jine);

}

void fast_order_2::to_change()//换菜
{
    int row = ui->tableView->tableview->currentIndex().row();//ui->tableView->tableview->currentIndex().row();

    if(row <= -1)
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("请选择需要换菜的菜品!"));

        return;
    }

    if(modelBase.item(row,10)->text() == "Y")
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("选中点菜记录为特价,不能换菜!"));

        return;
    }

    if(modelBase.item(row,9)->text() == "P")
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("选中点菜记录为主套菜,不能换菜!"));

        return;
    }

    if(modelBase.item(row,8)->text() == "Y")
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("选中点菜记录已被赠送,不能换菜!"));

        return;
    }

    dish_change change(row,"1");
    /*change.exec();*/backheader::static_dialogexec(change, tr("点菜替换操作"));
    if(change.btn_t_f == true)
    {
        if(modelBase.item(row,9)->text() != "Y")
        {
            modelBase.item(row,1)->setText(change.dish_no);
            modelBase.item(row,2)->setText(change.dish_name);
            modelBase.item(row,4)->setText(QString().sprintf("%0.2f",change.dish_num));
            modelBase.item(row,12)->setText(change.dish_unit);
            float zongjia = modelBase.item(row,3)->text().toFloat() * modelBase.item(row,4)->text().toFloat() * modelBase.item(row,5)->text().toFloat() * 0.01 + modelBase.item(row,3)->text().toFloat() * modelBase.item(row,21)->text().toFloat();
            modelBase.item(row,6)->setText(QString().sprintf("%0.2f",zongjia));
        }
        else
        {
            if(modelBase.item(row,4)->text().toFloat() != change.dish_num && change.quanbu == true)
            {
                if(0==lds_messagebox::warning(this, tr("消息提示"),tr("替换菜品和被替换菜品价格不一样\n是否在主套菜上补差价?"),tr("确认"),tr("取消")))
                {
                    float chajia = 0.00;
                    chajia = change.dish_num - modelBase.item(row,4)->text().toFloat();
                    modelBase.item(row,1)->setText(change.dish_no);
                    modelBase.item(row,2)->setText(change.dish_name);
                    modelBase.item(row,4)->setText(QString().sprintf("%0.2f",change.dish_num));
                    modelBase.item(row,12)->setText(change.dish_unit);
                    while(row--)
                    {
                        if(modelBase.item(row,9)->text() == "P")
                        {
                            float zhujia = 0.00;
                            zhujia = modelBase.item(row,6)->text().toFloat() + chajia + modelBase.item(row,3)->text().toFloat() * modelBase.item(row,21)->text().toFloat();

                            modelBase.item(row,6)->setText(QString().sprintf("%0.2f",zhujia));
                            break;
                        }
                    }
                }
                else
                {
                    return;
                }
            }
            else
            {
                modelBase.item(row,1)->setText(change.dish_no);
                modelBase.item(row,2)->setText(change.dish_name);
                modelBase.item(row,4)->setText(QString().sprintf("%0.2f",change.dish_num));
                modelBase.item(row,12)->setText(change.dish_unit);
            }
        }

        float label_shuliang = 0.00;
        float label_jine = 0.00;
        for(int i = 0; i < modelBase.rowCount(); i++)
        {
            modelBase.item(i,0)->setText(QString::number(i + 1));
            label_shuliang += modelBase.item(i,3)->text().toFloat();
            if(modelBase.item(i,8)->text() == "N" && modelBase.item(i,9)->text() != "Y" )
            {
                label_jine += modelBase.item(i,6)->text().toFloat();
            }
        }

        //ui->label_3->setText("￥:" + QString().sprintf("%0.2f",label_jine));
        //ui->label_5->setText(QString().sprintf("%0.2f",label_shuliang));
        modelBase.item(row,1)->setText(change.dish_no);
        modelBase.item(row,2)->setText(change.dish_name);
        modelBase.item(row,4)->setText(QString().sprintf("%0.2f",change.dish_num));
        modelBase.item(row,12)->setText(change.dish_unit);

        this->to_dish_title( label_shuliang, label_jine);

    }
}

void fast_order_2::to_num(const QString &_dishnum)//数量
{
    int row = ui->tableView->tableview->currentIndex().row();//ui->tableView->tableview->currentIndex().row();

    if(modelBase.rowCount() == 0)
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("您还未进行点菜,请确认!"));

        return;
    }

    if(row <= -1)
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("请选择需要修改数量的菜品!"));

        return;
    }

    if(modelBase.item(row,8)->text() == "Y")
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("已赠送的点菜记录,数量不可改!"));

        return;
    }

    if(modelBase.item(row,9)->text() == "Y")
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("套菜明细不能单独修改数量!"));

        return;
    }

    QSqlQueryModel model;
    model.setQuery("Select NOW()");
    QString guqingdate = model.record(0).value(0).toDateTime().toString("yyyy-MM-dd");

    QSqlQueryModel guqing;
    guqing.setQuery(QString("Select ifnull(num_num - num_sale , 0) from cey_bt_dish_warn where ch_dishno ='%1' and ch_state ='Y' and DATE_FORMAT(dt_date, '%Y-%m-%d') ='%2' ").arg(modelBase.item(row,1)->text()).arg(guqingdate));
    float guqingnum = 0.00;
    guqingnum = guqing.record(0).value(0).toFloat();
    modfiy_number modfiy(modelBase.item(row,3)->text());
    if(_dishnum.isEmpty()){
        backheader::static_dialogexec(modfiy, tr("点菜数量修改"));
    } else {
        bool ok=false;
        modfiy.btn_t_f=true;
        modfiy.modfiy_num=_dishnum.toFloat(&ok);
        if(!ok) return;
    }
    if(modfiy.btn_t_f == true)
    {
        if(modelBase.item(row,9)->text() == "P")//主套菜
        {
            if(guqing.rowCount() > 0)
            {
                float yidian = 0.00;
                for(int i = 0; i < modelBase.rowCount(); i++)
                {
                    if(i == row)
                    {
                        yidian += modfiy.modfiy_num;
                    }

                    if(modelBase.item(i,1)->text() == modelBase.item(row,1)->text() && i != row && modelBase.item(i,9)->text() != tr("Y"))
                    {
                        yidian += modelBase.item(i,3)->text().toFloat();
                    }
                }

                if(yidian > guqing.record(0).value(0).toFloat())
                {
                    lds_messagebox::warning(this, tr("消息提示"),tr("菜品已经估清,不够扣除此次数量!"));

                    return;
                }
                else
                {
                    float sheng = 0.00;
                    sheng = guqing.record(0).value(0).toFloat()- yidian ;
                    QString str = tr("菜品:") + modelBase.item(row,2)->text() + tr("已经估清,扣除此次数量,\n剩余数量为:") + QString().sprintf("%0.1f",sheng) + tr("!");
                    lds_messagebox::warning(this, tr("消息提示"),str);

                }
            }

            modelBase.item(row,3)->setText(QString().sprintf("%0.3f",modfiy.modfiy_num));
            float zongjia = modelBase.item(row,3)->text().toFloat() * modelBase.item(row,4)->text().toFloat() * modelBase.item(row,5)->text().toFloat() * 0.01 + modelBase.item(row,3)->text().toFloat() * modelBase.item(row,21)->text().toFloat();
            modelBase.item(row,6)->setText(QString().sprintf("%0.2f",zongjia));

            for(int i = row +1 ; i < modelBase.rowCount(); i++)
            {
                if(modelBase.item(i,9)->text() == "Y")
                {
                    float num = 0.00;
                    num = modelBase.item(i,3)->text().toFloat() * modfiy.modfiy_num;
                    modelBase.item(i,3)->setText(QString().sprintf("%0.3f",num));
                }
                else
                {
                    break;
                }
            }
        }
        else
        {
            if(guqing.rowCount() > 0)
            {
                float yidian = 0.00;
                for(int i = 0; i < modelBase.rowCount(); i++)
                {
                    if(i == row)
                    {
                        yidian += modfiy.modfiy_num;
                    }

                    if(modelBase.item(i,1)->text() == modelBase.item(row,1)->text() && i != row && modelBase.item(i,9)->text() != tr("Y"))
                    {
                        yidian += modelBase.item(i,3)->text().toFloat();
                    }
                }

                if(yidian > guqing.record(0).value(0).toFloat())
                {
                    lds_messagebox::warning(this, tr("消息提示"),tr("菜品已经估清,不够扣除此次数量!"));

                    return;
                }
                else
                {
                    float sheng = 0.00;
                    sheng = guqing.record(0).value(0).toFloat()- yidian ;
                    QString str = tr("菜品:") + modelBase.item(row,2)->text() + tr("已经估清,扣除此次数量,\n剩余数量为:") + QString().sprintf("%0.1f",sheng) + tr("!");
                    lds_messagebox::warning(this, tr("消息提示"),str);

                }
            }
            modelBase.item(row,3)->setText(QString().sprintf("%0.3f",modfiy.modfiy_num));
            float zongjia = modelBase.item(row,3)->text().toFloat() * modelBase.item(row,4)->text().toFloat() * modelBase.item(row,5)->text().toFloat() * 0.01 + modelBase.item(row,3)->text().toFloat() * modelBase.item(row,21)->text().toFloat();
            modelBase.item(row,6)->setText(QString().sprintf("%0.2f",zongjia));
        }

        float label_shuliang = 0.00;
        float label_jine = 0.00;
        for(int i = 0; i < modelBase.rowCount(); i++)
        {
            modelBase.item(i,0)->setText(QString::number(i + 1));
            label_shuliang += modelBase.item(i,3)->text().toFloat();
            if(modelBase.item(i,8)->text() == "N" && modelBase.item(i,9)->text() != "Y" )
            {
                label_jine += modelBase.item(i,6)->text().toFloat();
            }
        }
        //ui->label_3->setText("￥:" + QString().sprintf("%0.2f",label_jine));
        //ui->label_5->setText(QString().sprintf("%0.2f",label_shuliang));
        this->to_dish_title( label_shuliang, label_jine);

    }
}

void fast_order_2::to_specialcook()//特殊做法
{
    ;
    int row = ui->tableView->tableview->currentIndex().row();//ui->tableView->tableview->currentIndex().row();

    if(modelBase.rowCount() == 0)
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("您还未进行点菜,请确认!"));

        return;
    }

    if(row <= -1)
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("请选择需要特殊做法的菜品!"));

        return;
    }

    if(modelBase.item(row,9)->text() == "P")
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("主套菜不能设置厨打说明,请设置套菜明细!"));

        return;
    }

    w_scr_dish_special_cook specialcook(modelBase.item(row,1)->text(),modelBase.item(row,7)->text(),modelBase.item(row,21)->text());
    /*specialcook.exec();*/backheader::static_dialogexec(specialcook, tr("特殊做法"));
    if(specialcook.btn_t_f == true)
    {
        if(modelBase.item(row,9)->text() != "P" && modelBase.item(row,9)->text() != "Y")//不是套菜
        {
            modelBase.item(row,7)->setText(specialcook.zuofaneirong);
            modelBase.item(row,21)->setText(QString().sprintf("%0.2f",specialcook.add_price));
            float aa;
            aa = 0.00;
            aa = modelBase.item(row,3)->text().toFloat() * modelBase.item(row,4)->text().toFloat() * modelBase.item(row,5)->text().toFloat() * 0.01 + modelBase.item(row,3)->text().toFloat() * specialcook.add_price;
            modelBase.item(row,6)->setText(QString().sprintf("%0.2f",aa));
        }

        if(modelBase.item(row,9)->text() != "P" && modelBase.item(row,9)->text() == "Y")//是子套菜
        {
            modelBase.item(row,7)->setText(specialcook.zuofaneirong);
            modelBase.item(row,21)->setText(QString().sprintf("%0.2f",specialcook.add_price));
            float bb = 0.00;
            //bb = modelBase.item(row,21)->text().toFloat() * modelBase.item(row,3)->text().toFloat();
            for(int i = row;i < modelBase.rowCount(); i--)
            {
                if(modelBase.item(i,9)->text() == "P")
                {
                    int thisrow = i;
                    int taorow = i + 1;
                    float cc = 0.00;
                    modelBase.item(i,21)->setText(QString().sprintf("%0.2f",specialcook.add_price));
                    cc = modelBase.item(i,3)->text().toFloat() * modelBase.item(i,4)->text().toFloat() * modelBase.item(i,5)->text().toFloat() * 0.01;
                    // modelBase.item(i,6)->setText(QString().sprintf("%0.2f",bb));
                    for(int k = taorow; k < modelBase.rowCount(); k++)
                    {
                        if(modelBase.item(k,9)->text() == "Y")
                        {
                            bb += modelBase.item(k,21)->text().toFloat() * modelBase.item(k,3)->text().toFloat();
                            modelBase.item(i,21)->setText(QString().sprintf("%0.2f",bb));
                            if(k == modelBase.rowCount() - 1)
                            {
                                bb = bb + cc;
                                modelBase.item(thisrow,6)->setText(QString().sprintf("%0.2f",bb));
                                break;
                            }
                        }
                        else
                        {
                            bb = bb + cc;
                            modelBase.item(thisrow,6)->setText(QString().sprintf("%0.2f",bb));
                            break;
                        }
                    }
                    break;
                }
            }
        }
    }

    float label_shuliang = 0.00;
    float label_jine = 0.00;
    for(int i = 0; i < modelBase.rowCount(); i++)
    {
        modelBase.item(i,0)->setText(QString::number(i + 1));
        label_shuliang += modelBase.item(i,3)->text().toFloat();
        if(modelBase.item(i,8)->text() == "N" && modelBase.item(i,9)->text() != "Y" )
        {
            label_jine += modelBase.item(i,6)->text().toFloat();
        }
    }
    this->to_dish_title( label_shuliang, label_jine);

}

void fast_order_2::to_waimai()//外卖
{
    ;
    int row = ui->tableView->tableview->currentIndex().row();//ui->tableView->tableview->currentIndex().row();

    if(modelBase.rowCount() == 0)
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("您还未进行点菜,请确认!"));

        return;
    }

    takeout_cook takeout(this);
    /*takeout->exec();*/backheader::static_dialogexec(takeout, tr("外卖"));
    if(takeout.btn_t_f)
    {
        modelBase.removeRows(0, modelBase.rowCount());
        this->dish_detail_clear();
        this->to_dish_title( 0, 0);
        init_pay_model();
        ui->pushButton_takebill->setText(tr("取单"));
    }
}

void fast_order_2::to_waimaipay()//外卖结算
{
    take_cashier cashier(this);
    backheader::static_dialogexec(cashier, tr("外卖收银结算"));
    if(modelBase.rowCount() == 0)
    {
        ui->pushButton_takebill->setText(tr("取单"));
    }
}

void fast_order_2::to_takeweight()//取重
{
    int row = ui->tableView->tableview->currentIndex().row();//ui->tableView->tableview->currentIndex().row();

    if(modelBase.rowCount() == 0)
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("您还未进行点菜,请确认!"));
        return;
    }

    if(row <= -1)
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("请选择需要修改数量的菜品!"));
        return;
    }

    if(modelBase.item(row,8)->text() == "Y")
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("已赠送的点菜记录,数量不可改!"));

        return;
    }

    if(modelBase.item(row,9)->text() == "Y")
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("套菜明细不能单独修改数量!"));

        return;
    }

    QSqlQueryModel model;
    model.setQuery("Select NOW()");
    QString guqingdate = model.record(0).value(0).toDateTime().toString("yyyy-MM-dd");

    QSqlQueryModel guqing;
    guqing.setQuery(QString("Select ifnull(num_num - num_sale , 0) from cey_bt_dish_warn where ch_dishno ='%1' and ch_state ='Y' and DATE_FORMAT(dt_date, '%Y-%m-%d') ='%2' ").arg(modelBase.item(row,1)->text()).arg(guqingdate));
    float guqingnum = 0.00;
    guqingnum = guqing.record(0).value(0).toFloat();

    take_weight weight(this);
    weight.ui->lineEdit_dishname->setText(this->get_currow_dishname());
    weight.ui->lineEdit_dishprice->setText(this->get_currow_dishprice());
    weight.ui->lineEdit_getweight->setText(this->get_currow_dishnum());

    bool canberemove=false;
    if(this->get_currow_dishnum().toDouble() == 0){
        canberemove=true;
    }
    if(QDialog::Accepted!=backheader::static_dialogexec(weight, tr("取重"))){
        if(canberemove/*由于开始是零,所有这儿若是点的取消可以删除*/){
            modelBase.removeRow(row);

            float label_shuliang = 0.00;
            float label_jine = 0.00;
            for(int row = 0; row < modelBase.rowCount(); row++)
            {
                modelBase.item(row,0)->setText(QString::number(row + 1));
                label_shuliang += modelBase.item(row,3)->text().toFloat();
                if(modelBase.item(row,8)->text() == "N" && modelBase.item(row,9)->text() != "Y" )
                {
                    label_jine += modelBase.item(row,6)->text().toFloat();
                }
            }
            this->to_dish_title( label_shuliang, label_jine);
            return;
        }
    }
    else { //若是accept的话即可
        {
            if(modelBase.item(row,9)->text() == "P")//主套菜
            {
                if(guqing.rowCount() > 0)
                {
                    float yidian = 0.00;
                    for(int i = 0; i < modelBase.rowCount(); i++)
                    {
                        if(i == row)
                        {
                            yidian += weight._weight.toFloat();
                        }

                        if(modelBase.item(i,1)->text() == modelBase.item(row,1)->text() && i != row && modelBase.item(i,9)->text() != tr("Y"))
                        {
                            yidian += modelBase.item(i,3)->text().toFloat();
                        }
                    }

                    if(yidian > guqing.record(0).value(0).toFloat())
                    {
                        lds_messagebox::warning(this, tr("消息提示"),tr("菜品已经估清,不够扣除此次数量!"));

                        return;
                    }
                    else
                    {
                        float sheng = 0.00;
                        sheng = guqing.record(0).value(0).toFloat()- yidian ;
                        QString str = tr("菜品:") + modelBase.item(row,2)->text() + tr("已经估清,扣除此次数量,\n剩余数量为:") + QString().sprintf("%0.1f",sheng) + tr("!");
                        lds_messagebox::warning(this, tr("消息提示"),str);

                    }
                }

                modelBase.item(row,3)->setText(QString().sprintf("%0.3f",weight._weight.toFloat()));
                float zongjia = modelBase.item(row,3)->text().toFloat() * modelBase.item(row,4)->text().toFloat() * modelBase.item(row,5)->text().toFloat() * 0.01 + modelBase.item(row,3)->text().toFloat() * modelBase.item(row,21)->text().toFloat();
                modelBase.item(row,6)->setText(QString().sprintf("%0.2f",zongjia));

                for(int i = row +1 ; i < modelBase.rowCount(); i++)
                {
                    if(modelBase.item(i,9)->text() == "Y")
                    {
                        float num = 0.00;
                        num = modelBase.item(i,3)->text().toFloat() * weight._weight.toFloat();
                        modelBase.item(i,3)->setText(QString().sprintf("%0.3f",num));
                    }
                    else
                    {
                        break;
                    }
                }
            }
            else
            {
                if(guqing.rowCount() > 0)
                {
                    float yidian = 0.00;
                    for(int i = 0; i < modelBase.rowCount(); i++)
                    {
                        if(i == row)
                        {
                            yidian += weight._weight.toFloat();
                        }

                        if(modelBase.item(i,1)->text() == modelBase.item(row,1)->text() && i != row && modelBase.item(i,9)->text() != tr("Y"))
                        {
                            yidian += modelBase.item(i,3)->text().toFloat();
                        }
                    }

                    if(yidian > guqing.record(0).value(0).toFloat())
                    {
                        lds_messagebox::warning(this, tr("消息提示"),tr("菜品已经估清,不够扣除此次数量!"));

                        return;
                    }
                    else
                    {
                        float sheng = 0.00;
                        sheng = guqing.record(0).value(0).toFloat()- yidian ;
                        QString str = tr("菜品:") + modelBase.item(row,2)->text() + tr("已经估清,扣除此次数量,\n剩余数量为:") + QString().sprintf("%0.1f",sheng) + tr("!");
                        lds_messagebox::warning(this, tr("消息提示"),str);

                    }
                }
                modelBase.item(row,3)->setText(QString().sprintf("%0.3f",weight._weight.toFloat()));
                float zongjia = modelBase.item(row,3)->text().toFloat() * modelBase.item(row,4)->text().toFloat() * modelBase.item(row,5)->text().toFloat() * 0.01 + modelBase.item(row,3)->text().toFloat() * modelBase.item(row,21)->text().toFloat();
                modelBase.item(row,6)->setText(QString().sprintf("%0.2f",zongjia));
            }
            caculate_dishtitle();
        }
    }
}

void fast_order_2::to_sys_xiugaimima()//功能选择
{
    function_selection * function = new function_selection();
}

void fast_order_2::to_pay()//付款
{
    if(modelBase.rowCount() == 0)
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("没有点菜记录,不能付款!"));

        return;
    }


    if(n_func::gs_operid != "0000")
    {
        QSqlQueryModel model;
        model.setQuery(QString("select count(*)  from cey_sys_oper_purview  where ch_purviewno = '%1' and vch_operID ='%2' ").arg("0214").arg(n_func::gs_operid));
        if(model.record(0).value(0).toInt() == 0)
        {
            lds_messagebox::warning(this, tr("消息提示"),tr("该操作员无此操作权限!"));

            return;
        }
    }

    fast_pay pay(this);

    backheader::static_dialogexec(pay, tr("快餐付款"));
    if(pay.btn_t_f == true)
    {
        modelBase.removeRows(0, modelBase.rowCount());
    }

    init_pay_model();
    ui->pushButton_takebill->setText(tr("取单"));
}

//void fast_order_2::to_sys_chufangdaying()//大类上一页
//{
//    //ui->scrollArea->horizontalScrollBar()->setValue(//ui->scrollArea->horizontalScrollBar()->value() - //ui->scrollArea->width());
//}

//void fast_order_2::on_pushButton_15_clicked()//大类下一页
//{
//    //ui->scrollArea->horizontalScrollBar()->setValue(//ui->scrollArea->horizontalScrollBar()->value() + //ui->scrollArea->width());
//}

//void fast_order_2::on_pushButton_16_clicked()//小类上一页
//{
//    //ui->scrollArea_2->horizontalScrollBar()->setValue(//ui->scrollArea_2->horizontalScrollBar()->value() - //ui->scrollArea_2->width());
//}

//void fast_order_2::on_pushButton_17_clicked()//小类下一页
//{
//    //ui->scrollArea_2->horizontalScrollBar()->setValue(//ui->scrollArea_2->horizontalScrollBar()->value() + //ui->scrollArea_2->width());
//}

//void fast_order_2::on_pushButton_18_clicked()//菜品上一页
//{
//    //ui->scrollArea_3->verticalScrollBar()->setValue(//ui->scrollArea_3->verticalScrollBar()->value() - //ui->scrollArea_3->width());
//}

//void fast_order_2::on_pushButton_19_clicked()//菜品下一页
//{
//    //ui->scrollArea_3->verticalScrollBar()->setValue(//ui->scrollArea_3->verticalScrollBar()->value() + //ui->scrollArea_3->width());
//}

void fast_order_2::to_tuichu()//后台管理
{
    ui->pushButton_other->menu()->hide();
    if(modelBase.rowCount() > 0)
    {
        if(0==lds_messagebox::warning(this, tr("消息提示"),tr("存在消费记录! 是否整单取消然后退出?"),tr("确认"),tr("取消"))){
            if(to_allbillcancel()){

            } else {
                return;
            }
        } else {
            return;
        }
    }
    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    if(conf.value("system_setting/chongxidenglu").toString() == "1"){
        //返回登录界面
        _parent->hideall_and_back_login("", "");
    } else {
        this->reject();
    }
}

void fast_order_2::to_exitbill()//退单
{
    for(int r = 0, rcount = modelBase.rowCount();r < rcount; r++){
        modelBase.item(r, 3)->setText(QString().sprintf("-%.02f", modelBase.item(r, 3)->text().toFloat()));
    }
    fast_pay pay0(0, false);
    pay0.to_pay(false, false);
    pay0.fast_pay_print(pay0.get_curdandao(), tr("退菜单"),
                        fast_pay::PRINT_BILL
                        | fast_pay::PRINT_KITCHEN
                        | fast_pay::PRINT_ONLY_COSUME);
    modelBase.removeRows(0, modelBase.rowCount());
    lds_messagebox::warning(this,tr("退菜单"),tr("退菜成功"));
    return;
    QSqlQueryModel model;
    QSqlQuery query;
    QSqlDatabase::database().transaction();
    QString zhuohao = "Q000";
    model.setQuery(QString("select count(*)  from cey_sys_oper_purview  where ch_purviewno = '%1' and vch_operID ='%2' ").arg("0228").arg(n_func::gs_operid));
    if(model.record(0).value(0).toInt() == 0 && n_func::gs_operid != "0000")
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("该操作员无此操作权限!"));
        return;
    }

    if(modelBase.rowCount() == 0)
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("对不起,没有点菜记录,不能退单!"));
        return;
    }

    QSqlQueryModel m_billno;
    m_billno.setQuery("Select NOW()");
    QDateTime ldt_datetime;
    ldt_datetime.setTime(m_billno.record(0).value(0).toTime());
    ldt_datetime.setDate(m_billno.record(0).value(0).toDate());
    QString time = m_billno.record(0).value(0).toDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString currentDate = "Q" + ldt_datetime.toString("yyMMdd");
    QString timer = m_billno.record(0).value(0).toDateTime().toString("yyyy-MM-dd");

    m_billno.clear();
    m_billno.setQuery(QString("select Max(ch_billno) from cey_u_master where Left(ch_billno,7) = '%1' ").arg(currentDate));
    QString danhao = m_billno.record(0).value(0).toString().trimmed();
    if(danhao.trimmed().isEmpty())
    {
        danhao = currentDate + QString().sprintf("%05d",1);
        bendandanhao = currentDate + QString().sprintf("%05d",1);
    }
    else
    {
        QString str = danhao.right(danhao.length() - 7);
        int aa = str.toInt() + 1;
        danhao = currentDate + QString().sprintf("%05d",aa);
        bendandanhao = currentDate + QString().sprintf("%05d",aa);
    }

    liushuihao_p=system_setting::get_max_serialno_add1(bendandanhao);
    m_billno.clear();
    QString liushi = "P" + ldt_datetime.toString("yyMMdd");
    m_billno.setQuery(QString("select Max(ch_payno) from cey_u_checkout_master where Left(ch_payno,7) = '%1' ").arg(liushi));
    QString liushuihao = m_billno.record(0).value(0).toString().trimmed();
    if(liushuihao.trimmed().isEmpty())
    {
        liushuihao = liushi + QString().sprintf("%05d",1);
    }
    else
    {
        QString str = liushuihao.right(liushuihao.length() - 7);
        int aa = str.toInt() + 1;
        liushuihao = liushi + QString().sprintf("%05d",aa);
    }

    if(0==lds_messagebox::warning(this, tr("消息提示"),tr("确认退单?"),tr("确认"),tr("取消")))
    {
        for(int i = 0; i < modelBase.rowCount(); i++)
        {
            if(modelBase.item(i,9)->text() != tr("Y"))
            {
                modelBase.item(i,3)->setText(QString(tr("-") + modelBase.item(i,3)->text()));
                float zongjia = modelBase.item(i,3)->text().toFloat() * modelBase.item(i,4)->text().toFloat() * modelBase.item(i,5)->text().toFloat() * 0.01 + modelBase.item(i,3)->text().toFloat() * modelBase.item(i,21)->text().toFloat();
                modelBase.item(i,6)->setText(QString().sprintf("%0.2f",zongjia));
            }
            else
            {
                modelBase.item(i,3)->setText(QString(tr("-") + modelBase.item(i,3)->text()));
            }
        }

        init_tuidan_data();

        query.prepare(QString("insert into cey_u_master( ch_billno , ch_state , vch_operID , dt_operdate)values(:danhao1,:state,:oper,:time) "));
        query.bindValue(":danhao1",danhao);
        query.bindValue(":state","1");
        query.bindValue(":oper",n_func::gs_operid);
        query.bindValue(":time",time);
        if(query.exec())
        {
        }
        else
        {
            QSqlDatabase::database().rollback();
            qDebug() << query.lastError();  lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_u_master失败!"));
            return;
        }
        QString xiaof = lineEdit.right(lineEdit.length() - 1);
        if(query.exec(QString("insert into cey_u_checkout_master( ch_payno , ch_billno , ch_tableno , num_cost , num_discount , num_present , num_service , num_lowcost , num_blotout , num_run , vch_run_operID , ch_state , vch_operID , dt_operdate , vch_operID2 , dt_operdate2 ,num_rate) values('%1','%2','%3','%4','%5','%6','%7','%8','%9','%10',null,'%11','%12','%13',null,null,'%14')")
                      .arg(liushuihao).arg(danhao).arg(zhuohao).arg(lineEdit).arg(tr("-0.00")).arg(tr("0.00"))
                      .arg("0").arg("0").arg(xiaof).arg("0").arg("Y").arg(n_func::gs_operid).arg(time).arg(lineEdit_6)))
        {

        }
        else
        {
            QSqlDatabase::database().rollback();
            qDebug() << query.lastError();  lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_u_checkout_master失败!"));
            return;
        }


        if(query.exec(QString("insert into cey_u_table( ch_billno , ch_tableno , int_person , vch_memo , ch_state , ch_payno , vch_operID , dt_operdate , ch_order_state , num_cost)values('%1','%2','%3',null,'%4','%5','%6','%7','%8','%9')")
                      .arg(danhao).arg(zhuohao).arg("1").arg("1").arg(liushuihao).arg(n_func::gs_operid).arg(time).arg("4").arg(lineEdit)))
        {

        }
        else
        {
            QSqlDatabase::database().rollback();
            qDebug() << query.lastError();  lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_u_table失败!"));

            return;
        }

        float aa = 0.00;
        aa = lineEdit_4.toFloat();
        if(query.exec(QString("insert into cey_u_checkout_detail(ch_payno , ch_paymodeno , num_payamount , num_change , num_realamount , num_face ,vch_operID , dt_operdate ,num_card_remain,vch_voucherno, num_face_Num )values('%1','%2','%3','%4','%5','%6','%7','%8','%9','null','%10')")
                      .arg(liushuihao).arg("00").arg(QString().sprintf("%0.2f",aa)).arg(tr("00")).arg(QString().sprintf("%0.2f",aa)).arg("0.0")
                      .arg(n_func::gs_operid).arg(time).arg("0.00").arg("0.0")))
        {

        }
        else
        {
            QSqlDatabase::database().rollback();
            lds_messagebox::warning(this, tr("消息提示"),tr("cey_u_checkout_detail失败!"));
            return;
        }

        for(int i = 0; i < modelBase.rowCount(); i++)
        {
            query.prepare(QString("update cey_bt_dish_warn set num_sale = '0.0' where ch_state ='Y' and ch_dishno = :pinma and left(dt_date,10) =:time "));
            query.bindValue(":pinma",modelBase.item(i,1)->text());
            query.bindValue(":time",timer);
            if(query.exec())
            {
            }
            else
            {
                QSqlDatabase::database().rollback();
                lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_bt_dish_warn失败!"));
                return;
            }


            //菜品消费明细表================
            float add_price;
            add_price = 0.00;
            QString shuliang_m = modelBase.item(i,3)->text().right(modelBase.item(i,3)->text().length() - 1);

            add_price = shuliang_m.toFloat() * modelBase.item(i,21)->text().toFloat();
            if(modelBase.item(i,8)->text() == "Y")//赠送菜品
            {
                if(modelBase.item(i,7)->text().isEmpty())
                {
                    if(query.exec(QString("insert into cey_u_orderdish(ch_billno,ch_tableno,int_id,ch_dishno,num_price,num_price_org,num_price_add,num_num,num_back,vch_back_operID,dt_back_operdate,int_discount,vch_dis_operID,dt_dis_operdate,vch_memberno,ch_consult,ch_suitflag,ch_suitno,ch_specialflag,ch_presentflag,vch_pre_operID,dt_pre_operdate,vch_operID,dt_operdate,ch_togono,ch_payno,vch_print_memo) values('%1','%2','%3','%4','%5','%6','%7','%8','%9',null,null,'%10',null,null,null,'%11','%12',null,'%13','%14','%15','%16','%17','%18',null,'%19',null)")
                                  .arg(danhao).arg(zhuohao).arg(QString::number(i+1)).arg(modelBase.item(i,1)->text()).arg(modelBase.item(i,4)->text()).arg(modelBase.item(i,4)->text())
                                  .arg(QString().sprintf("%0.2f",add_price)).arg("0.00").arg(shuliang_m).arg(modelBase.item(i,5)->text()).arg("0").arg(modelBase.item(i,9)->text())
                                  .arg(modelBase.item(i,10)->text()).arg(modelBase.item(i,8)->text()).arg(n_func::gs_operid).arg(modelBase.item(i,20)->text()).arg(n_func::gs_operid)
                                  .arg(time).arg(liushuihao)))
                    {

                    }
                    else
                    {
                        QSqlDatabase::database().rollback();
                        qDebug() << query.lastError();  lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_u_orderdish菜品消费明细表失败!"));

                        return;
                    }
                }
                else
                {
                    query.prepare(QString("insert into cey_u_orderdish(ch_billno,ch_tableno,int_id,ch_dishno,num_price,num_price_org,num_price_add,num_num,num_back,vch_back_operID,dt_back_operdate,int_discount,vch_dis_operID,dt_dis_operdate,vch_memberno,ch_consult,ch_suitflag,ch_suitno,ch_specialflag,ch_presentflag,vch_pre_operID,dt_pre_operdate,vch_operID,dt_operdate,ch_togono,ch_payno,vch_print_memo) values(:str,:str1,:str2,:str3,:str4,:str5,:str6,:str7,:str8,null,null,:str9,null,null,null,:str10,:str11,null,:str12,:str13,:str14,:str15,:str16,:str17,null,:str18,:str19)"));
                    query.bindValue(":str",danhao);
                    query.bindValue(":str1",zhuohao);
                    query.bindValue(":str2",QString::number(i+1));
                    query.bindValue(":str3",modelBase.item(i,1)->text());
                    query.bindValue(":str4",modelBase.item(i,4)->text());
                    query.bindValue(":str5",modelBase.item(i,4)->text());
                    query.bindValue(":str6",QString().sprintf("%0.2f",add_price));
                    query.bindValue(":str7",tr("0.00"));
                    query.bindValue(":str8",shuliang_m);
                    query.bindValue(":str9",modelBase.item(i,5)->text());
                    query.bindValue(":str10","0");
                    query.bindValue(":str11",modelBase.item(i,9)->text());
                    query.bindValue(":str12",modelBase.item(i,10)->text());
                    query.bindValue(":str13",modelBase.item(i,8)->text());
                    query.bindValue(":str14",n_func::gs_operid);
                    query.bindValue(":str15",modelBase.item(i,20)->text());
                    query.bindValue(":str16",n_func::gs_operid);
                    query.bindValue(":str17",time);
                    query.bindValue(":str18",liushuihao);
                    query.bindValue(":str19",modelBase.item(i,7)->text());

                    if(query.exec())
                    {

                    }
                    else
                    {
                        QSqlDatabase::database().rollback();
                        qDebug() << query.lastError();  lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_u_orderdish失败菜品消费明细表!"));
                        return;
                    }
                }
            }
            else if(modelBase.item(i,5)->text().toFloat() != 100)//折扣菜品
            {
                if(modelBase.item(i,7)->text().isEmpty())
                {
                    if(query.exec(QString("insert into cey_u_orderdish(ch_billno,ch_tableno,int_id,ch_dishno,num_price,num_price_org,num_price_add,num_num,num_back,vch_back_operID,dt_back_operdate,int_discount,vch_dis_operID,dt_dis_operdate,vch_memberno,ch_consult,ch_suitflag,ch_suitno,ch_specialflag,ch_presentflag,vch_pre_operID,dt_pre_operdate,vch_operID,dt_operdate,ch_togono,ch_payno,vch_print_memo) values('%1','%2','%3','%4','%5','%6','%7','%8','%9','%10','%11','%12','%13','%14',null,'%15','%16',null,'%17','%18',null,null,'%19','%20',null,'%21',null)")
                                  .arg(danhao).arg(zhuohao).arg(QString::number(i+1)).arg(modelBase.item(i,1)->text()).arg(modelBase.item(i,4)->text()).arg(modelBase.item(i,4)->text())
                                  .arg(QString().sprintf("%0.2f",add_price)).arg("0.00").arg(shuliang_m).arg(n_func::gs_operid).arg(modelBase.item(i,19)->text()).arg(QString::number(int(modelBase.item(i,5)->text().toFloat()))).arg(n_func::gs_operid)
                                  .arg(modelBase.item(i,19)->text()).arg("1").arg("N").arg("N").arg("N").arg(n_func::gs_operid).arg(time).arg(liushuihao)))
                    {

                    }
                    else
                    {
                        QSqlDatabase::database().rollback();
                        qDebug() << query.lastError();  lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_u_orderdish失败菜品消费明细表!"));

                        return;
                    }
                }
                else
                {
                    query.prepare(QString("insert into cey_u_orderdish(ch_billno,ch_tableno,int_id,ch_dishno,num_price,num_price_org,num_price_add,num_num,num_back,vch_back_operID,dt_back_operdate,int_discount,vch_dis_operID,dt_dis_operdate,vch_memberno,ch_consult,ch_suitflag,ch_suitno,ch_specialflag,ch_presentflag,vch_pre_operID,dt_pre_operdate,vch_operID,dt_operdate,ch_togono,ch_payno,vch_print_memo) values(:str,:str1,:str2,:str3,:str4,:str5,:str6,:str7,:str8,:zhekouren,:zhekoutime,:str9,:str10,:str11,null,:str12,:str13,null,:str14,:str15,null,null,:str16,:str17,null,:str18,:str19)"));
                    query.bindValue(":str",danhao);
                    query.bindValue(":str1",zhuohao);
                    query.bindValue(":str2",QString::number(i+1));
                    query.bindValue(":str3",modelBase.item(i,1)->text());
                    query.bindValue(":str4",modelBase.item(i,4)->text());
                    query.bindValue(":str5",modelBase.item(i,4)->text());
                    query.bindValue(":str6",QString().sprintf("%0.2f",add_price));
                    query.bindValue(":str7",tr("0.00"));
                    query.bindValue(":str8",shuliang_m);
                    query.bindValue(":zhekouren",n_func::gs_operid);
                    query.bindValue(":zhekoutime",modelBase.item(i,19)->text());
                    query.bindValue(":str9",QString::number(int(modelBase.item(i,5)->text().toFloat())));
                    query.bindValue(":str10",n_func::gs_operid);
                    query.bindValue(":str11",modelBase.item(i,19)->text());
                    query.bindValue(":str12","1");
                    query.bindValue(":str13","N");
                    query.bindValue(":str14","N");
                    query.bindValue(":str15","N");
                    query.bindValue(":str16",n_func::gs_operid);
                    query.bindValue(":str17",time);
                    query.bindValue(":str18",liushuihao);
                    query.bindValue(":str19",modelBase.item(i,7)->text());
                    if(query.exec())
                    {

                    }
                    else
                    {
                        QSqlDatabase::database().rollback();
                        lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_u_orderdish菜品消费明细表折扣菜品失败!"));
                        return;
                    }
                }
            }
            else//普通菜品
            {
                if(modelBase.item(i,7)->text().isEmpty())//无做法
                {
                    if(query.exec(QString("insert into cey_u_orderdish(ch_billno,ch_tableno,int_id,ch_dishno,num_price,num_price_org,num_price_add,num_num,num_back,vch_back_operID,dt_back_operdate,int_discount,vch_dis_operID,dt_dis_operdate,vch_memberno,ch_consult,ch_suitflag,ch_suitno,ch_specialflag,ch_presentflag,vch_pre_operID,dt_pre_operdate,vch_operID,dt_operdate,ch_togono,ch_payno,vch_print_memo) values('%1','%2','%3','%4','%5','%6','%7','%8','%9',null,null,'%10',null,null,null,'%11','%12',null,'%13','%14',null,null,'%15','%16',null,'%17',null)")
                                  .arg(danhao).arg(zhuohao).arg(QString::number(i+1)).arg(modelBase.item(i,1)->text()).arg(modelBase.item(i,4)->text()).arg(modelBase.item(i,4)->text())
                                  .arg(QString().sprintf("%0.2f",add_price)).arg("0.00").arg(shuliang_m).arg(modelBase.item(i,5)->text()).arg("0").arg(modelBase.item(i,9)->text())
                                  .arg(modelBase.item(i,10)->text()).arg(modelBase.item(i,8)->text()).arg(n_func::gs_operid)
                                  .arg(time).arg(liushuihao)))
                    {

                    }
                    else
                    {
                        QSqlDatabase::database().rollback();
                        qDebug() << query.lastError();  lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_u_orderdish菜品消费明细表失败!"));

                        return;
                    }
                }
                else
                {
                    query.prepare(QString("insert into cey_u_orderdish(ch_billno,ch_tableno,int_id,ch_dishno,num_price,num_price_org,num_price_add,num_num,num_back,vch_back_operID,dt_back_operdate,int_discount,vch_dis_operID,dt_dis_operdate,vch_memberno,ch_consult,ch_suitflag,ch_suitno,ch_specialflag,ch_presentflag,vch_pre_operID,dt_pre_operdate,vch_operID,dt_operdate,ch_togono,ch_payno,vch_print_memo) values(:str,:str1,:str2,:str3,:str4,:str5,:str6,:str7,:str8,null,null,:str9,null,null,null,:str10,:str11,null,:str12,:str13,null,null,:str14,:str15,null,:str16,:str17)"));
                    query.bindValue(":str",danhao);
                    query.bindValue(":str1",zhuohao);
                    query.bindValue(":str2",QString::number(i+1));
                    query.bindValue(":str3",modelBase.item(i,1)->text());
                    query.bindValue(":str4",modelBase.item(i,4)->text());
                    query.bindValue(":str5",modelBase.item(i,4)->text());
                    query.bindValue(":str6",QString().sprintf("%0.2f",add_price));
                    query.bindValue(":str7",tr("0.00"));
                    query.bindValue(":str8",shuliang_m);
                    query.bindValue(":str9",modelBase.item(i,5)->text());
                    query.bindValue(":str10","0");
                    query.bindValue(":str11",modelBase.item(i,9)->text());
                    query.bindValue(":str12",modelBase.item(i,10)->text());
                    query.bindValue(":str13",modelBase.item(i,8)->text());
                    query.bindValue(":str14",n_func::gs_operid);
                    query.bindValue(":str15",time);
                    query.bindValue(":str16",liushuihao);
                    query.bindValue(":str17",modelBase.item(i,7)->text());

                    if(query.exec())
                    {

                    }
                    else
                    {
                        QSqlDatabase::database().rollback();
                        qDebug() << query.lastError();  lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_u_orderdish菜品消费明细表普通菜品失败!"));

                        return;
                    }
                }
            }
            //菜品消费明细表==================
            model.clear();
            model.setQuery(QString("select Max(int_orderflow) from cey_u_orderdish_log "));
            int flowid = model.record(0).value(0).toInt() + 1;

            query.prepare(QString("Insert into cey_u_orderdish_log(int_orderflow , ch_type , vch_impower , vch_operID , dt_operdate , vch_reason )values(:id,:type,:impower,:oper,:time,:tuidan)"));
            query.bindValue(":id",QString::number(flowid));
            query.bindValue(":type","1");
            query.bindValue(":impower",n_func::gs_operid);
            query.bindValue(":oper",n_func::gs_operid);
            query.bindValue(":time",time);
            query.bindValue(":tuidan",tr("退单"));

            if(query.exec())
            {

            }
            else
            {
                QSqlDatabase::database().rollback();
                qDebug() << query.lastError();  lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_u_orderdish_log菜品消费明细表普通菜品失败!"));

                return;
            }
        }
        dayindanhao = danhao;
        QSqlDatabase::database().commit();
        print();
        print_chufangdayin();
        re_model();
        //ui->pushButton_3->setText(tr("取单"));
        ui->pushButton_takebill->setText(tr("取单"));
        modelBase.removeRows(0, modelBase.rowCount());
        this->dish_detail_clear();
        this->to_dish_title( 0.000, 0.000);
    }
}

void fast_order_2::to_takebill()//挂单/取单
{
    QSqlQueryModel model;
    QSqlQuery query;
    QSqlDatabase::database().transaction();
    QString zhuohao = "Q000";
    model.setQuery(QString("select Max(int_flowID) from cey_u_hungbill"));
    QString flowid = QString::number(model.record(0).value(0).toInt() + 1);
    model.clear();
    model.setQuery(QString("Select NOW()"));
    QString time = model.record(0).value(0).toDateTime().toString("yyyy-MM-dd hh:mm:ss");

    if(ui->pushButton_takebill->text() == tr("挂单"))
    {
        if(0==lds_messagebox::warning(this, tr("消息提示"),tr("是否确定挂单?请确认!"),tr("确认"),tr("取消")))
        {
            for(int i = 0; i < modelBase.rowCount(); i++)
            {
                float add_price = 0.00;
                add_price = modelBase.item(i,3)->text().toFloat() * modelBase.item(i,21)->text().toFloat();
                if(modelBase.item(i,8)->text() == tr("Y"))//赠送菜
                {
                    if(modelBase.item(i,7)->text().trimmed().isEmpty())//无做法
                    {
                        query.prepare(QString("INSERT INTO cey_u_hungbill(int_flowID , int_id , ch_tableno , ch_dishno , num_price , num_num , num_back , vch_back_operID , dt_back_operdate , int_discount , vch_dis_operID , dt_dis_operdate , ch_printflag , vch_print_memo , ch_suitflag , ch_suitno , ch_specialflag , ch_presentflag , vch_pre_operID , dt_pre_operdate , vch_operID , dt_operdate , num_price_add)VALUES(:flowid , :hang , :zhuohao , :str1 ,:str2 , :str3 , :str4 , null , null , :str5 , null , null , :str6 , null, :str8 , null , :str9 , :str10 , :str11 , :str12 , :str13 , :str14 , :str15) "));
                        query.bindValue(":flowid",flowid);
                        query.bindValue(":hang",modelBase.item(i, 0)->text());
                        query.bindValue(":zhuohao",zhuohao);
                        query.bindValue(":str1",modelBase.item(i,1)->text());
                        query.bindValue(":str2",modelBase.item(i,4)->text());
                        query.bindValue("str3",modelBase.item(i,3)->text());
                        query.bindValue(":str4",tr("0.00"));
                        query.bindValue(":str5",tr("100"));
                        query.bindValue(":str6","N");
                        query.bindValue(":str8",modelBase.item(i,9)->text());
                        query.bindValue(":str9",modelBase.item(i,10)->text());
                        query.bindValue(":str10",tr("Y"));
                        query.bindValue(":str11",n_func::gs_operid);
                        query.bindValue(":str12",modelBase.item(i,20)->text());
                        query.bindValue(":str13",n_func::gs_operid);
                        query.bindValue(":str14",time);
                        query.bindValue(":str15",QString().sprintf("%0.2f",add_price));

                        if(query.exec())
                        {

                        }
                        else
                        {
                            QSqlDatabase::database().rollback();
                            qDebug() << query.lastError();  lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_u_hungbill失败!"));

                            return;
                        }
                    }
                    else
                    {
                        query.prepare(QString("INSERT INTO cey_u_hungbill(int_flowID , int_id , ch_tableno , ch_dishno , num_price , num_num , num_back , vch_back_operID , dt_back_operdate , int_discount , vch_dis_operID , dt_dis_operdate , ch_printflag , vch_print_memo , ch_suitflag , ch_suitno , ch_specialflag , ch_presentflag , vch_pre_operID , dt_pre_operdate , vch_operID , dt_operdate , num_price_add)VALUES(:flowid , :hang , :zhuohao , :str1 ,:str2 , :str3 , :str4 , null , null , :str5 , null , null , :str6 , :str7 , :str8 , null , :str9 , :str10 , :str11 , :str12 , :str13 , :str14 , :str15) "));
                        query.bindValue(":flowid",flowid);
                        query.bindValue(":hang",modelBase.item(i, 0)->text());
                        query.bindValue(":zhuohao",zhuohao);
                        query.bindValue(":str1",modelBase.item(i,1)->text());
                        query.bindValue(":str2",modelBase.item(i,4)->text());
                        query.bindValue("str3",modelBase.item(i,3)->text());
                        query.bindValue(":str4",tr("0.00"));
                        query.bindValue(":str5",tr("100"));
                        query.bindValue(":str6","N");
                        query.bindValue(":str7",modelBase.item(i,7)->text());
                        query.bindValue(":str8",modelBase.item(i,9)->text());
                        query.bindValue(":str9",modelBase.item(i,10)->text());
                        query.bindValue(":str10",tr("Y"));
                        query.bindValue(":str11",n_func::gs_operid);
                        query.bindValue(":str12",modelBase.item(i,20)->text());
                        query.bindValue(":str13",n_func::gs_operid);
                        query.bindValue(":str14",time);
                        query.bindValue(":str15",QString().sprintf("%0.2f",add_price));

                        if(query.exec())
                        {

                        }
                        else
                        {
                            QSqlDatabase::database().rollback();
                            qDebug() << query.lastError();  lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_u_hungbill失败!"));

                            return;
                        }
                    }
                }
                else if(modelBase.item(i,5)->text().toFloat() != 100)//折扣菜
                {
                    if(modelBase.item(i,7)->text().isEmpty())//无做法
                    {
                        query.prepare(QString("INSERT INTO cey_u_hungbill(int_flowID , int_id , ch_tableno , ch_dishno , num_price , num_num , num_back , vch_back_operID , dt_back_operdate , int_discount , vch_dis_operID , dt_dis_operdate , ch_printflag , vch_print_memo , ch_suitflag , ch_suitno , ch_specialflag , ch_presentflag , vch_pre_operID , dt_pre_operdate , vch_operID , dt_operdate , num_price_add)VALUES(:flowid , :hang , :zhuohao , :str1 , :str2 , :str3 , :str4 , null , null ,:str5, :str6 , :str7 ,:str8 , null , :str9 , null ,:str10 ,:Str11 , null , null , :str12 , :str13 ,:str14)  "));
                        query.bindValue(":flowid",flowid);
                        query.bindValue(":hang",modelBase.item(i, 0)->text());
                        query.bindValue(":zhuohao",zhuohao);
                        query.bindValue(":str1",modelBase.item(i,1)->text());
                        query.bindValue(":str2",modelBase.item(i,4)->text());
                        query.bindValue(":str3",modelBase.item(i,3)->text());
                        query.bindValue(":str4",tr("0.00"));
                        query.bindValue(":str5",QString::number(int(modelBase.item(i,5)->text().toFloat())));
                        query.bindValue(":str6",n_func::gs_operid);
                        query.bindValue(":str7",modelBase.item(i,19)->text());
                        query.bindValue(":str8","N");
                        query.bindValue(":str9",modelBase.item(i,9)->text());
                        query.bindValue(":str10",modelBase.item(i,10)->text());
                        query.bindValue(":str11",modelBase.item(i,8)->text());
                        query.bindValue(":str12",n_func::gs_operid);
                        query.bindValue(":str13",time);
                        query.bindValue(":str14",QString().sprintf("%0.2f",add_price));

                        if(query.exec())
                        {

                        }
                        else
                        {
                            QSqlDatabase::database().rollback();
                            qDebug() << query.lastError();  lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_u_hungbill失败!"));

                            return;
                        }
                    }
                    else
                    {
                        query.prepare(QString("INSERT INTO cey_u_hungbill(int_flowID , int_id , ch_tableno , ch_dishno , num_price , num_num , num_back , vch_back_operID , dt_back_operdate , int_discount , vch_dis_operID , dt_dis_operdate , ch_printflag , vch_print_memo , ch_suitflag , ch_suitno , ch_specialflag , ch_presentflag , vch_pre_operID , dt_pre_operdate , vch_operID , dt_operdate , num_price_add)VALUES(:flowid , :hang , :zhuohao , :str1 , :str2 , :str3 , :str4 , null , null ,:str5, :str6 , :str7 ,:str8 , :str15 , :str9 , null ,:str10 ,:Str11 , null , null , :str12 , :str13 ,:str14)  "));
                        query.bindValue(":flowid",flowid);
                        query.bindValue(":hang",modelBase.item(i, 0)->text());
                        query.bindValue(":zhuohao",zhuohao);
                        query.bindValue(":str1",modelBase.item(i,1)->text());
                        query.bindValue(":str2",modelBase.item(i,4)->text());
                        query.bindValue(":str3",modelBase.item(i,3)->text());
                        query.bindValue(":str4",tr("0.00"));
                        query.bindValue(":str5",QString::number(int(modelBase.item(i,5)->text().toFloat())));
                        query.bindValue(":str6",n_func::gs_operid);
                        query.bindValue(":str7",modelBase.item(i,19)->text());
                        query.bindValue(":str8","N");
                        query.bindValue(":str15",modelBase.item(i,7)->text());
                        query.bindValue(":str9",modelBase.item(i,9)->text());
                        query.bindValue(":str10",modelBase.item(i,10)->text());
                        query.bindValue(":str11",modelBase.item(i,8)->text());
                        query.bindValue(":str12",n_func::gs_operid);
                        query.bindValue(":str13",time);
                        query.bindValue(":str14",QString().sprintf("%0.2f",add_price));

                        if(query.exec())
                        {

                        }
                        else
                        {
                            QSqlDatabase::database().rollback();
                            qDebug() << query.lastError();  lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_u_hungbill失败!"));

                            return;
                        }
                    }
                }
                else//普通菜
                {
                    if(modelBase.item(i,7)->text().trimmed().isEmpty())//无做法
                    {
                        query.prepare(QString("INSERT INTO cey_u_hungbill(int_flowID , int_id , ch_tableno , ch_dishno , num_price , num_num , num_back , vch_back_operID , dt_back_operdate , int_discount , vch_dis_operID , dt_dis_operdate , ch_printflag , vch_print_memo , ch_suitflag , ch_suitno , ch_specialflag , ch_presentflag , vch_pre_operID , dt_pre_operdate , vch_operID , dt_operdate , num_price_add)VALUES(:flowid , :hang , :zhuohao  , :str1 , :str2 ,:str3 , :str4 , null , null , :str5 , null , null , :str6 , null , :str7 , null , :str8 , :str9 , null , null , :str10 , :str11 ,:str12)"));
                        query.bindValue(":flowid",flowid);
                        query.bindValue(":hang",modelBase.item(i, 0)->text());
                        query.bindValue(":zhuohao",zhuohao);
                        query.bindValue(":str1",modelBase.item(i,1)->text());
                        query.bindValue(":str2",modelBase.item(i,4)->text());
                        query.bindValue(":str3",modelBase.item(i,3)->text());
                        query.bindValue(":str4",tr("0.00"));
                        query.bindValue(":str5",tr("100"));
                        query.bindValue(":str6","N");
                        query.bindValue(":str7",modelBase.item(i,9)->text());
                        query.bindValue(":str8",modelBase.item(i,10)->text());
                        query.bindValue(":str9",modelBase.item(i,8)->text());
                        query.bindValue(":str10",n_func::gs_operid);
                        query.bindValue(":str11",time);
                        query.bindValue(":str12",QString().sprintf("%0.2f",add_price));

                        if(query.exec())
                        {

                        }
                        else
                        {
                            QSqlDatabase::database().rollback();
                            qDebug() << query.lastError();  lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_u_hungbill失败!"));

                            return;
                        }
                    }
                    else
                    {
                        query.prepare(QString("INSERT INTO cey_u_hungbill(int_flowID , int_id , ch_tableno , ch_dishno , num_price , num_num , num_back , vch_back_operID , dt_back_operdate , int_discount , vch_dis_operID , dt_dis_operdate , ch_printflag , vch_print_memo , ch_suitflag , ch_suitno , ch_specialflag , ch_presentflag , vch_pre_operID , dt_pre_operdate , vch_operID , dt_operdate , num_price_add)VALUES(:flowid , :hang , :zhuohao  , :str1 , :str2 ,:str3 , :str4 , null , null , :str5 , null , null , :str6 , :str13 , :str7 , null , :str8 , :str9 , null , null , :str10 , :str11 ,:str12)"));
                        query.bindValue(":flowid",flowid);
                        query.bindValue(":hang",modelBase.item(i, 0)->text());
                        query.bindValue(":zhuohao",zhuohao);
                        query.bindValue(":str1",modelBase.item(i,1)->text());
                        query.bindValue(":str2",modelBase.item(i,4)->text());
                        query.bindValue(":str3",modelBase.item(i,3)->text());
                        query.bindValue(":str4",tr("0.00"));
                        query.bindValue(":str5",tr("100"));
                        query.bindValue(":str6","N");
                        query.bindValue(":str13",modelBase.item(i,7)->text());
                        query.bindValue(":str7",modelBase.item(i,9)->text());
                        query.bindValue(":str8",modelBase.item(i,10)->text());
                        query.bindValue(":str9",modelBase.item(i,8)->text());
                        query.bindValue(":str10",n_func::gs_operid);
                        query.bindValue(":str11",time);
                        query.bindValue(":str12",QString().sprintf("%0.2f",add_price));

                        if(query.exec())
                        {

                        }
                        else
                        {
                            QSqlDatabase::database().rollback();
                            qDebug() << query.lastError();  lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_u_hungbill失败!"));

                            return;
                        }
                    }
                }
            }
            QSqlDatabase::database().commit();
            re_model();
            modelBase.removeRows(0, modelBase.rowCount());
            this->dish_detail_clear();
            this->to_dish_title( 0.000, 0.000);
            ui->pushButton_takebill->setText(tr("取单"));
            this->dish_detail_clear();
            lds_messagebox::warning(this, tr("消息提示"),tr("挂单成功,挂单号为:") + flowid);
            return;
        }
    }
    else//取单
    {
        fast_qudan qudan;
        /*qudan->exec();*/backheader::static_dialogexec(qudan, tr("快餐取单表"));
        if(qudan.btn_t_f == true)
        {
            re_model();
            //ui->tableView->tableview->setColumnHidden(1,true);
            //ui->tableView->tableview->setColumnHidden(3,true);
            //ui->tableView->tableview->setColumnHidden(4,true);
            //ui->tableView->tableview->setColumnHidden(5,true);
            //ui->tableView->tableview->setColumnHidden(6,true);
            //ui->tableView->tableview->setColumnHidden(7,true);
            //ui->tableView->tableview->setColumnHidden(8,true);
            //ui->tableView->tableview->setColumnHidden(9,true);
            //ui->tableView->tableview->setColumnHidden(10,true);
            //ui->tableView->tableview->setColumnHidden(11,true);
            //ui->tableView->tableview->setColumnHidden(12,true);
            //ui->tableView->tableview->setColumnHidden(14,true);
            //ui->tableView->tableview->setColumnHidden(15,true);
            //ui->tableView->tableview->setColumnHidden(16,true);
            //ui->tableView->tableview->setColumnHidden(17,true);
            //ui->tableView->tableview->setColumnHidden(18,true);
            //ui->tableView->tableview->setColumnHidden(19,true);
            //ui->tableView->tableview->setColumnHidden(20,true);
            //ui->tableView->tableview->setColumnHidden(21,true);
            //ui->tableView->tableview->setColumnHidden(22,true);
            //ui->tableView->tableview->setColumnHidden(23,true);

            float label_shuliang = 0.00;
            float label_jine = 0.00;
            for(int i = 0; i < modelBase.rowCount(); i++)
            {
                label_shuliang += modelBase.item(i,3)->text().toFloat();
                if(modelBase.item(i,8)->text() == "N" && modelBase.item(i,9)->text() != "Y" )
                {
                    label_jine += modelBase.item(i,6)->text().toFloat();
                }
            }

            ui->pushButton_takebill->setText(tr("挂单"));
        }
    }
}

void fast_order_2::print_lost(const QString &str)
{
    ;
    lds_messagebox::warning(this, tr("消息提示"),tr("打印失败")+str);

}

void fast_order_2::print_louda()
{
    QSqlQueryModel model;
    model.setQuery(QString("select NOW()"));
    //    label_t->setText(model.record(0).value(0).toDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    thread->start();
}

void fast_order_2::show_print_close()
{
    leiji = 0;
    print_pr->hide();
}

void fast_order_2::thread_signal()
{
    leiji++;
    if(leiji == 3)
    {
        print_pr->show();
        QTimer::singleShot(30000,this,SLOT(show_print_close()));
    }
}

void fast_order_2::to_bianma()//编码点菜
{
    //    bianma->setModal(false);//设置为非模态
    //    bianma->show();
    //    bianma->setFocus();
    //    bianma_daicai_type = true;
}

void fast_order_2::bianma_diancai(const QString &str)
{
    dalei="";
    xiaolei="";
    if(str=="enter"){
        ui->widget->widget3_item_event_click();
    } else {
        init_cook("", str);
    }
}

void fast_order_2::bianma_cancel()
{
    //    bianma_daicai_type = false;
    //    itemList.clear();
    //    qDeleteAll(itemList.begin(),itemList.end());
    //    //qDeleteAll(//ui->widget_3->children());
    //    layout_cook = new QGridLayout();
    //    for(int i = 0; i < item_count; ++i)
    //    {
    //        GoodItem *item = new GoodItem("","");
    //        itemList.append(item);
    //        connect(item,SIGNAL(touch(QString)),this,SLOT(addGood(QString)));
    //        layout_cook.addWidget(item,i/item_count_row,i%item_count_row);
    //    }
    //    layout_cook.setContentsMargins(0,0,0,0);
    //    layout_cook.setSpacing(2);
    //    //ui->widget_3->setLayout(layout_cook);

    //    if(changyongcook == false)
    //    {
    //        model_cook.clear();
    //        model_cook.setQuery(QString("SELECT ch_dishno,vch_dishname,num_price FROM cey_bt_dish where ch_dish_typeno = '%1' and ch_sub_typeno ='%2' and (ch_stopflag = null or ch_stopflag = 'N') ").arg(dalei).arg(xiaolei));
    //        QString kind = model_xiaolei.record(0).value(0).toString();
    //        showGoods(kind);
    //    }
    //    else
    //    {
    //        model_cook.clear();
    //        model_cook.setQuery(QString("select ch_dishno,vch_dishname,num_price from cey_bt_dish where ch_off ='Y' and (ch_stopflag = null or ch_stopflag = 'N')  "));
    //        QString kind = model_xiaolei.record(0).value(0).toString();
    //        showGoods(kind);
    //    }
}

void fast_order_2::caculate_dishtitle()
{
    float label_shuliang = 0.00;
    float label_jine = 0.00;
    for(int i = 0; i < modelBase.rowCount(); i++)
    {
        modelBase.item(i,0)->setText(QString::number(i + 1));
        label_shuliang += modelBase.item(i,3)->text().toFloat();
        if(modelBase.item(i,8)->text() == "N" && modelBase.item(i,9)->text() != "Y" )
        {
            label_jine += modelBase.item(i,6)->text().toFloat();
        }
    }
    this->to_dish_title( label_shuliang, label_jine);
}

QString fast_order_2::get_currow_dishno()
{
    int row=ui->tableView->tableview->currentIndex().row();
    if(row>=0)
        return modelBase.item(row, 1)->text();
    else
        return "";
}

QString fast_order_2::get_currow_dishname()
{
    int row=ui->tableView->tableview->currentIndex().row();
    if(row>=0)
        return modelBase.item(row, 2)->text();
    else
        return "";
}

QString fast_order_2::get_currow_dishprice()
{
    int row=ui->tableView->tableview->currentIndex().row();
    if(row>=0)
        return modelBase.item(row, 4)->text();
    else
        return "";
}

QString fast_order_2::get_currow_dishnum()
{
    int row=ui->tableView->tableview->currentIndex().row();
    if(row>=0)
        return modelBase.item(row, 3)->text();
    else
        return "";
}

QString fast_order_2::get_currow_dishtotal()
{
    int row=ui->tableView->tableview->currentIndex().row();
    if(row>=0)
        return modelBase.item(row, 3)->text();
    else
        return "";
}

void fast_order_2::dish_detail_clear()
{
}

void fast_order_2::to_grade0(const QList<frontViewDialog_autoexpand::content_item_icon> &item_icon_list)
{
    ui->widget->to_grade0(item_icon_list);
}

void fast_order_2::to_grade1(const QList<frontViewDialog_autoexpand::content_item_icon> &item_icon_list)
{
    ui->widget->to_grade1(item_icon_list);
}

void fast_order_2::to_grade2(const QList<frontViewDialog_autoexpand::content_item_icon> &item_icon_list)
{
    ui->widget->to_grade2(item_icon_list);
}

void fast_order_2::to_dish_title(float num, float total)
{
    if(num>=0){ui->label_num->setText(tr("数量")+QString().sprintf("%.03f", num));_num=num;}
    if(total>=0){ui->label_total->setText(tr("金额")+QString().sprintf("%.02f", total));_total=total;}
}

void fast_order_2::to_dish_title_other(const QString &num, const QString &total)
{
    if(!num.isEmpty())ui->label_num->setText(num);
    if(!total.isEmpty())ui->label_total->setText(total);
}

void fast_order_2::set_num_total_to_doublescreen()
{
    if(public_sql::save_login->double_screen){
        connect(ui->label_num,SIGNAL(textChanged(QString)),public_sql::save_login->double_screen->ui->label_num,SLOT(setText(QString)));
        connect(ui->label_total,SIGNAL(textChanged(QString)),public_sql::save_login->double_screen->ui->label_total,SLOT(setText(QString)));
    }
}

float fast_order_2::get_cur_num()
{
    return _num;
}

void fast_order_2::to_scanbar()
{
    if(ui->lineEdit_scan->text().isEmpty()) return;
    addGood(ui->lineEdit_scan->text());
    ui->lineEdit_scan->clear();
}
void fast_order_2::to_weixinwaimai()
{
    w_scr_dish_weixinwaimai dialog(this);
    backheader::static_dialogexec(dialog,tr("微信外卖"));
}

void fast_order_2::to_weixinquitbill()
{
    w_scr_dish_weixin_quitbill dialog(this);
    backheader::static_dialogexec(dialog,tr("退单查询"));
}

void fast_order_2::to_weixinbill_import()
{
    if(modelBase.rowCount()>0){
        lds_messagebox::warning(this, tr("消息提示"),tr("前台有点菜信息,操作取消"));
        return;
    }
    lds_dialog_input inputdialog;
    inputdialog.ui->label->setText(tr("请输入微信订单单号"));
    if(QDialog::Accepted==backheader::static_dialogexec(inputdialog, tr("微信订单"))){
        if(inputdialog._text.isEmpty()){
            lds_messagebox::warning(this, tr("消息提示"),tr("订单单号不能为空!"));
            return;
        }
        QSqlQueryModel querymodel;
        querymodel.setQuery(QString("select tel, orderer, date from cey_web_order_info where sn='%1' and orderState='01' ").arg(inputdialog._text));
        if(querymodel.rowCount()<=0) {lds_messagebox::warning(this, tr("消息提示"),tr("订单单号不存在!"));return;}

        w_scr_dish_weixin_getbill dialog(this);
        dialog.ui->lineEdit_billno->setText(inputdialog._text);
        dialog.ui->lineEdit_tel->setText(querymodel.record(0).value("tel").toString());
        dialog.ui->lineEdit_orderer->setText(querymodel.record(0).value("orderer").toString());
        dialog.ui->lineEdit_date->setText(querymodel.record(0).value("date").toString());

        querymodel.setQuery(QString("select dishNo as "+tr("品码")+", dishName as "+tr("品名")+", number as "+tr("数量")+", Price as "+tr("价格")+" from cey_web_order_dish where orderSn='%1' ").arg(inputdialog._text));
        dialog.ui->tableView->setModel(&querymodel);
        if(QDialog::Accepted==backheader::static_dialogexec(dialog, tr("微信订单详情"))){
            //    01->11
            if(w_sys_manage_cloudsync::up_weixin_change_takkeoutstate(
                        inputdialog._text,
                        "11"//交易结束-交易拒绝
                        )){
                //录入商品
                for(int row=0,rowcount=querymodel.rowCount();row<rowcount; row++){
                    addGood(querymodel.record(row).value(tr("品码")).toString());
                    to_num(querymodel.record(row).value(tr("数量")).toString());
                }
                lds_messagebox::warning(this, tr("消息提示"),tr("订单已受理!"));
            }
        }
    }
}
