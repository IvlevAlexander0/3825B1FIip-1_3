#include "utmatrix.h"
#include <stdexcept>

template <class ValType>
TVector<ValType>::TVector(int s, int si)
{
    if (s < 0 || s > MAX_VECTOR_SIZE) {
        throw std::out_of_range("Vector size is out of valid range [0, MAX_VECTOR_SIZE]");
    }
    if (si < 0 || (s > 0 && si >= s) || (s == 0 && si != 0)) {
        throw std::out_of_range("StartIndex is out of valid range");
    }
    Size = s;
    StartIndex = si;
    int actualLen = Size - StartIndex;
    if (actualLen > 0) {
        pVector = new ValType[actualLen]();
    } else {
        pVector = nullptr;
    }
}

template <class ValType>
TVector<ValType>::TVector(const TVector<ValType> &v)
    : Size(v.Size), StartIndex(v.StartIndex), pVector(nullptr)
{
    int actualLen = Size - StartIndex;
    if (actualLen > 0) {
        pVector = new ValType[actualLen];
        for (int i = 0; i < actualLen; ++i) {
            pVector[i] = v.pVector[i];
        }
    }
}

template <class ValType>
TVector<ValType>::~TVector()
{
    delete[] pVector;
    pVector = nullptr;
}

template <class ValType>
ValType& TVector<ValType>::operator[](int pos)
{
    if (pos < StartIndex || pos >= Size) {
        throw std::out_of_range("Vector index out of range");
    }
    return pVector[pos - StartIndex];
}

template <class ValType>
const ValType& TVector<ValType>::operator[](int pos) const
{
    if (pos < StartIndex || pos >= Size) {
        throw std::out_of_range("Vector index out of range");
    }
    return pVector[pos - StartIndex];
}

template <class ValType>
bool TVector<ValType>::operator==(const TVector<ValType> &v) const
{
    if (this == &v) {
        return true;
    }
    if (Size != v.Size || StartIndex != v.StartIndex) {
        return false;
    }
    int actualLen = Size - StartIndex;
    for (int i = 0; i < actualLen; ++i) {
        if (pVector[i] != v.pVector[i]) {
            return false;
        }
    }
    return true;
}

template <class ValType>
bool TVector<ValType>::operator!=(const TVector<ValType> &v) const
{
    return !(*this == v);
}

template <class ValType>
TVector<ValType>& TVector<ValType>::operator=(const TVector<ValType> &v)
{
    if (this == &v) {
        return *this;
    }
    int newActualLen = v.Size - v.StartIndex;
    int oldActualLen = Size - StartIndex;
    if (oldActualLen != newActualLen) {
        ValType* newVec = (newActualLen > 0) ? new ValType[newActualLen] : nullptr;
        delete[] pVector;
        pVector = newVec;
    }
    Size = v.Size;
    StartIndex = v.StartIndex;
    for (int i = 0; i < newActualLen; ++i) {
        pVector[i] = v.pVector[i];
    }
    return *this;
}

template <class ValType>
TVector<ValType> TVector<ValType>::operator+(const ValType &val) const
{
    TVector<ValType> res(*this);
    int actualLen = Size - StartIndex;
    for (int i = 0; i < actualLen; ++i) {
        res.pVector[i] = res.pVector[i] + val;
    }
    return res;
}

template <class ValType>
TVector<ValType> TVector<ValType>::operator-(const ValType &val) const
{
    TVector<ValType> res(*this);
    int actualLen = Size - StartIndex;
    for (int i = 0; i < actualLen; ++i) {
        res.pVector[i] = res.pVector[i] - val;
    }
    return res;
}

template <class ValType>
TVector<ValType> TVector<ValType>::operator*(const ValType &val) const
{
    TVector<ValType> res(*this);
    int actualLen = Size - StartIndex;
    for (int i = 0; i < actualLen; ++i) {
        res.pVector[i] = res.pVector[i] * val;
    }
    return res;
}

