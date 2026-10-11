#pragma once

#include <iostream>
#include <stdexcept>
#include <string>
typedef unsigned int TELEM;

class TBitField
{
private:
    int  BitLen; // длина битового поля - макс. к-во битов
    TELEM* pMem; // память для представления битового поля
    int  MemLen; // к-во эл-тов Мем для представления бит.поля

    // методы реализации
    int   GetMemIndex(const int n) const; // индекс в pМем для бита n       (#О2)
    TELEM GetMemMask(const int n) const; // битовая маска для бита n       (#О3)
public:
    TBitField(int len);                //                                   (#О1)
    TBitField(const TBitField& bf);    //                                   (#П1)
    ~TBitField();                      //                                    (#С)

    // доступ к битам
    int GetLength(void) const;      // получить длину (к-во битов)           (#О)
    void SetBit(const int n);       // установить бит                       (#О4)
    void ClrBit(const int n);       // очистить бит                         (#П2)
    int  GetBit(const int n) const; // получить значение бита               (#Л1)

    // битовые операции
    int operator==(const TBitField& bf) const; // сравнение                 (#О5)
    int operator!=(const TBitField& bf) const; // сравнение
    TBitField& operator=(const TBitField& bf); // присваивание              (#П3)
    TBitField  operator|(const TBitField& bf); // операция "или"            (#О6)
    TBitField  operator&(const TBitField& bf); // операция "и"              (#Л2)
    TBitField  operator~(void);                // отрицание                  (#С)

    friend std::istream& operator>>(std::istream& istr, TBitField& bf);       //      (#О7)
    friend std::ostream& operator<<(std::ostream& ostr, const TBitField& bf); //      (#П4)
};

inline int TBitField::GetMemIndex(const int n) const {
    return n / 32;
}

inline TELEM TBitField::GetMemMask(const int n) const {
    int offset = n % 32;
    TELEM mask = (TELEM)1 << offset;
    return mask;
}

inline TBitField::TBitField(int len) {
    if (len < 0) {
        throw std::invalid_argument("TBitField: negative length");
    }
    BitLen = len;
    MemLen = (len + 31) / 32;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++) {
        pMem[i] = 0;
    }
}

inline TBitField::TBitField(const TBitField& bf) {
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++) {
        pMem[i] = bf.pMem[i];
    }
}

inline TBitField::~TBitField() {
    delete[] pMem;
}

inline int TBitField::GetLength(void) const {
    return BitLen;
}

inline void TBitField::SetBit(const int n) {
    if (n < 0 || n >= BitLen) {
        throw std::out_of_range("TBitField::SetBit: index out of range");
    }
    int idx = GetMemIndex(n);
    TELEM mask = GetMemMask(n);
    pMem[idx] = pMem[idx] | mask;
}

inline void TBitField::ClrBit(const int n) {
    if (n < 0 || n >= BitLen) {
        throw std::out_of_range("TBitField::ClrBit: index out of range");
    }
    int idx = GetMemIndex(n);
    TELEM mask = GetMemMask(n);
    pMem[idx] = pMem[idx] & ~mask;
}

inline int TBitField::GetBit(const int n) const {
    if (n < 0 || n >= BitLen) {
        throw std::out_of_range("TBitField::GetBit: index out of range");
    }
    int idx = GetMemIndex(n);
    TELEM mask = GetMemMask(n);
    TELEM result = pMem[idx] & mask;
    if (result != 0) {
        return 1;
    }
    else {
        return 0;
    }
}

inline int TBitField::operator==(const TBitField& bf) const {
    if (BitLen != bf.BitLen) {
        return 0;
    }
    for (int i = 0; i < MemLen; i++) {
        if (pMem[i] != bf.pMem[i]) {
            return 0;
        }
    }
    return 1;
}

inline int TBitField::operator!=(const TBitField& bf) const {
    if (*this == bf) {
        return 0;
    }
    else {
        return 1;
    }
}

inline TBitField& TBitField::operator=(const TBitField& bf) {
    if (this == &bf) {
        return *this;
    }
    if (MemLen != bf.MemLen) {
        delete[] pMem;
        MemLen = bf.MemLen;
        pMem = new TELEM[MemLen];
    }
    BitLen = bf.BitLen;
    for (int i = 0; i < MemLen; i++) {
        pMem[i] = bf.pMem[i];
    }
    return *this;
}

inline TBitField TBitField::operator|(const TBitField& bf) {
    int maxLen;
    if (BitLen > bf.BitLen) {
        maxLen = BitLen;
    }
    else {
        maxLen = bf.BitLen;
    }
    TBitField result(maxLen);
    for (int i = 0; i < MemLen; i++) {
        result.pMem[i] = pMem[i];
    }
    for (int i = 0; i < bf.MemLen; i++) {
        result.pMem[i] = result.pMem[i] | bf.pMem[i];
    }
    return result;
}

inline TBitField TBitField::operator&(const TBitField& bf) {
    int maxLen;
    if (BitLen > bf.BitLen) {
        maxLen = BitLen;
    }
    else {
        maxLen = bf.BitLen;
    }
    TBitField result(maxLen);
    for (int i = 0; i < MemLen; i++) {
        result.pMem[i] = pMem[i];
    }
    for (int i = 0; i < bf.MemLen; i++) {
        result.pMem[i] = result.pMem[i] & bf.pMem[i];
    }
    return result;
}

inline TBitField TBitField::operator~(void) {
    TBitField result(BitLen);
    for (int i = 0; i < MemLen; i++) {
        result.pMem[i] = ~pMem[i];
    }
    int tail = BitLen % 32;
    if (tail != 0) {
        TELEM mask = (1u << tail) - 1;
        result.pMem[MemLen - 1] = result.pMem[MemLen - 1] & mask;
    }
    return result;
}

inline std::ostream& operator<<(std::ostream& ostr, const TBitField& bf) {
    for (int i = 0; i < bf.BitLen; i++) {
        ostr << bf.GetBit(i);
    }
    return ostr;
}

inline std::istream& operator>>(std::istream& istr, TBitField& bf) {
    std::string s;
    istr >> s;
    int len = (int)s.size();
    if (len != bf.BitLen) {
        delete[] bf.pMem;
        bf.BitLen = len;
        bf.MemLen = (len + 31) / 32;
        bf.pMem = new TELEM[bf.MemLen];
        for (int i = 0; i < bf.MemLen; i++) {
            bf.pMem[i] = 0;
        }
    }
    else {
        for (int i = 0; i < bf.MemLen; i++) {
            bf.pMem[i] = 0;
        }
    }
    for (int i = 0; i < len; i++) {
        if (s[i] == '1') {
            bf.SetBit(i);
        }
        else if (s[i] != '0') {
            throw std::invalid_argument("TBitField input: only 0/1 allowed");
        }
    }
    return istr;
}