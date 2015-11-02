#ifndef LABELPRINT_SET_VIDPID_DIALOG_H
#define LABELPRINT_SET_VIDPID_DIALOG_H

#include <QDialog>
#include "labelprint_set_dialog.h"
#include <QItemDelegate>

namespace Ui {
class labelprint_set_vidpid_dialog;
}

class labelprint_set_vidpid_dialog : public QDialog
{
    Q_OBJECT

public:
    explicit labelprint_set_vidpid_dialog(labelprint_set_dialog *parent = 0);
    ~labelprint_set_vidpid_dialog();

private slots:
    void on_pushButton_add_clicked();

    void on_pushButton_del_clicked();

    void on_pushButton_test_clicked();

    void on_pushButton_exit_clicked();

    void on_pushButton_app_clicked();
private:
    Ui::labelprint_set_vidpid_dialog *ui;
    labelprint_set_dialog *_parent;
};

class labelprint_set_vidpid_delegate: public QItemDelegate{
public:
    labelprint_set_vidpid_delegate(QObject *parent=0):QItemDelegate(parent){}

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};
#endif // LABELPRINT_SET_VIDPID_DIALOG_H
