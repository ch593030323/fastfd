#include "wei_xin_service_help.h"
#include "qsettings.h"
#include "public_sql.h"
#include <QDebug>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QByteArray>
#include <QNetworkReply>
#include"lds_messagebox.h"
#include "w_sys_manage_cloudsync_transfer.h"

wei_xin_service_help::wei_xin_service_help(w_sys_manage_cloudsync_basedataset *ui)
{
    myUi=ui;
    hostName = ui->ui->yun_server_ip->text();
    orgCode = ui->ui->orgCode->text();
    cloudUser = ui->ui->cloudUser->text();
    cloudPass = ui->ui->cloudPass->text();
    headInfo.insert("machineCode","");
    headInfo.insert("orgCode",orgCode);
    headInfo.insert("userName",cloudUser);
    headInfo.insert("password",cloudPass);
}

wei_xin_service_help::~wei_xin_service_help()
{

}

void wei_xin_service_help::checkUser()
{
    QString param;
    w_sys_manage_cloudsync_transfer transfer;
    QPair<cJSON*, QString> ret_json_err=transfer.sendPost2("http://" + hostName
                                                           + "/CateSystem/weixin-message/checkUser-rest", param, headInfo);
    if(ret_json_err.first==NULL){
        lds_messagebox::warning(0, tr("消息提示"),tr("账号校验失败!\n")+ret_json_err.second);
    } else {
        lds_messagebox::warning(0, tr("消息提示"),tr("账号校验成功!"));
        cJSON_Delete(ret_json_err.first);
    }
}
