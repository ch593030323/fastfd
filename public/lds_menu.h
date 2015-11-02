#ifndef LDS_MENU_H
#define LDS_MENU_H

#include <QMenu>
#include <QWidgetAction>

class lds_menu : public QMenu
{
    Q_OBJECT
public:
    explicit lds_menu(QWidget *parent = 0);
    ~lds_menu();

    void popup(const QPoint &pos, QAction *at=0);
    QAction *exec(const QPoint &pos, QAction *at=0);
protected:
    void changeEvent(QEvent *e);

    void mouseReleaseEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
};

#endif // LDS_MENU_H
