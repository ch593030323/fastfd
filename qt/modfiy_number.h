#ifndef MODFIY_NUMBER_H
#define MODFIY_NUMBER_H

#include <QDialog>

namespace Ui {
class modfiy_number;
}

class modfiy_number : public QDialog
{
    Q_OBJECT
    
public:
    explicit modfiy_number(const QString & num,QWidget *parent = 0);
    ~modfiy_number();
    bool btn_t_f;
    float modfiy_num;
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

    void on_pushButton_13_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_15_clicked();

    void on_pushButton_16_clicked();

    void on_pushButton_11_clicked();

private:
    Ui::modfiy_number *ui;
    QString yuan_num;
};

#endif // MODFIY_NUMBER_H
