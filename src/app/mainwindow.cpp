// Copyright 2020 Banbreach
// Written by Dwaipayan Ray
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <QMessageBox>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "configdialog.h"

// Initialize string constants
const QString wg::MainWindow::kPath              = "/etc/wireguard/";
const QString wg::MainWindow::kConnect           = "Connect";
const QString wg::MainWindow::kDisconnect        = "Disconnect";
const QString wg::MainWindow::kConnected         = "Connected";
const QString wg::MainWindow::kDisconnected      = "Disconnected";
const QString wg::MainWindow::kDns               = "DNS";
const QString wg::MainWindow::kListenPort        = "ListenPort";
const QString wg::MainWindow::kAddress           = "Address";
const QString wg::MainWindow::kPublicKey         = "PublicKey";
const QString wg::MainWindow::kAllowedIp         = "AllowedIPs";
const QString wg::MainWindow::kEndPoint          = "Endpoint";
const QString wg::MainWindow::kWgInterface       = "interface";
const QString wg::MainWindow::kWgLatestHandshake = "latest handshake";
const QString wg::MainWindow::kWgTransfer        = "transfer";
const QString wg::MainWindow::kWgPublicKey       = "public key";

wg::MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    ci_index(-1)
{
    ui->setupUi(this);
    QFile File(":ui/stylesheet.qss");

    if (!File.open(QFile::ReadOnly)) {
        throw std::runtime_error{ "could not open stylesheet" };
    }

    QString style_sheet = QLatin1String(File.readAll());

    this->setStyleSheet(style_sheet);

    // connect the Signals emitted on button clicks
    connect(this->ui->btn_new_tunnel, SIGNAL(released()), this,
            SLOT(newTunnel()));
    connect(this->ui->btn_edit, SIGNAL(released()), this, SLOT(editTunnel()));
}

wg::MainWindow::~MainWindow() {
    delete ui;
}

auto wg::MainWindow::create(QWidget *parent) -> QPointer<wg::MainWindow>

// Factory method to create the MainWindow object. Initializes the wireguard
// object and attaches it to MainWindow.
{
    QPointer<MainWindow> w = new MainWindow(parent);

    w->sync();
    w->wgcore = new WireGuard();

    // The wireguard instance contains a process which runs every second to
    // update the current information. So we connect the event Signal to the
    // Slot present in MainWindow object
    connect(w->wgcore,
            SIGNAL(readComplete(const QString&)),
            w,
            SLOT(processWgActive(const QString&)));
    return w;
}

void wg::MainWindow::syncData()

// Syncs the data variables with the files read from the system.
{
    confs = Config::getConfigs(kPath);

    // mapped_confs simply contains the mapping of data in confs for faster
    // access
    mapped_confs = Config::getMappedConfigs(confs);
}

void wg::MainWindow::syncList()

// Syncs the listWidget item with the current loaded configs. This is called
// whenever the application starts, a new config is created or an existing
// config is deleted
{
    ui->listWidget->clear();

    for (const auto& x:mapped_confs) {
        ui->listWidget->addItem(new QListWidgetItem(x.first));
    }

    // Always the topmost item in listWidget is selected to be the current
    // selected index to avoid discrepancy.
    if (mapped_confs.size()) ui->listWidget->setCurrentRow(0);
}

void wg::MainWindow::sync()
{
    syncData();
    syncList();
}

void wg::MainWindow::processWgActive(const QString& out)

// SLOT for handling the active wireguard instance data. This retrieves the data
// from the wireguard process and updates the ui component if it is the current
// active component
{
    QMap<QString, QString> dict = Config::parser(out, ':');

    current_interface = dict[kWgInterface];
    latest_handshake  = dict[kWgLatestHandshake];
    transfer          = dict[kWgTransfer];
    public_key        = dict[kWgPublicKey];

    ci_index = -1;

    for (int i = 0; i < mapped_confs.size(); i++) {
        if (mapped_confs[i].first == current_interface) {
            ci_index = i;
            break;
        }
    }
    updateWgActive();
}

void wg::MainWindow::newTunnel()

// Creates a new tunnel with the given configuration. Though, there seems no way
// to tell whether the config is valid or not at this point.
{
    wg::ConfigDialog cd(this);

    cd.show();

    if (!cd.exec()) return;

    QString name   = cd.getName();
    QString config = cd.getConfig();

    wg::Config::write(config, kPath + name + ".conf");
    sync();
}

void wg::MainWindow::editTunnel()

