#ifndef LABELPRINT_GOODINFO_DIALOG_H
#define LABELPRINT_GOODINFO_DIALOG_H

#include <QDialog>
#include <QStandardItemModel>

namespace Ui {
class labelprint_goodinfo_dialog;
}

class labelprint_goodinfo_dialog : public QWidget
{
    Q_OBJECT

public:
    explicit labelprint_goodinfo_dialog(QWidget *parent = 0);
    ~labelprint_goodinfo_dialog();

    QStandardItemModel *printmodel;
    QStandardItemModel *inputallgoods_model;bool isfinishinput;

public slots:
    void tocommvalue(int column);
    void togoodremove();
    void printmodel_has_newrow(const QModelIndex &parent, int first, int last);
signals:
    void signal_add();//当点击新增时，需要接受这个信号，对model进行添加

private:
    Ui::labelprint_goodinfo_dialog *ui;

};

#endif // LABELPRINT_GOODINFO_DIALOG_H
