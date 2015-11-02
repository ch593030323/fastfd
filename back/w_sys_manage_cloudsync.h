#ifndef W_SYS_MANAGE_CLOUDSYNC_H
#define W_SYS_MANAGE_CLOUDSYNC_H

#include <QDialog>
#include <QVariant>
#include <QString>
#include "w_sys_manage_cloudsync_transfer.h"
//云端的
//支付号 中餐 快餐 =======   F
//订单号 中餐         =======   Z
//订单号 快餐         =======   K
//外卖             -cey_u_togo_guest     C[ch_guestid]
//                     -cey_u_master           Q[ch_billno]
//                     -cey_u_table               Q[ch_billno]
//                     -cey_u_togo                T[ch_togono] Q[ch_billno]
//                     -cey_u_orderdish       Q[ch_billno]

//软件本身
//支付号 中餐 快餐 =======  P
//订单号 中餐         =======   B
//订单号 快餐         =======  Q
//外卖             -cey_u_togo_guest     C[ch_guestid]
//                     -cey_u_master           Q[ch_billno]
//                     -cey_u_table               Q[ch_billno]
//                     -cey_u_togo                W[ch_togono] Q[ch_billno]
//                     -cey_u_orderdish       Q[ch_billno]
struct orderMaster{
    //    float sn;
    QString omChBillNo;
    QString omChState;
    QString omVchOperId;
    QString omDtOperDate;
    QString omVchOperId2;
    QString omDtOperDate2;
    QString omVchMemo;
    QString orgCode;
    QString date;


};
struct checkOutMaster{
    checkOutMaster():
        comNumCost(0)
      , comNumDiscount(0)
      , comNumPresent(0)
      , comNumService(0)
      , comNumLowCost(0)
      , comNumBlotout(0)
      , comNumFree(0)
      , comNumCash(0)
      , comNumOther(0)
      , comNumRun(0)
      , comNumRate(0)
    {}
    //    float sn;
    QString comChPayNo;
    QString comChBillNo;
    QString comChTableNo;
    float comNumCost;
    float comNumDiscount;
    float comNumPresent;
    float comNumService;
    float comNumLowCost;
    float comNumBlotout;
    float comNumFree;
    QString comVchFreeOperId;
    float comNumCash;
    float comNumOther;
    float comNumRun;
    QString comVchRunOperId;
    QString comChState;
    QString comVchOperId;
    QString comDtOperDate;
    QString comVchOperId2;
    QString comDtOperDate2;
    QString comVchMemberNo;
    QString comChType;
    QString orgCode;
    QString date;
    float comNumRate;
};
struct memberPointList{
    memberPointList():
        mpIntFlowId(0)
      ,mpNumPoint(0){}
    float sn;
    float mpIntFlowId;
    QString mpVchMemberNo;
    QString mpChType;
    QString mpChPayNo;
    float mpNumPoint;
    QString mpVchMemo;
    QString mpVchOperId;
    QString mpDtOperDate;
    QString orgCode;
    QString date;
};
struct memberPay{
    memberPay():
        intFlowid(0),
        mpNumAmount(0),
        mpNumPay(0),
        mpIntCheckFlow(0)
    {}
    float sn;
    float intFlowid;
    QString mpVchMemberNo;
    QString mpChPayNo;
    QString mpChPayModeNo;
    float mpNumAmount;
    float mpNumPay;
    QString mpVchOperId;
    QString mpDtOperDate;
    QString mpChBalanceNo;
    float mpIntCheckFlow;
    QString orgCode;
    QString date;
};
struct memberCuramount{
    memberCuramount():
        numAmount(0),
        numHangamount(0),
        numPoint(0)
    {}
    float sn;
    QString vchMemberno;
    float numAmount;
    float numHangamount;
    float numPoint;
    QString vchOperId;
    QString dtOperdate;
    QString chBranchno;
    QString orgCode;
    QString date;
};

