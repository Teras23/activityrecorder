#ifndef ENTRY_H
#define ENTRY_H
#include "processinfo.h"

#include <vector>
#include <QTime>

class Entry
{
public:
    Entry();

    QTime m_startTime;
    QTime m_endTime;
	std::vector<ProcessInfo> m_processBuffer;

	void update(ProcessInfo);
	void endCurrent();
};

QDataStream& operator<<(QDataStream&, const Entry&);
QDataStream& operator>>(QDataStream&, Entry&);

#endif // ENTRY_H
