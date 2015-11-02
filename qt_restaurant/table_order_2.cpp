#include "table_order_2.h"
#include "ui_table_order.h"
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
#include "w_scr_dish_restaurant_dialog.h"
#include  "founding_cook.h"
#include "cook_zhuantai.h"
#include "cook_operation.h"
#include "cook_zhuantai_quanbu.h"
#include "linshi_cook.h"
#include "back_cook.h"
#include <QSqlError>
#include "lds_menu.h"
#include "backheader.h"
#include "system_setting.h"
#include "w_scr_dish_special_cook.h"
#include "fast_pay.h"
#include"frontviewdialog_virtual.h"
#include "table_pay_2.h"

table_order_2::table_order_2(QWidget *parent) :
    frontViewDialog_virtual(0),
    ui(new Ui_frontViewDialog_jiulou_dish_dialog)
{
    ui->setupUi(this);
    //    //this->setWindowFlags(Qt::FramelessWindowHint|Qt::Dialog);this->setStyleSheet("QDialog{border:2px solid gray}");
    da_xiao = false;
    tui_cai = false;
    cui_cai = false;
    luo_dan = false;
    init_control();
    bendandanhao = w_scr_dish_restaurant_dialog::table_danhao;
    leiji = 0;
    row_n = 0;
    num_da = 0;
    num_xiao = 0;
    num_cai = 0;
    changyongcook = false;
    bianma_daicai_type = false;
    currentGoodItem_zilei = NULL;
    ui->tableView->setModel(&modelBase);

    /**
     * ---------------------------------------------------begin*/
    {
        lds_menu *menu=new lds_menu;
        QWidgetAction *widgetaction=new QWidgetAction(ui->pushButton_other);
        widgetaction->setDefaultWidget(ui->frame_other_menu);
        menu->addAction(widgetaction);
        ui->pushButton_other->setMenu(menu);
    }
    {
        lds_menu *menu=new lds_menu;
        QWidgetAction *widgetaction=new QWidgetAction(ui->pushButton_bianma);
        bianma_cook *bianma = new bianma_cook(menu);
        bianma->setFixedSize(500,300);
        widgetaction->setDefaultWidget(bianma);
        menu->addAction(widgetaction);
        ui->pushButton_bianma->setMenu(menu);
        connect(bianma,SIGNAL(edit_text(QString)),this,SLOT(bianma_diancai(QString)));
    }

    connect(ui->widget,SIGNAL(signal1_text(QString)),this,SLOT(init_xiaolei(QString)));
    connect(ui->widget,SIGNAL(signal2_text(QString)),this,SLOT(init_cook(QString)));
    connect(ui->widget,SIGNAL(signal3_text(QString)),this,SLOT(addGood(QString)));


    connect(ui->pushButton_allcuicai,SIGNAL(clicked()),this,SLOT(on_pushButton_8_clicked()));
    connect(ui->pushButton_canceltable,SIGNAL(clicked()),this,SLOT(on_pushButton_14_clicked()));
    connect(ui->pushButton_change,SIGNAL(clicked()),this,SLOT(on_pushButton_6_clicked()));
    connect(ui->pushButton_changyong,SIGNAL(clicked()),this,SLOT(on_pushButton_clicked()));
    connect(ui->pushButton_cook,SIGNAL(clicked()),this,SLOT(on_pushButton_5_clicked()));

    connect(ui->pushButton_cuicai,SIGNAL(clicked()),this,SLOT(on_pushButton_7_clicked()));
    connect(ui->pushButton_quitdish,SIGNAL(clicked()),this,SLOT(on_pushButton_11_clicked()));
    //    connect(ui->pushButton_,SIGNAL(clicked()),this,SLOT(on_pushButton_13_clicked()));
    connect(ui->pushButton_discount,SIGNAL(clicked()),this,SLOT(on_pushButton_9_clicked()));
    connect(ui->pushButton_dishswitch,SIGNAL(clicked()),this,SLOT(on_pushButton_15_clicked()));

    connect(ui->pushButton_add,SIGNAL(clicked()),this,SLOT(on_pushButton_2_clicked()));
    connect(ui->pushButton_sub,SIGNAL(clicked()),this,SLOT(on_pushButton_3_clicked()));
    connect(ui->pushButton_present,SIGNAL(clicked()),this,SLOT(on_pushButton_10_clicked()));
    connect(ui->pushButton_num,SIGNAL(clicked()),this,SLOT(on_pushButton_4_clicked()));
    connect(ui->pushButton_tableswitch,SIGNAL(clicked()),this,SLOT(on_pushButton_16_clicked()));

    connect(ui->pushButton_dishtmp,SIGNAL(clicked()),this,SLOT(on_pushButton_12_clicked()));
    connect(ui->pushButton_prebill,SIGNAL(clicked()),this,SLOT(on_pushButton_24_clicked()));
    //    connect(ui->pushButton_,SIGNAL(clicked()),this,SLOT(on_pushButton_20_clicked()));
    connect(ui->pushButton_okbill,SIGNAL(clicked()),this,SLOT(on_pushButton_23_clicked()));
    connect(ui->pushButton_paybill,SIGNAL(clicked()),this,SLOT(on_pushButton_21_clicked()));

    connect(ui->pushButton_quit,SIGNAL(clicked()),this,SLOT(on_pushButton_22_clicked()));
    connect(ui->lineEdit_scan,SIGNAL(returnPressed()),this,SLOT(to_scanbar()));
    connect(ui->widget,SIGNAL(signal3_refresh()),this,SLOT(init_cook()));
    connect(ui->tableView->tableview,SIGNAL(clicked(QModelIndex)),this,SLOT(on_tableView_clicked(QModelIndex)));
    //老的初始化数据
    init_model();
    init_dalei();
    init_xiaolei(dalei);
    init_cook(xiaolei);
    init_bedi_db();
    print_pr = new print_prompt;
    print_pr->setGeometry(0,0,print_pr->width(),print_pr->height());
    thread = new print_threqd;
    connect(thread,SIGNAL(print_lost_thread()),this,SLOT(thread_signal()));
    print_timer = new QTimer();
    connect(print_timer, SIGNAL(timeout()), this, SLOT(print_louda()));
    print_timer->setInterval(1000);
}

table_order_2::~table_order_2()
{
    delete ui;
}

void table_order_2::to_dish_title(float num, float total)
{
    if(num>=0){ui->label_num->setText(tr("数量")+QString().sprintf("%.03f", num));_num=num;}
    if(total>=0){ui->label_total->setText(tr("金额")+QString().sprintf("%.02f", total));_total=total;}
}

void table_order_2::set_num_total_to_doublescreen()
{
    if(public_sql::save_login->double_screen){
        connect(ui->label_num,SIGNAL(textChanged(QString)),public_sql::save_login->double_screen->ui->label_num,SLOT(setText(QString)));
        connect(ui->label_total,SIGNAL(textChanged(QString)),public_sql::save_login->double_screen->ui->label_total,SLOT(setText(QString)));
    }
}

void table_order_2::to_dish_title_other(const QString &num, const QString &total)
{
    if(!num.isEmpty())ui->label_num->setText(num);
    if(!total.isEmpty())ui->label_total->setText(total);
}
void table_order_2::to_grade0(const QList<frontViewDialog_autoexpand::content_item_icon> &item_icon_list)
{
    ui->widget->to_grade0(item_icon_list);
}

void table_order_2::to_grade1(const QList<frontViewDialog_autoexpand::content_item_icon> &item_icon_list)
{
    ui->widget->to_grade1(item_icon_list);
}

void table_order_2::to_grade2(const QList<frontViewDialog_autoexpand::content_item_icon> &item_icon_list)
{
    ui->widget->to_grade2(item_icon_list);
}

float table_order_2::get_cur_num()
{
    return _num;
}

void table_order_2::to_scanbar()
{
    if(ui->lineEdit_scan->text().isEmpty()) return;
    addGood(ui->lineEdit_scan->text());
    ui->lineEdit_scan->clear();
}

void table_order_2::torefresh()
{
    QSqlDatabase::database();
    init_pay_model();
    QStringList daleis=init_dalei();
    QStringList xiaoleis=init_xiaolei(daleis);
    init_cook(xiaoleis);
}
/*---------------------------------------------------end*/
void table_order_2::on_pushButton_22_clicked()
{
    ui->pushButton_other->menu()->hide();

    QSqlQuery query;
    QSqlDatabase::database().transaction();
    int k = 0;
    for(int i = 0; i < modelBase.rowCount(); i++)
    {
        if(modelBase.item(i,24)->text() == "N")
        {
            k++;
        }
    }

    if(k > 0)
    {
        if(0==lds_messagebox::warning(this, tr("消息提示"),tr("点菜信息有更改,是否落单?"),tr("确认"),tr("取消"))){
            if(!toluodan()) return;
        }
    }

    query.prepare(QString("update cey_bt_table set ch_lockflag ='N' where ch_tableno =:table "));
    query.bindValue(":table",frontViewDialog_virtual::cur_desk_no);
    if(query.exec()){

    } else {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_bt_table失败!"));
        return;
    }
    QSqlDatabase::database().commit();
    this->close();
}

void table_order_2::init_model()
{
    model_dalei.setQuery(QString("SELECT  * FROM cey_bt_dish_type"));
    dalei = model_dalei.record(0).value(0).toString();

    model_xiaolei.setQuery(QString("SELECT  * FROM cey_bt_dish_subtype where ch_dish_typeno = '%1' ").arg(dalei));
    xiaolei = model_xiaolei.record(0).value(0).toString();

    model_cook.setQuery(QString("SELECT  * FROM cey_bt_dish where ch_dish_typeno = '%1' and ch_sub_typeno ='%2' and (ch_stopflag = null or ch_stopflag = 'N') and ch_tempflag = 'N'  ").arg(dalei).arg(xiaolei));
}

void table_order_2::init_control()
{
    QSqlQueryModel model;
    model.setQuery(QString("Select ch_billno,ch_tableno,int_person,vch_memo,ch_state,ch_payno,vch_operID,dt_operdate,vch_operID2,dt_operdate2,ch_order_state,num_cost,vch_waiter,(select IFNULL(vch_operator,'') from  cey_sys_operator where vch_operID = cey_u_table.vch_waiter)  ,(select IFNULL(vch_tablename,'') from cey_bt_table where ch_tableno = cey_u_table.ch_tableno) from cey_u_table where cey_u_table.ch_billno = '%1'  and cey_u_table.ch_tableno = '%2'").arg(w_scr_dish_restaurant_dialog::table_danhao).arg(frontViewDialog_virtual::cur_desk_no));

    ui->label_billno->setText(model.record(0).value(0).toString());
    ui->label_deskno->setText(model.record(0).value(1).toString());
    ui->label_opentableman->setText(model.record(0).value(6).toString());
    ui->label_servant->setText(model.record(0).value(12).toString());
    ui->label_personnum->setText(model.record(0).value(2).toString());
    frontViewDialog_virtual::cur_desk_no=ui->label_deskno->text();
    //    QString str;
    //    str = tr("开台单号:") + model.record(0).value(0).toString();
    //    ui->label_6->setText(str);

    //    str.clear();
    //    str = tr("餐桌号:") + model.record(0).value(1).toString() + tr("    开台人:") + model.record(0).value(6).toString() ;
    //    ui->label_7->setText(str);

    //    str.clear();
    //    str = tr("开台时间:") + model.record(0).value(7).toDateTime().toString("yyyy-MM-dd hh:mm:ss");
    //    ui->label_10->setText(str);

    //    str.clear();
    //    str = tr("服务员:") + model.record(0).value( 12).toString() + tr("     人数:") + model.record(0).value(2).toString();
    //    ui->label_11->setText(str);

    k_danhao = model.record(0).value(0).toString();
    k_zhuohao = model.record(0).value(1).toString();
    k_renshu = model.record(0).value(2).toString();
    k_kaitairen = model.record(0).value(6).toString();
    k_time = model.record(0).value(7).toDateTime().toString("yyyy-MM-dd hh:mm:ss");
    k_fuwuyuan = model.record(0).value( 12).toString();
    k_beizhu = model.record(0).value(3).toString();

    modelBase.removeRows(0, modelBase.rowCount());
    //    ui->tableView->horizontalHeader()->setStyleSheet("QHeaderView::section{background-color:#F4B206;color:white;height:35px;border-radius:0px;font-size:14pt;}");
    //    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    //    ui->tableView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //    ui->tableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //    ui->tableView->verticalHeader()->setDefaultSectionSize(50);
    //    //ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置属性为只读//351
    //    //Tableview_delegate *delegate = new Tableview_delegate();
    //    //ui->tableView->setItemDelegate(delegate);
    //    //ui->tableView->setStyleSheet("selection-background-color: rgba(255,255,255,0);");
    //    // ui->tableView->setStyleSheet("selection-background-color:#CCCCCC;");
    //    // ui->tableView->setStyleSheet("selection-background-color: transparent;");
    //    ui->tableView->setModel(modelBase);
    //    ui->tableView->setShowGrid(false);
    //    ui->tableView->setFocusPolicy(Qt::NoFocus);
    //    ui->tableView->verticalHeader()->hide();
    //    connect(ui->tableView,SIGNAL(clicked(QModelIndex)),this,SLOT(show_tableview_click(QModelIndex)));

    //    ui->tableView->setColumnWidth(0,35);//序号
    //    ui->tableView->setColumnWidth(2,189);//消费项目
    //    ui->tableView->setColumnWidth(13,68);//赠送

    //    ui->tableView->horizontalHeader()->setResizeMode(0,QHeaderView::Fixed);
    //    ui->tableView->horizontalHeader()->setResizeMode(1,QHeaderView::Fixed);
    //    ui->tableView->horizontalHeader()->setResizeMode(2,QHeaderView::Fixed);
    QSqlQueryModel type;
    model.clear();
    model.setQuery(QString("select a.ch_billno, a.ch_tableno, a.int_id,a.ch_dishno, a.num_price, a.num_price_org,a.num_price_add, a.num_num, a.num_back,vch_back_operID, a.dt_back_operdate,a.int_discount,a.vch_dis_operID, a.dt_dis_operdate, a.vch_memberno,a.ch_consult, a.ch_printflag, a.vch_print_memo,a.ch_suitflag, a.ch_suitno, a.ch_specialflag,a.ch_presentflag, a.vch_pre_operID,a.dt_pre_operdate, a.vch_operID, a.dt_operdate,a.ch_togono,b.vch_dishname,c.vch_unitname,b.num_price,c.num_default,b.ch_discount,b.ch_curprice,a.int_flowID ,d.vch_dish_typename,e.vch_sub_typename,b.vch_printer,b.vch_outprint,a.ch_outflag,b.ch_outflag,b.ch_printflag,b.int_rate from cey_u_orderdish a,cey_bt_dish b, cey_bt_unit c,cey_bt_dish_type d,cey_bt_dish_subtype e where  a.ch_dishno=b.ch_dishno and b.ch_unitno=c.ch_unitno and b.ch_sub_typeno = e.ch_sub_typeno and b.ch_dish_typeno = d.ch_dish_typeno and a.ch_billno = '%1' and a.ch_tableno ='%2'").arg(w_scr_dish_restaurant_dialog::table_danhao).arg(frontViewDialog_virtual::cur_desk_no));
    qDebug() << "---------------" << model.query().lastQuery();
    for(int i =0; i< model.rowCount(); i++)
    {
        int row = modelBase.rowCount();
        modelBase.setRowCount(row + 1);
        modelBase.setItem(row,0,new QStandardItem(QString::number(row+1))); //序号
        modelBase.setItem(row,1,new QStandardItem(model.record(i).value(3).toString())); //编码
        modelBase.setItem(row,2,new QStandardItem(model.record(i).value(27).toString())); //品名
        float num = 0.00;
        num = model.record(i).value(7).toFloat()- model.record(i).value(8).toFloat();
        modelBase.setItem(row,3,new QStandardItem(QString().sprintf("%0.2f",num))); //数量
        modelBase.setItem(row,4,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(4).toFloat()))); //单价
        modelBase.setItem(row,5,new QStandardItem(model.record(i).value(11).toString())); //折扣值
        float zongjia = 0.00;
        zongjia  = model.record(i).value(4).toFloat() * (model.record(i).value(7).toFloat()- model.record(i).value(8).toFloat())* model.record(i).value(11).toFloat() * 0.01 +  (model.record(i).value(7).toFloat()- model.record(i).value(8).toFloat()) * model.record(i).value(42).toFloat() *0.01 + (model.record(i).value(7).toFloat()- model.record(i).value(8).toFloat()) * model.record(i).value(6).toFloat();
        modelBase.setItem(row,6,new QStandardItem(QString().sprintf("%0.2f",zongjia))); //总价
        modelBase.setItem(row,7,new QStandardItem(model.record(i).value(17).toString())); //特殊做法
        modelBase.setItem(row,8,new QStandardItem(model.record(i).value(21).toString())); //是否赠送
        modelBase.setItem(row,9,new QStandardItem(model.record(i).value(18).toString())); //是否套菜
        modelBase.setItem(row,10,new QStandardItem(model.record(i).value(20).toString())); //是否特价
        modelBase.setItem(row,11,new QStandardItem("N")); //是否优惠
        modelBase.setItem(row,12,new QStandardItem(model.record(i).value(28).toString())); //单位
        modelBase.setItem(row,13,new QStandardItem(model.record(i).value(22).toString())); //赠送人
        modelBase.setItem(row,14,new QStandardItem(model.record(i).value(12).toString())); //定折人
        modelBase.setItem(row,15,new QStandardItem(model.record(i).value(15).toString())); //是否时价菜
        if(model.record(i).value(18).toString() == "Y")
        {
            modelBase.setItem(row,16,new QStandardItem(model.record(i).value(3).toString())); //子套菜所在主套菜编码
        }
        else
        {
            modelBase.setItem(row,16,new QStandardItem("")); //子套菜所在主套菜编码?
        }
        modelBase.setItem(row,17,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(42).toFloat())));//税点
        modelBase.setItem(row,18,new QStandardItem(model.record(i).value(31).toString()));//是否折扣
        modelBase.setItem(row,19,new QStandardItem(model.record(i).value(13).toDateTime().toString("yyyy-MM-dd hh:mm:ss")));//折扣时间
        modelBase.setItem(row,20,new QStandardItem(model.record(i).value(23).toDateTime().toString("yyyy-MM-dd hh:mm:ss")));//赠送时间
        float zuofajiage = 0.00;
        zuofajiage = model.record(i).value(6).toFloat();
        modelBase.setItem(row,21,new QStandardItem(QString().sprintf("%0.2f",zuofajiage)));//做法加价
        type.clear();
        type.setQuery(QString("select ch_dish_typeno from cey_bt_dish_type where vch_dish_typename = '%1' ").arg(model.record(i).value(34).toString()));
        modelBase.setItem(row,22,new QStandardItem(type.record(0).value(0).toString())); //所属大类编码
        type.clear();
        type.setQuery(QString("select ch_sub_typeno from cey_bt_dish_subtype where vch_sub_typename = '%1' ").arg(model.record(i).value(35).toString()));
        modelBase.setItem(row,23,new QStandardItem(type.record(0).value(0).toString())); //所属小类编码
        modelBase.setItem(row,24,new QStandardItem("Y")); //是否落单
        modelBase.setItem(row,25,new QStandardItem(model.record(i).value(33).toString()));//落单FLOWID
        modelBase.setItem(row,26,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(8).toFloat())));//已退量
    }

    //cook_time = new QDateTime();
    if(model.rowCount() > 0)
    {
        cook_time = model.record(0).value(25).toDateTime();
        back_diancaitime = model.record(0).value(25).toDateTime().toString("yyyy-MM-dd hh:mm:ss");
    }

    QSqlQueryModel thistime;
    thistime.setQuery(QString("Select NOW()"));
    diancaitime.clear();
    diancaitime = thistime.record(0).value(0).toDateTime().toString("yyyy-MM-dd hh:mm:ss");

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

    this->to_dish_title(label_shuliang, label_jine);
    //    ui->label_3->setText("￥:" + QString().sprintf("%0.2f",label_jine));
    //    ui->label_5->setText(QString().sprintf("%0.2f",label_shuliang));

    if(modelBase.rowCount() > 0)
    {
        ui->pushButton_quitdish->setText(tr("退菜"));
    }
    else
    {
        ui->pushButton_quitdish->setText(tr("删菜"));
    }
}

