#include "w_scr_dish_switch_detail.h"
#include "ui_w_scr_dish_switch_detail.h"
#include "public_sql.h"
#include <QSqlQueryModel>
#include "n_func.h"
#include <QSqlRecord>
#include "backheader.h"

w_scr_dish_switch_detail::w_scr_dish_switch_detail(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::w_scr_dish_switch_detail)
{
    ui->setupUi(this);
    ui->frame->setup(QStringList()<<tr("交班")<<tr("退出"), Qt::AlignRight);
    ui->tableWidget->setEditTriggers(QTableWidget::NoEditTriggers);
    //1
    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    QString date0,date1;
    QSqlQuery query;
    int currow;
    ui->checkBox->setChecked(conf.value("shift_work/isprint").toBool());
    connect(ui->checkBox,SIGNAL(toggled(bool)),this,SLOT(tosaveini()));
    {
        //2
        ui->tableWidget->setSpan(0,1,1,2);
        ui->tableWidget->setSpan(2,1,1,3);
        ui->tableWidget->setSpan(3,1,1,3);
        ui->tableWidget->setSpan(4,0,1,4);
        ui->tableWidget->setSpan(15,1,1,3);
        //2.1
        printins.append(backheader::printer->drawText("1B6101 1D2111", ui->tableWidget->item(0, 1)->text()+"\n"));
        //3
        //交班人
        QSqlQueryModel model;
        if(n_func::gs_operid == "0000")
        {
            ui->tableWidget->item(1,1)->setText(tr("system(0000)"));
        }
        else
        {
            model.setQuery(QString("select vch_operator from cey_sys_operator where vch_operID = '%1' ").arg(n_func::gs_operid));
            QString str = model.record(0).value(0).toString().trimmed() + tr("(") + n_func::gs_operid + tr(")");
            ui->tableWidget->item(1,1)->setText(str);
        }
        //接班人
        if(swtich_oper == "0000")
        {
            ui->tableWidget->item(1,3)->setText(tr("system(0000)"));
        }
        else
        {
            model.setQuery(QString("select vch_operator from cey_sys_operator where vch_operID = '%1' ").arg(swtich_oper));
            QString str = model.record(0).value(0).toString().trimmed() + tr("(") + swtich_oper + tr(")");
            ui->tableWidget->item(1,3)->setText(str);
        }
        printins.append(backheader::printer->drawText("1B6100 1D2100", ui->tableWidget->item(1, 0)->text()+":"+ui->tableWidget->item(1, 1)->text()
                                                      +"     "+ui->tableWidget->item(1, 2)->text()+":"+ui->tableWidget->item(1, 3)->text()+"\n"
                                                      ));
        //开始时间
        model.setQuery(QString("Select max( dt_end)from cey_u_handover_master where vch_operID ='%1'").arg(n_func::gs_operid));
        if(!model.record(0).value(0).toString().isEmpty())
        {
            date0=(model.record(0).value(0).toDateTime().toString("yyyy-MM-dd hh:mm:ss"));
        }
        else
        {

            model.setQuery(QString("Select Min( dt_operdate)from cey_u_checkout_master where vch_operid ='%1'").arg(n_func::gs_operid));
            date0=(model.record(0).value(0).toDateTime().toString("yyyy-MM-dd hh:mm:ss"));
        }
        ui->tableWidget->item(2,1)->setText(date0);
        printins.append(backheader::printer->drawText("1B6100 1D2100", ui->tableWidget->item(2, 0)->text()+":"+ui->tableWidget->item(2, 1)->text()+"\n"));
        //交班时间

        model.setQuery(QString("Select NOW()"));
        date1=(model.record(0).value(0).toDateTime().toString("yyyy-MM-dd hh:mm:ss"));
        ui->tableWidget->item(3,1)->setText(date1);
        printins.append(backheader::printer->drawText("1B6100 1D2100", ui->tableWidget->item(3, 0)->text()+":"+ui->tableWidget->item(3, 1)->text()+"\n"));

        //备注
        printins.append(backheader::printer->drawText("1B6100 1D2100", ui->tableWidget->item(4, 0)->text()+"\n"));

        query.exec(QString(" Select IfNull( sum( num_cost ) , 0 ) as sumcost , IfNull( sum(num_discount ) , 0 ) sumdiscount, IfNull(sum( num_present ) , 0 ) as sumpresent, IfNull( sum( num_service ) , 0 ) as sumservice, IfNull( sum( num_lowcost ) , 0 ) as sumlowcost, IfNull( sum( num_blotout ) , 0 ) as sumblotout, IfNull( sum( num_free ) , 0 ) as sumfree"
                           " from cey_u_checkout_master"
                           " where ch_state ='Y' and  ( vch_operID ='%1' ) and"
                           " ( dt_operdate >= '%2' and dt_operdate <= '%3' )")
                   .arg(n_func::gs_operid)
                   .arg(date0)
                   .arg(date1));
        query.next();
        //消费金额
        ui->tableWidget->item(5, 1)->setText(query.record().value("sumcost").toString());
        //优惠总和
        ui->tableWidget->item(6,1)->setText("xx.xx");
        //折扣金额
        ui->tableWidget->item(7, 2)->setText(query.record().value("sumdiscount").toString());
        //赠送金额
        ui->tableWidget->item(8, 2)->setText(query.record().value("sumpresent").toString());
        //抹零金额
        ui->tableWidget->item(9, 2)->setText(query.record().value("sumblotout").toString());

        ui->tableWidget->item(6,1)->setText(QString().sprintf("%.02f", ui->tableWidget->item(7,2)->text().toFloat()+
                                            ui->tableWidget->item(8,2)->text().toFloat()+
                                            ui->tableWidget->item(9,2)->text().toFloat())
                                            );


        //服务费+低消差
        ui->tableWidget->item(12,1)->setText("xx.xx");
        //服务费
        ui->tableWidget->item(13,2)->setText(query.record().value("sumservice").toString());
        //低消差
        ui->tableWidget->item(14,2)->setText(query.record().value("sumlowcost").toString());
        ui->tableWidget->item(12,1)->setText(QString().sprintf("%.02f",
                                                               ui->tableWidget->item(13,2)->text().toFloat()+ui->tableWidget->item(14,2)->text().toFloat()));

        //反结账
        query.exec(QString("SELECT sum(num_cost_un) FROM cey_u_uncheckout_log where vch_operID='%1' "
        " and dt_operdate >='%2' "
        " and dt_operdate <='%3' ;")
                   .arg(n_func::gs_operid)
                   .arg(date0)
                   .arg(date1));
        qDebug() << query.lastQuery();
        query.next();
        ui->tableWidget->item(10, 1)->setText(QString().sprintf("%.02f", query.record().value(0).toFloat()));
        //实收方式
        ui->tableWidget->item(11,1)->setText("xx.xx");
        query.exec(QString("Select (select aa.vch_paymodename from cey_bt_paymode aa where aa.ch_paymodeno=a.ch_paymodeno) as paymodename , count(a.ch_paymodeno) as count, sum(a.num_realamount) as realamount, sum(a.num_face) as face"
                           " from cey_u_checkout_detail a , cey_u_checkout_master b where a.ch_paymodeno<>'YY' and ( a.ch_payno =b.ch_payno ) and ( b.ch_state ='Y' ) and ( b.vch_operID ='%1' ) and ( b.dt_operdate >= '%2' and b.dt_operdate <= '%3' )"
                           " Group by a.ch_paymodeno having sum(a.num_realamount) > 0 or sum( a.num_payamount ) > 0 Order by a.ch_paymodeno")
                   .arg(n_func::gs_operid)
                   .arg(date0)
                   .arg(date1));
        currow=12;
        float realamountall=0.0;
        while(query.next()){
            ui->tableWidget->insertRow(currow);
            ui->tableWidget->setItem(currow,1,new QTableWidgetItem(query.record().value("paymodename").toString()));
            ui->tableWidget->setItem(currow,2,new QTableWidgetItem(query.record().value("realamount").toString()));
            realamountall+=query.record().value("realamount").toFloat();
            currow++;
        }
        ui->tableWidget->item(11,1)->setText(QString().sprintf("%.02f",realamountall));
    }
    //    backheader::asyn_dataprint(printins);
}

w_scr_dish_switch_detail::~w_scr_dish_switch_detail()
{
    delete ui;
}

void w_scr_dish_switch_detail::tosaveini()
{
    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    conf.setValue("shift_work/isprint", ui->checkBox->isChecked());
}

void w_scr_dish_switch_detail::toswitch()
{

}

void w_scr_dish_switch_detail::toexit()
{
    this->reject();
}
