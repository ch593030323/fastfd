#include "public_sql.h"
#include <QSqlError>
#include <QApplication>
#include <QFile>
#include <QDir>
#include "n_func.h"
#include <QDebug>
#include <QSqlRecord>
#include <qdir.h>
#include <QDirIterator>
#include "lds_loadingdialog.h"
#include "backheader.h"
#include <QSqlDriver>
#include "cloudsync_service_help.h"
#include "clientdisplay.h"
#include <QSqlRecord>
#include "lds_messagebox.h"

#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
#include <QtConcurrent>
#else
#include <QtCore>
#endif

//QSqlDatabase public_sql::sql_db_dish_image=QSqlDatabase::addDatabase("QSQLITE" , "sql_db_dish_image");
//QSqlDatabase public_sql::sql_db_loss_form=QSqlDatabase::addDatabase("QSQLITE", "loss_form");//出打漏单表
//
QString public_sql::dbHostName;
QString public_sql::dbDatabaseName;
QString public_sql::dbUserName;
QString public_sql::dbPassword;

bool public_sql::islocal;
QString public_sql::localdbHostName;
QString public_sql::localdbDatabaseName;
QString public_sql::localdbUserName;
QString public_sql::localdbPassword;

QString public_sql::MySQL_info;
QString public_sql::init_sql_file_dir;
QString public_sql::init_sql_file;
QString public_sql::sql_danwei;
QString public_sql::sql_backup_dir;
QString public_sql::sql_table_export_dir;


QString public_sql::show_chinese_sql_file_dir;
QString public_sql::show_chinese_sql_file;

QString public_sql::show_english_sql_file_dir;
QString public_sql::show_english_sql_file;

QString public_sql::image_dish_dir;
QString public_sql::image_double_screen_dir;

QString public_sql::settings_ini_dir_file;
QString public_sql::settings_txt_soft_limit_dir_file;

QString public_sql::err_path_arg = "<span style=\" color:#ff0000;\">%1</span>"
                                   "<span style=\" font-weight:600; color:#000000;\">%2</span>";

QList<public_sql::MySQL_SQLVERSION_DATA >public_sql::MySQL_sqlversion_data_content_list;

QString public_sql::label_printer_mode_dir;
QString public_sql::fastpay_printdata_dir;

public_loginDialog *public_sql::save_login=0;//保存前台的指针
QString public_sql::def_logo=":/image/widgets_pic/logo1.png";

//----------------------------------------------------------------------- "0602014500d21003"
//----------------------------------------------------------------------- "060202410101a71003"
//false
//----------------------------------------------------------------------- "0602034500e71003"
//----------------------------------------------------------------------- "060204410101df1003"
static void PUBLIC_EXCUTE_MYSQL(QObject *receiver, int *flag_from, const QString &cmd)
{
    *flag_from=
            ( (system(cmd.toStdString().data())==0)? 1:0);
    //由于备份之前移除了数据库,这次必须重启才会有效
    QMetaObject::invokeMethod(receiver,
                              "accept",
                              Qt::QueuedConnection
                              );
}

public_sql::public_sql(QObject *parent) :
    QObject(parent)
{
}

QString public_sql::get_MySQL_sqlversion()
{
    int version=MySQL_sqlversion_data_content_list.count();
    //1.0->2.3 step 13
    //1.0->1.1 step 1
    return QString().sprintf("%.01f", (10+version)/10.0);
}

QString public_sql::MySQL_sqlversion_update(const QString &fromversion)
{
    QString sql_error;
    QString toversion=get_MySQL_sqlversion();
    QString _fromversion=fromversion;
    if(_fromversion.isEmpty()) _fromversion="1.0";

    if(_fromversion > toversion){
        return  tr("数据库版本号大于软件版本号，请升级软件");
    }
    if(_fromversion==toversion){
        return "";
    }
    //1.2>>1.8
    int _from=_fromversion.toFloat()*10-10;
    int _to= toversion.toFloat()*10-10;
    //! <--transaction begin-->
    QSqlDatabase::database().transaction();
    QSqlQuery query;
    do{
        foreach(QString sql, MySQL_sqlversion_data_content_list[_from].normal_sql){
            if(query.exec(sql)){

            } else {
                qDebug() << query.lastError().text();
                sql_error=query.lastError().text();
                break;
            }
        }
        if(!sql_error.isEmpty()) break;
        _from++;
        query.prepare("insert into cey_sys_sqlversion(sqlversion, sqlupdatetime)values(?, ?);");
        query.addBindValue(QString().sprintf("%.01f", (_from+10)/10.0));
        query.addBindValue(n_func::f_get_sysdatetime());
        if(query.exec()){

        } else {
            qDebug() << query.lastError().text();
            sql_error=query.lastError().text();
            break;
        }
    } while(_from<_to);
    if(sql_error.isEmpty()){
        //! <--transaction commit-->
        QSqlDatabase::database().commit();
    } else {
        //! <--transaction rollback-->
        QSqlDatabase::database().rollback();
    }
    return  "["+_fromversion+"]>>>["+toversion+"]"+(sql_error.isEmpty()? "success" : sql_error);
}

