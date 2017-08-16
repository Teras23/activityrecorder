#include "activitychecker.h"
#include <iostream>

ActivityChecker::ActivityChecker()
{

}

std::string ActivityChecker::getActivity()
{
	HWND window = GetForegroundWindow();
	std::string title = "";

	if(window != NULL)
	{
		//std::cout << window << std::endl;
		int textLength = GetWindowTextLength(window);
		LPSTR text = new char[textLength];
		int response = GetWindowTextA(window, text, textLength + 1);

		DWORD pid = GetWindowThreadProcessId(window, NULL);
		//std::cout << pid << std::endl;

		LPSTR name = new char[256];

		UINT ret = GetWindowModuleFileNameA(window, name, 255);

		//std::cout << name << std::endl;

		if(response == 0) {
			DWORD error = GetLastError();
			//std::cout << "no text " << std::hex << error << std::endl;
		}

		title = text;
	}
	else {
		DWORD error = GetLastError();
		std::cout << "no win " << std::hex << error << std::endl;
	}

	return title;
}
