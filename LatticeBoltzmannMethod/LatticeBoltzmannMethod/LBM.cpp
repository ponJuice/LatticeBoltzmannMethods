#include "LBM.h"



CLBM::CLBM(LBMInfo lbm)
{
	//�ݒ���R�s�[
	info = lbm;

	//�����̌v�Z
	sos = std::sqrt((info.k*info.r*info.t)/info.m);

	//���q�̈ڗ����x�̌v�Z
	as = info.deltaLength / info.deltaTime;

	//�ɘa���Ԃ̌v�Z
	double kvc = (info.cv*info.cld) / info.re;		//���S���W��
	rt = (3.0*kvc) / (as*info.deltaLength) + 0.5;

	//�W���̐ݒ�
	w = new double[info.directionNum];
	w[0] = 4.0 / 9.0;
	w[1] = 1.0 / 9.0;
	w[2] = w[1];
	w[3] = w[1];
	w[4] = w[1];
	w[5] = 1.0 / 36.0;
	w[6] = w[5];
	w[7] = w[5];
	w[8] = w[5];

	//�i�q�_�̃������m��
	point = new Point[info.x*info.y*info.z];
	for (int n = 0; n < info.x*info.y*info.z; n++) {
		point[n].distribut = new double[info.directionNum];
		for (int a = 0; a < info.directionNum; a++) {
			point[n].distribut[a] = 0;
		}
	}
	//���x�A���͂̏����l�ݒ�

	//���x�����x�N�g���̃������m��
	e = new CVector3[info.directionNum];
	//���x�����x�N�g���̏�����
	//e[0] = 
}


CLBM::~CLBM()
{

}

double CLBM::getValue(int x, int y, int z,int direct) {
	return point[info.x * info.y * z + info.x * y + x].distribut[direct];
}

void CLBM::setValue(int x, int y, int z,int direct,double value) {
	point[info.x * info.y * z + info.x * y + x].distribut[direct] = value;
}

void CLBM::calcStep() {
	double p = 0;
	CVector3 u,temp;
	for (int x = 0; x < info.x; x++) {
		for (int y = 0; y < info.y; y++) {
			for (int z = 0; z < info.z; z++) {
				//���́A���x�̌v�Z
				for (int a = 0; a < info.directionNum; a++) {
					//�����I���͂̌v�Z
					p += point[x, y, z].distribut[a];
					//�����I���x�̌v�Z
					e[a].mult(point[x, y, z].distribut[a], &temp);
					u.add(&temp, &u);
				}
				u.div(info.density*sos, &u);
				//���z�֐��A���t���z�֐��̌v�Z
				for (int a = 0; a < info.directionNum; a++) {
					//���t���z�֐�
					double e_dot_u = e[a].dot(&u);		//���x�����x�N�g���Ƒ��x�x�N�g���̓���
					double peq = w[a] * (p + info.density * (e_dot_u 
								+ 1.5 * ((e_dot_u * e_dot_u) / (as * as))
							- 0.5*u.dot(&u)));
					//���t�֐�
					double pa = point[x, y, z].distribut[a]
						- (1.0 / rt)*(point[x,y,z].distribut[a]-peq);
				}
			}
		}
	}
}