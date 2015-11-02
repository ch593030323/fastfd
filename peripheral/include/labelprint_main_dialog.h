#ifndef LABELPRINT_MAIN_DIALOG_H
#define LABELPRINT_MAIN_DIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include "s_mainwindow.h"
//1需要设置配置文件的filepath:CommonHeader::com_ini_filepath=qApp->applicationDirPath()+"/conf.ini";
//2需要设置根目录的dirpath:CommonHeader::com_root_dirpath=qApp->applicationDirPath();
//3需要判断下端口是否有重复
//4扫描条码是发送的信号 void signal_scanbar(const QString &bar);//当有条码扫描是，需要接受这个信号，对model进行添加
//5点击新增时发送的信号 void signal_add();//当点击新增时，需要接受这个信号，对model进行添加

#ifdef LABELPRINT_MAIN_DIALOG
#define LABELPRINT_MAIN_DIALOG_COMMON_DLLSPEC  Q_DECL_EXPORT
#else
#define LABELPRINT_MAIN_DIALOG_COMMON_DLLSPEC  Q_DECL_IMPORT
#endif
namespace Ui {
class labelprint_main_dialog;
}
class labelprint_set_dialog;
class labelprint_goodinfo_dialog;
class labelprint_design_dialog;

class LABELPRINT_MAIN_DIALOG_COMMON_DLLSPEC labelprint_main_dialog : public QDialog
{
    Q_OBJECT

public:
    explicit labelprint_main_dialog(QWidget *parent = 0);
    ~labelprint_main_dialog();
    QStandardItemModel *printmodel;//开放printmodel方便外部及时修改
    void inputfinished();
    void toprint();
protected:
    void goodsadd(const QList<QStringList> &itemslist);
protected slots:
    virtual void togoodadd();//添加
private slots:
    void toexit();
    void togoodremove();
    void tooverview();
    void toset();

    void toswitch();

private:
    Ui::labelprint_main_dialog *ui;

    labelprint_set_dialog *setdialog;
    labelprint_goodinfo_dialog*gooddialog;
    labelprint_design_dialog*designdialog;
};

#endif // LABELPRINT_MAIN_DIALOG_H
