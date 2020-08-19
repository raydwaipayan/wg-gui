#include "mainwindow.h"

#include <QApplication>
#include <unistd.h>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if (getuid())
    {
        QMessageBox msgBox;
        msgBox.setText("The program must be run as root!");
        msgBox.exec();
        exit(EXIT_FAILURE);
    }
    MainWindow w;

    w.show();
    return a.exec();
}
