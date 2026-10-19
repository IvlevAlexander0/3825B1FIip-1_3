#include "tbitfield.h"
#include <cstring>
#include <stdexcept>

int TBitField::GetMemIndex(const int n) const {
    return n / static_cast<int>(sizeof(TELEM) * 8);
}

TELEM TBitField::GetMemMask(const int n) const {
    return static_cast<TELEM>(1U << (n % (sizeof(TELEM) * 8)));
}

TBitField::TBitField(int len): BitLen(len), pMem(nullptr), MemLen(0)
{
    if (len <= 0) {
        throw std::invalid_argument("TBitField: length should be positive");
    }
    const int bitsPerElem = static_cast<int>(sizeof(TELEM) * 8);
    MemLen = (len + bitsPerElem - 1) / bitsPerElem;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; ++i) {
        pMem[i] = 0;
    }
}

TBitField::TBitField(const TBitField& bf): BitLen(bf.BitLen), pMem(nullptr), MemLen(bf.MemLen)
{
    if (MemLen > 0) {
        pMem = new TELEM[MemLen];
        for (int i = 0; i < MemLen; ++i) {
            pMem[i] = bf.pMem[i];
        }
    }
}

TBitField::~TBitField() {
    delete[] pMem;
}

int TBitField::GetLength() const {
    return BitLen;
}

void TBitField::SetBit(const int n) {
    if (n < 0 || n >= BitLen) {
        throw std::out_of_range("TBitField::SetBit: index is out of range");
    }
    pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) {
    if (n < 0 || n >= BitLen) {
        throw std::out_of_range("TBitField::ClrBit: index is out of range");
    }
    pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const {
    if (n < 0 || n >= BitLen) {
        throw std::out_of_range("TBitField::GetBit: index is out of range");
    }
    return (pMem[GetMemIndex(n)] & GetMemMask(n)) ? 1 : 0;
}

TBitField& TBitField::operator=(const TBitField& bf) {
    if (this == &bf) return *this;
    if (MemLen != bf.MemLen) {
        delete[] pMem;
        MemLen = bf.MemLen;
        pMem = (MemLen > 0) ? new TELEM[MemLen] : nullptr;
    }
    BitLen = bf.BitLen;
    for (int i = 0; i < MemLen; ++i) {
        pMem[i] = bf.pMem[i];
    }
    return *this;
}

int TBitField::operator==(const TBitField& bf) const {
    if (BitLen != bf.BitLen) return 0;
    if (MemLen != bf.MemLen) return 0;

    for (int i = 0; i < MemLen - 1; ++i) {
        if (pMem[i] != bf.pMem[i]) return 0;
    }

    if (MemLen > 0) {
        int bitsInLast = BitLen % static_cast<int>(sizeof(TELEM) * 8);
        if (bitsInLast == 0) {
            if (pMem[MemLen - 1] != bf.pMem[MemLen - 1]) return 0;
        }
        else {
            TELEM mask = (1U << bitsInLast) - 1;
            if ((pMem[MemLen - 1] & mask) != (bf.pMem[MemLen - 1] & mask)) return 0;
        }
    }
    return 1;
}

int TBitField::operator!=(const TBitField& bf) const {
    return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField& bf) {
    if (BitLen != bf.BitLen) {
        throw std::invalid_argument("TBitField::operator|: different lenght of field");
    }
    TBitField result(BitLen);
    for (int i = 0; i < MemLen; ++i) {
        result.pMem[i] = pMem[i] | bf.pMem[i];
    }
    return result;
}

TBitField TBitField::operator&(const TBitField& bf) {
    if (BitLen != bf.BitLen) {
        throw std::invalid_argument("TBitField::operator&: different lenght of field");
    }
    TBitField result(BitLen);
    for (int i = 0; i < MemLen; ++i) {
        result.pMem[i] = pMem[i] & bf.pMem[i];
    }
    return result;
}

TBitField TBitField::operator~() {
    TBitField result(*this);
    for (int i = 0; i < MemLen; ++i) {
        result.pMem[i] = ~result.pMem[i];
    }
    int bitsInLast = BitLen % static_cast<int>(sizeof(TELEM) * 8);
    if (bitsInLast != 0 && MemLen > 0) {
        TELEM mask = (1U << bitsInLast) - 1;
        result.pMem[MemLen - 1] &= mask;
    }
    return result;
}

std::istream& operator>>(std::istream& istr, TBitField& bf) {
    for (int i = 0; i < bf.BitLen; ++i) {
        char c;
        istr >> c;
        if (istr.fail()) return istr;
        if (c == '1') bf.SetBit(i);
        else if (c == '0') bf.ClrBit(i);
        else {
            istr.setstate(std::ios::failbit);
            return istr;
        }
    }
    return istr;
}

std::ostream& operator<<(std::ostream& ostr, const TBitField& bf) {
    for (int i = 0; i < bf.BitLen; ++i) {
        ostr << bf.GetBit(i);
    }
    return ostr;
}