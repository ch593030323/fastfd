#ifndef CLIENTDISPLAY_H
#define CLIENTDISPLAY_H

#include <QObject>
class ClientDisplay_NO;
#ifdef CLIENTDISPLAY
#define CLIENTDISPLAY_COMMON_DLLSPEC  Q_DECL_EXPORT
#else
#define CLIENTDISPLAY_COMMON_DLLSPEC  Q_DECL_IMPORT
#endif
class CLIENTDISPLAY_COMMON_DLLSPEC ClientDisplay
{
private:
    QString _baudrate;

    ClientDisplay_NO *display;
public:
    explicit ClientDisplay(const QString &type, const QString & baudrate);
    ~ClientDisplay();
    QString _type;

    bool writeCom(int sqr, const QString &in, const QString &devpath, const QString &name=QString());
    bool cleanCom(const QString &devpath, const QString &baudrate);
    bool startShowinfo(const QString &text, const QString &devpath);
    QString lastError();

};

#endif // CLIENTDISPLAY_H
