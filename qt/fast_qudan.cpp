#include "fast_qudan.h"
#include "ui_fast_qudan.h"
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QDateTime>
#include "fast_order_2.h"
#include <QSqlQuery>
#include "lds_messagebox.h"
#include <QDebug>
#include"backheader.h"

fast_qudan::fast_qudan(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::fast_qudan)
{
    ui->setupUi(this);
    btn_t_f = false;
    init_control();
}

fast_qudan::~fast_qudan()
{
    delete ui;
}

void fast_qudan::init_control()
{
    QSqlQueryModel model;
    QStringList labels;
    labels<<"挂单号"<<"餐桌号"<<"挂单人"<<"挂单时间";
    model_top = new QStandardItemModel();
    model_top->setHorizontalHeaderLabels(labels);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);ui->tableView->setModel(model_top);

    model.setQuery(QString("Select int_flowID,ch_tableno,vch_operID,MAX(dt_operdate),left(dt_operdate,10),left(current_date(),10)  from cey_u_hungbill where left(dt_operdate,10) = left(current_date(),10) Group by int_flowID,ch_tableno,vch_operID"));
    for(int i = 0; i < model.rowCount(); i++ )
    {
        int row = model_top->rowCount();
        QString str = model.record(i).value(3).toDateTime().toString("yyyy-MM-dd hh:mm:ss");
        model_top->setRowCount(row + 1);
        model_top->setItem(row,0,new QStandardItem(model.record(i).value(0).toString()));
        model_top->setItem(row,1,new QStandardItem(model.record(i).value(1).toString()));
        model_top->setItem(row,2,new QStandardItem(model.record(i).value(2).toString()));
        model_top->setItem(row,3,new QStandardItem(str));
    }

    ui->tableView->selectRow(0);

    //ui->tableView_2->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置属性为只读

    QStringList labels1;
    labels1<<"行号"<<"品码"<<"品名"<<"数量"<<"价格"<<"套菜"<<"特价"<<"赠送"<<"特殊做法";
    model_buttom = new QStandardItemModel();
    model_buttom->setHorizontalHeaderLabels(labels1);
    ui->tableView_2->setModel(model_buttom);

    on_tableView_clicked(QModelIndex());
}

void fast_qudan::on_pushButton_14_clicked()
{
    btn_t_f = false;
    this->close();
}

