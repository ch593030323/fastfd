#include "w_sys_manage_businessdataclear.h"
//业务数据清除
#include "ui_w_sys_manage_businessdataclear_dialog.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
#include <QtConcurrent>
#else
#include <QtCore>
#endif
#include "public_sql.h"
#include "w_sys_manage.h"
#include "lds_messagebox.h"
#include "n_func.h"
static void SQL_TABLE_CLEAR(QObject *receiver, const QDateTime &adt_end, bool *isend, bool isclearkucun){
    *isend=false;
    QSqlQuery query;
    QString errtext;
    //! <--transaction begin-->
    QSqlDatabase::database().transaction();
    switch(0){//switch
    case 0:
        if(!query.exec(
                    QString("delete a from cey_u_checkout_detail a , cey_u_checkout_master b   where a.ch_payno =b.ch_payno and b.dt_operdate <= '%1' ")
                    .arg(adt_end.toString("yyyy-MM-dd hh:mm:ss"))
                    )){
            errtext=query.lastError().text();
            break;
        }
        QMetaObject::invokeMethod(receiver, "export_info_progress", Qt::QueuedConnection,Q_ARG(QString, ""),  Q_ARG(int,  0));
    case 1:
        if(!query.exec(
                    QString("delete a from cey_u_handover_detail a , cey_u_handover_master b   where a.int_id =b.int_id and b.dt_end <= '%1' ")
                    .arg(adt_end.toString("yyyy-MM-dd hh:mm:ss"))
                    )){
            errtext=query.lastError().text();
            break;
        }
        QMetaObject::invokeMethod(receiver, "export_info_progress", Qt::QueuedConnection,Q_ARG(QString, ""),  Q_ARG(int,  1));
    case 2:
        if(!query.exec(
                    QString("delete a from cey_u_handover_total a , cey_u_handover_master b   where a.int_id =b.int_id and b.dt_end <=  '%1' ")
                    .arg(adt_end.toString("yyyy-MM-dd hh:mm:ss"))
                    )){
            errtext=query.lastError().text();
            break;
        }
        QMetaObject::invokeMethod(receiver, "export_info_progress", Qt::QueuedConnection,Q_ARG(QString, ""),  Q_ARG(int,  2));
    case 3:
        if(!query.exec(
                    QString("delete from cey_u_handover_master where dt_end <= '%1' ")
                    .arg(adt_end.toString("yyyy-MM-dd hh:mm:ss"))
                    )){
            errtext=query.lastError().text();
            break;
        }
        QMetaObject::invokeMethod(receiver, "export_info_progress", Qt::QueuedConnection,Q_ARG(QString, ""),  Q_ARG(int,  3));
    case 4:
        if(!query.exec(
                    QString("delete a from cey_u_orderdish a , cey_u_checkout_master b  where a.ch_payno =b.ch_payno and b.dt_operdate <= '%1' ")
                    .arg(adt_end.toString("yyyy-MM-dd hh:mm:ss"))
                    )){
            errtext=query.lastError().text();
            break;
        }
        QMetaObject::invokeMethod(receiver, "export_info_progress", Qt::QueuedConnection,Q_ARG(QString, ""),  Q_ARG(int,  4));


    case 5:
        if(!query.exec(
                    QString("delete a from cey_u_service_low a , cey_u_checkout_master b  where a.ch_payno =b.ch_payno and b.dt_operdate <= '%1' ")
                    .arg(adt_end.toString("yyyy-MM-dd hh:mm:ss"))
                    )){
            errtext=query.lastError().text();
            break;
        }
        QMetaObject::invokeMethod(receiver, "export_info_progress", Qt::QueuedConnection,Q_ARG(QString, ""),  Q_ARG(int,  5));

    case 6:
        if(!query.exec(
                    QString("delete a from cey_u_table a , cey_u_checkout_master b  where a.ch_payno =b.ch_payno and b.dt_operdate <= '%1' ")
                    .arg(adt_end.toString("yyyy-MM-dd hh:mm:ss"))
                    )){
            errtext=query.lastError().text();
            break;
        }
        QMetaObject::invokeMethod(receiver, "export_info_progress", Qt::QueuedConnection,Q_ARG(QString, ""),  Q_ARG(int,  6));

    case 7:
        if(!query.exec(
                    QString("delete from cey_u_togo  where dt_operdate <= '%1' ")
                    .arg(adt_end.toString("yyyy-MM-dd hh:mm:ss"))
                    )){
            errtext=query.lastError().text();
            break;
        }
        QMetaObject::invokeMethod(receiver, "export_info_progress", Qt::QueuedConnection,Q_ARG(QString, ""),  Q_ARG(int,  7));

    case 8:
        if(!query.exec(
                    QString("delete a from cey_u_master a , cey_u_checkout_master b where a.ch_billno =b.ch_billno and b.dt_operdate <= '%1' ")
                    .arg(adt_end.toString("yyyy-MM-dd hh:mm:ss"))
                    )){
            errtext=query.lastError().text();
            break;
        }
        QMetaObject::invokeMethod(receiver, "export_info_progress", Qt::QueuedConnection,Q_ARG(QString, ""),  Q_ARG(int,  8));
    case 9:
        if(!query.exec(
                    QString("delete from cey_u_checkout_master   where dt_operdate <= '%1' ")
                    .arg(adt_end.toString("yyyy-MM-dd hh:mm:ss"))
                    )){
            errtext=query.lastError().text();
            break;
        }
        QMetaObject::invokeMethod(receiver, "export_info_progress", Qt::QueuedConnection,Q_ARG(QString, ""),  Q_ARG(int,  9));
    }//switch


    if(isclearkucun){
        switch(0){
        case 0:
            if(!query.exec(
                        QString("delete b from cey_st_check_master b , cey_st_check_detail a  where a.ch_sheetno =b.ch_sheetno and b.dt_operdate <= '%1' ")
                        .arg(adt_end.toString("yyyy-MM-dd hh:mm:ss"))
                        )){
                errtext=query.lastError().text();
                break;
            }
            QMetaObject::invokeMethod(receiver, "export_info_progress", Qt::QueuedConnection,Q_ARG(QString, ""),  Q_ARG(int,  10));
        case 1:
            if(!query.exec(
                        QString("delete from cey_st_check_master   where dt_operdate <= '%1' ")
                        .arg(adt_end.toString("yyyy-MM-dd hh:mm:ss"))
                        )){
                errtext=query.lastError().text();
                break;
            }
            QMetaObject::invokeMethod(receiver, "export_info_progress", Qt::QueuedConnection,Q_ARG(QString, ""),  Q_ARG(int,  11));
        case 2:
            if(!query.exec(
                        QString("delete from cey_st_material_flow   where dt_operdate <= '%1' ")
                        .arg(adt_end.toString("yyyy-MM-dd hh:mm:ss"))
                        )){
                errtext=query.lastError().text();
                break;
            }
            QMetaObject::invokeMethod(receiver, "export_info_progress", Qt::QueuedConnection,Q_ARG(QString, ""),  Q_ARG(int,  12));
        case 3:
            if(!query.exec(
                        QString("delete a from cey_st_sheet_detail a , cey_st_sheet_master b   where a.ch_sheetno =b.ch_sheetno and(b.dt_audit <= '%1' or b.dt_operdate <= '%1') ")
                        .arg(adt_end.toString("yyyy-MM-dd hh:mm:ss"))
                        )){
                errtext=query.lastError().text();
                break;
            }
            QMetaObject::invokeMethod(receiver, "export_info_progress", Qt::QueuedConnection,Q_ARG(QString, ""),  Q_ARG(int,  13));
        case 4:
            if(!query.exec(
                        QString("delete from cey_st_sheet_master  where(dt_audit <= '%1' or dt_operdate <= '%1') ")
                        .arg(adt_end.toString("yyyy-MM-dd hh:mm:ss"))
                        )){
                errtext=query.lastError().text();
                break;
            }
            QMetaObject::invokeMethod(receiver, "export_info_progress", Qt::QueuedConnection,Q_ARG(QString, ""),  Q_ARG(int,  14));
        }
    }
    QMetaObject::invokeMethod(receiver, "export_info_progress", Qt::QueuedConnection,Q_ARG(QString, errtext.isEmpty()?"success":errtext));

    if(errtext.isEmpty()){
        QSqlDatabase::database().commit();
    } else {
        QSqlDatabase::database().rollback();
    }
    *isend=true;
    //! <--transaction end-->
}

