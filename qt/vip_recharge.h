#ifndef VIP_RECHARGE_H
#define VIP_RECHARGE_H

#include <QDialog>

namespace Ui {
class vip_recharge;
}

class vip_recharge : public QDialog
{
    Q_OBJECT
    
public:
    explicit vip_recharge(QWidget *parent = 0);
    ~vip_recharge();
    void init_control();
private slots:
    void on_pushButton_5_clicked();

    void on_pushButton_7_clicked();

private:
    Ui::vip_recharge *ui;
};

#endif // VIP_RECHARGE_H
