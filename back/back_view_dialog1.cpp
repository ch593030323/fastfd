#include "back_view_dialog1.h"
#include <QListView>
#include "w_bt_dish_base.h"
#include "w_bt_dish_table.h"
#include "w_bt_dish_suit.h"
#include "w_bt_dish_cookset.h"
#include "w_bt_dish_kitchenprinter.h"
#include "w_bt_dish_paymode.h"
#include "w_bt_dish_joborder.h"
#include "w_bt_dish_quit.h"


#include "w_m_member.h"
#include "w_m_member_cardic.h"
#include "w_m_member_list.h"
#include "w_m_member_paramset.h"
#include "w_m_member_point.h"
#include"w_m_member_recharge.h"
#include"w_m_member_report.h"
#include"w_m_member_statemanage.h"
#include"w_m_member_type.h"

#include "w_rpt_report_dishsalereport.h"
#include "w_rpt_report_cashreport.h"
#include "w_rpt_report_daymonweekanalysis.h"
#include "w_rpt_report_dishquitreport.h"
#include "w_rpt_report_cashfreequery.h"
#include "w_rpt_report_categorysalesanalysis.h"
#include "w_rpt_report_daybusinessreprot.h"
#include "w_rpt_report_directsalegrossprofitreport.h"
#include "w_rpt_report_dishsalerank.h"
#include "w_rpt_report_periodbusinessanalysis.h"
#include "w_rpt_report_promotionreport.h"
#include "w_rpt_report_servantpromotquery.h"
#include "w_rpt_report_servicefeelowpinquery.h"
#include "w_rpt_report_tablebusinessreport.h"

#include "w_sys_manage.h"
#include "w_sys_manage_basicdataexport.h"
#include "w_sys_manage_basicdataimport.h"
#include "w_sys_manage_businessdataclear.h"
#include "w_sys_manage_companyinformation.h"
#include "w_sys_manage_filemanage.h"
#include "w_sys_manage_mysql.h"
#include "w_sys_manage_operatorinformation.h"
#include "w_sys_manage_operatorright.h"
#include "w_sys_manage_sqlcopy.h"
#include "w_sys_manage_sysreboot.h"
#include "w_sys_manage_sysset.h"
//#include "w_sys_manage_weixin.h"
#include "w_sys_manage_wirelessordermonitoring.h"
#include "w_sys_manage_cloudsync.h"

#include "w_ds_sale_anticheckout.h"
#include "w_ds_sale_businessserach.h"
#include "w_ds_sale_cashjournal.h"
#include "w_ds_sale_daybusiness.h"
#include "w_ds_sale_dayspecial.h"
#include "w_ds_sale_dishsaleout.h"
#include "w_ds_sale_qt.h"
#include "w_ds_sale_salejournal.h"
#include "w_bt_dish_suit.h"

#include "w_i_inventory_check.h"
#include "w_i_inventory_goodsinput.h"
#include "w_i_inventory_goodsquit.h"
#include "w_i_inventory_invoicing.h"
#include "w_i_inventory_loss.h"
#include "w_i_inventory_paremset.h"
#include "w_i_inventory_query.h"
#include "w_i_inventory_stocking.h"
#include "w_i_inventory_summaryquery.h"

#include "backheader.h"
#include "printer_pos_serial.h"
#include "qt/back_special.h"
#include "qt/back_warn.h"
#include "qt/business_documents_query.h"
#include "qt/back_anti_checkout.h"
#include "qt/back_sales.h"
#include "qt/back_cashier_query.h"
//#include "qt/fast_order.h"
#include "qt/back_business_day.h"
#include <QSettings>
#include "system_setting.h"
#include "lds_messagebox.h"
#include "fast_order_2.h"
#include "public_sql.h"
#include <QtDebug>

back_view_dialog1::back_view_dialog1(public_loginDialog *parent) :
    back_view_2(0),
    _parent(parent),
    dialog(0)
{
    connect(this,SIGNAL(signal_index(int ,int)),this,SLOT(back_view_dialog_clicked(int,int)));
    ui->stackedWidget->setCurrentIndex(0);
}

