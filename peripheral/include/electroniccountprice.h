#ifndef ELECTRONICCOUNTPRICE_H
#define ELECTRONICCOUNTPRICE_H
#include <QObject>
class ElectronicCountPrice_kaifeng;

#ifdef ELECTRONICCOUNTPRICE
#define ELECTRONICCOUNTPRICE_COMMON_DLLSPEC  Q_DECL_EXPORT
#else
#define ELECTRONICCOUNTPRICE_COMMON_DLLSPEC  Q_DECL_IMPORT
#endif
/**
1.建议在头文件里定义指正,方便调用;
2.    if(countprice_scale == 0) {
        countprice_scale=new   ElectronicCountPrice(0, "", "");
        //lineEdit_39会接受回读信息,
        connect(countprice_scale,SIGNAL(weighComming(QString)),ui->lineEdit_39,SLOT(setText(QString)));
    }
    countprice_scale->stop();//停止
    countprice_scale->setType(ui->comboBox_47->currentText());
    countprice_scale->setBaudrate(ui->comboBox_49->currentText());
    if(!countprice_scale->tryStart_qs(ui->comboBox_46->currentText())){//开始
        lds_messagebox::warning(this, tr("消息提示"),countprice_scale->lastError());//错误信息
    }
3.
*/
class ELECTRONICCOUNTPRICE_COMMON_DLLSPEC ElectronicCountPrice : public QObject
{
    Q_OBJECT
private:
    QString _type;
    QString _baudrate;
    ElectronicCountPrice_kaifeng *kaifeng;
    void setSendData(const QByteArray &data);
    QString jieshi(uchar a);
public:
    explicit ElectronicCountPrice(QObject *parent, const QString &type
                                  , const QString &baudrate);
    ~ElectronicCountPrice();

    bool isOpen();
    bool tryStart_qs(const QString &path, long timeout = 10);
    bool tryStart_s(const QString &path, long timeout = 10);
    void stop();
    QString lastError();

    inline QString type(){return _type;}
    void setType(const QString &type);
    inline QString baudrate(){return _baudrate;}
    void setBaudrate(const QString &baudrate);
public slots:
    void qupi();//发送去皮指令
    void zhiling();//发送置零指令
signals:
    void qupiComming(bool);//回读去皮信息
    void lingweiComming(bool);//回读指令信息
    void weighComming(const QString &weigh);//回读取重信息
private slots:
    void toreceive(const QByteArray & data);
};

#endif // ELECTRONICCOUNTPRICE_H
