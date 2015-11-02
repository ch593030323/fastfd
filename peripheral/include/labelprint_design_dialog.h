#ifndef LABELPRINT_DESIGN_DIALOG_H
#define LABELPRINT_DESIGN_DIALOG_H

#include <QDialog>
#include "s_mainwindow.h"
class Printer_POSTEK;
class printer_TSC_TTP_244_PLUS;
class Printer_ARGOX_PPLA;
class Printer_ARGOX_PPLB;
class Printer_POS_SERIAL;
class CommonHeader;
class Printer_GAINSCHA;
class QComboBox;
class S_MainWindow;
class Ui_TagprintSet;
class labelprint_set_tagbarcode_dialog;
namespace Ui {
class labelprint_design_dialog;
}

class labelprint_design_dialog : public QWidget
{
    Q_OBJECT
public:
    enum DIRECT{
        LEFTTOP,
        RIGHTBOTTOM
    };
public:
    explicit labelprint_design_dialog(QWidget *parent = 0);
    ~labelprint_design_dialog();
    S_MainWindow *design;
    labelprint_set_tagbarcode_dialog *setdialog;
    void setdialog_save();
    void tosave();//保存默认的
    void toload();//导入默认的


public slots:
    void toprint(bool istest=false, bool pop=true);
private slots:
    void toswitch();
    void todefbill();
    void toimportbill();
    void tosavebill();

    void totestprint();
    void on_pushButton_importbill_clicked();

private:
    QString print_piece_POSTEK(Printer_POSTEK &printer, int rr, int start_x = 0);
    QString print_piece_TSC(printer_TSC_TTP_244_PLUS &printer, int rr, int start_x = 0);
    QString print_piece_ARGOX_PPLA(Printer_ARGOX_PPLA &printer, int rr, int start_x = 0);
    QString print_piece_ARGOX_PPLB(Printer_ARGOX_PPLB &printer, int rr, int start_x = 0);
    QString print_piece_GAINSCHA(Printer_GAINSCHA &printer, int rr, int start_x = 0);

    float valueTranslate(float value, const QString &unit, const QString &rate);
    SSRPairMap ssrmapTranslate(const QString &unit, const QString &rate, DIRECT origin);
    void rectDiv(QRect &rect, double factor, DIRECT origin, const QSize &p1);
private:
    Ui::labelprint_design_dialog *ui;
};

#endif // LABELPRINT_DESIGN_DIALOG_H
