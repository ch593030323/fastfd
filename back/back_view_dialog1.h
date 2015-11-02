#ifndef BACK_VIEW_DIALOG1_H
#define BACK_VIEW_DIALOG1_H

#include <QDialog>
#include <QtDebug>
#include "public_logindialog.h"
#include  "back_view_2.h"

class back_view_dialog1 : public back_view_2
{
    Q_OBJECT
public:
    explicit back_view_dialog1(public_loginDialog *parent = 0);

public slots:
    void back_view_dialog_clicked(int page, int index);
private:
    QDialog *dialog;
    public_loginDialog *_parent;
};

#endif // BACK_VIEW_DIALOG1_H
