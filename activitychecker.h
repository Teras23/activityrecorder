#ifndef ACTIVITYCHECKER_H
#define ACTIVITYCHECKER_H

#include <windows.h>
#include <string>

class ActivityChecker
{
public:
	ActivityChecker();
	static std::string getActivity();
};

#endif // ACTIVITYCHECKER_H
