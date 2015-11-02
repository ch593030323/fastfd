#include "printer_gainscha_2120tf.h"

Printer_GAINSCHA_2120TF::Printer_GAINSCHA_2120TF()
{
}

QString Printer_GAINSCHA_2120TF::toPrint(const QString &in, int piece, float height, int gap, float width)
{
    return Printer_GAINSCHA::toPrint(in, piece, QString().sprintf("%.02f", height), gap, QString().sprintf("%.02f", width), "");
}
