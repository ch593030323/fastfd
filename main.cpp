#include <QApplication>
#include "mainwindow.h"
#include "public_logindialog.h"
#include <QTranslator>
#include <QTextCodec>
#include <QFile>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDebug>
#include <QTableView>
#include <QSqlError>
#include <QSqlRecord>
#include <QLineEdit>
#include "lds_select_member_types.h"
#include "n_func.h"
#include <QStyle>
#include "backheader.h"
#include "w_m_member.h"
#include "w_i_inventory.h"
#include "w_sys_manage.h"
#include "public_sql.h"
#include "lds_loadingdialog.h"

#include <QSettings>
#include <QTextStream>
#include <QFontDatabase>
#include "reader_rf.h"
#include "w_bt_dish.h"
#include <QTcpSocket>
#include "w_sys_manage_sqlcopy.h"
#include "lds_messagebox.h"
#include "lds_dialog_input.h"
#include "auto_expand_image.h"
#include "lds_dialog_drog.h"
#include "printer_pos_vidpid.h"
#include "asyn_printer.h"
#include "printer_normal_t2us.h"
#include "w_scr_dish.h"
#include "w_scr_dish_restaurant_dish_dialog.h"
#include "w_scr_dish_restaurant_dialog.h"
#include "frontviewdialog_tableview.h"
#include "w_scr_dish_switch_detail.h"
#include "public_encryption_dog.h"

//回调函数实现debug信息到文件
//有做法自动弹出，可以设置
//预订单，落单，付款，结束后返回
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
void customMessageHandler(QtMsgType type, const char *msg)
{
    QString txt;
    switch (type) {
    //调试信息提示
    case QtDebugMsg:
        break;
        //一般的warning提示
    case QtWarningMsg:
        txt = QString("Warning: %1").arg(msg);
        break;
        //严重错误提示
    case QtCriticalMsg:
        txt = QString("Critical: %1").arg(msg);
        break;
        //致命错误提示
    case QtFatalMsg:
        txt = QString("Fatal: %1").arg(msg);
        break;
    }

    //    if(txt.isEmpty()) return;
    //    QFile outFile(public_sql::init_sql_file_dir+"/"+"debuglog.txt");
    //    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    //    QTextStream ts(&outFile);
    //    ts << ("["+QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")+"]"+txt) << "\n";
}
#else
void customMessageHandler(QtMsgType type, const QMessageLogContext &msg, const QString &text)
{
    QString txt;
    switch (type) {
    //调试信息提示
    case QtDebugMsg:
        break;
        //一般的warning提示
    case QtWarningMsg:
        txt = QString("Warning: %1").arg(text);
        break;
        //严重错误提示
    case QtCriticalMsg:
        txt = QString("Critical: %1").arg(text);
        break;
        //致命错误提示
    case QtFatalMsg:
        txt = QString("Fatal: %1").arg(text);
        break;
    }

    //    if(txt.isEmpty()) return;
    //    QFile outFile(public_sql::init_sql_file_dir+"/"+"debuglog.txt");
    //    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    //    QTextStream ts(&outFile);
    //    ts << ("["+QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")+"]"+txt) << "\n";
}

#endif

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    //设置编码
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");//中文可用//包含头文件
    QTextCodec::setCodecForLocale(codec);

#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
#else
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);
#endif
    //登录界面,从参数中获取usr和passwd
    //./fastfd -u1111 -p1111 -d1 用户名 密码 debug模式
    QString u,p,d;
    if(argc>1){
        //1
        QStringList ps;
        for(int index=1;index < argc;index++){
            ps.append(argv[index]);
        }
        //2
        QMap<QString, QString> pre_value;
        foreach(QString p, ps){
            pre_value.insert(p.mid(0, 2), p.mid(2));
        }
        //3
        u=pre_value.value("-u");
        p=pre_value.value("-p");
        d=pre_value.value("-d");
    }
    //调试模式