struct tableRecordsConsumption{
    tableRecordsConsumption():
        intPerson(0),
        numCost(0),
        numTransService(0)
    {}
    long sn;
    QString chBillno;
    QString chTableno;
    float intPerson;
    QString vchMemo;
    QString chState;
    QString chPayno;
    QString vchOperId;
    QString dtOperdate;
    QString vchOperId2;
    QString dtOperdate2;
    QString chOrderState;
    float numCost;
    QString vchWaiter;
    float numTransService;
    QString orgCode;
    QString date;
};
struct orderDish{
    orderDish():
        bdIntFlowId(0),
        bdIntId(0),
        bdNumPrice(0),
        bdNumPriceOrg(0),
        bdNumPriceAdd(0),
        bdNumNum(0),
        bdNumBack(0),
        bdIntDiscount(0)
    {}
    float sn;
    float bdIntFlowId;
    QString bdChBillNo;
    QString bdChTableNo;
    float bdIntId;
    QString bdChDishNo;
    float bdNumPrice;
    float bdNumPriceOrg;
    float bdNumPriceAdd;
    float bdNumNum;
    float bdNumBack;
    QString bdVchBackOperId;
    QString bdDtBackOperDate;
    float bdIntDiscount;
    QString bdVchDisOperId;
    QString bdDtDisOperDate;
    QString btVchMemberNo;
    QString bdChConsult;
    QString bdChPrintFlag;
    QString bdVchPrintMemo;
    QString dcChSuitFlag;
    QString dcChSuitNo;
    QString bdChSpecialFlag;
    QString chPresentFlag;
    QString bdVchPreOperId;
    QString bdDtPreOperDate;
    QString bdVchOperId;
    QString bdDtOperDate;
    QString bdChTogoNo;
    QString bdChPayNo;
    QString bdChOutFlag;
    QString orgCode;
    QString date;
};
struct checkOutDetail{
    checkOutDetail():
        codIntFlowId(0),
        codNumPayAmount(0),
        codNumChange(0),
        codNumRealAmount(0),
        codNumFace(0),
        codNumCardRemain(0),
        codNumFaceNum(0)
    {}
    float sn;
    float codIntFlowId;
    QString codChPayNo;
    QString codChPayModeNo;
    float codNumPayAmount;
    float codNumChange;
    float codNumRealAmount;
    float codNumFace;
    QString codVchVoucherNo;
    QString codVchMemo;
    QString codVchOperId;
    QString codDtOperDate;
    float codNumCardRemain;
    float codNumFaceNum;
    QString orgCode;
    QString date;
};
struct memberDeposit{
    memberDeposit():
        mdIntFlowId(0),
        mdNumDeposit(0),
        mdNumRealaMount(0),
        mdRemainAmt(0)
    {}
    float sn;
    float mdIntFlowId;
    QString mdVchMemberNo;
    float mdNumDeposit;
    float mdNumRealaMount;
    QString mdChDepositMode;
    QString mdChPayMode;
    QString mdVchOperId;
    QString dtOperDate;
    QString mdVchMemo;
    float mdRemainAmt;
    QString orgCode;
    QString date;
    QString comChPayNo;
};

class member_up_yun{

public :
    member_up_yun():
        isrun(false){}
public:
    QList<orderMaster> orderMasterList;
    QList<checkOutMaster> checkOutMasterList;
    QList<memberPointList> memberPointListList;
    QList<memberPay> memberPayList;
    QList<memberCuramount> memberCuramountList;

    QList<tableRecordsConsumption> tableRecordsConsumptionList;
    QList<orderDish> orderDishList;
    QList<checkOutDetail> checkOutDetailList;
    QList<memberDeposit> memberDepositList;
    bool isrun;

public:
    void appitem(orderMaster &o){orderMasterList.append(o);}
    void appitem(checkOutMaster &o){checkOutMasterList.append(o);}
    void appitem(memberPointList &o){memberPointListList.append(o);}
    void appitem(memberPay &o){memberPayList.append(o);}
    void appitem(memberCuramount &o){memberCuramountList.append(o);}

    void appitem(tableRecordsConsumption &o){tableRecordsConsumptionList.append(o);}
    void appitem(orderDish &o){orderDishList.append(o);}
    void appitem(checkOutDetail &o){checkOutDetailList.append(o);}
    void appitem(memberDeposit &o){memberDepositList.append(o);}

    bool iscanupload(double v);
    bool iscanupload(float v);
    bool iscanupload(const QString &v);
    void make_JSON_Array(orderMaster &p, cJSON *root);
    void make_JSON_Array(checkOutMaster &p, cJSON *root);
    void make_JSON_Array(memberPointList &p, cJSON *root);
    void make_JSON_Array(memberPay &p, cJSON *root);
    void make_JSON_Array(memberCuramount &p, cJSON *root);

