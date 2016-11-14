#pragma once

namespace LBM{
	typedef struct IVector3{
		int x,y,z;
	}IVector3;

	typedef struct DVector3{
		double x,y,z;
	}DVector3;

	typedef struct LBMPoint{
		double* a;
		double density;
		double vx,vy,vz;
	}LBMPoint;

	typedef struct LatticeInfo{
		int offset;		//�ŏ��̌v�Z�_�܂ł̃I�t�Z�b�g
		int size;		//�v�Z�_�̐�
		int x_max_out;	//�Q�Ɨ̈���܂߂�x�����̊i�q��
		int y_max_out;	//�Q�Ɨ̈���܂߂�y�����̊i�q��
		int z_max_out;	//�Q�Ɨ̈���܂߂�z�����̊i�q��
		int x_max_in;	//�v�Z�̈��x�����̊i�q��
		int y_max_in;	//�v�Z�̈��y�����̊i�q��
		int z_max_in;	//�v�Z�̈��z�����̊i�q��
		int direct_num;	//���x�����̐�
		int max_speed;	//���x�̍ő�
	}LatticeInfo;
}