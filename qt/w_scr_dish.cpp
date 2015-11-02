#include "w_scr_dish.h"
#include "n_func.h"
#include "backheader.h"
#include "trans_u2g.h"
#include "qtsoap.h"
#include <QSettings>
#include <QSqlError>
#include <QSqlRecord>

w_scr_dish::w_scr_dish(QObject *parent) :
    QObject(parent)
{
}


void w_scr_dish::setprintsize(blockIns &b, const QString &fontlen, const QString &content)
{
    //若是下划线的
    if(content.startsWith(n_func::gs_print_line)){b.append(backheader::printer->drawText("",content));return;}

    //否则遵循放大
    QString fontsize=fontlen;
    if(fontsize.count()!=2){
        fontsize="00";
    }
    b.append(backheader::printer->drawText(
                 QByteArray("1D21").append(fontsize),
                 content)
             );
    b.append(backheader::printer->drawText("1D2100"));
}

void w_scr_dish::setprintsize(QString &content, int len, const QString &flag)
{
    //    pin dan shu jia jin zhe

    if(flag=="pin")setprintsize(content, len,n_func::f_get_sysparm("bp_dishname_left").toInt());
    if(flag=="dan")setprintsize(content, len,n_func::f_get_sysparm("bp_unit_left").toInt());
    if(flag=="shu")setprintsize(content, len,n_func::f_get_sysparm("bp_num_left").toInt());
    if(flag=="jia")setprintsize(content, len,n_func::f_get_sysparm("bp_price_left").toInt());
    if(flag=="jin")setprintsize(content, len,n_func::f_get_sysparm("bp_amount_left").toInt());
    if(flag=="zhe")setprintsize(content, len,n_func::f_get_sysparm("bp_discount_left").toInt());

}

void w_scr_dish::setprintsize(QString &content, int len, Qt::AlignmentFlag flag)
{
    switch(flag){
    case Qt::AlignLeft:
        content.append(QString(len-size_g_of_u(content), ' '));
        break;
    case Qt::AlignRight:
        content.prepend(QString(len-size_g_of_u(content), ' '));
        break;
    }
}

void w_scr_dish::setprintsize(QString &content, int len, int leftremain)
{
    content.append(QString(len-size_g_of_u(content), ' '));
    content.prepend(QString(leftremain, ' '));
}

QString w_scr_dish::TECH_TRANS_NJGJ_sql(const QString &txdate_yyyymmdd, const QString &txtime_hhmmss, const QString &tillid, const QString &txdocno, const QString &cashier, const QString &salesman, const QString &qty, const QString &netamount, const QList<w_scr_dish::TECH_TRANS_NJGJ_tendercode> &tendercodes)
{
    QSqlQuery query;
    QString sqlerr;
    qlonglong id;
    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    query.exec("select count(*) from cey_web_data_collection");
    query.next();
    id=query.record().value(0).toLongLong();
    query.prepare("insert into cey_web_data_collection values(?, ?,?,?,?,?,?, ?,?,?,?,?, ?,?,?,?,?,?,?,?, ?,?,?,  ?,?)");
    query.addBindValue(id);
    query.addBindValue(conf.value("system_setting/njgj_ip").toString());
    query.addBindValue(conf.value("system_setting/njgj_port").toString());
    query.addBindValue("http://tempurl.org/postsalescreate");
    query.addBindValue("postsalescreate");
    query.addBindValue("http://tempurl.org");
    query.addBindValue(conf.value("system_setting/njgj_web").toString());

    query.addBindValue(conf.value("system_setting/njgj_username").toString());
    query.addBindValue(conf.value("system_setting/njgj_password").toString());
    query.addBindValue("SALESDATA");
    query.addBindValue("332");
    query.addBindValue("V332M");

    query.addBindValue(txdate_yyyymmdd);
    query.addBindValue(txtime_hhmmss);
    query.addBindValue(conf.value("system_setting/njgj_mallid").toString());
    query.addBindValue(conf.value("system_setting/njgj_storecode").toString());
    query.addBindValue(tillid);
    query.addBindValue(txdocno);
    query.addBindValue(cashier);
    query.addBindValue(salesman);

    query.addBindValue(conf.value("system_setting/njgj_itemcode").toString());
    query.addBindValue(qty);
    query.addBindValue(netamount);

    query.addBindValue("N");
    query.addBindValue("ready");

    if(query.exec()){
        foreach(TECH_TRANS_NJGJ_tendercode t, tendercodes){
            query.prepare("insert into cey_web_data_collection_tendercode values(?,?,?)");
            query.addBindValue(id);
            query.addBindValue(t.tendercode);
            query.addBindValue(t.baseamount);
            if(!query.exec())break;
        }
    }
    if(sqlerr.isEmpty())sqlerr=query.lastError().text().trimmed();
    if(sqlerr.isEmpty()){
        w_scr_dish_TECH_TRANS_thread *t=new w_scr_dish_TECH_TRANS_thread;
        QObject().moveToThread(t);
        t->start();
    }

    qDebug() << sqlerr;
    return sqlerr;
}

