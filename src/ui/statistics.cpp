#include "statistics.h"
#include <QDebug>
#include <QString>

Statistics::Statistics()
{
    m_stats = std::map<std::wstring, std::map<std::wstring, qint64> >();
    m_titleIdDurationMap = std::map<int, qint64>();
    m_idTitleMap = std::map<int, std::wstring>();
}

void Statistics::update(Entry* entry) {
    qDebug() << entry->m_startTime.toString();

    for(int i = 0; i < entry->m_processBuffer.size(); i++) {
        auto info = entry->m_processBuffer[i];

        QDateTime lastTime;

        if(i - 1 < 0) {
            lastTime = entry->m_startTime;
        }
        else {
            lastTime = entry->m_processBuffer[i - 1].getRecordedTime();
        }

        qint64 diff = lastTime.msecsTo(info.getRecordedTime());

        qDebug() << diff << info.getProcessTitleId();

        //If that title is not in the map
        if(m_titleIdDurationMap.find(info.getProcessTitleId()) == m_titleIdDurationMap.end()) {
            m_titleIdDurationMap[info.getProcessTitleId()] = diff;
        }
        else {
            m_titleIdDurationMap[info.getProcessTitleId()] += diff;
        }
    }

    qDebug() << entry->m_endTime.toString();
}
