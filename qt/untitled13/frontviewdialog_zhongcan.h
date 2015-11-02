#ifndef FRONTVIEWDIALOG_ZHONGCAN_H
#define FRONTVIEWDIALOG_ZHONGCAN_H

#include <QDialog>
#include <QStringList>
#include "frontviewdialog_autoexpand.h"
#include "frontviewdialog_list.h"
class Ui_frontViewDialog_zhongcan_Dialog;
class frontViewDialog_zhongcan : public QDialog
{
    Q_OBJECT
public:
    explicit frontViewDialog_zhongcan(QWidget *parent = 0);

    void update_desk_info(const QString &void_num, const QString &use_num, const QString &oper, const QString &dt);
public slots:
    void to_desk(const QList<frontViewDialog_autoexpand::content_item_icon> &item_icon_list);
    void to_sub_desk(const QList<frontViewDialog_autoexpand::content_item_icon> &item_icon_list);
    void to_desk_sys(const QList<frontViewDialog_autoexpand::content_item_icon> &item_icon_list);

    void to_desk_sys_send(const QString &text);
signals:
    void signal_desk(const QString &text);
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
private:
    Ui_frontViewDialog_zhongcan_Dialog *ui;

};

#endif // FRONTVIEWDIALOG_ZHONGCAN_H
