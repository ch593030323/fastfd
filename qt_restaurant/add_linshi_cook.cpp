#include "add_linshi_cook.h"
#include "ui_add_linshi_cook.h"
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QDebug>
#include "lds_messagebox.h"
#include "getchinesespell.h"
#include <QSqlQuery>
#include <QSqlError>

add_linshi_cook::add_linshi_cook(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::add_linshi_cook)
{
    ui->setupUi(this);
    //this->setWindowFlags(Qt::FramelessWindowHint|Qt::Dialog);this->setStyleSheet("QDialog{border:2px solid gray}");
    btn_t_f = false;
    init_control();
}

add_linshi_cook::~add_linshi_cook()
{
    delete ui;
}

void add_linshi_cook::init_control()
{
    QSqlQueryModel model;
    model.setQuery(QString("SELECT  * FROM cey_bt_dish_type"));
    QPixmap pixmap(1, 40);
    pixmap.fill(Qt::transparent);
    QIcon icon(pixmap);
    ui->comboBox_ch_unitno_5->setIconSize(QSize(1, 40));//菜品大类
    for(int i = 0; i < model.rowCount(); i++)
    {
        ui->comboBox_ch_unitno_5->addItem(icon,model.record(i).value(1).toString());
    }

    //单位
    model.clear();
    model.setQuery(QString("select * from cey_bt_unit"));
    ui->comboBox_ch_unitno_4->setIconSize(QSize(1, 40));
    for(int i = 0; i < model.rowCount(); i++)
    {
        ui->comboBox_ch_unitno_4->addItem(icon,model.record(i).value(1).toString());
    }

    //采购单位
    ui->comboBox_vch_pur_unitno_4->setEnabled(false);
    model.clear();
    model.setQuery(QString("select * from cey_bt_unit"));
    ui->comboBox_vch_pur_unitno_4->setIconSize(QSize(1, 40));//菜品大类
    for(int i = 0; i < model.rowCount(); i++)
    {
        ui->comboBox_vch_pur_unitno_4->addItem(icon,model.record(i).value(1).toString());
    }
    ui->checkBox_ch_discount_4->setChecked(false);
    ui->spinBox_int_unit_rate_4->setEnabled(false);
    ui->lineEdit_num_sale_price_4->setEnabled(false);
    ui->checkBox_ch_suitflag_4->setEnabled(false);
    ui->checkBox_ch_tempflag_4->setEnabled(false);
    ui->checkBox_ch_tempflag_4->setChecked(true);
    ui->comboBox_ch_deducttype_4->setEnabled(false);
    ui->comboBox_ch_deducttype_4->setIconSize(QSize(1, 40));
    ui->comboBox_ch_deducttype_4->addItem(icon,tr("比例提成"));
    ui->comboBox_ch_deducttype_4->addItem(icon,tr("定额提成"));
    ui->lineEdit_int_deduct_4->setEnabled(false);
    ui->lineEdit_num_deduct_4->setEnabled(false);

    QValidator *validator=new QIntValidator(1,99999,this); // 0-100 only accept number
    ui->lineEdit_ch_dishno_4->setValidator(validator);

    QValidator *validator1 =new QIntValidator(0,100,this); // 0-100 only accept number
    ui->lineEdit_int_deduct_4->setValidator(validator1);
    ui->lineEdit_num_deduct_4->setValidator(validator1);
}

void add_linshi_cook::on_pushButton_15_clicked()
{
    btn_t_f = false;
    this->close();
}

void add_linshi_cook::on_comboBox_ch_unitno_5_currentIndexChanged(const QString &arg1)
{
    ui->comboBox_ch_unitno_6->clear();
    QSqlQueryModel xiaolei;
    QSqlQueryModel model;
    model.setQuery(QString("select ch_dish_typeno from cey_bt_dish_type where vch_dish_typename ='%1'  ").arg(arg1));
    xiaolei.setQuery(QString("select vch_sub_typename from cey_bt_dish_subtype where ch_dish_typeno = '%1' ").arg(model.record(0).value(0).toString()));
    QPixmap pixmap(1, 50);
    pixmap.fill(Qt::transparent);
    QIcon icon(pixmap);
    ui->comboBox_ch_unitno_6->setIconSize(QSize(1, 50));
    for(int i = 0; i < xiaolei.rowCount(); i++)
    {
        ui->comboBox_ch_unitno_6->addItem(icon,xiaolei.record(i).value(0).toString());
    }
}

