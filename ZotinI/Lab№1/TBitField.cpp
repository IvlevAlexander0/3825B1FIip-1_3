#include <iostream>
#include <stdexcept>
#include "C:\includes\tbitfield.h"

typedef unsigned int TELEM;

int TBitField::GetMemIndex(const int n) const {
	return n >> 5;
}

TELEM TBitField::GetMemMask(const int n) const {
	return 1 << (n % 32);
}

TBitField::TBitField(int len) {
	if (len < 0) {
		throw std::out_of_range("BitField length cannot be negative");
	}
	else if (len == 0) {
		BitLen = 0;
		pMem = nullptr;
		MemLen = 0;
	}
	else {
		BitLen = len;
		MemLen = (len + 31) / 32;
		pMem = new TELEM[MemLen];
		for (int i = 0; i < MemLen; ++i) {
			pMem[i] = 0;
		}
	}
}

TBitField::TBitField(const TBitField& bf) {
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;

	if (MemLen > 0) {
		pMem = new TELEM[MemLen];
		for (int i = 0; i < MemLen; ++i) {
			pMem[i] = bf.pMem[i];
		}
	}
	else {
		pMem = nullptr;
	}
}

TBitField::~TBitField() {
	delete[] pMem;
}

int TBitField::GetLength() const {
	return BitLen;
}

void TBitField::SetBit(const int n) {
	if (n >= 0 && n < BitLen) {
		int index = GetMemIndex(n);
		TELEM mask = GetMemMask(n);
		pMem[index] = pMem[index] | mask;
	}
	else {
		throw std::out_of_range("Bit index out of bounds");
	}
}

void TBitField::ClrBit(const int n) {
	if (n >= 0 && n < BitLen) {
		int index = GetMemIndex(n);
		TELEM mask = GetMemMask(n);

		if ((pMem[index] & mask) != 0) {
			pMem[index] = pMem[index] - mask;
		}
	}
	else {
		throw std::out_of_range("Bit index out of bounds");
	}
}
int TBitField::GetBit(const int n) const {
	if (n >= 0 && n < BitLen) {
		int index = GetMemIndex(n);
		TELEM mask = GetMemMask(n);
		return (pMem[index] & mask) != 0;
	}
	else {
		throw std::out_of_range("Bit index out of bounds");
	}
	return 0;
}

int TBitField::operator==(const TBitField& bf) const {
	if (BitLen != bf.BitLen) {
		return 0;
	}

	for (int i = 0; i < MemLen; i++) {
		if (pMem[i] != bf.pMem[i]) {
			return 0;
		}
	}
	return 1;
}

int TBitField::operator!=(const TBitField& bf) const {
	return 1 - (*this == bf);
}

TBitField& TBitField::operator=(const TBitField& bf) {
	if (this != &bf) {
		BitLen = bf.BitLen;
		if (MemLen != bf.MemLen) {
			if (pMem != nullptr) {
				delete[] pMem;
			}
			MemLen = bf.MemLen;
			if (MemLen > 0) {
				pMem = new TELEM[MemLen];
			}
			else {
				pMem = nullptr;
			}
		}

		for (int i = 0; i < MemLen; i++) {
			pMem[i] = bf.pMem[i];
		}
	}
	return *this;
}

TBitField TBitField::operator|(const TBitField& bf) {
	int maxlen;
	if (BitLen > bf.BitLen) {
		maxlen = BitLen;
	}
	else {
		maxlen = bf.BitLen;
	}

	TBitField res(maxlen);

	int minMemLen;
	if (MemLen > bf.MemLen) {
		minMemLen = bf.MemLen;
	}
	else {
		minMemLen = MemLen;
	}

	for (int i = 0; i < minMemLen; i++) {
		res.pMem[i] = pMem[i] | bf.pMem[i];
	}

	if (MemLen > bf.MemLen) {
		for (int i = minMemLen; i < MemLen; i++) {
			res.pMem[i] = pMem[i];
		}
	}
	else {
		for (int i = minMemLen; i < bf.MemLen; i++) {
			res.pMem[i] = bf.pMem[i];
		}
	}
	return res;
}

TBitField TBitField::operator&(const TBitField& bf) {
	int maxlen;
	if (BitLen > bf.BitLen) {
		maxlen = BitLen;
	}
	else {
		maxlen = bf.BitLen;
	}

	TBitField res(maxlen);

	int minMemLen;
	if (MemLen > bf.MemLen) {
		minMemLen = bf.MemLen;
	}
	else {
		minMemLen = MemLen;
	}

	for (int i = 0; i < minMemLen; i++) {
		res.pMem[i] = pMem[i] & bf.pMem[i];
	}
	return res;
}

TBitField TBitField::operator~() {
	TBitField res(BitLen);
	for (int i = 0; i < BitLen; i++) {
		if (GetBit(i) == 0) {
			res.SetBit(i);
		}
	}
	return res;
}

std::ostream& operator<<(std::ostream& stream, const TBitField& bf) {
	int len = bf.GetLength();

	for (int i = 0; i < len; i++) {
		if (bf.GetBit(i)) {
			stream << '1';
		}
		else {
			stream << '0';
		}
	}
	return stream;
}

std::istream& operator>>(std::istream& stream, TBitField& bf) {
	char elem;
	int len = bf.GetLength();
	for (int i = 0; i < len; i++) {
		bf.ClrBit(i);
	}

	for (int i = 0; i < len; i++) {
		stream >> elem;
		if (elem == '1') {
			bf.SetBit(i);
		}
		else if (elem == '0') {
			bf.ClrBit(i);
		}
		else {
			std::cout << "Input error\n";
			break;
		}
	}
	return stream;
}