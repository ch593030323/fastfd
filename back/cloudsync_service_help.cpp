
#include "cloudsync_service_help.h"

#include <QDebug>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QByteArray>
#include <QNetworkReply>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDate>
#include "lds_messagebox.h"
static void filter_cJSON_AddStringToObject(cJSON *item, const char *key, const char *data)
{
    if(QByteArray(data).isEmpty()) return;
    cJSON_AddStringToObject(item, key, data);
}

cloudsync_service_help::cloudsync_service_help(QProgressBar *progressBar)
{
    this->progressBar=progressBar;
    listCount = 1000;
    dateFormatStr="yyyy-MM-dd hh:mm:ss";

    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    hostName = conf.value("w_sys_manage_cloudsync_basedataset/yun_server_ip","").toString();
    orgCode = conf.value("w_sys_manage_cloudsync_basedataset/orgCode","").toString();
    cloudUser = conf.value("w_sys_manage_cloudsync_basedataset/cloudUser","").toString();
    cloudPass = conf.value("w_sys_manage_cloudsync_basedataset/cloudPass","").toString();

    headInfo.insert("machineCode","");
    headInfo.insert("orgCode",orgCode);
    headInfo.insert("userName",cloudUser);
    headInfo.insert("password",cloudPass);
}

cloudsync_service_help::~cloudsync_service_help()
{
}


//****************************************************************************************************************************************************************************
//****************************************************************************************************************************************************************************
//****************************************************************************************************************************************************************************
//****************************************************************************************************************************************************************************
//****************************************************************************************************************************************************************************
//****************************************************************************************************************************************************************************
//****************************************************************************************************************************************************************************
//****************************************************************************************************************************************************************************

