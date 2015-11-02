#include "waimai_pay.h"
#include "ui_waimai_pay.h"
#include <QSqlQueryModel>
#include <QSqlRecord>
#include "lds_messagebox.h"
#include "other_pay_view.h"
#include "bank_card.h"
#include <QSqlQuery>
#include <QDateTime>
#include "n_func.h"
#include "fast_order_2.h"
#include "take_cashier.h"
#include <QSettings>
#include <QSqlError>
#include "backheader.h"
#include "lds_messagebox.h"

waimai_pay::waimai_pay(const QString &xiaofei,const QString &zhekou,const QString &zengsong,const QString &moling,const QString &shuishou,const QString &billno,const QString &waimaihao,fast_order_2 *parent) :
    _xiaofei(xiaofei),_zhekou(zhekou),_zengsong(zengsong),_moling(moling),_shuishou(shuishou),_billno(billno),_waimaihao(waimaihao.trimmed()),QDialog(parent),
    ui(new Ui::waimai_pay),
    _parent(parent)
{
    ui->setupUi(this);
    //this->setWindowFlags(Qt::FramelessWindowHint|Qt::Dialog);this->setStyleSheet("QDialog{border:2px solid gray}");
    count_dis = 0;
    kafujine = 0.00;
    shifoujiazhang = _waimaihao;
    btn_t_f = false;
    init_control();
    init_data();
}

waimai_pay::~waimai_pay()
{
    delete ui;
}

void waimai_pay::init_control()
{
    ui->lineEdit->setEnabled(false);
    ui->lineEdit_2->setEnabled(false);
    ui->lineEdit_3->setEnabled(false);
    ui->lineEdit_4->setEnabled(false);
    ui->lineEdit_5->setEnabled(false);
    ui->lineEdit_6->setEnabled(false);
    ui->lineEdit_7->setEnabled(false);
    ui->lineEdit_9->setFocus();

    table_model = new QStandardItemModel();
    QStringList list;
    list <<tr("行号")<<tr("付款方式")<<tr("实付现金")<<tr("付款金额")<<tr("凭证号");
    table_model->setHorizontalHeaderLabels(list);
    ui->tableView->setModel(table_model);
    //    ui->tableView->horizontalHeader()->setStyleSheet("QHeaderView::section{background-color: qlineargradient(spread:reflect, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(115, 185, 185, 255), stop:1 rgba(63, 124, 124, 255));color:black;height:40px;border-radius:0px;font-size:14pt;}");
    //    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    //    ui->tableView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //    ui->tableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //    ui->tableView->verticalHeader()->setDefaultSectionSize(40);
    //    //ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置属性为只读//351
    //    ui->tableView->verticalHeader()->hide();
    //    ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::SelectedClicked);ui->tableView->setModel(table_model);

    //        int row = table_model->rowCount();
    //        table_model->setRowCount(row + 1);
    //        table_model->setItem(row,0,new QStandardItem(QString::number(row+1))); //序号
    //        table_model->setItem(row,1,new QStandardItem("银行卡")); //编码
    //        table_model->setItem(row,2,new QStandardItem("50.0")); //品名
    //        table_model->setItem(row,3,new QStandardItem("50.0")); //数量
    //        table_model->setItem(row,4,new QStandardItem("adfdf")); //金额

    //    ui->tableView->setColumnWidth(0,40);
    //    ui->tableView->setColumnWidth(1,90);
    //    ui->tableView->setColumnWidth(2,90);
    //    ui->tableView->setColumnWidth(3,125);
    //    ui->tableView->setColumnWidth(4,132);

    //    ui->tableView->horizontalHeader()->setResizeMode(0,QHeaderView::Fixed);
    //    ui->tableView->horizontalHeader()->setResizeMode(1,QHeaderView::Fixed);
    //    ui->tableView->horizontalHeader()->setResizeMode(2,QHeaderView::Fixed);
    //    ui->tableView->horizontalHeader()->setResizeMode(3,QHeaderView::Fixed);
    //    ui->tableView->horizontalHeader()->setResizeMode(4,QHeaderView::Fixed);
}

void waimai_pay::init_data()
{
    ui->lineEdit->setText(_xiaofei);
    ui->lineEdit_2->setText(_zhekou);
    ui->lineEdit_3->setText(_zengsong);
    ui->lineEdit_5->setText(_moling);
    ui->lineEdit_6->setText(_shuishou);

    float yingshou = 0.00;
    yingshou = _xiaofei.toFloat() - _zhekou.toFloat() - _zengsong.toFloat() - _moling.toFloat() + _shuishou.toFloat();
    ui->lineEdit_4->setText(QString().sprintf("%0.2f",yingshou));

    QSqlQueryModel model;
    model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='cash_payall' "));
    QString str = model.record(0).value(0).toString();
    if(str == "1")
    {
        ui->lineEdit_9->setText(QString().sprintf("%0.2f",yingshou));
    }
    else
    {
        ui->lineEdit_9->setText(tr("0.00"));
    }

    ui->lineEdit_7->setText(tr("0.00"));
    ui->label_12->setText(tr("0.00"));
    ui->label_14->setText(QString().sprintf("%0.2f",yingshou));
}

void waimai_pay::init_fast_model()
{
    //    fast_order_2::model_dalei.setQuery(QString("SELECT  * FROM cey_bt_dish_type"));
    //    for(int i = 0; i < fast_order_2::model_dalei.rowCount(); i++)
    //    {
    //        if(i == fast_order_2::num_da)
    //        {
    //            fast_order_2::dalei = fast_order_2::model_dalei.record(i).value(0).toString();
    //        }
    //    }

    //    fast_order_2::model_xiaolei.setQuery(QString("SELECT  * FROM cey_bt_dish_subtype where ch_dish_typeno = '%1' ").arg(fast_order_2::dalei));
    //    for(int i = 0; i < fast_order_2::model_xiaolei.rowCount(); i++)
    //    {
    //        if(i == fast_order_2::num_xiao)
    //        {
    //            fast_order_2::xiaolei = fast_order_2::model_xiaolei.record(0).value(0).toString();
    //        }
    //    }
    //    if(fast_order_2::changyongcook == false)
    //    {
    //        fast_order_2::model_cook.setQuery(QString("SELECT  * FROM cey_bt_dish where ch_dish_typeno = '%1' and ch_sub_typeno ='%2' and (ch_stopflag = null or ch_stopflag = 'N') ").arg(fast_order_2::dalei).arg(fast_order_2::xiaolei));
    //    }
    //    else
    //    {
    //        fast_order_2::model_cook.setQuery(QString("select * from cey_bt_dish where ch_off = 'Y' and (ch_stopflag = null or ch_stopflag = 'N') "));
    //    }
}

