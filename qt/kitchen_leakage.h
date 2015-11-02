#ifndef KITCHEN_LEAKAGE_H
#define KITCHEN_LEAKAGE_H

#include <QDialog>
#include <QStandardItemModel>
#include <QSqlDatabase>
#include <QTimer>

namespace Ui {
class kitchen_leakage;
}

class kitchen_leakage : public QDialog
{
    Q_OBJECT
    
public:
    explicit kitchen_leakage(QWidget *parent = 0);
    ~kitchen_leakage();
    void init_control();
    void re_model();
private slots:
    void on_pushButton_7_clicked();

    void on_radioButton_clicked();

    void on_radioButton_2_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

    void update_refresh(bool flag);

    void update_refresh_time(const QString &time);

    void torefresh();

    void to_auto_print(bool flag);

    void on_pushButton_11_clicked();

private:
    Ui::kitchen_leakage *ui;
    QStandardItemModel *tab_model;
    QSqlDatabase lostform_db;
    QTimer refresh_timer;

};

#endif // KITCHEN_LEAKAGE_H
