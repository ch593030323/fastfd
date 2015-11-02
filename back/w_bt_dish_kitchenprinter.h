#ifndef W_BT_DISH_KITCHENPRINTER_H
#define W_BT_DISH_KITCHENPRINTER_H

#include <QDialog>
#include<QSqlRelationalTableModel>
#include "lds_model_sqlrelationaltablemodel.h"
#include <QQueue>
#include <QPair>
#include "public_mainprinter.h"
#include <QDateTime>

class Ui_w_bt_dish_Dialog;
struct DATA_CONTENT{//厨打中间的内容
    DATA_CONTENT(const QString &_pre, const QString &_vch_dishname, int _value_width=-1, int _value_hang=-1):
        vch_dishname(_vch_dishname),
        value_width(_value_width),
        value_hang(_value_hang),
        pre(_pre)
    {}
    DATA_CONTENT(){}
    QString pre;//品名
    QString vch_dishname;//烧饼
    int value_width;//宽度
    int value_hang;//1
};
struct INDEX_DATA_CONTENT{
    INDEX_DATA_CONTENT(){}
    INDEX_DATA_CONTENT(const QString &_type,const QString &_ch_dishno,float _price,
                       const QVector<DATA_CONTENT>& _datas):
        type(_type),
        ch_dishno(_ch_dishno),
        price(_price),
        datas(_datas)
    {}
    QString type;
    QString ch_dishno;
    float price;//用于一桌一单的计算价格
    QVector<DATA_CONTENT> datas;
};
typedef QQueue<INDEX_DATA_CONTENT> DATACPQ;

struct K_TITLE{
    K_TITLE(){}
    K_TITLE(const QString &_pre,const QByteArray &_content,const QString _size=QString()):
        pre(_pre),
        content(_content),
        size(_size)
    {}
    QString pre;//流水号
    QByteArray content;//19
    QString size;//x1x1
};
struct K_CONTENT{
    K_CONTENT(){}
    K_CONTENT(const QQueue<INDEX_DATA_CONTENT> &_content,const QString _size=QString()):
        content(_content),
        size(_size)
    {}
    QQueue<INDEX_DATA_CONTENT> content;
    QString size;//x1x1
};
struct K_TAIL{
    K_TAIL(){}
    K_TAIL(const QString &_pre,const QString &_content,const QString _size=QString()):
        pre(_pre),
        content(_content),
        size(_size)
    {}
    QString pre;//菜品合计金额
    QString content;//71.2
    QString size;//x1x1
};
struct K_DATA_CONTENT{//厨打全部格式
    /*流水号:19
     * 开始单号:200
     */
    //    QMap<QString, QString> about_title;
    QQueue<K_TITLE >about_title;
    /*-------------------------------
     * 品名   单位   品码   数量   价格
     * ------------------------------
     *               -----饭菜------
     *1        烧饼     盘    2001     12.3
     * ------------------------------
     */
    K_CONTENT about_content;
    //        <QString/* 饭菜*/,QVector<DATA_CONTENT>/*1        烧饼     盘    2001     12.3*/ > about_content;

    /*菜品合计金额:71.2*/
    QQueue<K_TAIL> about_tail;
};
struct BINS_IP{
    blockIns bins;
    QString ip;
    QString billno;
    QDateTime datetime;
    QString key;/*"", out_*/
    float price;


};
class w_bt_dish_Kitchenprinter : public QDialog
{
    Q_OBJECT
public:
    explicit w_bt_dish_Kitchenprinter(QWidget *parent = 0);

    static K_DATA_CONTENT k_data_content_mode;
    static void k_data_content_mode_init();
    static int k_data_content_printer_width;

