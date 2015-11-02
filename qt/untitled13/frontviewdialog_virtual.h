#ifndef FRONTVIEWDIALOG_VIRTUAL_H
#define FRONTVIEWDIALOG_VIRTUAL_H

#include <QDialog>
#include "frontviewdialog_list.h"
#include "frontviewdialog_autoexpand.h"
#include "commonheader.h"
#include <QStandardItemModel>
#include <QSqlQueryModel>
#include <QDateTime>
#include <QTimer>

class ClientDisplay;
class frontViewDialog_virtual : public QDialog
{
    Q_OBJECT
public:
    explicit frontViewDialog_virtual(QWidget *parent = 0);
    ~frontViewDialog_virtual();
Q_SIGNALS:
    void signal_grade0(const QString &text);
    void signal_grade1(const QString &text);
    void signal_grade2(const QString &text);

    void signal_grade4(const QString &text);

    void signal_display_price(const QString &text);
    void signal_display_total(const QString &text);
    void signal_display_cash(const QString &text);
    void signal_display_change(const QString &text);
    void signal_display_clear(const QString &text);

    void signal_payfor_info(const blockIns &bins);
public:
    inline void emit_signal_display_price(const QString &text){emit signal_display_price(text);}
    inline void emit_signal_display_total(const QString &text){emit signal_display_total(text);}
    inline void emit_signal_display_cash(const QString &text){emit signal_display_cash(text);}
    inline void emit_signal_display_change(const QString &text){emit signal_display_change(text);}
    inline void emit_signal_display_clear(const QString &text){emit signal_display_clear(text);}
    inline void emit_signal_payfor_info(const blockIns &bins){emit signal_payfor_info(bins); }
public slots:
    void toprintsize();
    /*客显*/
public slots:
    void display_price(const QString &value);
    void display_total(const QString &value);
    void display_cash(const QString &value);
    void display_change(const QString &value);
    void display_clear(const QString &value);
    void display(int sqr, const QString &value);

public slots:
    virtual void to_dish_title_other(const QString &num, const QString &total)=0;
    virtual void to_dish_title(float num,  float total)=0;
    virtual float get_cur_num()=0;
    virtual void to_scanbar()=0;
    virtual void set_num_total_to_doublescreen()=0;

    virtual void addGood(int row);
    virtual void toDel();
    virtual void toPay();
public:
    static QStandardItemModel modelBase;
    static QItemSelectionModel modelBase_select;
    static QSqlQueryModel model_dalei;
    static QSqlQueryModel model_xiaolei;
    static QSqlQueryModel model_cook;
    static int row_n;
    static QString dalei;
    static QString xiaolei;
    static int num_da;
    static int num_xiao;
    static int num_cai;
    static bool changyongcook;
    static bool bianma_daicai_type;
    static QString diancaitime;
    static QStringList first_in1;
    static QString cur_desk_no;//1当前桌号 中餐和快餐下将会不同
    static QDateTime cook_time;//1中餐
    static QString k_fuwuyuan;//1中餐
    static QString k_beizhu;//1中餐
    static QString k_renshu;//1中餐
    static float _num;
    static float _total;
    static bool modelbase_headerlabel_isset;
private:
    ClientDisplay *display_led;
    QTimer timer;

};

#endif // FRONTVIEWDIALOG_VIRTUAL_H
