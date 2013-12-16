#-------------------------------------------------
#
# Project created by QtCreator 2013-01-29T15:04:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ImageResizingApplication
TEMPLATE = app


SOURCES += main.cpp\
        imageresizer.cpp \
    rescalethread.cpp \
    aboutdialog.cpp

HEADERS  += imageresizer.h \
    rescalethread.h \
    aboutdialog.h


FORMS    += imageresizer.ui \
    aboutdialog.ui

RESOURCES += \
    files.qrc
