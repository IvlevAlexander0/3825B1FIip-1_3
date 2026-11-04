#pragma once
#include <iostream>
typedef unsigned int TELEM;
class TBitField
{
private:
    int  BitLen;
    TELEM* pMem;
    int  MemLen;
    int  GetMemIndex(const int n) const {
        return n >> 5;
    }
    TELEM GetMemMask(const int n) const {
        return 1 << (n & 31);
    }
public:
    TBitField(int len) {
        if (len < 0) {
            throw "Negative length";
        }
        BitLen = len;
        MemLen = (BitLen + 31) >> 5;
        pMem = new TELEM[MemLen];
        for (int i = 0; i < MemLen; i++) {
            pMem[i] = 0;
        }
    }
    TBitField(const TBitField& bf) {
        BitLen = bf.BitLen;
        MemLen = bf.MemLen;
        pMem = new TELEM[MemLen];
        for (int i = 0; i < MemLen; i++) {
            pMem[i] = bf.pMem[i];
        }
    }
    ~TBitField() {
        delete[] pMem;
        pMem = nullptr;
        BitLen = 0;
        MemLen = 0;
    }
    int GetLength(void) const {
        return BitLen;
    }
    void SetBit(const int n) {
        if (n < 0 || n >= BitLen) {
            throw "Index out of range";
        }
        pMem[GetMemIndex(n)] |= GetMemMask(n);
    }
    void ClrBit(const int n) {
        if (n < 0 || n >= BitLen) {
            throw "Index out of range";
        }
        pMem[GetMemIndex(n)] &= ~GetMemMask(n);
    }
    int GetBit(const int n) const {
        if (n < 0 || n >= BitLen) {
            throw "Index out of range";
        }
        if ((pMem[GetMemIndex(n)] & GetMemMask(n)) != 0) {
            return 1;
        }
        else {
            return 0;
        }
    }
    int operator==(const TBitField& bf) const {
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
    int operator!=(const TBitField& bf) const {
        return !(*this == bf);
    }
    TBitField& operator=(const TBitField& bf) {
        if (this == &bf) {
            return *this;
        }
        BitLen = bf.BitLen;
        if (MemLen != bf.MemLen) {
            MemLen = bf.MemLen;
            delete[] pMem;
            pMem = new TELEM[MemLen];
        }
        for (int i = 0; i < MemLen; i++) {
            pMem[i] = bf.pMem[i];
        }
        return *this;
    }
    TBitField operator|(const TBitField& bf) {
        int len = BitLen > bf.BitLen ? BitLen : bf.BitLen;
        TBitField res(len);
        for (int i = 0; i < MemLen; i++) {
            res.pMem[i] = pMem[i];
        }
        for (int i = 0; i < bf.MemLen; i++) {
            res.pMem[i] |= bf.pMem[i];
        }
        return res;
    }
    TBitField operator&(const TBitField& bf) {
        int len = BitLen > bf.BitLen ? BitLen : bf.BitLen;
        TBitField res(len);
        for (int i = 0; i < MemLen; i++) {
            res.pMem[i] = pMem[i];
        }
        for (int i = 0; i < bf.MemLen; i++) {
            res.pMem[i] &= bf.pMem[i];
        }
        return res;
    }
    TBitField operator~(void) {
        TBitField res(BitLen);
        for (int i = 0; i < MemLen; i++) {
            res.pMem[i] = ~pMem[i];
        }
        int rem = BitLen & 31;
        if (rem != 0) {
            TELEM mask = (1 << rem) - 1;
            res.pMem[MemLen - 1] &= mask;
        }
        return res;
    }
    friend std::istream& operator>>(std::istream& is, TBitField& bf) {
        char ch;
        int i = 0;
        while (is >> ch) {
            if (ch == '0') {
                bf.ClrBit(i++);
            }
            else if (ch == '1') {
                bf.SetBit(i++);
            }
            else {
                break;
            }
            if (i >= bf.BitLen) {
                break;
            }
        }
        return is;
    }
    friend std::ostream& operator<<(std::ostream& os, const TBitField& bf) {
        for (int i = 0; i < bf.BitLen; i++) {
            os << bf.GetBit(i);
        }
        return os;
    }
};