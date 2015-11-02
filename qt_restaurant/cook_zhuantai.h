#ifndef COOK_ZHUANTAI_H
#define COOK_ZHUANTAI_H

#include <QDialog>
#include <QStandardItemModel>

namespace Ui {
class cook_zhuantai;
}

class cook_zhuantai : public QDialog
{
    Q_OBJECT
    
public:
    explicit cook_zhuantai(QWidget *parent = 0);
    ~cook_zhuantai();
    void init_control();
    bool btn_t_f;
    bool print;
    QStringList flow_id;
    QStringList int_id;
    QStringList hanghao;
    QString zhuohao;
    QString danhao;
private slots:
    void on_pushButton_15_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_7_clicked();

private:
    Ui::cook_zhuantai *ui;
    QStandardItemModel *table_model;
};

#endif // COOK_ZHUANTAI_H
