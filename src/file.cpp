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

	fileData.update(fileData);

	if (!QDir(dataLocation).exists()) {
		QDir().mkpath(dataLocation);
		std::clog << "Created directory " << dataLocation.toStdString() << std::endl;
	}

	if (data.open(QIODevice::WriteOnly | QIODevice::Text)) {
		QDataStream dataStream(&data);
		dataStream << static_cast<qint32>(fileData.m_processIndex);
		dataStream << static_cast<qint32>(fileData.m_processes.size());		//number of different processes				

		for (auto process : fileData.m_processes) {
			dataStream << static_cast<qint32>(process.second);				//id of the process	
			dataStream << QString::fromStdWString(process.first);			//process name
		}

		dataStream << static_cast<qint32>(fileData.m_processTitleIndex);
		dataStream << static_cast<qint32>(fileData.m_processTitles.size()); //number of different processes	

		for (auto process : fileData.m_processTitles) {
			dataStream << static_cast<qint32>(process.first);				//id of the process
			dataStream << static_cast<qint32>(process.second.size());		//number of window titles for process				

			for (auto processTitle : process.second) {
				dataStream << static_cast<qint32>(processTitle.second);		//id of the window title
				dataStream << QString::fromStdWString(processTitle.first);	//name of the process title
			}
		}

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

void File::update(std::vector<std::pair<Process, int>> processHistory)
{
	auto fileData = read();

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
