#ifndef BACK_CASHIER_QUERY_H
#define BACK_CASHIER_QUERY_H

#include <QDialog>
#include <QStandardItemModel>
//收银流水【204】
namespace Ui {
class back_cashier_query;
}

class back_cashier_query : public QDialog
{
    Q_OBJECT
    
public:
    explicit back_cashier_query(QWidget *parent = 0);
    ~back_cashier_query();
    void init_control();
private slots:
    void on_pushButton_12_clicked();

    void on_checkBox_2_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

private:
    Ui::back_cashier_query *ui;
    QStandardItemModel *model_top;
};

#endif // BACK_CASHIER_QUERY_H
