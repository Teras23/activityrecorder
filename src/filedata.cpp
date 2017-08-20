#include "filedata.h"
#include "process.h"
#include <iostream>

FileData::FileData()
{
	m_processes = std::map<std::wstring, int>();
	m_processIndex = 1;
}


FileData FileData::analize()
{
	auto fileData = FileData();

	auto processHistory = Process::_processHistory;

	auto processes = std::map<std::wstring, int>();
	auto processTitles = std::map<int, std::wstring>();

	for (auto processPair : processHistory) {
		auto process = processPair.first;
		auto time = processPair.second;

		int processIndex = 0;

		if (processes.find(process.getProcessName()) != processes.end()) {
			processes.insert(std::make_pair(process.getProcessName(), m_processIndex));
			processIndex = m_processIndex;
			m_processIndex++;
		}
		else {
			processIndex = processes.at(process.getProcessName());
		}
	}

	return fileData;
}