void public_sql::PUBLIC_SQL_INIT()
{
    qDebug() << "PUBLIC_SQL_INIT...";

    if(sql_danwei.isEmpty()){
        sql_danwei="sql";
    }
    if(settings_ini_dir_file.isEmpty()){
        settings_ini_dir_file = qApp->applicationDirPath()+"/fastfd_localdata/fastfd.ini";
    }

    if(init_sql_file_dir.isEmpty()){
        init_sql_file_dir=qApp->applicationDirPath()+"/fastfd_localdata";
        show_chinese_sql_file_dir = init_sql_file_dir;
        show_english_sql_file_dir = init_sql_file_dir;
    }
    if(settings_txt_soft_limit_dir_file.isEmpty()){
        settings_txt_soft_limit_dir_file = init_sql_file_dir+"/fastfd_limit.txt";
    }

    if(init_sql_file.isEmpty()
            ||show_chinese_sql_file.isEmpty()
            ||show_english_sql_file.isEmpty()
            ){

#ifdef Q_OS_WIN
        init_sql_file="win_sql_empty_data";
        show_chinese_sql_file="win_sql_chinese_data";
        show_english_sql_file="win_sql_english_data";
#else
        init_sql_file="sql_empty_data";
        show_chinese_sql_file="sql_chinese_data";
        show_english_sql_file="sql_english_data";
#endif
    }

    if(sql_backup_dir.isEmpty()){
        sql_backup_dir =  init_sql_file_dir+"/"+"sql_backup_dir";
    }
    if(sql_table_export_dir.isEmpty()){
        sql_table_export_dir =  init_sql_file_dir+"/"+"sql_table_export_dir";
    }

    if(image_dish_dir.isEmpty()){
        image_dish_dir =  init_sql_file_dir+"/"+"image_dish_dir";
    }
    if(image_double_screen_dir.isEmpty()){
        image_double_screen_dir =  init_sql_file_dir+"/"+"image_double_screen_dir";
    }
    if(label_printer_mode_dir.isEmpty()){
        label_printer_mode_dir =  init_sql_file_dir+"/"+"label_printer_mode_dir";
    }
    if(fastpay_printdata_dir.isEmpty()){
        fastpay_printdata_dir = init_sql_file_dir+"/"+"fastpay_printdata";
        qDebug()<< makedir(fastpay_printdata_dir);
    }
    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    if(conf.value("system_setting/dayinjixinghao").toString() == "0"){
        n_func::gs_print_line="--------------------------------";
    } else {
        n_func::gs_print_line="------------------------------------------------";
    }
    //vfd开机客显
    ClientDisplay display(conf.value("system_setting/kexianleixing").toString(), "");
    display.startShowinfo(conf.value("system_setting/kexian_vfd_start").toString(),
                          conf.value("system_setting/kexianduankou").toString());
    //! vfd开机客显
    //数据库升级的详细内容
    //1.1 厨打补充的说明
    MySQL_sqlversion_data_content_list.append(MySQL_SQLVERSION_DATA(
                                                  QStringList()
                                                  << "alter table cey_bt_kitchen_print change vch_printid vch_printip VARCHAR(32);"
                                                  << "alter table cey_bt_kitchen_print add column vch_printport VARCHAR(32);"
                                                  << "alter table cey_bt_kitchen_print add column vch_printother VARCHAR(32);"
                                                  ));
    //1.2称重属性,条码
    MySQL_sqlversion_data_content_list.append(MySQL_SQLVERSION_DATA(
                                                  QStringList()
                                                  << "alter table cey_bt_dish add column ch_weight VARCHAR(1);"
                                                  << "alter table cey_bt_dish add column vch_barcode  VARCHAR(64);"
                                                  ));
    //1.3操作员权限
    MySQL_sqlversion_data_content_list.append(MySQL_SQLVERSION_DATA(
                                                  QStringList()
                                                  << "insert into cey_sys_purview_list values('0910','MySQL服务器','N', 'EDIT',null, '系统管理', '09', 'N', null);"
                                                  << "insert into cey_sys_purview_list values('0911','文件管理器','N', 'EDIT',null, '系统管理', '09', 'N', null);"
                                                  << "insert into cey_sys_purview_list values('0912','无线点菜监控','N', 'EDIT',null, '系统管理', '09', 'N', null);"
                                                  << "insert into cey_sys_purview_list values('0913','云同步','N', 'EDIT',null, '系统管理', '09', 'N', null);"
                                                  ));
    //1.4流水号
    MySQL_sqlversion_data_content_list.append(MySQL_SQLVERSION_DATA(
                                                  QStringList()
                                                  <<"insert into cey_sys_parameter values('serial_no_begin', 0, '开始流水号');"
                                                  <<"insert into cey_sys_parameter values('serial_no_end', 999, '结束流水号');"
                                                  <<"insert into cey_sys_parameter values('serial_no_now', 0, '开始流水号');"
                                                  ));
    //1.5打印中倍高倍宽
    MySQL_sqlversion_data_content_list.append(MySQL_SQLVERSION_DATA(
                                                  QStringList()
                                                  <<"insert into cey_sys_parameter values('bp_dishname_left', 1, '账单品名左空');"
                                                  <<"insert into cey_sys_parameter values('bp_unit_left', 1, '账单品码左空');"
                                                  <<"insert into cey_sys_parameter values('bp_num_left', 1, '账单数量左空');"
                                                  <<"insert into cey_sys_parameter values('bp_price_left', 1, '账单价格左空');"
                                                  <<"insert into cey_sys_parameter values('bp_amount_left', 1, '账单金额左空');"
                                                  <<"insert into cey_sys_parameter values('bp_discount_left', 1, '账单折扣左空');"
                                                  <<"insert into cey_sys_parameter values('bp_content_len', '00', '账单内容是否倍高倍宽');"//00 10 11
                                                  ));
    //1.6 南京国际广场的，数据采集
    MySQL_sqlversion_data_content_list.append(MySQL_SQLVERSION_DATA(
                                                  QStringList()
                                                  <<
                                                  "CREATE TABLE IF NOT EXISTS cey_web_data_collection("
                                                  "id varchar(10) NOT NULL UNIQUE,"
                                                  "service_ip varchar(100) NOT NULL,"
                                                  "service_port varchar(100) NOT NULL,"
                                                  "service_action varchar(100) NOT NULL,"
                                                  "service_method_name varchar(100) NOT NULL,"
                                                  "service_method_uri varchar(100) NOT NULL,"
                                                  "service_submit_path varchar(100) NOT NULL,"

                                                  "username varchar(20) NOT NULL,"
                                                  "password varchar(20) ,"
                                                  "messagetype varchar(20) NOT NULL,"
                                                  "messageid varchar(20) NOT NULL,"
                                                  "version varchar(20) NOT NULL,"

                                                  "txdate_yyyymmdd date NOT NULL,"
                                                  "txtime_hhmmss time NOT NULL,"
                                                  "mallid varchar(20) NOT NULL,"
                                                  "storecode varchar(20) NOT NULL,"
                                                  "tillid varchar(20)  NULL,"
                                                  "txdocno varchar(20) NOT NULL,"
                                                  "cashier varchar(20)  NULL,"
                                                  "salesman varchar(20)  NULL,"

                                                  "itemcode varchar(20) NOT NULL,"
                                                  "qty DECIMAL(10,2) NOT NULL,"
                                                  "netamount DECIMAL(10,2) NOT NULL,"

                                                  "ch_uploadflag varchar(1) NULL,"
                                                  "ch_uploadmemo varchar(100) NULL"

                                                  ") ENGINE=InnoDB DEFAULT CHARSET=utf8;"
                                                  <<
                                                  "CREATE TABLE IF NOT EXISTS cey_web_data_collection_tendercode("
                                                  "id varchar(10) NOT NULL,"
                                                  "tendercode varchar(10) NOT NULL,"
                                                  "baseamount DECIMAL(10,2) NULL"
                                                  ") ENGINE=InnoDB DEFAULT CHARSET=utf8;"
                                                  ));
    //1.7 菜品大类权限
    MySQL_sqlversion_data_content_list.append(MySQL_SQLVERSION_DATA(
                                                  QStringList()
                                                  <<
                                                  "CREATE TABLE IF NOT EXISTS cey_bt_dish_type_oper("
                                                  "vch_operid varchar(10) NOT NULL,"
                                                  "ch_dish_typeno varchar(2) NOT NULL,"
                                                  "ch_state char(1) NULL"
                                                  ") ENGINE=InnoDB DEFAULT CHARSET=utf8;"
                                                  ));
    //1.8 库存入库失败
    MySQL_sqlversion_data_content_list.append(MySQL_SQLVERSION_DATA(
                                                  QStringList()
                                                  <<
                                                  "    DROP TRIGGER IF EXISTS tr_st_sheet_master_update;"
                                                  "    CREATE TRIGGER tr_st_sheet_master_update AFTER UPDATE ON cey_st_sheet_master FOR EACH ROW "
                                                  "    BEGIN\n"
                                                  "        DECLARE m_de int default 0;\n"
                                                  "        DECLARE m_done int default 0;\n"
                                                  "        DECLARE sheetNo  varchar(12);\n"
                                                  "        DECLARE sheetType varchar(2);\n"
                                                  "        DECLARE operation varchar(3);\n"
                                                  "        DECLARE dtaudit datetime;\n"
                                                  "        DECLARE materialno varchar(7);\n"
                                                  "        DECLARE numnum numeric(16,4);\n"
                                                  "        DECLARE numpresent numeric(15,4);\n"
                                                  "        DECLARE numprice numeric(16,4);\n"
                                                  "        DECLARE costprice  numeric(16,4);\n"
                                                  "        DECLARE unitrate numeric(16,2);\n"
                                                  "        DECLARE intflowId integer;\n"
                                                  "        DECLARE initqty   numeric(16,4);\n"
                                                  "        DECLARE  initprice numeric(16,4);\n"
                                                  "        DECLARE remainqty numeric(16,4);\n "
                                                  "        DECLARE remainprice numeric(16,4);\n"
                                                  "        DECLARE num_amount numeric(16,2);\n"
                                                  "        DECLARE cur_rows cursor for\n"
                                                  "          select b.ch_materialno, b.num_wrapping, b.num_present,  b.num_price, ifnull(c.int_unit_rate,1) "
                                                  "           from  cey_st_sheet_detail b, cey_bt_dish c"
                                                  "             where NEW.ch_sheetNo = b.ch_sheetno and b.ch_materialno = c.ch_dishno;\n"

                                                  "       DECLARE continue handler for not found set m_done = 1;\n"

                                                  "       open cur_rows;\n"

                                                  "       fetch cur_rows into  materialno, numnum, numpresent, numprice, unitrate;\n"

                                                  "     WHILE (m_done = 0) DO \n"

                                                  "          IF(NEW.ch_state = 'Y')THEN\n"

                                                  "            set sheetNo = NEW.ch_sheetNo ;\n"
                                                  "            set sheetType = NEW.ch_sheetType ;\n"
                                                  "            set operation = NEW.ch_operation ;\n"
                                                  "            set dtaudit = NEW.dt_audit ;\n"

                                                  "            set intflowId = (select ifnull(max(int_flowId),0) from cey_st_material_flow  where ch_materialno = materialno);\n"

                                                  "            IF (intflowId = 0) THEN \n"
                                                  "              set initqty =  0 ;\n"
                                                  "            ELSE\n"
                                                  "              set initqty = (select remain_qty from cey_st_material_flow where int_flowId = intflowId);\n"
                                                  "            END IF;\n"

                                                  "            IF (intflowId = 0) THEN\n"
                                                  "              set initprice =  0 ;\n"
                                                  "            ELSE\n"
                                                  "              set initprice = (select remain_price from cey_st_material_flow where int_flowId = intflowId);\n"
                                                  "            END IF;\n"

                                                  "            IF (sheetType = 'RK')  THEN\n"

                                                  "                 set remainqty = initqty + numnum;\n"

                                                  "                 IF (remainqty = 0) THEN\n"
                                                  "                    set remainprice = 0;\n"
                                                  "                 ELSE\n"
                                                  "                    set remainprice = (abs(((numnum / unitrate) * numprice  + initqty * initprice) / remainqty));\n"
                                                  "                 END IF;\n"

                                                  "                 insert cey_st_material_flow (ch_materialno, dt_operdate, sheettype, int_flag,\n"
                                                  "                           num_num, sheet_price, cost_price, chg_amount, remain_qty, remain_price, ch_sheetno)\n"
                                                  "                   values (materialno, now(), sheetType, 1, numnum, numprice / unitrate, remainqty, 0, remainqty, remainprice, sheetNo);\n"

                                                  "                 IF (numpresent > 0) THEN\n"

                                                  "                      set initqty = remainqty;\n"
                                                  "                      set initprice = remainprice;\n"
                                                  "                      set remainqty = initqty + numpresent;\n"

                                                  "                      IF (remainqty = 0) THEN\n"
                                                  "                         set remainprice = 0;\n"
                                                  "                      ELSEIF (initqty < 0 and remainqty > 0) THEN \n"
                                                  "                             set remainprice = numprice / unitrate;\n"
                                                  "                      ELSE \n"
                                                  "                            set remainprice = abs((initqty * initprice) / remainqty);\n"
                                                  "                      END IF;\n"

                                                  "                      insert cey_st_material_flow (ch_materialno, dt_operdate, sheettype, int_flag,\n"
                                                  "                             num_num, sheet_price, cost_price, chg_amount, remain_qty, remain_price, ch_sheetno)\n"
                                                  "                      values (materialno, now(), sheetType, 1, numpresent, 0, remainprice, 0, remainqty, remainprice, sheetNo);\n"
                                                  "                 END IF;\n"


                                                  "                     IF exists ( Select * from cey_sys_parameter where vch_parameter = 'sys_stock_price' and vch_value = '1') THEN\n"
                                                  "                    update cey_bt_dish set num_sale_price = numprice where ch_dishno = materialno;\n"
                                                  "                 END IF;\n"

                                                  "                 IF exists (select 1 from cey_st_material_num where ch_materialno = materialno) THEN\n"
                                                  "                  update cey_st_material_num set num_num = remainqty, cost_price = remainprice\n"
                                                  "                   where ch_materialno = materialno;\n"
                                                  "                 ELSE \n"
                                                  "                   insert into cey_st_material_num (ch_materialno, num_num, cost_price)\n"
                                                  "                     values (materialno, remainqty, remainprice);\n"
                                                  "                 END IF;\n"

                                                  "                 fetch cur_rows into  materialno, numnum, numpresent, numprice, unitrate;\n"

                                                  "            ELSEIF (sheetType = 'TH')  THEN\n"
                                                  "                 set remainqty = initqty - numnum;\n"
                                                  "                 IF (remainqty = 0) THEN\n"
                                                  "                    set remainprice = 0;\n"
                                                  "                 ELSE\n"
                                                  "                    set remainprice = (abs((initqty * initprice - numnum / unitrate * numprice   ) / remainqty));\n"
                                                  "                 END IF;\n"

                                                  "                 insert cey_st_material_flow (ch_materialno, dt_operdate, sheettype, int_flag,\n"
                                                  "                           num_num, sheet_price, cost_price, chg_amount, remain_qty, remain_price, ch_sheetno)\n"
                                                  "                   values (materialno, now(), sheetType, 1, numnum, numprice / unitrate, remainprice, 0, remainqty, remainprice, sheetNo);\n"


                                                  "                 IF (numpresent > 0) THEN \n"
                                                  "                      set initqty   = remainqty;\n"
                                                  "                      set initprice = remainprice;\n"
                                                  "                      set remainqty = initqty - numpresent;\n"

                                                  "                      IF (remainqty = 0) THEN\n"
                                                  "                         set remainprice = 0;\n"
                                                  "                      ELSE\n"
                                                  "                           set remainprice = abs((initqty * initprice) / remainqty);\n"
                                                  "                      END IF;\n"

                                                  "                      insert cey_st_material_flow (ch_materialno, dt_operdate, sheettype, int_flag,\n"
                                                  "                             num_num, sheet_price, cost_price, chg_amount, remain_qty, remain_price, ch_sheetno)\n"
                                                  "                      values (materialno, now(), sheetType, 1, numpresent, 0, remainprice, 0, remainqty, remainprice, sheetNo);\n"
                                                  "                  END IF;\n"

                                                  "                 IF exists (select 1 from cey_st_material_num where ch_materialno = materialno) THEN\n"
                                                  "                  update cey_st_material_num set num_num = remainqty, cost_price = remainprice\n"
                                                  "                   where ch_materialno = materialno;\n"
                                                  "                 ELSE \n"
                                                  "                   insert into cey_st_material_num (ch_materialno, num_num, cost_price)\n"
                                                  "                     values (materialno, remainqty, remainprice);\n"
                                                  "                 END IF;\n"

                                                  "                 fetch cur_rows into  materialno, numnum, numpresent, numprice, unitrate;\n"

                                                  "            ELSEIF (sheetType = 'CK') THEN\n"
                                                  "                 set remainqty = initqty - numnum;\n"
                                                  "                 IF (remainqty = 0) THEN\n"
                                                  "                    set remainprice = 0;\n"
                                                  "                 ELSEIF (initqty < 0 and remainqty > 0) THEN\n"
                                                  "                    set m_de = 1;\n"
                                                  "                 ELSE \n"
                                                  "                    set remainprice = abs((initqty * initprice - numnum / unitrate * numprice   ) / remainqty);\n"
                                                  "                  END IF;\n"

                                                  "                 insert cey_st_material_flow (ch_materialno, dt_operdate, sheettype, int_flag,\n"
                                                  "                           num_num, sheet_price, cost_price, chg_amount, remain_qty, remain_price, ch_sheetno)\n"
                                                  "                   values (materialno, now(), sheetType, 1, numnum, numprice/unitrate, remainprice, 0, remainqty, remainprice, sheetNo);\n"

                                                  "                 IF exists (select 1 from cey_st_material_num where ch_materialno = materialno) THEN\n"
                                                  "                  update cey_st_material_num set num_num = remainqty, cost_price = remainprice\n"
                                                  "                   where ch_materialno = materialno;\n"
                                                  "                 ELSE \n"
                                                  "                   insert into cey_st_material_num (ch_materialno, num_num, cost_price)\n"
                                                  "                     values (materialno, remainqty, remainprice);\n"
                                                  "                 END IF;\n"

                                                  "                 fetch cur_rows into  materialno, numnum, numpresent, numprice, unitrate;\n"

                                                  "            ELSE\n"
                                                  "                fetch cur_rows into  materialno, numnum, numpresent, numprice, unitrate;\n"
                                                  "            END IF;\n"

                                                  "          END IF;\n"

                                                  "     END WHILE;\n"

                                                  "      close cur_rows;\n"

                                                  "    END;\n"
                                                  ));
    //1.8 库存入库失败
    MySQL_sqlversion_data_content_list.append(MySQL_SQLVERSION_DATA(
                                                  QStringList()
                                                  << QString("update cey_bt_paymode set vch_paymodename = '%1' where ch_paymodeno='ZZ' ")
                                                  .arg(tr("手工优惠"))
                                                  ));

    //    //1.4
    //    MySQL_sqlversion_data_content_list.append(MySQL_SQLVERSION_DATA(
    //                                                  QStringList()
    //                                                  << "alter table cey_bt_dish_suit change column num_num num_num DECIMAL(10,3);"
    //                                                  << "alter table cey_bt_dish_warn change column num_num num_num DECIMAL(10,3);"
    //                                                  << "alter table cey_st_material_flow change column num_num num_num DECIMAL(10,3);"
    //                                                  << "alter table cey_st_material_num change column num_num num_num DECIMAL(10,3);"
    //                                                  << "alter table cey_st_sheet_detail change column num_num num_num DECIMAL(10,3);"
    //                                                  << "alter table cey_u_hungbill change column num_num num_num DECIMAL(10,3);"
    //                                                  << "alter table cey_u_orderdish change column num_num num_num DECIMAL(10,3);"
    //                                                  << "alter table cybr_book_detail change column num_num num_num DECIMAL(10,3);"
    //                                                  ));
    //! 数据库升级的详细内容

    //创建菜品对应图片数据库
    //    sql_db_dish_image.setDatabaseName(image_dish_dir+"/sql_db_dish_image.db");
    //    QSqlDatabase _sql_db_dish_image = QSqlDatabase::database("sql_db_dish_image");
    //    if(_sql_db_dish_image.isOpen()){
    //        QSqlQuery query(_sql_db_dish_image);
    //        query.exec("select count(*) from sql_db_dish_image");
    //        if(query.next()){
    //            qDebug() << "sql_db_dish_image create has existed!" << query.record().value(0).toString();
    //            query.exec("select *from sql_db_dish_image");
    //            while(query.next()){
    //                qDebug() << query.record().value(0).toString();
    //            }
    //        } else {
    //            if(query.exec(
    //                        "CREATE TABLE sql_db_dish_image("
    //                        "vch_dishno char (32)  primary key ,"
    //                        "vch_dishno_image_content char (100)"
    //                        ");"
    //                        )){
    //                qDebug() << "sql_db_dish_image create success";
    //                QDirIterator it(":/image/dish", QStringList() << "*.png"<< "*.jpg"<< "*.bmp");
    //                QFile file;
    //                QByteArray byteArray;
    //                while(it.hasNext()){
    //                    file.setFileName(it.filePath());
    //                    if (file.open(QIODevice::ReadOnly)){
    //                        byteArray = file.readAll();
    //                        query.prepare("INSERT INTO sql_db_dish_image (vch_dishno, vch_dishno_image_content) VALUES (?,?)");
    //                        query.addBindValue(it.fileName());
    //                        query.addBindValue(byteArray);
    //                        query.exec();
    //                        qDebug() << it.fileName() << byteArray;
    //                    }
    //                    file.close();
    //                    it.next();
    //                }
    //            } else {
    //                qDebug() << "sql_db_dish_image create failure" << query.lastError().text();
    //            }
    //        }
    //    } else {
    //        qDebug() << "sql_db_dish_image" << _sql_db_dish_image.lastError().text();
    //    }

    //本地厨打单
    QSqlDatabase lostform_db=QSqlDatabase::database("loss_form",false);
    lostform_db.setDatabaseName(image_dish_dir+"/sql_db_loss_form.db");
    if(lostform_db.open()){
        QSqlQuery query(lostform_db);
        query.exec("create table if not exists loss_form_table ("
                   "int_id int primary key,"
                   "vch_billno char(20),"
                   "vch_ip char(20), "
                   "vch_content char(2048) ,"
                   "dt_date datetime NULL,"
                   "vch_type char(20)"
                   ")"
                   );   //创建一个表
    }
    //! 本地厨打单
    CommonHeader::com_ini_filepath=public_sql::settings_ini_dir_file;
    CommonHeader::com_root_dirpath=public_sql::init_sql_file_dir;
    //    CommonHeader::com_widget_dir=Qt::RightToLeft;

}