void fast_qudan::on_tableView_clicked(const QModelIndex &index)
{
    QSqlQueryModel model;
    int row = ui->tableView->currentIndex().row();
    model_buttom->setHorizontalHeaderLabels(QStringList() << tr("行号") << tr("品码") << tr("品名") << tr("数量") << tr("价格") << tr("套菜") << tr("特价") << tr("赠送") << tr("特殊做法"));
    ui->tableView_2->setModel(model_buttom);
    QSqlQueryModel cook;
    if(model_top->rowCount() > 0)
    {
        model.clear();
        model.setQuery(QString("SELECT a.int_flowID,a.int_id,a.ch_tableno,a.ch_dishno,a.num_price,a.num_num,a.num_back,a.vch_back_operID,a.dt_back_operdate,a.int_discount,a.vch_dis_operID,a.dt_dis_operdate,a.ch_printflag,a.vch_print_memo,a.ch_suitflag,a.ch_suitno,a.ch_specialflag,a.ch_presentflag,a.vch_pre_operID,a.dt_pre_operdate,a.vch_operID,a.dt_operdate,a.num_price_add,b.vch_dishname,c.vch_unitname,b.num_price,c.num_default , b.ch_outflag ,d.vch_dish_typename,e.vch_sub_typename,e.ch_labelprint_flag,b.ch_discount,b.ch_curprice,b.vch_printer,b.vch_outprint FROM cey_u_hungbill a,cey_bt_dish b,cey_bt_unit c ,cey_bt_dish_type d ,cey_bt_dish_subtype e WHERE e.ch_dish_typeno = b.ch_dish_typeno and e.ch_sub_typeno = b.ch_sub_typeno and b.ch_dish_typeno = d.ch_dish_typeno and a.ch_dishno = b.ch_dishno and b.ch_unitno=c.ch_unitno and a.int_flowID='%1' order by a.int_id asc").arg(model_top->item(row,0)->text()));

        qDebug() << model.query().lastQuery();
        for(int i = 0; i  < model.rowCount(); i++)
        {
            cook.clear();
            int row = model_buttom->rowCount();
            model_buttom->setRowCount(row + 1);
            model_buttom->setItem(row,0,new QStandardItem(model.record(i).value("int_id").toString()));//行号
            model_buttom->setItem(row,1,new QStandardItem(model.record(i).value("ch_dishno").toString()));//品码
            cook.setQuery(QString("select ch_dish_typeno,int_rate,ch_curprice from cey_bt_dish where ch_dishno = '%1' ").arg(model.record(i).value("ch_dishno").toString()));
            model_buttom->setItem(row,2,new QStandardItem(model.record(i).value("vch_dishname").toString()));//品名
            model_buttom->setItem(row,3,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value("num_num").toFloat())));//数量
            if(model.record(i).value("ch_suitflag").toString() == tr("Y"))
            {
                model_buttom->setItem(row,4,new QStandardItem(tr("0.00")));//价格
            }
            else
            {
                model_buttom->setItem(row,4,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value("num_price").toFloat())));//价格
            }

            model_buttom->setItem(row,5,new QStandardItem(model.record(i).value("ch_suitflag").toString()));//套菜
            model_buttom->setItem(row,6,new QStandardItem(model.record(i).value("ch_specialflag").toString()));//特价
            model_buttom->setItem(row,7,new QStandardItem(model.record(i).value("ch_presentflag").toString()));//赠送
            model_buttom->setItem(row,8,new QStandardItem(model.record(i).value("vch_print_memo").toString()));//特殊做法
            model_buttom->setItem(row,9,new QStandardItem(model.record(i).value("vch_unitname").toString()));//单位
            model_buttom->setItem(row,10,new QStandardItem("N"));//是否优惠
            model_buttom->setItem(row,11,new QStandardItem(model.record(i).value("int_discount").toString()));//折扣值
            model_buttom->setItem(row,12,new QStandardItem(model.record(i).value("vch_pre_operID").toString()));//赠送人
            model_buttom->setItem(row,13,new QStandardItem(model.record(i).value("vch_dis_operID").toString()));//定折人
            model_buttom->setItem(row,14,new QStandardItem(cook.record(i).value("ch_curprice").toString()));//是否实价菜
            QString zitaocai;
            if(model.record(i).value("ch_suitflag").toString() == "Y")
            {
                QSqlQueryModel tt;
                tt.setQuery(QString("select ch_suitno from cey_bt_dish_suit where ch_dishno ='%1' ").arg(model.record(i).value("ch_dishno").toString()));
                zitaocai = tt.record(0).value("ch_suitno").toString();
            }
            model_buttom->setItem(row,15,new QStandardItem(zitaocai));//子套菜所在主套菜编码
            float bb = 0.00;
            bb = model.record(i).value("num_price").toFloat() * cook.record(0).value("int_rate").toFloat() * 0.01;
            model_buttom->setItem(row,16,new QStandardItem(QString().sprintf("%0.2f",bb)));//税点
            QString zhekou;
            if(model.record(i).value("int_discount").toFloat() != 100)
            {
                zhekou = tr("Y");
            }
            else
            {
                zhekou = "N";
            }
            model_buttom->setItem(row,17,new QStandardItem(zhekou));//是否折扣
            model_buttom->setItem(row,18,new QStandardItem(model.record(i).value("dt_dis_operdate").toDateTime().toString("yyyy-MM-dd hh:mm:ss")));//折扣时间
            model_buttom->setItem(row,19,new QStandardItem(model.record(i).value("dt_pre_operdate").toDateTime().toString("yyyy-MM-dd hh:mm:ss")));//赠送时间
            model_buttom->setItem(row,20,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value("num_price_add").toFloat())));//做法加价
            model_buttom->setItem(row,21,new QStandardItem(cook.record(0).value("ch_dish_typeno").toString()));//所属大类编码
            float aa = 0.00;
            aa = model.record(i).value("num_num").toFloat() * model.record(i).value("num_price").toFloat() * (model.record(i).value("int_discount").toFloat()) * 0.01 + model.record(i).value("num_price_add").toFloat()/* + cook.record(i).value(5).toFloat() + bb*/;
            if(model.record(i).value("ch_suitflag").toString() == tr("Y"))
            {
                model_buttom->setItem(row,22,new QStandardItem(tr("0.00")));//价格
            }
            else
            {
                model_buttom->setItem(row,22,new QStandardItem(QString().sprintf("%0.2f",aa)));//总价
            }
            cook.clear();
            cook.setQuery(QString("select ch_sub_typeno from cey_bt_dish where ch_dishno = '%1' ").arg(model.record(i).value("ch_dishno").toString()));
            model_buttom->setItem(row,23,new QStandardItem(cook.record(0).value("ch_sub_typeno").toString()));//所属小类编码

        }
    }


    ui->tableView_2->setColumnHidden(9,true);
    ui->tableView_2->setColumnHidden(10,true);
    ui->tableView_2->setColumnHidden(11,true);
    ui->tableView_2->setColumnHidden(12,true);
    ui->tableView_2->setColumnHidden(13,true);
    ui->tableView_2->setColumnHidden(14,true);
    ui->tableView_2->setColumnHidden(15,true);
    ui->tableView_2->setColumnHidden(16,true);
    ui->tableView_2->setColumnHidden(17,true);
    ui->tableView_2->setColumnHidden(18,true);
    ui->tableView_2->setColumnHidden(19,true);
    ui->tableView_2->setColumnHidden(20,true);
    ui->tableView_2->setColumnHidden(21,true);
    ui->tableView_2->setColumnHidden(22,true);
    ui->tableView_2->setColumnHidden(23,true);
    ui->tableView_2->selectRow(0);
}

