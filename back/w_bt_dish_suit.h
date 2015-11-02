#ifndef W_BT_DISH_SUIT_H
#define W_BT_DISH_SUIT_H

#include <QDialog>
#include "lds_model_sqlrelationaltablemodel.h"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QDebug>
#include <QSqlQueryModel>
#include "w_bt_dish.h"

class lds_tableview_sum;
class QStandardItemModel;
class Ui_w_bt_dish_suit_Dialog;


class w_bt_dish_suit : public QDialog
{
    Q_OBJECT
public:
    class RelationalTableModel_2 : public lds_model_sqlrelationaltablemodel{
    public:
        explicit RelationalTableModel_2(QObject *parent = 0,
                                          QSqlDatabase db = QSqlDatabase());
        QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const;
    };
public:
    class RelationalTableModel_3 : public lds_model_sqlrelationaltablemodel{
    public:
        explicit RelationalTableModel_3(QObject *parent = 0,
                                          QSqlDatabase db = QSqlDatabase());
        QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const;
    };
public:
    explicit w_bt_dish_suit(QWidget *parent = 0);

    lds_model_sqlrelationaltablemodel *tablemodel;
    RelationalTableModel_2 *tablemodel_2;
    RelationalTableModel_3 *tablemodel_3;

signals:
    void tomitt2t3();
public slots:
    void torefresh();//刷新
private slots:
    void t1_t2_t3();
    void t1_t2();
    void t2_t3();
    void tonew();
    void todel();
    void toreplace();
    void toreplace_del();
    bool tosave();
    void toexit();
public:
    Ui_w_bt_dish_suit_Dialog *ui;
};

//新增菜品
#include <QSortFilterProxyModel>
class Ui_w_bt_dish_suit_new_Dialog;
class lds_model_sqlrelationaltablemodel;
class QModelIndex;
class w_bt_dish_suit_new:public QDialog{
    Q_OBJECT
public:
    w_bt_dish_suit_new(w_bt_dish_suit *parent);
protected slots:
    virtual void torefresh();
    void toreset();
    virtual bool toapp();
    virtual void took();
    virtual void toexit();
protected:
    Ui_w_bt_dish_suit_new_Dialog *ui;
    lds_model_sqlrelationaltablemodel *tablemodel;
    w_bt_dish_suit *_parent;

};
//新增替换菜品
class Ui_w_bt_dish_replace_Dialog;
class w_bt_dish_suit_replace:public w_bt_dish_suit_new{
    Q_OBJECT
public:
    w_bt_dish_suit_replace(w_bt_dish_suit *parent);
protected slots:
    virtual void torefresh();
    virtual bool toapp();
    virtual void took();
    virtual void toexit();
};

#endif // W_BT_DISH_SUIT_H
