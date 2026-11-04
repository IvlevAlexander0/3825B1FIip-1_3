#pragma once
#include "tbitfield.h"
class TSet
{
private:
    int MaxPower;
    TBitField BitField;
public:
    TSet(int mp) : MaxPower(mp), BitField(mp) {}
    TSet(const TSet& s) : MaxPower(s.MaxPower), BitField(s.BitField) {}
    TSet(const TBitField& bf) : MaxPower(bf.GetLength()), BitField(bf) {}
    operator TBitField() {
        return BitField;
    }
    int GetMaxPower(void) const {
        return MaxPower;
    }
    void InsElem(const int Elem) {
        BitField.SetBit(Elem);
    }
    void DelElem(const int Elem) {
        BitField.ClrBit(Elem);
    }
    int IsMember(const int Elem) const {
        return BitField.GetBit(Elem);
    }
    int operator==(const TSet& s) const {
        return (BitField == s.BitField) && (MaxPower == s.MaxPower);
    }
    int operator!=(const TSet& s) const {
        return !(*this == s);
    }
    TSet& operator=(const TSet& s) {
        if (this == &s) {
            return *this;
        }
        MaxPower = s.MaxPower;
        BitField = s.BitField;
        return *this;
    }
    TSet operator+(const int Elem) {
        TSet res(*this);
        res.InsElem(Elem);
        return res;
    }
    TSet operator-(const int Elem) {
        TSet res(*this);
        res.DelElem(Elem);
        return res;
    }
    TSet operator+(const TSet& s) {
        int maxPower = MaxPower > s.MaxPower ? MaxPower : s.MaxPower;
        TSet res(maxPower);
        res.BitField = BitField | s.BitField;
        return res;
    }
    TSet operator*(const TSet& s) {
        int maxPower = MaxPower > s.MaxPower ? MaxPower : s.MaxPower;
        TSet res(maxPower);
        res.BitField = BitField & s.BitField;
        return res;
    }

    TSet operator~ (void) {
        TSet res(~BitField);
        return res;
    }
    friend std::istream& operator>>(std::istream& is, TSet& s) {
        int elem;
        char ch;
        do {
            is >> ch;
        } while (ch != '{');
        do {
            is >> elem;
            s.InsElem(elem);
            is >> ch;
        } while (ch == ',');
        return is;
    }
    friend std::ostream& operator<<(std::ostream& os, const TSet& s) {
        os << "{";
        int first = 1;
        for (int i = 0; i < s.MaxPower; i++) {
            if (s.IsMember(i)) {
                if (!first) {
                    os << ", ";
                }
                os << i;
                first = 0;
            }
        }
        os << "}";
        return os;
    }
};