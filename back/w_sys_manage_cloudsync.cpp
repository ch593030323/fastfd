#include "w_sys_manage_cloudsync.h"
//u云同步
#include "ui_w_sys_manage_cloudsync_dialog.h"
#include "w_sys_manage_cloudsync_basedataset.h"
#include "backheader.h"
#include "w_sys_manage_cloudsync_basedataup.h"
#include "w_sys_manage_cloudsync_basedatadown.h"
#include "w_sys_manage_cloudsync_flowdataup.h"
#include "lds_messagebox.h"
#include <QSqlQuery>
#include <QSqlError>
#include "n_func.h"
#include <QSqlRecord>


QMap<QString, QString> w_sys_manage_cloudsync::yun_MemberParameter;
QMap<QString, QString> w_sys_manage_cloudsync::yun_MemberReadcard;
QMap<QString, QString> w_sys_manage_cloudsync::yun_MemberType;
cJSON *w_sys_manage_cloudsync::json_yun_parameter=NULL;
cJSON *w_sys_manage_cloudsync::json_yun_readcard=NULL;
cJSON *w_sys_manage_cloudsync::json_yun_membertype=NULL;
bool w_sys_manage_cloudsync::this_action_is_successed=false;
bool w_sys_manage_cloudsync::is_match_local_time=false;

/**
 * 构造函数
 */
w_sys_manage_cloudsync::w_sys_manage_cloudsync(QWidget *parent) :
    QDialog(parent),
    ui(new Ui_w_sys_manage_cloudsync_Dialog)
{
    ui->setupUi(this);
    // 绑定相应的事件信号槽
    connect(ui->pushButton_close,SIGNAL(clicked()),this,SLOT(slot_close()));
    connect(ui->pushButton_cloudData_set,SIGNAL(clicked()),this,SLOT(slot_baseData_set()));
    connect(ui->pushButton_upBaseData,SIGNAL(clicked()),this,SLOT(slot_up_baseData()));
    connect(ui->pushButton_downBaseData,SIGNAL(clicked()),this,SLOT(slot_down_baseData()));
    connect(ui->pushButton_upFlowData,SIGNAL(clicked()),this,SLOT(slot_up_flowData()));
}

w_sys_manage_cloudsync::~w_sys_manage_cloudsync()
{
    delete ui;
}

/**
 * 关闭事件
 */
void w_sys_manage_cloudsync::slot_close()
{
    this->close();
}

/**
 * 云同步基本数据设置事件
 */
void w_sys_manage_cloudsync::slot_baseData_set()
{
    w_sys_manage_cloudsync_basedataset dialog(this);
    backheader::static_dialogexec(dialog,tr("云同步数据设置"));
}

/**
 * 上传基础数据事件
 */
void w_sys_manage_cloudsync::slot_up_baseData()
{
    w_sys_manage_cloudsync_basedataup dialog(this);
    backheader::static_dialogexec(dialog,tr("上传基础数据"));
}

/**
 * 下载基础数据事件
 */
void w_sys_manage_cloudsync::slot_down_baseData()
{
    w_sys_manage_cloudsync_basedatadown dialog(this);
    backheader::static_dialogexec(dialog,tr("下载基础数据"));
}

/**
 * 上传流水数据事件
 */
void w_sys_manage_cloudsync::slot_up_flowData()
{
    w_sys_manage_cloudsync_flowdataup dialog(this);
    backheader::static_dialogexec(dialog,tr("上传流水数据"));
}

cJSON * w_sys_manage_cloudsync::get_yun_httpp(const QString &http_key, const QString &param, QMap<QString, QString> &headInfo, const QString &msgtitle, bool ispop)
{
    QPair<cJSON*, QString> ret_json_err;
    w_sys_manage_cloudsync_transfer transfer;
    //! 获取map
    ret_json_err = transfer.sendPost2(http_key, param, headInfo);
    if(ret_json_err.first==NULL){
        w_sys_manage_cloudsync::this_action_is_successed=false;
        QString msgerr=msgtitle+tr("失败\n")+ret_json_err.second;
        if(ispop)
            lds_messagebox::warning(0, tr("消息提示"),msgerr);
        else
            qDebug() << msgerr;
    } else {
        w_sys_manage_cloudsync::this_action_is_successed=true;
    }
    return ret_json_err.first;
}

cJSON * w_sys_manage_cloudsync::get_yun_httpg(const QString &http_key, QMap<QString, QString> &headInfo, const QString &msgtitle, bool ispop)
{
    QPair<cJSON*, QString> ret_json_err;
    w_sys_manage_cloudsync_transfer transfer;
    //! 获取map
    ret_json_err = transfer.sendGet(http_key, headInfo);
    if(ret_json_err.first==NULL){
        w_sys_manage_cloudsync::this_action_is_successed=false;
        QString msgerr=msgtitle+tr("失败\n")+ret_json_err.second;
        if(ispop)
            lds_messagebox::warning(0, tr("消息提示"),msgerr, true);
        else
            qDebug() << msgerr;
    } else {
        w_sys_manage_cloudsync::this_action_is_successed=true;
    }
    return ret_json_err.first;
}

QString w_sys_manage_cloudsync::print_yun(cJSON *print)
{
    if(print==NULL) {
        qDebug() << "print is null";
        return "";
    }

    char* arrayDate = cJSON_Print(print);
    QString param=QString(arrayDate);
    free(arrayDate);
    qDebug() << "[read back msg:]"<<param;
    return param;
}

void w_sys_manage_cloudsync::printalljson_child(cJSON *print)
{
    qDebug() << "printalljson_child---------------------------------------";
    cJSON *tmp=print;
    while(tmp){qDebug() << tmp->string;tmp=tmp->child;}
}
/*  "vip_ic_machine": "明华RF",
"member_bank_print": "1",
"discountCard": "0",多出的
"local_member_select": "0",
"findMemberCard": "1",多出的
"m_deposit_negative": "0",
"vip_card_type": "磁卡",
"rechargeableCard": "1",多出的
"deposit_pay_all": "0",
"input_memberno": "0",
"m_fetch_base": "1",
"member_ATT_mode": "0",
"m_member_pay": "1",
"vip_ic_baud": "0",
"vip_ic_port": "USB",
"m_card_paydiscount": "0",
"discount_view": "1",
"vip_ic_password": "",
"m_max_deposit": "30,000.00"*/

QString w_sys_manage_cloudsync::get_yun_member_parameter(const QString &key, QVariant::Type keytype, const QString &def)
{
    QString ret;
    if(json_yun_parameter == NULL){
        QMap<QString, QString> headInfo;
        QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
        headInfo.insert("machineCode","");
        headInfo.insert("orgCode",conf.value("w_sys_manage_cloudsync_basedataset/orgCode").toString());
        headInfo.insert("userName",conf.value("w_sys_manage_cloudsync_basedataset/cloudUser").toString());
        headInfo.insert("password",conf.value("w_sys_manage_cloudsync_basedataset/cloudPass").toString());
        QString httpkey="http://" + conf.value("w_sys_manage_cloudsync_basedataset/yun_server_ip").toString()+
                "/CateSystem/cvs/member/parameter-rest";
        json_yun_parameter=get_yun_httpg(httpkey, headInfo,tr("获取云会员参数"));
    }

    if(json_yun_parameter == NULL) return "";

    if(cJSON_GetArraySize(json_yun_parameter) > 0){
        if(keytype==QVariant::Double){
            ret = QString::number(cJSON_GetObjectItem(json_yun_parameter, key.toLocal8Bit().data())->valuedouble);
        } else if(keytype==QVariant::Int){
            ret = QString::number(cJSON_GetObjectItem(json_yun_parameter, key.toLocal8Bit().data())->valueint);
        } else if(keytype==QVariant::String){
            ret = cJSON_GetObjectItem(json_yun_parameter, key.toLocal8Bit().data())->valuestring;
        }
    }

    print_yun(json_yun_parameter);

    return ret;
}

