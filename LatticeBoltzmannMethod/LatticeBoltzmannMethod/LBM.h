#pragma once
class CLBM
{
public:
	typedef struct {
		int x, y , z;			//�i�q�_��
		int directionNum;		//���x�x�N�g���̐�
		double deltaTime;		//���ԍ���
		double relaxationTime;	//�ɘa����
	}LBMInfo;
	LBMInfo info;
	int velocityNum;			//���x�̐�
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

