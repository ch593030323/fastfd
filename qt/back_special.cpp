#include "back_special.h"
#include "ui_back_special.h"
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QDebug>
#include "back_special_add.h"
#include "n_func.h"
#include "lds_messagebox.h"
#include "modify_view.h"
#include <QSqlQuery>
#include <QDesktopWidget>
#include <QApplication>
#include <QSqlError>
#include "backheader.h"
#include "lds_messagebox.h"

back_special::back_special(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::back_special)
{
    ui->setupUi(this);
    //this->setWindowFlags(Qt::FramelessWindowHint|Qt::Dialog);this->setStyleSheet("QDialog{border:2px solid gray}");
    ui->dateTimeEdit->setDateTime(QDateTime(n_func::f_get_sysdatetime().date(), QTime(0,0,0)));
    ui->dateTimeEdit_2->setDateTime(QDateTime(n_func::f_get_sysdatetime().date(), QTime(23,59,59)));

    init_control();
}

back_special::~back_special()
{
    delete ui;
}

void back_special::init_control()
{
    ui->radioButton->setChecked(true);
    ui->radioButton_2->setChecked(false);

    //ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置属性为只读
    QStringList labels;
    labels<<tr("行号")<<tr("品码")<<tr("品名")<<tr("品类")<<tr("单位")<<tr("原价")<<tr("特价")<<tr("建档人")<<tr("建档时间")<<tr("生效时间");
    table_model = new QStandardItemModel();
    table_model->setHorizontalHeaderLabels(labels);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);ui->tableView->setModel(table_model);

    QSqlQueryModel model;
    model.setQuery("Select NOW()");
    QDateTime time =model.record(0).value(0).toDateTime();
    QString str = time.toString("yyyy-MM-dd");
    QString str1 = "00:00";
    QString str2 = str + " " + str1;
    QDateTime time_box1;
    time_box1 = QDateTime::fromString(str,"yyyy-MM-dd");
    ui->dateTimeEdit->setDateTime(time_box1);
    ui->dateTimeEdit_2->setDateTime(time_box1);
    //QString date1 = ui->dateTimeEdit->dateTime().toString("yyyy-MM-dd hh:mm");
    // QString date2 = ui->dateTimeEdit_2->dateTime().toString("yyyy-MM-dd hh:mm");

    model.setQuery(QString("SELECT a.int_flowid,a.ch_dishno, b.vch_dishname, c.vch_dish_typename, d.vch_unitname,a.num_price, b.num_price, a.vch_operID,a.dt_operdate, a.dt_operdate_effect,a.ch_state  FROM cey_bt_unit d,cey_bt_dish_type c,cey_bt_dish b, cey_bt_dish_special a WHERE (b.ch_unitno=d.ch_unitno)and (b.ch_dish_typeno=c.ch_dish_typeno)and(b.ch_dishno = a.ch_dishno)and (( a.dt_operdate_effect = '%1')AND (a.ch_state = 'Y'))").arg(str));
    for(int i = 0; i < model.rowCount(); i++)
    {
        int row = table_model->rowCount();
        table_model->setRowCount(row + 1);
        table_model->setItem(row,0,new QStandardItem(QString::number(row+1))); //序号
        table_model->setItem(row,1,new QStandardItem(model.record(i).value(1).toString())); //编码
        table_model->setItem(row,2,new QStandardItem(model.record(i).value(2).toString())); //品名
        table_model->setItem(row,3,new QStandardItem(model.record(i).value(3).toString())); //品类
        table_model->setItem(row,4,new QStandardItem(model.record(i).value(4).toString())); //单位
        table_model->setItem(row,5,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(6).toFloat()))); //原价
        table_model->setItem(row,6,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(5).toFloat()))); //特价
        table_model->setItem(row,7,new QStandardItem(model.record(i).value(7).toString())); //建档人
        table_model->setItem(row,8,new QStandardItem(model.record(i).value(8).toDateTime().toString("yyyy-MM-dd hh:mm:ss"))); //建档时间
        table_model->setItem(row,9,new QStandardItem(model.record(i).value(9).toDateTime().toString("yyyy-MM-dd hh:mm:ss")));  //生效时间
        table_model->setItem(row,10,new QStandardItem(model.record(i).value(10).toString())); //状态
        table_model->setItem(row,11,new QStandardItem(model.record(i).value(0).toString())); //行号
    }



    if(table_model->rowCount() > 0)
    {
        ui->tableView->selectRow(0);
        ui->tableView->setColumnHidden(10,true);
        ui->tableView->setColumnHidden(11,true);
    }

}

