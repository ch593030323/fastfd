#ifndef LDS_DIALOG_INPUT_H
#define LDS_DIALOG_INPUT_H

#include <QDialog>
#include "ui_lds_dialog_input_dialog.h"
class lds_dialog_input : public QDialog
{
    Q_OBJECT
public:
    explicit lds_dialog_input(QWidget *parent = 0);

    QString _text;
public:
    Ui_lds_dialog_input_Dialog *ui;
    void set_key_hidden(bool flag);
    void set_second_hidden(bool flag);
    void set_pop_enable(bool flag);
protected slots:
    virtual void took();
    virtual void toexit();

    void update_state();
private slots:
    void set_num();
};

#endif // LDS_DIALOG_INPUT_H
