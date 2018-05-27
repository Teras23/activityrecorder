#include "filedata.h"
#include "process.h"
#include <iostream>

#define MAGIC "ARDF"
#define VERSION "0.1"

FileData::FileData()
{
    m_processes = std::map<std::wstring, int>();
    m_processesReverse = std::map<int, std::wstring>();
    m_processTitles = std::map<int, std::map<std::wstring, int>>();
    m_processIndex = 0;
    m_processTitleIndex = 0;

    m_entries = std::vector<Entry>();

    m_magic = QString(MAGIC);
    m_version = QString(VERSION);
    m_empty = true;
}


FileData FileData::update(FileData fileData, Entry entry)
{
    auto processHistory = entry.m_processBuffer;

    for (auto processPair : processHistory) {
        auto process = processPair.getProcess();
        auto time = processPair.getRecordedTime();

        auto processIndex = 0;

        if (m_processes.find(process.getProcessPath()) == m_processes.end()) {
            m_processes.insert(std::make_pair(process.getProcessPath(), m_processIndex));
            processIndex = m_processIndex;
            m_processIndex++;
        }
        else {
            processIndex = m_processes.at(process.getProcessPath());
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

            m_processTitles[processIndex] = processTitleMap;
        }
    }

    entry = updateProcessTitleIds(entry);

    m_entries.push_back(entry);

    m_empty = false;

    createReverse();

    return fileData;
}

Entry FileData::updateProcessTitleIds(Entry entry)
{
    for(int i = 0; i < entry.m_processBuffer.size(); i++) {
        Process process = entry.m_processBuffer[i].getProcess();
        std::wstring processPath = process.getProcessPath();
        int processId = m_processes.at(processPath);
        int windowId = m_processTitles.at(processId).at(process.getProcessTitle());
        entry.m_processBuffer[i].setProcessTitleId(windowId);
    }
    return entry;
}

void FileData::createReverse()
{
    m_processesReverse.clear();

    for(auto process : m_processes) {
        m_processesReverse.insert(std::make_pair(process.second, process.first));
    }
}

bool FileData::isValid()
{
    if(m_magic.compare(MAGIC) != 0) {
        return false;
    }

    if(m_version.compare(VERSION) != 0) {
        return false;
    }
    return true;
}

bool FileData::isEmpty()
{
    return m_empty;
}

QDataStream& operator<<(QDataStream& out, const FileData &fd)
{
    out << fd.m_magic;
    out << fd.m_version;

    //process tree
    out << static_cast<qint32>(fd.m_processIndex);
    out << static_cast<qint32>(fd.m_processes.size());		//number of different processes

    for (auto process : fd.m_processes) {
        out << static_cast<qint32>(process.second);				//id of the process
        out << QString::fromStdWString(process.first);			//process name
    }

    //window title tree
    out << static_cast<qint32>(fd.m_processTitleIndex);
    out << static_cast<qint32>(fd.m_processTitles.size()); //number of different processes

    for (auto process : fd.m_processTitles) {
        out << static_cast<qint32>(process.first);				//id of the process
        out << static_cast<qint32>(process.second.size());		//number of window titles for process

        for (auto processTitle : process.second) {
            out << static_cast<qint32>(processTitle.second);		//id of the window title
            out << QString::fromStdWString(processTitle.first);	//name of the process title
        }
    }

    //entries
    out << static_cast<qint32>(fd.m_entries.size());

    for (auto entry : fd.m_entries) {
        out << entry;
    }

    return out;
}

QDataStream& operator>>(QDataStream& in, FileData &fd)
{
    in >> fd.m_magic;
    in >> fd.m_version;

    //process tree
    qint32 processesSize;
    in >> fd.m_processIndex;
    in >> processesSize;

    for (auto i = 0; i < processesSize; i++) {
        int processId;
        QString processName;
        in >> processId >> processName;
        fd.m_processes.insert(std::make_pair(processName.toStdWString(), processId));
    }

    //window title tree
    qint32 processTitlesSize;
    in >> fd.m_processTitleIndex;
    in >> processTitlesSize;

    for (auto i = 0; i < processTitlesSize; i++) {
        qint32 processId;
        qint32 processWindowSize;

        in >> processId >> processWindowSize;

        auto windows = std::map<std::wstring, int>();

        for (auto j = 0; j < processWindowSize; j++) {
            qint32 windowId;
            QString windowName;
            in >> windowId >> windowName;
            windows.insert(std::make_pair(windowName.toStdWString(), windowId));
        }

        fd.m_processTitles.insert(std::make_pair(processId, windows));
    }

    //entries
    qint32 entriesSize;
    in >> entriesSize;

    for (int i = 0; i < entriesSize; i++) {
        Entry entry;
        in >> entry;
        fd.m_entries.push_back(entry);
    }

    fd.createReverse();

    return in;
}
