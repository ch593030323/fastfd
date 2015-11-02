#include "system_setting.h"
#include "ui_system_setting.h"
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QDebug>
#include <QSettings>
#include <QSqlQuery>
#include "lds_messagebox.h"
#include "trans_u2g.h"
#include <QFileDialog>
#include "printer_pos_net.h"
#include "printer_pos_serial.h"
#include <QTimer>
#include "fast_order_2.h"
#include "public_mainprinter.h"
#include <QSqlError>
#include "public_sql.h"
#include "n_func.h"
#include "backheader.h"
#include "w_bt_dish_kitchenprinter.h"
#include "backheader.h"
#include "clientdisplay.h"
#include "electroniccountprice.h"
#include "lds_dialog_input.h"
#include "printer_postek.h"
#include "printer_tsc_ttp_244_plus.h"
#include "printer_argox_ppla.h"
#include "printer_argox_pplb.h"
#include "printer_gainscha.h"
#include "printer_gainscha_2120tf.h"
#include "barscale_main_dialog.h"
#include "labelprint_main_dialog.h"
#include "printer_normal_t2us.h"
#include "trans_u2g.h"
#include "ui_w_bt_dish_unitset_dialog.h"
#include "w_scr_dish_data_collection.h"

system_setting::system_setting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::system_setting)
{
    ui->setupUi(this);

    connect(ui->radioButton,SIGNAL(toggled(bool)),this,SLOT(widget_blotout_round(bool)));
    btn_t_f = false;
    countprice_scale=0;
    configIniRead = new QSettings(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    configIniWrite = new QSettings(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    ui->label_version->setText(n_func::gs_soft_version);
    show_data();

    ui->tabWidget->setCurrentIndex(0);
    ui->widget_labelprinter->hide();
}

system_setting::~system_setting()
{
    delete ui;
    delete configIniRead;
    delete configIniWrite;
    if(countprice_scale)

        delete countprice_scale;
}

void system_setting::init_tab1()
{
    QSqlQueryModel model;
    QString str;

    str=n_func::f_get_sysparm("blotout_round");
    //str = model.record(0).value(0).toString();
    if(str == "1")
    {
        ui->radioButton->setChecked(true);
    }
    else if(str == "2")
    {
        ui->radioButton_2->setChecked(true);
    }
    else
    {
        ui->radioButton->setChecked(true);
    }
    widget_blotout_round(ui->radioButton->isChecked());

    model.clear();
    str.clear();
    str=n_func::f_get_sysparm("blotout_mode");
    //str = model.record(0).value(0).toString();
    if(str == "1")
    {
        ui->radioButton_3->setChecked(true);
    }
    else if(str == "2")
    {
        ui->radioButton_4->setChecked(true);
    }
    else if(str == "3")
    {
        ui->radioButton_5->setChecked(true);
    }
    else if(str == "4")
    {
        ui->radioButton_6->setChecked(true);
    }
    else
    {
        ui->radioButton_3->setChecked(true);
    }

    model.clear();
    str.clear();
    str=n_func::f_get_sysparm("round_mode");
    //str = model.record(0).value(0).toString();
    if(str == "1")
    {
        ui->radioButton_7->setChecked(true);
    }
    else if(str == "2")
    {
        ui->radioButton_8->setChecked(true);
    }
    else if(str == "3")
    {
        ui->radioButton_9->setChecked(true);
    }
    else if(str == "4")
    {
        ui->radioButton_10->setChecked(true);
    }
    else
    {
        ui->radioButton_7->setChecked(true);
    }

    //买单打印账单
    str.clear();
    str = configIniRead->value("system_setting/maidanzhangdan").toString();
    if(str == "0")
    {
        ui->checkBox->setChecked(false);
    }
    else if(str == "1")
    {
        ui->checkBox->setChecked(true);
    }
    else
    {
        ui->checkBox->setChecked(false);
    }
    //买单账单不显示明细
    str.clear();
    str = configIniRead->value("system_setting/maidanmingxi").toString();
    if(str == "0")
    {
        ui->checkBox_2->setChecked(true);
    }
    else if(str == "1")
    {
        ui->checkBox_2->setChecked(false);
    }
    else
    {
        ui->checkBox_2->setChecked(false);
    }

    QPixmap pixmap(1, 40);
    pixmap.fill(Qt::transparent);
    QIcon icon(pixmap);
    //账单连续打印
    ui->comboBox_2->addItems(QStringList()<<"1"<<"2"<<"3");

    model.clear();
    str.clear();
    str=n_func::f_get_sysparm("bp_count");
    //str = model.record(0).value(0).toString();
    for(int i = 0; i < ui->comboBox_2->count(); i++)
    {
        if(ui->comboBox_2->itemText(i) == str)
        {
            ui->comboBox_2->setCurrentIndex(i);
        }
    }

    //外卖账单连续打印
    ui->comboBox_3->addItems(QStringList()<<"1"<<"2"<<"3");
    model.clear();
    str.clear();
    str=n_func::f_get_sysparm("bp_togo_count");
    //str = model.record(0).value(0).toString();
    for(int i = 0; i < ui->comboBox_3->count(); i++)
    {
        if(ui->comboBox_3->itemText(i) == str)
        {
            ui->comboBox_3->setCurrentIndex(i);
        }
    }

    //交班必须接班人
    model.clear();
    str.clear();
    str=n_func::f_get_sysparm("handover_oper");
    //str = model.record(0).value(0).toString();
    if(str == "0")
    {
        ui->checkBox_3->setChecked(false);
    }
    else if(str == "1")
    {
        ui->checkBox_3->setChecked(true);
    }
    else
    {
        ui->checkBox_3->setChecked(false);
    }

    //触摸屏系统退出为重新登录
    str.clear();
    str = configIniRead->value("system_setting/chongxidenglu").toString();
    if(str == "0")
    {
        ui->checkBox_4->setChecked(false);
    }
    else if(str == "1")
    {
        ui->checkBox_4->setChecked(true);
    }
    else
    {
        ui->checkBox_4->setChecked(false);
    }

    //现金默认付全款
    model.clear();
    str.clear();
    str=n_func::f_get_sysparm("cash_payall");
    //str = model.record(0).value(0).toString();
    if(str == "0")
    {
        ui->checkBox_5->setChecked(false);
    }
    else if(str == "1")
    {
        ui->checkBox_5->setChecked(true);
    }
    else
    {
        ui->checkBox_5->setChecked(false);
    }

    //点菜数量不允许为负
    model.clear();
    str.clear();
    str=n_func::f_get_sysparm("negative");
    //str = model.record(0).value(0).toString();
    if(str == "0")
    {
        ui->checkBox_7->setChecked(false);
    }
    else if(str == "1")
    {
        ui->checkBox_7->setChecked(true);
    }
    else
    {
        ui->checkBox_7->setChecked(false);
    }

    //手工处理服务费
    model.clear();
    str.clear();
    str=n_func::f_get_sysparm("service_handwork");
    //str = model.record(0).value(0).toString();
    if(str == "0")
    {
        ui->checkBox_6->setChecked(false);
    }
    else if(str == "1")
    {
        ui->checkBox_6->setChecked(true);
    }
    else
    {
        ui->checkBox_6->setChecked(false);
    }

    //服务费计入低消费
    model.clear();
    str.clear();
    str=n_func::f_get_sysparm("low_service");
    //str = model.record(0).value(0).toString();
    if(str == "0")
    {
        ui->checkBox_8->setChecked(false);
    }
    else if(str == "1")
    {
        ui->checkBox_8->setChecked(true);
    }
    else
    {
        ui->checkBox_8->setChecked(false);
    }

    //退菜必须输入退菜原因
    model.clear();
    str.clear();
    str=n_func::f_get_sysparm("orderdish_back");
    //str = model.record(0).value(0).toString();
    if(str == "0")
    {
        ui->checkBox_9->setChecked(false);
    }
    else if(str == "1")
    {
        ui->checkBox_9->setChecked(true);
    }
    else
    {
        ui->checkBox_9->setChecked(false);
    }

    //快餐必须输入台牌号
    model.clear();
    str.clear();
    str=n_func::f_get_sysparm("shortorder_tableno");
    //str = model.record(0).value(0).toString();
    if(str == "0")
    {
        ui->checkBox_11->setChecked(false);
    }
    else if(str == "1")
    {
        ui->checkBox_11->setChecked(true);
    }
    else
    {
        ui->checkBox_11->setChecked(false);
    }

    //外卖落单立即结帐
    model.clear();
    str.clear();
    str=n_func::f_get_sysparm("togo_pay_atonce");
    //str = model.record(0).value(0).toString();
    if(str == "0")
    {
        ui->checkBox_12->setChecked(false);
    }
    else if(str == "1")
    {
        ui->checkBox_12->setChecked(true);
    }
    else
    {
        ui->checkBox_12->setChecked(false);
    }

    //账单自动折扣值
    model.clear();
    str.clear();
    str=n_func::f_get_sysparm("pay_auto_discount");
    //str = model.record(0).value(0).toString();
    if(str == "0")
    {
        ui->checkBox_10->setChecked(false);
        ui->lineEdit->setEnabled(false);
    }
    else if(str == "1")
    {
        ui->checkBox_10->setChecked(true);
        ui->lineEdit->setEnabled(true);
    }
    else
    {
        ui->checkBox_10->setChecked(false);
        ui->lineEdit->setEnabled(false);
    }

    //折扣值
    model.clear();
    str.clear();
    str=n_func::f_get_sysparm("pay_auto_discount_value");
    //str = model.record(0).value(0).toString();
    ui->lineEdit->setText(str);

    //点菜后自动数量录入
    model.clear();
    str.clear();
    str=n_func::f_get_sysparm("ordertonum");
    //str = model.record(0).value(0).toString();
    if(str == "0")
    {
        ui->checkBox_14->setChecked(false);
    }
    else if(str == "1")
    {
        ui->checkBox_14->setChecked(true);
    }
    else
    {
        ui->checkBox_14->setChecked(false);
    }

    //抹零默认当前操作员
    model.clear();
    str.clear();
    str=n_func::f_get_sysparm("bolt_operator");
    //str = model.record(0).value(0).toString();
    if(str == "0")
    {
        ui->checkBox_15->setChecked(false);
    }
    else if(str == "1")
    {
        ui->checkBox_15->setChecked(true);
    }
    else
    {
        ui->checkBox_15->setChecked(false);
    }


    //外卖落单进行厨打
    str.clear();
    str = configIniRead->value("system_setting/waimailuodanchuda").toString();
    if(str == "0")
    {
        ui->checkBox_13->setChecked(false);
    }
    else if(str == "1")
    {
        ui->checkBox_13->setChecked(true);
    }
    else
    {
        ui->checkBox_13->setChecked(false);
    }

    //奶茶模式
    str.clear();
    ui->checkBox_17->setChecked(configIniRead->value("system_setting/naichamoshi").toBool());


    if(ui->checkBox_17->isChecked() == false)
    {
        ui->stackedWidget->widget(5)->setEnabled(false);//->setTabEnabled(5,false);
    }
    else
    {
        ui->stackedWidget->widget(5)->setEnabled(true);//->setTabEnabled(5,false);
    }
    //本机落单打印总单
    str.clear();
    str = configIniRead->value("system_setting/luodanzongdan").toString();
    if(str == "0")
    {
        ui->checkBox_18->setChecked(false);
    }
    else if(str == "1")
    {
        ui->checkBox_18->setChecked(true);
    }
    else
    {
        ui->checkBox_18->setChecked(false);
    }

    ui->comboBox->addItems(QStringList()<<tr("快餐模式")<<tr("酒楼模式"));
    str.clear();
    str = configIniRead->value("system_setting/yingyemoshi").toString();
    for(int i = 0; i < ui->comboBox->count(); i++)
    {
        if(i == str.toInt())
        {
            ui->comboBox->setCurrentIndex(i);
        }
    }

    ui->comboBox_4->clear();
    ui->comboBox_4->addItems(QStringList() << tr("餐桌选台") << tr("餐区选台"));
    str.clear();
    str = configIniRead->value("system_setting/xuantaifangshi").toString();
    for(int i = 0; i < ui->comboBox_4->count(); i++)
    {
        if(i == str.toInt())
        {
            ui->comboBox_4->setCurrentIndex(i);
        }
    }

    ui->comboBox_5->clear();
    ui->comboBox_5->addItems(QStringList() << tr("按大类") << tr("按小类")<< tr("全部"));

    str.clear();
    str = configIniRead->value("system_setting/xuancaifangshi").toString();
    for(int i = 0; i < ui->comboBox_5->count(); i++)
    {
        if(i == str.toInt())
        {
            ui->comboBox_5->setCurrentIndex(i);
        }
    }

    str.clear();
    str = configIniRead->value("system_setting/jiesuoyuangong").toString();
    if(str == "0")
    {
        ui->checkBox_16->setChecked(false);
    }
    else if(str == "1")
    {
        ui->checkBox_16->setChecked(true);
    }
    else
    {
        ui->checkBox_16->setChecked(false);
    }
}

void system_setting::init_tab2()
{

    QString str;
    //打印机接口类型
    ui->comboBox_30->clear();
    ui->comboBox_30->addItems(CommonHeader::creat_normal_printpathtypes());
    ui->comboBox_30->setCurrentIndex(ui->comboBox_30->findText(configIniRead->value("system_setting/printertype").toString()));

    //打印机端口
    QStringList strlist;
    ui->comboBox_31->clear();
    ui->pushButton_8->setVisible(false);
    switch(ui->comboBox_30->currentIndex()){
    case 0://USB
        ui->comboBox_31->addItems(f_scan_usbPort());
        break;
    case 1://串口
        ui->comboBox_31->addItems(f_scan_ttyPort());
        break;
    case 2:{//网口
        QSqlQuery query;
        QStringList ipports;
        query.exec("select vch_printip from cey_bt_kitchen_print;");
        while(query.next()){
            ipports.append(query.record().value(0).toString()+":"+"9100");
        }
        ui->comboBox_31->addItems(ipports);
        ui->pushButton_8->setVisible(true);
    }
        break;
    }
    ui->comboBox_31->setCurrentIndex(ui->comboBox_31->findText(configIniRead->value("system_setting/printerpath").toString()));
    //打印机类型
    ui->comboBox_97->clear();
    ui->comboBox_97->addItems(CommonHeader::creat_normal_printtypes());
    ui->comboBox_97->setCurrentIndex(configIniRead->value("system_setting/dayinjixinghao").toInt());

    //结帐打开钱箱
    ui->checkBox_39->setChecked(configIniRead->value("system_setting/qianxiang").toBool());
    ui->lineEdit_9->setText(configIniRead->value("system_setting/qianxiangzhiling", "1B 70 00 28 28").toString());
    //客显类型
    ui->comboBox_32->clear();
    ui->comboBox_32->addItems(CommonHeader::creat_clientdisplay_ledtypes());
    ui->comboBox_32->setCurrentIndex(ui->comboBox_32->findText(configIniRead->value("system_setting/kexianleixing").toString()));
    //客显端口
    ui->comboBox_45->clear();
    ui->comboBox_45->addItems(f_scan_ttyPort());
    ui->comboBox_45->setCurrentIndex(ui->comboBox_45->findText(configIniRead->value("system_setting/kexianduankou").toString()));
    ui->lineEdit_38->setText(configIniRead->value("system_setting/kexian_vfd_start").toString());
    //电子称型号
    ui->comboBox_47->clear();
    ui->comboBox_47->addItems(CommonHeader::creat_pricecount_scaletypes());
    ui->comboBox_47->setCurrentIndex(ui->comboBox_47->findText(configIniRead->value("system_setting/dianzichengxinghao").toString()));
    //电子称端口
    ui->comboBox_46->clear();
    ui->comboBox_46->addItems(f_scan_ttyPort());
    ui->comboBox_46->setCurrentIndex(ui->comboBox_46->findText(configIniRead->value("system_setting/dianzichengduankou").toString()));
    //电子称波特率
    ui->comboBox_49->clear();
    ui->comboBox_49->addItems(CommonHeader::creat_baudrates().values());
    ui->comboBox_49->setCurrentIndex(ui->comboBox_49->findText(configIniRead->value("system_setting/dianzicheng_baudrate").toString()));
    //是否显示副屏
    ui->checkBox_40->setChecked(configIniRead->value("system_setting/fupingxianshi").toBool());
    //显示广告
    ui->lineEdit_13->setEnabled(false);
    ui->lineEdit_13->setText(configIniRead->value("system_setting/guanggaolujing").toString());
    //菜品图片循环播放
    ui->lineEdit_14->setEnabled(false);
    ui->lineEdit_14->setText(configIniRead->value("system_setting/bofanglujing").toString());
    //菜品滚动字幕
    ui->lineEdit_40->setText(configIniRead->value("system_setting/bofangg_runningtext").toString());
}

void system_setting::init_tab3()
{

    QPixmap pixmap(1, 40);
    pixmap.fill(Qt::transparent);
    QIcon icon(pixmap);
    QSqlQueryModel model;
    QString str;

    //账单标题
    str = configIniRead->value("system_setting/zhangdanbiaoti").toString();
    ui->lineEdit_4->setText(str);

    //表头设置
    ui->checkBox_25->setChecked(configIniRead->value("system_setting/biaotoushezhi").toBool());

    //付款号
    str.clear();
    str=n_func::f_get_sysparm("bp_payno");
    //str = model.record(0).value(0).toString();
    if(str == "0")
    {
        ui->checkBox_26->setChecked(false);
    }
    else
    {
        ui->checkBox_26->setChecked(true);
    }

    str.clear();
    str=n_func::f_get_sysparm("bp_payno_len");
    //str = model.record(0).value(0).toString();
    ui->lineEdit_46->setText(str);

    //工号
    str.clear();
    str=n_func::f_get_sysparm("bp_operid");
    //str = model.record(0).value(0).toString();
    if(str == "0")//不分组
    {
        ui->checkBox_65->setChecked(false);
    }
    else
    {
        ui->checkBox_65->setChecked(true);
    }

    str.clear();
    str=n_func::f_get_sysparm("bp_operid_len");
    //str = model.record(0).value(0).toString();
    ui->lineEdit_58->setText(str);

    //餐桌
    str.clear();
    str=n_func::f_get_sysparm("bp_table");
    //str = model.record(0).value(0).toString();
    if(str == "0")//不分组
    {
        ui->checkBox_66->setChecked(false);
    }
    else
    {
        ui->checkBox_66->setChecked(true);
    }

    str.clear();
    str=n_func::f_get_sysparm("bp_table_len");
    //str = model.record(0).value(0).toString();
    ui->lineEdit_47->setText(str);

    //人数
    str.clear();
    str=n_func::f_get_sysparm("bp_person");
    //str = model.record(0).value(0).toString();
    if(str == "0")//不分组
    {
        ui->checkBox_67->setChecked(false);
    }
    else
    {
        ui->checkBox_67->setChecked(true);
    }

    str.clear();
    str=n_func::f_get_sysparm("bp_person_len");
    //str = model.record(0).value(0).toString();
    ui->lineEdit_57->setText(str);

    //时间
    str.clear();
    str=n_func::f_get_sysparm("bp_time");
    //str = model.record(0).value(0).toString();
    if(str == "0")//不分组
    {
        ui->checkBox_68->setChecked(false);
    }
    else
    {
        ui->checkBox_68->setChecked(true);
    }

    str.clear();
    str=n_func::f_get_sysparm("bp_time_len");
    //str = model.record(0).value(0).toString();
    ui->lineEdit_48->setText(str);

    //时长
    str.clear();
    str=n_func::f_get_sysparm("bp_times");
    //str = model.record(0).value(0).toString();
    if(str == "0")//不分组
    {
        ui->checkBox_69->setChecked(false);
    }
    else
    {
        ui->checkBox_69->setChecked(true);
    }

    str.clear();
    str=n_func::f_get_sysparm("bp_times_len");
    //str = model.record(0).value(0).toString();
    ui->lineEdit_56->setText(str);

    //外卖地址
    str.clear();
    str=n_func::f_get_sysparm("bp_address");
    //str = model.record(0).value(0).toString();
    if(str == "0")//不分组
    {
        ui->checkBox_70->setChecked(false);
    }
    else
    {
        ui->checkBox_70->setChecked(true);
    }

    str.clear();
    str=n_func::f_get_sysparm("bp_address_len");
    //str = model.record(0).value(0).toString();
    ui->lineEdit_49->setText(str);

    //外卖人
    str.clear();
    str=n_func::f_get_sysparm("bp_linkman");
    //str = model.record(0).value(0).toString();
    if(str == "0")//不分组
    {
        ui->checkBox_71->setChecked(false);
    }
    else
    {
        ui->checkBox_71->setChecked(true);
    }

    str.clear();
    str=n_func::f_get_sysparm("bp_linkman_len");
    //str = model.record(0).value(0).toString();
    ui->lineEdit_55->setText(str);


    //外卖电话
    str.clear();
    str=n_func::f_get_sysparm("bp_tel");
    //str = model.record(0).value(0).toString();
    if(str == "0")//不分组
    {
        ui->checkBox_72->setChecked(false);
    }
    else
    {
        ui->checkBox_72->setChecked(true);
    }

    str.clear();
    str=n_func::f_get_sysparm("bp_tel_len");
    //str = model.record(0).value(0).toString();
    ui->lineEdit_50->setText(str);

    //服务员
    str.clear();
    str=n_func::f_get_sysparm("bp_waiter");
    //str = model.record(0).value(0).toString();
    if(str == "0")//不分组
    {
        ui->checkBox_73->setChecked(false);
    }
    else
    {
        ui->checkBox_73->setChecked(true);
    }

    str.clear();
    str=n_func::f_get_sysparm("bp_waiter_len");
    //str = model.record(0).value(0).toString();
    ui->lineEdit_54->setText(str);

    //服务员名称
    str.clear();
    str=n_func::f_get_sysparm("bp_waitername");
    //str = model.record(0).value(0).toString();
    if(str == "0")//不分组
    {
        ui->checkBox_74->setChecked(false);
    }
    else
    {
        ui->checkBox_74->setChecked(true);
    }

    str.clear();
    str=n_func::f_get_sysparm("bp_waitername_len");
    //str = model.record(0).value(0).toString();
    ui->lineEdit_51->setText(str);

    //开台备注
    str.clear();
    str=n_func::f_get_sysparm("bp_memo");
    //str = model.record(0).value(0).toString();
    if(str == "0")//不分组
    {
        ui->checkBox_75->setChecked(false);
    }
    else
    {
        ui->checkBox_75->setChecked(true);
    }

    str.clear();
    str=n_func::f_get_sysparm("bp_memo_len");
    //str = model.record(0).value(0).toString();
    ui->lineEdit_53->setText(str);

    //流水号
    str.clear();
    str=n_func::f_get_sysparm("serial_no");
    //str = model.record(0).value(0).toString();
    if(str == "0")//不分组
    {
        ui->checkBox_76->setChecked(false);
    }
    else
    {
        ui->checkBox_76->setChecked(true);
    }

    str.clear();
    str=n_func::f_get_sysparm("serial_no_len");
    //str = model.record(0).value(0).toString();
    ui->lineEdit_52->setText(str);

    //表尾每行打印列数
    model.clear();
    str.clear();
    str=n_func::f_get_sysparm("bp_offcols");
    //str = model.record(0).value(0).toString();
    ui->lineEdit_33->setText(str);

    //付款详情
    str.clear();
    str = configIniRead->value("system_setting/fukuanxiangqing").toString();
    if(str == "1")
    {
        ui->checkBox_77->setChecked(true);
    }
    else
    {
        ui->checkBox_77->setChecked(false);
    }

    //消费金额
    model.clear();
    str.clear();
    str=n_func::f_get_sysparm("bp_cost");
    //str = model.record(0).value(0).toString();
    if(str == "1")
    {
        ui->checkBox_78->setChecked(true);
    }
    else
    {
        ui->checkBox_78->setChecked(false);
    }

    //折扣金额
    model.clear();
    str.clear();
    str=n_func::f_get_sysparm("bp_discount");
    //str = model.record(0).value(0).toString();
    if(str == "1")
    {
        ui->checkBox_80->setChecked(true);
    }
    else
    {
        ui->checkBox_80->setChecked(false);
    }

    //赠送金额
    model.clear();
    str.clear();
    str=n_func::f_get_sysparm("bp_present");
    //str = model.record(0).value(0).toString();
    if(str == "1")
    {
        ui->checkBox_79->setChecked(true);
    }
    else
    {
        ui->checkBox_79->setChecked(false);
    }

    //服务费
    model.clear();
    str.clear();
    str=n_func::f_get_sysparm("bp_service");
    //str = model.record(0).value(0).toString();
    if(str == "1")
    {
        ui->checkBox_82->setChecked(true);
    }
    else
    {
        ui->checkBox_82->setChecked(false);
    }

    //税收金额
    str.clear();
    str = configIniRead->value("system_setting/shuishoujine").toString();
    if(str == "1")
    {
        ui->checkBox_81->setChecked(true);
    }
    else
    {
        ui->checkBox_81->setChecked(false);
    }

    //低消差
    model.clear();
    str.clear();
    str=n_func::f_get_sysparm("bp_lowcost");
    //str = model.record(0).value(0).toString();
    if(str == "1")
    {
        ui->checkBox_87->setChecked(true);
    }
    else
    {
        ui->checkBox_87->setChecked(false);
    }

    //抹零
    model.clear();
    str.clear();
    str=n_func::f_get_sysparm("bp_blotout");
    //str = model.record(0).value(0).toString();
    if(str == "1")
    {
        ui->checkBox_86->setChecked(true);
    }
    else
    {
        ui->checkBox_86->setChecked(false);
    }

    //免单
    model.clear();
    str.clear();
    str=n_func::f_get_sysparm("bp_free");
    //str = model.record(0).value(0).toString();
    if(str == "1")
    {
        ui->checkBox_84->setChecked(true);
    }
    else
    {
        ui->checkBox_84->setChecked(false);
    }

    //应收
    model.clear();
    str.clear();
    str=n_func::f_get_sysparm("bp_receive");
    //str = model.record(0).value(0).toString();
    if(str == "1")
    {
        ui->checkBox_83->setChecked(true);
    }
    else
    {
        ui->checkBox_83->setChecked(false);
    }

    //人均
    model.clear();
    str.clear();
    str=n_func::f_get_sysparm("bp_avg");
    //str = model.record(0).value(0).toString();
    if(str == "1")
    {
        ui->checkBox_85->setChecked(true);
    }
    else
    {
        ui->checkBox_85->setChecked(false);
    }

    //实收
    model.clear();
    str.clear();
    str=n_func::f_get_sysparm("bp_cash");
    //str = model.record(0).value(0).toString();
    if(str == "1")
    {
        ui->checkBox_88->setChecked(true);
    }
    else
    {
        ui->checkBox_88->setChecked(false);
    }

    //会员名称
    model.clear();
    str.clear();
    str=n_func::f_get_sysparm("bp_membername");
    //str = model.record(0).value(0).toString();
    if(str == "1")
    {
        ui->checkBox_93->setChecked(true);
    }
    else
    {
        ui->checkBox_93->setChecked(false);
    }

    //会员类型
    model.clear();
    str.clear();
    str=n_func::f_get_sysparm("bp_membertype");
    //str = model.record(0).value(0).toString();
    if(str == "1")
    {
        ui->checkBox_92->setChecked(true);
    }
    else
    {
        ui->checkBox_92->setChecked(false);
    }

    //找零
    str.clear();
    str = configIniRead->value("system_setting/zhaolingjine").toString();
    if(str == "1")
    {
        ui->checkBox_90->setChecked(true);
    }
    else
    {
        ui->checkBox_90->setChecked(false);
    }

    //会员打印余额
    model.clear();
    str.clear();
    str=n_func::f_get_sysparm("print_ye");
    //str = model.record(0).value(0).toString();
    if(str == "1")
    {
        ui->checkBox_89->setChecked(true);
    }
    else
    {
        ui->checkBox_89->setChecked(false);
    }

    //会员积分
    model.clear();
    str.clear();
    str=n_func::f_get_sysparm("print_vip_point");
    //str = model.record(0).value(0).toString();
    if(str == "1")
    {
        ui->checkBox_91->setChecked(true);
    }
    else
    {
        ui->checkBox_91->setChecked(false);
    }

    //签名
    model.clear();
    str.clear();
    str=n_func::f_get_sysparm("bp_underwrite");
    //str = model.record(0).value(0).toString();
    if(str == "1")
    {
        ui->checkBox_94->setChecked(true);
    }
    else
    {
        ui->checkBox_94->setChecked(false);
    }

    //尾语
    str.clear();
    str = configIniRead->value("system_setting/jieweiyu").toString();
    ui->lineEdit_34->setText(str);
}

void system_setting::init_tab4()
{
    ui->lineEdit_bp_amount_left->setText(n_func::f_get_sysparm("bp_amount_left"));
    ui->lineEdit_bp_discount_left->setText(n_func::f_get_sysparm("bp_discount_left"));
    ui->lineEdit_bp_dishname_left->setText(n_func::f_get_sysparm("bp_dishname_left"));
    ui->lineEdit_bp_num_left->setText(n_func::f_get_sysparm("bp_num_left"));
    ui->lineEdit_bp_price_left->setText(n_func::f_get_sysparm("bp_price_left"));
    ui->lineEdit_bp_unit_left->setText(n_func::f_get_sysparm("bp_unit_left"));

    ui->lineEdit_bp_content_len->setText(n_func::f_get_sysparm("bp_content_len"));

    QSqlQueryModel model;
    QString str;
    //打印方式
    QPixmap pixmap(1, 40);
    pixmap.fill(Qt::transparent);
    QIcon icon(pixmap);
    //账单连续打印
    ui->comboBox_6->addItems(QStringList() << tr("明细打印") << tr("汇总打印"));

    str=n_func::f_get_sysparm("bp_print_mode");
    //str = model.record(0).value(0).toString();
    if(str == "1")
    {
        ui->comboBox_6->setCurrentIndex(0);
    }
    else
    {
        ui->comboBox_6->setCurrentIndex(1);
    }

    //列表头内容，本地文件保存
    str.clear();
    str = configIniRead->value("system_setting/biaoneirong").toString();
    ui->lineEdit_2->setText(str);
    //菜品按大类分组
    model.clear();
    str.clear();
    str=n_func::f_get_sysparm("bp_series_group");
    //str = model.record(0).value(0).toString();
    if(str == "0")//不分组
    {
        ui->checkBox_19->setChecked(false);
    }
    else
    {
        ui->checkBox_19->setChecked(true);
    }

    //本单序号
    model.clear();
    str.clear();
    str=n_func::f_get_sysparm("bp_rowid");
    //str = model.record(0).value(0).toString();
    if(str == "0")
    {
        ui->checkBox_20->setChecked(false);
    }
    else
    {
        ui->checkBox_20->setChecked(true);
    }

    //赠送标志
    model.clear();
    str.clear();
    str=n_func::f_get_sysparm("bp_presentflag");
    //str = model.record(0).value(0).toString();
    if(str == "0")
    {
        ui->checkBox_21->setChecked(false);
    }
    else
    {
        ui->checkBox_21->setChecked(true);
    }

    //特价标志
    model.clear();
    str.clear();
    str=n_func::f_get_sysparm("bp_specialflag");
    //str = model.record(0).value(0).toString();
    if(str == "0")
    {
        ui->checkBox_22->setChecked(false);
    }
    else
    {
        ui->checkBox_22->setChecked(true);
    }

    //超出长度自动裁剪
    model.clear();
    str.clear();
    str=n_func::f_get_sysparm("bp_overstep_cut");
    //str = model.record(0).value(0).toString();
    if(str == "0")
    {
        ui->checkBox_23->setChecked(false);
    }
    else
    {
        ui->checkBox_23->setChecked(true);
    }

    //品名设置
    model.clear();
    str.clear();
    str=n_func::f_get_sysparm("bp_dishname");
    //str = model.record(0).value(0).toString();
    if(str == "0")
    {
        ui->checkBox_24->setChecked(false);
    }
    else
    {
        ui->checkBox_24->setChecked(true);
    }

    model.clear();
    str.clear();
    str=n_func::f_get_sysparm("bp_dishname_len");
    //str = model.record(0).value(0).toString();
    ui->lineEdit_3->setText(str);

    ui->comboBox_7->addItems(QStringList() << ("1") << ("2"));

    model.clear();
    str.clear();
    str=n_func::f_get_sysparm("bp_dishname_row");
    //str = model.record(0).value(0).toString();
    for(int i = 0; i < ui->comboBox_7->count(); i++)
    {
        if(ui->comboBox_7->itemText(i) == str)
        {
            ui->comboBox_7->setCurrentIndex(i);
        }
    }

    //单位设置
    model.clear();
    str.clear();
    str=n_func::f_get_sysparm("bp_unit");
    //str = model.record(0).value(0).toString();
    if(str == "0")
    {
        ui->checkBox_31->setChecked(false);
    }
    else
    {
        ui->checkBox_31->setChecked(true);
    }

    model.clear();
    str.clear();
    str=n_func::f_get_sysparm("bp_unit_len");
    //str = model.record(0).value(0).toString();
    ui->lineEdit_5->setText(str);
    ui->comboBox_9->addItems(QStringList() << ("1") << ("2"));


    model.clear();
    str.clear();
    str=n_func::f_get_sysparm("bp_unit_row");
    //str = model.record(0).value(0).toString();
    for(int i = 0; i < ui->comboBox_9->count(); i++)
    {
        if(ui->comboBox_9->itemText(i) == str)
        {
            ui->comboBox_9->setCurrentIndex(i);
        }
    }

    //数量设置
    model.clear();
    str.clear();
    str=n_func::f_get_sysparm("bp_num");
    //str = model.record(0).value(0).toString();
    if(str == "0")
    {
        ui->checkBox_32->setChecked(false);
    }
    else
    {
        ui->checkBox_32->setChecked(true);
    }

    model.clear();
    str.clear();
    str=n_func::f_get_sysparm("bp_num_len");
    //str = model.record(0).value(0).toString();
    ui->lineEdit_6->setText(str);
    ui->comboBox_10->addItems(QStringList() << ("1") << ("2"));

    model.clear();
    str.clear();
    str=n_func::f_get_sysparm("bp_num_row");
    //str = model.record(0).value(0).toString();
    for(int i = 0; i < ui->comboBox_10->count(); i++)
    {
        if(ui->comboBox_10->itemText(i) == str)
        {
            ui->comboBox_10->setCurrentIndex(i);
        }
    }

    //价格设置
    model.clear();
    str.clear();
    str=n_func::f_get_sysparm("bp_price");
    //str = model.record(0).value(0).toString();
    if(str == "0")
    {
        ui->checkBox_41->setChecked(false);
    }
    else
    {
        ui->checkBox_41->setChecked(true);
    }

    model.clear();
    str.clear();
    str=n_func::f_get_sysparm("bp_price_len");
    //str = model.record(0).value(0).toString();
    ui->lineEdit_10->setText(str);

    ui->comboBox_17->addItems(QStringList() << ("1") << ("2"));

    model.clear();
    str.clear();
    str=n_func::f_get_sysparm("bp_price_row");
    //str = model.record(0).value(0).toString();
    for(int i = 0; i < ui->comboBox_17->count(); i++)
    {
        if(ui->comboBox_17->itemText(i) == str)
        {
            ui->comboBox_17->setCurrentIndex(i);
        }
    }


    //金额设置
    model.clear();
    str.clear();
    str=n_func::f_get_sysparm("bp_amount");
    //str = model.record(0).value(0).toString();
    if(str == "0")
    {
        ui->checkBox_42->setChecked(false);
    }
    else
    {
        ui->checkBox_42->setChecked(true);
    }

    model.clear();
    str.clear();
    str=n_func::f_get_sysparm("bp_amount_len");
    //str = model.record(0).value(0).toString();
    ui->lineEdit_11->setText(str);

    ui->comboBox_19->addItems(QStringList() << ("1") << ("2"));

    model.clear();
    str.clear();
    str=n_func::f_get_sysparm("bp_amount_row");
    //str = model.record(0).value(0).toString();
    for(int i = 0; i < ui->comboBox_19->count(); i++)
    {
        if(ui->comboBox_19->itemText(i) == str)
        {
            ui->comboBox_19->setCurrentIndex(i);
        }
    }

    //折扣设置
    model.clear();
    str.clear();
    str=n_func::f_get_sysparm("bp_disdetail");
    //str = model.record(0).value(0).toString();
    if(str == "0")
    {
        ui->checkBox_43->setChecked(false);
    }
    else
    {
        ui->checkBox_43->setChecked(true);
    }

    model.clear();
    str.clear();
    str=n_func::f_get_sysparm("bp_disdetail_len");
    //str = model.record(0).value(0).toString();
    ui->lineEdit_12->setText(str);

    ui->comboBox_21->addItems(QStringList() << ("1") << ("2"));

    model.clear();
    str.clear();
    str=n_func::f_get_sysparm("bp_disdetail_row");
    //str = model.record(0).value(0).toString();
    for(int i = 0; i < ui->comboBox_21->count(); i++)
    {
        if(ui->comboBox_21->itemText(i) == str)
        {
            ui->comboBox_21->setCurrentIndex(i);
        }
    }

    //做法说明
    model.clear();
    str.clear();
    str=n_func::f_get_sysparm("bp_printmemo");
    //str = model.record(0).value(0).toString();
    if(str == "0")
    {
        ui->checkBox_44->setChecked(false);
    }
    else
    {
        ui->checkBox_44->setChecked(true);
    }
}

void system_setting::init_tab5()
{

    QString str;

    QPixmap pixmap(1, 40);
    pixmap.fill(Qt::transparent);
    QIcon icon(pixmap);
    //点菜异常记录
    str = configIniRead->value("system_setting/diancaiyichang").toString();
    if(str == "1")
    {
        ui->checkBox_47->setChecked(true);
    }
    else
    {
        ui->checkBox_47->setChecked(false);
    }

    //打印LOGO
    str.clear();
    str = configIniRead->value("system_setting/dayinlogo").toString();
    if(str == "1")
    {
        ui->checkBox_48->setChecked(true);
    }
    else
    {
        ui->checkBox_48->setChecked(false);
    }

    str.clear();
    str = configIniRead->value("system_setting/dayinlogoneirong").toString();
    ui->lineEdit_15->setText(str);
    ui->lineEdit_15->setEnabled(false);

    //打印一维码
    str.clear();
    str = configIniRead->value("system_setting/yiweima").toString();
    if(str == "1")
    {
        ui->checkBox_49->setChecked(true);
    }
    else
    {
        ui->checkBox_49->setChecked(false);
    }

    str.clear();
    str = configIniRead->value("system_setting/yiweimaneirong").toString();
    ui->lineEdit_16->setText(str);
    if(ui->checkBox_49->isChecked() == false)
    {
        ui->lineEdit_16->setEnabled(false);
    }

    //打印二维码
    str.clear();
    str = configIniRead->value("system_setting/erweima").toString();
    if(str == "1")
    {
        ui->checkBox_50->setChecked(true);
    }
    else
    {
        ui->checkBox_50->setChecked(false);
    }

    str.clear();
    str = configIniRead->value("system_setting/erweimaneirong").toString();
    ui->lineEdit_17->setText(str);
    if(ui->checkBox_50->isChecked() == false)
    {
        ui->lineEdit_17->setEnabled(false);
        ui->lineEdit_18->setEnabled(false);
    }

    ui->comboBox_48->setIconSize(QSize(1,40));
    QStringList strlist;
    strlist << "1" << "2" << "3" << "4" << "5" << "6" << "7" << "8"
            << "9" << "10" << "11" << "12" << "13" << "14" << "15" << "16";
    ui->comboBox_48->addItems(strlist);

    str.clear();
    str = configIniRead->value("system_setting/erweiziti").toString();
    for(int i = 0; i < ui->comboBox_48->count(); i++)
    {
        if(ui->comboBox_48->itemText(i) == str)
        {
            ui->comboBox_48->setCurrentIndex(i);
        }
    }

    str.clear();
    str = configIniRead->value("system_setting/erweimatishi").toString();
    ui->lineEdit_18->setText(str);

    //支付二维码配置
    str.clear();
    str = configIniRead->value("system_setting/dayinjiezhangma").toString();
    if(str == "1")
    {
        ui->checkBox_51->setChecked(true);
    }
    else
    {
        ui->checkBox_51->setChecked(false);
    }

    str.clear();
    str = configIniRead->value("system_setting/gongzhongid").toString();
    ui->lineEdit_19->setText(str);

    str.clear();
    str = configIniRead->value("system_setting/shanghuid").toString();
    ui->lineEdit_20->setText(str);

    str.clear();
    str = configIniRead->value("system_setting/zhifumiyao").toString();
    ui->lineEdit_35->setText(str);
    if(ui->checkBox_51->isChecked() == false)
    {
        ui->lineEdit_19->setEnabled(false);
        ui->lineEdit_20->setEnabled(false);
        ui->lineEdit_35->setEnabled(false);
    }

    //设置厨打漏单IP
    str.clear();
    str = configIniRead->value("system_setting/cdloudanip").toString();
    if(str == "1")
    {
        ui->checkBox_52->setChecked(true);
    }
    else
    {
        ui->checkBox_52->setChecked(false);
    }

    str.clear();
    str = configIniRead->value("system_setting/chudaloudaip").toString();
    ui->lineEdit_36->setText(str);
    if(ui->checkBox_52->isChecked() == false)
    {
        ui->lineEdit_36->setEnabled(false);
    }


    str.clear();
    str = configIniRead->value("system_setting/qingkonglouda").toString();
    if(str == "1")
    {
        ui->checkBox_53->setChecked(true);
    }
    else
    {
        ui->checkBox_53->setChecked(false);
    }
}

void system_setting::init_tab6()
{

    QSqlQueryModel model;
    QString str;
    return;
    //抬头
    str.clear();
    model.clear();
    str=n_func::f_get_sysparm("label_title");
    str = model.record(0).value(0).toString().trimmed();
    if(str == "1")
    {
        ui->checkBox_28->setChecked(true);
    }
    else
    {
        ui->checkBox_28->setChecked(false);
    }

    //标签品名
    str.clear();
    model.clear();
    str=n_func::f_get_sysparm("label_dishname");
    str = model.record(0).value(0).toString().trimmed();
    if(str == "1")
    {
        ui->checkBox_29->setChecked(true);
    }
    else
    {
        ui->checkBox_29->setChecked(false);
    }

    //标签单号
    str.clear();
    model.clear();
    str=n_func::f_get_sysparm("label_billno");
    str = model.record(0).value(0).toString().trimmed();
    if(str == "1")
    {
        ui->checkBox_30->setChecked(true);
    }
    else
    {
        ui->checkBox_30->setChecked(false);
    }

    //标签时间
    str.clear();
    model.clear();
    str=n_func::f_get_sysparm("label_time");
    str = model.record(0).value(0).toString().trimmed();
    if(str == "1")
    {
        ui->checkBox_33->setChecked(true);
    }
    else
    {
        ui->checkBox_33->setChecked(false);
    }

    //标签价格
    str.clear();
    model.clear();
    str=n_func::f_get_sysparm("label_price");
    str = model.record(0).value(0).toString().trimmed();
    if(str == "1")
    {
        ui->checkBox_34->setChecked(true);
    }
    else
    {
        ui->checkBox_34->setChecked(false);
    }

    //标签做法
    str.clear();
    model.clear();
    str=n_func::f_get_sysparm("label_memo");
    str = model.record(0).value(0).toString().trimmed();
    if(str == "1")
    {
        ui->checkBox_35->setChecked(true);
    }
    else
    {
        ui->checkBox_35->setChecked(false);
    }

    //标签电话
    str.clear();
    model.clear();
    str=n_func::f_get_sysparm("label_tel");
    str = model.record(0).value(0).toString().trimmed();
    if(str == "1")
    {
        ui->checkBox_36->setChecked(true);
    }
    else
    {
        ui->checkBox_36->setChecked(false);
    }
    //标签标题
    str.clear();
    model.clear();
    str=n_func::f_get_sysparm("label_title1");
    str = model.record(0).value(0).toString().trimmed();
    if(str == "1")
    {
        ui->checkBox_37->setChecked(true);
    }
    else
    {
        ui->checkBox_37->setChecked(false);
    }

    //标签地址
    str.clear();
    model.clear();
    str=n_func::f_get_sysparm("label_address");
    str = model.record(0).value(0).toString().trimmed();
    if(str == "1")
    {
        ui->checkBox_38->setChecked(true);
    }
    else
    {
        ui->checkBox_38->setChecked(false);
    }
}

void system_setting::init_tab7()
{
    //是否输入手牌号
    ui->checkBox_250->setChecked(configIniRead->value("system_setting/fukuanshishurushoupai").toBool());
    //做法是否弹出
    ui->checkBox_251->setChecked(configIniRead->value("system_setting/cook_info_pop").toBool());
    //流水号最大值
    ui->lineEdit_37->setText(n_func::f_get_sysparm("serial_no_end"));
    //流水号最小值
    ui->lineEdit_8->setText(n_func::f_get_sysparm("serial_no_begin"));
    //偶数杯半价
    ui->checkBox_252->setChecked(configIniRead->value("system_setting/even_halfprice").toBool());
    ui->checkBox_252->hide();
    //前台是否显示菜品图片
    ui->checkBox_253->setChecked(configIniRead->value("system_setting/is_show_dishpic").toBool());
    //前台只显示大类
    ui->checkBox_255->setChecked(configIniRead->value("system_setting/only_dalei").toBool());
    //点菜图片大小
    ui->lineEdit_21->setText(configIniRead->value("system_setting/qt_iconsize", "5x4").toString());
    //不显示小票抬头
    ui->checkBox_254->setChecked(configIniRead->value("system_setting/not_show_bill_title").toBool());

}

void system_setting::init_tab8()
{
    ui->lineEdit_itemcode->setText(configIniRead->value("system_setting/njgj_itemcode").toString());
    ui->lineEdit_storecode->setText(configIniRead->value("system_setting/njgj_storecode").toString());
    ui->lineEdit_ip->setText(configIniRead->value("system_setting/njgj_ip").toString());
    ui->lineEdit_license_agreement->setText(configIniRead->value("system_setting/njgj_license_agreement").toString());
    ui->lineEdit_mallid->setText(configIniRead->value("system_setting/njgj_mallid").toString());
    ui->lineEdit_port->setText(configIniRead->value("system_setting/njgj_port").toString());
    ui->lineEdit_password->setText(configIniRead->value("system_setting/njgj_password").toString());
    ui->lineEdit_username->setText(configIniRead->value("system_setting/njgj_username").toString());
    ui->lineEdit_web->setText(configIniRead->value("system_setting/njgj_web").toString());
    ui->checkBox_use->setChecked(configIniRead->value("system_setting/njgj_use").toBool());
    ui->checkBox_ftptp_use->setChecked(configIniRead->value("system_setting/ftptp_use").toBool());
    ui->lineEdit_ftptp_storecode->setText(configIniRead->value("system_setting/ftptp_storecode").toString());
    ui->lineEdit_ftptp_plu->setText(configIniRead->value("system_setting/ftptp_plu").toString());
    ui->lineEdit_ftptp_usr->setText(configIniRead->value("system_setting/ftptp_usr").toString());
    ui->lineEdit_ftptp_address->setText(configIniRead->value("system_setting/ftptp_address").toString());
    ui->lineEdit_ftptp_pwd->setText(configIniRead->value("system_setting/ftptp_pwd").toString());
    ui->lineEdit_41->setText(configIniRead->value("system_setting/doublescreen_timeout", 200).toString());
    ui->lineEdit_7->setText(configIniRead->value("system_setting/kitchen_timeout", 200).toString());

}

QString system_setting::get_max_serialno_add1(const QString &danhao)
{
    //min:1-max3
    //serial:0-1 1-2 2-3 4-0 5-1
    // 2-5
    //0-2 1-3 2-4 3-5 4-2 [serial%end-1+begin]0/4+2
    int serial=danhao.right(4).toInt();
    //    int serial=n_func::f_get_sysparm("serial_no_now").toInt();
    int begin=n_func::f_get_sysparm("serial_no_begin").toInt();
    int end=n_func::f_get_sysparm("serial_no_end").toInt();
    //    serial++;
    serial=(serial)%(end+1)+begin;
    //    if(isupdate){
    //        n_func::f_set_sysparm("serial_no_now", QString::number(serial), "");
    //    }

    return QString::number(serial);
}

void system_setting::widget_blotout_round(bool flag)
{
    ui->widget_blotout->setEnabled(flag);
    ui->widget_round->setEnabled(!flag);
}

void system_setting::on_checkBox_10_clicked()
{
    if(ui->checkBox_10->isChecked() == true)
    {
        ui->lineEdit->setEnabled(true);
    }
    else
    {
        ui->lineEdit->setEnabled(false);
    }
}

void system_setting::on_pushButton_4_clicked()//确定
{
    btn_t_f = true;

    QSqlQuery query;
    QSqlDatabase::database().transaction();
    QString str;
    //save tab1
    //抹零方式设置
    if(ui->radioButton->isChecked() == true)
    {
        str = "1";
    }
    else
    {
        str = "2";
    }

    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='blotout_round'")
                  .arg(str)))
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

        return;
    }

    //抹零设置
    str.clear();
    if(ui->radioButton_3->isChecked() == true)
    {
        str = "1";
    }
    else if(ui->radioButton_4->isChecked() == true)
    {
        str = "2";
    }
    else if(ui->radioButton_5->isChecked() == true)
    {
        str = "3";
    }
    else
    {
        str = "4";
    }
    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='blotout_mode'")
                  .arg(str)))
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

        return;
    }

    //四舍五入设置
    str.clear();
    if(ui->radioButton_7->isChecked() == true)
    {
        str = "1";
    }
    else if(ui->radioButton_8->isChecked() == true)
    {
        str = "2";
    }
    else if(ui->radioButton_9->isChecked() == true)
    {
        str = "3";
    }
    else
    {
        str = "4";
    }
    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='round_mode'")
                  .arg(str)))
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

        return;
    }

    //买单打印账单
    str.clear();
    if(ui->checkBox->isChecked() == true)
    {
        str = "1";
    }
    else
    {
        str = tr(0);
    }
    configIniWrite->setValue("system_setting/maidanzhangdan",str);

    //买单账单不显示明细
    str.clear();
    if(ui->checkBox_2->isChecked() == true)
    {
        str = "0";
    }
    else
    {
        str = "1";
    }
    configIniWrite->setValue("system_setting/maidanmingxi",str);

    //账单连续打印份数
    str.clear();
    str = ui->comboBox_2->currentText();
    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='bp_count'")
                  .arg(str)))
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

        return;
    }

    //卖外账单连续打印份数
    str.clear();
    str = ui->comboBox_3->currentText();
    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='bp_togo_count'")
                  .arg(str)))
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

        return;
    }

    //交班必须接班人
    str.clear();
    if(ui->checkBox_3->isChecked() == true)
    {
        str = "1";
    }
    else
    {
        str = "0";
    }
    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='handover_oper'")
                  .arg(str)))
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

        return;
    }

    //触摸屏退出系统为重新登录
    str.clear();
    if(ui->checkBox_4->isChecked() == true)
    {
        str = "1";
    }
    else
    {
        str = tr(0);
    }
    configIniWrite->setValue("system_setting/chongxidenglu",str);

    //默认现金付全款
    str.clear();
    if(ui->checkBox_5->isChecked() == true)
    {
        str = "1";
    }
    else
    {
        str = "0";
    }
    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='cash_payall'")
                  .arg(str)))
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

        return;
    }

    //手工处理服务费
    str.clear();
    if(ui->checkBox_6->isChecked() == true)
    {
        str = "1";
    }
    else
    {
        str = "0";
    }
    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='service_handwork'")
                  .arg(str)))
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

        return;
    }

    //点菜数量不允许为负
    str.clear();
    if(ui->checkBox_7->isChecked() == true)
    {
        str = "1";
    }
    else
    {
        str = "0";
    }
    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='negative'")
                  .arg(str)))
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

        return;
    }

    //服务费计入低消费
    str.clear();
    if(ui->checkBox_8->isChecked() == true)
    {
        str = "1";
    }
    else
    {
        str = "0";
    }
    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='low_service'")
                  .arg(str)))
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

        return;
    }

    //退菜必须输入退菜原因
    str.clear();
    if(ui->checkBox_9->isChecked() == true)
    {
        str = "1";
    }
    else
    {
        str = "0";
    }
    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='orderdish_back'")
                  .arg(str)))
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

        return;
    }

    //快餐必须输入台号
    str.clear();
    if(ui->checkBox_11->isChecked() == true)
    {
        str = "1";
    }
    else
    {
        str = "0";
    }
    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='shortorder_tableno'")
                  .arg(str)))
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

        return;
    }

    //外卖落单立即结帐
    str.clear();
    if(ui->checkBox_12->isChecked() == true)
    {
        str = "1";
    }
    else
    {
        str = "0";
    }
    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='togo_pay_atonce'")
                  .arg(str)))
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

        return;
    }

    //账单自动折扣
    str.clear();
    if(ui->checkBox_10->isChecked() == true)
    {
        str = "1";
    }
    else
    {
        str = "0";
    }
    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='pay_auto_discount'")
                  .arg(str)))
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

        return;
    }

    //折扣值
    str.clear();
    str = ui->lineEdit->text();
    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='pay_auto_discount_value'")
                  .arg(str)))
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

        return;
    }

    //点菜后自动录入数量
    str.clear();
    if(ui->checkBox_14->isChecked() == true)
    {
        str = "1";
    }
    else
    {
        str = "0";
    }
    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='ordertonum'")
                  .arg(str)))
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

        return;
    }

    //抹零默认当前操作员
    str.clear();
    if(ui->checkBox_15->isChecked() == true)
    {
        str = "1";
    }
    else
    {
        str = "0";
    }
    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='bolt_operator'")
                  .arg(str)))
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

        return;
    }

    //外卖落单进行厨打
    str.clear();
    if(ui->checkBox_13->isChecked() == true)
    {
        str = "1";
    }
    else
    {
        str = tr(0);
    }
    configIniWrite->setValue("system_setting/waimailuodanchuda",str);

    //奶茶模式
    configIniWrite->setValue("system_setting/naichamoshi",ui->checkBox_17->isChecked());

    //本机落单打印总单
    str.clear();
    if(ui->checkBox_18->isChecked() == true)
    {
        str = "1";
    }
    else
    {
        str = tr(0);
    }
    configIniWrite->setValue("system_setting/luodanzongdan",str);

    //营业模式
    str.clear();
    if(ui->comboBox->currentIndex() == 0)
    {
        str = "0";
    }
    else
    {
        str = "1";
    }
    configIniWrite->setValue("system_setting/yingyemoshi",str);

    //触屏选台方式
    str.clear();
    if(ui->comboBox_4->currentIndex() == 0)
    {
        str = "0";
    }
    else
    {
        str = "1";
    }
    configIniWrite->setValue("system_setting/xuantaifangshi",str);

    //选菜方式
    str.clear();
    if(ui->comboBox_5->currentIndex() == 0)
    {
        str = "0";
    }
    else if(ui->comboBox_5->currentIndex() == 1)
    {
        str = "1";
    }
    else
    {
        str = "2";
    }
    configIniWrite->setValue("system_setting/xuancaifangshi",str);
    //锁屏解锁必须为系统登录员工//本地
    str.clear();
    if(ui->checkBox_16->isChecked() == true)
    {
        str = "1";
    }
    else
    {
        str = tr(0);
    }
    configIniWrite->setValue("system_setting/jiesuoyuangong",str);

    //save tab4
    //打印机接口类型
    configIniWrite->setValue("system_setting/printerpath",ui->comboBox_31->currentText());
    configIniWrite->setValue("system_setting/printertype",ui->comboBox_30->currentText());

    //结帐打开钱箱
    configIniWrite->setValue("system_setting/qianxiang",ui->checkBox_39->isChecked());
    //指令
    configIniWrite->setValue("system_setting/qianxiangzhiling",ui->lineEdit_9->text());

    //客显类型
    configIniWrite->setValue("system_setting/kexianleixing",ui->comboBox_32->currentText());

    //客显端口
    configIniWrite->setValue("system_setting/kexianduankou",ui->comboBox_45->currentText());
    configIniWrite->setValue("system_setting/kexian_vfd_start",ui->lineEdit_38->text());

    //电子称型号
    configIniWrite->setValue("system_setting/dianzichengxinghao",ui->comboBox_47->currentText());
    //电子称端口
    configIniWrite->setValue("system_setting/dianzichengduankou",ui->comboBox_46->currentText());
    //电子称波特率
    configIniWrite->setValue("system_setting/dianzicheng_baudrate",ui->comboBox_49->currentText());

    //是否副屏显示
    configIniWrite->setValue("system_setting/fupingxianshi",ui->checkBox_40->isChecked());
    //显示广告
    configIniWrite->setValue("system_setting/guanggaolujing",ui->lineEdit_13->text());

    //菜品图片循环显示
    configIniWrite->setValue("system_setting/bofanglujing",ui->lineEdit_14->text());
    //菜品滚动字幕
    configIniWrite->setValue("system_setting/bofangg_runningtext",ui->lineEdit_40->text());

    str.clear();
    switch(ui->comboBox_97->currentIndex()){
    case 0:
        n_func::gs_print_line="--------------------------------";
        break;
    case 1:
        n_func::gs_print_line="------------------------------------------------";
        break;
    }
    configIniWrite->setValue("system_setting/dayinjixinghao",ui->comboBox_97->currentIndex());

    //save tab3
    //账单标题
    str.clear();
    str = ui->lineEdit_4->text();
    configIniWrite->setValue("system_setting/zhangdanbiaoti",str);

    //表头设置
    configIniWrite->setValue("system_setting/biaotoushezhi",ui->checkBox_25->isChecked());

    //付款号
    if(ui->checkBox_26->isChecked() == true)
    {
        str = "1";
    }
    else
    {
        str = "0";
    }

    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='bp_payno'")
                  .arg(str)))
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

        return;
    }

    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='bp_payno_len'")
                  .arg(ui->lineEdit_46->text())))
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

        return;
    }

    //    str.clear();
    //    str = ui->comboBox_8->currentText();
    //    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='bp_payno_row'")
    //                  .arg(str)))
    //    {

    //    }
    //    else
    //    {
    //        QSqlDatabase::database().rollback();
    //        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

    //        return;
    //    }

    //工号
    if(ui->checkBox_65->isChecked() == true)
    {
        str = "1";
    }
    else
    {
        str = "0";
    }

    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='bp_operid'")
                  .arg(str)))
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

        return;
    }

    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='bp_operid_len'")
                  .arg(ui->lineEdit_58->text())))
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

        return;
    }

    //    str.clear();
    //    str = ui->comboBox_33->currentText();
    //    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='bp_operid_row'")
    //                  .arg(str)))
    //    {

    //    }
    //    else
    //    {
    //        QSqlDatabase::database().rollback();
    //        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

    //        return;
    //    }

    //餐桌
    if(ui->checkBox_66->isChecked() == true)
    {
        str = "1";
    }
    else
    {
        str = "0";
    }

    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='bp_table'")
                  .arg(str)))
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

        return;
    }

    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='bp_table_len'")
                  .arg(ui->lineEdit_47->text())))
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

        return;
    }

    //    str.clear();
    //    str = ui->comboBox_34->currentText();
    //    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='bp_table_row'")
    //                  .arg(str)))
    //    {

    //    }
    //    else
    //    {
    //        QSqlDatabase::database().rollback();
    //        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

    //        return;
    //    }

    //人数
    if(ui->checkBox_67->isChecked() == true)
    {
        str = "1";
    }
    else
    {
        str = "0";
    }

    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='bp_person'")
                  .arg(str)))
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

        return;
    }

    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='bp_person_len'")
                  .arg(ui->lineEdit_57->text())))
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

        return;
    }

    //    str.clear();
    //    str = ui->comboBox_35->currentText();
    //    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='bp_person_row'")
    //                  .arg(str)))
    //    {

    //    }
    //    else
    //    {
    //        QSqlDatabase::database().rollback();
    //        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

    //        return;
    //    }

    //时间
    if(ui->checkBox_68->isChecked() == true)
    {
        str = "1";
    }
    else
    {
        str = "0";
    }

    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='bp_time'")
                  .arg(str)))
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

        return;
    }

    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='bp_time_len'")
                  .arg(ui->lineEdit_48->text())))
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

        return;
    }

    //    str.clear();
    //    str = ui->comboBox_36->currentText();
    //    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='bp_time_row'")
    //                  .arg(str)))
    //    {

    //    }
    //    else
    //    {
    //        QSqlDatabase::database().rollback();
    //        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

    //        return;
    //    }

    //时长
    if(ui->checkBox_69->isChecked() == true)
    {
        str = "1";
    }
    else
    {
        str = "0";
    }

    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='bp_times'")
                  .arg(str)))
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

        return;
    }

    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='bp_times_len'")
                  .arg(ui->lineEdit_56->text())))
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

        return;
    }

    //    str.clear();
    //    str = ui->comboBox_37->currentText();
    //    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='bp_times_row'")
    //                  .arg(str)))
    //    {

    //    }
    //    else
    //    {
    //        QSqlDatabase::database().rollback();
    //        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

    //        return;
    //    }

    //外卖地址
    if(ui->checkBox_70->isChecked() == true)
    {
        str = "1";
    }
    else
    {
        str = "0";
    }

    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='bp_address'")
                  .arg(str)))
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

        return;
    }

    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='bp_address_len'")
                  .arg(ui->lineEdit_49->text())))
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

        return;
    }

    //    str.clear();
    //    str = ui->comboBox_38->currentText();
    //    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='bp_address_row'")
    //                  .arg(str)))
    //    {

    //    }
    //    else
    //    {
    //        QSqlDatabase::database().rollback();
    //        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

    //        return;
    //    }

    //外卖人
    if(ui->checkBox_71->isChecked() == true)
    {
        str = "1";
    }
    else
    {
        str = "0";
    }

    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='bp_linkman'")
                  .arg(str)))
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

        return;
    }

    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='bp_linkman_len'")
                  .arg(ui->lineEdit_55->text())))
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

        return;
    }

    //    str.clear();
    //    str = ui->comboBox_39->currentText();
    //    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='bp_linkman_row'")
    //                  .arg(str)))
    //    {

    //    }
    //    else
    //    {
    //        QSqlDatabase::database().rollback();
    //        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

    //        return;
    //    }

    //外卖电话
    if(ui->checkBox_72->isChecked() == true)
    {
        str = "1";
    }
    else
    {
        str = "0";
    }

    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='bp_tel'")
                  .arg(str)))
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

        return;
    }

    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='bp_tel_len'")
                  .arg(ui->lineEdit_50->text())))
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

        return;
    }

    //    str.clear();
    //    str = ui->comboBox_40->currentText();
    //    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='bp_tel_row'")
    //                  .arg(str)))
    //    {

    //    }
    //    else
    //    {
    //        QSqlDatabase::database().rollback();
    //        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

    //        return;
    //    }

    //服务员
    if(ui->checkBox_73->isChecked() == true)
    {
        str = "1";
    }
    else
    {
        str = "0";
    }

    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='bp_waiter'")
                  .arg(str)))
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

        return;
    }

    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='bp_waiter_len'")
                  .arg(ui->lineEdit_54->text())))
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

        return;
    }

    //    str.clear();
    //    str = ui->comboBox_41->currentText();
    //    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='bp_waiter_row'")
    //                  .arg(str)))
    //    {

    //    }
    //    else
    //    {
    //        QSqlDatabase::database().rollback();
    //        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

    //        return;
    //    }

    //服务员名
    if(ui->checkBox_74->isChecked() == true)
    {
        str = "1";
    }
    else
    {
        str = "0";
    }

    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='bp_waitername'")
                  .arg(str)))
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

        return;
    }

    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='bp_waitername_len'")
                  .arg(ui->lineEdit_51->text())))
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

        return;
    }

    //    str.clear();
    //    str = ui->comboBox_42->currentText();
    //    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='bp_waitername_row'")
    //                  .arg(str)))
    //    {

    //    }
    //    else
    //    {
    //        QSqlDatabase::database().rollback();
    //        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

    //        return;
    //    }

    //开台备注
    if(ui->checkBox_75->isChecked() == true)
    {
        str = "1";
    }
    else
    {
        str = "0";
    }

    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='bp_memo'")
                  .arg(str)))
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

        return;
    }

    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='bp_memo_len'")
                  .arg(ui->lineEdit_53->text())))
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

        return;
    }

    //    str.clear();
    //    str = ui->comboBox_43->currentText();
    //    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='bp_memo_row'")
    //                  .arg(str)))
    //    {

    //    }
    //    else
    //    {
    //        QSqlDatabase::database().rollback();
    //        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

    //        return;
    //    }

    //流水号
    if(ui->checkBox_76->isChecked() == true)
    {
        str = "1";
    }
    else
    {
        str = "0";
    }

    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='serial_no'")
                  .arg(str)))
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

        return;
    }

    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='serial_no_len'")
                  .arg(ui->lineEdit_52->text())))
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

        return;
    }

    //    str.clear();
    //    str = ui->comboBox_44->currentText();
    //    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='serial_no_row'")
    //                  .arg(str)))
    //    {

    //    }
    //    else
    //    {
    //        QSqlDatabase::database().rollback();
    //        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

    //        return;
    //    }

    //每行打印列数
    str.clear();
    str = ui->lineEdit_33->text();
    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='bp_offcols'")
                  .arg(str)))
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

        return;
    }

    //付款详情
    str.clear();
    if(ui->checkBox_77->isChecked() == true)
    {
        str = "1";
    }
    else
    {
        str = "0";
    }
    configIniWrite->setValue("system_setting/fukuanxiangqing",str);

    //消费金额
    str.clear();
    if(ui->checkBox_78->isChecked() == true)
    {
        str = "1";
    }
    else
    {
        str = "0";
    }
    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='bp_cost'")
                  .arg(str)))
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

        return;
    }

    //折扣金额
    str.clear();
    if(ui->checkBox_80->isChecked() == true)
    {
        str = "1";
    }
    else
    {
        str = "0";
    }
    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='bp_discount'")
                  .arg(str)))
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

        return;
    }

    //赠送金额
    str.clear();
    if(ui->checkBox_79->isChecked() == true)
    {
        str = "1";
    }
    else
    {
        str = "0";
    }
    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='bp_present'")
                  .arg(str)))
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

        return;
    }

    //服务费
    str.clear();
    if(ui->checkBox_82->isChecked() == true)
    {
        str = "1";
    }
    else
    {
        str = "0";
    }
    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='bp_service'")
                  .arg(str)))
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

        return;
    }

    //税收
    str.clear();
    if(ui->checkBox_81->isChecked() == true)
    {
        str = "1";
    }
    else
    {
        str = "0";
    }
    configIniWrite->setValue("system_setting/shuishoujine",str);

    //低消差
    str.clear();
    if(ui->checkBox_87->isChecked() == true)
    {
        str = "1";
    }
    else
    {
        str = "0";
    }
    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='bp_lowcost'")
                  .arg(str)))
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

        return;
    }

    //抹零
    str.clear();
    if(ui->checkBox_86->isChecked() == true)
    {
        str = "1";
    }
    else
    {
        str = "0";
    }
    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='bp_blotout'")
                  .arg(str)))
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

        return;
    }

    //免单
    str.clear();
    if(ui->checkBox_84->isChecked() == true)
    {
        str = "1";
    }
    else
    {
        str = "0";
    }
    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='bp_free'")
                  .arg(str)))
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

        return;
    }

    //应收
    str.clear();
    if(ui->checkBox_83->isChecked() == true)
    {
        str = "1";
    }
    else
    {
        str = "0";
    }
    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='bp_receive'")
                  .arg(str)))
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

        return;
    }

    //人均
    str.clear();
    if(ui->checkBox_85->isChecked() == true)
    {
        str = "1";
    }
    else
    {
        str = "0";
    }
    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='bp_avg'")
                  .arg(str)))
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

        return;
    }

    //实收
    str.clear();
    if(ui->checkBox_88->isChecked() == true)
    {
        str = "1";
    }
    else
    {
        str = "0";
    }
    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='bp_cash'")
                  .arg(str)))
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

        return;
    }

    //会员名称
    str.clear();
    if(ui->checkBox_93->isChecked() == true)
    {
        str = "1";
    }
    else
    {
        str = "0";
    }
    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='bp_membername'")
                  .arg(str)))
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

        return;
    }

    //会员类型
    str.clear();
    if(ui->checkBox_92->isChecked() == true)
    {
        str = "1";
    }
    else
    {
        str = "0";
    }
    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='bp_membertype'")
                  .arg(str)))
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

        return;
    }

    //找零
    str.clear();
    if(ui->checkBox_90->isChecked() == true)
    {
        str = "1";
    }
    else
    {
        str = "0";
    }
    configIniWrite->setValue("system_setting/zhaolingjine",str);

    //会员打印余额
    str.clear();
    if(ui->checkBox_89->isChecked() == true)
    {
        str = "1";
    }
    else
    {
        str = "0";
    }
    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='print_ye'")
                  .arg(str)))
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

        return;
    }

    //会员积分
    str.clear();
    if(ui->checkBox_91->isChecked() == true)
    {
        str = "1";
    }
    else
    {
        str = "0";
    }
    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='print_vip_point'")
                  .arg(str)))
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

        return;
    }

    //签名
    str.clear();
    if(ui->checkBox_94->isChecked() == true)
    {
        str = "1";
    }
    else
    {
        str = "0";
    }
    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='bp_underwrite'")
                  .arg(str)))
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

        return;
    }

    //尾语
    str.clear();
    str = ui->lineEdit_34->text();
    configIniWrite->setValue("system_setting/jieweiyu",str);

    n_func::f_set_sysparm("bp_content_len", ui->lineEdit_bp_content_len->text());
    n_func::f_set_sysparm("bp_amount_left", ui->lineEdit_bp_amount_left->text());
    n_func::f_set_sysparm("bp_discount_left", ui->lineEdit_bp_discount_left->text());
    n_func::f_set_sysparm("bp_dishname_left", ui->lineEdit_bp_dishname_left->text());
    n_func::f_set_sysparm("bp_num_left", ui->lineEdit_bp_num_left->text());
    n_func::f_set_sysparm("bp_price_left", ui->lineEdit_bp_price_left->text());
    n_func::f_set_sysparm("bp_unit_left", ui->lineEdit_bp_unit_left->text());

    //save tab4
    //打印方式
    if( ui->comboBox_6->currentText() == tr("明细打印"))
    {
        str = "1";
    }
    else
    {
        str = "2";
    }
    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='bp_print_mode'")
                  .arg(str)))
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

        return;
    }

    //列表头
    str.clear();
    str = ui->lineEdit_2->text();
    configIniWrite->setValue("system_setting/biaoneirong",str);

    //菜品分组
    n_func::f_set_sysparm("bp_series_group", ui->checkBox_19->isChecked()?"1":"0");
    //本单序号
    n_func::f_set_sysparm("bp_rowid", ui->checkBox_20->isChecked()?"1":"0");
    //赠送标志
    n_func::f_set_sysparm("bp_presentflag", ui->checkBox_21->isChecked()?"1":"0");
    //特价标志
    n_func::f_set_sysparm("bp_specialflag", ui->checkBox_22->isChecked()?"1":"0");
    //超出设置长度自动裁剪
    n_func::f_set_sysparm("bp_overstep_cut", ui->checkBox_23->isChecked()?"1":"0");
    //品名
    n_func::f_set_sysparm("bp_dishname", ui->checkBox_24->isChecked()?"1":"0");
    n_func::f_set_sysparm("bp_dishname_len", ui->lineEdit_3->text());
    n_func::f_set_sysparm("bp_dishname_row", ui->comboBox_7->currentText());
    //单位设置
    n_func::f_set_sysparm("bp_unit", ui->checkBox_31->isChecked()?"1":"0");
    n_func::f_set_sysparm("bp_unit_len", ui->lineEdit_5->text());
    n_func::f_set_sysparm("bp_unit_row", ui->comboBox_9->currentText());
    //数量设置
    n_func::f_set_sysparm("bp_num", ui->checkBox_32->isChecked()?"1":"0");
    n_func::f_set_sysparm("bp_num_len", ui->lineEdit_6->text());
    n_func::f_set_sysparm("bp_num_row", ui->comboBox_10->currentText());
    //价格设置
    n_func::f_set_sysparm("bp_price", ui->checkBox_41->isChecked()?"1":"0");
    n_func::f_set_sysparm("bp_price_len", ui->lineEdit_10->text());
    n_func::f_set_sysparm("bp_price_row", ui->comboBox_17->currentText());
    //金额设置
    n_func::f_set_sysparm("bp_amount", ui->checkBox_42->isChecked()?"1":"0");
    n_func::f_set_sysparm("bp_amount_len", ui->lineEdit_11->text());
    n_func::f_set_sysparm("bp_amount_row", ui->comboBox_19->currentText());
    //折扣设置
    n_func::f_set_sysparm("bp_disdetail", ui->checkBox_43->isChecked()?"1":"0");
    n_func::f_set_sysparm("bp_disdetail_len", ui->lineEdit_12->text());
    n_func::f_set_sysparm("bp_disdetail_row", ui->comboBox_21->currentText());
    //折扣设置
    n_func::f_set_sysparm("bp_printmemo", ui->checkBox_44->isChecked()?"1":"0");
    //save tab5
    //点菜异常记录
    str.clear();
    if(ui->checkBox_47->isChecked() == true)
    {
        str = "1";
    }
    else
    {
        str = "0";
    }
    configIniWrite->setValue("system_setting/diancaiyichang",str);

    //打印LOGO
    str.clear();
    if(ui->checkBox_48->isChecked() == true)
    {
        str = "1";
    }
    else
    {
        str = "0";
    }
    configIniWrite->setValue("system_setting/dayinlogo",str);

    str.clear();
    str = ui->lineEdit_15->text();
    configIniWrite->setValue("system_setting/dayinlogoneirong",str);

    //打印一维码
    str.clear();
    if(ui->checkBox_49->isChecked() == true)
    {
        str = "1";
    }
    else
    {
        str = "0";
    }
    configIniWrite->setValue("system_setting/yiweima",str);

    str.clear();
    str = ui->lineEdit_16->text();
    configIniWrite->setValue("system_setting/yiweimaneirong",str);

    //打印二维码
    str.clear();
    if(ui->checkBox_50->isChecked() == true)
    {
        str = "1";
    }
    else
    {
        str = "0";
    }
    configIniWrite->setValue("system_setting/erweima",str);

    str.clear();
    str = ui->lineEdit_17->text();
    configIniWrite->setValue("system_setting/erweimaneirong",str);

    str.clear();
    str = ui->comboBox_48->currentText();
    configIniWrite->setValue("system_setting/erweiziti",str);

    str.clear();
    str = ui->lineEdit_18->text();
    configIniWrite->setValue("system_setting/erweimatishi",str);

    //打印结帐码
    str.clear();
    if(ui->checkBox_51->isChecked() == true)
    {
        str = "1";
    }
    else
    {
        str = "0";
    }
    configIniWrite->setValue("system_setting/dayinjiezhangma",str);

    str.clear();
    str = ui->lineEdit_19->text();
    configIniWrite->setValue("system_setting/gongzhongid",str);

    str.clear();
    str = ui->lineEdit_20->text();
    configIniWrite->setValue("system_setting/shanghuid",str);

    str.clear();
    str = ui->lineEdit_35->text();
    configIniWrite->setValue("system_setting/zhifumiyao",str);

    //设置厨打漏单IP
    str.clear();
    if(ui->checkBox_52->isChecked() == true)
    {
        str = "1";
    }
    else
    {
        str = "0";
    }
    configIniWrite->setValue("system_setting/cdloudanip",str);

    str.clear();
    str = ui->lineEdit_36->text();
    configIniWrite->setValue("system_setting/chudaloudaip",str);

    str.clear();
    if(ui->checkBox_53->isChecked() == true)
    {
        str = "1";
    }
    else
    {
        str = "0";
    }
    configIniWrite->setValue("system_setting/qingkonglouda",str);

    //save tab6
    str.clear();
    if( ui->checkBox_28->isChecked() == true)
    {
        str = "1";
    }
    else
    {
        str = "0";
    }
    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='label_title'")
                  .arg(str)))
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

        return;
    }

    //    str.clear();
    //    str = ui->comboBox_20->currentText();
    //    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='label_title_line'")
    //                  .arg(str)))
    //    {

    //    }
    //    else
    //    {
    //        QSqlDatabase::database().rollback();
    //        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));
    //
    //        return;
    //    }

    //标签品名
    str.clear();
    if( ui->checkBox_29->isChecked() == true)
    {
        str = "1";
    }
    else
    {
        str = "0";
    }
    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='label_dishname'")
                  .arg(str)))
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

        return;
    }

    //    str.clear();
    //    str = ui->comboBox_22->currentText();
    //    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='label_dishname_line'")
    //                  .arg(str)))
    //    {

    //    }
    //    else
    //    {
    //        QSqlDatabase::database().rollback();
    //        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));
    //
    //        return;
    //    }

    //标签单号
    str.clear();
    if( ui->checkBox_30->isChecked() == true)
    {
        str = "1";
    }
    else
    {
        str = "0";
    }
    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='label_billno'")
                  .arg(str)))
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

        return;
    }

    //    str.clear();
    //    str = ui->comboBox_23->currentText();
    //    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='label_billno_line'")
    //                  .arg(str)))
    //    {

    //    }
    //    else
    //    {
    //        QSqlDatabase::database().rollback();
    //        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));
    //
    //        return;
    //    }

    //标签时间
    str.clear();
    if( ui->checkBox_33->isChecked() == true)
    {
        str = "1";
    }
    else
    {
        str = "0";
    }
    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='label_time'")
                  .arg(str)))
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

        return;
    }

    //    str.clear();
    //    str = ui->comboBox_24->currentText();
    //    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='label_time_line'")
    //                  .arg(str)))
    //    {

    //    }
    //    else
    //    {
    //        QSqlDatabase::database().rollback();
    //        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));
    //
    //        return;
    //    }

    //标签价格
    str.clear();
    if( ui->checkBox_34->isChecked() == true)
    {
        str = "1";
    }
    else
    {
        str = "0";
    }
    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='label_price'")
                  .arg(str)))
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

        return;
    }

    //    str.clear();
    //    str = ui->comboBox_25->currentText();
    //    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='label_price_line'")
    //                  .arg(str)))
    //    {

    //    }
    //    else
    //    {
    //        QSqlDatabase::database().rollback();
    //        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));
    //
    //        return;
    //    }

    //标签做法
    str.clear();
    if( ui->checkBox_35->isChecked() == true)
    {
        str = "1";
    }
    else
    {
        str = "0";
    }
    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='label_memo'")
                  .arg(str)))
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

        return;
    }

    //    str.clear();
    //    str = ui->comboBox_26->currentText();
    //    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='label_memo_line'")
    //                  .arg(str)))
    //    {

    //    }
    //    else
    //    {
    //        QSqlDatabase::database().rollback();
    //        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));
    //
    //        return;
    //    }

    //标签电话
    str.clear();
    if( ui->checkBox_36->isChecked() == true)
    {
        str = "1";
    }
    else
    {
        str = "0";
    }
    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='label_tel'")
                  .arg(str)))
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

        return;
    }

    //    str.clear();
    //    str = ui->comboBox_27->currentText();
    //    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='label_tel_line'")
    //                  .arg(str)))
    //    {

    //    }
    //    else
    //    {
    //        QSqlDatabase::database().rollback();
    //        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));
    //
    //        return;
    //    }

    //标签标题
    str.clear();
    if( ui->checkBox_37->isChecked() == true)
    {
        str = "1";
    }
    else
    {
        str = "0";
    }
    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='label_title1'")
                  .arg(str)))
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

        return;
    }

    //    str.clear();
    //    str = ui->comboBox_28->currentText();
    //    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='label_title1_line'")
    //                  .arg(str)))
    //    {

    //    }
    //    else
    //    {
    //        QSqlDatabase::database().rollback();
    //        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));
    //
    //        return;
    //    }

    //标签地址
    str.clear();
    if( ui->checkBox_38->isChecked() == true)
    {
        str = "1";
    }
    else
    {
        str = "0";
    }
    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='label_address'")
                  .arg(str)))
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));

        return;
    }

    //    str.clear();
    //    str = ui->comboBox_29->currentText();
    //    if(query.exec(QString("update cey_sys_parameter set vch_value ='%1' where vch_parameter ='label_address_line'")
    //                  .arg(str)))
    //    {

    //    }
    //    else
    //    {
    //        QSqlDatabase::database().rollback();
    //        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_sys_parameter失败!"));
    //
    //        return;
    //    }

    if(ui->checkBox_52->isChecked() == true && !ui->lineEdit_36->text().isEmpty())
    {
        QSqlQueryModel model;
        model.setQuery("select count(*) from chudaloudan where vch_printflag = 'N'");
        if(model.record(0).value(0).toInt() > 0)
        {
            QSqlQuery loudaquery(QSqlDatabase::database("loss_form"));
            QSqlDatabase::database("loss_form").transaction();
            loudaquery.prepare(QString("update chudaloudan set vch_ip=:as_ip "));
            loudaquery.bindValue(":as_ip",ui->lineEdit_36->text());
            if(loudaquery.exec())
            {

            }
            else
            {
                QSqlDatabase::database("loss_form").rollback();
                lds_messagebox::warning(this, tr("消息提示"),tr("更新本地漏打单IP失败!") + QSqlDatabase::database("loss_form").lastError().text());

                return;
            }
            QSqlDatabase::database("loss_form").commit();
        }
    }

    if(ui->checkBox_53->isChecked() == true)
    {
        QFile f;
        f.remove("./chudaloudan.db");
    }

    //是否输入手牌号
    configIniWrite->setValue("system_setting/fukuanshishurushoupai",ui->checkBox_250->isChecked());
    //做法是否弹出
    configIniWrite->setValue("system_setting/cook_info_pop",ui->checkBox_251->isChecked());
    //流水号最大值
    n_func::f_set_sysparm("serial_no_end", ui->lineEdit_37->text(), "");
    //流水号最小值
    n_func::f_set_sysparm("serial_no_begin", ui->lineEdit_8->text(), "");
    //偶数杯半价
    configIniWrite->setValue("system_setting/even_halfprice",ui->checkBox_252->isChecked());
    //前台是否显示菜品图片
    configIniWrite->setValue("system_setting/is_show_dishpic",ui->checkBox_253->isChecked());
    //前台只显示大类
    configIniWrite->setValue("system_setting/only_dalei",ui->checkBox_255->isChecked());
    //点菜图片大小
    configIniWrite->setValue("system_setting/qt_iconsize", ui->lineEdit_21->text());
    //不显示小票抬头
    configIniWrite->setValue("system_setting/not_show_bill_title",ui->checkBox_254->isChecked());
    //8
    configIniWrite->setValue("system_setting/njgj_itemcode", ui->lineEdit_itemcode->text());
    configIniWrite->setValue("system_setting/njgj_storecode", ui->lineEdit_storecode->text());
    configIniWrite->setValue("system_setting/njgj_ip", ui->lineEdit_ip->text());
    configIniWrite->setValue("system_setting/njgj_license_agreement", ui->lineEdit_license_agreement->text());
    configIniWrite->setValue("system_setting/njgj_mallid", ui->lineEdit_mallid->text());
    configIniWrite->setValue("system_setting/njgj_port", ui->lineEdit_port->text());
    configIniWrite->setValue("system_setting/njgj_password", ui->lineEdit_password->text());
    configIniWrite->setValue("system_setting/njgj_username", ui->lineEdit_username->text());
    configIniWrite->setValue("system_setting/njgj_web", ui->lineEdit_web->text());
    configIniWrite->setValue("system_setting/njgj_use", ui->checkBox_use->isChecked());
    configIniWrite->setValue("system_setting/ftptp_use", ui->checkBox_ftptp_use->isChecked());
    configIniWrite->setValue("system_setting/ftptp_storecode", ui->lineEdit_ftptp_storecode->text());
    configIniWrite->setValue("system_setting/ftptp_plu", ui->lineEdit_ftptp_plu->text());
    configIniWrite->setValue("system_setting/ftptp_address", ui->lineEdit_ftptp_address->text());
    configIniWrite->setValue("system_setting/ftptp_usr", ui->lineEdit_ftptp_usr->text());
    configIniWrite->setValue("system_setting/ftptp_pwd", ui->lineEdit_ftptp_pwd->text());
    QSqlDatabase::database().commit();
    configIniWrite->sync();
    this->close();



    //    fast_order_2::model_dalei.setQuery(QString("SELECT  * FROM cey_bt_dish_type"));
    //    for(int i = 0; i < fast_order_2::model_dalei.rowCount(); i++)
    //    {
    //        if(i == fast_order_2::num_da)
    //        {
    //            fast_order_2::dalei = fast_order_2::model_dalei.record(i).value(0).toString();
    //        }
    //    }

    //    // model_xiaolei->clear();
    //    fast_order_2::model_xiaolei.setQuery(QString("SELECT  * FROM cey_bt_dish_subtype where ch_dish_typeno = '%1' ").arg(fast_order_2::dalei));
    //    for(int i = 0; i < fast_order_2::model_xiaolei.rowCount(); i++)
    //    {
    //        if(i == fast_order_2::num_xiao)
    //        {
    //            fast_order_2::xiaolei = fast_order_2::model_xiaolei.record(0).value(0).toString();
    //        }
    //    }


    //    //  model_cook->clear();
    //    if(fast_order_2::changyongcook == false)
    //    {
    //        fast_order_2::model_cook.setQuery(QString("SELECT  * FROM cey_bt_dish where ch_dish_typeno = '%1' and ch_sub_typeno ='%2' and (ch_stopflag = null or ch_stopflag = 'N') ").arg(fast_order_2::dalei).arg(fast_order_2::xiaolei));
    //    }
    //    else
    //    {
    //        fast_order_2::model_cook.setQuery(QString("select * from cey_bt_dish where ch_off = 'Y' and (ch_stopflag = null or ch_stopflag = 'N') "));
    //    }
}

