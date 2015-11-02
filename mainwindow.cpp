#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSql>
#include <QtDebug>
#include <QDataWidgetMapper>
#include "lds_model_sqlrelationaltablemodel.h"
#include <QPainter>
#include "nightcharts.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{

}