//上传基础资料  x-/////////////////////////////////////////////////////////////
void cloudsync_service_help::upLoadBaseDate()
{
    stopFlag=false;
    index=1;
    upSize=21;
    QSqlQuery sqlQuery;
    cJSON *pArray = cJSON_CreateArray();

    progressBar->setMaximum(upSize);

    //1单位表
    sqlQuery.exec("select * from cey_bt_unit;");

    while (sqlQuery.next()) {

        cJSON *pItem = cJSON_CreateObject();
        filter_cJSON_AddStringToObject(pItem, "duChUnitNo", QByteArray().append(sqlQuery.record().value("ch_unitno").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "duVchUnitName", QByteArray().append(sqlQuery.record().value("vch_unitname").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "duChNumType", QByteArray().append(sqlQuery.record().value("ch_numtype").toString()).data());
        cJSON_AddNumberToObject(pItem, "duNumDefault", sqlQuery.record().value("num_default").toDouble());
        filter_cJSON_AddStringToObject(pItem, "orgCode", QByteArray().append(orgCode).data());
        cJSON_AddItemToArray(pArray, pItem);
    }

    if(!execHttp(pArray,"http://" + hostName
                 + "/CateSystem/arm/dish-unit/add-rest"))
    {return;}
    initValue(pArray);


    //2菜品表
    sqlQuery.exec("select * from cey_bt_dish;");

    while (sqlQuery.next()) {

        cJSON *pItem = cJSON_CreateObject();
        filter_cJSON_AddStringToObject(pItem, "diChDishNo", QByteArray().append(sqlQuery.record().value("ch_dishno").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "diVchDishName", QByteArray().append(sqlQuery.record().value("vch_dishname").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "diVchSpell", QByteArray().append(sqlQuery.record().value("vch_spell").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "diVchEnglish", QByteArray().append(sqlQuery.record().value("vch_english").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "diChDishTypeNo", QByteArray().append(sqlQuery.record().value("ch_dish_typeno").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "diChUnitNo", QByteArray().append(sqlQuery.record().value("ch_unitno").toString()).data());
        cJSON_AddNumberToObject(pItem, "diNumPrice", sqlQuery.record().value("num_price").toFloat());
        filter_cJSON_AddStringToObject(pItem, "diChSuitflag", QByteArray().append(sqlQuery.record().value("ch_suitflag").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "diChDiscount", QByteArray().append(sqlQuery.record().value("ch_discount").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "diChCurprice", QByteArray().append(sqlQuery.record().value("ch_curprice").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "diChTempflag", QByteArray().append(sqlQuery.record().value("ch_tempflag").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "diChStopflag", QByteArray().append(sqlQuery.record().value("ch_stopflag").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "diChPrintFlag", QByteArray().append(sqlQuery.record().value("ch_printflag").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "diVchPrinter", QByteArray().append(sqlQuery.record().value("vch_printer").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "diChSubTypeNo", QByteArray().append(sqlQuery.record().value("ch_sub_typeno").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "diChMaterialNo", QByteArray().append(sqlQuery.record().value("ch_materialno").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "diChServiceFlag", QByteArray().append(sqlQuery.record().value("ch_serviceflag").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "diChLowFlag", QByteArray().append(sqlQuery.record().value("ch_lowflag").toString()).data());
        cJSON_AddNumberToObject(pItem, "diNumMprice", sqlQuery.record().value("num_m_price").toFloat());
        filter_cJSON_AddStringToObject(pItem, "diChDeductFlag", QByteArray().append(sqlQuery.record().value("ch_deductflag").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "diChDeductType", QByteArray().append(sqlQuery.record().value("ch_deducttype").toString()).data());
        cJSON_AddNumberToObject(pItem, "diIntDeduct", sqlQuery.record().value("int_deduct").toInt());
        cJSON_AddNumberToObject(pItem, "diNumDeduct", sqlQuery.record().value("num_deduct").toFloat());
        filter_cJSON_AddStringToObject(pItem, "diChSaleFlag", QByteArray().append(sqlQuery.record().value("ch_saleflag").toString()).data());
        cJSON_AddNumberToObject(pItem, "diChSaleType", sqlQuery.record().value("ch_saletype").toInt());
        cJSON_AddNumberToObject(pItem, "diIntSaleDeduct", sqlQuery.record().value("int_sale_deduct").toInt());
        cJSON_AddNumberToObject(pItem, "diNumSale", sqlQuery.record().value("num_sale").toFloat());
        filter_cJSON_AddStringToObject(pItem, "itemFlag", QByteArray().append(sqlQuery.record().value("item_flag").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "diVchPurUnitNo", QByteArray().append(sqlQuery.record().value("vch_pur_unitno").toString()).data());
        cJSON_AddNumberToObject(pItem, "diIntUnitRate", sqlQuery.record().value("int_unit_rate").toInt());
        filter_cJSON_AddStringToObject(pItem, "diChOff", QByteArray().append(sqlQuery.record().value("ch_off").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "diVchSubNo", QByteArray().append(sqlQuery.record().value("vch_subno").toString()).data());
        cJSON_AddNumberToObject(pItem, "diNumSalePrice", sqlQuery.record().value("num_sale_price").toFloat());
        filter_cJSON_AddStringToObject(pItem, "diChOutFlag", QByteArray().append(sqlQuery.record().value("ch_outflag").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "diVchOutPrint", QByteArray().append(sqlQuery.record().value("vch_outprint").toString()).data());
        cJSON_AddNumberToObject(pItem, "intRate", sqlQuery.record().value("int_rate").toFloat());
        filter_cJSON_AddStringToObject(pItem, "chWeight", QByteArray().append(sqlQuery.record().value("ch_weight").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "vchBarcode", QByteArray().append(sqlQuery.record().value("vch_barcode").toString()).data());

        filter_cJSON_AddStringToObject(pItem, "orgCode", QByteArray().append(orgCode).data());

        cJSON_AddItemToArray(pArray, pItem);
    }

    if(!execHttp(pArray,"http://" + hostName
                 + "/CateSystem/arm/dish-info/add-rest"))
    {return;}
    initValue(pArray);

    // 3菜品小类

    sqlQuery.exec("select * from cey_bt_dish_subtype;");

    while (sqlQuery.next()) {

        cJSON *pItem = cJSON_CreateObject();
        filter_cJSON_AddStringToObject(pItem, "dstChSubTypeNo", QByteArray().append(sqlQuery.record().value("ch_sub_typeno").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "dstVchSubTypeName", QByteArray().append(sqlQuery.record().value("vch_sub_typename").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "dstChDishTypeNo", QByteArray().append(sqlQuery.record().value("ch_dish_typeno").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "dstChLagelPrintFlag", QByteArray().append(sqlQuery.record().value("ch_labelprint_flag").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "orgCode", QByteArray().append(orgCode).data());
        cJSON_AddItemToArray(pArray, pItem);
    }

    if(!execHttp(pArray,"http://" + hostName
                 + "/CateSystem/arm/dish-sub-type/add-rest"))
    {return;}
    initValue(pArray);

    // 4菜品大类

    sqlQuery.exec("select * from cey_bt_dish_type;");
    while (sqlQuery.next()) {
        cJSON *pItem = cJSON_CreateObject();
        filter_cJSON_AddStringToObject(pItem, "dtChDishTypeNo", QByteArray().append(sqlQuery.record().value("ch_dish_typeno").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "dtVchDishTypeName", QByteArray().append(sqlQuery.record().value("vch_dish_typename").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "orgCode", QByteArray().append(orgCode).data());
        cJSON_AddItemToArray(pArray, pItem);
    }

    if(!execHttp(pArray,"http://" + hostName
                 + "/CateSystem/arm/dish-type/add-rest"))
    {return;}
    initValue(pArray);

    //5 菜品套菜
    sqlQuery.exec("select * from cey_bt_dish_suit;");

    while (sqlQuery.next()) {

        cJSON *pItem = cJSON_CreateObject();
        filter_cJSON_AddStringToObject(pItem, "dsIntFlowId", QByteArray().append(sqlQuery.record().value("int_flowid").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "dsChSuitNo", QByteArray().append(sqlQuery.record().value("ch_suitno").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "dsChDishNo", QByteArray().append(sqlQuery.record().value("ch_dishno").toString()).data());
        cJSON_AddNumberToObject(pItem, "dsNumNum", sqlQuery.record().value("num_num").toFloat());
        filter_cJSON_AddStringToObject(pItem, "dsChDishNoChang", QByteArray().append(sqlQuery.record().value("ch_dishno_chang").toString()).data());
        cJSON_AddNumberToObject(pItem, "dsNumNumChange", sqlQuery.record().value("num_num_change").toFloat());
        filter_cJSON_AddStringToObject(pItem, "orgCode", QByteArray().append(orgCode).data());
        cJSON_AddItemToArray(pArray, pItem);
    }


    if(!execHttp(pArray,"http://" + hostName
                 + "/CateSystem/arm/dish-suit/add-rest"))
    {return;}
    initValue(pArray);


    // 6菜品套菜替换
    sqlQuery.exec("select * from cey_bt_suit_change;");

    while (sqlQuery.next()) {

        cJSON *pItem = cJSON_CreateObject();
        filter_cJSON_AddStringToObject(pItem, "dscIntFlowId", QByteArray().append(sqlQuery.record().value("int_flowID").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "dscChDishNo", QByteArray().append(sqlQuery.record().value("ch_dishno").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "dscNumNumChange", QByteArray().append(sqlQuery.record().value("num_num_change").toString()).data());

        filter_cJSON_AddStringToObject(pItem, "orgCode", QByteArray().append(orgCode).data());
        cJSON_AddItemToArray(pArray, pItem);
    }

    if(!execHttp(pArray,"http://" + hostName
                 + "/CateSystem/arm/dish-suit-change/add-rest"))
    {return;}
    initValue(pArray);


    //7 餐桌管理

    sqlQuery.exec("select * from cey_bt_table;");

    while (sqlQuery.next()) {

        cJSON *pItem = cJSON_CreateObject();
        filter_cJSON_AddStringToObject(pItem, "dtChTableNo", QByteArray().append(sqlQuery.record().value("ch_tableno").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "dtVchTableName", QByteArray().append(sqlQuery.record().value("vch_tablename").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "dtChTypeNo", QByteArray().append(sqlQuery.record().value("ch_typeno").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "dtChBillNo", QByteArray().append(sqlQuery.record().value("ch_billno").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "dtChLockFlag", QByteArray().append(sqlQuery.record().value("ch_lockflag").toString()).data());
        cJSON_AddNumberToObject(pItem, "dtIntPerson", sqlQuery.record().value("int_person").toInt());
        filter_cJSON_AddStringToObject(pItem, "dtVchMemo", QByteArray().append(sqlQuery.record().value("vch_memo").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "dtChState", QByteArray().append(sqlQuery.record().value("ch_state").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "dtDtDateTime", QByteArray().append(sqlQuery.record().value("dt_datetime").toDateTime().toString(dateFormatStr)).data());
        filter_cJSON_AddStringToObject(pItem, "dtChAreaNo", QByteArray().append(sqlQuery.record().value("ch_areaNo").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "orgCode", QByteArray().append(orgCode).data());
        cJSON_AddItemToArray(pArray, pItem);
    }


    if(!execHttp(pArray,"http://" + hostName
                 + "/CateSystem/arm/dish-table/add-rest"))
    {return;}
    initValue(pArray);

    //8餐桌类型

    sqlQuery.exec("select * from cey_bt_table_type;");

    while (sqlQuery.next()) {

        cJSON *pItem = cJSON_CreateObject();
        filter_cJSON_AddStringToObject(pItem, "dttChTyperNo", QByteArray().append(sqlQuery.record().value("ch_typeno").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "dttVchTypeName", QByteArray().append(sqlQuery.record().value("vch_typename").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "dttChPriceKind", QByteArray().append(sqlQuery.record().value("ch_pricekind").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "dttChServiceMode", QByteArray().append(sqlQuery.record().value("ch_service_mode").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "dttChServiceFlag", QByteArray().append(sqlQuery.record().value("ch_serviceflag").toString()).data());
        cJSON_AddNumberToObject(pItem, "dttIntStartMinute", sqlQuery.record().value("int_start_minute").toInt());
        cJSON_AddNumberToObject(pItem, "dttNumStartAmount", sqlQuery.record().value("num_start_amount").toFloat());
        cJSON_AddNumberToObject(pItem, "dttIntUnitMinute", sqlQuery.record().value("int_unit_minute").toInt());
        cJSON_AddNumberToObject(pItem, "dttNumUnitAmount", sqlQuery.record().value("num_unit_amount").toFloat());
        cJSON_AddNumberToObject(pItem, "dttIntOver",sqlQuery.record().value("int_over").toInt());
        cJSON_AddNumberToObject(pItem, "dttNumRation", sqlQuery.record().value("num_ration").toFloat());
        cJSON_AddNumberToObject(pItem, "dttIntRate", sqlQuery.record().value("int_rate").toInt());
        cJSON_AddNumberToObject(pItem, "dttNumMaxService", sqlQuery.record().value("num_max_service").toFloat());
        filter_cJSON_AddStringToObject(pItem, "dttChLowMode", QByteArray().append(sqlQuery.record().value("ch_low_mode").toString()).data());
        cJSON_AddNumberToObject(pItem, "dttNumLowCost", sqlQuery.record().value("num_lowcost").toFloat());
        cJSON_AddNumberToObject(pItem, "dttIntBeginTime",sqlQuery.record().value("int_begin_time").toInt());

        filter_cJSON_AddStringToObject(pItem, "orgCode", QByteArray().append(orgCode).data());
        cJSON_AddItemToArray(pArray, pItem);
    }


    if(!execHttp(pArray,"http://" + hostName
                 + "/CateSystem/arm/dish-table-type/add-rest"))
    {return;}
    initValue(pArray);

    // 9餐区信息

    sqlQuery.exec("select * from cey_bt_table_area;");

    while (sqlQuery.next()) {

        cJSON *pItem = cJSON_CreateObject();
        filter_cJSON_AddStringToObject(pItem, "dtaChAreaNo", QByteArray().append(sqlQuery.record().value("ch_areano").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "dtaVchAreaName", QByteArray().append(sqlQuery.record().value("vch_areaname").toString()).data());

        filter_cJSON_AddStringToObject(pItem, "orgCode", QByteArray().append(orgCode).data());
        cJSON_AddItemToArray(pArray, pItem);
    }


    if(!execHttp(pArray,"http://" + hostName
                 + "/CateSystem/arm/dish-table-area/add-rest"))
    {return;}
    initValue(pArray);

    //10 会员信息

    sqlQuery.exec("select * from t_m_member;");

    while (sqlQuery.next()) {

        cJSON *pItem = cJSON_CreateObject();
        filter_cJSON_AddStringToObject(pItem, "miVchMemberNo", QByteArray().append(sqlQuery.record().value("vch_memberno").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "miVchMember", QByteArray().append(sqlQuery.record().value("vch_member").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "miChTypeNo", QByteArray().append(sqlQuery.record().value("ch_typeno").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "miVchId", QByteArray().append(sqlQuery.record().value("vch_id").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "miDtBirthday", QByteArray().append(sqlQuery.record().value("dt_birthday").toDateTime().toString(dateFormatStr)).data());
        filter_cJSON_AddStringToObject(pItem, "miVchTel", QByteArray().append(sqlQuery.record().value("vch_tel").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "miVchHandTel", QByteArray().append(sqlQuery.record().value("vch_handtel").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "miVchAddress", QByteArray().append(sqlQuery.record().value("vch_address").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "miDtLimit", QByteArray().append(sqlQuery.record().value("dt_limit").toDateTime().toString(dateFormatStr)).data());
        filter_cJSON_AddStringToObject(pItem, "miChState", QByteArray().append(sqlQuery.record().value("ch_state").toString()).data());
        cJSON_AddNumberToObject(pItem, "numLimit",sqlQuery.record().value("num_limit").toFloat());
        filter_cJSON_AddStringToObject(pItem, "miChCardFlag", QByteArray().append(sqlQuery.record().value("ch_cardflag").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "miVchCardNo", QByteArray().append(sqlQuery.record().value("vch_cardno").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "miVchPassword", QByteArray().append(sqlQuery.record().value("vch_password").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "miVchOperId", QByteArray().append(sqlQuery.record().value("vch_operID").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "miDtOperDate", QByteArray().append(sqlQuery.record().value("dt_operdate").toDateTime().toString(dateFormatStr)).data());
        cJSON_AddNumberToObject(pItem, "miIntBasedata", sqlQuery.record().value("int_basedata").toInt());
        filter_cJSON_AddStringToObject(pItem, "miDtSendTime", QByteArray().append(sqlQuery.record().value("dt_sendtime").toDateTime().toString(dateFormatStr)).data());

        filter_cJSON_AddStringToObject(pItem, "orgCode", QByteArray().append(orgCode).data());
        cJSON_AddItemToArray(pArray, pItem);
    }


    if(!execHttp(pArray,"http://" + hostName
                 + "/CateSystem/arm/member-info/add-rest"))
    {return;}
    initValue(pArray);

    // 11会员类型

    sqlQuery.exec("select * from t_m_member_type;");

    while (sqlQuery.next()) {

        cJSON *pItem = cJSON_CreateObject();
        filter_cJSON_AddStringToObject(pItem, "chTypeno", QByteArray().append(sqlQuery.record().value("ch_typeno").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "vchTypename", QByteArray().append(sqlQuery.record().value("vch_typename").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "chAttribute", QByteArray().append(sqlQuery.record().value("ch_attribute").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "chFavourable", QByteArray().append(sqlQuery.record().value("ch_favourable").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "chPricekind", QByteArray().append(sqlQuery.record().value("ch_pricekind").toString()).data());
        cJSON_AddNumberToObject(pItem, "intDiscount", sqlQuery.record().value("int_discount").toInt());
        filter_cJSON_AddStringToObject(pItem, "chProjectno", QByteArray().append(sqlQuery.record().value("ch_projectno").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "chCalc", QByteArray().append(sqlQuery.record().value("ch_calc").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "chCalcType", QByteArray().append(sqlQuery.record().value("ch_calc_type").toString()).data());
        cJSON_AddNumberToObject(pItem, "intCalcUnit", sqlQuery.record().value("int_calc_unit").toInt());
        filter_cJSON_AddStringToObject(pItem, "chAreaFlag", QByteArray().append(sqlQuery.record().value("ch_area_flag").toString()).data());
        cJSON_AddNumberToObject(pItem, "intBackcashRate", sqlQuery.record().value("int_backcash_rate").toInt());
        cJSON_AddNumberToObject(pItem, "intCardBaseamount", sqlQuery.record().value("int_card_baseamount").toInt());

        filter_cJSON_AddStringToObject(pItem, "orgCode", QByteArray().append(orgCode).data());
        cJSON_AddItemToArray(pArray, pItem);
    }


    if(!execHttp(pArray,"http://" + hostName
                 + "/CateSystem/arm/member-type/add-rest"))
    {return;}
    initValue(pArray);

    // 12会员参数

    sqlQuery.exec("select * from t_m_parameter;");

    while (sqlQuery.next()) {

        cJSON *pItem = cJSON_CreateObject();
        filter_cJSON_AddStringToObject(pItem, "vchParameter", QByteArray().append(sqlQuery.record().value("vch_parameter").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "vchValue", QByteArray().append(sqlQuery.record().value("vch_value").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "vchExplain", QByteArray().append(sqlQuery.record().value("vch_explain").toString()).data());

        filter_cJSON_AddStringToObject(pItem, "orgCode", QByteArray().append(orgCode).data());
        cJSON_AddItemToArray(pArray, pItem);
    }

    if(!execHttp(pArray,"http://" + hostName
                 + "/CateSystem/arm/member-parameter/add-rest"))
    {return;}
    initValue(pArray);

    // 13 特殊做法

    sqlQuery.exec("select * from cey_bt_cp_dish;");

    while (sqlQuery.next()) {

        cJSON *pItem = cJSON_CreateObject();
        filter_cJSON_AddStringToObject(pItem, "dpChDishNo", QByteArray().append(sqlQuery.record().value("ch_dishno").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "dpChNo", QByteArray().append(sqlQuery.record().value("ch_no").toString()).data());

        filter_cJSON_AddStringToObject(pItem, "orgCode", QByteArray().append(orgCode).data());
        cJSON_AddItemToArray(pArray, pItem);
    }

    if(!execHttp(pArray,"http://" + hostName
                 + "/CateSystem/arm/dish-practice/add-rest"))
    {return;}
    initValue(pArray);

    // 14做法信息:

    sqlQuery.exec("select * from cey_bt_cp_memo;");

    while (sqlQuery.next()) {

        cJSON *pItem = cJSON_CreateObject();
        filter_cJSON_AddStringToObject(pItem, "chNo", QByteArray().append(sqlQuery.record().value("ch_no").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "vchCpMemo", QByteArray().append(sqlQuery.record().value("vch_cp_memo").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "chTypeno", QByteArray().append(sqlQuery.record().value("ch_typeno").toString()).data());
        cJSON_AddNumberToObject(pItem, "numAddPrice", sqlQuery.record().value("num_add_price").toFloat());
        filter_cJSON_AddStringToObject(pItem, "orgCode", QByteArray().append(orgCode).data());
        cJSON_AddItemToArray(pArray, pItem);
    }


    if(!execHttp(pArray,"http://" + hostName
                 + "/CateSystem/arm/dish-practice-memo/add-rest"))
    {return;}
    initValue(pArray);

    // 15做法类型

    sqlQuery.exec("select * from cey_bt_cp_type;");

    while (sqlQuery.next()) {

        cJSON *pItem = cJSON_CreateObject();
        filter_cJSON_AddStringToObject(pItem, "dptChTypeNo", QByteArray().append(sqlQuery.record().value("ch_typeno").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "dptVchTypeName", QByteArray().append(sqlQuery.record().value("vch_typename").toString()).data());

        filter_cJSON_AddStringToObject(pItem, "orgCode", QByteArray().append(orgCode).data());
        cJSON_AddItemToArray(pArray, pItem);
    }


    if(!execHttp(pArray,"http://" + hostName
                 + "/CateSystem/arm/dish-practice-type/add-rest"))
    {return;}
    initValue(pArray);


    // 16.公司信息

    sqlQuery.exec("select * from cey_sys_company;");

    while (sqlQuery.next()) {

        cJSON *pItem = cJSON_CreateObject();
        filter_cJSON_AddStringToObject(pItem, "ciVchCompany", QByteArray().append(sqlQuery.record().value("vch_company").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "ciVchPrincipal", QByteArray().append(sqlQuery.record().value("vch_principal").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "ciVchTel", QByteArray().append(sqlQuery.record().value("vch_tel").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "ciVchHandTel", QByteArray().append(sqlQuery.record().value("vch_handtel").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "ciVchEmail", QByteArray().append(sqlQuery.record().value("vch_email").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "ciVchAddress", QByteArray().append(sqlQuery.record().value("vch_address").toString()).data());

        filter_cJSON_AddStringToObject(pItem, "orgCode", QByteArray().append(orgCode).data());
        cJSON_AddItemToArray(pArray, pItem);
    }

    if(!execHttp(pArray,"http://" + hostName
                 + "/CateSystem/arm/dish-companyInfo/add-rest"))
    {return;}
    initValue(pArray);

    //17.菜品估清

    sqlQuery.exec("select * from cey_bt_dish_warn;");

    while (sqlQuery.next()) {

        cJSON *pItem = cJSON_CreateObject();
        cJSON_AddNumberToObject(pItem, "dgqIntFlowId",sqlQuery.record().value("int_flowID").toInt());
        filter_cJSON_AddStringToObject(pItem, "dgqDtDate", QByteArray().append(sqlQuery.record().value("dt_date").toDateTime().toString(dateFormatStr)).data());
        filter_cJSON_AddStringToObject(pItem, "dgqChDishNo", QByteArray().append(sqlQuery.record().value("ch_dishno").toString()).data());
        cJSON_AddNumberToObject(pItem, "dgqNumNum", sqlQuery.record().value("num_num").toFloat());
        cJSON_AddNumberToObject(pItem, "dgqNumSale", sqlQuery.record().value("num_sale").toFloat());
        filter_cJSON_AddStringToObject(pItem, "dgqChState", QByteArray().append(sqlQuery.record().value("ch_state").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "dgqVchOperId", QByteArray().append(sqlQuery.record().value("vch_operID").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "dgqDtOperDate", QByteArray().append(sqlQuery.record().value("dt_operdate").toDateTime().toString(dateFormatStr)).data());
        filter_cJSON_AddStringToObject(pItem, "dgqVchOperId2", QByteArray().append(sqlQuery.record().value("vch_operID2").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "dgqDtOperDate2", QByteArray().append(sqlQuery.record().value("dt_operdate2").toDateTime().toString(dateFormatStr)).data());


        filter_cJSON_AddStringToObject(pItem, "orgCode", QByteArray().append(orgCode).data());
        cJSON_AddItemToArray(pArray, pItem);
    }


    if(!execHttp(pArray,"http://" + hostName
                 + "/CateSystem/arm/dish-guqing/add-rest"))
    {return;}
    initValue(pArray);


    //18.支付方式

    sqlQuery.exec("select * from cey_bt_paymode;");

    while (sqlQuery.next()) {

        cJSON *pItem = cJSON_CreateObject();
        filter_cJSON_AddStringToObject(pItem, "pmChPayModeNo", QByteArray().append(sqlQuery.record().value("ch_paymodeno").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "pmVchPayModeName", QByteArray().append(sqlQuery.record().value("vch_paymodename").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "pmChIdFlag", QByteArray().append(sqlQuery.record().value("ch_IDflag").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "pmChFaceFlag", QByteArray().append(sqlQuery.record().value("ch_faceflag").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "pmChChangeFlag", QByteArray().append(sqlQuery.record().value("ch_changeflag").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "pmChInComeFlag", QByteArray().append(sqlQuery.record().value("ch_incomeflag").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "pmChKey", QByteArray().append(sqlQuery.record().value("ch_key").toString()).data());


        filter_cJSON_AddStringToObject(pItem, "orgCode", QByteArray().append(orgCode).data());
        cJSON_AddItemToArray(pArray, pItem);
    }


    if(!execHttp(pArray,"http://" + hostName
                 + "/CateSystem/arm/pay-mode/add-rest"))
    {return;}
    initValue(pArray);


    // 19.会员礼品资料维护

    sqlQuery.exec("select * from t_m_gift;");

    while (sqlQuery.next()) {

        cJSON *pItem = cJSON_CreateObject();
        cJSON_AddNumberToObject(pItem, "intFlowid", sqlQuery.record().value("int_flowID").toFloat());
        filter_cJSON_AddStringToObject(pItem, "chGiftno", QByteArray().append(sqlQuery.record().value("ch_giftno").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "vhcGifname", QByteArray().append(sqlQuery.record().value("vch_giftname").toString()).data());
        cJSON_AddNumberToObject(pItem, "numPoint", sqlQuery.record().value("num_point").toFloat());
        filter_cJSON_AddStringToObject(pItem, "orgCode", QByteArray().append(orgCode).data());
        cJSON_AddItemToArray(pArray, pItem);
    }


    if(!execHttp(pArray,"http://" + hostName
                 + "/CateSystem/arm/member-gift/add-rest"))
    {return;}
    initValue(pArray);

    // 20会员积分

    sqlQuery.exec("select * from t_m_curamount;");

    while (sqlQuery.next()) {

        cJSON *pItem = cJSON_CreateObject();
        filter_cJSON_AddStringToObject(pItem, "mcaVchMemberNo", QByteArray().append(sqlQuery.record().value("vch_memberno").toString()).data());
        cJSON_AddNumberToObject(pItem, "mcaNumAmount", sqlQuery.record().value("num_amount").toFloat());
        cJSON_AddNumberToObject(pItem, "mcaNumHangAmount", sqlQuery.record().value("num_hangamount").toFloat());
        cJSON_AddNumberToObject(pItem, "npNumPoint", sqlQuery.record().value("num_point").toFloat());
        filter_cJSON_AddStringToObject(pItem, "mcaVchOperId", QByteArray().append(sqlQuery.record().value("vch_operID").toDouble()).data());
        filter_cJSON_AddStringToObject(pItem, "mcaDtOperDate", QByteArray().append(sqlQuery.record().value("dt_operdate").toDateTime().toString(dateFormatStr)).data());
        filter_cJSON_AddStringToObject(pItem, "mcaChBranchNo", QByteArray().append(sqlQuery.record().value("ch_branchno").toDouble()).data());

        filter_cJSON_AddStringToObject(pItem, "orgCode", QByteArray().append(orgCode).data());
        cJSON_AddItemToArray(pArray, pItem);
    }
    if(!execHttp(pArray,"http://" + hostName
                 + "/CateSystem/arm/member-point/add-rest"))
    {return;}
    initValue(pArray);


    // 21.营业班次

    sqlQuery.exec("select * from cey_bt_class;");

    while (sqlQuery.next()) {

        cJSON *pItem = cJSON_CreateObject();
        filter_cJSON_AddStringToObject(pItem, "chClassno", QByteArray().append(sqlQuery.record().value("ch_classno").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "vchClassname", QByteArray().append(sqlQuery.record().value("vch_classname").toString()).data());
        filter_cJSON_AddStringToObject(pItem, "dtBegin", QByteArray().append(sqlQuery.record().value("dt_begin").toDateTime().toString(dateFormatStr)).data());

        filter_cJSON_AddStringToObject(pItem, "orgCode", QByteArray().append(orgCode).data());
        cJSON_AddItemToArray(pArray, pItem);
    }

    if(!execHttp(pArray,"http://" + hostName
                 + "/CateSystem/arm/dish-class/add-rest"))
    {return;}

    initValue(pArray);

    lds_messagebox::warning(0, tr("消息提示"),tr("上传成功！"), true);
    //    QMessageBox::critical(NULL, tr("信息"), tr("上传成功！"), QMessageBox::Yes, QMessageBox::Yes);

}


//****************************************************************************************************************************************************************************
//****************************************************************************************************************************************************************************
//****************************************************************************************************************************************************************************
//****************************************************************************************************************************************************************************
//****************************************************************************************************************************************************************************
//****************************************************************************************************************************************************************************
//****************************************************************************************************************************************************************************
//****************************************************************************************************************************************************************************

//基础数据下载
void cloudsync_service_help::downLoadBaseDate()
{
    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    stopFlag=false;
    isApp = conf.value("w_sys_manage_cloudsync_basedataset/isapp").toBool();
    index = 1;
    upSize = 21;
    QNetworkReply* reply;

    progressBar->setMaximum(upSize);

    QSqlQuery sqlQuery;
    cJSON *pArray;

    // 1 下载单位
    reply = sendGet("http://" + hostName
                    + "/CateSystem/arm/dish-unit/get-rest", headInfo);
    if(reply->error()==QNetworkReply::NoError ){
        pArray = cJSON_Parse(reply->readAll());
        QSqlDatabase::database().transaction();

        sqlQuery.exec("DELETE from cey_bt_unit;");

        for(int i =0;i<cJSON_GetArraySize(pArray);i++){
            cJSON *pItem = cJSON_GetArrayItem(pArray,i);

            sqlQuery.prepare(QString("insert into cey_bt_unit values(?,?,?,?)"));
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"duChUnitNo")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"duVchUnitName")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"duChNumType")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"duNumDefault")->valuedouble);

            if(isStop()){QSqlDatabase::database().rollback();cJSON_Delete(pArray);return;}if(!sqlQuery.exec()){

                QSqlDatabase::database().rollback();
                lds_messagebox::warning(0, tr("消息提示"),tr("错误\n") + sqlQuery.lastError().text());
                return;
            }

        }

        QSqlDatabase::database().commit();
    }else{
        lds_messagebox::warning(0, tr("消息提示"),tr("错误\n") + reply->errorString()+reply->readAll());
        return ;
    }
    initDownBaseDataValue(pArray);


    // 2 菜品信息
    reply = sendGet("http://" + hostName
                    + "/CateSystem/arm/dish-info/get-rest", headInfo);
    if(reply->error()==QNetworkReply::NoError ){
        pArray = cJSON_Parse(reply->readAll());

        //qDebug() << QString(cJSON_Print(pArray));

        QSqlDatabase::database().transaction();

        sqlQuery.exec("DELETE from cey_bt_dish;");

        for(int i =0;i<cJSON_GetArraySize(pArray);i++){
            cJSON *pItem = cJSON_GetArrayItem(pArray,i);

            sqlQuery.prepare(QString("insert into cey_bt_dish values(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?);"));
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"diChDishNo")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"diVchDishName")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"diVchSpell")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"diVchEnglish")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"diChDishTypeNo")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"diChUnitNo")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"diNumPrice")->valuedouble);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"diChSuitflag")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"diChDiscount")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"diChCurprice")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"diChTempflag")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"diChStopflag")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"diChPrintFlag")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"diVchPrinter")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"diChSubTypeNo")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"diChMaterialNo")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"diChServiceFlag")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"diChLowFlag")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"diNumMprice")->valuedouble);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"diChDeductFlag")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"diChDeductType")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"diIntDeduct")->valuedouble);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"diNumDeduct")->valuedouble);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"diChSaleFlag")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"diChSaleType")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"diIntSaleDeduct")->valuedouble);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"diNumSale")->valuedouble);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"itemFlag")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"diVchPurUnitNo")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"diIntUnitRate")->valuedouble);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"diChOff")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"diVchSubNo")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"diNumSalePrice")->valuedouble);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"diChOutFlag")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"diVchOutPrint")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"intRate")->valuedouble);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"chWeight")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"vchBarcode")->valuestring);


            if(isStop()){QSqlDatabase::database().rollback();cJSON_Delete(pArray);return;}if(!sqlQuery.exec()){

                QSqlDatabase::database().rollback();
                lds_messagebox::warning(0, tr("消息提示"),tr("错误\n") + sqlQuery.lastError().text());
                return;
            }
        }
        QSqlDatabase::database().commit();


    }else{
        lds_messagebox::warning(0, tr("消息提示"),tr("错误\n") + reply->errorString()+reply->readAll());
        return ;
    }
    initDownBaseDataValue(pArray);


    // 3 菜品小类
    reply = sendGet("http://" + hostName
                    + "/CateSystem/arm/dish-sub-type/get-rest", headInfo);
    if(reply->error()==QNetworkReply::NoError ){
        pArray = cJSON_Parse(reply->readAll());

        //qDebug() << QString(cJSON_Print(pArray));

        QSqlDatabase::database().transaction();

        sqlQuery.exec("DELETE from cey_bt_dish_subtype;");

        for(int i =0;i<cJSON_GetArraySize(pArray);i++){
            cJSON *pItem = cJSON_GetArrayItem(pArray,i);

            sqlQuery.prepare(QString("insert into cey_bt_dish_subtype values(?,?,?,?);"));
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"dstChSubTypeNo")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"dstVchSubTypeName")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"dstChDishTypeNo")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"dstChLagelPrintFlag")->valuestring);

            if(isStop()){QSqlDatabase::database().rollback();cJSON_Delete(pArray);return;}if(!sqlQuery.exec()){

                QSqlDatabase::database().rollback();
                lds_messagebox::warning(0, tr("消息提示"),tr("错误\n") + sqlQuery.lastError().text());
                return;
            }
        }
        QSqlDatabase::database().commit();


    }else{
        lds_messagebox::warning(0, tr("消息提示"),tr("错误\n")+reply->errorString()+reply->readAll());
        return ;
    }
    initDownBaseDataValue(pArray);


    // 4 菜品大类
    reply = sendGet("http://" + hostName
                    + "/CateSystem/arm/dish-type/get-rest", headInfo);
    if(reply->error()==QNetworkReply::NoError ){
        pArray = cJSON_Parse(reply->readAll());

        //qDebug() << QString(cJSON_Print(pArray));

        QSqlDatabase::database().transaction();

        sqlQuery.exec("DELETE from cey_bt_dish_type;");

        for(int i =0;i<cJSON_GetArraySize(pArray);i++){
            cJSON *pItem = cJSON_GetArrayItem(pArray,i);

            sqlQuery.prepare(QString("insert into cey_bt_dish_type values(?,?)"));
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"dtChDishTypeNo")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"dtVchDishTypeName")->valuestring);


            if(isStop()){QSqlDatabase::database().rollback();cJSON_Delete(pArray);return;}if(!sqlQuery.exec()){

                QSqlDatabase::database().rollback();
                lds_messagebox::warning(0, tr("消息提示"),tr("错误\n") + sqlQuery.lastError().text());


                return;
            }
        }
        QSqlDatabase::database().commit();


    }else{
        lds_messagebox::warning(0, tr("消息提示"),tr("错误\n")+reply->errorString()+reply->readAll());
        return ;
    }
    initDownBaseDataValue(pArray);


    // 5 菜品套餐
    reply = sendGet("http://" + hostName
                    + "/CateSystem/arm/dish-suit/get-rest", headInfo);
    if(reply->error()==QNetworkReply::NoError ){
        pArray = cJSON_Parse(reply->readAll());

        //qDebug() << QString(cJSON_Print(pArray));

        QSqlDatabase::database().transaction();

        sqlQuery.exec("DELETE from cey_bt_dish_suit;");

        for(int i =0;i<cJSON_GetArraySize(pArray);i++){
            cJSON *pItem = cJSON_GetArrayItem(pArray,i);

            sqlQuery.prepare(QString("insert into cey_bt_dish_suit(int_flowid,ch_suitno,ch_dishno,num_num,ch_dishno_chang,num_num_change) values(?,?,?,?,?,?)"));
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"dsIntFlowId")->valueint);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"dsChSuitNo")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"dsChDishNo")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"dsNumNum")->valuedouble);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"dsChDishNoChang")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"dsNumNumChange")->valuedouble);

            if(isStop()){QSqlDatabase::database().rollback();cJSON_Delete(pArray);return;}if(!sqlQuery.exec()){

                QSqlDatabase::database().rollback();
                lds_messagebox::warning(0, tr("消息提示"),tr("错误\n") + sqlQuery.lastError().text());


                return;
            }
        }
        QSqlDatabase::database().commit();


    }else{
        lds_messagebox::warning(0, tr("消息提示"),tr("错误\n")+reply->errorString()+reply->readAll());
        return ;
    }
    initDownBaseDataValue(pArray);


    // 6 菜品套菜替换
    reply = sendGet("http://" + hostName
                    + "/CateSystem/arm/dish-suit-change/get-rest", headInfo);
    if(reply->error()==QNetworkReply::NoError ){
        pArray = cJSON_Parse(reply->readAll());

        //qDebug() << QString(cJSON_Print(pArray));

        QSqlDatabase::database().transaction();

        sqlQuery.exec("DELETE from cey_bt_suit_change;");

        for(int i =0;i<cJSON_GetArraySize(pArray);i++){
            cJSON *pItem = cJSON_GetArrayItem(pArray,i);

            sqlQuery.prepare(QString("insert into cey_bt_suit_change values(?,?,?)"));
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"dscIntFlowId")->valueint);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"dscChDishNo")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"dscNumNumChange")->valuedouble);


            if(isStop()){QSqlDatabase::database().rollback();cJSON_Delete(pArray);return;}if(!sqlQuery.exec()){

                QSqlDatabase::database().rollback();
                lds_messagebox::warning(0, tr("消息提示"),tr("错误\n") + sqlQuery.lastError().text());


                return;
            }
        }
        QSqlDatabase::database().commit();


    }else{
        lds_messagebox::warning(0, tr("消息提示"),tr("错误\n")+reply->errorString()+reply->readAll());
        return ;
    }
    initDownBaseDataValue(pArray);


    // 7 餐桌管理
    reply = sendGet("http://" + hostName
                    + "/CateSystem/arm/dish-table/get-rest", headInfo);
    if(reply->error()==QNetworkReply::NoError ){
        pArray = cJSON_Parse(reply->readAll());

        //qDebug() << QString(cJSON_Print(pArray));

        QSqlDatabase::database().transaction();

        sqlQuery.exec("DELETE from cey_bt_table;");

        for(int i =0;i<cJSON_GetArraySize(pArray);i++){
            cJSON *pItem = cJSON_GetArrayItem(pArray,i);

            sqlQuery.prepare(QString("insert into cey_bt_table values(?,?,?,?,?,?,?,?,?,?);"));
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"dtChTableNo")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"dtVchTableName")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"dtChTypeNo")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"dtChBillNo")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"dtChLockFlag")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"dtIntPerson")->valueint);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"dtVchMemo")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"dtChState")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"dtDtDateTime")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"dtChAreaNo")->valuestring);


            if(isStop()){QSqlDatabase::database().rollback();cJSON_Delete(pArray);return;}if(!sqlQuery.exec()){

                QSqlDatabase::database().rollback();
                lds_messagebox::warning(0, tr("消息提示"),tr("错误\n") + sqlQuery.lastError().text());


                return;
            }
        }
        QSqlDatabase::database().commit();


    }else{
        lds_messagebox::warning(0, tr("消息提示"),tr("错误\n")+reply->errorString()+reply->readAll());
        return ;
    }
    initDownBaseDataValue(pArray);


    //  8 餐桌类型
    reply = sendGet("http://" + hostName
                    + "/CateSystem/arm/dish-table-type/get-rest", headInfo);
    if(reply->error()==QNetworkReply::NoError ){
        pArray = cJSON_Parse(reply->readAll());

        //qDebug() << QString(cJSON_Print(pArray));

        QSqlDatabase::database().transaction();

        sqlQuery.exec("DELETE from cey_bt_table_type;");

        for(int i =0;i<cJSON_GetArraySize(pArray);i++){
            cJSON *pItem = cJSON_GetArrayItem(pArray,i);

            sqlQuery.prepare(QString("insert into cey_bt_table_type values(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?);"));
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"dttChTyperNo")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"dttVchTypeName")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"dttChPriceKind")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"dttChServiceMode")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"dttChServiceFlag")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"dttIntStartMinute")->valueint);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"dttNumStartAmount")->valuedouble);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"dttIntUnitMinute")->valueint);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"dttNumUnitAmount")->valuedouble);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"dttIntOver")->valueint);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"dttNumRation")->valuedouble);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"dttIntRate")->valueint);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"dttNumMaxService")->valuedouble);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"dttChLowMode")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"dttNumLowCost")->valuedouble);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"dttIntBeginTime")->valueint);

            if(isStop()){QSqlDatabase::database().rollback();cJSON_Delete(pArray);return;}if(!sqlQuery.exec()){

                QSqlDatabase::database().rollback();
                lds_messagebox::warning(0, tr("消息提示"),tr("错误\n") + sqlQuery.lastError().text());


                return;
            }
        }
        QSqlDatabase::database().commit();


    }else{
        lds_messagebox::warning(0, tr("消息提示"),tr("错误\n")+reply->errorString()+reply->readAll());
        return ;
    }
    initDownBaseDataValue(pArray);


    // 9 餐区信息
    reply = sendGet("http://" + hostName
                    + "/CateSystem/arm/dish-table-area/get-rest", headInfo);
    if(reply->error()==QNetworkReply::NoError ){
        pArray = cJSON_Parse(reply->readAll());

        //qDebug() << QString(cJSON_Print(pArray));

        QSqlDatabase::database().transaction();

        sqlQuery.exec("DELETE from cey_bt_table_area;");

        for(int i =0;i<cJSON_GetArraySize(pArray);i++){
            cJSON *pItem = cJSON_GetArrayItem(pArray,i);

            sqlQuery.prepare(QString("insert into cey_bt_table_area values(?,?);"));
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"dtaChAreaNo")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"dtaVchAreaName")->valuestring);


            if(isStop()){QSqlDatabase::database().rollback();cJSON_Delete(pArray);return;}if(!sqlQuery.exec()){

                QSqlDatabase::database().rollback();
                lds_messagebox::warning(0, tr("消息提示"),tr("错误\n") + sqlQuery.lastError().text());


                return;
            }
        }
        QSqlDatabase::database().commit();


    }else{
        lds_messagebox::warning(0, tr("消息提示"),tr("错误\n")+reply->errorString()+reply->readAll());
        return ;
    }
    initDownBaseDataValue(pArray);


    // 10 会员信息
    reply = sendGet("http://" + hostName
                    + "/CateSystem/arm/member-info/get-rest", headInfo);
    if(reply->error()==QNetworkReply::NoError ){
        pArray = cJSON_Parse(reply->readAll());

        //qDebug() << QString(cJSON_Print(pArray));

        QSqlDatabase::database().transaction();

        sqlQuery.exec("DELETE from t_m_member;");

        for(int i =0;i<cJSON_GetArraySize(pArray);i++){
            cJSON *pItem = cJSON_GetArrayItem(pArray,i);

            sqlQuery.prepare(QString("insert into t_m_member values(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?);"));
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"miVchMemberNo")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"miVchMember")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"miChTypeNo")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"miVchId")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"miDtBirthday")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"miVchTel")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"miVchHandTel")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"miVchAddress")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"miDtLimit")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"miChState")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"numLimit")->valuedouble);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"miChCardFlag")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"miVchCardNo")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"miVchPassword")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"miVchOperId")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"miDtOperDate")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"miIntBasedata")->valueint);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"miDtSendTime")->valuestring);


            if(isStop()){QSqlDatabase::database().rollback();cJSON_Delete(pArray);return;}if(!sqlQuery.exec()){

                QSqlDatabase::database().rollback();
                lds_messagebox::warning(0, tr("消息提示"),tr("错误\n") + sqlQuery.lastError().text());


                return;
            }
        }
        QSqlDatabase::database().commit();


    }else{
        lds_messagebox::warning(0, tr("消息提示"),tr("错误\n")+reply->errorString()+reply->readAll());
        return ;
    }
    initDownBaseDataValue(pArray);


    // 11 会员类型
    reply = sendGet("http://" + hostName
                    + "/CateSystem/arm/member-type/get-rest", headInfo);
    if(reply->error()==QNetworkReply::NoError ){
        pArray = cJSON_Parse(reply->readAll());

        //qDebug() << QString(cJSON_Print(pArray));

        QSqlDatabase::database().transaction();

        sqlQuery.exec("DELETE from t_m_member_type;");

        for(int i =0;i<cJSON_GetArraySize(pArray);i++){
            cJSON *pItem = cJSON_GetArrayItem(pArray,i);

            sqlQuery.prepare(QString("insert into t_m_member_type values(?,?,?,?,?,?,?,?,?,?,?,?,?)"));
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"chTypeno")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"vchTypename")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"chAttribute")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"chFavourable")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"chPricekind")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"intDiscount")->valueint);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"chProjectno")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"chCalc")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"chCalcType")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"intCalcUnit")->valueint);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"chAreaFlag")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"intBackcashRate")->valueint);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"intCardBaseamount")->valueint);


            if(isStop()){QSqlDatabase::database().rollback();cJSON_Delete(pArray);return;}if(!sqlQuery.exec()){

                QSqlDatabase::database().rollback();
                lds_messagebox::warning(0, tr("消息提示"),tr("错误\n") + sqlQuery.lastError().text());


                return;
            }
        }
        QSqlDatabase::database().commit();


    }else{
        lds_messagebox::warning(0, tr("消息提示"),tr("错误\n")+reply->errorString()+reply->readAll());
        return ;
    }
    initDownBaseDataValue(pArray);


    // 12 会员参数
    reply = sendGet("http://" + hostName
                    + "/CateSystem/arm/member-parameter/get-rest", headInfo);
    if(reply->error()==QNetworkReply::NoError ){
        pArray = cJSON_Parse(reply->readAll());

        //qDebug() << QString(cJSON_Print(pArray));

        QSqlDatabase::database().transaction();

        sqlQuery.exec("DELETE from t_m_parameter;");

        for(int i =0;i<cJSON_GetArraySize(pArray);i++){
            cJSON *pItem = cJSON_GetArrayItem(pArray,i);

            sqlQuery.prepare(QString("insert into t_m_parameter values(?,?,?);"));
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"vchParameter")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"vchValue")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"vchExplain")->valuestring);

            if(isStop()){QSqlDatabase::database().rollback();cJSON_Delete(pArray);return;}if(!sqlQuery.exec()){

                QSqlDatabase::database().rollback();
                lds_messagebox::warning(0, tr("消息提示"),tr("错误\n") + sqlQuery.lastError().text());
                return;
            }
        }
        QSqlDatabase::database().commit();


    }else{
        lds_messagebox::warning(0, tr("消息提示"),tr("错误\n")+reply->errorString()+reply->readAll());
        return ;
    }
    initDownBaseDataValue(pArray);


    // 13 特殊做法
    reply = sendGet("http://" + hostName
                    + "/CateSystem/arm/dish-practice/get-rest", headInfo);
    if(reply->error()==QNetworkReply::NoError ){
        pArray = cJSON_Parse(reply->readAll());

        //qDebug() << QString(cJSON_Print(pArray));

        QSqlDatabase::database().transaction();

        sqlQuery.exec("DELETE from cey_bt_cp_dish;");

        for(int i =0;i<cJSON_GetArraySize(pArray);i++){
            cJSON *pItem = cJSON_GetArrayItem(pArray,i);

            sqlQuery.prepare(QString("insert into cey_bt_cp_dish values(?,?)"));
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"dpChDishNo")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"dpChNo")->valuestring);

            if(isStop()){QSqlDatabase::database().rollback();cJSON_Delete(pArray);return;}if(!sqlQuery.exec()){

                QSqlDatabase::database().rollback();
                lds_messagebox::warning(0, tr("消息提示"),tr("错误\n") + sqlQuery.lastError().text());


                return;
            }
        }
        QSqlDatabase::database().commit();


    }else{
        lds_messagebox::warning(0, tr("消息提示"),tr("错误\n")+reply->errorString()+reply->readAll());
        return ;
    }
    initDownBaseDataValue(pArray);


    // 14 做法信息
    reply = sendGet("http://" + hostName
                    + "/CateSystem/arm/dish-practice-memo/get-rest", headInfo);
    if(reply->error()==QNetworkReply::NoError ){
        pArray = cJSON_Parse(reply->readAll());

        //qDebug() << QString(cJSON_Print(pArray));

        QSqlDatabase::database().transaction();

        sqlQuery.exec("DELETE from cey_bt_cp_memo;");

        for(int i =0;i<cJSON_GetArraySize(pArray);i++){
            cJSON *pItem = cJSON_GetArrayItem(pArray,i);

            sqlQuery.prepare(QString("insert into cey_bt_cp_memo values(?,?,?,?);"));
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"chNo")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"vchCpMemo")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"chTypeno")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"numAddPrice")->valuedouble);

            if(isStop()){QSqlDatabase::database().rollback();cJSON_Delete(pArray);return;}if(!sqlQuery.exec()){

                QSqlDatabase::database().rollback();
                lds_messagebox::warning(0, tr("消息提示"),tr("错误\n") + sqlQuery.lastError().text());


                return;
            }
        }
        QSqlDatabase::database().commit();


    }else{
        lds_messagebox::warning(0, tr("消息提示"),tr("错误\n")+reply->errorString()+reply->readAll());
        return ;
    }
    initDownBaseDataValue(pArray);


    // 15 做法类型
    reply = sendGet("http://" + hostName
                    + "/CateSystem/arm/dish-practice-type/get-rest", headInfo);
    if(reply->error()==QNetworkReply::NoError ){
        pArray = cJSON_Parse(reply->readAll());

        //qDebug() << QString(cJSON_Print(pArray));

        QSqlDatabase::database().transaction();

        sqlQuery.exec("DELETE from cey_bt_cp_type;");

        for(int i =0;i<cJSON_GetArraySize(pArray);i++){
            cJSON *pItem = cJSON_GetArrayItem(pArray,i);

            sqlQuery.prepare(QString("insert into cey_bt_cp_type values(?,?);"));
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"dptChTypeNo")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"dptVchTypeName")->valuestring);

            if(isStop()){QSqlDatabase::database().rollback();cJSON_Delete(pArray);return;}if(!sqlQuery.exec()){

                QSqlDatabase::database().rollback();
                lds_messagebox::warning(0, tr("消息提示"),tr("错误\n") + sqlQuery.lastError().text());


                return;
            }
        }
        QSqlDatabase::database().commit();


    }else{
        lds_messagebox::warning(0, tr("消息提示"),tr("错误\n")+reply->errorString()+reply->readAll());
        return ;
    }
    initDownBaseDataValue(pArray);


    // 16 公司信息
    reply = sendGet("http://" + hostName
                    + "/CateSystem/arm/dish-companyInfo/get-rest", headInfo);
    if(reply->error()==QNetworkReply::NoError ){
        pArray = cJSON_Parse(reply->readAll());

        //qDebug() << QString(cJSON_Print(pArray));

        QSqlDatabase::database().transaction();

        sqlQuery.exec("DELETE from cey_sys_company;");

        for(int i =0;i<cJSON_GetArraySize(pArray);i++){
            cJSON *pItem = cJSON_GetArrayItem(pArray,i);

            sqlQuery.prepare(QString("insert into cey_sys_company values(?,?,?,?,?,?);"));
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"ciVchCompany")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"ciVchPrincipal")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"ciVchTel")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"ciVchHandTel")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"ciVchEmail")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"ciVchAddress")->valuestring);

            if(isStop()){QSqlDatabase::database().rollback();cJSON_Delete(pArray);return;}if(!sqlQuery.exec()){

                QSqlDatabase::database().rollback();
                lds_messagebox::warning(0, tr("消息提示"),tr("错误\n") + sqlQuery.lastError().text());


                return;
            }
        }
        QSqlDatabase::database().commit();


    }else{
        lds_messagebox::warning(0, tr("消息提示"),tr("错误\n")+reply->errorString()+reply->readAll());
        return ;
    }
    initDownBaseDataValue(pArray);


    // 17 菜品估清
    reply = sendGet("http://" + hostName
                    + "/CateSystem/arm/dish-guqing/get-rest", headInfo);
    if(reply->error()==QNetworkReply::NoError ){
        pArray = cJSON_Parse(reply->readAll());

        //qDebug() << QString(cJSON_Print(pArray));

        QSqlDatabase::database().transaction();

        sqlQuery.exec("DELETE from cey_bt_dish_warn;");

        for(int i =0;i<cJSON_GetArraySize(pArray);i++){
            cJSON *pItem = cJSON_GetArrayItem(pArray,i);

            sqlQuery.prepare(QString("insert into cey_bt_dish_warn(int_flowID,dt_date,ch_dishno,num_num,num_sale,ch_state,vch_operID,dt_operdate,vch_operID2,dt_operdate2) values(?,?,?,?,?,?,?,?,?,?);"));
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"dgqIntFlowId")->valuedouble);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"dgqDtDate")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"dgqChDishNo")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"dgqNumNum")->valuedouble);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"dgqNumSale")->valuedouble);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"dgqChState")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"dgqVchOperId")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"dgqDtOperDate")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"dgqVchOperId2")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"dgqDtOperDate2")->valuestring);

            if(isStop()){QSqlDatabase::database().rollback();cJSON_Delete(pArray);return;}if(!sqlQuery.exec()){

                QSqlDatabase::database().rollback();
                lds_messagebox::warning(0, tr("消息提示"),tr("错误\n") + sqlQuery.lastError().text());


                return;
            }
        }
        QSqlDatabase::database().commit();


    }else{
        lds_messagebox::warning(0, tr("消息提示"),tr("错误\n")+reply->errorString()+reply->readAll());
        return ;
    }
    initDownBaseDataValue(pArray);


    // 18 支付方式
    reply = sendGet("http://" + hostName
                    + "/CateSystem/arm/pay-mode/get-rest", headInfo);
    if(reply->error()==QNetworkReply::NoError ){
        pArray = cJSON_Parse(reply->readAll());

        //qDebug() << QString(cJSON_Print(pArray));

        QSqlDatabase::database().transaction();

        sqlQuery.exec("DELETE from cey_bt_paymode;");

        for(int i =0;i<cJSON_GetArraySize(pArray);i++){
            cJSON *pItem = cJSON_GetArrayItem(pArray,i);

            sqlQuery.prepare(QString("insert into cey_bt_paymode values(?,?,?,?,?,?,?)"));
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"pmChPayModeNo")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"pmVchPayModeName")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"pmChIdFlag")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"pmChFaceFlag")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"pmChChangeFlag")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"pmChInComeFlag")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"pmChKey")->valuestring);


            if(isStop()){QSqlDatabase::database().rollback();cJSON_Delete(pArray);return;}if(!sqlQuery.exec()){

                QSqlDatabase::database().rollback();
                lds_messagebox::warning(0, tr("消息提示"),tr("错误\n") + sqlQuery.lastError().text());


                return;
            }
        }
        QSqlDatabase::database().commit();


    }else{
        lds_messagebox::warning(0, tr("消息提示"),tr("错误\n")+reply->errorString()+reply->readAll());
        return ;
    }
    initDownBaseDataValue(pArray);


    // 19.会员礼品资料维护
    reply = sendGet("http://" + hostName
                    + "/CateSystem/arm/member-gift/get-rest", headInfo);
    if(reply->error()==QNetworkReply::NoError ){
        pArray = cJSON_Parse(reply->readAll());

        //qDebug() << QString(cJSON_Print(pArray));

        QSqlDatabase::database().transaction();

        sqlQuery.exec("DELETE from t_m_gift;");

        for(int i =0;i<cJSON_GetArraySize(pArray);i++){
            cJSON *pItem = cJSON_GetArrayItem(pArray,i);

            sqlQuery.prepare(QString("insert into t_m_gift(int_flowID,ch_giftno,vch_giftname,num_point) values(?,?,?,?)"));
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"intFlowid")->valuedouble);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"chGiftno")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"vhcGifname")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"numPoint")->valuedouble);


            if(isStop()){QSqlDatabase::database().rollback();cJSON_Delete(pArray);return;}if(!sqlQuery.exec()){

                QSqlDatabase::database().rollback();
                lds_messagebox::warning(0, tr("消息提示"),tr("错误\n") + sqlQuery.lastError().text());


                return;
            }
        }
        QSqlDatabase::database().commit();


    }else{
        lds_messagebox::warning(0, tr("消息提示"),tr("错误\n")+reply->errorString()+reply->readAll());
        return ;
    }
    initDownBaseDataValue(pArray);


    // 20.会员积分
    reply = sendGet("http://" + hostName
                    + "/CateSystem/arm/member-point/get-rest", headInfo);
    if(reply->error()==QNetworkReply::NoError ){
        pArray = cJSON_Parse(reply->readAll());

        //qDebug() << QString(cJSON_Print(pArray));

        QSqlDatabase::database().transaction();

        sqlQuery.exec("DELETE from t_m_curamount;");

        for(int i =0;i<cJSON_GetArraySize(pArray);i++){
            cJSON *pItem = cJSON_GetArrayItem(pArray,i);

            sqlQuery.prepare(QString("insert into t_m_curamount values(?,?,?,?,?,?,?);"));
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"mcaVchMemberNo")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"mcaNumAmount")->valuedouble);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"mcaNumHangAmount")->valuedouble);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"npNumPoint")->valuedouble);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"mcaVchOperId")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"mcaDtOperDate")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"mcaChBranchNo")->valuestring);

            if(isStop()){QSqlDatabase::database().rollback();cJSON_Delete(pArray);return;}if(!sqlQuery.exec()){

                QSqlDatabase::database().rollback();
                lds_messagebox::warning(0, tr("消息提示"),tr("错误\n") + sqlQuery.lastError().text());


                return;
            }
        }
        QSqlDatabase::database().commit();


    }else{
        lds_messagebox::warning(0, tr("消息提示"),tr("错误\n")+reply->errorString()+reply->readAll());
        return ;
    }
    initDownBaseDataValue(pArray);


    // 21.营业班次
    reply = sendGet("http://" + hostName
                    + "/CateSystem/arm/dish-class/get-rest", headInfo);
    if(reply->error()==QNetworkReply::NoError ){
        pArray = cJSON_Parse(reply->readAll());

        //qDebug() << QString(cJSON_Print(pArray));

        QSqlDatabase::database().transaction();

        sqlQuery.exec("DELETE from cey_bt_class;");

        for(int i =0;i<cJSON_GetArraySize(pArray);i++){
            cJSON *pItem = cJSON_GetArrayItem(pArray,i);

            sqlQuery.prepare(QString("insert into cey_bt_class values(?,?,?);"));
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"chClassno")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"vchClassname")->valuestring);
            sqlQuery.addBindValue(cJSON_GetObjectItem(pItem,"dtBegin")->valuestring);

            if(isStop()){QSqlDatabase::database().rollback();cJSON_Delete(pArray);return;}if(!sqlQuery.exec()){

                QSqlDatabase::database().rollback();
                lds_messagebox::warning(0, tr("消息提示"),tr("错误\n") + sqlQuery.lastError().text());


                return;
            }
        }
        QSqlDatabase::database().commit();


    }else{
        lds_messagebox::warning(0, tr("消息提示"),tr("错误\n")+reply->errorString()+reply->readAll());
        return ;
    }
    initDownBaseDataValue(pArray);

    lds_messagebox::warning(0, tr("消息提示"),tr("基础数据下载成功！"), true);
    //    QMessageBox::critical(NULL, tr("信息"), tr("基础数据下载成功！"), QMessageBox::Yes, QMessageBox::Yes);
}



