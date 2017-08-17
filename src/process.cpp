#include "process.h"
#include <iostream>

Process::Process(std::string title, std::string name) :
	m_processTitle(title), m_processName(name)
{

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
	std::string name = "";

	if(window != NULL) {
		int textLength = GetWindowTextLength(window);
		LPSTR tempTitle = new char[textLength];
		int response = GetWindowTextA(window, tempTitle, textLength + 1);
		title = tempTitle;

		DWORD pids[2] = {0, 0}; //Owner and child pid
		GetWindowThreadProcessId(window, pids);
		pids[1] = pids[0];

		EnumChildWindows(window, EnumChildWindowsCallback, (LPARAM)pids);
		HANDLE process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pids[1]);

		std::cout << "Process: " << process << std::endl;

		LPSTR processName = new char[MAX_PATH];

		if(process != NULL) {
			HMODULE hMod;
			DWORD cb;
			if(EnumProcessModules(process, &hMod, sizeof(hMod), &cb)) {
				GetModuleBaseNameA(process, hMod, processName, MAX_PATH);
			}
			else {
				//TODO: log as error
				std::cout << GetLastError() << std::endl;
			}
		}

		CloseHandle(process);

		std::cout << "Pid: " << pids << std::endl << "Name: " << processName << std::endl;

		name = processName;

		if(response == 0) {
			DWORD error = GetLastError();
			//std::cout << "no text " << std::hex << error << std::endl;
		}
	}
	else {
		DWORD error = GetLastError();
		std::cout << "no win " << std::hex << error << std::endl;
	}

	return Process(title, name);
}
