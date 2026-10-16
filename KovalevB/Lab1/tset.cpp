#include "tset.h"
#include <stdexcept>

TSet::TSet(int mp): MaxPower(mp), BitField(mp) {}


TSet::TSet(const TSet &s): MaxPower(s.MaxPower), BitField(s.BitField) {}

TSet::TSet(const TBitField &bf): MaxPower(bf.GetLength()), BitField(bf) {}

TSet::operator TBitField() {
    return BitField;
}

int TSet::GetMaxPower(void) const {
    return MaxPower;
}

void TSet::InsElem(const int Elem) {
    BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) {
    BitField.ClrBit(Elem);
}

int TSet::IsMember(const int Elem) const {
    return BitField.GetBit(Elem);
}

int TSet::operator==(const TSet &s) const {
    return BitField == s.BitField;
}

int TSet::operator!=(const TSet &s) const {
    return BitField != s.BitField;
}

TSet& TSet::operator=(const TSet &s) {
    if (this == &s) {
        return *this;
    }
    MaxPower = s.MaxPower;
    BitField = s.BitField;
    return *this;
}

TSet TSet::operator+(const int Elem) {
    TSet temp(*this);
    temp.InsElem(Elem);
    return temp;
}

TSet TSet::operator-(const int Elem) {
    TSet temp(*this);
    temp.DelElem(Elem);
    return temp;
}

TSet TSet::operator+(const TSet &s) {
    if (MaxPower != s.MaxPower) {
        throw std::invalid_argument("Sets must belong to the same universe");
    }
    return TSet(BitField | s.BitField);
}

TSet TSet::operator*(const TSet &s) {
    if (MaxPower != s.MaxPower) {
        throw std::invalid_argument("Sets must belong to the same universe");
    }
    return TSet(BitField & s.BitField);
}

TSet TSet::operator~(void) {
    return TSet(~BitField);
}

std::istream &operator>>(std::istream &istr, TSet &s) {
    TSet tempSet(s.GetMaxPower());
    char ch;

    while (istr >> ch && ch != '{') {}
    if (istr.fail()) { return istr; }

    while (istr >> ch) {
        if (ch == '}') {
            break;
        }
        if (ch == ',') {
            continue;
        }
        istr.putback(ch);

        int elem;
        if (istr >> elem) {
            tempSet.InsElem(elem);
        } else {
            break;
        }
    }
    s = tempSet;
    return istr;
}

std::ostream &operator<<(std::ostream &ostr, const TSet &s) {
    ostr << "{ ";
    for (int i = 0; i < s.GetMaxPower(); i++) {
        if (s.IsMember(i)) {
            ostr << i << " ";
        }
    }
    ostr << "}";
    return ostr;
}