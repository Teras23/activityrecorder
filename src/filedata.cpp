#include "filedata.h"
#include "process.h"
#include <iostream>

FileData::FileData()
{
	m_processes = std::map<std::wstring, int>();
	m_processTitles = std::map<int, std::map<std::wstring, int>>();
	m_processIndex = 0;
	m_processTitleIndex = 0;
}


FileData FileData::update(FileData fileData)
{
	auto processHistory = Process::_processHistory;

	for (auto processPair : processHistory) {
		auto process = processPair.first;
		auto time = processPair.second;

		auto processIndex = 0;

		if (m_processes.find(process.getProcessName()) == m_processes.end()) {
			m_processes.insert(std::make_pair(process.getProcessName(), m_processIndex));
			processIndex = m_processIndex;
			m_processIndex++;
		}
		else {
			processIndex = m_processes.at(process.getProcessName());
		}

		if (m_processTitles.find(processIndex) == m_processTitles.end()) {
			auto processTitleMap = std::map<std::wstring, int>();
			processTitleMap.insert(std::make_pair(process.getProcessTitle(), m_processTitleIndex));
			m_processTitleIndex++;

			m_processTitles.insert(std::make_pair(processIndex, processTitleMap));
		}
		else {
			auto processTitleMap = m_processTitles.at(processIndex);

			if (processTitleMap.find(process.getProcessTitle()) == processTitleMap.end()) {
				processTitleMap.insert(std::make_pair(process.getProcessTitle(), m_processTitleIndex));
				m_processTitleIndex++;
			}
		}
	}

	return fileData;
}