    void make_JSON_Array(tableRecordsConsumption &p, cJSON *root);
    void make_JSON_Array(orderDish &p, cJSON *root);
    void make_JSON_Array(checkOutDetail &p, cJSON *root);
    void make_JSON_Array(memberDeposit &p, cJSON *root);

};

/**
16、连锁会员付款
http://192.168.1.81/CateSystem/cvs/member/fuk-rest
method: POST
head orgCode:000000060002
    head userName:admin
    head password:123456
    head flag:Q

    参数
{
"orderMaster":{"sn":null,"omChBillNo":null,"omChState":null,"omVchOperId":null,"omDtOperDate":null,"omVchOperId2":null,"omDtOperDate2":null,"omVchMemo":null,"orgCode":null,"date":null},

"checkOutMaster":{"sn":null,"comChPayNo":null,"comChBillNo":null,"comChTableNo":null,"comNumCost":null,"comNumDiscount":null,"comNumPresent":null,"comNumService":null,"comNumLowCost":null,"comNumBlotout":null,"comNumFree":null,"comVchFreeOperId":null,"comNumCash":null,"comNumOther":null,"comNumRun":null,"comVchRunOperId":null,"comChState":null,"comVchOperId":null,"comDtOperDate":null,"comVchOperId2":null,"comDtOperDate2":null,"comVchMemberNo":null,"comChType":null,"orgCode":null,"date":null,"comNumRate":null},

"memberPointList":[
{"sn":null,"mpIntFlowId":null,"mpVchMemberNo":null,"mpChType":null,"mpChPayNo":null,"mpNumPoint":null,"mpVchMemo":null,"mpVchOperId":null,"mpDtOperDate":null,"orgCode":null,"date":null},
{"sn":null,"mpIntFlowId":null,"mpVchMemberNo":null,"mpChType":null,"mpChPayNo":null,"mpNumPoint":null,"mpVchMemo":null,"mpVchOperId":null,"mpDtOperDate":null,"orgCode":null,"date":null}
],

"memberPay":[
{"sn":null,"intFlowid":null,"mpVchMemberNo":null,"mpChPayNo":null,"mpChPayModeNo":null,"mpNumAmount":null,"mpNumPay":null,"mpVchOperId":null,"mpDtOperDate":null,"mpChBalanceNo":null,"mpIntCheckFlow":null,"orgCode":null,"date":null},
{"sn":null,"intFlowid":null,"mpVchMemberNo":null,"mpChPayNo":null,"mpChPayModeNo":null,"mpNumAmount":null,"mpNumPay":null,"mpVchOperId":null,"mpDtOperDate":null,"mpChBalanceNo":null,"mpIntCheckFlow":null,"orgCode":null,"date":null}
],

"memberCuramount":[
{"sn":null,"vchMemberno":null,"numAmount":null,"numHangamount":null,"numPoint":null,"vchOperId":null,"dtOperdate":null,"chBranchno":null,"orgCode":null,"date":null},
{"sn":null,"vchMemberno":null,"numAmount":null,"numHangamount":null,"numPoint":null,"vchOperId":null,"dtOperdate":null,"chBranchno":null,"orgCode":null,"date":null}
],

"tableRecordsConsumption":{"sn":null,"chBillno":null,"chTableno":null,"intPerson":null,"vchMemo":null,"chState":null,"chPayno":null,"vchOperId":null,"dtOperdate":null,"vchOperId2":null,"dtOperdate2":null,"chOrderState":null,"numCost":null,"vchWaiter":null,"numTransService":null,"orgCode":null,"date":null},

"orderDish":[
{"sn":null,"bdIntFlowId":null,"bdChBillNo":null,"bdChTableNo":null,"bdIntId":null,"bdChDishNo":null,"bdNumPrice":null,"bdNumPriceOrg":null,"bdNumPriceAdd":null,"bdNumNum":null,"bdNumBack":null,"bdVchBackOperId":null,"bdDtBackOperDate":null,"bdIntDiscount":null,"bdVchDisOperId":null,"bdDtDisOperDate":null,"btVchMemberNo":null,"bdChConsult":null,"bdChPrintFlag":null,"bdVchPrintMemo":null,"dcChSuitFlag":null,"dcChSuitNo":null,"bdChSpecialFlag":null,"chPresentFlag":null,"bdVchPreOperId":null,"bdDtPreOperDate":null,"bdVchOperId":null,"bdDtOperDate":null,"bdChTogoNo":null,"bdChPayNo":null,"bdChOutFlag":null,"orgCode":null,"date":null},
{"sn":null,"bdIntFlowId":null,"bdChBillNo":null,"bdChTableNo":null,"bdIntId":null,"bdChDishNo":null,"bdNumPrice":null,"bdNumPriceOrg":null,"bdNumPriceAdd":null,"bdNumNum":null,"bdNumBack":null,"bdVchBackOperId":null,"bdDtBackOperDate":null,"bdIntDiscount":null,"bdVchDisOperId":null,"bdDtDisOperDate":null,"btVchMemberNo":null,"bdChConsult":null,"bdChPrintFlag":null,"bdVchPrintMemo":null,"dcChSuitFlag":null,"dcChSuitNo":null,"bdChSpecialFlag":null,"chPresentFlag":null,"bdVchPreOperId":null,"bdDtPreOperDate":null,"bdVchOperId":null,"bdDtOperDate":null,"bdChTogoNo":null,"bdChPayNo":null,"bdChOutFlag":null,"orgCode":null,"date":null}
 ],

"checkOutDetail":[
{"sn":null,"codIntFlowId":null,"codChPayNo":null,"codChPayModeNo":null,"codNumPayAmount":null,"codNumChange":null,"codNumRealAmount":null,"codNumFace":null,"codVchVoucherNo":null,"codVchMemo":null,"codVchOperId":null,"codDtOperDate":null,"codNumCardRemain":null,"codNumFaceNum":null,"orgCode":null,"date":null},
{"sn":null,"codIntFlowId":null,"codChPayNo":null,"codChPayModeNo":null,"codNumPayAmount":null,"codNumChange":null,"codNumRealAmount":null,"codNumFace":null,"codVchVoucherNo":null,"codVchMemo":null,"codVchOperId":null,"codDtOperDate":null,"codNumCardRemain":null,"codNumFaceNum":null,"orgCode":null,"date":null}
],

"memberDeposit":{"sn":null,"mdIntFlowId":null,"mdVchMemberNo":null,"mdNumDeposit":null,"mdNumRealaMount":null,"mdChDepositMode":null,"mdChPayMode":null,"mdVchOperId":null,"dtOperDate":null,"mdVchMemo":null,"mdRemainAmt":null,"orgCode":null,"date":null}

}

                                          返回结果

                                          200
                                        {
                                            "message": "ok"
                                        }

                                                         550
                 message : 错误信息
                                                         */

