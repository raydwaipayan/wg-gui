#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include "wireguard.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_listWidget_currentRowChanged(int currentRow);
    void newTunnel();
    void syncList();
    void processWgActive();
    void updateActive();

private:

    Ui::MainWindow *ui;
    WireGuard *wg;
    QVector<QPair<QString, QMap<QString, QString> > >confs;
    QString current_interface, latest_handshake, transfer, public_key;
    int ci_index;
    QProcess *process;
    const static QString path;
};
#endif // MAINWINDOW_H
