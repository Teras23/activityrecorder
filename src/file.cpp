#include "file.h"

#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include <QDataStream>
#include <QDebug>
#include <iostream>
#include <map>

void File::write(FileData fileData)
{
	QString dataLocation = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

	QFile data(dataLocation + getFileName());

	if (!QDir(dataLocation).exists()) {
		QDir().mkpath(dataLocation);
		std::clog << "Created directory " << dataLocation.toStdString() << std::endl;
	}

	if (data.open(QIODevice::WriteOnly | QIODevice::Text)) {
		QDataStream dataStream(&data);

        dataStream << fileData;

		data.close();
	}
	else {
		QFileInfo info(data);
		std::cerr << "Could not open file " << info.absoluteFilePath().toStdString() << std::endl;
	}
}

FileData File::read()
{
	auto fileData = FileData();

	QString dataLocation = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
	QFile data(dataLocation + getFileName());

	if (!QDir(dataLocation).exists()) {
		return fileData;
	}

	if (data.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QDataStream dataStream(&data);

		int processIndex, processesSize;
		dataStream >> processIndex >> processesSize;
		fileData.m_processIndex = processIndex;

		for (auto i = 0; i < processesSize; i++) {
			int processId;
			QString processName;
			dataStream >> processId >> processName;
			fileData.m_processes.insert(std::make_pair(processName.toStdWString(), processId));
		}

		int processTitleIndex, processTitlesSize;
		dataStream >> processTitleIndex >> processTitlesSize;
		fileData.m_processTitleIndex = processTitleIndex;

		for (auto i = 0; i < processTitlesSize; i++) {
			int processId;
			int processWindowSize;

			dataStream >> processId >> processWindowSize;

			auto windows = std::map<std::wstring, int>();

			for (auto j = 0; j < processWindowSize; j++) {
				int windowId;
				QString windowName;
				dataStream >> windowId >> windowName;
				windows.insert(std::make_pair(windowName.toStdWString(), windowId));
			}

			fileData.m_processTitles.insert(std::make_pair(processId, windows));
		}
	}

	return fileData;
}

void File::update(Entry entry)
{
	auto fileData = read();

    fileData.update(fileData, entry);

    write(fileData);

	Process::_processHistory.clear();
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
