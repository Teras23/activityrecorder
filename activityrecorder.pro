#-------------------------------------------------
#
# Project created by QtCreator 2017-08-15T20:11:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = activityrecorder
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

msvc: LIBS += -luser32 -lkernel32 -lpsapi

SOURCES +=  \
    src/main.cpp \
    src/mainwindow.cpp \
    src/process.cpp \
    src/file.cpp \
    src/entry.cpp \
    src/infowindow.cpp \
    src/filedata.cpp \
    src/processinfo.cpp


HEADERS += \
    src/mainwindow.h \
    src/process.h \
    src/file.h \
    src/filedata.h \
    src/entry.h \
    src/infowindow.h \
    src/processinfo.h \
    src/filedatainfowindow.h


FORMS += \
    src/mainwindow.ui \
    src/infowindow.ui

RESOURCES += \
    resources.qrc
