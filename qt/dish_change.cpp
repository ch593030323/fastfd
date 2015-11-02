#include "dish_change.h"
#include "ui_dish_change.h"
#include "fast_order_2.h"
#include <QSqlRecord>
#include <QScrollBar>
#include "table_order_2.h"

dish_change::dish_change(int num,const QString & str,QWidget *parent) :
    cur_row(num),_str(str),QDialog(parent),
    ui(new Ui::dish_change)
{
    ui->setupUi(this);
    //this->setWindowFlags(Qt::FramelessWindowHint|Qt::Dialog);this->setStyleSheet("QDialog{border:2px solid gray}");
    btn_t_f = false;
    quanbu = false;
    if(_str == "1")
    {
        if(fast_order_2::modelBase.item(cur_row,9)->text() == "Y")
        {
            ui->radioButton_15->setChecked(true);
            ui->radioButton_13->setChecked(false);
            ui->radioButton_14->setChecked(false);
        }
        else
        {
            ui->radioButton_13->setChecked(true);
            ui->radioButton_14->setChecked(false);
            ui->radioButton_15->setChecked(false);
            ui->radioButton_15->hide();
        }
    }
    else
    {
        if(table_order_2::modelBase.item(cur_row,9)->text() == "Y")
        {
            ui->radioButton_15->setChecked(true);
            ui->radioButton_13->setChecked(false);
            ui->radioButton_14->setChecked(false);
        }
        else
        {
            ui->radioButton_13->setChecked(true);
            ui->radioButton_14->setChecked(false);
            ui->radioButton_15->setChecked(false);
            ui->radioButton_15->hide();
        }
    }
    cur_num = 0;
    init_data();
}

dish_change::~dish_change()
{
    delete ui;
}

