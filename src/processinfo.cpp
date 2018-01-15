#include "processinfo.h"
#include "process.h"

#include <QDataStream>

ProcessInfo::ProcessInfo()
{
    m_recordedTime = QTime();
    m_process = Process();
    m_processTitleId = -1;
}

ProcessInfo::ProcessInfo(Process process)
{
    m_recordedTime = QTime::currentTime();
	m_process = process;
    m_processTitleId = -1;
}

ProcessInfo::ProcessInfo(QTime recordedTime, int processTitleId) :
    m_recordedTime(recordedTime)
{
    m_process = Process();
    m_processTitleId = processTitleId;
}

QTime ProcessInfo::getRecordedTime()
{
    return m_recordedTime;
}

Process ProcessInfo::getProcess()
{
	return m_process;
}

int ProcessInfo::getProcessTitleId()
{
    return m_processTitleId;
}

void ProcessInfo::setProcessTitleId(int id)
{
    if(m_processTitleId == -1)
        m_processTitleId = id;
}

QDataStream& operator<<(QDataStream& out, ProcessInfo &processInfo)
{
    out << processInfo.getRecordedTime();
    out << static_cast<qint32>(processInfo.getProcessTitleId());

    return out;
}

QDataStream& operator>>(QDataStream& in, ProcessInfo &processInfo)
{
    QTime recordedTime;
    in >> recordedTime;
    qint32 processTitleId;
    in >> processTitleId;

    processInfo = ProcessInfo(recordedTime, processTitleId);

    return in;
}
