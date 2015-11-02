#ifndef PUBLIC_CIKA_READ_DIALOG_H
#define PUBLIC_CIKA_READ_DIALOG_H

#include <QDialog>
#include "lds_dialog_input.h"

class public_cika_read_Dialog : public lds_dialog_input
{
    Q_OBJECT
    
public:
    explicit public_cika_read_Dialog(QWidget *parent = 0);
    ~public_cika_read_Dialog();
    QString getcardno();
};

#endif // PUBLIC_CIKA_READ_DIALOG_H
