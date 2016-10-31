#include "GnuplotFileManager.h"

CGnuplotFileManager::CGnuplotFileManager() {
}

CGnuplotFileManager::~CGnuplotFileManager() {

}

CGnuplotFileManager::FILE CGnuplotFileManager::writeData(string *fileName, double *data, int size) const{
	return NOTOPEN;
}

CGnuplotFileManager::FILE CGnuplotFileManager::isOpen(string *fileName) const {
	return FILE::NOTOPEN;
}

CGnuplotFileManager::FILE CGnuplotFileManager::nextDataBlock(string *fileName) const{
	return NOTOPEN;
}

CGnuplotFileManager::FILE CGnuplotFileManager::openFile(string *fileName,MODE mode,ofstream** ofs) const{
	if (*ofs != nullptr) {
		(*ofs)->close();
	}
	if (mode == MODE::POSTSCRIPT) {
		ofstream* _ofs = new ofstream(*fileName, ios::app);
		(*ofs) = _ofs;
	}
	else if (mode == MODE::NEW) {
		ofstream* _ofs = new ofstream(*fileName, ios::out);
		(*ofs) = _ofs;
	}
	else
		return FILE::NOTOPEN;
	return FILE::OK;
}