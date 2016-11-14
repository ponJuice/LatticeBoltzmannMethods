#pragma once
#include "lbm_data.h"

__global__ void lbm_calc(LBM::LatticeInfo* l_info,LBM::LBMPoint* points);

__global__ void lbm_test1();
__global__ void lbm_test2();