QStringList table_order_2::init_dalei()
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

void table_order_2::init_xiaolei(const QString &daleino)
{
    dalei= daleino;
    model_xiaolei.clear();
    model_xiaolei.setQuery(QString("SELECT  * FROM cey_bt_dish_subtype where ch_dish_typeno = '%1' ").arg(daleino));
    QList<frontViewDialog_autoexpand::content_item_icon> item_icon_list;

    for(int i = 0; i < model_xiaolei.rowCount(); i++)
    {
        item_icon_list.append(frontViewDialog_autoexpand::content_item_icon(model_xiaolei.record(i).value("ch_sub_typeno").toString(), model_xiaolei.record(i).value("vch_sub_typename").toString()));
    }
    this->to_grade1(item_icon_list);
    init_cook("");
}
QStringList table_order_2::init_xiaolei(const QStringList &daleinos)
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

void table_order_2::init_cook(const QString &xiaoleino, const QString &filter)
{
    ui->widget->widget3_tofresh();
    xiaolei=xiaoleino;
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

void table_order_2::init_cook(const QStringList &xiaoleinos)
{
    ui->widget->widget3_setsql("select ch_dishno,concat(vch_dishname,'\n',num_price) from cey_bt_dish where (ch_stopflag = null or ch_stopflag = 'N') ");
    ui->widget->widget3_start();
}

void table_order_2::init_pay_model()
{

    //   model_dalei.setQuery(QString("SELECT  * FROM cey_bt_dish_type"));
    //    for(int i = 0; i < model_dalei.rowCount(); i++)
    //    {
    //        if(i == num_da)
    //        {
    //            dalei = model_dalei.record(i).value(0).toString();
    //        }
    //    }

    // model_xiaolei.clear();
    //   model_xiaolei.setQuery(QString("SELECT  * FROM cey_bt_dish_subtype where ch_dish_typeno = '%1' ").arg(dalei));
    //    for(int i = 0; i < model_xiaolei.rowCount(); i++)
    //    {
    //        if(i == num_xiao)
    //        {
    //            xiaolei = model_xiaolei.record(0).value(0).toString();
    //        }
    //    }


    //    if(changyongcook == false)
    //    {
    //        model_cook.setQuery(QString("SELECT  * FROM cey_bt_dish where ch_dish_typeno = '%1' and ch_sub_typeno ='%2' and (ch_stopflag = null or ch_stopflag = 'N')  ").arg(dalei).arg(xiaolei));
    //    }
    //    else
    //    {
    //        model_cook.setQuery(QString("select * from cey_bt_dish where ch_off = 'Y' and (ch_stopflag = null or ch_stopflag = 'N')   "));
    //    }
}

void table_order_2::init_tuidan_data()
{
    QSqlQueryModel model_l;
    model_l.setQuery("Select NOW()");
    QString time = model_l.record(0).value(0).toDateTime().toString("yyyy-MM-dd hh:mm:ss");

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


void table_order_2::show_tableview_click(const QModelIndex &index)
{
    //    row_n = ui->tableView->currentIndex().row();// ui->tableView->currentIndex().row();
    //    ui->tableView->viewport()->update();
}

void table_order_2::change_label()
{
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
    this->to_dish_title(label_shuliang, label_jine);
}

void table_order_2::showxiaolei(const QString &str)
{
    //    if(str.trimmed().isEmpty())
    //    {
    //        return;
    //    }
    //    changyongcook = false;
    //    dalei = str;
    //    qDeleteAll(itemList_zilei.begin(),itemList_zilei.end());
    //    itemList_zilei.clear();
    //    qDeleteAll(ui->frame->children());
    //    layout_zilei = new QGridLayout();
    //    for(int i = 0; i < item_count_zilei; ++i)
    //    {
    //        restaurant_gooditem_zilei *item = new restaurant_gooditem_zilei("","");
    //        itemList_zilei.append(item);
    //        connect(item,SIGNAL(touch_zilei(QString)),this,SLOT(dis_xiaolei_str(QString)));
    //        layout_zilei->addWidget(item,i/item_count_row_zilei,i%item_count_row_zilei);
    //    }
    //    layout_zilei->setContentsMargins(0,6,0,0);
    //    layout_zilei->setSpacing(2);
    //    ui->frame->setLayout(layout_zilei);

    //    model_xiaolei.clear();
    //    model_xiaolei.setQuery(QString("SELECT  * FROM cey_bt_dish_subtype where ch_dish_typeno = '%1' ").arg(dalei));

    //    xiaolei = model_xiaolei.record(0).value(0).toString();
    //    QString kind = model_xiaolei.record(0).value(0).toString();
    //    showGoods_zilei(kind);
}

bool table_order_2::showGoods(QString kindId, int index)
{
    //    //changyongcook = false;
    //    if(kindId.trimmed().isEmpty())
    //    {
    //        return false;
    //    }
    //    goodIndex = index;
    //    goodKind = kindId;
    //    int preCount;//之前的商品数量

    //    if(0 == index)
    //    {
    //        preCount = 0;
    //    }
    //    else
    //    {
    //        preCount = item_count-1 + (index-1)*(item_count-2);
    //    }

    //    if(bianma_daicai_type == false && changyongcook == false)
    //    {
    //
    //        model_cook.setQuery(QString("SELECT  * FROM cey_bt_dish where ch_dish_typeno = '%1' and ch_sub_typeno ='%2' and (ch_stopflag = null or ch_stopflag = 'N') and ch_tempflag ='N' limit %3,%4 ").arg(dalei).arg(xiaolei).arg(preCount).arg(item_count+1));
    //        //qDebug() << QString("SELECT  * FROM cey_bt_dish where ch_dish_typeno = '%1' and ch_sub_typeno ='%2' and (ch_stopflag = null or ch_stopflag = 'N') limit %3,%4 ").arg(dalei).arg(xiaolei).arg(preCount).arg(item_count+1);
    //    }

    //    if(bianma_daicai_type == true  && changyongcook == false)
    //    {
    //        QString edit_bianma = ("%") + bianma_str + ("%");
    //
    //        model_cook.setQuery(QString("SELECT  * FROM cey_bt_dish where (ch_stopflag = null or ch_stopflag = 'N') and (ch_dishno like '%1' or vch_spell like '%2') and ch_tempflag ='N'   limit %3,%4")
    //                             .arg(edit_bianma,
    //                                  edit_bianma,
    //                                  QString::number(preCount),
    //                                  QString::number(item_count+1)));
    //    }

    //    if(bianma_daicai_type == false && changyongcook == true)
    //    {
    //
    //        model_cook.setQuery(QString("select * from cey_bt_dish where ch_off ='Y' and (ch_stopflag = null or ch_stopflag = 'N') and ch_tempflag ='N' limit %1,%2").arg(preCount).arg(item_count+1));
    //    }


    //    int count = 0;
    //    if(0 == index)       //如果是第一页
    //    {
    //        for(int i = 0; i < model_cook.rowCount(); i++)
    //        {
    //            if(count < item_count)      //填如所有商品到列表//子商品
    //            {
    //                itemList.value(count)->_barcode = model_cook.record(i).value(0).toString();
    //                itemList.value(count)->_isSelect = false;
    //                itemList.value(count)->setName(model_cook.record(i).value(1).toString() + "\n" + QString().sprintf("%0.2f",model_cook.record(i).value(6).toFloat()));
    //            }
    //            ++count;
    //        }
    //        if(count < item_count)
    //        {
    //            //如果商品不足列表则填满
    //            for(;count < item_count;++count)
    //            {
    //                itemList.value(count)->_barcode = "";
    //                itemList.value(count)->_isSelect = false;
    //                itemList.value(count)->setName("");
    //            }
    //        }
    //        else if(count == item_count)
    //        {
    //            //如果等于列表数量则完全填满
    //        }
    //        else if(count > item_count)
    //        {
    //            //如果大于则代表有下一页，最后一个商品设置为下一页
    //            itemList.value(itemList.count()-1)->_barcode = "next";
    //            itemList.value(itemList.count()-1)->_isSelect = false;
    //            itemList.value(itemList.count()-1)->setName(">>>");
    //        }
    //    }
    //    else if(index > 0)   //不是第一页
    //    {
    //        //添加上一页
    //        itemList.value(0)->_barcode = "pre";
    //        itemList.value(0)->_isSelect = false;
    //        itemList.value(0)->setName("<<<");

    //        for(int i = 0; i < model_cook.rowCount(); i++)
    //        {
    //            if(count < item_count-1)      //填如之后所有商品到列表
    //            {
    //                itemList.value(count+1)->_barcode = model_cook.record(i).value(0).toString();
    //                itemList.value(count+1)->_isSelect = false;
    //                itemList.value(count+1)->setName(model_cook.record(i).value(1).toString() + "\n" + QString().sprintf("%0.2f",model_cook.record(i).value(6).toFloat()));
    //            }
    //            ++count;
    //        }

    //        if(count < item_count - 1)  //如果商品不足剩下的
    //        {
    //            //如果商品不足列表则填满
    //            for(;count < item_count-1;++count)
    //            {
    //                itemList.value(count+1)->_barcode = "";
    //                itemList.value(count+1)->_isSelect = false;
    //                itemList.value(count+1)->setName("");
    //            }
    //        }
    //        else if(count == item_count - 1)
    //        {
    //            //如果刚好后下面的则满上
    //        }
    //        else if(count > item_count - 1)
    //        {
    //            //如果大于则代表有下一页，最后一个商品设置为下一页
    //            itemList.value(itemList.count()-1)->_barcode = "next";
    //            itemList.value(itemList.count()-1)->_isSelect = false;
    //            itemList.value(itemList.count()-1)->setName(">>>");
    //        }
    //    }
    return true;
}

bool table_order_2::showGoods_zilei(QString kindId, int index)
{
    //    if(kindId.isEmpty())
    //        return false;
    //    //保存当前页的信息
    //    goodIndex_xiaolei = index;
    //    goodKind_xiaolei = kindId;
    //    int preCount;//之前的商品数量
    //    if(0 == index)
    //    {
    //        preCount = 0;
    //    }
    //    else
    //    {
    //        preCount = item_count_zilei-1 + (index-1)*(item_count_zilei-2);
    //    }
    //    model_xiaolei.clear();
    //    model_xiaolei.setQuery(QString("SELECT  * FROM cey_bt_dish_subtype where ch_dish_typeno = '%1' limit %2,%3 ").arg(dalei).arg(preCount).arg(item_count_zilei+1));

    //    int count = 0;
    //    if(0 == index)       //如果是第一页
    //    {
    //        for(int i = 0; i < model_xiaolei.rowCount(); i++)
    //        {
    //            if(count < item_count_zilei)      //填如所有商品到列表
    //            {
    //                itemList_zilei.value(count)->_barcode = model_xiaolei.record(i).value(0).toString();
    //                itemList_zilei.value(count)->_isSelect = false;
    //                itemList_zilei.value(count)->setName(model_xiaolei.record(i).value(1).toString());
    //            }
    //            ++count;
    //        }
    //        if(count < item_count_zilei)
    //        {
    //            //如果商品不足列表则填满
    //            for(;count < item_count_zilei;++count)
    //            {
    //                itemList_zilei.value(count)->_barcode = "";
    //                itemList_zilei.value(count)->_isSelect = false;
    //                itemList_zilei.value(count)->setName("");
    //            }
    //        }
    //        else if(count == item_count_zilei)
    //        {
    //            //如果等于列表数量则完全填满
    //        }
    //        else if(count > item_count_zilei)
    //        {
    //            //如果大于则代表有下一页，最后一个商品设置为下一页
    //            itemList_zilei.value(itemList_zilei.count()-1)->_barcode = "next";
    //            itemList_zilei.value(itemList_zilei.count()-1)->_isSelect = false;
    //            itemList_zilei.value(itemList_zilei.count()-1)->setName(tr("下一页"));
    //        }
    //    }
    //    else if(index > 0)   //不是第一页
    //    {
    //        //添加上一页
    //        itemList_zilei.value(0)->_barcode = "pre";
    //        itemList_zilei.value(0)->_isSelect = false;
    //        itemList_zilei.value(0)->setName(tr("上一页"));

    //        for(int i = 0; i < model_xiaolei.rowCount(); i++)
    //        {
    //            if(count < item_count_zilei-1)      //填入之后所有商品到列表
    //            {
    //                itemList_zilei.value(count+1)->_barcode = model_xiaolei.record(i).value(0).toString();
    //                itemList_zilei.value(count+1)->_isSelect = false;
    //                itemList_zilei.value(count+1)->setName(model_xiaolei.record(i).value(1).toString());
    //            }
    //            ++count;
    //        }

    //        if(count < item_count_zilei - 1)  //如果商品不足剩下的
    //        {
    //            //如果商品不足列表则填满
    //            for(;count < item_count_zilei-1;++count)
    //            {
    //                itemList_zilei.value(count+1)->_barcode = "";
    //                itemList_zilei.value(count+1)->_isSelect = false;
    //                itemList_zilei.value(count+1)->setName("");
    //            }
    //        }
    //        else if(count == item_count_zilei - 1)
    //        {
    //            //如果刚好后下面的则满上
    //        }
    //        else if(count > item_count_zilei - 1)
    //        {
    //            //如果大于则代表有下一页，最后一个商品设置为下一页
    //            itemList_zilei.value(itemList_zilei.count()-1)->_barcode = "next";
    //            itemList_zilei.value(itemList_zilei.count()-1)->_isSelect = false;
    //            itemList_zilei.value(itemList_zilei.count()-1)->setName(tr("下一页"));
    //        }
    //    }
    return true;
}

void table_order_2::addGood(const QString &barcode)
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
    frontViewDialog_virtual::addGood(row_n);

    //    if(!barscaleflag/*不是条码商品*/
    //            &&isweight_flag/*是称重商品*/){
    //        on_pushButton_13_clicked();
    //    }
}

void table_order_2::dis_xiaolei_str(const QString &barcode)
{
    //    if(barcode.isEmpty())
    //        return;
    //    changyongcook = false;
    //    if(barcode != "pre" && barcode != "next")
    //    {
    //        xiaolei = barcode;
    //    }
    //    if(barcode == "pre")
    //    {
    //        if(goodIndex_xiaolei <= 0)
    //            return;

    //        --goodIndex_xiaolei;
    //        showGoods_zilei(goodKind_xiaolei,goodIndex_xiaolei);
    //        return;
    //    }
    //    else if(barcode == "next")
    //    {
    //        ++goodIndex_xiaolei;
    //        if(!showGoods_zilei(goodKind_xiaolei,goodIndex_xiaolei))
    //        {
    //            --goodIndex_xiaolei;
    //        }
    //        return;
    //    }
    //    if(!currentGoodItem_zilei.isNull() && currentGoodItem_zilei != sender())
    //        currentGoodItem_zilei->setSelect(false);

    //    if(sender() != NULL)
    //        currentGoodItem_zilei = static_cast<restaurant_gooditem_zilei*>(sender());

    //    bianma_daicai_type = false;
    //    itemList.clear();
    //    qDeleteAll(itemList.begin(),itemList.end());
    //    qDeleteAll(ui->widget_3->children());
    //    layout_cook = new QGridLayout();
    //    for(int i = 0; i < item_count; ++i)
    //    {
    //        GoodItem *item = new GoodItem("","");
    //        itemList.append(item);
    //        connect(item,SIGNAL(touch(QString)),this,SLOT(addGood(QString)));
    //        layout_cook->addWidget(item,i/item_count_row,i%item_count_row);
    //    }
    //    layout_cook->setContentsMargins(0,0,0,0);
    //    layout_cook->setSpacing(2);
    //    ui->widget_3->setLayout(layout_cook);
    //
    //    model_cook.setQuery(QString("select * from cey_bt_dish where ch_off ='Y' and (ch_stopflag = null or ch_stopflag = 'N') and ch_tempflag ='N'  "));
    //    QString kind = model_xiaolei.record(0).value(0).toString();
    //    showGoods(kind);
}

void table_order_2::on_pushButton_11_clicked()//删菜/退菜
{

    int row = ui->tableView->tableview->currentIndex().row();// ui->tableView->currentIndex().row();// ui->tableView->currentIndex().row();
    QSqlQuery query;
    QSqlDatabase::database().transaction();

    if(ui->pushButton_quitdish->text() == tr("删菜"))
    {
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
            if(modelBase.rowCount() > 1 && modelBase.rowCount() == row)
            {
                //                ui->tableView->selectRow(row - 1);
            }
            else if(modelBase.rowCount() > 1 && modelBase.rowCount() > row)
            {
                //                ui->tableView->selectRow(row);
            }
            else if(modelBase.rowCount() == 1)
            {
                //                ui->tableView->selectRow(0);
            }
            QSqlQueryModel tao;
            tao.setQuery(QString("Select * from cey_bt_dish_suit where ch_suitno ='%1'").arg(bianma));
            for(int i = 0; i < tao.rowCount(); i++)
            {
                modelBase.removeRow(row);

                if(modelBase.rowCount() > 1 && modelBase.rowCount() == row)
                {
                    //                    ui->tableView->selectRow(row - 1);
                }
                else if(modelBase.rowCount() > 1 && modelBase.rowCount() > row)
                {
                    //                    ui->tableView->selectRow(row);
                }
                else if(modelBase.rowCount() == 1)
                {
                    //                    ui->tableView->selectRow(0);
                }
            }
        }
        else
        {
            modelBase.removeRow(row);
            if(modelBase.rowCount() > 1 && modelBase.rowCount() == row)
            {
                //                ui->tableView->selectRow(row - 1);
            }
            else if(modelBase.rowCount() > 1 && modelBase.rowCount() > row)
            {
                //                ui->tableView->selectRow(row);
            }
            else if(modelBase.rowCount() == 1)
            {
                //                ui->tableView->selectRow(0);
            }
        }
    }
    else//退菜
    {
        if(modelBase.item(row,3)->text().toFloat() == 0)
        {
            lds_messagebox::warning(this, tr("消息提示"),tr("指定点菜数量为零,不能退菜!"));
            
            return;
        }
        QSqlQueryModel model;
        model.setQuery(QString("select NOW()"));
        QString date = model.record(0).value(0).toDateTime().toString("yyyy-MM-dd");
        QString time = model.record(0).value(0).toDateTime().toString("yyyy-MM-dd hh:mm:ss");
        QString name = modelBase.item(row,2)->text() + "(" + modelBase.item(row,1)->text() + ")";
        QString no = modelBase.item(row,1)->text();
        back_cook back(name,no,this);
        backheader::static_dialogexec(back, "");
        if(back.bakc_one == true && back.back_all == false)//退一个
        {
            tui_cai = true;
            query.prepare(QString("update cey_u_orderdish  set num_back = :num, vch_back_operID =:oper , dt_back_operdate =:time , ch_printflag ='N'  where int_flowId =:id"));
            query.bindValue(":num",QString().sprintf("%0.2f",back.back_num + modelBase.item(row,26)->text().toFloat()));
            query.bindValue(":oper",n_func::gs_operid);
            query.bindValue(":time",time);
            query.bindValue(":id",modelBase.item(row,25)->text());
            backone = QString().sprintf("%0.2f",back.back_num);
            if(query.exec())
            {
            }
            else
            {
                QSqlDatabase::database().rollback();
                lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_u_orderdish失败!"));
                
                return;
            }

            query.prepare(QString("update cey_bt_dish_warn  Set num_sale ='0.0'  where ch_dishno =:bianma and ch_state ='Y' and left( dt_date , 10)= :date"));
            query.bindValue(":bianma",modelBase.item(row,1)->text());
            query.bindValue(":date",date);
            if(query.exec())
            {
            }
            else
            {
                QSqlDatabase::database().rollback();
                lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_bt_dish_warn失败!"));
                
                return;
            }

            query.prepare(QString("Insert into cey_u_orderdish_log(int_orderflow , ch_type , vch_impower , vch_operID , dt_operdate , vch_reason)values(:id,'1',:oper1,:oper2,:time,:yuanyin)"));
            query.bindValue(":id",modelBase.item(row,25)->text());
            query.bindValue(":oper1",n_func::gs_operid);
            query.bindValue(":oper2",n_func::gs_operid);
            query.bindValue(":time",time);
            query.bindValue(":yuanyin",back.cook_causes);
            if(query.exec())
            {
            }
            else
            {
                QSqlDatabase::database().rollback();
                qDebug() << query.lastError();  lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_u_orderdish_log失败!"));
                
                return;
            }

            model.clear();
            model.setQuery(QString("select sum((case when ch_presentflag ='Y' then 0 else(num_num - num_back)*( num_price + num_price_add)*int_discount *0.01 end))  from cey_u_orderdish  where ch_billno = '%1' and ch_tableno = '%2' ").arg(w_scr_dish_restaurant_dialog::table_danhao).arg(frontViewDialog_virtual::cur_desk_no));
            //qDebug() << QString("select sum((case when ch_presentflag ='Y' then 0 else(num_num - num_back)*( num_price + num_price_add)*int_discount *0.01 end))  from cey_u_orderdish  where ch_billno = '%1' and ch_tableno = '%2").arg(w_scr_dish_restaurant_dialog::table_danhao).arg(frontViewDialog_virtual::cur_desk_no);
            query.prepare(QString("update cey_u_table  set num_cost = :num where ch_billno = :danhao and ch_tableno = :zhuohao "));
            query.bindValue(":num",QString().sprintf("%0.2f",model.record(0).value(0).toFloat()));
            query.bindValue(":danhao",w_scr_dish_restaurant_dialog::table_danhao);
            query.bindValue(":zhuohao",frontViewDialog_virtual::cur_desk_no);
            if(query.exec())
            {
            }
            else
            {
                QSqlDatabase::database().rollback();
                lds_messagebox::warning(this, tr("消息提示"),tr("cey_u_table!"));
                
                return;
            }
            QSqlDatabase::database().commit();
            print_back_one();
            //更新
            float num = 0.00;
            num = modelBase.item(row,3)->text().toFloat() - back.back_num;
            modelBase.item(row,3)->setText(QString().sprintf("%0.2f",num));
            float zongjia;
            zongjia = modelBase.item(row,3)->text().toFloat() * modelBase.item(row,4)->text().toFloat() * (int)modelBase.item(row,5)->text().toFloat() * 0.01 + modelBase.item(row,3)->text().toFloat() * modelBase.item(row,21)->text().toFloat();
            modelBase.item(row,6)->setText(QString().sprintf("%0.2f",zongjia));
        }
        else if(back.bakc_one == false && back.back_all == true)
        {
            tui_cai = true;
            QStandardItemModel *back_model = new QStandardItemModel();
            for(int i = 0; i < modelBase.rowCount(); i++)
            {
                if(modelBase.item(i,24)->text() == "Y")
                {
                    int back_row = back_model->rowCount();
                    back_model->setRowCount(back_row + 1);
                    back_model->setItem(back_row,0,new QStandardItem(modelBase.item(i,0)->text()));
                    back_model->setItem(back_row,1,new QStandardItem(modelBase.item(i,1)->text()));
                    back_model->setItem(back_row,2,new QStandardItem(modelBase.item(i,2)->text()));
                    back_model->setItem(back_row,3,new QStandardItem(modelBase.item(i,3)->text()));
                    back_model->setItem(back_row,4,new QStandardItem(modelBase.item(i,4)->text()));
                    back_model->setItem(back_row,5,new QStandardItem(modelBase.item(i,5)->text()));
                    back_model->setItem(back_row,6,new QStandardItem(modelBase.item(i,6)->text()));
                    back_model->setItem(back_row,7,new QStandardItem(modelBase.item(i,7)->text()));
                    back_model->setItem(back_row,8,new QStandardItem(modelBase.item(i,8)->text()));
                    back_model->setItem(back_row,9,new QStandardItem(modelBase.item(i,9)->text()));
                    back_model->setItem(back_row,10,new QStandardItem(modelBase.item(i,10)->text()));
                    back_model->setItem(back_row,11,new QStandardItem(modelBase.item(i,11)->text()));
                    back_model->setItem(back_row,12,new QStandardItem(modelBase.item(i,12)->text()));
                    back_model->setItem(back_row,13,new QStandardItem(modelBase.item(i,13)->text()));
                    back_model->setItem(back_row,14,new QStandardItem(modelBase.item(i,14)->text()));
                    back_model->setItem(back_row,15,new QStandardItem(modelBase.item(i,15)->text()));
                    back_model->setItem(back_row,16,new QStandardItem(modelBase.item(i,16)->text()));
                    back_model->setItem(back_row,17,new QStandardItem(modelBase.item(i,17)->text()));
                    back_model->setItem(back_row,18,new QStandardItem(modelBase.item(i,18)->text()));
                    back_model->setItem(back_row,19,new QStandardItem(modelBase.item(i,19)->text()));
                    back_model->setItem(back_row,20,new QStandardItem(modelBase.item(i,20)->text()));
                    back_model->setItem(back_row,21,new QStandardItem(modelBase.item(i,21)->text()));
                    back_model->setItem(back_row,22,new QStandardItem(modelBase.item(i,22)->text()));
                    back_model->setItem(back_row,23,new QStandardItem(modelBase.item(i,23)->text()));
                    back_model->setItem(back_row,24,new QStandardItem(modelBase.item(i,24)->text()));
                    back_model->setItem(back_row,25,new QStandardItem(modelBase.item(i,25)->text()));
                    back_model->setItem(back_row,26,new QStandardItem(modelBase.item(i,26)->text()));
                }
            }

            QSqlQueryModel num;
            for(int k = 0; k < back_model->rowCount(); k++)
            {
                num.clear();
                num.setQuery(QString("select num_num from cey_u_orderdish  where ch_billno = '%1' and ch_tableno ='%2' and int_flowID ='%3' ").arg(w_scr_dish_restaurant_dialog::table_danhao).arg(frontViewDialog_virtual::cur_desk_no).arg(back_model->item(k,25)->text()));
                //qDebug() << QString("select num_num from cey_u_orderdish  where ch_billno = '%1' ch_tableno ='%2' and int_flowID ='%3' ").arg(w_scr_dish_restaurant_dialog::table_danhao).arg(frontViewDialog_virtual::cur_desk_no).arg(back_model->item(k,25)->text());
                query.prepare(QString("update cey_u_orderdish  set num_back = :num, vch_back_operID =:oper , dt_back_operdate =:time , ch_printflag ='N'  where int_flowId =:id"));
                query.bindValue(":num",QString().sprintf("%0.2f",num.record(0).value(0).toFloat()));
                query.bindValue(":oper",n_func::gs_operid);
                query.bindValue(":time",time);
                query.bindValue(":id",back_model->item(k,25)->text());
                //backone = QString().sprintf("%0.2f",back.back_num);
                if(query.exec())
                {
                }
                else
                {
                    QSqlDatabase::database().rollback();
                    lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_u_orderdish失败!"));
                    
                    return;
                }

                query.prepare(QString("update cey_bt_dish_warn  Set num_sale ='0.0'  where ch_dishno =:bianma and ch_state ='Y' and left( dt_date , 10)= :date"));
                query.bindValue(":bianma",back_model->item(k,1)->text());
                query.bindValue(":date",date);
                if(query.exec())
                {
                }
                else
                {
                    QSqlDatabase::database().rollback();
                    lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_bt_dish_warn失败!"));
                    
                    return;
                }

                query.prepare(QString("Insert into cey_u_orderdish_log(int_orderflow , ch_type , vch_impower , vch_operID , dt_operdate , vch_reason)values(:id,'1',:oper1,:oper2,:time,:yuanyin)"));
                query.bindValue(":id",back_model->item(k,25)->text());
                query.bindValue(":oper1",n_func::gs_operid);
                query.bindValue(":oper2",n_func::gs_operid);
                query.bindValue(":time",time);
                query.bindValue(":yuanyin",back.cook_causes);
                if(query.exec())
                {
                }
                else
                {
                    QSqlDatabase::database().rollback();
                    qDebug() << query.lastError();  lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_u_orderdish_log失败!"));
                    
                    return;
                }
            }
            query.prepare(QString("update cey_u_table  set num_cost = :num where ch_billno = :danhao and ch_tableno = :zhuohao "));
            query.bindValue(":num","0.00");
            query.bindValue(":danhao",w_scr_dish_restaurant_dialog::table_danhao);
            query.bindValue(":zhuohao",frontViewDialog_virtual::cur_desk_no);
            if(query.exec())
            {
            }
            else
            {
                QSqlDatabase::database().rollback();
                lds_messagebox::warning(this, tr("消息提示"),tr("cey_u_table!"));
                
                return;
            }

            print_back_all();
            QSqlDatabase::database().commit();

            for(int i = 0; i < modelBase.rowCount(); i++)
            {
                if(modelBase.item(i,24)->text() == "Y")
                {
                    modelBase.item(i,3)->setText("0.00");
                    float zongjia;
                    zongjia = modelBase.item(i,3)->text().toFloat() * modelBase.item(i,4)->text().toFloat() * (int)modelBase.item(i,5)->text().toFloat() * 0.01 + modelBase.item(i,3)->text().toFloat() * modelBase.item(i,21)->text().toFloat();
                    modelBase.item(i,6)->setText(QString().sprintf("%0.2f",zongjia));
                }
            }
        }
    }


    caculate_dishtitle();
    ui->tableView->tableview->selectRow(qMin(modelBase.rowCount()-1,row));
    frontViewDialog_virtual::toDel();

    tui_cai = false;
}

