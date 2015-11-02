#ifndef W_BT_DISH_COOKSET_H
#define W_BT_DISH_COOKSET_H

#include <QDialog>
#include <QMap>
#include "lds_model_sqlrelationaltablemodel.h"
#include <QSqlRecord>
#include <QDebug>
#include "w_bt_dish.h"

class Ui_w_bt_dish_cookset_Dialog;
class QSqlQueryModel;
class lds_model_sqlrelationaltablemodel;
class lds_model_sqlrelationaltablemodel;
class w_bt_dish_cookset : public QDialog
{
    Q_OBJECT

public:
    explicit w_bt_dish_cookset(QWidget *parent = 0);
    lds_model_sqlrelationaltablemodel *tablemodel_2;
    QSqlQueryModel *querymodel_3;QString sql_select_3;
    QString t2_ch_dish_typeno;
    QString t3_ch_dishno;
public slots:
    void torefresh();
private slots:
    void t1_t2_t3();
    void t1_t2();
    void t2_t3();

    void tocooktype();
    void tospecialcook();
    void todel();
    void todishset();
    void todishsets();
    void toexport();
    void toexit();
signals:
    
public slots:
private :
    Ui_w_bt_dish_cookset_Dialog *ui;
    
};

//做法类型
class Ui_w_bt_dish_unitset_Dialog;
class lds_model_sqlrelationaltablemodel;
class w_bt_dish_cookset_cooktype:public QDialog{
    Q_OBJECT
public:
    explicit w_bt_dish_cookset_cooktype(QWidget *parent = 0);
private slots:
    void tonew();
    void todel();
    bool tosave();
    void torefresh();
    void toexit();
private :
    Ui_w_bt_dish_unitset_Dialog *ui;
    lds_model_sqlrelationaltablemodel *tablemodel;
};

//特殊做法
class Ui_w_bt_dish_cookset_specialcook__Dialog;
class w_bt_dish_cookset_specialcook:public QDialog{
    Q_OBJECT
public:
    explicit w_bt_dish_cookset_specialcook(QWidget *parent = 0);
private slots:
    void tonew();
    void todel();
    bool tosave();
    void torefresh();
    void toexit();
    void totable_2();
private :
    Ui_w_bt_dish_cookset_specialcook__Dialog *ui;
    lds_model_sqlrelationaltablemodel *tablemodel;
    lds_model_sqlrelationaltablemodel *tablemodel_2;
};

//做法设置之菜品设置
class Ui_w_bt_dish_cookset_dishset_Dialog;
class w_bt_dish_cookset_dishset:public QDialog{
    Q_OBJECT
public:
    explicit w_bt_dish_cookset_dishset(QWidget *parent = 0);
private slots:
    void toselectall();
    void toselectno();
    void toapp();
    void toappall();
    void took();
    void toexit();
    void torefresh();
    void treeselect();
private:
    Ui_w_bt_dish_cookset_dishset_Dialog *ui;
    lds_model_sqlrelationaltablemodel *tablemodel_2;QString t2_ch_typeno;
    w_bt_dish_cookset *_parent;
};

#endif // W_BT_DISH_COOKSET_H
