#ifndef W_I_INVENTORY_STOCKING_H
#define W_I_INVENTORY_STOCKING_H

#include <QDialog>
#include "w_i_inventory.h"
#include "w_rpt_report.h"

class QSqlQueryModel;
class QStandardItemModel;
class Ui_w_i_inventory_goodsinput_Dialog;
class lds_tableview_sum;
class lds_tableView;

class w_i_inventory_stocking : public QDialog
{
    Q_OBJECT
private:
    class QueryModel:public QSqlQueryModel{
    public:
        explicit QueryModel(QObject *parent = 0):QSqlQueryModel(parent){}
        QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const;
    };

    class StandardItemModel:public QStandardItemModel{
    public:
        explicit StandardItemModel(QObject *parent = 0):QStandardItemModel(parent){}
        Qt::ItemFlags flags(const QModelIndex &index) const;
    };
public:
    explicit w_i_inventory_stocking(QWidget *parent = 0);
    lds_tableview_sum *getTableview_2();
    w_i_inventory_stocking::StandardItemModel *getStandmodel_2();
    lds_tableView *getTableview();
    QSqlQueryModel *getQuerymodel();
    QString _type;//对外不能修改
public slots:
    void torefresh(const QString &findsheetno=QString());//为空则选中第一行，否则遍历选中sheetno所在行

private slots:
    void tonewstock();
    void toloadstock();
    void toexit();

    void toupdatetableview2(int index);
private:
    Ui_w_i_inventory_goodsinput_Dialog *ui;
    QSqlQueryModel *querymodel;
    QStandardItemModel *standmodel_2;
    
};
//查看库存单据
class Ui_w_i_inventory_stocking_change_Dialog;
class w_i_inventory_stocking_change : public QDialog{
    Q_OBJECT
public:
    explicit w_i_inventory_stocking_change(w_i_inventory_stocking *parent = 0);
protected slots:
    void toexit();

    void  tobillsave();//保存
    void  tobillcheck();//盘点审核
    void  tobillbad();//盘点作废


    void infoupdate(const QString &ls_sheetno);
    void infoupdate(int tableview_row);
    virtual void infoupdate();
    virtual void typewindow();
    void tochange();

protected:
    Ui_w_i_inventory_stocking_change_Dialog *ui;
    w_i_inventory_stocking *_parent;
    QStandardItemModel *standmodel;
    int parentrow;
};
//新增库存单据
class Ui_w_i_inventory_stocking_change_Dialog;
class w_i_inventory_stocking_new : public w_i_inventory_stocking_change{
    Q_OBJECT
public:
    explicit w_i_inventory_stocking_new(w_i_inventory_stocking *parent = 0);
protected slots:
    void togoods(const QStringList & texts);//选择物料
    void todel();//删除物料
    void tobillmaker();//盘点单生成

    virtual void infoupdate();
    virtual void typewindow();
private:
    QString getnewsheetno();
};
#endif // W_I_INVENTORY_STOCKING_H