/**
    cJSON *root,*fmt;
    root=cJSON_CreateObject();
    cJSON_AddItemToObject(root, "name", cJSON_CreateString("Jack (\"Bee\") Nimble"));
    cJSON_AddItemToObject(root, "format", fmt=cJSON_CreateObject());
    cJSON_AddStringToObject(fmt,"type",     "rect");
    cJSON_AddNumberToObject(fmt,"width",        1920);
    cJSON_AddNumberToObject(fmt,"height",       1080);
    cJSON_AddFalseToObject (fmt,"interlace");
    cJSON_AddNumberToObject(fmt,"frame rate",   24);


派生到我的代码片

    {
        "name": "Jack (\"Bee\") Nimble",
        "format": {
            "type":       "rect",
            "width":      1920,
            "height":     1080,
            "interlace":  false,
            "frame rate": 24
        }
    }


    cJSON *root;root=cJSON_CreateObject();
    cJSON_AddItemToObject(root, "name", cJSON_CreateString("Jack (\"Bee\") Nimble"));

    cJSON *name1;cJSON_AddItemToObject(root, "name1", name1=cJSON_CreateObject());
    cJSON_AddStringToObject(name1, "name2", "h1");
    cJSON_AddStringToObject(name1, "name3", "h1");


    cJSON *fmt;cJSON_AddItemToObject(root, "format", fmt=cJSON_CreateArray());
    cJSON *fmt1;cJSON_AddItemToObject(fmt, "fmt1", fmt1=cJSON_CreateObject());
    cJSON_AddStringToObject(fmt1,"fmt2",     "rect");
    cJSON_AddStringToObject(fmt1,"fmt3",     "rect");
    cJSON *fmt2;cJSON_AddItemToObject(fmt, "fmt1", fmt2=cJSON_CreateObject());
    cJSON_AddStringToObject(fmt2,"fmt4",     "rect");
    cJSON_AddStringToObject(fmt2,"fmt5",     "rect");
    qDebug() << cJSON_Print(root);
*/