void back_special::on_pushButton_16_clicked()
{
    if(xinzeng.count() > 0 || shanchu.count() > 0)//数据有改动
    {
        switch(lds_messagebox::warning(this, tr("消息提示"),tr("数据有改动,确认保存?"),tr("确认"),tr("取消"),tr("返回"))){
        case 0:thrid_true();break;
        case 1:thrid_false();break;
        case 2:return;
        }
    }
    this->close();
}

void back_special::on_pushButton_10_clicked()//新增特价
{
    if(ui->dateTimeEdit->dateTime() > ui->dateTimeEdit_2->dateTime())
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("生效日期不能小于当前日期!"));
        
        return;
    }

    if(ui->radioButton->isChecked() == true)
    {
        back_special_add b_special(this);
        backheader::static_dialogexec(b_special, tr("新增特价"));
        QSqlQueryModel model;
        QSqlQueryModel danwei;
        QSqlQueryModel pinlei;
        model.setQuery(QString("select NOW()"));
        QString time = model.record(0).value(0).toDateTime().toString("yyyy-MM-dd hh:mm:ss");
        QString shengxiao = ui->dateTimeEdit_2->dateTime().toString("yyyy-MM-dd");
        if(b_special.btn_t_f == true)
        {
            if(b_special.yingyong.count() > 0)//存在应用
            {
                for(int i = 0; i < b_special.yingyong.count(); i++)
                {
                    if(!tejialist.contains(b_special.yingyong[i]))
                    {
                        tejialist << b_special.yingyong[i];
                        xinzeng << b_special.yingyong[i];

                        model.clear();
                        model.setQuery(QString("select vch_dishname,ch_dish_typeno,ch_unitno,num_price from cey_bt_dish where ch_dishno = '%1' ").arg(b_special.yingyong[i]));
                        int row = table_model->rowCount();
                        table_model->setRowCount(row + 1);
                        table_model->setItem(row,0,new QStandardItem(QString::number(row+1))); //序号
                        table_model->setItem(row,1,new QStandardItem(b_special.yingyong[i])); //编码
                        table_model->setItem(row,2,new QStandardItem(model.record(0).value(0).toString())); //品名

                        pinlei.clear();
                        pinlei.setQuery(QString("select vch_dish_typename from cey_bt_dish_type where ch_dish_typeno ='%1' ").arg(model.record(0).value(1).toString()));
                        table_model->setItem(row,3,new QStandardItem(pinlei.record(0).value(0).toString())); //品类

                        danwei.clear();
                        danwei.setQuery(QString("select vch_unitname from cey_bt_unit where ch_unitno ='%1' ").arg(model.record(0).value(2).toString()));
                        table_model->setItem(row,4,new QStandardItem(danwei.record(0).value(0).toString())); //单位

                        float jiage = 0.00;
                        jiage = model.record(0).value(3).toFloat();
                        table_model->setItem(row,5,new QStandardItem(QString().sprintf("%0.2f",jiage)));//原价
                        table_model->setItem(row,6,new QStandardItem(QString().sprintf("%0.2f",jiage)));//特价
                        table_model->setItem(row,7,new QStandardItem(n_func::gs_operid));//建档人
                        table_model->setItem(row,8,new QStandardItem(time));//建档时间
                        table_model->setItem(row,9,new QStandardItem(shengxiao));//生效时间
                        table_model->setItem(row,10,new QStandardItem("N"));//状态
                    }
                }
            }
            else//不存在应用
            {
                for(int i = 0; i < b_special.truelist.count(); i++)
                {
                    if(!tejialist.contains(b_special.truelist[i]))
                    {
                        tejialist << b_special.truelist[i];
                        xinzeng << b_special.truelist[i];

                        model.clear();
                        model.setQuery(QString("select vch_dishname,ch_dish_typeno,ch_unitno,num_price from cey_bt_dish where ch_dishno = '%1' ").arg(b_special.truelist[i]));
                        int row = table_model->rowCount();
                        table_model->setRowCount(row + 1);
                        table_model->setItem(row,0,new QStandardItem(QString::number(row+1))); //序号
                        table_model->setItem(row,1,new QStandardItem(b_special.truelist[i])); //编码
                        table_model->setItem(row,2,new QStandardItem(model.record(0).value(0).toString())); //品名

                        pinlei.clear();
                        pinlei.setQuery(QString("select vch_dish_typename from cey_bt_dish_type where ch_dish_typeno ='%1' ").arg(model.record(0).value(1).toString()));
                        table_model->setItem(row,3,new QStandardItem(pinlei.record(0).value(0).toString())); //品类

                        danwei.clear();
                        danwei.setQuery(QString("select vch_unitname from cey_bt_unit where ch_unitno ='%1' ").arg(model.record(0).value(2).toString()));
                        table_model->setItem(row,4,new QStandardItem(danwei.record(0).value(0).toString())); //单位

                        float jiage = 0.00;
                        jiage = model.record(0).value(3).toFloat();
                        table_model->setItem(row,5,new QStandardItem(QString().sprintf("%0.2f",jiage)));//原价
                        table_model->setItem(row,6,new QStandardItem(QString().sprintf("%0.2f",jiage)));//特价
                        table_model->setItem(row,7,new QStandardItem(n_func::gs_operid));//建档人
                        table_model->setItem(row,8,new QStandardItem(time));//建档时间
                        table_model->setItem(row,9,new QStandardItem(shengxiao));//生效时间
                        table_model->setItem(row,10,new QStandardItem("N"));//状态
                    }
                }
            }
        }
        else
        {
            return;
        }


        if(table_model->rowCount() > 0)
        {
            ui->tableView->selectRow(0);
            ui->tableView->setColumnHidden(10,true);
            ui->tableView->setColumnHidden(11,true);
        }

    }
}

