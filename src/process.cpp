#include "process.h"
#include <iostream>
#include <regex>
#include <algorithm>

Process::Process(std::string title, std::string path) :
	m_processTitle(title), m_processPath(path)
{
	std::regex nameRegex("\\\\(.+\\\\)*(.+)\\.");
	std::smatch match;
	std::regex_search(path, match, nameRegex);

	if(match.size() > 0) {
		std::string name = match[match.size() - 1];
		std::transform(name.begin(), name.end(), name.begin(), ::tolower);
		m_processName = name;
	}
	else {
		m_processName = "null";
	}
}

std::string Process::getProcessTitle() {
	return m_processTitle;
}

std::string Process::getProcessName() {
	return m_processName;
}

BOOL CALLBACK EnumChildWindowsCallback(HWND window, LPARAM lp) {
	DWORD *pids = (DWORD *)lp;
	DWORD pid = 0;
	GetWindowThreadProcessId(window, &pid);
	if(pid != pids[0]) {
		pids[1] = pid;
	}
	return TRUE;
}

Process Process::getActiveProcess() {
	HWND window = GetForegroundWindow();
	std::string title = "";
	std::string path = "";

	if(window != NULL) {
		int textLength = GetWindowTextLength(window);
		LPSTR tempTitle = new char[textLength];
		int response = GetWindowTextA(window, tempTitle, textLength + 1);

		if(response == 0) {
			title = "null";
		}
		else {
			title = tempTitle;
		}

		DWORD pids[2] = {0, 0}; //Owner and child pid
		GetWindowThreadProcessId(window, pids);
		pids[1] = pids[0];



		EnumChildWindows(window, EnumChildWindowsCallback, (LPARAM)pids);
		HANDLE process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pids[1]);

		LPSTR processPath = new char[MAX_PATH];

		if(process != NULL) {
			HMODULE hMod;
			DWORD cb;
			if(EnumProcessModules(process, &hMod, sizeof(hMod), &cb)) {
				GetModuleFileNameExA(process, hMod, processPath, MAX_PATH);
			}
			else {
				DWORD error = GetLastError();
				std::cerr << error << std::endl;
			}
		}

		CloseHandle(process);

		path = processPath;
	}
	else {
		DWORD error = GetLastError();
		std::cerr << "no win " << error << std::endl;
	}

	return Process(title, path);
}
