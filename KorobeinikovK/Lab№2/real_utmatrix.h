#pragma once
#include "C:\includes\Lab2\utmatrix.h"
#include <stdexcept>

template <class ValType>
TVector<ValType>::TVector(int s, int si) : Size(s), StartIndex(si) {
	if (s < 0 || s > MAX_VECTOR_SIZE) {
		throw std::out_of_range("Invalid vector size!");
	}
	if (Size > 0) {
		pVector = new ValType[Size]();
	}
	else {
		pVector = nullptr;
	}
}
template <class ValType>
TVector<ValType>::TVector(const TVector& v) {
	Size = v.Size;
	StartIndex = v.StartIndex;
	if (Size > 0) {
		pVector = new ValType[Size];
		for (size_t i = 0; i < Size; ++i) {
			pVector[i] = v.pVector[i];
		}
	}
	else {
		pVector = nullptr;
	}
}
template <class ValType>
TVector<ValType>::~TVector() {
	delete[] pVector;
}
template <class ValType>
ValType& TVector<ValType>::operator[](int pos) {
	if (pos < StartIndex || pos >= Size + StartIndex) {
		throw std::out_of_range("Index out of range!");
	}
	return pVector[pos - StartIndex];
}
template <class ValType>
const ValType& TVector<ValType>::operator[](int pos) const {
	if (pos < StartIndex || pos >= Size + StartIndex) {
		throw std::out_of_range("Index out of range!");
	}
	return pVector[pos - StartIndex];
}
template <class ValType>
bool TVector<ValType>::operator==(const TVector& v) const {
	if (Size != v.Size || StartIndex != v.StartIndex) {
		return false;
	}
	for (size_t i = 0; i < Size; ++i) {
		if (pVector[i] != v.pVector[i]) {
			return false;
		}
	}
	return true;
}
template <class ValType>
bool TVector<ValType>::operator!=(const TVector& v) const {
	if (*this == v) {
		return false;
	}
	return true;
}
template <class ValType>
TVector<ValType>& TVector<ValType>::operator=(const TVector& v) {
	if (this != &v) {
		delete[] pVector;
		Size = v.Size;
		StartIndex = v.StartIndex;
		if (Size > 0) {
			pVector = new ValType[Size];
			for (size_t i = 0; i < Size; ++i) {
				pVector[i] = v.pVector[i];
			}
		}
		else {
			pVector = nullptr;
		}
	}
	return *this;
}
template<class ValType>
TVector<ValType> TVector<ValType>::operator+(const ValType& val) const {
	TVector<ValType> res(*this);
	for (size_t i = 0; i < Size; ++i) {
		res.pVector[i] = pVector[i] + val;
	}
	return res;
}
template<class ValType>
TVector<ValType> TVector<ValType>::operator-(const ValType& val) const {
	TVector<ValType> res(*this);
	for (size_t i = 0; i < Size; ++i) {
		res.pVector[i] = pVector[i] - val;
	}
	return res;
}
template<class ValType>
TVector<ValType> TVector<ValType>::operator*(const ValType& val) const {
	TVector<ValType> res(*this);
	for (size_t i = 0; i < Size; ++i) {
		res.pVector[i] = pVector[i] * val;
	}
	return res;
}
template<class ValType>
TVector<ValType> TVector<ValType>::operator+(const TVector& v) const {
	if (Size != v.Size) {
		throw std::invalid_argument("Different vector sizes!");
	}
	if (StartIndex != v.StartIndex) {
		throw std::invalid_argument("Different vector start indexes!");
	}
	TVector<ValType> res(*this);
	for (size_t i = 0; i < Size; ++i) {
		res.pVector[i] = pVector[i] + v.pVector[i];
	}
	return res;
}
template<class ValType>
TVector<ValType> TVector<ValType>::operator-(const TVector& v) const {
	if (Size != v.Size) {
		throw std::invalid_argument("Different vector sizes!");
	}
	if (StartIndex != v.StartIndex) {
		throw std::invalid_argument("Different vector start indexes!");
	}
	TVector<ValType> res(*this);
	for (size_t i = 0; i < Size; ++i) {
		res.pVector[i] = pVector[i] - v.pVector[i];
	}
	return res;
}
template<class ValType>
ValType TVector<ValType>::operator*(const TVector& v) const {
	if (Size != v.Size) {
		throw std::invalid_argument("Different vector sizes!");
	}
	if (StartIndex != v.StartIndex) {
		throw std::invalid_argument("Different vector start indexes!");
	}
	ValType res = 0;
	for (size_t i = 0; i < Size; ++i) {
		res += pVector[i] * v.pVector[i];
	}
	return res;
}


template <class ValType>
TMatrix<ValType>::TMatrix(int s) :TVector<TVector<ValType>>(s) {
	if (s < 0 || s > MAX_MATRIX_SIZE) {
		throw std::out_of_range("Invalid matrix size!");
	}
	for (size_t i = 0; i < this->Size; ++i) {
		this->pVector[i] = TVector<ValType>(this->Size - i, i);
	}
}
template <class ValType>
TMatrix<ValType>::TMatrix(const TMatrix& mt) : TVector<TVector<ValType>>(mt) {}
template <class ValType>
TMatrix<ValType>::TMatrix(const TVector<TVector<ValType>>& mt) : TVector<TVector<ValType>>(mt) {}
template <class ValType>
bool TMatrix<ValType>::operator==(const TMatrix& mt) const {
	if (this->Size != mt.Size) {
		return false;
	}
	for (size_t i = 0; i < this->Size; ++i) {
		if (this->pVector[i] != mt[i]) {
			return false;
		}
	}
	return true;
}
template <class ValType>
bool TMatrix<ValType>::operator!=(const TMatrix& mt) const {
	if (*this == mt) {
		return false;
	}
	return true;
}
template <class ValType>
TMatrix<ValType>& TMatrix<ValType>::operator=(const TMatrix<ValType>& mt) {
	if (this != &mt) {
		delete[] this->pVector;
		this->Size = mt.Size;
		this->StartIndex = mt.StartIndex;
		if (this->Size > 0) {
			this->pVector = new TVector<ValType>[this->Size];
			for (size_t i = 0; i < this->Size; ++i) {
				this->pVector[i] = mt[i];
			}
		}
		else {
			this->pVector = nullptr;
		}
	}
	return *this;
}
template <class ValType>
TMatrix<ValType> TMatrix<ValType>::operator+(const TMatrix& mt) const {
	if (this->Size != mt.Size) {
		throw std::invalid_argument("Different matrix sizes!");
	}
	TMatrix<ValType> res(*this);
	for (size_t i = 0; i < this->Size; ++i) {
		res[i] = this->pVector[i] + mt[i];
	}
	return res;
}
template <class ValType>
TMatrix<ValType> TMatrix<ValType>::operator-(const TMatrix& mt) const {
	if (this->Size != mt.Size) {
		throw std::invalid_argument("Different matrix sizes!");
	}
	TMatrix<ValType> res(*this);
	for (size_t i = 0; i < this->Size; ++i) {
		res[i] = this->pVector[i] - mt[i];
	}
	return res;
}