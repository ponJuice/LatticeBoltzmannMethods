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
		CVector3<double>* velocity;		//�������x
		double cld;				//��\����
		double cv;				//��\���x
		double re;				//���C�m���Y��
	}LBMInfo;

	typedef struct {
		double* distribut;	//�����̕��z�֐��l
		double p;			//�����I����
		CVector3<double> u;			//�����I���x
	}Point;

	LBMInfo info;
	double rt;				//�ɘa����
	double as;				//���q�̈ڗ����x
	double sos;				//����
	Point inflow;			//����
	Point objectBound;		//�I�u�W�F�N�g�̋��E�p
	Point* point;			//�i�q�_��̒l
	Point* point_next;		//���̎��Ԃ̒l
	CVector3<double>* e;	//���x�����x�N�g��
	double* w;				//�W��
	int stepNum = 0;		//�X�e�b�v��
public:
	CLBM(LBMInfo lbm);
	~CLBM();
	void init(Point* point);
	double getValue(int x, int y ,int z,int direct,ACCESS type);
	void setValue(int x, int y ,int z,int direct,double value,ACCESS type);
	Point* getPoint(int x, int y, int z,ACCESS type);
	//���t���z�֐�
	double calcPeq(double pressure, CVector<double>* velocity,int a);
	//���z�֐�
	double calcPa(double peq,int x,int y,int z,int a);
	//�����I���͂Ƌ����I���x�̌v�Z�A�ݒ�
	void calcPressAndVelocity(int x, int y, int z,ACCESS type);
	//1�X�e�b�v�v�Z
	void calcStep();
	//1�X�e�b�v�v�Z
	void calcStep_2();
	//���E����
	double boundaryGet(int x, int y, int z, int a, ACCESS type);
	Point* boundaryGet(int x, int y, int z, ACCESS type);
	//�I�u�W�F�N�g�̋��E����
	Point* objectBoundyGet(int x, int y, int z, ACCESS type);
	//�I�u�W�F�N�g�̋��E�������K�v��
	bool isObject(int x, int y, int z);
	//���U���x�𔽓]
	int invertVelocity(int a);
};

