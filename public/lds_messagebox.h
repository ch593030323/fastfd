#ifndef LDS_MESSAGEBOX_H
#define LDS_MESSAGEBOX_H

#include <QObject>
#include <QMessageBox>
#include <QDialogButtonBox>
#include<QPushButton>

class lds_msgbox:public QMessageBox{
    Q_OBJECT
public:
    lds_msgbox(Icon icon, const QString &title, const QString &text,
                StandardButtons buttons = NoButton, QWidget *parent = 0,
                Qt::WindowFlags flags = Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint):QMessageBox(icon, title, text,
                                                                                                   buttons, parent,
                                                                                                   flags){}
    QPushButton *btnok;
public slots:
    void updateText(int index){btnok->setText(btnok->text().split(":").value(0)+":"+QString::number(index));if(index==0) emit btnok_finished(0);}
signals:
    void btnok_finished(int index);
};

class lds_messagebox : public QObject
{
    Q_OBJECT
public:
    explicit lds_messagebox(QObject *parent = 0);

    static int warning(QWidget *parent, const QString &title,
                       const QString& text,
                       const QString& button0Text,
                       const QString& button1Text = QString(),
                       const QString& button2Text = QString()
            );
    static int warning(QWidget *parent, const QString &title,
                       const QString& text,
                       bool delayclose=false);
private:
    static int showOldMessageBox(QWidget *parent, QMessageBox::Icon icon,
                                 const QString &title, const QString &text,
                                 const QString &button0Text,
                                 const QString &button1Text,
                                 const QString &button2Text,
                                 bool delayclose=false);
};

#endif // LDS_MESSAGEBOX_H
