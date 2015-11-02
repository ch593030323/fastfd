#ifndef PUBLIC_HEARTBEATDIALOG_H
#define PUBLIC_HEARTBEATDIALOG_H

#include <QDialog>

namespace Ui {
class public_heartbeatDialog;
}

class public_heartbeatDialog : public QDialog
{
    Q_OBJECT

public:
    explicit public_heartbeatDialog(QWidget *parent = 0);
    ~public_heartbeatDialog();
public slots:
    void took();
    void toexit();
private:
    Ui::public_heartbeatDialog *ui;
};

#endif // PUBLIC_HEARTBEATDIALOG_H
