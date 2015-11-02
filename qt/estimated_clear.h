#ifndef ESTIMATED_CLEAR_H
#define ESTIMATED_CLEAR_H

#include <QDialog>
#include <QStandardItem>
//菜品估清查询
namespace Ui {
class estimated_clear;
}

class estimated_clear : public QDialog
{
    Q_OBJECT
    
public:
    explicit estimated_clear(QWidget *parent = 0);
    ~estimated_clear();
    void init_control();
private slots:
    void on_pushButton_5_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::estimated_clear *ui;
    QStandardItemModel *model;
};

#endif // ESTIMATED_CLEAR_H
