#include "w_scr_dish_restaurant_dialog.h"
#include "ui_w_scr_dish_restaurant_dialog.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include "backheader.h"
#include "founding_cook.h"
#include "table_order_2.h"
#include "lds_messagebox.h"
#include "n_func.h"
#include "manager_view.h"
#include "shift_work.h"
#include "lock_screen.h"
#include "power_oper.h"
#include "lds_menu.h"
#include "table_lock.h"
#include "function_selection.h"

QString w_scr_dish_restaurant_dialog::dalei;
QString w_scr_dish_restaurant_dialog::table_danhao;
//QString w_scr_dish_restaurant_dialog::table_lock;
//QString frontViewDialog_virtual::cur_desk_no;
w_scr_dish_restaurant_dialog::w_scr_dish_restaurant_dialog(public_loginDialog *parent) :
    QDialog(0),
    ui(new Ui::w_scr_dish_restaurant_dialog),
    _parent(parent)
{
    ui->setupUi(this);
    //1
    if(backheader::mainwindowsize.width()==800){
        ui->frame_main->horizontal_count=5;
        ui->frame_main->vertical_count=4;
    } else {
        ui->frame_main->horizontal_count=8;
        ui->frame_main->vertical_count=6;
    }
    ui->frame_main->set_btn_property("pushbutton_jiulou");
    ui->frame_main->tool_size=QSize(105, 105);
    //2
    if(backheader::mainwindowsize.width()==800){
        ui->frame_list->horizontal_count=1;
        ui->frame_list->vertical_count=5;
    } else {
        ui->frame_list->horizontal_count=1;
        ui->frame_list->vertical_count=8;
    }
    ui->frame_list->set_btn_property("pushbutton_jiuloulist");
    ui->frame_list->tool_size=QSize(100, 60);
    //2.1
    {
        manager_view *view=new manager_view;
        lds_menu *menu=new lds_menu(this);
        QWidgetAction *widgetaction=new QWidgetAction(ui->pushButton_set);
        widgetaction->setDefaultWidget(view);
        menu->addAction(widgetaction);
        ui->pushButton_set->setMenu(menu);
        connect(view->ui->pushButton_9,SIGNAL(clicked()),this,SLOT(toswitchback()));

    }
    //3
    connect(ui->pushButton_canceltable,SIGNAL(clicked()),this,SLOT(tocanceldeak()));
    connect(ui->pushButton_lockstate,SIGNAL(clicked()),this,SLOT(tolockstate()));
    connect(ui->pushButton_open,SIGNAL(clicked()),this,SLOT(toopen()));
    connect(ui->pushButton_order,SIGNAL(clicked()),this,SLOT(toorder()));
    connect(ui->pushButton_other,SIGNAL(clicked()),this,SLOT(toother()));
    connect(ui->pushButton_quit,SIGNAL(clicked()),this,SLOT(toquit()));
    connect(ui->pushButton_refresh,SIGNAL(clicked()),this,SLOT(torefresh()));
    connect(ui->pushButton_set,SIGNAL(clicked()),this,SLOT(toset()));
    connect(ui->pushButton_shift,SIGNAL(clicked()),this,SLOT(toshift()));
    connect(ui->pushButton_switch,SIGNAL(clicked()),this,SLOT(toswitch()));

//    connect(ui->frame_main,SIGNAL(signal_double_text(QString)),this,SLOT(totablemessage(QString)));
    connect(ui->frame_main,SIGNAL(signal_text(QString)),this,SLOT(totablesave(QString)));
    connect(ui->frame_list,SIGNAL(signal_text(QString)),this,SLOT(toupdatetable(QString)));
    connect(ui->pushButton_refresh,SIGNAL(clicked()),this,SLOT(torefresh()));
}

w_scr_dish_restaurant_dialog::~w_scr_dish_restaurant_dialog()
{
    delete ui;
}

void w_scr_dish_restaurant_dialog::tocanceldeak()
{
    table_order_2::tocanceldeak();
    torefresh();
}

void w_scr_dish_restaurant_dialog::tolockstate()
{
    table_lock dialog(this);
    backheader::static_dialogexec(dialog, tr("锁台管理"));
}

