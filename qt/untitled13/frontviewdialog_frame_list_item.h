#ifndef FRONTVIEWDIALOG_FRAME_LIST_ITEM_H
#define FRONTVIEWDIALOG_FRAME_LIST_ITEM_H

#include <QFrame>
#include "ui_frontViewDialog_list_item_dialog.h"
#include <QRadioButton>
#include <QListWidgetItem>

class frontviewdialog_frame_list_item : public QRadioButton
{
    Q_OBJECT
public:
    explicit frontviewdialog_frame_list_item(QListWidgetItem *parentitem,QWidget *parent = 0);
    frontviewdialog_frame_list_item(QWidget *parent = 0);
    void init();
    ~frontviewdialog_frame_list_item();
    Ui_frontViewDialog_list_item_Dialog *fui;
    void p5_append(const QString &text);
    void p5_clear();
    void close_press_color_red();
    QListWidgetItem *_item;
public slots:
    void update_fui(bool flag);
private:
    bool is_close_press_color_red;

};

#endif // FRONTVIEWDIALOG_FRAME_LIST_ITEM_H
