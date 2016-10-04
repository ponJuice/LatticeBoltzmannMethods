#pragma once
#include <stdexcept>
#include <stdio.h>

template<typename T>
class CVector
{
public:
	CVector() {};
	virtual ~CVector() {};
	virtual T get(int index) = 0;
	virtual void setAt(int index, T value) = 0;
	virtual void add(CVector* vec,CVector* out) = 0;
	virtual void add(CVector* vec) = 0;
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