#ifndef DISCOUNT_COOK_H
#define DISCOUNT_COOK_H

#include <QDialog>

namespace Ui {
class discount_cook;
}

class discount_cook : public QDialog
{
    Q_OBJECT
    
public:
    explicit discount_cook(QWidget *parent = 0);
    ~discount_cook();
    float discount;
    bool btn_t_f;
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
    Ui::discount_cook *ui;
};

#endif // DISCOUNT_COOK_H
