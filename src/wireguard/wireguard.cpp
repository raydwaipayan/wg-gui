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


#include "wireguard.h"
#include <QDebug>

wg::WireGuard::WireGuard()
{
    // The process is run every second to fetch the current wireguard active
    // configuration.
    process = new QProcess();
    timer   = new QTimer();

    connect(process, SIGNAL(finished(int,QProcess::ExitStatus)), this,
            SLOT(emitReadComplete(int,QProcess::ExitStatus)));
    connect(timer, SIGNAL(timeout()), this,
            SLOT(processWgActive()));

    processWgActive();
    timer->start(1000);
}

void wg::WireGuard::emitReadComplete(int, QProcess::ExitStatus)

// Once process is finished, emit a signal containing the stdout which shall be
// catched by the slot in the
// mainwindow component
{
    QString out = process->readAllStandardOutput();

    emit this->readComplete(out);
}

void wg::WireGuard::processWgActive()

// Starts the wg process
{
    process->start("wg", QStringList() << "show");
}

bool wg::WireGuard::connectTunnel(const QString& tunnel_name)

// Connect to the given wireguard tunnel. Creates a qprocess and calls wg-quick
// to bring up the tunnel
{
    QProcess p;

    p.start("wg-quick", QStringList() << "up" << tunnel_name);
    p.waitForFinished();

    if (p.exitCode()) {
        return false;
    }
    return true;
}

bool wg::WireGuard::disconnectTunnel(const QString& tunnel_name)

// Disonnect the given wireguard tunnel. Creates a qprocess and calls wg-quick
// to bring down the tunnel
{
    QProcess p;

    p.start("wg-quick", QStringList() << "down" << tunnel_name);
    p.waitForFinished();

    if (p.exitCode()) {
        return false;
    }
    return true;
}
