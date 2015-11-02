#ifndef PRINTER_NORMAL_80_H
#define PRINTER_NORMAL_80_H
#include "commonheader.h"
#ifdef PRINTER_NORMAL_80
#define PRINTER_NORMAL_80_COMMON_DLLSPEC  Q_DECL_EXPORT
#else
#define PRINTER_NORMAL_80_COMMON_DLLSPEC  Q_DECL_IMPORT
#endif
class PRINTER_NORMAL_80_COMMON_DLLSPEC Printer_Normal_80
{
public:
    Printer_Normal_80();
    static blockIns tran(const blockIns &bins);
};

#endif // PRINTER_NORMAL_80_H