void table_order_2::on_pushButton_9_clicked()//折扣
{
    QSqlQueryModel model;
    model.setQuery("Select NOW()");
    QString time = model.record(0).value(0).toDateTime().toString("yyyy-MM-dd hh:mm:ss");

    int row=ui->tableView->tableview->currentIndex().row();// ui->tableView->currentIndex().row();
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

    if(modelBase.item(row,5)->text() != "100" && n_func::gs_operid == modelBase.item(row,14)->text())
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
            power_oper oper(this);
            /*oper.exec();*/backheader::static_dialogexec(oper, tr("权限核实"));

            if(oper.btn_t_f == true)
            {
                if(oper.operid == modelBase.item(row,14)->text())
                {
                    modelBase.item(row,5)->setText(tr("100"));
                    modelBase.item(row,14)->setText("");
                    float jine = 0.00;
                    jine = modelBase.item(row,3)->text().toFloat() * modelBase.item(row,4)->text().toFloat() * modelBase.item(row,5)->text().toFloat() * 0.01 + modelBase.item(row,3)->text().toFloat() * modelBase.item(row,21)->text().toFloat();;
                    modelBase.item(row,6)->setText(QString().sprintf("%0.2f",jine));
                    modelBase.item(row,19)->setText("");
                }
                else
                {
                    QString str = "取消折扣操作人:" + oper.operid + "和折扣处理人:" + modelBase.item(row,14)->text() + "\n不一样,操作终止!";
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
        discount_cook discount_c(this);
        /*discount_c.exec();*/backheader::static_dialogexec(discount_c, tr("菜品定价或折扣"));
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
                        power_oper oper(this);
                        /*oper.exec();*/backheader::static_dialogexec(oper, tr("权限核实"));

                        if(oper.btn_t_f == true)
                        {
                            QSqlQueryModel model;
                            model.setQuery(QString("Select int_discount from cey_sys_operator where ch_flag = 'Y' and vch_operID ='%1' ").arg(oper.operid));
                            if(discount_num < model.record(0).value(0).toFloat())
                            {
                                lds_messagebox::warning(this, tr("消息提示"),tr("指定操作员没有该折扣操作权限!"));
                                
                                return;
                            }

                            modelBase.item(row,5)->setText(QString().sprintf("%0.0f",discount_num));
                            modelBase.item(row,14)->setText(oper.operid);
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

    this->to_dish_title(label_shuliang, label_jine);

    //    ui->label_3->setText("￥:" + QString().sprintf("%0.2f",label_jine));
    //    ui->label_5->setText(QString().sprintf("%0.2f",label_shuliang));
}

void table_order_2::print()
{
    QSqlQueryModel person;
    person.setQuery(QString("select int_person from cey_u_table where ch_billno= '%1' ").arg(w_scr_dish_restaurant_dialog::table_danhao));
    QString table_renshu = person.record(0).value(0).toString();

    QSettings *configIniRead = new QSettings(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    QSqlQueryModel model;
    QString str_t= configIniRead->value("system_setting/zhangdanbiaoti").toString();
    if(!str_t.isEmpty())
    {
        str_t.append("\n");
    }
    QString title = str_t +  tr("预打结帐单\n");//需居中
    QByteArray content;

    //流水号
    QString str;
    QString liushui;
    str.clear();

    liushui=system_setting::get_max_serialno_add1(bendandanhao);
    content.append(liushui);

    QSqlQueryModel m_billno;
    m_billno.setQuery("Select NOW()");
    QDateTime ldt_datetime;
    ldt_datetime.setTime(m_billno.record(0).value(0).toTime());
    ldt_datetime.setDate(m_billno.record(0).value(0).toDate());
    QString table_time = m_billno.record(0).value(0).toDateTime().toString("yyyy-MM-dd hh:mm:ss");
    m_billno.clear();
    QString liushi = "P" + ldt_datetime.toString("yyMMdd");
    m_billno.setQuery(QString("select Max(right(ch_payno, 5)) f from cey_u_checkout_master where Left(ch_payno,7) = '%1' ").arg(liushi));
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

    str.clear();
    model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_payno' "));
    str = model.record(0).value(0).toString();
    if(str != "0" && !liushuihao.isEmpty())
    {
        content.append(tr("付款号:") + liushuihao + "\n");
    }

    //人数
    str.clear();
    model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_person' "));
    str = model.record(0).value(0).toString();
    if(str != "0")
    {
        content.append(tr("人数:") + table_renshu + "    ");
    }


    //时间
    str.clear();
    model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_time' "));
    str = model.record(0).value(0).toString();
    if(str != "0")
    {
        content.append(tr("时间:") + table_time + "\n");
    }

    //工号
    str.clear();
    model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_operid' "));
    str = model.record(0).value(0).toString();
    if(str != "0")
    {
        content.append(tr("工号:") + n_func::gs_operid + "    ");
    }



    //餐桌名
    str.clear();
    model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_table' "));
    str = model.record(0).value(0).toString();
    if(str != "0")
    {
        content.append(tr("餐桌名:") + frontViewDialog_virtual::cur_desk_no + "\n");
    }
    //打印消费内容

    //表头
    QString shifoumingxi;
    shifoumingxi = configIniRead->value("system_setting/maidanmingxi").toString();
    if(shifoumingxi != "1")
    {
        QString tou = configIniRead->value("system_setting/biaoneirong").toString();
        //QString tou = tr("品吗       数量     价格    金额\n");
        QString str;
        str =  configIniRead->value("w_bt_dish_Kitchenprinter_param/printer_type_com").toString();
        if(str == "0") //80打印机
        {
            content.append(tr("---------------------------\n"));
            content.append(tou+"\n");
            content.append(tr("\n---------------------------\n"));
        }
        else
        {
            content.append(tr("---------------------------------\n"));
            content.append(tou+"\n");
            content.append(tr("---------------------------------\n"));
        }
    }
    else
    {
        QString str;
        str =  configIniRead->value("w_bt_dish_Kitchenprinter_param/printer_type_com").toString();
        if(str == "0") //80打印机
        {
            content.append(tr("---------------------------\n"));
        }
        else
        {
            content.append(tr("---------------------------------\n"));
        }
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
    if(fenzu == "1")//菜品按大类分组
    {
        QSqlQueryModel dalei;
        dalei.setQuery(QString("select ch_dish_typeno from cey_bt_dish_type"));
        for(int k = 0; k < dalei.rowCount(); k++)
        {
            QStandardItemModel cook;
            for(int m = 0; m < modelBase.rowCount(); m++)
            {
                if(modelBase.item(m,22)->text() == dalei.record(k).value(0).toString())
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
                }
            }
            QByteArray neirong;
            float zongjia = 0.00;
            for(int i = 0; i < cook.rowCount(); i++)
            {
                zongjia += cook.item(i,3)->text().toFloat() * cook.item(i,4)->text().toFloat() * (int)cook.item(i,5)->text().toFloat() * 0.01 + cook.item(i,3)->text().toFloat() * cook.item(i,21)->text().toFloat();
                QString hanghao = QString::number(i+1) + tr(".");
                if(hanghao.length() < 3)
                {
                    hanghao.append(" ");
                }
                if(bxuhao == "1")
                {
                    if(bxuhao=="1")neirong.append(hanghao);
                }
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
        QString str;
        str =  configIniRead->value("w_bt_dish_Kitchenprinter_param/printer_type_com").toString();
        if(str == "0") //80打印机
        {
            content.append(tr("---------------------------\n"));
        }
        else
        {
            content.append(tr("---------------------------------\n"));
        }
    }

    //    content.append(tr("应收:") + lineEdit_4 + "\n");
    //    content.append(n_func::gs_print_line+"\n");
    //    content.append(tr("现金:") + lineEdit_4 + "\n");

    if(_xiaofei.length() < 10)
    {
        int a = 10 - _xiaofei.length();
        for(int h = 0; h < a; h++)
        {
            _xiaofei.append(" ");
        }
    }

    content.append(tr("消费:") + _xiaofei);
    content.append(tr("折扣:") + _zhekou + "\n");

    if(_zengsong.length() < 10)
    {
        int a = 10 - _zengsong.length();
        for(int h = 0; h < a; h++)
        {
            _zengsong.append(" ");
        }
    }

    content.append(tr("赠送:") + _zengsong);
    content.append(tr("抹零:") + _moling + "\n");

    if(_yingshou.length() < 10)
    {
        int a = 10 - _yingshou.length();
        for(int h = 0; h < a; h++)
        {
            _yingshou.append(" ");
        }
    }

    content.append(tr("应收:") + _yingshou);
    content.append(tr("税收:") + _shuishou + "\n");
    float renjun  = 0.00;
    renjun = _xiaofei.toFloat() / table_renshu.toFloat();
    content.append(tr("人均:") + QString().sprintf("%0.2f",renjun) + "\n");
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
    //    printer = new public_mainprinter();
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

    //图片打印
    QString logo = configIniRead->value("system_setting/dayinlogo").toString();
    if(logo == "1")
    {
        QString str = configIniRead->value("system_setting/dayinlogoneirong").toString();//图片路径
        b+= backheader::printer->drawLogo("1B40 1B6101", str);//清除缓存 居中 pixmap路径
    }

    // path = "192.168.1.171:9100";
    content.append("\n");
    b+=backheader::printer->drawText("1B40 1B6101 1C5700",title);//居中不放大
    b+=backheader::printer->drawText("1B40,1C5701",liushui,Qt::AlignLeft);//放大
    b += backheader::printer->drawText("1B40,1C5700", content, Qt::AlignLeft);

    //    //打印一维码
    //    QString yiweima = configIniRead->value("system_setting/yiweima").toString();
    //    //yiweima  = "1";
    //    if(yiweima == "1")
    //    {
    //        QString str = configIniRead->value("system_setting/yiweimaneirong").toString();//一维码内容
    //        //str = "12345678";
    //        b+= printer->drawoneBarcode("1B40 1B6101",str);//清除缓存 居中 pixmap路径
    //    }

    //    //打印二维码
    //    QString erweima = configIniRead->value("system_setting/erweima").toString();
    //    if(erweima == "1")
    //    {
    //        QString str = configIniRead->value("system_setting/erweimaneirong").toString();//图片路径
    //        b+= printer->drawQR_Code("", str);//清除缓存 居中 pixmap路径
    //    }
    b += backheader::printer->drawText("1B6405 1D5601", "");//走纸5x， 切纸
    b=backheader::printer->setPrintCount(b, print_count);
    backheader::asyn_dataprint(b, path);
    //    printer->asynchPrint(b, path);
    //    connect(printer,SIGNAL(errorString(QString)),this,SLOT(print_lost(QString)));
}

void table_order_2::print_chufangdayin()
{
    //厨房打印机

}

void table_order_2::re_model()
{
    model_dalei.setQuery(QString("SELECT  * FROM cey_bt_dish_type"));
    for(int i = 0; i < model_dalei.rowCount(); i++)
    {
        if(i == num_da)
        {
            dalei = model_dalei.record(i).value(0).toString();
        }
    }

    model_xiaolei.setQuery(QString("SELECT  * FROM cey_bt_dish_subtype where ch_dish_typeno = '%1' ").arg(dalei));
    for(int i = 0; i < model_xiaolei.rowCount(); i++)
    {
        if(i == num_xiao)
        {
            xiaolei = model_xiaolei.record(0).value(0).toString();
        }
    }
    if(changyongcook == false)
    {
        model_cook.setQuery(QString("SELECT  * FROM cey_bt_dish where ch_dish_typeno = '%1' and ch_sub_typeno ='%2' and (ch_stopflag = null or ch_stopflag = 'N')and ch_tempflag ='N' ").arg(dalei).arg(xiaolei));
    }
    else
    {
        model_cook.setQuery(QString("select * from cey_bt_dish where ch_off = 'Y' and (ch_stopflag = null or ch_stopflag = 'N') and ch_tempflag ='N' "));
    }
}

void table_order_2::init_bedi_db()
{
}


void table_order_2::print_lost(const QString &str)
{
    lds_messagebox::warning(this, tr("消息提示"),str);
}

void table_order_2::print_louda()
{
    QSqlQueryModel model;
    model.setQuery(QString("select NOW()"));
    label_t->setText(model.record(0).value(0).toDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    thread->start();
}

void table_order_2::show_print_close()
{
    leiji = 0;
    print_pr->hide();
}

void table_order_2::thread_signal()
{
    leiji++;
    if(leiji == 3)
    {
        print_pr->show();
        QTimer::singleShot(3000,this,SLOT(show_print_close()));
    }
}

void table_order_2::caculate_dishtitle()
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

void table_order_2::kitchenprinte_lost()
{
    ;

    QSqlQueryModel model;
    model.setQuery(QString("select Max(id) from chudaloudan"),db);

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

    QSqlQuery query(db);
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
    query.bindValue(":vch_danaho",w_scr_dish_restaurant_dialog::table_danhao);
    query.bindValue(":title",louda_title);
    query.bindValue(":print","N");
    query.bindValue(":date",date);
    if(query.exec())
    {

    }
    else
    {
        db.rollback();
        lds_messagebox::warning(this, tr("消息提示"),("插入漏单失败!"));
        
        return;
    }
}

void table_order_2::bianma_diancai(const QString &str)
{
    dalei="";
    xiaolei="";
    if(str=="enter"){
        ui->widget->widget3_item_event_click();
    } else {
        init_cook("", str);
    }
}

void table_order_2::bianma_cancel()
{
    bianma_daicai_type = false;
    itemList.clear();
    qDeleteAll(itemList.begin(),itemList.end());
    //qDeleteAll(//ui->widget_3->children());
    layout_cook = new QGridLayout();
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

    if(changyongcook == false)
    {
        model_cook.setQuery(QString("SELECT  * FROM cey_bt_dish where ch_dish_typeno = '%1' and ch_sub_typeno ='%2' and (ch_stopflag = null or ch_stopflag = 'N') ").arg(dalei).arg(xiaolei));
        QString kind = model_xiaolei.record(0).value(0).toString();
        showGoods(kind);
    }
    else
    {
        model_cook.setQuery(QString("select * from cey_bt_dish where ch_off ='Y' and (ch_stopflag = null or ch_stopflag = 'N')  "));
        QString kind = model_xiaolei.record(0).value(0).toString();
        showGoods(kind);
    }
}

void table_order_2::timer()
{
    QSqlQueryModel model;
    model.setQuery(QString("select NOW()"));
    label_t->setText(model.record(0).value(0).toDateTime().toString("yyyy-MM-dd hh:mm:ss"));
}

void table_order_2::on_pushButton_10_clicked()//赠送
{
    QSqlQueryModel model;
    model.setQuery("Select NOW()");
    QString time = model.record(0).value(0).toDateTime().toString("yyyy-MM-dd hh:mm:ss");

    int row=ui->tableView->tableview->currentIndex().row();// ui->tableView->currentIndex().row();

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
            if(0==lds_messagebox::warning(this, tr("消息提示"),tr("是否确定将此菜品赠送?"),tr("确认"),tr("取消")))
            {
                modelBase.item(row,8)->setText("Y");
                modelBase.item(row,20)->setText(time);
                float label_jine = 0.00;
                for(int i = 0; i < modelBase.rowCount(); i++)
                {
                    if(modelBase.item(i,8)->text() == "N" && modelBase.item(i,9)->text() != "Y" )
                    {
                        label_jine += modelBase.item(i,6)->text().toFloat();
                    }
                }
                this->to_dish_title(-1, label_jine);

                //                ui->label_3->setText("￥:" + QString().sprintf("%0.2f",label_jine));
            }
        }
        else
        {
            if(0==lds_messagebox::warning(this, tr("消息提示"),tr("此菜品已经被赠送,是否取消赠送?"),tr("确认"),tr("取消")))
            {
                modelBase.item(row,8)->setText("N");
                modelBase.item(row,20)->setText("");
                float label_jine = 0.00;
                for(int i = 0; i < modelBase.rowCount(); i++)
                {
                    if(modelBase.item(i,8)->text() == "N" && modelBase.item(i,9)->text() != "Y" )
                    {
                        label_jine += modelBase.item(i,6)->text().toFloat();
                    }
                }this->to_dish_title(-1, label_jine);

                //                ui->label_3->setText("￥:" + QString().sprintf("%0.2f",label_jine));
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

        if(modelBase.item(row,8)->text() == "N")
        {
            if(zengsongjine > present)
            {
                if(0==lds_messagebox::warning(this, tr("消息提示"),tr("超出赠送金额,请另指定赠送人!"),tr("确认"),tr("取消")))
                {
                    power_oper oper(this);
                    /*oper.exec();*/backheader::static_dialogexec(oper, tr("权限核实"));

                    if(oper.btn_t_f == true)
                    {
                        QSqlQueryModel model_l;
                        model_l.setQuery(QString("select num_present from cey_sys_operator where vch_operID = '%1' and ch_waiter = 'Y' ").arg(oper.operid));
                        float present_0 = 0.00;
                        present_0 = model_l.record(0).value(0).toFloat();
                        if(oper.operid == "0000")
                        {
                            if(0==lds_messagebox::warning(this, tr("消息提示"),tr("是否确定将此菜品赠送?"),tr("确认"),tr("取消")))
                            {
                                modelBase.item(row,8)->setText("Y");
                                modelBase.item(row,20)->setText(time);
                                float label_jine = 0.00;
                                for(int i = 0; i < modelBase.rowCount(); i++)
                                {
                                    if(modelBase.item(i,8)->text() == "N" && modelBase.item(i,9)->text() != "Y" )
                                    {
                                        label_jine += modelBase.item(i,6)->text().toFloat();
                                    }
                                }this->to_dish_title(-1, label_jine);

                                //                                ui->label_3->setText("￥:" + QString().sprintf("%0.2f",label_jine));
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
                                    float label_jine = 0.00;
                                    for(int i = 0; i < modelBase.rowCount(); i++)
                                    {
                                        if(modelBase.item(i,8)->text() == "N" && modelBase.item(i,9)->text() != "Y" )
                                        {
                                            label_jine += modelBase.item(i,6)->text().toFloat();
                                        }
                                    }this->to_dish_title(-1, label_jine);

                                    //                                    ui->label_3->setText("￥:" + QString().sprintf("%0.2f",label_jine));
                                }
                            }
                        }
                    }
                    else
                    {
                        return;
                    }
                }
            }
            else
            {
                if(0==lds_messagebox::warning(this, tr("消息提示"),tr("是否确定将此菜品赠送?"),tr("确认"),tr("取消")))
                {
                    modelBase.item(row,8)->setText("Y");
                    modelBase.item(row,20)->setText(time);
                    float label_jine = 0.00;
                    for(int i = 0; i < modelBase.rowCount(); i++)
                    {
                        if(modelBase.item(i,8)->text() == "N" && modelBase.item(i,9)->text() != "Y" )
                        {
                            label_jine += modelBase.item(i,6)->text().toFloat();
                        }
                    }this->to_dish_title(-1, label_jine);

                    //                    ui->label_3->setText("￥:" + QString().sprintf("%0.2f",label_jine));
                }
            }
        }
        else
        {
            if(0==lds_messagebox::warning(this, tr("消息提示"),tr("此菜品已经被赠送,是否取消赠送?"),tr("确认"),tr("取消")))
            {
                modelBase.item(row,8)->setText("N");
                modelBase.item(row,20)->setText("");
                float label_jine = 0.00;
                for(int i = 0; i < modelBase.rowCount(); i++)
                {
                    if(modelBase.item(i,8)->text() == "N" && modelBase.item(i,9)->text() != "Y" )
                    {
                        label_jine += modelBase.item(i,6)->text().toFloat();
                    }
                }this->to_dish_title(-1, label_jine);

                //                ui->label_3->setText("￥:" + QString().sprintf("%0.2f",label_jine));
            }
        }
    }
}


void table_order_2::on_pushButton_6_clicked()//换菜
{
    int row = ui->tableView->tableview->currentIndex().row();// ui->tableView->currentIndex().row();// ui->tableView->currentIndex().row();

    if(row <= -1)
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("请选择需要换菜的菜品!"));
        
        return;
    }

    if(modelBase.item(row,24)->text() == "Y")
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("所点菜记录已保存,请进行退菜处理!"));
        
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

    dish_change change(row,"2", this);
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
        this->to_dish_title(label_shuliang, label_jine);

        //        ui->label_3->setText("￥:" + QString().sprintf("%0.2f",label_jine));
        //        ui->label_5->setText(QString().sprintf("%0.2f",label_shuliang));
    }
}


void table_order_2::on_pushButton_5_clicked()//做法
{
    ;
    int row = ui->tableView->tableview->currentIndex().row();// ui->tableView->currentIndex().row();

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

    if(modelBase.item(row,24)->text() == "Y")
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("已落单菜品不允许修改做法!"));
        
        return;
    }

    if(modelBase.item(row,9)->text() == "P")
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("主套菜不能设置厨打说明,请设置套菜明细!"));
        
        return;
    }

    w_scr_dish_special_cook cook(modelBase.item(row,1)->text(),modelBase.item(row,7)->text(),modelBase.item(row,21)->text(), this);
    /*special.exec();*/backheader::static_dialogexec(cook, tr("特殊做法"));
    if(cook.btn_t_f == true)
    {
        if(modelBase.item(row,9)->text() != "P" && modelBase.item(row,9)->text() != "Y")//不是套菜
        {
            modelBase.item(row,7)->setText(cook.zuofaneirong);
            modelBase.item(row,21)->setText(QString().sprintf("%0.2f",cook.add_price));
            float aa;
            aa = 0.00;
            aa = modelBase.item(row,3)->text().toFloat() * modelBase.item(row,4)->text().toFloat() * modelBase.item(row,5)->text().toFloat() * 0.01 + modelBase.item(row,3)->text().toFloat() * cook.add_price;
            modelBase.item(row,6)->setText(QString().sprintf("%0.2f",aa));
        }

        if(modelBase.item(row,9)->text() != "P" && modelBase.item(row,9)->text() == "Y")//是子套菜
        {
            modelBase.item(row,7)->setText(cook.zuofaneirong);
            modelBase.item(row,21)->setText(QString().sprintf("%0.2f",cook.add_price));
            float bb = 0.00;
            //bb = modelBase.item(row,21)->text().toFloat() * modelBase.item(row,3)->text().toFloat();
            for(int i = row;i < modelBase.rowCount(); i--)
            {
                if(modelBase.item(i,9)->text() == "P")
                {
                    int thisrow = i;
                    int taorow = i + 1;
                    float cc = 0.00;
                    modelBase.item(i,21)->setText(QString().sprintf("%0.2f",cook.add_price));
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
    this->to_dish_title(label_shuliang, label_jine);

    //    ui->label_3->setText("￥:" + QString().sprintf("%0.2f",label_jine));
    //    ui->label_5->setText(QString().sprintf("%0.2f",label_shuliang));
}

void table_order_2::on_pushButton_clicked()//常用菜
{
    changyongcook = true;
    init_cook();
    //    itemList.clear();
    //    qDeleteAll(itemList.begin(),itemList.end());
    //    qDeleteAll(ui->widget_3->children());
    //    layout_cook = new QGridLayout();
    //    for(int i = 0; i < item_count; ++i)
    //    {
    //        GoodItem *item = new GoodItem("","");
    //        itemList.append(item);
    //        connect(item,SIGNAL(touch(QString)),this,SLOT(addGood(QString)));
    //        layout_cook->addWidget(item,i/item_count_row,i%item_count_row);
    //    }
    //    layout_cook->setContentsMargins(0,0,0,0);
    //    layout_cook->setSpacing(2);
    //    ui->widget_3->setLayout(layout_cook);
    //
    //    model_cook.setQuery(QString("select * from cey_bt_dish where ch_off ='Y' and (ch_stopflag = null or ch_stopflag = 'N') and ch_tempflag ='N' "));
    //    QString kind = model_xiaolei.record(0).value(0).toString();
    //    showGoods(kind);
}

void table_order_2::on_pushButton_21_clicked()//付款
{
    if(modelBase.rowCount() == 0)
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("没有点菜记录,不能付款!"));
        
        return;
    }

    //1 落单菜会厨打
    int k = 0;
    for(int i = 0; i < modelBase.rowCount(); i++)
    {
        if(modelBase.item(i,24)->text() == "N")
        {
            k++;
        }
    }

    if(k > 0)
    {
        if(1==lds_messagebox::warning(this, tr("消息提示"),tr("有为落单的菜品，是否落单后埋单?"),tr("确认"),tr("取消"))){
            return;
        }
        if(!toluodan()) return;
    }

    //2
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

    //是否有落单菜品
    QString danhao = w_scr_dish_restaurant_dialog::table_danhao;
    QString zhuohao = frontViewDialog_virtual::cur_desk_no;
    QSqlQueryModel time_model;
    time_model.setQuery(QString("select NOW()"));
    QString time = time_model.record(0).value(0).toDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString guqingdate = time_model.record(0).value(0).toDateTime().toString("yyyy-MM-dd");
    int a = 0;
    a = w_scr_dish_restaurant_dialog::table_danhao.right(4).toInt()% 100;
    if(a < 10)
    {
        liushuihao_p = "0" + QString::number(a);
    }
    else
    {
        liushuihao_p = QString::number(a);
    }

    QSqlQueryModel m_billno;
    QString liushi = "P" + time_model.record(0).value(0).toDateTime().toString("yyyyMMdd");
    m_billno.setQuery(QString("select Max(right(ch_payno, 5)) f from cey_u_checkout_master where Left(ch_payno,7) = '%1' ").arg(liushi));
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

    //菜品消费明细表
    QSqlQuery query;
    QSqlDatabase::database().transaction();
    QStandardItemModel *model  = new QStandardItemModel();
    for(int i = 0; i < modelBase.rowCount();i++)
    {
        if(modelBase.item(i,24)->text() == "N")
        {
            int back_row = model->rowCount();
            model->setRowCount(back_row + 1);
            model->setItem(back_row,0,new QStandardItem(modelBase.item(i,0)->text()));
            model->setItem(back_row,1,new QStandardItem(modelBase.item(i,1)->text()));
            model->setItem(back_row,2,new QStandardItem(modelBase.item(i,2)->text()));
            model->setItem(back_row,3,new QStandardItem(modelBase.item(i,3)->text()));
            model->setItem(back_row,4,new QStandardItem(modelBase.item(i,4)->text()));
            model->setItem(back_row,5,new QStandardItem(modelBase.item(i,5)->text()));
            model->setItem(back_row,6,new QStandardItem(modelBase.item(i,6)->text()));
            model->setItem(back_row,7,new QStandardItem(modelBase.item(i,7)->text()));
            model->setItem(back_row,8,new QStandardItem(modelBase.item(i,8)->text()));
            model->setItem(back_row,9,new QStandardItem(modelBase.item(i,9)->text()));
            model->setItem(back_row,10,new QStandardItem(modelBase.item(i,10)->text()));
            model->setItem(back_row,11,new QStandardItem(modelBase.item(i,11)->text()));
            model->setItem(back_row,12,new QStandardItem(modelBase.item(i,12)->text()));
            model->setItem(back_row,13,new QStandardItem(modelBase.item(i,13)->text()));
            model->setItem(back_row,14,new QStandardItem(modelBase.item(i,14)->text()));
            model->setItem(back_row,15,new QStandardItem(modelBase.item(i,15)->text()));
            model->setItem(back_row,16,new QStandardItem(modelBase.item(i,16)->text()));
            model->setItem(back_row,17,new QStandardItem(modelBase.item(i,17)->text()));
            model->setItem(back_row,18,new QStandardItem(modelBase.item(i,18)->text()));
            model->setItem(back_row,19,new QStandardItem(modelBase.item(i,19)->text()));
            model->setItem(back_row,20,new QStandardItem(modelBase.item(i,20)->text()));
            model->setItem(back_row,21,new QStandardItem(modelBase.item(i,21)->text()));
            model->setItem(back_row,22,new QStandardItem(modelBase.item(i,22)->text()));
            model->setItem(back_row,23,new QStandardItem(modelBase.item(i,23)->text()));
            model->setItem(back_row,24,new QStandardItem(modelBase.item(i,24)->text()));
            model->setItem(back_row,25,new QStandardItem(modelBase.item(i,25)->text()));
            model->setItem(back_row,26,new QStandardItem(modelBase.item(i,26)->text()));
        }
    }
    QSqlQueryModel guqing;
    int int_id = 0;
    m_billno.clear();
    m_billno.setQuery(QString("Select IFNULL( max( int_id), 0)from cey_u_orderdish where ch_billno = '%1' and ch_tableno = '%2'").arg(w_scr_dish_restaurant_dialog::table_danhao).arg(frontViewDialog_virtual::cur_desk_no));
    int_id = m_billno.record(0).value(0).toInt();
    for(int i = 0; i < model->rowCount(); i++)
    {
        int_id++;
        guqing.clear();
        guqing.setQuery(QString("Select ifnull(num_num - num_sale , 0)from cey_bt_dish_warn where ch_dishno ='%1' and ch_state ='Y' and DATE_FORMAT(dt_date, '%Y-%m-%d')  ='%2' ").arg(model->item(i,1)->text()).arg(guqingdate));
        if(guqing.rowCount() > 0 && model->item(i,9)->text() != tr("Y"))
        {
            query.prepare(QString("update cey_bt_dish_warn set num_sale = num_sale + :as_num where ch_state ='Y' and ch_dishno = :as_dishno and DATE_FORMAT(dt_date, '%Y-%m-%d') = :as_date "));
            query.bindValue(":as_num",model->item(i,3)->text());
            query.bindValue(":as_dishno",model->item(i,1)->text());
            query.bindValue(":as_date",guqingdate);
            if(query.exec())
            {

            }
            else
            {
                QSqlDatabase::database().rollback();
                lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_bt_dish_warn菜品估清失败!"));
                
                return;
            }
        }

        float add_price;
        add_price = 0.00;
        add_price = model->item(i,3)->text().toFloat() * model->item(i,21)->text().toFloat();
        if(model->item(i,8)->text() == "Y")//赠送菜品
        {
            if(model->item(i,7)->text().isEmpty())
            {
                if(query.exec(QString("insert into cey_u_orderdish(ch_billno,ch_tableno,int_id,ch_dishno,num_price,num_price_org,num_price_add,num_num,num_back,vch_back_operID,dt_back_operdate,int_discount,vch_dis_operID,dt_dis_operdate,vch_memberno,ch_consult,ch_suitflag,ch_suitno,ch_specialflag,ch_presentflag,vch_pre_operID,dt_pre_operdate,vch_operID,dt_operdate,ch_togono,ch_payno,vch_print_memo) values('%1','%2','%3','%4','%5','%6','%7','%8','%9',null,null,'%10',null,null,null,'%11','%12',null,'%13','%14','%15','%16','%17','%18',null,'%19',null)")
                              .arg(danhao).arg(zhuohao).arg(QString::number(int_id)).arg(model->item(i,1)->text()).arg(model->item(i,4)->text()).arg(model->item(i,4)->text())
                              .arg(QString().sprintf("%0.2f",add_price)).arg(model->item(i,3)->text()).arg("0.00").arg(model->item(i,5)->text()).arg("0").arg(model->item(i,9)->text())
                              .arg(model->item(i,10)->text()).arg(model->item(i,8)->text()).arg(n_func::gs_operid).arg(model->item(i,20)->text()).arg(n_func::gs_operid)
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
                query.bindValue(":str2",QString::number(int_id));
                query.bindValue(":str3",model->item(i,1)->text());
                query.bindValue(":str4",model->item(i,4)->text());
                query.bindValue(":str5",model->item(i,4)->text());
                query.bindValue(":str6",QString().sprintf("%0.2f",add_price));
                query.bindValue(":str7",model->item(i,3)->text());
                query.bindValue(":str8","0.00");
                query.bindValue(":str9",model->item(i,5)->text());
                query.bindValue(":str10","0");
                query.bindValue(":str11",model->item(i,9)->text());
                query.bindValue(":str12",model->item(i,10)->text());
                query.bindValue(":str13",model->item(i,8)->text());
                query.bindValue(":str14",n_func::gs_operid);
                query.bindValue(":str15",model->item(i,20)->text());
                query.bindValue(":str16",n_func::gs_operid);
                query.bindValue(":str17",time);
                query.bindValue(":str18",liushuihao);
                query.bindValue(":str19",model->item(i,7)->text());

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
        else if(model->item(i,5)->text().toFloat() != 100)//折扣菜品
        {
            if(model->item(i,7)->text().isEmpty())
            {
                if(query.exec(QString("insert into cey_u_orderdish(ch_billno,ch_tableno,int_id,ch_dishno,num_price,num_price_org,num_price_add,num_num,num_back,vch_back_operID,dt_back_operdate,int_discount,vch_dis_operID,dt_dis_operdate,vch_memberno,ch_consult,ch_suitflag,ch_suitno,ch_specialflag,ch_presentflag,vch_pre_operID,dt_pre_operdate,vch_operID,dt_operdate,ch_togono,ch_payno,vch_print_memo) values('%1','%2','%3','%4','%5','%6','%7','%8','%9','%10','%11','%12','%13','%14',null,'%15','%16',null,'%17','%18',null,null,'%19','%20',null,'%21',null)")
                              .arg(danhao).arg(zhuohao).arg(QString::number(int_id)).arg(model->item(i,1)->text()).arg(model->item(i,4)->text()).arg(model->item(i,4)->text())
                              .arg(QString().sprintf("%0.2f",add_price)).arg(model->item(i,3)->text()).arg("0.00").arg(n_func::gs_operid).arg(model->item(i,19)->text()).arg(QString::number(int(model->item(i,5)->text().toFloat()))).arg(n_func::gs_operid)
                              .arg(model->item(i,19)->text()).arg("1").arg("N").arg("N").arg("N").arg(n_func::gs_operid).arg(time).arg(liushuihao)))
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
                query.bindValue(":str2",QString::number(int_id));
                query.bindValue(":str3",model->item(i,1)->text());
                query.bindValue(":str4",model->item(i,4)->text());
                query.bindValue(":str5",model->item(i,4)->text());
                query.bindValue(":str6",QString().sprintf("%0.2f",add_price));
                query.bindValue(":str7",model->item(i,3)->text());
                query.bindValue(":str8","0.00");
                query.bindValue(":zhekouren",n_func::gs_operid);
                query.bindValue(":zhekoutime",model->item(i,19)->text());
                query.bindValue(":str9",QString::number(int(model->item(i,5)->text().toFloat())));
                query.bindValue(":str10",n_func::gs_operid);
                query.bindValue(":str11",model->item(i,19)->text());
                query.bindValue(":str12","1");
                query.bindValue(":str13","N");
                query.bindValue(":str14","N");
                query.bindValue(":str15","N");
                query.bindValue(":str16",n_func::gs_operid);
                query.bindValue(":str17",time);
                query.bindValue(":str18",liushuihao);
                query.bindValue(":str19",model->item(i,7)->text());

                if(query.exec())
                {

                }
                else
                {
                    QSqlDatabase::database().rollback();
                    qDebug() << query.lastError();  lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_u_orderdish菜品消费明细表折扣菜品失败!"));
                    
                    return;
                }
            }
        }
        else//普通菜品
        {
            if(model->item(i,7)->text().isEmpty())//无做法
            {
                if(query.exec(QString("insert into cey_u_orderdish(ch_billno,ch_tableno,int_id,ch_dishno,num_price,num_price_org,num_price_add,num_num,num_back,vch_back_operID,dt_back_operdate,int_discount,vch_dis_operID,dt_dis_operdate,vch_memberno,ch_consult,ch_suitflag,ch_suitno,ch_specialflag,ch_presentflag,vch_pre_operID,dt_pre_operdate,vch_operID,dt_operdate,ch_togono,ch_payno,vch_print_memo) values('%1','%2','%3','%4','%5','%6','%7','%8','%9',null,null,'%10',null,null,null,'%11','%12',null,'%13','%14',null,null,'%15','%16',null,'%17',null)")
                              .arg(danhao).arg(zhuohao).arg(QString::number(int_id)).arg(model->item(i,1)->text()).arg(model->item(i,4)->text()).arg(model->item(i,4)->text())
                              .arg(QString().sprintf("%0.2f",add_price)).arg(model->item(i,3)->text()).arg("0.00").arg(model->item(i,5)->text()).arg("0").arg(model->item(i,9)->text())
                              .arg(model->item(i,10)->text()).arg(model->item(i,8)->text()).arg(n_func::gs_operid)
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
                query.bindValue(":str2",QString::number(int_id));
                query.bindValue(":str3",model->item(i,1)->text());
                query.bindValue(":str4",model->item(i,4)->text());
                query.bindValue(":str5",model->item(i,4)->text());
                query.bindValue(":str6",QString().sprintf("%0.2f",add_price));
                query.bindValue(":str7",model->item(i,3)->text());
                query.bindValue(":str8","0.00");
                query.bindValue(":str9",model->item(i,5)->text());
                query.bindValue(":str10","0");
                query.bindValue(":str11",model->item(i,9)->text());
                query.bindValue(":str12",model->item(i,10)->text());
                query.bindValue(":str13",model->item(i,8)->text());
                query.bindValue(":str14",n_func::gs_operid);
                query.bindValue(":str15",time);
                query.bindValue(":str16",liushuihao);
                query.bindValue(":str17",model->item(i,7)->text());

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
    }

    float label_jine = 0.00;
    for(int i = 0; i < modelBase.rowCount(); i++)
    {
        if(modelBase.item(i,8)->text() == "N" && modelBase.item(i,9)->text() != "Y" )
        {
            label_jine += modelBase.item(i,6)->text().toFloat();
        }
    }
    query.prepare(QString("update cey_u_table set num_cost =:num where ch_billno =:danhao"));
    query.bindValue(":num",QString().sprintf("%0.2f",label_jine));
    query.bindValue(":danhao",w_scr_dish_restaurant_dialog::table_danhao);
    if(query.exec())
    {
    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_u_table失败!"));
        return;
    }

    query.prepare(QString("update cey_bt_table set ch_lockflag ='N' where ch_tableno =:table "));
    query.bindValue(":table",frontViewDialog_virtual::cur_desk_no);
    if(query.exec())
    {
    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_bt_table失败!"));
        return;
    }
    QSqlDatabase::database().commit();
    //打印
    for(int i = 0; i < modelBase.rowCount(); i++)
    {
        if(modelBase.item(i,24)->text() == "N")
        {
            modelBase.item(i,24)->setText("Y");
        }
    }

    table_pay_2 pay(this);
    backheader::static_dialogexec(pay, tr("结算界面"));
    if(pay.btn_t_f == true){
        this->close();
        emit pay_over();
    }
}

void table_order_2::on_pushButton_4_clicked()//数量
{
    int row = ui->tableView->tableview->currentIndex().row();// ui->tableView->currentIndex().row();

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

    if(modelBase.item(row,24)->text() == "Y")
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("已落单不允许修改数量!"));
        
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
    guqing.setQuery(QString("Select ifnull(num_num - num_sale , 0)from cey_bt_dish_warn where ch_dishno ='%1' and ch_state ='Y' and DATE_FORMAT(dt_date, '%Y-%m-%d') ='%2' ").arg(modelBase.item(row,1)->text()).arg(guqingdate));
    float guqingnum = 0.00;
    guqingnum = guqing.record(0).value(0).toFloat();
    modfiy_number * modfiy = new modfiy_number(modelBase.item(row,3)->text(),this);
    /*modfiy->exec();*/backheader::static_dialogexec(*modfiy, "");
    if(modfiy->btn_t_f == true)
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
                        yidian += modfiy->modfiy_num;
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

            modelBase.item(row,3)->setText(QString().sprintf("%0.2f",modfiy->modfiy_num));
            float zongjia = modelBase.item(row,3)->text().toFloat() * modelBase.item(row,4)->text().toFloat() * modelBase.item(row,5)->text().toFloat() * 0.01 + modelBase.item(row,3)->text().toFloat() * modelBase.item(row,21)->text().toFloat();
            modelBase.item(row,6)->setText(QString().sprintf("%0.2f",zongjia));
            for(int i = row +1 ; i < modelBase.rowCount(); i++)
            {
                if(modelBase.item(i,9)->text() == "Y")
                {
                    float num = 0.00;
                    num = modelBase.item(i,3)->text().toFloat() * modfiy->modfiy_num;
                    modelBase.item(i,3)->setText(QString().sprintf("%0.2f",num));
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
                        yidian += modfiy->modfiy_num;
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
            modelBase.item(row,3)->setText(QString().sprintf("%0.2f",modfiy->modfiy_num));
            float zongjia = 0.00;
            zongjia = modelBase.item(row,3)->text().toFloat() * modelBase.item(row,4)->text().toFloat() * modelBase.item(row,5)->text().toFloat() * 0.01 + modelBase.item(row,3)->text().toFloat() * modelBase.item(row,21)->text().toFloat();
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
        }this->to_dish_title(label_shuliang, label_jine);

        //        ui->label_3->setText("￥:" + QString().sprintf("%0.2f",label_jine));
        //        ui->label_5->setText(QString().sprintf("%0.2f",label_shuliang));
    }
}

void table_order_2::on_pushButton_13_clicked()//开台信息
{
    founding_cook cook(k_danhao,k_zhuohao,k_renshu,k_kaitairen,k_time,"2",this);
    /*cook.exec();*/backheader::static_dialogexec(cook, "");
    if(cook.btn_t_f == true)
    {
        if(cook.beizhu != k_beizhu || cook.songcanren != k_fuwuyuan || cook._renshu != k_renshu)
        {
            qDebug() << cook._renshu;
            k_renshu = cook._renshu;
            k_beizhu = cook.beizhu;
            k_kaitairen = cook.songcanren;
            ;
            QSqlQuery query;
            QSqlDatabase::database().transaction();
            query.prepare(QString("update cey_u_table set int_person=:person , vch_waiter= :fuwuyuan , vch_memo= :beizhu where ch_billno= :danhao "));
            query.bindValue(":person",cook._renshu.toInt());
            query.bindValue(":fuwuyuan",cook.songcanren);
            query.bindValue(":beizhu",cook.beizhu);
            query.bindValue(":danhao",k_danhao);
            if(query.exec())
            {
            }
            else
            {
                QSqlDatabase::database().rollback();
                lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_bt_tabler失败！"));
                
                return;
            }

            QSqlDatabase::database().commit();

            k_renshu = cook._renshu;
            k_beizhu = cook.beizhu;
            k_fuwuyuan = cook.songcanren;
        }
    }
}

void table_order_2::on_pushButton_2_clicked()//菜品加1
{
    ;
    int row = ui->tableView->tableview->currentIndex().row();// ui->tableView->currentIndex().row();
    int i = row + 1;
    if(row <= -1)
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("请选择一条需要修改数量的菜品!"));
        
        return;
    }

    if(modelBase.item(row,24)->text() == "Y")
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("已落单不允许修改数量!"));
        
        return;
    }

    if(modelBase.item(row,9)->text() == "Y")
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("套菜明细不允许修改数量!"));
        
        return;
    }
    float num = 0.00;
    num = modelBase.item(row,3)->text().toFloat();
    num += 1.00;
    modelBase.item(row,3)->setText(QString().sprintf("%0.2f",num));
    QSqlQueryModel tao;

    if(modelBase.item(row,9)->text() == "P")
    {
        for(i;i < modelBase.rowCount(); i++)
        {
            if(modelBase.item(i,9)->text() == "Y")
            {
                tao.clear();
                tao.setQuery(QString("Select * from cey_bt_dish_suit where ch_suitno ='%1'").arg(modelBase.item(row,1)->text()));
                float num1 = 0.00;
                num1 = modelBase.item(i,3)->text().toFloat()  + tao.record(0).value(3).toFloat() * 1.00;
                modelBase.item(i,3)->setText(QString().sprintf("%0.2f",num1));
            }
            else
            {
                break;
            }
        }
    }
    //    ui->tableView->viewport()->update();
    float zongjia = 0.00;
    zongjia = modelBase.item(row,3)->text().toFloat() * modelBase.item(row,4)->text().toFloat() * modelBase.item(row,5)->text().toFloat() * 0.01 + modelBase.item(row,3)->text().toFloat() * modelBase.item(row,21)->text().toFloat();
    modelBase.item(row,6)->setText(QString().sprintf("%0.2f",zongjia));

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
    this->to_dish_title(label_shuliang, label_jine);

    //    ui->label_3->setText("￥:" + QString().sprintf("%0.2f",label_jine));
    //    ui->label_5->setText(QString().sprintf("%0.2f",label_shuliang));
}

void table_order_2::on_pushButton_3_clicked()//菜品减1
{
    ;
    int row = ui->tableView->tableview->currentIndex().row();// ui->tableView->currentIndex().row();
    int i = row + 1;
    if(row <= -1)
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("请选择一条需要修改数量的菜品!"));
        
        return;
    }

    if(modelBase.item(row,24)->text() == "Y")
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("已落单不允许修改数量!"));
        
        return;
    }

    if(modelBase.item(row,9)->text() == "Y")
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("套菜明细不允许修改数量!"));
        
        return;
    }

    float num = 0.00;
    num = modelBase.item(row,3)->text().toFloat();
    if(num - 1.00 <= 0)
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("修改菜品数量不能小于等于零!"));
        
        return;
    }

    num -= 1.00;
    modelBase.item(row,3)->setText(QString().sprintf("%0.2f",num));
    QSqlQueryModel tao;

    if(modelBase.item(row,9)->text() == "P")
    {
        for(i;i < modelBase.rowCount(); i++)
        {
            if(modelBase.item(i,9)->text() == "Y")
            {
                tao.clear();
                tao.setQuery(QString("Select * from cey_bt_dish_suit where ch_suitno ='%1'").arg(modelBase.item(row,1)->text()));
                float num1 = 0.00;
                num1 = modelBase.item(i,3)->text().toFloat()  - tao.record(0).value(3).toFloat() * 1.00;
                modelBase.item(i,3)->setText(QString().sprintf("%0.2f",num1));
            }
            else
            {
                break;
            }
        }
    }
    //    ui->tableView->viewport()->update();
    float zongjia = 0.00;
    zongjia = modelBase.item(row,3)->text().toFloat() * modelBase.item(row,4)->text().toFloat() * modelBase.item(row,5)->text().toFloat() * 0.01 + modelBase.item(row,3)->text().toFloat() * modelBase.item(row,21)->text().toFloat();
    modelBase.item(row,6)->setText(QString().sprintf("%0.2f",zongjia));

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
    this->to_dish_title(label_shuliang, label_jine);

    //    ui->label_3->setText("￥:" + QString().sprintf("%0.2f",label_jine));
    //    ui->label_5->setText(QString().sprintf("%0.2f",label_shuliang));
}

