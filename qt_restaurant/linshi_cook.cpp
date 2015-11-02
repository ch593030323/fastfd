#include "linshi_cook.h"
#include "ui_linshi_cook.h"
#include <QDebug>
#include "add_linshi_cook.h"
#include "backheader.h"

linshi_cook::linshi_cook(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::linshi_cook)
{
    ui->setupUi(this);
    //this->setWindowFlags(Qt::FramelessWindowHint|Qt::Dialog);this->setStyleSheet("QDialog{border:2px solid gray}");
    btn_t_f = false;

    item_count_zilei = 30;//商品列表中按钮的个数
    item_count_row_zilei = 5;//商品列表中一行个数
    init_control();
}

linshi_cook::~linshi_cook()
{
    delete ui;
}

void linshi_cook::init_control()
{
    model_xiaolei = new QSqlQueryModel;
    qDeleteAll(itemList_zilei.begin(),itemList_zilei.end());
    itemList_zilei.clear();
    layout_zilei = new QGridLayout();
    for(int i = 0; i < item_count_zilei; ++i)
    {
        goodItem_zilei *item = new goodItem_zilei("","");
        itemList_zilei.append(item);
        connect(item,SIGNAL(touch_zilei(QString)),this,SLOT(dis_xiaolei_str(QString)));
        layout_zilei->addWidget(item,i/item_count_row_zilei,i%item_count_row_zilei);
    }
    layout_zilei->setContentsMargins(0,9,0,0);
    layout_zilei->setSpacing(6);
    ui->frame->setLayout(layout_zilei);

    model_xiaolei->clear();
    // model_xiaolei->setQuery(QString("SELECT a.ch_dishno, a.vch_dishname,a.vch_spell, a.vch_english, a.ch_dish_typeno,b.vch_unitname, a.num_price, a.ch_suitflag,a.ch_discount, a.ch_curprice, a.ch_tempflag,a.ch_stopflag, a.ch_printflag,a.ch_outflag,a.ch_sub_typeno, a.ch_materialno, a.ch_serviceflag,a.ch_lowflag, a.num_m_price, a.ch_deductflag,a.ch_deducttype, a.int_deduct, a.num_deduct,a.ch_saleflag, a.ch_saletype, a.int_sale_deduct,a.num_sale, a.item_flag, a.vch_pur_unitno,a.int_unit_rate, a.ch_off, a.vch_subno,a.num_sale_price , b.num_default  ,0 ,c.vch_dish_typename,d.vch_sub_typename,a.vch_printer,a.vch_outprint,d.ch_labelprint_flag,a.int_rate FROM cey_bt_dish a , cey_bt_unit b,cey_bt_dish_type c ,cey_bt_dish_subtype d WHERE d.ch_dish_typeno = a.ch_dish_typeno and d.ch_sub_typeno = a.ch_sub_typeno and a.ch_dish_typeno = c.ch_dish_typeno and  a.ch_tempflag = 'Y'  and  a.ch_stopflag = 'N' "));
    model_xiaolei->setQuery(QString("select * from cey_bt_dish where ch_tempflag = 'Y'  and  ch_stopflag = 'N' "));
    QString kind = model_xiaolei->record(0).value(0).toString();
    showGoods_zilei(kind);//qDebug() << QString("SELECT a.ch_dishno, a.vch_dishname,a.vch_spell, a.vch_english, a.ch_dish_typeno,b.vch_unitname, a.num_price, a.ch_suitflag,a.ch_discount, a.ch_curprice, a.ch_tempflag,a.ch_stopflag, a.ch_printflag,a.ch_outflag,a.ch_sub_typeno, a.ch_materialno, a.ch_serviceflag,a.ch_lowflag, a.num_m_price, a.ch_deductflag,a.ch_deducttype, a.int_deduct, a.num_deduct,a.ch_saleflag, a.ch_saletype, a.int_sale_deduct,a.num_sale, a.item_flag, a.vch_pur_unitno,a.int_unit_rate, a.ch_off, a.vch_subno,a.num_sale_price , b.num_default  ,0 ,c.vch_dish_typename,d.vch_sub_typename,a.vch_printer,a.vch_outprint,d.ch_labelprint_flag,a.int_rate FROM cey_bt_dish a , cey_bt_unit b,cey_bt_dish_type c ,cey_bt_dish_subtype d WHERE d.ch_dish_typeno = a.ch_dish_typeno and d.ch_sub_typeno = a.ch_sub_typeno and a.ch_dish_typeno = c.ch_dish_typeno and  a.ch_tempflag = 'Y'  and  a.ch_stopflag = 'N' and a.ch_unitno=b.ch_unitno ");
}

