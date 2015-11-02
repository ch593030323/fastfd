#include "back_warn.h"
#include "ui_back_warn.h"
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QDateTime>
#include <QDebug>
#include "back_special_add.h"
#include "n_func.h"
#include "lds_messagebox.h"
#include "modify_view.h"
#include <QSqlQuery>
#include <QSqlError>
#include "backheader.h"

back_warn::back_warn(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::back_warn)
{
    ui->setupUi(this);
    //this->setWindowFlags(Qt::FramelessWindowHint|Qt::Dialog);this->setStyleSheet("QDialog{border:2px solid gray}");
    init_control();

}

back_warn::~back_warn()
{
    delete ui;
}

void back_warn::init_control()
{
    //ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置属性为只读
    QStringList labels;
    labels<<tr("行号")<<tr("品码")<<tr("品名")<<tr("品类")<<tr("单位")<<tr("估清数量")<<tr("已销数量")<<tr("建档人")<<tr("建档时间");
    table_model = new QStandardItemModel();
    table_model->setHorizontalHeaderLabels(labels);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);ui->tableView->setModel(table_model);

    QSqlQueryModel model;
    model.setQuery("Select NOW()");
    QDateTime time =model.record(0).value(0).toDateTime();
    QString str = time.toString("yyyy-MM-dd");


    model.setQuery(QString("SELECT a.int_flowID, a.ch_dishno  ,b.vch_dishname,c.vch_dish_typename ,d.vch_unitname, a.num_num , a.num_sale ,a.vch_operID , a.dt_operdate,a.ch_state FROM cey_bt_unit d,cey_bt_dish_type c,cey_bt_dish b,cey_bt_dish_warn a  WHERE (d.ch_unitno = b.ch_unitno) and (b.ch_dish_typeno = c.ch_dish_typeno)and(b.ch_dishno = a.ch_dishno)and(left(a.dt_date,10) = '%1')and(a.ch_state = 'Y')").arg(str));
    // qDebug() << QString("SELECT a.int_flowID, a.ch_dishno  ,b.vch_dishname,c.vch_dish_typename ,d.vch_unitname, a.num_num , a.num_sale ,a.vch_operID , a.dt_operdate，a.ch_state  FROM cey_bt_unit d,cey_bt_dish_type c,cey_bt_dish b,cey_bt_dish_warn a  WHERE (d.ch_unitno = b.ch_unitno) and (b.ch_dish_typeno = c.ch_dish_typeno)and(b.ch_dishno = a.ch_dishno)and(( left(a.dt_date,10))= '%1')and(a.ch_state = 'Y'))").arg(str);
    for(int i = 0; i < model.rowCount(); i++)
    {
        int row = table_model->rowCount();
        table_model->setRowCount(row + 1);
        table_model->setItem(row,0,new QStandardItem(QString::number(row+1))); //序号
        table_model->setItem(row,1,new QStandardItem(model.record(i).value(1).toString())); //编码
        table_model->setItem(row,2,new QStandardItem(model.record(i).value(2).toString())); //品名
        table_model->setItem(row,3,new QStandardItem(model.record(i).value(3).toString())); //品类
        table_model->setItem(row,4,new QStandardItem(model.record(i).value(4).toString())); //单位
        table_model->setItem(row,5,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(5).toFloat()))); //估清数量
        table_model->setItem(row,6,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(6).toFloat()))); //已销数量
        table_model->setItem(row,7,new QStandardItem(model.record(i).value(7).toString())); //建档人
        table_model->setItem(row,8,new QStandardItem(model.record(i).value(8).toDateTime().toString("yyyy-MM-dd hh:mm:ss"))); //建档时间
        table_model->setItem(row,9,new QStandardItem(model.record(i).value(9).toString())); //状态
        table_model->setItem(row,10,new QStandardItem(model.record(i).value(0).toString())); //行号
    }



    if(table_model->rowCount() > 0)
    {
        ui->tableView->selectRow(0);
        ui->tableView->setColumnHidden(10,true);
        ui->tableView->setColumnHidden(9,true);
    }
}


