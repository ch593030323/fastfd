#ifndef PRINTER_NORMAL_T2US_H
#define PRINTER_NORMAL_T2US_H
#include <QString>
#ifdef PRINTER_NORMAL_T2US
#define PRINTER_NORMAL_T2US_COMMON_DLLSPEC  Q_DECL_EXPORT
#else
#define PRINTER_NORMAL_T2US_COMMON_DLLSPEC  Q_DECL_IMPORT
#endif
class PRINTER_NORMAL_T2US_COMMON_DLLSPEC Printer_Normal_T2US
{
public:
    Printer_Normal_T2US();
    QString vidpid;
};

#endif // PRINTER_NORMAL_T2US_H