    static void k_data_content_example();
    static QList<BINS_IP> k_data_print(K_DATA_CONTENT &kdcontent, const QString &key/*"", out_*/,const QString &_dayindanhao);//isend = true时本次打印数据接受结束
    static void one_kdata_stragety_print(const QList<BINS_IP> &bins_ip_list);
    static QString value_of_kitchenkey(const QString &key);/*tr("厨打"), tr("出品")*/
public slots:
    void tokitchenprint();
    void toparamset();
    void toexit();
    bool tosave();
    void torefresh();
    void treeselect();
    void treeclicked();
    void tonews();
public:
    Ui_w_bt_dish_Dialog *ui;
    lds_model_sqlrelationaltablemodel *tablemodel;
    QString sql_select;
};
//厨打管理的checkbox和combobox
#include "lds_tableview_delegate_check.h"
#include <QSqlRelationalTableModel>
class w_bt_dish_Kitchenprinter_check_com : public lds_tableview_delegate_check
{
    Q_OBJECT

public:
    w_bt_dish_Kitchenprinter_check_com(QVector<int> &check_pos, const QString &indexname, QObject *parent = 0);

    QWidget *createEditor(QWidget *aParent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    QSqlRelationalTableModel *tablemodel;
};

// 厨打参数设置
class Ui_w_bt_dish_Kitchenprinter_param_Dialog;
class QSettings;
class QCheckBox;
class QComboBox;
class QSpinBox;
class w_bt_dish_Kitchenprinter_param:public QDialog{
    Q_OBJECT
public:
    explicit w_bt_dish_Kitchenprinter_param(QWidget *parent = 0);
    ~w_bt_dish_Kitchenprinter_param();
private:
    void loadini();
    void saveini();

    void widgetfromini(const QString &path, QCheckBox *checkbox);
    void widgetfromini(const QString &path, QComboBox *combobox);
    void widgetfromini(const QString &path, QSpinBox *spinbox);


private slots:
    void toexit();
    bool tosave();

    void updatecheck4(bool f);
    void updatecheck2(bool f);
    void updatecheck77(bool f);
    void updatecheck76(bool f);
private:
    Ui_w_bt_dish_Kitchenprinter_param_Dialog *ui;
    QSettings *backsetting;
};

//厨房打印机管理
class Ui_w_bt_dish_unitset_Dialog;
class lds_model_sqlrelationaltablemodel;
class lds_tableView;
class w_bt_dish_Kitchenprinter_printermanager : public QDialog
{
    Q_OBJECT
public:
    explicit w_bt_dish_Kitchenprinter_printermanager(w_bt_dish_Kitchenprinter *parent = 0);
private slots:
    void tonew();
    void todel();
    void torefresh();
    bool tosave();
    void toexit();
    void toprinttest();
private:
    Ui_w_bt_dish_unitset_Dialog *ui;
    lds_model_sqlrelationaltablemodel *tablemodel;
    w_bt_dish_Kitchenprinter *_parent;
};

//厨房打印机管理-新增-批量
class Ui_w_bt_dish_Kitchenprinter_printermanager_news_Dialog;
class w_bt_dish_Kitchenprinter_printermanager_news :public QDialog{
    Q_OBJECT
public:
    explicit w_bt_dish_Kitchenprinter_printermanager_news(w_bt_dish_Kitchenprinter *parent = 0);
public slots:
    void took();
    void toexit();
public:
    Ui_w_bt_dish_Kitchenprinter_printermanager_news_Dialog *ui;
    w_bt_dish_Kitchenprinter *_parent;
};

#include <QThread>
class w_bt_dish_Kitchenprinter_thread:public QThread
{
    Q_OBJECT
public:
    w_bt_dish_Kitchenprinter_thread(QObject *parent = 0) : QThread(parent),isbreak(false){}
    ~w_bt_dish_Kitchenprinter_thread();
    bool isbreak;
    QString vch_billno;
    void tomanageprinter(const QString &vch_billno="");
signals:
    void printererr(const QString &err, const QString &title);
protected:
    virtual void run();
};

#endif // W_BT_DISH_KITCHENPRINTER_H
