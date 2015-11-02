#ifndef BACK_SALES_H
#define BACK_SALES_H

#include <QDialog>
#include <QStandardItemModel>
//菜品销售流水查询【203】
namespace Ui {
class back_sales;
}

class back_sales : public QDialog
{
    Q_OBJECT
    
public:
    explicit back_sales(QWidget *parent = 0);
    ~back_sales();
    void init_control();
private slots:
    void on_pushButton_12_clicked();

    void on_checkBox_2_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

private:
    Ui::back_sales *ui;
    QStandardItemModel *model_top;
};

#endif // BACK_SALES_H
