#ifndef W_SYS_MANAGE_BASICDATAIMPORT_H
#define W_SYS_MANAGE_BASICDATAIMPORT_H

#include <QDialog>
class Ui_w_sys_manage_basicdataimport_Dialog;

class w_sys_manage_basicdataimport : public QDialog
{
    Q_OBJECT
public:
    explicit w_sys_manage_basicdataimport(QWidget *parent = 0);
protected:
    void closeEvent(QCloseEvent *e);
private slots:
    void togetfilepath();
    void tosave();
    void toexit();

    void export_info_progress(const QString &info, int step=-1, int max=-1);

private:
    Ui_w_sys_manage_basicdataimport_Dialog *ui;

    bool *isend;
};

#endif // W_SYS_MANAGE_BASICDATAIMPORT_H
