#ifndef FAST_QUDAN_H
#define FAST_QUDAN_H

#include <QDialog>
#include <QStandardItemModel>

namespace Ui {
class fast_qudan;
}

class fast_qudan : public QDialog
{
    Q_OBJECT
    
public:
    explicit fast_qudan(QWidget *parent = 0);
    ~fast_qudan();
    void init_control();
    bool btn_t_f;
private slots:
    void on_pushButton_14_clicked();

    void on_tableView_clicked(const QModelIndex &index);

    void on_pushButton_13_clicked();

private:
    Ui::fast_qudan *ui;
    QStandardItemModel *model_top;
    QStandardItemModel *model_buttom;
};

#endif // FAST_QUDAN_H
