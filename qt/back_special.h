#ifndef BACK_SPECIAL_H
#define BACK_SPECIAL_H

#include <QDialog>
#include <QStandardItemModel>
//今日特价【242】
namespace Ui {
class back_special;
}

class back_special : public QDialog
{
    Q_OBJECT
    
public:
    explicit back_special(QWidget *parent = 0);
    ~back_special();
    void init_control();
    void thrid_true();
    void thrid_false();
    void thrid_cancel();
private slots:
    void on_pushButton_16_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_14_clicked();

    void on_radioButton_clicked();

    void on_radioButton_2_clicked();

    void on_dateTimeEdit_dateChanged(const QDate &date);

private:
    Ui::back_special *ui;
    QStandardItemModel *table_model;
    QStringList tejialist;
    QStringList shanchu;
    QStringList shanchutime;
    QStringList shanchu_id;
    QStringList xinzeng;
};

#endif // BACK_SPECIAL_H
