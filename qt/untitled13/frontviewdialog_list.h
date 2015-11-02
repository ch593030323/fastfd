#ifndef FRONTVIEWDIALOG_LIST_H
#define FRONTVIEWDIALOG_LIST_H

#include <QListWidget>
#include "lds_listwidget.h"
#include <QPainter>
#include <QStaticText>

class frontViewDialog_list : public lds_listwidget
{
    Q_OBJECT
public:
    struct list_item{//保存item的信息
        list_item(const QString &dishno,const QString &dishname,const QString &price,const QString &num,const QString &total,
                  const QString &no=QString(),const QString &suit=QString(),const QString &present=QString(),const QString &special=QString(),
                  const QString &cook=QString()):
            _dishno(dishno),
            _dishname(dishname),
            _price(price),
            _num(num),
            _total(total),

            _no(no),
            _suit(suit),
            _present(present),
            _special(special),

            _cook(cook)
        {}
        QString _dishno;
        QString _dishname;
        QString _price;
        QString _num;
        QString _total;

        QString _no;
        QString _suit;
        QString _present;
        QString _special;

        QString _cook;
    };
public:
    explicit frontViewDialog_list(QWidget *parent = 0);
    ~frontViewDialog_list();
    void test();
    void setCurrentRow(int row);
public slots:
    void to_payfor_info(const QString &info);
protected:
    void paintEvent(QPaintEvent *e);
private:
    QString paintinfo;
};

#endif // FRONTVIEWDIALOG_LIST_H