void back_view_dialog1::back_view_dialog_clicked(int page, int index)
{
    switch(page){
    case 1:{
        /*==========================*/
        int sqr=-1;
        sqr++;if(index==sqr){
            if(backheader::operid_right_code("0101")){//菜品资料维护【 0101 】
                dialog=static_cast<QDialog *>(new w_bt_dish_base(this));
                dialog->resize(backheader::mainwindowsize);
                dialog->setWindowTitle(tr("菜品资料维护"));
            }
            break;
        }
        sqr++;if(index==sqr){
            if(backheader::operid_right_code("0105")){//餐桌资料维护【 0105 】
                dialog=static_cast<QDialog *>(new w_bt_dish_table(this));
                dialog->resize(backheader::mainwindowsize);
                dialog->setWindowTitle(tr("餐桌资料维护"));
            }
            break;
        }
        sqr++;if(index==sqr){
            if(backheader::operid_right_code("0107")){//营业班次【 0107 】
                dialog=static_cast<QDialog *>(new w_bt_dish_joborder(this));
                dialog->setWindowTitle(tr("营业班次"));
            }
            break;
        }

        /*==========================*/
        sqr++;if(index==sqr){
            if(backheader::operid_right_code("0106")){//收银方式【 0106 】
                dialog=static_cast<QDialog *>(new w_bt_dish_paymode(this));
                dialog->setWindowTitle(tr("收银方式"));
            }
            break;
        }
        sqr++;if(index==sqr){
            if(backheader::operid_right_code("0102")){//菜品套菜资料【 0102 】
                dialog=static_cast<QDialog *>(new w_bt_dish_suit(this));
                dialog->resize(backheader::mainwindowsize);
                dialog->setWindowTitle(tr("菜品套菜资料"));
            }
            break;
        }

        /*==========================*/
        sqr++;if(index==sqr){
            if(backheader::operid_right_code("0104")){//菜品做法设置【 0104 】
                dialog=static_cast<QDialog *>(new w_bt_dish_cookset(this));
                dialog->resize(backheader::mainwindowsize);
                dialog->setWindowTitle(tr("菜品做法设置"));
            }
            break;
        }
        sqr++;if(index==sqr){
            if(backheader::operid_right_code("0103")){//菜品厨打设置【 0103 】
                dialog=static_cast<QDialog *>(new w_bt_dish_Kitchenprinter(this));
                dialog->resize(backheader::mainwindowsize);
                dialog->setWindowTitle(tr("菜品厨打设置"));
            }
            break;
        }
        sqr++;if(index==sqr){
            if(backheader::operid_right_code("0110")){//退菜原因【 0110 】
                dialog=static_cast<QDialog *>(new w_bt_dish_quit(this));
                dialog->setWindowTitle(tr("退菜原因"));
            }
            break;
        }
    }

    case 2:{
        /*==========================*/
        int sqr=-1;
        sqr++;if(index==sqr){
            if(backheader::operid_right_code("0401")){//会员参数设置【 0401 】
                dialog=static_cast<QDialog *>(new w_m_member_paramset(this));
                dialog->setWindowTitle(tr("会员参数设置"));
            }
            break;
        }
        sqr++;if(index==sqr){
//            if(backheader::operid_right_code("0404")){//会员充值【 0404 】
                w_m_member_list::statictorecharge(this, 0, 0, "");
//            }
            break;
        }
        /*==========================*/
        sqr++;if(index==sqr){
            if(backheader::operid_right_code("0402")){//会员资料维护【 0402 】
                dialog=static_cast<QDialog *>(new w_m_member_list(this));
                dialog->setWindowTitle(tr("会员资料维护"));
                dialog->resize(backheader::mainwindowsize);
            }
            break;
        }
        sqr++;if(index==sqr){
            if(backheader::operid_right_code("0403")){//会员状态管理【 0403 】
                dialog=static_cast<QDialog *>(new w_m_member_statemanage(this));
                dialog->setWindowTitle(tr("会员状态管理"));
                dialog->resize(backheader::mainwindowsize);
            }
            break;
        }
        sqr++;if(index==sqr){
            if(backheader::operid_right_code("0407")){//会员类型变更【 0407 】
                dialog = static_cast<QDialog *>(new w_m_member_type(this));
                dialog->setWindowTitle(tr("会员类型变更"));
            }
            break;
        }
        /*==========================*/
        sqr++;if(index==sqr){
            if(backheader::operid_right_code("0406")){//会员积分管理【 0406 】
                dialog=static_cast<QDialog *>(new w_m_member_point(this));
                dialog->setWindowTitle(tr("会员积分管理"));
                dialog->resize(backheader::mainwindowsize);
            }
            break;
        }
        sqr++;if(index==sqr){
            if(backheader::operid_right_code("0411")){//会员报表【 0411 】
                dialog=static_cast<QDialog *>(new w_m_member_report(this));
                dialog->setWindowTitle(tr("会员报表"));
                dialog->resize(backheader::mainwindowsize);
            }
            break;
        }
        sqr++;if(index==sqr){
            if(backheader::operid_right_code("0413")){//芯片清除【 0413 】
                dialog=static_cast<QDialog *>(new w_m_member_cardic(this));
                dialog->setWindowTitle(tr("芯片清除"));
            }
            break;
        }
    }

    case 3:{
        /*==========================*/
        int sqr=-1;
        sqr++;if(index==sqr){
            if(backheader::operid_right_code("0242")){//今日特价【 0242 】
                dialog=static_cast<QDialog *>(new back_special(this));
                dialog->setWindowTitle(tr("今日特价"));
                dialog->resize(backheader::mainwindowsize);
            }
            break;
        }
        sqr++;if(index==sqr){
            if(backheader::operid_right_code("0240")){//营业日结【  0240 】
                dialog=static_cast<QDialog *>(new back_business_day(this));
                dialog->setWindowTitle(tr("营业日结"));
            }
            break;
        }
        sqr++;if(index==sqr){
            if(backheader::operid_right_code("0241")){//前台营业【 0241 】
                _parent->goto_pre();
                this->accept();
            }
            break;
        }
        /*==========================*/
        sqr++;if(index==sqr){
            if(backheader::operid_right_code("0243")){//菜品沽清【 0243 】
                dialog=static_cast<QDialog *>(new back_warn(this));
                dialog->setWindowTitle(tr("菜品沽清"));
                dialog->resize(backheader::mainwindowsize);
            }
            break;
        }
        sqr++;if(index==sqr){
            if(backheader::operid_right_code("0203")){//菜品销售流水查询【 0203 】
                dialog=static_cast<QDialog *>(new back_sales(this));
                dialog->setWindowTitle(tr("菜品销售流水查询"));
                dialog->resize(backheader::mainwindowsize);
            }
            break;
        }

        /*==========================*/
        sqr++;if(index==sqr){
            if(backheader::operid_right_code("0204")){//收银流水【 0204 】
                dialog=static_cast<QDialog *>(new back_cashier_query(this));
                dialog->setWindowTitle(tr("收银流水"));
                dialog->resize(backheader::mainwindowsize);
            }
            break;
        }
        sqr++;if(index==sqr){
            if(backheader::operid_right_code("0227")){//反结账查询【 0227 】
                dialog=static_cast<QDialog *>(new back_anti_checkout(this));
                dialog->setWindowTitle(tr("反结账查询"));
                dialog->resize(backheader::mainwindowsize);
            }
            break;
        }
        sqr++;if(index==sqr){
            if(backheader::operid_right_code("0201")){//营业单据查询【 0201 】
                dialog=static_cast<QDialog *>(new business_documents_query(this));
                dialog->setWindowTitle(tr("营业单据查询"));
                dialog->resize(backheader::mainwindowsize);
            }
            break;
        }
    }

    case 4:{
        /*==========================*/
        int sqr=-1;
        sqr++;if(index==sqr){
            if(backheader::operid_right_code("0612")){//库存参数设置【 0612 】
                dialog=static_cast<QDialog *>(new w_i_inventory_paremset(this));
                dialog->setWindowTitle(tr("库存参数设置"));
            }
            break;
        }
        sqr++;if(index==sqr){
            if(backheader::operid_right_code("0602")){//商品入库【 0602 】
                dialog=static_cast<QDialog *>(new w_i_inventory_goodsinput(this));
                dialog->setWindowTitle(tr("商品入库"));
                dialog->resize(backheader::mainwindowsize);
            }
            break;
        }
        sqr++;if(index==sqr){
            if(backheader::operid_right_code("0605")){//库存盘点【 0605 】
                dialog=static_cast<QDialog *>(new w_i_inventory_stocking(this));
                dialog->setWindowTitle(tr("库存盘点"));
            }
            break;
        }
        /*==========================*/
        sqr++;if(index==sqr){
            if(backheader::operid_right_code("0606")){//库存查询【 0606 】
                dialog=static_cast<QDialog *>(new w_i_inventory_query(this));
                dialog->setWindowTitle(tr("库存查询"));
                dialog->resize(backheader::mainwindowsize);
            }
            break;
        }
        sqr++;if(index==sqr){
            if(backheader::operid_right_code("0603")){//商品退货【 0603 】
                dialog=static_cast<QDialog *>(new w_i_inventory_goodsquit(this));
                dialog->setWindowTitle(tr("商品退货"));
                dialog->resize(backheader::mainwindowsize);
            }
            break;
        }
        sqr++;if(index==sqr){
            if(backheader::operid_right_code("0609")){//盘点查询【 0609 】
                dialog=static_cast<QDialog *>(new w_i_inventory_check(this));
                dialog->setWindowTitle(tr("盘点查询"));
                dialog->resize(backheader::mainwindowsize);
            }
            break;
        }
        /*==========================*/
        sqr++;if(index==sqr){
            if(backheader::operid_right_code("0611")){//单据汇总查询【 0611 】
                dialog=static_cast<QDialog *>(new w_i_inventory_summaryquery(this));
                dialog->setWindowTitle(tr("单据汇总查询"));
                dialog->resize(backheader::mainwindowsize);
            }
            break;
        }
        sqr++;if(index==sqr){
            if(backheader::operid_right_code("0604")){//报损单【 0604 】
                dialog=static_cast<QDialog *>(new w_i_inventory_loss(this));
                dialog->setWindowTitle(tr("报损单"));
                dialog->resize(backheader::mainwindowsize);
            }
            break;
        }
        sqr++;if(index==sqr){
            if(backheader::operid_right_code("0610")){//进销存汇总报表【 0610 】
                dialog=static_cast<QDialog *>(new w_i_inventory_Invoicing(this));
                dialog->setWindowTitle(tr("进销存汇总报表"));
                dialog->resize(backheader::mainwindowsize);
            }
            break;
        }
    }

    case 5:{
        /*==========================*/
        int sqr=-1;
        sqr++;if(index==sqr){
            if(backheader::operid_right_code("0701")){//菜品消费报表【 0701 】
                dialog=static_cast<QDialog *>(new w_rpt_report_dishsalereport(this));
                dialog->setWindowTitle(tr("菜品消费报表"));
                dialog->resize(backheader::mainwindowsize);
            }
            break;
        }
        sqr++;if(index==sqr){
            if(backheader::operid_right_code("0705")){//营业收银报表【 0705 】
                dialog=static_cast<QDialog *>(new w_rpt_report_cashreport(this));
                dialog->setWindowTitle(tr("营业收银报表"));
                dialog->resize(backheader::mainwindowsize);
            }
            break;
        }
        sqr++;if(index==sqr){
            if(backheader::operid_right_code("0720")){//直销商品毛利情况表【 0720 】
                dialog=static_cast<QDialog *>(new w_rpt_report_directsalegrossprofitreport(this));
                dialog->setWindowTitle(tr("直销商品毛利情况表"));
                dialog->resize(backheader::mainwindowsize);
            }
            break;
        }
        /*==========================*/
        sqr++;if(index==sqr){
            if(backheader::operid_right_code("0702")){//菜品促销报表【 0702 】
                dialog=static_cast<QDialog *>(new w_rpt_report_promotionreport(this));
                dialog->setWindowTitle(tr("菜品促销报表"));
                dialog->resize(backheader::mainwindowsize);
            }
            break;
        }
        sqr++;if(index==sqr){
            if(backheader::operid_right_code("0704")){//退菜情况报表【 0704 】
                dialog=static_cast<QDialog *>(new w_rpt_report_dishquitreport(this));
                dialog->setWindowTitle(tr("退菜情况报表"));
                dialog->resize(backheader::mainwindowsize);
            }
            break;
        }
        sqr++;if(index==sqr){
            if(backheader::operid_right_code("0715")){//服务员提成查询【 0715 】
                dialog=static_cast<QDialog *>(new w_rpt_report_servantpromotquery(this));
                dialog->setWindowTitle(tr("服务员提成查询"));
                dialog->resize(backheader::mainwindowsize);
            }
            break;
        }
        sqr++;if(index==sqr){
            if(backheader::operid_right_code("0706")){//服务费与低消查询【 0706 】
                dialog=static_cast<QDialog *>(new w_rpt_report_servicefeelowpinquery(this));
                dialog->setWindowTitle(tr("服务费与低消查询"));
                dialog->resize(backheader::mainwindowsize);
            }
            break;
        }
        /*==========================*/
        sqr++;if(index==sqr){
            if(backheader::operid_right_code("0707")){//收银免单情况查询【 0707 】
                dialog=static_cast<QDialog *>(new w_rpt_report_cashfreequery(this));
                dialog->setWindowTitle(tr("收银免单情况查询"));
                dialog->resize(backheader::mainwindowsize);
            }
            break;
        }
        sqr++;if(index==sqr){
            if(backheader::operid_right_code("0714")){//菜品销售排行统计【 0714 】
                dialog=static_cast<QDialog *>(new w_rpt_report_dishsalerank(this));
                dialog->setWindowTitle(tr("菜品销售排行统计"));
                dialog->resize(backheader::mainwindowsize);
            }
            break;
        }
        sqr++;if(index==sqr){
            if(backheader::operid_right_code("0703")){//餐桌营业数据统计【 0703 】
                dialog=static_cast<QDialog *>(new w_rpt_report_tablebusinessreport(this));
                dialog->setWindowTitle(tr("餐桌营业数据统计"));
                dialog->resize(backheader::mainwindowsize);
            }
            break;
        }
        sqr++;if(index==sqr){
            if(backheader::operid_right_code("0710")){//日营业情况统计【 0710 】
                dialog=static_cast<QDialog *>(new w_rpt_report_daybusinessreprot(this));
                dialog->setWindowTitle(tr("日营业情况统计"));
                dialog->resize(backheader::mainwindowsize);
            }
            break;
        }
        /*==========================*/
        sqr++;if(index==sqr){
            if(backheader::operid_right_code("0712")){//品类销售营业分析【 0712 】
                dialog=static_cast<QDialog *>(new w_rpt_report_categorysalesanalysis(this));
                dialog->setWindowTitle(tr("品类销售营业分析"));
                dialog->resize(backheader::mainwindowsize);
            }
            break;
        }
        sqr++;if(index==sqr){
            if(backheader::operid_right_code("0713")){//时段营业情况分析【 0713 】
                dialog=static_cast<QDialog *>(new w_rpt_report_periodbusinessanalysis(this));
                dialog->setWindowTitle(tr("时段营业情况分析"));
                dialog->resize(backheader::mainwindowsize);
            }
            break;
        }
        sqr++;if(index==sqr){
            if(backheader::operid_right_code("0708")){//日月周报表分析【 0708 】
                dialog=static_cast<QDialog *>(new w_rpt_report_daymonweekanalysis(this));
                dialog->setWindowTitle(tr("日月周报表分析"));
                dialog->resize(backheader::mainwindowsize);
            }
            break;
        }
    }

    case 6:{
        /*==========================*/
        int sqr=-1;
        sqr++;if(index==sqr){
            if(backheader::operid_right_code("0901")){//公司信息【 0901 】
                dialog=static_cast<QDialog *>(new w_sys_manage_companyinformation(this));
                dialog->setWindowTitle(tr("公司信息"));
            }
            break;
        }
        sqr++;if(index==sqr){
            if(backheader::operid_right_code("0903")){//操作员资料维护【 0903 】
                dialog=static_cast<QDialog *>(new w_sys_manage_operatorinformation(this));
                dialog->setWindowTitle(tr("操作员资料维护"));
            }
            break;
        }
        sqr++;if(index==sqr){
            if(backheader::operid_right_code("0904")){//操作员权限维护【 0904 】
                dialog=static_cast<QDialog *>(new w_sys_manage_operatorright(this));
                dialog->setWindowTitle(tr("操作员权限维护"));
                dialog->resize(backheader::mainwindowsize);
            }
            break;
        }
        sqr++;if(index==sqr){
            if(backheader::operid_right_code("0905")){//数据库备份【 0905 】
                dialog=static_cast<QDialog *>(new w_sys_manage_sqlcopy(this));
                dialog->setWindowTitle(tr("数据库备份"));
            }
            break;
        }
        /*==========================*/
        sqr++;if(index==sqr){
            if(backheader::operid_right_code("0907")){//基础数据导入【 0907 】
                dialog=static_cast<QDialog *>(new w_sys_manage_basicdataimport(this));
                dialog->setWindowTitle(tr("基础数据导入"));
            }
            break;
        }
        sqr++;if(index==sqr){
            if(backheader::operid_right_code("0906")){//基础数据导出【 0906 】
                dialog=static_cast<QDialog *>(new w_sys_manage_basicdataexport(this));
                dialog->setWindowTitle(tr("基础数据导出"));
            }
            break;
        }
        /*==========================*/
        sqr++;if(index==sqr){
            if(backheader::operid_right_code("0902")){//系统设置【 0902 】
                dialog=static_cast<QDialog *>(new system_setting(this));
                dialog->resize(backheader::mainwindowsize);
                dialog->setWindowTitle(tr("系统设置"));
            }
            break;
        }
        sqr++;if(index==sqr){
            if(backheader::operid_right_code("0913")){//u云同步【 0913 】
                dialog=static_cast<QDialog *>(new w_sys_manage_cloudsync(this));
                dialog->setWindowTitle(tr("u云同步"));
            }
            break;
        }
        sqr++;if(index==sqr){
            if(backheader::operid_right_code("0912")){//无线点菜监控【 0912 】
                dialog=static_cast<QDialog *>(new w_sys_manage_wirelessordermonitoring(this));
                dialog->setWindowTitle(tr("无线点菜监控"));
            }
            break;
        }
        sqr++;if(index==sqr){
            if(backheader::operid_right_code("0908")){//业务数据库清除【 0908 】
                dialog=static_cast<QDialog *>(new w_sys_manage_businessdataclear(this));
                dialog->setWindowTitle(tr("业务数据库清除"));
            }
            break;
        }
    }

    case 7:{
        if(0==lds_messagebox::warning(this, tr("消息提示"),tr("确认退出后台?"), tr("确认"),tr("取消"))){
            QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
            if(conf.value("system_setting/chongxidenglu").toString() == "1"){
                //返回登录界面
                _parent->hideall_and_back_login("", "");
            }  else {
                /*退出*/
                this->reject();
            }
        }
    }
    }//switch


    if(dialog){
        backheader::static_dialogexec(*dialog, "");
        delete dialog;
        dialog=0;
    }
}
