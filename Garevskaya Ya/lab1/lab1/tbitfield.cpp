#include "tbitfield.h"
#include <stdexcept>
#include <string>

int TBitField::GetMemIndex(const int n) const {
    return n / (sizeof(TELEM) * 8);
}

TELEM TBitField::GetMemMask(const int n) const {
    return 1u << (n % (sizeof(TELEM) * 8));
}

TBitField::TBitField(int len) {
    if (len < 0) throw std::invalid_argument("Length cannot be negative");
    BitLen = len;
    int bitsPerElem = sizeof(TELEM) * 8;
    MemLen = (len + bitsPerElem - 1) / bitsPerElem;
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
    if (n < 0 || n >= BitLen) return;
    pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) {
    if (n < 0 || n >= BitLen) return;
    pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const {
    if (n < 0 || n >= BitLen) return 0;
    return (pMem[GetMemIndex(n)] & GetMemMask(n)) ? 1 : 0;
}

int TBitField::operator==(const TBitField& bf) const {
    if (BitLen != bf.BitLen) return 0;
    for (int i = 0; i < MemLen; i++) {
        if (pMem[i] != bf.pMem[i]) return 0;
    }
    return 1;
}

int TBitField::operator!=(const TBitField& bf) const {
    return !(*this == bf);
}

TBitField& TBitField::operator=(const TBitField& bf) {
    if (this == &bf) return *this;
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
    if (BitLen != bf.BitLen) throw std::invalid_argument("Sizes must match");
    TBitField res(BitLen);
    for (int i = 0; i < MemLen; i++) {
        res.pMem[i] = pMem[i] | bf.pMem[i];
    }
    return res;
}

TBitField TBitField::operator&(const TBitField& bf) {
    if (BitLen != bf.BitLen) throw std::invalid_argument("Sizes must match");
    TBitField res(BitLen);
    for (int i = 0; i < MemLen; i++) {
        res.pMem[i] = pMem[i] & bf.pMem[i];
    }
    return res;
}

TBitField TBitField::operator~(void) {
    TBitField res(BitLen);
    for (int i = 0; i < MemLen; i++) {
        res.pMem[i] = ~pMem[i];
    }
    int tail = BitLen % (sizeof(TELEM) * 8);
    if (tail != 0 && MemLen > 0) {
        TELEM mask = (1u << tail) - 1;
        res.pMem[MemLen - 1] &= mask;
    }
    return res;
}

std::ostream& operator<<(std::ostream& ostr, const TBitField& bf) {
    for (int i = 0; i < bf.BitLen; i++) {
        ostr << bf.GetBit(i);
    }
    return ostr;
}

std::istream& operator>>(std::istream& istr, TBitField& bf) {
    char c;
    for (int i = 0; i < bf.BitLen; i++) {
        while (istr >> c && (c == ' ' || c == '\n' || c == '\r' || c == '\t')) {}

        if (!istr) break;

        if (c == '1') bf.SetBit(i);
        else bf.ClrBit(i);
    }
    return istr;
}