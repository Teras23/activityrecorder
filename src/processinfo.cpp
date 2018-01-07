#include "processinfo.h"
#include "process.h"

ProcessInfo::ProcessInfo(Process process)
{
	m_process = process;
	m_recordedTime = QTime::currentTime();;
}

Process ProcessInfo::getProcess()
{
	return m_process;
}

QTime ProcessInfo::getRecordedTime()
{
	return m_recordedTime;
}
