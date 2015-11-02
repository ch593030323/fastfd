#ifndef CONSUMPTION_VEIW_H
#define CONSUMPTION_VEIW_H

#include <QDialog>
#include <QStandardItemModel>
#include "public_mainprinter.h"

namespace Ui {
class consumption_veiw;
}

class consumption_veiw : public QDialog
{
    Q_OBJECT
    
public:
    explicit consumption_veiw(QWidget *parent = 0);
    ~consumption_veiw();
    void init_control();
private slots:
    void on_pushButton_6_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void print_lost(const QString & str);
private:
    Ui::consumption_veiw *ui;
    QStandardItemModel *model_top;
    QStandardItemModel *model_buttom;
    QString shuliang_num;
    QString zongji_num;
//    public_mainprinter *printer;
};

#endif // CONSUMPTION_VEIW_H
