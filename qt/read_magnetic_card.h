#ifndef READ_MAGNETIC_CARD_H
#define READ_MAGNETIC_CARD_H

#include <QDialog>

namespace Ui {
class read_magnetic_card;
}

class read_magnetic_card : public QDialog
{
    Q_OBJECT
    
public:
    explicit read_magnetic_card(QWidget *parent = 0);
    ~read_magnetic_card();
    
private slots:
    void on_pushButton_15_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_11_clicked();

private:
    Ui::read_magnetic_card *ui;
};

#endif // READ_MAGNETIC_CARD_H
