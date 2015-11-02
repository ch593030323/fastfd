#ifndef PRINTER_NORMAL_58_H
#define PRINTER_NORMAL_58_H
#include "commonheader.h"
#ifdef PRINTER_NORMAL_58
#define PRINTER_NORMAL_58_COMMON_DLLSPEC  Q_DECL_EXPORT
#else
#define PRINTER_NORMAL_58_COMMON_DLLSPEC  Q_DECL_IMPORT
#endif
class PRINTER_NORMAL_58_COMMON_DLLSPEC Printer_Normal_58
{
public:
    Printer_Normal_58();
    static blockIns tran(const blockIns &bins);
};

#endif // PRINTER_NORMAL_58_H