//****************************************************************************************************************************************************************************
//****************************************************************************************************************************************************************************
//****************************************************************************************************************************************************************************
//****************************************************************************************************************************************************************************
//****************************************************************************************************************************************************************************
//****************************************************************************************************************************************************************************
//****************************************************************************************************************************************************************************
//****************************************************************************************************************************************************************************



//上传流水数据
void cloudsync_service_help::upLoadFlowDate(QString date1, QString date2)
{
    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    stopFlag=false;
    index = 1;
    upSize = 0;
    qlonglong curstep=0; isApp = false;//开始默认不是追加是覆盖

    QSqlQuery sqlQuery;
    cJSON *pArray = cJSON_CreateArray();

    QString dateTime1 = date1 + " 00:00:00";
    QString dateTime2 = date2 + " 23:59:59";
    headInfo.insert("dataTimeStart",date1);
    headInfo.insert("dataTimeEnd",date2);

    if (conf.value("w_sys_manage_cloudsync_basedataset/membership_data_flag",true).toBool())
        upSize += 3;

    if (conf.value("w_sys_manage_cloudsync_basedataset/sales_data_flag",true).toBool())
        upSize += 8;

    progressBar->setMaximum(upSize);


    if (conf.value("w_sys_manage_cloudsync_basedataset/membership_data_flag",true).toBool()){


        // 1.会员营业数据
        sqlQuery.exec("select * from t_m_deposit WHERE dt_operdate >= '"
                      + dateTime1
                      + "' and dt_operdate <= '"
                      + dateTime2 + "';");

        while (sqlQuery.next()) {

            cJSON *pItem = cJSON_CreateObject();
            cJSON_AddNumberToObject(pItem, "mdIntFlowId", sqlQuery.record().value("int_flowID").toInt());
            filter_cJSON_AddStringToObject(pItem, "mdVchMemberNo", QByteArray().append(sqlQuery.record().value("vch_memberno").toString()).data());
            cJSON_AddNumberToObject(pItem, "mdNumDeposit", sqlQuery.record().value("num_deposit").toFloat());
            cJSON_AddNumberToObject(pItem, "mdNumRealaMount", sqlQuery.record().value("num_realamount").toFloat());
            filter_cJSON_AddStringToObject(pItem, "mdChDepositMode", QByteArray().append(sqlQuery.record().value("ch_deposit_mode").toString()).data());
            filter_cJSON_AddStringToObject(pItem, "mdChPayMode", QByteArray().append(sqlQuery.record().value("ch_pay_mode").toString()).data());
            filter_cJSON_AddStringToObject(pItem, "mdVchOperId", QByteArray().append(sqlQuery.record().value("vch_operID").toString()).data());
            filter_cJSON_AddStringToObject(pItem, "dtOperDate", QByteArray().append(sqlQuery.record().value("dt_operdate").toDateTime().toString(dateFormatStr)).data());
            filter_cJSON_AddStringToObject(pItem, "mdVchMemo", QByteArray().append(sqlQuery.record().value("vch_memo").toString()).data());
            cJSON_AddNumberToObject(pItem, "mdRemainAmt", sqlQuery.record().value("remain_amt").toFloat());

            filter_cJSON_AddStringToObject(pItem, "orgCode", QByteArray().append(orgCode).data());
            cJSON_AddItemToArray(pArray, pItem);

            curstep++;qDebug() << "----------------------------in"<<curstep;
            if(curstep==listCount){
                if(!execHttp(pArray,"http://" + hostName
                             + "/CateSystem/arm/member-deposit/add-rest"))
                {return;}
                if(isApp==false)isApp=true;
                curstep=0;
            }
        }

        {
            if(!execHttp(pArray,"http://" + hostName
                         + "/CateSystem/arm/member-deposit/add-rest"))
            {return;}
            curstep=0;isApp=false;
        }
        initValue(pArray);


        // 2.会员积分操作明细
        sqlQuery.exec("select * from t_m_point WHERE dt_operdate >= '"
                      + dateTime1
                      + "' and dt_operdate <= '"
                      + dateTime2 + "';");

        while (sqlQuery.next()) {

            cJSON *pItem = cJSON_CreateObject();
            cJSON_AddNumberToObject(pItem, "mpIntFlowId", sqlQuery.record().value("int_flowID").toInt());
            filter_cJSON_AddStringToObject(pItem, "mpVchMemberNo", QByteArray().append(sqlQuery.record().value("vch_memberno").toString()).data());
            filter_cJSON_AddStringToObject(pItem, "mpChType", QByteArray().append(sqlQuery.record().value("ch_type").toString()).data());
            filter_cJSON_AddStringToObject(pItem, "mpChPayNo", QByteArray().append(sqlQuery.record().value("ch_payno").toString()).data());
            cJSON_AddNumberToObject(pItem, "mpNumPoint", sqlQuery.record().value("num_point").toFloat());
            filter_cJSON_AddStringToObject(pItem, "mpVchMemo", QByteArray().append(sqlQuery.record().value("vch_memo").toString()).data());
            filter_cJSON_AddStringToObject(pItem, "mpVchOperId", QByteArray().append(sqlQuery.record().value("vch_operID").toString()).data());
            filter_cJSON_AddStringToObject(pItem, "mpDtOperDate", QByteArray().append(sqlQuery.record().value("dt_operdate").toDateTime().toString(dateFormatStr)).data());

            filter_cJSON_AddStringToObject(pItem, "orgCode", QByteArray().append(orgCode).data());
            cJSON_AddItemToArray(pArray, pItem);

            curstep++;qDebug() << "----------------------------in"<<curstep;
            if(curstep==listCount){
                if(!execHttp(pArray,"http://" + hostName
                             + "/CateSystem/arm/member-point-list/add-rest"))
                {return;}
                curstep=0;
            }
        }

        {
            if(!execHttp(pArray,"http://" + hostName
                         + "/CateSystem/arm/member-point-list/add-rest"))
            {return;}
            curstep=0;isApp=false;
        }
        initValue(pArray);



        // 3.会员消费记录
        sqlQuery.exec("select * from t_m_pay WHERE dt_operdate >= '"
                      + dateTime1
                      + "' and dt_operdate <= '"
                      + dateTime2 + "';");

        while (sqlQuery.next()) {

            cJSON *pItem = cJSON_CreateObject();
            cJSON_AddNumberToObject(pItem, "intFlowid", sqlQuery.record().value("int_flowID").toFloat());
            filter_cJSON_AddStringToObject(pItem, "mpVchMemberNo", QByteArray().append(sqlQuery.record().value("vch_memberno").toString()).data());
            filter_cJSON_AddStringToObject(pItem, "mpChPayNo", QByteArray().append(sqlQuery.record().value("ch_payno").toString()).data());
            filter_cJSON_AddStringToObject(pItem, "mpChPayModeNo", QByteArray().append(sqlQuery.record().value("ch_paymodeno").toString()).data());

            cJSON_AddNumberToObject(pItem, "mpNumAmount", sqlQuery.record().value("num_amount").toFloat());
            cJSON_AddNumberToObject(pItem, "mpNumPay", sqlQuery.record().value("num_pay").toFloat());

            filter_cJSON_AddStringToObject(pItem, "mpVchOperId", QByteArray().append(sqlQuery.record().value("vch_operID").toString()).data());
            filter_cJSON_AddStringToObject(pItem, "mpDtOperDate", QByteArray().append(sqlQuery.record().value("dt_operdate").toDateTime().toString(dateFormatStr)).data());
            filter_cJSON_AddStringToObject(pItem, "mpChBalanceNo", QByteArray().append(sqlQuery.record().value("ch_balanceno").toString()).data());
            cJSON_AddNumberToObject(pItem, "mpIntCheckFlow", sqlQuery.record().value("int_checkflow").toInt());

            filter_cJSON_AddStringToObject(pItem, "orgCode", QByteArray().append(orgCode).data());
            cJSON_AddItemToArray(pArray, pItem);

            curstep++;qDebug() << "----------------------------in"<<curstep;
            if(curstep==listCount){
                if(!execHttp(pArray,"http://" + hostName
                             + "/CateSystem/arm/member-pay/add-rest"))
                {return;}
                curstep=0;
            }
        }
        {
            if(!execHttp(pArray,"http://" + hostName
                         + "/CateSystem/arm/member-pay/add-rest"))
            {return;}
            curstep=0;isApp=false;
        }
        initValue(pArray);
    }

    // 销售营业数据
    if (conf.value("w_sys_manage_cloudsync_basedataset/sales_data_flag",true).toBool()){

        // 4.服务费低消费
        sqlQuery.exec("select * from cey_u_service_low WHERE dt_operdate >= '"
                      + dateTime1
                      + "' and dt_operdate <= '"
                      + dateTime2 + "';");

        while (sqlQuery.next()) {

            cJSON *pItem = cJSON_CreateObject();
            filter_cJSON_AddStringToObject(pItem, "slChPayNo", QByteArray().append(sqlQuery.record().value("ch_payno").toString()).data());
            filter_cJSON_AddStringToObject(pItem, "slChType", QByteArray().append(sqlQuery.record().value("ch_type").toString()).data());
            cJSON_AddNumberToObject(pItem, "slNumAmount", sqlQuery.record().value("num_amount").toFloat());
            filter_cJSON_AddStringToObject(pItem, "slVchOperId", QByteArray().append(sqlQuery.record().value("vch_operID").toString()).data());
            filter_cJSON_AddStringToObject(pItem, "slDtOperDate", QByteArray().append(sqlQuery.record().value("dt_operdate").toDateTime().toString(dateFormatStr)).data());
            filter_cJSON_AddStringToObject(pItem, "slChState", QByteArray().append(sqlQuery.record().value("ch_state").toString()).data());
            filter_cJSON_AddStringToObject(pItem, "slVchMemo", QByteArray().append(sqlQuery.record().value("vch_memo").toString()).data());

            filter_cJSON_AddStringToObject(pItem, "orgCode", QByteArray().append(orgCode).data());
            cJSON_AddItemToArray(pArray, pItem);
            curstep++;qDebug() << "----------------------------in"<<curstep;
            if(curstep==listCount){
                if(!execHttp(pArray,"http://" + hostName
                             + "/CateSystem/arm/service-low/add-rest"))
                {return;}
                curstep=0;
            }
        }

        {
            if(!execHttp(pArray,"http://" + hostName
                         + "/CateSystem/arm/service-low/add-rest"))
            {return;}
            curstep=0;isApp=false;
        }
        initValue(pArray);



        // 5.日结记录表
        sqlQuery.exec("select * from cey_u_day_sumup WHERE dt_operdate >= '"
                      + dateTime1
                      + "' and dt_operdate <= '"
                      + dateTime2 + "';");

        while (sqlQuery.next()) {

            cJSON *pItem = cJSON_CreateObject();
            filter_cJSON_AddStringToObject(pItem, "dtDate", QByteArray().append(sqlQuery.record().value("dt_date").toDateTime().toString(dateFormatStr)).data());
            filter_cJSON_AddStringToObject(pItem, "dtBegin", QByteArray().append(sqlQuery.record().value("dt_begin").toDateTime().toString(dateFormatStr)).data());
            filter_cJSON_AddStringToObject(pItem, "dtEnd", QByteArray().append(sqlQuery.record().value("dt_end").toDateTime().toString(dateFormatStr)).data());
            filter_cJSON_AddStringToObject(pItem, "vchOperId", QByteArray().append(sqlQuery.record().value("vch_operID").toString()).data());
            filter_cJSON_AddStringToObject(pItem, "dtOperdate", QByteArray().append(sqlQuery.record().value("dt_operdate").toDateTime().toString(dateFormatStr)).data());
            filter_cJSON_AddStringToObject(pItem, "chUpflag", QByteArray().append(sqlQuery.record().value("ch_upflag").toString()).data());

            filter_cJSON_AddStringToObject(pItem, "orgCode", QByteArray().append(orgCode).data());
            cJSON_AddItemToArray(pArray, pItem);

            curstep++;qDebug() << "----------------------------in"<<curstep;
            if(curstep==listCount){
                if(!execHttp(pArray,"http://" + hostName
                             + "/CateSystem/arm/day-sumup/add-rest"))
                {return;}
                curstep=0;
            }
        }

        {
            if(!execHttp(pArray,"http://" + hostName
                         + "/CateSystem/arm/day-sumup/add-rest"))
            {return;}
            curstep=0;isApp=false;
        }
        initValue(pArray);



        // 6.外卖表
        sqlQuery.exec("select * from cey_u_togo WHERE dt_operdate >= '"
                      + dateTime1
                      + "' and dt_operdate <= '"
                      + dateTime2 + "';");

        while (sqlQuery.next()) {

            cJSON *pItem = cJSON_CreateObject();
            filter_cJSON_AddStringToObject(pItem, "chTogono", QByteArray().append(sqlQuery.record().value("ch_togono").toString()).data());
            filter_cJSON_AddStringToObject(pItem, "chBillno", QByteArray().append(sqlQuery.record().value("ch_billno").toString()).data());
            filter_cJSON_AddStringToObject(pItem, "chPayno", QByteArray().append(sqlQuery.record().value("ch_payno").toString()).data());
            cJSON_AddNumberToObject(pItem, "numCost", sqlQuery.record().value("num_cost").toFloat());
            cJSON_AddNumberToObject(pItem, "numDiscount", sqlQuery.record().value("num_discount").toFloat());
            cJSON_AddNumberToObject(pItem, "numPresent", sqlQuery.record().value("num_present").toFloat());
            cJSON_AddNumberToObject(pItem, "numBlotout", sqlQuery.record().value("num_blotout").toFloat());
            filter_cJSON_AddStringToObject(pItem, "vchWaiter", QByteArray().append(sqlQuery.record().value("vch_waiter").toString()).data());
            filter_cJSON_AddStringToObject(pItem, "vchAddress", QByteArray().append(sqlQuery.record().value("vch_address").toString()).data());
            filter_cJSON_AddStringToObject(pItem, "vchLinkman", QByteArray().append(sqlQuery.record().value("vch_linkman").toString()).data());
            filter_cJSON_AddStringToObject(pItem, "vchTel", QByteArray().append(sqlQuery.record().value("vch_tel").toString()).data());
            filter_cJSON_AddStringToObject(pItem, "chState", QByteArray().append(sqlQuery.record().value("ch_state").toString()).data());
            filter_cJSON_AddStringToObject(pItem, "vchOperId", QByteArray().append(sqlQuery.record().value("vch_operID").toString()).data());
            filter_cJSON_AddStringToObject(pItem, "dtOperdate", QByteArray().append(sqlQuery.record().value("dt_operdate").toDateTime().toString(dateFormatStr)).data());
            cJSON_AddNumberToObject(pItem, "numRate", sqlQuery.record().value("num_rate").toFloat());

            filter_cJSON_AddStringToObject(pItem, "orgCode", QByteArray().append(orgCode).data());
            cJSON_AddItemToArray(pArray, pItem);


            curstep++;qDebug() << "----------------------------in"<<curstep;
            if(curstep==listCount){
                if(!execHttp(pArray,"http://" + hostName
                             + "/CateSystem/arm/to-go/add-rest"))
                {return;}
                curstep=0;
            }
        }

        {
            if(!execHttp(pArray,"http://" + hostName
                         + "/CateSystem/arm/to-go/add-rest"))
            {return;}
            curstep=0;isApp=false;
        }
        initValue(pArray);



        // 7.收款方式明细
        sqlQuery.exec("select * from cey_u_checkout_detail WHERE dt_operdate >= '"
                      + dateTime1
                      + "' and dt_operdate <= '"
                      + dateTime2 + "';");

        while (sqlQuery.next()) {

            cJSON *pItem = cJSON_CreateObject();
            cJSON_AddNumberToObject(pItem, "codIntFlowId", sqlQuery.record().value("int_flowID").toFloat());
            filter_cJSON_AddStringToObject(pItem, "codChPayNo", QByteArray().append(sqlQuery.record().value("ch_payno").toString()).data());
            filter_cJSON_AddStringToObject(pItem, "codChPayModeNo", QByteArray().append(sqlQuery.record().value("ch_paymodeno").toString()).data());
            cJSON_AddNumberToObject(pItem, "codNumPayAmount", sqlQuery.record().value("num_payamount").toFloat());
            cJSON_AddNumberToObject(pItem, "codNumChange", sqlQuery.record().value("num_change").toFloat());
            cJSON_AddNumberToObject(pItem, "codNumRealAmount", sqlQuery.record().value("num_realamount").toFloat());
            cJSON_AddNumberToObject(pItem, "codNumFace", sqlQuery.record().value("num_face").toFloat());
            filter_cJSON_AddStringToObject(pItem, "codVchVoucherNo", QByteArray().append(sqlQuery.record().value("vch_voucherno").toString()).data());
            filter_cJSON_AddStringToObject(pItem, "codVchMemo", QByteArray().append(sqlQuery.record().value("vch_memo").toString()).data());
            filter_cJSON_AddStringToObject(pItem, "codVchOperId", QByteArray().append(sqlQuery.record().value("vch_operID").toString()).data());
            filter_cJSON_AddStringToObject(pItem, "codDtOperDate", QByteArray().append(sqlQuery.record().value("dt_operdate").toDateTime().toString(dateFormatStr)).data());
            cJSON_AddNumberToObject(pItem, "codNumCardRemain", sqlQuery.record().value("num_card_remain").toFloat());
            cJSON_AddNumberToObject(pItem, "codNumFaceNum", sqlQuery.record().value("num_face_Num").toFloat());

            filter_cJSON_AddStringToObject(pItem, "orgCode", QByteArray().append(orgCode).data());
            cJSON_AddItemToArray(pArray, pItem);


            curstep++;qDebug() << "----------------------------in"<<curstep;
            if(curstep==listCount){
                if(!execHttp(pArray,"http://" + hostName
                             + "/CateSystem/arm/check-out-detail/add-rest"))
                {return;}
                curstep=0;
            }
        }

        {
            if(!execHttp(pArray,"http://" + hostName
                         + "/CateSystem/arm/check-out-detail/add-rest"))
            {return;}
            curstep=0;isApp=false;
        }
        initValue(pArray);


        // 8.收银明细
        sqlQuery.exec("select * from cey_u_checkout_master WHERE dt_operdate >= '"
                      + dateTime1
                      + "' and dt_operdate <= '"
                      + dateTime2 + "';");

        while (sqlQuery.next()) {

            cJSON *pItem = cJSON_CreateObject();
            filter_cJSON_AddStringToObject(pItem, "comChPayNo", QByteArray().append(sqlQuery.record().value("ch_payno").toString()).data());
            filter_cJSON_AddStringToObject(pItem, "comChBillNo", QByteArray().append(sqlQuery.record().value("ch_billno").toString()).data());
            filter_cJSON_AddStringToObject(pItem, "comChTableNo", QByteArray().append(sqlQuery.record().value("ch_tableno").toString()).data());
            cJSON_AddNumberToObject(pItem, "comNumCost", sqlQuery.record().value("num_cost").toFloat());
            cJSON_AddNumberToObject(pItem, "comNumDiscount", sqlQuery.record().value("num_discount").toFloat());
            cJSON_AddNumberToObject(pItem, "comNumPresent", sqlQuery.record().value("num_present").toFloat());
            cJSON_AddNumberToObject(pItem, "comNumService", sqlQuery.record().value("num_service").toFloat());
            cJSON_AddNumberToObject(pItem, "comNumLowCost", sqlQuery.record().value("num_lowcost").toFloat());
            cJSON_AddNumberToObject(pItem, "comNumBlotout", sqlQuery.record().value("num_blotout").toFloat());
            cJSON_AddNumberToObject(pItem, "comNumFree",sqlQuery.record().value("num_free").toFloat());
            filter_cJSON_AddStringToObject(pItem, "comVchFreeOperId", QByteArray().append(sqlQuery.record().value("vch_free_operID").toString()).data());
            cJSON_AddNumberToObject(pItem, "comNumCash", sqlQuery.record().value("num_cash").toFloat());
            cJSON_AddNumberToObject(pItem, "comNumOther", sqlQuery.record().value("num_other").toFloat());

            cJSON_AddNumberToObject(pItem, "comNumRun", sqlQuery.record().value("num_run").toFloat());
            filter_cJSON_AddStringToObject(pItem, "comVchRunOperId", QByteArray().append(sqlQuery.record().value("vch_run_operID").toString()).data());
            filter_cJSON_AddStringToObject(pItem, "comChState", QByteArray().append(sqlQuery.record().value("ch_state").toString()).data());
            filter_cJSON_AddStringToObject(pItem, "comVchOperId", QByteArray().append(sqlQuery.record().value("vch_operID").toString()).data());
            filter_cJSON_AddStringToObject(pItem, "comDtOperDate", QByteArray().append(sqlQuery.record().value("dt_operdate").toDateTime().toString(dateFormatStr)).data());
            filter_cJSON_AddStringToObject(pItem, "comVchOperId2", QByteArray().append(sqlQuery.record().value("vch_operID2").toString()).data());
            filter_cJSON_AddStringToObject(pItem, "comDtOperDate2", QByteArray().append(sqlQuery.record().value("dt_operdate2").toDateTime().toString(dateFormatStr)).data());
            filter_cJSON_AddStringToObject(pItem, "comVchMemberNo", QByteArray().append(sqlQuery.record().value("vch_memberno").toString()).data());
            filter_cJSON_AddStringToObject(pItem, "comChType", QByteArray().append(sqlQuery.record().value("ch_type").toString()).data());
            cJSON_AddNumberToObject(pItem, "comNumRate", sqlQuery.record().value("num_rate").toFloat());


            filter_cJSON_AddStringToObject(pItem, "orgCode", QByteArray().append(orgCode).data());
            cJSON_AddItemToArray(pArray, pItem);

            curstep++;qDebug() << "----------------------------in"<<curstep;
            if(curstep==listCount){
                if(!execHttp(pArray,"http://" + hostName
                             + "/CateSystem/arm/check-out-master/add-rest"))
                {return;}
                curstep=0;
            }
        }

        {
            if(!execHttp(pArray,"http://" + hostName
                         + "/CateSystem/arm/check-out-master/add-rest"))
            {return;}
            curstep=0;isApp=false;
        }
        initValue(pArray);


        // 9.账单明细
        sqlQuery.exec("select * from cey_u_orderdish WHERE dt_operdate >= '"
                      + dateTime1
                      + "' and dt_operdate <= '"
                      + dateTime2 + "';");

        //qDebug() << sqlQuery.lastQuery();

        while (sqlQuery.next()) {
            //qDebug() << sqlQuery.record().value("ch_dishno").toString();
            cJSON *pItem = cJSON_CreateObject();
            cJSON_AddNumberToObject(pItem, "bdIntFlowId", sqlQuery.record().value("int_flowID").toFloat());
            filter_cJSON_AddStringToObject(pItem, "bdChBillNo", QByteArray().append(sqlQuery.record().value("ch_billno").toString()).data());
            filter_cJSON_AddStringToObject(pItem, "bdChTableNo", QByteArray().append(sqlQuery.record().value("ch_tableno").toString()).data());
            cJSON_AddNumberToObject(pItem, "bdIntId", sqlQuery.record().value("int_id").toInt());
            filter_cJSON_AddStringToObject(pItem, "bdChDishNo", QByteArray().append(sqlQuery.record().value("ch_dishno").toString()).data());
            cJSON_AddNumberToObject(pItem, "bdNumPrice", sqlQuery.record().value("num_price").toFloat());
            cJSON_AddNumberToObject(pItem, "bdNumPriceOrg", sqlQuery.record().value("num_price_org").toFloat());
            cJSON_AddNumberToObject(pItem, "bdNumPriceAdd", sqlQuery.record().value("num_price_add").toFloat());
            cJSON_AddNumberToObject(pItem, "bdNumNum", sqlQuery.record().value("num_num").toFloat());
            cJSON_AddNumberToObject(pItem, "bdNumBack", sqlQuery.record().value("num_back").toFloat());
            filter_cJSON_AddStringToObject(pItem, "bdVchBackOperId", QByteArray().append(sqlQuery.record().value("vch_back_operID").toString()).data());
            filter_cJSON_AddStringToObject(pItem, "bdDtBackOperDate", QByteArray().append(sqlQuery.record().value("dt_back_operdate").toDateTime().toString(dateFormatStr)).data());
            cJSON_AddNumberToObject(pItem, "bdIntDiscount", sqlQuery.record().value("int_discount").toInt());
            filter_cJSON_AddStringToObject(pItem, "bdVchDisOperId", QByteArray().append(sqlQuery.record().value("vch_dis_operID").toString()).data());
            filter_cJSON_AddStringToObject(pItem, "bdDtDisOperDate", QByteArray().append(sqlQuery.record().value("dt_dis_operdate").toDateTime().toString(dateFormatStr)).data());
            filter_cJSON_AddStringToObject(pItem, "btVchMemberNo", QByteArray().append(sqlQuery.record().value("vch_memberno").toString()).data());
            filter_cJSON_AddStringToObject(pItem, "bdChConsult", QByteArray().append(sqlQuery.record().value("ch_consult").toString()).data());
            filter_cJSON_AddStringToObject(pItem, "bdChPrintFlag", QByteArray().append(sqlQuery.record().value("ch_printflag").toString()).data());
            filter_cJSON_AddStringToObject(pItem, "bdVchPrintMemo", QByteArray().append(sqlQuery.record().value("vch_print_memo").toString()).data());
            filter_cJSON_AddStringToObject(pItem, "dcChSuitFlag", QByteArray().append(sqlQuery.record().value("ch_suitflag").toString()).data());
            filter_cJSON_AddStringToObject(pItem, "dcChSuitNo", QByteArray().append(sqlQuery.record().value("ch_suitno").toString()).data());
            filter_cJSON_AddStringToObject(pItem, "bdChSpecialFlag", QByteArray().append(sqlQuery.record().value("ch_specialflag").toString()).data());
            filter_cJSON_AddStringToObject(pItem, "chPresentFlag", QByteArray().append(sqlQuery.record().value("ch_presentflag").toString()).data());
            filter_cJSON_AddStringToObject(pItem, "bdVchPreOperId", QByteArray().append(sqlQuery.record().value("vch_pre_operID").toString()).data());
            filter_cJSON_AddStringToObject(pItem, "bdDtPreOperDate", QByteArray().append(sqlQuery.record().value("dt_pre_operdate").toDateTime().toString(dateFormatStr)).data());
            filter_cJSON_AddStringToObject(pItem, "bdVchOperId", QByteArray().append(sqlQuery.record().value("vch_operID").toString()).data());
            filter_cJSON_AddStringToObject(pItem, "bdDtOperDate", QByteArray().append(sqlQuery.record().value("dt_operdate").toDateTime().toString(dateFormatStr)).data());
            //qDebug() << "-------------------" << sqlQuery.record().value("dt_operdate").toDateTime() << sqlQuery.record().value("dt_operdate").toDateTime().toString(dateFormatStr);
            filter_cJSON_AddStringToObject(pItem, "bdChTogoNo", QByteArray().append(sqlQuery.record().value("ch_togono").toString()).data());
            filter_cJSON_AddStringToObject(pItem, "bdChPayNo", QByteArray().append(sqlQuery.record().value("ch_payno").toString()).data());
            filter_cJSON_AddStringToObject(pItem, "bdChOutFlag", QByteArray().append(sqlQuery.record().value("ch_outflag").toString()).data());

            filter_cJSON_AddStringToObject(pItem, "orgCode", QByteArray().append(orgCode).data());
            cJSON_AddItemToArray(pArray, pItem);

            curstep++;qDebug() << "----------------------------in"<<curstep;
            if(curstep==listCount){
                if(!execHttp(pArray,"http://" + hostName
                             + "/CateSystem/arm/order-dish/add-rest"))
                {return;}
                curstep=0;
            }
        }

        {
            if(!execHttp(pArray,"http://" + hostName
                         + "/CateSystem/arm/order-dish/add-rest"))
            {return;}
            curstep=0;isApp=false;
        }
        initValue(pArray);



        // 10.账单明细日志
        sqlQuery.exec("select * from cey_u_orderdish_log WHERE dt_operdate >= '"
                      + dateTime1
                      + "' and dt_operdate <= '"
                      + dateTime2 + "';");

        while (sqlQuery.next()) {

            cJSON *pItem = cJSON_CreateObject();
            cJSON_AddNumberToObject(pItem, "odlIntFlowId", sqlQuery.record().value("int_flowID").toInt());
            filter_cJSON_AddStringToObject(pItem, "odlChType", QByteArray().append(sqlQuery.record().value("ch_type").toString()).data());
            filter_cJSON_AddStringToObject(pItem, "odlVchImpower", QByteArray().append(sqlQuery.record().value("vch_impower").toString()).data());
            filter_cJSON_AddStringToObject(pItem, "odlVchReason", QByteArray().append(sqlQuery.record().value("vch_reason").toString()).data());
            filter_cJSON_AddStringToObject(pItem, "odlVchOperId", QByteArray().append(sqlQuery.record().value("vch_operID").toString()).data());
            filter_cJSON_AddStringToObject(pItem, "odlDtOperDate", QByteArray().append(sqlQuery.record().value("dt_operdate").toDateTime().toString(dateFormatStr)).data());
            filter_cJSON_AddStringToObject(pItem, "odlVchMemberNo", QByteArray().append(sqlQuery.record().value("vch_memberno").toString()).data());
            filter_cJSON_AddStringToObject(pItem, "odlVchMemo", QByteArray().append(sqlQuery.record().value("vch_memo").toString()).data());
            cJSON_AddNumberToObject(pItem, "odlIntOrderFlow", sqlQuery.record().value("int_orderflow").toInt());

            filter_cJSON_AddStringToObject(pItem, "orgCode", QByteArray().append(orgCode).data());
            cJSON_AddItemToArray(pArray, pItem);

            curstep++;qDebug() << "----------------------------in"<<curstep;
            if(curstep>=listCount){
                if(!execHttp(pArray,"http://" + hostName
                             + "/CateSystem/arm/order-dish-log/add-rest"))
                {return;}
                curstep=0;
            }
        }

        {
            if(!execHttp(pArray,"http://" + hostName
                         + "/CateSystem/arm/order-dish-log/add-rest"))
            {return;}
            curstep=0;isApp=false;
        }
        initValue(pArray);


        // 11..账单流水表
        sqlQuery.exec("select * from cey_u_master WHERE dt_operdate >= '"
                      + dateTime1
                      + "' and dt_operdate <= '"
                      + dateTime2 + "';");

        while (sqlQuery.next()) {

            cJSON *pItem = cJSON_CreateObject();
            filter_cJSON_AddStringToObject(pItem, "omChBillNo", QByteArray().append(sqlQuery.record().value("ch_billno").toString()).data());
            filter_cJSON_AddStringToObject(pItem, "omChState", QByteArray().append(sqlQuery.record().value("ch_state").toString()).data());
            filter_cJSON_AddStringToObject(pItem, "omVchOperId", QByteArray().append(sqlQuery.record().value("vch_operID").toString()).data());
            filter_cJSON_AddStringToObject(pItem, "omDtOperDate", QByteArray().append(sqlQuery.record().value("dt_operdate").toDateTime().toString(dateFormatStr)).data());
            filter_cJSON_AddStringToObject(pItem, "omVchOperId2", QByteArray().append(sqlQuery.record().value("vch_operID2").toString()).data());
            filter_cJSON_AddStringToObject(pItem, "omDtOperDate2", QByteArray().append(sqlQuery.record().value("dt_operdate2").toDateTime().toString(dateFormatStr)).data());
            filter_cJSON_AddStringToObject(pItem, "omVchMemo", QByteArray().append(sqlQuery.record().value("vch_memo").toString()).data());

            filter_cJSON_AddStringToObject(pItem, "orgCode", QByteArray().append(orgCode).data());
            cJSON_AddItemToArray(pArray, pItem);

            curstep++;qDebug() << "----------------------------in"<<curstep;
            if(curstep==listCount){
                if(!execHttp(pArray,"http://" + hostName
                             + "/CateSystem/arm/order-master/add-rest"))
                {return;}
                curstep=0;
            }
        }

        {
            if(!execHttp(pArray,"http://" + hostName
                         + "/CateSystem/arm/order-master/add-rest"))
            {return;}
            curstep=0;isApp=false;
        }
        initValue(pArray);
    }


    //qDebug() << QDate::currentDate().addDays(-1).toString("yyyy-MM-dd");
    //    QMessageBox::critical(NULL, tr("信息"), tr("流水上传成功！"), QMessageBox::Yes, QMessageBox::Yes);
}
#include "w_sys_manage_cloudsync.h"
bool cloudsync_service_help::execHttp(cJSON *&pArray,QString url)
{
    char *arrayDate = cJSON_Print(pArray);
    QString param=QString(arrayDate);
    if(arrayDate!=NULL)
        free(arrayDate);
    cJSON_Delete(pArray);
    pArray=NULL;

    if (isApp) {
        headInfo.insert("dataFlag", "add");
    } else
        headInfo.insert("dataFlag", "cover");

    qDebug() << param << headInfo;
    //中断退出
    if(isStop())
        return false;
    QNetworkReply* reply = sendPost2(url, param, headInfo);

    if(reply->error()==QNetworkReply::NoError ){
        pArray = cJSON_CreateArray();
        reply->deleteLater();

        return true;
    }else{
        lds_messagebox::warning(0, tr("消息提示"),tr("错误\n")+reply->errorString()+reply->readAll());

        return false;
    }

}

