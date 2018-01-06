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


FileData FileData::update(FileData fileData, Entry entry)
{
    auto processHistory = entry.m_processBuffer;

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

#define MAGIC "ATDF"
#define VERSION "0.1"

QDataStream& operator<<(QDataStream& out, const FileData &fd) {
    QDataStream dataStream;
    dataStream << QString(MAGIC);
    dataStream << QString(VERSION);

    //process tree
    dataStream << static_cast<qint32>(fd.m_processIndex);
    dataStream << static_cast<qint32>(fd.m_processes.size());		//number of different processes

    for (auto process : fd.m_processes) {
        dataStream << static_cast<qint32>(process.second);				//id of the process
        dataStream << QString::fromStdWString(process.first);			//process name
    }

    //window title tree
    dataStream << static_cast<qint32>(fd.m_processTitleIndex);
    dataStream << static_cast<qint32>(fd.m_processTitles.size()); //number of different processes

    for (auto process : fd.m_processTitles) {
        dataStream << static_cast<qint32>(process.first);				//id of the process
        dataStream << static_cast<qint32>(process.second.size());		//number of window titles for process

        for (auto processTitle : process.second) {
            dataStream << static_cast<qint32>(processTitle.second);		//id of the window title
            dataStream << QString::fromStdWString(processTitle.first);	//name of the process title
        }
    }

    //entries

    dataStream << static_cast<qint32>(fd.m_entries.size());

//        for (auto entry : fs.m_entries) {

//        }

    return out;
}