void system_setting::on_pushButton_3_clicked()//取消
{
    btn_t_f = false;
    this->close();
}

void system_setting::on_checkBox_17_clicked()
{
    if(ui->checkBox_17->isChecked() == false)
    {
        ui->stackedWidget->widget(5)->setEnabled(false);//->setTabEnabled(5,false);
    }
    else
    {
        ui->stackedWidget->widget(5)->setEnabled(true);//->setTabEnabled(5,false);
    }
}

void system_setting::on_comboBox_30_currentIndexChanged(const QString &arg1)
{
    //打印机端口
    ui->comboBox_31->clear();
    ui->pushButton_8->setVisible(false);
    switch(ui->comboBox_30->currentIndex()){
    case 0://USB
        ui->comboBox_31->addItems(f_scan_usbPort());
        break;
    case 1://串口
        ui->comboBox_31->addItems(f_scan_ttyPort());
        break;
    case 2:{//网口
        QSqlQuery query;
        QStringList ipports;
        query.exec("select vch_printip from cey_bt_kitchen_print;");
        while(query.next()){
            ipports.append(query.record().value(0).toString()+":"+"9100");
        }
        ui->comboBox_31->addItems(ipports);
        ui->pushButton_8->setVisible(true);
    }
        break;
    }
}

void system_setting::on_pushButton_5_clicked()//广告路径
{
    QString str = ui->lineEdit_13->text();
    QString fileName=QFileDialog::getExistingDirectory(NULL, tr("选择文件夹"),str.isEmpty()?"/mnt":str,QFileDialog::ShowDirsOnly);
    QDir dir(fileName);
    QFileInfoList list;
    list = dir.entryInfoList();
    QString path;
    for(int i = 0; i < list.count(); i++)
    {
        path = list.value(i).path();
        //qDebug() <<  path;
    }
    if(path == ".")
    {
        path = str;
    }
    ui->lineEdit_13->setText(path);
}

