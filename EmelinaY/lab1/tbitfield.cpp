#include "tbitfield.h"
#include <stdexcept>
#include <string>

int TBitField::GetMemIndex(const int n) const {
    return n / (int)(sizeof(TELEM) * 8);
}


TELEM TBitField::GetMemMask(const int n) const {
    int bitPos = n % (int)(sizeof(TELEM) * 8);
    return static_cast<TELEM>(1u) << bitPos;
}

TBitField::TBitField(int len) {
    if (len < 0) {
        throw std::invalid_argument("Length cannot be negative");
    }
    BitLen = len;
    MemLen = (len + (int)(sizeof(TELEM) * 8) - 1) / (int)(sizeof(TELEM) * 8);
    if (MemLen > 0) {
        pMem = new TELEM[MemLen];
        for (int i = 0; i < MemLen; ++i) {
            pMem[i] = 0u;
        }
    }
    else {
        pMem = nullptr;
    }
}

TBitField::TBitField(const TBitField& bf) {
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    if (MemLen > 0) {
        pMem = new TELEM[MemLen];
        for (int i = 0; i < MemLen; ++i) {
            pMem[i] = bf.pMem[i];
        }
    }
    else {
        pMem = nullptr;
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
        throw std::out_of_range("Index out of range");
    }
    pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) {
    if (n < 0 || n >= BitLen) {
        throw std::out_of_range("Index out of range");
    }
    pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const {
    if (n < 0 || n >= BitLen) {
        throw std::out_of_range("Index out of range");
    }
    if (pMem[GetMemIndex(n)] & GetMemMask(n)) {
        return 1;
    }
    return 0;
}

int TBitField::operator==(const TBitField& bf) const {
    if (BitLen != bf.BitLen) {
        return 0;
    }
    for (int i = 0; i < MemLen; ++i) {
        if (pMem[i] != bf.pMem[i]) {
            return 0;
        }
    }
    return 1;
}

int TBitField::operator!=(const TBitField& bf) const {
    return !(*this == bf);
}

TBitField& TBitField::operator=(const TBitField& bf) {
    if (this == &bf) {
        return *this;
    }
    delete[] pMem;
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    if (MemLen > 0) {
        pMem = new TELEM[MemLen];
        for (int i = 0; i < MemLen; ++i) {
            pMem[i] = bf.pMem[i];
        }
    }
    else {
        pMem = nullptr;
    }
    return *this;
}

TBitField TBitField::operator|(const TBitField& bf) {
    if (BitLen != bf.BitLen) {
        throw std::invalid_argument("operator|: different lengths");
    }
    TBitField result(BitLen);
    for (int i = 0; i < MemLen; ++i) {
        result.pMem[i] = pMem[i] | bf.pMem[i];
    }
    return result;
}

TBitField TBitField::operator&(const TBitField& bf) {
    if (BitLen != bf.BitLen) {
        throw std::invalid_argument("operator&: different lengths");
    }
    TBitField result(BitLen);
    for (int i = 0; i < MemLen; ++i) {
        result.pMem[i] = pMem[i] & bf.pMem[i];
    }
    return result;
}

TBitField TBitField::operator~() {
    TBitField result(BitLen);
    for (int i = 0; i < MemLen; ++i) {
        result.pMem[i] = ~pMem[i];
    }
    
    int bitsInLast = BitLen % (int)(sizeof(TELEM) * 8);
    if (bitsInLast != 0 && MemLen > 0) {
        TELEM mask = (static_cast<TELEM>(1u) << bitsInLast) - 1u;
        result.pMem[MemLen - 1] &= mask;
    }
    return result;
}

std::istream& operator>>(std::istream& istr, TBitField& bf) {
    std::string s;
    istr >> s;

    for (int i = 0; i < (int)s.length(); ++i) {
        if (s[i] != '0' && s[i] != '1') {
            throw std::invalid_argument("Invalid character");
        }
    }

    for (int i = 0; i < bf.MemLen; ++i) {
        bf.pMem[i] = 0u;
    }
    int len = (int)s.length();
    if (len > bf.BitLen) {
        len = bf.BitLen;
    }
    for (int i = 0; i < len; ++i) {
        if (s[i] == '1') {
            bf.SetBit(i);
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