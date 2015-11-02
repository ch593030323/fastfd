#ifndef LABELPRINT_SET_TAGBARCODE_DIALOG_H
#define LABELPRINT_SET_TAGBARCODE_DIALOG_H

#include <QDialog>

namespace Ui {
class labelprint_set_tagbarcode_dialog;
}
class Printer_Virtual;
class labelprint_set_tagbarcode_dialog : public QWidget
{
    Q_OBJECT

public:
    explicit labelprint_set_tagbarcode_dialog(QWidget *parent = 0);
    ~labelprint_set_tagbarcode_dialog();

    void afterload();
public slots:
    void tosave();
    void toexit();
private slots:

    void update3fengbianlv(const QString &fengbianlv);
    void update3danwei(const QString &danwei);
    void update3label();
    void on_pushButton_normalprint_app_clicked();

private:
    Ui::labelprint_set_tagbarcode_dialog *ui;
    QString pre_danwei;
    QString pre_fengbianlv;
    Printer_Virtual *printer;

};

#endif // LABELPRINT_SET_TAGBARCODE_DIALOG_H
