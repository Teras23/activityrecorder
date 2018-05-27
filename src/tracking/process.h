#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <vector>

class Process {
public:
	Process();
	Process(std::wstring title, std::wstring path);

	static Process getActiveProcess();

    std::wstring getProcessTitle();
    std::wstring getProcessPath();
    std::wstring getProcessName();

    bool operator==(const Process &r) {
        return (getProcessName() == r.m_processName) && (getProcessPath() == r.m_processPath);
    }
	std::wstring m_processTitle;
	std::wstring m_processPath;
	std::wstring m_processName;
};

#endif // PROCESS_H
