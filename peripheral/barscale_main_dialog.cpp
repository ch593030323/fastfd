#include "barscale_main_dialog.h"
#include "ui_barscale_main_dialog.h"
#include <QTimer>
#include <QHostAddress>
#include "electronicscale_dahua.h"
#include "electronicscale_cas.h"
#include "electronicscale_ysheng.h"
#include "electronicscale_djian.h"
#include "electronicscale_kaifeng.h"
#include "printer_pos_net.h"
#include <QtCore>
#include "commonheader.h"
#if QT_VERSION > QT_VERSION_CHECK(5,0,0)
#include <QtConcurrent/QtConcurrent>
#endif
#include "barscale_net_set_dialog.h"
#include "barscale_downdialog.h"
#include <QFileDialog>
#include <QMessageBox>
#include "uinormal_message.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include "ui_popinfo_cannotpop_dialog.h"
#include "ui_labelprint_goodinfo_add_dialog.h"
#include "labelprint_goodinfo_add_dialog.h"

static void ScaleDownInfo(QObject *receiver,
                          QStringList conduct_scale,
                          bool *isbreak,
                          QStandardItemModel *downmodel,
                          QStandardItemModel *ipportmodel)
{
    //! 开始计时
    QMetaObject::invokeMethod(receiver,
                              "startTimer",
                              Qt::QueuedConnection
                              );
    //! 开始计时
    //! 修改pushbutton的 文本
    QMetaObject::invokeMethod(receiver,
                              "updateButtonText",
                              Qt::QueuedConnection,
                              Q_ARG(QString, QObject::tr("暂停"))
                              );
    //! 修改pushbutton的 文本
    //! 获取条码秤类别
    QString scaletype = conduct_scale[1];
    QString conduct_type=conduct_scale[0];
    //! 获取条码秤类别

    *isbreak = false;//初始化 手动中断 为 false
    switch(CommonHeader::creat_barcode_scaletypes().indexOf(scaletype)){
    case 0://上海大华 TM-15H
    {
        if(conduct_type == "down"){
            int ipport_row = 0;
            int ipport_rowcount = ipportmodel->rowCount();
            Printer_POS_NET printer;
            //! 按照ip port  下载数据 <--begin-->
            while(ipport_row < ipport_rowcount){
                if(printer.tryconnect(ipportmodel->item(ipport_row, 1)->text(),
                                      quint16(ipportmodel->item(ipport_row, 2)->text().toInt()),
                                      30000,
                                      isbreak
                                      )){
                    ipportmodel->item(ipport_row, 3)->setText(QObject::tr("连接成功"));
                    ipportmodel->item(ipport_row, 4)->setText(QString("%1").arg(downmodel->rowCount()));

                    //! 下载重传 <--begin-->
                    bool isSuccess = false;
                    for(int loop_step = 0; loop_step < 3; loop_step ++){
                        int down_row = 0;
                        int down_rowcount = downmodel->rowCount();
                        ipportmodel->item(ipport_row, 3)->setText(QObject::tr("传输中[%1]...").arg(loop_step));
                        //! 一次传输 <--begin-->
                        while(down_row < down_rowcount){
                            printer.writedirect(ElectronicScale_DAHUA::downloadPLUINFO(downmodel, down_row));
                            if(printer.waitForReadyRead(30000,isbreak)){
                                QByteArray ret = printer.tcpSocket->readAll();
                                //! 3次读取，读满11为止 <--begin-->
                                for(int c = 0; c < 10; c++){
                                    if(ret.length() >= 11) break;
                                    if(printer.waitForReadyRead(1000,isbreak)){
                                        ret += printer.tcpSocket->readAll();
                                    }
                                }
                                //! 3次读取，读满11为止 <--end-->

                                if(ret.startsWith("0v")){
                                    down_row++;
                                    QMetaObject::invokeMethod(receiver,
                                                              "updateProgressbar",
                                                              Qt::QueuedConnection,
                                                              Q_ARG(QString, QString("%1/%2").arg(down_row).arg(down_rowcount)),
                                                              Q_ARG(int, ipport_row)
                                                              );
                                    if(down_row >= down_rowcount){
                                        isSuccess = true;
                                        break;
                                    }
                                    continue;
                                }
                            }
                            break;
                        }
                        //! 一次传输 <--end-->
                        if(isSuccess)break;
                    }
                    //! 下载重传 <--end-->

                    if(isSuccess){
                        ipportmodel->item(ipport_row, 3)->setText(QObject::tr("传输成功"));
                    } else {
                        ipportmodel->item(ipport_row, 3)->setText(QObject::tr("传输失败"));
                    }
                } else {
                    ipportmodel->item(ipport_row, 3)->setText(QObject::tr("连接失败"));
                }
                printer.closeconnect(30000, isbreak);
                ipport_row++;
            }
            //! 按照ip port  下载数据 <--end-->

        } else if(conduct_type == "clear"){
            int ipport_row = 0;
            int ipport_rowcount = ipportmodel->rowCount();
            Printer_POS_NET printer;
            //! 按照ip port  下载数据 <--begin-->
            while(ipport_row < ipport_rowcount){
                if(printer.tryconnect(ipportmodel->item(ipport_row, 1)->text(),
                                      quint16(ipportmodel->item(ipport_row, 2)->text().toInt()),
                                      30000,
                                      isbreak
                                      )){
                    ipportmodel->item(ipport_row, 3)->setText(QObject::tr("连接成功"));
                    ipportmodel->item(ipport_row, 4)->setText(QString("%1").arg(1));

                    //! 下载重传 <--begin-->
                    bool isSuccess = false;
                    for(int loop_step = 0; loop_step < 3; loop_step ++){
                        int down_row = 0;
                        int down_rowcount = 1;
                        ipportmodel->item(ipport_row, 3)->setText(QObject::tr("传输中[%1]...").arg(loop_step));
                        //! 一次传输 <--begin-->
                        while(down_row < down_rowcount){
                            printer.writedirect(ElectronicScale_DAHUA::clearPLU());
                            if(printer.waitForReadyRead(30000,isbreak)){
                                QByteArray ret = printer.tcpSocket->readAll();
                                //! 3次读取，读满8为止 <--begin-->
                                int loop_count = 3;
                                while(loop_count --){
                                    if(ret.length() < 8){
                                        printer.waitForReadyRead(30000,isbreak);
                                        ret += printer.tcpSocket->readAll();
                                    }
                                }
                                //! 3次读取，读满8为止 <--end-->

                                if(ret.startsWith("0i")){
                                    down_row++;
                                    QMetaObject::invokeMethod(receiver,
                                                              "updateProgressbar",
                                                              Qt::QueuedConnection,
                                                              Q_ARG(QString, QString("%1/%2").arg(down_row).arg(down_rowcount)),
                                                              Q_ARG(int, ipport_row)
                                                              );
                                    isSuccess = true;
                                }
                            }
                            break;
                        }
                        //! 一次传输 <--end-->
                        if(isSuccess)break;
                    }
                    //! 下载重传 <--end-->

                    if(isSuccess){
                        ipportmodel->item(ipport_row, 3)->setText(QObject::tr("传输成功"));
                    } else {
                        ipportmodel->item(ipport_row, 3)->setText(QObject::tr("传输失败"));
                    }
                } else {
                    ipportmodel->item(ipport_row, 3)->setText(QObject::tr("连接失败"));
                }
                printer.closeconnect(30000, isbreak);
                ipport_row++;
            }
            //! 按照ip port  下载数据 <--end-->
        } else if(conduct_type == "setlabel"){
            int ipport_row = 0;
            int ipport_rowcount = ipportmodel->rowCount();
            Printer_POS_NET printer;
            //! 按照ip port  下载数据 <--begin-->
            while(ipport_row < ipport_rowcount){
                if(printer.tryconnect(ipportmodel->item(ipport_row, 1)->text(),
                                      quint16(ipportmodel->item(ipport_row, 2)->text().toInt()),
                                      30000,
                                      isbreak
                                      )){
                    ipportmodel->item(ipport_row, 3)->setText(QObject::tr("连接成功"));
                    ipportmodel->item(ipport_row, 4)->setText(QString("%1").arg(2));

                    //! 下载重传 <--begin-->
                    bool isSuccess = false;
                    for(int loop_step = 0; loop_step < 3; loop_step ++){
                        int down_row = 0;
                        int down_rowcount = 2;
                        ipportmodel->item(ipport_row, 3)->setText(QObject::tr("传输中[%1]...").arg(loop_step));
                        //! 一次传输 <--begin-->
                        while(down_row < down_rowcount){
                            printer.writedirect(ElectronicScale_DAHUA::setLABEL_40x30_step1());
                            if(printer.waitForReadyRead(30000,isbreak)){
                                QByteArray ret = printer.tcpSocket->readAll();
                                //! 3次读取，读满7为止 <--begin-->
                                int loop_count = 3;
                                while(loop_count --){
                                    if(ret.length() < 7){
                                        printer.waitForReadyRead(30000,isbreak);
                                        ret += printer.tcpSocket->readAll();
                                    }
                                }
                                //! 3次读取，读满7为止 <--end-->
                                if(ret.startsWith("0t")){
                                    down_row++;
                                    QMetaObject::invokeMethod(receiver,
                                                              "updateProgressbar",
                                                              Qt::QueuedConnection,
                                                              Q_ARG(QString, QString("%1/%2").arg(down_row).arg(down_rowcount)),
                                                              Q_ARG(int, ipport_row)
                                                              );
                                    printer.writedirect(ElectronicScale_DAHUA::setLABEL_40x30_step2());
                                    if(printer.waitForReadyRead(30000,isbreak)){
                                        QByteArray ret = printer.tcpSocket->readAll();
                                        qDebug() << ret << ret.length();
                                        //! 3次读取，读满7为止 <--begin-->
                                        int loop_count = 3;
                                        while(loop_count --){
                                            if(ret.length() < 7){
                                                printer.waitForReadyRead(30000,isbreak);
                                                ret += printer.tcpSocket->readAll();
                                            }
                                        }
                                        //! 3次读取，读满7为止 <--end-->
                                        if(ret.startsWith("0r")){
                                            down_row++;
                                            QMetaObject::invokeMethod(receiver,
                                                                      "updateProgressbar",
                                                                      Qt::QueuedConnection,
                                                                      Q_ARG(QString, QString("%1/%2").arg(down_row).arg(down_rowcount)),
                                                                      Q_ARG(int, ipport_row)
                                                                      );
                                            isSuccess = true;
                                            break;
                                        }
                                    }
                                }
                            }
                            break;
                        }
                        //! 一次传输 <--end-->
                        if(isSuccess)break;
                    }
                    //! 下载重传 <--end-->

                    if(isSuccess){
                        ipportmodel->item(ipport_row, 3)->setText(QObject::tr("传输成功"));
                    } else {
                        ipportmodel->item(ipport_row, 3)->setText(QObject::tr("传输失败"));
                    }
                } else {
                    ipportmodel->item(ipport_row, 3)->setText(QObject::tr("连接失败"));
                }
                printer.closeconnect(30000, isbreak);
                ipport_row++;
            }
            //! 按照ip port  下载数据 <--end-->
        }
    }
        break;
    case 1://凯士 CL5200
    {
        if(conduct_type == "down"){
            int ipport_row = 0;
            int ipport_rowcount = ipportmodel->rowCount();
            Printer_POS_NET printer;
            //! 按照ip port  下载数据 <--begin-->
            while(ipport_row < ipport_rowcount){
                if(printer.tryconnect(ipportmodel->item(ipport_row, 1)->text(),
                                      quint16(ipportmodel->item(ipport_row, 2)->text().toInt()),
                                      30000,
                                      isbreak
                                      )){
                    ipportmodel->item(ipport_row, 3)->setText(QObject::tr("连接成功"));
                    ipportmodel->item(ipport_row, 4)->setText(QString("%1").arg(downmodel->rowCount()));

                    //! 下载重传 <--begin-->
                    bool isSuccess = false;
                    for(int loop_step = 0; loop_step < 3; loop_step ++){
                        int down_row = 0;
                        int down_rowcount = downmodel->rowCount();
                        ipportmodel->item(ipport_row, 3)->setText(QObject::tr("传输中[%1]...").arg(loop_step));
                        //! 一次传输 <--begin-->
                        while(down_row < down_rowcount){
                            printer.writedirect(ElectronicScale_CAS::downloadPLUINFO(
                                                    downmodel->item(down_row,0)->text()
                                                    , downmodel->item(down_row,8)->text()
                                                    , downmodel->item(down_row,1)->text()
                                                    , downmodel->item(down_row,26)->text()
                                                    , downmodel->item(down_row,2)->text()

                                                    , downmodel->item(down_row,7)->text()
                                                    , downmodel->item(down_row,3)->text()
                                                    )
                                                );
                            if(printer.waitForReadyRead(30000,isbreak)){
                                QByteArray ret = printer.tcpSocket->readAll();
                                if(ret.endsWith("W02:O01")){
                                    down_row++;
                                    QMetaObject::invokeMethod(receiver,
                                                              "updateProgressbar",
                                                              Qt::QueuedConnection,
                                                              Q_ARG(QString, QString("%1/%2").arg(down_row).arg(down_rowcount)),
                                                              Q_ARG(int, ipport_row)
                                                              );
                                    if(down_row >= down_rowcount){
                                        isSuccess = true;
                                        break;
                                    }
                                    continue;
                                }
                            }
                            break;
                        }
                        //! 一次传输 <--end-->
                        if(isSuccess)break;
                    }
                    //! 下载重传 <--end-->

                    if(isSuccess){
                        ipportmodel->item(ipport_row, 3)->setText(QObject::tr("传输成功"));
                    } else {
                        ipportmodel->item(ipport_row, 3)->setText(QObject::tr("传输失败"));
                    }
                } else {
                    ipportmodel->item(ipport_row, 3)->setText(QObject::tr("连接失败"));
                }
                printer.closeconnect(30000, isbreak);
                ipport_row++;
            }
            //! 按照ip port  下载数据 <--end-->
        } else if(conduct_type == "clear"){
            int ipport_row = 0;
            int ipport_rowcount = ipportmodel->rowCount();
            Printer_POS_NET printer;
            //! 按照ip port  下载数据 <--begin-->
            while(ipport_row < ipport_rowcount){
                if(printer.tryconnect(ipportmodel->item(ipport_row, 1)->text(),
                                      quint16(ipportmodel->item(ipport_row, 2)->text().toInt()),
                                      30000,
                                      isbreak
                                      )){
                    ipportmodel->item(ipport_row, 3)->setText(QObject::tr("连接成功"));
                    ipportmodel->item(ipport_row, 4)->setText(QString("%1").arg(downmodel->rowCount()));

                    //! 下载重传 <--begin-->
                    bool isSuccess = false;
                    for(int loop_step = 0; loop_step < 3; loop_step ++){
                        int down_row = 0;
                        int down_rowcount = downmodel->rowCount();
                        ipportmodel->item(ipport_row, 3)->setText(QObject::tr("传输中[%1]...").arg(loop_step));
                        //! 一次传输 <--begin-->
                        while(down_row < down_rowcount){
                            printer.writedirect(ElectronicScale_CAS::clearPLU(
                                                    downmodel->item(down_row, 0)->text()
                                                    , downmodel->item(down_row, 8)->text()
                                                    )
                                                );
                            if(printer.waitForReadyRead(30000,isbreak)){
                                QByteArray ret = printer.tcpSocket->readAll();
                                if(ret.endsWith("C003:O13")){
                                    down_row++;
                                    QMetaObject::invokeMethod(receiver,
                                                              "updateProgressbar",
                                                              Qt::QueuedConnection,
                                                              Q_ARG(QString, QString("%1/%2").arg(down_row).arg(down_rowcount)),
                                                              Q_ARG(int, ipport_row)
                                                              );
                                    if(down_row >= down_rowcount){
                                        isSuccess = true;
                                        break;
                                    }
                                    continue;
                                }
                            }
                            break;
                        }
                        //! 一次传输 <--end-->
                        if(isSuccess)break;
                    }
                    //! 下载重传 <--end-->

                    if(isSuccess){
                        ipportmodel->item(ipport_row, 3)->setText(QObject::tr("传输成功"));
                    } else {
                        ipportmodel->item(ipport_row, 3)->setText(QObject::tr("传输失败"));
                    }
                } else {
                    ipportmodel->item(ipport_row, 3)->setText(QObject::tr("连接失败"));
                }
                printer.closeconnect(30000, isbreak);
                ipport_row++;
            }
            //! 按照ip port  下载数据 <--end-->
        }
    }
        break;
    case 2: //友声
    {
        if(conduct_type == "down"){
            int ipport_row = 0;
            int ipport_rowcount = ipportmodel->rowCount();
            Printer_POS_NET printer;
            //! 按照ip port  下载数据 <--begin-->
            while(ipport_row < ipport_rowcount){
                if(printer.tryconnect(ipportmodel->item(ipport_row, 1)->text(),
                                      quint16(ipportmodel->item(ipport_row, 2)->text().toInt()),
                                      30000,
                                      isbreak
                                      )){
                    ipportmodel->item(ipport_row, 3)->setText(QObject::tr("连接成功"));
                    ipportmodel->item(ipport_row, 4)->setText(QString("%1").arg(downmodel->rowCount()));

                    //! 下载重传 <--begin-->
                    bool isSuccess = false;
                    for(int loop_step = 0; loop_step < 3; loop_step ++){
                        int down_row = 0;
                        int down_rowcount = downmodel->rowCount();
                        ipportmodel->item(ipport_row, 3)->setText(QObject::tr("传输中[%1]...").arg(loop_step));
                        //! 一次传输 <--begin-->
                        while(down_row < down_rowcount){
                            printer.writedirect(ElectronicScale_YSHENG::downloadPLUINFO(downmodel, down_row));
                            if(printer.waitForReadyRead(30000,isbreak)){
                                QByteArray ret = printer.tcpSocket->readAll();
                                if(ret.startsWith(ElectronicScale_YSHENG::end_ans())){
                                    down_row++;
                                    QMetaObject::invokeMethod(receiver,
                                                              "updateProgressbar",
                                                              Qt::QueuedConnection,
                                                              Q_ARG(QString, QString("%1/%2").arg(down_row).arg(down_rowcount)),
                                                              Q_ARG(int, ipport_row)
                                                              );
                                    if(down_row >= down_rowcount){
                                        isSuccess = true;
                                        break;
                                    }
                                    continue;
                                }
                            }
                            break;
                        }
                        //! 一次传输 <--end-->
                        if(isSuccess)break;
                    }
                    //! 下载重传 <--end-->

                    if(isSuccess){
                        ipportmodel->item(ipport_row, 3)->setText(QObject::tr("传输成功"));
                    } else {
                        ipportmodel->item(ipport_row, 3)->setText(QObject::tr("传输失败"));
                    }
                } else {
                    ipportmodel->item(ipport_row, 3)->setText(QObject::tr("连接失败"));
                }
                printer.closeconnect(30000, isbreak);
                ipport_row++;
            }
            //! 按照ip port  下载数据 <--end-->
        } else if(conduct_type == "clear"){
            int ipport_row = 0;
            int ipport_rowcount = ipportmodel->rowCount();
            Printer_POS_NET printer;
            //! 按照ip port  下载数据 <--begin-->
            while(ipport_row < ipport_rowcount){
                if(printer.tryconnect(ipportmodel->item(ipport_row, 1)->text(),
                                      quint16(ipportmodel->item(ipport_row, 2)->text().toInt()),
                                      30000,
                                      isbreak
                                      )){
                    ipportmodel->item(ipport_row, 3)->setText(QObject::tr("连接成功"));
                    ipportmodel->item(ipport_row, 4)->setText(QString("%1").arg(1));

                    //! 下载重传 <--begin-->
                    bool isSuccess = false;
                    for(int loop_step = 0; loop_step < 3; loop_step ++){
                        int down_row = 0;
                        int down_rowcount = 1;
                        ipportmodel->item(ipport_row, 3)->setText(QObject::tr("传输中[%1]...").arg(loop_step));
                        //! 一次传输 <--begin-->
                        while(down_row < down_rowcount){
                            printer.writedirect(ElectronicScale_YSHENG::clearPLU());
                            if(printer.waitForReadyRead(30000,isbreak)){
                                QByteArray ret = printer.tcpSocket->readAll();
                                if(ret.startsWith(ElectronicScale_YSHENG::end_ans())){
                                    down_row++;
                                    QMetaObject::invokeMethod(receiver,
                                                              "updateProgressbar",
                                                              Qt::QueuedConnection,
                                                              Q_ARG(QString, QString("%1/%2").arg(down_row).arg(down_rowcount)),
                                                              Q_ARG(int, ipport_row)
                                                              );
                                    if(down_row >= down_rowcount){
                                        isSuccess = true;
                                        break;
                                    }
                                    continue;
                                }
                            }
                            break;
                        }
                        //! 一次传输 <--end-->
                        if(isSuccess)break;
                    }
                    //! 下载重传 <--end-->

                    if(isSuccess){
                        ipportmodel->item(ipport_row, 3)->setText(QObject::tr("传输成功"));
                    } else {
                        ipportmodel->item(ipport_row, 3)->setText(QObject::tr("传输失败"));
                    }
                } else {
                    ipportmodel->item(ipport_row, 3)->setText(QObject::tr("连接失败"));
                }
                printer.closeconnect(30000, isbreak);
                ipport_row++;
            }
            //! 按照ip port  下载数据 <--end-->
        }
    }
        break;
    case 3://厦门顶尖LS2CX
    {
        if(conduct_type == "down"){
            int ipport_row = 0;
            int ipport_rowcount = ipportmodel->rowCount();
            Printer_POS_NET printer;
            printer.resetSocket(Printer_POS_NET::UDP);
            //! 按照ip port  下载数据 <--begin-->
            while(ipport_row < ipport_rowcount){
                if(printer.tryconnect(ipportmodel->item(ipport_row, 1)->text(),
                                      quint16(ipportmodel->item(ipport_row, 2)->text().toInt()),
                                      30000,
                                      isbreak
                                      )){
                    ipportmodel->item(ipport_row, 3)->setText(QObject::tr("连接成功"));
                    ipportmodel->item(ipport_row, 4)->setText(QString("%1").arg(downmodel->rowCount()));

                    //! 下载重传 <--begin-->
                    bool isSuccess = false;
                    for(int loop_step = 0; loop_step < 3; loop_step ++){
                        int down_row = 0;
                        int down_rowcount = downmodel->rowCount();
                        ipportmodel->item(ipport_row, 3)->setText(QObject::tr("传输中[%1]...").arg(loop_step));
                        //! 一次传输 <--begin-->
                        while(down_row < down_rowcount){
                            qDebug()<< printer.writedirect(ElectronicScale_DJIAN::downloadPLUINFO(downmodel->item(down_row, 0)->text(),
                                                                                                  downmodel->item(down_row, 26)->text(),
                                                                                                  downmodel->item(down_row, 1)->text(),
                                                                                                  downmodel->item(down_row, 2)->text(),
                                                                                                  downmodel->item(down_row, 3)->text(),
                                                                                                  downmodel->item(down_row, 7)->text()
                                                                                                  ));
                            if(printer.waitForReadyRead(30000,isbreak)){
                                QByteArray ret = printer.tcpSocket->readAll();
                                qDebug() << ret.toHex() << down_row << down_rowcount;
                                if(ret.startsWith(ElectronicScale_DJIAN::downloadPLUINFO_back(downmodel->item(down_row, 1)->text()))){
                                    qDebug() << ret.toHex() << down_row << down_rowcount;
                                    down_row++;
                                    QMetaObject::invokeMethod(receiver,
                                                              "updateProgressbar",
                                                              Qt::QueuedConnection,
                                                              Q_ARG(QString, QString("%1/%2").arg(down_row).arg(down_rowcount)),
                                                              Q_ARG(int, ipport_row)
                                                              );
                                    if(down_row >= down_rowcount){
                                        isSuccess = true;
                                        break;
                                    }
                                    continue;
                                }
                            }
                            break;
                        }
                        //! 一次传输 <--end-->
                        if(isSuccess)break;
                    }
                    //! 下载重传 <--end-->

                    if(isSuccess){
                        ipportmodel->item(ipport_row, 3)->setText(QObject::tr("传输成功"));
                    } else {
                        ipportmodel->item(ipport_row, 3)->setText(QObject::tr("传输失败"));
                    }
                } else {
                    ipportmodel->item(ipport_row, 3)->setText(QObject::tr("连接失败"));
                }
                printer.closeconnect(30000, isbreak);
                ipport_row++;
            }
            //! 按照ip port  下载数据 <--end-->
        } else if(conduct_type == "clear"){

        }
    }
        break;
    case 4://凯丰电子秤
    {
        if(conduct_type == "down"){
            int ipport_row = 0;
            int ipport_rowcount = ipportmodel->rowCount();
            Printer_POS_NET printer;
            printer.resetSocket(Printer_POS_NET::UDP);
            //! 按照ip port  下载数据 <--begin-->
            while(ipport_row < ipport_rowcount){
                if(printer.tryconnect(ipportmodel->item(ipport_row, 1)->text(),
                                      quint16(ipportmodel->item(ipport_row, 2)->text().toInt()),
                                      30000,
                                      isbreak
                                      )){
                    ipportmodel->item(ipport_row, 3)->setText(QObject::tr("连接成功"));
                    ipportmodel->item(ipport_row, 4)->setText(QString("%1").arg(downmodel->rowCount()));

                    if(downmodel->rowCount()){
                        bool pre_set = false;
                        //设置商店名称
                        printer.writedirect(ElectronicScale_KAIFENG::setShopName(downmodel->item(0,27)->text()));
                        if(printer.waitForReadyRead(30000, isbreak)){
                            if(printer.tcpSocket->readAll().startsWith(ElectronicScale_KAIFENG::setShopName_back())){

                                // 和商店prefix
                                printer.writedirect(ElectronicScale_KAIFENG::getSysParam());
                                if(printer.waitForReadyRead(30000,isbreak)){
                                    QByteArray b = printer.tcpSocket->readAll();
                                    if(b.startsWith(ElectronicScale_KAIFENG::getSysParam_back())){

                                        printer.writedirect(ElectronicScale_KAIFENG::setShopPrefix(downmodel->item(0,8)->text(), b));
                                        if(printer.waitForReadyRead(30000,isbreak)){
                                            if(printer.tcpSocket->readAll().startsWith(ElectronicScale_KAIFENG::setShopPrefix_back())){
                                                pre_set = true;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        if(!pre_set){
                            ipport_row++;
                            continue;
                        }
                    }


                    //! 下载重传 <--begin-->
                    bool isSuccess = false;
                    for(int loop_step = 0; loop_step < 3; loop_step ++){
                        int down_row = 0;
                        int down_rowcount = downmodel->rowCount();
                        ipportmodel->item(ipport_row, 3)->setText(QObject::tr("传输中[%1]...").arg(loop_step));
                        //! 一次传输 <--begin-->
                        while(down_row < down_rowcount){
                            printer.writedirect(ElectronicScale_KAIFENG::downloadPLUINFO(downmodel->item(down_row, 0)->text(),
                                                                                         downmodel->item(down_row, 26)->text(),
                                                                                         downmodel->item(down_row, 1)->text(),
                                                                                         downmodel->item(down_row, 2)->text(),
                                                                                         downmodel->item(down_row, 3)->text(),
                                                                                         downmodel->item(down_row, 7)->text()
                                                                                         ));
                            if(printer.waitForReadyRead(30000,isbreak)){
                                QByteArray ret = printer.tcpSocket->readAll();
                                if(ret.startsWith(ElectronicScale_KAIFENG::downloadPLUINFO_back())){
                                    down_row++;
                                    QMetaObject::invokeMethod(receiver,
                                                              "updateProgressbar",
                                                              Qt::QueuedConnection,
                                                              Q_ARG(QString, QString("%1/%2").arg(down_row).arg(down_rowcount)),
                                                              Q_ARG(int, ipport_row)
                                                              );
                                    if(down_row >= down_rowcount){
                                        isSuccess = true;
                                        break;
                                    }
                                    continue;
                                }
                            }
                            break;
                        }
                        //! 一次传输 <--end-->
                        if(isSuccess)break;
                    }
                    //! 下载重传 <--end-->

                    if(isSuccess){
                        ipportmodel->item(ipport_row, 3)->setText(QObject::tr("传输成功"));
                    } else {
                        ipportmodel->item(ipport_row, 3)->setText(QObject::tr("传输失败"));
                    }
                } else {
                    ipportmodel->item(ipport_row, 3)->setText(QObject::tr("连接失败"));
                }
                printer.closeconnect(30000, isbreak);
                ipport_row++;
            }
            //! 按照ip port  下载数据 <--end-->
        } else if(conduct_type == "clear"){
            int ipport_row = 0;
            int ipport_rowcount = ipportmodel->rowCount();
            Printer_POS_NET printer;
            printer.resetSocket(Printer_POS_NET::UDP);

            //! 按照ip port  下载数据 <--begin-->
            while(ipport_row < ipport_rowcount){
                if(printer.tryconnect(ipportmodel->item(ipport_row, 1)->text(),
                                      quint16(ipportmodel->item(ipport_row, 2)->text().toInt()),
                                      30000,
                                      isbreak
                                      )){
                    ipportmodel->item(ipport_row, 3)->setText(QObject::tr("连接成功"));
                    ipportmodel->item(ipport_row, 4)->setText(QString("%1").arg(1));

                    //! 下载重传 <--begin-->
                    bool isSuccess = false;
                    for(int loop_step = 0; loop_step < 3; loop_step ++){
                        int down_row = 0;
                        int down_rowcount = 1;
                        ipportmodel->item(ipport_row, 3)->setText(QObject::tr("传输中[%1]...").arg(loop_step));
                        //! 一次传输 <--begin-->
                        while(down_row < down_rowcount){
                            printer.writedirect(ElectronicScale_KAIFENG::clearPLU());
                            if(printer.waitForReadyRead(30000,isbreak)){
                                QByteArray ret = printer.tcpSocket->readAll();
                                if(ret.startsWith(ElectronicScale_KAIFENG::clearPLU_back())){
                                    down_row++;
                                    QMetaObject::invokeMethod(receiver,
                                                              "updateProgressbar",
                                                              Qt::QueuedConnection,
                                                              Q_ARG(QString, QString("%1/%2").arg(down_row).arg(down_rowcount)),
                                                              Q_ARG(int, ipport_row)
                                                              );
                                    if(down_row >= down_rowcount){
                                        isSuccess = true;
                                        break;
                                    }
                                    continue;
                                }
                            }
                            break;
                        }
                        //! 一次传输 <--end-->
                        if(isSuccess)break;
                    }
                    //! 下载重传 <--end-->

                    if(isSuccess){
                        ipportmodel->item(ipport_row, 3)->setText(QObject::tr("传输成功"));
                    } else {
                        ipportmodel->item(ipport_row, 3)->setText(QObject::tr("传输失败"));
                    }
                } else {
                    ipportmodel->item(ipport_row, 3)->setText(QObject::tr("连接失败"));
                }
                printer.closeconnect(30000, isbreak);
                ipport_row++;
            }
            //! 按照ip port  下载数据 <--end-->
        }
    }
        break;//switch
    }

    //! 修改pushbutton的 文本
    // ((barscale_downdialog*)receiver)->updateButtonText(tr("完成"));
    QMetaObject::invokeMethod(receiver,
                              "updateButtonText",
                              Qt::QueuedConnection,
                              Q_ARG(QString, QObject::tr("完成"))
                              );
    //! 修改pushbutton的 文本
    //! 结束计时
    QMetaObject::invokeMethod(receiver,
                              "stopTimer",
                              Qt::QueuedConnection
                              );
    //! 结束计时
}

barscale_main_dialog::barscale_main_dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::barscale_main_dialog),
    down_isbreak(false)
{
    ui->setupUi(this);setWindowFlags(Qt::Dialog|Qt::FramelessWindowHint);this->setLayoutDirection(CommonHeader::com_widget_dir);
    {
        QSqlDatabase db_sql=QSqlDatabase::addDatabase("QSQLITE", "barscale_main_dialog");
        db_sql.setDatabaseName(CommonHeader::com_root_dirpath+"/db_barscale_main_dialog.db");
    }
    QSqlQuery query(QSqlDatabase::database("barscale_main_dialog"));
    qDebug() << "create barscale_main_dialog_table"<< query.exec("create table if not exists barscale_main_dialog_table ("
                                                                 "vch_plu char(20) primary key,"
                                                                 "vch_name char(200),"
                                                                 "vch_no char(20), "
                                                                 "vch_price char(20) ,"

                                                                 "vch_weighttype char(20),"
                                                                 "vch_validdate char(20),"
                                                                 "vch_flag char(20)"
                                                                 ")"
                                                                 );
    downmodel0=new QSqlTableModel(0, QSqlDatabase::database("barscale_main_dialog"));
    downmodel0->setTable("barscale_main_dialog_table");
    downmodel0->setEditStrategy(QSqlTableModel::OnManualSubmit);
    downmodel0->select();
    downmodel0->setHeaderData(downmodel0->fieldIndex("vch_plu"), Qt::Horizontal, tr("PLU编号"));
    downmodel0->setHeaderData(downmodel0->fieldIndex("vch_name"), Qt::Horizontal, tr("商品名"));
    downmodel0->setHeaderData(downmodel0->fieldIndex("vch_no"), Qt::Horizontal, tr("商品代码"));
    downmodel0->setHeaderData(downmodel0->fieldIndex("vch_price"), Qt::Horizontal, tr("单价(/kg)"));

    downmodel0->setHeaderData(downmodel0->fieldIndex("vch_weighttype"), Qt::Horizontal, tr("称重方式"));
    downmodel0->setHeaderData(downmodel0->fieldIndex("vch_validdate"), Qt::Horizontal, tr("有效日期"));
    downmodel0->setHeaderData(downmodel0->fieldIndex("vch_flag"), Qt::Horizontal, tr("标识号"));

    ui->tableView->setModel(downmodel0);
    ui->tableView->setEditTriggers(QTableView::DoubleClicked|QTableView::SelectedClicked);
    ui->tableView->setItemDelegate(new barscale_main_delegate(this, ui->tableView));

    downmodel=new QStandardItemModel(this);//下载用
    downmodel->setHorizontalHeaderLabels(QStringList()
                                         << tr("PLU编号") << tr("商品代码") << tr("单价") << tr("称重方式")
                                         << tr("特殊信息1") << tr("特殊信息2") << tr("特殊信息3") << tr("有效日期")
                                         << tr("店名号") << tr("部门号") << tr("数字代号") << tr("皮重")
                                         << tr("标签号") << tr("是否打折") << tr("1时间段开始") << tr("1时间段结束")

                                         << tr("1时间段折扣") << tr("2时间段开始") << tr("2时间段结束") << tr("2时间段折扣")
                                         << tr("3时间段开始") << tr("3时间段结束") << tr("3时间段折扣") << tr("4时间段开始")
                                         << tr("4时间段结束") << tr("4时间段折扣") << tr("商品名1") << tr("商品名2")
                                         << tr("商品名3"));

    downdialog=new barscale_downdialog(this);
    connect(downdialog->finishButton(),SIGNAL(clicked()),SLOT(downdialog_exit()));

    QSettings conf(CommonHeader::com_ini_filepath, QSettings::IniFormat);
    ui->comboBox_barcode_scaletypes->addItems(CommonHeader::creat_barcode_scaletypes());
    ui->comboBox_barcode_scaletypes->setCurrentIndex(ui->comboBox_barcode_scaletypes->findText(conf.value("barscale/scaletype").toString()));
    update_barcodetype(ui->comboBox_barcode_scaletypes->currentText());
    ui->comboBox_barcode_scalebarcodetype->setCurrentIndex(ui->comboBox_barcode_scalebarcodetype->findText(conf.value("barscale/scalebartype").toString()));
    ui->spinBox_barcode_scaleflag->setValue(conf.value("barscale/scaleflag", 21).toInt());
    update_scaleflag(QString::number(ui->spinBox_barcode_scaleflag->value()));
    connect(ui->comboBox_barcode_scaletypes,SIGNAL(currentIndexChanged(QString)),this,SLOT(save_barscale()));
    connect(ui->comboBox_barcode_scaletypes,SIGNAL(currentIndexChanged(QString)),this,SLOT(popdjianinfo()));
    connect(ui->comboBox_barcode_scalebarcodetype,SIGNAL(currentIndexChanged(QString)),this,SLOT(save_barscale()));
    connect(ui->spinBox_barcode_scaleflag,SIGNAL(valueChanged(QString)),this,SLOT(save_barscale()));
    connect(ui->spinBox_barcode_scaleflag,SIGNAL(valueChanged(QString)),this,SLOT(update_scaleflag(QString)));
    connect(ui->comboBox_barcode_scaletypes,SIGNAL(currentIndexChanged(QString)),this,SLOT(update_barcodetype(QString)));
    connect(ui->pushButton_add,SIGNAL(clicked()),this,SLOT(togoodadd()));
}

barscale_main_dialog::~barscale_main_dialog()
{
    delete ui;
    delete downmodel0;
    QSqlDatabase::removeDatabase("barscale_main_dialog");
}

void barscale_main_dialog::add_down_infolist(const barscale_main_dialog::BARSCAEL_ITEMS &items)
{
    items_list.append(items);
}

void barscale_main_dialog::set_shop_name(const QString &shop_name)
{
    ui->lineEdit_shopname->setText(shop_name);
}

QString barscale_main_dialog::shop_name()
{
    return ui->lineEdit_shopname->text();
}

void barscale_main_dialog::inputfinished()
{

    //! init_start_transaction
    QStringList vch_nolist0,vch_nolist1;
    QSqlDatabase::database("barscale_main_dialog").transaction();
    QSqlQuery query0(QSqlDatabase::database("barscale_main_dialog"));

    query0.exec("select vch_no from barscale_main_dialog_table");
    while(query0.next())vch_nolist0.append(query0.record().value(0).toString());//本地数据库
    foreach(BARSCAEL_ITEMS items, items_list)vch_nolist1.append(items.dishno);//数据库
    //若是数据库不存在，本地也需要删除
    for(int index=0;index<vch_nolist0.count();){
        if(vch_nolist1.contains(vch_nolist0[index])){
            index++;
        } else {
            query0.exec(QString("delete from barscale_main_dialog_table where vch_no='%1' ").arg(vch_nolist0[index]));
            vch_nolist0.removeAt(index);
        }
    }

    //对比数据库，本地数据库对照insert update
    foreach(BARSCAEL_ITEMS items, items_list){
        query0.exec(QString("select * from barscale_main_dialog_table where vch_plu='%1' ").arg(items.pluno));
        if(query0.next()){
            query0.exec(QString("update barscale_main_dialog_table set vch_name='%1', vch_no='%2', vch_price='%3' where vch_plu='%4' ")
                        .arg(items.dishname)
                        .arg(items.dishno)
                        .arg(items.price)
                        .arg(items.pluno)
                        );
        }/* else {
            if(query0.exec(QString("insert into  barscale_main_dialog_table values('%1','%2','%3','%4','%5','%6','%7')")
                           .arg(items.pluno)
                           .arg(items.dishname)
                           .arg(items.dishno)
                           .arg(items.price)

                           .arg("")
                           .arg("")
                           .arg("")
                           )){
            }
        }*/
    }

    QString text_sqlerr=query0.lastError().text().trimmed();
    //! init_end_transaction
    if(text_sqlerr.isEmpty()){
        QSqlDatabase::database("barscale_main_dialog").commit();
        downmodel0->select();
    } else {
        qDebug() << text_sqlerr;
        uinormal_message::warning(this, tr("条码秤"), tr("条码秤错误\n")+text_sqlerr);
        QSqlDatabase::database("barscale_main_dialog").rollback();
        return;
    }
}

void barscale_main_dialog::update_downmodel()
{
    downmodel->removeRows(0, downmodel->rowCount());
    for(int row=0,rowcount=downmodel0->rowCount();row<rowcount;row++){
        QList<QStandardItem *> items;
        creatItem(items, QStringList()
                  << downmodel0->record(row).value(0).toString()
                  << downmodel0->record(row).value(2).toString()
                  << QString::number((int)(downmodel0->record(row).value(3).toFloat() * 100))
                  << downmodel0->record(row).value(4).toString());
        creatItem(items, QStringList() << QString("00") << QString("00") << QString("00")
                  << downmodel0->record(row).value(5).toString());
        creatItem(items, QStringList()
                  << downmodel0->record(row).value(6).toString()
                  << QString("00") << QString("0000000000000")  << QString("000000"));
        creatItem(items, QStringList() << QString("00") << QString("00") << QString("00")  << QString("00"));

        creatItem(items, QStringList() << QString("00") << QString("00") << QString("00")  << QString("00"));
        creatItem(items, QStringList() << QString("00") << QString("00") << QString("00")  << QString("00"));
        creatItem(items, QStringList() << QString("00") << QString("00")
                  << downmodel0->record(row).value(1).toString()
                  << ui->lineEdit_shopname->text());// Document::getOption("店名"));

        creatItem(items, QStringList() << QString(tr("测试三")));
        downmodel->appendRow(items);
    }
}

void barscale_main_dialog::creatItem(QList<QStandardItem *> &items, const QStringList &list)
{
    foreach(QString s, list)
        items <<new QStandardItem(s);
}

bool barscale_main_dialog::brefore_getscaletypes()
{
    //1
    QSettings conf(CommonHeader::com_ini_filepath, QSettings::IniFormat);
    QString curscaletype=conf.value("barscale/scaletype").toString();
    labelprint_goodinfo_add_dialog dialog(this);
    dialog.ui->label_2->setText(tr("选择条码秤"));
    dialog.ui->label->setText(tr("如果没有找到想要条码秤，请到上一窗口修改秤的型号"));
    dialog.model->setHorizontalHeaderLabels(QStringList() << tr("秤号") << tr("秤名") << tr("地址/端口") << tr("状态"));
    QByteArray ba = conf.value("barscale/netinfo").toByteArray();
    QDataStream out(&ba, QIODevice::ReadOnly);
    while(!out.atEnd()){
        QList<QStandardItem *> items;
        QString p1, p2, p3, p4;
        out >> p1 >> p2 >> p3 >> p4;
        items <<new QStandardItem(p1);
        items <<new QStandardItem(p2);
        items <<new QStandardItem(p3);
        items <<new QStandardItem(p4);
        if(p4==tr("使用")
                && p2==curscaletype)
            dialog.model->appendRow(items);
    }

    if(QDialog::Accepted!=dialog.exec()){
        return false;
    }
    //2
    update_downmodel();
    //2.5
    downdialog->model->removeRows(0, downdialog->model->rowCount());
    QModelIndexList indexlist=dialog.ui->tableView->selectionModel()->selectedIndexes();
    QSet<int> row_set;
    foreach(QModelIndex index, indexlist)row_set.insert(index.row());
    foreach(int row, row_set){
        downdialog->model->appendRow(QList<QStandardItem*>()
                                     <<new QStandardItem(dialog.model->item(row, 0)->text())
                                     <<new QStandardItem(dialog.model->item(row, 2)->text().split(":").value(0))
                                     <<new QStandardItem(dialog.model->item(row, 2)->text().split(":").value(1))
                                     <<new QStandardItem(tr("未连接")) //状态
                                     <<new QStandardItem("0") //计数
                                     <<new QStandardItem("0/1") //百分比
                                     );
    }
    if(downdialog->model->rowCount()<=0){
        return false;
    }
    downdialog->ui->timeEdit->setTime(QTime(0,0,0));
    downdialog->show();
    return true;
    //    downdialog->showUpdate();
}

void barscale_main_dialog::goodsadd(const QList<BARSCAEL_ITEMS> items)
{
    ///重载步骤：
    /// 1调用其他窗口，生成对应的itemlist，
    /// 2调用goodsadd(itemslist);
    //1
    int row;
    foreach(BARSCAEL_ITEMS item, items){
        downmodel0->insertRow(row=downmodel0->rowCount());
        downmodel0->setData(downmodel0->index(row, 0), row+1);
        downmodel0->setData(downmodel0->index(row, 1), item.dishname);
        downmodel0->setData(downmodel0->index(row, 2), item.dishno);
        downmodel0->setData(downmodel0->index(row, 3), item.price);
    }
    for(int row=0,rowcount=downmodel0->rowCount();row<rowcount;row++){
        if(downmodel0->index(row, 0).data().toInt()!=(row+1))
            downmodel0->setData(downmodel0->index(row, 0), row+1);
    }
    //2
    downmodel0->submitAll();
}

void barscale_main_dialog::on_pushButton_clicked()
{
    QSettings conf(CommonHeader::com_ini_filepath, QSettings::IniFormat);
    if(!brefore_getscaletypes()) return;
    QFuture<void> future = QtConcurrent::run(ScaleDownInfo,
                      downdialog,
                      QStringList()<<"down"<<conf.value("barscale/scaletype").toString(),
                      &down_isbreak,
                      downmodel,
                      downdialog->model);
    future.waitForFinished();
}

void barscale_main_dialog::on_pushButton_2_clicked()
{
    QSettings conf(CommonHeader::com_ini_filepath, QSettings::IniFormat);
    if(!brefore_getscaletypes()) return;
    QFuture<void> future = QtConcurrent::run(ScaleDownInfo,
                      downdialog,
                      QStringList()<<"clear"<<conf.value("barscale/scaletype").toString(),
                      &down_isbreak,
                      downmodel,
                      downdialog->model);
    future.waitForFinished();
}

void barscale_main_dialog::on_pushButton_3_clicked()
{
    QSettings conf(CommonHeader::com_ini_filepath, QSettings::IniFormat);
    if(!brefore_getscaletypes()) return;
    QFuture<void> future = QtConcurrent::run(ScaleDownInfo,
                      downdialog,
                      QStringList()<<"setlabel"<<conf.value("barscale/scaletype").toString(),
                      &down_isbreak,
                      downmodel,
                      downdialog->model);
    future.waitForFinished();
}

void barscale_main_dialog::on_pushButton_4_clicked()
{
    //外部输入
    barscale_net_set_dialog dialog(this);
    dialog.exec();
}

void barscale_main_dialog::on_pushButton_5_clicked()
{
    downmodel0->submitAll();
    this->reject();
}

void barscale_main_dialog::downdialog_exit()
{
    if(qobject_cast<QPushButton *>(downdialog->finishButton())->text() == tr("完成")){
        downdialog->accept();
    } else {
        down_isbreak =  true;
    }
}

void barscale_main_dialog::update_barcodetype(const QString scaletype)
{
    ui->comboBox_barcode_scalebarcodetype->clear();
    ui->comboBox_barcode_scalebarcodetype->addItems(CommonHeader::creat_barcode_scalebarcodetype(scaletype));
}

void barscale_main_dialog::save_barscale()
{
    QSettings conf(CommonHeader::com_ini_filepath, QSettings::IniFormat);
    conf.setValue("barscale/scaletype", ui->comboBox_barcode_scaletypes->currentText());
    conf.setValue("barscale/scalebartype", ui->comboBox_barcode_scalebarcodetype->currentText());
    conf.setValue("barscale/scaleflag",ui->spinBox_barcode_scaleflag->value());
}

void barscale_main_dialog::popdjianinfo()
{
    QSettings conf(CommonHeader::com_ini_filepath, QSettings::IniFormat);
    //只是为了get_barcode_scaleflag而用,外设内部不使用该函数,以及该变量
    CommonHeader::barcode_scaleflag=QString::number(ui->spinBox_barcode_scaleflag->value());
    if(CommonHeader::creat_barcode_scaletypes().indexOf(ui->comboBox_barcode_scaletypes->currentText())==3
            &&conf.value("barscale/djianpisnotop", false).toBool()==false){
        QDialog dialog(this);
        Ui_popinfo_cannotpop_dialog dui;
        dui.setupUi(&dialog);
        dui.checkBox->setChecked(conf.value("barscale/djianpisnotop").toBool());
        if(QDialog::Accepted==dialog.exec()){
            conf.setValue("barscale/djianpisnotop",dui.checkBox->isChecked());
        }
    }
}

void barscale_main_dialog::update_scaleflag(const QString &flag)
{
    for(int row=0,rowcount=downmodel0->rowCount();row<rowcount;row++){
        downmodel0->setData(downmodel0->index(row, 6), flag);
    }
}

barscale_main_delegate::barscale_main_delegate(barscale_main_dialog *parent, QObject *parent1):
    QItemDelegate(parent1),
    _parent(parent)
{
}

QWidget *barscale_main_delegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    int c = index.column();
    if(c == 4){
        QComboBox *editor = new QComboBox(parent);
        editor->addItems(CommonHeader::creat_barcode_scale_weigettype(QSettings(CommonHeader::com_ini_filepath, QSettings::IniFormat).value("barscale/scaletype").toString()).values());
        return editor;
    } else {
        return QItemDelegate::createEditor(parent, option, index);
    }
}

void barscale_main_delegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    int c = index.column();
    if(c == 4){
        QString value = index.model()->data(index, Qt::EditRole).toString();

        QComboBox *spinBox = static_cast<QComboBox*>(editor);
        spinBox->setCurrentIndex(spinBox->findText(CommonHeader::creat_barcode_scale_weigettype(QSettings(CommonHeader::com_ini_filepath, QSettings::IniFormat).value("barscale/scaletype").toString())[value.toInt()]));
    } else {
        QItemDelegate::setEditorData(editor, index);
    }
}

void barscale_main_delegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    int c = index.column();
    if(c == 4){
        QComboBox *spinBox = static_cast<QComboBox*>(editor);
        model->setData(index, CommonHeader::creat_barcode_scale_weigettype(QSettings(CommonHeader::com_ini_filepath, QSettings::IniFormat).value("barscale/scaletype").toString()).key(spinBox->currentText()), Qt::EditRole);
    } else {
        QItemDelegate::setModelData(editor, model, index);
    }
}

void barscale_main_delegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    int c = index.column();
    if(c == 4){
        editor->setGeometry(option.rect);
    } else {
        QItemDelegate::updateEditorGeometry(editor, option, index);
    }
}

void barscale_main_delegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    int  c = index.column();
    if(c != 4){
        QItemDelegate::paint(painter,option,index);
    } else {
        int key = index.data(Qt::EditRole).toInt();
        QRect x = option.rect;
        int flag = Qt::AlignCenter | Qt::TextWordWrap;
        painter->drawText(x, flag, CommonHeader::creat_barcode_scale_weigettype(QSettings(CommonHeader::com_ini_filepath, QSettings::IniFormat).value("barscale/scaletype").toString())[key]);//);
    }
}

