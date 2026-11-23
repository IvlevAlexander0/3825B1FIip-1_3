#pragma once

#include <iostream>
#include <stdexcept>

const int MAX_VECTOR_SIZE = 100000000;
const int MAX_MATRIX_SIZE = 10000;

// Ўаблон вектора
template <class ValType>
class TVector
{
protected:
    int Size;       // размер вектора
    int StartIndex; // индекс первого элемента вектора
    ValType* pVector;
public:
    TVector(int s = 10, int si = 0);
    TVector(const TVector& v);                // конструктор копировани€
    ~TVector();
    int GetSize() const { return Size; }             // размер вектора
    int GetStartIndex() const { return StartIndex; } // индекс первого элемента
    ValType& operator[](int pos);             // доступ
    const ValType& operator[](int pos) const; // доступ
    bool operator==(const TVector& v) const;  // сравнение
    bool operator!=(const TVector& v) const;  // сравнение
    TVector& operator=(const TVector& v);     // присваивание

    // скал€рные операции
    TVector  operator+(const ValType& val) const;   // прибавить скал€р
    TVector  operator-(const ValType& val) const;   // вычесть скал€р
    TVector  operator*(const ValType& val) const;   // умножить на скал€р

    // векторные операции
    TVector  operator+(const TVector& v) const;     // сложение
    TVector  operator-(const TVector& v) const;     // вычитание
    ValType  operator*(const TVector& v) const;     // скал€рное произведение

    // ввод-вывод
    friend std::istream& operator>>(std::istream& in, TVector& v) {
        int size = v.GetSize();
        int startIndex = v.GetStartIndex();
        TVector tmp(size, startIndex);
        for (int i = startIndex; i < size; ++i) {
            in >> tmp[i];
        }

        if (!in.fail()) {
            v = tmp;
        }
        return in;
    }

    friend std::ostream& operator<<(std::ostream& out, const TVector& v) {
        for (int i = 0; i < v.GetSize(); ++i) {
            if (i > 0) {
                out << ' ';
            }
            if (i < v.GetStartIndex()) {
                out << ValType{};
            }
            else {
                out << v[i];
            }
        }
        return out;
    }
};

// ¬ерхнетреугольна€ матрица
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
        int size = mt.GetSize();
        TMatrix tmp(size);
        for (int i = 0; i < size; ++i) {
            in >> tmp[i];
        }

        if (!in.fail()) {
            mt = tmp;
        }
        return in;
    }

    friend std::ostream& operator<<(std::ostream& out, const TMatrix& mt) {
        for (int i = 0; i < mt.GetSize(); ++i) {
            if (i > 0) {
                out << '\n';
            }
            out << mt[i];
        }
        return out;
    }
};

template <class ValType>
TVector<ValType>::TVector(int s, int si) {
    if (s < 0 || s > MAX_VECTOR_SIZE) throw std::invalid_argument("Invalid size");
    Size = s;
    StartIndex = si;
    pVector = new ValType[Size];
    for (int i = 0; i < Size; ++i) {
        pVector[i] = ValType(0);
    }
}

template <class ValType>
TVector<ValType>::TVector(const TVector& v) {
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
    if (Size != v.Size) {
        delete[] pVector;
        Size = v.Size;
        pVector = new ValType[Size];
    }
    StartIndex = v.StartIndex;
    for (int i = 0; i < Size; ++i) {
        pVector[i] = v.pVector[i];
    }
    return *this;
}

template <class ValType>
TVector<ValType> TVector<ValType>::operator+(const ValType& val) const {
    TVector res(*this);
    for (int i = 0; i < Size; ++i) res.pVector[i] += val;
    return res;
}

template <class ValType>
TVector<ValType> TVector<ValType>::operator-(const ValType& val) const {
    TVector res(*this);
    for (int i = 0; i < Size; ++i) res.pVector[i] -= val;
    return res;
}

template <class ValType>
TVector<ValType> TVector<ValType>::operator*(const ValType& val) const {
    TVector res(*this);
    for (int i = 0; i < Size; ++i) res.pVector[i] *= val;
    return res;
}

template <class ValType>
TVector<ValType> TVector<ValType>::operator+(const TVector& v) const {
    if (Size != v.Size || StartIndex != v.StartIndex) {
        throw std::invalid_argument("Sizes or start indices do not match");
    }
    TVector res(*this);
    for (int i = 0; i < Size; ++i) res.pVector[i] += v.pVector[i];
    return res;
}

template <class ValType>
TVector<ValType> TVector<ValType>::operator-(const TVector& v) const {
    if (Size != v.Size || StartIndex != v.StartIndex) {
        throw std::invalid_argument("Sizes or start indices do not match");
    }
    TVector res(*this);
    for (int i = 0; i < Size; ++i) res.pVector[i] -= v.pVector[i];
    return res;
}

template <class ValType>
ValType TVector<ValType>::operator*(const TVector& v) const {
    if (Size != v.Size || StartIndex != v.StartIndex) {
        throw std::invalid_argument("Sizes or start indices do not match");
    }
    ValType res = ValType(0);
    for (int i = 0; i < Size; ++i) res += pVector[i] * v.pVector[i];
    return res;
}

template <class ValType>
TMatrix<ValType>::TMatrix(int s) : TVector<TVector<ValType>>(s) {
    if (s < 0 || s > MAX_MATRIX_SIZE) throw std::invalid_argument("Invalid size");
    for (int i = 0; i < s; ++i) {
        this->pVector[i] = TVector<ValType>(s - i, 0); // ќЎ»Ѕ ј «ƒ≈—№: должно быть i вместо 0
    }
}

template <class ValType>
TMatrix<ValType>::TMatrix(const TMatrix& mt) : TVector<TVector<ValType>>(mt) {}

template <class ValType>
TMatrix<ValType>::TMatrix(const TVector<TVector<ValType>>& mt) : TVector<TVector<ValType>>(mt) {
    if (mt.GetSize() > MAX_MATRIX_SIZE) throw std::invalid_argument("Invalid size");
}

template <class ValType>
bool TMatrix<ValType>::operator==(const TMatrix& mt) const {
    if (this->Size != mt.Size) return false;
    for (int i = 0; i < this->Size; ++i) {
        if (this->pVector[i] != mt.pVector[i]) return false;
    }
    return true;
}

template <class ValType>
bool TMatrix<ValType>::operator!=(const TMatrix& mt) const {
    return !(*this == mt);
}

template <class ValType>
TMatrix<ValType>& TMatrix<ValType>::operator=(const TMatrix& mt) {
    if (this == &mt) return *this;
    TVector<TVector<ValType>>::operator=(mt);
    return *this;
}

template <class ValType>
TMatrix<ValType> TMatrix<ValType>::operator+(const TMatrix& mt) const {
    if (this->Size != mt.Size) throw std::invalid_argument("Sizes do not match");
    TMatrix res(*this);
    for (int i = 0; i < this->Size; ++i) {
        res.pVector[i] = this->pVector[i] + mt.pVector[i];
    }
    return res;
}

template <class ValType>
TMatrix<ValType> TMatrix<ValType>::operator-(const TMatrix& mt) const {
    if (this->Size != mt.Size) throw std::invalid_argument("Sizes do not match");
    TMatrix res(*this);
    for (int i = 0; i < this->Size; ++i) {
        res.pVector[i] = this->pVector[i] - mt.pVector[i];
    }
    return res;
}
