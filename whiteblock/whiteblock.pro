#-------------------------------------------------
#
# Project created by QtCreator 2020-06-25T21:08:32
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = whiteblock
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    blockdata.cpp \
    testwidget.cpp

HEADERS  += mainwindow.h \
    blockdata.h \
    testwidget.h

FORMS    += mainwindow.ui \
    testwidget.ui
