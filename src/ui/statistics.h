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

    std::map<std::wstring, std::map<std::wstring, qint64> > m_stats;
    std::map<int, qint64> m_titleIdDurationMap;

    std::map<int, std::wstring> m_idTitleMap;

    void update(Entry*);
};

#endif // STATISTICS_H
