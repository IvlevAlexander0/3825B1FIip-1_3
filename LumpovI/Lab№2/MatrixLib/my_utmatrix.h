#pragma once

#include <iostream>
#include <stdexcept>


const int MAX_VECTOR_SIZE = 100000000;
const int MAX_MATRIX_SIZE = 10000;

// Шаблон вектора
template <class ValType>
class TVector
{
protected:
    int Size;       // размер вектора
    int StartIndex; // индекс первого элемента вектора
    ValType* pVector;
public:
    TVector(int s = 10, int si = 0);
    TVector(const TVector& v);                // конструктор копирования
    ~TVector();
    int GetSize() const { return Size; }             // размер вектора
    int GetStartIndex() const { return StartIndex; } // индекс первого элемента
    ValType& operator[](int pos);             // доступ
    const ValType& operator[](int pos) const; // доступ
    bool operator==(const TVector& v) const;  // сравнение
    bool operator!=(const TVector& v) const;  // сравнение
    TVector& operator=(const TVector& v);     // присваивание

    // скалярные операции
    TVector  operator+(const ValType& val) const;   // прибавить скаляр
    TVector  operator-(const ValType& val) const;   // вычесть скаляр
    TVector  operator*(const ValType& val) const;   // умножить на скаляр

    // векторные операции
    TVector  operator+(const TVector& v) const;     // сложение
    TVector  operator-(const TVector& v) const;     // вычитание
    ValType  operator*(const TVector& v) const;     // скалярное произведение

    // ввод-вывод

    friend std::istream& operator>>(std::istream& in, TVector& v) {
        for (int i = 0; i < v.Size; ++i) in >> v.pVector[i];
        return in;
    }
    friend std::ostream& operator<<(std::ostream& out, const TVector& v) {
        for (int i = 0; i < v.Size; ++i) out << v.pVector[i] << " ";
        return out;
    }
};

// Верхнетреугольная матрица
template <class ValType>
class TMatrix : public TVector<TVector<ValType>>
{
public:
    TMatrix(int s = 10);
    TMatrix(const TMatrix& mt);                    // копирование
    TMatrix(const TVector<TVector<ValType>>& mt);  // преобразование типа
    bool operator==(const TMatrix& mt) const;      // сравнение
    bool operator!=(const TMatrix& mt) const;      // сравнение
    TMatrix& operator= (const TMatrix& mt);        // присваивание
    TMatrix  operator+ (const TMatrix& mt) const;        // сложение
    TMatrix  operator- (const TMatrix& mt) const;        // вычитание

    // ввод / вывод
    friend std::istream& operator>>(std::istream& in, TMatrix& mt) {
        for (int i = 0; i < mt.Size; ++i) {
            int start = mt.pVector[i].GetStartIndex();
            int end = start + mt.pVector[i].GetSize();
            for (int j = start; j < end; ++j) {
                in >> mt.pVector[i][j];
            }
        }
        return in;
    }
    friend std::ostream& operator<<(std::ostream& out, const TMatrix& mt) {
        for (int i = 0; i < mt.Size; ++i) {
            int start = mt.pVector[i].GetStartIndex();
            int end = start + mt.pVector[i].GetSize();
            for (int j = start; j < end; ++j) {
                out << mt.pVector[i][j] << " ";
            }
            out << "\n";
        }
        return out;
    }
};


// Реализация TVector

template <class ValType>
TVector<ValType>::TVector(int s, int si) : Size(s), StartIndex(si), pVector(nullptr) {
    if (s < 0 || s > MAX_VECTOR_SIZE) throw std::invalid_argument("Invalid vector size");
    if (s > 0) {
        pVector = new ValType[s];
        for (int i = 0; i < s; ++i) pVector[i] = ValType();
    }
}

template <class ValType>
TVector<ValType>::TVector(const TVector& v) : Size(v.Size), StartIndex(v.StartIndex), pVector(nullptr) {
    if (Size > 0) {
        pVector = new ValType[Size];
        for (int i = 0; i < Size; ++i) pVector[i] = v.pVector[i];
    }
}

