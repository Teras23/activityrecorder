#include "file.h"

#include <Qfile>
#include <QDir>
#include <QStandardPaths>
#include <iostream>
#include <map>

File::File() {}


void File::Write()
{
	QString dataLocation = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

	QFile data(dataLocation + "/data.dat");

	FileData fileData = FileData();

	fileData.analize();

	std::clog << "Save " << Process::_processHistory.size() << std::endl;

	if (!QDir(dataLocation).exists()) {
		QDir().mkpath(dataLocation);
		std::clog << "Created directory " << dataLocation.toStdString() << std::endl;
	}

	if (data.open(QIODevice::WriteOnly | QIODevice::Text)) {
		data.write("test");
		data.close();
	}
	else {
		QFileInfo info(data);
		std::cerr << "Could not open file " << info.absoluteFilePath().toStdString() << std::endl;
	}
}

void File::Read() {}

void File::Update(std::vector<std::pair<Process, int>> processHistory)
{
	Process::_processHistory.clear();
}
