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


#ifndef WIREGUARD_H
#define WIREGUARD_H

#include <QProcess>
#include <QPointer>
#include <QTimer>

namespace wg {
class WireGuard : public QObject {
    Q_OBJECT

private:

    QPointer<QProcess>process;
    QPointer<QTimer>timer;

public:

    WireGuard();
    ~WireGuard() {}

    QString getProcessOutput();
    bool    connectTunnel(const QString&);
    bool    disconnectTunnel(const QString&);

private slots:

    void emitReadComplete(int,
                          QProcess::ExitStatus);
    void processWgActive();

signals:

    void readComplete(const QString&);
};
}
#endif // WIREGUARD_H