void w_scr_dish_restaurant_dialog::toopen()
{
    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    QString instructionedit = conf.value("system_setting/qianxiangzhiling").toString();
    blockIns bins;
    bins.append(pieceIns(instructionedit.toLocal8Bit(),""));
    backheader::asyn_dataprint(bins);
}

void w_scr_dish_restaurant_dialog::toorder()
{

}

void w_scr_dish_restaurant_dialog::toother()
{

}

void w_scr_dish_restaurant_dialog::toquit()
{
    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    if(conf.value("system_setting/chongxidenglu").toString() == "1"){
        //返回登录界面
        _parent->hideall_and_back_login("", "");
    } else {
        /*退出*/
        this->reject();
    }
}

void w_scr_dish_restaurant_dialog::torefresh()
{
    //1
    ui->frame_main->clearall_item();
    QSqlQuery query;
    QSqlQuery query2;
    QString tabletext;
    QString tablechbillno;
    frontViewDialog_toolbtn::THISSTATE state;
    query.exec(QString("select ch_tableno,vch_tablename,ch_billno from cey_bt_table where ch_typeno like '%1%' ").arg(_curtypeno));
    while(query.next()){
        state =frontViewDialog_toolbtn::KONGXIAN;
        tabletext=query.record().value("vch_tablename").toString();
        if(!(tablechbillno=query.record().value("ch_billno").toString()).isEmpty()){
            query2.exec(QString("select int_person, num_cost from cey_u_table where ch_billno='%1' ").arg(tablechbillno));
            qDebug() << query2.lastQuery();
            query2.next();
            state=frontViewDialog_toolbtn::XUANZHONG;
            tabletext=query.record().value("vch_tablename").toString()
                    +"\n"+tr("人数:")+query2.record().value("int_person").toString()
                    +"\n"+tr("消费:")+query2.record().value("num_cost").toString()
                    ;
        }
        ui->frame_main->add_item(query.record().value("ch_tableno").toString(),
                                 tabletext,
                                 false,
                                 QPixmap(),
                                 state
                                 );
    }
    ui->frame_main->generat_from_items();
    //2
    ui->frame_list->clearall_item();
    query.exec(QString("SELECT concat(vch_typename,'[',ch_typeno,']') as nametype,ch_typeno  FROM hddpos.cey_bt_table_type union select '"+tr("所有桌台")+"' as vch_typename,'%' as ch_typeno ORDER BY ch_typeno ASC;"));
    while(query.next()){
        ui->frame_list->add_item(query.record().value("ch_typeno").toString(),
                                 query.record().value("nametype").toString(),
                                 false);
    }
    ui->frame_list->generat_from_items();
}

void w_scr_dish_restaurant_dialog::toset()
{
}

void w_scr_dish_restaurant_dialog::toshift()
{
    table_order_2::toswitchdesk();
    torefresh();
}

void w_scr_dish_restaurant_dialog::toswitch()//交班
{
    function_selection::static_toswitch();
}

