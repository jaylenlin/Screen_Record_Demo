//
// Created by jcchen on 2019-2-28.
//

#ifndef VOIPMODULE_H264FILEUTILS_H
#define VOIPMODULE_H264FILEUTILS_H

#include "H264FileUtils.h"

#include <string>
#include <functional>
#include <fstream>

class H264FileWriter
{
public:
    H264FileWriter(std::string path);
    virtual ~H264FileWriter();
    virtual int WriteNalu(unsigned char *p, size_t size);

private:
    std::string mPath;
};


class H264FileReader
{
public:
    H264FileReader(std::string iPath);
    virtual ~H264FileReader();
    virtual int ReadNalu(unsigned char *p, size_t &size, int &type);

private:
	int find_start_code(unsigned char* buf, int start, int end);
	
	std::string mPath;
    std::ifstream mFil;
    unsigned char* mBuf;
	int mPos0;
	int mSize;
};


#endif //VOIPMODULE_H264FILEUTILS_H
