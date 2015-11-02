#ifndef W_SCR_DISH_H
#define W_SCR_DISH_H

#include <QObject>
#include "public_mainprinter.h"
#include <QString>
#include<QSqlDatabase>

class w_scr_dish : public QObject
{
    Q_OBJECT
public:
    explicit w_scr_dish(QObject *parent = 0);

    static void setprintsize(blockIns &b, const QString & fontlen, const QString &content);
    static void setprintsize(QString &content, int len, const QString &flag);
    static void setprintsize(QString &content, int len, Qt::AlignmentFlag flag);
    static void setprintsize(QString &content, int len, int leftremain);

    //数据采集
    /**
POST /esPOS_WebService/nv_espos_webservice.asmx HTTP/1.1
Host: 59.41.59.58
Content-Type: application/soap+xml; charset=utf-8
Content-Length: length

<?xml version="1.0" encoding="utf-8"?>
<soap12:Envelope xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:xsd="http://www.w3.org/2001/XMLSchema" xmlns:soap12="http://www.w3.org/2003/05/soap-envelope">
  <soap12:Body>
    <postsalescreate xmlns="http://tempurl.org">
      <astr_request>
        <header>
          <username>100283</username>
          <password>100283</password>
          <messagetype>SALESDATA</messagetype>
          <messageid>332</messageid>
          <version>V332M</version>
        </header>
        <salestotal>
          <txdate_yyyymmdd>20140925</txdate_yyyymmdd>
          <txtime_hhmmss>173912</txtime_hhmmss>
          <storecode>100283</storecode>
          <tillid>01</tillid>
          <txdocno>20</txdocno>
          <cashier>1022</cashier>
          <salesman>1022</salesman>
        </salestotal>
        <salesitems>
          <salesitem>
            <itemcode>100262</itemcode>
            <qty>3.00</qty>
            <netamount>60.00</netamount>
          </salesitem>
        </salesitems>
        <salestenders>
          <salestender>
            <tendercode>CH</tendercode>
            <baseamount>25.00</baseamount>
          </salestender>
          <salestender>
            <tendercode>CI</tendercode>
            <baseamount>50.00</baseamount>
          </salestender>
        </salestenders>
      </astr_request>
    </postsalescreate>
  </soap12:Body>
</soap12:Envelope>

tendercode 付款方式代号解释:CH----现金   CI----国内银行   CO----国外银行   OT-----前台付款*/
    struct TECH_TRANS_NJGJ_tendercode{
        TECH_TRANS_NJGJ_tendercode(){}
        TECH_TRANS_NJGJ_tendercode(const QString &_tendercode, const QString &_baseamount):
            tendercode(_tendercode),
            baseamount(_baseamount){}
        QString tendercode;//付款方式 ;付款方式代号解释:CH----现金   CI----国内银行   CO----国外银行   OT-----前台付款
        QString baseamount;//本位币金额 ;忽略本位金额,已净金额为主
    };

    static QString TECH_TRANS_NJGJ_sql(  /*astr_request/header*/
                                         //            const  QString &username,//can_void_N
                                         //            const  QString &password,//can_void_N
                                         //            const  QString &messagetype,//can_void_N; fixedvalue:SALESDATA
                                         //            const  QString &messageid,//can_void_N; fixedvalue:332
                                         //            const  QString &version,//can_void_N;fixedvalue:V332M

                                         /*astr_request/salestotal*/
                                         const  QString &txdate_yyyymmdd,//交易日期
                                         const  QString &txtime_hhmmss,//交易时间
                                         //            const  QString &storecode,//店铺号
                                         const  QString &tillid,//收银机号
                                         const  QString &txdocno,//销售单号
                                         const  QString &cashier,//收银员编号
                                         const  QString &salesman,//销售员

                                         /*astr_request/salesitems/salesitem*/
                                         //            const  QString &itemcode,//can_void_N
                                         const  QString &qty,//数量
                                         const  QString &netamount,//净金额

                                         /*astr_request/salestenders*/
                                         const QList<TECH_TRANS_NJGJ_tendercode> &tendercodes
                                         );
    static void TECH_TRANS_NJGJ_postsalescreate();//post 销售数据
};

#include <QThread>
class w_scr_dish_TECH_TRANS_thread:public QThread
{
    Q_OBJECT
public:
    w_scr_dish_TECH_TRANS_thread(QObject *parent = 0) : QThread(parent),isbreak(false){}
    ~w_scr_dish_TECH_TRANS_thread();
    bool isbreak;
private:
    static QMutex mutex;
protected:
    virtual void run();
};

#endif // W_SCR_DISH_H
