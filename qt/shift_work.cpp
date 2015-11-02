#include "shift_work.h"
#include "ui_shift_work.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include "n_func.h"
#include <QDebug>
#include "lds_messagebox.h"
#include "lock_screen.h"
#include "fast_order_2.h"
#include <QSettings>
#include <QSqlError>
#include "public_sql.h"
#include "backheader.h"

#include "w_scr_dish_switch_detail.h"
shift_work::shift_work(const QString &str,QWidget *parent) :
    _str(str),QDialog(parent),
    ui(new Ui::shift_work)
{
    ui->setupUi(this);
    //this->setWindowFlags(Qt::FramelessWindowHint|Qt::Dialog);this->setStyleSheet("QDialog{border:2px solid gray}");
    btn_t_f = false;
    inti_control();
    init_table();
    init_data();

    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    ui->checkBox_17->setChecked(conf.value("shift_work/isprint").toBool());
    connect(ui->checkBox_17,SIGNAL(toggled(bool)),this,SLOT(tosaveini()));
    w_scr_dish_switch_detail dialog(this);
    dialog.exec();
}

shift_work::~shift_work()
{
    delete ui;
}

void shift_work::inti_control()
{
    QSqlQueryModel model;
    if(n_func::gs_operid == "0000")
    {
        ui->label_3->setText(tr("system(0000)"));
    }
    else
    {
        model.setQuery(QString("select vch_operator from cey_sys_operator where vch_operID = '%1' ").arg(n_func::gs_operid));
        QString str = model.record(0).value(0).toString().trimmed() + tr("(") + n_func::gs_operid + tr(")");
        ui->label_3->setText(str);
    }

    if(_str == "0000")
    {
        ui->label_5->setText(tr("system(0000)"));
    }
    else
    {
        model.setQuery(QString("select vch_operator from cey_sys_operator where vch_operID = '%1' ").arg(_str));
        QString str = model.record(0).value(0).toString().trimmed() + tr("(") + _str + tr(")");
        ui->label_5->setText(str);
    }


    model.clear();
    model.setQuery(QString("Select max( dt_end)from cey_u_handover_master where vch_operID ='%1'").arg(n_func::gs_operid));
    if(!model.record(0).value(0).toString().isEmpty())
    {
        ui->label_7->setText(model.record(0).value(0).toDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    }
    else
    {
        model.clear();
        model.setQuery(QString("Select Min( dt_operdate)from cey_u_checkout_master where vch_operid ='%1'").arg(n_func::gs_operid));
        ui->label_7->setText(model.record(0).value(0).toDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    }

    model.clear();
    model.setQuery(QString("Select NOW()"));
    ui->label_8->setText(model.record(0).value(0).toDateTime().toString("yyyy-MM-dd hh:mm:ss"));
}

void shift_work::init_table()
{
    QString date1 = ui->label_7->text();
    QString date2 = ui->label_8->text();
    QSqlQueryModel model;
    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);

    model_top = new QStandardItemModel();
    model_rfb = new QStandardItemModel();
    model_top->setHorizontalHeaderLabels(QStringList()<<tr("收款方式")<<tr("收款次数")<<tr("收款金额")<<tr("面额"));
    model_rfb->setHorizontalHeaderLabels(QStringList()<<tr("会员卡操作方式")<<tr("操作次数")<<tr("金额"));
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);ui->tableView->setModel(model_top);
    ui->tableView_2->setEditTriggers(QAbstractItemView::NoEditTriggers);ui->tableView_2->setModel(model_rfb);
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#else
    ui->tableView->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->tableView_2->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
#endif
    model.setQuery(QString("Select a.ch_paymodeno , count( a.ch_paymodeno), sum( a.num_realamount), sum( a.num_face),IFNULL(c.vch_paymodename,a.ch_paymodeno) , IFNULL(c.ch_faceflag,'N') from cey_u_checkout_detail a , cey_u_checkout_master b ,cey_bt_paymode c where (c.ch_paymodeno =a.ch_paymodeno)and( a.ch_payno =b.ch_payno)and(b.ch_state ='Y') and(b.vch_operID ='%1')and(b.dt_operdate >= '%2' and b.dt_operdate <= '%3')Group by a.ch_paymodeno ,c.vch_paymodename , c.ch_faceflag  having sum( a.num_realamount)> 0 or sum( a.num_payamount)> 0 Order by a.ch_paymodeno ,c.vch_paymodename , c.ch_faceflag").arg(n_func::gs_operid).arg(date1).arg(date2));
    for(int i = 0; i < model.rowCount(); i++)
    {
        int row = model_top->rowCount();
        model_top->setRowCount(row + 1);
        model_top->setItem(row,0,new QStandardItem(model.record(i).value(4).toString()));
        model_top->setItem(row,1,new QStandardItem(model.record(i).value(1).toString()));
        model_top->setItem(row,2,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(2).toFloat())));
        model_top->setItem(row,3,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(3).toFloat())));
    }

    model.setQuery(QString("select sum(num_deposit),count(*) from t_m_deposit where ch_deposit_mode='1' and dt_operdate >= '%1' and dt_operdate <= '%2' ") .arg(date1).arg(date2));
    model_rfb->appendRow(QList<QStandardItem *>()<<new QStandardItem("充值")<< new QStandardItem(model.record(0).value(1).toString())<< new QStandardItem(kafue=model.record(0).value(0).toString()));
    model.setQuery(QString("select sum(num_realamount),count(*) from t_m_deposit where ch_deposit_mode='1' and dt_operdate >= '%1' and dt_operdate <= '%2' ") .arg(date1).arg(date2));
    model_rfb->appendRow(QList<QStandardItem *>()<<new QStandardItem("充值实收")<< new QStandardItem(model.record(0).value(1).toString())<< new QStandardItem(kafue_realamount=model.record(0).value(0).toString()));
    model.setQuery(QString("select sum(num_deposit),count(*) from t_m_deposit where ch_deposit_mode='8' and dt_operdate >= '%1' and dt_operdate <= '%2' ") .arg(date1).arg(date2));
    model_rfb->appendRow(QList<QStandardItem *>()<<new QStandardItem("取款")<< new QStandardItem(model.record(0).value(1).toString())<< new QStandardItem(card_fetch=model.record(0).value(0).toString()));
    model.setQuery(QString("select sum(num_deposit),count(*) from t_m_deposit where ch_deposit_mode='5' and dt_operdate >= '%1' and dt_operdate <= '%2' ") .arg(date1).arg(date2));
    model_rfb->appendRow(QList<QStandardItem *>()<<new QStandardItem("退卡")<< new QStandardItem(model.record(0).value(1).toString())<< new QStandardItem(card_back=model.record(0).value(0).toString()));

    if(conf.value("w_sys_manage_cloudsync_basedataset/yun_start",false).toBool()){
        w_sys_manage_cloudsync::member_r_f_b_info mrfb=w_sys_manage_cloudsync::get_yun_member_r_f_b(n_func::gs_operid, date1, date2);
        model_rfb->appendRow(QList<QStandardItem *>()<<new QStandardItem("充值(云)")<< new QStandardItem("")<< new QStandardItem(card_recharge_yun=mrfb.rechargeAmount));
        model_rfb->appendRow(QList<QStandardItem *>()<<new QStandardItem("充值实收(云)")<< new QStandardItem("")<< new QStandardItem(card_recharge_realamount_yun=mrfb.realRechargeAmount));
        model_rfb->appendRow(QList<QStandardItem *>()<<new QStandardItem("取款(云)")<< new QStandardItem("")<< new QStandardItem(card_fetch_yun=mrfb.withdrawalAmount));
        model_rfb->appendRow(QList<QStandardItem *>()<<new QStandardItem("退卡(云)")<< new QStandardItem("")<< new QStandardItem(card_back_yun=mrfb.backCardAmount));
    }
}