/*
  "vch_memberno": "Y10004",
  "vch_member": "10004",
  "ch_typeno": "11",
  "dt_birthday": null,
  "vch_handtel": "13324569874",
  "vch_tel": "",

  "dt_limit": "2015-07-31 12:00:00",
  "ch_state": "1",
  "num_limit": null,
  "ch_cardflag": "Y",
  "vch_cardno": "10004",
  "int_basedata": 5,

  "vch_typename": "优惠卡1",
  "ch_favourable": "2",
  "ch_pricekind": null,
  "int_discount": 10,
  "ch_calc": "N",
  "ch_calc_type": "1",

  "int_calc_unit": 1,
  "ch_area_flag": "0",
  "int_backcash_rate": 0,
  "int_card_baseamount": 50,
  "password": null,
  "num_amount": 200,

  "num_hangamount": 0,
  "num_point": 0,
  "vch_operID": null,
  "dt_operdate": null,
  "ch_attribute": "",
  "ch_payno": "",

  "ch_paymodeno": "",
  "num_pay": 0,
  "ch_balanceno": "",

  "int_checkflow": ""
*/
QString w_sys_manage_cloudsync::get_yun_member_readcard(const QString cardNo, const QString &operId, const QString &key, QVariant::Type keytype, const QString &def)
{
    QString ret;
    qDebug() << "--------" << cardNo <<
                (json_yun_readcard!=NULL?QString(cJSON_GetObjectItem(json_yun_readcard, "vch_memberno")->valuestring):"null");
    if(json_yun_readcard==NULL
            || (json_yun_readcard!=NULL && QString(cJSON_GetObjectItem(json_yun_readcard, "vch_memberno")->valuestring) != cardNo)){
        cJSON *pItem = cJSON_CreateObject();
        cJSON_AddStringToObject(pItem,"cardNo", cardNo.toLocal8Bit().data());
        cJSON_AddStringToObject(pItem,"operId", operId.toLocal8Bit().data());
        char* arrayDate = cJSON_Print(pItem);
        QString param=QString(arrayDate);
        cJSON_Delete(pItem);
        free(arrayDate);

        //获取map
        QMap<QString, QString> headInfo;
        QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
        headInfo.insert("machineCode","");
        headInfo.insert("orgCode",conf.value("w_sys_manage_cloudsync_basedataset/orgCode").toString());
        headInfo.insert("userName",conf.value("w_sys_manage_cloudsync_basedataset/cloudUser").toString());
        headInfo.insert("password",conf.value("w_sys_manage_cloudsync_basedataset/cloudPass").toString());
        QString http_key="http://" + conf.value("w_sys_manage_cloudsync_basedataset/yun_server_ip").toString()+
                "/CateSystem/cvs/member/dukFuk-rest";
        //"/CateSystem/cvs/member/dukFuk-rest"
        json_yun_readcard =  get_yun_httpp(http_key, param, headInfo,tr("云会员读卡"));
    }

    if(json_yun_readcard==NULL) return "";

    if(cJSON_GetArraySize(json_yun_readcard) > 0){
        if(keytype==QVariant::Double){
            ret = QString::number(cJSON_GetObjectItem(json_yun_readcard, key.toLocal8Bit().data())->valuedouble);
        } else if(keytype==QVariant::Int){
            ret = QString::number(cJSON_GetObjectItem(json_yun_readcard, key.toLocal8Bit().data())->valueint);
        } else if(keytype==QVariant::String){
            ret = cJSON_GetObjectItem(json_yun_readcard, key.toLocal8Bit().data())->valuestring;
        }
    }

    if(key=="num_point")qDebug() << print_yun(json_yun_readcard);
    return ret;
}

//"message": "ok",
//"maxOrderNo": "1"
QString w_sys_manage_cloudsync::get_yun_member_maxorderno(const QString &key, QVariant::Type keytype, const QString &def)
{
    QString ret;
    QString param;
    cJSON *root=NULL;
    //获取map
    QMap<QString, QString> headInfo;
    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    headInfo.insert("machineCode","");
    headInfo.insert("orgCode",conf.value("w_sys_manage_cloudsync_basedataset/orgCode").toString());
    headInfo.insert("userName",conf.value("w_sys_manage_cloudsync_basedataset/cloudUser").toString());
    headInfo.insert("password",conf.value("w_sys_manage_cloudsync_basedataset/cloudPass").toString());
    headInfo.insert("flag","K");
    QString http_key="http://" + conf.value("w_sys_manage_cloudsync_basedataset/yun_server_ip").toString()+
            "/CateSystem/cvs/member/maxOrderNo-rest";
    root = get_yun_httpp(http_key, param, headInfo, tr("获取最大订单号"));
    if(root==NULL) return ret;
    if(cJSON_GetArraySize(root) > 0){
        if(keytype==QVariant::Double){
            ret = QString::number(cJSON_GetObjectItem(root, key.toLocal8Bit().data())->valuedouble);
        } else if(keytype==QVariant::Int){
            ret = QString::number(cJSON_GetObjectItem(root, key.toLocal8Bit().data())->valueint);
        } else if(keytype==QVariant::String){
            ret = cJSON_GetObjectItem(root, key.toLocal8Bit().data())->valuestring;
        }
    }
    cJSON_Delete(root);
    return ret;
}
//"message": "ok",
//"maxOrderNo": "1"
QString w_sys_manage_cloudsync::get_yun_member_maxmasterno(const QString &key, QVariant::Type keytype, const QString &def)
{
    QString ret;
    QString param;
    cJSON *root=NULL;
    //获取map
    QMap<QString, QString> headInfo;
    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    headInfo.insert("machineCode","");
    headInfo.insert("orgCode",conf.value("w_sys_manage_cloudsync_basedataset/orgCode").toString());
    headInfo.insert("userName",conf.value("w_sys_manage_cloudsync_basedataset/cloudUser").toString());
    headInfo.insert("password",conf.value("w_sys_manage_cloudsync_basedataset/cloudPass").toString());
    headInfo.insert("flag","F");
    QString http_key="http://" + conf.value("w_sys_manage_cloudsync_basedataset/yun_server_ip").toString()+
            "/CateSystem/cvs/member/maxMasterNo-rest";
    root = get_yun_httpp(http_key, param, headInfo,tr("获取最大支付号"));
    if(root==NULL) return ret;
    if(cJSON_GetArraySize(root) > 0){
        if(keytype==QVariant::Double){
            ret = QString::number(cJSON_GetObjectItem(root, key.toLocal8Bit().data())->valuedouble);
        } else if(keytype==QVariant::Int){
            ret = QString::number(cJSON_GetObjectItem(root, key.toLocal8Bit().data())->valueint);
        } else if(keytype==QVariant::String){
            ret = cJSON_GetObjectItem(root, key.toLocal8Bit().data())->valuestring;
        }
    }
    cJSON_Delete(root);
    return ret;
}

/*
  "cvsMemberType": {
    "sn": "5",
    "chTypeno": "11",
    "vchTypename": "优惠卡1",
    "chAttribute": "2",
    "chFavourable": "2",
    "chPricekind": null,
    "intDiscount": 10,
    "chProjectno": null,
    "chCalc": "N",
    "chCalcType": "1",
    "intCalcUnit": 1,
    "chAreaFlag": "0",
    "intBackcashRate": 0,
    "intCardBaseamount": 50,
    "orgCode": "000000060001",
    "date": "2015-07-02 18:27:22",
    "chAttributeStr": "充值卡",
    "chFavourableStr": "固定折扣",
    "idStr": "5",
    "operate": null
},
"message":"ok"*/
QString w_sys_manage_cloudsync::get_yun_member_membertype(const QString &typeNo, const QString &key, QVariant::Type keytype, const QString &def)
{
    QString ret;
    if(json_yun_membertype==NULL
            || (json_yun_membertype!=NULL && typeNo!=QString(cJSON_GetObjectItem(cJSON_GetObjectItem(json_yun_membertype, "cvsMemberType"), "chTypeno")->valuestring))
            ){
        cJSON *pItem = cJSON_CreateObject();
        cJSON_AddStringToObject(pItem,"typeNo", typeNo.toLocal8Bit().data());
        char* arrayDate = cJSON_Print(pItem);
        QString param=QString(arrayDate);
        cJSON_Delete(pItem);
        free(arrayDate);
        //获取map
        QMap<QString, QString> headInfo;
        QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
        headInfo.insert("machineCode","");
        headInfo.insert("orgCode",conf.value("w_sys_manage_cloudsync_basedataset/orgCode").toString());
        headInfo.insert("userName",conf.value("w_sys_manage_cloudsync_basedataset/cloudUser").toString());
        headInfo.insert("password",conf.value("w_sys_manage_cloudsync_basedataset/cloudPass").toString());
        QString http_key="http://" + conf.value("w_sys_manage_cloudsync_basedataset/yun_server_ip").toString()+
                "/CateSystem/cvs/member/getMemberType-rest";
        json_yun_membertype = get_yun_httpp(http_key, param, headInfo,tr("获取云会员类型"));//, def, "cvsMemberType"
    }

    if(json_yun_membertype==NULL) return "";

    cJSON *subroot=cJSON_GetObjectItem(json_yun_membertype, "cvsMemberType");
    if(subroot==NULL) return ret;
    if(cJSON_GetArraySize(subroot) > 0){
        if(keytype==QVariant::Double){
            ret = QString::number(cJSON_GetObjectItem(subroot, key.toLocal8Bit().data())->valuedouble);
        } else if(keytype==QVariant::Int){
            ret = QString::number(cJSON_GetObjectItem(subroot, key.toLocal8Bit().data())->valueint);
        } else if(keytype==QVariant::String){
            ret = cJSON_GetObjectItem(subroot, key.toLocal8Bit().data())->valuestring;
        }
    }
    qDebug() << "json_yun_membertype" << print_yun(json_yun_membertype);
    return ret;
}

