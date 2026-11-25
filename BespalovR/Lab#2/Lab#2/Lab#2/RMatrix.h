#pragma once
#include "Rvector.h"


template <class ValType>
TMatrix<ValType>::TMatrix(int s) : TVector<TVector<ValType>>(s) {
    if (s < 0 || s > MAX_MATRIX_SIZE) {
        throw std::out_of_range("Invalid matrix size");
    }
    for (int i = 0; i < s; ++i) {
        (*this)[i] = TVector<ValType>(s, i);
    }
}


template <class ValType>
TMatrix<ValType>::TMatrix(const TMatrix<ValType>& mt) : TVector<TVector<ValType>>(mt) {}


template <class ValType>
TMatrix<ValType>::TMatrix(const TVector<TVector<ValType>>& mt) : TVector<TVector<ValType>>(mt) {}


template <class ValType>
bool TMatrix<ValType>::operator==(const TMatrix<ValType>& mt) const { return TVector<TVector<ValType>>::operator==(mt); }


template <class ValType>
bool TMatrix<ValType>::operator!=(const TMatrix<ValType>& mt) const { return !(*this == mt); }


template <class ValType>
TMatrix<ValType>& TMatrix<ValType>::operator=(const TMatrix<ValType>& mt) {
    if (this == &mt) {
        return *this;
    }
    TVector<TVector<ValType>>::operator=(mt);
    return *this;
}


template <class ValType>
TMatrix<ValType> TMatrix<ValType>::operator+(const TMatrix<ValType>& mt) const {
    if (GetSize() != mt.GetSize()) {
        throw std::invalid_argument("Matrices must have the same size");
    }
    return TMatrix<ValType>(TVector<TVector<ValType>>::operator+(mt));
}


template <class ValType>
TMatrix<ValType> TMatrix<ValType>::operator-(const TMatrix<ValType>& mt) const {
    if (GetSize() != mt.GetSize()) {
        throw std::invalid_argument("Matrices must have the same size");
    }
    return TMatrix<ValType>(TVector<TVector<ValType>>::operator-(mt));
}