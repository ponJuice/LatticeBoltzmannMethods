#include "GnuplotFileManager.h"

CGnuplotFileManager::CGnuplotFileManager() {
	streams = new unordered_map<string, ofstream*>();
}

CGnuplotFileManager::~CGnuplotFileManager() {
	OutputDebugString("GnuplotFileManager Destructor\n");
	auto ite = streams->begin();
	while (ite != streams->end()) {
		ite->second->close();
		delete ite->second;
		ite++;
	}
	streams->clear();
	delete streams;
}

CGnuplotFileManager::FILE CGnuplotFileManager::writeData(string *fileName, double *data, int size) const{
	if (streams->find(*fileName) == streams->end()) {
		//キーが設定されていない
		return FILE::NOTOPEN;
	}
	ofstream* ofs = (*streams)[(*fileName)];
	for (int n = 0; n < size; n++) {
		(*ofs) << data[n] << " ";
	}
	(*ofs) << endl;
}

CGnuplotFileManager::FILE CGnuplotFileManager::isOpen(string *fileName) const {
	if (streams->find(*fileName) == streams->end()) {
		//キーが設定されていない
		return FILE::NOTOPEN;
	}
	return FILE::OK;
}

CGnuplotFileManager::FILE CGnuplotFileManager::nextDataBlock(string *fileName) const{
	if (isOpen(fileName) == FILE::NOTOPEN)
		return isOpen(fileName);
	ofstream* ofs = (*streams)[(*fileName)];
	(*ofs) << endl;
	(*ofs) << endl;
	(*ofs) << endl;
}

CGnuplotFileManager::FILE CGnuplotFileManager::openFile(string *fileName,MODE mode) const{
	ofstream* ofs;
	if(mode == MODE::POSTSCRIPT)
		ofs = new ofstream(*fileName,ios::app);
	else if (mode == MODE::NEW)
		ofs = new ofstream(*fileName, ios::out);
	else
		return FILE::NOTOPEN;
	(*streams)[(*fileName)] = ofs;
	return FILE::OK;
}