#ifndef RESTAURANT_VIEW_2_H
#define RESTAURANT_VIEW_2_H

#include <QWidget>
#include "frontviewdialog_zhongcan.h"
#include "fontviewdialog_jiulou_dialog.h"
#include <QSqlQueryModel>
#include <QSqlRecord>
#include "table_number_message.h"
#include "public_logindialog.h"
class manager_view;
class restaurant_view_2 : public fontviewDialog_jiulou_Dialog
{
    Q_OBJECT
public:
    explicit restaurant_view_2(public_loginDialog *parent = 0);
    ~restaurant_view_2();
    void init_widget();
public slots:
    void init_cook(const QString &table_filter=QString());
    void updtae_page();
    void update_label();
    void update_deskinfo();
        void torefresh();
public:
    static QSqlQueryModel* &get_dalei() {
        return model_cook;
    }
    static QSqlQueryModel* &get_cook() {
        return model_dalei;
    }
    static QSqlQueryModel *model_dalei;
    static QSqlQueryModel *model_cook;
   // QSqlQueryModel * model_cook_on;
    static QString dalei;
    static QString table_lock;
    static QString table_danhao;
    static QString table_no;
private slots:
    void on_pushButton_10_clicked();

    void on_pushButton_clicked();

    void btn1();
    void btn2();
    void btn3();
    void btn4();
    void btn5();
    void btn6();
    void btn7();
    void btn8();
    void btn9();
    void btn0();
    void btn00();
    void btnd();
    void btndian();
    void btnxian();
    void quxiao();
    void showxiaolei(const QString &str);
    void table_message(int num = 0);//点击餐桌的操作
    void table_message(const QString &tableno);
    void show_order();//切换到快餐模式,关闭本页面
    void pay_over();//付款结束后刷新页面
    void on_pushButton_11_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_9_clicked();

private:
    QStringList first_in;
    Table_Number_Message *table_number_message;//显示餐桌控件
    QTimer *timer;
    QStringList table_name;//餐桌名字
    QStringList table_ren;//餐桌人数
    QStringList table_yuan;//餐桌消费金额
    QString kaitai_dalei;//
    QString edit_text;//edit_text不为空，可根据edit_text过滤餐桌
    int width;//全屏时屏幕宽度
    QString sys_model;//营业模式是酒楼模式还是快餐模式
    QString _init_cook_filter;
    QTimer desktimer;
    public_loginDialog *_parent;
};

#endif // RESTAURANT_VIEW_2_H
