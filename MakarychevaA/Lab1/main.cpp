#include <iostream>
#include "tbitfield.h"
#include "tset.h"

int main()
{
    TBitField bf(8);
    bf.SetBit(0);
    bf.SetBit(3);
    std::cout << "TBitField (8 bits, set to 0 and 3):\n  " << bf << "\n\n";

    TSet a(8), b(8);
    a.InsElem(0); a.InsElem(1);
    b.InsElem(1); b.InsElem(2);

    std::cout << "A         = " << a << "\n";
    std::cout << "B         = " << b << "\n";
    std::cout << "A | B     = " << (a + b) << "\n";
    std::cout << "A & B     = " << (a * b) << "\n";
    std::cout << "~A        = " << (~a) << "\n";
    std::cout << "A + {5}   = " << (a + 5) << "\n";
    std::cout << "A - {0}   = " << (a - 0) << "\n";
    std::cout << "5 in A?   = " << (a.IsMember(5) ? "yes" : "no") << "\n";


    return 0;
}
