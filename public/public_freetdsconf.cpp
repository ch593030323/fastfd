
#include "public_freetdsconf.h"
#include <QSettings>
#include <QFile>

public_freetdsconf::public_freetdsconf(const QString &freetds_path) :path(freetds_path)
{
}

void public_freetdsconf::setValue(const QString &key, const QVariant &value)
{
    QFile f(path);
    QSettings c(path, QSettings::NativeFormat);

    f.open(QFile::ReadOnly);
    QByteArray pre;
    while(!f.atEnd()){
        QByteArray r = f.readLine();
        if(r.startsWith(";\t")){
            pre += r;
            pre += '\n';
        }
    }
    f.close();

    c.setValue(key, value);
    c.sync();

    f.open(QFile::ReadOnly);
    QByteArray b = f.readAll();
    f.close();

    //若是第一次修改，则需要替换下面数据
    QByteArray text = QByteArray::fromPercentEncoding(b);
    if(text.startsWith("[global]\ntext size")){
        text.replace("[global]\ntext size",
                     "[global]\n"+pre+"text size");
    }
    text.replace(" = ", "=");
    text.replace("=", " = ");

    f.open(QFile::WriteOnly);
    f.write(text);
    f.close();
}

QVariant public_freetdsconf::value(const QString &key, const QVariant &defaultValue) const
{
    return QSettings(path, QSettings::IniFormat).
            value(key, defaultValue);
}
