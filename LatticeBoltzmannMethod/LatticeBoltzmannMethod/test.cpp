#pragma once
#include "Vector.h"
#include <stdio.h>
#include <iostream>

void main() {
	CVector* vec = new CVector3();
	vec->set(1);

	printf("(%f,%f,%f)\n", vec->get(CVector3::Dim::X)
		, vec->get(CVector3::Dim::Y)
		, vec->get(CVector3::Dim::Z));
	getchar();

	delete vec;
}