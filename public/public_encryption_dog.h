#ifndef PUBLIC_ENCRYPTION_DOG_H
#define PUBLIC_ENCRYPTION_DOG_H

#include <QWidget>
//加密狗
#ifdef Q_OS_WIN
#include<QLibrary>
//typedef long ( *NoxGetLastError)();
//typedef long ( *NoxClose)(long keyHandle);
//typedef long ( *NoxFind)(long ,long * , long *); //定义函数指针
//typedef long ( *NoxOpen)(long keyHandle, char * userPin);
//typedef long ( *NoxReadStorage)(long keyHandle, unsigned char * pBuffer);

typedef long (__stdcall *NoxGetLastError)();
typedef long (__stdcall *NoxClose)(long keyHandle);
typedef long (__stdcall *NoxFind)(long ,long * , long *); //定义函数指针
typedef long (__stdcall *NoxOpen)(long keyHandle, char * userPin);
typedef long (__stdcall *NoxReadStorage)(long keyHandle,   unsigned char * pBuffer);
#endif

class public_encryption_dog : public QWidget
{
    Q_OBJECT
public:
    explicit public_encryption_dog(QWidget *parent = 0);
    bool conductlock();
    QString errorString();
private:
    QString errsrring;
signals:

public slots:
};

#endif // PUBLIC_ENCRYPTION_DOG_H
