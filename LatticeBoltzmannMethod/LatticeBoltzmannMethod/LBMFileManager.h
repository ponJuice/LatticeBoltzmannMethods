#pragma once
#include "GnuplotFileManager.h"
#include "LBM.h"
class CLBMFileManager :
	public CGnuplotFileManager
{
public:
	enum TYPE {
		VELOCITY,
		PRESSURE
	};
	CLBMFileManager();
	~CLBMFileManager();
	CGnuplotFileManager::FILE writeColorMap(string *fileName, CLBM* lbm, CLBM::LBMInfo info,CLBMFileManager::TYPE type) const;
};