void back_warn::on_pushButton_10_clicked()//新增估清
{
    back_special_add *b_special = new back_special_add();
    backheader::static_dialogexec(*b_special, tr("菜品资料选择"));

    QSqlQueryModel model;
    model.setQuery(QString("select NOW()"));
    QSqlQueryModel danwei;
    QSqlQueryModel pinlei;
    QString time = model.record(0).value(0).toDateTime().toString("yyyy-MM-dd hh:mm:ss");
    if(b_special->btn_t_f == true)
    {
        if(b_special->yingyong.count() > 0)//存在应用
        {
            for(int i = 0; i < b_special->yingyong.count(); i++)
            {
                if(!tejialist.contains(b_special->yingyong[i]))
                {
                    tejialist << b_special->yingyong[i];
                    xinzeng << b_special->yingyong[i];

                    model.clear();
                    model.setQuery(QString("select vch_dishname,ch_dish_typeno,ch_unitno,num_price from cey_bt_dish where ch_dishno = '%1' ").arg(b_special->yingyong[i]));
                    int row = table_model->rowCount();
                    table_model->setRowCount(row + 1);
                    table_model->setItem(row,0,new QStandardItem(QString::number(row+1))); //序号
                    table_model->setItem(row,1,new QStandardItem(b_special->yingyong[i])); //编码
                    table_model->setItem(row,2,new QStandardItem(model.record(0).value(0).toString())); //品名
                    pinlei.clear();
                    pinlei.setQuery(QString("select vch_dish_typename from cey_bt_dish_type where ch_dish_typeno ='%1' ").arg(model.record(0).value(1).toString()));
                    table_model->setItem(row,3,new QStandardItem(pinlei.record(0).value(0).toString())); //品类

                    danwei.clear();
                    danwei.setQuery(QString("select vch_unitname from cey_bt_unit where ch_unitno ='%1' ").arg(model.record(0).value(2).toString()));
                    table_model->setItem(row,4,new QStandardItem(danwei.record(0).value(0).toString())); //单位
                    table_model->setItem(row,5,new QStandardItem(tr("10.00")));//估清数量
                    table_model->setItem(row,6,new QStandardItem(tr("0.00")));//已销售数量
                    table_model->setItem(row,7,new QStandardItem(n_func::gs_operid));//建档人
                    table_model->setItem(row,8,new QStandardItem(time));//建档时间
                    table_model->setItem(row,9,new QStandardItem("N"));//状态
                    table_model->setItem(row,10,new QStandardItem("11"));//id
                }
            }
        }
        else//不存在应用
        {
            for(int i = 0; i < b_special->truelist.count(); i++)
            {
                if(!tejialist.contains(b_special->truelist[i]))
                {
                    tejialist << b_special->truelist[i];
                    xinzeng << b_special->truelist[i];

                    model.clear();
                    model.setQuery(QString("select vch_dishname,ch_dish_typeno,ch_unitno,num_price from cey_bt_dish where ch_dishno = '%1' ").arg(b_special->truelist[i]));
                    int row = table_model->rowCount();
                    table_model->setRowCount(row + 1);
                    table_model->setItem(row,0,new QStandardItem(QString::number(row+1))); //序号
                    table_model->setItem(row,1,new QStandardItem(b_special->truelist[i])); //编码
                    table_model->setItem(row,2,new QStandardItem(model.record(0).value(0).toString())); //品名
                    pinlei.clear();
                    pinlei.setQuery(QString("select vch_dish_typename from cey_bt_dish_type where ch_dish_typeno ='%1' ").arg(model.record(0).value(1).toString()));
                    table_model->setItem(row,3,new QStandardItem(pinlei.record(0).value(0).toString())); //品类

                    danwei.clear();
                    danwei.setQuery(QString("select vch_unitname from cey_bt_unit where ch_unitno ='%1' ").arg(model.record(0).value(2).toString()));
                    table_model->setItem(row,4,new QStandardItem(danwei.record(0).value(0).toString())); //单位

                    table_model->setItem(row,5,new QStandardItem(tr("10.00")));//估清数量
                    table_model->setItem(row,6,new QStandardItem(tr("0.00")));//已销售数量
                    table_model->setItem(row,7,new QStandardItem(n_func::gs_operid));//建档人
                    table_model->setItem(row,8,new QStandardItem(time));//建档时间
                    table_model->setItem(row,9,new QStandardItem("N"));//状态
                    table_model->setItem(row,10,new QStandardItem("11"));//id
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
        ui->tableView->setColumnHidden(9,true);
        ui->tableView->setColumnHidden(10,true);
    }

}

void back_warn::on_pushButton_11_clicked()//删除估清
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

    if(table_model->item(row,9)->text() == "Y")
    {
        shanchu << table_model->item(row,1)->text();
        shanchu_id << table_model->item(row,10)->text();
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

void back_warn::on_pushButton_12_clicked()//修改数量
{
    int row = ui->tableView->currentIndex().row();
    ;
    if(row <= -1)
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("请选择一条要修改的数据!"));
        
        return;
    }

    if(table_model->item(row,9)->text() == tr("Y"))
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("该估清已经保存,不能修改!"));
        
        return;
    }

    modify_view view(table_model->item(row,6)->text());
    backheader::static_dialogexec(view, tr("修改界面"));

    if(view.btn_t_f == true)
    {
        table_model->item(row,5)->setText(QString().sprintf("%0.2f",view.jiage_m.toFloat()));
    }
}

