#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "configdialog.h"
#include "config.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    ci_index(-1)
{
    ui->setupUi(this);
    const QString path = "/etc/wireguard/";
    QFile File(":/src/ui/stylesheet.qss");

    File.open(QFile::ReadOnly);
    QString StyleSheet = QLatin1String(File.readAll());

    this->setStyleSheet(StyleSheet);


    confs = Config::getConfigs(path);
    syncList();
    connect(ui->btnNewTunnel, SIGNAL(released()), this,
            SLOT(newTunnel()));

    wg = new WireGuard();
    connect(wg, SIGNAL(readComplete()), this, SLOT(processWgActive()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::processWgActive()
{
    QString out = wg->getProcessOutput();

    QMap<QString, QString> dict = Config::parser(out, ':');

    current_interface = dict["interface"];
    latest_handshake  = dict["latest handshake"];
    transfer          = dict["transfer"];
    public_key        = dict["public key"];

    for (int i = 0; i < confs.size(); i++) {
        if (confs[i].first.split('.')[0] == current_interface) {
            ci_index = i;
            break;
        }
    }
    updateActive();
}

void MainWindow::newTunnel()
{
    ConfigDialog cd;

    cd.show();

    if (!cd.exec()) return;

    QString name   = cd.getName();
    QString config = cd.getConfig();
}

void MainWindow::syncList()
{
    ui->listWidget->clear();

    for (const auto& x:confs) {
        QListWidgetItem *temp = new QListWidgetItem();
        temp->setText(x.first.split('.')[0]);
        ui->listWidget->addItem(temp);
    }

    if (confs.size()) ui->listWidget->setCurrentRow(0);
}

void MainWindow::updateActive()
{
    if (ui->listWidget->currentRow() == ci_index) {
        ui->interface_status->setText("Connected");
        ui->peer_latest_handshake->setText(latest_handshake);
        ui->peer_transfer->setText(transfer);
        ui->interface_public_key->setText(public_key);
        ui->btnConnection->setText("Disconnect");
    }
}

void MainWindow::on_listWidget_currentRowChanged(int currentRow)
{
    if (currentRow == -1) {
        return;
    }
    QString interface            = confs[currentRow].first.split('.')[0];
    QMap<QString, QString>& dict = confs[currentRow].second;

    ui->interface_dns_server->setText(dict["DNS"]);
    ui->interface_listen_port->setText(dict["ListenPort"]);
    ui->interface_listen_address->setText(dict["Address"]);
    ui->peer_public_key->setText(dict["PublicKey"]);
    ui->peer_allowed_ip->setText(dict["AllowedIPs"]);
    ui->peer_endpoint->setText(dict["Endpoint"]);
    ui->interface_status->setText("Disconnected");
    ui->peer_latest_handshake->clear();
    ui->peer_transfer->clear();
    ui->interface_public_key->clear();
    ui->btnConnection->setText("Connect");
    ui->btnEdit->setText("Edit");
    ui->groupBox_interface->setTitle("Interface: " + interface);
    updateActive();
}
