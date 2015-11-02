#ifndef FONTVIEWDIALOG_JIULOU_DIANCAN_H
#define FONTVIEWDIALOG_JIULOU_DIANCAN_H

#include <QDialog>
#include "frontviewdialog_jiulou_dish_dialog.h"

class fontviewDialog_jiulou_diancan : public frontViewDialog_jiulou_dish_dialog
{
    Q_OBJECT
public:
    explicit fontviewDialog_jiulou_diancan(QWidget *parent = 0);
    void set_btn_del_text(const QString &text);
    QString get_btn_del_text();
    void pushbutton_other_hide();
signals:
    void signal_bianma_diancai(const QString &text);
signals:
    void signal_alldesk_cuicai();
    void signal_cancel_desk();
    void signal_changedish();
    void signal_changyong();
    void signal_cook();

    void signal_cuicai();
    void signal_del();
    void signal_desk_info();
    void signal_discount();
    void signal_dish_switch_desk();

    void signal_jia();
    void signal_jian();
    void signal_present();
    void signal_shuliang();
    void signal_switch_desk();

    void signal_timedish();
    void signal_yuding();
    void signal_bianma();
    void signal_luodan();
    void signal_jisuan();

    void signal_tuichu();

    void signal_grade0(const QString &text);
    void signal_grade1(const QString &text);
    void signal_grade2(const QString &text);
};

#endif // FONTVIEWDIALOG_JIULOU_DIANCAN_H