void back_special::on_pushButton_11_clicked()//删除
{
    int row = ui->tableView->currentIndex().row();
    QSqlQueryModel model;
    model.setQuery(QString("select NOW()"));
    QString time = model.record(0).value(0).toDateTime().toString("yyyy-MM-dd hh:mm:ss");
    ;
    if(row <= -1)
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("请选择一条要删除的数据!"));
        
        return;
    }

    if(table_model->item(row,10)->text() == "Y")
    {
        shanchu << table_model->item(row,10)->text();
        shanchu_id << table_model->item(row,11)->text();
        shanchutime << time;
        table_model->removeRow(row);
    }
    else
    {
        for(int i = 0; i < xinzeng.count(); i++)
        {
            if(xinzeng[i] == table_model->item(row,1)->text())
            {
                xinzeng.removeAt(i);
            }
        }
        table_model->removeRow(row);
    }

    for(int i = 0; i < table_model->rowCount(); i++)
    {
        table_model->item(i,0)->setText(QString::number(i+1));
    }
    if(table_model->rowCount() > 0)
    {
        ui->tableView->selectRow(0);
    }
}

void back_special::on_pushButton_12_clicked()//修改特价
{
    int row = ui->tableView->currentIndex().row();
    ;
    if(row <= -1)
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("请选择一条要修改的数据!"));
        
        return;
    }

    if(table_model->item(row,10)->text() == tr("Y"))
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("该特价已经保存,不能修改!"));
        
        return;
    }

    modify_view view(table_model->item(row,6)->text());
    backheader::static_dialogexec(view, tr("修改"));

    if(view.btn_t_f == true)
    {
        table_model->item(row,6)->setText(QString().sprintf("%0.2f",view.jiage_m.toFloat()));
    }
}

