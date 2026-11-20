#pragma once

#include <iostream>
#include <stdexcept>
#include "utmatrix.h"

using namespace std;


template <class ValType>
TVector<ValType>::TVector(int s, int si) {
	if (s < 0 || s > MAX_VECTOR_SIZE || si < 0 || si > MAX_VECTOR_SIZE) {
		throw out_of_range("Invalid size");
	}

	Size = s;
	StartIndex = si;

	if (Size == 0) {
		pVector = nullptr;
	}
	else {
		pVector = new ValType[Size]();
	}
}

template <class ValType>
TVector<ValType>::TVector(const TVector<ValType>& v) : Size(v.Size), StartIndex(v.StartIndex) {
	if (Size == 0) {
		pVector = nullptr;
	}
	else {
		pVector = new ValType[Size];
		for (int i = 0; i < Size; i++) {
			pVector[i] = v.pVector[i];
		}
	}
}

template <class ValType>
TVector<ValType>::~TVector() {
	delete[] pVector;
}


template <class ValType>
ValType& TVector<ValType>::operator[](int pos) {
	if (pos < StartIndex || pos >= StartIndex + Size) {
		throw out_of_range("Invalid index");
	}
	return pVector[pos - StartIndex];
}

template <class ValType>
const ValType& TVector<ValType>::operator[](int pos) const {
	if (pos < StartIndex || pos >= StartIndex + Size) {
		throw out_of_range("Invalid index");
	}
	return pVector[pos - StartIndex];
}

template <class ValType>
bool TVector<ValType>::operator==(const TVector& v) const {
	if (Size != v.Size || StartIndex != v.StartIndex) {
		return false;
	}
	for (int i = 0; i < Size; i++) {
		if (pVector[i] != v.pVector[i]) {
			return false;
		}
	}
	return true;
}

template <class ValType>
bool TVector<ValType>::operator!=(const TVector& v) const {
	return !(*this == v);
}

template <class ValType>
TVector<ValType>& TVector<ValType>::operator=(const TVector<ValType>& v) {
	if (this != &v) {
		if (Size != v.Size) {
			delete[] pVector;
			Size = v.Size;
			if (Size == 0) {
				pVector = nullptr;
			}
			else {
				pVector = new ValType[Size];
			}
		}
		StartIndex = v.StartIndex;
		for (int i = 0; i < Size; i++) {
			pVector[i] = v.pVector[i];
		}
	}
	return *this;
}

template <class ValType>
TVector<ValType>  TVector<ValType>::operator+(const ValType& val) const {
	TVector<ValType> res(*this);
	for (int i = 0; i < Size; i++) {
		res.pVector[i] += val;
	}
	return res;
}

template <class ValType>
TVector<ValType>  TVector<ValType>::operator-(const ValType& val) const {
	TVector<ValType> res(*this);
	for (int i = 0; i < Size; i++) {
		res.pVector[i] -= val;
	}
	return res;
}

template <class ValType>
TVector<ValType>  TVector<ValType>::operator*(const ValType& val) const {
	TVector<ValType> res(*this);
	for (int i = 0; i < Size; i++) {
		res.pVector[i] *= val;
	}
	return res;
}

template <class ValType>
TVector<ValType>  TVector<ValType>::operator+(const TVector<ValType>& v) const {
	if (Size != v.Size || StartIndex != v.StartIndex) {
		throw invalid_argument("Invalid size or start index");
	}
	TVector<ValType> res(*this);
	for (int i = 0; i < Size; i++) {
		res.pVector[i] = res.pVector[i] + v.pVector[i];
	}
	return res;
}

template <class ValType>
TVector<ValType>  TVector<ValType>::operator-(const TVector<ValType>& v) const {
	if (Size != v.Size || StartIndex != v.StartIndex) {
		throw invalid_argument("Invalid size or start index");
	}
	TVector<ValType> res(*this);
	for (int i = 0; i < Size; i++) {
		res.pVector[i] = res.pVector[i] - v.pVector[i];
	}
	return res;
}

template <class ValType>
ValType TVector<ValType>::operator*(const TVector<ValType>& v) const {
	if (Size != v.Size || StartIndex != v.StartIndex) {
		throw invalid_argument("Invalid size or start index");
	}
	ValType res = ValType();
	for (int i = 0; i < Size; i++) {
		res += pVector[i] * v.pVector[i];
	}
	return res;
}

