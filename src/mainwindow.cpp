#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "configdialog.h"
#include "config.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QString path="/etc/wireguard/";
    confs = Config::getConfigs(path);
    syncList();
    connect(ui->btnNewTunnel, SIGNAL(released()), this, SLOT(newTunnel()));

    process=new QProcess();
    connect(process,SIGNAL(readyReadStandardOutput()),this, SLOT(processActive()));
    process->start("wg",QStringList()<<"show");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::processActive()
{
    QString out=process->readAllStandardOutput();
    QMap<QString,QString> dict=Config::parser(out,':');
    current_interface=dict["interface"];
    latest_handshake=dict["latest handshake"];
    transfer=dict["transfer"];

}
void MainWindow::newTunnel()
{
    ConfigDialog cd;
    cd.show();
    if(!cd.exec()) return;

    QString name=cd.getName();
    QString config=cd.getConfig();
}
void MainWindow::syncList()
{
    ui->listWidget->clear();
    for(const auto &x:confs)
    {
        QListWidgetItem *temp = new QListWidgetItem();
        temp->setText(x.first.split('.')[0]);
        ui->listWidget->addItem(temp);
    }
    if(confs.size()) ui->listWidget->setCurrentRow(0);
}

void MainWindow::on_listWidget_currentRowChanged(int currentRow)
{
    qDebug()<<currentRow;
}