void back_special::on_pushButton_13_clicked()//保存
{
     ;
    if(xinzeng.count() == 0 && shanchu.count() == 0)
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("保存成功!"));
        
        return;
    }

    QSqlQueryModel model;
    model.setQuery(QString("select NOW()"));
    QString riqi = model.record(0).value(0).toDateTime().toString("yyyy-MM-dd");

    QSqlQuery query;
    QSqlDatabase::database().transaction();
    QString shengxiao = ui->dateTimeEdit_2->dateTime().toString("yyyy-MM-dd");
    for(int i = 0; i < shanchu.count(); i++)
    {
        query.prepare(QString("UPDATE cey_bt_dish_special SET ch_state = 'N', vch_operID2 = :shanchu, dt_operdate2 = :time  WHERE int_flowID = :id "));
        query.bindValue(":shanchu",n_func::gs_operid);
        query.bindValue(":time",shanchutime[i]);
        query.bindValue(":id",shanchu_id[i]);
        if(query.exec())
        {

        }
        else
        {
            QSqlDatabase::database().rollback();
            lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_bt_dish_special失败!"));
            
            return;
        }
    }

    for(int k = 0; k < xinzeng.count(); k++)
    {
        for(int m = 0; m < table_model->rowCount(); m++)
        {
            if(table_model->item(m,1)->text() == xinzeng[k] && table_model->item(m,10)->text() == "N")
            {
                query.prepare(QString("insert into cey_bt_dish_special(ch_state,ch_dishno,num_price,vch_operID,dt_operdate,dt_operdate_effect,dt_date)  values('Y',:bianma,:price,:oper,:dangtime,:shengxiaotime,:danqianriqi)"));
                query.bindValue(":bianma",xinzeng[k]);
                query.bindValue(":price",table_model->item(m,6)->text());
                query.bindValue(":oper",table_model->item(m,7)->text());
                query.bindValue(":dangtime",table_model->item(m,8)->text());
                query.bindValue(":shengxiaotime",shengxiao);
                query.bindValue(":danqianriqi",riqi);

                if(query.exec())
                {

                }
                else
                {
                    QSqlDatabase::database().rollback();
                               qDebug() << query.lastError();  lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_bt_dish_special失败!"));
                    
                    return;
                }
            }
        }
    }

    if(xinzeng.count() > 0 || shanchu.count() > 0)
    {
         QSqlDatabase::database().commit();
         lds_messagebox::warning(this, tr("消息提示"),tr("保存成功!"));
         
         shanchu.clear();
         shanchutime.clear();
         shanchu_id.clear();
         xinzeng.clear();
    }

    table_model->removeRows(0,table_model->rowCount());
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);ui->tableView->setModel(table_model);


    model.setQuery("Select NOW()");
    QDateTime time =model.record(0).value(0).toDateTime();
    QString str = time.toString("yyyy-MM-dd");
    // QString str1 = "00:00";
    // QString str2 = str + " " + str1;
    //  QDateTime time_box1;
    //  time_box1 = QDateTime::fromString(str,"yyyy-MM-dd");


    model.setQuery(QString("SELECT a.int_flowid,a.ch_dishno, b.vch_dishname, c.vch_dish_typename, d.vch_unitname,a.num_price, b.num_price, a.vch_operID,a.dt_operdate, a.dt_operdate_effect,a.ch_state  FROM cey_bt_unit d,cey_bt_dish_type c,cey_bt_dish b, cey_bt_dish_special a WHERE (b.ch_unitno=d.ch_unitno)and (b.ch_dish_typeno=c.ch_dish_typeno)and(b.ch_dishno = a.ch_dishno)and (( a.dt_operdate_effect = '%1')AND (a.ch_state = 'Y'))").arg(str));
    for(int i = 0; i < model.rowCount(); i++)
    {
        int row = table_model->rowCount();
        table_model->setRowCount(row + 1);
        table_model->setItem(row,0,new QStandardItem(QString::number(row+1))); //序号
        table_model->setItem(row,1,new QStandardItem(model.record(i).value(1).toString())); //编码
        table_model->setItem(row,2,new QStandardItem(model.record(i).value(2).toString())); //品名
        table_model->setItem(row,3,new QStandardItem(model.record(i).value(3).toString())); //品类
        table_model->setItem(row,4,new QStandardItem(model.record(i).value(4).toString())); //单位
        table_model->setItem(row,5,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(6).toFloat()))); //原价
        table_model->setItem(row,6,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(5).toFloat()))); //特价
        table_model->setItem(row,7,new QStandardItem(model.record(i).value(7).toString())); //建档人
        table_model->setItem(row,8,new QStandardItem(model.record(i).value(8).toDateTime().toString("yyyy-MM-dd hh:mm:ss"))); //建档时间
        table_model->setItem(row,9,new QStandardItem(model.record(i).value(9).toDateTime().toString("yyyy-MM-dd hh:mm:ss")));  //生效时间
        table_model->setItem(row,10,new QStandardItem(model.record(i).value(10).toString())); //状态
        table_model->setItem(row,11,new QStandardItem(model.record(i).value(0).toString())); //行号
    }



    if(table_model->rowCount() > 0)
    {
        ui->tableView->selectRow(0);
        ui->tableView->setColumnHidden(10,true);
        ui->tableView->setColumnHidden(11,true);
    }
}

void back_special::on_pushButton_14_clicked()//刷新
{
    if(xinzeng.count() > 0 || shanchu.count() > 0)//数据有改动
    {
        switch(lds_messagebox::warning(this, tr("消息提示"),tr("数据有改动,确认保存?"),tr("确认"),tr("取消"),tr("返回"))){
        case 0:thrid_true();break;
        case 1:thrid_false();break;
        case 2:thrid_cancel();break;
        }
    }
}

