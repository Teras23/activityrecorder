#include "file.h"

#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include <QDataStream>
#include <QDebug>
#include <iostream>
#include <map>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonDocument>

FileData* File::fileData = nullptr;

void File::write(FileData* fileData)
{
	QString dataLocation = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

	QFile data(dataLocation + getFileName());

	if (!QDir(dataLocation).exists()) {
		QDir().mkpath(dataLocation);
		std::clog << "Created directory " << dataLocation.toStdString() << std::endl;
	}

	if (data.open(QIODevice::WriteOnly | QIODevice::Text)) {
		QDataStream dataStream(&data);
		QJsonDocument jsonDocument = fileData->write();
        dataStream << jsonDocument.toBinaryData();

		data.close();
	}
	else {
		QFileInfo info(data);
		std::cerr << "Could not open file " << info.absoluteFilePath().toStdString() << std::endl;
	}
}

FileData* File::read()
{
    cleanFileData();
    auto fileData = new FileData();

	QString dataLocation = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
	QFile data(dataLocation + getFileName());

	if (!QDir(dataLocation).exists()) {
		return fileData;
	}

	if (data.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QDataStream dataStream(&data);

		QJsonDocument jsonDocument = QJsonDocument();
	}

	return fileData;
}

void File::cleanFileData() {
    if(fileData != nullptr) {
        delete fileData;
    }
}

QString File::getFileName()
{
#ifdef QT_DEBUG
	return QString("/data_debug.dat");
#else
	return QString("/data.dat");
#endif
}

Statistics File::generateStatistics() {
    return Statistics();
}

QString File::getFilePath()
{
	return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + getFileName();
}