void w_scr_dish_restaurant_dialog::totablemessage(const QString &tableno)
{
    QSqlQueryModel model;
    QSqlQuery query;
    QSqlDatabase::database().transaction();
    QString canzhuohao;

    model.setQuery(QString("select * from cey_bt_table  where ch_tableno = '%1' ").arg(tableno));//已开台的
    if(model.record(0).value(4).toString() == "Y") {
        lds_messagebox::warning(this, tr("消息提示"),tr("该餐桌已被锁定,请确定!"));
        return;
    }
    canzhuohao = model.record(0).value(0).toString();

    if(model.record(0).value(3).toString().trimmed().isEmpty()){//未开台
        QSqlQueryModel m_billno;
        m_billno.setQuery("Select NOW()");
        QString danhao;
        QDateTime ldt_datetime;
        ldt_datetime.setTime(m_billno.record(0).value(0).toTime());
        ldt_datetime.setDate(m_billno.record(0).value(0).toDate());
        QString currentDate = tr("B") + ldt_datetime.toString("yyMMdd");
        m_billno.clear();
        m_billno.setQuery(QString("Select Max( ch_billno)from cey_u_table where left( ch_billno , 7)= '%1' ").arg(currentDate));
        danhao = m_billno.record(0).value(0).toString().trimmed();
        if(danhao.trimmed().isEmpty()){
            danhao = currentDate + QString().sprintf("%05d",1);
        } else {
            QString str = danhao.right(danhao.length() - 7);
            int aa = str.toInt() + 1;
            danhao = currentDate + QString().sprintf("%05d",aa);
        }
        table_danhao = danhao;
        QString zhuohao = model.record(0).value(0).toString();
        QString renshu = model.record(0).value(5).toString();
        QString kaitairen = n_func::gs_operid;
        QString time = ldt_datetime.toString("yyyy-MM-dd hh:mm:ss");
        founding_cook cook(danhao,zhuohao,renshu,kaitairen,time,"1", this);
        backheader::static_dialogexec(cook, tr("开台操作"));
        if(cook.dianccai == false && cook.wangluo == false && cook.btn_t_f == true){
            query.prepare("update cey_bt_table set ch_billno=:danhao where ch_tableno= :zhuohao");
            query.bindValue(":danhao",cook._danhao);
            query.bindValue(":zhuohao",cook._zhuohao);
            if(query.exec())
            { }else{
                QSqlDatabase::database().rollback();
                lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_bt_table失败"));
                return;
            }
            if(!cook.songcanren.isEmpty() && !cook.beizhu.isEmpty()){
                query.prepare(QString("insert into cey_u_table( ch_billno , ch_tableno , int_person , ch_state ,  vch_operID , dt_operdate , ch_order_state , num_cost,vch_waiter,vch_memo) values(:danhao,:zhuohao,:renshu,:state,:oper,:time,:order_state,:num,:wait,:beizhu)"));
                query.bindValue(":danhao",cook._danhao);
                query.bindValue(":zhuohao",cook._zhuohao);
                query.bindValue(":renshu",cook._renshu);
                query.bindValue(":state","1");
                query.bindValue(":oper",cook._kaitairen);
                query.bindValue(":time",cook._time);
                query.bindValue(":order_state","0");
                query.bindValue(":num","0.00");
                query.bindValue(":wait",cook.songcanren);
                query.bindValue(":beizhu",cook.beizhu);
                if(query.exec())
                { } else{
                    QSqlDatabase::database().rollback();
                    lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_u_table失败！"));
                    return;
                }
            } else if(!cook.songcanren.isEmpty() && cook.beizhu.isEmpty()) {
                query.prepare(QString("insert into cey_u_table( ch_billno , ch_tableno , int_person , ch_state ,  vch_operID , dt_operdate , ch_order_state , num_cost,vch_waiter,vch_memo) values(:danhao,:zhuohao,:renshu,:state,:oper,:time,:order_state,:num,:wait,null)"));
                query.bindValue(":danhao",cook._danhao);
                query.bindValue(":zhuohao",cook._zhuohao);
                query.bindValue(":renshu",cook._renshu);
                query.bindValue(":state","1");
                query.bindValue(":oper",cook._kaitairen);
                query.bindValue(":time",cook._time);
                query.bindValue(":order_state","0");
                query.bindValue(":num","0.00");
                query.bindValue(":wait",cook.songcanren);
                if(query.exec())
                {} else {
                    QSqlDatabase::database().rollback();
                    lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_u_table失败！"));
                    return;
                }
            } else if(cook.songcanren.isEmpty() && !cook.beizhu.isEmpty())  {
                query.prepare(QString("insert into cey_u_table( ch_billno , ch_tableno , int_person , ch_state ,  vch_operID , dt_operdate , ch_order_state , num_cost,vch_waiter,vch_memo) values(:danhao,:zhuohao,:renshu,:state,:oper,:time,:order_state,:num,null,:beizhu)"));
                query.bindValue(":danhao",cook._danhao);
                query.bindValue(":zhuohao",cook._zhuohao);
                query.bindValue(":renshu",cook._renshu);
                query.bindValue(":state","1");
                query.bindValue(":oper",cook._kaitairen);
                query.bindValue(":time",cook._time);
                query.bindValue(":order_state","0");
                query.bindValue(":num","0.00");
                query.bindValue(":beizhu",cook.beizhu);
                if(query.exec())
                { } else  {
                    QSqlDatabase::database().rollback();
                    lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_u_table失败！"));
                    return;
                }
            } else {
                query.prepare(QString("insert into cey_u_table( ch_billno , ch_tableno , int_person , ch_state ,  vch_operID , dt_operdate , ch_order_state , num_cost,vch_waiter,vch_memo) values(:danhao,:zhuohao,:renshu,:state,:oper,:time,:order_state,:num,null,null)"));
                query.bindValue(":danhao",cook._danhao);
                query.bindValue(":zhuohao",cook._zhuohao);
                query.bindValue(":renshu",cook._renshu);
                query.bindValue(":state","1");
                query.bindValue(":oper",cook._kaitairen);
                query.bindValue(":time",cook._time);
                query.bindValue(":order_state","0");
                query.bindValue(":num",tr("0.00"));
                if(query.exec())
                { } else {
                    QSqlDatabase::database().rollback();
                    lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_u_table失败！"));
                    return;
                }
            }
            query.prepare(QString("insert into cey_u_master( ch_billno , ch_state , vch_operID , dt_operdate )values(:danhao,:state,:oper,:time)"));
            query.bindValue(":danhao",cook._danhao);
            query.bindValue(":state","1");
            query.bindValue(":oper",cook._kaitairen);
            query.bindValue(":time",cook._time);
            if(query.exec())
            {  }  else {
                QSqlDatabase::database().rollback();
                lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_u_master失败！"));
                return;
            }
        }  else if(cook.dianccai == true && cook.wangluo == false && cook.btn_t_f == false)//点击点菜
        {
            query.prepare("update cey_bt_table set ch_billno=:danhao where ch_tableno= :zhuohao");
            query.bindValue(":danhao",cook._danhao);
            query.bindValue(":zhuohao",cook._zhuohao);
            if(query.exec())
            {  }   else  {
                QSqlDatabase::database().rollback();
                lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_bt_table失败"));
                return;
            }

            if(!cook.songcanren.isEmpty() && !cook.beizhu.isEmpty())
            {
                query.prepare(QString("insert into cey_u_table( ch_billno , ch_tableno , int_person , ch_state ,  vch_operID , dt_operdate , ch_order_state , num_cost,vch_waiter,vch_memo) values(:danhao,:zhuohao,:renshu,:state,:oper,:time,:order_state,:num,:wait,:beizhu)"));
                query.bindValue(":danhao",cook._danhao);
                query.bindValue(":zhuohao",cook._zhuohao);
                query.bindValue(":renshu",cook._renshu);
                query.bindValue(":state","1");
                query.bindValue(":oper",cook._kaitairen);
                query.bindValue(":time",cook._time);
                query.bindValue(":order_state","0");
                query.bindValue(":num","0.00");
                query.bindValue(":wait",cook.songcanren);
                query.bindValue(":beizhu",cook.beizhu);
                if(query.exec()) {
                } else{
                    QSqlDatabase::database().rollback();
                    lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_u_table失败！"));
                    return;
                }
            }
            else if(!cook.songcanren.isEmpty() && cook.beizhu.isEmpty()){
                query.prepare(QString("insert into cey_u_table( ch_billno , ch_tableno , int_person , ch_state ,  vch_operID , dt_operdate , ch_order_state , num_cost,vch_waiter,vch_memo) values(:danhao,:zhuohao,:renshu,:state,:oper,:time,:order_state,:num,:wait,null)"));
                query.bindValue(":danhao",cook._danhao);
                query.bindValue(":zhuohao",cook._zhuohao);
                query.bindValue(":renshu",cook._renshu);
                query.bindValue(":state","1");
                query.bindValue(":oper",cook._kaitairen);
                query.bindValue(":time",cook._time);
                query.bindValue(":order_state","0");
                query.bindValue(":num","0.00");
                query.bindValue(":wait",cook.songcanren);
                if(query.exec())  {
                }  else {
                    QSqlDatabase::database().rollback();
                    lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_u_table失败！"));
                    return;
                }
            }else if(cook.songcanren.isEmpty() && !cook.beizhu.isEmpty()){
                query.prepare(QString("insert into cey_u_table( ch_billno , ch_tableno , int_person , ch_state ,  vch_operID , dt_operdate , ch_order_state , num_cost,vch_waiter,vch_memo) values(:danhao,:zhuohao,:renshu,:state,:oper,:time,:order_state,:num,null,:beizhu)"));
                query.bindValue(":danhao",cook._danhao);
                query.bindValue(":zhuohao",cook._zhuohao);
                query.bindValue(":renshu",cook._renshu);
                query.bindValue(":state","1");
                query.bindValue(":oper",cook._kaitairen);
                query.bindValue(":time",cook._time);
                query.bindValue(":order_state","0");
                query.bindValue(":num","0.00");
                query.bindValue(":beizhu",cook.beizhu);
                if(query.exec())
                {}else{
                    QSqlDatabase::database().rollback();
                    lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_u_table失败！"));
                    return;
                }
            }  else  {
                query.prepare(QString("insert into cey_u_table( ch_billno , ch_tableno , int_person , ch_state ,  vch_operID , dt_operdate , ch_order_state , num_cost,vch_waiter,vch_memo) values(:danhao,:zhuohao,:renshu,:state,:oper,:time,:order_state,:num,null,null)"));
                query.bindValue(":danhao",cook._danhao);
                query.bindValue(":zhuohao",cook._zhuohao);
                query.bindValue(":renshu",cook._renshu);
                query.bindValue(":state","1");
                query.bindValue(":oper",cook._kaitairen);
                query.bindValue(":time",cook._time);
                query.bindValue(":order_state","0");
                query.bindValue(":num","0.00");
                if(query.exec())  {
                } else {
                    QSqlDatabase::database().rollback();
                    lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_u_table失败！"));
                    return;
                }
            }

            query.prepare(QString("insert into cey_u_master( ch_billno , ch_state , vch_operID , dt_operdate )values(:danhao,:state,:oper,:time)"));
            query.bindValue(":danhao",cook._danhao);
            query.bindValue(":state","1");
            query.bindValue(":oper",cook._kaitairen);
            query.bindValue(":time",cook._time);
            if(query.exec()) {
            } else  {
                QSqlDatabase::database().rollback();
                lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_u_master失败！"));
                return;
            }

            query.prepare(QString("update cey_bt_table set  ch_lockflag ='Y'  where ch_tableno = :zhuohao"));
            query.bindValue(":zhuohao",cook._zhuohao);
            if(query.exec())  {
            }  else{
                QSqlDatabase::database().rollback();
                lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_bt_tabler失败！"));
                return;
            }
            QSqlDatabase::database().commit();
            //弹出点菜页面
            table_order_2 order(this);
            connect(&order,SIGNAL(pay_over()),this,SLOT(pay_over()));
            backheader::static_dialogexec(order, tr("中餐点菜"), backheader::fullwindowsize);
        }
        else if(cook.dianccai == true && cook.wangluo == false && cook.btn_t_f == false)//点击网络订单导入
        {

        }
        else//点击取消
        {
            return;
        }
    }
    else//已开台
    {
        table_danhao = model.record(0).value(3).toString();
        frontViewDialog_virtual::cur_desk_no = model.record(0).value(0).toString();
        query.prepare(QString("update cey_bt_table set  ch_lockflag ='Y'  where ch_tableno = :zhuohao"));
        query.bindValue(":zhuohao",canzhuohao);
        if(query.exec())
        { } else  {
            QSqlDatabase::database().rollback();
            lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_bt_tabler失败！"));
            return;
        }
        QSqlDatabase::database().commit();
        //弹出点菜页面
        table_order_2 order(this);
        connect(&order,SIGNAL(pay_over()),this,SLOT(pay_over()));
        backheader::static_dialogexec(order, tr("中餐点菜"), backheader::fullwindowsize);qDebug() << "中餐点菜";
    }
    torefresh();
}

void w_scr_dish_restaurant_dialog::totablesave(const QString &tableno)
{
    if(table_order_2::cur_desk_no!=tableno){
        table_order_2::cur_desk_no=tableno;
    } else {
        totablemessage(table_order_2::cur_desk_no);
    }
}

void w_scr_dish_restaurant_dialog::toupdatetable(const QString &tabletype)
{
    _curtypeno=tabletype;
    torefresh();
}

void w_scr_dish_restaurant_dialog::toswitchback()
{
    ui->pushButton_set->menu()->close();
    _parent->goto_back();
}
