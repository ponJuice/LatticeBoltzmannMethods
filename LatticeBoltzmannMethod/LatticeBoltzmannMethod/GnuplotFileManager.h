#pragma once

#include <unordered_map>
#include <iostream>
#include <fstream>
#include <Windows.h>

using namespace std;

class CGnuplotFileManager
{
public: enum MODE{
		POSTSCRIPT,
		NEW
	};
	enum FILE {
		NOTOPEN,
		OK
	};
protected:
	CGnuplotFileManager(const CGnuplotFileManager&) = delete;
	CGnuplotFileManager& operator=(const CGnuplotFileManager&) = delete;
	CGnuplotFileManager(CGnuplotFileManager&&) = delete;
	CGnuplotFileManager& operator=(CGnuplotFileManager&&) = delete;
public:
	CGnuplotFileManager();
	~CGnuplotFileManager();
	CGnuplotFileManager::FILE isOpen(string *fileName) const;
	CGnuplotFileManager::FILE nextDataBlock(string *fileName) const;
	CGnuplotFileManager::FILE writeData(string *fileName, double* data, int size) const;
	CGnuplotFileManager::FILE openFile(string *fileName,MODE mode,ofstream** ors) const;
};

