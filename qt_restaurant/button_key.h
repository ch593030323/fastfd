#ifndef BUTTON_KEY_H
#define BUTTON_KEY_H

#include <QDialog>

namespace Ui {
class button_key;
}

class button_key : public QDialog
{
    Q_OBJECT
    
public:
    explicit button_key(QWidget *parent = 0);
    ~button_key();
    bool btn_t_f;
signals:
    void btn1();
    void btn2();
    void btn3();
    void btn4();
    void btn5();
    void btn6();
    void btn7();
    void btn8();
    void btn9();
    void btn0();
    void btn00();
    void btnd();
    void btndian();
    void btnxian();
    void quxiao();
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_15_clicked();

    void on_pushButton_16_clicked();

private:
    Ui::button_key *ui;
};

#endif // BUTTON_KEY_H