void back_special::thrid_true()//数据有改动且保存
{
    QSqlQueryModel model;
    model.setQuery(QString("select NOW()"));
    QString riqi = model.record(0).value(0).toDateTime().toString("yyyy-MM-dd");
    ;
    QSqlQuery query;
    QSqlDatabase::database().transaction();
    QString shengxiao = ui->dateTimeEdit_2->dateTime().toString("yyyy-MM-dd");
    for(int i = 0; i < shanchu.count(); i++)
    {
        query.prepare(QString("UPDATE cey_bt_dish_special SET ch_state = 'N', vch_operID2 = :shanchu, dt_operdate2 = :time  WHERE int_flowID = :id "));
        query.bindValue(":shanchu",n_func::gs_operid);
        query.bindValue(":time",shanchutime[i]);
        query.bindValue(":id",shanchu_id[i]);
        if(query.exec())
        {

        }
        else
        {
            QSqlDatabase::database().rollback();
            lds_messagebox::warning(this, tr("消息提示"),tr("更新cey_bt_dish_special失败!"));
            
            return;
        }
    }

    for(int k = 0; k < xinzeng.count(); k++)
    {
        for(int m = 0; m < table_model->rowCount(); m++)
        {
            if(table_model->item(m,1)->text() == xinzeng[k] && table_model->item(m,10)->text() == "N")
            {

                query.prepare(QString("insert into cey_bt_dish_special(ch_state,ch_dishno,num_price,vch_operID,dt_operdate,dt_operdate_effect,dt_date)  values('Y',:bianma,:price,:oper,:dangtime,:shengxiaotime,:danqianriqi)"));
                query.bindValue(":bianma",xinzeng[k]);
                query.bindValue(":price",table_model->item(m,6)->text());
                query.bindValue(":oper",table_model->item(m,7)->text());
                query.bindValue(":dangtime",table_model->item(m,8)->text());
                query.bindValue(":shengxiaotime",shengxiao);
                query.bindValue(":danqianriqi",riqi);

                if(query.exec())
                {

                }
                else
                {
                    QSqlDatabase::database().rollback();
                               qDebug() << query.lastError();  lds_messagebox::warning(this, tr("消息提示"),tr("插入cey_bt_dish_special失败!"));
                    
                    return;
                }
            }
        }
    }

    if(xinzeng.count() > 0 || shanchu.count() > 0)
    {
         QSqlDatabase::database().commit();
         lds_messagebox::warning(this, tr("消息提示"),tr("保存成功!"));
         
         shanchu.clear();
         shanchutime.clear();
         shanchu_id.clear();
         xinzeng.clear();
    }

    table_model->removeRows(0,table_model->rowCount());
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);ui->tableView->setModel(table_model);


    model.setQuery("Select NOW()");
    QDateTime time =model.record(0).value(0).toDateTime();
    QString str = time.toString("yyyy-MM-dd");
    // QString str1 = "00:00";
    // QString str2 = str + " " + str1;
    //  QDateTime time_box1;
    //  time_box1 = QDateTime::fromString(str,"yyyy-MM-dd");


    model.setQuery(QString("SELECT a.int_flowid,a.ch_dishno, b.vch_dishname, c.vch_dish_typename, d.vch_unitname,a.num_price, b.num_price, a.vch_operID,a.dt_operdate, a.dt_operdate_effect,a.ch_state  FROM cey_bt_unit d,cey_bt_dish_type c,cey_bt_dish b, cey_bt_dish_special a WHERE (b.ch_unitno=d.ch_unitno)and (b.ch_dish_typeno=c.ch_dish_typeno)and(b.ch_dishno = a.ch_dishno)and (( a.dt_operdate_effect = '%1')AND (a.ch_state = 'Y'))").arg(str));
    for(int i = 0; i < model.rowCount(); i++)
    {
        int row = table_model->rowCount();
        table_model->setRowCount(row + 1);
        table_model->setItem(row,0,new QStandardItem(QString::number(row+1))); //序号
        table_model->setItem(row,1,new QStandardItem(model.record(i).value(1).toString())); //编码
        table_model->setItem(row,2,new QStandardItem(model.record(i).value(2).toString())); //品名
        table_model->setItem(row,3,new QStandardItem(model.record(i).value(3).toString())); //品类
        table_model->setItem(row,4,new QStandardItem(model.record(i).value(4).toString())); //单位
        table_model->setItem(row,5,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(6).toFloat()))); //原价
        table_model->setItem(row,6,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(5).toFloat()))); //特价
        table_model->setItem(row,7,new QStandardItem(model.record(i).value(7).toString())); //建档人
        table_model->setItem(row,8,new QStandardItem(model.record(i).value(8).toDateTime().toString("yyyy-MM-dd hh:mm:ss"))); //建档时间
        table_model->setItem(row,9,new QStandardItem(model.record(i).value(9).toDateTime().toString("yyyy-MM-dd hh:mm:ss")));  //生效时间
        table_model->setItem(row,10,new QStandardItem(model.record(i).value(10).toString())); //状态
        table_model->setItem(row,11,new QStandardItem(model.record(i).value(0).toString())); //行号
    }



    if(table_model->rowCount() > 0)
    {
        ui->tableView->selectRow(0);
        ui->tableView->setColumnHidden(10,true);
        ui->tableView->setColumnHidden(11,true);
    }
}

