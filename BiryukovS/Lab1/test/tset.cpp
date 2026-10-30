#include <iostream>
#include "../../../includes/Lab1/tset.h"
#include "../../../includes/Lab1/tbitfield.h"
#include <stdexcept>
#include <string>

TSet::TSet(int mp) : BitField(mp) {
    if (mp < 0) {
        throw std::invalid_argument("TSet: negative MaxPower");
    }
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

int TSet::GetMaxPower(void) const {
    return MaxPower;
}

void TSet::InsElem(const int Elem) {
    if (Elem < 0 || Elem >= MaxPower) {
        throw std::out_of_range("TSet::InsElem: element out of universe");
    }
    BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) {
    if (Elem < 0 || Elem >= MaxPower) {
        throw std::out_of_range("TSet::DelElem: element out of universe");
    }
    BitField.ClrBit(Elem);
}

int TSet::IsMember(const int Elem) const {
    if (Elem < 0 || Elem >= MaxPower) {
        throw std::out_of_range("TSet::IsMember: element out of universe");
    }
    return BitField.GetBit(Elem);
}

int TSet::operator==(const TSet& s) const {
    if (MaxPower != s.MaxPower) {
        return 0;
    }
    return BitField == s.BitField;
}

int TSet::operator!=(const TSet& s) const {
    if (*this == s) {
        return 0;
    }
    else {
        return 1;
    }
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
        throw std::out_of_range("TSet::operator+ (int): element out of universe");
    }
    TSet result(*this);
    result.InsElem(Elem);
    return result;
}

TSet TSet::operator-(const int Elem) {
    if (Elem < 0 || Elem >= MaxPower) {
        throw std::out_of_range("TSet::operator- (int): element out of universe");
    }
    TSet result(*this);
    result.DelElem(Elem);
    return result;
}

TSet TSet::operator+(const TSet& s) {
    if (MaxPower != s.MaxPower) {
        throw std::invalid_argument("Sets must be from the same universe");
    }
    TSet result(MaxPower);
    result.BitField = BitField | s.BitField;
    return result;
}

TSet TSet::operator*(const TSet& s) {
    if (MaxPower != s.MaxPower) {
        throw std::invalid_argument("Sets must be from the same universe");
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

std::ostream& operator<<(std::ostream& ostr, const TSet& s) {
    ostr << "{";
    int first = 1;
    for (int i = 0; i < s.MaxPower; i++) {
        if (s.BitField.GetBit(i) == 1) {
            if (first == 0) {
                ostr << ", ";
            }
            ostr << i;
            first = 0;
        }
    }
    ostr << "}";
    return ostr;
}

std::istream& operator>>(std::istream& istr, TSet& s) {
    std::string str;
    std::getline(istr, str);
    int i = 0;
    int n = (int)str.size();
    while (i < n && str[i] != '{') i++;
    if (i < n) i++;
    for (int k = 0; k < s.MaxPower; k++) {
        s.BitField.ClrBit(k);
    }
    while (i < n && str[i] != '}') {
        while (i < n && (str[i] == ',' || str[i] == ' ' || str[i] == '\t')) {
            i++;
        }
        if (i >= n || str[i] == '}') break;
        int value = 0;
        int hasDigits = 0;
        while (i < n && str[i] >= '0' && str[i] <= '9') {
            value = value * 10 + (str[i] - '0');
            i++;
            hasDigits = 1;
        }
        if (hasDigits) {
            if (value < 0 || value >= s.MaxPower) {
                throw std::out_of_range("TSet input: element out of universe");
            }
            s.BitField.SetBit(value);
        }
    }
    return istr;
}