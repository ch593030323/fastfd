#include "auto_expand_image.h"
#include <QPainter>
#include <QTimerEvent>
#include <QApplication>
#include <QDir>
#include <QList>
#include <QFileInfo>
#include <QTextOption>
#include "math.h"
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
#include <QtConcurrent>
#else
#include <QtCore>
#endif
#include <QLabel>
#include "public_sql.h"
#include "backheader.h"

//-----------------------
//|                     |
//|                     |
//|                     |
//|                     |
//-----------------------
//将矩形，按时针来分割
//#define M_E		2.7182818284590452354
//#define M_LOG2E		1.4426950408889634074
//#define M_LOG10E	0.43429448190325182765
//#define M_LN2		0.69314718055994530942
//#define M_LN10		2.30258509299404568402
//#define M_PI		3.14159265358979323846
//#define M_PI_2		1.57079632679489661923
//#define M_PI_4		0.78539816339744830962
//#define M_1_PI		0.31830988618379067154
//#define M_2_PI		0.63661977236758134308
//#define M_2_SQRTPI	1.12837916709551257390
//#define M_SQRT2		1.41421356237309504880
//#define M_SQRT1_2	0.70710678118654752440
bool auto_expand_image::isdish=false;
QString auto_expand_image::curdish_imagepath;

static QImage getscaledimage(const QImage &image)
{
    if(image.isNull()) return QImage();
    else return image.scaled(backheader::fullwindowsize,Qt::KeepAspectRatio,Qt::FastTransformation);
}

static QPixmap getscaledpixmap(const QPixmap &pixmap)
{
    if(pixmap.isNull()) return QPixmap();
    else return pixmap.scaled(backheader::fullwindowsize,Qt::KeepAspectRatio,Qt::FastTransformation);
}

static void  POP_INTERVAL_IMAGE(QObject *receiver , double paint_rotate,
                                double percent_finish, const QVector<QPolygonF> &polygon_vect,
                                const QString &imagepath){
    QPainterPath p0;
    foreach(QPolygonF polygon, polygon_vect){
        if(paint_rotate==0.0){//垂直
            polygon[2].rx()=polygon[1].rx()=polygon[0].x()+(polygon[1].x()-polygon[0].x())*percent_finish;

        } else if(paint_rotate==90.0){//水平
            polygon[3].ry()=polygon[2].ry()=polygon[1].y()+(polygon[2].y()-polygon[1].y())*percent_finish;

        } else {
            polygon[1].rx()=polygon[0].x()+(polygon[1].x()-polygon[0].x())*percent_finish;
            polygon[2].ry()=polygon[3].y()+(polygon[2].y()-polygon[3].y())*percent_finish;
        }
        QPainterPath p;
        p.addPolygon(polygon);
        p0=p0.united(p);
    }

    QImage image1=getscaledimage(QImage(imagepath));
    QPainterPath ppr;
    ppr.addPolygon(QPolygonF(QVector<QPointF>()
                             <<(QPointF(0,0))
                             <<(QPointF(image1.width(),0))
                             <<(QPointF(image1.width(),image1.height()))
                             <<(QPointF(0,image1.height()))));
    p0.intersected(ppr);

    QImage cur_image;

    if(auto_expand_image::isdish){//菜品
        cur_image=getscaledimage(QImage(auto_expand_image::curdish_imagepath));
    } else {//广告
        cur_image=QImage(backheader::fullwindowsize, QImage::Format_ARGB32);
        cur_image.fill(Qt::transparent);
        QPainter painter(&cur_image);
        painter.fillPath(p0, image1);
    }

    QMetaObject::invokeMethod(receiver,
                              "toupdate",
                              Qt::QueuedConnection,
                              Q_ARG(QImage,cur_image)
                              );
}

auto_expand_image::auto_expand_image(const QString &imagepath, const QString &dishimagepath,QWidget *parent) :
    QWidget(parent)
{
    //1
    paint_interval=1000;
    paint_rotate=45.0;
    image_running_step=0;
    image_running_step_total=10;//!=1
    image_switch_step=0;
    image_width=backheader::fullwindowsize.width();
    image_height=backheader::fullwindowsize.height();
    QFileInfoList fileinfos;

    //2
    int step_direct_meta=image_width/image_running_step_total;
    for(int s=0;s<image_running_step_total;s++)
        draw_pixmap_rects.append(QRect(step_direct_meta*s,0,step_direct_meta, image_height));

    //3
    fileinfos=QDir(imagepath).entryInfoList();
    foreach(QFileInfo info, fileinfos){
        if(info.fileName() == "."
                || info.fileName() == "..")
            continue;
        draw_pixmap_paths.append(info.filePath());
    }
    //3.5
    fileinfos=QDir(dishimagepath).entryInfoList();
    foreach(QFileInfo info, fileinfos){
        if(info.fileName() == "."
                || info.fileName() == "..")
            continue;
        draw_dish_pixmap_paths.append(info.filePath());
    }
    //4
    draw_pixmap_path_pre;
    running_text.setText(
                "<p> "
                "<span style=\"font-size:18pt;color:red;\">to</span>"
                "<span style=\"font-size:18pt;font-weight:600;\"> percent_finish=1.0*(image_running_step%image_running_step_total)/(image_running_step_total-1);</span>"
                "</p>"
                );

    QTextOption textoption;
    textoption.setWrapMode(QTextOption::NoWrap);
    running_text.setTextOption(textoption);
    running_text.setTextWidth(QFontMetrics(this->font()).width(running_text.text()));
    running_text_left=-running_text.textWidth();
    running_text_left_add=100;
    running_text_top=0;
    //5
    regenrate_polygon_vect(paint_rotate);
}

