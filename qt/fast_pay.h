#ifndef FAST_PAY_H
#define FAST_PAY_H

#include <QDialog>
#include <QStandardItemModel>
#include "public_mainprinter.h"
#include "lds_dialog_input.h"
#include "w_bt_dish_kitchenprinter.h"
#include "ui_fast_pay.h"
class frontViewDialog_virtual;
class fast_order_2;
class lds_loadingDialog;
class fast_pay : public QDialog
{
    Q_OBJECT
public:
    enum PRINT_CHOOSE{
        PRINT_BILL                     =1,/*小票*/
        PRINT_LABEL                  =2,/*标签*/
        PRINT_KITCHEN             =4,/*厨打*/
        PRINT_KITCHEN_OUT    =8,/*出品*/
        PRINT_DRAWER             =16,/*开钱箱*/
        PRINT_BARTYPE             =32,/*二维码一维码*/
        PRINT_PIC                      =64,/*图片打印*/
        PRINT_ONLY_COSUME  =128/*只有消费*/
    };

public:
    explicit fast_pay(frontViewDialog_virtual *parent = 0, bool on_d_p_b_r/*折扣，赠送，抹零，税率*/=true);
    ~fast_pay();
    QString get_total();
    void init_control();
    void init_data(bool on_d_p_b_r);
    bool btn_t_f;
    QString get_curdandao() const ;
    static void kitchen_or_outprint(const QString &key/*"", out_*/, const QString &_dayindanhao, const QString &serial_no);
    Ui_fast_pay *ui;
    void update_lowcost_service();
    void setjiulou_visiable(bool isvisibled);
public slots:
    virtual void to_pay(bool is_manul_input_serialno=false, bool isprint=true);
    virtual void tokitchenprint(const QString &key/*"", out_*/, const QString &_dayindanhao, const QString &serial_no);
    virtual void tolabelprint();
    void fast_pay_print(const QString &_dayindanhao,
                        const QString &title0=tr("付款账单"),
                        int printchoose= PRINT_BILL|PRINT_LABEL
            |PRINT_KITCHEN
            |PRINT_KITCHEN_OUT
            |PRINT_DRAWER
            |PRINT_BARTYPE
            |PRINT_PIC
            );
private slots:

    void on_pushButton_21_clicked();

    void on_pushButton_15_clicked();

    void on_pushButton_17_clicked();

    void on_pushButton_18_clicked();

    void on_pushButton_19_clicked();

    void on_pushButton_20_clicked();

    void on_pushButton_22_clicked();

    void on_pushButton_23_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_16_clicked();

    void on_pushButton_13_clicked();//买单

    void lineEdit_9_textChanged(const QString &arg1);

    void print_lost(const QString & str);

    void on_lineEdit_4_textChanged(const QString &arg1);

    void on_lineEdit_7_textChanged(const QString &arg1);

    void setprintsize(blockIns &b, const QString & fontlen, const QString &content);
    void setprintsize(QString &content, int len, const QString &flag);
    void setprintsize(QString &content, int len, Qt::AlignmentFlag flag);

signals:
    void change();
private:
    QStandardItemModel *table_model;//付款方式的MODE
    float kafujine;//使用会员卡付款的金额
    QString fukuanpingzheng;//table_model里的付款凭证字段
    bool shifouyouhuika;//是否使用了优惠卡付款
    QString youhuikahao;//优惠卡付款卡号
    QString timer;
    QString diancaishijian;//
    int count_dis;//折扣值
    QString wdanhao;//外卖单号
    QString pay_timer;//结帐时间
    QString liushuihao_p;//厨打，出品打印的漏打的流水号
    QString liushuihao_p_is_shoudog;//是否手动输入流水号
    QString huyuankayue;//会员卡的余额
    //    public_mainprinter *printer;
    QString bendandanhao;//结帐的单号
    QString louda_ip;//厨打，出品打印的漏打的IP
    QString louda_neirong;//厨打，出品打印的漏打的内容
    QString louda_title;//厨打，出品打印的漏打的标题

    frontViewDialog_virtual *_parent;
    QString yun_pre_billno;//上一次付款单号
    QString yun_pre_memberno;//上一次付款会员

protected:
    bool pay_finished;
    float pay_lowcost;
    float pay_servicecost;
    float otherpay;
};

#endif // FAST_PAY_H
