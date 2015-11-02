#ifndef LDS_FRAME_PIXMAP_H
#define LDS_FRAME_PIXMAP_H

#include <QFrame>
#include <QLabel>

class lds_frame_pixmap : public QFrame
{
    Q_OBJECT
public:
    explicit lds_frame_pixmap(QWidget *parent = 0);
public slots:
    void drawPixmap(const QPixmap &pixmap);
private:
    QLabel *labelicon;
};
#endif // LDS_FRAME_PIXMAP_H