bool public_sql::fastfd_init_UNZIP()
{
    //解压对应的菜品包
    bool retflag=false;
    if(QFile(qApp->applicationDirPath()+"/fastfd_localdata.zip").exists()){
        while(1){//while
            QProcess myProcess;
            myProcess.start(QString("rm -rf %1/fastfd_localdata").arg(qApp->applicationDirPath()));
            if(myProcess.waitForFinished()){
                myProcess.start(QString("unzip %1/fastfd_localdata.zip -d %1").arg(qApp->applicationDirPath()));
                if(myProcess.waitForFinished()){
                    public_sql::MySQL_info=public_sql::err_path_arg.arg(tr("成功")).arg("unzip  fastfd_localdata");
                    retflag=true;
                    break;
                }
            }
            public_sql::MySQL_info=public_sql::err_path_arg.arg(tr("失败")).arg(myProcess.errorString());
            break;
        }//! while
    } else {
        public_sql::MySQL_info=public_sql::err_path_arg.arg(tr("失败")).arg("not find  fastfd_localdata.zip");
    }
    //! 解压对应的菜品包
    return retflag;
}

void public_sql::MySQL_PARAM_INIT()
{
    qDebug() << "MySQL_PARAM_INIT...";
    QSettings * backsetting=new QSettings(public_sql::settings_ini_dir_file, QSettings::IniFormat);

    islocal=backsetting->value("public_sql/islocal", false).toBool();
    localdbHostName=backsetting->value("public_sql/localdbHostName", "127.0.0.1").toString();
    localdbDatabaseName=backsetting->value("public_sql/localdbDatabaseName","hddpos").toString();
    localdbUserName=backsetting->value("public_sql/localdbUserName","root").toString();
#ifdef QT_DEBUG
    localdbPassword=backsetting->value("public_sql/localdbPassword","111111").toString();
#else
    localdbPassword=backsetting->value("public_sql/localdbPassword","").toString();
#endif
    dbHostName=backsetting->value("public_sql/dbHostName").toString();
    dbDatabaseName=backsetting->value("public_sql/dbDatabaseName").toString();
    dbUserName=backsetting->value("public_sql/dbUserName").toString();
    dbPassword=backsetting->value("public_sql/dbPassword").toString();

    delete backsetting;
}

