QT -= gui

TEMPLATE = lib
CONFIG += staticlib
TARGET = wireguard

SOURCES += wireguard.cpp
HEADERS += wireguard.h

include(./wireguard.pri)


