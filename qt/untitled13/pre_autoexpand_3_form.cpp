#include "pre_autoexpand_3_form.h"
#include "ui_pre_autoexpand_3_form.h"
#include "backheader.h"
#include "frontviewdialog_virtual.h"
#include "n_func.h"
#include "w_scr_dish_special_cook.h"
#include "modfiy_number.h"
#include "special_cook.h"
#include "lds_messagebox.h"
#include "cur_price.h"
#include "pre_autoexpand_rb.h"


pre_autoexpand_3_Form::pre_autoexpand_3_Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::pre_autoexpand_3_Form)
{
    ui->setupUi(this);
    int horizontal_count;
    int vertical_count;
    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);

    if(backheader::mainwindowsize.width()==800){
        ui->widget_1->tool_size=QSize(97,44);
        ui->widget_1->tool_icon_size=QSize(0,0);
        ui->widget_2->tool_size=QSize(97,44);
        ui->widget_2->tool_icon_size=QSize(0,0);
        horizontal_count=5;
        vertical_count=5;
    } else {
        ui->widget_1->tool_size=QSize(99,46);
        ui->widget_1->tool_icon_size=QSize(0,0);
        ui->widget_2->tool_size=QSize(99,46);
        ui->widget_2->tool_icon_size=QSize(0,0);
        horizontal_count=7;
        vertical_count=7;
    }
    if(conf.value("system_setting/only_dalei", false).toBool()){
        ui->widget_1->hide();
    }
    ui->frame_2->setFixedHeight((ui->widget_1->isHidden()?0:ui->widget_1->tool_size.height())+
                                (ui->widget_2->isHidden()?0:ui->widget_2->tool_size.height())
                                +4);
    ui->widget_1->horizontal_count=horizontal_count;
    ui->widget_2->horizontal_count=horizontal_count;
    ui->widget_1->vertical_count=1;
    ui->widget_2->vertical_count=1;
    ui->widget_1->set_btn_property("pushbutton_pink");
    ui->widget_2->set_btn_property("pushbutton_yellow");
    widget3_tofresh();

    connect(ui->widget_1,SIGNAL(signal_text(QString)),this,SIGNAL(signal1_text(QString)));
    connect(ui->widget_2,SIGNAL(signal_text(QString)),this,SIGNAL(signal2_text(QString)));
    connect(ui->widget_3,SIGNAL(signal_text(QString)),this,SIGNAL(signal3_text(QString)));

}

pre_autoexpand_3_Form::~pre_autoexpand_3_Form()
{
    delete ui;
}

void pre_autoexpand_3_Form::to_grade0(const QList<frontViewDialog_autoexpand::content_item_icon> &item_icon_list)
{
    ui->widget_1->clearall_item();
    foreach(frontViewDialog_autoexpand::content_item_icon item_icon, item_icon_list){
        ui->widget_1->add_item(item_icon.index, item_icon.text, item_icon.is_use_pixmap, item_icon.icon);
    }
    ui->widget_1->generat_from_items();
}

void pre_autoexpand_3_Form::to_grade1(const QList<frontViewDialog_autoexpand::content_item_icon> &item_icon_list)
{
    ui->widget_2->clearall_item();
    foreach(frontViewDialog_autoexpand::content_item_icon item_icon, item_icon_list){
        ui->widget_2->add_item(item_icon.index, item_icon.text, item_icon.is_use_pixmap, item_icon.icon);
    }
    ui->widget_2->generat_from_items();
}

void pre_autoexpand_3_Form::to_grade2(const QList<frontViewDialog_autoexpand::content_item_icon> &item_icon_list)
{

}

void pre_autoexpand_3_Form::widget3_setsql(const QString &sql)
{
    ui->widget_3->setsql(sql);
}

void pre_autoexpand_3_Form::widget3_start()
{
    ui->widget_3->start();
}

void pre_autoexpand_3_Form::widget3_item_event_click()
{
    ui->widget_3->item_event_click();
}

void pre_autoexpand_3_Form::widget3_tofresh()
{
    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    QStringList wh=conf.value("system_setting/qt_iconsize", "5x4").toString().split("x");
    QSize size3=backheader::mainwindowsize-QSize(300,ui->frame_2->height()+47+14/*结算，折扣按钮的高度*/);
    ui->widget_3->setshowpixmapEnable(conf.value("system_setting/is_show_dishpic").toBool());
    ui->widget_3->setpixmappath(conf.value("system_setting/bofanglujing").toString());
    ui->widget_3->sethcount(wh.value(0).toInt());
    ui->widget_3->setvcount(wh.value(1).toInt());
    ui->widget_3->seticonsize(QSize(
                                  (size3.width()-(ui->widget_3->gethcount()+1)*ui->widget_3->getspace()-ui->widget_3->gethcount()*pre_autoexpand_rb::border)
                                  /ui->widget_3->gethcount(),
                                  (size3.height()-(ui->widget_3->getvcount()+1)*ui->widget_3->getspace()-ui->widget_3->getvcount()*pre_autoexpand_rb::border/2)
                                  /(ui->widget_3->getvcount())-(pre_autoexpand_rb::textheight+ui->widget_3->getspace()/*上下按钮的高度*/)
                                  ));
}

int pre_autoexpand_3_Form::getframe2_h()
{
    return ui->frame_2->height();
}