void barscale_main_dialog::on_pushButton_export_clicked()
{
    downmodel0->submitAll();
    //获取路径
    QFileDialog dialog(this);dialog.setWindowFlags(Qt::Dialog|Qt::FramelessWindowHint);
    dialog.setWindowTitle(tr("导出"));
    dialog.setDirectory(CommonHeader::com_root_dirpath);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.selectFile("barscale_main.csv");
    QStringList files;
    if(dialog.exec()){
        files=dialog.selectedFiles();
    } else {
        return;
    }
    QString filename=files.value(0);
    QFile file(filename);
    if(!file.open(QFile::WriteOnly)) {
        qDebug() << file.errorString();
        uinormal_message::warning(this, tr("条码秤"), tr("条码秤plu导出失败:")+file.errorString());
        return;
    }
    QString errtext;
    QString table_column_name;
    QString table_column_value;
    QString tablename=downmodel0->tableName();
    qlonglong columncount;
    qlonglong rowcount;
    QSqlQuery query(QSqlDatabase::database("barscale_main_dialog"));
    while(1){///while

        //!--------------begin0--------------
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
        }
        //!--------------end0--------------

        //!--------------begin1--------------
        if(!query.exec("select count(*) from "+tablename)) {
            errtext= query.lastError().text();
            break;
        }
        query.next();
        rowcount=query.record().value(0).toLongLong();
        //!--------------end1--------------

        //!--------------begin2--------------
        if(!query.exec("select * from "+tablename)){
            errtext= query.lastError().text();
            break;
        }
        for(int r=0;r<rowcount;r++){
            query.next();
            for(int c=0; c < columncount; c++){
                table_column_value = query.record().value(c).toString();
                qDebug() << "table_column_value" << columncount<<c<<table_column_value;

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
        }
        //!--------------end2--------------
        //!--------------begin3--------------
        //!--------------end3--------------
        //!--------------begin4--------------
        //!--------------end4--------------
        //!--------------begin5--------------
        //!--------------end5--------------
        break;
    }///while

    file.close();
    uinormal_message::warning(this, tr("条码秤"), tr("导出成功"));
}