void table_order_2::on_tableView_clicked(const QModelIndex &index)
{
    int row = ui->tableView->tableview->currentIndex().row();// ui->tableView->currentIndex().row();
    if(modelBase.rowCount() <= -1)
    {
        return;
    }

    if(modelBase.item(row,24)->text() == "Y")
    {
        ui->pushButton_quitdish->setText(tr("退菜"));
    }
    else
    {
        ui->pushButton_quitdish->setText(tr("删菜"));
    }
}

void table_order_2::on_pushButton_14_clicked()//消台
{
    if(tocanceldeak())
        this->close();
}

bool table_order_2::tocanceldeak()
{
    QSqlQuery query;
    QSqlDatabase::database().transaction();
    query.exec(QString("select ch_billno from cey_bt_table where ch_tableno = '%1' ").arg(frontViewDialog_virtual::cur_desk_no));
    query.next();
    if(query.record().value(0).toString().isEmpty()){
        lds_messagebox::warning(0, tr("中餐消台"), tr("该台没有开台,操作取消"));
        return false;
    }
    query.exec(QString("select count(*) from cey_u_orderdish a where  a.ch_billno = '%1' and a.ch_tableno ='%2'").arg(w_scr_dish_restaurant_dialog::table_danhao).arg(frontViewDialog_virtual::cur_desk_no));
    query.next();
    if(query.record().value(0).toLongLong() > 0)
    {
        lds_messagebox::warning(0, tr("消息提示"),tr("当前桌位存在菜品消费,不能取消开台!"));

        return false;
    }

    if(0==lds_messagebox::warning(0, tr("消息提示"),tr("是否确定取消此开台?"),tr("确认"),tr("取消")))
    {
        query.prepare(QString("update cey_bt_table set ch_billno =null , ch_lockflag ='N'  where  ch_tableno = :table_no"));
        query.bindValue(":table_no",frontViewDialog_virtual::cur_desk_no);
        if(query.exec())
        {
        }
        else
        {
            QSqlDatabase::database().rollback();
            lds_messagebox::warning(0, tr("消息提示"),tr("更新cey_bt_table失败!"));
            return false;
        }

        QSqlQueryModel model;
        model.setQuery(QString("select NOW()"));
        QString time = model.record(0).value(0).toDateTime().toString("yyyy-MM-dd hh:mm:ss");
        query.prepare(QString("update cey_u_table set ch_state ='0' , vch_operID2 =:oper , dt_operdate2 =:time where ch_tableno = :table_no and ch_billno = :danhao"));
        query.bindValue(":oper",n_func::gs_operid);
        query.bindValue(":time",time);
        query.bindValue(":table_no",frontViewDialog_virtual::cur_desk_no);
        query.bindValue(":danhao",w_scr_dish_restaurant_dialog::table_danhao);
        if(query.exec())
        {
        }
        else
        {
            QSqlDatabase::database().rollback();
            lds_messagebox::warning(0, tr("消息提示"),tr("更新cey_bt_table失败!"));
            return false;
        }

        query.prepare(QString("update cey_u_master set ch_state ='0' , vch_operID2 =:oper , dt_operdate2 =:time where  ch_billno =:danhao"));
        query.bindValue(":oper",n_func::gs_operid);
        query.bindValue(":time",time);
        query.bindValue(":danhao",w_scr_dish_restaurant_dialog::table_danhao);
        if(query.exec())
        {
        }
        else
        {
            QSqlDatabase::database().rollback();
            lds_messagebox::warning(0, tr("消息提示"),tr("更新cey_u_master失败!"));
            return false;
        }

        query.prepare(QString("update cey_bt_table set  ch_lockflag ='N'  where ch_tableno = :zhuohao"));
        query.bindValue(":zhuohao",frontViewDialog_virtual::cur_desk_no);
        if(query.exec())
        {
        }
        else
        {
            QSqlDatabase::database().rollback();
            lds_messagebox::warning(0, tr("消息提示"),tr("更新cey_bt_tabler失败！"));

            return false;
        }

        QSqlDatabase::database().commit();
        return true;
    }
    return false;
}

