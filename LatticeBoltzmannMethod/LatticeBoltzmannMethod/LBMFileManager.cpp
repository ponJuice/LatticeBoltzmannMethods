#include "LBMFileManager.h"



CLBMFileManager::CLBMFileManager()
{
}


CLBMFileManager::~CLBMFileManager()
{
}

CGnuplotFileManager::FILE CLBMFileManager::writeColorMap(string * fileName, CLBM* lbm, CLBM::LBMInfo info,CLBMFileManager::TYPE type) const
{
	int velocity_writeRate = 16;
	ofstream* ofs = nullptr;
	if (openFile(fileName, NEW, &ofs) != OK) {
		OutputDebugString("File Open Failured");
		return NOTOPEN;
	}
	for (int x = 1; x <= info.x;x++) {
		for (int y = 1; y <= info.y;y++) {
			if (type == TYPE::PRESSURE) {
				(*ofs) << info.deltaLength*x << " " << info.deltaLength*y << " " << lbm->getPoint(x - 1, y - 1, 0, CLBM::ACCESS::NOW)->density << std::endl;
				if ((y + 1) > info.y)
					(*ofs) << endl;
			}
			else if(x % velocity_writeRate == 0 && y % velocity_writeRate == 0){
				(*ofs) << 
					info.deltaLength*x << " " << 
					info.deltaLength*y << " " << 
					lbm->getPoint(x-1, y-1, 0, CLBM::ACCESS::NOW)->velocity.get(CVector3<int>::Dim::X) << " " <<
					lbm->getPoint(x-1, y-1, 0, CLBM::ACCESS::NOW)->velocity.get(CVector3<int>::Dim::Y) <<
					std::endl;
			}
		}
	}
}

CGnuplotFileManager::FILE CLBMFileManager::writeVelocityDistribution(string * fileName, CLBM * lbm, CLBM::LBMInfo info) const
{
	ofstream* ofs = nullptr;
	if (openFile(fileName, MODE::NEW, &ofs) != FILE::OK) {
		OutputDebugString("error can't write data");
		return FILE::NOTOPEN;
	}
	for (int n = 0; n < info.y; n += 8) {
		CLBM::Point *p = lbm->getPoint(info.x / 2, n, 0, CLBM::ACCESS::NOW);
		(*ofs) << p->velocity.get(0) << " "<<n << endl;
	}
	ofs->close();
	return FILE::OK;
}
