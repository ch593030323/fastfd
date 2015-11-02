#ifndef TABLE_STATISTICS_H
#define TABLE_STATISTICS_H

#include <QDialog>
#include <QStandardItemModel>

namespace Ui {
class table_statistics;
}

class table_statistics : public QDialog
{
    Q_OBJECT
    
public:
    explicit table_statistics(QWidget *parent = 0);
    ~table_statistics();
    void init_control();
private slots:
    void on_pushButton_14_clicked();

    void on_pushButton_7_clicked();

    void on_radioButton_clicked();

    void on_radioButton_2_clicked();

    void on_radioButton_3_clicked();

private:
    Ui::table_statistics *ui;
    QStandardItemModel *table_model;
};

#endif // TABLE_STATISTICS_H
