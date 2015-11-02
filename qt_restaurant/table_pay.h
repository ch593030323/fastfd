#ifndef TABLE_PAY_H
#define TABLE_PAY_H

#include <QDialog>
#include <QStandardItemModel>
#include "public_mainprinter.h"
#include "w_scr_dish.h"

namespace Ui {
class table_pay;
}

class table_pay : public QDialog
{
    Q_OBJECT
    
public:
    explicit table_pay(QWidget *parent = 0);
    ~table_pay();
    void init_control();
    void init_data();
    bool btn_t_f;
    void print();
    void kitchenprinte_lost();
private slots:
    void on_pushButton_21_clicked();

    void on_pushButton_15_clicked();

    void on_pushButton_17_clicked();

    void on_pushButton_18_clicked();

    void on_pushButton_19_clicked();

    void on_pushButton_20_clicked();

    void on_pushButton_22_clicked();

    void on_pushButton_23_clicked();

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

    void on_pushButton_24_clicked();

signals:
    void change();
    void yudadan();
private:
    Ui::table_pay *ui;
    QStandardItemModel table_model;
    float kafujine;
    QString fukuanpingzheng;
    bool shifouyouhuika;
    QString youhuikahao;
    QString timer;
    QString diancaishijian;
    int count_dis;
    QString wdanhao;
    QString pay_timer;
    QString liushuihao_p;
    QString huyuankayue;
//    public_mainprinter *printer;
    QString bendandanhao;
    QString louda_ip;
    QString louda_neirong;
    QString louda_title;
    QString dayindanhao;
    float yingshou_jine;
};

#endif // TABLE_PAY_H
