#include "w_scr_dish_restaurant_dialog.h"
#include "button_key.h"
#include <QDebug>
#include "founding_cook.h"
#include "lds_messagebox.h"
#include "n_func.h"
#include <QSqlQuery>
#include "business_documents_query.h"
#include "power_oper.h"
#include "shift_work.h"
#include "lock_screen.h"
#include "manager_view.h"
#include <QSettings>
#include "public_mainprinter.h"
#include  "table_order_2.h"
#include <QDesktopWidget>
#include <QApplication>
#include <QScrollBar>
#include "backheader.h"
#include <QSqlError>
#include "lds_menu.h"

QSqlQueryModel *w_scr_dish_restaurant_dialog::model_dalei;
QSqlQueryModel *w_scr_dish_restaurant_dialog::model_cook;
QString w_scr_dish_restaurant_dialog::dalei;
QString w_scr_dish_restaurant_dialog::table_danhao;
QString w_scr_dish_restaurant_dialog::table_lock;
QString frontViewDialog_virtual::cur_desk_no;
w_scr_dish_restaurant_dialog::restaurant_view_2(public_loginDialog *parent) :
    fontviewDialog_jiulou_Dialog(0)
{
    sys_model.clear();
    QSettings *configIniRead = new QSettings(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    sys_model = configIniRead->value("system_setting/yingyemoshi").toString();

    init_widget();
    init_cook();
    //    connect(this,SIGNAL(signal_desk(QString)),this,SLOT(init_cook(QString)));
    connect(this,SIGNAL(signal_sub_desk(QString)), this,SLOT(table_message(QString)));

    connect(this,SIGNAL(signal_jinliguanli()), this,SLOT(on_pushButton_9_clicked()));
    //    connect(this,SIGNAL(signal_shezhi()), this,SLOT(on_pushButton_9_clicked()));
    connect(this,SIGNAL(signal_yuding()), this,SLOT(on_pushButton_6_clicked()));
    connect(this,SIGNAL(signal_shuaxin()), this,SLOT(on_pushButton_4_clicked()));
    connect(this,SIGNAL(signal_yingyeshuju()), this,SLOT(on_pushButton_8_clicked()));

    connect(this,SIGNAL(signal_jiaoban()), this,SLOT(on_pushButton_7_clicked()));
    //    connect(this,SIGNAL(signal_shoudongshuru()), this,SLOT(on_pushButton_9_clicked()));
    connect(this,SIGNAL(signal_kaiqianxiang()), this,SLOT(on_pushButton_5_clicked()));
    connect(this, SIGNAL(signal_tuichu()),this,SLOT(on_pushButton_10_clicked()));

    connect(&desktimer,SIGNAL(timeout()),this,SLOT(update_deskinfo()));
    desktimer.start(1000);
}

w_scr_dish_restaurant_dialog::~restaurant_view_2()
{

}

void w_scr_dish_restaurant_dialog::init_widget()
{
}

void w_scr_dish_restaurant_dialog::init_cook(const QString &table_filter)
{
    _init_cook_filter=table_filter;
    //    static QString table_filter=_table_filter;
    //    if(table_filter.isEmpty()){

    //    } else {
    //        table_filter=_table_filter;
    //    }
    QList<frontViewDialog_autoexpand::content_item_icon> item_icon_list;
    table_name.clear();
    //    ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //    ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QSqlQueryModel model1 ;
    QSqlQueryModel table_on ;
    model1.setQuery(QString("select * from cey_bt_table where ch_typeno like '%1%'").arg(table_filter));
    table_on.setQuery(QString("select * from cey_bt_table where ch_billno <> ''"));//已开台的
    int int_kongtai = model1.rowCount() - table_on.rowCount();
    int int_table_on =table_on.rowCount() ;
    QString st_oper;
    QSqlQueryModel model_t;
    if(n_func::gs_operid == "0000")
    {
        st_oper=("system");
    }
    else
    {
        QSqlQueryModel model_t;
        model_t.setQuery(QString("select vch_operator from cey_sys_operator where vch_operID = '%1' ").arg(n_func::gs_operid));
        st_oper=(model_t.record(0).value(0).toString());
    }
    //更新桌台总信息
    this->update_desk_info(int_kongtai,int_table_on, st_oper, n_func::f_get_sysdatetime().toString("yyyy-MM-dd hh:mm:ss"));
}

void w_scr_dish_restaurant_dialog::updtae_page()
{
}

void w_scr_dish_restaurant_dialog::update_label()
{
    //    QSqlQueryModel *model1 = new QSqlQueryModel;
    //    QSqlQueryModel *table_on = new QSqlQueryModel;
    //    model1->setQuery(QString("select * from cey_bt_table"));
    //    table_on->setQuery(QString("select * from cey_bt_table where ch_billno <> ''"));//已开台的

    //    int kongtai = model1->rowCount() - table_on->rowCount();
    //    ui->label_3->setText(QString::number(kongtai));
    //    ui->label_9->setText(QString::number(table_on->rowCount()));
}

void w_scr_dish_restaurant_dialog::update_deskinfo()
{
    QSqlQueryModel model1 ;
    QSqlQueryModel table_on ;
    model1.setQuery(QString("select * from cey_bt_table where ch_typeno like '%1%'").arg(""));
    table_on.setQuery(QString("select * from cey_bt_table where ch_billno <> ''"));//已开台的
    int int_kongtai = model1.rowCount() - table_on.rowCount();
    int int_table_on =table_on.rowCount() ;
    QString st_time;
    QString st_oper;
    QSqlQueryModel model_t;
    model_t.setQuery("Select NOW()");
    st_time=(model_t.record(0).value(0).toDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    if(n_func::gs_operid == "0000")
    {
        st_oper=("system");
    }
    else
    {
        model_t.clear();
        model_t.setQuery(QString("select vch_operator from cey_sys_operator where vch_operID = '%1' ").arg(n_func::gs_operid));
        st_oper=(model_t.record(0).value(0).toString());
    }
    //更新桌台总信息
    this->update_desk_info(int_kongtai,int_table_on, st_oper, st_time);
}

void w_scr_dish_restaurant_dialog::torefresh()
{

}

void w_scr_dish_restaurant_dialog::on_pushButton_10_clicked()
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

void w_scr_dish_restaurant_dialog::on_pushButton_clicked()//...
{/*
    button_key *key = new button_key(this);
    connect(key,SIGNAL(btn1()),this,SLOT(btn1()));
    connect(key,SIGNAL(btn2()),this,SLOT(btn2()));
    connect(key,SIGNAL(btn3()),this,SLOT(btn3()));
    connect(key,SIGNAL(btn4()),this,SLOT(btn4()));
    connect(key,SIGNAL(btn5()),this,SLOT(btn5()));
    connect(key,SIGNAL(btn6()),this,SLOT(btn6()));
    connect(key,SIGNAL(btn7()),this,SLOT(btn7()));
    connect(key,SIGNAL(btn8()),this,SLOT(btn8()));
    connect(key,SIGNAL(btn9()),this,SLOT(btn9()));
    connect(key,SIGNAL(btn0()),this,SLOT(btn0()));
    connect(key,SIGNAL(btn00()),this,SLOT(btn00()));
    connect(key,SIGNAL(btnd()),this,SLOT(btnd()));
    connect(key,SIGNAL(btndian()),this,SLOT(btndian()));
    connect(key,SIGNAL(btnxian()),this,SLOT(btnxian()));
    connect(key,SIGNAL(quxiao()),this,SLOT(quxiao()));
    key->exec();
    if(key->btn_t_f == true)
    {
        edit_text = ui->lineEdit->text();
        //更新主页面
        Table_Number_Message::lable1 = NULL;
        //right_down->verticalScrollBar()->setValue(0);
        qDeleteAll(ui->scrollAreaWidgetContents->children());

        QSqlQueryModel *model1 = new QSqlQueryModel;
        QSqlQueryModel *table_on = new QSqlQueryModel;
        QString table = "%" + edit_text + "%";
        model1->setQuery(QString("select * from cey_bt_table  where ch_tableno like '%1' ").arg(table));
        table_on->setQuery(QString("select * from cey_bt_table where ch_billno <> '' and ch_tableno like '%1' ").arg(table));//已开台的

        QSqlQueryModel *taineirong = new QSqlQueryModel;
        QGridLayout* layout =  new QGridLayout();
        bool on;
        for(int m = 0; m < model1->rowCount(); m++)
        {
            table_ren.clear();
            table_yuan.clear();
            on = false;
            table_name << model1->record(m).value(1).toString();

            for(int i = 0; i < table_on->rowCount(); i++)
            {
                table_ren << table_on->record(i).value(2).toString();
                table_yuan << table_on->record(i).value(8).toString();
                //qDebug() << model1->record(m).value(0).toString() << "Xx" << table_on->record(i).value(1).toString();
                if(model1->record(m).value(0).toString() == table_on->record(i).value(0).toString() )//桌号相同
                {
                    taineirong->clear();
                    taineirong->setQuery(QString("Select ch_billno,ch_tableno,int_person,vch_memo,ch_state,ch_payno,vch_operID,dt_operdate,vch_operID2,dt_operdate2,ch_order_state,num_cost,(select IFNULL(vch_operator,'') from  cey_sys_operator where vch_operID = cey_u_table.vch_waiter) ,(select IFNULL(vch_tablename,'') from cey_bt_table where ch_tableno = cey_u_table.ch_tableno) from cey_u_table where cey_u_table.ch_billno = '%1'  and cey_u_table.ch_tableno = '%2'").arg(table_on->record(i).value(3).toString()).arg(table_on->record(i).value(0).toString()));
                    if(m == i)
                    {
                        table_danhao = table_on->record(i).value(0).toString();
                        table_lock = table_on->record(i).value(1).toString();
                    }

                    on = true;
                    QString index_str;
                    index_str = taineirong->record(0).value(13).toString() + "\n" + taineirong->record(0).value(2).toString() + tr("人\n")  + "￥" + QString().sprintf("%0.2f",taineirong->record(0).value(11).toFloat());
                    if(width > 800)
                    {
                        table_number_message =  new Table_Number_Message(m/8,m%8,index_str,2);
                        table_number_message->setStyleSheet("background-color:#FFF4D6;border-radius:5px;");
                        connect(table_number_message,SIGNAL(toType(int)),this,SLOT(table_message(int)));
                        layout->addWidget(table_number_message,m/8,m%8,1,1,Qt::AlignTop);
                    }
                    else
                    {
                        table_number_message =  new Table_Number_Message(m/6,m%6,index_str,2);
                        table_number_message->setStyleSheet("background-color:#FFF4D6;border-radius:5px;");
                        connect(table_number_message,SIGNAL(toType(int)),this,SLOT(table_message(int)));
                        layout->addWidget(table_number_message,m/6,m%6,1,1,Qt::AlignTop);
                    }

                }
                else
                {

                }
            }
            if(on == false)
            {
                QString index_str;
                index_str.clear();
                index_str = model1->record(m).value(1).toString() + "\n" + model1->record(m).value(5).toString() +tr("人");
                if(width > 800)
                {
                    table_number_message =  new Table_Number_Message(m/8,m%8,index_str,1);
                    table_number_message->setStyleSheet("background-color:#FFFFFF;border-radius:5px;");
                    layout->addWidget(table_number_message,m/8,m%8,1,1,Qt::AlignTop);
                    connect(table_number_message,SIGNAL(toType(int)),this,SLOT(table_message(int)));
                }
                else
                {
                    table_number_message =  new Table_Number_Message(m/6,m%6,index_str,1);
                    table_number_message->setStyleSheet("background-color:#FFFFFF;border-radius:5px;");
                    layout->addWidget(table_number_message,m/6,m%6,1,1,Qt::AlignTop);
                    connect(table_number_message,SIGNAL(toType(int)),this,SLOT(table_message(int)));
                }
            }
            on = false;
        }
        layout->setAlignment(Qt::AlignTop);
        layout->setSpacing(12);
        ui->scrollAreaWidgetContents->setLayout(layout);
    }
    else
    {
        edit_text.clear();
    }*/
}

void w_scr_dish_restaurant_dialog::btn1()
{
    //ui->lineEdit->insert("1");
}

void w_scr_dish_restaurant_dialog::btn2()
{
    //ui->lineEdit->insert("2");
}
void w_scr_dish_restaurant_dialog::btn3()
{
    //ui->lineEdit->insert("3");
}

void w_scr_dish_restaurant_dialog::btn4()
{
    //ui->lineEdit->insert("4");
}

void w_scr_dish_restaurant_dialog::btn5()
{
    //ui->lineEdit->insert("5");
}
void w_scr_dish_restaurant_dialog::btn6()
{
    //ui->lineEdit->insert("6");
}

void w_scr_dish_restaurant_dialog::btn7()
{
    //ui->lineEdit->insert("7");
}

void w_scr_dish_restaurant_dialog::btn8()
{
    //ui->lineEdit->insert("8");
}
void w_scr_dish_restaurant_dialog::btn9()
{
    //ui->lineEdit->insert("9");
}

void w_scr_dish_restaurant_dialog::btn0()
{
    //ui->lineEdit->insert("0");
}

void w_scr_dish_restaurant_dialog::btn00()
{
    //ui->lineEdit->insert("00");
}
void w_scr_dish_restaurant_dialog::btnd()
{
    //ui->lineEdit->backspace();
}

void w_scr_dish_restaurant_dialog::btndian()
{
    //ui->lineEdit->insert(".");
}

void w_scr_dish_restaurant_dialog::btnxian()
{
    //ui->lineEdit->insert("-");
}

void w_scr_dish_restaurant_dialog::quxiao()
{
    //ui->lineEdit->clear();
}

void w_scr_dish_restaurant_dialog::showxiaolei(const QString &str)
{/*
    if(str.isEmpty())
    {
        return;
    }
    kaitai_dalei = str;
    table_name.clear();
    edit_text.clear();

    Table_Number_Message::lable1 = NULL;
    //right_down->verticalScrollBar()->setValue(0);
    qDeleteAll(ui->scrollAreaWidgetContents->children());

    QSqlQueryModel *model1 = new QSqlQueryModel;
    QSqlQueryModel *table_on = new QSqlQueryModel;
    model1->setQuery(QString("select * from cey_bt_table where ch_areaNo = '%1' ").arg(str));
    table_on->setQuery(QString("select * from cey_bt_table where ch_billno <> '' and ch_areaNo = '%1' ").arg(str));//已开台的

    QSqlQueryModel *taineirong = new QSqlQueryModel;
    QGridLayout* layout =  new QGridLayout();
    bool on;
    for(int m = 0; m < model1->rowCount(); m++)
    {
        table_ren.clear();
        table_yuan.clear();
        on = false;
        table_name << model1->record(m).value(1).toString();

        for(int i = 0; i < table_on->rowCount(); i++)
        {
            table_ren << table_on->record(i).value(2).toString();
            table_yuan << table_on->record(i).value(8).toString();
            //qDebug() << model1->record(m).value(0).toString() << "Xx" << table_on->record(i).value(1).toString();
            if(model1->record(m).value(0).toString() == table_on->record(i).value(0).toString() )//桌号相同
            {
                taineirong->clear();
                taineirong->setQuery(QString("Select ch_billno,ch_tableno,int_person,vch_memo,ch_state,ch_payno,vch_operID,dt_operdate,vch_operID2,dt_operdate2,ch_order_state,num_cost,(select IFNULL(vch_operator,'') from  cey_sys_operator where vch_operID = cey_u_table.vch_waiter) ,(select IFNULL(vch_tablename,'') from cey_bt_table where ch_tableno = cey_u_table.ch_tableno) from cey_u_table where cey_u_table.ch_billno = '%1'  and cey_u_table.ch_tableno = '%2'").arg(table_on->record(i).value(3).toString()).arg(table_on->record(i).value(0).toString()));
                if(m == i)
                {
                    table_danhao = table_on->record(i).value(0).toString();
                    table_lock = table_on->record(i).value(1).toString();
                }

                on = true;
                QString index_str;
                index_str = taineirong->record(0).value(13).toString() + "\n" + taineirong->record(0).value(2).toString() + tr("人\n")  + "￥" + QString().sprintf("%0.2f",taineirong->record(0).value(11).toFloat());
                if(width > 800)
                {
                    table_number_message =  new Table_Number_Message(m/8,m%8,index_str,2);
                    table_number_message->setStyleSheet("background-color:#FFF4D6;border-radius:5px;");
                    connect(table_number_message,SIGNAL(toType(int)),this,SLOT(table_message(int)));
                    layout->addWidget(table_number_message,m/8,m%8,1,1,Qt::AlignTop);
                }
                else
                {
                    table_number_message =  new Table_Number_Message(m/6,m%6,index_str,2);
                    table_number_message->setStyleSheet("background-color:#FFF4D6;border-radius:5px;");
                    connect(table_number_message,SIGNAL(toType(int)),this,SLOT(table_message(int)));
                    layout->addWidget(table_number_message,m/6,m%6,1,1,Qt::AlignTop);
                }

            }
            else
            {

            }
        }
        if(on == false)
        {
            QString index_str;
            index_str.clear();
            index_str = model1->record(m).value(1).toString() + "\n" + model1->record(m).value(5).toString() +tr("人");
            if(width > 800)
            {
                table_number_message =  new Table_Number_Message(m/8,m%8,index_str,1);
                table_number_message->setStyleSheet("background-color:#FFFFFF;border-radius:5px;");
                layout->addWidget(table_number_message,m/8,m%8,1,1,Qt::AlignTop);
                connect(table_number_message,SIGNAL(toType(int)),this,SLOT(table_message(int)));
            }
            else
            {
                table_number_message =  new Table_Number_Message(m/6,m%6,index_str,1);
                table_number_message->setStyleSheet("background-color:#FFFFFF;border-radius:5px;");
                layout->addWidget(table_number_message,m/6,m%6,1,1,Qt::AlignTop);
                connect(table_number_message,SIGNAL(toType(int)),this,SLOT(table_message(int)));
            }
        }
        on = false;
    }
    layout->setAlignment(Qt::AlignTop);
    layout->setSpacing(12);
    ui->scrollAreaWidgetContents->setLayout(layout);*/
}

void w_scr_dish_restaurant_dialog::table_message(int num)//点击餐桌的操作
{

}

void w_scr_dish_restaurant_dialog::table_message(const QString &tableno)
{
    QSqlQueryModel *model = new QSqlQueryModel;
    QSqlQuery query;
    QSqlDatabase::database().transaction();

    model->setQuery(QString("select * from cey_bt_table  where ch_tableno = '%1' ").arg(tableno));//已开台的

    if(model->record(0).value(4).toString() == "Y")
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("该餐桌已被锁定,请确定!"));
        
        return;
    }
    QString canzhuohao = model->record(0).value(0).toString();

    if(model->record(0).value(3).toString().trimmed().isEmpty())//未开台
    {
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
        if(danhao.trimmed().isEmpty())
        {
            danhao = currentDate + QString().sprintf("%05d",1);
        }
        else
        {
            QString str = danhao.right(danhao.length() - 7);
            int aa = str.toInt() + 1;
            danhao = currentDate + QString().sprintf("%05d",aa);
        }

        table_danhao = danhao;
        table_no = model->record(0).value(0).toString();

        QString zhuohao = model->record(0).value(0).toString();
        QString renshu = model->record(0).value(5).toString();

        QString kaitairen = n_func::gs_operid;
        QString time = ldt_datetime.toString("yyyy-MM-dd hh:mm:ss");
        founding_cook * cook = new founding_cook(danhao,zhuohao,renshu,kaitairen,time,"1");
        /*cook->exec();*/backheader::static_dialogexec(*cook, "");

        if(cook->dianccai == false && cook->wangluo == false && cook->btn_t_f == true)//点击确定
        {
            query.prepare("update cey_bt_table set ch_billno=:danhao where ch_tableno= :zhuohao");
            query.bindValue(":danhao",danhao);
            query.bindValue(":zhuohao",zhuohao);
            if(query.exec())
            {
            }
            else
            {
                QSqlDatabase::database().rollback();
                lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_bt_table失败"));
                
                return;
            }

            if(!cook->songcanren.isEmpty() && !cook->beizhu.isEmpty())
            {
                query.prepare(QString("insert into cey_u_table( ch_billno , ch_tableno , int_person , ch_state ,  vch_operID , dt_operdate , ch_order_state , num_cost,vch_waiter,vch_memo) values(:danhao,:zhuohao,:renshu,:state,:oper,:time,:order_state,:num,:wait,:beizhu)"));
                query.bindValue(":danhao",danhao);
                query.bindValue(":zhuohao",zhuohao);
                query.bindValue(":renshu",renshu);
                query.bindValue(":state","1");
                query.bindValue(":oper",kaitairen);
                query.bindValue(":time",time);
                query.bindValue(":order_state","0");
                query.bindValue(":num","0.00");
                query.bindValue(":wait",cook->songcanren);
                query.bindValue(":beizhu",cook->beizhu);
                if(query.exec())
                {
                }
                else
                {
                    QSqlDatabase::database().rollback();
                    qDebug() << query.lastError();  lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_u_table失败！"));
                    
                    return;
                }
            }
            else if(!cook->songcanren.isEmpty() && cook->beizhu.isEmpty())
            {
                query.prepare(QString("insert into cey_u_table( ch_billno , ch_tableno , int_person , ch_state ,  vch_operID , dt_operdate , ch_order_state , num_cost,vch_waiter,vch_memo) values(:danhao,:zhuohao,:renshu,:state,:oper,:time,:order_state,:num,:wait,null)"));
                query.bindValue(":danhao",danhao);
                query.bindValue(":zhuohao",zhuohao);
                query.bindValue(":renshu",renshu);
                query.bindValue(":state","1");
                query.bindValue(":oper",kaitairen);
                query.bindValue(":time",time);
                query.bindValue(":order_state","0");
                query.bindValue(":num","0.00");
                query.bindValue(":wait",cook->songcanren);
                if(query.exec())
                {
                }
                else
                {
                    QSqlDatabase::database().rollback();
                    qDebug() << query.lastError();  lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_u_table失败！"));
                    
                    return;
                }
            }
            else if(cook->songcanren.isEmpty() && !cook->beizhu.isEmpty())
            {
                query.prepare(QString("insert into cey_u_table( ch_billno , ch_tableno , int_person , ch_state ,  vch_operID , dt_operdate , ch_order_state , num_cost,vch_waiter,vch_memo) values(:danhao,:zhuohao,:renshu,:state,:oper,:time,:order_state,:num,null,:beizhu)"));
                query.bindValue(":danhao",danhao);
                query.bindValue(":zhuohao",zhuohao);
                query.bindValue(":renshu",renshu);
                query.bindValue(":state","1");
                query.bindValue(":oper",kaitairen);
                query.bindValue(":time",time);
                query.bindValue(":order_state","0");
                query.bindValue(":num","0.00");
                query.bindValue(":beizhu",cook->beizhu);
                //qDebug() << query.boundValues();
                if(query.exec())
                {
                }
                else
                {
                    QSqlDatabase::database().rollback();
                    qDebug() << query.lastError();  lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_u_table失败！"));
                    
                    return;
                }
            }
            else
            {
                query.prepare(QString("insert into cey_u_table( ch_billno , ch_tableno , int_person , ch_state ,  vch_operID , dt_operdate , ch_order_state , num_cost,vch_waiter,vch_memo) values(:danhao,:zhuohao,:renshu,:state,:oper,:time,:order_state,:num,null,null)"));
                query.bindValue(":danhao",danhao);
                query.bindValue(":zhuohao",zhuohao);
                query.bindValue(":renshu",renshu);
                query.bindValue(":state","1");
                query.bindValue(":oper",kaitairen);
                query.bindValue(":time",time);
                query.bindValue(":order_state","0");
                query.bindValue(":num",tr("0.00"));
                if(query.exec())
                {
                }
                else
                {
                    QSqlDatabase::database().rollback();
                    qDebug() << query.lastError();  lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_u_table失败！"));
                    
                    return;
                }
            }

            query.prepare(QString("insert into cey_u_master( ch_billno , ch_state , vch_operID , dt_operdate )values(:danhao,:state,:oper,:time)"));
            query.bindValue(":danhao",danhao);
            query.bindValue(":state","1");
            query.bindValue(":oper",kaitairen);
            query.bindValue(":time",time);
            if(query.exec())
            {
            }
            else
            {
                QSqlDatabase::database().rollback();
                qDebug() << query.lastError();  lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_u_master失败！"));
                
                return;
            }
        }
        else if(cook->dianccai == true && cook->wangluo == false && cook->btn_t_f == false)//点击点菜
        {
            query.prepare("update cey_bt_table set ch_billno=:danhao where ch_tableno= :zhuohao");
            query.bindValue(":danhao",danhao);
            query.bindValue(":zhuohao",zhuohao);
            if(query.exec())
            {
            }
            else
            {
                QSqlDatabase::database().rollback();
                lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_bt_table失败"));
                
                return;
            }

            if(!cook->songcanren.isEmpty() && !cook->beizhu.isEmpty())
            {
                query.prepare(QString("insert into cey_u_table( ch_billno , ch_tableno , int_person , ch_state ,  vch_operID , dt_operdate , ch_order_state , num_cost,vch_waiter,vch_memo) values(:danhao,:zhuohao,:renshu,:state,:oper,:time,:order_state,:num,:wait,:beizhu)"));
                query.bindValue(":danhao",danhao);
                query.bindValue(":zhuohao",zhuohao);
                query.bindValue(":renshu",renshu);
                query.bindValue(":state","1");
                query.bindValue(":oper",kaitairen);
                query.bindValue(":time",time);
                query.bindValue(":order_state","0");
                query.bindValue(":num","0.00");
                query.bindValue(":wait",cook->songcanren);
                query.bindValue(":beizhu",cook->beizhu);
                if(query.exec())
                {
                }
                else
                {
                    QSqlDatabase::database().rollback();
                    qDebug() << query.lastError();  lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_u_table失败！"));
                    
                    return;
                }
            }
            else if(!cook->songcanren.isEmpty() && cook->beizhu.isEmpty())
            {
                query.prepare(QString("insert into cey_u_table( ch_billno , ch_tableno , int_person , ch_state ,  vch_operID , dt_operdate , ch_order_state , num_cost,vch_waiter,vch_memo) values(:danhao,:zhuohao,:renshu,:state,:oper,:time,:order_state,:num,:wait,null)"));
                query.bindValue(":danhao",danhao);
                query.bindValue(":zhuohao",zhuohao);
                query.bindValue(":renshu",renshu);
                query.bindValue(":state","1");
                query.bindValue(":oper",kaitairen);
                query.bindValue(":time",time);
                query.bindValue(":order_state","0");
                query.bindValue(":num","0.00");
                query.bindValue(":wait",cook->songcanren);
                if(query.exec())
                {
                }
                else
                {
                    QSqlDatabase::database().rollback();
                    qDebug() << query.lastError();  lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_u_table失败！"));
                    
                    return;
                }
            }
            else if(cook->songcanren.isEmpty() && !cook->beizhu.isEmpty())
            {
                query.prepare(QString("insert into cey_u_table( ch_billno , ch_tableno , int_person , ch_state ,  vch_operID , dt_operdate , ch_order_state , num_cost,vch_waiter,vch_memo) values(:danhao,:zhuohao,:renshu,:state,:oper,:time,:order_state,:num,null,:beizhu)"));
                query.bindValue(":danhao",danhao);
                query.bindValue(":zhuohao",zhuohao);
                query.bindValue(":renshu",renshu);
                query.bindValue(":state","1");
                query.bindValue(":oper",kaitairen);
                query.bindValue(":time",time);
                query.bindValue(":order_state","0");
                query.bindValue(":num","0.00");
                query.bindValue(":beizhu",cook->beizhu);
                if(query.exec())
                {
                }
                else
                {
                    QSqlDatabase::database().rollback();
                    qDebug() << query.lastError();  lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_u_table失败！"));
                    
                    return;
                }
            }
            else
            {
                query.prepare(QString("insert into cey_u_table( ch_billno , ch_tableno , int_person , ch_state ,  vch_operID , dt_operdate , ch_order_state , num_cost,vch_waiter,vch_memo) values(:danhao,:zhuohao,:renshu,:state,:oper,:time,:order_state,:num,null,null)"));
                query.bindValue(":danhao",danhao);
                query.bindValue(":zhuohao",zhuohao);
                query.bindValue(":renshu",renshu);
                query.bindValue(":state","1");
                query.bindValue(":oper",kaitairen);
                query.bindValue(":time",time);
                query.bindValue(":order_state","0");
                query.bindValue(":num","0.00");
                if(query.exec())
                {
                }
                else
                {
                    QSqlDatabase::database().rollback();
                    qDebug() << query.lastError();  lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_u_table失败！"));
                    
                    return;
                }
            }

            query.prepare(QString("insert into cey_u_master( ch_billno , ch_state , vch_operID , dt_operdate )values(:danhao,:state,:oper,:time)"));
            query.bindValue(":danhao",danhao);
            query.bindValue(":state","1");
            query.bindValue(":oper",kaitairen);
            query.bindValue(":time",time);
            if(query.exec())
            {
            }
            else
            {
                QSqlDatabase::database().rollback();
                qDebug() << query.lastError();  lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_u_master失败！"));
                
                return;
            }

            query.prepare(QString("update cey_bt_table set  ch_lockflag ='Y'  where ch_tableno = :zhuohao"));
            query.bindValue(":zhuohao",zhuohao);
            if(query.exec())
            {
            }
            else
            {
                QSqlDatabase::database().rollback();
                lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_bt_tabler失败！"));
                
                return;
            }

            //弹出点菜页面
            table_order_2 order(this);
            connect(&order,SIGNAL(pay_over()),this,SLOT(pay_over()));
            backheader::static_dialogexec(order, tr("中餐点菜"), backheader::fullwindowsize);qDebug() << "中餐点菜";
        }
        else if(cook->dianccai == true && cook->wangluo == false && cook->btn_t_f == false)//点击网络订单导入
        {

        }
        else//点击取消
        {
            return;
        }
    }
    else//已开台
    {
        table_danhao = model->record(0).value(3).toString();
        table_no = model->record(0).value(0).toString();
        query.prepare(QString("update cey_bt_table set  ch_lockflag ='Y'  where ch_tableno = :zhuohao"));
        query.bindValue(":zhuohao",canzhuohao);
        if(query.exec())
        {
        }
        else
        {
            QSqlDatabase::database().rollback();
            lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_bt_tabler失败！"));
            
            return;
        }

        //弹出点菜页面
        table_order_2 order(this);
        connect(&order,SIGNAL(pay_over()),this,SLOT(pay_over()));
        backheader::static_dialogexec(order, tr("中餐点菜"), backheader::fullwindowsize);qDebug() << "中餐点菜";

    }
    QSqlDatabase::database().commit();
    //更新主页面
    //    updtae_page();
    //    update_label();
    init_cook(_init_cook_filter);
}

