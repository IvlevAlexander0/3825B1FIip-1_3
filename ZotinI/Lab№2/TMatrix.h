#pragma once

#include <iostream>
#include <stdexcept>
#include "utmatrix.h"
#include "TVector.h"

using namespace std;


template <class ValType>
TMatrix<ValType>::TMatrix(int s) : TVector<TVector<ValType>>(s, 0) {
	if (s < 0 || s > MAX_MATRIX_SIZE) {
		throw out_of_range("Invalid size");
	}

	for (int i = 0; i < s; i++) {
		TVector<TVector<ValType>>::pVector[i] = TVector<ValType>(s - i, i);
	}
}

template <class ValType>
TMatrix<ValType>::TMatrix(const TMatrix& mt): TVector<TVector<ValType>>(mt) {}

template <class ValType>
TMatrix<ValType>::TMatrix(const TVector<TVector<ValType>>& mt) : TVector<TVector<ValType>>(mt) {}

template <class ValType>
bool TMatrix<ValType>::operator==(const TMatrix<ValType>& mt) const {
	return TVector<TVector<ValType>>::operator==(mt);
}

template <class ValType>
bool TMatrix<ValType>::operator!=(const TMatrix<ValType>& mt) const {
	return !(*this == mt);
}

template <class ValType>
TMatrix<ValType>& TMatrix<ValType>::operator=(const TMatrix<ValType>& mt) {
	if (TVector<TVector<ValType>>::Size != mt.Size) {
		throw invalid_argument("Invalid size");
	}
	if (this != &mt) {
		TVector<TVector<ValType>>::operator=(mt);
	}
	return *this;
}

template <class ValType>
TMatrix<ValType> TMatrix<ValType>::operator+(const TMatrix<ValType>& mt) const {
	return TVector<TVector<ValType>>::operator+(mt);
}

template <class ValType>
TMatrix<ValType> TMatrix<ValType>::operator-(const TMatrix<ValType>& mt) const {
	return TVector<TVector<ValType>>::operator-(mt);
}


