#ifndef TABLE_ORDER_2_H
#define TABLE_ORDER_2_H

#include <QDialog>
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
#include "restaurant_gooditem_zilei.h"
#include "fontviewdialog_jiulou_diancan.h"
#include "w_scr_dish.h"
#include "ui_frontviewdialog_jiulou_dish_dialog.h"
#include "frontviewdialog_virtual.h"
class frontViewDialog_virtual;
class table_order_2 : public frontViewDialog_virtual
{
    Q_OBJECT
public:
    explicit table_order_2(QWidget *parent = 0);
    ~table_order_2();
    void init_model();
    void init_control();
    /*----------------------------------------being*/
signals:
    void signal_bianma_diancai(const QString &text);
    void signal_scanbar(const QString &scantext);
public slots:
    void to_grade0(const QList<frontViewDialog_autoexpand::content_item_icon> &item_icon_list);
    void to_grade1(const QList<frontViewDialog_autoexpand::content_item_icon> &item_icon_list);
    void to_grade2(const QList<frontViewDialog_autoexpand::content_item_icon> &item_icon_list);

public slots:
    virtual void to_dish_title_other(const QString &num, const QString &total);
    virtual void to_dish_title(float num,  float total);
    virtual float get_cur_num();
    virtual void to_scanbar();
    virtual void set_num_total_to_doublescreen();
public:
    Ui_frontViewDialog_jiulou_dish_dialog *ui;
    /*----------------------------------------end*/
public slots:
    QStringList init_dalei();
    void init_xiaolei(const QString &daleino);
    QStringList init_xiaolei(const QStringList &daleinos);
    void init_cook(const QString &xiaoleino=QString(), const QString &filter=QString());
    void init_cook(const QStringList &xiaoleinos);
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
    void print_zhuantai();
    void print_zhuanquan();
    void print_back_one();
    void print_back_all();
    void init_yuda_data();
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

    QList<restaurant_gooditem_zilei*> itemList_zilei;
    QPointer<restaurant_gooditem_zilei> currentGoodItem_zilei;
public slots:
    void show_tableview_click(const QModelIndex &index);
    void change_label();
    void showxiaolei(const QString &str);
    bool showGoods(QString kindId,int index = 0);
    bool showGoods_zilei(QString kindId, int index = 0);
    void addGood(const QString &barcode);
    void dis_xiaolei_str(const QString & barcode);
    void print_lost(const QString & str);
    void print_louda();
    void show_print_close();
    void thread_signal();
    void caculate_dishtitle();

    void bianma_diancai(const QString &str);
    void bianma_cancel();
    void timer();
    static bool tocanceldeak();
    static bool toswitchdesk();

private slots:
    void on_pushButton_22_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_clicked();

    void on_pushButton_21_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_tableView_clicked(const QModelIndex &index);

    void on_pushButton_14_clicked();

    void on_pushButton_15_clicked();

    void on_pushButton_16_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_23_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_24_clicked();

    void pay_yudadan();

    bool toluodan();

signals:
    void pay_over();
private:
    QString lineEdit;
    QString lineEdit_2;
    QString lineEdit_3;
    QString lineEdit_4;
    QString lineEdit_5;
    QString lineEdit_6;
    QString lineEdit_9;
    QString label_14;
    QString liushuihao_p;
    //    public_mainprinter *printer;

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
    QString k_danhao;
    QString k_zhuohao;
    QString k_renshu;
    QString k_kaitairen;
    QString k_time;
    QString zhuanprint;
    QString backone;
    QString back_diancaitime;
    bool tui_cai;
    bool cui_cai;
    bool luo_dan;
    QString _xiaofei;
    QString _zhekou;
    QString _moling;
    QString _yingshou;
    QString _renjjun;
    QString _shuishou;
    QString _zengsong;
    QTimer *label_time;
    QLabel *label_t;
    QLabel *company;

};

#endif // TABLE_ORDER_2_H