bool linshi_cook::showGoods_zilei(QString kindId, int index)
{
    if(kindId.isEmpty())
        return false;
    //保存当前页的信息
    goodIndex_xiaolei = index;
    goodKind_xiaolei = kindId;
    int preCount;//之前的商品数量
    if(0 == index)
    {
        preCount = 0;
    }
    else
    {
        preCount = item_count_zilei-1 + (index-1)*(item_count_zilei-2);
    }

    model_xiaolei->clear();
    // model_xiaolei->setQuery(QString("SELECT a.ch_dishno, a.vch_dishname,a.vch_spell, a.vch_english, a.ch_dish_typeno,b.vch_unitname, a.num_price, a.ch_suitflag,a.ch_discount, a.ch_curprice, a.ch_tempflag,a.ch_stopflag, a.ch_printflag,a.ch_outflag,a.ch_sub_typeno, a.ch_materialno, a.ch_serviceflag,a.ch_lowflag, a.num_m_price, a.ch_deductflag,a.ch_deducttype, a.int_deduct, a.num_deduct,a.ch_saleflag, a.ch_saletype, a.int_sale_deduct,a.num_sale, a.item_flag, a.vch_pur_unitno,a.int_unit_rate, a.ch_off, a.vch_subno,a.num_sale_price , b.num_default  ,0 ,c.vch_dish_typename,d.vch_sub_typename,a.vch_printer,a.vch_outprint,d.ch_labelprint_flag,a.int_rate FROM cey_bt_dish a , cey_bt_unit b,cey_bt_dish_type c ,cey_bt_dish_subtype d WHERE d.ch_dish_typeno = a.ch_dish_typeno and d.ch_sub_typeno = a.ch_sub_typeno and a.ch_dish_typeno = c.ch_dish_typeno and  a.ch_tempflag = 'Y'  and  a.ch_stopflag = 'N' limit %1,%2 ").arg(preCount).arg(item_count_zilei+1));
    model_xiaolei->setQuery(QString("select * from cey_bt_dish where ch_tempflag = 'Y' and ch_stopflag = 'N' limit  %1,%2 ").arg(preCount).arg(item_count_zilei+1));

    int count = 0;
    if(0 == index)       //如果是第一页
    {
        for(int i = 0; i < model_xiaolei->rowCount(); i++)
        {
            if(count < item_count_zilei)      //填如所有商品到列表
            {
                itemList_zilei.value(count)->_barcode = model_xiaolei->record(i).value(0).toString();
                itemList_zilei.value(count)->_isSelect = false;
                itemList_zilei.value(count)->setName(model_xiaolei->record(i).value(1).toString());
            }
            ++count;
        }
        if(count < item_count_zilei)
        {
            //如果商品不足列表则填满
            for(;count < item_count_zilei;++count)
            {
                itemList_zilei.value(count)->_barcode = "";
                itemList_zilei.value(count)->_isSelect = false;
                itemList_zilei.value(count)->setName("");
            }
        }
        else if(count == item_count_zilei)
        {
            //如果等于列表数量则完全填满
        }
        else if(count > item_count_zilei)
        {
            //如果大于则代表有下一页，最后一个商品设置为下一页
            itemList_zilei.value(itemList_zilei.count()-1)->_barcode = "next";
            itemList_zilei.value(itemList_zilei.count()-1)->_isSelect = false;
            itemList_zilei.value(itemList_zilei.count()-1)->setName(tr("下一页"));
        }
    }
    else if(index > 0)   //不是第一页
    {
        //添加上一页
        itemList_zilei.value(0)->_barcode = "pre";
        itemList_zilei.value(0)->_isSelect = false;
        itemList_zilei.value(0)->setName(tr("上一页"));

        for(int i = 0; i < model_xiaolei->rowCount(); i++)
        {
            if(count < item_count_zilei-1)      //填入之后所有商品到列表
            {
                itemList_zilei.value(count+1)->_barcode = model_xiaolei->record(i).value(0).toString();
                itemList_zilei.value(count+1)->_isSelect = false;
                itemList_zilei.value(count+1)->setName(model_xiaolei->record(i).value(1).toString());
            }
            ++count;
        }

        if(count < item_count_zilei - 1)  //如果商品不足剩下的
        {
            //如果商品不足列表则填满
            for(;count < item_count_zilei-1;++count)
            {
                itemList_zilei.value(count+1)->_barcode = "";
                itemList_zilei.value(count+1)->_isSelect = false;
                itemList_zilei.value(count+1)->setName("");
            }
        }
        else if(count == item_count_zilei - 1)
        {
            //如果刚好后下面的则满上
        }
        else if(count > item_count_zilei - 1)
        {
            //如果大于则代表有下一页，最后一个商品设置为下一页
            itemList_zilei.value(itemList_zilei.count()-1)->_barcode = "next";
            itemList_zilei.value(itemList_zilei.count()-1)->_isSelect = false;
            itemList_zilei.value(itemList_zilei.count()-1)->setName(tr("下一页"));
        }
    }
    return true;
}