template <class ValType>
TVector<ValType> TVector<ValType>::operator+(const TVector<ValType> &v) const
{
    if (Size != v.Size || StartIndex != v.StartIndex) {
        throw std::invalid_argument("Vectors must have equal Size and StartIndex for addition");
    }
    TVector<ValType> res(Size, StartIndex);
    int actualLen = Size - StartIndex;
    for (int i = 0; i < actualLen; ++i) {
        res.pVector[i] = pVector[i] + v.pVector[i];
    }
    return res;
}

template <class ValType>
TVector<ValType> TVector<ValType>::operator-(const TVector<ValType> &v) const
{
    if (Size != v.Size || StartIndex != v.StartIndex) {
        throw std::invalid_argument("Vectors must have equal Size and StartIndex for subtraction");
    }
    TVector<ValType> res(Size, StartIndex);
    int actualLen = Size - StartIndex;
    for (int i = 0; i < actualLen; ++i) {
        res.pVector[i] = pVector[i] - v.pVector[i];
    }
    return res;
}

template <class ValType>
ValType TVector<ValType>::operator*(const TVector<ValType> &v) const
{
    if (Size != v.Size || StartIndex != v.StartIndex) {
        throw std::invalid_argument("Vectors must have equal Size and StartIndex for dot product");
    }
    ValType res = ValType();
    int actualLen = Size - StartIndex;
    for (int i = 0; i < actualLen; ++i) {
        res = res + (pVector[i] * v.pVector[i]);
    }
    return res;
}

template <class ValType>
TMatrix<ValType>::TMatrix(int s)
    : TVector<TVector<ValType>>(s, 0)
{
    if (s < 0 || s > MAX_MATRIX_SIZE) {
        throw std::out_of_range("Matrix size is out of valid range [0, MAX_MATRIX_SIZE]");
    }
    for (int i = 0; i < s; ++i) {
        this->pVector[i] = TVector<ValType>(s, i);
    }
}

template <class ValType>
TMatrix<ValType>::TMatrix(const TMatrix<ValType> &mt)
    : TVector<TVector<ValType>>(mt)
{
}

template <class ValType>
TMatrix<ValType>::TMatrix(const TVector<TVector<ValType>> &mt)
    : TVector<TVector<ValType>>(mt)
{
    if (this->Size > MAX_MATRIX_SIZE || this->StartIndex != 0) {
        throw std::invalid_argument("Invalid vector dimensions for upper triangular matrix conversion");
    }
    for (int i = 0; i < this->Size; ++i) {
        if (this->pVector[i].GetSize() != this->Size || this->pVector[i].GetStartIndex() != i) {
            throw std::invalid_argument("Row structure does not match upper triangular matrix layout");
        }
    }
}

template <class ValType>
bool TMatrix<ValType>::operator==(const TMatrix<ValType> &mt) const
{
    return TVector<TVector<ValType>>::operator==(mt);
}

template <class ValType>
bool TMatrix<ValType>::operator!=(const TMatrix<ValType> &mt) const
{
    return !(*this == mt);
}

template <class ValType>
TMatrix<ValType>& TMatrix<ValType>::operator=(const TMatrix<ValType> &mt)
{
    TVector<TVector<ValType>>::operator=(mt);
    return *this;
}

template <class ValType>
TMatrix<ValType> TMatrix<ValType>::operator+(const TMatrix<ValType> &mt) const
{
    return TVector<TVector<ValType>>::operator+(mt);
}

template <class ValType>
TMatrix<ValType> TMatrix<ValType>::operator-(const TMatrix<ValType> &mt) const
{
    return TVector<TVector<ValType>>::operator-(mt);
}

// Явное инстанцирование шаблонов для раздельной компиляции без изменения utmatrix.h
template class TVector<int>;
template class TVector<double>;
template class TVector<float>;
template class TVector<TVector<int>>;
template class TVector<TVector<double>>;
template class TVector<TVector<float>>;
template class TMatrix<int>;
template class TMatrix<double>;
template class TMatrix<float>;