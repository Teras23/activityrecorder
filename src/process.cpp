#include "process.h"
#include <iostream>
#include <regex>
#include <algorithm>
#include <cctype>
#include <regex>

#ifdef _WIN32
#include <windows.h>
#include <Psapi.h>
#endif

std::vector<std::pair<Process, int>> Process::_processHistory = std::vector<std::pair<Process, int>>();

Process::Process(std::wstring title, std::wstring path) :
	m_processTitle(title), m_processPath(path)
{
#ifdef _WIN32
	std::wregex nameRegex(L"\\\\(.+\\\\)*(.+)\\.");
#elif linux
    std::wregex nameRegex(L"\\/(.+\\/\\/)*(.+)\\/(.*)");
#endif
	std::wsmatch match;
	std::regex_search(path, match, nameRegex);

	if (match.size() > 0) {
		std::wstring name = match[match.size() - 1];
        std::transform(name.begin(), name.end(), name.begin(), tolower);
		m_processName = name;
	}
	else {
		m_processName = L"null";
	}
}

std::wstring Process::getProcessTitle()
{
	return m_processTitle;
}

std::wstring Process::getProcessPath()
{
	return m_processPath;
}

std::wstring Process::getProcessName()
{
	return m_processName;
}

#ifdef _WIN32
BOOL CALLBACK EnumChildWindowsCallback(HWND window, LPARAM lp)
{
	DWORD *pids = (DWORD *)lp;
	DWORD pid = 0;
	GetWindowThreadProcessId(window, &pid);
	if (pid != pids[0]) {
		pids[1] = pid;
	}
	return TRUE;
}
#elif linux

std::wstring callCommand(std::string c)
{
    std::array<char, 128> buffer;
    std::wstring result;
    std::shared_ptr<FILE> pipe(popen(c.c_str(), "r"), pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");
    while (!feof(pipe.get())) {
        if (fgets(buffer.data(), 128, pipe.get()) != nullptr) {
            //get wide string from command output
            const size_t tempSize = strlen(buffer.data());
            std::wstring temp(tempSize, L'#');
            mbstowcs(&temp[0], buffer.data(), 128);
            result += temp;
        }
    }

    return result;
}

#endif

Process Process::getActiveProcess()
{
#ifdef _WIN32
	HWND window = GetForegroundWindow();
	std::wstring title = L"";
	std::wstring path = L"";

	if (window != NULL) {
		const int MAX_TITLE_SIZE = 256;
		wchar_t processTitle[MAX_TITLE_SIZE];
		int response = GetWindowText(window, processTitle, MAX_TITLE_SIZE + 1);

		if (response == 0) {
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

		if (process != NULL) {
			HMODULE hMod;
			DWORD cb;
			if (EnumProcessModules(process, &hMod, sizeof(hMod), &cb)) {
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
#elif linux
    std::wstring result = callCommand("xprop -id $(xprop -root _NET_ACTIVE_WINDOW | cut -d ' ' -f 5) _NET_WM_NAME _NET_WM_PID");

    std::wregex regex(L"\\=\\s(.*)");
    std::wregex lines(L"[\\n\\r]+");
    std::wsmatch m;

    std::regex_search(result, m, regex);
    std::wstring title = m[1];
    title = title.substr(1, title.size() - 2); //Remove quotes
    result = m.suffix();

    std::regex_search(result, m, regex);
    int pid = std::stoi(m[1]);

    std::wregex pathRegex(L"\\-\\>\\s(.*)");

    result = callCommand("ls -l /proc/" + std::to_string(pid) + "/exe");

    std::regex_search(result, m, pathRegex);
    std::wstring path = m[1];
#endif
	return Process(title, path);
}
