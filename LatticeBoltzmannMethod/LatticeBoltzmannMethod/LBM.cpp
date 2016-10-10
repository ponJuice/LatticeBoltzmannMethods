#include "LBM.h"
#include "Debug.h"


CLBM::CLBM(LBMInfo lbm)
{
	//�ݒ���R�s�[
	info = lbm;

	//���q�̈ڗ����x�̌v�Z
	as = info.deltaLength / info.deltaTime;

	//�����̌v�Z
	//sos = std::sqrt((info.k*info.r*info.t)/info.m);
	sos = as / sqrt(3.0);

	//�ɘa���Ԃ̌v�Z
	rt = (3.0*info.kvc*info.deltaTime)/(info.deltaLength*info.deltaLength) + 0.5;

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

	//���x�����x�N�g���̃������m��
	e = new CVector3<double>[info.directionNum];
	//���x�����x�N�g���̏�����
	e[0].set(0, 0, 0);
	e[1].set(1, 0, 0);
	e[2].set(0, 1, 0);
	e[3].set(-1, 0, 0);
	e[4].set(0, -1, 0);
	e[5].set(1, 1, 0);
	e[6].set(-1, 1, 0);
	e[7].set(-1, -1, 0);
	e[8].set(1, -1, 0);

	//�������̐ݒ�
	inflow.p = info.pressure;
	inflow.u.copy(info.velocity);
	inflow.distribut = new double[info.directionNum];
	for (int a = 0; a < info.directionNum; a++) {
		inflow.distribut[a] = calcPeq(info.pressure, info.velocity, a);
	}

	//�I�u�W�F�N�g���̐ݒ�
	objectBound.u.set(0, 0, 0);
	objectBound.p = 0;
	objectBound.distribut = new double[info.directionNum];
	for (int a = 0; a < info.directionNum; a++) {
		objectBound.distribut[a] = calcPeq(objectBound.p,&objectBound.u,a);
	}

	//�i�q�_�̃������m��
	point = new Point[info.x*info.y*info.z];
	point_next = new Point[info.x*info.y*info.z];
	for (int n = 0; n < info.x*info.y*info.z; n++) {
		point[n].distribut = new double[info.directionNum];
		point_next[n].distribut = new double[info.directionNum];
		//���́A���x�A���z�֐��̏�����
		init(&point[n]);
	}
	/****************************************/

	/******debug init*******/
	/*int total = 1;
	for (int y = 0; y < info.y; y++) {
		for (int x = 0; x < info.x; x++) {
			int index = info.x * y + x;
			for (int a = 0; a < info.directionNum; a++) {
				DebugDistribut dd;
				dd.id[0] = x;
				dd.id[1] = y;
				dd.id[2] = a;
				point[index].distribut[a] = dd.all;
			}
			point[index].p = total;
			point[index].u.set(total, total, total);
			total++;
		}
	}*/

	/*for (int x = 0; x < info.y; x++) {
		for (int y = 0; y < info.x; y++) {
			int index = info.x * y + x;
			Debug::debugOutputInfo(&point[index]);
		}
	}*/



	//�I�u�W�F�N�g���̏�����
	initData();
}


CLBM::~CLBM()
{
	for (int n = 0; n < info.x*info.y*info.z; n++) {
		delete[] point[n].distribut;
		delete[] point_next[n].distribut;
	}
	delete[] objectBound.distribut;
	delete[] inflow.distribut;
	delete[] point;
	delete[] point_next;
	delete[] e;
	delete[] w;
}

void CLBM::init(CLBM::Point* point) {
	point->p = info.pressure;
	point->u.copy(info.velocity);
	for (int a = 0; a < info.directionNum; a++) {
		point->distribut[a] = calcPeq(point->p, &point->u, a);
	}
}


void CLBM::setValue(int x, int y, int z, int direct, double value, CLBM::ACCESS type) {
	if (x < 0 || x >= info.x || y < 0 || y >= info.y) {
		//�i�q�O���Q�Ɓ��������Ȃ�
		return;
	}
	if (type == ACCESS::NOW)
		point[info.x * info.y * z + info.x * y + x].distribut[direct] = value;
	else
		point_next[info.x * info.y * z + info.x * y + x].distribut[direct] = value;
}
double CLBM::calcPeq(double pressure, CVector<double>* velocity, int a) {
	double e_dot_u = e[a].dot(velocity);		//���x�����x�N�g���Ƒ��x�x�N�g���̓���
	//���t���z�֐�
	double peq = w[a] * (pressure + info.density * (e_dot_u
		+ 1.5 * ((e_dot_u * e_dot_u) / (as * as))
		- 0.5*velocity->dot(velocity)));
	//double peq = w[a] * (1.0 - 1.5 * (velocity->dot(velocity)) + 3.0 * e_dot_u + 4.5 * (e_dot_u * e_dot_u));
	/*double peq = w[a] * pressure * (1.0
		+ 3.0*e_dot_u
		- 1.5*velocity->dot(velocity)
		+ 4.5*e_dot_u*e_dot_u);*/
	return peq;
}