void back_special::thrid_false()//不保存，刷新
{
    if(ui->radioButton->isChecked() == true)
    {
        xinzeng.clear();
        shanchu.clear();
        shanchutime.clear();
        shanchu_id.clear();
        tejialist.clear();
        table_model->removeRows(0,table_model->rowCount());
        QStringList labels;
        labels<<tr("行号")<<tr("品码")<<tr("品名")<<tr("品类")<<tr("单位")<<tr("原价")<<tr("特价")<<tr("建档人")<<tr("建档时间")<<tr("生效时间");
        table_model->setHorizontalHeaderLabels(labels);
        ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);ui->tableView->setModel(table_model);

        QSqlQueryModel model;
        model.setQuery("Select NOW()");
        QDateTime time =model.record(0).value(0).toDateTime();
        QString str = time.toString("yyyy-MM-dd");
        // QString str1 = "00:00";
        // QString str2 = str + " " + str1;
        //  QDateTime time_box1;
        //  time_box1 = QDateTime::fromString(str,"yyyy-MM-dd");


        model.setQuery(QString("SELECT a.int_flowid,a.ch_dishno, b.vch_dishname, c.vch_dish_typename, d.vch_unitname,a.num_price, b.num_price, a.vch_operID,a.dt_operdate, a.dt_operdate_effect,a.ch_state  FROM cey_bt_unit d,cey_bt_dish_type c,cey_bt_dish b, cey_bt_dish_special a WHERE (b.ch_unitno=d.ch_unitno)and (b.ch_dish_typeno=c.ch_dish_typeno)and(b.ch_dishno = a.ch_dishno)and (( a.dt_operdate_effect = '%1')AND (a.ch_state = 'Y'))").arg(str));
        for(int i = 0; i < model.rowCount(); i++)
        {
            int row = table_model->rowCount();
            table_model->setRowCount(row + 1);
            table_model->setItem(row,0,new QStandardItem(QString::number(row+1))); //序号
            table_model->setItem(row,1,new QStandardItem(model.record(i).value(1).toString())); //编码
            table_model->setItem(row,2,new QStandardItem(model.record(i).value(2).toString())); //品名
            table_model->setItem(row,3,new QStandardItem(model.record(i).value(3).toString())); //品类
            table_model->setItem(row,4,new QStandardItem(model.record(i).value(4).toString())); //单位
            table_model->setItem(row,5,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(6).toFloat()))); //原价
            table_model->setItem(row,6,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(5).toFloat()))); //特价
            table_model->setItem(row,7,new QStandardItem(model.record(i).value(7).toString())); //建档人
            table_model->setItem(row,8,new QStandardItem(model.record(i).value(8).toDateTime().toString("yyyy-MM-dd hh:mm:ss"))); //建档时间
            table_model->setItem(row,9,new QStandardItem(model.record(i).value(9).toDateTime().toString("yyyy-MM-dd hh:mm:ss")));  //生效时间
            table_model->setItem(row,10,new QStandardItem(model.record(i).value(10).toString())); //状态
            table_model->setItem(row,11,new QStandardItem(model.record(i).value(0).toString())); //行号
        }


        if(table_model->rowCount() > 0)
        {
            ui->tableView->selectRow(0);
            ui->tableView->setColumnHidden(10,true);
            ui->tableView->setColumnHidden(11,true);
        }

    }

}

void back_special::thrid_cancel()//关闭提示页面，不做任何操作
{

}

