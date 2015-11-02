#ifndef LDS_LABEL_PIXMAP_TEXT_H
#define LDS_LABEL_PIXMAP_TEXT_H

#include <QLabel>

class lds_label_pixmap_text : public QLabel
{
    Q_OBJECT
    Q_PROPERTY(QString label_text READ label_text WRITE setLabel_text)

public:
    explicit lds_label_pixmap_text(QWidget *parent = 0);

    QString label_text();
    void setLabel_text(const QString &text);
private:
    QLabel *_label_text;
};

#endif // LDS_LABEL_PIXMAP_TEXT_H
