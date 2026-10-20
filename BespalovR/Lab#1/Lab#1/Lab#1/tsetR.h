#pragma once
#include "../../../../includes/Lab1/tset.h"
#include "tbitfieldR.h"
#include <stdexcept>
#include <ostream>
#include <istream>

TSet::TSet(int mp) : MaxPower(mp), BitField(mp) {
    if (mp < 0) {
        throw std::invalid_argument("TSet: max power cannot be negative");
    }
}

TSet::TSet(const TSet& s) : MaxPower(s.MaxPower), BitField(s.BitField) {}

TSet::TSet(const TBitField& bf) : MaxPower(bf.GetLength()), BitField(bf) {}

TSet::operator TBitField() {
    return BitField;
}

int TSet::GetMaxPower(void) const {
    return MaxPower;
}

void TSet::InsElem(const int Elem) {
    if (Elem < 0 || Elem >= MaxPower) {
        throw std::out_of_range("TSet: element is out of universe");
    }

    BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) {
    if (Elem < 0 || Elem >= MaxPower) {
        throw std::out_of_range("TSet: element is out of universe");
    }

    BitField.ClrBit(Elem);
}

int TSet::IsMember(const int Elem) const {
    if (Elem < 0 || Elem >= MaxPower) {
        throw std::out_of_range("TSet: element is out of universe");
    }

    return BitField.GetBit(Elem);
}

int TSet::operator==(const TSet& s) const {
    return MaxPower == s.MaxPower && BitField == s.BitField;
}

int TSet::operator!=(const TSet& s) const {
    return !(*this == s);
}

TSet& TSet::operator=(const TSet& s) {
    if (this != &s) {
        MaxPower = s.MaxPower;
        BitField = s.BitField;
    }

    return *this;
}

TSet TSet::operator+(const int Elem) {
    if (Elem < 0 || Elem >= MaxPower) {
        throw std::out_of_range("TSet: element is out of universe");
    }

    TSet result(*this);
    result.InsElem(Elem);
    return result;
}

TSet TSet::operator-(const int Elem) {
    if (Elem < 0 || Elem >= MaxPower) {
        throw std::out_of_range("TSet: element is out of universe");
    }

    TSet result(*this);
    result.DelElem(Elem);
    return result;
}

TSet TSet::operator+(const TSet& s) {
    if (MaxPower != s.MaxPower) {
        throw std::invalid_argument("TSet: different universes");
    }

    TSet result(MaxPower);
    result.BitField = BitField | s.BitField;
    return result;
}

TSet TSet::operator*(const TSet& s) {
    if (MaxPower != s.MaxPower) {
        throw std::invalid_argument("TSet: different universes");
    }

    TSet result(MaxPower);
    result.BitField = BitField & s.BitField;
    return result;
}

TSet TSet::operator~(void) {
    TSet result(MaxPower);
    result.BitField = ~BitField;
    return result;
}

//Change 2
std::istream& operator>>(std::istream& istr, TSet& s) {
    istr >> s.BitField;
    return istr;
}

std::ostream& operator<<(std::ostream& ostr, const TSet& s) {
    ostr << "{ ";

    bool first = true;

    for (int i = 0; i < s.MaxPower; ++i) {
        if (s.IsMember(i)) {
            if (!first) {
                ostr << ", ";
            }

            ostr << i;
            first = false;
        }
    }

    ostr << " }";
    return ostr;
}