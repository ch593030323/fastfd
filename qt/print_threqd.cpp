#include "print_threqd.h"
#include "fast_order_2.h"
#include <QSqlQueryModel>
#include <QSqlRecord>
#include "lds_messagebox.h"
#include "public_mainprinter.h"
#include <QSettings>
#include <QApplication>
#include <QSqlQuery>
#include <QFileInfo>
#include <QDateTime>
#include "backheader.h"

print_threqd::print_threqd(QObject *parent) :
    QThread(parent)
{
}

void print_threqd::run()
{
//    QFileInfo fi("/home/gyq");
//    fi.setFile("./chudaloudan.db");
//    if(fi.exists() == false)
//    {
//        return;
//    }
//   // ;
//    QString str;
//    QSettings *kitchenprinter = new  QSettings(public_sql::settings_ini_dir_file, QSettings::IniFormat);

//    //    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE",tr("出打漏单表"));
//    //    db.setDatabaseName("./chudaloudan.db");  //chudaloudan.db的文件
//    //    if(!db.open())
//    //    {
//    //        lds_messagebox::warning(this, tr("消息提示"),tr("打开数库失败!"));
//    //
//    //        return;
//    //    }

//    QSqlQueryModel model;

//    model.clear();
//    model.setQuery("Select NOW()");
//    QString date = model.record(0).value(0).toDateTime().toString("yyyy-MM-dd");

//    model.setQuery(QString("select count(*) from chudaloudan where vch_printflag = 'N' and dt_date >= '%1' ").arg(date),QSqlDatabase::database("loss_form"));
//    if(model.record(0).value(0).toInt() == 0)
//    {
//        return;
//    }


//    model.clear();
//    model.setQuery(QString("select * from chudaloudan where vch_printflag = 'N' and dt_date >= '%1' ").arg(date),QSqlDatabase::database("loss_form"));
//    for(int i = 0; i < model.rowCount(); i++)
//    {
////        qDebug() << model.record(i).value(0).toString() << "\n";
////        qDebug() << model.record(i).value(1).toString() << "\n";
////        qDebug() << model.record(i).value(2).toString() << "\n";
////        qDebug() << model.record(i).value(3).toString() << "\n";
////        qDebug() << model.record(i).value(4).toString() << "\n";
////        qDebug() << model.record(i).value(5).toString() << "\n";
//        //打印份数
//        str.clear();
//        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/kitchen_num_spin").toString();
//        int print_count = str.toInt();
//        blockIns b;
//        QString path = model.record(i).value(1).toString() + ":9100";


//        str.clear();
//        str = kitchenprinter->value("w_bt_dish_Kitchenprinter_param/printer_fontsize_com").toString();
//        if(str == "1")//普通字体
//        {
//            b+=backheader::printer->drawText("1B40","");
//            b+=backheader::printer->drawText("1B40 1B6101 1C5700",model.record(i).value(4).toString() + "\n");//居中放大
//            b += backheader::printer->drawText("1B40 1C5700", model.record(i).value(2).toString(), Qt::AlignLeft);

//        }
//        else
//        {
//            b+=backheader::printer->drawText("1B40","");
//            b+=backheader::printer->drawText("1B40 1B6101 1D2101",model.record(i).value(4).toString() + "\n");//居中放大
//            b += backheader::printer->drawText("1B40 1D2101", model.record(i).value(2).toString(), Qt::AlignLeft);
//        }
//        b += backheader::printer->drawText("1B6405 1D5601", "");//走纸5x， 切纸
//        b=backheader::printer->setPrintCount(b, print_count);

//        if(backheader::syn_dataprint(b,path) == true)//成功,更新数据库打印菜品大类
//        {
//            QSqlQuery query;
//            QSqlDatabase::database().transaction();

//            QSqlQueryModel print_model;
//            print_model.setQuery(QString("select ch_dishno from cey_u_orderdish where ch_billno = '%1' ").arg(model.record(i).value(3).toString()));
//            QSqlQueryModel shifoudayin;
//            for(int ii = 0; ii < print_model.rowCount(); ii++)
//            {
//                shifoudayin.clear();
//                shifoudayin.setQuery(QString("select ch_printflag,ch_outflag from cey_bt_dish where ch_dishno = '%1'  ").arg(print_model.record(0).value(0).toString()));

//                if(shifoudayin.record(0).value(1).toString() == tr("Y") && model.record(i).value(4).toString() == tr("出品打印单"))
//                {
//                    query.prepare(QString("update cey_u_orderdish set ch_outflag ='Y' where ch_billno =:danhao and ch_dishno = :bianma"));
//                    query.bindValue(":danhao",model.record(i).value(3).toString());
//                    query.bindValue(":bianma",print_model.record(ii).value(0).toString());
//                    if(query.exec())
//                    {

//                    }
//                    else
//                    {
//                        QSqlDatabase::database().rollback();
//                        lds_messagebox::warning(0, tr("消息提示"),tr("更新出品打印失败!"));
                        
//                        return;
//                    }
//                }
//                else if(shifoudayin.record(0).value(0).toString() == tr("Y") && (model.record(i).value(4).toString() == tr("厨房打印单") || model.record(i).value(4).toString() == tr("菜品退菜单")))
//                {
//                    query.prepare(QString("update cey_u_orderdish set ch_printflag ='Y' where ch_billno =:danhao and ch_dishno = :bianma"));
//                    query.bindValue(":danhao",model.record(i).value(3).toString());
//                    query.bindValue(":bianma",print_model.record(ii).value(0).toString());
//                    if(query.exec())
//                    {

//                    }
//                    else
//                    {
//                        QSqlDatabase::database().rollback();
//                        lds_messagebox::warning(0, tr("消息提示"),tr("更新厨打打印失败!"));
//                        return;
//                    }
//                }
//            }
//            QSqlDatabase::database().commit();

//            QSqlQuery loudaquery(QSqlDatabase::database("loss_form"));
//            QSqlDatabase::database("loss_form").transaction();
//            loudaquery.prepare(QString("update chudaloudan set vch_printflag=:flag where vch_danaho = :dan"));
//            loudaquery.bindValue(":flag",tr("Y"));
//            loudaquery.bindValue(":dan",model.record(i).value(3).toString());
//            if(loudaquery.exec())
//            {

//            }
//            else
//            {
//                QSqlDatabase::database("loss_form").rollback();
//              //  lds_messagebox::warning(this, tr("消息提示"),tr("更新本地漏打单IP失败!"));
//              //
//                return;
//            }
//            QSqlDatabase::database("loss_form").commit();
//            re_model();
//        }
//    }

//    model.clear();
//    model.setQuery(QString("select count(*) from chudaloudan where vch_printflag = 'N' and dt_date >= '%1' ").arg(date),QSqlDatabase::database("loss_form"));
//    if(model.record(0).value(0).toInt() != 0)
//    {
////        leiji++;
////        if(leiji == 3)
////        {
////            print_pr->show();
////            QTimer::singleShot(3000,this,SLOT(show_print_close()));
////        }
//        emit print_lost_thread();
//    }
}

