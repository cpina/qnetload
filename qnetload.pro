#-------------------------------------------------
#
# Project created by QtCreator 2017-04-21T21:52:54
#
#-------------------------------------------------

QT       += core gui core

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qnetload
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    networkinformationreader.cpp \
    informationstorage.cpp \
    formatnumber.cpp

HEADERS  += mainwindow.h \
    networkinformationreader.h \
    informationstorage.h \
    formatnumber.h

FORMS    += mainwindow.ui