void system_setting::on_pushButton_6_clicked()//图片路径
{
    QString str = ui->lineEdit_14->text();
    QString fileName=QFileDialog::getExistingDirectory(NULL, tr("选择文件夹"),"/mnt",QFileDialog::ShowDirsOnly);
    QDir dir(fileName);
    QFileInfoList list;
    list = dir.entryInfoList();
    QString path;
    for(int i = 0; i < list.count(); i++)
    {
        path = list.value(i).path();
        //qDebug() <<  path;
    }
    if(path == ".")
    {
        path = str;
    }
    ui->lineEdit_14->setText(path);
}

void system_setting::on_pushButton_clicked()//打印机测试
{
    backheader::syn_dataprin_test(ui->comboBox_31->currentText());
}

void system_setting::on_pushButton_2_clicked()//钱箱测试
{
    if(ui->checkBox_39->isChecked()){
        blockIns bins;
        bins.append(pieceIns(ui->lineEdit_9->text().toLocal8Bit(),""));
        backheader::asyn_dataprint(bins, ui->comboBox_31->currentText());
    } else {
        lds_messagebox::warning(this, tr("消息提示"),tr("开钱箱未勾选!"));
    }
}

void system_setting::show_data()
{
    init_tab1();
    init_tab2();
    init_tab3();
    init_tab4();
    init_tab5();
    init_tab6();
    init_tab7();
    init_tab8();
}

