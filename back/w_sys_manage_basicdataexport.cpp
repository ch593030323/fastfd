#include "w_sys_manage_basicdataexport.h"
//基础数据导出
#include "ui_w_sys_manage_basicdataimport_dialog.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
#include <QtConcurrent>
#else
#include <QtCore>
#endif
#include "public_sql.h"
#include "lds_messagebox.h"

static void SQL_TABLE_EXPORT(QObject *receiver, const QString &path, const QStringList &tablenames, bool *isend, const QString &dbname)
{
    *isend=false;
    QSqlQuery query(QSqlDatabase::database(dbname));
    QString errtext;
    QString table_column_name;
    QString table_column_value;
    QString tablename;
    qlonglong columncount;
    qlonglong rowcount;
    QFile file;

    for(int table_c=0,table_count=tablenames.count();table_c<table_count;table_c++){//0
        errtext.clear();

        tablename=tablenames[table_c];
        file.setFileName(path+"/"+tablename+".csv");
        QMetaObject::invokeMethod(receiver,
                                  "export_info_progress",
                                  Qt::QueuedConnection,
                                  Q_ARG(QString, QString("%1[%2/%3]    start export...")
                                        .arg(tablename)
                                        .arg(table_c+1)
                                        .arg(table_count)
                                        )
                                  );

        switch(0){
        case 0:
            if(!file.open(QFile::WriteOnly)){
                errtext= file.errorString();
                break;
            }
        case 1:
            if(!query.exec("select * from "+tablename)){
                errtext= query.lastError().text();
                break;
            }
            columncount=query.record().count();
            for(int c=0; c < columncount; c++){
                table_column_name = query.record().fieldName(c);
                file.write(table_column_name.toLocal8Bit());
                //是否到结尾
                if(c==columncount-1){
                    file.write("\n");
                } else {
                    file.write(",");
                }
                QMetaObject::invokeMethod(receiver,
                                          "export_info_progress",
                                          Qt::QueuedConnection,
                                          Q_ARG(QString, QString("%1[%2/%3]    table_column_name start export...")
                                                .arg(tablename)
                                                .arg(table_c+1)
                                                .arg(table_count)
                                                ),
                                          Q_ARG(int , c+1),
                                          Q_ARG(int,  columncount)
                                          );

            }
        case 2:
            if(!query.exec("select count(*) from "+tablename)) {
                errtext= query.lastError().text();
                break;
            }
            query.next();
            rowcount=query.record().value(0).toLongLong();
        case 3:
            if(!query.exec("select * from "+tablename)){
                errtext= query.lastError().text();
                break;
            }
            for(int r=0;r<rowcount;r++){
                query.next();
                for(int c=0; c < columncount; c++){
                    table_column_value = query.record().value(c).toString();

                    if(query.record().value(c).type()==QVariant::DateTime
                            && table_column_value.isEmpty()){
                        table_column_value="null";
                    } else {
                        table_column_value.replace("'", "\\'");
                        table_column_value.replace("\"", "\\\"");
                        table_column_value=QString(" '%1' ").arg(table_column_value);
                        table_column_value=table_column_value.trimmed();
                    }

                    file.write(table_column_value.toLocal8Bit());
                    //是否到结尾
                    if(c==columncount-1){
                        file.write("\n");
                    } else {
                        file.write(",");
                    }
                }
                QMetaObject::invokeMethod(receiver,
                                          "export_info_progress",
                                          Qt::QueuedConnection,
                                          Q_ARG(QString, QString("%1[%2/%3]    table_column_value start export...")
                                                .arg(tablename)
                                                .arg(table_c+1)
                                                .arg(table_count)
                                                ),
                                          Q_ARG(int , r+1),
                                          Q_ARG(int,  rowcount)
                                          );
            }
        case 4:
            break;

        case 5:
            break;
        case 6:
            break;
        case 7:
            break;
        case 8:
            break;
        case 9:
            break;
        }
        file.close();

        QMetaObject::invokeMethod(receiver,
                                  "export_info_progress",
                                  Qt::QueuedConnection,
                                  Q_ARG(QString, QString("%1[%2/%3]   %4")
                                        .arg(tablename)
                                        .arg(table_c+1)
                                        .arg(table_count)
                                        .arg(errtext.isEmpty()?QString("success"):errtext)
                                        )
                                  );

        if(!errtext.isEmpty()){
            break;
        }
    }//0
    *isend=true;
}

