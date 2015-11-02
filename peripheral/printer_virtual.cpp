#include "printer_virtual.h"

Printer_Virtual::Printer_Virtual()
{
}

QMap<QString, QSizeF> Printer_Virtual::get_speical_PRINTSIZE()
{
    return QMap<QString/*表格特例*/, QSizeF/*宽高*/>();
}