void public_sql::MySQL_PARAM_SAVE()
{

    qDebug() << "QSettings * backsetting=new QSettings(public_sql::settings_ini_dir_file, QSettings::IniFormat);" <<  public_sql::settings_ini_dir_file;
    QSettings * backsetting=new QSettings(public_sql::settings_ini_dir_file, QSettings::IniFormat);

    backsetting->setValue("public_sql/dbHostName", dbHostName);
    backsetting->setValue("public_sql/dbDatabaseName", dbDatabaseName);
    backsetting->setValue("public_sql/dbUserName", dbUserName);
    backsetting->setValue("public_sql/dbPassword", dbPassword);

    backsetting->setValue("public_sql/islocal", islocal);
    backsetting->setValue("public_sql/localdbHostName", localdbHostName);
    backsetting->setValue("public_sql/localdbDatabaseName", localdbDatabaseName);
    backsetting->setValue("public_sql/localdbUserName", localdbUserName);
    backsetting->setValue("public_sql/localdbPassword", localdbPassword);

    backsetting->sync();
    delete backsetting;

}

//
bool public_sql::CONNECT_MySQL()
{
    bool flag=false;
    //1
    {
        QSqlDatabase::removeDatabase("db_kitchen");
        QSqlDatabase db_kitchen=QSqlDatabase::addDatabase("QSQLITE", "db_kitchen");//厨打
        db_kitchen.setDatabaseName(public_sql::init_sql_file_dir+"/db_kitchen.db");
        if(db_kitchen.open()){
            QSqlQuery query(db_kitchen);
            query.exec("create table if not exists loss_form_table ("
                       "int_id int primary key,"
                       "vch_billno char(20),"
                       "vch_ip char(20), "
                       "vch_content char(2048) ,"
                       "dt_date datetime NULL,"
                       "vch_type char(20)"
                       ")"
                       );   //创建一个表
        }
    }
    //2
    {
        QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
        QSqlDatabase sql_db=QSqlDatabase::addDatabase("QMYSQL");//本地数据库
        sql_db.setHostName(public_sql::getdbHostName());
        sql_db.setDatabaseName(public_sql::getdbDatabaseName());
        sql_db.setUserName(public_sql::getdbUserName());
        sql_db.setPassword(public_sql::getdbPassword());
        sql_db.setConnectOptions("CLIENT_SSL=1;CLIENT_IGNORE_SPACE=1;");
        if(sql_db.hostName().isEmpty()
                || sql_db.databaseName().isEmpty()
                || sql_db.userName().isEmpty()
                ){
            flag=false;
            public_sql::MySQL_info=public_sql::err_path_arg.arg(tr("数据库连接失败!")).arg("有参数为空");
        } else {
            flag=sql_db.open();
            public_sql::MySQL_info=public_sql::err_path_arg.arg(flag?tr("数据库连接成功"):tr("数据库连接失败")).arg(public_sql::islocal?tr("【本地数据】"):tr("【服务器】"));//sql_db.lastError().text()
            if(!flag) qDebug() << sql_db.lastError();
        }
    }

    QSqlQuery query;
    if(!query.exec("select now();")){
        public_sql::MySQL_info=public_sql::err_path_arg.arg(tr("数据库连接失败!")).arg(tr("连接错误!"));
        qDebug() << query.lastError().text();
        return false;
    }
    return true;
}

