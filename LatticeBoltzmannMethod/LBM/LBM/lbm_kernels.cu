#pragma once
#include "lbm_kernels.cuh"

#include <stdio.h>

__global__ void lbm_calc(LBM::LatticeInfo* l_info,LBM::LBMPoint* points){
	//スレッドIDとブロックIDからインデックスを算出
	int index_in = blockIdx.x * blockDim.x + threadIdx.x;
	
	//境界を超える場合は0
	int branch = index_in >= 0;
	branch *= index_in < l_info->size;

	//参照領域も含めたインデックスを算出
	int index_out = l_info->offset 
		+ index_in 
		+ (index_in / ( l_info->x_max_in * l_info->y_max_in ))
			* (l_info->x_max_out * l_info->y_max_out - l_info->x_max_in * l_info->y_max_in) 
		+ (index_in / l_info->x_max_in) % 2 
			* l_info->max_speed
			* 2;
	index_out = index_out * branch;

	printf("lbm_calc in : %d  out : %d size : %d branch : %d\n",index_in,index_out,l_info->size,branch);

	//計算
	points[index_out].density = 0;
	points[index_out].vx = 0;
	points[index_out].vy = 0;
	points[index_out].vz = 0;

	for(int n = 0;n < l_info->direct_num;n++){
		printf("	access a : %d index : %d\n",n,index_in);
		points[index_out].a[n] = 0;
	}
}

__global__ void lbm_test1(){
	int index = threadIdx.x;

	printf("test1 %d\n",index);
}

__global__ void lbm_test2(){
	int index = threadIdx.x;

	printf("test2 %d\n",index);
}