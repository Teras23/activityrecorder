#include "entry.h"

Entry Entry::_currentEntry = Entry();

Entry::Entry()
{
    m_startTime = QTime::currentTime();
    m_endTime = QTime();
    m_processBuffer = std::vector<std::pair<Process, int>>();
}

void Entry::update(Process process) {
    if(m_processBuffer.size() > 0 && m_processBuffer[m_processBuffer.size() - 1].first == process) {
        m_processBuffer[m_processBuffer.size() - 1].second++;
    }
    else {
        m_processBuffer.push_back(std::make_pair(process, 1));
    }
}

Entry Entry::getCurrent() {
    return _currentEntry;
}

Entry Entry::endCurrent() {
    _currentEntry.m_endTime = QTime::currentTime();
    return _currentEntry;
}