void system_setting::on_checkBox_49_clicked()
{
    if(ui->checkBox_49->isChecked() == false)
    {
        ui->lineEdit_16->setEnabled(false);
    }
    else
    {
        ui->lineEdit_16->setEnabled(true);
    }
}

void system_setting::on_checkBox_50_clicked()
{
    if(ui->checkBox_50->isChecked() == false)
    {
        ui->lineEdit_17->setEnabled(false);
        ui->lineEdit_18->setEnabled(false);
    }
    else
    {
        ui->lineEdit_17->setEnabled(true);
        ui->lineEdit_18->setEnabled(true);
    }
}

void system_setting::on_checkBox_51_clicked()
{
    if(ui->checkBox_51->isChecked() == false)
    {
        ui->lineEdit_19->setEnabled(false);
        ui->lineEdit_20->setEnabled(false);
        ui->lineEdit_35->setEnabled(false);
    }
    else
    {
        ui->lineEdit_19->setEnabled(true);
        ui->lineEdit_20->setEnabled(true);
        ui->lineEdit_35->setEnabled(true);
    }
}

void system_setting::on_checkBox_52_clicked()
{
    if(ui->checkBox_52->isChecked() == false)
    {
        ui->lineEdit_36->setEnabled(false);
    }
    else
    {
        ui->lineEdit_36->setEnabled(true);
    }
}

