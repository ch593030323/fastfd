#ifndef TABLE_LOCK_H
#define TABLE_LOCK_H

#include <QDialog>
#include <QStandardItemModel>

namespace Ui {
class table_lock;
}

class table_lock : public QDialog
{
    Q_OBJECT
    
public:
    explicit table_lock(QWidget *parent = 0);
    ~table_lock();
    void init_control();
private slots:
    void on_pushButton_7_clicked();

    void on_pushButton_14_clicked();

private:
    Ui::table_lock *ui;
    QStandardItemModel *table_model;
};

#endif // TABLE_LOCK_H
