#include "../../../includes/Lab1/tbitfield.h"
#include <iostream>
#include <stdexcept>
#include <string>

const int unint = sizeof(unsigned int) * 8;

int TBitField::GetMemIndex(const int n) const {
    return n / unint;
}

TELEM TBitField::GetMemMask(const int n) const {
    int offset = n % unint;
    TELEM mask = (TELEM)1u << offset;
    return mask;
}

TBitField::TBitField(int len) {
    if (len < 0) {
        throw std::invalid_argument("TBitField: negative length");
    }
    BitLen = len;
    MemLen = (len + unint - 1) / unint;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++) {
        pMem[i] = 0;
    }
}

TBitField::TBitField(const TBitField& bf) {
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++) {
        pMem[i] = bf.pMem[i];
    }
}

TBitField::~TBitField() {
    delete[] pMem;
}

int TBitField::GetLength(void) const {
    return BitLen;
}

void TBitField::SetBit(const int n) {
    if (n < 0 || n >= BitLen) {
        throw std::out_of_range("TBitField::SetBit: index out of range");
    }
    int idx = GetMemIndex(n);
    TELEM mask = GetMemMask(n);
    pMem[idx] = pMem[idx] | mask;
}

void TBitField::ClrBit(const int n) {
    if (n < 0 || n >= BitLen) {
        throw std::out_of_range("TBitField::ClrBit: index out of range");
    }
    int idx = GetMemIndex(n);
    TELEM mask = GetMemMask(n);
    pMem[idx] = pMem[idx] & ~mask;
}

int TBitField::GetBit(const int n) const {
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

int TBitField::operator==(const TBitField& bf) const {
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

int TBitField::operator!=(const TBitField& bf) const {
    if (*this == bf) {
        return 0;
    }
    else {
        return 1;
    }
}

TBitField& TBitField::operator=(const TBitField& bf) {
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

TBitField TBitField::operator|(const TBitField& bf) {
    if (BitLen != bf.BitLen) {
        throw std::invalid_argument("Sizes must be the same");
    }
    TBitField result(BitLen);
    for (int i = 0; i < MemLen; i++) {
        result.pMem[i] = pMem[i] | bf.pMem[i];
    }
    return result;
}

TBitField TBitField::operator&(const TBitField& bf) {
    if (BitLen != bf.BitLen) {
        throw std::invalid_argument("Sizes must be the same");
    }
    TBitField result(BitLen);
    for (int i = 0; i < MemLen; i++) {
        result.pMem[i] = pMem[i] & bf.pMem[i];
    }
    return result;
}

TBitField TBitField::operator~(void) {
    TBitField result(BitLen);
    for (int i = 0; i < MemLen; i++) {
        result.pMem[i] = ~pMem[i];
    }
    int tail = BitLen % unint;
    if (tail != 0) {
        TELEM mask = (1u << tail) - 1;
        result.pMem[MemLen - 1] = result.pMem[MemLen - 1] & mask;
    }
    return result;
}

std::ostream& operator<<(std::ostream& ostr, const TBitField& bf) {
    for (int i = 0; i < bf.BitLen; i++) {
        ostr << bf.GetBit(i);
    }
    return ostr;
}

std::istream& operator>>(std::istream& istr, TBitField& bf) {
    std::string s;
    istr >> s;
    int len = (int)s.size();
    if (len != bf.BitLen) {
        throw std::invalid_argument("Input length does not match BitField length");
    }
    for (int i = 0; i < bf.MemLen; i++) {
        bf.pMem[i] = 0;
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