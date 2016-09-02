#pragma once
#include "Vector.h"
#include <math.h>

class CLBM
{
public:
	typedef struct {
		int x, y , z;			//�i�q�_��
		int directionNum;		//���x�x�N�g���̐�
		double k;				//�C�̂̔�M��
		double r;				//�C�̒萔
		double t;				//�C�̉��x
		double m;				//�C�̂̕��ϕ��q��
		double deltaTime;		//���ԍ���
		double deltaLength;		//��ԍ���
		double pressure;		//��������
		double density;			//�������x
		double cld;				//��\����
		double cv;				//��\���x
		double re;				//���C�m���Y��
	}LBMInfo;

	typedef struct {
		double* distribut;
	}Point;

	LBMInfo info;
	double rt;				//�ɘa����
	double as;				//���q�̈ڗ����x
	double sos;				//����
	Point* point;				//�i�q�_��̒l
	CVector3* e;				//���x�����x�N�g��
	double* w;					//�W��
	int stepNum = 0;			//�X�e�b�v��
public:
	CLBM(LBMInfo lbm);
	~CLBM();
	double getValue(int x, int y ,int z,int direct);
	void setValue(int x, int y ,int z,int direct,double value);
	void calcStep();
};

