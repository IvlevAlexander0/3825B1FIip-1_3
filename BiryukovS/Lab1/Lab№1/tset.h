#pragma once
#include <iostream>
#include "tbitfield.h"
#include <stdexcept>
#include <string>

class TSet
{
private:
    int MaxPower;       // максимальная мощность множества
    TBitField BitField; // битовое поле для хранения характеристического вектора
public:
    TSet(int mp);
    TSet(const TSet& s);       // конструктор копирования
    TSet(const TBitField& bf); // конструктор преобразования типа
    operator TBitField();      // преобразование типа к битовому полю
    // доступ к битам
    int GetMaxPower(void) const;     // максимальная мощность множества
    void InsElem(const int Elem);       // включить элемент в множество
    void DelElem(const int Elem);       // удалить элемент из множества
    int IsMember(const int Elem) const; // проверить наличие элемента в множестве
    // теоретико-множественные операции
    int operator== (const TSet& s) const; // сравнение
    int operator!= (const TSet& s) const; // сравнение
    TSet& operator=(const TSet& s);  // присваивание
    TSet operator+ (const int Elem); // объединение с элементом
    // элемент должен быть из того же универса
    TSet operator- (const int Elem); // разность с элементом
    // элемент должен быть из того же универса
    TSet operator+ (const TSet& s);  // объединение
    TSet operator* (const TSet& s);  // пересечение
    TSet operator~ (void);           // дополнение

    friend std::istream& operator>>(std::istream& istr, TSet& bf);
    friend std::ostream& operator<<(std::ostream& ostr, const TSet& bf);
};

inline TSet::TSet(int mp) : BitField(mp) {
    if (mp < 0) {
        throw std::invalid_argument("TSet: negative MaxPower");
    }
    MaxPower = mp;
}

inline TSet::TSet(const TSet& s) : BitField(s.BitField) {
    MaxPower = s.MaxPower;
}

inline TSet::TSet(const TBitField& bf) : BitField(bf) {
    MaxPower = bf.GetLength();
}

inline TSet::operator TBitField() {
    return BitField;
}

inline int TSet::GetMaxPower(void) const {
    return MaxPower;
}

inline void TSet::InsElem(const int Elem) {
    if (Elem < 0 || Elem >= MaxPower) {
        throw std::out_of_range("TSet::InsElem: element out of universe");
    }
    BitField.SetBit(Elem);
}

inline void TSet::DelElem(const int Elem) {
    if (Elem < 0 || Elem >= MaxPower) {
        throw std::out_of_range("TSet::DelElem: element out of universe");
    }
    BitField.ClrBit(Elem);
}

inline int TSet::IsMember(const int Elem) const {
    if (Elem < 0 || Elem >= MaxPower) {
        throw std::out_of_range("TSet::IsMember: element out of universe");
    }
    return BitField.GetBit(Elem);
}

inline int TSet::operator==(const TSet& s) const {
    if (MaxPower != s.MaxPower) {
        return 0;
    }
    return BitField == s.BitField;
}

inline int TSet::operator!=(const TSet& s) const {
    if (*this == s) {
        return 0;
    }
    else {
        return 1;
    }
}

inline TSet& TSet::operator=(const TSet& s) {
    if (this == &s) {
        return *this;
    }
    MaxPower = s.MaxPower;
    BitField = s.BitField;
    return *this;
}

inline TSet TSet::operator+(const int Elem) {
    if (Elem < 0 || Elem >= MaxPower) {
        throw std::out_of_range("TSet::operator+ (int): element out of universe");
    }
    TSet result(*this);
    result.InsElem(Elem);
    return result;
}

inline TSet TSet::operator-(const int Elem) {
    if (Elem < 0 || Elem >= MaxPower) {
        throw std::out_of_range("TSet::operator- (int): element out of universe");
    }
    TSet result(*this);
    result.DelElem(Elem);
    return result;
}

inline TSet TSet::operator+(const TSet& s) {
    int maxPow;
    if (MaxPower > s.MaxPower) {
        maxPow = MaxPower;
    }
    else {
        maxPow = s.MaxPower;
    }
    TSet result(maxPow);
    result.BitField = BitField | s.BitField;
    return result;
}

inline TSet TSet::operator*(const TSet& s) {
    int maxPow;
    if (MaxPower > s.MaxPower) {
        maxPow = MaxPower;
    }
    else {
        maxPow = s.MaxPower;
    }
    TSet result(maxPow);
    result.BitField = BitField & s.BitField;
    return result;
}

inline TSet TSet::operator~(void) {
    TSet result(MaxPower);
    result.BitField = ~BitField;
    return result;
}

inline std::ostream& operator<<(std::ostream& ostr, const TSet& s) {
    ostr << "{";
    int first = 1;
    for (int i = 0; i < s.MaxPower; i++) {
        if (s.BitField.GetBit(i) == 1) {
            if (first == 0) {
                ostr << ", ";
            }
            ostr << i;
            first = 0;
        }
    }
    ostr << "}";
    return ostr;
}

inline std::istream& operator>>(std::istream& istr, TSet& s) {
    std::string str;
    istr >> str;
    int i = 0;
    int n = (int)str.size();
    while (i < n && str[i] != '{') i++;
    if (i < n) i++;
    for (int k = 0; k < s.MaxPower; k++) {
        s.BitField.ClrBit(k);
    }
    while (i < n && str[i] != '}') {
        while (i < n && (str[i] == ',' || str[i] == ' ' || str[i] == '\t')) {
            i++;
        }
        if (i >= n || str[i] == '}') break;
        int value = 0;
        int hasDigits = 0;
        while (i < n && str[i] >= '0' && str[i] <= '9') {
            value = value * 10 + (str[i] - '0');
            i++;
            hasDigits = 1;
        }
        if (hasDigits) {
            if (value < 0 || value >= s.MaxPower) {
                throw std::out_of_range("TSet input: element out of universe");
            }
            s.BitField.SetBit(value);
        }
    }
    return istr;
}