void linshi_cook::dis_xiaolei_str(const QString &barcode)
{
    if(barcode.isEmpty())
        return;
    if(barcode != "pre" && barcode != "next")
    {
        no = barcode;
    }
    if(barcode == "pre")
    {
        if(goodIndex_xiaolei <= 0)
            return;

        --goodIndex_xiaolei;
        showGoods_zilei(goodKind_xiaolei,goodIndex_xiaolei);
        return;
    }
    else if(barcode == "next")
    {
        ++goodIndex_xiaolei;
        if(!showGoods_zilei(goodKind_xiaolei,goodIndex_xiaolei))
        {
            --goodIndex_xiaolei;
        }
        return;
    }
    if(!currentGoodItem_zilei.isNull() && currentGoodItem_zilei != sender())
        currentGoodItem_zilei->setSelect(false);

    if(sender() != NULL)
        currentGoodItem_zilei = static_cast<goodItem_zilei*>(sender());
}

void linshi_cook::on_pushButton_14_clicked()
{
    btn_t_f = false;
    this->close();
}

void linshi_cook::on_pushButton_15_clicked()//新增
{
    add_linshi_cook add_linshi(this);
    backheader::static_dialogexec(add_linshi, tr("新增临时菜"));
    if(add_linshi.btn_t_f == true)
    {
        qDeleteAll(itemList_zilei.begin(),itemList_zilei.end());
        itemList_zilei.clear();
        layout_zilei = new QGridLayout();
        qDeleteAll(ui->frame->children());
        for(int i = 0; i < item_count_zilei; ++i)
        {
            goodItem_zilei *item = new goodItem_zilei("","");
            itemList_zilei.append(item);
            connect(item,SIGNAL(touch_zilei(QString)),this,SLOT(dis_xiaolei_str(QString)));
            layout_zilei->addWidget(item,i/item_count_row_zilei,i%item_count_row_zilei);
        }
        layout_zilei->setContentsMargins(0,6,0,0);
        layout_zilei->setSpacing(9);
        ui->frame->setLayout(layout_zilei);

        model_xiaolei->clear();
        // model_xiaolei->setQuery(QString("SELECT a.ch_dishno, a.vch_dishname,a.vch_spell, a.vch_english, a.ch_dish_typeno,b.vch_unitname, a.num_price, a.ch_suitflag,a.ch_discount, a.ch_curprice, a.ch_tempflag,a.ch_stopflag, a.ch_printflag,a.ch_outflag,a.ch_sub_typeno, a.ch_materialno, a.ch_serviceflag,a.ch_lowflag, a.num_m_price, a.ch_deductflag,a.ch_deducttype, a.int_deduct, a.num_deduct,a.ch_saleflag, a.ch_saletype, a.int_sale_deduct,a.num_sale, a.item_flag, a.vch_pur_unitno,a.int_unit_rate, a.ch_off, a.vch_subno,a.num_sale_price , b.num_default  ,0 ,c.vch_dish_typename,d.vch_sub_typename,a.vch_printer,a.vch_outprint,d.ch_labelprint_flag,a.int_rate FROM cey_bt_dish a , cey_bt_unit b,cey_bt_dish_type c ,cey_bt_dish_subtype d WHERE d.ch_dish_typeno = a.ch_dish_typeno and d.ch_sub_typeno = a.ch_sub_typeno and a.ch_dish_typeno = c.ch_dish_typeno and  a.ch_tempflag = 'Y'  and  a.ch_stopflag = 'N' and a.ch_unitno=b.ch_unitno "));
        model_xiaolei->setQuery(QString("select * from cey_bt_dish where ch_tempflag = 'Y'  and  ch_stopflag = 'N' "));
        QString kind = model_xiaolei->record(0).value(0).toString();
        showGoods_zilei(kind);
    }
}

void linshi_cook::on_pushButton_7_clicked()
{
    btn_t_f = true;
    this->close();
}