void cloudsync_service_help::initValue(cJSON *&pArray)
{

    //qDebug() << index;
    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    isApp = conf.value("w_sys_manage_cloudsync_basedataset/isapp").toBool();
    progressBar->setValue(index++);

    {
        cJSON_Delete(pArray);
        pArray=NULL;
        if(index<=upSize){
            pArray = cJSON_CreateArray();
        }
    }
}

void cloudsync_service_help::initDownBaseDataValue(cJSON *&pArray)
{
    //qDebug() << index;
    progressBar->setValue(index++);
    cJSON_Delete(pArray);
}

void cloudsync_service_help::stop()
{
    stopFlag=true;
}

bool cloudsync_service_help::isStop()
{
    if(stopFlag){//取消退出
        lds_messagebox::warning(0, tr("消息提示"),tr("手动传输中断，已经传输的数据无法取消!"));
        return true;
    }else
        return false;
}



QNetworkReply *cloudsync_service_help::sendPost2(QString url, QString param, QMap<QString, QString> &headInfo)
{

    QByteArray data;

    if(param !=NULL){
        //qDebug() << param.replace("\"\"","null");
        //qDebug() << url;
        data.append(param);
    }

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

    QNetworkAccessManager *networkAccessManager = new QNetworkAccessManager();

    connect(networkAccessManager,SIGNAL(finished(QNetworkReply*)),networkAccessManager,SLOT(deleteLater()));
    connect(networkAccessManager,SIGNAL(finished(QNetworkReply*)),&eloop,SLOT(quit()));

    QNetworkReply *reply = networkAccessManager->post(request,data);
    eloop.exec(QEventLoop::ExcludeUserInputEvents);

    return reply;
}


QNetworkReply *cloudsync_service_help::sendGet(QString url, QMap<QString, QString> &headInfo)
{

    QNetworkRequest request;
    request.setUrl(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json;charset=utf-8"));

    if(!headInfo.isEmpty()){
        QMap<QString, QString>::const_iterator i;
        for( i=headInfo.constBegin(); i!=headInfo.constEnd(); ++i){
            request.setRawHeader(QByteArray().append(i.key()),QByteArray().append(i.value()));
        }
    }

    QNetworkAccessManager *networkAccessManager = new QNetworkAccessManager();

    connect(networkAccessManager,SIGNAL(finished(QNetworkReply*)),networkAccessManager,SLOT(deleteLater()));
    connect(networkAccessManager,SIGNAL(finished(QNetworkReply*)),&eloop,SLOT(quit()));

    QNetworkReply *reply = networkAccessManager->get(request);

    eloop.exec(QEventLoop::ExcludeUserInputEvents);

    return reply;
}
