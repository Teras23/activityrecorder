#ifndef PROCESSINFO_H
#define PROCESSINFO_H

#include "process.h"

#include <QDateTime>
#include <vector>

class ProcessInfo
{
public:
    ProcessInfo();
	ProcessInfo(Process);
    ProcessInfo(QDateTime, int);

	Process getProcess();
    QDateTime getRecordedTime();
    int getProcessTitleId();
    void setProcessTitleId(int);

private:
    QDateTime m_recordedTime;
	Process m_process;
    int m_processTitleId;
};

QDataStream& operator<<(QDataStream&, ProcessInfo&);
QDataStream& operator>>(QDataStream&, ProcessInfo&);

#endif // PROCESSINFO_H