void add_linshi_cook::on_checkBox_ch_deductflag_4_clicked()
{
    if(ui->checkBox_ch_deductflag_4->isChecked() == true)
    {
        ui->comboBox_ch_deducttype_4->setEnabled(true);
        QString arg1 = ui->comboBox_ch_deducttype_4->currentText();
        if(arg1 == tr("比例提成"))
        {
            ui->lineEdit_int_deduct_4->setEnabled(true);
            ui->lineEdit_num_deduct_4->setEnabled(false);
        }
        else
        {
            ui->lineEdit_int_deduct_4->setEnabled(false);
            ui->lineEdit_num_deduct_4->setEnabled(true);
        }
    }
    else
    {
        ui->comboBox_ch_deducttype_4->setEnabled(false);
        ui->lineEdit_int_deduct_4->setEnabled(false);
        ui->lineEdit_num_deduct_4->setEnabled(false);
    }
}

void add_linshi_cook::on_pushButton_7_clicked()
{
    QSqlQueryModel model;
    ;
    QSqlQuery query;
    QSqlDatabase::database().transaction();
    QString bianma = ui->lineEdit_ch_dishno_4->text();
    if(bianma.isEmpty())
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("菜品编码不能为空!"));
        
        return;
    }

    QString pinming = ui->lineEdit_vch_dishname_4->text();
    if(pinming.isEmpty())
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("菜品名称不能为空!"));
        
        return;
    }

    model.setQuery(QString("select ch_dishno from cey_bt_dish"));
    for(int i = 0; i < model.rowCount(); i++)
    {
        if(model.record(i).value(0).toString() == bianma)
        {
            lds_messagebox::warning(this, tr("消息提示"),tr("该编码的菜品已存在!"));
            
            return;
        }
    }

    QString dalei;
    QString xiaolei;
    model.clear();
    model.setQuery(QString("select ch_dish_typeno from cey_bt_dish_type where vch_dish_typename = '%1' ").arg(ui->comboBox_ch_unitno_5->currentText()));
    dalei = model.record(0).value(0).toString();

    model.clear();
    model.setQuery(QString("select ch_sub_typeno from cey_bt_dish_subtype where vch_sub_ytpename = '%1' ").arg(ui->comboBox_ch_unitno_6->currentText()));
    xiaolei = model.record(0).value(0).toString();

    QString  spell = ui->lineEdit_vch_spell_4->text();
    QString fuzhubiama = ui->lineEdit_vch_subno_4->text();
    QString jiage = ui->lineEdit_num_price_4->text();
    QString danwie;
    model.clear();
    model.setQuery(QString("select ch_unitno from cey_bt_unit where vch_unitname ='%1' ").arg(ui->comboBox_ch_unitno_4->currentText()));
    danwie = model.record(0).value(0).toString();

    //直销商品
    QString zhi_1;
    QString zhi_2;
    QString zhi_3;
    QString zhi_4;
    if(ui->checkBox_item_flag_4->isChecked() == false)
    {
        zhi_1 = "N";
        zhi_2.clear();
        zhi_3.clear();
        zhi_4.clear();
    }
    else
    {
        model.clear();
        model.setQuery(QString("select ch_unitno from cey_bt_unit where vch_unitname ='%1' ").arg(ui->comboBox_vch_pur_unitno_4->currentText()));
        danwie = model.record(0).value(0).toString();
        zhi_1 = "Y";
        zhi_2 = model.record(0).value(0).toString();
        zhi_3 = QString::number(ui->spinBox_int_unit_rate_4->value());
        zhi_4 = ui->lineEdit_num_sale_price_4->text();
    }

    QString taocai;
    if(ui->checkBox_ch_suitflag_4->isChecked() == true)
    {
        taocai = "Y";
    }
    else
    {
        taocai = "N";
    }

    QString shijia;
    if(ui->checkBox_ch_curprice_4->isChecked() == true)
    {
        shijia = "Y";
    }
    else
    {
        shijia = "N";
    }

    QString linshi  = "Y";
    QString changyongcai;
    if(ui->checkBox_ch_off_4->isChecked() == true)
    {
        changyongcai = "Y";
    }
    else
    {
        changyongcai = "N";
    }

    QString zhekou;
    if(ui->checkBox_ch_discount_4->isChecked() == true)
    {
        zhekou = "Y";
    }
    else
    {
        zhekou = "N";
    }

    QString fuwufei;
    if(ui->checkBox_ch_serviceflag_4->isChecked() == true)
    {
        fuwufei = "Y";
    }
    else
    {
        fuwufei = "N";
    }

    QString dixiao;
    if(ui->checkBox_ch_lowflag_4->isChecked() == true)
    {
        dixiao = "Y";
    }
    else
    {
        dixiao = "N";
    }

    QString tiyong;
    if(ui->checkBox_ch_stopflag_4->isChecked() == true)
    {
        tiyong = "Y";
    }
    else
    {
        tiyong = "N";
    }

    QString fuwuticheng;
    QString tichengfangshi;
    QString bili;
    QString dinge;
    if(ui->checkBox_ch_deductflag_4->isChecked() == true)
    {
        fuwuticheng = "Y";
        if(ui->comboBox_ch_deducttype_4->currentText() == tr("比例提成"))
        {
            tichengfangshi.clear();
            bili = ui->lineEdit_int_deduct_4->text();
            dinge.clear();
        }
    }
    else
    {
        fuwuticheng = "N";
        if(ui->comboBox_ch_deducttype_4->currentText() == tr("比例提成"))
        {
            tichengfangshi.clear();
            bili.clear();
            dinge = ui->lineEdit_num_deduct_4->text();
        }
    }

    QString chuda;
    QString ip;
    if(ui->checkBox_ch_suitflag_5->isChecked() == true)
    {
        chuda = "Y";
        ip = ui->lineEdit_vch_sub_typename_5->text();
    }
    else
    {
        chuda = "N";
        ip.clear();
    }

    QString shuilv = QString::number(ui->spinBox_int_rate_4->value());
    if(ui->checkBox_item_flag_4->isChecked() == true)
    {
        query.prepare(QString("insert into cey_bt_dish (ch_dishno,ch_dish_typeno,ch_sub_typeno,vch_dishname,vch_spell,vch_subno,num_price,ch_unitno,item_flag,vch_pur_unitno,int_unit_rate,num_sale_price,ch_suitflag,ch_curprice,ch_tempflag,ch_off,ch_discount,ch_serviceflag,ch_lowflag,ch_stopflag,ch_deductflag,ch_deducttype,int_deduct,num_deduct,ch_printflag,vch_printer,int_rate) values(:str1,:str2,:str3,:str4,:str5,:str6,:str7,:str8,:str9,:str10,:str11,:str12,:str13,:str14,:str15,:str16,:str17,:str18,:str19,:str20,:str21,:str22,:str23,:str24,:str25,:str26,:str27)"));
        query.bindValue(":str1",bianma);
        query.bindValue(":str2",dalei);
        query.bindValue(":str3",xiaolei);
        query.bindValue(":str4",pinming);
        query.bindValue(":str5",spell);
        query.bindValue(":str6",fuzhubiama);
        query.bindValue(":str7",QString().sprintf("%0.2f",jiage.toFloat()));
        query.bindValue(":str8",danwie);
        query.bindValue(":str9",zhi_1);
        query.bindValue(":str10",zhi_2);
        query.bindValue(":str11",zhi_3);
        query.bindValue(":str12",zhi_4);
        query.bindValue(":str13",taocai);
        query.bindValue(":str14",shijia);
        query.bindValue(":str15",linshi);
        query.bindValue(":str16",changyongcai);
        query.bindValue(":str17",zhekou);
        query.bindValue(":str18",fuwufei);
        query.bindValue(":str19",dixiao);
        query.bindValue(":str20",tiyong);
        query.bindValue(":str21",fuwuticheng);
        query.bindValue(":str22",tichengfangshi);
        query.bindValue(":str23",bili);
        query.bindValue(":str24",dinge);
        query.bindValue(":str25",chuda);
        query.bindValue(":str26",ip);
        query.bindValue(":str27",shuilv);
    }
    else
    {
        query.prepare(QString("insert into cey_bt_dish (ch_dishno,ch_dish_typeno,ch_sub_typeno,vch_dishname,vch_spell,vch_subno,num_price,ch_unitno,item_flag,vch_pur_unitno,int_unit_rate,num_sale_price,ch_suitflag,ch_curprice,ch_tempflag,ch_off,ch_discount,ch_serviceflag,ch_lowflag,ch_stopflag,ch_deductflag,ch_deducttype,int_deduct,num_deduct,ch_printflag,vch_printer,int_rate) values(:str1,:str2,:str3,:str4,:str5,:str6,:str7,:str8,:str9,null,null,:str12,:str13,:str14,:str15,:str16,:str17,:str18,:str19,:str20,:str21,:str22,:str23,:str24,:str25,:str26,:str27)"));
        query.bindValue(":str1",bianma);
        query.bindValue(":str2",dalei);
        query.bindValue(":str3",xiaolei);
        query.bindValue(":str4",pinming);
        query.bindValue(":str5",spell);
        query.bindValue(":str6",fuzhubiama);
        query.bindValue(":str7",QString().sprintf("%0.2f",jiage.toFloat()));
        query.bindValue(":str8",danwie);
        query.bindValue(":str9",zhi_1);
        query.bindValue(":str12",zhi_4);
        query.bindValue(":str13",taocai);
        query.bindValue(":str14",shijia);
        query.bindValue(":str15",linshi);
        query.bindValue(":str16",changyongcai);
        query.bindValue(":str17",zhekou);
        query.bindValue(":str18",fuwufei);
        query.bindValue(":str19",dixiao);
        query.bindValue(":str20",tiyong);
        query.bindValue(":str21",fuwuticheng);
        query.bindValue(":str22",tichengfangshi);
        query.bindValue(":str23",bili);
        query.bindValue(":str24",dinge);
        query.bindValue(":str25",chuda);
        query.bindValue(":str26",ip);
        query.bindValue(":str27",shuilv);
    }

    //    query.prepare(QString("insert into cey_bt_dish (ch_dishno,ch_dish_typeno,ch_sub_typeno,vch_dishname,vch_spell,vch_subno,num_price,ch_unitno,item_flag,vch_pur_unitno,int_unit_rate,num_sale_price,ch_suitflag,ch_curprice,ch_tempflag,ch_off,ch_discount,ch_serviceflag,ch_lowflag,ch_stopflag,ch_deductflag,ch_deducttype,int_deduct,num_deduct,ch_printflag,vch_printer,int_rate) values('%1','%2','%3','%4','%5','%6','%7','%8','%9','%10','%11','%12','%13','%14','%15','%16','%17','%18','%19','%20','%21','%22','%23','%24','%25','%26','%27')")
    //                  .arg(bianma).arg(dalei).arg(xiaolei).arg(pinming).arg(spell)
    //                  .arg(fuzhubiama).arg(QString().sprintf("%0.2f",jiage.toFloat())).arg(danwie).arg(zhi_1).arg(zhi_2)
    //                  .arg(zhi_3).arg(zhi_4).arg(taocai).arg(shijia).arg(linshi)
    //                  .arg(changyongcai).arg(zhekou).arg(fuwufei).arg(dixiao).arg(tiyong)
    //                  .arg(fuwuticheng).arg(tichengfangshi).arg(bili).arg(dinge).arg(chuda)
    //                  .arg(ip).arg(shuilv));
    //    if(query.exec(QString("insert into cey_bt_dish (ch_dishno,ch_dish_typeno,ch_sub_typeno,vch_dishname,vch_spell,vch_subno,num_price,ch_unitno,item_flag,vch_pur_unitno,int_unit_rate,num_sale_price,ch_suitflag,ch_curprice,ch_tempflag,ch_off,ch_discount,ch_serviceflag,ch_lowflag,ch_stopflag,ch_deductflag,ch_deducttype,int_deduct,num_deduct,ch_printflag,vch_printer,int_rate) values('%1','%2','%3','%4','%5','%6','%7','%8','%9','%10','%11','%12','%13','%14','%15','%16','%17','%18','%19','%20','%21','%22','%23','%24','%25','%26','%27')")
    //                  .arg(bianma).arg(dalei).arg(xiaolei).arg(pinming).arg(spell)
    //                  .arg(fuzhubiama).arg(QString().sprintf("%0.2f",jiage.toFloat())).arg(danwie).arg(zhi_1).arg(zhi_2)
    //                  .arg(zhi_3).arg(zhi_4).arg(taocai).arg(shijia).arg(linshi)
    //                  .arg(changyongcai).arg(zhekou).arg(fuwufei).arg(dixiao).arg(tiyong)
    //                  .arg(fuwuticheng).arg(tichengfangshi).arg(bili).arg(dinge).arg(chuda)
    //                  .arg(ip).arg(shuilv)))
    if(query.exec())
    {
    }
    else
    {
        QSqlDatabase::database().rollback();
                   qDebug() << query.lastError();  lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_bt_dish失败!"));
        
        return;
    }
    QSqlDatabase::database().commit();
    btn_t_f = true;
    this->close();
}