bool table_order_2::toswitchdesk()
{
    QSqlQuery query;
    QSqlDatabase::database().transaction();
    query.exec(QString("select ch_billno from cey_bt_table where ch_tableno = '%1' ").arg(frontViewDialog_virtual::cur_desk_no));
    query.next();
    if(query.record().value(0).toString().isEmpty()){
        lds_messagebox::warning(0, tr("中餐转台"), tr("该台没有开台,操作取消"));
        return false;
    }
    cook_zhuantai_quanbu operation(frontViewDialog_virtual::cur_desk_no,0);
    backheader::static_dialogexec(operation, tr("转台操作"));
    if(operation.btn_t_f == true)
    {

        //        query.prepare(QString("update cey_u_table set num_cost =:num where ch_billno =:danhao"));
        //        query.bindValue(":num","0.00");
        //        query.bindValue(":danhao",w_scr_dish_restaurant_dialog::table_danhao);
        //        if(query.exec())
        //        {
        //        }
        //        else
        //        {
        //            QSqlDatabase::database().rollback();
        //            lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_u_table失败！"));
        //
        //            return;
        //        }
        //服务费还没有算
        float fuwufei = 0.00;
        query.prepare(QString("update cey_u_table set ch_tableno =:zhuohao , num_trans_service  =:fuwufei where  ch_billno = :danhao and ch_tableno =:zhuohao_y"));
        query.bindValue(":zhuohao",operation.zhuan_zhuohao);
        query.bindValue(":fuwufei",QString().sprintf("%0.2f",fuwufei));
        query.bindValue(":danhao",w_scr_dish_restaurant_dialog::table_danhao);
        query.bindValue(":zhuohao_y",frontViewDialog_virtual::cur_desk_no);
        // qDebug() <<query.boundValues();
        if(query.exec())
        {
        }
        else
        {
            QSqlDatabase::database().rollback();
            lds_messagebox::warning(0, tr("消息提示"),tr("更新cey_u_table失败！"));

            return false;
        }

        query.prepare(QString("update cey_u_orderdish set ch_tableno = :zhuohao where  ch_billno =:danhao and ch_tableno = :zhuohao_y"));
        query.bindValue(":zhuohao",operation.zhuan_zhuohao);
        query.bindValue(":danhao",w_scr_dish_restaurant_dialog::table_danhao);
        query.bindValue(":zhuohao_y",frontViewDialog_virtual::cur_desk_no);
        // qDebug() <<query.boundValues();
        if(query.exec())
        {
        }
        else
        {
            QSqlDatabase::database().rollback();
            lds_messagebox::warning(0, tr("消息提示"),tr("更新cey_u_orderdish失败！"));

            return false;
        }

        query.prepare(QString("update cey_bt_table set ch_lockflag ='Y' , ch_billno = :danhao where  ch_tableno = :zhuohao"));
        query.bindValue(":danhao",w_scr_dish_restaurant_dialog::table_danhao);
        query.bindValue(":zhuohao",operation.zhuan_zhuohao);
        //  qDebug() <<query.boundValues();
        if(query.exec())
        {
        }
        else
        {
            QSqlDatabase::database().rollback();
            lds_messagebox::warning(0, tr("消息提示"),tr("更新cey_u_table失败！"));

            return false;
        }

        query.prepare(QString("update cey_u_checkout_master set ch_tableno = :zhuohao where ch_billno = :danhao and ch_tableno = :zhuohao_y"));
        query.bindValue("zhuohao:",operation.zhuan_zhuohao);
        query.bindValue(":danhao",w_scr_dish_restaurant_dialog::table_danhao);
        query.bindValue(":zhuohao_y",frontViewDialog_virtual::cur_desk_no);
        // qDebug() <<query.boundValues();
        if(query.exec())
        {
        }
        else
        {
            QSqlDatabase::database().rollback();
            lds_messagebox::warning(0, tr("消息提示"),tr("更新cey_u_table失败！"));

            return false;
        }

        query.prepare(QString("update cey_bt_table set ch_billno =null , ch_lockflag ='N'  where  ch_tableno = :zhuohao_y"));
        query.bindValue(":zhuohao_y",frontViewDialog_virtual::cur_desk_no);
        //qDebug() <<query.boundValues();
        if(query.exec())
        {
        }
        else
        {
            QSqlDatabase::database().rollback();
            lds_messagebox::warning(0, tr("消息提示"),tr("更新cey_u_table失败！"));

            return false;
        }
        QSqlDatabase::database().commit();
        //更新页面
        frontViewDialog_virtual::cur_desk_no = operation.zhuan_zhuohao;
        return true;
    }
    return false;
}

