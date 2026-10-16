#include <iostream>
#include <stdexcept>
#include "C:\includes\tset.h"

TSet::TSet(int mp) : MaxPower(mp), BitField(mp) {
	if (mp < 0) {
		throw std::out_of_range("MaxPower cannot be negative");
	}
}

TSet::TSet(const TSet& s) : MaxPower(s.MaxPower), BitField(s.BitField) {}

TSet::TSet(const TBitField& bf) : MaxPower(bf.GetLength()), BitField(bf) {}

TSet::operator TBitField() {
	return BitField;
}

int TSet::GetMaxPower() const {
	return MaxPower;
}

void TSet::InsElem(const int Elem) {
	if (Elem >= 0 && Elem < MaxPower) {
		BitField.SetBit(Elem);
	}
	else {
		throw std::out_of_range("Element out of universe bounds");
	}
}

void TSet::DelElem(const int Elem) {
	if (Elem >= 0 && Elem < MaxPower) {
		BitField.ClrBit(Elem);
	}
	else {
		throw std::out_of_range("Element out of universe bounds");
	}
}

int TSet::IsMember(const int Elem) const {
	if (Elem >= 0 && Elem < MaxPower) {
		return BitField.GetBit(Elem);
	}
	else {
		throw std::out_of_range("Element out of universe bounds");
	}
}

int TSet::operator== (const TSet& s) const {
	if (MaxPower != s.MaxPower) {
		return 0;
	}
	return BitField == s.BitField;
}

int TSet::operator!=(const TSet& s) const {
	return 1 - (*this == s);
}

TSet& TSet::operator=(const TSet& s) {
	if (this != &s) {
		MaxPower = s.MaxPower;
		BitField = s.BitField;
	}
	return *this;
}

TSet TSet::operator+ (const int Elem) {
	if (Elem < 0 || Elem >= MaxPower) {
		throw std::out_of_range("Element out of universe bounds");
	}
	TSet s(*this);
	s.InsElem(Elem);
	return s;
}

TSet TSet::operator- (const int Elem) {
	if (Elem < 0 || Elem >= MaxPower) {
		throw std::out_of_range("Element out of universe bounds");
	}
	TSet s(*this);
	s.DelElem(Elem);
	return s;
}

TSet TSet::operator+ (const TSet& s) {
	if (MaxPower != s.MaxPower) {
		throw std::invalid_argument("Set dimensions must be equal");
	}
	return TSet(BitField | s.BitField);
}

TSet TSet::operator* (const TSet& s) {
	if (MaxPower != s.MaxPower) {
		throw std::invalid_argument("Set dimensions must be equal");
	}
	return TSet(BitField & s.BitField);
}

TSet TSet::operator~ () {
	return TSet(~BitField);
}


std::ostream& operator<<(std::ostream& stream, const TSet& s) {
	for (int i = 0; i < s.GetMaxPower(); i++) {
		if (s.IsMember(i)) {
			stream << i << ' ';
		}
	}
	stream << '\n';
	return stream;
}

std::istream& operator>>(std::istream& stream, TSet& s) {
	int elem;

	for (int i = 0; i < s.GetMaxPower(); i++) {
		s.DelElem(i);
	}

	while (true) {
		stream >> elem;
		if (stream.fail()) {
			std::cout << "Input error\n";
			stream.clear();
			stream.ignore(10000, '\n');
			continue;
		}

		if (elem < 0) {
			break;
		}
		s.InsElem(elem);
	}

	return stream;
}