QString public_sql::getdbHostName()
{
    return islocal?localdbHostName:dbHostName;
}

QString public_sql::getdbDatabaseName()
{
    return islocal?localdbDatabaseName:dbDatabaseName;
}

QString public_sql::getdbUserName()
{
    return islocal?localdbUserName:dbUserName;
}

QString public_sql::getdbPassword()
{
    return islocal?localdbPassword:dbPassword;
}

QString public_sql::setdbHostName(const QString &db_host_name)
{
    if(islocal){
        localdbHostName = db_host_name;
    } else {
        dbHostName = db_host_name;
    }
}

QString public_sql::setdbDatabaseName(const QString &db_database_name)
{
    if(islocal){
        localdbDatabaseName = db_database_name;
    } else {
        dbDatabaseName = db_database_name;
    }
}

QString public_sql::setdbUserName(const QString &db_usr_name)
{
    if(islocal){
        localdbUserName = db_usr_name;
    } else {
        dbUserName = db_usr_name;
    }
}

QString public_sql::setdbPassword(const QString &db_pwd)
{
    if(islocal){
        localdbPassword = db_pwd;
    } else {
        dbPassword = db_pwd;
    }
}

bool public_sql::MySQL_backup(const QString &dbpath, const QString &filename, const QString &disktype)
{
    //! ->备份之前需要移除 defaultConnection;
    //1
    QSqlDatabase::cloneDatabase(QSqlDatabase::database(QSqlDatabase::defaultConnection, false), "tmpdb");
    QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);

    //2
    MySQL_info.clear();
    qDebug() << "---------------------" << dbpath << filename << disktype;
    QDir dir(dbpath);
    if(dir.exists()){
    } else {
        MySQL_info=err_path_arg.arg(tr("路径不存在!")).arg(dbpath);
        return false;
    }
    QFile file(dbpath+"/"+filename);
    //3
    //获取备份 指令
    QString path = file.fileName();
    QString cmd;
    QString ip = getdbHostName();
    QString username =getdbUserName();
    QString password = getdbPassword();
    QString databaseName = getdbDatabaseName();
