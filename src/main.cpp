#include <iostream>

#include <QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

#include "ui/mainwindow.h"

int main(int argc, char **argv) {
    QApplication::setApplicationName("activityrecorder");

    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    return app.exec();
}