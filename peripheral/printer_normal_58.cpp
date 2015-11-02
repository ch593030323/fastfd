#include "printer_normal_58.h"
#include "printer_normal_80.h"

Printer_Normal_58::Printer_Normal_58()
{
}

blockIns Printer_Normal_58::tran(const blockIns &bins)
{
    return Printer_Normal_80::tran(bins);
}
