#include "tset.h"

TSet::TSet(int mp)
    : MaxPower(mp)
    , BitField(mp)
{
}

TSet::TSet(const TSet& s)
    : MaxPower(s.MaxPower)
    , BitField(s.BitField)
{
}

TSet::TSet(const TBitField& bf)
    : MaxPower(bf.GetLength())
    , BitField(bf)
{
}

TSet::operator TBitField() {
    return BitField;
}

int TSet::GetMaxPower() const {
    return MaxPower;
}

void TSet::InsElem(const int Elem) {
    if (Elem < 0 || Elem >= MaxPower) return;
    BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) {
    if (Elem < 0 || Elem >= MaxPower) return;
    BitField.ClrBit(Elem);
}

int TSet::IsMember(const int Elem) const {
    if (Elem < 0 || Elem >= MaxPower) return 0;
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

// объединение с элементом
TSet TSet::operator+(const int Elem) {
    TSet result(*this);
    result.InsElem(Elem);
    return result;
}

// разность с элементом
TSet TSet::operator-(const int Elem) {
    TSet result(*this);
    result.DelElem(Elem);
    return result;
}


 // объединение двух множеств
TSet TSet::operator+(const TSet& s) {
    TBitField resultBF = BitField | s.BitField;
    TSet result(resultBF);
    return result;
}


 // пересечение двух множеств
TSet TSet::operator*(const TSet& s) {
    TBitField resultBF = BitField & s.BitField;
    TSet result(resultBF);
    return result;
}

// дополнение множетсва
TSet TSet::operator~() {
    TBitField resultBF = ~BitField;
    TSet result(resultBF);
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
            s.InsElem(num);
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