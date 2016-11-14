#pragma once

namespace LBM{
	typedef struct IVector3{
		int x,y,z;
	}IVector3;

	typedef struct DVector3{
		double x,y,z;
	}DVector3;

	typedef struct LBMPoint{
		double* a;
		double density;
		double vx,vy,vz;
	}LBMPoint;

	typedef struct LatticeInfo{
		int offset;		//最初の計算点までのオフセット
		int size;		//計算点の数
		int x_max_out;	//参照領域を含めたx方向の格子数
		int y_max_out;	//参照領域を含めたy方向の格子数
		int z_max_out;	//参照領域を含めたz方向の格子数
		int x_max_in;	//計算領域のx方向の格子数
		int y_max_in;	//計算領域のy方向の格子数
		int z_max_in;	//計算領域のz方向の格子数
		int direct_num;	//速度方向の数
		int max_speed;	//速度の最大
	}LatticeInfo;
}