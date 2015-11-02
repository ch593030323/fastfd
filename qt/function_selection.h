#ifndef FUNCTION_SELECTION_H
#define FUNCTION_SELECTION_H

#include <QDialog>

namespace Ui {
class function_selection;
}

class function_selection : public QDialog
{
    Q_OBJECT
    
public:
    explicit function_selection(QWidget *parent = 0);
    ~function_selection();
    bool btn_t_f;
    QString fast_model;
    QString sys_model;
    static void static_toswitch();
public slots:
    void on_pushButton_12_clicked();

    void to_discount();

    void to_present();

    void to_takebill();

    void to_change();

    void to_exitbill();

    void to_allbillcancel();

    void on_pushButton_8_clicked();

    void to_num();

    void to_member_qukuan();

    void on_pushButton_5_clicked();

    void to_sys_xiugaimima();

    void to_suopint();

    void on_pushButton_11_clicked();

    void to_sys_chufangdaying();

private:
    Ui::function_selection *ui;
};

#endif // FUNCTION_SELECTION_H