void waimai_pay::print()
{
    QString shui = take_cashier::model_top->record(take_cashier::row).value(1).toString().trimmed();
    QString liushuihao_no = tr("流水号:") + shui.right(2) + "\n";
    QSettings *configIniRead = new QSettings(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    QSqlQueryModel model;
    QString str_t= configIniRead->value("system_setting/zhangdanbiaoti").toString();
    if(!str_t.isEmpty())
    {
        str_t.append("\n");
    }
    QString title = str_t +  tr("外卖/付款账单\n");//需居中

    //单号
    model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_payno' "));
    QString danhao = model.record(0).value(0).toString();
    model.clear();
    model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_payno_len' "));
    int danlen = model.record(0).value(0).toInt();
    model.clear();
    model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_payno_row' "));
    int danrow = model.record(0).value(0).toInt();

    //工号
    model.clear();
    model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_operid' "));
    QString gonghao = model.record(0).value(0).toString();
    model.clear();
    model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_operid_len' "));
    int gonglen = model.record(0).value(0).toInt();
    model.clear();
    model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_operid_row' "));
    int gongrow = model.record(0).value(0).toInt();

    //餐桌
    model.clear();
    model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_table' "));
    QString table = model.record(0).value(0).toString();
    model.clear();
    model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_table_len' "));
    int tablelen = model.record(0).value(0).toInt();
    model.clear();
    model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_table_row' "));
    int tablerow = model.record(0).value(0).toInt();

    //人数
    model.clear();
    model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_person' "));
    QString renshu = model.record(0).value(0).toString();
    model.clear();
    model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_person_len' "));
    int renshulen = model.record(0).value(0).toInt();
    model.clear();
    model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_person_row' "));
    int renshurow = model.record(0).value(0).toInt();

    //时间
    model.clear();
    model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_time' "));
    QString timer = model.record(0).value(0).toString();
    model.clear();
    model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_time_len' "));
    int timerlen = model.record(0).value(0).toInt();
    model.clear();
    model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_time_row' "));
    int timerrow = model.record(0).value(0).toInt();

    //外卖地址
    model.clear();
    model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_address' "));
    QString address = model.record(0).value(0).toString();
    model.clear();
    model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_address_len' "));
    int addressrlen = model.record(0).value(0).toInt();
    model.clear();
    model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_address_row' "));
    int addressrow = model.record(0).value(0).toInt();

    //外卖人
    model.clear();
    model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_linkman' "));
    QString waimairen = model.record(0).value(0).toString();
    model.clear();
    model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_linkman_len' "));
    int waimairenlen = model.record(0).value(0).toInt();
    model.clear();
    model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_linkman_row' "));
    int waimairenrow = model.record(0).value(0).toInt();

    //外卖电话
    model.clear();
    model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_tel' "));
    QString tel = model.record(0).value(0).toString();
    model.clear();
    model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_tel_len' "));
    int tellen = model.record(0).value(0).toInt();
    model.clear();
    model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_tel_row' "));
    int telrow = model.record(0).value(0).toInt();

    //服务员
    model.clear();
    model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_waiter' "));
    QString waiter = model.record(0).value(0).toString();
    model.clear();
    model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_waiter_len' "));
    int waiterlen = model.record(0).value(0).toInt();
    model.clear();
    model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_waiter_row' "));
    int waiterrow = model.record(0).value(0).toInt();

    //服务员名称
    model.clear();
    model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_waitername' "));
    QString wname = model.record(0).value(0).toString();
    model.clear();
    model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_waitername_len' "));
    int wnamelen = model.record(0).value(0).toInt();
    model.clear();
    model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='bp_waitername_row' "));
    int wnamerow = model.record(0).value(0).toInt();

    //流水号
    model.clear();
    model.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='serial_no' "));
    //QString liushui = model.record(0).value(0).toString();
    QString wdanhao;

    model.setQuery("Select NOW()");
    QString time = model.record(0).value(0).toDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QDateTime ldt_datetime;
    ldt_datetime.setTime(model.record(0).value(0).toTime());
    ldt_datetime.setDate(model.record(0).value(0).toDate());

    QString liushi = "P" + ldt_datetime.toString("yyMMdd");
    model.clear();
    model.setQuery(QString("select Max(ch_payno) from cey_u_checkout_master where Left(ch_payno,7) = '%1' ").arg(liushi));
    QString liushuihao = model.record(0).value(0).toString().trimmed();
    if(liushuihao.trimmed().isEmpty())
    {
        liushuihao = liushi + QString().sprintf("%05d",1);
        wdanhao = liushuihao;
    }
    else
    {
        QString str = liushuihao.right(liushuihao.length() - 7);
        int aa = str.toInt() + 1;
        liushuihao = liushi + QString().sprintf("%05d",aa);
        wdanhao = liushuihao;
    }

    QString shifoudayi = configIniRead->value("system_setting/maidanzhangdan").toString();
    //shifoudayi = "1";
    if(shifoudayi == "1")
    {
        QByteArray content;//标题内容
        //第一行=====================================================================================
        QByteArray content1;
        //单号
        if(danhao == "1")
        {
            QString str = tr("付款号:") + wdanhao;
            if(str.length() >= danlen)
            {
                str = str.left(danlen);
            }
            else
            {
                int len = danlen - str.length();
                for(int i = 0; i < len; i++)
                {
                    str.append(" ");
                }
            }

            if(danrow == 1)
            {
                content1.append(str);
            }
        }

        //工号
        if(gonghao == "1")
        {
            QString str = "\n"+tr("工号:") + n_func::gs_operid;
            if(str.length() >= gonglen)
            {
                str = str.left(gonglen);
            }
            else
            {
                int len = gonglen - str.length();
                for(int i = 0; i <  len; i++)
                {
                    str.append(" ");
                }
            }

            if(gongrow == 1)
            {
                content1.append(str);
            }
        }

        //餐桌
        if(table == "1")
        {
            QString str = tr("餐桌:Q000");
            if(str.length() >= tablelen)
            {
                str = str.left(tablelen);
            }
            else
            {
                int len = tablelen - str.length();
                for(int i = 0; i <  len; i++)
                {
                    str.append(" ");
                }
            }

            if(tablerow == 1)
            {
                content1.append(str);
            }
        }

        //人数
        if(renshu == "1")
        {
            QString str = tr("人数:1");
            if(str.length() >= renshulen)
            {
                str = str.left(renshulen);
            }
            else
            {
                int len = renshulen - str.length();
                for(int i = 0; i <  len; i++)
                {
                    str.append(" ");
                }
            }

            if(renshurow == 1)
            {
                content1.append(str);
            }
        }

        //时间
        if(timer == "1")
        {
            QString str = tr("时间:") + time;
            if(str.length() >= timerlen)
            {
                str = str.left(timerlen);
            }
            else
            {
                int len = timerlen - str.length();
                for(int i = 0; i <  len; i++)
                {
                    str.append(" ");
                }
            }

            if(timerrow == 1)
            {
                content1.append(str);
            }
        }

        //外卖地址
        if(address == "1")
        {
            QString str = tr("外卖地址:") + take_cashier::model_top->record(take_cashier::row).value(10).toString();
            if(str.length() >= addressrlen)
            {
                str = str.left(addressrlen);
            }
            else
            {
                int len = addressrlen - str.length();
                for(int i = 0; i <  len; i++)
                {
                    str.append(" ");
                }
            }

            if(addressrow == 1)
            {
                content1.append(str);
            }
        }

        //外卖人
        if(waimairen == "1")
        {
            QString str = tr("外卖人:") + take_cashier::model_top->record(take_cashier::row).value(9).toString();
            if(str.length() >= waimairenlen)
            {
                str = str.left(waimairenlen);
            }
            else
            {
                int len = waimairenlen - str.length();
                for(int i = 0; i <  len; i++)
                {
                    str.append(" ");
                }
            }

            if(waimairenrow == 1)
            {
                content1.append(str);
            }
        }

        //电话
        if(tel == "1")
        {
            QString str = tr("电话:") + take_cashier::model_top->record(take_cashier::row).value(11).toString();
            if(str.length() >= tellen)
            {
                str = str.left(tellen);
            }
            else
            {
                int len = tellen - str.length();
                for(int i = 0; i <  len; i++)
                {
                    str.append(" ");
                }
            }

            if(telrow == 1)
            {
                content1.append(str);
            }
        }

        //服务员

        if(waiter == "1")
        {
            //            QSqlQuery model;
            //            model.prepare(QString("select vch_operID from cey_sys_operator where vch_operator = :name "));
            //            model.bindValue(":name",take_cashier::model_top->record(take_cashier::row).value(11).toString().trimmed());
            //            model.exec();
            //            QString aa;
            //            while(model.next())
            //            {
            //                aa = model.value(0).toString();
            //            }
            QString aa = take_cashier::model_top->record(take_cashier::row).value(13).toString().trimmed();
            QString str = tr("服务员:") + aa;
            if(str.length() >= waiterlen)
            {
                str = str.left(waiterlen);
            }
            else
            {
                int len = waiterlen - str.length();
                for(int i = 0; i <  len; i++)
                {
                    str.append(" ");
                }
            }

            if(waiterrow == 1)
            {
                content1.append(str);
            }
        }

        //服务员名称
        if(wname == "1")
        {
            model.clear();
            model.setQuery(QString("select vch_operator from cey_sys_operator where vch_operID = '%1' ").arg(take_cashier::model_top->record(take_cashier::row).value(13).toString()));
            QString str = tr("服务员名称:") + model.record(0).value(0).toString();
            if(str.length() >= wnamelen)
            {
                str = str.left(wnamelen);
            }
            else
            {
                int len = wnamelen - str.length();
                for(int i = 0; i <  len; i++)
                {
                    str.append(" ");
                }
            }

            if(wnamerow == 1)
            {
                content1.append(str);
            }
        }

        if(!content1.isEmpty())
        {
            content1.append("\n");
            content.append(content1);
        }

        //第二行=====================================================================================
        QByteArray content2;
        //单号
        if(danhao == "1")
        {
            QString str = tr("付款号:") + wdanhao;
            if(str.length() >= danlen)
            {
                str = str.left(danlen);
            }
            else
            {
                int len = danlen - str.length();
                for(int i = 0; i < len; i++)
                {
                    str.append(" ");
                }
            }

            if(danrow == 2)
            {
                content2.append(str);
            }
        }

        //工号
        if(gonghao == "1")
        {
            QString str = "\n"+tr("工号:") + n_func::gs_operid;
            if(str.length() >= gonglen)
            {
                str = str.left(gonglen);
            }
            else
            {
                int len = gonglen - str.length();
                for(int i = 0; i <  len; i++)
                {
                    str.append(" ");
                }
            }

            if(gongrow == 2)
            {
                content2.append(str);
            }
        }

        //餐桌
        if(table == "1")
        {
            QString str = tr("餐桌:Q000");
            if(str.length() >= tablelen)
            {
                str = str.left(tablelen);
            }
            else
            {
                int len = tablelen - str.length();
                for(int i = 0; i <  len; i++)
                {
                    str.append(" ");
                }
            }

            if(tablerow == 2)
            {
                content2.append(str);
            }
        }

        //人数
        if(renshu == "1")
        {
            QString str = tr("人数:1");
            if(str.length() >= renshulen)
            {
                str = str.left(renshulen);
            }
            else
            {
                int len = renshulen - str.length();
                for(int i = 0; i <  len; i++)
                {
                    str.append(" ");
                }
            }

            if(renshurow == 2)
            {
                content2.append(str);
            }
        }

        //时间
        if(timer == "1")
        {
            QString str = tr("时间:") + time;
            if(str.length() >= timerlen)
            {
                str = str.left(timerlen);
            }
            else
            {
                int len = timerlen - str.length();
                for(int i = 0; i <  len; i++)
                {
                    str.append(" ");
                }
            }

            if(timerrow == 2)
            {
                content2.append(str);
            }
        }

        //外卖地址
        if(address == "1")
        {
            QString str = tr("外卖地址:") + take_cashier::model_top->record(take_cashier::row).value(10).toString();
            if(str.length() >= addressrlen)
            {
                str = str.left(addressrlen);
            }
            else
            {
                int len = addressrlen - str.length();
                for(int i = 0; i <  len; i++)
                {
                    str.append(" ");
                }
            }

            if(addressrow == 2)
            {
                content2.append(str);
            }
        }

        //外卖人
        if(waimairen == "1")
        {
            QString str = tr("外卖人:") + take_cashier::model_top->record(take_cashier::row).value(9).toString();
            if(str.length() >= waimairenlen)
            {
                str = str.left(waimairenlen);
            }
            else
            {
                int len = waimairenlen - str.length();
                for(int i = 0; i <  len; i++)
                {
                    str.append(" ");
                }
            }

            if(waimairenrow == 2)
            {
                content2.append(str);
            }
        }

        //电话
        if(tel == "1")
        {
            QString str = tr("电话:") + take_cashier::model_top->record(take_cashier::row).value(11).toString();
            if(str.length() >= tellen)
            {
                str = str.left(tellen);
            }
            else
            {
                int len = tellen - str.length();
                for(int i = 0; i <  len; i++)
                {
                    str.append(" ");
                }
            }

            if(telrow == 2)
            {
                content2.append(str);
            }
        }

        //服务员

        if(waiter == "1")
        {
            QSqlQuery model;
            model.prepare(QString("select vch_operID from cey_sys_operator where vch_operator = :name "));
            model.bindValue(":name",take_cashier::model_top->record(take_cashier::row).value(11).toString().trimmed());
            model.exec();
            QString aa;
            while(model.next())
            {
                aa = model.value(0).toString();
            }
            QString str = tr("服务员:") + aa;
            if(str.length() >= waiterlen)
            {
                str = str.left(waiterlen);
            }
            else
            {
                int len = waiterlen - str.length();
                for(int i = 0; i <  len; i++)
                {
                    str.append(" ");
                }
            }

            if(waiterrow == 2)
            {
                content2.append(str);
            }
        }

        //服务员名称
        if(wname == "1")
        {
            QString str = tr("服务员名称:") + take_cashier::model_top->record(take_cashier::row).value(11).toString();
            if(str.length() >= wnamelen)
            {
                str = str.left(wnamelen);
            }
            else
            {
                int len = wnamelen - str.length();
                for(int i = 0; i <  len; i++)
                {
                    str.append(" ");
                }
            }

            if(wnamerow == 2)
            {
                content2.append(str);
            }
        }

        if(!content2.isEmpty())
        {
            content2.append("\n");
            content.append(content2);
        }

        //第3行=====================================================================================
        QByteArray content3;
        //单号
        if(danhao == "1")
        {
            QString str = tr("付款号:") + wdanhao;
            if(str.length() >= danlen)
            {
                str = str.left(danlen);
            }
            else
            {
                int len = danlen - str.length();
                for(int i = 0; i < len; i++)
                {
                    str.append(" ");
                }
            }

            if(danrow == 3)
            {
                content3.append(str);
            }
        }

        //工号
        if(gonghao == "1")
        {
            QString str = "\n"+tr("工号:") + n_func::gs_operid;
            if(str.length() >= gonglen)
            {
                str = str.left(gonglen);
            }
            else
            {
                int len = gonglen - str.length();
                for(int i = 0; i <  len; i++)
                {
                    str.append(" ");
                }
            }

            if(gongrow == 3)
            {
                content3.append(str);
            }
        }

        //餐桌
        if(table == "1")
        {
            QString str = tr("餐桌:Q000");
            if(str.length() >= tablelen)
            {
                str = str.left(tablelen);
            }
            else
            {
                int len = tablelen - str.length();
                for(int i = 0; i <  len; i++)
                {
                    str.append(" ");
                }
            }

            if(tablerow == 3)
            {
                content3.append(str);
            }
        }

        //人数
        if(renshu == "1")
        {
            QString str = tr("人数:1");
            if(str.length() >= renshulen)
            {
                str = str.left(renshulen);
            }
            else
            {
                int len = renshulen - str.length();
                for(int i = 0; i <  len; i++)
                {
                    str.append(" ");
                }
            }

            if(renshurow == 3)
            {
                content3.append(str);
            }
        }

        //时间
        if(timer == "1")
        {
            QString str = tr("时间:") + time;
            if(str.length() >= timerlen)
            {
                str = str.left(timerlen);
            }
            else
            {
                int len = timerlen - str.length();
                for(int i = 0; i <  len; i++)
                {
                    str.append(" ");
                }
            }

            if(timerrow == 3)
            {
                content3.append(str);
            }
        }

        //外卖地址
        if(address == "1")
        {
            QString str = tr("外卖地址:") + take_cashier::model_top->record(take_cashier::row).value(10).toString();
            if(str.length() >= addressrlen)
            {
                str = str.left(addressrlen);
            }
            else
            {
                int len = addressrlen - str.length();
                for(int i = 0; i <  len; i++)
                {
                    str.append(" ");
                }
            }

            if(addressrow == 3)
            {
                content3.append(str);
            }
        }

        //外卖人
        if(waimairen == "1")
        {
            QString str = tr("外卖人:") + take_cashier::model_top->record(take_cashier::row).value(9).toString();
            if(str.length() >= waimairenlen)
            {
                str = str.left(waimairenlen);
            }
            else
            {
                int len = waimairenlen - str.length();
                for(int i = 0; i <  len; i++)
                {
                    str.append(" ");
                }
            }

            if(waimairenrow == 3)
            {
                content3.append(str);
            }
        }

        //电话
        if(tel == "1")
        {
            QString str = tr("电话:") + take_cashier::model_top->record(take_cashier::row).value(11).toString();
            if(str.length() >= tellen)
            {
                str = str.left(tellen);
            }
            else
            {
                int len = tellen - str.length();
                for(int i = 0; i <  len; i++)
                {
                    str.append(" ");
                }
            }

            if(telrow == 3)
            {
                content3.append(str);
            }
        }

        //服务员

        if(waiter == "1")
        {
            QSqlQuery model;
            model.prepare(QString("select vch_operID from cey_sys_operator where vch_operator = :name "));
            model.bindValue(":name",take_cashier::model_top->record(take_cashier::row).value(11).toString().trimmed());
            model.exec();
            QString aa;
            while(model.next())
            {
                aa = model.value(0).toString();
            }
            QString str = tr("服务员:") + aa;
            if(str.length() >= waiterlen)
            {
                str = str.left(waiterlen);
            }
            else
            {
                int len = waiterlen - str.length();
                for(int i = 0; i <  len; i++)
                {
                    str.append(" ");
                }
            }

            if(waiterrow == 3)
            {
                content3.append(str);
            }
        }

        //服务员名称
        if(wname == "1")
        {
            QString str = tr("服务员名称:") + take_cashier::model_top->record(take_cashier::row).value(11).toString();
            if(str.length() >= wnamelen)
            {
                str = str.left(wnamelen);
            }
            else
            {
                int len = wnamelen - str.length();
                for(int i = 0; i <  len; i++)
                {
                    str.append(" ");
                }
            }

            if(wnamerow == 3)
            {
                content3.append(str);
            }
        }

        if(!content3.isEmpty())
        {
            content3.append("\n");
            content.append(content3);
        }

        //第4行=====================================================================================
        QByteArray content4;
        //单号
        if(danhao == "1")
        {
            QString str = tr("付款号:") + wdanhao;
            if(str.length() >= danlen)
            {
                str = str.left(danlen);
            }
            else
            {
                int len = danlen - str.length();
                for(int i = 0; i < len; i++)
                {
                    str.append(" ");
                }
            }

            if(danrow == 4)
            {
                content4.append(str);
            }
        }

        //工号
        if(gonghao == "1")
        {
            QString str = "\n"+tr("工号:") + n_func::gs_operid;
            if(str.length() >= gonglen)
            {
                str = str.left(gonglen);
            }
            else
            {
                int len = gonglen - str.length();
                for(int i = 0; i <  len; i++)
                {
                    str.append(" ");
                }
            }

            if(gongrow == 4)
            {
                content4.append(str);
            }
        }

        //餐桌
        if(table == "1")
        {
            QString str = tr("餐桌:Q000");
            if(str.length() >= tablelen)
            {
                str = str.left(tablelen);
            }
            else
            {
                int len = tablelen - str.length();
                for(int i = 0; i <  len; i++)
                {
                    str.append(" ");
                }
            }

            if(tablerow == 4)
            {
                content4.append(str);
            }
        }

        //人数
        if(renshu == "1")
        {
            QString str = tr("人数:1");
            if(str.length() >= renshulen)
            {
                str = str.left(renshulen);
            }
            else
            {
                int len = renshulen - str.length();
                for(int i = 0; i <  len; i++)
                {
                    str.append(" ");
                }
            }

            if(renshurow == 4)
            {
                content4.append(str);
            }
        }

        //时间
        if(timer == "1")
        {
            QString str = tr("时间:") + time;
            if(str.length() >= timerlen)
            {
                str = str.left(timerlen);
            }
            else
            {
                int len = timerlen - str.length();
                for(int i = 0; i <  len; i++)
                {
                    str.append(" ");
                }
            }

            if(timerrow == 4)
            {
                content4.append(str);
            }
        }

        //外卖地址
        if(address == "1")
        {
            QString str = tr("外卖地址:") + take_cashier::model_top->record(take_cashier::row).value(10).toString();
            if(str.length() >= addressrlen)
            {
                str = str.left(addressrlen);
            }
            else
            {
                int len = addressrlen - str.length();
                for(int i = 0; i <  len; i++)
                {
                    str.append(" ");
                }
            }

            if(addressrow == 4)
            {
                content4.append(str);
            }
        }

        //外卖人
        if(waimairen == "1")
        {
            QString str = tr("外卖人:") + take_cashier::model_top->record(take_cashier::row).value(9).toString();
            if(str.length() >= waimairenlen)
            {
                str = str.left(waimairenlen);
            }
            else
            {
                int len = waimairenlen - str.length();
                for(int i = 0; i <  len; i++)
                {
                    str.append(" ");
                }
            }

            if(waimairenrow == 4)
            {
                content4.append(str);
            }
        }

        //电话
        if(tel == "1")
        {
            QString str = tr("电话:") + take_cashier::model_top->record(take_cashier::row).value(11).toString();
            if(str.length() >= tellen)
            {
                str = str.left(tellen);
            }
            else
            {
                int len = tellen - str.length();
                for(int i = 0; i <  len; i++)
                {
                    str.append(" ");
                }
            }

            if(telrow == 4)
            {
                content4.append(str);
            }
        }

        //服务员

        if(waiter == "1")
        {
            QSqlQuery model;
            model.prepare(QString("select vch_operID from cey_sys_operator where vch_operator = :name "));
            model.bindValue(":name",take_cashier::model_top->record(take_cashier::row).value(11).toString().trimmed());
            model.exec();
            QString aa;
            while(model.next())
            {
                aa = model.value(0).toString();
            }
            QString str = tr("服务员:") + aa;
            if(str.length() >= waiterlen)
            {
                str = str.left(waiterlen);
            }
            else
            {
                int len = waiterlen - str.length();
                for(int i = 0; i <  len; i++)
                {
                    str.append(" ");
                }
            }

            if(waiterrow == 4)
            {
                content4.append(str);
            }
        }

        //服务员名称
        if(wname == "1")
        {
            QString str = tr("服务员名称:") + take_cashier::model_top->record(take_cashier::row).value(11).toString();
            if(str.length() >= wnamelen)
            {
                str = str.left(wnamelen);
            }
            else
            {
                int len = wnamelen - str.length();
                for(int i = 0; i <  len; i++)
                {
                    str.append(" ");
                }
            }

            if(wnamerow == 4)
            {
                content4.append(str);
            }
        }

        if(!content4.isEmpty())
        {
            content4.append("\n");
            content.append(content4);
        }

        //第5行=====================================================================================
        QByteArray content5;
        //单号
        if(danhao == "1")
        {
            QString str = tr("付款号:") + wdanhao;
            if(str.length() >= danlen)
            {
                str = str.left(danlen);
            }
            else
            {
                int len = danlen - str.length();
                for(int i = 0; i < len; i++)
                {
                    str.append(" ");
                }
            }

            if(danrow == 5)
            {
                content5.append(str);
            }
        }

        //工号
        if(gonghao == "1")
        {
            QString str = "\n"+tr("工号:") + n_func::gs_operid;
            if(str.length() >= gonglen)
            {
                str = str.left(gonglen);
            }
            else
            {
                int len = gonglen - str.length();
                for(int i = 0; i <  len; i++)
                {
                    str.append(" ");
                }
            }

            if(gongrow == 5)
            {
                content5.append(str);
            }
        }

        //餐桌
        if(table == "1")
        {
            QString str = tr("餐桌:Q000");
            if(str.length() >= tablelen)
            {
                str = str.left(tablelen);
            }
            else
            {
                int len = tablelen - str.length();
                for(int i = 0; i <  len; i++)
                {
                    str.append(" ");
                }
            }

            if(tablerow == 5)
            {
                content5.append(str);
            }
        }

        //人数
        if(renshu == "1")
        {
            QString str = tr("人数:1");
            if(str.length() >= renshulen)
            {
                str = str.left(renshulen);
            }
            else
            {
                int len = renshulen - str.length();
                for(int i = 0; i <  len; i++)
                {
                    str.append(" ");
                }
            }

            if(renshurow == 5)
            {
                content5.append(str);
            }
        }

        //时间
        if(timer == "1")
        {
            QString str = tr("时间:") + time;
            if(str.length() >= timerlen)
            {
                str = str.left(timerlen);
            }
            else
            {
                int len = timerlen - str.length();
                for(int i = 0; i <  len; i++)
                {
                    str.append(" ");
                }
            }

            if(timerrow == 5)
            {
                content5.append(str);
            }
        }

        //外卖地址
        if(address == "1")
        {
            QString str = tr("外卖地址:") + take_cashier::model_top->record(take_cashier::row).value(10).toString();
            if(str.length() >= addressrlen)
            {
                str = str.left(addressrlen);
            }
            else
            {
                int len = addressrlen - str.length();
                for(int i = 0; i <  len; i++)
                {
                    str.append(" ");
                }
            }

            if(addressrow == 5)
            {
                content5.append(str);
            }
        }

        //外卖人
        if(waimairen == "1")
        {
            QString str = tr("外卖人:") + take_cashier::model_top->record(take_cashier::row).value(9).toString();
            if(str.length() >= waimairenlen)
            {
                str = str.left(waimairenlen);
            }
            else
            {
                int len = waimairenlen - str.length();
                for(int i = 0; i <  len; i++)
                {
                    str.append(" ");
                }
            }

            if(waimairenrow == 5)
            {
                content5.append(str);
            }
        }

        //电话
        if(tel == "1")
        {
            QString str = tr("电话:") + take_cashier::model_top->record(take_cashier::row).value(11).toString();
            if(str.length() >= tellen)
            {
                str = str.left(tellen);
            }
            else
            {
                int len = tellen - str.length();
                for(int i = 0; i <  len; i++)
                {
                    str.append(" ");
                }
            }

            if(telrow == 5)
            {
                content5.append(str);
            }
        }

        //服务员

        if(waiter == "1")
        {
            QSqlQuery model;
            model.prepare(QString("select vch_operID from cey_sys_operator where vch_operator = :name "));
            model.bindValue(":name",take_cashier::model_top->record(take_cashier::row).value(11).toString().trimmed());
            model.exec();
            QString aa;
            while(model.next())
            {
                aa = model.value(0).toString();
            }
            QString str = tr("服务员:") + aa;
            if(str.length() >= waiterlen)
            {
                str = str.left(waiterlen);
            }
            else
            {
                int len = waiterlen - str.length();
                for(int i = 0; i <  len; i++)
                {
                    str.append(" ");
                }
            }

            if(waiterrow == 5)
            {
                content5.append(str);
            }
        }

        //服务员名称
        if(wname == "1")
        {
            QString str = tr("服务员名称:") + take_cashier::model_top->record(take_cashier::row).value(11).toString();
            if(str.length() >= wnamelen)
            {
                str = str.left(wnamelen);
            }
            else
            {
                int len = wnamelen - str.length();
                for(int i = 0; i <  len; i++)
                {
                    str.append(" ");
                }
            }

            if(wnamerow == 5)
            {
                content5.append(str);
            }
        }

        if(!content5.isEmpty())
        {
            content5.append("\n");
            content.append(content5);
        }

        //第6行=====================================================================================
        QByteArray content6;
        //单号
        if(danhao == "1")
        {
            QString str = tr("付款号:") + wdanhao;
            if(str.length() >= danlen)
            {
                str = str.left(danlen);
            }
            else
            {
                int len = danlen - str.length();
                for(int i = 0; i < len; i++)
                {
                    str.append(" ");
                }
            }

            if(danrow == 6)
            {
                content6.append(str);
            }
        }

        //工号
        if(gonghao == "1")
        {
            QString str = "\n"+tr("工号:") + n_func::gs_operid;
            if(str.length() >= gonglen)
            {
                str = str.left(gonglen);
            }
            else
            {
                int len = gonglen - str.length();
                for(int i = 0; i <  len; i++)
                {
                    str.append(" ");
                }
            }

            if(gongrow == 6)
            {
                content6.append(str);
            }
        }

        //餐桌
        if(table == "1")
        {
            QString str = tr("餐桌:Q000");
            if(str.length() >= tablelen)
            {
                str = str.left(tablelen);
            }
            else
            {
                int len = tablelen - str.length();
                for(int i = 0; i <  len; i++)
                {
                    str.append(" ");
                }
            }

            if(tablerow == 6)
            {
                content6.append(str);
            }
        }

        //人数
        if(renshu == "1")
        {
            QString str = tr("人数:1");
            if(str.length() >= renshulen)
            {
                str = str.left(renshulen);
            }
            else
            {
                int len = renshulen - str.length();
                for(int i = 0; i <  len; i++)
                {
                    str.append(" ");
                }
            }

            if(renshurow == 6)
            {
                content6.append(str);
            }
        }

        //时间
        if(timer == "1")
        {
            QString str = tr("时间:") + time;
            if(str.length() >= timerlen)
            {
                str = str.left(timerlen);
            }
            else
            {
                int len = timerlen - str.length();
                for(int i = 0; i <  len; i++)
                {
                    str.append(" ");
                }
            }

            if(timerrow == 6)
            {
                content6.append(str);
            }
        }

        //外卖地址
        if(address == "1")
        {
            QString str = tr("外卖地址:") + take_cashier::model_top->record(take_cashier::row).value(10).toString();
            if(str.length() >= addressrlen)
            {
                str = str.left(addressrlen);
            }
            else
            {
                int len = addressrlen - str.length();
                for(int i = 0; i <  len; i++)
                {
                    str.append(" ");
                }
            }

            if(addressrow == 6)
            {
                content6.append(str);
            }
        }

        //外卖人
        if(waimairen == "1")
        {
            QString str = tr("外卖人:") + take_cashier::model_top->record(take_cashier::row).value(9).toString();
            if(str.length() >= waimairenlen)
            {
                str = str.left(waimairenlen);
            }
            else
            {
                int len = waimairenlen - str.length();
                for(int i = 0; i <  len; i++)
                {
                    str.append(" ");
                }
            }

            if(waimairenrow == 6)
            {
                content6.append(str);
            }
        }

        //电话
        if(tel == "1")
        {
            QString str = tr("电话:") + take_cashier::model_top->record(take_cashier::row).value(11).toString();
            if(str.length() >= tellen)
            {
                str = str.left(tellen);
            }
            else
            {
                int len = tellen - str.length();
                for(int i = 0; i <  len; i++)
                {
                    str.append(" ");
                }
            }

            if(telrow == 6)
            {
                content6.append(str);
            }
        }

        //服务员

        if(waiter == "1")
        {
            QSqlQuery model;
            model.prepare(QString("select vch_operID from cey_sys_operator where vch_operator = :name "));
            model.bindValue(":name",take_cashier::model_top->record(take_cashier::row).value(11).toString().trimmed());
            model.exec();
            QString aa;
            while(model.next())
            {
                aa = model.value(0).toString();
            }
            QString str = tr("服务员:") + aa;
            if(str.length() >= waiterlen)
            {
                str = str.left(waiterlen);
            }
            else
            {
                int len = waiterlen - str.length();
                for(int i = 0; i <  len; i++)
                {
                    str.append(" ");
                }
            }

            if(waiterrow == 6)
            {
                content6.append(str);
            }
        }

        //服务员名称
        if(wname == "1")
        {
            QString str = tr("服务员名称:") + take_cashier::model_top->record(take_cashier::row).value(11).toString();
            if(str.length() >= wnamelen)
            {
                str = str.left(wnamelen);
            }
            else
            {
                int len = wnamelen - str.length();
                for(int i = 0; i <  len; i++)
                {
                    str.append(" ");
                }
            }

            if(wnamerow == 6)
            {
                content6.append(str);
            }
        }

        if(!content6.isEmpty())
        {
            content6.append("\n");
            content.append(content6);
        }

        //第7行=====================================================================================
        QByteArray content7;
        //单号
        if(danhao == "1")
        {
            QString str = tr("付款号:") + wdanhao;
            if(str.length() >= danlen)
            {
                str = str.left(danlen);
            }
            else
            {
                int len = danlen - str.length();
                for(int i = 0; i < len; i++)
                {
                    str.append(" ");
                }
            }

            if(danrow == 7)
            {
                content7.append(str);
            }
        }

        //工号
        if(gonghao == "1")
        {
            QString str = "\n"+tr("工号:") + n_func::gs_operid;
            if(str.length() >= gonglen)
            {
                str = str.left(gonglen);
            }
            else
            {
                int len = gonglen - str.length();
                for(int i = 0; i <  len; i++)
                {
                    str.append(" ");
                }
            }

            if(gongrow == 7)
            {
                content7.append(str);
            }
        }

        //餐桌
        if(table == "1")
        {
            QString str = tr("餐桌:Q000");
            if(str.length() >= tablelen)
            {
                str = str.left(tablelen);
            }
            else
            {
                int len = tablelen - str.length();
                for(int i = 0; i <  len; i++)
                {
                    str.append(" ");
                }
            }

            if(tablerow == 7)
            {
                content7.append(str);
            }
        }

        //人数
        if(renshu == "1")
        {
            QString str = tr("人数:1");
            if(str.length() >= renshulen)
            {
                str = str.left(renshulen);
            }
            else
            {
                int len = renshulen - str.length();
                for(int i = 0; i <  len; i++)
                {
                    str.append(" ");
                }
            }

            if(renshurow == 7)
            {
                content7.append(str);
            }
        }

        //时间
        if(timer == "1")
        {
            QString str = tr("时间:") + time;
            if(str.length() >= timerlen)
            {
                str = str.left(timerlen);
            }
            else
            {
                int len = timerlen - str.length();
                for(int i = 0; i <  len; i++)
                {
                    str.append(" ");
                }
            }

            if(timerrow == 7)
            {
                content7.append(str);
            }
        }

        //外卖地址
        if(address == "1")
        {
            QString str = tr("外卖地址:") + take_cashier::model_top->record(take_cashier::row).value(10).toString();
            if(str.length() >= addressrlen)
            {
                str = str.left(addressrlen);
            }
            else
            {
                int len = addressrlen - str.length();
                for(int i = 0; i <  len; i++)
                {
                    str.append(" ");
                }
            }

            if(addressrow == 7)
            {
                content7.append(str);
            }
        }

        //外卖人
        if(waimairen == "1")
        {
            QString str = tr("外卖人:") + take_cashier::model_top->record(take_cashier::row).value(9).toString();
            if(str.length() >= waimairenlen)
            {
                str = str.left(waimairenlen);
            }
            else
            {
                int len = waimairenlen - str.length();
                for(int i = 0; i <  len; i++)
                {
                    str.append(" ");
                }
            }

            if(waimairenrow == 7)
            {
                content7.append(str);
            }
        }

        //电话
        if(tel == "1")
        {
            QString str = tr("电话:") + take_cashier::model_top->record(take_cashier::row).value(11).toString();
            if(str.length() >= tellen)
            {
                str = str.left(tellen);
            }
            else
            {
                int len = tellen - str.length();
                for(int i = 0; i <  len; i++)
                {
                    str.append(" ");
                }
            }

            if(telrow == 7)
            {
                content7.append(str);
            }
        }

        //服务员

        if(waiter == "1")
        {
            QSqlQuery model;
            model.prepare(QString("select vch_operID from cey_sys_operator where vch_operator = :name "));
            model.bindValue(":name",take_cashier::model_top->record(take_cashier::row).value(11).toString().trimmed());
            model.exec();
            QString aa;
            while(model.next())
            {
                aa = model.value(0).toString();
            }
            QString str = tr("服务员:") + aa;
            if(str.length() >= waiterlen)
            {
                str = str.left(waiterlen);
            }
            else
            {
                int len = waiterlen - str.length();
                for(int i = 0; i <  len; i++)
                {
                    str.append(" ");
                }
            }

            if(waiterrow == 7)
            {
                content7.append(str);
            }
        }

        //服务员名称
        if(wname == "1")
        {
            QString str = tr("服务员名称:") + take_cashier::model_top->record(take_cashier::row).value(11).toString();
            if(str.length() >= wnamelen)
            {
                str = str.left(wnamelen);
            }
            else
            {
                int len = wnamelen - str.length();
                for(int i = 0; i <  len; i++)
                {
                    str.append(" ");
                }
            }

            if(wnamerow == 7)
            {
                content7.append(str);
            }
        }

        if(!content7.isEmpty())
        {
            content7.append("\n");
            content.append(content7);
        }
        //打印消费内容

        //表头
        QString shifoumingxi;
        shifoumingxi = configIniRead->value("system_setting/maidanmingxi").toString();
        if(shifoumingxi == "1")
        {
            QString tou = configIniRead->value("system_setting/biaoneirong").toString();
            //QString tou = tr("项目               数量           金额\n");
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
            QByteArray neirong;
            for(int i = 0; i < take_cashier::model_bottom->rowCount(); i++)
            {
                QString hanghao = QString::number(i+1) + tr(".");
                if(hanghao.length() < 3)
                {
                    hanghao.append(" ");
                }
                if(bxuhao=="1")neirong.append(hanghao);
                if(bzensong == "1")
                {
                    if(take_cashier::model_bottom->record(i).value(18).toString() == tr("Y"))
                    {
                        neirong.append(tr("赠)"));
                    }
                }

                if(btejia == "1")
                {
                    if(take_cashier::model_bottom->record(i).value(17).toString() == tr("Y"))
                    {
                        neirong.append(tr("特)"));
                    }
                }

                if(take_cashier::model_bottom->record(i).value(16).toString() == "P")
                {
                    neirong.append(tr("套)"));
                }

                if(take_cashier::model_bottom->record(i).value(16).toString() == tr("Y"))
                {
                    neirong.append(tr("细)"));
                }

                //第一行=========================================================================
                QByteArray neirong1;
                //品名
                if(pinming == "1")
                {
                    QString pin = take_cashier::model_bottom->record(i).value(5).toString();
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
                    QString dan = take_cashier::model_bottom->record(i).value(6).toString();
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
                    QString shu = take_cashier::model_bottom->record(i).value(8).toString();
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
                    QString jia = take_cashier::model_bottom->record(i).value(7).toString();
                    if(jiagexiaoshu == 0)
                    {
                        if(take_cashier::model_bottom->record(i).value(16).toString() == "Y")
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
                        if(take_cashier::model_bottom->record(i).value(16).toString() == "Y")
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
                        if(take_cashier::model_bottom->record(i).value(16).toString() == "Y")
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
                    aa = take_cashier::model_bottom->record(i).value(8).toString().toFloat() * take_cashier::model_bottom->record(i).value(7).toString().toFloat();
                    QString jin = QString().sprintf("%0.2f",aa);

                    if(jinexiaoshu == 0)
                    {
                        if(take_cashier::model_bottom->record(i).value(16).toString() == "Y")
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
                        if(take_cashier::model_bottom->record(i).value(16).toString() == "Y")
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
                        if(take_cashier::model_bottom->record(i).value(16).toString() == "Y")
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
                    float aa = take_cashier::model_bottom->record(i).value(8).toString().toFloat() * take_cashier::model_bottom->record(i).value(7).toString().toFloat() * (100 - take_cashier::model_bottom->record(i).value(9).toInt() )*0.01;
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
                    QString pin = take_cashier::model_bottom->record(i).value(5).toString();
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
                    QString dan = take_cashier::model_bottom->record(i).value(6).toString();
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
                    QString shu = take_cashier::model_bottom->record(i).value(8).toString();
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
                    QString jia = take_cashier::model_bottom->record(i).value(7).toString();
                    if(jiagexiaoshu == 0)
                    {
                        if(take_cashier::model_bottom->record(i).value(16).toString() == "Y")
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
                        if(take_cashier::model_bottom->record(i).value(16).toString() == "Y")
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
                        if(take_cashier::model_bottom->record(i).value(16).toString() == "Y")
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
                    aa = take_cashier::model_bottom->record(i).value(8).toString().toFloat() * take_cashier::model_bottom->record(i).value(7).toString().toFloat();
                    QString jin = QString().sprintf("%0.2f",aa);

                    if(jinexiaoshu == 0)
                    {
                        if(take_cashier::model_bottom->record(i).value(16).toString() == "Y")
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
                        if(take_cashier::model_bottom->record(i).value(16).toString() == "Y")
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
                        if(take_cashier::model_bottom->record(i).value(16).toString() == "Y")
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
                    float aa = take_cashier::model_bottom->record(i).value(8).toString().toFloat() * take_cashier::model_bottom->record(i).value(7).toString().toFloat() * (100 - take_cashier::model_bottom->record(i).value(9).toInt() )*0.01;
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
                    if(!take_cashier::model_bottom->record(i).value(12).toString().isEmpty())
                    {
                        neirong.append(take_cashier::model_bottom->record(i).value(12).toString());
                        neirong.append("\n");
                    }
                }

            }
            content.append(neirong);
            content.append(n_func::gs_print_line+"\n");
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
        // xiangqing = "1";
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
            QString str = tr("消费:") + QString().sprintf("%0.2f",take_cashier::model_top->record(take_cashier::row).value(3).toFloat());
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
            if(take_cashier::model_top->record(take_cashier::row).value(5).toString().toFloat() > 0)
            {
                line_num++;
                QString str = tr("折扣:") + QString().sprintf("%0.2f",take_cashier::model_top->record(take_cashier::row).value(5).toFloat());
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
            if(take_cashier::model_top->record(take_cashier::row).value(6).toString().toFloat() > 0)
            {
                line_num++;
                QString str = tr("赠送:") + QString().sprintf("%0.2f",take_cashier::model_top->record(take_cashier::row).value(6).toFloat());
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
            if(take_cashier::model_top->record(take_cashier::row).value(7).toString().toFloat() != 0)
            {
                line_num++;
                QString str = tr("抹零:") + QString().sprintf("%0.2f",take_cashier::model_top->record(take_cashier::row).value(7).toFloat());
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
            if(take_cashier::model_top->record(take_cashier::row).value(4).toString().toFloat() != 0)
            {
                line_num++;
                QString str = tr("税收:") + QString().sprintf("%0.2f", take_cashier::model_top->record(take_cashier::row).value(4).toFloat());
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
            QString str = tr("应收:") + QString().sprintf("%0.2f",take_cashier::model_top->record(take_cashier::row).value(8).toFloat());
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

        if(content.right(2) != "\n")
        {
            content.append("\n");
        }

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
        //        printer = new public_mainprinter();
        blockIns b;

        //图片打印
        QString logo = configIniRead->value("system_setting/dayinlogo").toString();
        if(logo == "1")
        {
            QString str = configIniRead->value("system_setting/dayinlogoneirong").toString();//图片路径
            b+= backheader::printer->drawLogo("1B40 1B6101", str);//清除缓存 居中 pixmap路径
        }

        // QString liuhuihaoma = tr("流水号:") + liushuihao + "\n";
        // path = "192.168.1.172:9100";
        content.append("\n");
        b+=backheader::printer->drawText("1B40 1B6101 1C5700",title);//居中不放大
        b+=backheader::printer->drawText("1B40,1C5701",liushuihao_no,Qt::AlignLeft);//放大
        b+=backheader::printer->drawText("1B40,1C5700", content, Qt::AlignLeft);

        //打印一维码
        QString yiweima = configIniRead->value("system_setting/yiweima").toString();
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
            QString str = configIniRead->value("system_setting/erweimaneirong").toString();
            b+= backheader::printer->drawQR_Code("", str);//清除缓存 居中 pixmap路径
        }

        b += backheader::printer->drawText("1B6405 1D5601", "");//走纸5x， 切纸
        b=backheader::printer->setPrintCount(b, print_count);
        //是否开钱箱
        if(configIniRead->value("system_setting/qianxiang").toBool()){
            blockIns bins;
            bins.append(pieceIns(configIniRead->value("system_setting/qianxiangzhiling").toString().toLocal8Bit(), ""));
            b+=bins;
        }
        backheader::asyn_dataprint(b);
        //付款打印

        _parent->to_payfor_info(b);
    }
}


void waimai_pay::on_pushButton_21_clicked()//取消
{
    btn_t_f = false;
    this->close();
}

void waimai_pay::on_pushButton_19_clicked()//代金券
{

    QSqlQuery model_type;
    model_type.prepare("select ch_paymodeno from cey_bt_paymode where vch_paymodename = :as_mode");
    model_type.bindValue(":as_mode",tr("代金券"));
    model_type.exec();
    QString paytype;
    while(model_type.next())
    {
        paytype = model_type.value(0).toString();
    }


    float label_14 = 0.00;
    label_14 = ui->label_14->text().toFloat();
    ;
    if(ui->label_14->text().toFloat() == 0)
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("已足额付款无需继续付款!"));
        return;
    }
    QString timer;
    QString title = tr("[") + paytype + tr("]") + tr("代金券");
    bank_card bank(title,ui->label_14->text());
    /*bank.exec();*/backheader::static_dialogexec(bank, tr("代金券"));
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
                ui->lineEdit_9->setText("0.00");
                float l_12 = 0.00;
                l_12 = ui->label_14->text().toFloat() + ui->label_12->text().toFloat();
                ui->label_12->setText(QString().sprintf("%0.2f",l_12));
                ui->label_14->setText("0.00");
            }
            else
            {
                float yue = 0.00;
                yue = ui->label_14->text().toFloat() - bank.shishoujine;
                ui->lineEdit_9->setText(QString().sprintf("%0.2f",yue));
                ui->label_12->setText(QString().sprintf("%0.2f",bank.shishoujine + ui->label_12->text().toFloat()));
                ui->label_14->setText(QString().sprintf("%0.2f",yue));
            }
        }
        else//不是现金付全款
        {
            if(ui->label_14->text().toFloat() <= bank.shishoujine)
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
                yue = ui->label_14->text().toFloat() - bank.shishoujine;
                ui->label_12->setText(QString().sprintf("%0.2f",bank.shishoujine + ui->label_12->text().toFloat()));
                ui->label_14->setText(QString().sprintf("%0.2f",yue));
                ui->lineEdit_9->setText("0.00");
                ui->lineEdit_7->setText("0.00");
            }
        }
        timer = bank.time;
    }
    else
    {
        return;
    }

    int row = table_model->rowCount();
    table_model->setRowCount(row + 1);
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

    table_model->setItem(row,0,new QStandardItem(QString::number(row+1))); //序号
    table_model->setItem(row,1,new QStandardItem(tr("代金券")));
    table_model->setItem(row,2,new QStandardItem(QString().sprintf("%0.2f",t_shifuxianjin)));
    table_model->setItem(row,3,new QStandardItem(QString().sprintf("%0.2f",t_fukuanjie)));
    table_model->setItem(row,4,new QStandardItem(fukuanpingzheng));
    table_model->setItem(row,5,new QStandardItem(paytype));
    table_model->setItem(row,6,new QStandardItem(""));
    table_model->setItem(row,7,new QStandardItem(timer));
    ui->tableView->selectRow(row);

    ui->tableView->setColumnHidden(5,true);
    ui->tableView->setColumnHidden(6,true);
    ui->tableView->setColumnHidden(7,true);
}

void waimai_pay::on_pushButton_20_clicked()//其他付款
{
    float label_14 = 0.00;
    label_14 = ui->label_14->text().toFloat();

    if(ui->label_14->text().toFloat() == 0)
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("已足额付款无需继续付款!"));
        
        return;
    }

    other_pay_view other_pay("2");
    /*other_pay.exec();*/backheader::static_dialogexec(other_pay, tr("其他付款"));
    if(other_pay.btn_t_f == true)
    {
        QString timer;
        QString str = tr("[") + other_pay.payfanshi[other_pay.curindex_id] + tr("]") + other_pay.payname[other_pay.curindex_id];
        bank_card bank(str,ui->label_14->text());
        /*bank.exec();*/backheader::static_dialogexec(bank, "");
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
                    ui->lineEdit_9->setText("0.00");
                    float l_12 = 0.00;
                    l_12 = ui->label_14->text().toFloat() + ui->label_12->text().toFloat();
                    ui->label_12->setText(QString().sprintf("%0.2f",l_12));
                    ui->label_14->setText("0.00");
                }
                else
                {
                    float yue = 0.00;
                    yue = ui->label_14->text().toFloat() - bank.shishoujine;
                    ui->lineEdit_9->setText(QString().sprintf("%0.2f",yue));
                    ui->label_12->setText(QString().sprintf("%0.2f",bank.shishoujine + ui->label_12->text().toFloat()));
                    ui->label_14->setText(QString().sprintf("%0.2f",yue));
                }
            }
            else//不是现金付全款
            {
                if(ui->label_14->text().toFloat() <= bank.shishoujine)
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
                    yue = ui->label_14->text().toFloat() - bank.shishoujine;
                    ui->label_12->setText(QString().sprintf("%0.2f",bank.shishoujine + ui->label_12->text().toFloat()));
                    ui->label_14->setText(QString().sprintf("%0.2f",yue));
                    ui->lineEdit_9->setText("0.00");
                    ui->lineEdit_7->setText("0.00");
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

        table_model->setItem(row,0,new QStandardItem(QString::number(row+1))); //序号
        table_model->setItem(row,1,new QStandardItem(other_pay.payname[other_pay.curindex_id]));
        table_model->setItem(row,2,new QStandardItem(QString().sprintf("%0.2f",t_shifuxianjin)));
        table_model->setItem(row,3,new QStandardItem(QString().sprintf("%0.2f",t_fukuanjie)));
        table_model->setItem(row,4,new QStandardItem(fukuanpingzheng));
        table_model->setItem(row,5,new QStandardItem(paytype));
        table_model->setItem(row,6,new QStandardItem(""));
        table_model->setItem(row,7,new QStandardItem(timer));
        ui->tableView->selectRow(row);
        ui->tableView->setColumnHidden(5,true);
        ui->tableView->setColumnHidden(6,true);
        ui->tableView->setColumnHidden(7,true);
    }

}

