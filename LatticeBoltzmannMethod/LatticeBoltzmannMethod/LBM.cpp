#include "LBM.h"
#include "Debug.h"


CLBM::CLBM(LBMInfo lbm)
{
	//設定をコピー
	info = lbm;

	//粒子の移流速度の計算
	as = info.deltaLength / info.deltaTime;

	//音速の計算
	//sos = std::sqrt((info.k*info.r*info.t)/info.m);
	sos = as / sqrt(3.0);

	//緩和時間の計算
	rt = (3.0*info.kvc*info.deltaTime)/(info.deltaLength*info.deltaLength) + 0.5;

	//係数の設定
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

	//速度方向ベクトルのメモリ確保
	e = new CVector3<double>[info.directionNum];
	//速度方向ベクトルの初期化
	e[0].set(0, 0, 0);
	e[1].set(1, 0, 0);
	e[2].set(0, 1, 0);
	e[3].set(-1, 0, 0);
	e[4].set(0, -1, 0);
	e[5].set(1, 1, 0);
	e[6].set(-1, 1, 0);
	e[7].set(-1, -1, 0);
	e[8].set(1, -1, 0);

	//流入口の設定
	inflow.p = info.pressure;
	inflow.u.copy(info.velocity);
	inflow.distribut = new double[info.directionNum];
	for (int a = 0; a < info.directionNum; a++) {
		inflow.distribut[a] = calcPeq(info.pressure, info.velocity, a);
	}

	//オブジェクト内の設定
	objectBound.u.set(0, 0, 0);
	objectBound.p = 0;
	objectBound.distribut = new double[info.directionNum];
	for (int a = 0; a < info.directionNum; a++) {
		objectBound.distribut[a] = calcPeq(objectBound.p,&objectBound.u,a);
	}

	//格子点のメモリ確保
	point = new Point[info.x*info.y*info.z];
	point_next = new Point[info.x*info.y*info.z];
	for (int n = 0; n < info.x*info.y*info.z; n++) {
		point[n].distribut = new double[info.directionNum];
		point_next[n].distribut = new double[info.directionNum];
		//圧力、速度、分布関数の初期化
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



	//オブジェクト内の初期化
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
		//格子外を参照→何もしない
		return;
	}
	if (type == ACCESS::NOW)
		point[info.x * info.y * z + info.x * y + x].distribut[direct] = value;
	else
		point_next[info.x * info.y * z + info.x * y + x].distribut[direct] = value;
}
double CLBM::calcPeq(double pressure, CVector<double>* velocity, int a) {
	double e_dot_u = e[a].dot(velocity);		//速度方向ベクトルと速度ベクトルの内積
	//平衡分布関数
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
	//分布関数
	double pa = point->distribut[a] - (1.0 / rt)*(point->distribut[a] - peq);
	return pa;
}

CLBM::Point* CLBM::getPoint(int x, int y, int z, CLBM::ACCESS type) {
	if (x < 0 || x >= info.x || y < 0 || y >= info.y) {
		//格子外を参照→境界条件で値を取得
		return boundaryGet(x, y, z, type);
	}
	/*else if (isObject(x, y, z)){
		//オブジェクト内
		return &objectBound;
	
	}*/else if (type == ACCESS::NOW) {
		return &point[info.x * info.y * z + info.x * y + x];
	}
	else {
		return &point_next[info.x * info.y * z + info.x * y + x];

	}

}

