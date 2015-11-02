#ifndef SHIFT_WORK_H
#define SHIFT_WORK_H

#include <QDialog>
#include <QStandardItemModel>
#include <QSqlQueryModel>
#include "public_mainprinter.h"

namespace Ui {
class shift_work;
}

class shift_work : public QDialog
{
    Q_OBJECT
public:
    explicit shift_work(const QString &str,QWidget *parent = 0);
    ~shift_work();
    void inti_control();
    void init_table();
    void init_data();
    void init_fast_model();
    void print();
    bool btn_t_f;
private slots:
    void on_pushButton_5_clicked();

    void on_pushButton_4_clicked();

    void print_lost(const QString & str);

    void tosaveini();
private:
    Ui::shift_work *ui;
    QStandardItemModel *model_top;
    QStandardItemModel *model_rfb;//会员充值,取款,退卡的总金额
    QString taishu;
    QString renshu;
    QString xiaofei;
    QString zhekou;
    QString zengsong;
    QString fuwufei;
    QString dixiao;
    QString moling;
    QString miandan;
    QString yingshou;
    QString shuishou;
    QString kajifen;
    QString kafue;
    QString kafue_realamount;
    QString card_fetch;
    QString card_back;
    QString card_recharge_yun;
    QString card_recharge_realamount_yun;
    QString card_fetch_yun;
    QString card_back_yun;

//    public_mainprinter *printer;
    QString _str;
};

#endif // SHIFT_WORK_H
