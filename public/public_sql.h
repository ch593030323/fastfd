#ifndef PUBLIC_SQL_H
#define PUBLIC_SQL_H

#include <QObject>
#include <QSqlDatabase>
#include <QSettings>
#include <QDialog>
#include <QSqlQueryModel>
#include <public_logindialog.h>

class public_sql : public QObject
{
    Q_OBJECT
public:
    explicit public_sql(QObject *parent = 0);
    struct MySQL_SQLVERSION_DATA{
        MySQL_SQLVERSION_DATA(const QStringList &p0)
            :normal_sql(p0){}
        QStringList normal_sql;
    };

    static QList<MySQL_SQLVERSION_DATA> MySQL_sqlversion_data_content_list;
    static QString get_MySQL_sqlversion();
    static QString MySQL_sqlversion_update(const QString &fromversion);

    static void PUBLIC_SQL_INIT();

    static bool fastfd_init_UNZIP();
    static void MySQL_PARAM_INIT();//初始化数据库相关参数
    static void MySQL_PARAM_SAVE();//保存数据库相关参数
    static bool CONNECT_MySQL();//return 错误信息
    static QString MySQL_info;//任何关于数据库操作的信息都会保存到合格变量里

//    static QSqlDatabase sql_db_dish_image;//菜品对应的图片
//    static QSqlDatabase sql_db_loss_form;//出打漏单表

    static QString dbHostName;
    static QString dbDatabaseName;
    static QString dbUserName;
    static QString dbPassword;

    static bool islocal;
    static QString localdbHostName;
    static QString localdbDatabaseName;
    static QString localdbUserName;
    static QString localdbPassword;

    static QString getdbHostName();
    static QString getdbDatabaseName();
    static QString getdbUserName();
    static QString getdbPassword();
    static QString setdbHostName(const QString &db_host_name);
    static QString setdbDatabaseName(const QString &db_database_name);
    static QString setdbUserName(const QString &db_usr_name);
    static QString setdbPassword(const QString &db_pwd);

    static QString init_sql_file_dir;//数据库备份文件路径 /usr/local
    static QString init_sql_file;//数据库备份文件路径 /usr/local
    static QString sql_danwei;//数据库备份文件的拓展名

    static QString show_chinese_sql_file_dir;//数据库备份文件路径
    static QString show_chinese_sql_file;//数据库备份文件路径

    static QString show_english_sql_file_dir;//数据库备份文件路径
    static QString show_english_sql_file;//数据库备份文件路径

    static QString sql_backup_dir;//数据库备份默认路径
    static QString sql_table_export_dir;//数据库基础数据默认导出路径

    static QString image_dish_dir;//外部图片dish路径
    static QString image_double_screen_dir;//外部图片double_screen路径

    static QString settings_txt_soft_limit_dir_file;//配置文件的软件限时ini，filepath
    static QString settings_ini_dir_file;//配置文件的路径和问价名
    static QString label_printer_mode_dir;//标签打印机的模板路径

    static QString fastpay_printdata_dir;//前台埋单后的小票数据内容

    static bool MySQL_backup(const QString &dbpath, const QString &filename, const QString &disktype="");//数据库备份
    static bool MySQL_restore(const QString &dbpath, const QString &filename, const QString &disktype="");//数据库还原
    static bool MySQL_creatlocal_mysql_hddpos();//本地数据库创建
    static void YUN_POLLING();//云端轮询判断状态
    static QString makedir(const QString &dir);
    static QString copyfile(const QString &filefrom, const QString &fileto);
    static QString remove_copyfile(const QString &filefrom, const QString &fileto);
    static bool removefile(const QString &filename);
    static QString err_path_arg;

    static public_loginDialog *save_login;//保存前台的指针
    static QString def_logo;

    static QString get_logo_path();
};

//数据库设置
class Ui_public_sqlset_Dialog;
class public_loginDialog;
class public_sqlSet:public QDialog{
    Q_OBJECT
public:
    explicit public_sqlSet(public_loginDialog *parent = 0);

private slots:
    void took();
    void toexit();

    void switch_server_loacl(bool flag);
    void tohddpos_yaoqianshu();
private:
    Ui_public_sqlset_Dialog *ui;
    public_loginDialog *_parent;
};

#include <QThread>
#include <QMutex>
class YUN_POLLING_thread:public QThread
{
    Q_OBJECT
public:
    YUN_POLLING_thread(QObject *parent = 0) : QThread(parent),isbreak(false){}
    ~YUN_POLLING_thread();
    bool isbreak;
private:
    static QMutex mutex;
    void WEIXIN_POLLING_FROM_YUNPOS();
protected:
    virtual void run();
};
#endif // PUBLIC_SQL_H
