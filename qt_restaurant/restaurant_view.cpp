#include "restaurant_view.h"
#include "ui_restaurant_view.h"
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
#include "fast_order_2.h"
#include <QScrollBar>
#include "backheader.h"
#include <QSqlError>

QSqlQueryModel *restaurant_view::model_dalei;
QSqlQueryModel *restaurant_view::model_cook;
QString restaurant_view::dalei;
QString restaurant_view::table_danhao;
QString restaurant_view::table_lock;
QString restaurant_view::table_no;

restaurant_view::restaurant_view(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::restaurant_view)
{
    ui->setupUi(this);
    //this->setWindowFlags(Qt::FramelessWindowHint|Qt::Dialog);this->setStyleSheet("QDialog{border:2px solid gray}");
    QDesktopWidget *d=QApplication::desktop();
    width = d->width();
    //    this->setFixedSize(d->width(),d->height() - 40);
    sys_model.clear();
    QSettings *configIniRead = new QSettings(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    sys_model = configIniRead->value("system_setting/yingyemoshi").toString();
    init_widget();
    init_cook();
}

restaurant_view::~restaurant_view()
{
    delete ui;
}

void restaurant_view::init_widget()
{
    model_dalei = new QSqlQueryModel();
    model_dalei->setQuery(QString("select * from cey_bt_table_type"));
    for(int i = 0; i < model_dalei->rowCount(); i++)
    {
        if(i == 0)
        {
            first_in.append("1");
        }
        else
        {
            first_in.append("0");
        }
        ui->widget->addSort(model_dalei->record(i).value(0).toString(),model_dalei->record(i).value(1).toString());
        connect(ui->widget,SIGNAL(kindSelect(QString)),this,SLOT(showxiaolei(QString)));//小类菜品
    }
    dalei = model_dalei->record(0).value(0).toString();
    ui->widget->showSort();
}

void restaurant_view::init_cook()
{
    table_name.clear();
    ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QSqlQueryModel *model1 = new QSqlQueryModel;
    QSqlQueryModel *table_on = new QSqlQueryModel;
    model1->setQuery(QString("select * from cey_bt_table"));
    table_on->setQuery(QString("select * from cey_bt_table where ch_billno <> ''"));//已开台的

    int kongtai = model1->rowCount() - table_on->rowCount();
    ui->label_3->setText(QString::number(kongtai));
    ui->label_9->setText(QString::number(table_on->rowCount()));
    QSqlQueryModel model_t;
    model_t.setQuery("Select NOW()");
    ui->label_8->setText(model_t.record(0).value(0).toDateTime().toString("yyyy-MM-dd"));
    if(n_func::gs_operid == "0000")
    {
        ui->label_5->setText("system");
    }
    else
    {
        model_t.clear();
        model_t.setQuery(QString("select vch_operator from cey_sys_operator where vch_operID = '%1' ").arg(n_func::gs_operid));
        ui->label_5->setText(model_t.record(0).value(0).toString());
    }

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

void restaurant_view::updtae_page()
{
    Table_Number_Message::lable1 = NULL;
    //right_down->verticalScrollBar()->setValue(0);
    qDeleteAll(ui->scrollAreaWidgetContents->children());

    QSqlQueryModel *model1 = new QSqlQueryModel;
    QSqlQueryModel *table_on = new QSqlQueryModel;
    if(!kaitai_dalei.isEmpty() && edit_text.isEmpty())
    {
        model1->setQuery(QString("select * from cey_bt_table where ch_areaNo = '%1' ").arg(kaitai_dalei));
        table_on->setQuery(QString("select * from cey_bt_table where ch_billno <> '' and ch_areaNo = '%1' ").arg(kaitai_dalei));//已开台的
    }
    else if(kaitai_dalei.isEmpty() && edit_text.isEmpty())//所有桌台
    {
        model1->setQuery(QString("select * from cey_bt_table  "));
        table_on->setQuery(QString("select * from cey_bt_table where ch_billno <> '' "));//已开台的
    }
    else//编码选台
    {
        QString table = "%" + edit_text + "%";
        model1->setQuery(QString("select * from cey_bt_table  "));
        table_on->setQuery(QString("select * from cey_bt_table where ch_billno <> '' and ch_tableno like '%1' ").arg(table));//已开台的
    }

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

void restaurant_view::update_label()
{
    QSqlQueryModel *model1 = new QSqlQueryModel;
    QSqlQueryModel *table_on = new QSqlQueryModel;
    model1->setQuery(QString("select * from cey_bt_table"));
    table_on->setQuery(QString("select * from cey_bt_table where ch_billno <> ''"));//已开台的

    int kongtai = model1->rowCount() - table_on->rowCount();
    ui->label_3->setText(QString::number(kongtai));
    ui->label_9->setText(QString::number(table_on->rowCount()));
}

void restaurant_view::on_pushButton_10_clicked()
{
    this->close();
}

void restaurant_view::on_pushButton_clicked()//...
{
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
    }
}

void restaurant_view::btn1()
{
    ui->lineEdit->insert("1");
}

void restaurant_view::btn2()
{
    ui->lineEdit->insert("2");
}
void restaurant_view::btn3()
{
    ui->lineEdit->insert("3");
}

void restaurant_view::btn4()
{
    ui->lineEdit->insert("4");
}

void restaurant_view::btn5()
{
    ui->lineEdit->insert("5");
}
void restaurant_view::btn6()
{
    ui->lineEdit->insert("6");
}

void restaurant_view::btn7()
{
    ui->lineEdit->insert("7");
}

void restaurant_view::btn8()
{
    ui->lineEdit->insert("8");
}
void restaurant_view::btn9()
{
    ui->lineEdit->insert("9");
}

void restaurant_view::btn0()
{
    ui->lineEdit->insert("0");
}

void restaurant_view::btn00()
{
    ui->lineEdit->insert("00");
}
void restaurant_view::btnd()
{
    ui->lineEdit->backspace();
}

void restaurant_view::btndian()
{
    ui->lineEdit->insert(".");
}

void restaurant_view::btnxian()
{
    ui->lineEdit->insert("-");
}

void restaurant_view::quxiao()
{
    ui->lineEdit->clear();
}

void restaurant_view::showxiaolei(const QString &str)
{
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
    ui->scrollAreaWidgetContents->setLayout(layout);
}

void restaurant_view::table_message(int num)//点击餐桌的操作
{
    ;
    QSqlQueryModel *model = new QSqlQueryModel;
    QSqlQuery query;
    QSqlDatabase::database().transaction();

    if(kaitai_dalei.isEmpty() && edit_text.isEmpty())//全部桌台
    {
        model->setQuery(QString("select * from cey_bt_table"));
    }
    else if(!kaitai_dalei.isEmpty() && edit_text.isEmpty())
    {
        model->setQuery(QString("select * from cey_bt_table where ch_areaNo = '%1'  ").arg(kaitai_dalei));
    }
    else//编码选择
    {
        QString table = "%" + edit_text + "%";
        model->setQuery(QString("select * from cey_bt_table  where ch_tableno like '%1' ").arg(table));//已开台的
    }

    if(model->record(num).value(4).toString() == "Y")
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("该餐桌已被锁定,请确定!"));
        
        return;
    }
    QString canzhuohao = model->record(num).value(0).toString();

    if(model->record(num).value(3).toString().trimmed().isEmpty())//未开台
    {
        QSqlQueryModel m_billno;
        m_billno.setQuery("Select NOW()");

        QString danhao;
        QDateTime ldt_datetime;
        ldt_datetime.setTime(m_billno.record(0).value(0).toTime());
        ldt_datetime.setDate(m_billno.record(0).value(0).toDate());
        QString currentDate = tr("B") + ldt_datetime.toString("yyMMdd");
        m_billno.clear();
        m_billno.setQuery(QString("Select Max( ch_billno ) from cey_u_table where left( ch_billno , 7 ) = '%1' ").arg(currentDate));
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
        table_no = model->record(num).value(0).toString();

        QString zhuohao = model->record(num).value(0).toString();
        QString renshu = model->record(num).value(5).toString();

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

            query.prepare(QString("insert into cey_u_master( ch_billno , ch_state , vch_operID , dt_operdate  ) values(:danhao,:state,:oper,:time)"));
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

            query.prepare(QString("insert into cey_u_master( ch_billno , ch_state , vch_operID , dt_operdate  ) values(:danhao,:state,:oper,:time)"));
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
        table_danhao = model->record(num).value(3).toString();
        table_no = model->record(num).value(0).toString();
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
    updtae_page();
    update_label();
}

void restaurant_view::show_order()
{
    this->close();
}

void restaurant_view::pay_over()
{
    on_pushButton_4_clicked();
}

void restaurant_view::on_pushButton_11_clicked()//所有餐桌
{
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
    ui->scrollAreaWidgetContents->setLayout(layout);
}

void restaurant_view::on_pushButton_4_clicked()//数据刷新
{
    updtae_page();
}

void restaurant_view::on_pushButton_6_clicked()//预订
{

}

void restaurant_view::on_pushButton_13_clicked()//上一页
{
    ui->scrollArea->verticalScrollBar()->setValue(ui->scrollArea->verticalScrollBar()->value() - ui->scrollArea->height());
}

void restaurant_view::on_pushButton_12_clicked()//下一页
{
    ui->scrollArea->verticalScrollBar()->setValue(ui->scrollArea->verticalScrollBar()->value() + ui->scrollArea->height());
}

void restaurant_view::on_pushButton_8_clicked()//营业单据
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

void restaurant_view::on_pushButton_7_clicked()//交班操作
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

void restaurant_view::on_pushButton_5_clicked()//开钱箱
{
    ;
    if(n_func::gs_operid != "0000")
    {
        QSqlQueryModel model;
        model.setQuery(QString("select count(*)  from cey_sys_oper_purview  where ch_purviewno = '0209' and vch_operID ='%1'").arg(n_func::gs_operid));
        if(model.record(0).value(0).toInt() == 0)
        {
            lds_messagebox::warning(this, tr("消息提示"),tr("该操作员无此操作权限!"));
            
            return;
        }
    }


    QSettings *configIniRead = new QSettings(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    QStringList instructionedit = configIniRead->value("system_setting/qianxiangzhiling").toStringList();
    QString instr;
    for(int i = 0; i < instructionedit.count(); i++)
    {
        if(i != instructionedit.count() -1)
        {
            instr.append(instructionedit[i]);
            instr.append(",");
        }
        else
        {
            instr.append(instructionedit[i]);
        }
    }

    //instr = "27,112,0,40,40";
    if(instr == "27,112,0,40,40")
    {
//        public_mainprinter printer;
        blockIns b;
        QString path;
        QString duankoutype = configIniRead->value("system_setting/dyjjiekouleixing").toString();
        if(duankoutype == tr("串口打印") || duankoutype == tr("USB打印"))
        {
            path = configIniRead->value("system_setting/dayinjiduankou").toString();
        }
        else
        {
            path = configIniRead->value("system_setting/dayinjiip").toString() + ":9100";
        }
        b+=backheader::printer->drawText(instr.toLocal8Bit(),"");
        b=backheader::printer->setPrintCount(b, 1);
        backheader::asyn_dataprint(b, path);
    }
    else
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("开钱箱指令错误,请检查设置!"));
        
        return;
    }
}

void restaurant_view::on_pushButton_9_clicked()//经理管理
{
    manager_view *view = new manager_view();
    connect(view,SIGNAL(kuacan()),this,SLOT(show_order()));
    view->exec();
    if(view->fast_model != sys_model)
    {
        this->close();
        fast_order_2 *fast = new fast_order_2();
        fast->exec();
    }
}
