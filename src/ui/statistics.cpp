#include "statistics.h"
#include <QDebug>
#include <QString>

Statistics::Statistics()
{
    m_stats = std::map<std::wstring, std::map<std::wstring, QDateTime> >();
}

void Statistics::update(Entry* entry) {
    qDebug() << entry->m_startTime.toString();
    for(auto info : entry->m_processBuffer) {
        qDebug() <<
            QString::fromStdWString(info.getProcess().getProcessName()) << " " <<
            QString::fromStdWString(info.getProcess().getProcessPath()) << " " <<
            QString::fromStdWString(info.getProcess().getProcessTitle()) << " " <<
            info.getRecordedTime().toString() << info.getProcessTitleId();
    }
    qDebug() << entry->m_endTime.toString();
}
