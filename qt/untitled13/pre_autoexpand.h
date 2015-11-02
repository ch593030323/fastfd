#ifndef PRE_AUTOEXPAND_H
#define PRE_AUTOEXPAND_H
#include <QFrame>
#include "pre_autoexpand_help.h"
#include <QPushButton>

class pre_autoexpand:public QFrame
{
        Q_OBJECT
public:
    pre_autoexpand(QWidget *parent=0);
    void setsql(const QString &value);//select index, text from S
    void setpixmappath(const QString &value);

    void sethcount(int value);
    int gethcount();

    void setvcount(int value);
    int getvcount();

    int getspace();

    void setshowpixmapEnable(bool enabled);
    bool isshowpixmapEnable();

    void seticonsize(const QSize &size);
    QSize geticonsize();
    void start();
    void item_event_click();
private slots:
    void tonext();
    void toprevious();
    void toclicktext();
private:
    void toconduct(const pre_autoexpand_help::PVN &pagepvn);
signals:
    void signal_text(const QString &value);
private:
    pre_autoexpand_help *help;
    QFutureWatcher<QImage> *imagewatcher;
    pre_autoexpand_rb *saverb;
    bool isshowpixmap;
    QPushButton *btn_up;
    QPushButton *btn_down;
private slots:
    void finished();
    void updateImage(int step);
};

#endif // PRE_AUTOEXPAND_H
