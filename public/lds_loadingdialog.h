#ifndef LDS_LOADINGDIALOG_H
#define LDS_LOADINGDIALOG_H

#include <QDialog>
#include <QMovie>

#include "ui_lds_loadingdialog.h"


class lds_loadingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit lds_loadingDialog(QWidget *parent = 0);
    ~lds_loadingDialog();
public slots:
    void updatetext(const QString &text);
public:
    Ui_lds_loadingDialog *ui;
    QMovie *movie;
};

#endif // LDS_LOADINGDIALOG_H
