#include "processinfo.h"
#include "process.h"

#include <QDataStream>

ProcessInfo::ProcessInfo()
{
    m_recordedTime = QTime();
    m_process = Process();
}

ProcessInfo::ProcessInfo(Process process)
{
    m_recordedTime = QTime::currentTime();
	m_process = process;
}

ProcessInfo::ProcessInfo(QTime recordedTime, int processId) :
    m_recordedTime(recordedTime)
{
    m_process = Process();
}

QTime ProcessInfo::getRecordedTime()
{
    return m_recordedTime;
}

Process ProcessInfo::getProcess()
{
	return m_process;
}

QDataStream& operator<<(QDataStream& out, ProcessInfo &processInfo)
{
    out << processInfo.getRecordedTime();
    int processId = 0;
    out << static_cast<qint32>(processId);

    return out;
}

QDataStream& operator>>(QDataStream& in, ProcessInfo &processInfo)
{
    QTime recordedTime;
    in >> recordedTime;
    qint32 processId;
    in >> processId;

    processInfo = ProcessInfo(recordedTime, processId);

    return in;
}
