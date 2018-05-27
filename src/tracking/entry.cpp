#include "entry.h"

#include <QDataStream>
#include <QDebug>

Entry::Entry()
{
    m_startTime = QDateTime::currentDateTime();
    m_endTime = QDateTime();
	m_processBuffer = std::vector<ProcessInfo>();
}

Entry::Entry(Entry& entry)
{
    m_startTime = entry.m_startTime;
    m_endTime = entry.m_endTime;
    m_processBuffer = entry.m_processBuffer;
}

void Entry::update(ProcessInfo processInfo) {
	m_processBuffer.push_back(processInfo);
}

void Entry::endCurrent() {
    m_endTime = QDateTime::currentDateTime();
}

QDataStream& operator<<(QDataStream& out, const Entry &entry)
{
    out << entry.m_startTime;
    out << entry.m_endTime;

    qDebug() << entry.m_startTime << " " << entry.m_endTime;

    out << static_cast<qint32>(entry.m_processBuffer.size());

    for(auto processInfo : entry.m_processBuffer) {
        out << processInfo;
    }

    return out;
}

QDataStream& operator>>(QDataStream& in, Entry &entry)
{
    in >> entry.m_startTime;
    in >> entry.m_endTime;

    qint32 processBufferSize;
    in >> processBufferSize;

    for(int i = 0; i < processBufferSize; i++) {
        ProcessInfo processInfo;
        in >> processInfo;
        entry.m_processBuffer.push_back(processInfo);
    }

    return in;
}
