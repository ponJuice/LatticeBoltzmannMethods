#pragma once
#include <stdio.h>
#include "LBM.h"
#include "vector3.h"
//#define LATTICE
//#define BOUNDARY
//#define POINT

union DebugDistribut {
	char id[3];
	double all;
};

namespace Debug {
	void debugOutputInfo(CLBM::Point* p) {
		for (int n = 0; n < 9; n++) {
			DebugDistribut dd;
			dd.all = p->distribut[n];
			printf("x : %d | ", dd.id[0]);
			printf("y : %d | ", dd.id[1]);
			printf("a : %d\n", dd.id[2]);
		}
		printf("d : %f\n", p->density);
		printf("v : %f , %f , %f"
			, p->velocity.get(CVector3<int>::Dim::X)
			, p->velocity.get(CVector3<int>::Dim::Y)
			, p->velocity.get(CVector3<int>::Dim::Z));
		printf("\n");
		printf("\n");
	}

	void debugOutputLatticeInfo(int x, int y, int z, int a, int _x, int _y, int _z) {
#ifdef LATTICE
		printf(" --- lattice info ---\n");
		printf(" x : %2d | ", x);
		printf(" y : %2d | ", y);
		printf(" z : %2d | ", z);
		printf(" a : %2d |\n", a);
		printf("_x : %2d | ", _x);
		printf("_y : %2d | ", _y);
		printf("_z : %2d |\n", _z);
#endif
	}

	void debugOutputBoundaryInfo(int x, int y, int z, int _x, int _y, int _z) {
#ifdef BOUNDARY
		printf(" --- boundary info ---\n");
		printf(" x : %2d | ", x);
		printf(" y : %2d | ", y);
		printf(" z : %2d | \n", z);
		printf("_x : %2d | ", _x);
		printf("_y : %2d | ", _y);
		printf("_z : %2d |\n", _z);
#endif
	}

	void debugOutputPointInfo(CLBM::Point* p) {
#ifdef POINT
		printf(" --- point info ----\n");
		printf("d : %f\n", p->density);
		printf("v : %f , %f , %f"
			, p->velocity.get(CVector3<int>::Dim::X)
			, p->velocity.get(CVector3<int>::Dim::Y)
			, p->velocity.get(CVector3<int>::Dim::Z));
		printf("\n");
#endif
	}

	void debugOutputCalcInfo(int x, int y, int z, int a, int _x, int _y, int _z, CLBM::Point* p) {
		printf(" x : %2d | ", x);
		printf(" y : %2d | ", y);
		printf(" z : %2d | ", z);
		printf(" a : %2d |\n", a);
		printf("_x : %2d | ", _x);
		printf("_y : %2d | ", _y);
		printf("_z : %2d |\n", _z);
		DebugDistribut dd;
		dd.all = p->distribut[a];
		printf("p : %f\n", p->density);
		printf("v : %f , %f , %f"
			, p->velocity.get(CVector3<int>::Dim::X)
			, p->velocity.get(CVector3<int>::Dim::Y)
			, p->velocity.get(CVector3<int>::Dim::Z));
		printf("\n");
		printf("\n");

	}
}