void system_setting::on_pushButton_7_clicked()
{
    //    QString str = ui->lineEdit_15->text();
    //    QString fileName=QFileDialog::getExistingDirectory(NULL, tr("选择文件夹"),"/mnt",QFileDialog::ShowDirsOnly);
    //    QDir dir(fileName);
    //    QFileInfoList list;
    //    list = dir.entryInfoList();
    //    QString path;
    //    for(int i = 0; i < list.count(); i++)
    //    {
    //        path = list.value(i).path();
    //        //qDebug() <<  path;
    //    }
    //    if(path == ".")
    //    {
    //        path = str;
    //    }
    //    ui->lineEdit_15->setText(path);
    QString file_full, file_name, file_path;
    QFileInfo fi;
    file_full = QFileDialog::getOpenFileName(this);
    fi = QFileInfo(file_full);
    file_name = fi.fileName();
    file_path = fi.absolutePath();
    ui->lineEdit_15->setText(file_path + tr("/") + file_name);
}

void system_setting::on_pushButton_8_clicked()
{
    configIniWrite->setValue("system_setting/printerpath",ui->comboBox_31->currentText());
    w_bt_dish_Kitchenprinter_printermanager dialog(0);
    backheader::static_dialogexec(dialog,qobject_cast<QAbstractButton* >(sender())->text());
    init_tab2();
}

