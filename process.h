#ifndef PROCESS_H
#define PROCESS_H

#include <windows.h>
#include <Psapi.h>
#include <string>

class Process {
public:
	Process(std::string title, std::string path);

	static Process getActiveProcess();

	std::string getProcessTitle();
	std::string getProcessName();

private:
	std::string m_processTitle;
	std::string m_processName;
};

#endif // PROCESS_H