void back_warn::on_pushButton_13_clicked()//保存
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
    for(int i = 0; i < shanchu.count(); i++)
    {
        query.prepare(QString("UPDATE cey_bt_dish_warn SET ch_state = 'N', vch_operID2 = :shanchu, dt_operdate2 = :time WHERE int_flowID = :id "));
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
            if(table_model->item(m,1)->text() == xinzeng[k] && table_model->item(m,9)->text() == "N")
            {
                query.prepare(QString("insert into  cey_bt_dish_warn(ch_dishno,num_num,num_sale,vch_operid,dt_operdate,dt_date,ch_state)  values(:bianma,:num,:sale,:oper,:time,:time1,'Y')"));
                query.bindValue(":bianma",xinzeng[k]);
                query.bindValue(":num",table_model->item(m,5)->text());
                query.bindValue(":sale",table_model->item(m,6)->text());
                query.bindValue(":oper",table_model->item(m,7)->text());
                query.bindValue(":time",table_model->item(m,8)->text());
                query.bindValue(":time1",riqi);

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
    tejialist.clear();

    table_model->removeRows(0,table_model->rowCount());
    model.clear();
    model.setQuery("Select NOW()");
    QDateTime time =model.record(0).value(0).toDateTime();
    QString str = time.toString("yyyy-MM-dd");

    model.clear();
    model.setQuery(QString("SELECT a.int_flowID, a.ch_dishno  ,b.vch_dishname,c.vch_dish_typename ,d.vch_unitname, a.num_num , a.num_sale ,a.vch_operID , a.dt_operdate,a.ch_state FROM cey_bt_unit d,cey_bt_dish_type c,cey_bt_dish b,cey_bt_dish_warn a  WHERE (d.ch_unitno = b.ch_unitno) and (b.ch_dish_typeno = c.ch_dish_typeno)and(b.ch_dishno = a.ch_dishno)and(left(a.dt_date,10) = '%1')and(a.ch_state = 'Y')").arg(str));
    // qDebug() << QString("SELECT a.int_flowID, a.ch_dishno  ,b.vch_dishname,c.vch_dish_typename ,d.vch_unitname, a.num_num , a.num_sale ,a.vch_operID , a.dt_operdate，a.ch_state  FROM cey_bt_unit d,cey_bt_dish_type c,cey_bt_dish b,cey_bt_dish_warn a  WHERE (d.ch_unitno = b.ch_unitno) and (b.ch_dish_typeno = c.ch_dish_typeno)and(b.ch_dishno = a.ch_dishno)and(( left(a.dt_date,10))= '%1')and(a.ch_state = 'Y'))").arg(str);
    for(int i = 0; i < model.rowCount(); i++)
    {
        int row = table_model->rowCount();
        table_model->setRowCount(row + 1);
        table_model->setItem(row,0,new QStandardItem(QString::number(row+1))); //序号
        table_model->setItem(row,1,new QStandardItem(model.record(i).value(1).toString())); //编码
        table_model->setItem(row,2,new QStandardItem(model.record(i).value(2).toString())); //品名
        table_model->setItem(row,3,new QStandardItem(model.record(i).value(3).toString())); //品类
        table_model->setItem(row,4,new QStandardItem(model.record(i).value(4).toString())); //单位
        table_model->setItem(row,5,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(5).toFloat()))); //估清数量
        table_model->setItem(row,6,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(6).toFloat()))); //已销数量
        table_model->setItem(row,7,new QStandardItem(model.record(i).value(7).toString())); //建档人
        table_model->setItem(row,8,new QStandardItem(model.record(i).value(8).toDateTime().toString("yyyy-MM-dd hh:mm:ss"))); //建档时间
        table_model->setItem(row,9,new QStandardItem(model.record(i).value(9).toString())); //状态
        table_model->setItem(row,10,new QStandardItem(model.record(i).value(0).toString())); //行号
    }



    if(table_model->rowCount() > 0)
    {
        ui->tableView->selectRow(0);
        ui->tableView->setColumnHidden(10,true);
        ui->tableView->setColumnHidden(9,true);
    }

}

