#pragma once
#include "utmatrix.h"
#include <stdexcept>
#include <iostream>

template <class ValType>
TVector<ValType>::TVector(int s, int si) : Size(s), StartIndex(si) {
    if (s < 0 || s > MAX_VECTOR_SIZE) {
        throw std::out_of_range("Invalid vector size!");
    }
    if (si < 0) {
        throw std::out_of_range("Invalid start index!");
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
    if (pos < StartIndex || pos >= StartIndex + Size) {
        throw std::out_of_range("Index out of range");
    }
    return pVector[pos - StartIndex];
}

template <class ValType>
const ValType& TVector<ValType>::operator[](int pos) const {
    if (pos < StartIndex || pos >= StartIndex + Size) {
        throw std::out_of_range("Index out of range");
    }
    return pVector[pos - StartIndex];
}

template <class ValType>
bool TVector<ValType>::operator==(const TVector& v) const
{
    if (Size != v.Size) {
        return false;
    }
    if (StartIndex != v.StartIndex) {
        return false;
    }
    for (int i = 0; i < Size; ++i) {
        if (!(pVector[i] == v.pVector[i])) {
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

template <class ValType>
TVector<ValType> TVector<ValType>::operator+(const ValType& val) const {
    TVector<ValType> result(*this);
    for (int i = 0; i < Size; ++i) {
        result.pVector[i] = pVector[i] + val;
    }
    return result;
}

template <class ValType>
TVector<ValType> TVector<ValType>::operator-(const ValType& val) const {
    TVector<ValType> result(*this);
    for (int i = 0; i < Size; ++i) {
        result.pVector[i] = pVector[i] - val;
    }
    return result;
}

template <class ValType>
TVector<ValType> TVector<ValType>::operator*(const ValType& val) const {
    TVector<ValType> result(*this);
    for (int i = 0; i < Size; ++i) {
        result.pVector[i] = pVector[i] * val;
    }
    return result;
}

template <class ValType>
TVector<ValType> TVector<ValType>::operator+(const TVector& v) const {
    if (Size != v.Size) {
        throw std::invalid_argument("Different vector sizes");
    }
    if (StartIndex != v.StartIndex) {
        throw std::invalid_argument("Different vector start indexes");
    }
    TVector<ValType> result(*this);
    for (int i = 0; i < Size; ++i) {
        result.pVector[i] = pVector[i] + v.pVector[i];
    }
    return result;
}

template <class ValType>
TVector<ValType> TVector<ValType>::operator-(const TVector& v) const
{
    if (Size != v.Size) {
        throw std::invalid_argument("Different vector sizes");
    }
    if (StartIndex != v.StartIndex) {
        throw std::invalid_argument("Different vector start indexes");
    }
    TVector<ValType> result(*this);
    for (int i = 0; i < Size; ++i) {
        result.pVector[i] = pVector[i] - v.pVector[i];
    }
    return result;
}

template <class ValType>
ValType TVector<ValType>::operator*(const TVector& v) const {
    if (Size != v.Size) {
        throw std::invalid_argument("Different vector sizes");
    }
    if (StartIndex != v.StartIndex) {
        throw std::invalid_argument("Different vector start indexes");
    }
    ValType sum = 0;
    for (int i = 0; i < Size; ++i) {
        sum += pVector[i] * v.pVector[i];
    }
    return sum;
}


template <class ValType>
TMatrix<ValType>::TMatrix(int s) : TVector<TVector<ValType>>(s) {
    if (s < 0 || s > MAX_MATRIX_SIZE) {
        throw std::out_of_range("Invalid size");
    }
    for (size_t i = 0; i < this->GetSize(); ++i) {
        this->pVector[i] = TVector<ValType>(this->GetSize() - i, i);
    }
}

template <class ValType>
TMatrix<ValType>::TMatrix(const TMatrix& mt) : TVector<TVector<ValType>>(mt) {}

template <class ValType>
TMatrix<ValType>::TMatrix(const TVector<TVector<ValType>>& mt) : TVector<TVector<ValType>>(mt) {}

template <class ValType>
bool TMatrix<ValType>::operator==(const TMatrix& mt) const {
    return TVector<TVector<ValType>>::operator==(mt);
}

template <class ValType>
bool TMatrix<ValType>::operator!=(const TMatrix& mt) const {
    return !(*this == mt);
}

template <class ValType>
TMatrix<ValType>& TMatrix<ValType>::operator=(const TMatrix& mt) {
    if (this == &mt) {
        return *this;
    }
    TVector<TVector<ValType>>::operator=(mt);
    return *this;
}

template <class ValType>
TMatrix<ValType> TMatrix<ValType>::operator+(const TMatrix& mt) const {
    if (this->GetSize() != mt.GetSize()) {
        throw std::invalid_argument("Different matrix sizes");
    }
    TMatrix<ValType> result(*this);
    int n = this->GetSize();
    for (int i = 0; i < n; ++i) {
        for (int j = i; j < n; ++j) {
            result[i][j] = (*this)[i][j] + mt[i][j];
        }
    }
    return result;
}

template <class ValType>
TMatrix<ValType> TMatrix<ValType>::operator-(const TMatrix& mt) const
{
    if (this->GetSize() != mt.GetSize()) {
        throw std::invalid_argument("Different matrix sizes");
    }
    TMatrix<ValType> result(*this);
    int n = this->GetSize();
    for (int i = 0; i < n; ++i) {
        for (int j = i; j < n; ++j) {
            result[i][j] = (*this)[i][j] - mt[i][j];
        }
    }
    return result;
}
