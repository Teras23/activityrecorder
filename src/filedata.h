#ifndef FILEDATA_H
#define FILEDATA_H
#include "entry.h"

#include <map>
#include <vector>
#include <string>

#include <QDataStream>

class FileData {
public:
	FileData();

	int m_processIndex;
	int m_processTitleIndex;

	std::map<std::wstring, int> m_processes;
	std::map<int, std::map<std::wstring, int>> m_processTitles;

    std::vector<Entry> m_entries;

    FileData update(FileData, Entry);
};

QDataStream& operator<<(QDataStream&, const FileData&);

#endif // FILEDATA_H
