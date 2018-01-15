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
    int getProcessTitleId();
    void setProcessTitleId(int);

private:
    QTime m_recordedTime;
	Process m_process;
    int m_processTitleId;
};

QDataStream& operator<<(QDataStream&, ProcessInfo&);
QDataStream& operator>>(QDataStream&, ProcessInfo&);

#endif // PROCESSINFO_H
