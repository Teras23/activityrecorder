#include "process.h"
#include <iostream>
#include <regex>
#include <algorithm>

std::vector<Process> Process::_processHistory = std::vector<Process>();

Process::Process(std::wstring title, std::wstring path) :
	m_processTitle(title), m_processPath(path)
{
	std::wregex nameRegex(L"\\\\(.+\\\\)*(.+)\\.");
	std::wsmatch match;
	std::regex_search(path, match, nameRegex);

	if(match.size() > 0) {
		std::wstring name = match[match.size() - 1];
		std::transform(name.begin(), name.end(), name.begin(), ::tolower);
		m_processName = name;
	}
	else {
		m_processName = L"null";
	}
}

std::wstring Process::getProcessTitle() {
	return m_processTitle;
}

std::wstring Process::getProcessPath() {
	return m_processPath;
}

std::wstring Process::getProcessName() {
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
	std::wstring title = L"";
	std::wstring path = L"";

	if(window != NULL) {
		const int MAX_TITLE_SIZE = 256;
		wchar_t processTitle[MAX_TITLE_SIZE];
		int response = GetWindowText(window, processTitle, MAX_TITLE_SIZE + 1);

		if(response == 0) {
			title = L"null";
		}
		else {
			title = std::wstring(processTitle);
		}

		DWORD pids[2] = {0, 0}; //Owner and child pid
		GetWindowThreadProcessId(window, pids);
		pids[1] = pids[0];

		EnumChildWindows(window, EnumChildWindowsCallback, (LPARAM)pids);
		HANDLE process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pids[1]);

		wchar_t processPath[MAX_PATH];

		if(process != NULL) {
			HMODULE hMod;
			DWORD cb;
			if(EnumProcessModules(process, &hMod, sizeof(hMod), &cb)) {
				DWORD len = GetModuleFileNameEx(process, hMod, processPath, MAX_PATH);
				path = std::wstring(processPath);
			}
			else {
				DWORD error = GetLastError();
				std::cerr << error << std::endl;
				path = L"null";
			}
		}

		CloseHandle(process);
	}

	return 	Process(title, path);
}
