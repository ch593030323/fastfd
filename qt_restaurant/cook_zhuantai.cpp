#include "cook_zhuantai.h"
#include "ui_cook_zhuantai.h"
#include <QSqlQueryModel>
#include <QSqlRecord>
#include "lds_tableview_delegate_check.h"
#include "w_scr_dish_restaurant_dialog.h"
#include <QDebug>
#include "lds_messagebox.h"
#include "cook_operation.h"
#include "backheader.h"

cook_zhuantai::cook_zhuantai(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::cook_zhuantai)
{
    ui->setupUi(this);
    //this->setWindowFlags(Qt::FramelessWindowHint|Qt::Dialog);this->setStyleSheet("QDialog{border:2px solid gray}");
    btn_t_f = false;
    init_control();
}

cook_zhuantai::~cook_zhuantai()
{
    delete ui;
}

void cook_zhuantai::init_control()
{
//    ui->tableView->verticalHeader()->hide();
//    ui->tableView->horizontalHeader()->setStyleSheet("QHeaderView::section{background-color: qlineargradient(spread:reflect, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(255, 255, 74, 255), stop:1 rgba(255, 255,21, 255));color:black;height:35px;border-radius:0px;font-size:14pt;}");
//    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
//    ui->tableView->verticalHeader()->setDefaultSectionSize(50);
    // //ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置属性为只读
    //ui->tableView->horizontalHeader()->setResizeMode(QHeaderView::Stretch );//文字自适应表格

    QStringList labels;
    labels <<tr("转台标志")<<tr("行号")<<tr("品号")<<tr("品名")<<"单位" <<tr("价格")<<tr("数量")<<tr("厨打")<<tr("套菜")<<tr("赠送")<<tr("特殊做法");
    table_model = new QStandardItemModel();
    table_model->setHorizontalHeaderLabels(labels);
    //    ui->tableView->setModel(table_model);


    ui->tableView->setModel(table_model);
    lds_tableview_delegate_check *delegate=new lds_tableview_delegate_check(QVector<int>() << 0 << 7 << 8 << 9 ,ui->tableView);
    delegate->setClickable();
    ui->tableView->setItemDelegate(delegate) ;

    QSqlQueryModel model;
    model.setQuery(QString("select a.int_flowID,a.ch_billno,a.ch_tableno,a.int_id,a.ch_dishno,b.vch_dishname,c.vch_unitname,a.num_price,a.num_num,a.num_back,a.vch_back_operID,a.dt_back_operdate,a.int_discount,a.vch_dis_operID,a.dt_dis_operdate,a.ch_printflag,a.vch_print_memo,a.ch_suitflag,a.ch_suitno,a.ch_specialflag, a.ch_presentflag,a.vch_pre_operID,a.dt_pre_operdate,a.vch_operID, a.dt_operdate,a.ch_togono,a.ch_payno, 'N' FROM cey_bt_dish b, cey_u_orderdish a , cey_bt_unit c WHERE(a.ch_dishno = b.ch_dishno)and( a.ch_billno = '%1')and(a.ch_tableno = '%2')and b.ch_unitno = c.ch_unitno ").arg(w_scr_dish_restaurant_dialog::table_danhao).arg(frontViewDialog_virtual::cur_desk_no));
   // qDebug() << QString("a.int_flowID,a.ch_billno,a.ch_tableno,a.int_id,a.ch_dishno,b.vch_dishname,c.vch_unitname,a.num_price,a.num_num,a.num_back,a.vch_back_operID,a.dt_back_operdate,a.int_discount,a.vch_dis_operID,a.dt_dis_operdate,a.ch_printflag,a.vch_print_memo,a.ch_suitflag,a.ch_suitno,a.ch_specialflag, a.ch_presentflag,a.vch_pre_operID,a.dt_pre_operdate,a.vch_operID, a.dt_operdate,a.ch_togono,a.ch_payno, 'N' FROM cey_bt_dish b, cey_u_orderdish a , cey_bt_unit c WHERE(a.ch_dishno = b.ch_dishno)and( a.ch_billno = '%1')and(a.ch_tableno = '%2')and b.ch_unitno = c.ch_unitno ").arg(w_scr_dish_restaurant_dialog::table_danhao).arg(frontViewDialog_virtual::cur_desk_no);
    for(int i = 0; i  < model.rowCount(); i++)
    {
        int row = table_model->rowCount();
        table_model->setRowCount(row + 1);

        table_model->setItem(row,0,new QStandardItem("N")); //转台标志
        table_model->setItem(row,1,new QStandardItem(QString::number(row + 1))); //行号
        table_model->setItem(row,2,new QStandardItem(model.record(i).value(4).toString())); //品号
        table_model->setItem(row,3,new QStandardItem(model.record(i).value(5).toString())); //品名
        table_model->setItem(row,4,new QStandardItem(model.record(i).value(6).toString())); //单位
        table_model->setItem(row,5,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(7).toFloat()))); //价格
        table_model->setItem(row,6,new QStandardItem(QString().sprintf("%0.2f",model.record(i).value(8).toFloat()))); //数量
        table_model->setItem(row,7,new QStandardItem(model.record(i).value(15).toString())); //厨打
        table_model->setItem(row,8,new QStandardItem(model.record(i).value(17).toString())); //套菜
        table_model->setItem(row,9,new QStandardItem(model.record(i).value(20).toString())); //赠送
        table_model->setItem(row,10,new QStandardItem(model.record(i).value(16).toString())); //特殊做法
        table_model->setItem(row,11,new QStandardItem(model.record(i).value(0).toString()));//flow_id
        table_model->setItem(row,12,new QStandardItem(model.record(i).value(3).toString()));//int_id
    }

    QFont font;
    font.setPointSize(14);


    ui->tableView->setColumnHidden(11,true);
    ui->tableView->setColumnHidden(12,true);
    if(table_model->rowCount() > 0)
    {
        ui->tableView->selectRow(0);
    }
}

void cook_zhuantai::on_pushButton_15_clicked()
{
    this->close();
}

void cook_zhuantai::on_pushButton_14_clicked()//全选
{
    for(int i = 0; i < table_model->rowCount(); i++)
    {
        table_model->item(i,0)->setText("Y");
    }
}

void cook_zhuantai::on_pushButton_13_clicked()//全消
{
    for(int i = 0; i < table_model->rowCount(); i++)
    {
        table_model->item(i,0)->setText("N");
    }
}

void cook_zhuantai::on_pushButton_7_clicked()//下一步
{
    ;
    int num = 0;
    for(int i = 0; i < table_model->rowCount(); i++)
    {
        if(table_model->item(i,0)->text() == "Y")
        {
            num++;
        }
    }
    if(num <= 0)
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("没有指定转菜记录!"));
        
        return;
    }

    cook_operation *operation = new cook_operation("1", this);
    /*operation->exec();*/backheader::static_dialogexec(*operation, "");
    if(operation->btn_t_f = true)//转台
    {
        btn_t_f = true;
        print = operation->print;
        zhuohao = operation->zhuan_zhuohao;
        danhao = operation->zhuan_danhao;
        flow_id.clear();
        int_id.clear();
        hanghao.clear();
        for(int i = 0; i < table_model->rowCount(); i++)
        {
            if(table_model->item(i,0)->text() == "Y")
            {
                flow_id << table_model->item(i,11)->text();
                int_id << table_model->item(i,12)->text();
                hanghao << table_model->item(i,1)->text();
            }
        }
    }
    else
    {
        btn_t_f = false;
        return;
    }

    this->close();
}
