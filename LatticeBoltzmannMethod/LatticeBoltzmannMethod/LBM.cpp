#include "LBM.h"



CLBM::CLBM(LBMInfo lbm)
{
	//設定をコピー
	info = lbm;

	//音速の計算
	sos = std::sqrt((info.k*info.r*info.t)/info.m);

	//粒子の移流速度の計算
	as = info.deltaLength / info.deltaTime;

	//緩和時間の計算
	double kvc = (info.cv*info.cld) / info.re;		//動粘性係数
	rt = (3.0*kvc) / (as*info.deltaLength) + 0.5;

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

	//格子点のメモリ確保
	point = new Point[info.x*info.y*info.z];
	for (int n = 0; n < info.x*info.y*info.z; n++) {
		point[n].distribut = new double[info.directionNum];
		for (int a = 0; a < info.directionNum; a++) {
			point[n].distribut[a] = 0;
		}
	}
	//速度、圧力の初期値設定

	//速度方向ベクトルのメモリ確保
	e = new CVector3[info.directionNum];
	//速度方向ベクトルの初期化
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
				//圧力、速度の計算
				for (int a = 0; a < info.directionNum; a++) {
					//巨視的圧力の計算
					p += point[x, y, z].distribut[a];
					//巨視的速度の計算
					e[a].mult(point[x, y, z].distribut[a], &temp);
					u.add(&temp, &u);
				}
				u.div(info.density*sos, &u);
				//分布関数、平衡分布関数の計算
				for (int a = 0; a < info.directionNum; a++) {
					//平衡分布関数
					double e_dot_u = e[a].dot(&u);		//速度方向ベクトルと速度ベクトルの内積
					double peq = w[a] * (p + info.density * (e_dot_u 
								+ 1.5 * ((e_dot_u * e_dot_u) / (as * as))
							- 0.5*u.dot(&u)));
					//平衡関数
					double pa = point[x, y, z].distribut[a]
						- (1.0 / rt)*(point[x,y,z].distribut[a]-peq);
				}
			}
		}
	}
}