void back_special::on_radioButton_clicked()//当日
{
    table_model->removeRows(0,table_model->rowCount());
    QStringList labels;
    labels<<tr("行号")<<tr("品码")<<tr("品名")<<tr("品类")<<tr("单位")<<tr("原价")<<tr("特价")<<tr("建档人")<<tr("建档时间")<<tr("生效时间");
    table_model->setHorizontalHeaderLabels(labels);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);ui->tableView->setModel(table_model);

    QSqlQueryModel model;
    model.setQuery("Select NOW()");
    QDateTime time =model.record(0).value(0).toDateTime();
    QString str = time.toString("yyyy-MM-dd");
    // QString str1 = "00:00";
    // QString str2 = str + " " + str1;
    //  QDateTime time_box1;
    //  time_box1 = QDateTime::fromString(str,"yyyy-MM-dd");


    model.setQuery(QString("SELECT a.int_flowid,a.ch_dishno, b.vch_dishname, c.vch_dish_typename, d.vch_unitname,a.num_price, b.num_price, a.vch_operID,a.dt_operdate, a.dt_operdate_effect,a.ch_state  FROM cey_bt_unit d,cey_bt_dish_type c,cey_bt_dish b, cey_bt_dish_special a WHERE (b.ch_unitno=d.ch_unitno)and (b.ch_dish_typeno=c.ch_dish_typeno)and(b.ch_dishno = a.ch_dishno)and (( a.dt_operdate_effect = '%1')AND (a.ch_state = 'Y'))").arg(str));
    for(int i = 0; i < model.rowCount(); i++)
    {
        int row = table_model->rowCount();
        table_model->setRowCount(row + 1);
        table_model->setItem(row,0,new QStandardItem(QString::number(row+1))); //序号
        table_model->setItem(row,1,new QStandardItem(model.record(i).value(1).toString())); //编码
        table_model->setItem(row,2,new QStandardItem(model.record(i).value(2).toString())); //品名
        table_model->setItem(row,3,new QStandardItem(model.record(i).value(3).toString())); //品类
        table_model->setItem(row,4,new QStandardItem(model.record(i).value(4).toString())); //单位
        table_model->setItem(row,5,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(6).toFloat()))); //原价
        table_model->setItem(row,6,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(5).toFloat()))); //特价
        table_model->setItem(row,7,new QStandardItem(model.record(i).value(7).toString())); //建档人
        table_model->setItem(row,8,new QStandardItem(model.record(i).value(8).toDateTime().toString("yyyy-MM-dd hh:mm:ss"))); //建档时间
        table_model->setItem(row,9,new QStandardItem(model.record(i).value(9).toDateTime().toString("yyyy-MM-dd hh:mm:ss")));  //生效时间
        table_model->setItem(row,10,new QStandardItem(model.record(i).value(10).toString())); //状态
        table_model->setItem(row,11,new QStandardItem(model.record(i).value(0).toString())); //行号
    }

    tejialist.clear();
    xinzeng.clear();
    shanchu.clear();
    shanchutime.clear();
    shanchu_id.clear();
    QFont font;
    font.setPointSize(12);
    tejialist.clear();

    if(table_model->rowCount() > 0)
    {
        ui->tableView->selectRow(0);
        ui->tableView->setColumnHidden(10,true);
        ui->tableView->setColumnHidden(11,true);
    }
}

void back_special::on_radioButton_2_clicked()//日期查询
{
    if(xinzeng.count() > 0 || shanchu.count() > 0)//数据有改动
    {
        switch(lds_messagebox::warning(this, tr("消息提示"),tr("数据有改动,确认保存?"),tr("确认"),tr("取消"),tr("返回"))){
        case 0:thrid_true();break;
        case 1:thrid_false();break;
        case 2:thrid_cancel();break;
        }
    }

    table_model->removeRows(0,table_model->rowCount());
    QStringList labels;
    labels<<tr("行号")<<tr("品码")<<tr("品名")<<tr("品类")<<tr("单位")<<tr("原价")<<tr("特价")<<tr("建档人")<<tr("建档时间")<<tr("生效时间");
    table_model->setHorizontalHeaderLabels(labels);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);ui->tableView->setModel(table_model);

    QSqlQueryModel model;
    // model.setQuery("Select NOW()");
    //  QDateTime time =model.record(0).value(0).toDateTime();
    QString str = ui->dateTimeEdit->dateTime().toString("yyyy-MM-dd");
    // QString str1 = "00:00";
    // QString str2 = str + " " + str1;
    //  QDateTime time_box1;
    //  time_box1 = QDateTime::fromString(str,"yyyy-MM-dd");


    model.setQuery(QString("SELECT a.int_flowid,a.ch_dishno, b.vch_dishname, c.vch_dish_typename, d.vch_unitname,a.num_price, b.num_price, a.vch_operID,a.dt_operdate, a.dt_operdate_effect,a.ch_state  FROM cey_bt_unit d,cey_bt_dish_type c,cey_bt_dish b, cey_bt_dish_special a WHERE (b.ch_unitno=d.ch_unitno)and (b.ch_dish_typeno=c.ch_dish_typeno)and(b.ch_dishno = a.ch_dishno)and (( a.dt_operdate_effect = '%1')AND (a.ch_state = 'Y'))").arg(str));
    for(int i = 0; i < model.rowCount(); i++)
    {
        int row = table_model->rowCount();
        table_model->setRowCount(row + 1);
        table_model->setItem(row,0,new QStandardItem(QString::number(row+1))); //序号
        table_model->setItem(row,1,new QStandardItem(model.record(i).value(1).toString())); //编码
        table_model->setItem(row,2,new QStandardItem(model.record(i).value(2).toString())); //品名
        table_model->setItem(row,3,new QStandardItem(model.record(i).value(3).toString())); //品类
        table_model->setItem(row,4,new QStandardItem(model.record(i).value(4).toString())); //单位
        table_model->setItem(row,5,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(6).toFloat()))); //原价
        table_model->setItem(row,6,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(5).toFloat()))); //特价
        table_model->setItem(row,7,new QStandardItem(model.record(i).value(7).toString())); //建档人
        table_model->setItem(row,8,new QStandardItem(model.record(i).value(8).toDateTime().toString("yyyy-MM-dd hh:mm:ss"))); //建档时间
        table_model->setItem(row,9,new QStandardItem(model.record(i).value(9).toDateTime().toString("yyyy-MM-dd hh:mm:ss")));  //生效时间
        table_model->setItem(row,10,new QStandardItem(model.record(i).value(10).toString())); //状态
        table_model->setItem(row,11,new QStandardItem(model.record(i).value(0).toString())); //行号
    }

    tejialist.clear();
    xinzeng.clear();
    shanchu.clear();
    shanchutime.clear();
    shanchu_id.clear();


    if(table_model->rowCount() > 0)
    {
        ui->tableView->selectRow(0);
        ui->tableView->setColumnHidden(10,true);
        ui->tableView->setColumnHidden(11,true);
    }
}