void back_warn::on_pushButton_14_clicked()//刷新
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

void back_warn::on_pushButton_15_clicked()//导出
{

}

void back_warn::on_pushButton_16_clicked()//退出
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

void back_warn::thrid_true()
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
    for(int i = 0; i < shanchu.count(); i++)
    {
        query.prepare(QString("UPDATE cey_bt_dish_warn SET ch_state = 'N', vch_operID2 = :shanchu, dt_operdate2 = :time WHERE int_flowID = :id "));
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
            if(table_model->item(m,1)->text() == xinzeng[k] && table_model->item(m,9)->text() == "N")
            {
                query.prepare(QString("insert into  cey_bt_dish_warn(ch_dishno,num_num,num_sale,vch_operid,dt_operdate,dt_date,ch_state)  values(:bianma,:num,:sale,:oper,:time,:time1,'Y')"));
                query.bindValue(":bianma",xinzeng[k]);
                query.bindValue(":num",table_model->item(m,5)->text());
                query.bindValue(":sale",table_model->item(m,6)->text());
                query.bindValue(":oper",table_model->item(m,7)->text());
                query.bindValue(":time",table_model->item(m,8)->text());
                query.bindValue(":time1",riqi);

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
    tejialist.clear();

    table_model->removeRows(0,table_model->rowCount());
    model.setQuery("Select NOW()");
    QDateTime time =model.record(0).value(0).toDateTime();
    QString str = time.toString("yyyy-MM-dd");

    model.clear();
    model.setQuery(QString("SELECT a.int_flowID, a.ch_dishno  ,b.vch_dishname,c.vch_dish_typename ,d.vch_unitname, a.num_num , a.num_sale ,a.vch_operID , a.dt_operdate,a.ch_state FROM cey_bt_unit d,cey_bt_dish_type c,cey_bt_dish b,cey_bt_dish_warn a  WHERE (d.ch_unitno = b.ch_unitno) and (b.ch_dish_typeno = c.ch_dish_typeno)and(b.ch_dishno = a.ch_dishno)and(left(a.dt_date,10) = '%1')and(a.ch_state = 'Y')").arg(str));
    // qDebug() << QString("SELECT a.int_flowID, a.ch_dishno  ,b.vch_dishname,c.vch_dish_typename ,d.vch_unitname, a.num_num , a.num_sale ,a.vch_operID , a.dt_operdate，a.ch_state  FROM cey_bt_unit d,cey_bt_dish_type c,cey_bt_dish b,cey_bt_dish_warn a  WHERE (d.ch_unitno = b.ch_unitno) and (b.ch_dish_typeno = c.ch_dish_typeno)and(b.ch_dishno = a.ch_dishno)and(( left(a.dt_date,10))= '%1')and(a.ch_state = 'Y'))").arg(str);
    for(int i = 0; i < model.rowCount(); i++)
    {
        int row = table_model->rowCount();
        table_model->setRowCount(row + 1);
        table_model->setItem(row,0,new QStandardItem(QString::number(row+1))); //序号
        table_model->setItem(row,1,new QStandardItem(model.record(i).value(1).toString())); //编码
        table_model->setItem(row,2,new QStandardItem(model.record(i).value(2).toString())); //品名
        table_model->setItem(row,3,new QStandardItem(model.record(i).value(3).toString())); //品类
        table_model->setItem(row,4,new QStandardItem(model.record(i).value(4).toString())); //单位
        table_model->setItem(row,5,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(5).toFloat()))); //估清数量
        table_model->setItem(row,6,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(6).toFloat()))); //已销数量
        table_model->setItem(row,7,new QStandardItem(model.record(i).value(7).toString())); //建档人
        table_model->setItem(row,8,new QStandardItem(model.record(i).value(8).toDateTime().toString("yyyy-MM-dd hh:mm:ss"))); //建档时间
        table_model->setItem(row,9,new QStandardItem(model.record(i).value(9).toString())); //状态
        table_model->setItem(row,10,new QStandardItem(model.record(i).value(0).toString())); //行号
    }



    if(table_model->rowCount() > 0)
    {
        ui->tableView->selectRow(0);
        ui->tableView->setColumnHidden(10,true);
        ui->tableView->setColumnHidden(9,true);
    }
}