#ifdef Q_OS_UNIX
    cmd = "mysqldump";

    if(ip != "127.0.0.1")
        cmd += " -h" + ip;
    cmd += " -u"+username;
    if(!password.isEmpty())
        cmd += " -p" + password;
    cmd += " " + databaseName;
    cmd += " > " + path;
#else
    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    cmd = QString("\"%1\" ")
            .arg(conf.value("w_sys_manage_sqlcopy/winmysql_dir").toString()+"/mysqldump");
    if(ip != "127.0.0.1")
        cmd += " -h" + ip;
    cmd += " -u"+username;
    if(!password.isEmpty())
        cmd += " -p" + password;
    cmd += " " + databaseName;
    cmd += " > " + path;
#endif
    qDebug() << cmd;

    //开始备份，提交备份信息
    int flag=-1;
    lds_loadingDialog loading_dialog;
    loading_dialog.ui->label_text->setText(disktype+tr("正在备份数据库..."));
    QFuture<void> future = QtConcurrent::run(PUBLIC_EXCUTE_MYSQL,
                      &loading_dialog,
                      &flag,
                      cmd);
    future.waitForFinished();
    if(flag==-1){
        backheader::static_dialogexec(loading_dialog, tr("LOADING..."));
    }
    //4
    QSqlDatabase::cloneDatabase(QSqlDatabase::database("tmpdb"), QSqlDatabase::defaultConnection);
    QSqlDatabase::removeDatabase("tmpdb");
    //5
    if(flag==1){
        MySQL_info=err_path_arg.arg(tr("备份成功!")).arg(dbpath+"/"+filename);
        return true;
    } else {
        MySQL_info=err_path_arg.arg(tr("备份失败!")).arg(dbpath+"/"+filename);
        return false;
    }
}

