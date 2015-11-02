#ifndef CUR_PRICE_H
#define CUR_PRICE_H

#include <QDialog>

namespace Ui {
class cur_price;
}

class cur_price : public QDialog
{
    Q_OBJECT
    
public:
    explicit cur_price(const QString & str,QWidget *parent = 0);
    ~cur_price();
    bool btn_t_f;
    QString price;
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_15_clicked();

    void on_pushButton_16_clicked();

private:
    Ui::cur_price *ui;
    QString linetext;
};

#endif // CUR_PRICE_H
