#ifndef SYSTEM_SETTING_H
#define SYSTEM_SETTING_H

#include <QDialog>
#include <QSettings>
#include "lds_messagebox.h"
#include "w_sys_manage.h"

class ElectronicCountPrice;
namespace Ui {
class system_setting;
}

class system_setting : public QDialog
{
    Q_OBJECT
public:
    explicit system_setting(QWidget *parent = 0);
    ~system_setting();
    void init_tab1();
    void init_tab2();
    void init_tab3();
    void init_tab4();
    void init_tab5();
    void init_tab6();
    void init_tab7();
    void init_tab8();
    bool btn_t_f;
    static QString get_max_serialno_add1(const QString &danhao);//获取最大的流水号,并且写入数据
public slots:
    static void to_print_label_test(QMap<QString, QRect> _str_rect_map, const QString &type, const QString &font, const QString &path, const QMap<QString, QString> &printdata);

private slots:
    void on_checkBox_10_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_3_clicked();

    void on_checkBox_17_clicked();

    void on_comboBox_30_currentIndexChanged(const QString &arg1);

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void show_data();

    void on_checkBox_49_clicked();

    void on_checkBox_50_clicked();

    void on_checkBox_51_clicked();

    void on_checkBox_52_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();//客显测试

    void on_comboBox_32_currentIndexChanged(int index);

    void on_pushButton_10_clicked();//电子秤测试

    void on_pushButton_12_clicked();//标签打印机

    void on_pushButton_17_clicked();//条码秤

    void widget_blotout_round(bool flag);

    void on_comboBox_97_currentIndexChanged(int index);

    void on_pushButton_automaketitle_clicked();
    void on_pushButton_18_clicked();

    void on_pushButton_19_clicked();

    void on_lineEdit_41_editingFinished();

    void on_lineEdit_7_editingFinished();

    void on_pushButton_ftptp_example_clicked();

    void on_comboBox_97_currentIndexChanged(const QString &arg1);

private:
    Ui::system_setting *ui;
    QSettings *configIniRead;
    QSettings *configIniWrite;
    ElectronicCountPrice *countprice_scale;
};

#endif // SYSTEM_SETTING_H
