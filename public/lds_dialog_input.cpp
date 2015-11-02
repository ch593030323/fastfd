#include "lds_dialog_input.h"

lds_dialog_input::lds_dialog_input(QWidget *parent) :
    QDialog(parent),
    ui(new Ui_lds_dialog_input_Dialog)
{
    ui->setupUi(this);

    ui->widget->setup(QStringList() << tr("确定") << tr("取消"), Qt::AlignRight);
    connect(ui->widget->index_widget(tr("确定")), SIGNAL(clicked()),this,SLOT(took()));
    connect(ui->widget->index_widget(tr("取消")), SIGNAL(clicked()),this,SLOT(toexit()));

    connect(ui->pushButton_num0,SIGNAL(clicked()),this,SLOT(set_num()));
    connect(ui->pushButton_num1,SIGNAL(clicked()),this,SLOT(set_num()));;
    connect(ui->pushButton_num2,SIGNAL(clicked()),this,SLOT(set_num()));
    connect(ui->pushButton_num3,SIGNAL(clicked()),this,SLOT(set_num()));
    connect(ui->pushButton_num4,SIGNAL(clicked()),this,SLOT(set_num()));
    connect(ui->pushButton_num5,SIGNAL(clicked()),this,SLOT(set_num()));
    connect(ui->pushButton_num6,SIGNAL(clicked()),this,SLOT(set_num()));
    connect(ui->pushButton_num7,SIGNAL(clicked()),this,SLOT(set_num()));
    connect(ui->pushButton_num8,SIGNAL(clicked()),this,SLOT(set_num()));
    connect(ui->pushButton_num9,SIGNAL(clicked()),this,SLOT(set_num()));
    connect(ui->pushButton_dot,SIGNAL(clicked()),this,SLOT(set_num()));
    connect(ui->pushButton_clear,SIGNAL(clicked()),this,SLOT(set_num()));
    connect(ui->toolButton,SIGNAL(clicked()),this,SLOT(update_state()));
    set_key_hidden(false);
    set_second_hidden(true);
}

void lds_dialog_input::set_key_hidden(bool flag)
{
    flag=!flag;
    ui->frame->setVisible(flag);

    if(flag){
        setFixedHeight(280+(ui->label_2->isHidden()?0:40));
        qDebug() << this->height()<<ui->label_2->isVisible();
        /*        NoArrow,
        UpArrow,
        DownArrow,
        LeftArrow,
        RightArrow*/
        ui->toolButton->setArrowType(Qt::UpArrow);
    } else {
        ui->toolButton->setArrowType(Qt::DownArrow);
        setFixedHeight(280+(ui->label_2->isHidden()?0:40)-180);

    }
}

void lds_dialog_input::set_second_hidden(bool flag)
{
    flag=!flag;
    ui->label_2->setVisible(flag);
    ui->lineEdit_2->setVisible(flag);
    qDebug() << this->height()<<ui->label_2->isVisible() << flag;
}

void lds_dialog_input::set_pop_enable(bool flag)
{
    ui->toolButton->setEnabled(flag);
    ui->toolButton->setVisible(flag);
}

void lds_dialog_input::took()
{
    _text=ui->lineEdit->text();
    this->accept();
}

void lds_dialog_input::toexit()
{
    this->reject();
}

void lds_dialog_input::update_state()
{
    if(ui->toolButton->arrowType()==Qt::UpArrow){
        set_key_hidden(true);
    } else if(ui->toolButton->arrowType()==Qt::DownArrow){
        set_key_hidden(false);
    }
}

void lds_dialog_input::set_num()
{
    QPushButton *btn=qobject_cast<QPushButton *>(sender());
    if(!btn)return;

    QLineEdit *line=qobject_cast<QLineEdit *>(this->focusWidget());
    if(!line)return;

    if(btn==ui->pushButton_clear){
        line->clear();
    } else {
        line->insert(btn->text());
    }
}
