#include "tbitfield.h"

#include <stdexcept>
#include <string>

namespace {
    const int TELEM_BITS = static_cast<int>(sizeof(TELEM) * 8);
}

int TBitField::GetMemIndex(const int n) const
{
    return n / TELEM_BITS;
}

TELEM TBitField::GetMemMask(const int n) const
{
    return static_cast<TELEM>(1u) << (n % TELEM_BITS);
}

TBitField::TBitField(int len)
{
    if (len < 0)
        throw std::invalid_argument("TBitField: len must be >= 0");

    BitLen = len;
    MemLen = (len + TELEM_BITS - 1) / TELEM_BITS;
    if (MemLen == 0)
        MemLen = 1;

    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; ++i)
        pMem[i] = 0;
}

TBitField::TBitField(const TBitField& bf)
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; ++i)
        pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
    delete[] pMem;
    pMem = nullptr;
}

int TBitField::GetLength(void) const
{
    return BitLen;
}

void TBitField::SetBit(const int n)
{
    if (n < 0 || n >= BitLen)
        throw std::out_of_range("TBitField::SetBit: index out of range");
    pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n)
{
    if (n < 0 || n >= BitLen)
        throw std::out_of_range("TBitField::ClrBit: index out of range");
    pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const
{
    if (n < 0 || n >= BitLen)
        throw std::out_of_range("TBitField::GetBit: index out of range");
    return (pMem[GetMemIndex(n)] & GetMemMask(n)) ? 1 : 0;
}

int TBitField::operator==(const TBitField& bf) const
{
    if (BitLen != bf.BitLen)
        return 0;
    for (int i = 0; i < MemLen; ++i)
        if (pMem[i] != bf.pMem[i])
            return 0;
    return 1;
}

int TBitField::operator!=(const TBitField& bf) const
{
    return ~(*this == bf);
}

TBitField& TBitField::operator=(const TBitField& bf)
{
    if (this == &bf)
        return *this;

    if (MemLen != bf.MemLen)
    {
        delete[] pMem;
        MemLen = bf.MemLen;
        pMem = new TELEM[MemLen];
    }
    BitLen = bf.BitLen;
    for (int i = 0; i < MemLen; ++i)
        pMem[i] = bf.pMem[i];
    return *this;
}

TBitField TBitField::operator|(const TBitField& bf)
{
    const int maxLen = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;
    TBitField res(maxLen);

    const int minMem = (MemLen < bf.MemLen) ? MemLen : bf.MemLen;
    for (int i = 0; i < minMem; ++i)
        res.pMem[i] = pMem[i] | bf.pMem[i];

    const TBitField* longer = (MemLen > bf.MemLen) ? this : &bf;
    for (int i = minMem; i < longer->MemLen; ++i)
        res.pMem[i] = longer->pMem[i];

    return res;
}

TBitField TBitField::operator&(const TBitField& bf)
{
    const int maxLen = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;
    TBitField res(maxLen);

    const int minMem = (MemLen < bf.MemLen) ? MemLen : bf.MemLen;
    for (int i = 0; i < minMem; ++i)
        res.pMem[i] = pMem[i] & bf.pMem[i];

    return res;
}

TBitField TBitField::operator~(void)
{
    TBitField res(BitLen);
    for (int i = 0; i < MemLen; ++i)
        res.pMem[i] = ~pMem[i];

    const int tail = BitLen % TELEM_BITS;
    if (tail != 0)
    {
        const TELEM mask = (static_cast<TELEM>(1u) << tail) - 1u;
        res.pMem[MemLen - 1] &= mask;
    }
    return res;
}

std::ostream& operator<<(std::ostream& ostr, const TBitField& bf)
{
    for (int i = bf.BitLen - 1; i >= 0; --i)
        ostr << bf.GetBit(i);
    return ostr;
}

std::istream& operator>>(std::istream& istr, TBitField& bf)
{
    std::string s;
    istr >> s;

    if (static_cast<int>(s.size()) != bf.BitLen)
    {
        istr.setstate(std::ios::failbit);
        return istr;
    }
    for (int i = 0; i < bf.BitLen; ++i)
    {
        const char c = s[bf.BitLen - 1 - i];
        if (c == '1')      bf.SetBit(i);
        else if (c == '0') bf.ClrBit(i);
        else { istr.setstate(std::ios::failbit); return istr; }
    }
    return istr;
}