void system_setting::on_pushButton_9_clicked()//客显测试
{
    static int sqr=-0;
    ClientDisplay display(ui->comboBox_32->currentText(), "");
    if(sqr%5==0){
        if(!display.startShowinfo(ui->lineEdit_38->text(), ui->comboBox_45->currentText())){
            lds_messagebox::warning(this, tr("消息提示"),display.lastError());
        }
    } else {
        if(!display.writeCom(sqr%5, "-12.6", ui->comboBox_45->currentText())){
            lds_messagebox::warning(this, tr("消息提示"),display.lastError());
        }
    }
    sqr ++;
}

void system_setting::on_comboBox_32_currentIndexChanged(int index)
{
    ui->widget_kexian_start->setVisible(false);
    switch(index){
    case 0:
        break;
    case 1:
        ui->widget_kexian_start->setVisible(true);
        break;
    }
}

void system_setting::on_pushButton_10_clicked()//电子秤测试
{
    if(countprice_scale == 0) {
        countprice_scale=new   ElectronicCountPrice(0, "", "");
        connect(countprice_scale,SIGNAL(weighComming(QString)),ui->lineEdit_39,SLOT(setText(QString)));
    }
    countprice_scale->stop();
    countprice_scale->setType(ui->comboBox_47->currentText());
    countprice_scale->setBaudrate(ui->comboBox_49->currentText());
    if(!countprice_scale->tryStart_qs(ui->comboBox_46->currentText())){
        lds_messagebox::warning(this, tr("消息提示"),countprice_scale->lastError());
    }

    if(CommonHeader::creat_pricecount_scaletypes().indexOf(ui->comboBox_47->currentText())
            ==6){
        //copy fromres
        public_sql::copyfile(":/image/fh-pc", qApp->applicationDirPath()+"/fh-pc");

        //conduct outer exe
        QProcess p;
        p.start("\""+qApp->applicationDirPath()+"/fh-pc"+"\"");
        p.waitForFinished();
        p.close();
    }
}

