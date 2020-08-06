#ifndef WIREGUARD_H
#define WIREGUARD_H

#include <QProcess>
#include <QTimer>

class WireGuard : public QObject {
    Q_OBJECT

private:

    QProcess *process;
    QTimer *timer;

public:

    WireGuard();
    ~WireGuard();
    QString getProcessOutput();

private slots:

    void emitReadComplete();
    void processWgActive();

signals:

    void readComplete();
};

#endif // WIREGUARD_H
