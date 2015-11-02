#include "printer_normal_kf58.h"

Printer_Normal_KF58::Printer_Normal_KF58()
{
}

blockIns Printer_Normal_KF58::tran(const blockIns &bins)
{
    blockIns tmpins=bins;
    for(int c=0,c_count=tmpins.count(); c<c_count;c++){//清除
        tmpins[c].first.clear();
    }
    if(tmpins.count() > 0){
        tmpins[0].first="1B6300";
        tmpins << pieceIns("20200a20200a20200a","");//出纸
    }

    return tmpins;
}