#ifdef QT_DEBUG
#else
    if(d.isEmpty()
            ||d.toInt()==0){
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
        qInstallMsgHandler(customMessageHandler);
#else
        qInstallMessageHandler(customMessageHandler);
#endif
    } else {
    }
#endif

    backheader::BACKHEADER_INIT();//后台分辨率等信息的初始化
    public_sql::PUBLIC_SQL_INIT();//数据库的初始化
    public_sql::MySQL_PARAM_INIT();//mysql的一些初始化

    w_i_inventory::MODEL_INVENTORY_INIT();//盘点的初始化
    w_sys_manage::MODEL_SYS_INIT();//系统设置的初始化
    w_bt_dish::MODEL_DISH_INIT();//菜品资料的初始化
    //翻译
#ifdef Q_OS_WIN
    backheader::sys_language="zn";
#endif
    QTranslator translator;
    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
//    if(backheader::sys_language=="en"){
        translator.load(public_sql::init_sql_file_dir+"/fastfd.qm");
        app.installTranslator(&translator);
//    }
    //字体
    int font_id = QFontDatabase::addApplicationFont(public_sql::init_sql_file_dir+"/FZZHJW.TTF");
    if(-1 != font_id){
        QFont font=app.font();
        font.setFamily(QFontDatabase::applicationFontFamilies(font_id).first());
        app.setFont(font);
    }
    //log
    app.setWindowIcon(QIcon(":/fastfd.ico"));
    //样式表
    QFile file(":/fastfd.qss");
    if(file.open(QFile::ReadOnly)){
        //logo
        QString login_logo=conf.value("public_sql/login_logo").toString();
        if(login_logo.isEmpty()){
            login_logo=public_sql::def_logo;
        }
        app.setStyleSheet(file.readAll());
        file.close();
    }
    //软件到期自检

#ifdef Q_OS_WIN
    //加密狗检测
    {
        public_encryption_dog edog;
        int remainday;
        QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\fastfd",
                           QSettings::NativeFormat);
        //默认
        if(settings.value("window_soft_limit/date",QDate()).toDate().isNull()){
            settings.setValue("window_soft_limit/date", QDate::currentDate());
            settings.setValue("window_soft_limit/canuse", false);
        }
        while(1){
            //若是没有激活的话
            if(!settings.value("window_soft_limit/canuse").toBool()){
                if((remainday=(settings.value("window_soft_limit/date").toDate().daysTo(QDate::currentDate())))>30){
                    lds_messagebox::warning(0, QObject::tr("软件有效期")+QObject::tr("剩余")+QString::number(30-remainday)+QObject::tr("天"),QObject::tr("软件使用已经超过30天，请插入加密狗以便继续使用!"));
                } else {
                    //软件在使用期内
                    if(0!=lds_messagebox::warning(0, QObject::tr("软件有效期/软件在试用期内")+QObject::tr("剩余")+QString::number(30-remainday)+QObject::tr("天")
                                                  ,QObject::tr("是否插入加密狗激活?"), QObject::tr("确定"),QObject::tr("取消"))){
                        break;
                    }
                }

                if(0==lds_messagebox::warning(0, QObject::tr("软件有效期"),QObject::tr("请插入加密狗"),QObject::tr("确定"),QObject::tr("取消"))){
                    if(!edog.conductlock()){
                        lds_messagebox::warning(0, QObject::tr("软件有效期/加密狗检测"),edog.errorString());
                        return 0;
                    }
                    settings.setValue("window_soft_limit/canuse", true);
                    lds_messagebox::warning(0, QObject::tr("软件有效期/加密狗检测"),QObject::tr("软件激活成功!"));
                } else {
                    lds_messagebox::warning(0, QObject::tr("软件有效期/加密狗检测"),QObject::tr("操作取消"));
                    return 0;
                }
            }
            break;
        }
    }
#else
    if(!w_sys_manage_sqlcopy_soft_limit::static_soft_limit_check()){
        return 0;
    }
