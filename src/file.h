#ifndef FILE_H
#define FILE_H

#include "process.h"
#include "filedata.h"
#include "entry.h"

#include <vector>

class File {
public:
	static FileData Read();
	static void Write(FileData);
    static void Update(Entry);
};

#endif // FILE_H
