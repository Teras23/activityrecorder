#ifndef ENTRY_H
#define ENTRY_H
#include "process.h"

#include <vector>
#include <QTime>

class Entry
{
private:
    static Entry _currentEntry;
public:
    Entry();

    QTime m_startTime;
    QTime m_endTime;
    std::vector<std::pair<Process, int>> m_processBuffer;

    void update(Process);

    static Entry getCurrent();
    static Entry endCurrent();
};

#endif // ENTRY_H