double CLBM::calcPa(double peq,Point* point, int a) {
	//Point* temp = getPoint(x, y, z, ACCESS::NOW);
	//���z�֐�
	double pa = point->distribut[a] - (1.0 / rt)*(point->distribut[a] - peq);
	return pa;
}

CLBM::Point* CLBM::getPoint(int x, int y, int z, CLBM::ACCESS type) {
	if (x < 0 || x >= info.x || y < 0 || y >= info.y) {
		//�i�q�O���Q�Ɓ����E�����Œl���擾
		return boundaryGet(x, y, z, type);
	}
	/*else if (isObject(x, y, z)){
		//�I�u�W�F�N�g��
		return &objectBound;
	
	}*/else if (type == ACCESS::NOW) {
		return &point[info.x * info.y * z + info.x * y + x];
	}
	else {
		return &point_next[info.x * info.y * z + info.x * y + x];

	}

}

void CLBM::calcStep_2() {
	/*�v�Z���@...
	/����i�q�_�̊e�����̒l�����߂邽�߂ɁA
	/���̎���̊i�q�_�̒l�����ɂ���
	/���ׂĂ̕����̒l�����߂���
	/���̓_�ł̋����I���͂Ƌ����I���x���v�Z����*/
	
	//printf("%d\n",stepNum);

	//���ׂĂ̊i�q�_�𑖍�����
	for (int x = 0; x < info.x; x++) {
		for (int y = 0; y < info.y; y++) {
			for (int z = 0; z < info.z; z++) {
				//�����Ɋւ��Čv�Z����
				for (int a = 0; a < info.directionNum; a++) {
					//�܂��v�Z�Ɏg�p����i�q�_���擾����
					int _x = x - e[a].get(CVector3<double>::Dim::X);
					int _y = y - e[a].get(CVector3<double>::Dim::Y);
					int _z = z - e[a].get(CVector3<double>::Dim::Z);
					//printf("a:%d\n \tx:%d _x:%d\n\ty:%d _y:%d\n\tz:%d _z:%d\n",a,x, _x,y, _y,z, _z);
					Point* p = getPoint(_x,_y,_z,ACCESS::NOW);
					Debug::debugOutputCalcInfo(x,y,z,a,_x,_y,_z,p);
					//���t���z�֐����v�Z����
					double peq = 0;
					double pa = 0;
					if (p != nullptr) {
						peq = calcPeq(p->p, &p->u, a);
						//���z�֐����v�Z����
						pa = calcPa(peq,p, a);
						//debug
						//pa = getPoint(x, y, z, ACCESS::NOW)->distribut[a];
					}
					else {
						pa = getPoint(x, y, z, ACCESS::NOW)->distribut[invertVelocity(a)];
					}
					//printf("peq %d : %f  pa : %f",a, peq,pa);
					//printf("\n");
					//���̎��ԂɊi�[
					setValue(x, y, z, a, pa, ACCESS::NEXT);
				}
				//���ׂĂ̕����̕��z�֐������܂�����A�����I���x�Ƌ����I���x���v�Z
				calcPressAndVelocity(x, y, z,ACCESS::NEXT);
			}
			//printf("\n");
		}
	}
	//�Q�Ƃ����݂ɓ���ւ��邱�ƂŎ��Ԃ�i�܂���
	Point* p_temp = point;
	point = point_next;
	point_next = p_temp;

	stepNum++;
}

