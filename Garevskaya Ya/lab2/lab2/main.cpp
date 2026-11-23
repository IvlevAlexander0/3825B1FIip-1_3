#include <iostream>
#include "utmatrix.h"

int main() {
    try {
        TMatrix<int> A(3);
        TMatrix<int> B(3);

        std::cout << "Enter matrix A:\n";
        std::cin >> A;

        std::cout << "Enter matrix B:\n";
        std::cin >> B;

        std::cout << "\nMatrix A:\n" << A << "\n";
        std::cout << "\nMatrix B:\n" << B << "\n";

        TMatrix<int> C = A + B;
        std::cout << "\nA + B:\n" << C << "\n";

        if (A == B) std::cout << "\nA and B are equal.\n";
        else std::cout << "\nA and B are not equal.\n";

    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}