void dish_change::init_data()
{
    model = new QSqlQueryModel;
    QGridLayout *layout = new QGridLayout();
    if(_str == "1")
    {
        if(fast_order_2::modelBase.item(cur_row,9)->text() != "Y")//非套菜
        {
            model->setQuery(QString("SELECT a.ch_dishno, a.vch_dishname,a.vch_spell, a.vch_english, a.ch_dish_typeno,b.vch_unitname, a.num_price, a.ch_suitflag,a.ch_discount, a.ch_curprice, a.ch_tempflag,a.ch_stopflag, a.ch_printflag,a.ch_outflag,a.ch_sub_typeno, a.ch_materialno, a.ch_serviceflag,a.ch_lowflag, a.num_m_price, a.ch_deductflag,a.ch_deducttype, a.int_deduct, a.num_deduct,a.ch_saleflag, a.ch_saletype, a.int_sale_deduct,a.num_sale, a.item_flag, a.vch_pur_unitno,a.int_unit_rate, a.ch_off, a.vch_subno,a.num_sale_price , b.num_default  ,b.num_default ,d.vch_dish_typename,e.vch_sub_typename,a.vch_printer,a.vch_outprint,e.ch_labelprint_flag,a.int_rate FROM cey_bt_dish a , cey_bt_unit b ,cey_bt_dish_type d ,cey_bt_dish_subtype e  WHERE e.ch_dish_typeno = a.ch_dish_typeno and e.ch_sub_typeno = a.ch_sub_typeno and a.ch_dish_typeno = d.ch_dish_typeno  and a.ch_unitno = b.ch_unitno and a.ch_stopflag = 'N'  and ch_stopflag = 'N' and ch_suitflag <> 'Y' and (num_price = '%1') and (ch_dishno <> '%2') and a.ch_tempflag = 'N'").arg(fast_order_2::modelBase.item(cur_row,4)->text()).arg(fast_order_2::modelBase.item(cur_row,1)->text()));
            for(int i = 0; i < model->rowCount(); i++)
            {
                QString str_index;
                str_index = model->record(i).value(0).toString() + "\n" + model->record(i).value(1).toString() + "\n" + QString().sprintf("%0.2f",model->record(i).value(6).toFloat());
                list_info =  new lsit_label_info(i/5,i%5,str_index,"background-color:rgb(90,90,90);color:white;","background-color:rgb(255,0,0);color:white;");
                layout->addWidget(list_info,i/5,i%5,1,1,Qt::AlignTop);
                connect(list_info,SIGNAL(toType(int)),this,SLOT(show_label_num(int)));
            }

            layout->setAlignment(Qt::AlignTop);
            layout->setSpacing(10);
            layout->setContentsMargins(5,5,5,5);
            ui->scrollAreaWidgetContents_5->setLayout(layout);
            int count = model->rowCount();
            if(count > 20 && count%5 == 0)
            {
                ui->scrollAreaWidgetContents_5->resize(ui->scrollAreaWidgetContents_5->width(),count/5 * 90 + count/5 * 2);
            }
            else if(count > 20 && count%5 != 0)
            {
                ui->scrollAreaWidgetContents_5->resize( ui->scrollAreaWidgetContents_5->width(),count/5 * 90 + count/5 * 2 + 95);
            }
        }
        else//套菜
        {
            flowid = new QSqlQueryModel();
            flowid->setQuery(QString("SELECT  c.int_flowid  FROM cey_bt_dish a , cey_bt_unit b,cey_bt_dish_suit c WHERE (a.ch_unitno=b.ch_unitno) and  (a.ch_dishno = c.ch_dishno) and (c.ch_suitno = '%1')and c.ch_dishno = '%2'").arg(fast_order_2::modelBase.item(cur_row,16)->text()).arg(fast_order_2::modelBase.item(cur_row,1)->text()));
            QString id;
            if(flowid->rowCount() > 0)
            {
                id = flowid->record(0).value(0).toString();
            }

            model->setQuery(QString("SELECT a.ch_dishno, a.vch_dishname,a.vch_spell, a.vch_english, a.ch_dish_typeno,b.vch_unitname, a.num_price, a.ch_suitflag,a.ch_discount, a.ch_curprice, a.ch_tempflag,a.ch_stopflag, a.ch_printflag,a.ch_outflag,a.ch_sub_typeno, a.ch_materialno, a.ch_serviceflag,a.ch_lowflag, a.num_m_price, a.ch_deductflag,a.ch_deducttype, a.int_deduct, a.num_deduct,a.ch_saleflag, a.ch_saletype, a.int_sale_deduct,a.num_sale, a.item_flag, a.vch_pur_unitno,a.int_unit_rate, a.ch_off, a.vch_subno,a.num_sale_price , b.num_default  ,c.int_flowID ,d.vch_dish_typename,e.vch_sub_typename,a.vch_printer,a.vch_outprint,e.ch_labelprint_flag,a.int_rate FROM cey_bt_suit_change c,  cey_bt_dish  a ,cey_bt_unit b ,cey_bt_dish_type d ,cey_bt_dish_subtype e WHERE e.ch_dish_typeno = a.ch_dish_typeno and e.ch_sub_typeno = a.ch_sub_typeno and a.ch_dish_typeno = d.ch_dish_typeno and a.ch_unitno=b.ch_unitno and a.ch_stopflag = 'N' and c.ch_dishno = a.ch_dishno and  c.int_flowID = '%1'  and a.ch_tempflag = 'N'").arg(id));
            for(int i = 0; i < model->rowCount(); i++)
            {
                QString str_index;
                str_index = model->record(i).value(0).toString() + "\n" + model->record(i).value(1).toString() + "\n" + QString().sprintf("%0.2f",model->record(i).value(6).toFloat());
                list_info =  new lsit_label_info(i/5,i%5,str_index,"background-color:rgb(90,90,90);color:white;","background-color:rgb(255,0,0);color:white;");
                layout->addWidget(list_info,i/5,i%5,1,1,Qt::AlignTop);
                connect(list_info,SIGNAL(toType(int)),this,SLOT(show_label_num(int)));
            }


            layout->setAlignment(Qt::AlignTop);
            layout->setSpacing(10);
            layout->setContentsMargins(5,5,5,5);
            ui->scrollAreaWidgetContents_5->setLayout(layout);
            int count = model->rowCount();
            if(count > 20 && count%5 == 0)
            {
                ui->scrollAreaWidgetContents_5->resize(ui->scrollAreaWidgetContents_5->width(),count/5 * 90 + count/5 * 2);
            }
            else if(count > 20 && count%5 != 0)
            {
                ui->scrollAreaWidgetContents_5->resize( ui->scrollAreaWidgetContents_5->width(),count/5 * 90 + count/5 * 2 + 95);
            }
        }
    }
    else
    {
        if(table_order_2::modelBase.item(cur_row,9)->text() != "Y")//非套菜
        {
            model->setQuery(QString("SELECT a.ch_dishno, a.vch_dishname,a.vch_spell, a.vch_english, a.ch_dish_typeno,b.vch_unitname, a.num_price, a.ch_suitflag,a.ch_discount, a.ch_curprice, a.ch_tempflag,a.ch_stopflag, a.ch_printflag,a.ch_outflag,a.ch_sub_typeno, a.ch_materialno, a.ch_serviceflag,a.ch_lowflag, a.num_m_price, a.ch_deductflag,a.ch_deducttype, a.int_deduct, a.num_deduct,a.ch_saleflag, a.ch_saletype, a.int_sale_deduct,a.num_sale, a.item_flag, a.vch_pur_unitno,a.int_unit_rate, a.ch_off, a.vch_subno,a.num_sale_price , b.num_default  ,b.num_default ,d.vch_dish_typename,e.vch_sub_typename,a.vch_printer,a.vch_outprint,e.ch_labelprint_flag,a.int_rate FROM cey_bt_dish a , cey_bt_unit b ,cey_bt_dish_type d ,cey_bt_dish_subtype e  WHERE e.ch_dish_typeno = a.ch_dish_typeno and e.ch_sub_typeno = a.ch_sub_typeno and a.ch_dish_typeno = d.ch_dish_typeno  and a.ch_unitno = b.ch_unitno and a.ch_stopflag = 'N'  and ch_stopflag = 'N' and ch_suitflag <> 'Y' and (num_price = '%1') and (ch_dishno <> '%2') and a.ch_tempflag = 'N'").arg(table_order_2::modelBase.item(cur_row,4)->text()).arg(table_order_2::modelBase.item(cur_row,1)->text()));
            for(int i = 0; i < model->rowCount(); i++)
            {
                QString str_index;
                str_index = model->record(i).value(0).toString() + "\n" + model->record(i).value(1).toString() + "\n" + QString().sprintf("%0.2f",model->record(i).value(6).toFloat());
                list_info =  new lsit_label_info(i/5,i%5,str_index,"background-color:rgb(90,90,90);color:white;","background-color:rgb(255,0,0);color:white;");
                layout->addWidget(list_info,i/5,i%5,1,1,Qt::AlignTop);
                connect(list_info,SIGNAL(toType(int)),this,SLOT(show_label_num(int)));
            }

            layout->setAlignment(Qt::AlignTop);
            layout->setSpacing(10);
            layout->setContentsMargins(5,5,5,5);
            ui->scrollAreaWidgetContents_5->setLayout(layout);
            int count = model->rowCount();
            if(count > 20 && count%5 == 0)
            {
                ui->scrollAreaWidgetContents_5->resize(ui->scrollAreaWidgetContents_5->width(),count/5 * 90 + count/5 * 2);
            }
            else if(count > 20 && count%5 != 0)
            {
                ui->scrollAreaWidgetContents_5->resize( ui->scrollAreaWidgetContents_5->width(),count/5 * 90 + count/5 * 2 + 95);
            }
        }
        else//套菜
        {
            flowid = new QSqlQueryModel();
            flowid->setQuery(QString("SELECT  c.int_flowid  FROM cey_bt_dish a , cey_bt_unit b,cey_bt_dish_suit c WHERE (a.ch_unitno=b.ch_unitno) and  (a.ch_dishno = c.ch_dishno) and (c.ch_suitno = '%1')and c.ch_dishno = '%2'").arg(table_order_2::modelBase.item(cur_row,16)->text()).arg(table_order_2::modelBase.item(cur_row,1)->text()));
            QString id;
            if(flowid->rowCount() > 0)
            {
                id = flowid->record(0).value(0).toString();
            }

            model->setQuery(QString("SELECT a.ch_dishno, a.vch_dishname,a.vch_spell, a.vch_english, a.ch_dish_typeno,b.vch_unitname, a.num_price, a.ch_suitflag,a.ch_discount, a.ch_curprice, a.ch_tempflag,a.ch_stopflag, a.ch_printflag,a.ch_outflag,a.ch_sub_typeno, a.ch_materialno, a.ch_serviceflag,a.ch_lowflag, a.num_m_price, a.ch_deductflag,a.ch_deducttype, a.int_deduct, a.num_deduct,a.ch_saleflag, a.ch_saletype, a.int_sale_deduct,a.num_sale, a.item_flag, a.vch_pur_unitno,a.int_unit_rate, a.ch_off, a.vch_subno,a.num_sale_price , b.num_default  ,c.int_flowID ,d.vch_dish_typename,e.vch_sub_typename,a.vch_printer,a.vch_outprint,e.ch_labelprint_flag,a.int_rate FROM cey_bt_suit_change c,  cey_bt_dish  a ,cey_bt_unit b ,cey_bt_dish_type d ,cey_bt_dish_subtype e WHERE e.ch_dish_typeno = a.ch_dish_typeno and e.ch_sub_typeno = a.ch_sub_typeno and a.ch_dish_typeno = d.ch_dish_typeno and a.ch_unitno=b.ch_unitno and a.ch_stopflag = 'N' and c.ch_dishno = a.ch_dishno and  c.int_flowID = '%1'  and a.ch_tempflag = 'N'").arg(id));
            for(int i = 0; i < model->rowCount(); i++)
            {
                QString str_index;
                str_index = model->record(i).value(0).toString() + "\n" + model->record(i).value(1).toString() + "\n" + QString().sprintf("%0.2f",model->record(i).value(6).toFloat());
                list_info =  new lsit_label_info(i/5,i%5,str_index,"background-color:rgb(90,90,90);color:white;","background-color:rgb(255,0,0);color:white;");
                layout->addWidget(list_info,i/5,i%5,1,1,Qt::AlignTop);
                connect(list_info,SIGNAL(toType(int)),this,SLOT(show_label_num(int)));
            }


            layout->setAlignment(Qt::AlignTop);
            layout->setSpacing(10);
            layout->setContentsMargins(5,5,5,5);
            ui->scrollAreaWidgetContents_5->setLayout(layout);
            int count = model->rowCount();
            if(count > 20 && count%5 == 0)
            {
                ui->scrollAreaWidgetContents_5->resize(ui->scrollAreaWidgetContents_5->width(),count/5 * 90 + count/5 * 2);
            }
            else if(count > 20 && count%5 != 0)
            {
                ui->scrollAreaWidgetContents_5->resize( ui->scrollAreaWidgetContents_5->width(),count/5 * 90 + count/5 * 2 + 95);
            }
        }
    }
}