void w_scr_dish_restaurant_dialog::show_order()
{
    this->close();
}

void w_scr_dish_restaurant_dialog::pay_over()
{
    on_pushButton_4_clicked();
}

void w_scr_dish_restaurant_dialog::on_pushButton_11_clicked()//所有餐桌
{/*
    kaitai_dalei.clear();
    table_name.clear();
    edit_text.clear();

    Table_Number_Message::lable1 = NULL;
    //right_down->verticalScrollBar()->setValue(0);
    qDeleteAll(ui->scrollAreaWidgetContents->children());

    QSqlQueryModel *model1 = new QSqlQueryModel;
    QSqlQueryModel *table_on = new QSqlQueryModel;
    model1->setQuery(QString("select * from cey_bt_table "));
    table_on->setQuery(QString("select * from cey_bt_table where ch_billno <> '' "));//已开台的

    QSqlQueryModel *taineirong = new QSqlQueryModel;
    QGridLayout* layout =  new QGridLayout();
    bool on;
    for(int m = 0; m < model1->rowCount(); m++)
    {
        table_ren.clear();
        table_yuan.clear();
        on = false;
        table_name << model1->record(m).value(1).toString();

        for(int i = 0; i < table_on->rowCount(); i++)
        {
            table_ren << table_on->record(i).value(2).toString();
            table_yuan << table_on->record(i).value(8).toString();
            //qDebug() << model1->record(m).value(0).toString() << "Xx" << table_on->record(i).value(1).toString();
            if(model1->record(m).value(0).toString() == table_on->record(i).value(0).toString() )//桌号相同
            {
                taineirong->clear();
                taineirong->setQuery(QString("Select ch_billno,ch_tableno,int_person,vch_memo,ch_state,ch_payno,vch_operID,dt_operdate,vch_operID2,dt_operdate2,ch_order_state,num_cost,(select IFNULL(vch_operator,'') from  cey_sys_operator where vch_operID = cey_u_table.vch_waiter) ,(select IFNULL(vch_tablename,'') from cey_bt_table where ch_tableno = cey_u_table.ch_tableno) from cey_u_table where cey_u_table.ch_billno = '%1'  and cey_u_table.ch_tableno = '%2'").arg(table_on->record(i).value(3).toString()).arg(table_on->record(i).value(0).toString()));
                if(m == i)
                {
                    table_danhao = table_on->record(i).value(0).toString();
                    table_lock = table_on->record(i).value(1).toString();
                }

                on = true;
                QString index_str;
                index_str = taineirong->record(0).value(13).toString() + "\n" + taineirong->record(0).value(2).toString() + tr("人\n")  + "￥" + QString().sprintf("%0.2f",taineirong->record(0).value(11).toFloat());
                if(width > 800)
                {
                    table_number_message =  new Table_Number_Message(m/8,m%8,index_str,2);
                    table_number_message->setStyleSheet("background-color:#FFF4D6;border-radius:5px;");
                    connect(table_number_message,SIGNAL(toType(int)),this,SLOT(table_message(int)));
                    layout->addWidget(table_number_message,m/8,m%8,1,1,Qt::AlignTop);
                }
                else
                {
                    table_number_message =  new Table_Number_Message(m/6,m%6,index_str,2);
                    table_number_message->setStyleSheet("background-color:#FFF4D6;border-radius:5px;");
                    connect(table_number_message,SIGNAL(toType(int)),this,SLOT(table_message(int)));
                    layout->addWidget(table_number_message,m/6,m%6,1,1,Qt::AlignTop);
                }

            }
            else
            {

            }
        }
        if(on == false)
        {
            QString index_str;
            index_str.clear();
            index_str = model1->record(m).value(1).toString() + "\n" + model1->record(m).value(5).toString() +tr("人");
            if(width > 800)
            {
                table_number_message =  new Table_Number_Message(m/8,m%8,index_str,1);
                table_number_message->setStyleSheet("background-color:#FFFFFF;border-radius:5px;");
                layout->addWidget(table_number_message,m/8,m%8,1,1,Qt::AlignTop);
                connect(table_number_message,SIGNAL(toType(int)),this,SLOT(table_message(int)));
            }
            else
            {
                table_number_message =  new Table_Number_Message(m/6,m%6,index_str,1);
                table_number_message->setStyleSheet("background-color:#FFFFFF;border-radius:5px;");
                layout->addWidget(table_number_message,m/6,m%6,1,1,Qt::AlignTop);
                connect(table_number_message,SIGNAL(toType(int)),this,SLOT(table_message(int)));
            }
        }
        on = false;
    }
    layout->setAlignment(Qt::AlignTop);
    layout->setSpacing(12);
    ui->scrollAreaWidgetContents->setLayout(layout);*/
}

