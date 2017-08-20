#include "file.h"

#include <Qfile>
#include <QDir>
#include <QStandardPaths>
#include <iostream>
#include <map>

void File::Write(FileData fileData)
{
	QString dataLocation = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

	QFile data(dataLocation + "/data.dat");

	fileData.update();

	std::clog << "Save " << Process::_processHistory.size() << std::endl;

	if (!QDir(dataLocation).exists()) {
		QDir().mkpath(dataLocation);
		std::clog << "Created directory " << dataLocation.toStdString() << std::endl;
	}

	if (data.open(QIODevice::WriteOnly | QIODevice::Text)) {
		QByteArray byteArray;
		byteArray.append(fileData.m_processes.size());					//number of different processes

		for(auto process : fileData.m_processes) {
			byteArray.append(process.second);							//id of the process
			byteArray.append(process.first.size());						//size of the procress name
			byteArray.append(QString::fromStdWString(process.first));	//procress name
		}

		byteArray.append(fileData.m_processTitles.size());				//number of different processes

		for(auto process : fileData.m_processTitles) {
			byteArray.append(process.first);							//id of the process
			byteArray.append(process.second.size());					//number of window titles for process

			for(auto processTitle : process.second) {				
				byteArray.append(processTitle.second);					//id of the window title
				byteArray.append(processTitle.first.size());			//size of the title name
				byteArray.append(QString::fromStdWString(processTitle.first));	//name of the process title
			}
		}

		data.write(byteArray);

		data.close();
	}
	else {
		QFileInfo info(data);
		std::cerr << "Could not open file " << info.absoluteFilePath().toStdString() << std::endl;
	}
}

FileData File::Read()
{
	auto fileData = FileData();

	return fileData;
}

void File::Update(std::vector<std::pair<Process, int>> processHistory)
{
	auto fileData = Read();

	Write(fileData);

	Process::_processHistory.clear();
}
