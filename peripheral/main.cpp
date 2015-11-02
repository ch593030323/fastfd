#include "mainwindow.h"
#include <QApplication>
#include <QSettings>
#include <QTranslator>
#include <QTextCodec>
#include <QtDebug>
#include <QFile>

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

    QString sys_language="en";
#ifdef Q_OS_UNIX
    QSettings conf("/usr/desk/desktop.ini", QSettings::IniFormat);
    QString tran=conf.value("desktop/language").toString();
    if(tran!="1"){
        sys_language="en";
    } else {
        sys_language="zn";
    }
#endif
    //翻译
    QTranslator translator;
    //    if(sys_language=="en"){
    qDebug() <<  translator.load(":/peripheral.qm");
    app.installTranslator(&translator);
    //    }

    MainWindow w;
    w.setWindowTitle("DeviceTest");
    w.setWindowIcon(QPixmap(":"));
    w.show();

    return app.exec();
}
