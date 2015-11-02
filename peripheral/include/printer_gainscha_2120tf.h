#ifndef PRINTER_GAINSCHA_2120TF_H
#define PRINTER_GAINSCHA_2120TF_H
#include "printer_gainscha.h"
#ifdef PRINTER_GAINSCHA_2120TF
#define PRINTER_GAINSCHA_2120TF_COMMON_DLLSPEC  Q_DECL_EXPORT
#else
#define PRINTER_GAINSCHA_2120TF_COMMON_DLLSPEC  Q_DECL_IMPORT
#endif
class PRINTER_GAINSCHA_2120TF_COMMON_DLLSPEC Printer_GAINSCHA_2120TF:public Printer_GAINSCHA
{
public:
    Printer_GAINSCHA_2120TF();
    //说明这儿height，width必须为inch，而且打印机对宽和高度敏感，类似postek
    virtual QString toPrint(const QString &in,int piece = 1, float height = 1.5, int gap = 0,float width = 1.12);
};

#endif // PRINTER_GAINSCHA_2120TF_H
