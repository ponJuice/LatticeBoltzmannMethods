#pragma once
#include "LBM.h"
#include "LBMFileManager.h"
#include <stdio.h>
#include <sstream>
#include <iostream>
#include <future>

void lbmThread();

volatile bool stop;
std::mutex mtx;

void main() {
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
	vel.set(0.6, 0, 0);

	CLBM::LBMInfo info;
	info.x = 256;
	info.y = 256;
	info.z = 1;
	info.directionNum = 9;
	info.cld = 0.5;	//0.1[m]
	info.cv = 1.0;	//1 [m/s]
	info.deltaLength = info.cld / info.x;
	info.deltaTime = 1.0 / 100.0;
	info.density = 1.185;
	info.velocity = &vel;
	info.pressure = 1.0 / 3.0;
	info.re = 10.0;
	CLBM lbm(info);

	printf("初期化終了\n");
	int index = 0;
	bool _stop;
	for (int n = 0; n <= 360; n++) {
		
		/*排他制御*/
		mtx.lock();
		_stop = stop;
		mtx.unlock();

		if (_stop)
			break;
		/*排他制御*/

		if (n % (360 / 12) == 0) {
			fm.writeColorMap(&press[index].str(), &lbm, info, CLBMFileManager::TYPE::PRESSURE);
			fm.writeColorMap(&velocity[index].str(), &lbm, info, CLBMFileManager::TYPE::VELOCITY);
			printf("%dステップ終了(%d)\n", n,index);
			index++;
		}
		lbm.calcStep_2();
	}

	printf("終了します\n");
}