void shift_work::init_data()
{
    QString date1 = ui->label_7->text();
    QString date2 = ui->label_8->text();
    QSqlQueryModel model;
    model.setQuery(QString("select count(a.ch_tableno), ifnull(sum(a.int_person), 0)from cey_u_table a , cey_u_checkout_master b where a.ch_payno =b.ch_payno and b.ch_state ='Y' and(b.vch_operID ='%1')and(b.dt_operdate >= '%2' and b.dt_operdate <='%3' )").arg(n_func::gs_operid).arg(date1).arg(date2));
    taishu = model.record(0).value(0).toString();
    renshu = model.record(0).value(1).toString();

    model.clear();
    model.setQuery(QString("Select ifnull(sum(num_cost), 0), ifnull(sum(num_discount), 0), ifnull(sum(num_present), 0), ifnull(sum(num_service), 0), ifnull(sum(num_lowcost), 0), ifnull(sum(num_blotout), 0), ifnull(sum (ifnull( num_free ,0)) , 0 ),ifnull(sum(num_cost), 0 )+ ifnull(sum(num_rate), 0 )- ifnull(sum(num_discount), 0)-ifnull(sum(num_present), 0)+ifnull(sum(num_service), 0)+ ifnull(sum(num_lowcost), 0)-ifnull(sum(num_blotout), 0)-ifnull(sum (ifnull(num_free,0 )) , 0 ),ifnull(sum(num_rate), 0)from cey_u_checkout_master where ch_state ='Y' and(vch_operID ='%1')and(dt_operdate >= '%2' and dt_operdate <= '%3' )").arg(n_func::gs_operid).arg(date1).arg(date2));
    xiaofei = QString().sprintf("%0.2f",model.record(0).value(0).toFloat());
    zhekou = QString().sprintf("%0.2f",model.record(0).value(1).toFloat());
    zengsong = QString().sprintf("%0.2f",model.record(0).value(2).toFloat());
    fuwufei = QString().sprintf("%0.2f",model.record(0).value(3).toFloat());
    dixiao = QString().sprintf("%0.2f",model.record(0).value(4).toFloat());
    moling = QString().sprintf("%0.2f",model.record(0).value(5).toFloat());
    miandan = QString().sprintf("%0.2f",model.record(0).value(6).toFloat());
    yingshou = QString().sprintf("%0.2f",model.record(0).value(7).toFloat());
    shuishou = QString().sprintf("%0.2f",model.record(0).value(8).toFloat());

    //    model.clear();
    //    model.setQuery(QString("Select ifnull(sum(num_deposit), 0) from t_m_deposit where(vch_operID ='%1')and(dt_operdate >= '%2' and dt_operdate <= '%3')").arg(n_func::gs_operid).arg(date1).arg(date2));
    //    kafue = QString().sprintf("%0.2f",model.record(0).value(0).toFloat());//充值金额
    //    qDebug() << model.query().lastQuery();

    //    model.clear();
    //    model.setQuery(QString("Select ifnull(sum(num_realamount), 0) from t_m_deposit where(vch_operID ='%1')and(dt_operdate >= '%2' and dt_operdate <= '%3' )").arg(n_func::gs_operid).arg(date1).arg(date2));
    //    kafue_realamount = QString().sprintf("%0.2f",model.record(0).value(0).toFloat());//充值实收
    //    qDebug() << model.query().lastQuery();
}

