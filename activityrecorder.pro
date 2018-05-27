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
    src/tracking/process.cpp \
    src/tracking/file.cpp \
    src/tracking/entry.cpp \
    src/ui/infowindow.cpp \
    src/tracking/filedata.cpp \
    src/tracking/processinfo.cpp \
    src/ui/statistics.cpp \
    src/ui/mainwindow.cpp


HEADERS += \
    src/tracking/process.h \
    src/tracking/file.h \
    src/tracking/filedata.h \
    src/tracking/entry.h \
    src/ui/infowindow.h \
    src/tracking/processinfo.h \
    src/ui/filedatainfowindow.h \
    src/ui/statistics.h \
    src/ui/mainwindow.h


FORMS += \
    src/ui/infowindow.ui \
    src/ui/mainwindow.ui

RESOURCES += \
    resources.qrc