void barscale_main_dialog::on_pushButton_import_clicked()//导入
{
    //获取路径
    QFileDialog dialog(this);dialog.setWindowFlags(Qt::Dialog|Qt::FramelessWindowHint);
    dialog.setWindowTitle(tr("导出"));
    dialog.setDirectory(CommonHeader::com_root_dirpath);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setFileMode(QFileDialog::ExistingFile);
    QStringList files;
    if(dialog.exec()){
        files=dialog.selectedFiles();
    } else {
        return;
    }
    QString filename=files.value(0);
    QFile file(filename);
    if(!file.open(QFile::ReadOnly)) {
        qDebug() << file.errorString();
        uinormal_message::warning(this, tr("条码秤"), tr("条码秤plu导入失败:")+file.errorString());
        return;
    }

    QSqlQuery query(QSqlDatabase::database("barscale_main_dialog"));
    QByteArray file_read;
    QStringList tablename_dawei;
    QString sql_language;
    QString tablename=downmodel0->tableName();
    QString errtext;
    //! <--transaction begin-->
    QSqlDatabase::database("barscale_main_dialog").transaction();
    while(1){///while
        //!--------------begin0--------------
        if(!query.exec("delete from "+tablename)){
            errtext=query.lastError().text();
            break;
        }
        //!--------------end0--------------
        //!--------------begin1--------------
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
        //!--------------end1--------------
        //!--------------begin2--------------
        //!--------------end2--------------
        //!--------------begin3--------------
        //!--------------end3--------------
        break;
    }///while

    //! <--transaction begin-->
    QSqlDatabase::database("barscale_main_dialog").commit();
    file.close();
    downmodel0->select();
    uinormal_message::warning(this, tr("条码秤"), tr("导入成功"));
}

