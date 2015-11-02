#ifndef PRINTER_NORMAL_KF58_H
#define PRINTER_NORMAL_KF58_H
#include "commonheader.h"

#ifdef PRINTER_NORMAL_KF58
#define PRINTER_NORMAL_KF58_COMMON_DLLSPEC  Q_DECL_EXPORT
#else
#define PRINTER_NORMAL_KF58_COMMON_DLLSPEC  Q_DECL_IMPORT
#endif
class PRINTER_NORMAL_KF58_COMMON_DLLSPEC Printer_Normal_KF58
{
public:
    Printer_Normal_KF58();
    static blockIns tran(const blockIns &bins);
};

#endif // PRINTER_NORMAL_KF58_H
