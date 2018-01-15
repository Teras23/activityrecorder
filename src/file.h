#ifndef FILE_H
#define FILE_H

#include "process.h"
#include "filedata.h"
#include "entry.h"

#include <QString>
#include <vector>

class File {
public:
    static FileData load();
	static FileData read();
	static void write(FileData);
	static FileData update(Entry);
	static QString getFileName();
	static QString getFilePath();

    static FileData fileData;
};

#endif // FILE_H
