#ifndef STATISTICS_H
#define STATISTICS_H

#include "../tracking/entry.h"

#include <map>
#include <string>
#include <QDateTime>

class Statistics
{
public:
    Statistics();

    std::map<std::wstring, std::map<std::wstring, QDateTime> > m_stats;

    void update(Entry*);
    void addWindow(std::wstring, QDateTime);
};

#endif // STATISTICS_H
