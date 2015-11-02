#ifndef BACK_BUSINESS_DAY_H
#define BACK_BUSINESS_DAY_H

#include <QDialog>
//营业日结
namespace Ui {
class back_business_day;
}

class back_business_day : public QDialog
{
    Q_OBJECT
    
public:
    explicit back_business_day(QWidget *parent = 0);
    ~back_business_day();
    void init_control();
private slots:
    void on_pushButton_12_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_10_clicked();

    void on_commandLinkButton_clicked();

private:
    Ui::back_business_day *ui;
};

#endif // BACK_BUSINESS_DAY_H
