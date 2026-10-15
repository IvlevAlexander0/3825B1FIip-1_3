#include "tset.h"
#include <stdexcept>

TSet::TSet(int mp) : BitField(mp) {
    MaxPower = mp;
}

TSet::TSet(const TSet& s) : BitField(s.BitField) {
    MaxPower = s.MaxPower;
}

TSet::TSet(const TBitField& bf) : BitField(bf) {
    MaxPower = bf.GetLength();
}

TSet::operator TBitField() {
    return BitField;
}

int TSet::GetMaxPower() const {
    return MaxPower;
}

void TSet::InsElem(const int Elem) {
    if (Elem < 0 || Elem >= MaxPower) {
        throw std::out_of_range("Element out of range");
    }
    BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) {
    if (Elem < 0 || Elem >= MaxPower) {
        throw std::out_of_range("Element out of range");
    }
    BitField.ClrBit(Elem);
}

int TSet::IsMember(const int Elem) const {
    if (Elem < 0 || Elem >= MaxPower) {
        throw std::out_of_range("Element out of range");
    }
    return BitField.GetBit(Elem);
}

int TSet::operator==(const TSet& s) const {
    if (MaxPower != s.MaxPower)
        return 0;
    return BitField == s.BitField;
}

int TSet::operator!=(const TSet& s) const {
    return !(*this == s);
}

TSet& TSet::operator=(const TSet& s) {
    if (this == &s) {
        return *this;
    }
    MaxPower = s.MaxPower;
    BitField = s.BitField;
    return *this;
}

TSet TSet::operator+(const int Elem) {
    if (Elem < 0 || Elem >= MaxPower) {
        throw std::out_of_range("Element out of range");
    }
    TSet result(*this);
    result.InsElem(Elem);
    return result;
}

TSet TSet::operator-(const int Elem) {
    if (Elem < 0 || Elem >= MaxPower) {
        throw std::out_of_range("Element out of range");
    }
    TSet result(*this);
    result.DelElem(Elem);
    return result;
}

TSet TSet::operator+(const TSet& s) {
    if (MaxPower != s.MaxPower) {
        throw std::invalid_argument("operator+: sets must have equal MaxPower");
    }
    TBitField bf = BitField | s.BitField;
    return TSet(bf);
}

TSet TSet::operator*(const TSet& s) {
    if (MaxPower != s.MaxPower) {
        throw std::invalid_argument("operator*: sets must have equal MaxPower");
    }
    TBitField bf = BitField & s.BitField;
    return TSet(bf);
}

TSet TSet::operator~() {
    TBitField bf = ~BitField;
    TSet result(bf);
    result.MaxPower = MaxPower;
    return result;
}

std::istream& operator>>(std::istream& istr, TSet& bf) {
    for (int i = 0; i < bf.MaxPower; ++i) {
        bf.BitField.ClrBit(i);
    }
    char c;
    istr >> c;
    if (!istr) {
        return istr;
    }
    if (c != '{') {
        throw std::invalid_argument("Expected '{'");
    }

    while (istr >> c) {
        if (c == '}') {
            return istr;
        }
        if (c == ',' || c == ' ') {
            continue;
        }
        if (c < '0' || c > '9') {
            throw std::invalid_argument("Invalid character");
        }
        int elem = c - '0';
        while (istr.peek() >= '0' && istr.peek() <= '9') {
            istr.get(c);
            elem = elem * 10 + (c - '0');
        }
        if (elem < 0 || elem >= bf.MaxPower) {
            throw std::out_of_range("Element out of range");
        }
        bf.BitField.SetBit(elem);
    }
    throw std::invalid_argument("Expected '}'");
}

std::ostream& operator<<(std::ostream& ostr, const TSet& bf) {
    ostr << '{';
    bool first = true;
    for (int i = 0; i < bf.MaxPower; ++i) {
        if (bf.BitField.GetBit(i)) {
            if (!first) {
                ostr << ", ";
            }
            ostr << i;
            first = false;
        }
    }
    ostr << '}';
    return ostr;
}