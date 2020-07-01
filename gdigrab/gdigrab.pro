#-------------------------------------------------
#
# Project created by QtCreator 2020-06-07T13:05:50
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gdigrab
TEMPLATE = app

INCLUDEPATH += ../../ffmpeg/include/
INCLUDEPATH += $$PWD/src/

LIBS += -L../../ffmpeg/lib -lavutil-56 -lavformat-58 -lavcodec-58 -lavdevice-58 -lavfilter-7 -lpostproc-55 -lswresample-3 -lswscale-5

SOURCES += src/main.cpp\
        src/mainwindow.cpp \
    src/gdigrab.cpp \
    src/aencode.cpp

HEADERS  += src/mainwindow.h \
    src/gdigrab.h \
    src/aencode.h

FORMS    += src/mainwindow.ui
