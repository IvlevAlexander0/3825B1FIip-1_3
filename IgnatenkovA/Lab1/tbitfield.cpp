#include "tbitfield.h"

#include <climits>
#include <stdexcept>
#include <string>

const int bitsPerElem = sizeof(TELEM) * CHAR_BIT;

TBitField::TBitField(int len) {
    if (len < 0) {
        throw std::invalid_argument("Negative length");
    }

    BitLen = len;
    MemLen = len / bitsPerElem;
    if (len % bitsPerElem != 0) {
        MemLen++;
    }

    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++) {
        pMem[i] = 0;
    }
}

TBitField::TBitField(const TBitField& bf) {
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; ++i) {
        pMem[i] = bf.pMem[i];
    }
}

TBitField::~TBitField() {
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const {
    return n / bitsPerElem;
}
TELEM TBitField::GetMemMask(const int n) const {
    return TELEM(1) << (n % bitsPerElem);
}
int TBitField::GetLength(void) const {
    return BitLen;
}

void TBitField::SetBit(const int n) {
    if (n < 0 || n >= BitLen) {
        throw std::out_of_range("Invalid bit index");
    }
    int index = GetMemIndex(n);
    TELEM mask = GetMemMask(n);
    pMem[index] |= mask;
}

void TBitField::ClrBit(const int n) {
    if (n < 0 || n >= BitLen) {
        throw std::out_of_range("Invalid bit index");
    }
    int index = GetMemIndex(n);
    TELEM mask = GetMemMask(n);
    pMem[index] &= ~mask;
}

int TBitField::GetBit(const int n) const {
    if (n < 0 || n >= BitLen) {
        throw std::out_of_range("Invalid bit index");
    }
    int index = GetMemIndex(n);
    TELEM mask = GetMemMask(n);
    return (pMem[index] & mask) != 0;
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

    TELEM* newMem = new TELEM[bf.MemLen];
    for (int i = 0; i < bf.MemLen; ++i) {
        newMem[i] = bf.pMem[i];
    }

    delete[] pMem;
    pMem = newMem;
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    return *this;
}

TBitField TBitField::operator|(const TBitField& bf) {
    int len = BitLen;
    if (bf.BitLen > len) {
        len = bf.BitLen;
    }
    TBitField result(len);

    for (int i = 0; i < result.MemLen; i++) {
        TELEM a = 0;
        TELEM b = 0;
        if (i < MemLen) {
            a = pMem[i];
        }
        if (i < bf.MemLen) {
            b = bf.pMem[i];
        }
        result.pMem[i] = a | b;
    }
    return result;
}

TBitField TBitField::operator&(const TBitField& bf) {
    int len = BitLen;
    if (bf.BitLen > len) {
        len = bf.BitLen;
    }
    TBitField result(len);

    for (int i = 0; i < MemLen && i < bf.MemLen; i++) {
        result.pMem[i] = pMem[i] & bf.pMem[i];
    }
    return result;
}

TBitField TBitField::operator~(void) {
    TBitField result(BitLen);
    for (int i = 0; i < BitLen; i++) {
        if (GetBit(i) == 0) {
            result.SetBit(i);
        }
    }
    return result;
}

std::ostream& operator<<(std::ostream& ostr, const TBitField& bf) {
    for (int n = 0; n < bf.BitLen; ++n) {
        ostr << bf.GetBit(n);
    }
    return ostr;
}

std::istream& operator>>(std::istream& istr, TBitField& bf) {
    if (bf.BitLen == 0) {
        return istr;
    }
    std::string token;
    if (!(istr >> token)) {
        return istr;
    }
    if (token.size() != static_cast<std::size_t>(bf.BitLen)) {
        istr.setstate(std::ios::failbit);
        return istr;
    }

    TBitField temporary(bf.BitLen);
    for (int i = 0; i < bf.BitLen; i++) {
        if (token[i] == '1') {
            temporary.SetBit(i);
        } else if (token[i] != '0') {
            istr.setstate(std::ios::failbit);
            return istr;
        }
    }
    bf = temporary;
    return istr;
}