void back_special::on_dateTimeEdit_dateChanged(const QDate &date)
{
    if(ui->radioButton_2->isChecked() == true)
    {
        table_model->removeRows(0,table_model->rowCount());
        QStringList labels;
        labels<<tr("行号")<<tr("品码")<<tr("品名")<<tr("品类")<<tr("单位")<<tr("原价")<<tr("特价")<<tr("建档人")<<tr("建档时间")<<tr("生效时间");
        table_model->setHorizontalHeaderLabels(labels);
        ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);ui->tableView->setModel(table_model);

        QSqlQueryModel model;
        // model.setQuery("Select NOW()");
        //  QDateTime time =model.record(0).value(0).toDateTime();
        QString str = ui->dateTimeEdit->dateTime().toString("yyyy-MM-dd");
        // QString str1 = "00:00";
        // QString str2 = str + " " + str1;
        //  QDateTime time_box1;
        //  time_box1 = QDateTime::fromString(str,"yyyy-MM-dd");


        model.setQuery(QString("SELECT a.int_flowid,a.ch_dishno, b.vch_dishname, c.vch_dish_typename, d.vch_unitname,a.num_price, b.num_price, a.vch_operID,a.dt_operdate, a.dt_operdate_effect,a.ch_state  FROM cey_bt_unit d,cey_bt_dish_type c,cey_bt_dish b, cey_bt_dish_special a WHERE (b.ch_unitno=d.ch_unitno)and (b.ch_dish_typeno=c.ch_dish_typeno)and(b.ch_dishno = a.ch_dishno)and (( a.dt_operdate_effect = '%1')AND (a.ch_state = 'Y'))").arg(str));
        for(int i = 0; i < model.rowCount(); i++)
        {
            int row = table_model->rowCount();
            table_model->setRowCount(row + 1);
            table_model->setItem(row,0,new QStandardItem(QString::number(row+1))); //序号
            table_model->setItem(row,1,new QStandardItem(model.record(i).value(1).toString())); //编码
            table_model->setItem(row,2,new QStandardItem(model.record(i).value(2).toString())); //品名
            table_model->setItem(row,3,new QStandardItem(model.record(i).value(3).toString())); //品类
            table_model->setItem(row,4,new QStandardItem(model.record(i).value(4).toString())); //单位
            table_model->setItem(row,5,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(6).toFloat()))); //原价
            table_model->setItem(row,6,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(5).toFloat()))); //特价
            table_model->setItem(row,7,new QStandardItem(model.record(i).value(7).toString())); //建档人
            table_model->setItem(row,8,new QStandardItem(model.record(i).value(8).toDateTime().toString("yyyy-MM-dd hh:mm:ss"))); //建档时间
            table_model->setItem(row,9,new QStandardItem(model.record(i).value(9).toDateTime().toString("yyyy-MM-dd hh:mm:ss")));  //生效时间
            table_model->setItem(row,10,new QStandardItem(model.record(i).value(10).toString())); //状态
            table_model->setItem(row,11,new QStandardItem(model.record(i).value(0).toString())); //行号
        }

        tejialist.clear();
        xinzeng.clear();
        shanchu.clear();
        shanchutime.clear();
        shanchu_id.clear();


        if(table_model->rowCount() > 0)
        {
            ui->tableView->selectRow(0);
            ui->tableView->setColumnHidden(10,true);
            ui->tableView->setColumnHidden(11,true);
        }
    }
}