void dish_change::on_pushButton_13_clicked()//上一页
{
    ui->scrollArea->verticalScrollBar()->setValue(ui->scrollArea->verticalScrollBar()->value() - ui->scrollArea->height());
}

void dish_change::on_pushButton_14_clicked()//下一页
{
    ui->scrollArea->verticalScrollBar()->setValue(ui->scrollArea->verticalScrollBar()->value() + ui->scrollArea->height());
}

void dish_change::on_pushButton_16_clicked()//确定
{
    dish_no = model->record(cur_num).value(0).toString();
    dish_name = model->record(cur_num).value(1).toString();
    dish_num = model->record(cur_num).value(6).toFloat();
    dish_unit = model->record(cur_num).value(5).toString();
    btn_t_f  = true;
    this->close();
}

void dish_change::on_pushButton_15_clicked()//取消
{
    btn_t_f = false;
    this->close();
}

void dish_change::show_label_num(int num)
{
    cur_num = num;
}

void dish_change::on_radioButton_13_clicked()//1
{
    ui->radioButton_13->setChecked(true);
    ui->radioButton_14->setChecked(false);
    ui->radioButton_15->setChecked(false);
    lsit_label_info::flag = QPoint(0,0);
    lsit_label_info::lable1 = NULL;
    quanbu = false;
    qDeleteAll(ui->scrollAreaWidgetContents_5->children());
    QGridLayout *layout = new QGridLayout();
    model->clear();
    if(_str == "1")
    {
        model->setQuery(QString("SELECT a.ch_dishno, a.vch_dishname,a.vch_spell, a.vch_english, a.ch_dish_typeno,b.vch_unitname, a.num_price, a.ch_suitflag,a.ch_discount, a.ch_curprice, a.ch_tempflag,a.ch_stopflag, a.ch_printflag,a.ch_outflag,a.ch_sub_typeno, a.ch_materialno, a.ch_serviceflag,a.ch_lowflag, a.num_m_price, a.ch_deductflag,a.ch_deducttype, a.int_deduct, a.num_deduct,a.ch_saleflag, a.ch_saletype, a.int_sale_deduct,a.num_sale, a.item_flag, a.vch_pur_unitno,a.int_unit_rate, a.ch_off, a.vch_subno,a.num_sale_price , b.num_default  ,b.num_default ,d.vch_dish_typename,e.vch_sub_typename,a.vch_printer,a.vch_outprint,e.ch_labelprint_flag,a.int_rate FROM cey_bt_dish a , cey_bt_unit b ,cey_bt_dish_type d ,cey_bt_dish_subtype e  WHERE e.ch_dish_typeno = a.ch_dish_typeno and e.ch_sub_typeno = a.ch_sub_typeno and a.ch_dish_typeno = d.ch_dish_typeno  and a.ch_unitno = b.ch_unitno and a.ch_stopflag = 'N'  and ch_stopflag = 'N' and ch_suitflag <> 'Y' and (num_price = '%1') and (ch_dishno <> '%2') and a.ch_tempflag = 'N'").arg(fast_order_2::modelBase.item(cur_row,4)->text()).arg(fast_order_2::modelBase.item(cur_row,1)->text()));
        for(int i = 0; i < model->rowCount(); i++)
        {
            QString str_index;
            str_index = model->record(i).value(0).toString() + "\n" + model->record(i).value(1).toString() + "\n" + QString().sprintf("%0.2f",model->record(i).value(6).toFloat());
            list_info =  new lsit_label_info(i/5,i%5,str_index,"background-color:rgb(90,90,90);color:white;","background-color:rgb(255,0,0);color:white;");
            layout->addWidget(list_info,i/5,i%5,1,1,Qt::AlignTop);
            connect(list_info,SIGNAL(toType(int)),this,SLOT(show_label_num(int)));
        }
    }
    else
    {
        model->setQuery(QString("SELECT a.ch_dishno, a.vch_dishname,a.vch_spell, a.vch_english, a.ch_dish_typeno,b.vch_unitname, a.num_price, a.ch_suitflag,a.ch_discount, a.ch_curprice, a.ch_tempflag,a.ch_stopflag, a.ch_printflag,a.ch_outflag,a.ch_sub_typeno, a.ch_materialno, a.ch_serviceflag,a.ch_lowflag, a.num_m_price, a.ch_deductflag,a.ch_deducttype, a.int_deduct, a.num_deduct,a.ch_saleflag, a.ch_saletype, a.int_sale_deduct,a.num_sale, a.item_flag, a.vch_pur_unitno,a.int_unit_rate, a.ch_off, a.vch_subno,a.num_sale_price , b.num_default  ,b.num_default ,d.vch_dish_typename,e.vch_sub_typename,a.vch_printer,a.vch_outprint,e.ch_labelprint_flag,a.int_rate FROM cey_bt_dish a , cey_bt_unit b ,cey_bt_dish_type d ,cey_bt_dish_subtype e  WHERE e.ch_dish_typeno = a.ch_dish_typeno and e.ch_sub_typeno = a.ch_sub_typeno and a.ch_dish_typeno = d.ch_dish_typeno  and a.ch_unitno = b.ch_unitno and a.ch_stopflag = 'N'  and ch_stopflag = 'N' and ch_suitflag <> 'Y' and (num_price = '%1') and (ch_dishno <> '%2') and a.ch_tempflag = 'N'").arg(table_order_2::modelBase.item(cur_row,4)->text()).arg(table_order_2::modelBase.item(cur_row,1)->text()));
        for(int i = 0; i < model->rowCount(); i++)
        {
            QString str_index;
            str_index = model->record(i).value(0).toString() + "\n" + model->record(i).value(1).toString() + "\n" + QString().sprintf("%0.2f",model->record(i).value(6).toFloat());
            list_info =  new lsit_label_info(i/5,i%5,str_index,"background-color:rgb(90,90,90);color:white;","background-color:rgb(255,0,0);color:white;");
            layout->addWidget(list_info,i/5,i%5,1,1,Qt::AlignTop);
            connect(list_info,SIGNAL(toType(int)),this,SLOT(show_label_num(int)));
        }
    }
    layout->setAlignment(Qt::AlignTop);
    layout->setSpacing(10);
    layout->setContentsMargins(5,5,5,5);
    ui->scrollAreaWidgetContents_5->setLayout(layout);
    int count = model->rowCount();
    if(count > 20 && count%5 == 0)
    {
        ui->scrollAreaWidgetContents_5->resize(ui->scrollAreaWidgetContents_5->width(),count/5 * 90 + count/5 * 2);
    }
    else if(count > 20 && count%5 != 0)
    {
        ui->scrollAreaWidgetContents_5->resize( ui->scrollAreaWidgetContents_5->width(),count/5 * 90 + count/5 * 2 + 95);
    }
}