w_sys_manage_basicdataexport::w_sys_manage_basicdataexport(QWidget *parent) :
    QDialog(parent),
    ui(new Ui_w_sys_manage_basicdataimport_Dialog)
{
    ui->setupUi(this);

    //! toolbar
    ui->widget->setup(QStringList() << tr("确定") << tr("取消"), Qt::AlignRight);
    connect(ui->toolButton,SIGNAL(clicked()),this,SLOT(togetfilepath()));
    connect(ui->widget->index_widget(tr("确定")),SIGNAL(clicked()),this,SLOT(tosave()));
    connect(ui->widget->index_widget(tr("取消")),SIGNAL(clicked()),this,SLOT(toexit()));

    //! after
    _confname="w_sys_manage_basicdataexport/path";
    isend=new bool;
    *isend=true;
    _tablenames=QStringList()
            << "cey_bt_cp_dish"
            << "cey_bt_cp_memo"
            << "cey_bt_cp_type"
            << "cey_bt_dish"
            << "cey_bt_dish_suit"

            << "cey_bt_dish_type"
            << "cey_bt_dish_subtype"
            << "cey_bt_dish_warn"
            << "cey_bt_paymode"
            << "cey_bt_suit_change"

            << "cey_bt_table"
            << "cey_bt_table_type"
            << "cey_bt_unit"
            << "cey_sys_company"
            << "cey_bt_table_area";

    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    ui->lineEdit->setText(conf.value(_confname, public_sql::sql_table_export_dir).toString());
}

void w_sys_manage_basicdataexport::reset_export_tablenames(const QStringList &tablenames)
{
    _tablenames=tablenames;
}

void w_sys_manage_basicdataexport::reset_export_confname(const QString &confname)
{
    _confname=confname;
    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    ui->lineEdit->setText(conf.value(_confname, public_sql::sql_table_export_dir).toString());

}

void w_sys_manage_basicdataexport::togetfilepath()
{
    //获取路径
    QString dirname=QFileDialog::getExistingDirectory(this,
                                                      tr("Open File"),
                                                      ui->lineEdit->text(),
                                                      QFileDialog::ShowDirsOnly);
    if(!dirname.isEmpty()){
        ui->lineEdit->setText( dirname);
        QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
        conf.setValue(_confname, ui->lineEdit->text());
    }
}

bool w_sys_manage_basicdataexport::tosave()
{
    //开始写入
    if(0==lds_messagebox::warning(this, tr("消息提示"),tr("该操作会覆盖目录下的同名文件，是否继续"),tr("确认"),tr("取消"))){
        QFuture<void> future = QtConcurrent::run(SQL_TABLE_EXPORT,
                          this,
                          ui->lineEdit->text(),
                          _tablenames,
                          isend,
                          QSqlDatabase::defaultConnection
                          );
        future.waitForFinished();
    }
}

void w_sys_manage_basicdataexport::toexit()
{
    if(*isend){
        this->reject();
    }
}

void w_sys_manage_basicdataexport::export_info_progress(const QString &info, int step, int max)
{
    if(max!=-1){
        if(ui->progressBar->maximum() != max){
            ui->progressBar->setMaximum(max);
        }
    }
    if((step)!=-1){
        ui->progressBar->setValue(step);
    }
    ui->textBrowser->setText(info);
}

void w_sys_manage_basicdataexport::closeEvent(QCloseEvent *e)
{
    e->ignore();
}
