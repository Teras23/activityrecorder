#include "entry.h"

Entry::Entry()
{
    m_startTime = QTime::currentTime();
    m_endTime = QTime();
	m_processBuffer = std::vector<ProcessInfo>();
}

void Entry::update(ProcessInfo processInfo) {
	m_processBuffer.push_back(processInfo);
}

void Entry::endCurrent() {
	m_endTime = QTime::currentTime();
}