void dish_change::on_radioButton_14_clicked()//2
{
    ui->radioButton_13->setChecked(false);
    ui->radioButton_14->setChecked(true);
    ui->radioButton_15->setChecked(false);
    lsit_label_info::flag = QPoint(0,0);
    lsit_label_info::lable1 = NULL;
    quanbu = true;
    qDeleteAll(ui->scrollAreaWidgetContents_5->children());
    QGridLayout *layout = new QGridLayout();
    model->clear();
    if(_str == "1")
    {
        model->setQuery(QString("SELECT a.ch_dishno, a.vch_dishname,a.vch_spell, a.vch_english, a.ch_dish_typeno,b.vch_unitname, a.num_price, a.ch_suitflag,a.ch_discount, a.ch_curprice, a.ch_tempflag,a.ch_stopflag, a.ch_printflag,a.ch_outflag,a.ch_sub_typeno, a.ch_materialno, a.ch_serviceflag,a.ch_lowflag, a.num_m_price, a.ch_deductflag,a.ch_deducttype, a.int_deduct, a.num_deduct,a.ch_saleflag, a.ch_saletype, a.int_sale_deduct,a.num_sale, a.item_flag, a.vch_pur_unitno,a.int_unit_rate, a.ch_off, a.vch_subno,a.num_sale_price ,b.num_default  ,b.num_default ,c.vch_dish_typename,d.vch_sub_typename,a.vch_printer,a.vch_outprint,d.ch_labelprint_flag,a.int_rate FROM cey_bt_dish a , cey_bt_unit b,cey_bt_dish_type c ,cey_bt_dish_subtype d  WHERE  d.ch_dish_typeno = a.ch_dish_typeno and d.ch_sub_typeno = a.ch_sub_typeno and a.ch_dish_typeno = c.ch_dish_typeno and a.ch_unitno = b.ch_unitno and a.ch_stopflag = 'N' and a.ch_suitflag <> 'Y'  and (a.ch_dishno <> '%1') and a.ch_tempflag = 'N'").arg(fast_order_2::modelBase.item(cur_row,1)->text()));
        for(int i = 0; i < model->rowCount(); i++)
        {
            QString str_index;
            str_index = model->record(i).value(0).toString() + "\n" + model->record(i).value(1).toString() + "\n" + QString().sprintf("%0.2f",model->record(i).value(6).toFloat());
            list_info =  new lsit_label_info(i/5,i%5,str_index,"background-color:rgb(90,90,90);color:white;","background-color:rgb(255,0,0);color:white;");
            layout->addWidget(list_info,i/5,i%5,1,1,Qt::AlignTop);
            connect(list_info,SIGNAL(toType(int)),this,SLOT(show_label_num(int)));
        }
    }
    else
    {
        model->setQuery(QString("SELECT a.ch_dishno, a.vch_dishname,a.vch_spell, a.vch_english, a.ch_dish_typeno,b.vch_unitname, a.num_price, a.ch_suitflag,a.ch_discount, a.ch_curprice, a.ch_tempflag,a.ch_stopflag, a.ch_printflag,a.ch_outflag,a.ch_sub_typeno, a.ch_materialno, a.ch_serviceflag,a.ch_lowflag, a.num_m_price, a.ch_deductflag,a.ch_deducttype, a.int_deduct, a.num_deduct,a.ch_saleflag, a.ch_saletype, a.int_sale_deduct,a.num_sale, a.item_flag, a.vch_pur_unitno,a.int_unit_rate, a.ch_off, a.vch_subno,a.num_sale_price ,b.num_default  ,b.num_default ,c.vch_dish_typename,d.vch_sub_typename,a.vch_printer,a.vch_outprint,d.ch_labelprint_flag,a.int_rate FROM cey_bt_dish a , cey_bt_unit b,cey_bt_dish_type c ,cey_bt_dish_subtype d  WHERE  d.ch_dish_typeno = a.ch_dish_typeno and d.ch_sub_typeno = a.ch_sub_typeno and a.ch_dish_typeno = c.ch_dish_typeno and a.ch_unitno = b.ch_unitno and a.ch_stopflag = 'N' and a.ch_suitflag <> 'Y'  and (a.ch_dishno <> '%1') and a.ch_tempflag = 'N'").arg(table_order_2::modelBase.item(cur_row,1)->text()));
        for(int i = 0; i < model->rowCount(); i++)
        {
            QString str_index;
            str_index = model->record(i).value(0).toString() + "\n" + model->record(i).value(1).toString() + "\n" + QString().sprintf("%0.2f",model->record(i).value(6).toFloat());
            list_info =  new lsit_label_info(i/5,i%5,str_index,"background-color:rgb(90,90,90);color:white;","background-color:rgb(255,0,0);color:white;");
            layout->addWidget(list_info,i/5,i%5,1,1,Qt::AlignTop);
            connect(list_info,SIGNAL(toType(int)),this,SLOT(show_label_num(int)));
        }
    }
    layout->setAlignment(Qt::AlignTop);
    layout->setSpacing(10);
    layout->setContentsMargins(5,5,5,5);
    ui->scrollAreaWidgetContents_5->setLayout(layout);
    int count = model->rowCount();
    if(count > 20 && count%5 == 0)
    {
        ui->scrollAreaWidgetContents_5->resize(ui->scrollAreaWidgetContents_5->width(),count/5 * 90 + count/5 * 2);
    }
    else if(count > 20 && count%5 != 0)
    {
        ui->scrollAreaWidgetContents_5->resize( ui->scrollAreaWidgetContents_5->width(),count/5 * 90 + count/5 * 2 + 95);
    }
}

