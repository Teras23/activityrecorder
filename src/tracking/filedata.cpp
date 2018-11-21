#include "filedata.h"
#include <QDebug>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonDocument>

FileData::FileData() {
    m_processes = std::map<std::wstring, int>();
    m_processesReverse = std::map<int, std::wstring>();
    m_processTitles = std::map<int, std::map<std::wstring, int>>();
    m_processTitlesDirect = std::map<int, std::wstring>();

    m_processInfoVector = std::vector<ProcessInfo *>();
}

FileData::~FileData() {
    for (auto it = m_processInfoVector.begin(); it != m_processInfoVector.end(); it++) {
        if (*it != nullptr) {
            //delete *it;
        }
    }
    m_processInfoVector.clear();
}

void FileData::update(ProcessInfo *processInfo) {
    m_processInfoVector.push_back(processInfo);

    createReverse();
}

void FileData::createReverse() {
    m_processesReverse.clear();

    for (auto process : m_processes) {
        m_processesReverse.insert(std::make_pair(process.second, process.first));
    }
}

QJsonDocument FileData::write() {
    QJsonArray processes;
    for (auto processInfo : m_processInfoVector) {
        QJsonObject process;
        process.insert("title", QJsonValue::fromVariant(
                QString::fromStdWString(processInfo->getProcess().m_processTitle)));
        process.insert("window_name", QJsonValue::fromVariant(
                QString::fromStdWString(processInfo->getProcess().m_processName)));
        process.insert("process_path", QJsonValue::fromVariant(
                QString::fromStdWString(processInfo->getProcess().m_processPath)));

        process.insert("datetime", QJsonValue::fromVariant(
                processInfo->getRecordedTime().toString(Qt::ISODate)));
        processes.append(process);
    }
    QJsonDocument doc(processes);
    return doc;
}
