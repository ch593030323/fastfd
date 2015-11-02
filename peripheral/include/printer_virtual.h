#ifndef PRINTER_VIRTUAL_H
#define PRINTER_VIRTUAL_H

#include <QObject>
#include<QMap>
#include<QSize>

class Printer_Virtual : public QObject
{
    Q_OBJECT
public:
    explicit Printer_Virtual();
    virtual QMap<QString/*表格特例*/, QSizeF/*宽高*/> get_speical_PRINTSIZE();

};

#endif // PRINTER_VIRTUAL_H
