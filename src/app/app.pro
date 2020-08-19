TEMPLATE = app
TARGET = wg-gui

CONFIG += qt
QT += core gui
DESTDIR = ../..
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += main.cpp \
		mainwindow.cpp \
		configdialog.cpp \
		config.cpp

HEADERS += mainwindow.h \
		configdialog.h \
		config.h

FORMS += ../ui/mainwindow.ui \
		../ui/configdialog.ui
		
RESOURCES += resources.qrc

LIBS += -L../wireguard -lwireguard
include(../wireguard/wireguard.pri)

unix:!macx {
    target.files += $$DESTDIR/$$TARGET
    target.path = /usr/local/bin
    INSTALLS += target
}
