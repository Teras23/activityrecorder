#ifndef FILE_H
#define FILE_H

#include "process.h"
#include "filedata.h"

#include <QString>
#include <vector>

class File {
public:
	static FileData read();
	static void write(FileData);
	static void update(std::vector<std::pair<Process, int>>);
	static QString getFileName();
	static QString getFilePath();
};

#endif // FILE_H