void table_order_2::on_pushButton_15_clicked()//菜品转台
{
    int num = 0;
    for(int i = 0; i < modelBase.rowCount(); i++)
    {
        if(modelBase.item(i,24)->text() == "Y")
        {
            num++;
        }
    }

    if(num <= 0)
    {
        lds_messagebox::warning(this, tr("消息提示"),"当前餐桌没有可转出的菜品记录,请核实!");
        
        return;
    }

    QSqlQuery query;
    QSqlDatabase::database().transaction();
    cook_zhuantai zhuantai(this);
    /*zhuantai.exec();*/backheader::static_dialogexec(zhuantai, tr("菜品转台"));
    if(zhuantai.btn_t_f == true)//确定转台
    {
        if(zhuantai.print == true)//转台打印
        {
            print_zhuantai();
        }

        QSqlQueryModel model;
        for(int i = 0; i < zhuantai.hanghao.count(); i++)
        {
            model.clear();
            model.setQuery(QString("Select IFNULL( max( int_id), 0) from cey_u_orderdish where ch_billno ='%1' and ch_tableno ='%2'").arg(zhuantai.danhao).arg(zhuantai.zhuohao));
            int id = 0;
            id = model.record(0).value(0).toInt();
            id++;
            query.prepare(QString("update cey_u_orderdish set ch_billno =:danhao , ch_tableno =:zhuohao , int_id =:int_id  where int_flowID = :flow_id"));
            query.bindValue(":danhao",zhuantai.danhao);
            query.bindValue(":zhuohao",zhuantai.zhuohao);
            query.bindValue(":int_id",QString::number(id));
            query.bindValue(":flow_id",zhuantai.flow_id[i]);
            if(query.exec())
            {
            }
            else
            {
                QSqlDatabase::database().rollback();
                lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_u_orderdish失败！"));
                
                return;
            }

            model.clear();
            model.setQuery(QString("Select IFNULL( sum((num_num - num_back)*( num_price + num_price_add)) , 0)from cey_u_orderdish where ch_billno ='%1' and ch_tableno ='%2'").arg(w_scr_dish_restaurant_dialog::table_danhao).arg(frontViewDialog_virtual::cur_desk_no));
            QString str;
            str = QString().sprintf("%0.5f",model.record(0).value(0).toFloat());
            query.prepare("update cey_u_table set num_cost = :num where ch_billno = :danhao and ch_tableno =:zhuoaho ");
            query.bindValue(":num",str);
            query.bindValue(":danhao",w_scr_dish_restaurant_dialog::table_danhao);
            query.bindValue(":zhuoaho",frontViewDialog_virtual::cur_desk_no);
            if(query.exec())
            {
            }
            else
            {
                QSqlDatabase::database().rollback();
                lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_u_table失败！"));
                
                return;
            }

            model.clear();
            model.setQuery(QString("Select IFNULL( sum((num_num - num_back)*( num_price + num_price_add)) , 0)from cey_u_orderdish where ch_billno ='%1' and ch_tableno ='%2'").arg(zhuantai.danhao).arg(zhuantai.zhuohao));
            str.clear();
            str = QString().sprintf("%0.5f",model.record(0).value(0).toFloat());
            query.prepare(QString("update cey_u_table set num_cost =:num  where ch_billno =:danhao and ch_tableno =:zhuoaho"));
            query.bindValue(":num",str);
            query.bindValue(":danhao",zhuantai.danhao);
            query.bindValue(":zhuoaho",zhuantai.zhuohao);
            if(query.exec())
            {
            }
            else
            {
                QSqlDatabase::database().rollback();
                lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_u_table失败！"));
                
                return;
            }
        }

        //更新主页面
        for(int i = 0; i < modelBase.rowCount(); i++)
        {
            for(int k = 0; k < zhuantai.hanghao.count(); k++)
            {
                if(modelBase.item(i,0)->text() == zhuantai.hanghao[k])
                {
                    modelBase.removeRow(i);
                }
            }
        }

        for(int i = 0; i < modelBase.rowCount(); i++)
        {
            modelBase.item(i,0)->setText(QString::number(i+1));
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
        }this->to_dish_title(label_shuliang, label_jine);

        //        ui->label_3->setText("￥:" + QString().sprintf("%0.2f",label_jine));
        //        ui->label_5->setText(QString().sprintf("%0.2f",label_shuliang));
    }

}

void table_order_2::print_zhuantai()
{

}

void table_order_2::print_zhuanquan()
{

}

