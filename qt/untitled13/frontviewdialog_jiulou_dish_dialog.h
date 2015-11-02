#ifndef FRONTVIEWDIALOG_JIULOU_DISH_DIALOG_H
#define FRONTVIEWDIALOG_JIULOU_DISH_DIALOG_H

#include <QDialog>
#include "frontviewdialog_virtual.h"
#include "ui_frontviewdialog_jiulou_dish_dialog.h"

class frontViewDialog_jiulou_dish_dialog : public frontViewDialog_virtual
{
    Q_OBJECT
public:
    explicit frontViewDialog_jiulou_dish_dialog(QWidget *parent = 0);
    ~frontViewDialog_jiulou_dish_dialog();
    void dish_detail_clear();
    int dish_detail_currow();

    void set_btn_del_text(const QString &text);
    QString get_btn_del_text();
    void pushbutton_other_hide();
    void listpayforclear();//清空list的所有信息
signals:
    void signal_bianma_diancai(const QString &text);
    void signal_scanbar(const QString &scantext);
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
public slots:
    virtual void to_grade0(const QList<frontViewDialog_autoexpand::content_item_icon> &item_icon_list);
    virtual void to_grade1(const QList<frontViewDialog_autoexpand::content_item_icon> &item_icon_list);
    virtual void to_grade2(const QList<frontViewDialog_autoexpand::content_item_icon> &item_icon_list);
    virtual void to_add_dish_detail(const QList<frontViewDialog_list::list_item> &list_item_list);
    virtual void to_del_dish_detail(int row);
    virtual void to_update_dish_detail(const frontViewDialog_list::list_item &list_item, int row);
    virtual void to_dish_title_other(const QString &num, const QString &total);
    virtual void to_dish_title(double num,  double total);
    virtual float get_cur_num();
    virtual void to_scanbar();
public:
    Ui_frontViewDialog_jiulou_dish_dialog *ui;
    QMap<QString, QString> kvmap;
};

#endif // FRONTVIEWDIALOG_JIULOU_DISH_DIALOG_H
