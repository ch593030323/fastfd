#ifndef BACK_SPECIAL_ADD_H
#define BACK_SPECIAL_ADD_H

#include <QDialog>
#include <QStandardItemModel>
//今日特价 ->新增特价
namespace Ui {
class back_special_add;
}

class back_special_add : public QDialog
{
    Q_OBJECT
    
public:
    explicit back_special_add(QWidget *parent = 0);
    ~back_special_add();
    void init_control();
    QStringList yingyong;
    QStringList truelist;
    bool btn_t_f;
private slots:
    void on_pushButton_14_clicked();

    void on_pushButton_15_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_13_clicked();

private:
    Ui::back_special_add *ui;
    QStandardItemModel *table_model;
};

#endif // BACK_SPECIAL_ADD_H