QString w_sys_manage_cloudsync::upload_yun_pay_info(member_up_yun &muy)
{/*    QList<orderMaster> orderMasterList;
    QList<checkOutMaster> checkOutMasterList;
    QList<memberPointList> memberPointListList;
    QList<memberPay> memberPayList;
    QList<memberCuramount> memberCuramountList;

    QList<tableRecordsConsumption> tableRecordsConsumptionList;
    QList<orderDish> orderDishList;
    QList<checkOutDetail> checkOutDetailList;
    QList<memberDeposit> memberDepositList;*/
    cJSON *root = cJSON_CreateObject();
    cJSON *subarray;
    foreach(orderMaster p, muy.orderMasterList)muy.make_JSON_Array(p, root);
    foreach(checkOutMaster p, muy.checkOutMasterList)muy.make_JSON_Array(p, root);

    if(!muy.memberPointListList.isEmpty())
    {
        cJSON_AddItemToObject(root, "memberPointList", subarray=cJSON_CreateArray());
        foreach(memberPointList p, muy.memberPointListList)muy.make_JSON_Array(p, subarray);
    }

    if(!muy.memberPayList.isEmpty())
    {
        cJSON_AddItemToObject(root, "memberPay", subarray=cJSON_CreateArray());
        foreach(memberPay p, muy.memberPayList)muy.make_JSON_Array(p, subarray);
    }

    if(!muy.memberCuramountList.isEmpty())
    {
        cJSON_AddItemToObject(root, "memberCuramount", subarray=cJSON_CreateArray());
        foreach(memberCuramount p, muy.memberCuramountList)muy.make_JSON_Array(p, subarray);
    }

    foreach(tableRecordsConsumption p, muy.tableRecordsConsumptionList)muy.make_JSON_Array(p, root);

    if(!muy.orderDishList.isEmpty())
    {
        cJSON_AddItemToObject(root, "orderDish", subarray=cJSON_CreateArray());
        foreach(orderDish p, muy.orderDishList)muy.make_JSON_Array(p, subarray);
    }

    if(!muy.checkOutDetailList.isEmpty())
    {
        cJSON_AddItemToObject(root, "checkOutDetail", subarray=cJSON_CreateArray());
        foreach(checkOutDetail p, muy.checkOutDetailList)muy.make_JSON_Array(p, subarray);
    }

    foreach(memberDeposit p, muy.memberDepositList)muy.make_JSON_Array(p, root);

    QString param = print_yun(root);
    cJSON_Delete(root);
    root=NULL;
    print_yun(json_yun_parameter);

    //开始向云端插入
    //获取map
    QMap<QString, QString> headInfo;
    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    headInfo.insert("machineCode","");
    headInfo.insert("orgCode",conf.value("w_sys_manage_cloudsync_basedataset/orgCode").toString());
    headInfo.insert("userName",conf.value("w_sys_manage_cloudsync_basedataset/cloudUser").toString());
    headInfo.insert("password",conf.value("w_sys_manage_cloudsync_basedataset/cloudPass").toString());
    headInfo.insert("flag","K");

    //"/CateSystem/cvs/member/maxOrderNo-rest"
    QString key="message";
    QVariant::Type keytype=QVariant::String;
    QString def;
    QString ret;
    QString http_key="http://" + conf.value("w_sys_manage_cloudsync_basedataset/yun_server_ip").toString()+
            "/CateSystem/cvs/member/fuk-rest";
    cJSON *receive_json=NULL;
    receive_json=get_yun_httpp(http_key, param, headInfo,tr("向云端提交云会员信息"));
    if(receive_json==NULL) return ret;
    if(cJSON_GetArraySize(receive_json) > 0){
        ret = cJSON_GetObjectItem(receive_json, "message")->valuestring;
    }
    cJSON_Delete(receive_json);
    return ret;
}

bool w_sys_manage_cloudsync::get_yun_member_recharge(const QString &cardNo, const QString &operId, const QString &chongzje, const QString &shisje, const QString &zhiffs)
{
    QString ret;
    QString param;
    QMap<QString, QString> headInfo;
    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    headInfo.insert("machineCode","");
    headInfo.insert("orgCode",conf.value("w_sys_manage_cloudsync_basedataset/orgCode").toString());
    headInfo.insert("userName",conf.value("w_sys_manage_cloudsync_basedataset/cloudUser").toString());
    headInfo.insert("password",conf.value("w_sys_manage_cloudsync_basedataset/cloudPass").toString());
    QString httpkey="http://" + conf.value("w_sys_manage_cloudsync_basedataset/yun_server_ip").toString()+
            "/CateSystem/cvs/member/chongz-rest";
    {//param
        cJSON *pItem = cJSON_CreateObject();
        cJSON_AddStringToObject(pItem,"cardNo", cardNo.toLocal8Bit().data());
        cJSON_AddStringToObject(pItem,"operId", operId.toLocal8Bit().data());
        cJSON_AddStringToObject(pItem,"chongzje", chongzje.toLocal8Bit().data());
        cJSON_AddStringToObject(pItem,"shisje", shisje.toLocal8Bit().data());
        cJSON_AddStringToObject(pItem,"zhiffs", zhiffs.toLocal8Bit().data());
        char* arrayDate = cJSON_Print(pItem);
        param=QString(arrayDate);
        cJSON_Delete(pItem);
        free(arrayDate);
    }

    qDebug() << param;
    cJSON *receive_json=get_yun_httpp(httpkey, param, headInfo, tr("连锁会员充值"));
    if(receive_json == NULL) return false;
    if(cJSON_GetArraySize(receive_json) > 0){
        ret = cJSON_GetObjectItem(receive_json, "message")->valuestring;
    }
    print_yun(receive_json);
    cJSON_Delete(receive_json);
    return true;
}

bool w_sys_manage_cloudsync::get_yun_member_blankcard(const QString &cardNo, const QString &tuikje, const QString &zhiffs)
{
    QString ret;
    QString param;
    QMap<QString, QString> headInfo;
    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    headInfo.insert("machineCode","");
    headInfo.insert("orgCode",conf.value("w_sys_manage_cloudsync_basedataset/orgCode").toString());
    headInfo.insert("userName",conf.value("w_sys_manage_cloudsync_basedataset/cloudUser").toString());
    headInfo.insert("password",conf.value("w_sys_manage_cloudsync_basedataset/cloudPass").toString());
    QString httpkey="http://" + conf.value("w_sys_manage_cloudsync_basedataset/yun_server_ip").toString()+
            "/CateSystem/cvs/member/tuik-rest";
    {//param
        cJSON *pItem = cJSON_CreateObject();
        cJSON_AddStringToObject(pItem,"cardNo", cardNo.toLocal8Bit().data());
        cJSON_AddStringToObject(pItem,"tuikje", tuikje.toLocal8Bit().data());
        cJSON_AddStringToObject(pItem,"orgCode", conf.value("w_sys_manage_cloudsync_basedataset/orgCode").toString().toLocal8Bit());
        cJSON_AddStringToObject(pItem,"zhiffs", zhiffs.toLocal8Bit().data());
        char* arrayDate = cJSON_Print(pItem);
        param=QString(arrayDate);
        cJSON_Delete(pItem);
        free(arrayDate);
    }

    cJSON *receive_json=get_yun_httpp(httpkey, param, headInfo, tr("退卡"));
    w_sys_manage_cloudsync::print_yun(receive_json);
    if(receive_json == NULL) return false;
    if(cJSON_GetArraySize(receive_json) > 0){
        ret = cJSON_GetObjectItem(receive_json, "message")->valuestring;
    }
    print_yun(receive_json);
    cJSON_Delete(receive_json);
    return true;
}

bool w_sys_manage_cloudsync::get_yun_member_fetch(const QString &cardNo, const QString &operId, const QString &qukje, const QString &zhiffs)
{
    QString ret;
    QString param;
    QMap<QString, QString> headInfo;
    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    headInfo.insert("machineCode","");
    headInfo.insert("orgCode",conf.value("w_sys_manage_cloudsync_basedataset/orgCode").toString());
    headInfo.insert("userName",conf.value("w_sys_manage_cloudsync_basedataset/cloudUser").toString());
    headInfo.insert("password",conf.value("w_sys_manage_cloudsync_basedataset/cloudPass").toString());
    QString httpkey="http://" + conf.value("w_sys_manage_cloudsync_basedataset/yun_server_ip").toString()+
            "/CateSystem/cvs/member/quk-rest";
    {//param
        cJSON *pItem = cJSON_CreateObject();
        cJSON_AddStringToObject(pItem,"cardNo", cardNo.toLocal8Bit().data());
        cJSON_AddStringToObject(pItem,"operId", operId.toLocal8Bit().data());
        cJSON_AddStringToObject(pItem,"qukje", qukje.toLocal8Bit().data());
        cJSON_AddStringToObject(pItem,"zhiffs", zhiffs.toLocal8Bit().data());
        char* arrayDate = cJSON_Print(pItem);
        param=QString(arrayDate);
        cJSON_Delete(pItem);
        free(arrayDate);
    }

    cJSON *receive_json=get_yun_httpp(httpkey, param, headInfo, tr("取款"));
    w_sys_manage_cloudsync::print_yun(receive_json);
    if(receive_json == NULL) return false;
    if(cJSON_GetArraySize(receive_json) > 0){
        ret = cJSON_GetObjectItem(receive_json, "message")->valuestring;
    }
    print_yun(receive_json);
    cJSON_Delete(receive_json);
    return true;
}

w_sys_manage_cloudsync::member_r_f_b_info w_sys_manage_cloudsync::get_yun_member_r_f_b(const QString &operId, const QString &dateStart, const QString &dateEnd)
{
    member_r_f_b_info ret;
    QString param;
    QMap<QString, QString> headInfo;
    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    headInfo.insert("orgCode",conf.value("w_sys_manage_cloudsync_basedataset/orgCode").toString());
    headInfo.insert("userName",conf.value("w_sys_manage_cloudsync_basedataset/cloudUser").toString());
    headInfo.insert("password",conf.value("w_sys_manage_cloudsync_basedataset/cloudPass").toString());
    headInfo.insert("operId",operId);
    headInfo.insert("dateStart",dateStart);
    headInfo.insert("dateEnd",dateEnd);
    QString httpkey="http://" + conf.value("w_sys_manage_cloudsync_basedataset/yun_server_ip").toString()+
            "/CateSystem/cvs/member/getCvsMemberOperaInfo-rest";

    cJSON *receive_json=get_yun_httpp(httpkey, param, headInfo, tr("获取会员交班时操作信息"), false);
    //    w_sys_manage_cloudsync::print_yun(receive_json);
    if(receive_json == NULL) return ret;
    if(cJSON_GetArraySize(receive_json) > 0){
        ret.rechargeAmount=QString::number(cJSON_GetObjectItem(receive_json, "rechargeAmount")->valuedouble);
        ret.realRechargeAmount=QString::number(cJSON_GetObjectItem(receive_json, "realRechargeAmount")->valuedouble);
        ret.withdrawalAmount=QString::number(cJSON_GetObjectItem(receive_json, "withdrawalAmount")->valuedouble);
        ret.backCardAmount=QString::number(cJSON_GetObjectItem(receive_json, "backCardAmount")->valuedouble);
    }
    print_yun(receive_json);
    cJSON_Delete(receive_json);
    return ret;
}

