#include "Vector.h"

CVector::CVector()
{
}


CVector::~CVector()
{
}

int CVector3::getDimention() {
	return 3;
}

double CVector3::get(int index) {
	if (index == Dim::X) {
		return x;
	}
	else if (index == Dim::Y) {
		return y;
	}
	else if (index == Dim::Z) {
		return z;
	}
	else {
		return 0;
	}
}

void CVector3::setAt(int index, double value) {
	if (index >= 3) {
		return;
	}
	else if (index == Dim::X) {
		x = value;
	}
	else if (index == Dim::Y) {
		y = value;
	}
	else if (index == Dim::Z) {
		z = value;
	}
}

void CVector3::add(CVector* vec, CVector* out) {
	CVector3 temp;
	temp.setAt(Dim::X, x + vec->get(Dim::X));
	temp.setAt(Dim::Y, y + vec->get(Dim::Y));
	temp.setAt(Dim::Z, z + vec->get(Dim::Z));
	out->copy(&temp);
}

void CVector3::sub(CVector* vec, CVector* out) {
	CVector3 temp;
	temp.setAt(Dim::X, x - vec->get(Dim::X));
	temp.setAt(Dim::Y, y - vec->get(Dim::Y));
	temp.setAt(Dim::Z, z - vec->get(Dim::Z));
	out->copy(&temp);
}

void CVector3::mult(double scalar,CVector* out) {
	CVector3 temp;
	temp.x = x * scalar;
	temp.y = y * scalar;
	temp.z = z * scalar;
	out->copy(&temp);
}

void CVector3::div(double scalar,CVector* out) {
	if(scalar == 0)
		throw std::range_error("Divided by zero.");
	CVector3 temp;
	temp.x = x / scalar;
	temp.y = y / scalar;
	temp.z = z / scalar;
	out->copy(&temp);
}

double CVector3::dot(CVector* vec) {
	return x*vec->get(Dim::X) + y*vec->get(Dim::Y) + z*vec->get(Dim::Z);
}

void CVector3::cross(CVector* operand, CVector* output) {
	CVector3 temp;
	temp.setAt(Dim::X, y*operand->get(Dim::Z) - z*operand->get(Dim::Y));
	temp.setAt(Dim::Y, z*operand->get(Dim::X) - x*operand->get(Dim::Z));
	temp.setAt(Dim::Z, x*operand->get(Dim::Y) - y*operand->get(Dim::X));
	output->copy(&temp);
}

void CVector3::copy(CVector* vec) {
	x = vec->get(Dim::X);
	y = vec->get(Dim::Y);
	z = vec->get(Dim::Z);
}