bool public_sql::MySQL_restore(const QString &dbpath, const QString &filename, const QString &disktype)
{
    //! ->备份之前需要移除 defaultConnection;
    //1
    QSqlDatabase::cloneDatabase(QSqlDatabase::database(QSqlDatabase::defaultConnection, false), "tmpdb");
    QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
    MySQL_info.clear();
    //2
    QDir dir(dbpath);
    if(dir.exists()){

    } else {
        MySQL_info=err_path_arg.arg(tr("路径不存在!")).arg(dbpath);
        return false;
    }

    QFile file(dbpath+"/"+filename);
    if(!filename.isEmpty()
            &&file.exists()){

    } else {//
        MySQL_info=err_path_arg.arg(tr("文件不存在!")).arg(dbpath+"/"+filename);
        return false;
    }//

    //3
    QString path = file.fileName();

    QString cmd;
    QString ip = getdbHostName();
    QString username =getdbUserName();
    QString password = getdbPassword();
    QString databaseName = getdbDatabaseName();
#ifdef Q_OS_UNIX
    cmd = "mysql";

    if(ip != "127.0.0.1")
        cmd += " -h" + ip;
    cmd += " -u"+username;
    if(!password.isEmpty())
        cmd += " -p" + password;
    cmd += " " + databaseName;
    cmd += " < " + path;
#else
    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    cmd = QString("\"%1\" ")
            .arg(conf.value("w_sys_manage_sqlcopy/winmysql_dir").toString()+"/mysql");
    if(ip != "127.0.0.1")
        cmd += " -h" + ip;
    cmd += " -u"+username;
    if(!password.isEmpty())
        cmd += " -p" + password;
    cmd += " " + databaseName;
    cmd += " < " + path;
#endif
    qDebug()  <<   cmd;
    //开始还原，提交备份信息
    int flag=-1;
    lds_loadingDialog loading_dialog;
    loading_dialog.ui->label_text->setText(disktype+tr("正在还原数据库(约30s),..."));
    QFuture<void> future = QtConcurrent::run(PUBLIC_EXCUTE_MYSQL,
                      &loading_dialog,
                      &flag,
                      cmd);
    future.waitForFinished();
    if(flag==-1){
        backheader::static_dialogexec(loading_dialog, tr("LOADING..."));
    }
    //4
    QSqlDatabase::cloneDatabase(QSqlDatabase::database("tmpdb"), QSqlDatabase::defaultConnection);
    QSqlDatabase::removeDatabase("tmpdb");
    if(flag==1){
        MySQL_info=err_path_arg.arg(tr("还原成功!")).arg(dbpath+"/"+filename);
        return true;
    } else {
        MySQL_info=err_path_arg.arg(tr("还原失败!")).arg(dbpath+"/"+filename);
        return false;
    }
}

bool public_sql::MySQL_creatlocal_mysql_hddpos()
{
    MySQL_info.clear();
    QSqlDatabase::addDatabase("QMYSQL", "tmpmysql");
    bool dberrflag=false;
    //! <loop_1_begin>
    while(1){
        QSqlDatabase db = QSqlDatabase::database("tmpmysql");
        db.setHostName(localdbHostName);
#ifdef Q_OS_UNIX
        db.setDatabaseName("mysql");
#else
        db.setDatabaseName("test");
#endif
        db.setUserName(localdbUserName);
        db.setPassword(localdbPassword);

        dberrflag=db.open();
        if(dberrflag){//1
            qDebug() << "mysql mysql connect success";
            QSqlQuery query(db);
            dberrflag=query.exec("SHOW DATABASES;");
            if(dberrflag){//3
                //! <loop_2_begin>
                while((dberrflag=query.next())){//4
                    if(query.record().value(0).toString().contains("hddpos")){//5
                        qDebug() << "hddpos database has existed";
                        break;
                    }//5
                }//4
                //! <loop_2_end>
                if(dberrflag)break;
            }//3
            //创建数据库
            qDebug() << "hddpos database not existed";
            dberrflag=query.exec("create database if not exists hddpos;");
            if(dberrflag){//2
                qDebug() << "create mysql hddpos success";
                //修改权限
                query.exec("grant all privileges on *.* to root@\"%\" identified by \"\"");
                query.exec("flush privileges");
                qDebug() << "grant all privileges" << tr("修改权限");

                //初始化配置文件
                fastfd_init_UNZIP();
                //数据库中文备份
                public_sql::MySQL_restore(public_sql::show_chinese_sql_file_dir, public_sql::show_chinese_sql_file, tr("首次使用:"));

            } else {//2
                qDebug() << "create mysql hddpos failure";
            }//2
        } else {//1
            qDebug() << "mysql mysql connect failure" << db.lastError();
        }//1
        break;
    }
    //! <loop_1_end>

    QSqlDatabase::removeDatabase("tmpmysql");
    return dberrflag;
}

void public_sql::YUN_POLLING()
{
    //    qDebug() << "微信轮询读取云端数据";
    YUN_POLLING_thread *t=new YUN_POLLING_thread;
    QObject().moveToThread(t);
    t->start();
}

QString public_sql::makedir(const QString &dir)
{
    if(QDir().exists(dir))return "dir is existed";

    if(QDir().mkdir(dir)) return "dir create success";

    return "dir create failure";
}

