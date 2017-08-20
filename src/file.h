#ifndef FILE_H
#define FILE_H

#include "process.h"
#include "filedata.h"

#include <vector>

class File {
public:
	File();

	static void Read();
	static void Write();
	static void Update(std::vector<std::pair<Process, int>>);
};

#endif // FILE_H
