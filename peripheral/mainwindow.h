#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
class ElectronicCountPrice;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_comboBox_currentIndexChanged(int index);

    void on_groupBox_4_toggled(bool arg1);
    void on_groupBox_3_toggled(bool arg1);
    void on_groupBox_2_toggled(bool arg1);
    void on_groupBox_toggled(bool arg1);
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_comboBox_7_currentIndexChanged(int index);

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

private:
    Ui::MainWindow *ui;
    ElectronicCountPrice *countprice_scale;
};

#endif // MAINWINDOW_H
