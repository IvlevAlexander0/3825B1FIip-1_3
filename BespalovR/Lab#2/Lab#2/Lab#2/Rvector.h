#pragma once
#include <stdexcept>
#include "../../../../includes/Lab2/utmatrix.h"


template <class ValType>
TVector<ValType>::TVector(int s, int si) {
    if (s < 0) {
        throw std::out_of_range("Vector size must be non-negative");
    }
    if (si < 0 || si > s) {
        throw std::out_of_range("Invalid start index");
    }
    Size = s;
    StartIndex = si;
    pVector = new ValType[Size];
    for (int i = 0; i < Size; ++i) {
        pVector[i] = ValType{};
    }
}


template <class ValType>
TVector<ValType>::TVector(const TVector<ValType>& v) {
    Size = v.Size;
    StartIndex = v.StartIndex;
    pVector = new ValType[Size];
    for (int i = 0; i < Size; ++i) {
        pVector[i] = v.pVector[i];
    }
}


template <class ValType>
TVector<ValType>::~TVector() {
    delete[] pVector;
    pVector = nullptr;
}


template <class ValType>
ValType& TVector<ValType>::operator[](int pos) {
    if (pos < StartIndex || pos >= Size) {
        throw std::out_of_range("Vector index out of range");
    }
    return pVector[pos - StartIndex];
}


template <class ValType>
const ValType& TVector<ValType>::operator[](int pos) const {
    if (pos < StartIndex || pos >= Size) {
        throw std::out_of_range("Vector index out of range");
    }
    return pVector[pos - StartIndex];
}


template <class ValType>
bool TVector<ValType>::operator==(const TVector<ValType>& v) const {
    if (Size != v.Size || StartIndex != v.StartIndex) {
        return false;
    }
    for (int i = 0; i < Size; ++i) {
        if (pVector[i] != v.pVector[i]) {
            return false;
        }
    }
    return true;
}


template <class ValType>
bool TVector<ValType>::operator!=(const TVector<ValType>& v) const { return !(*this == v); }


template <class ValType>
TVector<ValType>& TVector<ValType>::operator=(const TVector<ValType>& v) {
    if (this == &v) {
        return *this;
    }
    ValType* newvec = new ValType[v.Size];
    for (int i = 0; i < v.Size; ++i) {
        newvec[i] = v.pVector[i];
    }
    delete[] pVector;
    Size = v.Size;
    StartIndex = v.StartIndex;
    pVector = newvec;
    return *this;
}


template <class ValType>
TVector<ValType> TVector<ValType>::operator+(const ValType& val) const {
    TVector<ValType> result(*this);
    for (int i = 0; i < Size - StartIndex; ++i) {
        result.pVector[i] += val;
    }
    return result;
}


template <class ValType>
TVector<ValType> TVector<ValType>::operator-(const ValType& val) const {
    TVector<ValType> result(*this);
    for (int i = 0; i < Size - StartIndex; ++i) {
        result.pVector[i] -= val;
    }
    return result;
}


template <class ValType>
TVector<ValType> TVector<ValType>::operator*(const ValType& val) const {
    TVector<ValType> result(*this);
    for (int i = 0; i < Size - StartIndex; ++i) {
        result.pVector[i] *= val;
    }
    return result;
}


template <class ValType>
TVector<ValType> TVector<ValType>::operator+(const TVector<ValType>& v) const {
    if (Size != v.Size || StartIndex != v.StartIndex) {
        throw std::invalid_argument("Vectors must have the same size and start index");
    }
    TVector<ValType> result(Size, StartIndex);
    for (int i = 0; i < Size; ++i) {
        result.pVector[i] = pVector[i] + v.pVector[i];
    }
    return result;
}


template <class ValType>
TVector<ValType> TVector<ValType>::operator-(const TVector<ValType>& v) const {
    if (Size != v.Size || StartIndex != v.StartIndex) {
        throw std::invalid_argument("Vectors must have the same size and start index");
    }
    TVector<ValType> result(Size, StartIndex);
    for (int i = 0; i < Size; ++i) {
        result.pVector[i] = pVector[i] - v.pVector[i];
    }
    return result;
}


template <class ValType>
ValType TVector<ValType>::operator*(const TVector<ValType>& v) const {
    if (Size != v.Size || StartIndex != v.StartIndex) {
        throw std::invalid_argument("Vectors must have the same size and start index");
    }
    ValType result = ValType{};
    for (int i = 0; i < Size; ++i) {
        result += pVector[i] * v.pVector[i];
    }
    return result;
}