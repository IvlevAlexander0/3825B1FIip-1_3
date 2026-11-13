#include "C:\includes\Lab1\tbitfield.h"
#include <iostream>

const int BitInElem = sizeof(TELEM) * 8;

int TBitField::GetMemIndex(const int n) const {
	if (n < 0 || n >= BitLen) {
		std::cerr << "Error: Bit index " << n << " out of range!\n";
		return 0;
	}
	return n / (BitInElem);
}
TELEM TBitField::GetMemMask(const int n) const {
	if (n < 0 || n >= BitLen) {
		std::cerr << "Error: Bit index " << n << " out of range!\n";
		return 0;
	}
	return 1 << (n % (BitInElem));
}
TBitField::TBitField(int len) {
	if (len < 0) {
		std::cerr << "Negative length!\n";
		BitLen = 0;
		pMem = nullptr;
		MemLen = 0;
		return;
	}
	BitLen = len;
	MemLen = (BitLen + BitInElem - 1) / (BitInElem);
	if (MemLen > 0) {
		pMem = new TELEM[MemLen]();
	}
	else {
		pMem = nullptr;
	}
}
TBitField::TBitField(const TBitField& bf) {
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	if (MemLen > 0) {
		pMem = new TELEM[MemLen];
		for (size_t i = 0; i < MemLen; ++i) {
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
	pMem[GetMemIndex(n)] |= GetMemMask(n);
}
void TBitField::ClrBit(const int n) {
	pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}
int TBitField::GetBit(const int n) const {
	return (pMem[GetMemIndex(n)] & GetMemMask(n)) != 0;
}
int TBitField::operator==(const TBitField& bf) const {
	if (BitLen != bf.BitLen) {
		return 0;
	}
	for (size_t i = 0; i < MemLen; ++i) {
		if (pMem[i] != bf.pMem[i]) {
			return 0;
		}
	}
	return 1;
}
int TBitField::operator!=(const TBitField& bf) const {
	if (*this == bf) {
		return 0;
	}
	return 1;
}
TBitField& TBitField::operator=(const TBitField& bf) {
	if (this != &bf) {
		delete[] pMem;
		BitLen = bf.BitLen;
		MemLen = bf.MemLen;
		if (MemLen > 0) {
			pMem = new TELEM[MemLen];
			for (size_t i = 0; i < MemLen; ++i) {
				pMem[i] = bf.pMem[i];
			}
		}
		else {
			pMem = nullptr;
		}
	}
	return *this;
}
TBitField TBitField::operator|(const TBitField& bf) {
	if (BitLen != bf.BitLen) {
		std::cerr << "Error: Different length!\n";
		return TBitField(0);
	}
	TBitField res(BitLen);
	for (size_t i = 0; i < MemLen; ++i) {
		res.pMem[i] = pMem[i] | bf.pMem[i];
	}
	return res;
}
TBitField TBitField::operator&(const TBitField& bf) {
	if (BitLen != bf.BitLen) {
		std::cerr << "Error: Different length!\n";
		return TBitField(0);
	}
	TBitField res(BitLen);
	for (size_t i = 0; i < MemLen; ++i) {
		res.pMem[i] = pMem[i] & bf.pMem[i];
	}
	return res;
}
TBitField TBitField::operator~() {
	TBitField rev(BitLen);
	for (size_t i = 0; i < MemLen; ++i) {
		rev.pMem[i] = ~pMem[i];
	}
	if (BitLen % BitInElem != 0) {
		TELEM mask = (1 << (BitLen % BitInElem)) - 1;
		rev.pMem[MemLen - 1] &= mask;
	}
	return rev;
}
std::istream& operator>>(std::istream& istr, TBitField& bf) {
	char ch;
	for (size_t i = 0; i < bf.BitLen; ++i) {
		istr >> ch;
		if (ch == '1') {
			bf.SetBit(i);
		}
		else if (ch == '0') {
			bf.ClrBit(i);
		}
		else {
			std::cerr << "Error: Invalid number (must be 1 or 0)!\n";
			return istr;
		}
	}
	return istr;
}
std::ostream& operator<<(std::ostream& ostr, const TBitField& bf) {
	for (size_t i = 0; i < bf.BitLen; ++i) {
		ostr << bf.GetBit(i);
	}
	return ostr;
}