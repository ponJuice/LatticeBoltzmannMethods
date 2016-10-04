#pragma once
#ifndef VECTOR3_H

#include "Vector.h"

template<typename T>
class CVector3 : public CVector<T> {
public:
	enum Dim {
		X = 0,
		Y = 1,
		Z = 2
	};
	T x = 0, y = 0, z = 0;
	CVector3(T _x, T _y, T _z) { x = _x; y = _y; z = _z; };
	CVector3() { x = 0; y = 0; z = 0; };
	virtual ~CVector3() {};
	virtual T get(int index);
	virtual void setAt(int index, T value);
	virtual int getDimention();
	virtual void add(CVector* vec, CVector* out);
	virtual void add(CVector* vec);
	virtual void sub(CVector* vec, CVector* out);
	virtual void mult(double scalar, CVector* out);
	virtual void div(double scalar, CVector* out);
	//virtual double dot(CVector<int>* vec);
	virtual double dot(CVector* vec);
	virtual void cross(CVector* operand, CVector* output);
	virtual void copy(CVector* vec);
};

#include "vector3_private.h"

#endif