void pre_autoexpand_3_Form::addGoods(const QString &barcode, const bool &bianma_daicai_type, const bool &changyongcook, const QString &bianma_str, const QString &senderclassname, bool &barscaleflag, bool &isweight_flag, QString &pay_barcode, int &row_n)
{
    QList<frontViewDialog_list::list_item> list_item_list;
    QSqlQuery query;
    QSettings configIniRead(public_sql::settings_ini_dir_file, QSettings::IniFormat);

    int num  = 0;
    pay_barcode=barcode;
    switch(0){
    case 0://判断
    {
        if(pay_barcode.isEmpty())
            return;
    }
    case 1://iconsize的设置120x120
    {
        if(senderclassname=="QLineEdit"&&barcode.contains("x")){
            configIniRead.setValue("system_setting/qt_iconsize", barcode);
            emit signal3_refresh();
            break;
        }
        if(barcode.contains("ftp:")){
            //            QFile txt1("/home/cz/images.zip");
            QFile txt1("/usr/desk/pos/fastfd");
            qDebug()<<"remove:"<< txt1.remove();
            QEventLoop eloop;
            QTimer timer;
            QUrl u("ftp://192.168.1.251/fastfd");
            QNetworkAccessManager manager;
            timer.setSingleShot(true);
            connect(&timer,SIGNAL(timeout()),&eloop,SLOT(quit()));
            txt1.open(QFile::WriteOnly|QFile::Append);
            txt1.close();
            u.setUserName("cz");
            u.setPassword("111111");
            connect(&manager,SIGNAL(finished(QNetworkReply*)),&eloop,SLOT(quit()));
            QNetworkReply *reply=manager.get(QNetworkRequest(u));
            timer.start(10*1000);//10s的超时eply*)),&eloop,SLOT(quit()));
            eloop.exec(QEventLoop::ExcludeUserInputEvents);
            if(timer.isActive()
                    &&reply->error()==QNetworkReply::NoError){
                while(!reply->atEnd()){
                    txt1.write(reply->readLine());
                }
                txt1.close();
                lds_messagebox::warning(this,tr("下载文件"), tr("下载成功"), true);
            } else {
                qDebug() << "failure";
                lds_messagebox::warning(this,tr("下载文件"), tr("下载失败"), true);
            }
        }
    }
    case 2://如果条码秤的条码的话，需要解析
    {
        //先进行解析
        QString _barcode, _price, _weight, _amount;
        if(senderclassname=="QLineEdit"&&/*必须是lineedit发送的slot才会触发*/
                CommonHeader::barcode_scale_resolve(_barcode,_price,_weight,_amount,pay_barcode)){
            //只支持一个商品
            query.exec(QString("select ch_dishno,vch_dishname,num_price from cey_bt_dish where ch_stopflag <> 'Y' and vch_barcode='%1' ").arg(_barcode));
            if(query.next()){
                if(_price.isEmpty()){
                    _price=QString().sprintf("%.02f", query.record().value("num_price").toFloat());
                } else {
                    _price=QString().sprintf("%.02f",_price.toFloat()/100.0);
                }

                if(_weight.isEmpty()&&!_amount.isEmpty()){
                    _weight=QString().sprintf("%.03f",_amount.toFloat()/100.0 /_price.toFloat());
                    _amount=QString().sprintf("%.02f",_amount.toFloat()/100.0);
                }
                if(!_weight.isEmpty()&&_amount.isEmpty()){
                    _weight=QString().sprintf("%.03f",_weight.toFloat()/1000.0);
                    _amount=QString().sprintf("%.02f",_weight.toFloat()*_price.toFloat());
                }
                if(_weight.isEmpty()&&_amount.isEmpty()){
                    _weight="1.000";
                    _amount=QString().sprintf("%.02f",_weight.toFloat()*_price.toFloat());
                }

                QSqlQuery q;
                q.exec(QString("select vch_unitname from cey_bt_unit where ch_unitno = '%1' ").arg(query.record().value("ch_unitno").toString()));
                QString danwei=q.record().value("vch_unitname").toString();

                int row = frontViewDialog_virtual::modelBase.rowCount();
                frontViewDialog_virtual::modelBase.setRowCount(row + 1);
                frontViewDialog_virtual::modelBase.setItem(row,0,new QStandardItem(QString::number(row+1))); //序号
                frontViewDialog_virtual::modelBase.setItem(row,1,new QStandardItem(query.record().value(0).toString()));//编码
                frontViewDialog_virtual::modelBase.setItem(row,2,new QStandardItem(query.record().value(1).toString())); //品名
                frontViewDialog_virtual::modelBase.setItem(row,3,new QStandardItem(_weight));//数量
                frontViewDialog_virtual::modelBase.setItem(row,4,new QStandardItem(_price)); //金额
                //            frontViewDialog_virtual::modelBase.setItem(row,4,new QStandardItem(QString().sprintf("%0.2f",jiage.toFloat() * model_danwei.record(0).value(3).toFloat()))); //金额
                frontViewDialog_virtual::modelBase.setItem(row,5,new QStandardItem("100")); //折扣值
                frontViewDialog_virtual::modelBase.setItem(row,6,new QStandardItem(_amount)); //总价
                frontViewDialog_virtual::modelBase.setItem(row,7,new QStandardItem(tr(""))); //特殊做法
                frontViewDialog_virtual::modelBase.setItem(row,8,new QStandardItem("N")); //是否赠送
                frontViewDialog_virtual::modelBase.setItem(row,9,new QStandardItem("N")); //是否套菜
                frontViewDialog_virtual::modelBase.setItem(row,10,new QStandardItem("N")); //是否特价
                frontViewDialog_virtual::modelBase.setItem(row,11,new QStandardItem("N")); //是否优惠
                frontViewDialog_virtual::modelBase.setItem(row,12,new QStandardItem(danwei)); //单位
                frontViewDialog_virtual::modelBase.setItem(row,13,new QStandardItem("")); //赠送
                frontViewDialog_virtual::modelBase.setItem(row,14,new QStandardItem("")); //定折人
                frontViewDialog_virtual::modelBase.setItem(row,15,new QStandardItem("Y")); //是否时价菜
                frontViewDialog_virtual::modelBase.setItem(row,16,new QStandardItem("")); //子套菜所在主套菜编码
                frontViewDialog_virtual::modelBase.setItem(row,17,new QStandardItem(query.record().value("int_rate").toString()));//税点
                frontViewDialog_virtual::modelBase.setItem(row,18,new QStandardItem(query.record().value(8).toString()));//是否折扣
                frontViewDialog_virtual::modelBase.setItem(row,19,new QStandardItem(n_func::f_get_sysdatetime().toString("yyyy-MM-dd hh:mm:ss")));//折扣时间
                frontViewDialog_virtual::modelBase.setItem(row,20,new QStandardItem(""));//赠送时间
                frontViewDialog_virtual::modelBase.setItem(row,21,new QStandardItem("0"));//做法加价
                frontViewDialog_virtual::modelBase.setItem(row,22,new QStandardItem(query.record().value(4).toString())); //所属大类编码
                frontViewDialog_virtual::modelBase.setItem(row,23,new QStandardItem(query.record().value(14).toString())); //所属小类编码
                frontViewDialog_virtual::modelBase.setItem(row,24,new QStandardItem("N")); //是否落单
                frontViewDialog_virtual::modelBase.setItem(row,25,new QStandardItem("")); //落单flowid
                frontViewDialog_virtual::modelBase.setItem(row,26,new QStandardItem("0.00")); //已退量码
            }
            barscaleflag=true;
            break;
        }
    }

    case 3://单纯的条码
    {
        query.exec(QString("select ch_dishno from cey_bt_dish where vch_barcode = '%1' ").arg(pay_barcode));
        if(query.next()){
            pay_barcode=query.record().value("ch_dishno").toString();//将对应的条码转换秤 商品的编码
            barscaleflag=true;
        }
    }
    case 4://正常情况
    {
        //判断是否是称重商品
        query.exec(QString("select ch_weight from cey_bt_dish where ch_dishno='%1' ").arg(pay_barcode));
        query.next();
        isweight_flag=(query.record().value("ch_weight").toString()=="Y");

        query.exec(QString("select ch_dishno from cey_bt_dish where ch_stopflag <> 'Y' and ch_dishno='%1' ").arg(pay_barcode));
        if(!query.next()) return;
        //barcode
        QSqlQueryModel diancai_model;
        if(bianma_daicai_type == false  && changyongcook == false)
        {
            diancai_model.setQuery(QString("SELECT ch_dishno, vch_dishname, ch_curprice,ch_unitno,int_rate,num_price,ch_suitflag,ch_discount,ch_dish_typeno, ch_sub_typeno FROM cey_bt_dish where  (ch_stopflag = null or ch_stopflag = 'N') and ch_dishno = '%3' ").arg(pay_barcode));
        }

        if(bianma_daicai_type == true && changyongcook == false)
        {
            diancai_model.setQuery(QString("SELECT ch_dishno, vch_dishname, ch_curprice,ch_unitno,int_rate,num_price,ch_suitflag,ch_discount,ch_dish_typeno, ch_sub_typeno FROM cey_bt_dish where (ch_stopflag = null or ch_stopflag = 'N') and (ch_dishno like '%" + bianma_str + "%' or vch_spell like '%" + bianma_str + "%') and  ch_dishno = '%3' ").arg(pay_barcode));
        }

        if(bianma_daicai_type == false && changyongcook == true)
        {
            diancai_model.setQuery(QString("select ch_dishno, vch_dishname, ch_curprice,ch_unitno,int_rate,num_price,ch_suitflag,ch_discount,ch_dish_typeno, ch_sub_typeno from cey_bt_dish where ch_off ='Y' and (ch_stopflag = null or ch_stopflag = 'N') and  ch_dishno = '%3' ").arg(pay_barcode));
        }


        QSqlQueryModel zufa_model;
        QDateTime datetime = n_func::f_get_sysdatetime();
        QString date = datetime.toString("yyyy-MM-dd");
        QString time;
        QSqlQueryModel zitaocai_tem;
        QSqlQueryModel model_specil;
        model_specil.setQuery(QString("Select IFNULL(num_price , 0)from cey_bt_dish_special where ch_dishno ='%1' and ch_state ='Y' and left( dt_operdate_effect , 10)='%2' ").arg(diancai_model.record(num).value("ch_dishno").toString()).arg(date));

        QSqlQueryModel model;
        QSqlQueryModel model_danwei;
        model.setQuery(QString("Select IFNULL(num_num - num_sale , 0)from cey_bt_dish_warn where ch_dishno ='%1' and ch_state ='Y'  and left( dt_date , 10)='%2' ").arg(diancai_model.record(num).value("ch_dishno").toString()).arg(date));

        model_danwei.setQuery(QString("select vch_unitname, num_default   from cey_bt_unit where ch_unitno = '%1' ").arg(diancai_model.record(num).value("ch_unitno").toString()));
        QString danwei = model_danwei.record(0).value("vch_unitname").toString();
        float zuofajiage;
        zuofajiage = 0.00;
        if(model.rowCount() > 0)
        {
            if(model.record(0).value(0).toFloat() <= 0)
            {
                lds_messagebox::warning(this, tr("消息提示"),tr("菜品已经估清,请确认!"));

                return;
            }
            else
            {
                float zongdian = 0.00;
                for(int i = 0; i < frontViewDialog_virtual::modelBase.rowCount(); i++)
                {
                    if(frontViewDialog_virtual::modelBase.item(i,1)->text() == diancai_model.record(num).value("ch_dishno").toString())
                    {
                        zongdian += frontViewDialog_virtual::modelBase.item(i,3)->text().toFloat();
                    }
                }


                if(zongdian < model.record(0).value(0).toFloat())
                {
                    float sheng = 0.00;
                    sheng = model.record(0).value(0).toFloat()  - zongdian - model_danwei.record(0).value("num_default").toFloat();
                    QString str = tr("菜品:") + diancai_model.record(num).value("vch_dishname").toString() + tr("已经估清,扣除此次数量,\n剩余数量为:") + QString().sprintf("%0.1f",sheng) + tr("!");
                    lds_messagebox::warning(this, tr("消息提示"),str);

                }
                else
                {
                    lds_messagebox::warning(this, tr("消息提示"),tr("菜品已经估清,请确认!"));

                    return;
                }
            }
        }

        QSqlQueryModel model_num;
        model_num.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='ordertonum' "));
        QString cook_num = model_num.record(0).value(0).toString();
        float zitaocaishuling = 0.00;
        if(diancai_model.record(num).value("ch_curprice").toString() == "Y")//时价菜
        {
            cur_price curprice(QString().sprintf("%0.1f",diancai_model.record(num).value("num_price").toFloat()));
            float shuishou = 0.00;
            shuishou = diancai_model.record(num).value("int_rate").toFloat();
            /*curprice.exec();*/backheader::static_dialogexec(curprice, tr("点菜价格修改"));
            if(curprice.btn_t_f == true)
            {
                QString suit;
                if(diancai_model.record(num).value("ch_suitflag").toString() == "Y")
                {
                    suit = "P";
                }
                else
                {
                    suit = "N";
                }
                QString jiage = QString().sprintf("%0.2f",curprice.price.toFloat());
                QString zhutaobianma = diancai_model.record(num).value("ch_dishno").toString();
                int row = frontViewDialog_virtual::modelBase.rowCount();
                frontViewDialog_virtual::modelBase.setRowCount(row + 1);
                frontViewDialog_virtual::modelBase.setItem(row,0,new QStandardItem(QString::number(row+1))); //序号
                frontViewDialog_virtual::modelBase.setItem(row,1,new QStandardItem(diancai_model.record(num).value("ch_dishno").toString()));//编码
                frontViewDialog_virtual::modelBase.setItem(row,2,new QStandardItem(diancai_model.record(num).value("vch_dishname").toString())); //品名
                frontViewDialog_virtual::modelBase.setItem(row,3,new QStandardItem(QString().sprintf("%0.2f",model_danwei.record(0).value("num_default").toFloat())));//数量
                frontViewDialog_virtual::modelBase.setItem(row,4,new QStandardItem(jiage)); //金额
                //            frontViewDialog_virtual::modelBase.setItem(row,4,new QStandardItem(QString().sprintf("%0.2f",jiage.toFloat() * model_danwei.record(0).value(3).toFloat()))); //金额
                frontViewDialog_virtual::modelBase.setItem(row,5,new QStandardItem("100")); //折扣值
                frontViewDialog_virtual::modelBase.setItem(row,6,new QStandardItem(QString().sprintf("%0.2f",jiage.toFloat() * model_danwei.record(0).value("num_default").toFloat()))); //总价
                frontViewDialog_virtual::modelBase.setItem(row,7,new QStandardItem(tr(""))); //特殊做法
                frontViewDialog_virtual::modelBase.setItem(row,8,new QStandardItem("N")); //是否赠送
                frontViewDialog_virtual::modelBase.setItem(row,9,new QStandardItem(suit)); //是否套菜
                frontViewDialog_virtual::modelBase.setItem(row,10,new QStandardItem("N")); //是否特价
                frontViewDialog_virtual::modelBase.setItem(row,11,new QStandardItem("N")); //是否优惠
                frontViewDialog_virtual::modelBase.setItem(row,12,new QStandardItem(danwei)); //单位
                frontViewDialog_virtual::modelBase.setItem(row,13,new QStandardItem("")); //赠送
                frontViewDialog_virtual::modelBase.setItem(row,14,new QStandardItem("")); //定折人
                frontViewDialog_virtual::modelBase.setItem(row,15,new QStandardItem("Y")); //是否时价菜
                frontViewDialog_virtual::modelBase.setItem(row,16,new QStandardItem("")); //子套菜所在主套菜编码
                frontViewDialog_virtual::modelBase.setItem(row,17,new QStandardItem(QString().sprintf("%0.2f",shuishou)));//税点
                frontViewDialog_virtual::modelBase.setItem(row,18,new QStandardItem(diancai_model.record(num).value("ch_discount").toString()));//是否折扣
                frontViewDialog_virtual::modelBase.setItem(row,19,new QStandardItem(time));//折扣时间
                frontViewDialog_virtual::modelBase.setItem(row,20,new QStandardItem(time));//赠送时间
                frontViewDialog_virtual::modelBase.setItem(row,21,new QStandardItem(QString().sprintf("%0.2f",zuofajiage)));//做法加价
                frontViewDialog_virtual::modelBase.setItem(row,22,new QStandardItem(diancai_model.record(num).value("ch_dish_typeno").toString())); //所属大类编码
                frontViewDialog_virtual::modelBase.setItem(row,23,new QStandardItem(diancai_model.record(num).value("ch_sub_typeno").toString())); //所属小类编码
                frontViewDialog_virtual::modelBase.setItem(row,24,new QStandardItem("N")); //是否落单
                frontViewDialog_virtual::modelBase.setItem(row,25,new QStandardItem("")); //落单flowid
                frontViewDialog_virtual::modelBase.setItem(row,26,new QStandardItem("0.00")); //已退量码

                row_n = row;
                //ui->tableView->tableview->selectRow(row);
                if(cook_num == "1")//自动录入点菜数量
                {
                    modfiy_number modfiy(frontViewDialog_virtual::modelBase.item(row,3)->text());
                    /*modfiy.exec();*/backheader::static_dialogexec(modfiy, tr("点菜数量修改"));
                    if(modfiy.btn_t_f == true)
                    {
                        frontViewDialog_virtual::modelBase.item(row,3)->setText(QString().sprintf("%0.3f",modfiy.modfiy_num));
                        zitaocaishuling = modfiy.modfiy_num;
                        float zongjia = frontViewDialog_virtual::modelBase.item(row,3)->text().toFloat() * frontViewDialog_virtual::modelBase.item(row,4)->text().toFloat() * frontViewDialog_virtual::modelBase.item(row,5)->text().toFloat() * 0.01;
                        frontViewDialog_virtual::modelBase.item(row,6)->setText(QString().sprintf("%0.2f",zongjia));
                    }
                }

                //存在特殊做法
                if(suit != "P" && suit != "Y")
                {
                    zufa_model.clear();
                    zufa_model.setQuery(QString("SELECT a.ch_no,a.vch_cp_memo,a.ch_typeno,a.num_add_price FROM cey_bt_cp_memo a,cey_bt_cp_dish b WHERE(a.ch_no = b.ch_no)and( b.ch_dishno = '%1')").arg(diancai_model.record(num).value("ch_dishno").toString()));
                    if(zufa_model.rowCount() > 0)
                    {
                        special_cook special(diancai_model.record(num).value("ch_dishno").toString(),"","0.00");
                        /*special->exec();*/backheader::static_dialogexec(special, tr("特殊做法"));
                        if(special.btn_t_f == true)
                        {
                            frontViewDialog_virtual::modelBase.item(row,7)->setText(special.zuofaneirong);
                            frontViewDialog_virtual::modelBase.item(row,21)->setText(QString().sprintf("%0.2f",special.add_price));
                            float aa;
                            aa = 0.00;
                            aa = frontViewDialog_virtual::modelBase.item(row,3)->text().toFloat() * frontViewDialog_virtual::modelBase.item(row,4)->text().toFloat() * frontViewDialog_virtual::modelBase.item(row,5)->text().toFloat() * 0.01 + frontViewDialog_virtual::modelBase.item(row,3)->text().toFloat() * special.add_price;
                            frontViewDialog_virtual::modelBase.item(row,6)->setText(QString().sprintf("%0.2f",aa));
                        }
                    }
                }

                if(diancai_model.record(num).value("ch_suitflag").toString() == "Y")//存在套菜
                {
                    QSqlQueryModel tao;
                    QSqlQueryModel model_zi;
                    tao.setQuery(QString("select ch_dishno,num_num from cey_bt_dish_suit where ch_suitno ='%1'").arg(diancai_model.record(num).value("ch_dishno").toString()));
                    qDebug() << diancai_model.record(num).value("ch_dishno").toString();
                    for(int i = 0; i < tao.rowCount(); i++)
                    {
                        QString dish_no = tao.record(i).value("ch_dishno").toString();//子套菜编码
                        zitaocai_tem.clear();
                        zitaocai_tem.setQuery(QString("select ch_tempflag from cey_bt_dish where ch_dishno = '%1' ").arg(dish_no));
                        if(zitaocai_tem.record(0).value(0).toString() == "N")
                        {
                            float zi_num = 0.00;//子套菜数量
                            if(cook_num == "0")
                            {
                                zi_num = tao.record(i).value("num_num").toFloat();
                            }
                            else
                            {
                                zi_num = tao.record(i).value("num_num").toFloat() * zitaocaishuling;
                            }
                            model_zi.clear();
                            model_zi.setQuery(QString("select ch_dishno,vch_dishname,num_price from cey_bt_dish where ch_dishno = '%1' ").arg(dish_no));
                            model_danwei.clear();
                            model_danwei.setQuery(QString("select vch_unitname, num_default from cey_bt_unit where ch_unitno = '%1' ").arg(model_zi.record(0).value("ch_unitno").toString()));
                            QString zi_danwei = model_danwei.record(0).value("vch_unitname").toString();//子套菜单位
                            int row = frontViewDialog_virtual::modelBase.rowCount();
                            frontViewDialog_virtual::modelBase.setRowCount(row + 1);
                            frontViewDialog_virtual::modelBase.setItem(row,0,new QStandardItem(QString::number(row+1))); //序号
                            frontViewDialog_virtual::modelBase.setItem(row,1,new QStandardItem(dish_no)); //编码
                            frontViewDialog_virtual::modelBase.setItem(row,2,new QStandardItem(model_zi.record(0).value("vch_dishname").toString())); //品名
                            frontViewDialog_virtual::modelBase.setItem(row,3,new QStandardItem(QString().sprintf("%0.2f",zi_num))); //数量
                            frontViewDialog_virtual::modelBase.setItem(row,4,new QStandardItem(QString().sprintf("%0.2f",model_zi.record(0).value("num_price").toFloat()))); //金额
                            frontViewDialog_virtual::modelBase.setItem(row,5,new QStandardItem("100")); //折扣值
                            frontViewDialog_virtual::modelBase.setItem(row,6,new QStandardItem("0.00")); //总价
                            frontViewDialog_virtual::modelBase.setItem(row,7,new QStandardItem(tr(""))); //特殊做法
                            frontViewDialog_virtual::modelBase.setItem(row,8,new QStandardItem("N")); //是否赠送
                            frontViewDialog_virtual::modelBase.setItem(row,9,new QStandardItem("Y")); //是否套菜
                            frontViewDialog_virtual::modelBase.setItem(row,10,new QStandardItem("N")); //是否特价
                            frontViewDialog_virtual::modelBase.setItem(row,11,new QStandardItem("N")); //是否优惠
                            frontViewDialog_virtual::modelBase.setItem(row,12,new QStandardItem(zi_danwei)); //单位
                            frontViewDialog_virtual::modelBase.setItem(row,13,new QStandardItem("")); //赠送
                            frontViewDialog_virtual::modelBase.setItem(row,14,new QStandardItem("")); //定折人
                            frontViewDialog_virtual::modelBase.setItem(row,15,new QStandardItem("N")); //是否时价菜
                            frontViewDialog_virtual::modelBase.setItem(row,16,new QStandardItem(zhutaobianma)); //子套菜所在主套菜编码
                            frontViewDialog_virtual::modelBase.setItem(row,17,new QStandardItem("0.00"));//税点
                            frontViewDialog_virtual::modelBase.setItem(row,18,new QStandardItem("N"));//是否折扣
                            frontViewDialog_virtual::modelBase.setItem(row,19,new QStandardItem(time));//折扣时间
                            frontViewDialog_virtual::modelBase.setItem(row,20,new QStandardItem(time));//赠送时间
                            frontViewDialog_virtual::modelBase.setItem(row,21,new QStandardItem(QString().sprintf("%0.2f",zuofajiage)));//做法加价
                            frontViewDialog_virtual::modelBase.setItem(row,22,new QStandardItem(diancai_model.record(num).value("ch_dish_typeno").toString())); //所属大类编码
                            frontViewDialog_virtual::modelBase.setItem(row,23,new QStandardItem(diancai_model.record(num).value("ch_sub_typeno").toString())); //所属小类编码
                            frontViewDialog_virtual::modelBase.setItem(row,24,new QStandardItem("N")); //是否落单
                            frontViewDialog_virtual::modelBase.setItem(row,25,new QStandardItem("")); //落单flowid
                            frontViewDialog_virtual::modelBase.setItem(row,26,new QStandardItem("0.00")); //已退量码
                            ////ui->tableView->tableview->selectRow(row);
                            row_n = row;
                        }
                    }//临时菜
                }
            }
        }
        else if(model_specil.rowCount() > 0 && diancai_model.record(num).value("ch_curprice").toString() != "Y")//菜品为特价
        {
            QString suit;
            if(diancai_model.record(num).value("ch_suitflag").toString() == "Y")
            {
                suit = "P";
            }
            else
            {
                suit = "N";
            }
            QString jiage = QString().sprintf("%0.2f",model_specil.record(0).value(0).toFloat());
            QString zhutaobianma = diancai_model.record(num).value("ch_dishno").toString();
            float shuishou = 0.00;
            shuishou = diancai_model.record(num).value("int_rate").toFloat();
            int row = frontViewDialog_virtual::modelBase.rowCount();
            frontViewDialog_virtual::modelBase.setRowCount(row + 1);
            frontViewDialog_virtual::modelBase.setItem(row,0,new QStandardItem(QString::number(row+1))); //序号
            frontViewDialog_virtual::modelBase.setItem(row,1,new QStandardItem(diancai_model.record(num).value("ch_dishno").toString())); //编码
            frontViewDialog_virtual::modelBase.setItem(row,2,new QStandardItem(diancai_model.record(num).value("vch_dishname").toString())); //品名
            frontViewDialog_virtual::modelBase.setItem(row,3,new QStandardItem(QString().sprintf("%0.2f",model_danwei.record(0).value("num_default").toFloat()))); //数量
            frontViewDialog_virtual::modelBase.setItem(row,4,new QStandardItem(QString().sprintf("%0.2f",jiage.toFloat()))); //金额
            //        frontViewDialog_virtual::modelBase.setItem(row,4,new QStandardItem(QString().sprintf("%0.2f",jiage.toFloat()))); //金额
            frontViewDialog_virtual::modelBase.setItem(row,5,new QStandardItem("100")); //折扣值
            frontViewDialog_virtual::modelBase.setItem(row,6,new QStandardItem(QString().sprintf("%0.2f",jiage.toFloat() * model_danwei.record(0).value("num_default").toFloat() ))); //总价
            frontViewDialog_virtual::modelBase.setItem(row,7,new QStandardItem(tr(""))); //特殊做法
            frontViewDialog_virtual::modelBase.setItem(row,8,new QStandardItem("N")); //7
            frontViewDialog_virtual::modelBase.setItem(row,9,new QStandardItem(suit)); //是否套菜
            frontViewDialog_virtual::modelBase.setItem(row,10,new QStandardItem("Y")); //是否特价
            frontViewDialog_virtual::modelBase.setItem(row,11,new QStandardItem("N")); //是否优惠
            frontViewDialog_virtual::modelBase.setItem(row,12,new QStandardItem(danwei)); //单位
            frontViewDialog_virtual::modelBase.setItem(row,13,new QStandardItem("")); //赠送
            frontViewDialog_virtual::modelBase.setItem(row,14,new QStandardItem("")); //定折人
            frontViewDialog_virtual::modelBase.setItem(row,15,new QStandardItem("N")); //是否时价菜
            frontViewDialog_virtual::modelBase.setItem(row,16,new QStandardItem("")); //子套菜所在主套菜编码
            frontViewDialog_virtual::modelBase.setItem(row,17,new QStandardItem(QString().sprintf("%0.2f",shuishou)));//税点
            frontViewDialog_virtual::modelBase.setItem(row,18,new QStandardItem(diancai_model.record(num).value("ch_discount").toString()));//是否折扣
            frontViewDialog_virtual::modelBase.setItem(row,19,new QStandardItem(time));//折扣时间
            frontViewDialog_virtual::modelBase.setItem(row,20,new QStandardItem(time));//赠送时间
            frontViewDialog_virtual::modelBase.setItem(row,21,new QStandardItem(QString().sprintf("%0.2f",zuofajiage)));//做法加价
            frontViewDialog_virtual::modelBase.setItem(row,22,new QStandardItem(diancai_model.record(num).value("ch_dish_typeno").toString())); //所属大类编码
            frontViewDialog_virtual::modelBase.setItem(row,23,new QStandardItem(diancai_model.record(num).value("ch_sub_typeno").toString())); //所属小类编码
            frontViewDialog_virtual::modelBase.setItem(row,24,new QStandardItem("N")); //是否落单
            frontViewDialog_virtual::modelBase.setItem(row,25,new QStandardItem("")); //落单flowid
            frontViewDialog_virtual::modelBase.setItem(row,26,new QStandardItem("0.00")); //已退量码

            row_n = row;
            //ui->tableView->tableview->selectRow(row);

            if(cook_num == "1")//自动录入点菜数量
            {
                modfiy_number modfiy(frontViewDialog_virtual::modelBase.item(row,3)->text());
                /*modfiy.exec();*/backheader::static_dialogexec(modfiy, tr("点菜数量修改"));
                if(modfiy.btn_t_f == true)
                {
                    zitaocaishuling = modfiy.modfiy_num;
                    frontViewDialog_virtual::modelBase.item(row,3)->setText(QString().sprintf("%0.3f",modfiy.modfiy_num));
                    float zongjia = frontViewDialog_virtual::modelBase.item(row,3)->text().toFloat() * frontViewDialog_virtual::modelBase.item(row,4)->text().toFloat() * frontViewDialog_virtual::modelBase.item(row,5)->text().toFloat() * 0.01;
                    frontViewDialog_virtual::modelBase.item(row,6)->setText(QString().sprintf("%0.2f",zongjia));
                }
            }

            //存在特殊做法
            if(suit != "P" && suit != "Y")
            {
                zufa_model.clear();
                zufa_model.setQuery(QString("SELECT a.ch_no,a.vch_cp_memo,a.ch_typeno,a.num_add_price FROM cey_bt_cp_memo a,cey_bt_cp_dish b WHERE(a.ch_no = b.ch_no)and( b.ch_dishno = '%1')").arg(diancai_model.record(num).value("ch_dishno").toString()));
                if(zufa_model.rowCount() > 0)
                {
                    w_scr_dish_special_cook special(diancai_model.record(num).value("ch_dishno").toString(),"","0.00");
                    /*special.exec();*/backheader::static_dialogexec(special, tr("特殊做法"));
                    if(special.btn_t_f == true)
                    {
                        frontViewDialog_virtual::modelBase.item(row,7)->setText(special.zuofaneirong);
                        frontViewDialog_virtual::modelBase.item(row,21)->setText(QString().sprintf("%0.2f",special.add_price));
                        float aa;
                        aa = 0.00;
                        aa = frontViewDialog_virtual::modelBase.item(row,3)->text().toFloat() * frontViewDialog_virtual::modelBase.item(row,4)->text().toFloat() * frontViewDialog_virtual::modelBase.item(row,5)->text().toFloat() * 0.01 + frontViewDialog_virtual::modelBase.item(row,3)->text().toFloat() * special.add_price;
                        frontViewDialog_virtual::modelBase.item(row,6)->setText(QString().sprintf("%0.2f",aa));
                    }
                }
            }

            if(diancai_model.record(num).value("ch_suitflag").toString() == "Y")//存在套菜
            {
                QSqlQueryModel tao;
                QSqlQueryModel model_zi;
                tao.setQuery(QString("select ch_dishno,num_num from cey_bt_dish_suit where ch_suitno ='%1'").arg(diancai_model.record(num).value("ch_dishno").toString()));
                qDebug() << tao.query().lastQuery();
                for(int i = 0; i < tao.rowCount(); i++)
                {
                    QString dish_no = tao.record(i).value("ch_dishno").toString();//子套菜编
                    zitaocai_tem.clear();
                    zitaocai_tem.setQuery(QString("select ch_tempflag from cey_bt_dish where ch_dishno = '%1' ").arg(dish_no));
                    if(zitaocai_tem.record(0).value(0).toString() == "N")
                    {
                        float zi_num = 0.00;//子套菜数量
                        if(cook_num == "0")
                        {
                            zi_num = tao.record(i).value("num_num").toFloat();
                        }
                        else
                        {
                            zi_num = tao.record(i).value("num_num").toFloat() * zitaocaishuling;
                        }
                        model_zi.clear();
                        model_zi.setQuery(QString("select ch_dishno,vch_dishname,num_price from cey_bt_dish where ch_dishno = '%1' ").arg(dish_no));
                        model_danwei.setQuery(QString("select vch_unitname, num_default from cey_bt_unit where ch_unitno = '%1' ").arg(model_zi.record(0).value("ch_unitno").toString()));
                        QString zi_danwei = model_danwei.record(0).value("vch_unitname").toString();//子套菜单位
                        int row = frontViewDialog_virtual::modelBase.rowCount();
                        frontViewDialog_virtual::modelBase.setRowCount(row + 1);
                        frontViewDialog_virtual::modelBase.setItem(row,0,new QStandardItem(QString::number(row+1))); //序号
                        frontViewDialog_virtual::modelBase.setItem(row,1,new QStandardItem(dish_no)); //编码
                        frontViewDialog_virtual::modelBase.setItem(row,2,new QStandardItem(model_zi.record(0).value("vch_dishname").toString())); //品名
                        frontViewDialog_virtual::modelBase.setItem(row,3,new QStandardItem(QString().sprintf("%0.2f",zi_num))); //数量
                        frontViewDialog_virtual::modelBase.setItem(row,4,new QStandardItem(QString().sprintf("%0.2f",model_zi.record(0).value("num_price").toFloat()))); //金额
                        frontViewDialog_virtual::modelBase.setItem(row,5,new QStandardItem("100")); //折扣值
                        frontViewDialog_virtual::modelBase.setItem(row,6,new QStandardItem("0.00")); //总价
                        frontViewDialog_virtual::modelBase.setItem(row,7,new QStandardItem(tr(""))); //特殊做法
                        frontViewDialog_virtual::modelBase.setItem(row,8,new QStandardItem("N")); //是否赠送
                        frontViewDialog_virtual::modelBase.setItem(row,9,new QStandardItem("Y")); //是否套菜
                        frontViewDialog_virtual::modelBase.setItem(row,10,new QStandardItem("N")); //是否特价
                        frontViewDialog_virtual::modelBase.setItem(row,11,new QStandardItem("N")); //是否优惠
                        frontViewDialog_virtual::modelBase.setItem(row,12,new QStandardItem(zi_danwei)); //单位
                        frontViewDialog_virtual::modelBase.setItem(row,13,new QStandardItem("")); //赠送
                        frontViewDialog_virtual::modelBase.setItem(row,14,new QStandardItem("")); //定折人
                        frontViewDialog_virtual::modelBase.setItem(row,15,new QStandardItem("N")); //是否时价菜
                        frontViewDialog_virtual::modelBase.setItem(row,16,new QStandardItem(zhutaobianma)); //子套菜所在主套菜编码
                        frontViewDialog_virtual::modelBase.setItem(row,17,new QStandardItem("0.00"));//税点
                        frontViewDialog_virtual::modelBase.setItem(row,18,new QStandardItem("N"));//是否折扣
                        frontViewDialog_virtual::modelBase.setItem(row,19,new QStandardItem(time));//折扣时间
                        frontViewDialog_virtual::modelBase.setItem(row,20,new QStandardItem(time));//赠送时间
                        frontViewDialog_virtual::modelBase.setItem(row,21,new QStandardItem(QString().sprintf("%0.2f",zuofajiage)));//做法加价
                        frontViewDialog_virtual::modelBase.setItem(row,22,new QStandardItem(diancai_model.record(num).value("ch_dish_typeno").toString())); //所属大类编码
                        frontViewDialog_virtual::modelBase.setItem(row,23,new QStandardItem(diancai_model.record(num).value("ch_sub_typeno").toString())); //所属小类编码
                        frontViewDialog_virtual::modelBase.setItem(row,24,new QStandardItem("N")); //是否落单
                        frontViewDialog_virtual::modelBase.setItem(row,25,new QStandardItem("")); //落单flowid
                        frontViewDialog_virtual::modelBase.setItem(row,26,new QStandardItem("0.00")); //已退量码
                        // //ui->tableView->tableview->selectRow(row);
                        row_n = row;
                    }
                }
            }
        }
        else
        {
            bool can_add_pre=true;
            QString suit;
            if(diancai_model.record(num).value("ch_suitflag").toString() == "Y")
            {
                suit = "P";
            }
            else
            {
                suit = "N";
            }
            QString zhutaobianma = diancai_model.record(num).value("ch_dishno").toString();
            float shuishou = 0.00;
            shuishou = diancai_model.record(num).value("int_rate").toFloat();
            int row = frontViewDialog_virtual::modelBase.rowCount();
            frontViewDialog_virtual::modelBase.setRowCount(row + 1);
            frontViewDialog_virtual::modelBase.setItem(row,0,new QStandardItem(QString::number(row+1))); //序号

            frontViewDialog_virtual::modelBase.setItem(row,1,new QStandardItem(diancai_model.record(num).value("ch_dishno").toString()));
            frontViewDialog_virtual::modelBase.setItem(row,2,new QStandardItem(diancai_model.record(num).value("vch_dishname").toString()));
            frontViewDialog_virtual::modelBase.setItem(row,3,new QStandardItem(QString().sprintf("%0.3f",model_danwei.record(0).value("num_default").toFloat())));

            frontViewDialog_virtual::modelBase.setItem(row,4,new QStandardItem(QString().sprintf("%0.2f",diancai_model.record(num).value("num_price").toFloat()))); //金额
            frontViewDialog_virtual::modelBase.setItem(row,5,new QStandardItem("100")); //折扣值
            float zongjia = 0.00;zongjia = model_danwei.record(0).value("num_default").toFloat() * diancai_model.record(num).value("num_price").toFloat();
            frontViewDialog_virtual::modelBase.setItem(row,6,new QStandardItem(QString().sprintf("%0.2f",zongjia))); //总价
            frontViewDialog_virtual::modelBase.setItem(row,7,new QStandardItem(tr(""))); //特殊做法
            frontViewDialog_virtual::modelBase.setItem(row,8,new QStandardItem("N")); //是否赠送
            frontViewDialog_virtual::modelBase.setItem(row,9,new QStandardItem(suit)); //是否套菜
            frontViewDialog_virtual::modelBase.setItem(row,10,new QStandardItem("N")); //是否特价
            frontViewDialog_virtual::modelBase.setItem(row,11,new QStandardItem("N")); //是否优惠
            frontViewDialog_virtual::modelBase.setItem(row,12,new QStandardItem(danwei)); //单位
            frontViewDialog_virtual::modelBase.setItem(row,13,new QStandardItem("")); //赠送
            frontViewDialog_virtual::modelBase.setItem(row,14,new QStandardItem("")); //定折人
            frontViewDialog_virtual::modelBase.setItem(row,15,new QStandardItem("N")); //是否时价菜
            frontViewDialog_virtual::modelBase.setItem(row,16,new QStandardItem("")); //子套菜所在主套菜编码
            frontViewDialog_virtual::modelBase.setItem(row,17,new QStandardItem(QString().sprintf("%0.2f",shuishou)));//税点
            frontViewDialog_virtual::modelBase.setItem(row,18,new QStandardItem(diancai_model.record(num).value("ch_discount").toString()));//是否折扣
            frontViewDialog_virtual::modelBase.setItem(row,19,new QStandardItem(time));//折扣时间
            frontViewDialog_virtual::modelBase.setItem(row,20,new QStandardItem(time));//赠送时间
            frontViewDialog_virtual::modelBase.setItem(row,21,new QStandardItem(QString().sprintf("%0.2f",zuofajiage)));//做法加价
            frontViewDialog_virtual::modelBase.setItem(row,22,new QStandardItem(diancai_model.record(num).value("ch_dish_typeno").toString())); //所属大类编码
            frontViewDialog_virtual::modelBase.setItem(row,23,new QStandardItem(diancai_model.record(num).value("ch_sub_typeno").toString())); //所属小类编码
            row_n = row;
            frontViewDialog_virtual::modelBase.setItem(row,24,new QStandardItem("N")); //是否落单
            frontViewDialog_virtual::modelBase.setItem(row,25,new QStandardItem("")); //落单flowid
            frontViewDialog_virtual::modelBase.setItem(row,26,new QStandardItem("0.00")); //已退量码
            //ui->tableView->tableview->selectRow(row);

            if(cook_num == "1")//自动录入点菜数量
            {
                can_add_pre=false;
                modfiy_number modfiy(frontViewDialog_virtual::modelBase.item(row,3)->text());
                /*modfiy.exec();*/backheader::static_dialogexec(modfiy, tr("点菜数量修改"));
                if(modfiy.btn_t_f == true)
                {
                    zitaocaishuling = modfiy.modfiy_num;
                    frontViewDialog_virtual::modelBase.item(row,3)->setText(QString().sprintf("%0.3f",modfiy.modfiy_num));
                    float zongjia = frontViewDialog_virtual::modelBase.item(row,3)->text().toFloat() * frontViewDialog_virtual::modelBase.item(row,4)->text().toFloat() * frontViewDialog_virtual::modelBase.item(row,5)->text().toFloat() * 0.01;
                    frontViewDialog_virtual::modelBase.item(row,6)->setText(QString().sprintf("%0.2f",zongjia));
                }
            }

            //存在特殊做法
            if(suit != "P" && suit != "Y")
            {
                zufa_model.clear();

                zufa_model.setQuery(QString("SELECT a.ch_no,a.vch_cp_memo,a.ch_typeno,a.num_add_price FROM cey_bt_cp_memo a,cey_bt_cp_dish b WHERE(a.ch_no = b.ch_no)and( b.ch_dishno = '%1')").arg(diancai_model.record(num).value("ch_dishno").toString()));
                if(zufa_model.rowCount() > 0
                        && configIniRead.value("system_setting/cook_info_pop").toBool())
                {
                    can_add_pre=false;
                    w_scr_dish_special_cook special(diancai_model.record(num).value("ch_dishno").toString(),"","0.00");
                    /*special.exec();*/backheader::static_dialogexec(special, tr("特殊做法"));
                    if(special.btn_t_f == true)
                    {
                        frontViewDialog_virtual::modelBase.item(row,7)->setText(special.zuofaneirong);
                        frontViewDialog_virtual::modelBase.item(row,21)->setText(QString().sprintf("%0.2f",special.add_price));
                        float aa;
                        aa = 0.00;
                        aa = frontViewDialog_virtual::modelBase.item(row,3)->text().toFloat() * frontViewDialog_virtual::modelBase.item(row,4)->text().toFloat() * frontViewDialog_virtual::modelBase.item(row,5)->text().toFloat() * 0.01 + frontViewDialog_virtual::modelBase.item(row,3)->text().toFloat() * special.add_price;
                        frontViewDialog_virtual::modelBase.item(row,6)->setText(QString().sprintf("%0.2f",aa));
                    }
                }
            }
            if(diancai_model.record(num).value("ch_suitflag").toString() == "Y")//存在套菜
            {
                can_add_pre=false;
                QSqlQueryModel tao;
                QSqlQueryModel model_zi;
                tao.setQuery(QString("select * from cey_bt_dish_suit where ch_suitno ='%1'").arg(diancai_model.record(num).value("ch_dishno").toString()));
                qDebug() << tao.query().lastQuery();

                for(int i = 0; i < tao.rowCount(); i++)
                {
                    QString dish_no = tao.record(i).value("ch_dishno").toString();//子套菜编码
                    zitaocai_tem.clear();
                    zitaocai_tem.setQuery(QString("select ch_tempflag from cey_bt_dish where ch_dishno = '%1' ").arg(dish_no));
                    if(zitaocai_tem.record(0).value(0).toString() == "N")
                    {
                        float zi_num = 0.00;//子套菜数量
                        if(cook_num == "0")
                        {
                            zi_num = tao.record(i).value("num_num").toFloat();
                        }
                        else
                        {
                            zi_num = tao.record(i).value("num_num").toFloat() * zitaocaishuling;
                        }
                        model_zi.clear();
                        model_zi.setQuery(QString("select ch_dishno,vch_dishname,num_price from cey_bt_dish where ch_dishno = '%1' ").arg(dish_no));
                        model_danwei.clear();
                        model_danwei.setQuery(QString("select vch_unitname, num_default from cey_bt_unit where ch_unitno = '%1' ").arg(model_zi.record(0).value("ch_unitno").toString()));
                        QString zi_danwei = model_danwei.record(0).value("vch_unitname").toString();//子套菜单位
                        int row = frontViewDialog_virtual::modelBase.rowCount();
                        frontViewDialog_virtual::modelBase.setRowCount(row + 1);
                        frontViewDialog_virtual::modelBase.setItem(row,0,new QStandardItem(QString::number(row+1))); //序号
                        frontViewDialog_virtual::modelBase.setItem(row,1,new QStandardItem(dish_no)); //编码
                        frontViewDialog_virtual::modelBase.setItem(row,2,new QStandardItem(model_zi.record(0).value("vch_dishname").toString())); //品名
                        frontViewDialog_virtual::modelBase.setItem(row,3,new QStandardItem(QString().sprintf("%0.3f",zi_num))); //数量
                        frontViewDialog_virtual::modelBase.setItem(row,4,new QStandardItem(QString().sprintf("%0.2f",model_zi.record(0).value("num_price").toFloat()))); //金额
                        frontViewDialog_virtual::modelBase.setItem(row,5,new QStandardItem("100")); //折扣值
                        frontViewDialog_virtual::modelBase.setItem(row,6,new QStandardItem("0.00")); //总价
                        frontViewDialog_virtual::modelBase.setItem(row,7,new QStandardItem(tr(""))); //特殊做法
                        frontViewDialog_virtual::modelBase.setItem(row,8,new QStandardItem("N")); //是否赠送
                        frontViewDialog_virtual::modelBase.setItem(row,9,new QStandardItem("Y")); //是否套菜
                        frontViewDialog_virtual::modelBase.setItem(row,10,new QStandardItem("N")); //是否特价
                        frontViewDialog_virtual::modelBase.setItem(row,11,new QStandardItem("N")); //是否优惠
                        frontViewDialog_virtual::modelBase.setItem(row,12,new QStandardItem(zi_danwei)); //单位
                        frontViewDialog_virtual::modelBase.setItem(row,13,new QStandardItem("")); //赠送
                        frontViewDialog_virtual::modelBase.setItem(row,14,new QStandardItem("")); //定折人
                        frontViewDialog_virtual::modelBase.setItem(row,15,new QStandardItem("N")); //是否时价菜
                        frontViewDialog_virtual::modelBase.setItem(row,16,new QStandardItem(zhutaobianma)); //子套菜所在主套菜编码
                        frontViewDialog_virtual::modelBase.setItem(row,17,new QStandardItem("0.00"));//税点
                        frontViewDialog_virtual::modelBase.setItem(row,18,new QStandardItem("N"));//是否折扣
                        frontViewDialog_virtual::modelBase.setItem(row,19,new QStandardItem(time));//折扣时间
                        frontViewDialog_virtual::modelBase.setItem(row,20,new QStandardItem(time));//赠送时间
                        frontViewDialog_virtual::modelBase.setItem(row,21,new QStandardItem(QString().sprintf("%0.2f",zuofajiage)));//做法加价
                        frontViewDialog_virtual::modelBase.setItem(row,22,new QStandardItem(diancai_model.record(num).value("ch_dish_typeno").toString())); //所属大类编码
                        frontViewDialog_virtual::modelBase.setItem(row,23,new QStandardItem(diancai_model.record(num).value("ch_sub_typeno").toString())); //所属小类编码
                        frontViewDialog_virtual::modelBase.setItem(row,24,new QStandardItem("N")); //是否落单
                        frontViewDialog_virtual::modelBase.setItem(row,25,new QStandardItem("")); //落单flowid
                        frontViewDialog_virtual::modelBase.setItem(row,26,new QStandardItem("0.00")); //已退量码
                        // //ui->tableView->tableview->selectRow(row);
                        row_n = row;
                    }
                }
            }

            if(can_add_pre
                    &&list_item_list.count()==1){//在原来的基础上数量加1
                QString ch_dishno=list_item_list.last()._dishno;
                for(int r=0,r_count=frontViewDialog_virtual::modelBase.rowCount();r<r_count;r++){
                    if(frontViewDialog_virtual::modelBase.item(r, 1)->text() == ch_dishno
                            && r!=row
                            && frontViewDialog_virtual::modelBase.item(r, 9)->text() != "Y"/*子套菜*/
                            && frontViewDialog_virtual::modelBase.item(r, 7)->text().isEmpty()/*没有做法*/){

                        query.exec(QString("select num_default from cey_bt_unit where ch_unitno = (select ch_unitno from cey_bt_dish where ch_dishno ='%1') ").arg(frontViewDialog_virtual::modelBase.item(r,1)->text()));
                        query.next();
                        double num=frontViewDialog_virtual::modelBase.item(r,3)->text().toDouble() +
                                (isweight_flag/*是称重商品*/?0.000:query.record().value("num_default").toFloat());

                        frontViewDialog_virtual::modelBase.item(r,3)->setText(QString().sprintf("%.03f", num));
                        float zongjia =
                                frontViewDialog_virtual::modelBase.item(r,3)->text().toFloat() *
                                frontViewDialog_virtual::modelBase.item(r,4)->text().toFloat() *
                                frontViewDialog_virtual::modelBase.item(r,5)->text().toFloat() * 0.01 +
                                frontViewDialog_virtual::modelBase.item(r,3)->text().toFloat() *
                                frontViewDialog_virtual::modelBase.item(r,21)->text().toFloat();
                        frontViewDialog_virtual::modelBase.item(r,6)->setText(QString().sprintf("%0.2f",zongjia));
                        row_n=r;
                        list_item_list.clear();
                        frontViewDialog_virtual::modelBase.removeRow(frontViewDialog_virtual::modelBase.rowCount()-1);
                        break;
                    }
                }
                /*如果没有找到的*/
                if(isweight_flag/*若是是称重商品*/
                        &&row < frontViewDialog_virtual::modelBase.rowCount()){
                    frontViewDialog_virtual::modelBase.item(row, 3)->setText("0.000");
                }
                break;
            }
        }
    }
        break;
    }//! swtich
}

