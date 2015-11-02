#ifndef PRE_AUTOEXPAND_HELP_H
#define PRE_AUTOEXPAND_HELP_H

#include <QObject>
#include <QRadioButton>
#include <QMap>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>
#include <QGridLayout>
#include <QtCore>
#if QT_VERSION > QT_VERSION_CHECK(5,0,0)
#include <QtConcurrent/QtConcurrent>
#endif
#include "pre_autoexpand_rb.h"
#include <QLabel>

class pre_autoexpand_help : public QObject
{
    Q_OBJECT
public:
    struct PVN{
        /**
        -1 0 0
         0 0 0
         0 0 1*/
        inline PVN(int _p, int _v,int _n,const QString _sql=QString()):
            p(_p),v(_v),n(_n),sql(_sql){}
        int p;//若有上一个p=-1，否则=-2
        int v;//0的数量
        int n;//若有下一个p=1，否则=-2
        QString sql;
    };


public:
    explicit pre_autoexpand_help(QObject *parent = 0);
    int hcount;
    int vcount;
    int space;
    PVN get_next_actualcount();
    PVN get_previous_actualcount();

    QList<pre_autoexpand_rb *> pagekeylist;//即使每个page多有自己的keylist，page切换时会删除并更新
    QString sql;
    QString pixmappath;
    QSqlQuery query;
    QGridLayout *glayout;
    void glayout_deleteall();
    void pvnlist_clear();
private:
    QList<PVN> pvnlist;
};

#endif // PRE_AUTOEXPAND_HELP_H
