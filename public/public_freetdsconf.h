#ifndef PUBLIC_FREETDSCONF_H
#define PUBLIC_FREETDSCONF_H

#include <QObject>
#include <QVariant>

class public_freetdsconf{
public:
    explicit public_freetdsconf(const QString &freetds_path);
    QVariant value(const QString & key, const QVariant & defaultValue = QVariant())const;
    void setValue(const QString & key, const QVariant & value );
private:
    QString path;

};
#endif // PUBLIC_FREETDSCONF_H
