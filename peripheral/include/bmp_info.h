#ifndef BMP_INFO_H
#define BMP_INFO_H

#include <QByteArray>
#include <QFile>
#include <QImage>
#include <QRgb>
#include <QVector>
#include <QDebug>
#ifdef BMP_INFO
#define BMP_INFO_COMMON_DLLSPEC  Q_DECL_EXPORT
#else
#define BMP_INFO_COMMON_DLLSPEC  Q_DECL_IMPORT
#endif
#define W_BDIVE (0xffdbc3c9)//(rgb)黑白分水岭，f白>，0黑<=

/**
exmaple:
    QByteArray b0;//生成的打印十六进制指令
    Bmp_info bmpmaker;
    bmpmaker.Pic2printIns(logopath);
    bmpmaker.makeIns(b0);
*/
class BMP_INFO_COMMON_DLLSPEC Bmp_info
{
public:

    //用于热敏打印机打图片生成
    typedef  QVector <int> Picrow;//每行多少个int
    typedef  QVector <Picrow> Picmatrix;
public:
    Bmp_info():flag(false){}

    bool Pic2printIns(const QString &filename = QString("0"));
    bool Pic2printIns(const QImage fileimage);

    void makeIns(QByteArray &in);

private:

    bool isrun(){return flag;}

    int image_w(){ return image.width();}

    Picrow & part(int i){return pic_matrix[i];}

    int  body_size(){return pic_matrix.size();}

    void setimage(const QImage &img){image = img;}

    void every24_row(int x, int y, int &item);//将一行转成[int]

    bool flag;//获取图片结束

    Picmatrix pic_matrix;//将图片转成对应到[int]/[int]/[int]矩阵

    QImage image;//存放原图

};

#endif // BMP_INFO_H
