#include "file.h"

#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include <QDataStream>
#include <QDebug>
#include <iostream>
#include <map>

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

        dataStream << *fileData;

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

        dataStream >> *fileData;
	}

	return fileData;
}

FileData* File::load()
{
    fileData = read();

    return fileData;
}

FileData* File::update(Entry* entry)
{
    if(fileData == nullptr || fileData->isEmpty()) {
        fileData = read();

        if(!fileData->isValid()) {
            qDebug() << "DATA FORMAT IS NOT VALID!";
            //TODO: make backup of old file and start again
            return fileData;
        }
    }

    fileData->update(entry);

    write(fileData);
    return fileData;
}

FileData* File::saveOver(Entry* entry) {
    cleanFileData();
    auto fileData = new FileData();

    fileData->update(entry);

    write(fileData);
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

QString File::getFilePath()
{
	return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + getFileName();
}
