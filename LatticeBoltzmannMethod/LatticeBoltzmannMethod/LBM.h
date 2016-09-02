#pragma once
#include "Vector.h"
#include <math.h>

class CLBM
{
public:
	typedef struct {
		int x, y , z;			//格子点数
		int directionNum;		//速度ベクトルの数
		double k;				//気体の比熱比
		double r;				//気体定数
		double t;				//気体温度
		double m;				//気体の平均分子量
		double deltaTime;		//時間刻み
		double deltaLength;		//空間刻み
		double pressure;		//初期圧力
		double density;			//初期密度
		double cld;				//代表長さ
		double cv;				//代表速度
		double re;				//レイノルズ数
	}LBMInfo;

	typedef struct {
		double* distribut;
	}Point;

	LBMInfo info;
	double rt;				//緩和時間
	double as;				//粒子の移流速度
	double sos;				//音速
	Point* point;				//格子点上の値
	CVector3* e;				//速度方向ベクトル
	double* w;					//係数
	int stepNum = 0;			//ステップ数
public:
	CLBM(LBMInfo lbm);
	~CLBM();
	double getValue(int x, int y ,int z,int direct);
	void setValue(int x, int y ,int z,int direct,double value);
	void calcStep();
};

