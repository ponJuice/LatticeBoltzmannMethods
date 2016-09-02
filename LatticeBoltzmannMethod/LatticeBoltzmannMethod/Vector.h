#pragma once
#include <stdexcept>
#include <stdio.h>

class CVector
{
public:
	CVector();
	virtual ~CVector();
	virtual double get(int index) = 0;
	virtual void setAt(int index, double value) = 0;
	virtual void add(CVector* vec,CVector* out) = 0;
	virtual int getDimention() = 0;
	virtual void sub(CVector* vec,CVector* out) = 0;
	virtual void mult(double scalar,CVector* out) = 0;
	virtual void div(double scalar,CVector* out) = 0;
	virtual double dot(CVector* vec) = 0;
	virtual void cross(CVector* operand, CVector* output) = 0;
	virtual void copy(CVector* vec) = 0;

/*---------テンプレート関数用スペース-----------*/

public:
	template <class... Args>
	void set(Args... args) {
		set_imp(0, args...);
	}
private:
	template<class First, class... Rest>
	void set_imp(int index, First value, Rest... rest) {
		setAt(index, value);
		set_imp(++index, rest...);
	}
	void set_imp(int index) {
		return;
	}

/*----------------------------------------------*/

};

class CVector3 : public CVector {
public:
	enum Dim {
		X = 0,
		Y = 1,
		Z = 2
	};
	double x = 0, y = 0, z = 0;
	virtual double get(int index);
	virtual void setAt(int index, double value);
	virtual int getDimention();
	virtual void add(CVector* vec, CVector* out);
	virtual void sub(CVector* vec, CVector* out);
	virtual void mult(double scalar,CVector* out);
	virtual void div(double scalar,CVector* out);
	virtual double dot(CVector* vec);
	virtual void cross(CVector* operand, CVector* output);
	virtual void copy(CVector* vec);
};