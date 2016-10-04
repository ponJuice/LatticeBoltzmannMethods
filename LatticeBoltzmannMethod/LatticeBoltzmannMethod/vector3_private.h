#pragma once
#ifndef VECTOR3_PRIVATE_H
#define VECTOR3_PRIVATE_H

template<typename T>
int CVector3<T>::getDimention() {
	return 3;
}

template<typename T>
T CVector3<T>::get(int index) {
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

template<typename T>
void CVector3<T>::setAt(int index, T value) {
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

template<typename T>
void CVector3<T>::add(CVector* vec, CVector* out) {
	CVector3<T> temp;
	temp.setAt(Dim::X, x + vec->get(Dim::X));
	temp.setAt(Dim::Y, y + vec->get(Dim::Y));
	temp.setAt(Dim::Z, z + vec->get(Dim::Z));
	out->copy(&temp);
}

template<typename T>
void CVector3<T>::add(CVector* vec) {
	setAt(Dim::X, get(Dim::X) + vec->get(Dim::X));
	setAt(Dim::Y, get(Dim::Y) + vec->get(Dim::Y));
	setAt(Dim::Z, get(Dim::Z) + vec->get(Dim::Z));
}

template<typename T>
void CVector3<T>::sub(CVector* vec, CVector* out) {
	CVector3<T> temp;
	temp.setAt(Dim::X, x - vec->get(Dim::X));
	temp.setAt(Dim::Y, y - vec->get(Dim::Y));
	temp.setAt(Dim::Z, z - vec->get(Dim::Z));
	out->copy(&temp);
}

template<typename T>
void CVector3<T>::mult(double scalar, CVector* out) {
	CVector3<T> temp;
	temp.x = x * scalar;
	temp.y = y * scalar;
	temp.z = z * scalar;
	out->copy(&temp);
}

template<typename T>
void CVector3<T>::div(double scalar, CVector* out) {
	if (scalar == 0)
		throw std::range_error("Divided by zero.");
	CVector3<T> temp;
	temp.x = x / scalar;
	temp.y = y / scalar;
	temp.z = z / scalar;
	out->copy(&temp);
}

template<typename T>
double CVector3<T>::dot(CVector* vec) {
	return x*vec->get(Dim::X) + y*vec->get(Dim::Y) + z*vec->get(Dim::Z);
}

/*template<typename T>
double CVector3<T>::dot(CVector<int>* vec) {
return x*(double)vec->get(Dim::X) + y*(double)vec->get(Dim::Y) + z*(double)vec->get(Dim::Z);
}*/

template<typename T>
void CVector3<T>::cross(CVector* operand, CVector* output) {
	CVector3<T> temp;
	temp.setAt(Dim::X, y*operand->get(Dim::Z) - z*operand->get(Dim::Y));
	temp.setAt(Dim::Y, z*operand->get(Dim::X) - x*operand->get(Dim::Z));
	temp.setAt(Dim::Z, x*operand->get(Dim::Y) - y*operand->get(Dim::X));
	output->copy(&temp);
}

template<typename T>
void CVector3<T>::copy(CVector* vec) {
	x = vec->get(Dim::X);
	y = vec->get(Dim::Y);
	z = vec->get(Dim::Z);
}

#endif