#pragma once
#include "vector3.h"
#include <math.h>

class CLBM
{
public:
	enum ACCESS {
		NOW,
		NEXT
	};

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
		CVector3<double>* velocity;		//初期速度
		double cld;				//代表長さ
		double cv;				//代表速度
		double re;				//レイノルズ数
	}LBMInfo;

	typedef struct {
		double* distribut;	//方向の分布関数値
		double p;			//巨視的圧力
		CVector3<double> u;			//巨視的速度
	}Point;

	LBMInfo info;
	double rt;				//緩和時間
	double as;				//粒子の移流速度
	double sos;				//音速
	Point inflow;			//流入
	Point objectBound;		//オブジェクトの境界用
	Point* point;			//格子点上の値
	Point* point_next;		//次の時間の値
	CVector3<double>* e;	//速度方向ベクトル
	double* w;				//係数
	int stepNum = 0;		//ステップ数
public:
	CLBM(LBMInfo lbm);
	~CLBM();
	void init(Point* point);
	double getValue(int x, int y ,int z,int direct,ACCESS type);
	void setValue(int x, int y ,int z,int direct,double value,ACCESS type);
	Point* getPoint(int x, int y, int z,ACCESS type);
	//平衡分布関数
	double calcPeq(double pressure, CVector<double>* velocity,int a);
	//分布関数
	double calcPa(double peq,int x,int y,int z,int a);
	//巨視的圧力と巨視的速度の計算、設定
	void calcPressAndVelocity(int x, int y, int z,ACCESS type);
	//1ステップ計算
	void calcStep();
	//1ステップ計算
	void calcStep_2();
	//境界条件
	double boundaryGet(int x, int y, int z, int a, ACCESS type);
	Point* boundaryGet(int x, int y, int z, ACCESS type);
	//オブジェクトの境界条件
	Point* objectBoundyGet(int x, int y, int z, ACCESS type);
	//オブジェクトの境界条件が必要か
	bool isObject(int x, int y, int z);
	//離散速度を反転
	int invertVelocity(int a);
};

