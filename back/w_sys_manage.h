#ifndef W_SYS_MANAGE_H
#define W_SYS_MANAGE_H

#include <QObject>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>
#include <QStringList>
#include <QStandardItemModel>
#include <QtDebug>
#include "backheader.h"

class w_sys_manage : public QObject
{
    Q_OBJECT
public:
    explicit w_sys_manage(QObject *parent = 0);

    //系统管理 送菜控制 对照表
    static QStandardItemModel MODEL_SYS_DISH_CONTROL;//t_m_member, ch_state

    static int MODEL_SYS_INIT();

    static QStringList get_flilename_path(const QString &path, const QStringList &filter=QStringList());//返回 一系列 文件名
    static QString set_flilename_path(const QString &flilename, const QString &path);//返回错入信息
};

#endif // W_SYS_MANAGE_H
