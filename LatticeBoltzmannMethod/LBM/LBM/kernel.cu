#pragma once
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "lbm_kernels.cuh"
#include <stdio.h>
#include <math.h>

#include "lbm_data.h"
#include "lbm_util.h"

int main(){
	cudaError ce;

	int device_num = 0;	//TITANÇégóp
	cudaDeviceProp dev;
	cudaGetDeviceProperties(&dev,device_num);

	//åvéZóÃàÊÇÃèÓïÒ
	LBM::LatticeInfo info;
	LBM::getLbmLatticeInfo(&info,1,4,4,4,15);
	LBM::printLatticeInfo(&info);

	int threads = Math::min(dev.maxThreadsPerBlock,info.size);//32 * ((info.size - 1) / 32 + 1);
	int block = (int)ceil((double)threads/(double)dev.maxThreadsPerBlock);

	printf("threads : %d\n",threads);
	printf("block : %d\n",block);

	int a_array_size = sizeof(double) * info.direct_num;
	int point_size = 36;//sizeof(LBM::LBMPoint);

	int a = sizeof(double*);

	printf("a_size : %d\n",a_array_size);
	printf("point size : %d\n",point_size);

	LBM::LBMPoint* d_point;
	LBM::LBMPoint h_point;
	LBM::LatticeInfo* d_info;

	ce = cudaMalloc((void**)&d_point,point_size*info.x_max_out*info.y_max_out*info.z_max_out);
	if(ce != cudaSuccess){
		printf("failed cudaMalloc <<point>>\n");
		printf("error : %s\n",cudaGetErrorString(ce));
		goto allCudaFree;
	}
	for(int n = 0;n < info.size;n++){
		double* _a;
		ce = cudaMalloc((void**)&_a,a_array_size);
		//ce = cudaMalloc((void**)&d_point[n].a,a_size * info.direct_num);
		if(ce != cudaSuccess){
			printf("failed cudaMalloc <<point.a>>\n");
			printf("error : %s\n",cudaGetErrorString(ce));
			goto allCudaFree;
		}
		h_point.a = _a;
		d_point[n].a = _a;
		ce = cudaMemcpy(&d_point[n],&h_point,point_size,cudaMemcpyHostToDevice);
		if(ce != cudaSuccess){
			printf("failed cudaMemcpy <<point.a>>\n");
			printf("error : %s\n",cudaGetErrorString(ce));
			goto allCudaFree;
		}
	}

	int info_size = sizeof(LBM::LatticeInfo);
	ce = cudaMalloc((void**)&d_info,info_size);
	if(ce != cudaSuccess){
		printf("failed cudaMalloc <<info>>\n");
		printf("error : %s\n",cudaGetErrorString(ce));
		goto allCudaFree;
	}

	ce = cudaMemcpy(d_info,&info,info_size,cudaMemcpyHostToDevice);	
	if(ce != cudaSuccess){
		printf("failed cudaMemcpy <<info>>\n");
		printf("error : %s\n",cudaGetErrorString(ce));
		goto allCudaFree;
	}

	lbm_calc<<<block,threads>>>(d_info,d_point);
	cudaThreadSynchronize();
	ce = cudaGetLastError();
	if(ce != cudaSuccess){
		printf("failed karnel <<lbm_calc>>\n");
		printf("error : %s\n",cudaGetErrorString(ce));
		goto allCudaFree;
	}
	

allCudaFree:
	for(int n = 0;n < info.size;n++){
		ce = cudaFree(d_point[n].a);
		if(ce != cudaSuccess){
			printf("failed cudaFree <<point.a>>\n");
			printf("error : %s\n",cudaGetErrorString(ce));
		}
	}
	ce = cudaFree(d_point);
	if(ce != cudaSuccess){
		printf("failed cudaFree <<point>>\n");
		printf("error : %s\n",cudaGetErrorString(ce));
	}
	ce = cudaFree(d_info);
	if(ce != cudaSuccess){
		printf("failed cudaFree <<info>>\n");
		printf("error : %s\n",cudaGetErrorString(ce));
	}
	cudaDeviceReset();

	return 0;
}