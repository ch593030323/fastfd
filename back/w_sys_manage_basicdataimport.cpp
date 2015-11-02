
#include "w_sys_manage_basicdataimport.h"
//基础数据导入
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
#include "w_sys_manage.h"
#include "lds_messagebox.h"

static void SQL_TABLE_IMPORT(QObject *receiver, const QString &path, bool *isend){
    *isend=false;
    QFile file;
    QStringList filenames=w_sys_manage::get_flilename_path(path, QStringList() << "*.csv");
    QString filename;
    QSqlQuery query;
    QByteArray file_read;
    QStringList tablename_dawei;
    QString sql_language;
    QString tablename;
    QString errtext;
    //! <--transaction begin-->
    QSqlDatabase::database().transaction();
    for(int table_c=0,table_count=filenames.count();table_c<table_count;table_c++){//0
        filename=filenames[table_c];
        switch(0){//switch
        case 0:
            file.setFileName(path+"/"+filename);
            QMetaObject::invokeMethod(receiver,
                                      "export_info_progress",
                                      Qt::QueuedConnection,
                                      Q_ARG(QString, QString("%1[%2/%3]    start export...")
                                            .arg(filename)
                                            .arg(table_c+1)
                                            .arg(table_count)),
                                      Q_ARG(int , table_c+1),
                                      Q_ARG(int,  table_count)
                                      );
            if(!file.open(QFile::ReadOnly)){
                errtext= file.errorString();
                break;
            }

        case 1:
            tablename_dawei=filename.split(".");
            if(tablename_dawei.count() != 2){
                errtext=QObject::tr("文件格式错误!");
                break;
            }
            tablename=tablename_dawei[0];
            if(!query.exec("delete from "+tablename)){
                errtext=query.lastError().text();
                break;
            }
        case 2://列名
            file_read=file.readLine();//先读一行/去除表头后继续
            sql_language=QString("insert into %1(%2)")
                    .arg(tablename)
                    .arg(QString(file_read))
                    +"values(%1)"
                    ;
            //开始插入数据
            while(!file.atEnd()){
                file_read=file.readLine();
                qDebug() << sql_language.arg(QString(file_read));
                if(!query.exec(sql_language.arg(QString(file_read)))){
                    errtext=query.lastError().text();
                    break;
                }
            }
        }//switch
        file.close();
        QMetaObject::invokeMethod(receiver,
                                  "export_info_progress",
                                  Qt::QueuedConnection,
                                  Q_ARG(QString, QString("%1[%2/%3]   %4")
                                        .arg(tablename)
                                        .arg(table_c+1)
                                        .arg(table_count)
                                        .arg(errtext.isEmpty()?QString("success"):errtext)),
                                  Q_ARG(int , table_c+1),
                                  Q_ARG(int,  table_count)
                                  );

        if(!errtext.isEmpty()){
            break;
        }
    }

    if(errtext.isEmpty()){
        QSqlDatabase::database().commit();
    } else {
        QSqlDatabase::database().rollback();
    }
    *isend=true;

    //! <--transaction end-->
}


w_sys_manage_basicdataimport::w_sys_manage_basicdataimport(QWidget *parent) :
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
    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    ui->lineEdit->setText(conf.value("w_sys_manage_basicdataimport/path", public_sql::sql_table_export_dir).toString());
    isend=new bool;
    *isend=true;

    ui->label->setText(tr("如现有数据已存在数据，导入时将清除;导入前做好备份,以防数据丢失"));
    ui->label_2->setText(tr("指定导入文件夹位置"));
}

void w_sys_manage_basicdataimport::togetfilepath()
{
    //获取路径
    QString dirname=QFileDialog::getExistingDirectory(this,
                                                      tr("Open File"),
                                                      ui->lineEdit->text(),
                                                      QFileDialog::ShowDirsOnly);
    if(!dirname.isEmpty()){
        ui->lineEdit->setText( dirname);
        QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
        conf.setValue("w_sys_manage_basicdataimport/path", ui->lineEdit->text());
    }
}

void w_sys_manage_basicdataimport::tosave()
{
    if(0==lds_messagebox::warning(this, tr("消息提示"),tr("该操作会重置数据库一些表的内容，是否继续"), tr("确认"),tr("取消"))){
        //判断文件数量是否正确
        QStringList tablenames= QStringList()
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

        QStringList filenames=w_sys_manage::get_flilename_path(ui->lineEdit->text(), QStringList() << "*.csv");
        if(filenames.count() != tablenames.count()){
            if(0==lds_messagebox::warning(this, tr("消息提示"),tr("文件夹文件数目不正确，是否继续导入?"), tr("确认"),tr("取消"))){

            } else {
                return;
            }
        }

        //清除数据库对应table
        QFuture<void> future = QtConcurrent::run(SQL_TABLE_IMPORT,
                          this,
                          ui->lineEdit->text(),
                          isend
                          );
        future.waitForFinished();
        //将数据插入
    }

}

void w_sys_manage_basicdataimport::toexit()
{
    if(*isend)
        this->reject();

}

void w_sys_manage_basicdataimport::export_info_progress(const QString &info, int step, int max)
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
    qDebug() << info << step << max;
}

void w_sys_manage_basicdataimport::closeEvent(QCloseEvent *e)
{
    e->ignore();
}
