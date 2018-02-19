#-------------------------------------------------
#
# Project created by QtCreator 2018-02-05T11:39:17
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ServerPerformanceStatistics
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
    servers/threadedserver.cpp \
    servers/selectserver.cpp \
    ui/cmainwindow.cpp \
    ui/mainwindow.cpp \
    ui/tsmainwindow.cpp \
    client/client.cpp \
    servers/server.cpp

HEADERS += \
    servers/selectserver.h \
    servers/threadedserver.h \
    ui/cmainwindow.h \
    ui/mainwindow.h \
    ui/tsmainwindow.h \
    client/client.h \
    servers/server.h

FORMS += \
    ui/cmainwindow.ui \
    ui/mainwindow.ui \
    ui/tsmainwindow.ui
