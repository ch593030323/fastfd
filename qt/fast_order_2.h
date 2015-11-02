#ifndef FAST_ORDER_2_H
#define FAST_ORDER_2_H
#include <QDialog>
#include <QStandardItemModel>
#include <QSqlQueryModel>
#include <QSqlDatabase>
#include "dishes_label.h"
#include "public_mainprinter.h"
#include <QTimer>
#include "print_prompt.h"
#include <QSqlDatabase>
#include "print_threqd.h"
#include "bianma_cook.h"
#include "kindindexwidget.h"
#include "gooditem.h"
#include "gooditem_zilei.h"
#include <QWidget>
#include "function_selection.h"
#include "public_logindialog.h"
#include <QElapsedTimer>
#include "w_scr_dish.h"
#include "ui_frontviewdialog_fast_2.h"
#include "frontviewdialog_virtual.h"

class fast_order_2 : public frontViewDialog_virtual
{
    Q_OBJECT
public:
    explicit fast_order_2(public_loginDialog *parent = 0);
    ~fast_order_2();
    void init_model();
    void init_control();
    void list_clear();//左边菜品信息清空
    void caculate_dishtitle();
    /*--------------------------------begin*/
public:
    QString get_currow_dishno();
    QString get_currow_dishname();
    QString get_currow_dishprice();
    QString get_currow_dishnum();
    QString get_currow_dishtotal();
    void dish_detail_clear();
signals:
    void signal_bianma_diancai(const QString &bianma);
    void signal_scanbar(const QString &scantext);
public slots:
    void to_grade0(const QList<frontViewDialog_autoexpand::content_item_icon> &item_icon_list);
    void to_grade1(const QList<frontViewDialog_autoexpand::content_item_icon> &item_icon_list);
    void to_grade2(const QList<frontViewDialog_autoexpand::content_item_icon> &item_icon_list);
public slots:
    virtual void to_dish_title_other(const QString &num, const QString &total);
    virtual void to_dish_title(float num, float total);
    virtual float get_cur_num();
    virtual void to_scanbar();
    virtual void set_num_total_to_doublescreen();

public:
    Ui_frontViewDialog_fast_2 *ui;
    /*--------------------------------end*/
public slots:
    QStringList init_dalei();
    void init_xiaolei(const QString &daleino);
    QStringList init_xiaolei(const QStringList &daleinos);
    void init_cook(const QString &xiaoleino=QString(), const QString &filter=QString());
    void init_cooks();
    void torefresh();
public:
    void init_pay_model();
    void init_tuidan_data();
    void print();
    void print_chufangdayin();
    void kitchenprinte_lost();
    void re_model();
    void init_bedi_db();
    void init_color();
    bool da_xiao;
public:
    QSqlDatabase db;
    int goodIndex;
    int goodIndex_xiaolei;
    QString goodKind;
    QString goodKind_xiaolei;
    QList<GoodItem*> itemList;
    QStringList firstSortKindIdList;
    QPointer<GoodItem> currentGoodItem;
    QStringList first_in;

    QList<goodItem_zilei*> itemList_zilei;
    QPointer<goodItem_zilei> currentGoodItem_zilei;
public slots:
    void show_tableview_click(const QModelIndex &index);
    void change_label(const QString &text);
    void to_payfor_info(const blockIns &bins);
    void showxiaolei(const QString &str);
    bool showGoods(QString kindId,int index = 0);
    bool showGoods_zilei(QString kindId, int index = 0);
    void addGood(const QString &barcode, bool manual_weight=true);
    void dis_xiaolei_str(const QString & barcode);
    void toswitchback();
    void to_usual_dish();
public slots:
    void to_exitbill();
private slots:
    void to_present();

    void to_del();

    bool to_allbillcancel();

    void to_discount();

    void to_change();

    void to_num(const QString &_dishnum=QString());

    void to_specialcook();

    void to_waimai();

    void to_waimaipay();

    void to_takeweight();

    void to_sys_xiugaimima();

    void to_pay();

    //    void to_sys_chufangdaying();

    //    void on_pushButton_15_clicked();

    //    void on_pushButton_16_clicked();

    //    void on_pushButton_17_clicked();

    //    void on_pushButton_18_clicked();

    //    void on_pushButton_19_clicked();

    void to_tuichu();

    void to_takebill();

    void print_lost(const QString & str);

    void print_louda();
    void show_print_close();
    void thread_signal();

    void to_bianma();
    void bianma_diancai(const QString &str);
    void bianma_cancel();
    void to_weixinwaimai();//微信外卖
    void to_weixinquitbill();//微信退单查询
    void to_weixinbill_import();//微信订单录入
private:
    Type_label* type_label;
    Dish_label* dish_label;
    Cookie_label* cook_label;
    QString lineEdit;
    QString lineEdit_2;
    QString lineEdit_3;
    QString lineEdit_4;
    QString lineEdit_5;
    QString lineEdit_6;
    QString lineEdit_9;
    QString label_14;
    QString liushuihao_p;
    public_mainprinter *printer;

    QString bendandanhao;
    QString louda_ip;
    QString louda_neirong;
    QString louda_title;
    QString dayindanhao;
    QTimer *print_timer;
    print_prompt * print_pr;
    int leiji;
    print_threqd *thread;
    bianma_cook *bianma;

    int item_count;
    int item_count_row;
    int item_count_zilei;
    int item_count_row_zilei;
    QGridLayout *layout_cook;
    QGridLayout *layout_zilei;
    QString bianma_str;
    QString pay_barcode;
    QLabel *label_t;
    QLabel *company;
    QString yingye_model;

    function_selection *functions;
    public_loginDialog *_parent;
    QElapsedTimer etimer;
};

#endif // FAST_ORDER_2_H
