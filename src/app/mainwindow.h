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


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QPointer>

#include "wireguard.h"
#include "config.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

namespace wg {
class MainWindow : public QMainWindow {
    Q_OBJECT
    MainWindow(QWidget *parent = nullptr);

public:

    ~MainWindow();
    static QPointer<MainWindow>create(QWidget *parent = nullptr);
    void                       setUi(int);
    void                       clearUi();

private slots:

    void on_listWidget_currentRowChanged(int);
    void newTunnel();
    void editTunnel();

    void sync();
    void syncData();
    void syncList();
    void processWgActive(const QString&);
    void updateWgActive();


    void on_btn_connection_released();

private:

    // shared constant strings
    const static QString kPath, kConnect, kDisconnect, kConnected, kDisconnected,
                         kDns, kListenPort, kAddress, kPublicKey, kAllowedIp,
                         kEndPoint;

    // constant strings or use in parsing wg process output
    const static QString kWgInterface, kWgLatestHandshake, kWgTransfer,
                         kWgPublicKey;

    // Contains the Ui file
    Ui::MainWindow *ui;

    QPointer<WireGuard>wgcore;
    QPointer<QProcess>process;

    // mapped_confs keeps the configs parsed for faster access
    QVector<QPair<QString, QMap<QString, QString> > >mapped_confs;
    QVector<QPair<QString, QString> >confs;
    QString current_interface, latest_handshake, transfer, public_key;

    int ci_index;
};
}
#endif // MAINWINDOW_H
