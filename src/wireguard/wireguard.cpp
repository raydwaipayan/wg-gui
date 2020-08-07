#include "wireguard.h"

WireGuard::WireGuard()
{
    process = new QProcess();
    timer   = new QTimer();
    connect(process, SIGNAL(readyReadStandardOutput()), this,
            SLOT(emitReadComplete()));
    connect(timer, SIGNAL(timeout()), this, SLOT(processWgActive()));

    processWgActive();
    timer->start(1000);
}

void WireGuard::emitReadComplete()
{
    emit this->readComplete();
}

void WireGuard::processWgActive()
{
    process->start("wg", QStringList() << "show");
}

WireGuard::~WireGuard()
{
    delete process;
    delete timer;
}

QString WireGuard::getProcessOutput()
{
    QString out = process->readAllStandardOutput();

    return out;
}
