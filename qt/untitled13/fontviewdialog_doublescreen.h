#ifndef FONTVIEWDIALOG_DOUBLESCREEN_H
#define FONTVIEWDIALOG_DOUBLESCREEN_H

#include <QDialog>
#include <QStringList>
#include "frontviewdialog_autoexpand.h"
#include "frontviewdialog_list.h"
#include <QTimer>
#include "ui_fontviewdialog_doublescreen.h"
#include "frontviewdialog_virtual.h"

class auto_expand_image;
namespace Ui {
class fontviewDialog_doublescreen;
}

class fontviewDialog_doublescreen : public frontViewDialog_virtual
{
    Q_OBJECT
public:
    explicit fontviewDialog_doublescreen(QWidget *parent = 0);
    ~fontviewDialog_doublescreen();
    QString get_currow_dishno();
    Ui::fontviewDialog_doublescreen *ui;
public slots:
    void to_show_and_hide();

    void toback_guanggao();

public slots:
    virtual void to_dish_title_other(const QString &num, const QString &total){}
    virtual void to_dish_title(float num,  float total){}
    virtual float get_cur_num(){}
    virtual void to_scanbar(){}
    virtual void set_num_total_to_doublescreen(){}
private:
    auto_expand_image*expand;
    QTimer timer;
};

#endif // FONTVIEWDIALOG_DOUBLESCREEN_H
