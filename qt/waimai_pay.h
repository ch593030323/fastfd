#ifndef WAIMAI_PAY_H
#define WAIMAI_PAY_H

#include <QDialog>
#include <QStandardItemModel>
#include "public_mainprinter.h"
#include "w_scr_dish.h"
#include "fast_order_2.h"

namespace Ui {
class waimai_pay;
}

class waimai_pay : public QDialog
{
    Q_OBJECT
    
public:
    explicit waimai_pay(const QString &xiaofei,const QString &zhekou,const QString &zengsong,const QString &moling,const QString &shuishou,const QString &billno,const QString &waimaihao,fast_order_2 *parent = 0);
    ~waimai_pay();
    void init_control();
    void init_data();
    void init_fast_model();
    bool btn_t_f;
    void print();
private slots:
    void on_pushButton_21_clicked();

    void on_pushButton_19_clicked();

    void on_pushButton_20_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_16_clicked();

    void on_pushButton_13_clicked();

    void on_lineEdit_9_textChanged(const QString &arg1);

    void print_lost(const QString & str);

private:
    Ui::waimai_pay *ui;
    QString _xiaofei;
    QString _zhekou;
    QString _zengsong;
    QString _shuishou;
    QString _moling;
    QString _billno;
    QString _waimaihao;
    QString shifoujiazhang;
    int count_dis;
    QStandardItemModel *table_model;
    float kafujine;
    QString fukuanpingzheng;
//    public_mainprinter *printer;
    fast_order_2 *_parent;
};

#endif // WAIMAI_PAY_H
