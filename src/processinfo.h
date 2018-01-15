#ifndef PROCESSINFO_H
#define PROCESSINFO_H

#include "process.h"

#include <QTime>
#include <vector>

class ProcessInfo
{
public:
    ProcessInfo();
	ProcessInfo(Process);
    ProcessInfo(QTime, int);

	Process getProcess();
	QTime getRecordedTime();

private:
    QTime m_recordedTime;
	Process m_process;
};

QDataStream& operator<<(QDataStream&, ProcessInfo&);
QDataStream& operator>>(QDataStream&, ProcessInfo&);

#endif // PROCESSINFO_H
