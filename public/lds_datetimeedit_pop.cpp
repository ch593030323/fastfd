#include "lds_datetimeedit_pop.h"
#include "ui_lds_datetimeedit_pop.h"
#include <QtDebug>

lds_datetimeEdit_pop::lds_datetimeEdit_pop(QDateTimeEdit *parent) :
    lds_roundedwidget(parent),
    ui(new Ui::lds_datetimeEdit_pop),
    _parent(parent)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::Popup);

    ui->calendarWidget->setLocale(QLocale(QLocale::Chinese, QLocale::China));
    ui->calendarWidget->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    ui->calendarWidget->setNavigationBarVisible(false);

    QStringList yearlist;
    for(int y=1900;y<2051;y++)yearlist<<QString::number(y);
    ui->comboBox_year->addItems(yearlist);
    ui->comboBox_year->setMaxVisibleItems(6);


    QStringList monthlist;
    for(int m=1;m<=12;m++)monthlist<<QString(QDate(2000,m,1).toString("M")+tr("月"));
    ui->comboBox_month->addItems(monthlist);
    ui->comboBox_month->setMaxVisibleItems(6);

    connect(ui->timeEdit,SIGNAL(timechanged(QTime)),this,SLOT(setTime(QTime)));

    QString df=_parent->displayFormat();
    if(df.contains("y")
            ||df.contains("M")
            ||df.contains("d")){
    } else {
        ui->comboBox_month->hide();
        ui->comboBox_year->hide();
        ui->dateEdit->hide();
        ui->calendarWidget->hide();
    }

    ui->timeEdit->setShowVisiable(df);
    this->resize(0,0);
}

lds_datetimeEdit_pop::~lds_datetimeEdit_pop()
{
    delete ui;
}

void lds_datetimeEdit_pop::setDateTime(const QDateTime &dt)
{
    setDate(dt.date());
    setTime(dt.time());
}

void lds_datetimeEdit_pop::setDate(const QDate &dt)
{
    QDate dt0;
    ui->calendarWidget->setSelectedDate(dt);
    dt0=ui->calendarWidget->selectedDate();
    ui->dateEdit->setDate(dt0);
    ui->comboBox_year->setCurrentIndex(ui->comboBox_year->findText(QString::number(dt0.year())));
    ui->comboBox_month->setCurrentIndex(ui->comboBox_month->findText(dt0.toString("M")+tr("月")));
    _dt.setDate(dt0);
}

void lds_datetimeEdit_pop::setTime(const QTime &time)
{
    ui->timeEdit->setTime(time);
    _dt.setTime(time);
}

QDateTime lds_datetimeEdit_pop::getDateTime()
{
    return _dt;
}

void lds_datetimeEdit_pop::on_pushButton_backtoday_clicked()
{
    setDateTime(QDateTime::currentDateTime());
}

void lds_datetimeEdit_pop::on_calendarWidget_clicked(const QDate &date)
{
    setDate(date);
}

void lds_datetimeEdit_pop::on_comboBox_month_currentIndexChanged(const QString &arg1)
{
    QDate d =_dt.date();
    d = (QDate::fromString(QString("%1-%2-%3").arg(d.year()).arg(arg1.left(arg1.length()-1/*剔除最后一个月*/)).arg(d.day()), "yyyy-MM-d"));
    setDate(d);
}

void lds_datetimeEdit_pop::on_comboBox_year_currentIndexChanged(const QString &arg1)
{
    QDate d=_dt.date();
    d = (QDate::fromString(QString("%1-%2-%3").arg(arg1).arg(d.month()).arg(d.day()), "yyyy-M-d"));
    setDate(d);
}

void lds_datetimeEdit_pop::on_dateEdit_dateChanged(const QDate &date)
{
    setDate(date);
}


void lds_datetimeEdit_pop::on_pushButton_clicked()
{
    _parent->setDateTime(_dt);
    this->close();
}
