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

    bool m_empty;

	std::map<std::wstring, int> m_processes;
    std::map<int, std::wstring> m_processesReverse;
	std::map<int, std::map<std::wstring, int>> m_processTitles;

    std::vector<Entry> m_entries;

    FileData update(FileData, Entry);
    Entry updateProcessTitleIds(Entry);
    void createReverse();
    bool isValid();
    bool isEmpty();
};

QDataStream& operator<<(QDataStream&, const FileData&);
QDataStream& operator>>(QDataStream&, FileData&);

#endif // FILEDATA_H
