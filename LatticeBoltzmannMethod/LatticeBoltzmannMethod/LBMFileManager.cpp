#include "LBMFileManager.h"



CLBMFileManager::CLBMFileManager()
{
}


CLBMFileManager::~CLBMFileManager()
{
}

CGnuplotFileManager::FILE CLBMFileManager::writeColorMap(string * fileName, CLBM* lbm, CLBM::LBMInfo info,CLBMFileManager::TYPE type) const
{
	if (streams->find(*fileName) == streams->end()) {
		//キーが設定されていない
		return FILE::NOTOPEN;
	}
	ofstream* ofs = (*streams)[(*fileName)];
	for (int x = 1; x <= info.x;x++) {
		for (int y = 1; y <= info.y;y++) {
			if (type == TYPE::PRESSURE) {
				(*ofs) << info.deltaLength*x << " " << info.deltaLength*y << " " << lbm->getPoint(x - 1, y - 1, 0, CLBM::ACCESS::NOW)->p << std::endl;
				if ((y + 1) > info.y)
					(*ofs) << endl;
			}
			else if(x % 5 == 0 && y % 5 == 0){
				(*ofs) << 
					info.deltaLength*x << " " << 
					info.deltaLength*y << " " << 
					lbm->getPoint(x-1, y-1, 0, CLBM::ACCESS::NOW)->u.get(CVector3<int>::Dim::X) << " " <<
					lbm->getPoint(x-1, y-1, 0, CLBM::ACCESS::NOW)->u.get(CVector3<int>::Dim::Y) <<
					std::endl;
			}
			if(type == TYPE::VELOCITY)
				printf("%2d , %2d , %5f , %5f\n", x, y, lbm->getPoint(x - 1, y - 1, 0, CLBM::ACCESS::NOW)->u.get(CVector3<int>::Dim::X), lbm->getPoint(x - 1, y - 1, 0, CLBM::ACCESS::NOW)->u.get(CVector3<int>::Dim::Y));
		}
	}
}