void w_scr_dish_restaurant_dialog::on_pushButton_4_clicked()//数据刷新
{
    //    updtae_page();
    init_cook(_init_cook_filter);

}

void w_scr_dish_restaurant_dialog::on_pushButton_6_clicked()//预订
{

}

void w_scr_dish_restaurant_dialog::on_pushButton_13_clicked()//上一页
{
    //    ui->scrollArea->verticalScrollBar()->setValue(ui->scrollArea->verticalScrollBar()->value() - ui->scrollArea->height());
}

void w_scr_dish_restaurant_dialog::on_pushButton_12_clicked()//下一页
{
    //    ui->scrollArea->verticalScrollBar()->setValue(ui->scrollArea->verticalScrollBar()->value() + ui->scrollArea->height());
}

void w_scr_dish_restaurant_dialog::on_pushButton_8_clicked()//营业单据
{
    if(n_func::gs_operid != "0000")
    {
        QSqlQueryModel model;
        ;
        model.setQuery(QString("select count(*)  from cey_sys_oper_purview  where ch_purviewno = '0201' and vch_operID ='%1'").arg(n_func::gs_operid));
        if(model.record(0).value(0).toInt() == 0)
        {
            lds_messagebox::warning(this, tr("消息提示"),tr("该操作员无此操作权限!"));
            
            return;
        }
    }
    business_documents_query query(this);
    query.resize(backheader::mainwindowsize);
    /*query->exec();*/backheader::static_dialogexec(query, tr("营业单据查询"));
}

