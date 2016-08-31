#pragma once
class CLBM
{
public:
	typedef struct {
		int x, y , z;			//格子点数
		int directionNum;		//速度ベクトルの数
		double deltaTime;		//時間刻み
		double relaxationTime;	//緩和時間
	}LBMInfo;
	LBMInfo info;
	int velocityNum;			//速度の数
	double* velocity;
	double** direction;
public:
	CLBM(LBMInfo lbm);
	~CLBM();
	double getValue(int x, int y ,int z,int direct);
	void setValue(int x, int y ,int z,int direct,double value);
	void calcStep();
	double particleDistributionFunction(int direct, double time, int x, int y, int z);
	double equilibriumParticleProbabilityDistributionFunction(double time,int x,int y,int z);
};

