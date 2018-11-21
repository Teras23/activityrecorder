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
    ~FileData();

	std::map<std::wstring, int> m_processes;
    std::map<int, std::wstring> m_processesReverse;
	std::map<int, std::map<std::wstring, int>> m_processTitles;
    std::map<int, std::wstring> m_processTitlesDirect;

    std::vector<ProcessInfo*> m_processInfoVector;

	QJsonDocument write();
	void update(ProcessInfo*);
    void read(QJsonDocument&);

    void createReverse();
};

#endif // FILEDATA_H