void waimai_pay::on_pushButton_clicked()//7
{
    count_dis++;
    ui->lineEdit_9->insert("7");
}

void waimai_pay::on_pushButton_2_clicked()//8
{
    count_dis++;
    ui->lineEdit_9->insert("8");
}

void waimai_pay::on_pushButton_3_clicked()//9
{
    count_dis++;
    ui->lineEdit_9->insert("9");
}

void waimai_pay::on_pushButton_4_clicked()//50
{
    count_dis++;
    ui->lineEdit_9->clear();
    ui->lineEdit_9->insert(tr("50"));
}

void waimai_pay::on_pushButton_6_clicked()//4
{
    count_dis++;
    ui->lineEdit_9->insert("4");
}

void waimai_pay::on_pushButton_8_clicked()//5
{
    count_dis++;
    ui->lineEdit_9->insert("5");
}

void waimai_pay::on_pushButton_5_clicked()//6
{
    count_dis++;
    ui->lineEdit_9->insert("6");
}

void waimai_pay::on_pushButton_7_clicked()//100
{
    count_dis++;
    ui->lineEdit_9->clear();
    ui->lineEdit_9->insert(tr("100"));
}

void waimai_pay::on_pushButton_10_clicked()//1
{
    count_dis++;
    ui->lineEdit_9->insert("1");
}