void system_setting::on_pushButton_12_clicked()//标签打印机
{
    labelprint_main_dialog dialog(this);
    if(1==lds_messagebox::warning(this,tr("选择"), tr("标签打印机"), tr("导入基础资料"), tr("进入奶茶模式"))){
        dialog.printmodel->setHorizontalHeaderLabels(QStringList()
                                                     << tr("打印次数")
                                                     //                                                 << tr("抬头")
                                                     << tr("单号")
                                                     << tr("价格")
                                                     //                                                 << tr("电话")
                                                     //                                                 << tr("地址")

                                                     << tr("品名")
                                                     << tr("品码")
                                                     << tr("时间")
                                                     << tr("做法")
                                                     //                                                 << tr("标题")
                                                     );
        dialog.printmodel->appendRow(QList<QStandardItem*>()
                                     <<new QStandardItem("1")
                                     //                                 <<new QStandardItem(tr("抬头"))
                                     <<new QStandardItem(tr("单号"))
                                     <<new QStandardItem(tr("价格"))
                                     //                                 <<new QStandardItem(tr("电话"))
                                     //                                 <<new QStandardItem(tr("地址"))

                                     <<new QStandardItem(tr("品名"))
                                     <<new QStandardItem(tr("品码"))
                                     <<new QStandardItem(tr("时间"))
                                     <<new QStandardItem(tr("做法"))
                                     //                                 <<new QStandardItem(tr("标题"))
                                     );
    } else {
        dialog.printmodel->setHorizontalHeaderLabels(QStringList()
                                                     << tr("打印次数")
                                                     << tr("品名")
                                                     << tr("品码")
                                                     << tr("价格")
                                                     << tr("生产日期")
                                                     << tr("保质期")
                                                     );
        QSqlQuery query;
        query.exec("select * from cey_bt_dish where ch_stopflag='N' ");
        while(query.next()){
            dialog.printmodel->appendRow(QList<QStandardItem*>()
                                         <<new QStandardItem("1")
                                         <<new QStandardItem(query.record().value("vch_dishname").toString())
                                         <<new QStandardItem(query.record().value("ch_dishno").toString())
                                         <<new QStandardItem(QString().sprintf("%.02f", query.record().value("num_price").toFloat()))
                                         <<new QStandardItem(QDate::currentDate().toString("yyyy-MM-dd"))
                                         <<new QStandardItem("")
                                         );
        }
    }
    dialog.inputfinished();
    backheader::static_dialogexec(dialog, tr("标签打印机"), backheader::mainwindowsize);

}