void w_scr_dish::TECH_TRANS_NJGJ_postsalescreate()
{
    {
        QSqlDatabase db=QSqlDatabase::cloneDatabase(QSqlDatabase::database(), "TECH_TRANS_NJGJ_postsalescreate");
        if(db.open()){
            QSqlQuery query(db),query2(db),query3(db);
            QEventLoop eloop;
            QtSoapHttpTransport http;
            QTimer timer;
            QtSoapStruct *soapstruct=0;
            QtSoapMessage request;
            qlonglong id;
            connect(&timer,SIGNAL(timeout()),&eloop,SLOT(quit()));
            connect(&http, SIGNAL(responseReady()), &eloop,SLOT(quit()));
            timer.setSingleShot(true);

            query.exec("select * from cey_web_data_collection where ch_uploadflag='N' ");
            while(query.next()){//!while(query.next()){
                http.setHost(query.record().value("service_ip").toString(),query.record().value("service_port").toInt());
                http.setAction(query.record().value("service_action").toString());
                soapstruct=request.setMethod(query.record().value("service_method_name").toString(),
                                             query.record().value("service_method_uri").toString());
                id=query.record().value("id").toLongLong();
                int loopcount=3;

                while(loopcount--){//! while(loopcount--)
                    {//!request
                        QtSoapStruct *astr_request=0;
                        soapstruct->insert(astr_request=new QtSoapStruct(QtSoapQName("astr_request", "")));
                        {//!astr_request
                            QtSoapStruct *header=0;
                            astr_request->insert(header=new QtSoapStruct(QtSoapQName("header", "")));
                            header->insert(new QtSoapSimpleType(QtSoapQName("username", ""), query.record().value("username").toString()));
                            header->insert(new QtSoapSimpleType(QtSoapQName("password", ""), query.record().value("password").toString()));
                            header->insert(new QtSoapSimpleType(QtSoapQName("messagetype", ""), query.record().value("messagetype").toString()));
                            header->insert(new QtSoapSimpleType(QtSoapQName("messageid", ""), query.record().value("messageid").toString()));
                            header->insert(new QtSoapSimpleType(QtSoapQName("version", ""), query.record().value("version").toString()));

                            QtSoapStruct *salestotal=0;
                            astr_request->insert(salestotal=new QtSoapStruct(QtSoapQName("salestotal", "")));
                            salestotal->insert(new QtSoapSimpleType(QtSoapQName("txdate_yyyymmdd", ""), query.record().value("txdate_yyyymmdd").toDateTime().toString("yyyyMMdd")));
                            salestotal->insert(new QtSoapSimpleType(QtSoapQName("txtime_hhmmss", ""), query.record().value("txtime_hhmmss").toTime().toString("hhmmss")));
                            //                    salestotal->insert(new QtSoapSimpleType(QtSoapQName("mallid", ""), query.record().value("mallid").toString()));
                            salestotal->insert(new QtSoapSimpleType(QtSoapQName("storecode", ""), query.record().value("storecode").toString()));
                            salestotal->insert(new QtSoapSimpleType(QtSoapQName("tillid", ""), query.record().value("tillid").toString()));
                            salestotal->insert(new QtSoapSimpleType(QtSoapQName("txdocno", ""), query.record().value("txdocno").toString()));
                            salestotal->insert(new QtSoapSimpleType(QtSoapQName("cashier", ""), query.record().value("cashier").toString()));
                            salestotal->insert(new QtSoapSimpleType(QtSoapQName("salesman", ""), query.record().value("salesman").toString()));

                            QtSoapStruct *salesitems=0;
                            astr_request->insert(salesitems=new QtSoapStruct(QtSoapQName("salesitems", "")));
                            {//!salesitems:也支持多条item上传,但是目前只是支持商品汇总后上传,itemcode是唯一的
                                QtSoapStruct *salesitem=0;
                                salesitems->insert(salesitem=new QtSoapStruct(QtSoapQName("salesitem", "")));
                                salesitem->insert(new QtSoapSimpleType(QtSoapQName("itemcode", ""), query.record().value("itemcode").toString()));
                                salesitem->insert(new QtSoapSimpleType(QtSoapQName("qty", ""), query.record().value("qty").toDouble()));
                                salesitem->insert(new QtSoapSimpleType(QtSoapQName("netamount", ""), query.record().value("netamount").toDouble()));
                            }//!salesitems

                            QtSoapStruct *salestenders=0;
                            astr_request->insert(salestenders=new QtSoapStruct(QtSoapQName("salestenders", "")));
                            {//!salestenders:可以有多条如现金付款,国内银行卡付款
                                query3.exec(QString("select * from cey_web_data_collection_tendercode where id='%1' ").arg(id));
                                while(query3.next()){
                                    QtSoapStruct *salestender=0;
                                    salestenders->insert(salestender=new QtSoapStruct(QtSoapQName("salestender", "")));
                                    salestender->insert(new QtSoapSimpleType(QtSoapQName("tendercode", ""), query3.record().value("tendercode").toString()));
                                    salestender->insert(new QtSoapSimpleType(QtSoapQName("baseamount", ""), query3.record().value("baseamount").toDouble()));
                                }
                            }//!salestenders
                        }//!astr_request
                    }//!request
                    qDebug() << request.toXmlString(2) << query.record().value("service_submit_path").toString();
                    http.submitRequest(request, query.record().value("service_submit_path").toString());

                    timer.start(30*1000);//30s的超时eply*)),&eloop,SLOT(quit()));缩短时间,方便及时退出
                    eloop.exec(QEventLoop::ExcludeUserInputEvents);
                    if(timer.isActive()){
                        const QtSoapMessage &message = http.getResponse();
                        if (message.isFault()) {
                            qDebug("Error: %s", message.faultString().value().toString().toLatin1().constData());
                        } else {
                            qDebug() << "read from http is---- :\n"<<message.toXmlString();
                            if(query2.exec(QString("update cey_web_data_collection set ch_uploadflag='Y',ch_uploadmemo='success' where id='%1' ").arg(id))){
                                qDebug() << query2.lastQuery();
                                break;
                            } else {
                                qDebug() << query2.lastError().text();
                            }
                        }
                    }

                    if(loopcount==0){
                        query2.exec(QString("update cey_web_data_collection set ch_uploadmemo='%2' where id='%1' ").arg(id).arg(http.networkReply()->errorString()));
                    }
                }//!while(loopcount--)
                qDebug() << "------------------------" << id;
            }//!while(query.next())

        }
    }QSqlDatabase::removeDatabase("TECH_TRANS_NJGJ_postsalescreate");
}

//! 线程对数据库操作需要,克隆数据库
//! 线程锁保证,两次付款不会冲突(a成功,b失败的情况)
//! QThread随app结束而结束,所以付款负责插入操作,以免丢失
QMutex w_scr_dish_TECH_TRANS_thread::mutex;
w_scr_dish_TECH_TRANS_thread::~w_scr_dish_TECH_TRANS_thread()
{
}

void w_scr_dish_TECH_TRANS_thread::run()
{
    if(mutex.tryLock(1)){
        w_scr_dish::TECH_TRANS_NJGJ_postsalescreate();
        mutex.unlock();
    }
}
