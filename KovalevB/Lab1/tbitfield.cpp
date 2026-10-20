#include "tbitfield.h"
#include <stdexcept>
#include <algorithm>

const int BITS_IN_TELEM = sizeof(TELEM) * 8;

int TBitField::GetMemIndex(const int n) const {
    if (n < 0 || n >= BitLen) {
        throw std::out_of_range("Bit index out of range");
    }
    return n / BITS_IN_TELEM;
}

TELEM TBitField::GetMemMask(const int n) const {
    if (n < 0 || n >= BitLen) {
        throw std::out_of_range("Bit index out of range");
    }
    return 1u << (n % BITS_IN_TELEM);
}

TBitField::TBitField(int len) {
    if (len <= 0) {
        throw std::invalid_argument("Bit field length must be positive");
    }
    BitLen = len;
    
    MemLen = (len + BITS_IN_TELEM - 1) / BITS_IN_TELEM;
    
    pMem = new TELEM[MemLen]();
}

TBitField::TBitField(const TBitField &bf) {
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;

    pMem = new TELEM[MemLen];
    std::copy(bf.pMem, bf.pMem + MemLen, pMem);
}

TBitField::~TBitField() {
    delete[] pMem;
}

int TBitField::GetLength(void) const {
    return BitLen;
}

void TBitField::SetBit(const int n) {
    pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) {
    pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const {
    return (pMem[GetMemIndex(n)] & GetMemMask(n)) != 0;
}

int TBitField::operator==(const TBitField &bf) const {
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

int TBitField::operator!=(const TBitField &bf) const {
    return !(*this == bf);
}

TBitField& TBitField::operator=(const TBitField &bf) {
    if (this == &bf) {
        return *this;
    }
    if (MemLen != bf.MemLen) {
        delete[] pMem;
        MemLen = bf.MemLen;
        pMem = new TELEM[MemLen];
    }
    BitLen = bf.BitLen;

    std::copy(bf.pMem, bf.pMem + MemLen, pMem);
    return *this;
}

TBitField TBitField::operator|(const TBitField &bf) {
    int len = BitLen > bf.BitLen ? BitLen : bf.BitLen;
    TBitField temp(len);

    for (int i = 0; i < MemLen; i++) {
        temp.pMem[i] = pMem[i];
    }

    for (int i = 0; i < bf.MemLen; i++) {
        temp.pMem[i] |= bf.pMem[i];
    }
    return temp;
}

TBitField TBitField::operator&(const TBitField &bf) {
    int len = BitLen > bf.BitLen ? BitLen : bf.BitLen;
    TBitField temp(len);

    int minMemLen = MemLen < bf.MemLen ? MemLen : bf.MemLen;
    for (int i = 0; i < minMemLen; i++) {
        temp.pMem[i] = pMem[i] & bf.pMem[i];
    }
    return temp;
}

TBitField TBitField::operator~(void) {
    TBitField temp(BitLen);

    for (int i = 0; i < MemLen; i++) {
        temp.pMem[i] = ~pMem[i];
    }

    int lastBits = BitLen % BITS_IN_TELEM;
    if (lastBits != 0) {
        TELEM mask = (1u << lastBits) - 1;
        temp.pMem[MemLen - 1] &= mask;
    }
    return temp;
}

std::istream &operator>>(std::istream &istr, TBitField &bf) {
    std::fill(bf.pMem, bf.pMem + bf.MemLen, 0);

    char ch;
    if (!(istr >> ch)) {
        return istr;
    }
    istr.putback(ch);

    for (int i = 0; i < bf.BitLen; i++) {
        if (!istr.get(ch)) {
            break;
        }
        if (ch == '1') {
            bf.SetBit(i);
        } else if (ch == '0') {
            bf.ClrBit(i);
        } else {
            istr.unget();
            break;
        }
    }
    return istr;
}

std::ostream &operator<<(std::ostream &ostr, const TBitField &bf) {
    for (int i = 0; i < bf.BitLen; i++) {
        ostr << bf.GetBit(i);
    }
    return ostr;
}