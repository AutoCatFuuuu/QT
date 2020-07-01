#-------------------------------------------------
#
# Project created by QtCreator 2020-05-23T17:14:38
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ffmpeg
TEMPLATE = app

INCLUDEPATH += $$PWD/include/
INCLUDEPATH += $$PWD/src/

LIBS += -L$$PWD/lib -lavutil-56 -lavformat-58 -lavcodec-58 -lavdevice-58 -lavfilter-7 -lpostproc-55 -lswresample-3 -lswscale-5

SOURCES += src/main.cpp\
    src/affmpeg.cpp \
    src/testwidget.cpp \
    src/mainwindow.cpp \
    src/afilter.cpp \
    src/switchbtn.cpp \
    src/danmuku.cpp

HEADERS  += src/affmpeg.h \
    src/testwidget.h \
    src/mainwindow.h \
    src/afilter.h \
    src/switchbtn.h \
    src/danmuku.h


FORMS += src/testwidget.ui \
    src/mainwindow.ui \
    src/switchbtn.ui

RESOURCES += \
    image/image.qrc
