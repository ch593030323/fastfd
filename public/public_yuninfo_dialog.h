#ifndef PUBLIC_YUNINFO_DIALOG_H
#define PUBLIC_YUNINFO_DIALOG_H

#include <QDialog>
#include "ui_public_yuninfo_dialog.h"

namespace Ui {
class public_yuninfo_Dialog;
}

class public_yuninfo_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit public_yuninfo_Dialog(QWidget *parent = 0);
    ~public_yuninfo_Dialog();

public:
    Ui::public_yuninfo_Dialog *ui;
};

#endif // PUBLIC_YUNINFO_DIALOG_H