void w_sys_manage_cloudsync::json_free()
{
    if(json_yun_parameter!=NULL){cJSON_Delete(json_yun_parameter);json_yun_parameter=NULL;}
    if(json_yun_readcard!=NULL){cJSON_Delete(json_yun_readcard);json_yun_readcard=NULL;}
    if(json_yun_membertype!=NULL){cJSON_Delete(json_yun_membertype);json_yun_membertype=NULL;}
}

bool w_sys_manage_cloudsync::match_local_time()
{
    QString ret;
    cJSON *root=NULL;
    //获取map
    QMap<QString, QString> headInfo;
    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    headInfo.insert("orgCode",conf.value("w_sys_manage_cloudsync_basedataset/orgCode").toString());
    headInfo.insert("userName",conf.value("w_sys_manage_cloudsync_basedataset/cloudUser").toString());
    headInfo.insert("password",conf.value("w_sys_manage_cloudsync_basedataset/cloudPass").toString());

    QString http_key="http://" + conf.value("w_sys_manage_cloudsync_basedataset/yun_server_ip").toString()+
            "/CateSystem/cvs/member/getServerTime-rest";

    root = get_yun_httpg(http_key, headInfo,tr("本地同步云端时间"));

    if(root==NULL) return false;
    if(cJSON_GetArraySize(root) > 0){
        ret = cJSON_GetObjectItem(root, "time")->valuestring;
    }
    cJSON_Delete(root);
    QString curdatetime=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString curdate=QDateTime::currentDateTime().toString("yyyy-MM-dd");
    int curhour=QTime::currentTime().hour();
    qDebug() << "yun hour"<<QDateTime::fromString(ret, "yyyy-MM-dd hh:mm:ss").time().hour() << "sys hour"<<curhour;
    if(ret.startsWith(curdate)
            && qAbs(QDateTime::fromString(ret, "yyyy-MM-dd hh:mm:ss").time().hour()-curhour) <= 1){
        is_match_local_time=true;
    } else {
        lds_messagebox::warning(0, tr("消息提示"),tr("系统时间%1\n和云时间%2不符;\n请到[主菜单->系统设置->日期时间] 修改")
                                .arg(curdatetime)
                                .arg(ret)
                                );
    }
    return is_match_local_time;
}

