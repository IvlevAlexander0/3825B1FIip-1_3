#include <gtest/gtest.h>
#include "utmatrix.h"
#include <sstream>

TEST(TVectorTest, ConstructorAndAccess) {
    TVector<int> v(3, 1);
    EXPECT_EQ(v.GetSize(), 3);
    EXPECT_EQ(v.GetStartIndex(), 1);
    EXPECT_EQ(v[1], 0);
    v[1] = 10;
    EXPECT_EQ(v[1], 10);
}

TEST(TVectorTest, Exceptions) {
    EXPECT_THROW(TVector<int> v(-1), std::invalid_argument);
    TVector<int> v(3);
    EXPECT_THROW(v[-1], std::out_of_range);
    EXPECT_THROW(v[3], std::out_of_range);
}

TEST(TVectorTest, CopyAndAssign) {
    TVector<int> a(3);
    a[0] = 1; a[1] = 2; a[2] = 3;

    TVector<int> b(a);
    EXPECT_TRUE(a == b);

    TVector<int> c(5);
    c = a;
    EXPECT_EQ(c.GetSize(), 3);
    EXPECT_TRUE(a == c);
}

TEST(TVectorTest, Equality) {
    TVector<int> a(3), b(3);
    EXPECT_TRUE(a == b);
    a[0] = 5;
    EXPECT_TRUE(a != b);
}

TEST(TVectorTest, ScalarOperations) {
    TVector<int> a(3);
    a[0] = 1; a[1] = 2; a[2] = 3;

    TVector<int> b = a + 10;
    EXPECT_EQ(b[0], 11);

    TVector<int> c = a - 1;
    EXPECT_EQ(c[0], 0);

    TVector<int> d = a * 2;
    EXPECT_EQ(d[2], 6);
}

TEST(TVectorTest, VectorOperations) {
    TVector<int> a(2), b(2);
    a[0] = 1; a[1] = 2;
    b[0] = 3; b[1] = 4;

    TVector<int> sum = a + b;
    EXPECT_EQ(sum[0], 4);
    EXPECT_EQ(sum[1], 6);

    TVector<int> diff = b - a;
    EXPECT_EQ(diff[0], 2);

    int dot = a * b;
    EXPECT_EQ(dot, 11);
}

TEST(TVectorTest, VectorOperationsException) {
    TVector<int> a(2), b(3);
    EXPECT_THROW(a + b, std::invalid_argument);
    EXPECT_THROW(a * b, std::invalid_argument);
}

TEST(TVectorTest, IOOperator) {
    TVector<int> v(3);
    std::stringstream ss("1 2 3");
    ss >> v;
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);

    std::stringstream out;
    out << v;
    EXPECT_EQ(out.str(), "1 2 3");
}

TEST(TMatrixTest, ConstructorAndAccess) {
    TMatrix<int> m(3);
    EXPECT_EQ(m.GetSize(), 3);
    EXPECT_EQ(m[0].GetSize(), 3);
    EXPECT_EQ(m[1].GetSize(), 2);
    EXPECT_EQ(m[2].GetSize(), 1);

    m[0][0] = 5;
    EXPECT_EQ(m[0][0], 5);
}

TEST(TMatrixTest, CopyAndAssign) {
    TMatrix<int> a(2);
    a[0][0] = 1;
    TMatrix<int> b(a);
    EXPECT_TRUE(a == b);
}

TEST(TMatrixTest, AddSub) {
    TMatrix<int> a(2), b(2);
    a[0][0] = 1; a[0][1] = 2;
    b[0][0] = 3; b[0][1] = 4;

    TMatrix<int> sum = a + b;
    EXPECT_EQ(sum[0][0], 4);

    TMatrix<int> diff = b - a;
    EXPECT_EQ(diff[0][1], 2);

    TMatrix<int> c(3);
    EXPECT_THROW(a + c, std::invalid_argument);
}