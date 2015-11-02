#ifndef DISCOUNT_DISPOSE_H
#define DISCOUNT_DISPOSE_H

#include <QDialog>

namespace Ui {
class discount_dispose;
}

class discount_dispose : public QDialog
{
    Q_OBJECT
    
public:
    explicit discount_dispose(QWidget *parent = 0);
    ~discount_dispose();
    float zhekouzhi;
    bool btn_t_f;
private slots:
    void on_pushButton_13_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_14_clicked();

private:
    Ui::discount_dispose *ui;
};

#endif // DISCOUNT_DISPOSE_H