void table_order_2::print_back_one()
{
    int row = ui->tableView->tableview->currentIndex().row();// ui->tableView->currentIndex().row();
    QSettings *kitchenprinter = new  QSettings(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    QByteArray content_kitchen;
    QSqlQueryModel t_time;
    t_time.setQuery(QString("select NOW()"));
    QString str;
    str =  kitchenprinter->value("w_bt_dish_Kitchenprinter_param/printer_type_com").toString();
    QString str_type = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/printer_type_com").toString();

    int hanghao = 0;
    QSqlQueryModel model;
    model.setQuery(QString("select ch_printflag,vch_printer,ch_outflag,vch_outprint from cey_bt_dish where ch_dishno = '%1'").arg(modelBase.item(row,1)->text()));
    QString iphao = model.record(0).value(1).toString();
    if(model.record(0).value(0).toString() == tr("Y"))//后台进行了厨打设置
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
            liushuhao=system_setting::get_max_serialno_add1(bendandanhao);
            content_kitchen.append(liushuhao);
        }

        //开始单号
        str.clear();
        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/openbillno_check").toString();
        if(str == "1")
        {
            content_kitchen.append(tr("开始单号:") + w_scr_dish_restaurant_dialog::table_danhao + "\n");
        }

        //餐桌名
        str.clear();
        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/tablename_check").toString();
        if(str == "1")
        {
            content_kitchen.append(tr("餐桌名:") + frontViewDialog_virtual::cur_desk_no + "\n");
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
            content_kitchen.append(tr("点菜时间:") + back_diancaitime  + "\n");
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
            //content_kitchen.append(tr("人数:1\n"));
            QSqlQueryModel person;
            person.setQuery(QString("select int_person from cey_u_table where ch_billno= '%1' ").arg(w_scr_dish_restaurant_dialog::table_danhao));
            content_kitchen.append(tr("人数:") + person.record(0).value(0).toString() + "\n");
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
        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/dish_bill_printtype_check").toString();
        if(str == "1")
        {
            QString daleimingcheng;
            QSqlQueryModel mymodel;
            mymodel.setQuery(QString("select vch_sub_typename from cey_bt_dish_subtype where ch_sub_typeno = '%1' ").arg(modelBase.item(row,23)->text()));
            if(str_type == "0")
            {
                daleimingcheng = tr("            ---------") + mymodel.record(0).value(0).toString() + tr("---------\n");
            }
            else
            {
                daleimingcheng = tr("      ---------") + mymodel.record(0).value(0).toString() + tr("---------\n");
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
            if(modelBase.item(row,8)->text() == "Y")
            {
                QString zb =("赠)");
                content_kitchen.append(zb);
            }
        };

        if(modelBase.item(row,9)->text() == "Y")
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
            QString pinming = modelBase.item(row,2)->text();

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
            QString danw = modelBase.item(row,12)->text();

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
            QString pinma = modelBase.item(row,1)->text();

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
                shuliang = QString::number(int(backone.toFloat()));
            }
            else if(str == "1")
            {
                shuliang = QString().sprintf("%0.1f",backone.toFloat());
            }
            else
            {
                shuliang = QString().sprintf("%0.2f",backone.toFloat());
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
                jiage = QString::number(int(modelBase.item(row,4)->text().toFloat()));
            }
            else if(str == "1")
            {
                jiage = QString().sprintf("%0.1f",modelBase.item(row,4)->text().toFloat());
            }
            else
            {
                jiage = QString().sprintf("%0.2f",modelBase.item(row,4)->text().toFloat());
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
            QString pinming = modelBase.item(row,2)->text();

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
            QString danw = modelBase.item(row,12)->text();

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
            QString pinma = modelBase.item(row,1)->text();

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
                shuliang = QString::number(int(backone.toFloat()));
            }
            else if(str == "1")
            {
                shuliang = QString().sprintf("%0.1f",backone.toFloat());
            }
            else
            {
                shuliang = QString().sprintf("%0.2f",backone.toFloat());
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
                jiage = QString::number(int(modelBase.item(row,4)->text().toFloat()));
            }
            else if(str == "1")
            {
                jiage = QString().sprintf("%0.1f",modelBase.item(row,4)->text().toFloat());
            }
            else
            {
                jiage = QString().sprintf("%0.2f",modelBase.item(row,4)->text().toFloat());
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
            if(str_type == "0")
            {
                content_kitchen.append("----------------------------\n");
            }
            else
            {
                content_kitchen.append("--------------------------------\n");
            }
            content_kitchen.append(tr("菜品合计金额:") + QString().sprintf("%0.2f",backone.toFloat() * modelBase.item(row,4)->text().toFloat()));
        }
        //打印份数
        str.clear();
        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/kitchen_num_spin").toString();
        int print_count = str.toInt();
        blockIns b;
        QString path = iphao + ":9100";
        louda_ip = path;
        louda_neirong = content_kitchen;
        if(cui_cai == true && luo_dan == false && tui_cai == false)
        {
            louda_title = tr("催菜单");
        }
        else if(cui_cai == false && luo_dan == false && tui_cai == true)
        {
            louda_title = tr("退菜出品单");
        }
        else if(cui_cai == false && luo_dan == true && tui_cai == false)
        {
            louda_title = tr("厨房打印单");
        }

        //        public_mainprinter *chuda_print = new public_mainprinter();
        str.clear();
        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/printer_fontsize_com").toString();
        if(str == "1")//普通字体
        {
            b+=backheader::printer->drawText("1B40","");
            if(cui_cai == false && luo_dan == false && tui_cai == true)
            {
                b+=backheader::printer->drawText("1B40 1B6101 1C5700",tr("退菜出品单\n"));//居中放大
            }
            else if(cui_cai == true && luo_dan == false && tui_cai == false)
            {
                b+=backheader::printer->drawText("1B40 1B6101 1C5700",tr("催菜单\n"));//居中放大
            }
            else if(cui_cai == false && luo_dan == true && tui_cai == false)
            {
                b+=backheader::printer->drawText("1B40 1B6101 1C5700",tr("厨房打印单\n"));//居中放大
            }

            b += backheader::printer->drawText("1B40 1C5700", content_kitchen, Qt::AlignLeft);

        }
        else
        {
            b+=backheader::printer->drawText("1B40","");
            if(cui_cai == false && luo_dan == false && tui_cai == true)
            {
                b+=backheader::printer->drawText("1B40 1B6101 1C5701",tr("退菜出品单\n"));//居中放大
            }
            else if(cui_cai == true && luo_dan == false && tui_cai == false)
            {
                b+=backheader::printer->drawText("1B40 1B6101 1C5701",tr("催菜单\n"));//居中放大
            }
            else if(cui_cai == false && luo_dan == true && tui_cai == false)
            {
                b+=backheader::printer->drawText("1B40 1B6101 1C5701",tr("厨房打印单\n"));//居中放大
            }
            b += backheader::printer->drawText("1B40 1D2101", content_kitchen, Qt::AlignLeft);
        }
        b += backheader::printer->drawText("1B6405 1D5601", "");//走纸5x， 切纸
        b=backheader::printer->setPrintCount(b, print_count);
        // printer->asynchPrint(b, path);
        if(backheader::syn_dataprint(b,path) == true)//成功,更新数据库打印菜品大类
        {
            QSqlQuery query;
            QSqlDatabase::database().transaction();

            query.prepare(QString("update cey_u_orderdish set ch_printflag ='Y' where ch_billno =:danhao and ch_dishno = :bianma and int_flowId = :id"));
            query.bindValue(":danhao",w_scr_dish_restaurant_dialog::table_danhao);
            query.bindValue(":bianma",modelBase.item(row,1)->text());
            query.bindValue(":id",modelBase.item(row,25)->text());
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

void table_order_2::on_pushButton_16_clicked()//全部转台
{
    if(toswitchdesk())this->reject();
}

void table_order_2::on_pushButton_12_clicked()//临时菜
{
    QList<frontViewDialog_list::list_item> list_item_list;
    linshi_cook linshi(this);
    backheader::static_dialogexec(linshi, tr("临时菜品"));
    if(linshi.btn_t_f)
    {
        QSqlQueryModel diancai_model;
        int num = 0;
        diancai_model.setQuery(QString("select * from cey_bt_dish where ch_tempflag = 'Y'  and  ch_stopflag = 'N' and  ch_dishno = '%1' ").arg(linshi.no));
        QSqlQueryModel zufa_model;
        QFont font;
        font.setPointSize(14);
        ;
        QSqlQueryModel time_model;
        time_model.setQuery(QString("Select NOW()"));
        QDateTime datetime = time_model.record(0).value(0).toDateTime();
        QString date = datetime.toString("yyyy-MM-dd");
        QString time;
        QSqlQueryModel zitaocai_tem;
        QSqlQueryModel model_specil;
        model_specil.setQuery(QString("Select IFNULL(num_price , 0)from cey_bt_dish_special where ch_dishno ='%1' and ch_state ='Y' and left( dt_operdate_effect , 10)='%2' ").arg(diancai_model.record(num).value(0).toString()).arg(date));

        QSqlQueryModel model;
        QSqlQueryModel model_danwei;
        model.setQuery(QString("Select IFNULL(num_num - num_sale , 0)from cey_bt_dish_warn where ch_dishno ='%1' and ch_state ='Y'  and left( dt_date , 10)='%2' ").arg(diancai_model.record(num).value(0).toString()).arg(date));
        //qDebug() << QString("Select ifnull(num_num - num_sale , 0)from cey_bt_dish_warn where ch_dishno ='%1' and ch_state ='Y' and DATE_FORMAT(dt_date, '%Y-%m-%d')  ='%2' ").arg(diancai_model.record(num).value(0).toString()).arg(date);
        model_danwei.setQuery(QString("select * from cey_bt_unit where ch_unitno = '%1' ").arg(diancai_model.record(num).value(5).toString()));
        QString danwei = model_danwei.record(0).value(1).toString();
        float zuofajiage;
        zuofajiage = 0.00;
        //        if(model.rowCount() > 0)
        //        {
        //            if(model.record(0).value(0).toFloat() <= 0)
        //            {
        //                lds_messagebox::warning(this, tr("消息提示"),tr("菜品已经估清,请确认!"));
        //
        //                return;
        //            }
        //            else
        //            {
        //                float zongdian = 0.00;
        //                for(int i = 0; i < modelBase.rowCount(); i++)
        //                {
        //                    if(modelBase.item(i,1)->text() == diancai_model.record(num).value(0).toString())
        //                    {
        //                        zongdian += modelBase.item(i,3)->text().toFloat();
        //                    }

        //                }


        //                if(zongdian < model.record(0).value(0).toFloat())
        //                {
        //                    float sheng = 0.00;
        //                    sheng = model.record(0).value(0).toFloat()  - zongdian - model_danwei.record(0).value(3).toFloat();
        //                    QString str = tr("菜品:") + diancai_model.record(num).value(1).toString() + tr("已经估清,扣除此次数量,\n剩余数量为:") + QString().sprintf("%0.1f",sheng) + tr("!");
        //                    lds_messagebox::warning(this, tr("消息提示"),str);
        //
        //                }
        //                else
        //                {
        //                    lds_messagebox::warning(this, tr("消息提示"),tr("菜品已经估清,请确认!"));
        //
        //                    return;
        //                }

        //            }


        QSqlQueryModel model_num;
        model_num.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='ordertonum' "));
        QString cook_num = model_num.record(0).value(0).toString();
        float zitaocaishuling = 0.00;
        if(diancai_model.record(num).value(9).toString() == "Y")//时价菜
        {
            cur_price curprice(QString().sprintf("%0.1f",diancai_model.record(num).value(6).toFloat()), this);
            float shuishou = 0.00;
            shuishou = diancai_model.record(num).value(35).toFloat();
            /*curprice.exec();*/backheader::static_dialogexec(curprice, tr("点菜价格修改"));
            if(curprice.btn_t_f == true)
            {
                QString suit;
                if(diancai_model.record(num).value(7).toString() == "Y")
                {
                    suit = "P";
                }
                else
                {
                    suit = "N";
                }
                QString jiage = QString().sprintf("%0.2f",curprice.price.toFloat());
                QString zhutaobianma = diancai_model.record(num).value(0).toString();
                int row = modelBase.rowCount();
                modelBase.setRowCount(row + 1);
                modelBase.setItem(row,0,new QStandardItem(QString::number(row+1))); //序号
                modelBase.setItem(row,1,new QStandardItem(diancai_model.record(num).value(0).toString())); //编码
                modelBase.setItem(row,2,new QStandardItem(diancai_model.record(num).value(1).toString())); //品名
                modelBase.setItem(row,3,new QStandardItem(QString().sprintf("%0.2f",model_danwei.record(0).value(3).toFloat()))); //数量
                // modelBase.setItem(row,4,new QStandardItem(QString().sprintf("%0.2f",diancai_model.record(num).value(6).toFloat()))); //金额
                modelBase.setItem(row,4,new QStandardItem(QString().sprintf("%0.2f",jiage.toFloat() * model_danwei.record(0).value(3).toFloat()))); //金额
                modelBase.setItem(row,5,new QStandardItem("100")); //折扣值
                modelBase.setItem(row,6,new QStandardItem(QString().sprintf("%0.2f",jiage.toFloat() * model_danwei.record(0).value(3).toFloat()))); //总价
                modelBase.setItem(row,7,new QStandardItem(tr(""))); //特殊做法
                modelBase.setItem(row,8,new QStandardItem("N")); //是否赠送
                modelBase.setItem(row,9,new QStandardItem(suit)); //是否套菜
                modelBase.setItem(row,10,new QStandardItem("N")); //是否特价
                modelBase.setItem(row,11,new QStandardItem("N")); //是否优惠
                modelBase.setItem(row,12,new QStandardItem(danwei)); //单位
                modelBase.setItem(row,13,new QStandardItem("")); //赠送
                modelBase.setItem(row,14,new QStandardItem("")); //定折人
                modelBase.setItem(row,15,new QStandardItem("Y")); //是否时价菜
                modelBase.setItem(row,16,new QStandardItem("")); //子套菜所在主套菜编码
                modelBase.setItem(row,17,new QStandardItem(QString().sprintf("%0.2f",shuishou)));//税点
                modelBase.setItem(row,18,new QStandardItem(diancai_model.record(num).value(8).toString()));//是否折扣
                modelBase.setItem(row,19,new QStandardItem(time));//折扣时间
                modelBase.setItem(row,20,new QStandardItem(time));//赠送时间
                modelBase.setItem(row,21,new QStandardItem(QString().sprintf("%0.2f",zuofajiage)));//做法加价
                modelBase.setItem(row,22,new QStandardItem(diancai_model.record(num).value(4).toString())); //所属大类编码
                modelBase.setItem(row,23,new QStandardItem(diancai_model.record(num).value(14).toString())); //所属小类编码
                modelBase.setItem(row,24,new QStandardItem("N")); //是否落单
                modelBase.setItem(row,25,new QStandardItem(""));//落单FLOWID
                modelBase.setItem(row,26,new QStandardItem("0.00"));//已退量
                row_n = row;
                //                ui->tableView->selectRow(row);

                if(cook_num == "1")//自动录入点菜数量
                {
                    modfiy_number * modfiy = new modfiy_number(modelBase.item(row,3)->text(), this);
                    /*modfiy->exec();*/backheader::static_dialogexec(*modfiy, "");
                    if(modfiy->btn_t_f == true)
                    {
                        modelBase.item(row,3)->setText(QString().sprintf("%0.2f",modfiy->modfiy_num));
                        zitaocaishuling = modfiy->modfiy_num;
                        float zongjia = modelBase.item(row,3)->text().toFloat() * modelBase.item(row,4)->text().toFloat() * modelBase.item(row,5)->text().toFloat() * 0.01;
                        modelBase.item(row,6)->setText(QString().sprintf("%0.2f",zongjia));
                    }
                }

                //存在特殊做法
                if(suit != "P" && suit != "Y")
                {
                    zufa_model.clear();
                    zufa_model.setQuery(QString("SELECT a.ch_no,a.vch_cp_memo,a.ch_typeno,a.num_add_price FROM cey_bt_cp_memo a,cey_bt_cp_dish b WHERE(a.ch_no = b.ch_no)and( b.ch_dishno = '%1')").arg(diancai_model.record(num).value(0).toString()));
                    if(zufa_model.rowCount() > 0)
                    {
                        w_scr_dish_special_cook special(diancai_model.record(num).value("ch_dishno").toString(),"","0.00");
                        /*special.exec();*/backheader::static_dialogexec(special, tr("特殊做法"));


                        if(special.btn_t_f == true)
                        {
                            modelBase.item(row,7)->setText(special.zuofaneirong);
                            modelBase.item(row,21)->setText(QString().sprintf("%0.2f",special.add_price));
                            float aa;
                            aa = 0.00;
                            aa = modelBase.item(row,3)->text().toFloat() * modelBase.item(row,4)->text().toFloat() * modelBase.item(row,5)->text().toFloat() * 0.01 + modelBase.item(row,3)->text().toFloat() * special.add_price;
                            modelBase.item(row,6)->setText(QString().sprintf("%0.2f",aa));
                        }
                    }
                }

            }
        }
        else if(model_specil.rowCount() > 0 && diancai_model.record(num).value(9).toString() != "Y")//菜品为特价
        {
            QString suit;
            if(diancai_model.record(num).value(7).toString() == "Y")
            {
                suit = "P";
            }
            else
            {
                suit = "N";
            }
            QString jiage = QString().sprintf("%0.2f",model_specil.record(0).value(0).toFloat());
            QString zhutaobianma = diancai_model.record(num).value(0).toString();
            float shuishou = 0.00;
            shuishou = diancai_model.record(num).value(35).toFloat();
            int row = modelBase.rowCount();
            modelBase.setRowCount(row + 1);
            modelBase.setItem(row,0,new QStandardItem(QString::number(row+1))); //序号
            modelBase.setItem(row,1,new QStandardItem(diancai_model.record(num).value(0).toString())); //编码
            modelBase.setItem(row,2,new QStandardItem(diancai_model.record(num).value(1).toString())); //品名
            modelBase.setItem(row,3,new QStandardItem(QString().sprintf("%0.2f",model_danwei.record(0).value(3).toFloat()))); //数量
            // modelBase.setItem(row,4,new QStandardItem(QString().sprintf("%0.2f",diancai_model.record(num).value(6).toFloat()))); //金额
            modelBase.setItem(row,4,new QStandardItem(QString().sprintf("%0.2f",jiage.toFloat()))); //金额
            modelBase.setItem(row,5,new QStandardItem("100")); //折扣值
            modelBase.setItem(row,6,new QStandardItem(QString().sprintf("%0.2f",jiage.toFloat() * model_danwei.record(0).value(3).toFloat() ))); //总价
            modelBase.setItem(row,7,new QStandardItem(tr(""))); //特殊做法
            modelBase.setItem(row,8,new QStandardItem("N")); //是否赠送
            modelBase.setItem(row,9,new QStandardItem(suit)); //是否套菜
            modelBase.setItem(row,10,new QStandardItem("Y")); //是否特价
            modelBase.setItem(row,11,new QStandardItem("N")); //是否优惠
            modelBase.setItem(row,12,new QStandardItem(danwei)); //单位
            modelBase.setItem(row,13,new QStandardItem("")); //赠送
            modelBase.setItem(row,14,new QStandardItem("")); //定折人
            modelBase.setItem(row,15,new QStandardItem("N")); //是否时价菜
            modelBase.setItem(row,16,new QStandardItem("")); //子套菜所在主套菜编码
            modelBase.setItem(row,17,new QStandardItem(QString().sprintf("%0.2f",shuishou)));//税点
            modelBase.setItem(row,18,new QStandardItem(diancai_model.record(num).value(8).toString()));//是否折扣
            modelBase.setItem(row,19,new QStandardItem(time));//折扣时间
            modelBase.setItem(row,20,new QStandardItem(time));//赠送时间
            modelBase.setItem(row,21,new QStandardItem(QString().sprintf("%0.2f",zuofajiage)));//做法加价
            modelBase.setItem(row,22,new QStandardItem(diancai_model.record(num).value(4).toString())); //所属大类编码
            modelBase.setItem(row,23,new QStandardItem(diancai_model.record(num).value(14).toString())); //所属小类编码
            modelBase.setItem(row,24,new QStandardItem("N")); //是否落单
            modelBase.setItem(row,25,new QStandardItem(""));//落单FLOWID
            modelBase.setItem(row,26,new QStandardItem("0.00"));//已退量
            row_n = row;
            //            ui->tableView->selectRow(row);


            if(cook_num == "1")//自动录入点菜数量
            {
                modfiy_number * modfiy = new modfiy_number(modelBase.item(row,3)->text(), this);
                /*modfiy->exec();*/backheader::static_dialogexec(*modfiy, "");
                if(modfiy->btn_t_f == true)
                {
                    zitaocaishuling = modfiy->modfiy_num;
                    modelBase.item(row,3)->setText(QString().sprintf("%0.2f",modfiy->modfiy_num));
                    float zongjia = modelBase.item(row,3)->text().toFloat() * modelBase.item(row,4)->text().toFloat() * modelBase.item(row,5)->text().toFloat() * 0.01;
                    modelBase.item(row,6)->setText(QString().sprintf("%0.2f",zongjia));
                }
            }

            //存在特殊做法
            if(suit != "P" && suit != "Y")
            {
                zufa_model.clear();
                zufa_model.setQuery(QString("SELECT a.ch_no,a.vch_cp_memo,a.ch_typeno,a.num_add_price FROM cey_bt_cp_memo a,cey_bt_cp_dish b WHERE(a.ch_no = b.ch_no)and( b.ch_dishno = '%1')").arg(diancai_model.record(num).value(0).toString()));
                if(zufa_model.rowCount() > 0)
                {
                    special_cook special(diancai_model.record(num).value(0).toString(),"","0.00", this);
                    /*special.exec();*/backheader::static_dialogexec(special, tr("做法选择"));
                    if(special.btn_t_f == true)
                    {
                        modelBase.item(row,7)->setText(special.zuofaneirong);
                        modelBase.item(row,21)->setText(QString().sprintf("%0.2f",special.add_price));
                        float aa;
                        aa = 0.00;
                        aa = modelBase.item(row,3)->text().toFloat() * modelBase.item(row,4)->text().toFloat() * modelBase.item(row,5)->text().toFloat() * 0.01 + modelBase.item(row,3)->text().toFloat() * special.add_price;
                        modelBase.item(row,6)->setText(QString().sprintf("%0.2f",aa));
                    }
                }
            }

        }
        else
        {
            QString suit;
            if(diancai_model.record(num).value(7).toString() == "Y")
            {
                suit = "P";
            }
            else
            {
                suit = "N";
            }
            QString zhutaobianma = diancai_model.record(num).value(0).toString();
            float shuishou = 0.00;
            shuishou = diancai_model.record(num).value(35).toFloat();
            int row = modelBase.rowCount();
            modelBase.setRowCount(row + 1);
            modelBase.setItem(row,0,new QStandardItem(QString::number(row+1))); //序号
            modelBase.setItem(row,1,new QStandardItem(diancai_model.record(num).value(0).toString())); //编码
            modelBase.setItem(row,2,new QStandardItem(diancai_model.record(num).value(1).toString())); //品名
            modelBase.setItem(row,3,new QStandardItem(QString().sprintf("%0.2f",model_danwei.record(0).value(3).toFloat()))); //数量
            modelBase.setItem(row,4,new QStandardItem(QString().sprintf("%0.2f",diancai_model.record(num).value(6).toFloat()))); //金额
            modelBase.setItem(row,5,new QStandardItem("100")); //折扣值
            float zongjia = 0.00;
            zongjia = model_danwei.record(0).value(3).toFloat() * diancai_model.record(num).value(6).toFloat();
            modelBase.setItem(row,6,new QStandardItem(QString().sprintf("%0.2f",zongjia))); //总价
            modelBase.setItem(row,7,new QStandardItem(tr(""))); //特殊做法
            modelBase.setItem(row,8,new QStandardItem("N")); //是否赠送
            modelBase.setItem(row,9,new QStandardItem(suit)); //是否套菜
            modelBase.setItem(row,10,new QStandardItem("N")); //是否特价
            modelBase.setItem(row,11,new QStandardItem("N")); //是否优惠
            modelBase.setItem(row,12,new QStandardItem(danwei)); //单位
            modelBase.setItem(row,13,new QStandardItem("")); //赠送
            modelBase.setItem(row,14,new QStandardItem("")); //定折人
            modelBase.setItem(row,15,new QStandardItem("N")); //是否时价菜
            modelBase.setItem(row,16,new QStandardItem("")); //子套菜所在主套菜编码
            modelBase.setItem(row,17,new QStandardItem(QString().sprintf("%0.2f",shuishou)));//税点
            modelBase.setItem(row,18,new QStandardItem(diancai_model.record(num).value(8).toString()));//是否折扣
            modelBase.setItem(row,19,new QStandardItem(time));//折扣时间
            modelBase.setItem(row,20,new QStandardItem(time));//赠送时间
            modelBase.setItem(row,21,new QStandardItem(QString().sprintf("%0.2f",zuofajiage)));//做法加价
            modelBase.setItem(row,22,new QStandardItem(diancai_model.record(num).value(4).toString())); //所属大类编码
            modelBase.setItem(row,23,new QStandardItem(diancai_model.record(num).value(14).toString())); //所属小类编码
            modelBase.setItem(row,24,new QStandardItem("N")); //是否落单
            modelBase.setItem(row,25,new QStandardItem(""));//落单FLOWID
            modelBase.setItem(row,26,new QStandardItem("0.00"));//已退量
            row_n = row;
            //            ui->tableView->selectRow(row);

            if(cook_num == "1")//自动录入点菜数量
            {
                modfiy_number * modfiy = new modfiy_number(modelBase.item(row,3)->text(), this);
                /*modfiy->exec();*/backheader::static_dialogexec(*modfiy, "");
                if(modfiy->btn_t_f == true)
                {
                    zitaocaishuling = modfiy->modfiy_num;
                    modelBase.item(row,3)->setText(QString().sprintf("%0.2f",modfiy->modfiy_num));
                    float zongjia = modelBase.item(row,3)->text().toFloat() * modelBase.item(row,4)->text().toFloat() * modelBase.item(row,5)->text().toFloat() * 0.01;
                    modelBase.item(row,6)->setText(QString().sprintf("%0.2f",zongjia));
                }
            }

            //存在特殊做法
            if(suit != "P" && suit != "Y")
            {
                zufa_model.clear();
                zufa_model.setQuery(QString("SELECT a.ch_no,a.vch_cp_memo,a.ch_typeno,a.num_add_price FROM cey_bt_cp_memo a,cey_bt_cp_dish b WHERE(a.ch_no = b.ch_no)and( b.ch_dishno = '%1')").arg(diancai_model.record(num).value(0).toString()));
                if(zufa_model.rowCount() > 0)
                {
                    special_cook special(diancai_model.record(num).value(0).toString(),"","0.00", this);
                    /*special.exec();*/backheader::static_dialogexec(special, tr("做法选择"));
                    if(special.btn_t_f == true)
                    {
                        modelBase.item(row,7)->setText(special.zuofaneirong);
                        modelBase.item(row,21)->setText(QString().sprintf("%0.2f",special.add_price));
                        float aa;
                        aa = 0.00;
                        aa = modelBase.item(row,3)->text().toFloat() * modelBase.item(row,4)->text().toFloat() * modelBase.item(row,5)->text().toFloat() * 0.01 + modelBase.item(row,3)->text().toFloat() * special.add_price;
                        modelBase.item(row,6)->setText(QString().sprintf("%0.2f",aa));
                    }
                }
            }
        }
    }

    //    ui->tableView->setColumnHidden(1,true);
    //    ui->tableView->setColumnHidden(3,true);
    //    ui->tableView->setColumnHidden(4,true);
    //    ui->tableView->setColumnHidden(5,true);
    //    ui->tableView->setColumnHidden(6,true);
    //    ui->tableView->setColumnHidden(7,true);
    //    ui->tableView->setColumnHidden(8,true);
    //    ui->tableView->setColumnHidden(9,true);
    //    ui->tableView->setColumnHidden(10,true);
    //    ui->tableView->setColumnHidden(11,true);
    //    ui->tableView->setColumnHidden(12,true);
    //    ui->tableView->setColumnHidden(14,true);
    //    ui->tableView->setColumnHidden(15,true);
    //    ui->tableView->setColumnHidden(16,true);
    //    ui->tableView->setColumnHidden(17,true);
    //    ui->tableView->setColumnHidden(18,true);
    //    ui->tableView->setColumnHidden(19,true);
    //    ui->tableView->setColumnHidden(20,true);
    //    ui->tableView->setColumnHidden(21,true);
    //    ui->tableView->setColumnHidden(22,true);
    //    ui->tableView->setColumnHidden(23,true);
    //    ui->tableView->setColumnHidden(24,true);
    //    ui->tableView->setColumnHidden(25,true);
    //    ui->tableView->setColumnHidden(26,true);


    QSqlQueryModel thistime;
    thistime.setQuery(QString("Select NOW()"));
    diancaitime.clear();
    diancaitime = thistime.record(0).value(0).toDateTime().toString("yyyy-MM-dd hh:mm:ss");

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
    this->to_dish_title(label_shuliang, label_jine);
    this->ui->pushButton_quitdish->setText(tr("删菜"));
}

void table_order_2::print_back_all()
{
    QString serialno=system_setting::get_max_serialno_add1(w_scr_dish_restaurant_dialog::table_danhao);
    fast_pay::kitchen_or_outprint("", w_scr_dish_restaurant_dialog::table_danhao,serialno);
    fast_pay::kitchen_or_outprint("out_", w_scr_dish_restaurant_dialog::table_danhao,serialno);
}


void table_order_2::on_pushButton_23_clicked()//落单
{
    if(toluodan())
        this->close();
}

void table_order_2::on_pushButton_7_clicked()//催菜
{
    int row = ui->tableView->tableview->currentIndex().row();// ui->tableView->currentIndex().row();
    if(row <= -1)
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("请选择一条需要催菜的菜品!"));
        
        return;
    }

    if(modelBase.item(row,24)->text() == "N")
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("指定菜品尚未落单,无需催菜!"));
        
        return;
    }

    if(modelBase.item(row,3)->text().toFloat() == 0)
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("指定菜品数量为零,无需催菜!"));
        
        return;
    }

    if(0==lds_messagebox::warning(this, tr("消息提示"),tr("确认催菜?"),tr("确认"),tr("取消")))
    {
        cui_cai = true;
        print_back_one();
    }
    cui_cai = false;
}