bool w_sys_manage_cloudsync::get_weixin_orderinfo(QSqlDatabase &sql_db, const QString &beginTime, const QString &endTime, int downFlag)
{
    bool retflag=true;
    QSqlQuery query(sql_db);

    while(1){//while
        QString param;{//param
            cJSON *pItem = cJSON_CreateObject();
            cJSON_AddStringToObject(pItem,"beginTime", beginTime.toLocal8Bit().data());
            cJSON_AddStringToObject(pItem,"endTime", endTime.toLocal8Bit().data());
            cJSON_AddNumberToObject(pItem,"downFlag", downFlag);
            char* arrayDate = cJSON_Print(pItem);
            param=QString(arrayDate);
            cJSON_Delete(pItem);
            free(arrayDate);
        }

        //订单订桌外卖汇总表
        QMap<QString, QString> headInfo;
        QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
        headInfo.insert("machineCode","");
        headInfo.insert("orgCode",conf.value("w_sys_manage_cloudsync_basedataset/orgCode").toString());
        headInfo.insert("userName",conf.value("w_sys_manage_cloudsync_basedataset/cloudUser").toString());
        headInfo.insert("password",conf.value("w_sys_manage_cloudsync_basedataset/cloudPass").toString());
        QString http_key="http://" + conf.value("w_sys_manage_cloudsync_basedataset/yun_server_ip").toString()+"/CateSystem/weixinOrder-message/getWebOrderInfo-rest";
        cJSON *getWebOrderInfo_json=NULL;//! getWebOrderInfo_json new
        getWebOrderInfo_json=get_yun_httpp(http_key, param, headInfo, tr("索取微信订单号"), false);
        if(getWebOrderInfo_json==NULL) {retflag=false;break;}

        qDebug() << conf.value("w_sys_manage_cloudsync_basedataset/orgCode").toString();
        qDebug() << conf.value("w_sys_manage_cloudsync_basedataset/cloudUser").toString();
        qDebug() << conf.value("w_sys_manage_cloudsync_basedataset/cloudPass").toString();

        qDebug() << param;
        //遍历所有的云端 订单订桌外卖汇总表
        qDebug() << "遍历所有的云端 订单订桌外卖汇总表";
        for(int i =0;i<cJSON_GetArraySize(getWebOrderInfo_json);i++){//for
            cJSON *pItem = cJSON_GetArrayItem(getWebOrderInfo_json,i);
            if("1"==cJSON_GetObjectItem(pItem,"paystate")->valuestring){//1 已支付 0未支付
                QString param;{ //param
                    cJSON_AddStringToObject(pItem,"orderSn", cJSON_GetObjectItem(pItem,"sn")->valuestring);
                    char* arrayDate = cJSON_Print(pItem);
                    param=QString(arrayDate);
                    cJSON_Delete(pItem);
                    free(arrayDate);
                }
                //根据每单外卖汇总表,获取 下载支付流水
                qDebug() << "根据每单外卖汇总表,获取 下载支付流水";
                QString http_key=
                        "http://" + conf.value("w_sys_manage_cloudsync_basedataset/yun_server_ip").toString()
                        +"/CateSystem/weixinOrder-message/getPayForWater-rest";

                cJSON *getPayForWater_json=NULL;//! getPayForWater_json new
                getPayForWater_json=get_yun_httpp(http_key, param, headInfo, tr("下载支付流水"), false);
                if(getPayForWater_json==NULL) {continue;}
                if(cJSON_GetArraySize(getPayForWater_json) > 0){
                    query.prepare("insert into cey_web_pay_for_water (sn,orderSn,mount,note,openid,orgCode,date,upLoadFlag) values(?,?,?,?,?,?,?)");
                    query.addBindValue(cJSON_GetObjectItem(getPayForWater_json,"sn")->valuestring);
                    query.addBindValue(cJSON_GetObjectItem(getPayForWater_json,"orderSn")->valuestring);
                    query.addBindValue(cJSON_GetObjectItem(getPayForWater_json,"mount")->valuestring);
                    query.addBindValue(cJSON_GetObjectItem(getPayForWater_json,"note")->valuestring);
                    query.addBindValue(cJSON_GetObjectItem(getPayForWater_json,"openid")->valuestring);
                    query.addBindValue(cJSON_GetObjectItem(getPayForWater_json,"orgCode")->valuestring);
                    query.addBindValue(cJSON_GetObjectItem(getPayForWater_json,"date")->valuestring);
                    if(query.exec()){

                    } else {
                        qDebug() << "  " << query.lastError().text();
                        //                    lds_messagebox::warning(this, tr("消息提示"),tr("下载支付流水")+query.lastError().text());
                        {retflag=false;break;}
                    }
                }//! cJSON_GetArraySize

                cJSON_Delete(getPayForWater_json);//! getPayForWater_json delete
            }//! //==1

            //更新或插入支付流水信息,若是已有流水表,则更具云端更新对应的状态
            query.exec(QString("select * from cey_web_order_info where sn='%1' ").arg(cJSON_GetObjectItem(pItem,"sn")->valuestring));
            if(query.next()){
                if(query.exec(QString("UPDATE cey_web_order_info  SET orderState = '%1' where sn='%2' ")
                              .arg(cJSON_GetObjectItem(pItem,"orderState")->valuestring)
                              .arg(cJSON_GetObjectItem(pItem,"sn")->valuestring)
                              )){
                } else {
                    qDebug() << "  " << query.lastError().text();
                    //                lds_messagebox::warning(this, tr("消息提示"),tr("外卖汇总表")+query.lastError().text());
                    {continue;}
                }
            } else {
                query.prepare("insert into cey_web_order_info (sn,eatTime,eatPels,tableAreaNo,tel,orderer,sex,note,address,sendTime,payType,payState,orderType,orderState,openid,orgCode,date,refuseReason,downFlag) values(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
                query.addBindValue(cJSON_GetObjectItem(pItem,"sn")->valuestring);
                query.addBindValue(cJSON_GetObjectItem(pItem,"eatTime")->valuestring);
                query.addBindValue(cJSON_GetObjectItem(pItem,"eatPels")->valuedouble);
                //eatTableNo 这个没有insertinto
                query.addBindValue(cJSON_GetObjectItem(pItem,"eatTable")->valuestring);
                query.addBindValue(cJSON_GetObjectItem(pItem,"tel")->valuestring);

                query.addBindValue(cJSON_GetObjectItem(pItem,"orderer")->valuestring);
                query.addBindValue(cJSON_GetObjectItem(pItem,"sex")->valuestring);
                query.addBindValue(cJSON_GetObjectItem(pItem,"note")->valuestring);
                query.addBindValue(cJSON_GetObjectItem(pItem,"address")->valuestring);
                query.addBindValue(cJSON_GetObjectItem(pItem,"sendTime")->valuestring);

                query.addBindValue(cJSON_GetObjectItem(pItem,"payType")->valuestring);
                query.addBindValue(cJSON_GetObjectItem(pItem,"payState")->valuestring);
                query.addBindValue(cJSON_GetObjectItem(pItem,"orderType")->valuestring);
                query.addBindValue(cJSON_GetObjectItem(pItem,"orderState")->valuestring);
                query.addBindValue(cJSON_GetObjectItem(pItem,"openid")->valuestring);

                query.addBindValue(cJSON_GetObjectItem(pItem,"orgCode")->valuestring);
                query.addBindValue(cJSON_GetObjectItem(pItem,"date")->valuestring);
                query.addBindValue(cJSON_GetObjectItem(pItem,"refuseReason")->valuestring);
                query.addBindValue(cJSON_GetObjectItem(pItem,"downFlag")->valueint);
                if(query.exec()){

                } else {
                    qDebug() << "  " << query.lastError().text();
                    //                lds_messagebox::warning(this, tr("消息提示"),tr("外卖汇总表")+query.lastError().text());
                    //                return false;
                    {continue;}
                }
            }
        }//! for

        //根据当前信息,提示
        qDebug() << "pre 根据当前信息,提示";
        query.exec(QString("select * from cey_web_order_info where (orderType='1' or orderType='2') and orderState='01'"));
        if(query.next()){
            emit backheader::printer->sendError(QObject::tr("新的网络订单通知"),QObject::tr("您有新的网络订单通知"));
        }
        query.exec(QString("select * from cey_web_order_info where (orderType='1' or orderType='2') and orderState='02'"));
        if(query.next()){
            emit backheader::printer->sendError(QObject::tr("新的网络退订申请通知"),QObject::tr("您有新的网络退订申请通知"));
        }
        cJSON_Delete(getWebOrderInfo_json);//! getWebOrderInfo_json delete
        break;
    }//! while
    return retflag;
}

bool w_sys_manage_cloudsync::get_weixin_orderdish(QSqlDatabase &sql_db, const QString &beginTime, const QString &endTime, int downFlag)
{
    bool retflag=true;
    //遍历订单菜品明细表,插入本地
    qDebug() << "遍历订单菜品明细表,插入本地";
    QSqlQuery query(sql_db);

    while(1){//while
        QString param;{//param
            cJSON *pItem = cJSON_CreateObject();
            cJSON_AddStringToObject(pItem,"beginTime", beginTime.toLocal8Bit().data());
            cJSON_AddStringToObject(pItem,"endTime", endTime.toLocal8Bit().data());
            cJSON_AddNumberToObject(pItem,"downFlag", downFlag);
            char* arrayDate = cJSON_Print(pItem);
            param=QString(arrayDate);
            cJSON_Delete(pItem);
            free(arrayDate);
        }
        QMap<QString, QString> headInfo;
        QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
        headInfo.insert("machineCode","");
        headInfo.insert("orgCode",conf.value("w_sys_manage_cloudsync_basedataset/orgCode").toString());
        headInfo.insert("userName",conf.value("w_sys_manage_cloudsync_basedataset/cloudUser").toString());
        headInfo.insert("password",conf.value("w_sys_manage_cloudsync_basedataset/cloudPass").toString());
        QString http_key="http://" + conf.value("w_sys_manage_cloudsync_basedataset/yun_server_ip").toString()+"/CateSystem/weixinOrder-message/getWebOrderDish-rest";
        cJSON *getWebOrderDish_json=NULL;//! getWebOrderDish_json new
        getWebOrderDish_json=get_yun_httpp(http_key, param, headInfo, tr("遍历订单菜品明细表"), false);
        if(getWebOrderDish_json==NULL) {break;}

        for(int i =0;i<cJSON_GetArraySize(getWebOrderDish_json);i++){//for
            cJSON *pItem = cJSON_GetArrayItem(getWebOrderDish_json,i);
            if(cJSON_GetArraySize(pItem) > 0){
                query.prepare(QString("insert into cey_web_order_dish (sn,orderSn,dishNo,dishName,number,price,note,orgCode,date,downFlag) values(?,?,?,?,?,?,?,?,?,?)"));
                query.addBindValue(cJSON_GetObjectItem(pItem,"sn")->valuestring);
                query.addBindValue(cJSON_GetObjectItem(pItem,"orderSn")->valuestring);
                query.addBindValue(cJSON_GetObjectItem(pItem,"dishNo")->valuestring);
                query.addBindValue(cJSON_GetObjectItem(pItem,"dishName")->valuestring);
                query.addBindValue(cJSON_GetObjectItem(pItem,"number")->valuedouble);

                query.addBindValue(cJSON_GetObjectItem(pItem,"price")->valuedouble);
                //            query.addBindValue(cJSON_GetObjectItem(pItem,"suitFlag")->valueint);
                query.addBindValue(cJSON_GetObjectItem(pItem,"note")->valuestring);
                query.addBindValue(cJSON_GetObjectItem(pItem,"orgCode")->valuestring);
                query.addBindValue(cJSON_GetObjectItem(pItem,"date")->valuestring);

                query.addBindValue(cJSON_GetObjectItem(pItem,"downFlag")->valueint);
                if(query.exec()){

                } else {
                    qDebug() << "cey_web_order_dish" << query.lastError().text();
                    //                lds_messagebox::warning(this, tr("消息提示"),tr("外卖汇总表")+query.lastError().text());
                    //                return false;
                    {retflag=false;break;}
                }

                //插入本地后下载确认,更新云端状态
                QString param;{//param
                    cJSON *pItem0 = cJSON_CreateObject();
                    cJSON_AddStringToObject(pItem0,"sn", cJSON_GetObjectItem(pItem,"sn")->valuestring);
                    char* arrayDate = cJSON_Print(pItem0);
                    param=QString(arrayDate);
                    cJSON_Delete(pItem0);
                    free(arrayDate);
                }
                QString http_key="http://" + conf.value("w_sys_manage_cloudsync_basedataset/yun_server_ip").toString()+"/CateSystem/weixinOrder-message/confirmWebOrderDish-rest";
                cJSON *confirmWebOrderDish=NULL;//! confirmWebOrderDish new
                confirmWebOrderDish=get_yun_httpp(http_key, param, headInfo, tr("插入本地后下载确认,更新云端状态") ,false);
                if(confirmWebOrderDish==NULL) {continue;}
                cJSON_Delete(confirmWebOrderDish);//! confirmWebOrderDish delete
            }
        }//! for

        cJSON_Delete(getWebOrderDish_json);//! getWebOrderDish_json delete
        break;
    }//! while
    return retflag;
}

bool w_sys_manage_cloudsync::set_weixin_payforwater(QSqlDatabase &sql_db, const QString &beginTime, const QString &endTime, int downFlag)
{
    bool retflag=true;
    QSqlQuery query(sql_db);
    while(1){//while
        QMap<QString, QString> headInfo;
        QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
        headInfo.insert("machineCode","");
        headInfo.insert("orgCode",conf.value("w_sys_manage_cloudsync_basedataset/orgCode").toString());
        headInfo.insert("userName",conf.value("w_sys_manage_cloudsync_basedataset/cloudUser").toString());
        headInfo.insert("password",conf.value("w_sys_manage_cloudsync_basedataset/cloudPass").toString());
        //获取本地菜品明细
        qDebug() << "获取本地菜品明细";
        QString param;bool successflag=true;{//param
            cJSON *array = cJSON_CreateArray();
            cJSON *item=0;

            //!<QSqlDatabase transaction>
            sql_db.transaction();
            query.exec("select * from cey_web_pay_for_water where upLoadFlag=0");
            while(query.next()){
                cJSON_AddNumberToObject(item,"sn", query.record().value("sn").toDouble());
                cJSON_AddNumberToObject(item,"orderSn", query.record().value("orderSn").toDouble());
                cJSON_AddNumberToObject(item,"mount", query.record().value("mount").toDouble());
                cJSON_AddStringToObject(item,"note", query.record().value("note").toString().toLocal8Bit().data());
                cJSON_AddStringToObject(item,"note", query.record().value("note").toString().toLocal8Bit().data());
                cJSON_AddStringToObject(item,"openid", query.record().value("openid").toString().toLocal8Bit().data());
                cJSON_AddStringToObject(item,"orgCode", query.record().value("orgCode").toString().toLocal8Bit().data());
                cJSON_AddStringToObject(item,"date", query.record().value("date").toString().toLocal8Bit().data());
                cJSON_AddNumberToObject(item,"upLoadFlag", query.record().value("upLoadFlag").toLongLong());

                item = cJSON_CreateObject();
                cJSON_AddItemToArray(array, item);

                if(query.exec(QString("update cey_web_pay_for_water set upLoadFlag=1 where sn = '%1' ").arg(query.record().value("sn").toString()))){

                } else {
                    successflag=false;
                    break;
                }
            }
            if(item){
                char* arrayDate = cJSON_Print(array);
                param=QString(arrayDate);
                cJSON_Delete(array);
                free(arrayDate);
            }
        }
        //上传到云端

        //收到确认,更新本地数据库
        if(!successflag) {
            QSqlDatabase::database().rollback();
            lds_messagebox::warning(0, tr("消息提示"),tr("收到确认,更新本地数据库")+tr("失败"));
            {retflag=false;break;}
        }
        QString http_key="http://" + conf.value("w_sys_manage_cloudsync_basedataset/yun_server_ip").toString()+"/CateSystem/weixinOrder-message/setPayForWater-rest";
        cJSON *root=NULL;//! root new
        if(!param.isEmpty())root=get_yun_httpp(http_key, param, headInfo, tr("收到确认,更新本地数据库"), false);
        if(root==NULL) {
            QSqlDatabase::database().rollback();
            {retflag=false;break;}
        }
        QSqlDatabase::database().commit();
        cJSON_Delete(root);//! root delete
        break;
    }//! while
    return retflag;
}

bool w_sys_manage_cloudsync::up_weixin_change_takkeoutstate(const QString &sn, const QString &orderState, const QString &refuseReason)
{
    QMap<QString, QString> headInfo;
    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    headInfo.insert("machineCode","");
    headInfo.insert("orgCode",conf.value("w_sys_manage_cloudsync_basedataset/orgCode").toString());
    headInfo.insert("userName",conf.value("w_sys_manage_cloudsync_basedataset/cloudUser").toString());
    headInfo.insert("password",conf.value("w_sys_manage_cloudsync_basedataset/cloudPass").toString());
    QString param;{//param
        cJSON *pItem = cJSON_CreateObject();
        cJSON_AddStringToObject(pItem,"sn", sn.toLocal8Bit().data());
        cJSON_AddStringToObject(pItem,"orderState", orderState.toLocal8Bit().data());
        cJSON_AddStringToObject(pItem,"refuseReason", refuseReason.toLocal8Bit().data());
        char* arrayDate = cJSON_Print(pItem);
        param=QString(arrayDate);
        cJSON_Delete(pItem);
        free(arrayDate);
    }

    qDebug() << param;
    QString http_key="http://" + conf.value("w_sys_manage_cloudsync_basedataset/yun_server_ip").toString()+"/CateSystem/weixinOrder-message/changeTakeoutState-rest";
    cJSON *changeOrderState_json=NULL;//! changeOrderState_json new
    changeOrderState_json=get_yun_httpp(http_key, param, headInfo, tr("订单状态更新"));
    if(changeOrderState_json==NULL) return false;
    cJSON_Delete(changeOrderState_json);//! changeOrderState_json delete
    return true;
}


bool w_sys_manage_cloudsync::get_yun_member_checkbillnopay(const QString &billno, const QString &memberno)
{
    QString ls_message;
    QString ls_pay;
    cJSON *root=NULL;
    //获取map
    QMap<QString, QString> headInfo;
    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    headInfo.insert("orgCode",conf.value("w_sys_manage_cloudsync_basedataset/orgCode").toString());
    headInfo.insert("userName",conf.value("w_sys_manage_cloudsync_basedataset/cloudUser").toString());
    headInfo.insert("password",conf.value("w_sys_manage_cloudsync_basedataset/cloudPass").toString());
    headInfo.insert("memberNo",memberno);
    headInfo.insert("billNo",billno);

    QString http_key="http://" + conf.value("w_sys_manage_cloudsync_basedataset/yun_server_ip").toString()+
            "/CateSystem/cvs/member/checkBillNoPay-rest";
    root = get_yun_httpg(http_key, headInfo,tr("会员付款核实,是否已经付款"));

    if(root==NULL) return false;
    if(cJSON_GetArraySize(root) > 0){
        ls_pay = cJSON_GetObjectItem(root, "pay")->valuestring;
    }
    cJSON_Delete(root);

    return ls_pay=="true";
}

bool member_up_yun::iscanupload(double v)
{
    return true;
}

bool member_up_yun::iscanupload(float v)
{
    return true;
}

bool member_up_yun::iscanupload(const QString &v)
{
    return !v.isEmpty();
}

void member_up_yun::make_JSON_Array(orderMaster &p, cJSON *root)
{
    cJSON *subroot;
    cJSON_AddItemToObject(root, "orderMaster", subroot=cJSON_CreateObject());
    if(iscanupload(p.omChBillNo))cJSON_AddStringToObject(subroot, "omChBillNo", p.omChBillNo.toLocal8Bit().data());
    if(iscanupload(p.omChState))cJSON_AddStringToObject(subroot, "omChState", p.omChState.toLocal8Bit().data());
    if(iscanupload(p.omVchOperId))cJSON_AddStringToObject(subroot, "omVchOperId", p.omVchOperId.toLocal8Bit().data());
    if(iscanupload(p.omDtOperDate))cJSON_AddStringToObject(subroot, "omDtOperDate", p.omDtOperDate.toLocal8Bit().data());
    if(iscanupload(p.omVchOperId2))cJSON_AddStringToObject(subroot, "omVchOperId2", p.omVchOperId2.toLocal8Bit().data());
    if(iscanupload(p.omDtOperDate2))cJSON_AddStringToObject(subroot, "omDtOperDate2", p.omDtOperDate2.toLocal8Bit().data());
    if(iscanupload(p.omVchMemo))cJSON_AddStringToObject(subroot, "omVchMemo", p.omVchMemo.toLocal8Bit().data());
    if(iscanupload(p.orgCode))cJSON_AddStringToObject(subroot, "orgCode", p.orgCode.toLocal8Bit().data());
    if(iscanupload(p.date))cJSON_AddStringToObject(subroot, "date", p.date.toLocal8Bit().data());
}

void member_up_yun::make_JSON_Array(checkOutMaster &p, cJSON *root)
{
    cJSON *subroot;
    cJSON_AddItemToObject(root, "checkOutMaster", subroot=cJSON_CreateObject());
    //    if(iscanupload(p.omChBillNo))cJSON_AddStringToObject(subroot, "omChBillNo", p.sn;
    if(iscanupload(p.comChPayNo))cJSON_AddStringToObject(subroot, "comChPayNo", p.comChPayNo.toLocal8Bit().data());
    if(iscanupload(p.comChBillNo))cJSON_AddStringToObject(subroot, "comChBillNo", p.comChBillNo.toLocal8Bit().data());
    if(iscanupload(p.comChTableNo))cJSON_AddStringToObject(subroot, "comChTableNo", p.comChTableNo.toLocal8Bit().data());
    if(iscanupload(p.comNumCost))cJSON_AddNumberToObject(subroot, "comNumCost", p.comNumCost);
    if(iscanupload(p.comNumDiscount))cJSON_AddNumberToObject(subroot, "comNumDiscount", p.comNumDiscount);
    if(iscanupload(p.comNumPresent))cJSON_AddNumberToObject(subroot, "comNumPresent", p.comNumPresent);
    if(iscanupload(p.comNumService))cJSON_AddNumberToObject(subroot, "comNumService", p.comNumService);
    if(iscanupload(p.comNumLowCost))cJSON_AddNumberToObject(subroot, "comNumLowCost", p.comNumLowCost);
    if(iscanupload(p.comNumBlotout))cJSON_AddNumberToObject(subroot, "comNumBlotout", p.comNumBlotout);
    if(iscanupload(p.comNumFree))cJSON_AddNumberToObject(subroot, "comNumFree", p.comNumFree);
    if(iscanupload(p.comVchFreeOperId))cJSON_AddStringToObject(subroot, "comVchFreeOperId", p.comVchFreeOperId.toLocal8Bit().data());
    if(iscanupload(p.comNumCash))cJSON_AddNumberToObject(subroot, "comNumCash", p.comNumCash);
    if(iscanupload(p.comNumOther))cJSON_AddNumberToObject(subroot, "comNumOther", p.comNumOther);
    if(iscanupload(p.comNumRun))cJSON_AddNumberToObject(subroot, "comNumRun", p.comNumRun);
    if(iscanupload(p.comVchRunOperId))cJSON_AddStringToObject(subroot, "comVchRunOperId", p.comVchRunOperId.toLocal8Bit().data());
    if(iscanupload(p.comChState))cJSON_AddStringToObject(subroot, "comChState", p.comChState.toLocal8Bit().data());
    if(iscanupload(p.comVchOperId))cJSON_AddStringToObject(subroot, "comVchOperId", p.comVchOperId.toLocal8Bit().data());
    if(iscanupload(p.comDtOperDate))cJSON_AddStringToObject(subroot, "comDtOperDate", p.comDtOperDate.toLocal8Bit().data());
    if(iscanupload(p.comVchOperId2))cJSON_AddStringToObject(subroot, "comVchOperId2", p.comVchOperId2.toLocal8Bit().data());
    if(iscanupload(p.comDtOperDate2))cJSON_AddStringToObject(subroot, "comDtOperDate2", p.comDtOperDate2.toLocal8Bit().data());
    if(iscanupload(p.comVchMemberNo))cJSON_AddStringToObject(subroot, "comVchMemberNo", p.comVchMemberNo.toLocal8Bit().data());
    if(iscanupload(p.comChType))cJSON_AddStringToObject(subroot, "comChType", p.comChType.toLocal8Bit().data());
    if(iscanupload(p.orgCode))cJSON_AddStringToObject(subroot, "orgCode", p.orgCode.toLocal8Bit().data());
    if(iscanupload(p.date))cJSON_AddStringToObject(subroot, "date", p.date.toLocal8Bit().data());
    if(iscanupload(p.comNumRate))cJSON_AddNumberToObject(subroot, "comNumRate", p.comNumRate);

}

void member_up_yun::make_JSON_Array(memberPointList &p, cJSON *root)
{
    cJSON *subroot=cJSON_CreateObject();
    cJSON_AddItemToArray(root, subroot);
    //    float sn;
    if(iscanupload(p.mpIntFlowId))cJSON_AddNumberToObject(subroot, "mpIntFlowId", p.mpIntFlowId);
    if(iscanupload(p.mpVchMemberNo))cJSON_AddStringToObject(subroot, "mpVchMemberNo", p.mpVchMemberNo.toLocal8Bit().data());
    if(iscanupload(p.mpChType))cJSON_AddStringToObject(subroot, "mpChType", p.mpChType.toLocal8Bit().data());
    if(iscanupload(p.mpChPayNo))cJSON_AddStringToObject(subroot, "mpChPayNo", p.mpChPayNo.toLocal8Bit().data());
    if(iscanupload(p.mpNumPoint))cJSON_AddNumberToObject(subroot, "mpNumPoint", p.mpNumPoint);
    if(iscanupload(p.mpVchMemo))cJSON_AddStringToObject(subroot, "mpVchMemo", p.mpVchMemo.toLocal8Bit().data());
    if(iscanupload(p.mpVchOperId))cJSON_AddStringToObject(subroot, "mpVchOperId", p.mpVchOperId.toLocal8Bit().data());
    if(iscanupload(p.mpDtOperDate))cJSON_AddStringToObject(subroot, "mpDtOperDate", p.mpDtOperDate.toLocal8Bit().data());
    if(iscanupload(p.orgCode))cJSON_AddStringToObject(subroot, "orgCode", p.orgCode.toLocal8Bit().data());
    if(iscanupload(p.date))cJSON_AddStringToObject(subroot, "date", p.date.toLocal8Bit().data());

}

void member_up_yun::make_JSON_Array(memberPay &p, cJSON *root)
{
    cJSON *subroot=cJSON_CreateObject();
    cJSON_AddItemToArray(root, subroot);
    //     if(iscanupload(p.date))cJSON_AddStringToObject(subroot, "date", p.sn.toLocal8Bit().data());
    if(iscanupload(p.intFlowid))cJSON_AddNumberToObject(subroot, "intFlowid", p.intFlowid);
    if(iscanupload(p.mpVchMemberNo))cJSON_AddStringToObject(subroot, "mpVchMemberNo", p.mpVchMemberNo.toLocal8Bit().data());
    if(iscanupload(p.mpChPayNo))cJSON_AddStringToObject(subroot, "mpChPayNo", p.mpChPayNo.toLocal8Bit().data());
    if(iscanupload(p.mpChPayModeNo))cJSON_AddStringToObject(subroot, "mpChPayModeNo", p.mpChPayModeNo.toLocal8Bit().data());
    if(iscanupload(p.mpNumAmount))cJSON_AddNumberToObject(subroot, "mpNumAmount", p.mpNumAmount);
    if(iscanupload(p.mpNumPay))cJSON_AddNumberToObject(subroot, "mpNumPay", p.mpNumPay);
    if(iscanupload(p.mpVchOperId))cJSON_AddStringToObject(subroot, "mpVchOperId", p.mpVchOperId.toLocal8Bit().data());
    if(iscanupload(p.mpDtOperDate))cJSON_AddStringToObject(subroot, "mpDtOperDate", p.mpDtOperDate.toLocal8Bit().data());
    if(iscanupload(p.mpChBalanceNo))cJSON_AddStringToObject(subroot, "mpChBalanceNo", p.mpChBalanceNo.toLocal8Bit().data());
    if(iscanupload(p.mpIntCheckFlow))cJSON_AddNumberToObject(subroot, "mpIntCheckFlow", p.mpIntCheckFlow);
    if(iscanupload(p.orgCode))cJSON_AddStringToObject(subroot, "orgCode", p.orgCode.toLocal8Bit().data());
    if(iscanupload(p.date))cJSON_AddStringToObject(subroot, "date", p.date.toLocal8Bit().data());

}

void member_up_yun::make_JSON_Array(memberCuramount &p, cJSON *root)
{
    cJSON *subroot=cJSON_CreateObject();
    cJSON_AddItemToArray(root, subroot);
    //    if(iscanupload(p.date))cJSON_AddStringToObject(subroot, "date", p.sn.toLocal8Bit().data());
    if(iscanupload(p.vchMemberno))cJSON_AddStringToObject(subroot, "vchMemberno", p.vchMemberno.toLocal8Bit().data());
    if(iscanupload(p.numAmount))cJSON_AddNumberToObject(subroot, "numAmount", p.numAmount);
    if(iscanupload(p.numHangamount))cJSON_AddNumberToObject(subroot, "numHangamount", p.numHangamount);
    if(iscanupload(p.numPoint))cJSON_AddNumberToObject(subroot, "numPoint", p.numPoint);
    if(iscanupload(p.vchOperId))cJSON_AddStringToObject(subroot, "vchOperId", p.vchOperId.toLocal8Bit().data());
    if(iscanupload(p.dtOperdate))cJSON_AddStringToObject(subroot, "dtOperdate", p.dtOperdate.toLocal8Bit().data());
    if(iscanupload(p.chBranchno))cJSON_AddStringToObject(subroot, "chBranchno", p.chBranchno.toLocal8Bit().data());
    if(iscanupload(p.orgCode))cJSON_AddStringToObject(subroot, "orgCode", p.orgCode.toLocal8Bit().data());
    if(iscanupload(p.date))cJSON_AddStringToObject(subroot, "date", p.date.toLocal8Bit().data());

}

void member_up_yun::make_JSON_Array(tableRecordsConsumption &p, cJSON *root)
{
    cJSON *pItem = cJSON_CreateObject();
    //    long sn.toLocal8Bit().data());
    if(iscanupload(p.chBillno))cJSON_AddStringToObject(pItem, "chBillno", p.chBillno.toLocal8Bit().data());
    if(iscanupload(p.chTableno))cJSON_AddStringToObject(pItem, "chTableno", p.chTableno.toLocal8Bit().data());
    if(iscanupload(p.intPerson))cJSON_AddNumberToObject(pItem, "intPerson", p.intPerson);
    if(iscanupload(p.vchMemo))cJSON_AddStringToObject(pItem, "vchMemo", p.vchMemo.toLocal8Bit().data());
    if(iscanupload(p.chState))cJSON_AddStringToObject(pItem, "chState", p.chState.toLocal8Bit().data());
    if(iscanupload(p.chPayno))cJSON_AddStringToObject(pItem, "chPayno", p.chPayno.toLocal8Bit().data());
    if(iscanupload(p.vchOperId))cJSON_AddStringToObject(pItem, "vchOperId", p.vchOperId.toLocal8Bit().data());
    if(iscanupload(p.dtOperdate))cJSON_AddStringToObject(pItem, "dtOperdate", p.dtOperdate.toLocal8Bit().data());
    if(iscanupload(p.vchOperId2))cJSON_AddStringToObject(pItem, "vchOperId2", p.vchOperId2.toLocal8Bit().data());
    if(iscanupload(p.dtOperdate2))cJSON_AddStringToObject(pItem, "dtOperdate2", p.dtOperdate2.toLocal8Bit().data());
    if(iscanupload(p.chOrderState))cJSON_AddStringToObject(pItem, "chOrderState", p.chOrderState.toLocal8Bit().data());
    if(iscanupload(p.numCost))cJSON_AddNumberToObject(pItem, "numCost", p.numCost);
    if(iscanupload(p.vchWaiter))cJSON_AddStringToObject(pItem, "vchWaiter", p.vchWaiter.toLocal8Bit().data());
    if(iscanupload(p.numTransService))cJSON_AddNumberToObject(pItem, "numTransService", p.numTransService);
    if(iscanupload(p.orgCode))cJSON_AddStringToObject(pItem, "orgCode", p.orgCode.toLocal8Bit().data());
    if(iscanupload(p.date))cJSON_AddStringToObject(pItem, "date", p.date.toLocal8Bit().data());
}

void member_up_yun::make_JSON_Array(orderDish &p, cJSON *root)
{
    cJSON *subroot=cJSON_CreateObject();
    cJSON_AddItemToArray(root, subroot);
    //    if(iscanupload(p.date))cJSON_AddStringToObject(subroot, "date", p.sn.toLocal8Bit().data());
    if(iscanupload(p.bdIntFlowId))cJSON_AddNumberToObject(subroot, "bdIntFlowId", p.bdIntFlowId);
    if(iscanupload(p.bdChBillNo))cJSON_AddStringToObject(subroot, "bdChBillNo", p.bdChBillNo.toLocal8Bit().data());
    if(iscanupload(p.bdChTableNo))cJSON_AddStringToObject(subroot, "bdChTableNo", p.bdChTableNo.toLocal8Bit().data());
    if(iscanupload(p.bdIntId))cJSON_AddNumberToObject(subroot, "bdIntId", p.bdIntId);
    if(iscanupload(p.bdChDishNo))cJSON_AddStringToObject(subroot, "bdChDishNo", p.bdChDishNo.toLocal8Bit().data());
    if(iscanupload(p.bdNumPrice))cJSON_AddNumberToObject(subroot, "bdNumPrice", p.bdNumPrice);
    if(iscanupload(p.bdNumPriceOrg))cJSON_AddNumberToObject(subroot, "bdNumPriceOrg", p.bdNumPriceOrg);
    if(iscanupload(p.bdNumPriceAdd))cJSON_AddNumberToObject(subroot, "bdNumPriceAdd", p.bdNumPriceAdd);
    if(iscanupload(p.bdNumNum))cJSON_AddNumberToObject(subroot, "bdNumNum", p.bdNumNum);
    if(iscanupload(p.bdNumBack))cJSON_AddNumberToObject(subroot, "bdNumBack", p.bdNumBack);
    if(iscanupload(p.bdVchBackOperId))cJSON_AddStringToObject(subroot, "bdVchBackOperId", p.bdVchBackOperId.toLocal8Bit().data());
    if(iscanupload(p.bdDtBackOperDate))cJSON_AddStringToObject(subroot, "bdDtBackOperDate", p.bdDtBackOperDate.toLocal8Bit().data());
    if(iscanupload(p.bdIntDiscount))cJSON_AddNumberToObject(subroot, "bdIntDiscount", p.bdIntDiscount);
    if(iscanupload(p.bdVchDisOperId))cJSON_AddStringToObject(subroot, "bdVchDisOperId", p.bdVchDisOperId.toLocal8Bit().data());
    if(iscanupload(p.bdDtDisOperDate))cJSON_AddStringToObject(subroot, "bdDtDisOperDate", p.bdDtDisOperDate.toLocal8Bit().data());
    if(iscanupload(p.btVchMemberNo))cJSON_AddStringToObject(subroot, "btVchMemberNo", p.btVchMemberNo.toLocal8Bit().data());
    if(iscanupload(p.bdChConsult))cJSON_AddStringToObject(subroot, "bdChConsult", p.bdChConsult.toLocal8Bit().data());
    if(iscanupload(p.bdChPrintFlag))cJSON_AddStringToObject(subroot, "bdChPrintFlag", p.bdChPrintFlag.toLocal8Bit().data());
    if(iscanupload(p.bdVchPrintMemo))cJSON_AddStringToObject(subroot, "bdVchPrintMemo", p.bdVchPrintMemo.toLocal8Bit().data());
    if(iscanupload(p.dcChSuitFlag))cJSON_AddStringToObject(subroot, "dcChSuitFlag", p.dcChSuitFlag.toLocal8Bit().data());
    if(iscanupload(p.dcChSuitNo))cJSON_AddStringToObject(subroot, "dcChSuitNo", p.dcChSuitNo.toLocal8Bit().data());
    if(iscanupload(p.bdChSpecialFlag))cJSON_AddStringToObject(subroot, "bdChSpecialFlag", p.bdChSpecialFlag.toLocal8Bit().data());
    if(iscanupload(p.chPresentFlag))cJSON_AddStringToObject(subroot, "chPresentFlag", p.chPresentFlag.toLocal8Bit().data());
    if(iscanupload(p.bdVchPreOperId))cJSON_AddStringToObject(subroot, "bdVchPreOperId", p.bdVchPreOperId.toLocal8Bit().data());
    if(iscanupload(p.bdDtPreOperDate))cJSON_AddStringToObject(subroot, "bdDtPreOperDate", p.bdDtPreOperDate.toLocal8Bit().data());
    if(iscanupload(p.bdVchOperId))cJSON_AddStringToObject(subroot, "bdVchOperId", p.bdVchOperId.toLocal8Bit().data());
    if(iscanupload(p.bdDtOperDate))cJSON_AddStringToObject(subroot, "bdDtOperDate", p.bdDtOperDate.toLocal8Bit().data());
    if(iscanupload(p.bdChTogoNo))cJSON_AddStringToObject(subroot, "bdChTogoNo", p.bdChTogoNo.toLocal8Bit().data());
    if(iscanupload(p.bdChPayNo))cJSON_AddStringToObject(subroot, "bdChPayNo", p.bdChPayNo.toLocal8Bit().data());
    if(iscanupload(p.bdChOutFlag))cJSON_AddStringToObject(subroot, "bdChOutFlag", p.bdChOutFlag.toLocal8Bit().data());
    if(iscanupload(p.orgCode))cJSON_AddStringToObject(subroot, "orgCode", p.orgCode.toLocal8Bit().data());
    if(iscanupload(p.date))cJSON_AddStringToObject(subroot, "date", p.date.toLocal8Bit().data());

    QString param = w_sys_manage_cloudsync::print_yun(subroot);

    //    qDebug() << "======================subroot" << param;
}

void member_up_yun::make_JSON_Array(checkOutDetail &p, cJSON *root)
{
    cJSON *subroot=cJSON_CreateObject();
    cJSON_AddItemToArray(root, subroot);
    //    if(iscanupload(p.date))cJSON_AddStringToObject(subroot, "date", p.sn.toLocal8Bit().data());
    if(iscanupload(p.codIntFlowId))cJSON_AddNumberToObject(subroot, "codIntFlowId", p.codIntFlowId);
    if(iscanupload(p.codChPayNo))cJSON_AddStringToObject(subroot, "codChPayNo", p.codChPayNo.toLocal8Bit().data());
    if(iscanupload(p.codChPayModeNo))cJSON_AddStringToObject(subroot, "codChPayModeNo", p.codChPayModeNo.toLocal8Bit().data());
    if(iscanupload(p.codNumPayAmount))cJSON_AddNumberToObject(subroot, "codNumPayAmount", p.codNumPayAmount);
    if(iscanupload(p.codNumChange))cJSON_AddNumberToObject(subroot, "codNumChange", p.codNumChange);
    if(iscanupload(p.codNumRealAmount))cJSON_AddNumberToObject(subroot, "codNumRealAmount", p.codNumRealAmount);
    if(iscanupload(p.codNumFace))cJSON_AddNumberToObject(subroot, "codNumFace", p.codNumFace);
    if(iscanupload(p.codVchVoucherNo))cJSON_AddStringToObject(subroot, "codVchVoucherNo", p.codVchVoucherNo.toLocal8Bit().data());
    if(iscanupload(p.codVchMemo))cJSON_AddStringToObject(subroot, "codVchMemo", p.codVchMemo.toLocal8Bit().data());
    if(iscanupload(p.codVchOperId))cJSON_AddStringToObject(subroot, "codVchOperId", p.codVchOperId.toLocal8Bit().data());
    if(iscanupload(p.codDtOperDate))cJSON_AddStringToObject(subroot, "codDtOperDate", p.codDtOperDate.toLocal8Bit().data());
    if(iscanupload(p.codNumCardRemain))cJSON_AddNumberToObject(subroot, "codNumCardRemain", p.codNumCardRemain);
    if(iscanupload(p.codNumFaceNum))cJSON_AddNumberToObject(subroot, "codNumFaceNum", p.codNumFaceNum);
    if(iscanupload(p.orgCode))cJSON_AddStringToObject(subroot, "orgCode", p.orgCode.toLocal8Bit().data());
    if(iscanupload(p.date))cJSON_AddStringToObject(subroot, "date", p.date.toLocal8Bit().data());
}

void member_up_yun::make_JSON_Array(memberDeposit &p, cJSON *root)
{
    cJSON *pItem = cJSON_CreateObject();
    //    if(iscanupload(p.date))cJSON_AddStringToObject(pItem, "date", p.sn.toLocal8Bit().data());
    if(iscanupload(p.mdIntFlowId))cJSON_AddNumberToObject(pItem, "mdIntFlowId", p.mdIntFlowId);
    if(iscanupload(p.mdVchMemberNo))cJSON_AddStringToObject(pItem, "mdVchMemberNo", p.mdVchMemberNo.toLocal8Bit().data());
    if(iscanupload(p.mdNumDeposit))cJSON_AddNumberToObject(pItem, "mdNumDeposit", p.mdNumDeposit);
    if(iscanupload(p.mdNumRealaMount))cJSON_AddNumberToObject(pItem, "mdNumRealaMount", p.mdNumRealaMount);
    if(iscanupload(p.mdChDepositMode))cJSON_AddStringToObject(pItem, "mdChDepositMode", p.mdChDepositMode.toLocal8Bit().data());
    if(iscanupload(p.mdChPayMode))cJSON_AddStringToObject(pItem, "mdChPayMode", p.mdChPayMode.toLocal8Bit().data());
    if(iscanupload(p.mdVchOperId))cJSON_AddStringToObject(pItem, "mdVchOperId", p.mdVchOperId.toLocal8Bit().data());
    if(iscanupload(p.dtOperDate))cJSON_AddStringToObject(pItem, "dtOperDate", p.dtOperDate.toLocal8Bit().data());
    if(iscanupload(p.mdVchMemo))cJSON_AddStringToObject(pItem, "mdVchMemo", p.mdVchMemo.toLocal8Bit().data());
    if(iscanupload(p.mdRemainAmt))cJSON_AddNumberToObject(pItem, "mdRemainAmt", p.mdRemainAmt);
    if(iscanupload(p.orgCode))cJSON_AddStringToObject(pItem, "orgCode", p.orgCode.toLocal8Bit().data());
    if(iscanupload(p.date))cJSON_AddStringToObject(pItem, "date", p.date.toLocal8Bit().data());
    if(iscanupload(p.comChPayNo))cJSON_AddStringToObject(pItem, "comChPayNo", p.comChPayNo.toLocal8Bit().data());
}


w_sys_manage_cloudsync_pop_info::w_sys_manage_cloudsync_pop_info(QWidget *parent):QTextEdit(parent)
{
    this->setWindowFlags(Qt::Popup);
}
