#include "consumption_veiw.h"
#include "ui_consumption_veiw.h"
#include <QSqlQueryModel>
#include <QSqlRecord>
#include "lds_tableview.h"
#include "n_func.h"
#include <QSqlError>
#include <QSettings>
#include "lds_messagebox.h"
#include <QDesktopWidget>
#include <QApplication>
#include "public_sql.h"
#include "backheader.h"

consumption_veiw::consumption_veiw(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::consumption_veiw)
{
    ui->setupUi(this);
    //this->setWindowFlags(Qt::FramelessWindowHint|Qt::Dialog);this->setStyleSheet("QDialog{border:2px solid gray}");
    init_control();
}

consumption_veiw::~consumption_veiw()
{
    delete ui;
}

void consumption_veiw::on_pushButton_6_clicked()//退出
{
    this->close();
}

void consumption_veiw::init_control()
{
    QPixmap pixmap(1, 40);
    pixmap.fill(Qt::transparent);
    QIcon icon(pixmap);
    //账单连续打印
//    ui->comboBox->setIconSize(QSize(1, 50));
    ui->comboBox->addItem(/*icon,*/tr("消费情况统计"));
    ui->comboBox->addItem(/*icon,*/tr("营业综合统计"));

//    ui->comboBox_2->setIconSize(QSize(1, 50));
    ui->comboBox_2->addItem(/*icon,*/tr("品项统计"));
    ui->comboBox_2->addItem(/*icon,*/tr("品类统计"));

    QSqlQueryModel model1;
    model1.setQuery("Select NOW()");
    QDateTime time =model1.record(0).value(0).toDateTime();
    QString str = time.toString("yyyy-MM-dd");
    QString str1 = "00:00";
    QString str2 = str + " " + str1;
    QDateTime time_box1;
    time_box1 = QDateTime::fromString(str2, "yyyy-MM-dd hh:mm");
    ui->dateTimeEdit->setDateTime(time_box1);
    ui->dateTimeEdit_2->setDateTime(model1.record(0).value(0).toDateTime());

    ui->tableView->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置属性为只读

    QStringList labels;
    labels<<"项目"<<"数量"<<"金额";
    model_top = new QStandardItemModel();
    model_top->setHorizontalHeaderLabels(labels);
    ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::SelectedClicked);ui->tableView->setModel(model_top);

    model_buttom = new QStandardItemModel();
    ui->tableView_2->setModel(model_buttom);
    //ui->tableView_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void consumption_veiw::on_pushButton_2_clicked()//检索
{
    QSqlQueryModel model;

    ui->label_7->setText(ui->comboBox->currentText());
    ui->label_8->setText(ui->comboBox_2->currentText());
    QString date1 = ui->dateTimeEdit->dateTime().toString("yyyy-MM-dd hh:mm");
    QString date2 = ui->dateTimeEdit_2->dateTime().toString("yyyy-MM-dd hh:mm");
    ui->label_10->setText(date1);
    ui->label_12->setText(date2);
    model.setQuery("Select NOW()");
    QString time = model.record(0).value(0).toDateTime().toString("yyyy-MM-dd hh:mm:ss");
    ui->label_14->setText(time);
    ui->label_15->setText(n_func::gs_operid);
    model_top->clear();
    model_buttom->clear();
    QStringList labels;
    labels<<"项目"<<"数量"<<"金额";
    model_top->setHorizontalHeaderLabels(labels);
    if(ui->comboBox->currentIndex() == 0)
    {
        ui->tableView_2->hide();
        if(ui->comboBox_2->currentIndex() == 0)//按品项统计
        {
            model.clear();
            model.setQuery(QString("SELECT b.vch_dishname,sum(a.num_num - a.num_back),sum(Round(case when a.ch_presentflag = 'Y' then 0 else (a.num_num - a.num_back) * (a.num_price + a.num_price_add)* a.int_discount * 0.01 end,2)), a.ch_dishno FROM cey_u_orderdish a,cey_bt_dish b, cey_u_checkout_master c where a.ch_dishno = b.ch_dishno and a.ch_payno = c.ch_payno  and c.ch_state = 'Y' and left(c.dt_operdate,16) >= '%1' and left(c.dt_operdate,16) <= '%2' Group by b.ch_dish_typeno,a.ch_dishno,b.vch_dishname Order by b.ch_dish_typeno,a.ch_dishno,b.vch_dishname").arg(date1).arg(date2));
            for(int i = 0; i < model.rowCount(); i++)
            {
                int row = model_top->rowCount();
                model_top->setRowCount(row + 1);
                QString xiangmu;
                if(ui->checkBox->isChecked() == true)
                {
                    xiangmu = tr("(") + model.record(i).value(3).toString() + tr(")") + model.record(i).value(0).toString();
                }
                else
                {
                    xiangmu = model.record(i).value(0).toString();
                }
                model_top->setItem(row,0,new QStandardItem(xiangmu));
                float aa = 0.00;
                aa = model.record(i).value(1).toFloat();
                model_top->setItem(row,1,new QStandardItem(QString().sprintf("%0.2f",aa)));
                float bb = 0.00;
                bb = model.record(i).value(2).toFloat();
                model_top->setItem(row,2,new QStandardItem(QString().sprintf("%0.2f",bb)));
            }
            shuliang_num = ui->tableView->updateSum(1);
            zongji_num = ui->tableView->updateSum(2);
        }
        else //按品类统计
        {
            model.clear();
            model.setQuery(QString("SELECT d.vch_dish_typename,sum(a.num_num - a.num_back), sum(Round(case when a.ch_presentflag = 'Y' then 0 else (a.num_num - a.num_back) * (a.num_price + a.num_price_add)* a.int_discount * 0.01 end,2)),b.ch_dish_typeno FROM cey_u_orderdish a,cey_bt_dish b,cey_u_checkout_master c , cey_bt_dish_type d where b.ch_dish_typeno=d.ch_dish_typeno and a.ch_dishno = b.ch_dishno and a.ch_payno = c.ch_payno  and  c.ch_state = 'Y' and  left(c.dt_operdate,16) >= '%1' and left(c.dt_operdate,16) <= '%2' Group by b.ch_dish_typeno,d.vch_dish_typename  Order by b.ch_dish_typeno,d.vch_dish_typename").arg(date1).arg(date2));
            for(int i = 0; i < model.rowCount(); i++)
            {
                int row = model_top->rowCount();
                model_top->setRowCount(row + 1);
                QString xiangmu;
                if(ui->checkBox->isChecked() == true)
                {
                    xiangmu = tr("(") + model.record(i).value(3).toString() + tr(")") + model.record(i).value(0).toString();
                }
                else
                {
                    xiangmu = model.record(i).value(0).toString();
                }
                model_top->setItem(row,0,new QStandardItem(xiangmu));
                float aa = 0.00;
                aa = model.record(i).value(1).toFloat();
                model_top->setItem(row,1,new QStandardItem(QString().sprintf("%0.2f",aa)));
                float bb = 0.00;
                bb = model.record(i).value(2).toFloat();
                model_top->setItem(row,2,new QStandardItem(QString().sprintf("%0.2f",bb)));
            }
            ui->tableView->updateSum(1);
            ui->tableView->updateSum(2);
        }
    }
    else
    {
        QString zongji;
        ui->tableView_2->show();
        if(ui->comboBox_2->currentIndex() == 0)//按品项统计
        {
            model.clear();
            model.setQuery(QString("SELECT b.vch_dishname,sum(a.num_num - a.num_back),sum(Round(case when a.ch_presentflag = 'Y' then 0 else (a.num_num - a.num_back) * (a.num_price + a.num_price_add)* a.int_discount * 0.01 end,2)), a.ch_dishno FROM cey_u_orderdish a,cey_bt_dish b, cey_u_checkout_master c where a.ch_dishno = b.ch_dishno and a.ch_payno = c.ch_payno  and c.ch_state = 'Y' and left(c.dt_operdate,16) >= '%1' and left(c.dt_operdate,16) <= '%2' Group by b.ch_dish_typeno,a.ch_dishno,b.vch_dishname Order by b.ch_dish_typeno,a.ch_dishno,b.vch_dishname").arg(date1).arg(date2));
            for(int i = 0; i < model.rowCount(); i++)
            {
                int row = model_top->rowCount();
                model_top->setRowCount(row + 1);
                QString xiangmu;
                if(ui->checkBox->isChecked() == true)
                {
                    xiangmu = tr("(") + model.record(i).value(3).toString() + tr(")") + model.record(i).value(0).toString();
                }
                else
                {
                    xiangmu = model.record(i).value(0).toString();
                }
                model_top->setItem(row,0,new QStandardItem(xiangmu));
                float aa = 0.00;
                aa = model.record(i).value(1).toFloat();
                model_top->setItem(row,1,new QStandardItem(QString().sprintf("%0.2f",aa)));
                float bb = 0.00;
                bb = model.record(i).value(2).toFloat();
                model_top->setItem(row,2,new QStandardItem(QString().sprintf("%0.2f",bb)));
            }
            shuliang_num = ui->tableView->updateSum(1);
            zongji = ui->tableView->updateSum(2);
            zongji_num = zongji;qDebug()<<shuliang_num << zongji_num;
        }
        else //按品类统计
        {
            model.clear();
            model.setQuery(QString("SELECT d.vch_dish_typename,sum(a.num_num - a.num_back), sum(Round(case when a.ch_presentflag = 'Y' then 0 else (a.num_num - a.num_back) * (a.num_price + a.num_price_add)* a.int_discount * 0.01 end,2)),b.ch_dish_typeno FROM cey_u_orderdish a,cey_bt_dish b,cey_u_checkout_master c , cey_bt_dish_type d where b.ch_dish_typeno=d.ch_dish_typeno and a.ch_dishno = b.ch_dishno and a.ch_payno = c.ch_payno  and  c.ch_state = 'Y' and  left(c.dt_operdate,16) >= '%1' and left(c.dt_operdate,16) <= '%2' Group by b.ch_dish_typeno,d.vch_dish_typename  Order by b.ch_dish_typeno,d.vch_dish_typename").arg(date1).arg(date2));
            for(int i = 0; i < model.rowCount(); i++)
            {
                int row = model_top->rowCount();
                model_top->setRowCount(row + 1);
                QString xiangmu;
                if(ui->checkBox->isChecked() == true)
                {
                    xiangmu = tr("(") + model.record(i).value(3).toString() + tr(")") + model.record(i).value(0).toString();
                }
                else
                {
                    xiangmu = model.record(i).value(0).toString();
                }
                model_top->setItem(row,0,new QStandardItem(xiangmu));
                float aa = 0.00;
                aa = model.record(i).value(1).toFloat();
                model_top->setItem(row,1,new QStandardItem(QString().sprintf("%0.2f",aa)));
                float bb = 0.00;
                bb = model.record(i).value(2).toFloat();
                model_top->setItem(row,2,new QStandardItem(QString().sprintf("%0.2f",bb)));
            }
            ui->tableView->updateSum(1);
            zongji = ui->tableView->updateSum(2);
        }

        //收费明细
        model.clear();
        //台数和人数
        model.setQuery(QString("select count( a.ch_tableno), IFNULL( sum( a.int_person), 0)from cey_u_table a , cey_u_checkout_master b where a.ch_payno =b.ch_payno and b.ch_state ='Y' and b.dt_operdate between '%1' and '%2' ").arg(date1).arg(date2));
        //qDebug() << QString("select count( a.ch_tableno), IFNULL( sum( a.int_person), 0)from cey_u_table a , cey_u_checkout_master b where a.ch_payno =b.ch_payno and b.ch_state ='Y' and b.dt_operdate between '%1' and '%2' ").arg(date1).arg(date2);
        QString taishu;
        taishu = tr("台数:") + model.record(0).value(0).toString();
        QString renshu;
        renshu = tr("人数:") + model.record(0).value(0).toString();
        int row = model_buttom->rowCount();
        model_buttom->setRowCount(row + 1);
        model_buttom->setItem(row,0,new QStandardItem(taishu));
        model_buttom->setItem(row,1,new QStandardItem(renshu));

        QString taijun;
        QString renjun;
        if(model.record(0).value(0).toInt() == 0)
        {
            taijun = tr("台均:0.00");
        }
        else
        {
            float aa = 0.00;
            aa = zongji.toFloat() /model.record(0).value(0).toFloat();
            taijun = tr("台均:") + QString().sprintf("%0.2f",aa);
        }

        if(model.record(0).value(1).toInt() == 0)
        {
            renjun = tr("人均:0.00");
        }
        else
        {
            float aa = 0.00;
            aa = zongji.toFloat() /model.record(0).value(1).toFloat();
            renjun = tr("人均:") + QString().sprintf("%0.2f",aa);
        }

        int row_1 = model_buttom->rowCount();
        model_buttom->setRowCount(row_1 + 1);
        model_buttom->setItem(row_1,0,new QStandardItem(taijun));
        model_buttom->setItem(row_1,1,new QStandardItem(renjun));

        model.clear();
        //model.setQuery(QString("Select IFNULL(sum(num_cost), 0), IFNULL(sum(num_discount), 0), IFNULL(sum(num_present), 0), IFNULL(sum(num_service), 0), IFNULL(sum(num_lowcost), 0), IFNULL(sum (IFNULL( num_blotout,0 )) , 0), IFNULL(sum (IFNULL( num_free ,0)) , 0 ),IFNULL(sum(num_cost), 0 )+ IFNULL(sum(num_rate), 0 )- IFNULL(sum(num_discount), 0)-IFNULL(sum(num_present), 0)+IFNULL(sum(num_service), 0)+ IFNULL(sum(num_lowcost), 0)-IFNULL(sum(ifnull( num_blotout,0 )) , 0)-IFNULL(sum (ifnull( num_free,0 )) , 0 ),IFNULL(sum(num_rate), 0)from cey_u_checkout_master where(dt_operdate between '%1' and '%2')and ch_state ='Y' ").arg(date1).arg(date2));
        //qDebug()<< QString("Select ifnull(sum(num_cost), 0), ifnull(sum(num_discount), 0), ifnull(sum(num_present), 0), ifnull(sum(num_service), 0), ifnull(sum(num_lowcost), 0), ifnull(sum(num_blotout), 0), ifnull(sum(num_free), 0 ),ifnull(sum(num_cost), 0 )+ ifnull(sum(num_rate), 0 )- ifnull(sum(num_discount), 0)-ifnull(sum(num_present), 0)+ifnull(sum(num_service), 0)+ ifnull(sum(num_lowcost), 0)-ifnull(sum(num_blotout), 0)-ifnull(sum(num_free), 0 ),ifnull(sum(num_rate), 0)from cey_u_checkout_master where(dt_operdate between '%1' and '%2')and ch_state ='Y' ").arg(date1).arg(date2);
        model.setQuery(QString("Select IFNULL( sum( num_cost), 0), IFNULL( sum( num_discount), 0), IFNULL( sum( num_present), 0), IFNULL( sum( num_blotout), 0), IFNULL( sum( num_free), 0), IFNULL( sum( num_service), 0), IFNULL( sum( num_lowcost), 0 ),  IFNULL( sum( num_rate), 0)from cey_u_checkout_master where(dt_operdate between '%1' and '%2')and ch_state ='Y' ").arg(date1).arg(date2));

        float xiaofei = 0.00;
        xiaofei = model.record(0).value(0).toFloat();
        QString xiaofei_text = tr("消费:") + QString().sprintf("%0.2f",xiaofei);

        float zhekou = 0.00;
        zhekou = model.record(0).value(1).toFloat();
        QString zhekou_text = tr("折扣:") + QString().sprintf("%0.2f",zhekou);
        int row_2 = model_buttom->rowCount();
        model_buttom->setRowCount(row_2 + 1);
        model_buttom->setItem(row_2,0,new QStandardItem(xiaofei_text));
        model_buttom->setItem(row_2,1,new QStandardItem(zhekou_text));

        float zengson = 0.00;
        zengson = model.record(0).value(2).toFloat();
        QString zengson_text = tr("赠送:") + QString().sprintf("%0.2f",zengson);

        float fuwufei = 0.00;
        fuwufei = model.record(0).value(5).toFloat();
        QString fuwufei_text = tr("服务费:") + QString().sprintf("%0.2f",fuwufei);
        int row_3 = model_buttom->rowCount();
        model_buttom->setRowCount(row_3 + 1);
        model_buttom->setItem(row_3,0,new QStandardItem(zengson_text));
        model_buttom->setItem(row_3,1,new QStandardItem(fuwufei_text));

        float dixiao = 0.00;
        dixiao = model.record(0).value(6).toFloat();
        QString dixiao_text = tr("低消:") + QString().sprintf("%0.2f",dixiao);

        float moling = 0.00;
        moling = model.record(0).value(3).toFloat();
        QString moling_text = tr("抹零:") + QString().sprintf("%0.2f",moling);
        int row_4 = model_buttom->rowCount();
        model_buttom->setRowCount(row_4 + 1);
        model_buttom->setItem(row_4,0,new QStandardItem(dixiao_text));
        model_buttom->setItem(row_4,1,new QStandardItem(moling_text));

        float miandan = 0.00;
        miandan = model.record(0).value(4).toFloat();
        QString miandan_text = tr("免单:") + QString().sprintf("%0.2f",miandan);

        float shuishou = 0.00;
        shuishou = model.record(0).value(7).toFloat();

        float yingshou = 0.00;
        yingshou = xiaofei + shuishou + fuwufei + dixiao - moling - zengson - zhekou -miandan;
       // yingshou = model.record(0).value(7).toFloat();
        QString yingshou_text = tr("应收:") + QString().sprintf("%0.2f",yingshou);
        int row_5 = model_buttom->rowCount();
        model_buttom->setRowCount(row_5 + 1);
        model_buttom->setItem(row_5,0,new QStandardItem(miandan_text));
        model_buttom->setItem(row_5,1,new QStandardItem(yingshou_text));


        QString shuishou_text = tr("税收:") + QString().sprintf("%0.2f",shuishou);
        int row_6 = model_buttom->rowCount();
        model_buttom->setRowCount(row_6 + 1);
        model_buttom->setItem(row_6,0,new QStandardItem(shuishou_text));
        model_buttom->setItem(row_6,1,new QStandardItem(tr("")));

        model.clear();
        model.setQuery(QString("Select (select d.vch_paymodename from cey_bt_paymode d where d.ch_paymodeno = a.ch_paymodeno  ), sum( a.num_realamount), count( a.ch_paymodeno ),a.ch_paymodeno , count( a.ch_paymodeno) , sum( a.num_face)from cey_u_checkout_detail a , cey_u_checkout_master b where(a.ch_payno =b.ch_payno)and(b.dt_operdate between '%1' and '%2')and b.ch_state ='Y' Group by a.ch_paymodeno having sum( a.num_realamount)> 0 or sum( a.num_payamount)> 0 Order by a.ch_paymodeno ").arg(date1).arg(date2));

        for(int i = 0;i < model.rowCount(); i++)
        {
            int row = model_buttom->rowCount();
            model_buttom->setRowCount(row + 1);
            QString str1 = model.record(i).value(0).toString() + tr("  ") + QString().sprintf("%0.2f",model.record(i).value(1).toFloat());
            QString str2 = tr("次数:") + QString().sprintf("%0.2f",model.record(i).value(2).toFloat());
            model_buttom->setItem(row,0,new QStandardItem(str1));
            model_buttom->setItem(row,1,new QStandardItem(str2));
        }

        //=====================================================================
    }

}

