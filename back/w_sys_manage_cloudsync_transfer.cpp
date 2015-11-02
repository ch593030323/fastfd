#include "w_sys_manage_cloudsync_transfer.h"
#include <QtDebug>
#include <QTimer>
#include "w_sys_manage_cloudsync.h"

w_sys_manage_cloudsync_transfer::w_sys_manage_cloudsync_transfer(QObject *parent) :
    QObject(parent)
{
}

QPair<cJSON*, QString> w_sys_manage_cloudsync_transfer::sendPost2(QString url, QString param, QMap<QString, QString> &headInfo)
{
    QPair<cJSON*, QString> ret_json_err;
    ret_json_err.first=NULL;
    QEventLoop eloop;
    QByteArray data;
    data.append(param);

    QNetworkRequest request;
    request.setUrl(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json;charset=utf-8"));
    request.setHeader(QNetworkRequest::ContentLengthHeader, data.length());

    if(!headInfo.isEmpty()){
        QMap<QString, QString>::const_iterator i;
        for( i=headInfo.constBegin(); i!=headInfo.constEnd(); ++i){
            request.setRawHeader(QByteArray().append(i.key()),QByteArray().append(i.value()));
        }
    }

    QNetworkAccessManager networkAccessManager;
    QTimer timer;
    timer.setSingleShot(true);
    connect(&timer,SIGNAL(timeout()),&eloop,SLOT(quit()));
    connect(&networkAccessManager,SIGNAL(finished(QNetworkReply*)),&eloop,SLOT(quit()));
    QNetworkReply *reply = networkAccessManager.post(request,data);
    timer.start(10*1000);//10s的超时eply*)),&eloop,SLOT(quit()));
    eloop.exec(QEventLoop::ExcludeUserInputEvents);
    if(timer.isActive()
            &&reply->error()==QNetworkReply::NoError){
        ret_json_err.first=cJSON_Parse(reply->readAll());
        ret_json_err.second="";
    } else {
        reply->abort();
        ret_json_err.first=0;
        cJSON *retparse=cJSON_Parse(reply->readAll());
        cJSON *retmessage=0;
        cJSON *retcode=0;
        w_sys_manage_cloudsync::print_yun(retparse);
        if(retparse){
            retmessage=cJSON_GetObjectItem(retparse, "message");
            retcode=cJSON_GetObjectItem(retparse, "code");
        }
        if(retcode)
            ret_json_err.second=retcode->valuestring;
        if(retmessage)
            ret_json_err.second=retmessage->valuestring;
        if(ret_json_err.second.isEmpty()){
            ret_json_err.second=reply->errorString();
        }
    }
    return ret_json_err;
}

QPair<cJSON*, QString> w_sys_manage_cloudsync_transfer::sendGet(QString url, QMap<QString, QString> &headInfo)
{
    QPair<cJSON*, QString> ret_json_err;
    ret_json_err.first=NULL;
    QNetworkRequest request;
    QEventLoop eloop;
    request.setUrl(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json;charset=utf-8"));

    if(!headInfo.isEmpty()){
        QMap<QString, QString>::const_iterator i;
        for( i=headInfo.constBegin(); i!=headInfo.constEnd(); ++i){
            request.setRawHeader(QByteArray().append(i.key()),QByteArray().append(i.value()));
        }
    }
    QNetworkAccessManager networkAccessManager;
    QTimer timer;
    timer.setSingleShot(true);
    connect(&timer,SIGNAL(timeout()),&eloop,SLOT(quit()));
    connect(&networkAccessManager,SIGNAL(finished(QNetworkReply*)),&eloop,SLOT(quit()));
    QNetworkReply *reply = networkAccessManager.get(request);
    timer.start(10*1000);//10s的超时
    eloop.exec(QEventLoop::ExcludeUserInputEvents);
    if(timer.isActive()
            &&reply->error()==QNetworkReply::NoError){
        ret_json_err.first=cJSON_Parse(reply->readAll());
        ret_json_err.second="";
    } else {
        reply->abort();
        ret_json_err.first=0;
        cJSON *retparse=cJSON_Parse(reply->readAll());
        cJSON *retmessage=0;
        cJSON *retcode=0;
        w_sys_manage_cloudsync::print_yun(retparse);
        if(retparse){
            retmessage=cJSON_GetObjectItem(retparse, "message");
            retcode=cJSON_GetObjectItem(retparse, "code");
        }
        if(retcode)
            ret_json_err.second=retcode->valuestring;
        if(retmessage)
            ret_json_err.second=retmessage->valuestring;
        if(ret_json_err.second.isEmpty()){
            ret_json_err.second=reply->errorString();
        }
    }
    return ret_json_err;
}
