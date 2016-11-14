#pragma once
#include "lbm_data.h"

namespace LBM{
	void getLbmLatticeInfo(LatticeInfo* info,int max_speed,int x_max_out,int y_max_out,int z_max_out,int direct_num);
	void printLatticeInfo(LatticeInfo* info);
}

namespace Math{
	int min(int a, int b);
	int max(int a, int b);
}