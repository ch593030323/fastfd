#ifndef AUTO_EXPAND_IMAGE_H
#define AUTO_EXPAND_IMAGE_H

#include <QWidget>
#include <QBasicTimer>
#include <QStaticText>
#include <QTimer>


class auto_expand_image : public QWidget
{
    Q_OBJECT
public:
    explicit auto_expand_image(const QString &imagepath,const QString &dishimagepath, QWidget *parent = 0);
    ~auto_expand_image();
    int paint_interval;//paintEvent的间隔时间
    double paint_rotate;//paintEvent的旋转角度

    int image_running_step;//图片的runstep，意思是百叶窗的进度
    int image_running_step_total;
    int image_switch_step;//图片的switchstep,意思是第几张图片当前
    double image_width;
    double image_height;

    QList<QRect> draw_pixmap_rects;
    QVector<QPolygonF> polygon_vect;
    QList<QString> draw_pixmap_paths;
    QList<QString> draw_dish_pixmap_paths;
    QString draw_pixmap_path_pre;
    QString draw_pixmap_path_cur;

    QStaticText running_text;
    int running_text_left;
    int running_text_left_add;
    int running_text_top;

    void regenrate_polygon_vect(double rotate);
    void reset_runningtext(const QString &text);

    static bool isdish;
    static QString curdish_imagepath;

public slots:
    void start();
    void stop();//停止

    void run();//开始双屏
    void toupdate(const QImage &image);

protected:
    void paintEvent(QPaintEvent *event);
private:
    QImage _image;
    double percent_finish;
};

#endif // AUTO_EXPAND_IMAGE_H
