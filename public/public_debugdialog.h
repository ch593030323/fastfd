#ifndef PUBLIC_DEBUGDIALOG_H
#define PUBLIC_DEBUGDIALOG_H

#include <QDialog>
#include <QTimer>
#include "lds_dialog_drog.h"

namespace Ui {
class public_debugDialog;
}

class public_debugDialog : public lds_dialog_drog
{
    Q_OBJECT

public:
    explicit public_debugDialog(QWidget *parent = 0);
    ~public_debugDialog();

private slots:
    void on_pushButton_clicked();

    void on_lineEdit_editingFinished();

private:
    Ui::public_debugDialog *ui;
    QTimer timer;
};

#endif // PUBLIC_DEBUGDIALOG_H
