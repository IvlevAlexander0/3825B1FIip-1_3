#include "pch.h"
#include "Utmatrixx.h"
#include <stdexcept>
#include <sstream>

TEST(TMatrixTest, ConstructZeroMatrix) {
    TMatrix<int> m(3);
    EXPECT_EQ(m.GetSize(), 3);
    for (int i = 0; i < 3; ++i)
        for (int j = i; j < 3; ++j)
            EXPECT_EQ(m[i][j], 0);
}

TEST(TMatrixTest, ConstructZeroSize) {
    TMatrix<int> m(0);
    EXPECT_EQ(m.GetSize(), 0);
}

TEST(TMatrixTest, ConstructNegativeSizeThrows) {
    EXPECT_THROW(TMatrix<int> m(-1), std::invalid_argument);
}

TEST(TMatrixTest, ConstructTooLargeSizeThrows) {
    EXPECT_THROW(TMatrix<int> m(MAX_MATRIX_SIZE + 1), std::invalid_argument);
}

TEST(TMatrixTest, RowStructure) {
    TMatrix<int> m(4);
    for (int i = 0; i < 4; ++i) {
        EXPECT_EQ(m[i].GetSize(), 4 - i);
        EXPECT_EQ(m[i].GetStartIndex(), i);
    }
}

TEST(TMatrixTest, CopyConstructor) {
    TMatrix<int> a(3);
    a[0][0] = 1; a[0][2] = 3; a[1][1] = 5;
    TMatrix<int> b(a);
    EXPECT_EQ(b.GetSize(), 3);
    EXPECT_EQ(b[0][0], 1);
    EXPECT_EQ(b[0][2], 3);
    EXPECT_EQ(b[1][1], 5);
}

TEST(TMatrixTest, ConstructFromVectorValid) {
    TVector<TVector<int>> rows(3, 0);
    rows[0] = TVector<int>(3, 0);
    rows[1] = TVector<int>(2, 1);
    rows[2] = TVector<int>(1, 2);
    rows[0][0] = 1; rows[1][2] = 5; rows[2][2] = 9;
    TMatrix<int> m(rows);
    EXPECT_EQ(m.GetSize(), 3);
    EXPECT_EQ(m[0][0], 1);
    EXPECT_EQ(m[1][2], 5);
    EXPECT_EQ(m[2][2], 9);
}

TEST(TMatrixTest, EqualitySame) {
    TMatrix<int> a(3), b(3);
    a[0][0] = 1; a[1][2] = 7;
    b[0][0] = 1; b[1][2] = 7;
    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);
}

TEST(TMatrixTest, EqualityDifferentContent) {
    TMatrix<int> a(3), b(3);
    a[0][0] = 1; b[0][0] = 2;
    EXPECT_FALSE(a == b);
    EXPECT_TRUE(a != b);
}

TEST(TMatrixTest, EqualityDifferentSize) {
    TMatrix<int> a(3), b(4);
    EXPECT_FALSE(a == b);
}

TEST(TMatrixTest, Assignment) {
    TMatrix<int> a(3), b(2);
    a[0][0] = 5; a[2][2] = 9;
    b = a;
    EXPECT_EQ(b.GetSize(), 3);
    EXPECT_TRUE(a == b);
}

TEST(TMatrixTest, AssignmentIndependence) {
    TMatrix<int> a(3), b(2);
    a[0][0] = 5;
    b = a;
    b[0][0] = 100;
    EXPECT_EQ(a[0][0], 5);
}

TEST(TMatrixTest, SelfAssignment) {
    TMatrix<int> a(3);
    a[0][0] = 1;
    a = a;
    EXPECT_EQ(a[0][0], 1);
}

TEST(TMatrixTest, MatrixAddition) {
    TMatrix<int> a(3), b(3);
    a[0][0] = 1; a[0][1] = 2; a[1][1] = 3;
    b[0][0] = 10; b[0][1] = 20; b[1][1] = 30;
    TMatrix<int> c = a + b;
    EXPECT_EQ(c[0][0], 11);
    EXPECT_EQ(c[0][1], 22);
    EXPECT_EQ(c[1][1], 33);
    EXPECT_EQ(c[2][2], 0);
}

TEST(TMatrixTest, MatrixAdditionSizeMismatchThrows) {
    TMatrix<int> a(3), b(4);
    EXPECT_THROW(a + b, std::invalid_argument);
}

TEST(TMatrixTest, MatrixSubtraction) {
    TMatrix<int> a(3), b(3);
    a[0][0] = 10; a[1][1] = 20;
    b[0][0] = 1;  b[1][1] = 2;
    TMatrix<int> c = a - b;
    EXPECT_EQ(c[0][0], 9);
    EXPECT_EQ(c[1][1], 18);
    EXPECT_EQ(c[2][2], 0);
}

TEST(TMatrixTest, MatrixSubtractionSizeMismatchThrows) {
    TMatrix<int> a(3), b(4);
    EXPECT_THROW(a - b, std::invalid_argument);
}

TEST(TMatrixTest, AdditionSubtractionRoundTrip) {
    TMatrix<int> a(4), b(4);
    for (int i = 0; i < 4; ++i)
        for (int j = i; j < 4; ++j) {
            a[i][j] = i + j;
            b[i][j] = i * j;
        }
    TMatrix<int> c = (a + b) - b;
    EXPECT_TRUE(c == a);
}

TEST(TMatrixTest, LowerTriangleAccessThrows) {
    TMatrix<int> m(3);
    EXPECT_THROW(m[1][0], std::out_of_range);
}

TEST(TMatrixTest, DiagonalAndUpperAccessible) {
    TMatrix<int> m(3);
    EXPECT_NO_THROW(m[0][0]);
    EXPECT_NO_THROW(m[0][2]);
    EXPECT_NO_THROW(m[1][1]);
    EXPECT_NO_THROW(m[2][2]);
}

TEST(MatrixIO, OutputFormat) {
    TMatrix<int> mt(2);
    mt[0][0] = 1; mt[0][1] = 2;
    mt[1][1] = 3;
    std::ostringstream out;
    out << mt;
    EXPECT_EQ(out.str(), "1 2 \n3 \n");
}

TEST(MatrixIO, InputFormat) {
    TMatrix<int> mt(2);
    std::istringstream in("10 20 30");
    in >> mt;
    EXPECT_EQ(mt[0][0], 10);
    EXPECT_EQ(mt[0][1], 20);
    EXPECT_EQ(mt[1][1], 30);
}