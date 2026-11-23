#pragma once
#include "utmatrix.h"
#include <stdexcept>

inline int checkMatrixSize(int s)
{
    if (s < 0 || s > MAX_MATRIX_SIZE)
        throw std::invalid_argument("TMatrix: invalid size");
    return s;
}

//TVector

template <class ValType>
TVector<ValType>::TVector(int s, int si)
{
    if (s < 0 || si < 0 || si > s)
        throw std::invalid_argument("TVector: invalid size or start index");
    if (s > MAX_VECTOR_SIZE)
        throw std::invalid_argument("TVector: size exceeds MAX_VECTOR_SIZE");

    Size = s;
    StartIndex = si;
    pVector = new ValType[Size > 0 ? Size : 1]();
}

template <class ValType>
TVector<ValType>::TVector(const TVector& v)
{
    Size = v.Size;
    StartIndex = v.StartIndex;
    pVector = new ValType[Size > 0 ? Size : 1]();
    for (int i = 0; i < Size; ++i)
        pVector[i] = v.pVector[i];
}

template <class ValType>
TVector<ValType>::~TVector()
{
    delete[] pVector;
}

template <class ValType>
ValType& TVector<ValType>::operator[](int pos)
{
    if (pos < StartIndex || pos >= Size)
        throw std::out_of_range("TVector: index out of range");
    return pVector[pos];
}

template <class ValType>
const ValType& TVector<ValType>::operator[](int pos) const
{
    if (pos < StartIndex || pos >= Size)
        throw std::out_of_range("TVector: index out of range");
    return pVector[pos];
}

template <class ValType>
bool TVector<ValType>::operator==(const TVector& v) const
{
    if (Size != v.Size || StartIndex != v.StartIndex)
        return false;

    for (int i = StartIndex; i < Size; ++i)
        if (pVector[i] != v.pVector[i])
            return false;
    return true;
}

template <class ValType>
bool TVector<ValType>::operator!=(const TVector& v) const
{
    return !(*this == v);
}

template <class ValType>
TVector<ValType>& TVector<ValType>::operator=(const TVector& v)
{
    if (this == &v)
        return *this;

    if (Size != v.Size)
    {
        delete[] pVector;
        Size = v.Size;
        pVector = new ValType[Size > 0 ? Size : 1]();
    }
    StartIndex = v.StartIndex;
    for (int i = 0; i < Size; ++i)
        pVector[i] = (i >= StartIndex) ? v.pVector[i] : ValType{};
    return *this;
}

template <class ValType>
TVector<ValType> TVector<ValType>::operator+(const ValType& val) const
{
    TVector res(*this);
    for (int i = StartIndex; i < Size; ++i)
        res.pVector[i] += val;
    return res;
}

template <class ValType>
TVector<ValType> TVector<ValType>::operator-(const ValType& val) const
{
    TVector res(*this);
    for (int i = StartIndex; i < Size; ++i)
        res.pVector[i] -= val;
    return res;
}

template <class ValType>
TVector<ValType> TVector<ValType>::operator*(const ValType& val) const
{
    TVector res(*this);
    for (int i = StartIndex; i < Size; ++i)
        res.pVector[i] *= val;
    return res;
}

template <class ValType>
TVector<ValType> TVector<ValType>::operator+(const TVector& v) const
{
    if (Size != v.Size || StartIndex != v.StartIndex)
        throw std::invalid_argument("TVector: size/startIndex mismatch");

    TVector res(*this);
    for (int i = StartIndex; i < Size; ++i)
        res.pVector[i] += v.pVector[i];
    return res;
}

template <class ValType>
TVector<ValType> TVector<ValType>::operator-(const TVector& v) const
{
    if (Size != v.Size || StartIndex != v.StartIndex)
        throw std::invalid_argument("TVector: size/startIndex mismatch");

    TVector res(*this);
    for (int i = StartIndex; i < Size; ++i)
        res.pVector[i] -= v.pVector[i];
    return res;
}

template <class ValType>
ValType TVector<ValType>::operator*(const TVector& v) const
{
    if (Size != v.Size || StartIndex != v.StartIndex)
        throw std::invalid_argument("TVector: size/startIndex mismatch");

    ValType sum = ValType{};
    for (int i = StartIndex; i < Size; ++i)
        sum += pVector[i] * v.pVector[i];
    return sum;
}

//TMatrix

template <class ValType>
TMatrix<ValType>::TMatrix(int s)
    : TVector<TVector<ValType>>(checkMatrixSize(s), 0)
{
    for (int i = 0; i < s; ++i)
    {
        this->pVector[i] = TVector<ValType>(s, i);
    }
}

template <class ValType>
TMatrix<ValType>::TMatrix(const TMatrix& mt)
    : TVector<TVector<ValType>>(mt)
{
}

template <class ValType>
TMatrix<ValType>::TMatrix(const TVector<TVector<ValType>>& mt)
    : TVector<TVector<ValType>>(mt)
{
    for (int i = 0; i < this->Size; ++i)
    {
        if (this->pVector[i].GetStartIndex() != i)
            throw std::invalid_argument("TMatrix: row start index must equal row number");
    }
}

template <class ValType>
bool TMatrix<ValType>::operator==(const TMatrix& mt) const
{
    return TVector<TVector<ValType>>::operator==(mt);
}

template <class ValType>
bool TMatrix<ValType>::operator!=(const TMatrix& mt) const
{
    return !(*this == mt);
}

template <class ValType>
TMatrix<ValType>& TMatrix<ValType>::operator=(const TMatrix& mt)
{
    TVector<TVector<ValType>>::operator=(mt);
    return *this;
}

template <class ValType>
TMatrix<ValType> TMatrix<ValType>::operator+(const TMatrix& mt) const
{
    if (this->GetSize() != mt.GetSize())
        throw std::invalid_argument("TMatrix: size mismatch");

    TMatrix res(*this);
    for (int i = 0; i < this->GetSize(); ++i)
        res[i] = (*this)[i] + mt[i];
    return res;
}

template <class ValType>
TMatrix<ValType> TMatrix<ValType>::operator-(const TMatrix& mt) const
{
    if (this->GetSize() != mt.GetSize())
        throw std::invalid_argument("TMatrix: size mismatch");

    TMatrix res(*this);
    for (int i = 0; i < this->GetSize(); ++i)
        res[i] = (*this)[i] - mt[i];
    return res;
}