void add_linshi_cook::on_lineEdit_vch_dishname_4_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
    {
        return;
    }

    QString spell=getChineseSpell::ChineseSpell(arg1);
    spell=spell.toUpper();
    ui->lineEdit_vch_spell_4->setText(spell);
    ui->lineEdit_vch_subno_4->setText(spell);
}


void add_linshi_cook::on_comboBox_ch_deducttype_4_currentIndexChanged(const QString &arg1)
{
    if(arg1 == tr("比例提成"))
    {
        ui->lineEdit_int_deduct_4->setEnabled(true);
        ui->lineEdit_num_deduct_4->setEnabled(false);
    }
    else
    {
        ui->lineEdit_int_deduct_4->setEnabled(false);
        ui->lineEdit_num_deduct_4->setEnabled(true);
    }
}

void add_linshi_cook::on_checkBox_item_flag_4_clicked()
{
    if(ui->checkBox_item_flag_4->isChecked() == true)
    {
        ui->comboBox_vch_pur_unitno_4->setEnabled(true);
        ui->spinBox_int_unit_rate_4->setEnabled(true);
        ui->lineEdit_num_sale_price_4->setEnabled(true);
    }
    else
    {
        ui->comboBox_vch_pur_unitno_4->setEnabled(false);
        ui->spinBox_int_unit_rate_4->setEnabled(false);
        ui->lineEdit_num_sale_price_4->setEnabled(false);
    }
}
