#ifndef W_SYS_MANAGE_CLOUDSYNC_TRANSFER_H
#define W_SYS_MANAGE_CLOUDSYNC_TRANSFER_H

#include <QObject>
#include <QMap>
#include <QNetworkReply>
#include <cJSON.h>
#include <QTextCodec>
#include <QEventLoop>
#include <QProgressBar>
#include "qsettings.h"
#include "public_sql.h"
#include <QDateTime>
class w_sys_manage_cloudsync_transfer : public QObject
{
    Q_OBJECT
public:
    explicit w_sys_manage_cloudsync_transfer(QObject *parent = 0);
    //若cJSON* 不为空,则必须在外部 及时释放,否则导致new的空间无法及时释放
    QPair<cJSON*, QString> sendPost2(QString url,QString param,QMap<QString,QString> &headInfo);
    QPair<cJSON*, QString> sendGet(QString url,QMap<QString,QString> &headInfo);
};

#endif // W_SYS_MANAGE_CLOUDSYNC_TRANSFER_H
