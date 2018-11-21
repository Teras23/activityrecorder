#ifndef FILE_H
#define FILE_H

#include "process.h"
#include "filedata.h"
#include "entry.h"
#include "../ui/statistics.h"

#include <QString>
#include <vector>

class File {
public:
    static FileData* read();
    static void write(FileData*);
	static QString getFileName();
	static QString getFilePath();

    static Statistics generateStatistics();

    static void cleanFileData();

    static FileData* fileData;
};

#endif // FILE_H
