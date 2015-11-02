#ifndef LDS_ROUNDEDWIDGET_H
#define LDS_ROUNDEDWIDGET_H

#include <QWidget>

class lds_roundedwidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor bgcolor READ getBgColor WRITE setBgColor DESIGNABLE true)

public:
    explicit lds_roundedwidget(QWidget *parent = 0);

signals:

public slots:

protected:
    void setBgColor(const QColor &color);
    QColor getBgColor() const;
    void paintEvent(QPaintEvent *e);
    QColor bgcolor;
};

#endif // LDS_ROUNDEDWIDGET_H
