#ifndef PROCESSINFO_H
#define PROCESSINFO_H

#include "process.h"

#include <QTime>
#include <vector>

class ProcessInfo
{
public:
	ProcessInfo(Process);
	Process getProcess();
	QTime getRecordedTime();

private:
	Process m_process;
	QTime m_recordedTime;
};

#endif // PROCESSINFO_H
