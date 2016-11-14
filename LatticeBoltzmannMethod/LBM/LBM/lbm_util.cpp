#pragma once
#include <stdio.h>
#include "lbm_util.h"

namespace LBM{

	void getLbmLatticeInfo(LatticeInfo* info,int max_speed,int x_max_out,int y_max_out,int z_max_out,int direct_num){
		info->x_max_out = x_max_out;
		info->y_max_out = y_max_out;
		info->z_max_out = z_max_out;
		info->direct_num = direct_num;
		info->max_speed = max_speed;
		info->x_max_in = x_max_out - max_speed * 2;
		info->y_max_in = y_max_out - max_speed * 2;
		info->z_max_in = z_max_out - max_speed * 2;
		info->size = info->x_max_in * info->y_max_in * info->z_max_in;
		info->offset = x_max_out * y_max_out + max_speed * x_max_out + max_speed;

	}

	void printLatticeInfo(LatticeInfo* info){
		printf("/// Lattice Info ///\n");
		printf("direct_num : %d\n",info->direct_num);
		printf("max_speed : %d\n",info->max_speed);
		printf("x_max_out : %d\n",info->x_max_out);
		printf("y_max_out : %d\n",info->x_max_out);
		printf("z_max_out : %d\n",info->x_max_out);
		printf("x_max_in : %d\n",info->x_max_in);
		printf("y_max_in : %d\n",info->y_max_in);
		printf("z_max_in : %d\n",info->z_max_in);
		printf("size : %d\n",info->size);
		printf("offset : %d\n",info->offset);
		printf("////////////////////\n");
	}
}

namespace Math{
		int min(int a, int b) {
		int t = a <= b;
		return t*a + (1 - t)*b;
	}

	int max(int a, int b) {
		int t = a >= b;
		return t*a + (1 - t)*b;
	}
}