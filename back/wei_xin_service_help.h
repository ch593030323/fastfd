#ifndef WEI_XIN_SERVICE_HELP
#define WEI_XIN_SERVICE_HELP
#include <qstring.h>
#include <QMap>
#include <QNetworkReply>
#include <QEventLoop>

#include "w_sys_manage_cloudsync_basedataset.h"
class wei_xin_service_help : QObject
{
    Q_OBJECT

public:
    wei_xin_service_help(w_sys_manage_cloudsync_basedataset *ui);
    ~wei_xin_service_help();
private:
    int index;
    QString hostName;
     QString orgCode;
      QString cloudUser;
       QString cloudPass;
      QMap<QString, QString> headInfo;
      w_sys_manage_cloudsync_basedataset *myUi;
public:
      void checkUser();

};

#endif // WEI_XIN_SERVICE_HELP

