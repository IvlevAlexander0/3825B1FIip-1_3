#include "tset.h"

#include <sstream>
#include <stdexcept>

#include <stdexcept>

TSet::TSet(int mp) : MaxPower(mp), BitField(mp)
{
}

TSet::TSet(const TSet& s) : MaxPower(s.MaxPower), BitField(s.BitField)
{
}

TSet::TSet(const TBitField& bf) : MaxPower(bf.GetLength()), BitField(bf)
{
}

TSet::operator TBitField()
{
    return BitField;
}

int TSet::GetMaxPower(void) const
{
    return MaxPower;
}

void TSet::InsElem(const int Elem)
{
    if (Elem < 0 || Elem >= MaxPower)
        throw std::out_of_range("TSet::InsElem: element out of universe");
    BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem)
{
    if (Elem < 0 || Elem >= MaxPower)
        throw std::out_of_range("TSet::DelElem: element out of universe");
    BitField.ClrBit(Elem);
}

int TSet::IsMember(const int Elem) const
{
    if (Elem < 0 || Elem >= MaxPower)
        throw std::out_of_range("TSet::IsMember: element out of universe");
    return BitField.GetBit(Elem);
}

int TSet::operator==(const TSet& s) const
{
    return BitField == s.BitField;
}

int TSet::operator!=(const TSet& s) const
{
    return BitField != s.BitField;
}

TSet& TSet::operator=(const TSet& s)
{
    if (this == &s)
        return *this;
    MaxPower = s.MaxPower;
    BitField = s.BitField;
    return *this;
}

TSet TSet::operator+(const int Elem)
{
    TSet res(*this);
    res.InsElem(Elem);
    return res;
}

TSet TSet::operator-(const int Elem)
{
    TSet res(*this);
    res.DelElem(Elem);
    return res;
}

TSet TSet::operator+(const TSet& s)
{
    TBitField bf = BitField | s.BitField;
    TSet res(bf);
    res.MaxPower = (MaxPower > s.MaxPower) ? MaxPower : s.MaxPower;
    return res;
}

TSet TSet::operator*(const TSet& s)
{
    TBitField bf = BitField & s.BitField;
    TSet res(bf);
    res.MaxPower = (MaxPower > s.MaxPower) ? MaxPower : s.MaxPower;
    return res;
}

TSet TSet::operator~(void)
{
    TBitField bf = ~BitField;
    TSet res(bf);
    res.MaxPower = MaxPower;
    return res;
}

std::ostream& operator<<(std::ostream& ostr, const TSet& bf)
{
    ostr << "{ ";
    bool first = true;
    for (int i = 0; i < bf.MaxPower; ++i)
    {
        if (bf.BitField.GetBit(i))
        {
            if (!first) ostr << ", ";
            ostr << i;
            first = false;
        }
    }
    ostr << " }";
    return ostr;
}

std::istream& operator>>(std::istream& istr, TSet& bf)
{
    int n;
    if (!(istr >> n) || n < 0)
    {
        istr.setstate(std::ios::failbit);
        return istr;
    }

    for (int i = 0; i < bf.MaxPower; ++i)
        bf.BitField.ClrBit(i);

    for (int k = 0; k < n; ++k)
    {
        int e;
        if (!(istr >> e) || e < 0 || e >= bf.MaxPower)
        {
            istr.setstate(std::ios::failbit);
            return istr;
        }
        bf.BitField.SetBit(e);
    }
    return istr;
}
