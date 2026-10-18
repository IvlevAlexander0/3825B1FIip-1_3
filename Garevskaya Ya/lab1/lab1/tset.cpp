#include "../../includes/Lab1/tset.h"
#include <iostream>
#include <stdexcept>

TSet::TSet(int mp) : BitField(mp + 1) {
    MaxPower = mp;
}

TSet::TSet(const TSet& s) : BitField(s.BitField) {
    MaxPower = s.MaxPower;
}

TSet::TSet(const TBitField& bf) : BitField(bf) {
    MaxPower = bf.GetLength() - 1;
}

TSet::operator TBitField() {
    return BitField;
}

int TSet::GetMaxPower(void) const {
    return MaxPower;
}

void TSet::InsElem(const int Elem) {
    if (Elem < 0 || Elem > MaxPower) return;
    BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) {
    if (Elem < 0 || Elem > MaxPower) return;
    BitField.ClrBit(Elem);
}

int TSet::IsMember(const int Elem) const {
    if (Elem < 0 || Elem > MaxPower) return 0;
    return BitField.GetBit(Elem);
}

int TSet::operator==(const TSet& s) const {
    return (MaxPower == s.MaxPower) && (BitField == s.BitField);
}

int TSet::operator!=(const TSet& s) const {
    return !(*this == s);
}

TSet& TSet::operator=(const TSet& s) {
    if (this == &s) return *this;
    MaxPower = s.MaxPower;
    BitField = s.BitField;
    return *this;
}

TSet TSet::operator+(const int Elem) {
    TSet res(*this);
    res.InsElem(Elem);
    return res;
}

TSet TSet::operator-(const int Elem) {
    TSet res(*this);
    res.DelElem(Elem);
    return res;
}

TSet TSet::operator+(const TSet& s) {
    if (MaxPower != s.MaxPower) throw std::invalid_argument("Sizes must match");
    TSet res(MaxPower);
    res.BitField = BitField | s.BitField;
    return res;
}

TSet TSet::operator*(const TSet& s) {
    if (MaxPower != s.MaxPower) throw std::invalid_argument("Sizes must match");
    TSet res(MaxPower);
    res.BitField = BitField & s.BitField;
    return res;
}

TSet TSet::operator~(void) {
    TSet res(MaxPower);
    res.BitField = ~BitField;
    return res;
}

std::ostream& operator<<(std::ostream& ostr, const TSet& set) {
    ostr << "{ ";
    bool first = true;
    for (int i = 0; i <= set.MaxPower; i++) {
        if (set.BitField.GetBit(i) == 1) {
            if (!first) ostr << ", ";
            ostr << i;
            first = false;
        }
    }
    ostr << " }";
    return ostr;
}

std::istream& operator>>(std::istream& istr, TSet& set) {
    int x;
    while (istr >> x && x != -1) {
        set.InsElem(x);
    }
    return istr;
}
