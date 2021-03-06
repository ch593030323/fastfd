#ifndef W_M_MEMBER_STATEMANAGE_H
#define W_M_MEMBER_STATEMANAGE_H

#include <QDialog>
#include "lds_model_sqlrelationaltablemodel.h"
class Ui_w_m_member_statemanage_Dialog;
class lds_tableView;
class w_m_member_statemanage : public QDialog
{
    Q_OBJECT
public:
    class RelationalTableModel : public lds_model_sqlrelationaltablemodel{
    public:
        explicit RelationalTableModel(QObject *parent = 0,
                                          QSqlDatabase db = QSqlDatabase());
        QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const;
        Qt::ItemFlags flags(const QModelIndex &index) const;

    };
public:
    explicit w_m_member_statemanage(QWidget *parent = 0);
    lds_tableView *getTableview();
    lds_model_sqlrelationaltablemodel *getTablemodel();
public slots:
    void torefresh();

private slots:
    void todetail();
    void tostop();
    void toloss();
    void toblank();
    void toopen();
    void toblackcash();
    void tofetch();
    void toblankcard();
    void tonewsend();
    void toexit();
private:
    Ui_w_m_member_statemanage_Dialog *ui;
    lds_model_sqlrelationaltablemodel *tablemodel_2;QString t2_ch_typeno; QStringList headerstmp_2;
};

//会员状态处理
class Ui_w_m_member_state_Dialog;
class w_m_member_statemanage_state:public QDialog{
    Q_OBJECT
public:
    explicit w_m_member_statemanage_state(QWidget *parent = 0);
    void setTitile(const QString &text);
    QString getExplain();
    QString textkey;
private slots:
    void took();
    void tocancel();
private:
    Ui_w_m_member_state_Dialog *ui;
};
//会员状态处理
class Ui_w_m_member_trans_Dialog;
class uo_rw_member;
class w_m_member_statemanage_trans:public QDialog{
    Q_OBJECT
public:
    explicit w_m_member_statemanage_trans(QWidget *parent = 0);
    ~w_m_member_statemanage_trans();
    bool initData1();
    //    void setTitile(const QString &text);
    //    QString getExplain();
    //    QString textkey;
private slots:
    void tonewcard();
    void toexit();
    QString wf_addsendcard();//有数据库update insert
private:
    Ui_w_m_member_trans_Dialog *ui;
    uo_rw_member *cb_read1, *cb_read2;
    w_m_member_statemanage *_parent;
    QString is_type, is_lmemberno;
    int ii_basedata;
};

#endif // W_M_MEMBER_STATEMANAGE_H
