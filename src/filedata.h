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

	std::map<std::wstring, int> m_processes;
	std::map<int, std::wstring> m_processTitles;

	FileData analize();
};

#endif // FILEDATA_H
