#ifndef PRINT_PROMPT_H
#define PRINT_PROMPT_H

#include <QDialog>

namespace Ui {
class print_prompt;
}

class print_prompt : public QDialog
{
    Q_OBJECT
    
public:
    explicit print_prompt(QWidget *parent = 0);
    ~print_prompt();
    
private:
    Ui::print_prompt *ui;
};

#endif // PRINT_PROMPT_H