void waimai_pay::on_pushButton_12_clicked()//2
{
    count_dis++;
    ui->lineEdit_9->insert("2");
}

void waimai_pay::on_pushButton_9_clicked()//3
{
    count_dis++;
    ui->lineEdit_9->insert("3");
}

void waimai_pay::on_pushButton_11_clicked()//d
{
    ui->lineEdit_9->backspace();
}

void waimai_pay::on_pushButton_14_clicked()//0
{
    count_dis++;
    ui->lineEdit_9->insert("0");
}

void waimai_pay::on_pushButton_16_clicked()//.
{
    count_dis++;
    ui->lineEdit_9->insert(tr("."));
}

void waimai_pay::on_pushButton_13_clicked()//买单
{
    if(shifoujiazhang != _waimaihao)
    {
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
    QDateTime ldt_datetime;
    ldt_datetime.setTime(m_billno.record(0).value(0).toTime());
    ldt_datetime.setDate(m_billno.record(0).value(0).toDate());
    QString time = m_billno.record(0).value(0).toDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString currentDate = "Q" + ldt_datetime.toString("yyMMdd");

    m_billno.clear();
    m_billno.setQuery(QString("select Max(ch_billno) from cey_u_master where Left(ch_billno,7) = '%1' ").arg(currentDate));
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

    m_billno.clear();
    QString liushi = "P" + ldt_datetime.toString("yyMMdd");
    m_billno.setQuery(QString("select Max(ch_payno) from cey_u_checkout_master where Left(ch_payno,7) = '%1' ").arg(liushi));
    liushuihao = m_billno.record(0).value(0).toString().trimmed();
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
    QSqlQuery query;
    QSqlDatabase::database().transaction();
    QString zhuohao = "Q000";

    //付款明细
    if(query.exec(QString("insert into cey_u_checkout_master( ch_payno , ch_billno , ch_tableno , num_cost , num_discount , num_present , num_service , num_lowcost , num_blotout , num_run , vch_run_operID , ch_state , vch_operID , dt_operdate , vch_operID2 , dt_operdate2 ,num_rate) values('%1','%2','%3','%4','%5','%6','%7','%8','%9','%10',null,'%11','%12','%13',null,null,'%14')")
                  .arg(liushuihao).arg(danhao).arg(zhuohao).arg(ui->lineEdit->text()).arg(ui->lineEdit_2->text()).arg(ui->lineEdit_3->text())
                  .arg("0").arg("0").arg(ui->lineEdit_5->text()).arg("0").arg("Y").arg(n_func::gs_operid).arg(time).arg(ui->lineEdit_6->text())))
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_u_checkout_master失败!"));
        return;
    }

    //现金付款
    if(ui->label_14->text().toFloat() != 0)
    {
        float aa = 0.00;
        aa = ui->lineEdit_9->text().toFloat() - ui->lineEdit_7->text().toFloat();
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


    for(int i = 0; i < table_model->rowCount(); i++)
    {
        QString pingzheng = table_model->item(i,4)->text();
        if(pingzheng.trimmed().isEmpty())
        {
            pingzheng = "null";
        }

        if(query.exec(QString("insert into cey_u_checkout_detail(ch_payno , ch_paymodeno , num_payamount , num_change , num_realamount , num_face ,vch_operID , dt_operdate ,num_card_remain,vch_voucherno, num_face_Num )values('%1','%2','%3','%4','%5','%6','%7','%8','%9','%10','%11')")
                      .arg(liushuihao).arg(table_model->item(i,5)->text()).arg(table_model->item(i,2)->text()).arg("0.0").arg(table_model->item(i,3)->text())
                      .arg("0.0").arg(n_func::gs_operid).arg(table_model->item(i,7)->text()).arg("0.0").arg(pingzheng).arg("0.0")))
        {

        }
        else
        {
            QSqlDatabase::database().rollback();
            lds_messagebox::warning(this, tr("消息提示"),tr("cey_u_checkout_detail失败!"));
            
            return;
        }
    }

    if(query.exec(QString("update cey_u_table set ch_payno  = '%1' where ch_tableno ='%2' and ch_billno  ='%3'")
                  .arg(liushuihao).arg(zhuohao).arg(_waimaihao)))
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_u_table失败!"));
        
        return;
    }

    query.prepare(QString("update cey_u_orderdish set ch_payno = :liushui where ch_togono = :waimai "));
    query.bindValue(":liushui",liushuihao);
    query.bindValue(":waimai",_waimaihao);
    if(query.exec())
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_u_orderdish失败!"));
        
        return;
    }

    if(query.exec(QString("update cey_u_togo set ch_payno  = '%1' , ch_state ='%2' where ch_togono ='%3'")
                  .arg(liushuihao).arg(tr("Y")).arg(_waimaihao)))
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_u_togo失败!"));
        
        return;
    }

    QString waimai_weixin_info;
    if(!_waimaihao.startsWith("T")
            ||(_waimaihao.startsWith("T")
               &&w_sys_manage_cloudsync::up_weixin_change_takkeoutstate(
                   QString::number(_waimaihao.right(5).toInt()),
                   "21"//交易结束-交易成功
                   )
               )
            ){
        if(_waimaihao.startsWith("T"))waimai_weixin_info=(tr("[微信")+tr("交易结束-交易成功!]"));
        QSqlDatabase::database().commit();
        print();
        init_fast_model();
        shifoujiazhang.clear();
        btn_t_f = true;
        lds_messagebox::warning(this, tr("消息提示"),tr("外卖交易成功") +waimai_weixin_info);
        this->close();
        return;
    }
    lds_messagebox::warning(this, tr("消息提示"),tr("外卖交易失败"));
    QSqlDatabase::database().rollback();
    return;
}

void waimai_pay::on_lineEdit_9_textChanged(const QString &arg1)
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

void waimai_pay::print_lost(const QString &str)
{
    ;
    lds_messagebox::warning(this, tr("消息提示"),str);
    
}
