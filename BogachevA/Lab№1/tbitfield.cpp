#include "tbitfield.h"
int TBitField::GetMemIndex(const int n) const {
    return n >> 5;
}
TELEM TBitField::GetMemMask(const int n) const {
    return 1u << (n & 31);
}
TBitField::TBitField(int len) {
    if (len < 0) {
        throw std::invalid_argument("Negative length");
    }
    BitLen = len;
    MemLen = (BitLen + 31) >> 5;
    if (MemLen == 0) {
        pMem = nullptr;
    }
    else {
        pMem = new TELEM[MemLen];

        for (int i = 0; i < MemLen; i++) {
            pMem[i] = 0;
        }
    }
}
TBitField::TBitField(const TBitField& bf) {
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    if (MemLen == 0) {
        pMem = nullptr;
    }
    else {
        pMem = new TELEM[MemLen];

        for (int i = 0; i < MemLen; i++) {
            pMem[i] = bf.pMem[i];
        }
    }
}
TBitField::~TBitField() {
    delete[] pMem;
    pMem = nullptr;
    BitLen = 0;
    MemLen = 0;
}
int TBitField::GetLength(void) const {
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
    if ((pMem[GetMemIndex(n)] & GetMemMask(n)) != 0) {
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
    return !(*this == bf);
}
TBitField& TBitField::operator=(const TBitField& bf) {
    if (this == &bf) {
        return *this;
    }
    if (MemLen != bf.MemLen) {
        delete[] pMem;
        MemLen = bf.MemLen;
        if (MemLen == 0) {
            pMem = nullptr;
        }
        else {
            pMem = new TELEM[MemLen];
        }
    }
    BitLen = bf.BitLen;
    for (int i = 0; i < MemLen; i++) {
        pMem[i] = bf.pMem[i];
    }
    return *this;
}
TBitField TBitField::operator|(const TBitField& bf)  {
    int len = BitLen > bf.BitLen ? BitLen : bf.BitLen;
    TBitField res(len);
    for (int i = 0; i < MemLen; i++) {
        res.pMem[i] |= pMem[i];
    }
    for (int i = 0; i < bf.MemLen; i++) {
        res.pMem[i] |= bf.pMem[i];
    }
    return res;
}
TBitField TBitField::operator&(const TBitField& bf)  {
    int len = BitLen < bf.BitLen ? BitLen : bf.BitLen;
    TBitField res(len);
    int memLen = MemLen < bf.MemLen ? MemLen : bf.MemLen;
    for (int i = 0; i < memLen; i++) {
        res.pMem[i] = pMem[i] & bf.pMem[i];
    }
    return res;
}
TBitField TBitField::operator~() {
    TBitField res(BitLen);
    for (int i = 0; i < MemLen; i++) {
        res.pMem[i] = ~pMem[i];
    }
    int rest = BitLen % 32;
    if (rest != 0) {

        TELEM mask = (1u << rest) - 1;

        res.pMem[MemLen - 1] &= mask;
    }
    return res;
}
std::istream& operator>>(std::istream& is, TBitField& bf) {
    char ch;
    int i = 0;
    while (is >> ch) {
        if (ch != '0' && ch != '1') {
            break;
        }
        if (i >= bf.BitLen) {
            break;
        }
        if (ch == '1') {
            bf.SetBit(i);
        }
        else {
            bf.ClrBit(i);
        }
        i++;
    }
    return is;
}
std::ostream& operator<<(std::ostream& os, const TBitField& bf) {
    for (int i = 0; i < bf.BitLen; i++) {
        os << bf.GetBit(i);
    }
    return os;
}