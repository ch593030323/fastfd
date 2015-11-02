
#include "w_m_member.h"
#include "n_func.h"
//会员状态对照表
QStandardItemModel w_m_member::VIP_MODEL_STATE;
//持卡状态对照表
QStandardItemModel w_m_member::VIP_MODEL_FLAG;
//会员卡类型对照表
QStandardItemModel w_m_member::VIP_MODEL_ATTRI;
//会员卡优惠方式对照表
QStandardItemModel w_m_member::VIP_MODEL_FAVOUR;
//会员卡积分
QStandardItemModel w_m_member::VIP_MODEL_CALC;
//会员卡是否取整
QStandardItemModel w_m_member::VIP_MODEL_CALCTYPE;
//会员卡消费积分范围
QStandardItemModel w_m_member::VIP_MODEL_AREA;
//会员卡积分类型
QStandardItemModel w_m_member::VIP_MODEL_POINT_TYPE;
//充值支付方式
QStandardItemModel w_m_member::RECHARGE_MODEL_PAYMODE;//t_m_member_type ch_area_flag

w_m_member::w_m_member(QWidget *parent) :
    QDialog(parent)
{
}

int w_m_member::VIP_MODEL_INIT()
{
    qDebug() << "VIP_MODEL_INIT...";
    QString ls_value;
    //! init
    QList<QStandardItem *> items;
    VIP_MODEL_STATE.removeRows(0, VIP_MODEL_STATE.rowCount());
    items.clear();items<<new QStandardItem("1")<<new QStandardItem(tr("正常"));VIP_MODEL_STATE.appendRow(items);
    items.clear();items<<new QStandardItem("2")<<new QStandardItem(tr("冻结"));VIP_MODEL_STATE.appendRow(items);
    items.clear();items<<new QStandardItem("3")<<new QStandardItem(tr("挂失"));VIP_MODEL_STATE.appendRow(items);
    items.clear();items<<new QStandardItem("4")<<new QStandardItem(tr("作废"));VIP_MODEL_STATE.appendRow(items);
    items.clear();items<<new QStandardItem("5")<<new QStandardItem(tr("已退卡"));VIP_MODEL_STATE.appendRow(items);

    VIP_MODEL_FLAG.removeRows(0, VIP_MODEL_FLAG.rowCount());
    items.clear();items<<new QStandardItem("Y")<<new QStandardItem(tr("已发卡"));VIP_MODEL_FLAG.appendRow(items);
    items.clear();items<<new QStandardItem("N")<<new QStandardItem(tr("未发卡"));VIP_MODEL_FLAG.appendRow(items);

    VIP_MODEL_ATTRI.removeRows(0, VIP_MODEL_ATTRI.rowCount());
    ls_value=n_func::f_m_get_parameter("member_ATT_mode", "123");
    if(ls_value.contains("1")){
        items.clear();items<<new QStandardItem("1")<<new QStandardItem(tr("优惠卡"));VIP_MODEL_ATTRI.appendRow(items);
    }
    if(ls_value.contains("2")){
        items.clear();items<<new QStandardItem("2")<<new QStandardItem(tr("充值卡"));VIP_MODEL_ATTRI.appendRow(items);
    }
    //        items.clear(); items<<new QStandardItem("3")<<new QStandardItem(tr("借记卡"));VIP_MODEL_ATTRI.appendRow(items);

    VIP_MODEL_FAVOUR.removeRows(0, VIP_MODEL_FAVOUR.rowCount());
    items.clear();items<<new QStandardItem("2")<<new QStandardItem(tr("固定方式"));VIP_MODEL_FAVOUR.appendRow(items);

    VIP_MODEL_CALC.removeRows(0, VIP_MODEL_CALC.rowCount());
    items.clear();items<<new QStandardItem("Y")<<new QStandardItem(tr("积分"));VIP_MODEL_CALC.appendRow(items);
    items.clear();items<<new QStandardItem("N")<<new QStandardItem(tr("不积分"));VIP_MODEL_CALC.appendRow(items);

    VIP_MODEL_CALCTYPE.removeRows(0, VIP_MODEL_CALCTYPE.rowCount());
    items.clear();items<<new QStandardItem("0")<<new QStandardItem(tr("保留小数")); VIP_MODEL_CALCTYPE.appendRow(items);
    items.clear();items<<new QStandardItem("1")<<new QStandardItem(tr("取整")); VIP_MODEL_CALCTYPE.appendRow(items);

    VIP_MODEL_AREA.removeRows(0, VIP_MODEL_AREA.rowCount());
    items.clear();items<<new QStandardItem("0")<<new QStandardItem(tr("消费额"));VIP_MODEL_AREA.appendRow(items);
    items.clear();items<<new QStandardItem("1")<<new QStandardItem(tr("折后额或卡付款"));VIP_MODEL_AREA.appendRow(items);
    items.clear();items<<new QStandardItem("2")<<new QStandardItem(tr("实际收入"));VIP_MODEL_AREA.appendRow(items);

    VIP_MODEL_POINT_TYPE.removeRows(0, VIP_MODEL_POINT_TYPE.rowCount());
    items.clear();items<<new QStandardItem("3")<<new QStandardItem(tr("消费积分"));VIP_MODEL_POINT_TYPE.appendRow(items);
    items.clear();items<<new QStandardItem("4")<<new QStandardItem(tr("礼品兑换"));VIP_MODEL_POINT_TYPE.appendRow(items);
    items.clear();items<<new QStandardItem("5")<<new QStandardItem(tr("手工充积分"));VIP_MODEL_POINT_TYPE.appendRow(items);
    items.clear();items<<new QStandardItem("6")<<new QStandardItem(tr("手工减积分"));VIP_MODEL_POINT_TYPE.appendRow(items);

    RECHARGE_MODEL_PAYMODE.removeRows(0, RECHARGE_MODEL_PAYMODE.rowCount());
    items.clear();items<<new QStandardItem("1")<<new QStandardItem(tr("现金"));RECHARGE_MODEL_PAYMODE.appendRow(items);
    items.clear();items<<new QStandardItem("2")<<new QStandardItem(tr("银行卡转账"));RECHARGE_MODEL_PAYMODE.appendRow(items);
    items.clear();items<<new QStandardItem("3")<<new QStandardItem(tr("银行卡支票"));RECHARGE_MODEL_PAYMODE.appendRow(items);

}
