#ifndef LDS_COMBOBOX_H
#define LDS_COMBOBOX_H

#include <QComboBox>
#include <QPixmap>




//combobox 的下拉高度，比寻常的高一些





class lds_ComboBox : public QComboBox
{
    Q_OBJECT
    Q_PROPERTY(QString comusrdata READ comusrdata WRITE setcomusrdata USER true)
public:
    struct PAIRLIST{
        inline PAIRLIST(const QString &_k, const QVariant &_u):
        key(_k),
        usrvalue(_u){}
        QString key;
        QVariant usrvalue;
    };

public:
    explicit lds_ComboBox(QWidget *parent = 0);
    void addItems(const QStringList &texts);
    void addItems(const  QList<PAIRLIST>&texts);
    void setindex_c1(const QString c1_text);//xx(c1_text)
    void setcomusrdata(const QString usrtext);
    /*
    2   xx
    5   yy

    setcolumn2index1(2)->xx;
    column2index1()->2
    */
//    void setcolumn2index1(const QString&index);//model有两列，会更具第一列的值来更新索引
    QString column2index1();//model有两列，会返回第一列的值
    QString comusrdata();
private slots:
    void to_currentchange(int index);
signals:
//    void currentIndexChanged0(const QString text);//发送第零列的
    void currentIndexChanged1(const QString text);//发送第一列的
    void currentIndexChanged2(const QString text);
    void currentIndexChanged3(const QString text);
    void currentIndexChanged4(const QString text);
};

#endif // LDS_COMBOBOX_H