// Edit the selected wireguard tunnel configuration
{
    int current_row = ui->listWidget->currentRow();

    if (current_row == -1) {
        return;
    }

    // Create a config dialog to allow user to edit the config
    wg::ConfigDialog cd;

    cd.setName(mapped_confs[current_row].first);
    cd.disableName(true);
    cd.setConfig(confs[current_row].second);

    cd.show();

    if (!cd.exec()) {
        return;
    }
    const QString& config_cur = cd.getConfig();

    // Store the new config and its parsed version
    confs[current_row].second        = cd.getConfig();
    mapped_confs[current_row].second = Config::parser(confs[current_row].second,
                                                      '=');

    // Update the new config to the file
    Config::write(config_cur, kPath + confs[current_row].first + ".conf");

    // Update ui components with the edited config
    setUi(current_row);
}

void wg::MainWindow::updateWgActive()

// Updates the connection dependent ui components according to whether the
// current row is the running wireguard tunnel
{
    // If no row is selected, clear all connection data
    if (ui->listWidget->currentRow() == -1)
    {
        ui->peer_latest_handshake->clear();
        ui->peer_transfer->clear();
        ui->interface_public_key->clear();
        ui->interface_status->clear();
    }

    // Set the active connection data if selected row is the active wireguard
    // tunnel
    else if (ui->listWidget->currentRow() == ci_index) {
        ui->interface_status->setText(kConnected);
        ui->peer_latest_handshake->setText(latest_handshake);
        ui->peer_transfer->setText(transfer);
        ui->interface_public_key->setText(public_key);
        ui->btn_connection->setText(kDisconnect);
    }

    // Reset active connection data if selected row is not the active tunnel
    else {
        ui->btn_connection->setText(kConnect);
        ui->interface_status->setText(kDisconnected);
        ui->peer_latest_handshake->clear();
        ui->peer_transfer->clear();
        ui->interface_public_key->clear();
    }
}

void wg::MainWindow::clearUi()

// Clears the ui components
{
    ui->interface_dns_server->clear();
    ui->interface_listen_port->clear();
    ui->interface_listen_address->clear();
    ui->peer_public_key->clear();
    ui->peer_allowed_ip->clear();
    ui->peer_endpoint->clear();
    ui->interface_status->clear();
    ui->peer_latest_handshake->clear();
    ui->peer_transfer->clear();
    ui->interface_public_key->clear();
    ui->btn_connection->setText(kConnect);
    ui->btn_connection->setDisabled(true);
    ui->groupBox_interface->setTitle("Interface: none");
}

void wg::MainWindow::setUi(int current_row)

// Sets the ui components
{
    if (current_row == -1) {
        return;
    }
    QString interface            = mapped_confs[current_row].first;
    QMap<QString, QString>& dict = mapped_confs[current_row].second;

    ui->interface_dns_server->setText(dict[kDns]);
    ui->interface_listen_port->setText(dict[kListenPort]);
    ui->interface_listen_address->setText(dict[kAddress]);
    ui->peer_public_key->setText(dict[kPublicKey]);
    ui->peer_allowed_ip->setText(dict[kAllowedIp]);
    ui->peer_endpoint->setText(dict[kEndPoint]);
    ui->interface_status->setText(kDisconnected);
    ui->btn_connection->setDisabled(false);
    ui->groupBox_interface->setTitle("Interface: " + interface);

    // Set or clear the connection dependent ui components
    updateWgActive();
}

void wg::MainWindow::on_listWidget_currentRowChanged(int current_row)

// SLOT for change signals on list widget. updates the ui compoents with the
// selected config
{
    // If the current row is -1, clear all the ui component texts and return
    if (current_row == -1) {
        clearUi();
        return;
    }

    // Set ui components and update wg status with selected row if active
    setUi(current_row);
}

void wg::MainWindow::on_btn_connection_released()

// Handles connection button operations.
{
    int current_row = ui->listWidget->currentRow();

    if (current_row == -1) {
        return;
    }

    if (ui->btn_connection->text() == kConnect)
    {
        int status = wgcore->connectTunnel(confs[current_row].first);

        if (status)
        {
            // Displays a success message on successful connection
            QMessageBox msgbox(this);
            msgbox.setText("Connection Success!");
            msgbox.exec();
        }
        else {
            // Displays error message
            QMessageBox msgbox(this);
            msgbox.setText(
                "Error! Connection Failed! Check logs for further details");
            msgbox.exec();
        }
    }
    else {
        int status = wgcore->disconnectTunnel(confs[current_row].first);

        if (status)
        {
            QMessageBox msgbox(this);
            msgbox.setText("Disconnected successfully!");
            msgbox.exec();
        }
        else {
            QMessageBox msgbox(this);
            msgbox.setText(
                "Error! Connection Failed! Check logs for further details");
            msgbox.exec();
        }
    }
}
