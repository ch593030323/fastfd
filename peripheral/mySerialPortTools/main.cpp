#include "frmmain.h"
//#include <QApplication>
#include <QTranslator>
#include "myhelper.h"
#include<QDesktopWidget>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //设置字符编码和外观样式
    myHelper::SetUTF8Code();
    myHelper::SetStyle();

    //加载中文字符
    QTranslator translator;
    translator.load(":/qt_zh_CN.qm");
    a.installTranslator(&translator);

    frmMain window;

    QDesktopWidget *desktop=QApplication::desktop();
    int w=desktop->width();
        //获取桌面高度
    int h=desktop->height();
   // window.move((w-window.width()),0);
   // window.setFixedHeight(h);

    window.show();
    return a.exec();
}