void consumption_veiw::on_pushButton_3_clicked()//打印
{
    QString title1 = ui->label_7->text() + "\n";
    QString title2 = ui->label_8->text() + "\n";

    QByteArray content;
    content.append(tr("开始时间:") + ui->label_10->text() + "\n");
    content.append(tr("结束时间:") + ui->label_12->text() + "\n");
    content.append(tr("制单时间:") + ui->label_14->text() + "\n");
    content.append(tr("制单人:") + ui->label_15->text() + "\n");

    QSettings *configIniRead = new QSettings(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    QString print_type = configIniRead->value("system_setting/dyjjiekouleixing").toString();
    if(print_type != "1")//58打印机
    {
        QString taitou = tr("项目             数量   金额\n");
        content.append(taitou);
        QString str;
        for(int i = 0; i < model_top->rowCount(); i++)
        {
             str = model_top->item(i,0)->text();
             if(str.length() < 15)
             {
                 int a = 17 - str.length() * 2;
                 for(int k = 0; k < a; k++)
                 {
                     str.append(" ");
                 }
             }
             content.append(str);

             str = model_top->item(i,1)->text();
             if(str.length() < 10)
             {
                 int a = 8 - str.length();
                 for(int k = 0; k < a; k++)
                 {
                     str.append(" ");
                 }
             }
             content.append(str);

             str = model_top->item(i,2)->text();
             content.append(str);
             content.append("\n");
        }


        if(ui->comboBox->currentIndex() == 0)//消费情况统计
        {
            QString str = tr("合计:            ");
            content.append(str);
            content.append(shuliang_num);
            content.append("   ");
            content.append(zongji_num);
        }
        else
        {
            content.append("\n");
            for(int i = 0; i < model_buttom->rowCount(); i++)
            {
                str = model_buttom->item(i,0)->text();
                if(str.length() < 15)
                {
                    int a = 15 - str.length();
                    for(int k = 0; k < a; k++)
                    {
                        str.append(" ");
                    }
                }
                content.append(str);

                str = model_buttom->item(i,1)->text();
                content.append(str);
                content.append("\n");
            }
        }
    }
    else//80打印机
    {
        QString taitou = tr("项目                数量      金额\n");
        content.append(taitou);
        QString str;
        for(int i = 0; i < model_top->rowCount(); i++)
        {
             str = model_top->item(i,0)->text();
             if(str.length() < 15)
             {
                 int a = 20 - str.length() * 2;
                 for(int k = 0; k < a; k++)
                 {
                     str.append(" ");
                 }
             }
             content.append(str);

             str = model_top->item(i,1)->text();
             if(str.length() < 10)
             {
                 int a = 10 - str.length();
                 for(int k = 0; k < a; k++)
                 {
                     str.append(" ");
                 }
             }
             content.append(str);

             str = model_top->item(i,2)->text();
             content.append(str);
             content.append("\n");
        }


        if(ui->comboBox->currentIndex() == 0)//消费情况统计
        {
            QString str = tr("合计:               ");
            content.append(str);
            content.append(shuliang_num);
            content.append("     ");
            content.append(zongji_num);
        }
        else
        {
            content.append("\n");
            for(int i = 0; i < model_buttom->rowCount(); i++)
            {
                str = model_buttom->item(i,0)->text();
                if(str.length() < 15)
                {
                    int a = 25 - str.length();
                    for(int k = 0; k < a; k++)
                    {
                        str.append(" ");
                    }
                }
                content.append(str);

                str = model_buttom->item(i,1)->text();
                content.append(str);
                content.append("\n");
            }
        }
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
    //path = "192.168.1.172:9100";
    b+=backheader::printer->drawText("1B40 1B6101 1C5701",title1);//居中不放大
    b+=backheader::printer->drawText("1B40 1B6101 1C5701",title2);//居中不放大
    b += backheader::printer->drawText("1B40,1C5700", content, Qt::AlignLeft);
    b += backheader::printer->drawText("1B6405 1D5601", "");//走纸5x， 切纸
    b=backheader::printer->setPrintCount(b, 1);
    backheader::asyn_dataprint(b);

//    printer->asynchPrint(b, path);
//    backheader::asyn_dataprint(b);
//    connect(printer,SIGNAL(errorString(QString)),this,SLOT(print_lost(QString)));
}

void consumption_veiw::print_lost(const QString &str)
{
    ;
    lds_messagebox::warning(this, tr("消息提示"),str);
    
}
