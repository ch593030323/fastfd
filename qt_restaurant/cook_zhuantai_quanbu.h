#ifndef COOK_ZHUANTAI_QUANBU_H
#define COOK_ZHUANTAI_QUANBU_H

#include <QDialog>
#include <QStandardItemModel>

namespace Ui {
class cook_zhuantai_quanbu;
}

class cook_zhuantai_quanbu : public QDialog
{
    Q_OBJECT
    
public:
    explicit cook_zhuantai_quanbu(const QString & str,QWidget *parent = 0);
    ~cook_zhuantai_quanbu();
    void init_control();
    bool btn_t_f;
    QString zhuan_zhuohao;
private slots:
    void on_pushButton_15_clicked();

    void on_pushButton_7_clicked();

    void on_tableView_clicked(const QModelIndex &index);

private:
    Ui::cook_zhuantai_quanbu *ui;
    QStandardItemModel *table_model;
    QString _str;
};

#endif // COOK_ZHUANTAI_QUANBU_H
