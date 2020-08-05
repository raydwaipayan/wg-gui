#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_listWidget_currentRowChanged(int currentRow);
    void newTunnel();
    void syncList();
    void processActive();
private:
    Ui::MainWindow *ui;
    QVector<QPair<QString,QString>> confs;
    QString current_interface, latest_handshake, transfer;
    QProcess *process;
};
#endif // MAINWINDOW_H