void dish_change::on_radioButton_15_clicked()//3
{
    quanbu  = false;
    ui->radioButton_13->setChecked(false);
    ui->radioButton_14->setChecked(false);
    ui->radioButton_15->setChecked(true);
    lsit_label_info::flag = QPoint(0,0);
    lsit_label_info::lable1 = NULL;
    QGridLayout *layout = new QGridLayout();

    flowid->clear();
    if(_str == "1")
    {
        flowid->setQuery(QString("SELECT  c.int_flowid  FROM cey_bt_dish a , cey_bt_unit b,cey_bt_dish_suit c WHERE (a.ch_unitno=b.ch_unitno) and  (a.ch_dishno = c.ch_dishno) and (c.ch_suitno = '%1')and c.ch_dishno = '%2'").arg(fast_order_2::modelBase.item(cur_row,16)->text()).arg(fast_order_2::modelBase.item(cur_row,1)->text()));
    }
    else
    {
        flowid->setQuery(QString("SELECT  c.int_flowid  FROM cey_bt_dish a , cey_bt_unit b,cey_bt_dish_suit c WHERE (a.ch_unitno=b.ch_unitno) and  (a.ch_dishno = c.ch_dishno) and (c.ch_suitno = '%1')and c.ch_dishno = '%2'").arg(table_order_2::modelBase.item(cur_row,16)->text()).arg(table_order_2::modelBase.item(cur_row,1)->text()));
    }
    QString id;
    if(flowid->rowCount() > 0)
    {
        id = flowid->record(0).value(0).toString();
    }

    model->clear();
    model->setQuery(QString("SELECT a.ch_dishno, a.vch_dishname,a.vch_spell, a.vch_english, a.ch_dish_typeno,b.vch_unitname, a.num_price, a.ch_suitflag,a.ch_discount, a.ch_curprice, a.ch_tempflag,a.ch_stopflag, a.ch_printflag,a.ch_outflag,a.ch_sub_typeno, a.ch_materialno, a.ch_serviceflag,a.ch_lowflag, a.num_m_price, a.ch_deductflag,a.ch_deducttype, a.int_deduct, a.num_deduct,a.ch_saleflag, a.ch_saletype, a.int_sale_deduct,a.num_sale, a.item_flag, a.vch_pur_unitno,a.int_unit_rate, a.ch_off, a.vch_subno,a.num_sale_price , b.num_default  ,c.int_flowID ,d.vch_dish_typename,e.vch_sub_typename,a.vch_printer,a.vch_outprint,e.ch_labelprint_flag,a.int_rate FROM cey_bt_suit_change c,  cey_bt_dish  a ,cey_bt_unit b ,cey_bt_dish_type d ,cey_bt_dish_subtype e WHERE e.ch_dish_typeno = a.ch_dish_typeno and e.ch_sub_typeno = a.ch_sub_typeno and a.ch_dish_typeno = d.ch_dish_typeno and a.ch_unitno=b.ch_unitno and a.ch_stopflag = 'N' and c.ch_dishno = a.ch_dishno and  c.int_flowID = '%1'  and a.ch_tempflag = 'N'").arg(id));
    for(int i = 0; i < model->rowCount(); i++)
    {
        QString str_index;
        str_index = model->record(i).value(0).toString() + "\n" + model->record(i).value(1).toString() + "\n" + QString().sprintf("%0.2f",model->record(i).value(6).toFloat());
        list_info =  new lsit_label_info(i/5,i%5,str_index,"background-color:rgb(90,90,90);color:white;","background-color:rgb(255,0,0);color:white;");
        layout->addWidget(list_info,i/5,i%5,1,1,Qt::AlignTop);
        connect(list_info,SIGNAL(toType(int)),this,SLOT(show_label_num(int)));
    }

    layout->setAlignment(Qt::AlignTop);
    layout->setSpacing(10);
    layout->setContentsMargins(5,5,5,5);
    ui->scrollAreaWidgetContents_5->setLayout(layout);
    int count = model->rowCount();
    if(count > 20 && count%5 == 0)
    {
        ui->scrollAreaWidgetContents_5->resize(ui->scrollAreaWidgetContents_5->width(),count/5 * 90 + count/5 * 2);
    }
    else if(count > 20 && count%5 != 0)
    {
        ui->scrollAreaWidgetContents_5->resize( ui->scrollAreaWidgetContents_5->width(),count/5 * 90 + count/5 * 2 + 95);
    }
}
