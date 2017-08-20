#ifndef FILEDATA_H
#define FILEDATA_H

#include <map>
#include <vector>
#include <string>

class FileData {
private:
	struct DataDay { };

public:
	FileData();

	int m_processIndex;
	int m_processTitleIndex;

	std::map<std::wstring, int> m_processes;
	std::map<int, std::map<std::wstring, int>> m_processTitles;

	FileData update(FileData);
};

#endif // FILEDATA_H