void fast_qudan::on_pushButton_13_clicked()
{
    for(int i = 0; i < model_buttom->rowCount(); i++)
    {
        int row = fast_order_2::modelBase.rowCount();
        fast_order_2::modelBase.setRowCount(row + 1);
        fast_order_2::modelBase.setItem(row,0,new QStandardItem(model_buttom->item(i,0)->text()));//
        fast_order_2::modelBase.setItem(row,1,new QStandardItem(model_buttom->item(i,1)->text()));
        fast_order_2::modelBase.setItem(row,2,new QStandardItem(model_buttom->item(i,2)->text()));
        fast_order_2::modelBase.setItem(row,3,new QStandardItem(model_buttom->item(i,3)->text()));
        fast_order_2::modelBase.setItem(row,4,new QStandardItem(model_buttom->item(i,4)->text()));
        fast_order_2::modelBase.setItem(row,5,new QStandardItem(model_buttom->item(i,11)->text()));//折扣
        fast_order_2::modelBase.setItem(row,6,new QStandardItem(model_buttom->item(i,22)->text()));//总价
        fast_order_2::modelBase.setItem(row,7,new QStandardItem(model_buttom->item(i,8)->text()));//特殊做法
        fast_order_2::modelBase.setItem(row,8,new QStandardItem(model_buttom->item(i,7)->text()));//是否赠送
        fast_order_2::modelBase.setItem(row,9,new QStandardItem(model_buttom->item(i,5)->text()));//是否套菜
        fast_order_2::modelBase.setItem(row,10,new QStandardItem(model_buttom->item(i,6)->text()));//是否特价
        fast_order_2::modelBase.setItem(row,11,new QStandardItem(model_buttom->item(i,10)->text()));//是否优惠
        fast_order_2::modelBase.setItem(row,12,new QStandardItem(model_buttom->item(i,9)->text()));//单位
        fast_order_2::modelBase.setItem(row,13,new QStandardItem(model_buttom->item(i,12)->text()));//赠送人
        fast_order_2::modelBase.setItem(row,14,new QStandardItem(model_buttom->item(i,13)->text()));//定折人
        fast_order_2::modelBase.setItem(row,15,new QStandardItem(model_buttom->item(i,14)->text()));//实价
        fast_order_2::modelBase.setItem(row,16,new QStandardItem(model_buttom->item(i,15)->text()));//子套菜所在主套菜编码
        fast_order_2::modelBase.setItem(row,17,new QStandardItem(model_buttom->item(i,16)->text()));//税点
        fast_order_2::modelBase.setItem(row,18,new QStandardItem(model_buttom->item(i,17)->text()));//是否折扣
        fast_order_2::modelBase.setItem(row,19,new QStandardItem(model_buttom->item(i,18)->text()));//折扣时间
        fast_order_2::modelBase.setItem(row,20,new QStandardItem(model_buttom->item(i,19)->text()));//赠送时间
        fast_order_2::modelBase.setItem(row,21,new QStandardItem(model_buttom->item(i,20)->text()));//做法加价
        fast_order_2::modelBase.setItem(row,22,new QStandardItem(model_buttom->item(i,21)->text()));//所属大类编码
        fast_order_2::modelBase.setItem(row,23,new QStandardItem(model_buttom->item(i,23)->text()));//所属小类编码
    }

    btn_t_f = true;
    QSqlQuery query;
    QSqlDatabase::database().transaction();
    query.prepare(QString("delete from cey_u_hungbill where int_flowID = :hanghao"));
    query.bindValue(":hanghao",model_top->item(ui->tableView->currentIndex().row(),0)->text());
    if(query.exec())
    {

    }
    else
    {
        QSqlDatabase::database().rollback();
        ;
        lds_messagebox::warning(this, tr("消息提示"),tr("删除cey_u_hungbill失败!"));
        
        return;
    }
    QSqlDatabase::database().commit();
    this->close();
}