void print_threqd::re_model()
{
//    fast_order_2::model_dalei.setQuery(QString("SELECT  * FROM cey_bt_dish_type"));
//    for(int i = 0; i < fast_order_2::model_dalei.rowCount(); i++)
//    {
//        if(i == fast_order_2::num_da)
//        {
//            fast_order_2::dalei = fast_order_2::model_dalei.record(i).value(0).toString();
//        }
//    }

//    // model_xiaolei->clear();
//    fast_order_2::model_xiaolei.setQuery(QString("SELECT  * FROM cey_bt_dish_subtype where ch_dish_typeno = '%1' ").arg(fast_order_2::dalei));
//    for(int i = 0; i < fast_order_2::model_xiaolei.rowCount(); i++)
//    {
//        if(i == fast_order_2::num_xiao)
//        {
//            fast_order_2::xiaolei = fast_order_2::model_xiaolei.record(0).value(0).toString();
//        }
//    }


//    //  model_cook->clear();
//    if(fast_order_2::changyongcook == false)
//    {
//        fast_order_2::model_cook.setQuery(QString("SELECT  * FROM cey_bt_dish where ch_dish_typeno = '%1' and ch_sub_typeno ='%2' and (ch_stopflag = null or ch_stopflag = 'N') ").arg(fast_order_2::dalei).arg(fast_order_2::xiaolei));
//    }
//    else
//    {
//        fast_order_2::model_cook.setQuery(QString("select * from cey_bt_dish where ch_off = 'Y' and (ch_stopflag = null or ch_stopflag = 'N') "));
//    }
}

void print_threqd::show_print_close()
{

}
