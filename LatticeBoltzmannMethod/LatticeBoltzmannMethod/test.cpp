#pragma once
#include "LBM.h"
#include "LBMFileManager.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sstream>
#include <iostream>
#include <future>

void lbmThread();

volatile bool stop;
std::mutex mtx;

void dispPoint(CLBM::Point* point){
	printf("dist\n");
	for (int n = 0; n < 9; n++) {
		printf("\t%d:%f\n", n, point->distribut[n]);
	}
	printf("p:%f\n",point->p);
	printf("vect:(%f,%f,%f)]\n",point->u.get(0), point->u.get(1), point->u.get(1));
}

void main() {

	/*CVector3<double> vel;
	vel.set(0.1, 0, 0);

	CLBM::LBMInfo info;
	info.x = 2;
	info.y = 2;
	info.z = 1;
	info.directionNum = 9;
	info.cld = 0.5;	//0.1[m]
	info.cv = 0.00;	//1 [m/s]
	info.deltaLength = info.cld / info.x;
	info.deltaTime = 1.0e-6;
	info.density = 1.205;
	info.velocity = &vel;
	info.pressure = 1.0 / 3.0;
	info.re = 400.0;
	info.kvc = 1.512e-5;
	CLBM lbm(info);

	//CLBM::Point* p = lbm.getPoint(50, -1, 0, CLBM::ACCESS::NOW);
	lbm.calcStep_2();
	//dispPoint(p);

	getchar();

	return;*/

	stop = false;

	auto thread = std::thread([] {lbmThread(); });
	getchar();
	
	mtx.lock();
	stop = true;
	mtx.unlock();

	thread.join();
}

void lbmThread() {
	CLBMFileManager fm;
	stringstream press[13];
	stringstream velocity[13];

	for (int n = 0; n < 13; n++) {
		press[n] << "press_" << n << ".dat";
		velocity[n] << "velocity_" << n << ".dat";
		fm.openFile(&press[n].str(), CGnuplotFileManager::MODE::NEW);
		fm.openFile(&velocity[n].str(), CGnuplotFileManager::MODE::NEW);
	}
	CVector3<double> vel;
	vel.set(1, 0, 0);

	CLBM::LBMInfo info;
	info.x = 500;
	info.y = 500;
	info.z = 1;
	info.directionNum = 9;
	info.cld = 1.0;	//0.1[m]
	info.cv = 1.0;	//1 [m/s]
	info.deltaLength = 0.002;
	info.deltaTime = 1.0e-4;
	info.density = 1.205;
	info.velocity = &vel;
	info.pressure = 0.327308;
	info.re = 400.0;
	info.kvc = 1.512e-5;
	CLBM lbm(info);

	

	printf("初期化終了\n");
	int index = 0;
	bool _stop;
	for (int n = 0; n < 13; n++) {
		
		/*排他制御*/
		mtx.lock();
		_stop = stop;
		mtx.unlock();
		
		if (_stop)
			break;
		/*排他制御*/

		//if (n % (360 / 12) == 0) {
			fm.writeColorMap(&press[index].str(), &lbm, info, CLBMFileManager::TYPE::PRESSURE);
			fm.writeColorMap(&velocity[index].str(), &lbm, info, CLBMFileManager::TYPE::VELOCITY);
			printf("%dステップ終了(%d)\n", n,index);
			index++;
		//}
		lbm.calcStep_2();
	}

	printf("終了します\n");
}