void shift_work::init_fast_model()
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

void shift_work::print()
{
    QSettings *configIniRead = new QSettings(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    QByteArray content;
    content.append(tr("交班员:") + ui->label_3->text() + tr("    "));
    QString jiebren = ui->label_5->text();
    if(jiebren == "0")
    {
        jiebren = tr("");
    }
    content.append(tr("接班人:") + jiebren + "\n");
    content.append(tr("开始时间:") + ui->label_7->text() + "\n");
    content.append(tr("交班时间:") + ui->label_8->text() + "\n");
    content.append(tr("备注:") + ui->label_11->text() + "\n");
    QString print_type = configIniRead->value("system_setting/dyjjiekouleixing").toString();
    int width_blank=0;
    if(print_type != "1")//58打印机
    {
        width_blank=10;
        QString taitou = tr("收款方式  收款次数  收款金额  面额\n");
        content.append(taitou);
        for(int i = 0; i < model_top->rowCount(); i++)
        {
            //收款方式
            QString fangshi = model_top->item(i,0)->text();
            if(fangshi.length() < 10)
            {
                int a = 10 - fangshi.length() * 2;
                for(int k = 0; k < a; k++)
                {
                    fangshi.append(" ");
                }
            }

            //收款次数
            QString cishu = model_top->item(i,1)->text();
            if(cishu.length() < 10)
            {
                w_scr_dish::setprintsize(cishu, 10,Qt::AlignLeft);
            }

            //收款金额
            QString jine = QString().sprintf("%0.2f",model_top->item(i,2)->text().toFloat());
            if(jine.length() < 10)
            {
                int a = 10 - jine.length();
                for(int k = 0; k < a; k++)
                {
                    jine.append(" ");
                }
            }

            //收款金额
            QString miane = QString().sprintf("%0.2f",model_top->item(i,3)->text().toFloat());
            if(miane.length() < 10)
            {
                int a = 10 - miane.length();
                for(int k = 0; k < a; k++)
                {
                    miane.append(" ");
                }
            }

            content.append(fangshi);
            content.append(cishu);
            content.append(jine);
            content.append(miane);
            content.append("\n");
        }
        content.append("\n");
        QString str;
        str = taishu;
        if(str.length() < 9)
        {
            int b = 9 - str.length();
            for(int i = 0; i < b;i++)
            {
                str.append(" ");
            }
        }
        QString xtaishu = tr("消费台数:") + str;
        content.append(xtaishu);

        QString xrenshu = tr("消费人数:") + renshu + "\n";
        content.append(xrenshu);

        str = xiaofei;
        if(str.length() < 9)
        {
            int b = 9 - str.length();
            for(int i = 0; i < b;i++)
            {
                str.append(" ");
            }
        }
        QString xiaofeijine = tr("消费金额：") + str + tr("");
        content.append(xiaofeijine);

        QString zhkoujine = tr("折扣金额:") + zhekou + "\n";
        content.append(zhkoujine);

        str = zengsong;
        if(str.length() < 9)
        {
            int b = 9 - str.length();
            for(int i = 0; i < b;i++)
            {
                str.append(" ");
            }
        }
        QString zengsongjine = tr("赠送金额：") + str + tr("");
        content.append(zengsongjine);

        QString fuwu = tr("服务费用:") + fuwufei + "\n";
        content.append(fuwu);

        str = dixiao;
        if(str.length() < 9)
        {
            int b = 9 - str.length();
            for(int i = 0; i < b;i++)
            {
                str.append(" ");
            }
        }
        QString dixiaoshouru = tr("低消收入:") + str + tr("");
        content.append(dixiaoshouru);

        QString mljine = tr("抹零金额:") + moling + "\n";
        content.append(mljine);

        str = miandan;
        if(str.length() < 9)
        {
            int b = 9 - str.length();
            for(int i = 0; i < b;i++)
            {
                str.append(" ");
            }
        }
        QString miandanjine = tr("免单金额：") + str + tr("");
        content.append(miandanjine);

        QString ysjine = tr("应收金额:") + yingshou + "\n";
        content.append(ysjine);

        QString shuishoujine = tr("税收金额:") + shuishou + "\n";
        content.append(shuishoujine);
    }
    else//80打印机
    {
        width_blank=15;
        QString taitou = tr("收款方式    收款次数      收款金额    面额\n");
        content.append(taitou);
        for(int i = 0; i < model_top->rowCount(); i++)
        {
            //收款方式
            QString fangshi = model_top->item(i,0)->text();
            if(fangshi.length() < 13)
            {
                int a = 15 - fangshi.length() * 2;
                for(int k = 0; k < a; k++)
                {
                    fangshi.append(" ");
                }
            }

            //收款次数
            QString cishu = model_top->item(i,1)->text();
            if(cishu.length() < 11)
            {
                w_scr_dish::setprintsize(cishu, 11,Qt::AlignLeft);
            }

            //收款金额
            QString jine = QString().sprintf("%0.2f",model_top->item(i,2)->text().toFloat());
            if(jine.length() < 11)
            {
                int a = 11 - jine.length();
                for(int k = 0; k < a; k++)
                {
                    jine.append(" ");
                }
            }

            //面额
            QString miane = QString().sprintf("%0.2f",model_top->item(i,3)->text().toFloat());
            if(miane.length() < 11)
            {
                int a = 11 - miane.length();
                for(int k = 0; k < a; k++)
                {
                    miane.append(" ");
                }
            }

            content.append(fangshi);
            content.append(cishu);
            content.append(jine);
            content.append(miane);
            content.append("\n");
        }
        content.append("\n");
        QString str;
        str = taishu;
        if(str.length() < 16)
        {
            int b = 16 - str.length();
            for(int i = 0; i < b;i++)
            {
                str.append(" ");
            }
        }
        QString xtaishu = tr("消费台数:") + str;
        content.append(xtaishu);

        QString xrenshu = tr("消费人数:") + renshu + "\n";
        content.append(xrenshu);

        str = xiaofei;
        if(str.length() < 15)
        {
            int b = 15 - str.length();
            for(int i = 0; i < b;i++)
            {
                str.append(" ");
            }
        }
        QString xiaofeijine = tr("消费金额：") + str;
        content.append(xiaofeijine);

        QString zhkoujine = tr("折扣金额:") + zhekou + "\n";
        content.append(zhkoujine);

        str = zengsong;
        if(str.length() < 15)
        {
            int b = 15 - str.length();
            for(int i = 0; i < b;i++)
            {
                str.append(" ");
            }
        }
        QString zengsongjine = tr("赠送金额：") + str;
        content.append(zengsongjine);

        QString fuwu = tr("服务费用:") + fuwufei + "\n";
        content.append(fuwu);

        str = dixiao;
        if(str.length() < 15)
        {
            int b = 15 - str.length();
            for(int i = 0; i < b;i++)
            {
                str.append(" ");
            }
        }
        QString dixiaoshouru = tr("低消收入:") + str;
        content.append(dixiaoshouru);

        QString mljine = tr("抹零金额:") + moling + "\n";
        content.append(mljine);

        str = miandan;
        if(str.length() < 15)
        {
            int b = 15 - str.length();
            for(int i = 0; i < b;i++)
            {
                str.append(" ");
            }
        }
        QString miandanjine = tr("免单金额：") + str;
        content.append(miandanjine);

        QString ysjine = tr("应收金额:") + yingshou + "\n";
        content.append(ysjine);

        QString shuishoujine = tr("税收金额:") + shuishou + "\n";
        content.append(shuishoujine);


    }

    //===========


    if(!kafue.isEmpty()&&kafue.toFloat()!=0)content.append(tr("充值金额：") + kafue+QString(width_blank-4, ' '));
    if(!kafue_realamount.isEmpty()&&kafue_realamount.toFloat()!=0)content.append(tr("充值实收:") + kafue_realamount + "\n");
    if(!card_fetch.isEmpty()&&card_fetch.toFloat()!=0)content.append(tr("取款金额:") +  card_fetch+QString(width_blank, ' '));
    if(!card_back.isEmpty()&&card_back.toFloat()!=0)content.append(tr("退卡金额:") + card_back + "\n");
    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    if(conf.value("w_sys_manage_cloudsync_basedataset/yun_start",false).toBool()){
        if(!card_recharge_yun.isEmpty()&&card_recharge_yun.toFloat()!=0)content.append(tr("云会员充值金额:") + card_recharge_yun+QString(width_blank, ' '));
        if(!card_recharge_realamount_yun.isEmpty()&&card_recharge_realamount_yun.toFloat()!=0)content.append(tr("云会员充值实收:") + card_recharge_realamount_yun+"\n");
        if(!card_fetch_yun.isEmpty()&&card_fetch_yun.toFloat()!=0)content.append(tr("云会员取款金额:") + card_fetch_yun+QString(width_blank, ' '));
        if(!card_back_yun.isEmpty()&&card_back_yun.toFloat()!=0)content.append(tr("云会员退卡金额:") + card_back_yun + "\n");
    }
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
    // path = "192.168.1.172:9100";
    b+= backheader::printer->drawText("1B40 1B6101 1C5701", "交班对账表\n");//清除缓存 居中 1x1x
    b+= backheader::printer->drawText("1B401C5700", content, Qt::AlignLeft);
    b+= backheader::printer->drawText("1B6405 1D5601", "");//走纸5x， 切纸
    b=backheader::printer->setPrintCount(b,1);
    backheader::syn_dataprint(b);
}

void shift_work::on_pushButton_5_clicked()//退出
{
    btn_t_f = false;
    this->close();
}

void shift_work::on_pushButton_4_clicked()//交班
{
    QString date1 = ui->label_7->text();
    QString date2 = ui->label_8->text();
    QSqlQueryModel model;
    QSqlQuery query;
    QSqlDatabase::database().transaction();

    model.setQuery(QString("Select IFNULL( max( int_id), 0)from cey_u_handover_master"));
    QString int_id;
    if(model.record(0).value(0).toString().trimmed().isEmpty())
    {
        int_id = "1";
    }
    else
    {
        int aa = 0;
        aa = model.record(0).value(0).toInt();
        int_id = QString::number(aa + 1);
    }

    query.prepare(QString("INSERT INTO cey_u_handover_master(int_id, vch_operID, dt_begin, dt_end, vch_accept, vch_memo)VALUES (:id,:oper,:date1,:date2,null,null)"));
    query.bindValue(":id",int_id);
    query.bindValue(":oper",n_func::gs_operid);
    query.bindValue(":date1",date1);
    query.bindValue(":date2",date2);
    // qDebug() << QString("INSERT INTO cey_u_handover_master(int_id, vch_operID, dt_begin, dt_end, vch_accept, vch_memo)VALUES (:id,:oper,:date1,:date2,null,null)");
    if(query.exec())
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        qDebug() << query.lastError();  lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_u_handover_master失败!"));
        
        return;
    }

    for(int i = 0; i < model_top->rowCount(); i++)
    {
        query.prepare(QString("INSERT INTO cey_u_handover_detail(int_id, vch_paymode, int_count, num_amount)VALUES (:id,:fkfangshi,:cishu,:jine)"));
        query.bindValue(":id",int_id);;
        query.bindValue(":fkfangshi",model_top->item(i,0)->text());
        query.bindValue(":cishu",model_top->item(i,1)->text());
        query.bindValue(":jine",model_top->item(i,2)->text());
        if(query.exec())
        {

        }
        else
        {
            QString str = tr("插入cey_u_handover_detail") + model_top->item(i,1)->text() + tr("付款方式失败!");
            QSqlDatabase::database().rollback();
            lds_messagebox::warning(this, tr("消息提示"),str);
            
            return;
        }
    }

    query.prepare(QString("INSERT INTO cey_u_handover_total(int_id, int_row, vch_item, num_amount, vch_item2, num_amount2, int_height)VALUES (:id,:idd,:xiaofeitaishu,:tnum,:xiaofeirenshu,:rnum,:height)"));
    query.bindValue(":id",int_id);
    query.bindValue(":idd","1");
    query.bindValue(":xiaofeitaishu",tr("消费台数:"));
    query.bindValue("tnum",taishu);
    query.bindValue(":xiaofeirenshu",tr("消费人数:"));
    query.bindValue(":rnum",renshu);
    query.bindValue(":height",tr("120"));
    if(query.exec())
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        qDebug() << query.lastError();  lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_u_handover_total失败!"));
        
        return;
    }

    query.prepare(QString("INSERT INTO cey_u_handover_total(int_id, int_row, vch_item, num_amount, vch_item2, num_amount2, int_height)VALUES (:id,:idd,:xiaofeitaishu,:tnum,:xiaofeirenshu,:rnum,:height)"));
    query.bindValue(":id",int_id);
    query.bindValue(":idd","2");
    query.bindValue(":xiaofeitaishu",tr("消费金额:"));
    query.bindValue("tnum",xiaofei);
    query.bindValue(":xiaofeirenshu",tr("折扣金额:"));
    query.bindValue(":rnum",zhekou);
    query.bindValue(":height",tr("80"));
    if(query.exec())
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        qDebug() << query.lastError();  lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_u_handover_total失败!"));
        
        return;
    }

    query.prepare(QString("INSERT INTO cey_u_handover_total(int_id, int_row, vch_item, num_amount, vch_item2, num_amount2, int_height)VALUES (:id,:idd,:xiaofeitaishu,:tnum,:xiaofeirenshu,:rnum,:height)"));
    query.bindValue(":id",int_id);
    query.bindValue(":idd","3");
    query.bindValue(":xiaofeitaishu",tr("赠送金额:"));
    query.bindValue("tnum",zengsong);
    query.bindValue(":xiaofeirenshu",tr("服务费用:"));
    query.bindValue(":rnum",fuwufei);
    query.bindValue(":height",tr("80"));
    if(query.exec())
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        qDebug() << query.lastError();  lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_u_handover_total失败!"));
        
        return;
    }

    query.prepare(QString("INSERT INTO cey_u_handover_total(int_id, int_row, vch_item, num_amount, vch_item2, num_amount2, int_height)VALUES (:id,:idd,:xiaofeitaishu,:tnum,:xiaofeirenshu,:rnum,:height)"));
    query.bindValue(":id",int_id);
    query.bindValue(":idd","4");
    query.bindValue(":xiaofeitaishu",tr("低消收入:"));
    query.bindValue("tnum",dixiao);
    query.bindValue(":xiaofeirenshu",tr("抹零金额:"));
    query.bindValue(":rnum",moling);
    query.bindValue(":height",tr("80"));
    if(query.exec())
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        qDebug() << query.lastError();  lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_u_handover_total失败!"));
        
        return;
    }

    query.prepare(QString("INSERT INTO cey_u_handover_total(int_id, int_row, vch_item, num_amount, vch_item2, num_amount2, int_height)VALUES (:id,:idd,:xiaofeitaishu,:tnum,:xiaofeirenshu,:rnum,:height)"));
    query.bindValue(":id",int_id);
    query.bindValue(":idd","5");
    query.bindValue(":xiaofeitaishu",tr("免单金额:"));
    query.bindValue("tnum",miandan);
    query.bindValue(":xiaofeirenshu",tr("应收金额:"));
    query.bindValue(":rnum",yingshou);
    query.bindValue(":height",tr("80"));
    if(query.exec())
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        qDebug() << query.lastError();  lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_u_handover_total失败!"));
        
        return;
    }

    query.prepare(QString("INSERT INTO cey_u_handover_total(int_id, int_row, vch_item, num_amount, vch_item2, num_amount2, int_height)VALUES (:id,:idd,:xiaofeitaishu,:tnum,:xiaofeirenshu,:rnum,:height)"));
    query.bindValue(":id",int_id);
    query.bindValue(":idd","6");
    query.bindValue(":xiaofeitaishu",tr("税收:"));
    query.bindValue("tnum",shuishou);
    query.bindValue(":xiaofeirenshu",tr(""));
    query.bindValue(":rnum",tr("0.00"));
    query.bindValue(":height",tr("80"));
    if(query.exec())
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        qDebug() << query.lastError();  lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_u_handover_total失败!"));
        
        return;
    }

    if(!(kafue.toFloat() == 0 && kajifen.toFloat() == 0 ))
    {
        query.prepare(QString("INSERT INTO cey_u_handover_total(int_id, int_row, vch_item, num_amount, vch_item2, num_amount2, int_height)VALUES (:id,:idd,:xiaofeitaishu,:tnum,:xiaofeirenshu,:rnum,:height)"));
        query.bindValue(":id",int_id);
        query.bindValue(":idd","7");
        query.bindValue(":xiaofeitaishu",tr("充值金额:"));
        query.bindValue("tnum",kajifen);
        query.bindValue(":xiaofeirenshu",tr("充值实收:"));
        query.bindValue(":rnum",kafue_realamount);
        query.bindValue(":height",tr("80"));
        if(query.exec())
        {

        }
        else
        {
            QSqlDatabase::database().rollback();
            qDebug() << query.lastError();  lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_u_handover_total失败!"));
            return;
        }
    }


    //    QString card_fetch;
    if(card_fetch.toFloat() != 0){
        query.prepare(QString("INSERT INTO cey_u_handover_total(int_id, int_row, vch_item, num_amount, vch_item2, num_amount2, int_height)VALUES (:id,:idd,:xiaofeitaishu,:tnum,:xiaofeirenshu,:rnum,:height)"));
        query.bindValue(":id",int_id);
        query.bindValue(":idd","8");
        query.bindValue(":xiaofeitaishu",tr("取款金额:"));
        query.bindValue("tnum",card_fetch);
        query.bindValue(":xiaofeirenshu",tr("取款金额:"));
        query.bindValue(":rnum",card_fetch);
        query.bindValue(":height",tr("80"));
        if(!query.exec()){
            QSqlDatabase::database().rollback();
            qDebug() << query.lastError();  lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_u_handover_total失败!"));
            return;
        }
    }
    //    QString card_back;
    if(card_fetch.toFloat() != 0){
        query.prepare(QString("INSERT INTO cey_u_handover_total(int_id, int_row, vch_item, num_amount, vch_item2, num_amount2, int_height)VALUES (:id,:idd,:xiaofeitaishu,:tnum,:xiaofeirenshu,:rnum,:height)"));
        query.bindValue(":id",int_id);
        query.bindValue(":idd","9");
        query.bindValue(":xiaofeitaishu",tr("退卡金额:"));
        query.bindValue("tnum",card_back);
        query.bindValue(":xiaofeirenshu",tr("退卡金额:"));
        query.bindValue(":rnum",card_back);
        query.bindValue(":height",tr("80"));
        if(!query.exec()){
            QSqlDatabase::database().rollback();
            qDebug() << query.lastError();  lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_u_handover_total失败!"));
            return;
        }
    }
    //    QString card_recharge_yun;
    if(card_fetch.toFloat() != 0){
        query.prepare(QString("INSERT INTO cey_u_handover_total(int_id, int_row, vch_item, num_amount, vch_item2, num_amount2, int_height)VALUES (:id,:idd,:xiaofeitaishu,:tnum,:xiaofeirenshu,:rnum,:height)"));
        query.bindValue(":id",int_id);
        query.bindValue(":idd","10");
        query.bindValue(":xiaofeitaishu",tr("云会员充值:"));
        query.bindValue("tnum",card_recharge_yun);
        query.bindValue(":xiaofeirenshu",tr("云会员充值:"));
        query.bindValue(":rnum",card_recharge_realamount_yun);
        query.bindValue(":height",tr("80"));
        if(!query.exec()){
            QSqlDatabase::database().rollback();
            qDebug() << query.lastError();  lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_u_handover_total失败!"));
            return;
        }
    }
    //    QString card_fetch_yun;
    if(card_fetch.toFloat() != 0){
        query.prepare(QString("INSERT INTO cey_u_handover_total(int_id, int_row, vch_item, num_amount, vch_item2, num_amount2, int_height)VALUES (:id,:idd,:xiaofeitaishu,:tnum,:xiaofeirenshu,:rnum,:height)"));
        query.bindValue(":id",int_id);
        query.bindValue(":idd","11");
        query.bindValue(":xiaofeitaishu",tr("云会员取款:"));
        query.bindValue("tnum",card_fetch_yun);
        query.bindValue(":xiaofeirenshu",tr("云会员取款:"));
        query.bindValue(":rnum",card_fetch_yun);
        query.bindValue(":height",tr("80"));
        if(!query.exec()){
            QSqlDatabase::database().rollback();
            qDebug() << query.lastError();  lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_u_handover_total失败!"));
            return;
        }
    }
    //    QString card_back_yun;
    if(card_fetch.toFloat() != 0){
        query.prepare(QString("INSERT INTO cey_u_handover_total(int_id, int_row, vch_item, num_amount, vch_item2, num_amount2, int_height)VALUES (:id,:idd,:xiaofeitaishu,:tnum,:xiaofeirenshu,:rnum,:height)"));
        query.bindValue(":id",int_id);
        query.bindValue(":idd","12");
        query.bindValue(":xiaofeitaishu",tr("云会员退卡:"));
        query.bindValue("tnum",card_back_yun);
        query.bindValue(":xiaofeirenshu",tr("云会员退卡:"));
        query.bindValue(":rnum",card_back_yun);
        query.bindValue(":height",tr("80"));
        if(!query.exec()){
            QSqlDatabase::database().rollback();
            qDebug() << query.lastError();  lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_u_handover_total失败!"));
            return;
        }
    }
    QSqlDatabase::database().commit();
    if(ui->checkBox_17->isChecked() == true)
    {
        //打印加班单据
        print();
    }
    btn_t_f = true;
    //重新得到fast_order数据库
    init_fast_model();
    this->close();
}

void shift_work::print_lost(const QString &str)
{
    ;
    lds_messagebox::warning(this, tr("消息提示"),str);
    
}

void shift_work::tosaveini()
{
    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    conf.setValue("shift_work/isprint", ui->checkBox_17->isChecked());
}
