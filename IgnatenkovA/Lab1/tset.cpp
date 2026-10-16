#include "tset.h"

TSet::TSet(int mp) : MaxPower(mp), BitField(mp) {}
TSet::TSet(const TSet& s) : MaxPower(s.MaxPower), BitField(s.BitField) {}
TSet::TSet(const TBitField& bf) : MaxPower(bf.GetLength()), BitField(bf) {}
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

int TSet::operator==(const TSet& s) const {
    return BitField == s.BitField;
}

int TSet::operator!=(const TSet& s) const {
    return !(*this == s);
}

TSet& TSet::operator=(const TSet& s) {
    BitField = s.BitField;
    MaxPower = s.MaxPower;
    return *this;
}
TSet TSet::operator+(const int Elem) {
    TSet result(*this);
    result.InsElem(Elem);
    return result;
}
TSet TSet::operator-(const int Elem) {
    TSet result(*this);
    result.DelElem(Elem);
    return result;
}
TSet TSet::operator+(const TSet& s) {
    return TSet(BitField | s.BitField);
}

TSet TSet::operator*(const TSet& s) {
    return TSet(BitField & s.BitField);
}

TSet TSet::operator~(void) {
    return TSet(~BitField);
}


std::ostream& operator<<(std::ostream& ostr, const TSet& s) {
    ostr << '{';
    bool first = true;
    for (int n = 0; n < s.MaxPower; ++n) {
        if (s.IsMember(n)) {
            if (!first) ostr << ' ';
            ostr << n;
            first = false;
        }
    }
    return ostr << '}';
}

std::istream& operator>>(std::istream& istr, TSet& s) {
    char opening;
    if (!(istr >> opening)) return istr;
    if (opening != '{') {
        istr.setstate(std::ios::failbit);
        return istr;
    }
    TSet temporary(s.MaxPower);
    while (istr >> std::ws) {
        if (istr.peek() == '}') {
            istr.get();
            s = temporary;
            return istr;
        }
        int elem;
        if (!(istr >> elem)) return istr;
        if (elem < 0 || elem >= s.MaxPower) {
            istr.setstate(std::ios::failbit);
            return istr;
        }
        temporary.InsElem(elem);
    }
    istr.setstate(std::ios::failbit);
    return istr;
}
