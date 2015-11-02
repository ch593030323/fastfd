#ifndef W_BT_DISH_TABLE_H
#define W_BT_DISH_TABLE_H

#include <QDialog>
class Ui_w_bt_dish_Dialog;
class QSqlQueryModel;
class lds_treeWidget;
class lds_model_sqlrelationaltablemodel;
class lds_tableView;
class w_bt_dish_table : public QDialog
{
    Q_OBJECT
public:
    explicit w_bt_dish_table(QWidget *parent = 0);
public slots:
    void torefresh();
    lds_treeWidget *getTreew();//获取tree
    lds_tableView *getTableview();//获取tableview
    lds_model_sqlrelationaltablemodel *getTablemodel();//获取model

private slots:
    void tonew();//新建
    void tochange();//修改
    void todel();//删除
    void tonews();//批量新建
    void totable();//桌类设置
    void toarea();//区域设置
    void toexport();//导出
    void toexit();//退出

    void treeselect();//tree update
public:
    Ui_w_bt_dish_Dialog *ui;
    lds_model_sqlrelationaltablemodel *tablemodel;
};

class Ui_w_bt_dish_table_new_Dialog;
class lds_model_sqlrelationaltablemodel;
class QDataWidgetMapper;
class QTableView;
//修改
class w_bt_dish_table_change:public QDialog{
    Q_OBJECT
public:
    explicit w_bt_dish_table_change(QWidget *parent, bool ischange=true);
private slots:
    virtual bool tosave();
    virtual void toexit();
    virtual void toup();
    virtual void todown();
    virtual void tocontinue();

    virtual void tomapperindexupdate();
protected slots:
    void mapperindexupdate();//mapper更新tableview的当前行
    void mapperindexupdate_select();//mapper更新tableview的当前行 and parent的model select，tree select

protected:
    Ui_w_bt_dish_table_new_Dialog *ui;
    QDataWidgetMapper *mapper;
    QSqlQueryModel *querymodel;
    QTableView *tableview;
    w_bt_dish_table *_parent;
    bool _ischange;

};
//新增
class w_bt_dish_table_new:public w_bt_dish_table_change{
    Q_OBJECT
public:
    explicit w_bt_dish_table_new(QWidget *parent);
private slots:
    virtual bool tosave();
    virtual void tocontinue();
};
//批量新增
class Ui_w_bt_table_news_Dialog;
class w_bt_dish_table_news:public QDialog{
    Q_OBJECT
public:
    explicit w_bt_dish_table_news(QWidget *parent);
private slots:
    void togenrate();
    void toexit();
private:
    Ui_w_bt_table_news_Dialog *ui;
    w_bt_dish_table *_parent;
};
//餐桌设置
class Ui_w_bt_dish_table_tableset_Dialog;
class w_bt_dish_table_tableset:public QDialog{
    Q_OBJECT
public:
    explicit w_bt_dish_table_tableset(QWidget *parent);
private slots:
    void toup();
    void todown();
    void tonew();
    void todel();
    bool tosave();
    void toexit();
    void toupdate_ch_serviceflagtext(bool f);
    void update_enabled(int index);

    void mapperrefresh();
    void isnewq(int r);//是否新建
private:
    Ui_w_bt_dish_table_tableset_Dialog *ui;
    lds_model_sqlrelationaltablemodel *tablemodel;
    QDataWidgetMapper *mapper;
    w_bt_dish_table *_parent;
    void mapperindexupdate();//mapper更新tableview的当前行

};
//区域设置
class Ui_w_bt_dish_unitset_Dialog;
class lds_model_sqlrelationaltablemodel;
class w_bt_dish_table_areaset:public QDialog{
    Q_OBJECT
public:
    explicit w_bt_dish_table_areaset(w_bt_dish_table *parent);
private slots:
    void tonew();
    void todel();
    void torefresh();
    bool tosave();
    void toexit();
private:
    int findareano(const QString &areano, int exceptrow);

private:
    Ui_w_bt_dish_unitset_Dialog *ui;
    lds_model_sqlrelationaltablemodel *tablemodel;
    QDataWidgetMapper *mapper;
    w_bt_dish_table *_parent;
};
#endif // W_BT_DISH_TABLE_H