template <class ValType>
TVector<ValType>::~TVector() {
    delete[] pVector;
}

template <class ValType>
ValType& TVector<ValType>::operator[](int pos) {
    if (pos < StartIndex || pos >= StartIndex + Size) throw std::out_of_range("Index out of range");
    return pVector[pos - StartIndex];
}

template <class ValType>
const ValType& TVector<ValType>::operator[](int pos) const {
    if (pos < StartIndex || pos >= StartIndex + Size) throw std::out_of_range("Index out of range");
    return pVector[pos - StartIndex];
}

template <class ValType>
bool TVector<ValType>::operator==(const TVector& v) const {
    if (Size != v.Size || StartIndex != v.StartIndex) return false;
    for (int i = 0; i < Size; ++i) {
        if (pVector[i] != v.pVector[i]) return false;
    }
    return true;
}

template <class ValType>
bool TVector<ValType>::operator!=(const TVector& v) const {
    return !(*this == v);
}

template <class ValType>
TVector<ValType>& TVector<ValType>::operator=(const TVector& v) {
    if (this == &v) return *this;
    delete[] pVector;
    Size = v.Size;
    StartIndex = v.StartIndex;
    pVector = (Size > 0) ? new ValType[Size] : nullptr;
    for (int i = 0; i < Size; ++i) pVector[i] = v.pVector[i];
    return *this;
}

template <class ValType>
TVector<ValType> TVector<ValType>::operator+(const ValType& val) const {
    TVector result(*this);
    for (int i = 0; i < Size; ++i) result.pVector[i] += val;
    return result;
}

template <class ValType>
TVector<ValType> TVector<ValType>::operator-(const ValType& val) const {
    TVector result(*this);
    for (int i = 0; i < Size; ++i) result.pVector[i] -= val;
    return result;
}

template <class ValType>
TVector<ValType> TVector<ValType>::operator*(const ValType& val) const {
    TVector result(*this);
    for (int i = 0; i < Size; ++i) result.pVector[i] *= val;
    return result;
}

template <class ValType>
TVector<ValType> TVector<ValType>::operator+(const TVector& v) const {
    if (Size != v.Size || StartIndex != v.StartIndex) throw std::invalid_argument("Vector sizes mismatch");
    TVector result(*this);
    for (int i = 0; i < Size; ++i) result.pVector[i] += v.pVector[i];
    return result;
}

template <class ValType>
TVector<ValType> TVector<ValType>::operator-(const TVector& v) const {
    if (Size != v.Size || StartIndex != v.StartIndex) throw std::invalid_argument("Vector sizes mismatch");
    TVector result(*this);
    for (int i = 0; i < Size; ++i) result.pVector[i] -= v.pVector[i];
    return result;
}

template <class ValType>
ValType TVector<ValType>::operator*(const TVector& v) const {
    if (Size != v.Size || StartIndex != v.StartIndex) throw std::invalid_argument("Vector sizes mismatch");
    ValType result = ValType();
    for (int i = 0; i < Size; ++i) result += pVector[i] * v.pVector[i];
    return result;
}


// Реализация TMatrix

template <class ValType>
TMatrix<ValType>::TMatrix(int s) : TVector<TVector<ValType>>(s) {
    if (s < 0 || s > MAX_MATRIX_SIZE) throw std::invalid_argument("Invalid matrix size");
    for (int i = 0; i < s; ++i) {
        this->pVector[i] = TVector<ValType>(s - i, i);
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
    TVector<TVector<ValType>>::operator=(mt);
    return *this;
}

template <class ValType>
TMatrix<ValType> TMatrix<ValType>::operator+(const TMatrix& mt) const {
    TMatrix result(*this);
    for (int i = 0; i < this->Size; ++i) {
        result.pVector[i] = this->pVector[i] + mt.pVector[i];
    }
    return result;
}

template <class ValType>
TMatrix<ValType> TMatrix<ValType>::operator-(const TMatrix& mt) const {
    TMatrix result(*this);
    for (int i = 0; i < this->Size; ++i) {
        result.pVector[i] = this->pVector[i] - mt.pVector[i];
    }
    return result;
}
