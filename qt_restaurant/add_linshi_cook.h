#ifndef ADD_LINSHI_COOK_H
#define ADD_LINSHI_COOK_H

#include <QDialog>

namespace Ui {
class add_linshi_cook;
}

class add_linshi_cook : public QDialog
{
    Q_OBJECT
    
public:
    explicit add_linshi_cook(QWidget *parent = 0);
    ~add_linshi_cook();
    void init_control();
    bool btn_t_f;
private slots:
    void on_pushButton_15_clicked();

    void on_comboBox_ch_unitno_5_currentIndexChanged(const QString &arg1);

    void on_checkBox_ch_deductflag_4_clicked();

    void on_pushButton_7_clicked();

    void on_lineEdit_vch_dishname_4_textChanged(const QString &arg1);

    void on_comboBox_ch_deducttype_4_currentIndexChanged(const QString &arg1);

    void on_checkBox_item_flag_4_clicked();

private:
    Ui::add_linshi_cook *ui;
};

#endif // ADD_LINSHI_COOK_H