void barscale_main_dialog::togoodadd()
{
    //1
    QList<BARSCAEL_ITEMS> items;
    labelprint_goodinfo_add_dialog dialog(this);
    dialog.model->setHorizontalHeaderLabels(QStringList() << tr("商品名") << tr("商品代码") << tr("价格"));
    foreach(BARSCAEL_ITEMS item, items_list)
        dialog.model->appendRow(QList<QStandardItem*>()
                                <<new QStandardItem(item.dishname)
                                <<new QStandardItem(item.dishno)
                                <<new QStandardItem(item.price)
                                );
    //2
    if(QDialog::Accepted==dialog.exec()){
        QModelIndexList indexlist=dialog.ui->tableView->selectionModel()->selectedRows();
        foreach(QModelIndex index, indexlist){
            items.append(BARSCAEL_ITEMS(
                             ""/*plu为空*/,
                             dialog.model->item(index.row(), 0)->text(),
                             dialog.model->item(index.row(), 1)->text(),
                             dialog.model->item(index.row(), 2)->text())
                         );
        }
    }
    goodsadd(items);
}

void barscale_main_dialog::on_pushButton_remove_clicked()
{
    //1
    QModelIndexList indexlist=ui->tableView->selectionModel()->selectedIndexes();
    QList<int> index_list;
    foreach(QModelIndex index, indexlist){
        index_list.append(index.row());
    }
    //2
    qSort(index_list.begin(), index_list.end(), qGreater<int>());
    foreach(int index, index_list){
        downmodel0->removeRow(index);
        downmodel0->submit();
    }
    downmodel0->submitAll();
    //3
    for(int row=0,rowcount=downmodel0->rowCount();row<rowcount;row++){
        downmodel0->setData(downmodel0->index(row, 0), row+1);
    }
    downmodel0->submitAll();
}
