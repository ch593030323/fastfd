#include "bmp_info.h"

bool Bmp_info::Pic2printIns(const QString &filename)
{
    return Pic2printIns(QImage(filename));
}

bool Bmp_info::Pic2printIns(const QImage fileimage)
{
    //  50 * 50
    /*  50 * 24
        50 * 24
        50 * 24 */
    image = fileimage;
    if(image.isNull()){
        qDebug()<<"image.isNull";
        return false;
    }

    /*获得合适高度*/
    int h = image.height();
    int row_sum = h%24 > 0 ? h/24 + 1 : h/24;

    while(row_sum--)pic_matrix.push_back(Picrow(image.width()));

    for(int y = 0; y < pic_matrix.size(); y++){
        for(int x = 0; x < pic_matrix[0].size(); x++){
            every24_row(x, y,pic_matrix[y][x]);//item会自动清零
        }//item
    }//row
    flag = true;
    return true;
}

void Bmp_info::every24_row(int x, int y, int &item)// (x,y) 24 -> item
{
    item = 0;
    unsigned m = 0;
    for(int i = 0; i < 24; i++)
    {
        if(y*24 + i < image.height()){//越界
            qint32 m = image.pixel(x,y*24 + i);
            if(m <= W_BDIVE)//黑色
                item |= 0x01;
            //白色默认
        }
        if(i >= 23)break;//若是到底则跳出，避免最后一位多0的情况
        item <<= 1;
    }
}

void Bmp_info::makeIns(QByteArray &in)
{
    /*处理图片*/

    if(!isrun())return;//pic load error

    /*获得开始打印的表头*/
    QByteArray start_head;
    //    start_head.append("1d 50 00 c8 1b 33 18");//设置行高
    start_head.append("1b 33 18");//设置行高
    qDebug()<<start_head;

    /*获得每段打印的表头*/
    QByteArray p_head;
    p_head.append("0a1b 2a 21 ");

    {
        int w = image_w();
        /*像素宽度,最大255 * 256 个像素*/
        p_head.append(QString().sprintf("%02x ",(int)(w & 0x00ff)));
        p_head.append(QString().sprintf("%02x\n",(int)(w & 0xff00)>>8));
    }

    /*对bmp图片转换成打印内容*/
    QByteArray &printer_ins = in;
    printer_ins.clear();
    QByteArray part_body;
    printer_ins.append(start_head);
    for(int i = 0; i < body_size(); i++){
        part_body.clear();
        part_body.append(p_head);
        /*获得part数量*/
        const Picrow &part =pic_matrix[i];
        /*将part转换成qbytearray*/
        for(int t = 0; t < part.size(); t++){
            part_body.append(QString().sprintf("%02x",(part[t] & 0xff0000)>>16));
            part_body.append(QString().sprintf("%02x",(part[t] & 0x00ff00)>>8));
            part_body.append(QString().sprintf("%02x",(part[t] & 0x0000ff)));
        }
        printer_ins.append(part_body);
        //        printer_ins.append("0a");
    }
}
