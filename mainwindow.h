#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
class lds_model_sqlrelationaltablemodel;
class QDataWidgetMapper;
class QSqlRelationalTableModel;
class QSqlTableModel;

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
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
