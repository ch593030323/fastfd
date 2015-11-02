#ifndef FRONTVIEWDIALOG_ZHONGCAN_DIANCAI_H
#define FRONTVIEWDIALOG_ZHONGCAN_DIANCAI_H

#include <QDialog>
#include <QStringList>
#include "frontviewdialog_autoexpand.h"
#include "frontviewdialog_list.h"
#include "ui_frontviewdialog_zhongcan_diancai_dialog.h"

class frontViewDialog_zhongcan_diancai : public QDialog
{
    Q_OBJECT
public:
    explicit frontViewDialog_zhongcan_diancai(QWidget *parent = 0);
    ~frontViewDialog_zhongcan_diancai();
    void dish_detail_clear();
    int dish_detail_currow();

    QString get_btn_del_text();
    void set_btn_del_text(const QString &text);

public slots:
    void to_grade0(const QList<frontViewDialog_autoexpand::content_item_icon> &item_icon_list);
    void to_grade1(const QList<frontViewDialog_autoexpand::content_item_icon> &item_icon_list);
    void to_grade2(const QList<frontViewDialog_autoexpand::content_item_icon> &item_icon_list);
    void to_dish_detail(const QList<frontViewDialog_list::list_item> &list_item_list);
    void to_desk_detail(const QStringList &list_item);

    void to_add_dish_detail(const QList<frontViewDialog_list::list_item> &list_item_list);
    void to_del_dish_detail(int row);
    void to_update_dish_detail(const frontViewDialog_list::list_item &list_item, int row);

    void to_update_desk_info(const QString &label_deskno=QString(), const QString &label_deskoperid=QString(),const QString &label_deskperson=QString(),
                             const QString &label_opemtime=QString(),const QString &label_openbill=QString(), const QString &label_openperson=QString() );
    void to_dish_title(const QString &num, const QString &total);

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
private slots:
    void to_btn_text();
protected:
    Ui_frontviewdialog_zhongcan_diancai_Dialog *ui;
};

#endif // FRONTVIEWDIALOG_ZHONGCAN_DIANCAI_H