void system_setting::to_print_label_test(QMap<QString, QRect> _str_rect_map, const QString &type, const QString &font, const QString &path, const QMap<QString, QString> &printdata)
{
    QString ins;
    float width=_str_rect_map.value("back").width()/8.0;
    float height=_str_rect_map.value("back").height()/8.0;

    switch(CommonHeader::creat_printtypes().indexOf(type)){
    case 0://POSTEK Q8/200
    {
        Printer_POSTEK printer;
        QMap<QString, QRect>::iterator itrbegin=_str_rect_map.begin();
        while(itrbegin != _str_rect_map.end()){
            QRect r=itrbegin.value();
            ins+=printer.drawText(printdata.value(itrbegin.key()),r.x(), r.y(), font);
            itrbegin++;
        }
        backheader::asyn_dataprint(printer.toPrint(ins, 1, height), path);
    }
        break;
    case 1://TSC TTP-244 PLUS
    {
        printer_TSC_TTP_244_PLUS printer;
        QMap<QString, QRect>::iterator itrbegin=_str_rect_map.begin();
        while(itrbegin != _str_rect_map.end()){
            QRect r=itrbegin.value();
            ins+=printer.drawText(printdata.value(itrbegin.key()),r.x(), r.y(), font);
            itrbegin++;
        }
        backheader::asyn_dataprint(printer.toPrint(ins, 1, QString::number(height), 0, QString::number(width)), path);
    }
        break;
    case 2://SC2100(商超专用)
    {
        Printer_ARGOX_PPLA printer;
        QMap<QString, QRect>::iterator itrbegin=_str_rect_map.begin();
        while(itrbegin != _str_rect_map.end()){
            QRect r=itrbegin.value();
            ins+=printer.drawText(printdata.value(itrbegin.key()),r.x(), r.y(), font);
            itrbegin++;
        }
        backheader::asyn_dataprint(printer.toPrint(ins,1), path);
    }
        break;
    case 3://ARGOX CP-2140
    {
        Printer_ARGOX_PPLB printer;
        QMap<QString, QRect>::iterator itrbegin=_str_rect_map.begin();
        while(itrbegin != _str_rect_map.end()){
            QRect r=itrbegin.value();
            ins+=printer.drawText(printdata.value(itrbegin.key()),r.x(), r.y(), font);
            itrbegin++;
        }
        backheader::asyn_dataprint(printer.toPrint(ins), path);
    }
        break;
    case 4://佳博GP-3120TN
    {
        Printer_GAINSCHA printer;
        QMap<QString, QRect>::iterator itrbegin=_str_rect_map.begin();
        while(itrbegin != _str_rect_map.end()){
            QRect r=itrbegin.value();
            ins+=printer.drawText(printdata.value(itrbegin.key()),r.x(), r.y(), font);
            itrbegin++;
        }
        backheader::asyn_dataprint(printer.toPrint(ins, 1, QString::number(height), 0, QString::number(width)), path);
    }
        break;
    case 5://佳博GP-2120TF
    {
        Printer_GAINSCHA_2120TF printer;
        QMap<QString, QRect>::iterator itrbegin=_str_rect_map.begin();
        while(itrbegin != _str_rect_map.end()){
            QRect r=itrbegin.value();
            ins+=printer.drawText(printdata.value(itrbegin.key()),r.x(), r.y(), font);
            itrbegin++;
        }
        float fheight=height*1.0/25.4;
        float fwidth=width*1.0/25.4;
        backheader::asyn_dataprint(printer.toPrint(ins, 1, fheight, 0, fwidth), path);
    }
        break;
    }
}

void system_setting::on_pushButton_17_clicked()
{
    barscale_main_dialog dialog;
    QSqlQuery query;
    int sqr=1;
    query.exec("select *from cey_bt_dish where  ch_stopflag <> 'Y'");//没有停用
    while(query.next()){
        //条码不能为空
        if(query.record().value("vch_barcode").toString().isEmpty()) continue;

        dialog.add_down_infolist(barscale_main_dialog::BARSCAEL_ITEMS(
                                     QString::number(sqr),
                                     query.record().value("vch_dishname").toString(),
                                     query.record().value("vch_barcode").toString(),
                                     query.record().value("num_price").toString()
                                     ));
        sqr++;
    }

    query.exec("SELECT vch_company FROM hddpos.cey_sys_company;");
    query.next();
    dialog.set_shop_name(query.record().value("vch_company").toString());
    dialog.inputfinished();
    backheader::static_dialogexec(dialog, tr("条码秤"), backheader::mainwindowsize);
}

void system_setting::on_comboBox_97_currentIndexChanged(int index)
{
    ui->comboBox_30->setEnabled(true);
    ui->pushButton_8->setEnabled(true);
    ui->comboBox_31->setEnabled(true);
    if(index==3){//T2US
        ui->comboBox_30->setEnabled(false);
        ui->pushButton_8->setEnabled(false);
        ui->comboBox_31->setEnabled(false);
        ui->comboBox_31->setEditText(Printer_Normal_T2US().vidpid);
    }
}
void system_setting::on_pushButton_automaketitle_clicked()//自动生成表头
{
    ui->lineEdit_2->clear();
    //品名
    if(ui->checkBox_24->isChecked())ui->lineEdit_2->setText(ui->lineEdit_2->text()+ui->checkBox_24->text()+QString(ui->lineEdit_3->text().toInt()-size_g_of_u(ui->checkBox_24->text()), ' '));
    //单位
    if(ui->checkBox_31->isChecked())ui->lineEdit_2->setText(ui->lineEdit_2->text()+ui->checkBox_31->text()+QString(ui->lineEdit_5->text().toInt()-size_g_of_u(ui->checkBox_31->text()), ' '));
    //数量
    if(ui->checkBox_32->isChecked())ui->lineEdit_2->setText(ui->lineEdit_2->text()+ui->checkBox_32->text()+QString(ui->lineEdit_6->text().toInt()-size_g_of_u(ui->checkBox_32->text()), ' '));
    //价格
    if(ui->checkBox_41->isChecked())ui->lineEdit_2->setText(ui->lineEdit_2->text()+ui->checkBox_41->text()+QString(ui->lineEdit_10->text().toInt()-size_g_of_u(ui->checkBox_41->text()), ' '));
    //金额
    if(ui->checkBox_42->isChecked())ui->lineEdit_2->setText(ui->lineEdit_2->text()+ui->checkBox_42->text()+QString(ui->lineEdit_11->text().toInt()-size_g_of_u(ui->checkBox_42->text()), ' '));
    //折扣
    if(ui->checkBox_43->isChecked())ui->lineEdit_2->setText(ui->lineEdit_2->text()+ui->checkBox_43->text()+QString(ui->lineEdit_12->text().toInt()-size_g_of_u(ui->checkBox_43->text()), ' '));

}

void system_setting::on_pushButton_18_clicked()
{
    //1
    w_scr_dish_data_collection dialog(this);
    backheader::static_dialogexec(dialog,tr("数据采集"));
}

void system_setting::on_pushButton_19_clicked()//演示用例
{
    ui->lineEdit_ip->setText("211.94.93.147");
    ui->lineEdit_itemcode->setText("33201L15000131");
    ui->lineEdit_license_agreement->setText("");
    ui->lineEdit_mallid->setText("33201");
    ui->lineEdit_port->setText("7088");
    ui->lineEdit_storecode->setText("33201L1500013");
    ui->lineEdit_username->setText("jinmao");
    ui->lineEdit_web->setText("/salestrans.asmx");
}

void system_setting::on_lineEdit_41_editingFinished()
{
    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    conf.setValue("system_setting/doublescreen_timeout", qMax(ui->lineEdit_41->text().toInt(), 200));
}

void system_setting::on_lineEdit_7_editingFinished()
{
    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    conf.setValue("system_setting/kitchen_timeout", qMax(ui->lineEdit_7->text().toInt(), 200));
}

void system_setting::on_pushButton_ftptp_example_clicked()
{
    /*
商铺号 B0295N01

货号 B0295N01CY0001

FTP地址  ftp.lncrland.com

账号 B41B0295N01

密码 J9w8400Q*/
    ui->lineEdit_ftptp_storecode->setText("B0295N01");
    ui->lineEdit_ftptp_plu->setText("B0295N01CY0001");
    ui->lineEdit_ftptp_address->setText("ftp.lncrland.com");
    ui->lineEdit_ftptp_usr->setText("B41B0295N01");
//    ui->lineEdit_ftptp_pwd->setText("J9w8400Q");
}

void system_setting::on_comboBox_97_currentIndexChanged(const QString &arg1)
{

}
