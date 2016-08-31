#include "LBM.h"



CLBM::CLBM(LBMInfo lbm)
{
	info = lbm;
	info.relaxationTime = 1 / info.relaxationTime;
	(*direction) = new double[info.directionNum];
	for (int n = 0; n < info.directionNum ; n++) {
		direction[n] = new double[info.x * info.y * info.z];
	}
}


CLBM::~CLBM()
{
	for (int n = 0; n < info.directionNum; n++) {
		delete[] direction[n];
	}
	delete[] direction;
}

double CLBM::getValue(int x, int y, int z,int direct) {
	return direction[direct][info.x * info.y * z + info.x * y + x];
}

void CLBM::setValue(int x, int y, int z,int direct,double value) {
	direction[direct][info.x * info.y * z + info.x * y + x] = value;
}

double CLBM::particleDistributionFunction(int direct, double time, int x, int y, int z) {
	return getValue(x, y, z, direct)*(1 - info.relaxationTime)
		+ info.relaxationTime * equilibriumParticleProbabilityDistributionFunction(time, x, y, z);
}

double CLBM::equilibriumParticleProbabilityDistributionFunction(double time, int x, int y, int z) {

}