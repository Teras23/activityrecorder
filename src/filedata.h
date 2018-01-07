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

    QString m_magic;
    QString m_version;

	int m_processIndex;
	int m_processTitleIndex;

	std::map<std::wstring, int> m_processes;
	std::map<int, std::map<std::wstring, int>> m_processTitles;

    std::vector<Entry> m_entries;

    FileData update(FileData, Entry);
    bool isValid();
};

QDataStream& operator<<(QDataStream&, const FileData&);
QDataStream& operator>>(QDataStream&, FileData&);

#endif // FILEDATA_H
