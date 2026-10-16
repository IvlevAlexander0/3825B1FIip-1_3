#pragma once
#include "../../../../includes/Lab1/tbitfield.h"
#include <stdexcept>
#define BITS_PER_CHUNK (sizeof(TELEM) * 8)


TBitField::TBitField(int len): BitLen(len), pMem(nullptr), MemLen(0)  {
    if (len < 0) {
        throw std::invalid_argument("TBitField: length cannot be negative");
    }

    MemLen = len / BITS_PER_CHUNK + (len % BITS_PER_CHUNK != 0);

    if (MemLen > 0) {
        pMem = new TELEM[MemLen]();
    }
}


TBitField::TBitField(const TBitField& bf): BitLen(bf.BitLen), pMem(nullptr), MemLen(bf.MemLen) {
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


int TBitField::GetMemIndex(const int n) const {
    return n / BITS_PER_CHUNK;
}


TELEM TBitField::GetMemMask(const int n) const {
    return TELEM(1) << (n % BITS_PER_CHUNK);
}


int TBitField::GetLength(void) const {
    return BitLen;
}


void TBitField::SetBit(const int n) {
    if (n >= 0 && n < BitLen) {
        pMem[GetMemIndex(n)] |= GetMemMask(n);
    }
}


void TBitField::ClrBit(const int n) {
    if (n >= 0 && n < BitLen) {
        pMem[GetMemIndex(n)] &= ~GetMemMask(n);
    }
}


int TBitField::GetBit(const int n) const {
    if (n < 0 || n >= BitLen) {
        throw std::out_of_range("TBitField: bit does not exist");
    }

    return (pMem[GetMemIndex(n)] & GetMemMask(n)) != 0;
}


int TBitField::operator==(const TBitField& bf) const {
    if (BitLen != bf.BitLen){
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

    TELEM* newMem = nullptr;

    if (bf.MemLen > 0) {
        newMem = new TELEM[bf.MemLen];
        for (int i = 0; i < bf.MemLen; ++i) {
            newMem[i] = bf.pMem[i];
        }
    }

    delete[] pMem;
    pMem = newMem;
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    return *this;
}


TBitField TBitField::operator|(const TBitField& bf) {
    if (BitLen != bf.BitLen) {
        throw std::invalid_argument("TBitField: different lengths");
    }

    TBitField result(BitLen);

    for (int i = 0; i < MemLen; ++i) {
        result.pMem[i] = pMem[i] | bf.pMem[i];
    }

    return result;
}


TBitField TBitField::operator&(const TBitField& bf) {
    if (BitLen != bf.BitLen) {
        throw std::invalid_argument("TBitField: different lengths");
    }

    TBitField result(BitLen);

    for (int i = 0; i < MemLen; ++i) {
        result.pMem[i] = pMem[i] & bf.pMem[i];
    }

    return result;
}


TBitField TBitField::operator~(void) {
    TBitField result(BitLen);

    for (int i = 0; i < MemLen; ++i) {
        result.pMem[i] = ~pMem[i];
    }

    if (BitLen > 0 && BitLen % BITS_PER_CHUNK != 0) {
        int used_bits = BitLen % BITS_PER_CHUNK;
        TELEM mask = (TELEM(1) << used_bits) - 1;
        result.pMem[MemLen - 1] &= mask;
    }

    return result;
}


std::istream& operator>>(std::istream& istr, TBitField& bf) {
    for (int n = 0; n < bf.BitLen; ++n) {
        char ch;
        if (!(istr >> ch)) {
            return istr;
        }

        if (ch != '0' && ch != '1') {
            istr.setstate(std::ios::failbit);
            return istr;
        }

        if (ch == '1') {
            bf.SetBit(n);
        }
        else {
            bf.ClrBit(n);
        }
    }

    return istr;
}


std::ostream& operator<<(std::ostream& ostr, const TBitField& bf) {
    for (int n = 0; n < bf.BitLen; ++n) {
        ostr << (bf.GetBit(n) ? '1' : '0');
    }

    return ostr;
}