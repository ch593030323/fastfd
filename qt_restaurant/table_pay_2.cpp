#include "table_pay_2.h"
#include "frontviewdialog_virtual.h"
#include "w_scr_dish_restaurant_dialog.h"
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QSqlError>
#include "n_func.h"

table_pay_2::table_pay_2(frontViewDialog_virtual *parent) :
    fast_pay(parent)
{
    jiuloushow();
}
void table_pay_2::jiuloushow()
{
    QSqlQueryModel person;
    QString table_renshu;
    QSqlQueryModel canzhuo;
    QDateTime pay_time;
    QString areano;
    QString dixiao_type;
    QSqlQueryModel model_this;
    QString str;
    QString dixiao;

    this->setjiulou_visiable(true);
    person.setQuery(QString("select int_person,vch_memo from cey_u_table where ch_billno= '%1' ").arg(w_scr_dish_restaurant_dialog::table_danhao));
    table_renshu = person.record(0).value(0).toString();

    //    ui->lineEdit_13->setText();//低消
    ui->lineEdit_11->setText(frontViewDialog_virtual::k_beizhu);//备注
    ui->lineEdit_12->setText(table_renshu);//人数
    ui->lineEdit_15->setText(w_scr_dish_restaurant_dialog::table_danhao);//单号
    ui->lineEdit_8->setText(frontViewDialog_virtual::cur_desk_no);
    ///2

    pay_time=QDateTime::currentDateTime();
    float yingshou = 0.00;
    canzhuo.setQuery(QString("select ch_typeno from cey_bt_table where ch_tableno = '%1' ").arg(frontViewDialog_virtual::cur_desk_no));
    areano = canzhuo.record(0).value(0).toString();
    canzhuo.clear();
    canzhuo.setQuery(QString("select ch_service_mode, ch_low_mode, num_lowcost from cey_bt_table_type where ch_typeno = '%1' ").arg(areano));
    int server_type = 0;
    server_type = canzhuo.record(0).value("ch_service_mode").toInt();
    yingshou=ui->lineEdit_4->text().toFloat();

    pay_servicecost=n_func::f_ub_get_service(w_scr_dish_restaurant_dialog::table_danhao,
                             frontViewDialog_virtual::cur_desk_no,
                             ""
                             );

    dixiao_type = canzhuo.record(0).value("ch_low_mode").toString();
    if(dixiao_type == "1")//餐桌低消
    {
        pay_lowcost = canzhuo.record(0).value("num_lowcost").toFloat();
    }
    else//人均低消
    {
        person.setQuery(QString("select int_person from cey_u_table where ch_billno= '%1' ").arg(w_scr_dish_restaurant_dialog::table_danhao));
        pay_lowcost = person.record(0).value(0).toFloat() * canzhuo.record(0).value("num_lowcost").toFloat();
    }
    model_this.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='service_handwork' "));//手工处理服务费
    str = model_this.record(0).value(0).toString();
    if(str != "0")
    {
        pay_servicecost = 0;
    }

    //服务费计入低消费
    model_this.clear();
    model_this.setQuery(QString("Select vch_value from cey_sys_parameter where vch_parameter ='low_service' "));
    dixiao = model_this.record(0).value(0).toString();
    if(dixiao != "0")
    {
        pay_servicecost = 0;
    }
    update_lowcost_service();
    ui->lineEdit_9->setText(ui->lineEdit_autualpay->text());

}

void table_pay_2::to_pay()
{
    fast_pay::to_pay();
}

void table_pay_2::tokitchenprint(const QString &key, const QString &_dayindanhao, const QString &serial_no)
{
    //中餐结算不厨打
}

void table_pay_2::tolabelprint()
{
    //中餐结算不打印奶茶
}