void back_warn::thrid_false()
{
    shanchu.clear();
    shanchutime.clear();
    shanchu_id.clear();
    xinzeng.clear();
    tejialist.clear();

    table_model->removeRows(0,table_model->rowCount());
    QSqlQueryModel model;
    model.setQuery("Select NOW()");
    QDateTime time =model.record(0).value(0).toDateTime();
    QString str = time.toString("yyyy-MM-dd");

    model.clear();
    model.setQuery(QString("SELECT a.int_flowID, a.ch_dishno  ,b.vch_dishname,c.vch_dish_typename ,d.vch_unitname, a.num_num , a.num_sale ,a.vch_operID , a.dt_operdate,a.ch_state FROM cey_bt_unit d,cey_bt_dish_type c,cey_bt_dish b,cey_bt_dish_warn a  WHERE (d.ch_unitno = b.ch_unitno) and (b.ch_dish_typeno = c.ch_dish_typeno)and(b.ch_dishno = a.ch_dishno)and(left(a.dt_date,10) = '%1')and(a.ch_state = 'Y')").arg(str));
    // qDebug() << QString("SELECT a.int_flowID, a.ch_dishno  ,b.vch_dishname,c.vch_dish_typename ,d.vch_unitname, a.num_num , a.num_sale ,a.vch_operID , a.dt_operdate，a.ch_state  FROM cey_bt_unit d,cey_bt_dish_type c,cey_bt_dish b,cey_bt_dish_warn a  WHERE (d.ch_unitno = b.ch_unitno) and (b.ch_dish_typeno = c.ch_dish_typeno)and(b.ch_dishno = a.ch_dishno)and(( left(a.dt_date,10))= '%1')and(a.ch_state = 'Y'))").arg(str);
    for(int i = 0; i < model.rowCount(); i++)
    {
        int row = table_model->rowCount();
        table_model->setRowCount(row + 1);
        table_model->setItem(row,0,new QStandardItem(QString::number(row+1))); //序号
        table_model->setItem(row,1,new QStandardItem(model.record(i).value(1).toString())); //编码
        table_model->setItem(row,2,new QStandardItem(model.record(i).value(2).toString())); //品名
        table_model->setItem(row,3,new QStandardItem(model.record(i).value(3).toString())); //品类
        table_model->setItem(row,4,new QStandardItem(model.record(i).value(4).toString())); //单位
        table_model->setItem(row,5,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(5).toFloat()))); //估清数量
        table_model->setItem(row,6,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(6).toFloat()))); //已销数量
        table_model->setItem(row,7,new QStandardItem(model.record(i).value(7).toString())); //建档人
        table_model->setItem(row,8,new QStandardItem(model.record(i).value(8).toDateTime().toString("yyyy-MM-dd hh:mm:ss"))); //建档时间
        table_model->setItem(row,9,new QStandardItem(model.record(i).value(9).toString())); //状态
        table_model->setItem(row,10,new QStandardItem(model.record(i).value(0).toString())); //行号
    }



    if(table_model->rowCount() > 0)
    {
        ui->tableView->selectRow(0);
        ui->tableView->setColumnHidden(10,true);
        ui->tableView->setColumnHidden(9,true);
    }
}

void back_warn::thrid_cancel()
{

}
