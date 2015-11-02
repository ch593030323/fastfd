#ifndef FONTVIEWDIALOG_JIULOU_DIALOG_H
#define FONTVIEWDIALOG_JIULOU_DIALOG_H

#include <QDialog>
#include "manager_view.h"

namespace Ui {
class fontviewDialog_jiulou_Dialog;
}

class fontviewDialog_jiulou_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit fontviewDialog_jiulou_Dialog(QWidget *parent = 0);
    ~fontviewDialog_jiulou_Dialog();
    void update_desk_info(int void_num, int use_num, const QString &oper, const QString &dt);
signals:
    void signal_sub_desk(const QString &text);
//    void signal_desk_sys(const QString &text);

    void signal_jinliguanli();
    void signal_shezhi();
    void signal_yuding();
    void signal_shuaxin();
    void signal_yingyeshuju();

    void signal_jiaoban();
    void signal_shoudongshuru();
    void signal_kaiqianxiang();
    void signal_tuichu();
private slots:
    void to_tabe_clicked(const QString &text);
    void filter_content();
    void print_geometry();
private:
    Ui::fontviewDialog_jiulou_Dialog *ui;
    manager_view *mview;
};

#endif // FONTVIEWDIALOG_JIULOU_DIALOG_H