void CLBM::initData() {

	//���ׂĂ̊i�q�_�𑖍�����
	for (int x = 0; x < info.x; x++) {
		for (int y = 0; y < info.y; y++) {
			for (int z = 0; z < info.z; z++) {
				//�����Ɋւ��Čv�Z����
				for (int a = 0; a < info.directionNum; a++) {
					//�܂��v�Z�Ɏg�p����i�q�_���擾����
					int _x = x - e[a].get(CVector3<double>::Dim::X);
					int _y = y - e[a].get(CVector3<double>::Dim::Y);
					int _z = z - e[a].get(CVector3<double>::Dim::Z);
					//printf("a:%d\n \tx:%d _x:%d\n\ty:%d _y:%d\n\tz:%d _z:%d\n",a,x, _x,y, _y,z, _z);
					Point* p = getPoint(_x, _y, _z, ACCESS::NOW);
					//���t���z�֐����v�Z����
					double peq = 0;
					double pa = 0;
					if (p != nullptr) {
						peq = calcPeq(p->p, &p->u, a);
						//���z�֐����v�Z����
						pa = calcPa(peq, p, a);
					}
					else {
						pa = getPoint(x, y, z, ACCESS::NOW)->distribut[invertVelocity(a)];
					}
					//���̎��ԂɊi�[
					setValue(x, y, z, a, pa, ACCESS::NEXT);
				}
				//���ׂĂ̕����̕��z�֐������܂�����A�����I���x�Ƌ����I���x���v�Z
				calcPressAndVelocity(x, y, z, ACCESS::NEXT);
			}
			//printf("\n");
		}
	}
	for (int n = 0; n < info.x * info.y * info.z; n++) {
		point_next[n].p = point[n].p;
		point_next[n].u.copy(&point[n].u);
		for (int a = 0; a < info.directionNum; a++) {
			point_next[n].distribut[a] = point[n].distribut[a];
		}
	}
}

void CLBM::calcPressAndVelocity(int x, int y, int z,CLBM::ACCESS type) {
	Point* p = getPoint(x, y, z, type);
	CVector3<double> temp;
	p->p = 0;
	p->u.set(0, 0, 0);
	//���́A���x�̌v�Z
	for (int a = 0; a < info.directionNum; a++) {
		//�����I���͂̌v�Z
		p->p += p->distribut[a];
		//�����I���x�̌v�Z
		e[a].mult(p->distribut[a], &temp);
		p->u.add(&temp);
	}
	//�����I���x�̌v�Z
	p->u.div(info.density*(sos*sos), &(p->u));
}

void CLBM::calcStep() {
	double p = 0;
	CVector3<double> u,temp;
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
				//�����I���x�̌v�Z
				u.div(info.density*sos, &u);
				//���z�֐��A���t���z�֐��̌v�Z
				for (int a = 0; a < info.directionNum; a++) {
					double peq = calcPeq(p, &u, a);
					//���t�֐�
					double pa = calcPa(peq, getPoint(x,y,z,NOW) ,a);
					//���̎��Ԃɒl����
					setValue(x + e[a].get(CVector3<int>::Dim::X),
						y + e[a].get(CVector3<int>::Dim::Y),
						z + e[a].get(CVector3<int>::Dim::Z),
						a,
						peq,
						ACCESS::NEXT);
				}
			}
		}
	}
	//�Q�Ƃ����݂ɓ���ւ��邱�ƂŎ��Ԃ�i�܂���
	Point* p_temp = point;
	point = point_next;
	point_next = p_temp;

	stepNum++;
}

int CLBM::invertVelocity(int a) {
	if (a == 0)
		return 0;
	int t = a - 2;
	if (a <= 4) {
		if (t <= 0) {
			return a + 2;
		}
		else {
			return a - 2;
		}
	}
	else if (a <= 8) {
		if (t <= 4) {
			return a + 2;
		}
		else {
			return a - 2;
		}
	}
}

CLBM::Point* CLBM::boundaryGet(int x, int y, int z,CLBM::ACCESS type) {
	int _x = x, _y = y, _z = z;
	//���E�͎������E
	if (x < 0 || x >= info.x) {
		_x = info.x - abs(x);
		//return boundaryGet(_x, y, z, type);
	}
	if (y < 0 || y >= info.y) {
		//return nullptr;
		//�e�X�g�@�������E
		_y = info.y - abs(y);
		//return boundaryGet(x, _y, z, type);
	}
	/*else {
		return getPoint(x, y, z,type);
	}*/
	return getPoint(_x, _y, z, type);
}

CLBM::Point * CLBM::objectBoundyGet(int x, int y, int z, ACCESS type)
{
	return &objectBound;
}

bool CLBM::isObject(int x, int y, int z)
{
	//return false;
	/*double centorX = info.x/2.0, centorY = info.y/2.0, centorZ = 1;
	double lengthX = (double)x - centorX;
	double lengthY = (double)y - centorY;
	double lengthZ = (double)z - centorZ;*/
	int cx = info.x / 2;
	int cy = info.y / 2;
	return x >= (cx - 10) && x <= (cx + 10)
		&& y >= (cy - 10) && y <= (cy + 10);
}