auto_expand_image::~auto_expand_image()
{

}

void auto_expand_image::regenrate_polygon_vect(double rotate)
{
    polygon_vect.clear();
    paint_rotate=rotate;

    if(rotate == 0.0){//垂直
        qreal actual_length=image_width;
        qreal actual_meta_step= actual_length/image_running_step_total;
        for(int s=0;s<image_running_step_total;s++){
            polygon_vect.append(QPolygonF(
                                    QVector<QPointF>()
                                    << QPointF(actual_meta_step*s, 0)
                                    << QPointF(actual_meta_step*(s+1), 0)
                                    << QPointF(actual_meta_step*(s+1), image_height)
                                    << QPointF(actual_meta_step*(s), image_height)
                                    ));

        }
    } else if(rotate == 90.0){//水平
        qreal actual_length=image_height;
        qreal actual_meta_step= actual_length/image_running_step_total;
        for(int s=0;s<image_running_step_total;s++){
            polygon_vect.append(QPolygonF(
                                    QVector<QPointF>()
                                    << QPointF(0, actual_meta_step*s)
                                    << QPointF(image_width, actual_meta_step*s)
                                    << QPointF(image_width, actual_meta_step*(s+1))
                                    << QPointF(0, actual_meta_step*(s+1))
                                    ));

        }
    } else if(rotate>0.0 && rotate <90.0){//0-90
        qreal paint_rotate_tmp=M_PI*rotate/180.0;
        qreal actual_length=image_width/cos(paint_rotate_tmp)-(tan(paint_rotate_tmp)*image_width-image_height)*sin(paint_rotate_tmp);
        qreal actual_meta_step= fabs(actual_length/image_running_step_total);
        for(int s=0;s<image_running_step_total;s++){
            polygon_vect.append(QPolygonF(
                                    QVector<QPointF>()
                                    << QPointF(actual_meta_step*s/cos(paint_rotate_tmp), 0)
                                    << QPointF(actual_meta_step*(s+1)/cos(paint_rotate_tmp), 0)
                                    << QPointF(0, actual_meta_step*(s+1)/sin(paint_rotate_tmp))
                                    << QPointF(0, actual_meta_step*(s)/sin(paint_rotate_tmp))
                                    ));

        }
    }
}

void auto_expand_image::reset_runningtext(const QString &text)
{
    running_text.setText(
                QString("<p> "
                        "<span style=\"font:bold 25pt;color:khaki;\">%1</span>"
                        "</p>").arg(text)
                );
    running_text.setTextWidth(QFontMetrics(this->font()).width(running_text.text()));
    running_text_left=-running_text.textWidth();
}

void auto_expand_image::run()
{
    //qRegisterMetaType<QPainterPath >("QPainterPath");
    //! =============== begin
    running_text_left+=running_text_left_add;//滚动字幕
    if(running_text_left>=image_width)running_text_left=-running_text.textWidth();

    percent_finish=1.0*(image_running_step%image_running_step_total)/(image_running_step_total-1);
    if(image_running_step%image_running_step_total==0){
        image_switch_step++;
        //获取上一个图片的path
        if(draw_pixmap_paths.count() > 0)
            draw_pixmap_path_pre=draw_pixmap_paths[(image_switch_step-1)%draw_pixmap_paths.count()];
    }
    //当前绘制的pixmap
    draw_pixmap_path_cur=draw_pixmap_paths[image_switch_step%draw_pixmap_paths.count()];
    image_running_step++;//图片切换
    //! =============== end
    QFuture<void> future = QtConcurrent::run(POP_INTERVAL_IMAGE,
                                             this,
                                             paint_rotate,
                                             percent_finish,
                                             polygon_vect,
                                             draw_pixmap_path_cur
                                             );
    future.waitForFinished();

}

void auto_expand_image::start()
{
    isdish=false;
}

void auto_expand_image::stop()
{
    isdish=true;
}

void auto_expand_image::toupdate(const QImage &image)
{
    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    _image=image;
    update();

    QTimer::singleShot(conf.value("system_setting/doublescreen_timeout", 200).toInt(),
                       this,SLOT(run()));
}

void auto_expand_image::paintEvent(QPaintEvent */*event*/)
{
    QPainter painter(this);

    //绘制上一个图片
    QPixmap p0 = getscaledpixmap(QPixmap(draw_pixmap_path_pre));
    if(!p0.isNull()){
        painter.drawPixmap(QPoint(0,0), p0);
    }
    //根据step来动态绘制图片
    if(!_image.isNull()){
        painter.drawImage(0,0,_image);
    }
    //滚动字幕
    painter.drawStaticText(running_text_left, running_text_top, running_text);
}