class Ui_w_sys_manage_cloudsync_Dialog;
class w_sys_manage_cloudsync : public QDialog
{
    Q_OBJECT
public:
    explicit w_sys_manage_cloudsync(QWidget *parent = 0);
    ~w_sys_manage_cloudsync();

    static bool this_action_is_successed;
signals:

public slots:
    // 关闭按钮事件
    void slot_close();
    // 基本数据设置事件
    void slot_baseData_set();
    // 上传基本数据事件
    void slot_up_baseData();
    // 下载基本数据事件
    void slot_down_baseData();
    // 上传流水数据
    void slot_up_flowData();

public:
    //! 会员部分
    static cJSON * get_yun_httpp(
            const QString &http_key,
            const QString &param,
            QMap<QString, QString> &headInfo,
            const QString &msgtitle,
            bool ispop=true
            );
    static cJSON * get_yun_httpg(
            const QString &http_key,
            QMap<QString, QString> &headInfo,
            const QString &msgtitle,
            bool ispop=true
            );

    static QString print_yun(cJSON *print);
    static void printalljson_child(cJSON *print);
    //获取会员参数
    static QString get_yun_member_parameter(const QString &key, QVariant::Type keytype, const QString &def="");
    //获取会员读卡
    static QString get_yun_member_readcard(const QString cardNo, const QString &operId, const QString &key, QVariant::Type keytype, const QString &def="");
    //获取会员最大订单号
    static QString get_yun_member_maxorderno(const QString &key, QVariant::Type keytype, const QString &def="");
    //获取会员最大支付号
    static QString get_yun_member_maxmasterno(const QString &key, QVariant::Type keytype, const QString &def="");
    //获取会员类型
    static QString get_yun_member_membertype(const QString &typeNo, const QString &key, QVariant::Type keytype, const QString &def="");
    //提交关于会员的信息
    static QString upload_yun_pay_info(member_up_yun &muy);
    //提交关于会员的充值
    static bool get_yun_member_recharge(const QString &cardNo, const QString &operId, const QString &chongzje, const QString &shisje, const QString &zhiffs);
    //提交关于会员的退卡
    static bool get_yun_member_blankcard(const QString &cardNo, const QString &tuikje, const QString &zhiffs);
    //提交关于会员的取款
    static bool get_yun_member_fetch(const QString &cardNo, const QString &operId, const QString &qukje, const QString &zhiffs);
    //会员充值,取款,退卡相关金额
    struct member_r_f_b_info{
        QString rechargeAmount;//充值金额
        QString realRechargeAmount;//充值实收
        QString withdrawalAmount;//取款金额
        QString backCardAmount;//退卡金额
    };
    static member_r_f_b_info  get_yun_member_r_f_b(const QString &operId,const QString &dateStart, const QString &dateEnd);
    //释放json
    static void json_free();
    //会员付款核实
    static bool get_yun_member_checkbillnopay(const QString &billno, const QString &memberno);

    //! 匹配系统时间是否和 云时间相同
    static bool match_local_time();
public:
    static bool is_match_local_time;
public slots:
    //! 微信点餐-轮询获取微信订单订桌外卖汇总表
    static bool get_weixin_orderinfo(QSqlDatabase &sql_db,  const QString&beginTime, const QString&endTime, int downFlag);
    static bool get_weixin_orderdish(QSqlDatabase &sql_db, const QString&beginTime, const QString&endTime, int downFlag);
    static bool set_weixin_payforwater(QSqlDatabase &sql_db, const QString&beginTime, const QString&endTime, int downFlag);
    static bool up_weixin_change_takkeoutstate(const QString &sn, const QString &orderState/*11,21,22,25*/, const QString &refuseReason=QString());

private:
    Ui_w_sys_manage_cloudsync_Dialog *ui;
    QDialog *dialog;
    static QMap<QString, QString> yun_MemberParameter;
    static QMap<QString, QString> yun_MemberReadcard;
    static QMap<QString, QString> yun_MemberType;

    // 这儿对c的指针要做专门处理制度json_free负责释放
    static cJSON *json_yun_parameter;
    static cJSON *json_yun_readcard;
    static cJSON *json_yun_membertype;
};

//会员付款信息提示
#include <QTextEdit>
class w_sys_manage_cloudsync_pop_info:public QTextEdit{
    Q_OBJECT
public:
    explicit w_sys_manage_cloudsync_pop_info(QWidget *parent = 0);
    QTextEdit *textedit;
};

#endif // W_SYS_MANAGE_CLOUDSYNC_H
