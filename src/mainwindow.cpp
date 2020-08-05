#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "configdialog.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->btnNewTunnel, SIGNAL(released()), this, SLOT(newTunnel()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::newTunnel()
{
    ConfigDialog cd;
    cd.show();
    if(!cd.exec()) return;

    QString name=cd.getName();
    QString config=cd.getConfig();
    qDebug()<<name;
    qDebug()<<config;
}
void MainWindow::on_listWidget_currentRowChanged(int currentRow)
{

}
