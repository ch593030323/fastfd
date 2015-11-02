#ifndef W_I_INVENTORY_GOODSINPUT_H
#define W_I_INVENTORY_GOODSINPUT_H

#include <QDialog>
#include "w_i_inventory.h"
#include "w_rpt_report.h"
#include"ui_w_i_inventory_goodsinput_dialog.h"

class QSqlQueryModel;
class QStandardItemModel;
class lds_tableview_sum;
class lds_tableView;
class w_i_inventory_goodsinput : public QDialog
{
    Q_OBJECT
private:
    class QueryModel:public QSqlQueryModel{
    public:
        explicit QueryModel(QObject *parent = 0):QSqlQueryModel(parent){}
        QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const;

    };
public:
    explicit w_i_inventory_goodsinput(QWidget *parent = 0, const QString &type=QString("RK"));
    lds_tableview_sum *getTableview_2();
    QStandardItemModel *getStandmodel_2();
    lds_tableView *getTableview();
    QSqlQueryModel *getQuerymodel();
    QString _type;//对外不能修改
    Ui_w_i_inventory_goodsinput_Dialog *ui;
public slots:
    void torefresh(const QString &findsheetno=QString());//为空则选中第一行，否则遍历选中sheetno所在行
private slots:
    void tonewbill();
    void tolookbill();
    void toexit();

    void toupdatetableview2(int index);
public slots:
private:
    QSqlQueryModel *querymodel;
    QStandardItemModel *standmodel_2;
};

//查看单据
class Ui_w_i_inventory_goodsinput_change_Dialog;
class w_i_inventory_goodsinput_virtual : public QDialog{
    Q_OBJECT
public:
    explicit w_i_inventory_goodsinput_virtual(w_i_inventory_goodsinput *parent = 0);
public slots:
    void infoupdate(int tableview_row);
    void infoupdate(const QString &ls_sheetno);
    void  tosheet();
    void  tobad();
protected slots:
    virtual void  toadd()=0;
    virtual void  todel()=0;
    virtual void  tochange()=0;
    virtual void  togoods(const QStringList & texts)=0;
    virtual bool  tosave()=0;

    virtual void typewindow()=0;//查看的话是 上一页， 下一页

    virtual void toup();
    virtual void todown();
    virtual void toexit();
protected:
    Ui_w_i_inventory_goodsinput_change_Dialog *ui;
    w_i_inventory_goodsinput *_parent;
    QStandardItemModel *standmodel;
};

//查看单据
class w_i_inventory_goodsinput_change : public w_i_inventory_goodsinput_virtual{
    Q_OBJECT
public:
    explicit w_i_inventory_goodsinput_change(w_i_inventory_goodsinput *parent = 0);
protected slots:
    void infoupdate();
    virtual void typewindow();//查看的话是 上一页， 下一页

    virtual void  toadd();
    virtual void  todel();
    virtual void  tochange();
    virtual void  togoods(const QStringList & texts);
    virtual bool  tosave();
};

//查看单据的修改
#include "ui_w_i_inventory_goodsinput_subchange_dialog.h"
class w_i_inventory_goodsinput_change_change:public QDialog{
    Q_OBJECT
public:
    explicit w_i_inventory_goodsinput_change_change(QWidget *parent = 0);
    Ui_w_i_inventory_goodsinput_subchange_Dialog *ui;
};

//新增单据
class w_i_inventory_goodsinput_new : public w_i_inventory_goodsinput_virtual{
    Q_OBJECT
public:
    explicit w_i_inventory_goodsinput_new(w_i_inventory_goodsinput *parent = 0);
private slots:
    virtual void infoupdate();
    virtual void typewindow();//新建的话指示 新增单据

    virtual void  toadd();
    virtual void  todel();
    virtual void  tochange();
    virtual void  togoods(const QStringList & texts);
    virtual bool  tosave();
private:
    QString getnewsheetno();

};
#endif // W_I_INVENTORY_GOODSINPUT_H
