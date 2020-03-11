//
// Created by jcchen on 2019-2-28.
//

#include <ostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include "H264FileUtils.h"


H264FileWriter::H264FileWriter(std::string iPath) : mPath(iPath)
{
}

H264FileWriter::~H264FileWriter()
{
}

int H264FileWriter::WriteNalu(unsigned char *p, size_t size)
{
    std::ofstream fil;
    fil.open(mPath, std::ios::binary | std::ios::out | std::ios::app);
    if (fil.is_open())
    {
        fil.write((const char*)p, size);
        fil.close();
    } else
    {
        printf("write h264 frame error\n");
    }
}


H264FileReader::H264FileReader(std::string iPath) : mPath(iPath)
{
	mBuf = NULL;
    mFil.open(iPath, std::ios::binary | std::ios::in);
	mSize = mPos0 = 0;
}

H264FileReader::~H264FileReader()
{
	if(mBuf != NULL) {
		delete []mBuf;
		mBuf = NULL;
	}
    if (mFil.is_open()){
        mFil.close();
    }
}

int H264FileReader::ReadNalu(unsigned char *p, size_t &size, int &type)
{
    if (mBuf == NULL)
    {
		if (!mFil.is_open()){
			printf("file is not open,path:%s\n", mPath.c_str());
			return -1;
		}
		
		mFil.seekg(0, std::ios::end); 
		mSize = mFil.tellg();
		mFil.seekg(0, std::ios::beg); 
		
		if (mSize <= 100 ){
			printf("file is too short, path:%s\n", mPath.c_str());
			return -2;
		}
		
		mBuf = new unsigned char [mSize];
		if (auto rLen = mFil.read((char *)mBuf, mSize).gcount() != mSize)
		{
			printf("read file len error:%d,%d\n", rLen, mSize);
			return -3;
		}
		mPos0 = find_start_code(mBuf, 0, mSize);
		if(mPos0 < 0) {
			printf("find start code error:%d,\n", mPos0);
			return -4;
		}
	}
	
	int pos1 = find_start_code(mBuf, mPos0+4, mSize);
    if (pos1 < 0){
        pos1 = mSize;
    }
    if(pos1 - mPos0) {
		memcpy(p, mBuf+mPos0, pos1-mPos0);
		size = pos1 - mPos0;
		type = p[4] & 0x1f;
		mPos0 = pos1;
		return 1;
	}
    return 0;
}

int H264FileReader::find_start_code(unsigned char* buf, int start, int end)
{
    int pos = -1, i = 0;
	
	for (i = start; i < end - 4; i++)
	{
		if (buf[i] == 0 && buf[i+1] == 0 && buf[i+2] == 0 && buf[i+3] == 1)
		{
			pos = i;
			break;
		}
	}
    return pos;
}
