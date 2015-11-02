#ifndef PRINT_THREQD_H
#define PRINT_THREQD_H

#include <QObject>
#include <QThread>

class print_threqd : public QThread
{
    Q_OBJECT
public:
    explicit print_threqd(QObject *parent = 0);
    void run();
    void re_model();
signals:
    void print_lost_thread();
public slots:
    void show_print_close();
};

#endif // PRINT_THREQD_H
