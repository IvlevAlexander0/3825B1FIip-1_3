#include "tset.h"
#include <stdexcept>

TSet::TSet(int mp): MaxPower(mp), BitField(mp){}

TSet::TSet(const TSet& s): MaxPower(s.MaxPower), BitField(s.BitField){}

TSet::TSet(const TBitField& bf): MaxPower(bf.GetLength()), BitField(bf){}

TSet::operator TBitField() {
    return BitField;
}


int TSet::GetMaxPower() const {
    return MaxPower;
}

void TSet::InsElem(const int Elem) {
    if (Elem < 0 || Elem >= MaxPower) {
        throw std::out_of_range("TSet::InsElem: element is out of universum");
    }
    BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) {
    if (Elem < 0 || Elem >= MaxPower) {
        throw std::out_of_range("TSet::DelElem: element is out of universum");
    }
    BitField.ClrBit(Elem);
}

int TSet::IsMember(const int Elem) const {
    if (Elem < 0 || Elem >= MaxPower) {
        throw std::out_of_range("TSet::IsMember: element is out of universum");
    }
    return BitField.GetBit(Elem);
}


int TSet::operator==(const TSet& s) const {
    return BitField == s.BitField;
}

int TSet::operator!=(const TSet& s) const {
    return BitField != s.BitField;
}

TSet& TSet::operator=(const TSet& s) {
    if (this == &s) return *this;
    MaxPower = s.MaxPower;
    BitField = s.BitField;
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
    if (MaxPower != s.MaxPower) {
        throw std::invalid_argument("TSet::operator+: different universums");
    }
    TSet result(*this);
    result.BitField = BitField | s.BitField;
    return result;
}

TSet TSet::operator*(const TSet& s) {
    if (MaxPower != s.MaxPower) {
        throw std::invalid_argument("TSet::operator*: different universums");
    }
    TSet result(*this);
    result.BitField = BitField & s.BitField;
    return result;
}

TSet TSet::operator~() {
    TSet result(*this);
    result.BitField = ~BitField;
    return result;
}


std::istream& operator>>(std::istream& istr, TSet& s) {
    char c;
    istr >> c;
    if (c != '{') {
        istr.setstate(std::ios::failbit);
        return istr;
    }
    for (int i = 0; i < s.MaxPower; ++i) {
        s.BitField.ClrBit(i);
    }
    while (istr >> c && c != '}') {
        if (c >= '0' && c <= '9') {
            int num = c - '0';
            while (istr.peek() >= '0' && istr.peek() <= '9') {
                istr >> c;
                num = num * 10 + (c - '0');
            }
            if (num >= 0 && num < s.MaxPower) {
                s.InsElem(num);
            }
        }
    }
    return istr;
}

std::ostream& operator<<(std::ostream& ostr, const TSet& s) {
    ostr << "{ ";
    bool first = true;
    for (int i = 0; i < s.MaxPower; ++i) {
        if (s.BitField.GetBit(i)) {
            if (!first) ostr << ", ";
            ostr << i;
            first = false;
        }
    }
    ostr << " }";
    return ostr;
}