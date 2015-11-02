#ifndef BACK_WARN_H
#define BACK_WARN_H

#include <QDialog>
#include <QStandardItemModel>
//菜品沽清【243】
namespace Ui {
class back_warn;
}

class back_warn : public QDialog
{
    Q_OBJECT
    
public:
    explicit back_warn(QWidget *parent = 0);
    ~back_warn();
    void init_control();
    void thrid_true();
    void thrid_false();
    void thrid_cancel();
private slots:
    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_15_clicked();

    void on_pushButton_16_clicked();

private:
    Ui::back_warn *ui;
    QStandardItemModel *table_model;
    QStringList tejialist;
    QStringList xinzeng;
    QStringList shanchu;
    QStringList shanchutime;
    QStringList shanchu_id;
};

#endif // BACK_WARN_H
