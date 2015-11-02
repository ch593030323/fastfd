#ifndef NVO_ICCARD_H
#define NVO_ICCARD_H

#include <QObject>

class nvo_iccard : public QObject
{
    Q_OBJECT
public:
    explicit nvo_iccard(QObject *parent = 0);

    bool of_clear_card();
    QString of_get_card();
    bool of_init();
    bool of_send_card();
    QString of_set_cost();
    bool of_set_data();
    QString of_set_deposit(int adec_deposit);
    bool of_set_deposit(const QString &as_member, int adec_deposit);
    
};

#endif // NVO_ICCARD_H