void CLBM::calcStep_2() {
	/*計算方法...
	/ある格子点の各方向の値を求めるために、
	/その周りの格子点の値を元にする
	/すべての方向の値を求めたら
	/その点での巨視的圧力と巨視的速度を計算する*/
	
	//printf("%d\n",stepNum);

	//すべての格子点を走査する
	for (int x = 0; x < info.x; x++) {
		for (int y = 0; y < info.y; y++) {
			for (int z = 0; z < info.z; z++) {
				//方向に関して計算する
				for (int a = 0; a < info.directionNum; a++) {
					//まず計算に使用する格子点を取得する
					int _x = x - e[a].get(CVector3<double>::Dim::X);
					int _y = y - e[a].get(CVector3<double>::Dim::Y);
					int _z = z - e[a].get(CVector3<double>::Dim::Z);
					//printf("a:%d\n \tx:%d _x:%d\n\ty:%d _y:%d\n\tz:%d _z:%d\n",a,x, _x,y, _y,z, _z);
					Point* p = getPoint(_x,_y,_z,ACCESS::NOW);
					Debug::debugOutputCalcInfo(x,y,z,a,_x,_y,_z,p);
					//平衡分布関数を計算する
					double peq = 0;
					double pa = 0;
					if (p != nullptr) {
						peq = calcPeq(p->p, &p->u, a);
						//分布関数を計算する
						pa = calcPa(peq,p, a);
						//debug
						//pa = getPoint(x, y, z, ACCESS::NOW)->distribut[a];
					}
					else {
						pa = getPoint(x, y, z, ACCESS::NOW)->distribut[invertVelocity(a)];
					}
					//printf("peq %d : %f  pa : %f",a, peq,pa);
					//printf("\n");
					//次の時間に格納
					setValue(x, y, z, a, pa, ACCESS::NEXT);
				}
				//すべての方向の分布関数が求まったら、巨視的速度と巨視的密度を計算
				calcPressAndVelocity(x, y, z,ACCESS::NEXT);
			}
			//printf("\n");
		}
	}
	//参照を交互に入れ替えることで時間を進ませる
	Point* p_temp = point;
	point = point_next;
	point_next = p_temp;

	stepNum++;
}

void CLBM::initData() {

	//すべての格子点を走査する
	for (int x = 0; x < info.x; x++) {
		for (int y = 0; y < info.y; y++) {
			for (int z = 0; z < info.z; z++) {
				//方向に関して計算する
				for (int a = 0; a < info.directionNum; a++) {
					//まず計算に使用する格子点を取得する
					int _x = x - e[a].get(CVector3<double>::Dim::X);
					int _y = y - e[a].get(CVector3<double>::Dim::Y);
					int _z = z - e[a].get(CVector3<double>::Dim::Z);
					//printf("a:%d\n \tx:%d _x:%d\n\ty:%d _y:%d\n\tz:%d _z:%d\n",a,x, _x,y, _y,z, _z);
					Point* p = getPoint(_x, _y, _z, ACCESS::NOW);
					//平衡分布関数を計算する
					double peq = 0;
					double pa = 0;
					if (p != nullptr) {
						peq = calcPeq(p->p, &p->u, a);
						//分布関数を計算する
						pa = calcPa(peq, p, a);
					}
					else {
						pa = getPoint(x, y, z, ACCESS::NOW)->distribut[invertVelocity(a)];
					}
					//次の時間に格納
					setValue(x, y, z, a, pa, ACCESS::NEXT);
				}
				//すべての方向の分布関数が求まったら、巨視的速度と巨視的密度を計算
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
	//圧力、速度の計算
	for (int a = 0; a < info.directionNum; a++) {
		//巨視的圧力の計算
		p->p += p->distribut[a];
		//巨視的速度の計算
		e[a].mult(p->distribut[a], &temp);
		p->u.add(&temp);
	}
	//巨視的速度の計算
	p->u.div(info.density*(sos*sos), &(p->u));
}

void CLBM::calcStep() {
	double p = 0;
	CVector3<double> u,temp;
	for (int x = 0; x < info.x; x++) {
		for (int y = 0; y < info.y; y++) {
			for (int z = 0; z < info.z; z++) {
				//圧力、速度の計算
				for (int a = 0; a < info.directionNum; a++) {
					//巨視的圧力の計算
					p += point[x, y, z].distribut[a];
					//巨視的速度の計算
					e[a].mult(point[x, y, z].distribut[a], &temp);
					u.add(&temp, &u);
				}
				//巨視的速度の計算
				u.div(info.density*sos, &u);
				//分布関数、平衡分布関数の計算
				for (int a = 0; a < info.directionNum; a++) {
					double peq = calcPeq(p, &u, a);
					//平衡関数
					double pa = calcPa(peq, getPoint(x,y,z,NOW) ,a);
					//次の時間に値を代入
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
	//参照を交互に入れ替えることで時間を進ませる
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
	//左右は周期境界
	if (x < 0 || x >= info.x) {
		_x = info.x - abs(x);
		//return boundaryGet(_x, y, z, type);
	}
	if (y < 0 || y >= info.y) {
		//return nullptr;
		//テスト　周期境界
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