#endif

    //登录界面
    public_loginDialog login(0, u, p);
    login.setversion(QObject::tr("版本号:")+public_sql::get_MySQL_sqlversion()/*数据库版本*/+"1"/*软件版本*/+"."+"151030"/*修改日期*/);
    QObject::connect(backheader::printer,SIGNAL(errorString(QString, QString)),&login,SLOT(showprinterr(QString, QString)));
    backheader::static_dialogshow(login, QObject::tr("用户登录"));

    if(d=="1")
        login.showmemoinfo();
//    blockIns ins;
//    ins.append(backheader::printer->drawText("1B40 1B6101 1D2111", QObject::tr("交班对账表\n")));
//    ins.append(backheader::printer->drawText("1B40"));
//    ins.append(backheader::printer->drawText("",QObject::tr("交班员：张晓红(0001)   j接班人:()\n")));
//    ins.append(backheader::printer->drawText("",QObject::tr("开始时间：2015-10-09 13：32：17\n")));
//    ins.append(backheader::printer->drawText("",QObject::tr("交班时间：2015-10-09 16：32：17\n")));
//    ins.append(backheader::printer->drawText("",QObject::tr("备注\n")));
//    ins.append(backheader::printer->drawText("",QObject::tr("\n")));
//    ins.append(backheader::printer->drawText("1D2101",QObject::tr("消费金额：3479.00\n")));
//    ins.append(backheader::printer->drawText("1D2100"));
//    ins.append(backheader::printer->drawText("",QObject::tr("折扣金额:"), Qt::AlignRight, 12,12,QObject::tr("100.00\n")));
//    ins.append(backheader::printer->drawText("",QObject::tr("赠送金额:"), Qt::AlignRight, 12,12,QObject::tr("100.00\n")));
//    ins.append(backheader::printer->drawText("",QObject::tr("抹零金额:"), Qt::AlignRight, 12,12,QObject::tr("9.00\n")));
//    ins.append(backheader::printer->drawText("1D2101",QObject::tr("实收金额：3270.00\n")));
//    ins.append(backheader::printer->drawText("1D2100"));
//    ins.append(backheader::printer->drawText("",QObject::tr("现金:"), Qt::AlignRight, 12,12,QObject::tr("100.00\n")));
//    ins.append(backheader::printer->drawText("",QObject::tr("银行卡:"), Qt::AlignRight, 12,12,QObject::tr("100.00\n")));
//    ins.append(backheader::printer->drawText("",QObject::tr("储值卡:"), Qt::AlignRight, 12,12,QObject::tr("100.00\n")));
//    ins.append(backheader::printer->drawText("",QObject::tr("微信:"), Qt::AlignRight, 12,12,QObject::tr("100.00\n")));
//    ins.append(backheader::printer->drawText("",QObject::tr("支付宝:"), Qt::AlignRight, 12,12,QObject::tr("100.00\n")));
//    ins.append(backheader::printer->drawText("",QObject::tr("代金券:"), Qt::AlignRight, 12,12,QObject::tr("100.00\n")));
//    ins.append(backheader::printer->drawText("",QObject::tr("大众点评:"), Qt::AlignRight, 12,12,QObject::tr("100.00\n")));
//    ins.append(backheader::printer->drawText("",QObject::tr("美团:"), Qt::AlignRight, 12,12,QObject::tr("100.00\n")));
//    ins.append(backheader::printer->drawText("",QObject::tr("百度糯米:"), Qt::AlignRight, 12,12,QObject::tr("100.00\n")));
//    ins.append(backheader::printer->drawText("",QObject::tr("其他方式:"), Qt::AlignRight, 12,12,QObject::tr("100.00\n")));
//    ins.append(backheader::printer->drawText("",QObject::tr("反结账:"), Qt::AlignRight, 12,12,QObject::tr("100.00\n")));
//    ins.append(backheader::printer->drawText("",QObject::tr("\n")));
//    ins.append(backheader::printer->drawText("",QObject::tr("签字\n")));

    return app.exec();
}