void table_order_2::on_pushButton_8_clicked()
{

    int k = 0;
    for(int i = 0; i < modelBase.rowCount(); i++)
    {
        if(modelBase.item(i,24)->text() == "Y" && modelBase.item(i,3)->text().toFloat() != 0)
        {
            k++;
        }
    }
    if(k == 0)
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("没有可催菜的菜品!"));
        
        return;
    }

    if(0==lds_messagebox::warning(this, tr("消息提示"),tr("确认催菜?"),tr("确认"),tr("取消")))
    {
        cui_cai = true;
        print_back_all();
    }
    cui_cai = false;
}

void table_order_2::on_pushButton_24_clicked()//预打单
{
    ui->pushButton_other->menu()->hide();
    int k = 0;
    for(int i = 0; i < modelBase.rowCount(); i++)
    {
        if(modelBase.item(i,24)->text() == "N")
        {
            k++;
        }
    }
    if(k > 0)
    {
        if(0==lds_messagebox::warning(this, tr("消息提示"),tr("点菜信息有更改,是否落单?"),tr("确认"),tr("取消")))
        {
            if(!toluodan()) return;
        }
    }

    //    init_yuda_data();
    //    print();
    table_pay_2 pay0(this);
    pay0.fast_pay_print(w_scr_dish_restaurant_dialog::table_danhao,
                        tr("预打单"),
                        fast_pay::PRINT_BILL
                        |fast_pay::PRINT_ONLY_COSUME);

    this->close();
}

void table_order_2::pay_yudadan()
{
    print();
}

bool table_order_2::toluodan()
{
    QString danhao = w_scr_dish_restaurant_dialog::table_danhao;
    QString zhuohao = frontViewDialog_virtual::cur_desk_no;
    QSqlQueryModel time_model;
    time_model.setQuery(QString("select NOW()"));
    QString time = time_model.record(0).value(0).toDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString guqingdate = time_model.record(0).value(0).toDateTime().toString("yyyy-MM-dd");
    int a = 0;
    a = w_scr_dish_restaurant_dialog::table_danhao.right(4).toInt()% 100;
    if(a < 10)
    {
        liushuihao_p = "0" + QString::number(a);
    }
    else
    {
        liushuihao_p = QString::number(a);
    }

    QSqlQueryModel m_billno;
    QString liushi = "P" + time_model.record(0).value(0).toDateTime().toString("yyyyMMdd");
    m_billno.setQuery(QString("select Max(right(ch_payno, 5)) f from cey_u_checkout_master where Left(ch_payno,7) = '%1' ").arg(liushi));
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

    //菜品消费明细表
    QSqlQuery query;
    QSqlDatabase::database().transaction();
    QStandardItemModel *model  = new QStandardItemModel();
    for(int i = 0; i < modelBase.rowCount();i++)
    {
        if(modelBase.item(i,24)->text() == "N")
        {
            int back_row = model->rowCount();
            model->setRowCount(back_row + 1);
            model->setItem(back_row,0,new QStandardItem(modelBase.item(i,0)->text()));
            model->setItem(back_row,1,new QStandardItem(modelBase.item(i,1)->text()));
            model->setItem(back_row,2,new QStandardItem(modelBase.item(i,2)->text()));
            model->setItem(back_row,3,new QStandardItem(modelBase.item(i,3)->text()));
            model->setItem(back_row,4,new QStandardItem(modelBase.item(i,4)->text()));
            model->setItem(back_row,5,new QStandardItem(modelBase.item(i,5)->text()));
            model->setItem(back_row,6,new QStandardItem(modelBase.item(i,6)->text()));
            model->setItem(back_row,7,new QStandardItem(modelBase.item(i,7)->text()));
            model->setItem(back_row,8,new QStandardItem(modelBase.item(i,8)->text()));
            model->setItem(back_row,9,new QStandardItem(modelBase.item(i,9)->text()));
            model->setItem(back_row,10,new QStandardItem(modelBase.item(i,10)->text()));
            model->setItem(back_row,11,new QStandardItem(modelBase.item(i,11)->text()));
            model->setItem(back_row,12,new QStandardItem(modelBase.item(i,12)->text()));
            model->setItem(back_row,13,new QStandardItem(modelBase.item(i,13)->text()));
            model->setItem(back_row,14,new QStandardItem(modelBase.item(i,14)->text()));
            model->setItem(back_row,15,new QStandardItem(modelBase.item(i,15)->text()));
            model->setItem(back_row,16,new QStandardItem(modelBase.item(i,16)->text()));
            model->setItem(back_row,17,new QStandardItem(modelBase.item(i,17)->text()));
            model->setItem(back_row,18,new QStandardItem(modelBase.item(i,18)->text()));
            model->setItem(back_row,19,new QStandardItem(modelBase.item(i,19)->text()));
            model->setItem(back_row,20,new QStandardItem(modelBase.item(i,20)->text()));
            model->setItem(back_row,21,new QStandardItem(modelBase.item(i,21)->text()));
            model->setItem(back_row,22,new QStandardItem(modelBase.item(i,22)->text()));
            model->setItem(back_row,23,new QStandardItem(modelBase.item(i,23)->text()));
            model->setItem(back_row,24,new QStandardItem(modelBase.item(i,24)->text()));
            model->setItem(back_row,25,new QStandardItem(modelBase.item(i,25)->text()));
            model->setItem(back_row,26,new QStandardItem(modelBase.item(i,26)->text()));
        }
    }
    QSqlQueryModel guqing;
    int int_id = 0;
    m_billno.clear();
    m_billno.setQuery(QString("Select IFNULL( max( int_id), 0)from cey_u_orderdish where ch_billno = '%1' and ch_tableno = '%2'").arg(w_scr_dish_restaurant_dialog::table_danhao).arg(frontViewDialog_virtual::cur_desk_no));
    int_id = m_billno.record(0).value(0).toInt();
    for(int i = 0; i < model->rowCount(); i++)
    {
        int_id++;
        guqing.clear();
        guqing.setQuery(QString("Select ifnull(num_num - num_sale,0) from cey_bt_dish_warn where ch_dishno ='%1' and ch_state ='Y' and DATE_FORMAT(dt_date, '%Y-%m-%d') ='%2' ").arg(model->item(i,1)->text()).arg(guqingdate));
        if(guqing.rowCount() > 0 && model->item(i,9)->text() != tr("Y"))
        {
            query.prepare(QString("update cey_bt_dish_warn set num_sale = num_sale + :as_num where ch_state ='Y' and ch_dishno = :as_dishno and DATE_FORMAT(dt_date, '%Y-%m-%d') = :as_date "));
            query.bindValue(":as_num",model->item(i,3)->text());
            query.bindValue(":as_dishno",model->item(i,1)->text());
            query.bindValue(":as_date",guqingdate);
            if(query.exec())
            {

            }
            else
            {
                QSqlDatabase::database().rollback();
                lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_bt_dish_warn菜品估清失败!"));

                return false;
            }
        }

        float add_price;
        add_price = 0.00;
        add_price = model->item(i,3)->text().toFloat() * model->item(i,21)->text().toFloat();
        if(model->item(i,8)->text() == "Y")//赠送菜品
        {
            if(model->item(i,7)->text().isEmpty())
            {
                if(query.exec(QString("insert into cey_u_orderdish(ch_billno,ch_tableno,int_id,ch_dishno,num_price,num_price_org,num_price_add,num_num,num_back,vch_back_operID,dt_back_operdate,int_discount,vch_dis_operID,dt_dis_operdate,vch_memberno,ch_consult,ch_suitflag,ch_suitno,ch_specialflag,ch_presentflag,vch_pre_operID,dt_pre_operdate,vch_operID,dt_operdate,ch_togono,ch_payno,vch_print_memo) values('%1','%2','%3','%4','%5','%6','%7','%8','%9',null,null,'%10',null,null,null,'%11','%12',null,'%13','%14','%15','%16','%17','%18',null,'%19',null)")
                              .arg(danhao).arg(zhuohao).arg(QString::number(int_id)).arg(model->item(i,1)->text()).arg(model->item(i,4)->text()).arg(model->item(i,4)->text())
                              .arg(QString().sprintf("%0.2f",add_price)).arg(model->item(i,3)->text()).arg("0.00").arg(model->item(i,5)->text()).arg("0").arg(model->item(i,9)->text())
                              .arg(model->item(i,10)->text()).arg(model->item(i,8)->text()).arg(n_func::gs_operid).arg(model->item(i,20)->text()).arg(n_func::gs_operid)
                              .arg(time).arg(liushuihao)))
                {

                }
                else
                {
                    QSqlDatabase::database().rollback();
                    qDebug() << query.lastError();  lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_u_orderdish菜品消费明细表失败!"));

                    return false;
                }
            }
            else
            {
                query.prepare(QString("insert into cey_u_orderdish(ch_billno,ch_tableno,int_id,ch_dishno,num_price,num_price_org,num_price_add,num_num,num_back,vch_back_operID,dt_back_operdate,int_discount,vch_dis_operID,dt_dis_operdate,vch_memberno,ch_consult,ch_suitflag,ch_suitno,ch_specialflag,ch_presentflag,vch_pre_operID,dt_pre_operdate,vch_operID,dt_operdate,ch_togono,ch_payno,vch_print_memo) values(:str,:str1,:str2,:str3,:str4,:str5,:str6,:str7,:str8,null,null,:str9,null,null,null,:str10,:str11,null,:str12,:str13,:str14,:str15,:str16,:str17,null,:str18,:str19)"));
                query.bindValue(":str",danhao);
                query.bindValue(":str1",zhuohao);
                query.bindValue(":str2",QString::number(int_id));
                query.bindValue(":str3",model->item(i,1)->text());
                query.bindValue(":str4",model->item(i,4)->text());
                query.bindValue(":str5",model->item(i,4)->text());
                query.bindValue(":str6",QString().sprintf("%0.2f",add_price));
                query.bindValue(":str7",model->item(i,3)->text());
                query.bindValue(":str8","0.00");
                query.bindValue(":str9",model->item(i,5)->text());
                query.bindValue(":str10","0");
                query.bindValue(":str11",model->item(i,9)->text());
                query.bindValue(":str12",model->item(i,10)->text());
                query.bindValue(":str13",model->item(i,8)->text());
                query.bindValue(":str14",n_func::gs_operid);
                query.bindValue(":str15",model->item(i,20)->text());
                query.bindValue(":str16",n_func::gs_operid);
                query.bindValue(":str17",time);
                query.bindValue(":str18",liushuihao);
                query.bindValue(":str19",model->item(i,7)->text());

                if(query.exec())
                {

                }
                else
                {
                    QSqlDatabase::database().rollback();
                    qDebug() << query.lastError();  lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_u_orderdish失败菜品消费明细表!"));

                    return false;
                }
            }
        }
        else if(model->item(i,5)->text().toFloat() != 100)//折扣菜品
        {
            if(model->item(i,7)->text().isEmpty())
            {
                if(query.exec(QString("insert into cey_u_orderdish(ch_billno,ch_tableno,int_id,ch_dishno,num_price,num_price_org,num_price_add,num_num,num_back,vch_back_operID,dt_back_operdate,int_discount,vch_dis_operID,dt_dis_operdate,vch_memberno,ch_consult,ch_suitflag,ch_suitno,ch_specialflag,ch_presentflag,vch_pre_operID,dt_pre_operdate,vch_operID,dt_operdate,ch_togono,ch_payno,vch_print_memo) values('%1','%2','%3','%4','%5','%6','%7','%8','%9','%10','%11','%12','%13','%14',null,'%15','%16',null,'%17','%18',null,null,'%19','%20',null,'%21',null)")
                              .arg(danhao).arg(zhuohao).arg(QString::number(int_id)).arg(model->item(i,1)->text()).arg(model->item(i,4)->text()).arg(model->item(i,4)->text())
                              .arg(QString().sprintf("%0.2f",add_price)).arg(model->item(i,3)->text()).arg("0.00").arg(n_func::gs_operid).arg(model->item(i,19)->text()).arg(QString::number(int(model->item(i,5)->text().toFloat()))).arg(n_func::gs_operid)
                              .arg(model->item(i,19)->text()).arg("1").arg("N").arg("N").arg("N").arg(n_func::gs_operid).arg(time).arg(liushuihao)))
                {

                }
                else
                {
                    QSqlDatabase::database().rollback();
                    qDebug() << query.lastError();  lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_u_orderdish失败菜品消费明细表!"));

                    return false;
                }
            }
            else
            {
                query.prepare(QString("insert into cey_u_orderdish(ch_billno,ch_tableno,int_id,ch_dishno,num_price,num_price_org,num_price_add,num_num,num_back,vch_back_operID,dt_back_operdate,int_discount,vch_dis_operID,dt_dis_operdate,vch_memberno,ch_consult,ch_suitflag,ch_suitno,ch_specialflag,ch_presentflag,vch_pre_operID,dt_pre_operdate,vch_operID,dt_operdate,ch_togono,ch_payno,vch_print_memo) values(:str,:str1,:str2,:str3,:str4,:str5,:str6,:str7,:str8,:zhekouren,:zhekoutime,:str9,:str10,:str11,null,:str12,:str13,null,:str14,:str15,null,null,:str16,:str17,null,:str18,:str19)"));
                query.bindValue(":str",danhao);
                query.bindValue(":str1",zhuohao);
                query.bindValue(":str2",QString::number(int_id));
                query.bindValue(":str3",model->item(i,1)->text());
                query.bindValue(":str4",model->item(i,4)->text());
                query.bindValue(":str5",model->item(i,4)->text());
                query.bindValue(":str6",QString().sprintf("%0.2f",add_price));
                query.bindValue(":str7",model->item(i,3)->text());
                query.bindValue(":str8","0.00");
                query.bindValue(":zhekouren",n_func::gs_operid);
                query.bindValue(":zhekoutime",model->item(i,19)->text());
                query.bindValue(":str9",QString::number(int(model->item(i,5)->text().toFloat())));
                query.bindValue(":str10",n_func::gs_operid);
                query.bindValue(":str11",model->item(i,19)->text());
                query.bindValue(":str12","1");
                query.bindValue(":str13","N");
                query.bindValue(":str14","N");
                query.bindValue(":str15","N");
                query.bindValue(":str16",n_func::gs_operid);
                query.bindValue(":str17",time);
                query.bindValue(":str18",liushuihao);
                query.bindValue(":str19",model->item(i,7)->text());

                if(query.exec())
                {

                }
                else
                {
                    QSqlDatabase::database().rollback();
                    qDebug() << query.lastError();  lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_u_orderdish菜品消费明细表折扣菜品失败!"));

                    return false;
                }
            }
        }
        else//普通菜品
        {
            if(model->item(i,7)->text().isEmpty())//无做法
            {
                if(query.exec(QString("insert into cey_u_orderdish(ch_billno,ch_tableno,int_id,ch_dishno,num_price,num_price_org,num_price_add,num_num,num_back,vch_back_operID,dt_back_operdate,int_discount,vch_dis_operID,dt_dis_operdate,vch_memberno,ch_consult,ch_suitflag,ch_suitno,ch_specialflag,ch_presentflag,vch_pre_operID,dt_pre_operdate,vch_operID,dt_operdate,ch_togono,ch_payno,vch_print_memo) values('%1','%2','%3','%4','%5','%6','%7','%8','%9',null,null,'%10',null,null,null,'%11','%12',null,'%13','%14',null,null,'%15','%16',null,'%17',null)")
                              .arg(danhao).arg(zhuohao).arg(QString::number(int_id)).arg(model->item(i,1)->text()).arg(model->item(i,4)->text()).arg(model->item(i,4)->text())
                              .arg(QString().sprintf("%0.2f",add_price)).arg(model->item(i,3)->text()).arg("0.00").arg(model->item(i,5)->text()).arg("0").arg(model->item(i,9)->text())
                              .arg(model->item(i,10)->text()).arg(model->item(i,8)->text()).arg(n_func::gs_operid)
                              .arg(time).arg(liushuihao)))
                {

                }
                else
                {
                    QSqlDatabase::database().rollback();
                    qDebug() << query.lastError();  lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_u_orderdish菜品消费明细表失败!"));

                    return false;
                }
            }
            else
            {
                query.prepare(QString("insert into cey_u_orderdish(ch_billno,ch_tableno,int_id,ch_dishno,num_price,num_price_org,num_price_add,num_num,num_back,vch_back_operID,dt_back_operdate,int_discount,vch_dis_operID,dt_dis_operdate,vch_memberno,ch_consult,ch_suitflag,ch_suitno,ch_specialflag,ch_presentflag,vch_pre_operID,dt_pre_operdate,vch_operID,dt_operdate,ch_togono,ch_payno,vch_print_memo) values(:str,:str1,:str2,:str3,:str4,:str5,:str6,:str7,:str8,null,null,:str9,null,null,null,:str10,:str11,null,:str12,:str13,null,null,:str14,:str15,null,:str16,:str17)"));
                query.bindValue(":str",danhao);
                query.bindValue(":str1",zhuohao);
                query.bindValue(":str2",QString::number(int_id));
                query.bindValue(":str3",model->item(i,1)->text());
                query.bindValue(":str4",model->item(i,4)->text());
                query.bindValue(":str5",model->item(i,4)->text());
                query.bindValue(":str6",QString().sprintf("%0.2f",add_price));
                query.bindValue(":str7",model->item(i,3)->text());
                query.bindValue(":str8","0.00");
                query.bindValue(":str9",model->item(i,5)->text());
                query.bindValue(":str10","0");
                query.bindValue(":str11",model->item(i,9)->text());
                query.bindValue(":str12",model->item(i,10)->text());
                query.bindValue(":str13",model->item(i,8)->text());
                query.bindValue(":str14",n_func::gs_operid);
                query.bindValue(":str15",time);
                query.bindValue(":str16",liushuihao);
                query.bindValue(":str17",model->item(i,7)->text());

                if(query.exec())
                {

                }
                else
                {
                    QSqlDatabase::database().rollback();
                    qDebug() << query.lastError();  lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_u_orderdish菜品消费明细表普通菜品失败!"));

                    return false;
                }
            }
        }
    }

    float label_jine = 0.00;
    for(int i = 0; i < modelBase.rowCount(); i++)
    {
        if(modelBase.item(i,8)->text() == "N" && modelBase.item(i,9)->text() != "Y" )
        {
            label_jine += modelBase.item(i,6)->text().toFloat();
        }
    }
    query.prepare(QString("update cey_u_table set num_cost =:num where ch_billno =:danhao"));
    query.bindValue(":num",QString().sprintf("%0.2f",label_jine));
    query.bindValue(":danhao",w_scr_dish_restaurant_dialog::table_danhao);
    if(query.exec())
    {
    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_u_table失败!"));

        return false;
    }

    query.prepare(QString("update cey_bt_table set ch_lockflag ='N' where ch_tableno =:table "));
    query.bindValue(":table",frontViewDialog_virtual::cur_desk_no);
    if(query.exec())
    {
    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_bt_table失败!"));

        return false;
    }
    QSqlDatabase::database().commit();
    //打印
    luo_dan = true;
    print_back_all();
    luo_dan = false;
    lds_messagebox::warning(this, tr("消息提示"),tr("落单成功!"));
    return true;
}

void table_order_2::init_yuda_data()
{
    QSqlQueryModel model_l;
    model_l.setQuery("Select NOW()");
    QString time = model_l.record(0).value(0).toDateTime().toString("yyyy-MM-dd hh:mm:ss");

    //QStandardItemModel *& model = modelBase;
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

    _xiaofei = QString().sprintf("%0.2f",xiaofei);
    _zhekou = QString().sprintf("%0.2f",zhekou);
    _zengsong = QString().sprintf("%0.2f",zengsong);
    _yingshou = QString().sprintf("%0.2f",yingshou);
    _moling = QString().sprintf("%0.2f",moling);
    _shuishou = QString().sprintf("%0.2f",shuishou);
    //    ui->label_3->setText(QString().sprintf("%0.2f",yingshou));
    this->to_dish_title(-1, yingshou);

}