void w_scr_dish_restaurant_dialog::on_pushButton_7_clicked()//交班操作
{
    power_oper oper;
    /*oper.exec();*/backheader::static_dialogexec(oper, tr("权限核实"));

    if(oper.btn_t_f == true)
    {
        if(oper.operid == n_func::gs_operid)
        {
            ;
            lds_messagebox::warning(this, tr("消息提示"),tr("sorry,接班人和交班人不能为同一人!"));
            
            return;
        }
    }
    else
    {
        return;
    }

    shift_work work(oper.operid);
    backheader::static_dialogexec(work, tr("交班对账表"));
    // this->close();
    if(work.btn_t_f == true)
    {
        //this->close();
        lock_screen lock("2");
        /*lock->exec();*/backheader::static_dialogexec(lock, tr("前台锁屏"));
    }
}

void w_scr_dish_restaurant_dialog::on_pushButton_5_clicked()//开钱箱
{
    //    ;
    //    if(n_func::gs_operid != "0000")
    //    {
    //        QSqlQueryModel model;
    //        model.setQuery(QString("select count(*)  from cey_sys_oper_purview  where ch_purviewno = '0209' and vch_operID ='%1'").arg(n_func::gs_operid));
    //        if(model.record(0).value(0).toInt() == 0)
    //        {
    //            lds_messagebox::warning(this, tr("消息提示"),tr("该操作员无此操作权限!"));
    //
    //            return;
    //        }
    //    }


    //    QSettings *configIniRead = new QSettings(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    //    QStringList instructionedit = configIniRead->value("system_setting/qianxiangzhiling").toStringList();
    //    QString instr;
    //    for(int i = 0; i < instructionedit.count(); i++)
    //    {
    //        if(i != instructionedit.count() -1)
    //        {
    //            instr.append(instructionedit[i]);
    //            instr.append(",");
    //        }
    //        else
    //        {
    //            instr.append(instructionedit[i]);
    //        }
    //    }

    //    //instr = "27,112,0,40,40";
    //    if(instr == "27,112,0,40,40")
    //    {
    //        public_mainprinter printer;
    //        QByteArray b;
    //        QString path;
    //        QString duankoutype = configIniRead->value("system_setting/dyjjiekouleixing").toString();
    //        if(duankoutype == tr("串口打印") || duankoutype == tr("USB打印"))
    //        {
    //            path = configIniRead->value("system_setting/dayinjiduankou").toString();
    //        }
    //        else
    //        {
    //            path = configIniRead->value("system_setting/dayinjiip").toString() + ":9100";
    //        }
    //        b+=printer.drawText(instr.toLocal8Bit(),"");
    //        b=printer.setPrintCount(b, 1);
    //        printer.asynchPrint(b, path);
    //    }
    //    else
    //    {
    //        lds_messagebox::warning(this, tr("消息提示"),tr("开钱箱指令错误,请检查设置!"));
    //
    //        return;
    //    }
}

void w_scr_dish_restaurant_dialog::on_pushButton_9_clicked()//经理管理
{
    //    manager_view view;
    //    connect(&view,SIGNAL(kuacan()),this,SLOT(show_order()));
    //    view.exec();

    //        qDebug() << view->fast_model << sys_model;
    //    if(view->fast_model != sys_model)
    //    {
    //        this->close();
    //        fast_order_2 *fast = new fast_order_2();
    //        fast->exec();
    //    }
}

