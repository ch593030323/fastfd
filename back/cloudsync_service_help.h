#ifndef CLOUDSYNC_SERVICE_HELP
#define CLOUDSYNC_SERVICE_HELP
#include <qstring.h>
#include <QMap>
#include <QNetworkReply>
#include <cJSON.h>
#include <QTextCodec>
#include <QEventLoop>
#include <QProgressBar>
#include "qsettings.h"
#include "public_sql.h"
#include <QDateTime>
#include "w_sys_manage_cloudsync.h"

class cloudsync_service_help: QObject
{
    Q_OBJECT

public:
    cloudsync_service_help(QProgressBar *progressBar);
    ~cloudsync_service_help();
private:
    int index;
    int listCount;
    int upSize;
    bool isApp;
    bool stopFlag;
    QString hostName;
    QString orgCode;
    QString cloudUser;
    QString cloudPass;
    QString dateFormatStr;
    QMap<QString, QString> headInfo;
    bool execHttp(cJSON *&pArray,QString url);
    void initValue(cJSON *&pArray);
    void initDownBaseDataValue(cJSON *&pArray);
    bool isStop();
    QProgressBar *progressBar;

    QEventLoop eloop;

public:
    void upLoadBaseDate();
    void downLoadBaseDate();
    void upLoadFlowDate(QString date1,QString date2);
    void stop();

//    void sendPost(QString url,QString param,QMap<QString,QString> &headInfo);
    QNetworkReply* sendPost2(QString url,QString param,QMap<QString,QString> &headInfo);
    QNetworkReply* sendGet(QString url,QMap<QString,QString> &headInfo);
//    void next();


//public slots:
//    void httpRecevit(QNetworkReply*);

};

#endif // CLOUDSYNC_SERVICE_HELP