QString public_sql::copyfile(const QString &filefrom, const QString &fileto)
{
    if(QFile(fileto).exists()) return "file is existed";
    //    ReadOwner = 0x4000, WriteOwner = 0x2000, ExeOwner = 0x1000,
    //    ReadUser  = 0x0400, WriteUser  = 0x0200, ExeUser  = 0x0100,
    //    ReadGroup = 0x0040, WriteGroup = 0x0020, ExeGroup = 0x0010,
    //    ReadOther = 0x0004, WriteOther = 0x0002, ExeOther = 0x0001
    if(QFile::copy(filefrom,  fileto)){
        QFile::setPermissions(fileto, QFile::Permission(0x777));
        return "file copy success";
    }

    return "file create failure";
}

QString public_sql::remove_copyfile(const QString &filefrom, const QString &fileto)
{
    if(QFile::copy(filefrom,  fileto)){
        QFile::setPermissions(fileto, QFile::Permission(0x777));
        return "file copy success";
    }
    return "file create failure";
}

bool public_sql::removefile(const QString &filename)
{
    return QFile(filename).remove();
}

QString public_sql::get_logo_path()
{
    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    return conf.value("public_sql/login_logo", ":/image/widgets_pic/logo1.png").toString();
}


//=========================
#include "ui_public_sqlset_dialog.h"
#include <QFontDialog>
#include "public_logindialog.h"

public_sqlSet::public_sqlSet(public_loginDialog *parent):
    QDialog(parent),
    ui(new Ui_public_sqlset_Dialog),
    _parent(parent)
{
    ui->setupUi(this);
    switch_server_loacl(public_sql::islocal);
    ui->checkBox->setChecked(QSettings(public_sql::settings_ini_dir_file, QSettings::IniFormat).value("public_sql/islocal").toBool());

    //! toolbar
    ui->widget->setup(QStringList() << tr("hddpos->yaoqianshu转换")<< tr("确定")<< tr("退出"), Qt::AlignRight);
    connect(ui->widget->index_widget(tr("hddpos->yaoqianshu转换")), SIGNAL(clicked()),this,SLOT(tohddpos_yaoqianshu()));
    connect(ui->widget->index_widget(tr("确定")), SIGNAL(clicked()),this,SLOT(took()));
    connect(ui->widget->index_widget(tr("退出")), SIGNAL(clicked()),this,SLOT(toexit()));
    connect(ui->checkBox,SIGNAL(toggled(bool)), this, SLOT(switch_server_loacl(bool)));
}

void public_sqlSet::took()
{
    if(public_sql::islocal){
        public_sql::localdbHostName = ui->lineEdit_hostname->text();
        public_sql::localdbDatabaseName = ui->lineEdit_dbname->text();
        public_sql::localdbUserName = ui->lineEdit_usrname->text();
        public_sql::localdbPassword = ui->lineEdit_password->text();
    } else {
        public_sql::dbHostName = ui->lineEdit_hostname->text();
        public_sql::dbDatabaseName = ui->lineEdit_dbname->text();
        public_sql::dbUserName = ui->lineEdit_usrname->text();
        public_sql::dbPassword = ui->lineEdit_password->text();
    }
    if(public_sql::islocal){
        public_sql::MySQL_creatlocal_mysql_hddpos();
    }
    public_sql::MySQL_PARAM_SAVE();
    this->accept();
}

void public_sqlSet::toexit()
{
    this->reject();
}

void public_sqlSet::switch_server_loacl(bool flag)
{
    public_sql::islocal = flag;
    ui->lineEdit_hostname->setText(public_sql::getdbHostName());
    ui->lineEdit_dbname->setText(public_sql::getdbDatabaseName());
    ui->lineEdit_usrname->setText(public_sql::getdbUserName());
    ui->lineEdit_password->setText(public_sql::getdbPassword());

    ui->lineEdit_hostname->setEnabled(!flag);
    ui->lineEdit_dbname->setEnabled(!flag);

#ifdef Q_OS_UNIX
    ui->lineEdit_usrname->setEnabled(!flag);
    ui->lineEdit_password->setEnabled(!flag);
#endif
}

void public_sqlSet::tohddpos_yaoqianshu()
{
    //copy fromres
    public_sql::remove_copyfile(":/image/MyApplication", qApp->applicationDirPath()+"/MyApplication");
    public_sql::remove_copyfile(":/image/emptyData.sql", qApp->applicationDirPath()+"/emptyData.sql");

    //conduct outer exe
    QProcess p;
    p.start("\""+qApp->applicationDirPath()+"/MyApplication"+"\"");
    p.waitForFinished();
    qDebug() << p.readAll();
    qDebug() << "\""+qApp->applicationDirPath()+"/MyApplication"+"\"";
    p.close();
    //delete fromres
    public_sql::removefile(qApp->applicationDirPath()+"/MyApplication");
    public_sql::removefile(qApp->applicationDirPath()+"/emptyData.sql");
}

//=============
QMutex YUN_POLLING_thread::mutex;
YUN_POLLING_thread::~YUN_POLLING_thread()
{

}

void YUN_POLLING_thread::run()
{
    if(mutex.tryLock(1)){
        WEIXIN_POLLING_FROM_YUNPOS();
        mutex.unlock();
    }
}
//定时从云端获取信息,并且更新到本地数据库
void YUN_POLLING_thread::WEIXIN_POLLING_FROM_YUNPOS(){
    {
        QSqlDatabase sql_db=QSqlDatabase::cloneDatabase(QSqlDatabase::database(), "WEIXIN_POLLING_FROM_YUNPOS");
        if(false==sql_db.open()) qDebug() << sql_db.lastError();
        QEventLoop eloop;
        while(1){
            {//分开计算,方便及时退出
                int loopcount=20;//=10 000
                while(loopcount--){
                    QTimer::singleShot(500, &eloop, SLOT(quit()));
                    eloop.exec(QEventLoop::ExcludeUserInputEvents);
                }
            }
            {//云端同步
                QString bdate=QDateTime::currentDateTime().toString("yyyy-MM-dd");
                QString edate=bdate;
                int flag=0;
                w_sys_manage_cloudsync::get_weixin_orderinfo(sql_db,bdate, edate, flag);
                w_sys_manage_cloudsync::get_weixin_orderdish(sql_db,bdate, edate, flag);
                w_sys_manage_cloudsync::set_weixin_payforwater(sql_db,bdate, edate, flag);
            }
        }
    }
    QSqlDatabase::removeDatabase("WEIXIN_POLLING_FROM_YUNPOS");
}
