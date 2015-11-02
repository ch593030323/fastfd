#ifndef UINORMAL_COMMBOX_H
#define UINORMAL_COMMBOX_H

#include <QComboBox>

class uinormal_commbox : public QComboBox
{
    Q_OBJECT
public:
    explicit uinormal_commbox(QWidget *parent = 0);
    void addItems(const QStringList &texts);


};

#endif // UINORMAL_COMMBOX_H