w_sys_manage_businessdataclear::w_sys_manage_businessdataclear(QWidget *parent) :
    QDialog(parent),
    ui(new Ui_w_sys_manage_businessdataclear_Dialog)
{
    ui->setupUi(this);

    //! toolbar
    ui->widget->setup(QStringList() << tr("执行")<< tr("取消"), Qt::AlignRight);
    connect(ui->widget->index_widget(tr("执行")),SIGNAL(clicked()),this,SLOT(tosave()));
    connect(ui->widget->index_widget(tr("取消")),SIGNAL(clicked()),this,SLOT(toexit()));

    //! after
    ui->checkBox->setChecked(true);
    ui->checkBox->setEnabled(false);
    isend=new bool;
    *isend=true;
    ui->dateTimeEdit->setDisplayFormat("yyyy-MM-dd");
    ui->dateTimeEdit->setDate(n_func::f_get_sysdatetime().date());
//    ui->dateTimeEdit->setDateTime(QDateTime::fromString("2000-01-02", "yyyy-MM-dd"));
}

void w_sys_manage_businessdataclear::closeEvent(QCloseEvent *e)
{

    if(*isend){
        QDialog::closeEvent(e);
    }
}

void w_sys_manage_businessdataclear::togetfilepath()
{

}

void w_sys_manage_businessdataclear::tosave()
{
    QDate ldt_day;
    QDateTime ldt_date;

    ldt_date=ui->dateTimeEdit->dateTime();
    ldt_day=ldt_date.date();

    QSqlQuery query;
    query.exec(QString("select count(*) from cey_u_day_sumup where dt_date = '%1' ")
               .arg(ldt_day.toString("yyyy-MM-dd"))
               );
    query.next();
    if(query.record().value(0).toLongLong() == 0){
        lds_messagebox::warning(this, tr("消息提示"),tr("不存在此营业日，请先进行日结!"));
        return;
    }
    query.exec(QString("select dt_end from cey_u_day_sumup where dt_date = '%1' ")
               .arg(ldt_day.toString("yyyy-MM-dd"))
               );
    query.next();
    ldt_date=query.record().value(0).toDateTime();

    qlonglong ll_pos = 10;
    if(ui->checkBox_2->isChecked()){
        ll_pos+=5;
    }
    ui->progressBar->setRange(0, ll_pos);
    ui->progressBar->setValue(0);

    if(0==lds_messagebox::warning(this, tr("消息提示"),tr("该操作会重置数据库一些表的内容，是否继续"), tr("确认"),tr("取消"))){
        QFuture<void> future = QtConcurrent::run(SQL_TABLE_CLEAR,
                          this,
                          ldt_date,
                          isend,
                          ui->checkBox_2->isChecked()
                          );
        future.waitForFinished();
    }


}

void w_sys_manage_businessdataclear::toexit()
{
    this->reject();
}

void w_sys_manage_businessdataclear::export_info_progress(const QString &info, int step)
{
    if((step)!=-1){
        ui->progressBar->setValue(step+1);
    }
    if(info.isEmpty()){

    }else {
        ui->textBrowser_2->setText(info);
    }
}

