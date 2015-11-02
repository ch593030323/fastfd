#ifndef COOK_OPERATION_H
#define COOK_OPERATION_H

#include <QDialog>
#include <QStandardItemModel>

namespace Ui {
class cook_operation;
}

class cook_operation : public QDialog
{
    Q_OBJECT
    
public:
    explicit cook_operation(const QString &str,QWidget *parent = 0);
    ~cook_operation();
    void init_control();
    QString _str;
    bool btn_t_f;
    bool print;
    QString zhuan_danhao;
    QString zhuan_zhuohao;
private slots:
    void on_pushButton_15_clicked();

    void on_tableView_clicked(const QModelIndex &index);

    void on_pushButton_7_clicked();

    void on_checkBox_clicked();

private:
    Ui::cook_operation *ui;
    QStandardItemModel *table_model;
};

#endif // COOK_OPERATION_H
