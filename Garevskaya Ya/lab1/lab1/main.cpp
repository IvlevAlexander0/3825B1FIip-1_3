#include <iostream>
#include "../../includes/Lab1/tset.h"

int main() {
    TSet setA(10);
    setA.InsElem(1);
    setA.InsElem(3);
    setA.InsElem(5);

    std::cout << "Set A: " << setA << std::endl;
    return 0;
}