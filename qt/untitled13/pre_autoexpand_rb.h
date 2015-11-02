#ifndef PRE_AUTOEXPAND_RB_H
#define PRE_AUTOEXPAND_RB_H

#include <QRadioButton>
#include <QLabel>

class pre_autoexpand_rb : public QRadioButton
{
    Q_OBJECT
public:
    explicit pre_autoexpand_rb(bool isshowpixmap, QWidget *parent = 0);

    QString index;
    void setText(const QString &text);
    QString text() const;
    QSize sizeHint() const;
    static QSize iconsize;
    static int border;
    static QSize iconsize0;
    static int border0;
    static int textheight;
    static void setshowpixmapEnabel(bool enabled);
protected:
    void paintEvent(QPaintEvent *e);
private:
    QString _text;
};

class pre_autoexpand_label : public QLabel
{
    Q_OBJECT
public:
    explicit pre_autoexpand_label(QWidget *parent = 0);
    QSize sizeHint() const;
};
#endif // PRE_AUTOEXPAND_RB_H
