#include "pch.h"
#include <gtest/gtest.h>
#include "realization_utmatrix.h"

//TVector
TEST(TVector, DefaultConstructor)
{
    TVector<int> v;
    EXPECT_EQ(v.GetSize(), 10);
    EXPECT_EQ(v.GetStartIndex(), 0);
}

TEST(TVector, ParametrizedConstructor)
{
    TVector<int> v(5, 2);
    EXPECT_EQ(v.GetSize(), 5);
    EXPECT_EQ(v.GetStartIndex(), 2);
}

TEST(TVector, CopyConstructor)
{
    TVector<int> v(4, 1);
    v[1] = 10; v[2] = 20; v[3] = 30;
    TVector<int> c(v);
    EXPECT_EQ(c, v);
}

TEST(TVector, IndexAccess)
{
    TVector<int> v(5, 0);
    for (int i = 0; i < 5; ++i) v[i] = i * i;
    for (int i = 0; i < 5; ++i) EXPECT_EQ(v[i], i * i);
}

TEST(TVector, IndexOutOfRange)
{
    TVector<int> v(5, 1);
    EXPECT_THROW(v[0], std::out_of_range);
    EXPECT_THROW(v[5], std::out_of_range);
}

TEST(TVector, EqualityAndInequality)
{
    TVector<int> a(4, 0), b(4, 0);
    EXPECT_EQ(a, b);
    a[1] = 5;
    EXPECT_NE(a, b);
}

TEST(TVector, Assignment)
{
    TVector<int> a(4, 0), b(4, 0);
    a[0] = 1; a[1] = 2;
    b = a;
    EXPECT_EQ(a, b);
}

TEST(TVector, SelfAssignment)
{
    TVector<int> a(3, 0);
    a[0] = 7;
    a = a;
    EXPECT_EQ(a[0], 7);
}

TEST(TVector, ScalarAdd)
{
    TVector<int> v(3, 0);
    v[0] = 1; v[1] = 2; v[2] = 3;
    TVector<int> r = v + 10;
    EXPECT_EQ(r[0], 11); EXPECT_EQ(r[1], 12); EXPECT_EQ(r[2], 13);
}

TEST(TVector, ScalarSub)
{
    TVector<int> v(3, 0);
    v[0] = 5; v[1] = 6; v[2] = 7;
    TVector<int> r = v - 2;
    EXPECT_EQ(r[0], 3); EXPECT_EQ(r[1], 4); EXPECT_EQ(r[2], 5);
}

TEST(TVector, ScalarMul)
{
    TVector<int> v(3, 0);
    v[0] = 1; v[1] = 2; v[2] = 3;
    TVector<int> r = v * 3;
    EXPECT_EQ(r[0], 3); EXPECT_EQ(r[1], 6); EXPECT_EQ(r[2], 9);
}

TEST(TVector, VectorAdd)
{
    TVector<int> a(3, 0), b(3, 0);
    a[0] = 1; a[1] = 2; a[2] = 3;
    b[0] = 4; b[1] = 5; b[2] = 6;
    TVector<int> r = a + b;
    EXPECT_EQ(r[0], 5); EXPECT_EQ(r[1], 7); EXPECT_EQ(r[2], 9);
}

TEST(TVector, VectorSub)
{
    TVector<int> a(3, 0), b(3, 0);
    a[0] = 4; a[1] = 5; a[2] = 6;
    b[0] = 1; b[1] = 2; b[2] = 3;
    TVector<int> r = a - b;
    EXPECT_EQ(r[0], 3); EXPECT_EQ(r[1], 3); EXPECT_EQ(r[2], 3);
}

TEST(TVector, DotProduct)
{
    TVector<int> a(3, 0), b(3, 0);
    a[0] = 1; a[1] = 2; a[2] = 3;
    b[0] = 4; b[1] = 5; b[2] = 6;
    EXPECT_EQ(a * b, 32);
}

TEST(TVector, SizeMismatchThrows)
{
    TVector<int> a(3, 0), b(4, 0);
    EXPECT_THROW(a + b, std::invalid_argument);
    EXPECT_THROW(a - b, std::invalid_argument);
    EXPECT_THROW(a * b, std::invalid_argument);
}

TEST(TVector, InputWithNonZeroStartIndex)
{
    TVector<int> v(4, 2);
    std::istringstream is("10 20");
    is >> v;
    EXPECT_EQ(v[2], 10);
    EXPECT_EQ(v[3], 20);
}

//TMatrix

TEST(TMatrix, DefaultConstructor)
{
    TMatrix<int> m(5);
    EXPECT_EQ(m.GetSize(), 5);
    for (int i = 0; i < 5; ++i)
    {
        EXPECT_EQ(m[i].GetSize(), 5);
        EXPECT_EQ(m[i].GetStartIndex(), i);
    }
}

TEST(TMatrix, CopyConstructor)
{
    TMatrix<int> m(3);
    m[0][0] = 1; m[0][1] = 2; m[0][2] = 3;
    m[1][1] = 4; m[1][2] = 5;
    m[2][2] = 6;
    TMatrix<int> c(m);
    EXPECT_EQ(c, m);
}

TEST(TMatrix, Equality)
{
    TMatrix<int> a(3), b(3);
    EXPECT_EQ(a, b);

    a[0][0] = 1;
    EXPECT_NE(a, b);

    b[0][0] = 1;
    EXPECT_EQ(a, b);
}

TEST(TMatrix, Assignment)
{
    TMatrix<int> a(3), b(3);
    a[0][1] = 42;
    b = a;
    EXPECT_EQ(a, b);
}

TEST(TMatrix, Addition)
{
    TMatrix<int> a(3), b(3);
    a[0][0] = 1; a[0][1] = 2; a[0][2] = 3;
    a[1][1] = 4; a[1][2] = 5; a[2][2] = 6;

    b[0][0] = 10; b[0][1] = 20; b[0][2] = 30;
    b[1][1] = 40; b[1][2] = 50; b[2][2] = 60;

    TMatrix<int> c = a + b;
    EXPECT_EQ(c[0][0], 11); EXPECT_EQ(c[0][1], 22); EXPECT_EQ(c[0][2], 33);
    EXPECT_EQ(c[1][1], 44); EXPECT_EQ(c[1][2], 55);
    EXPECT_EQ(c[2][2], 66);
}

TEST(TMatrix, Subtraction)
{
    TMatrix<int> a(3), b(3);
    a[0][0] = 10; a[1][1] = 20; a[2][2] = 30;
    b[0][0] = 1;  b[1][1] = 2;  b[2][2] = 3;

    TMatrix<int> c = a - b;
    EXPECT_EQ(c[0][0], 9);
    EXPECT_EQ(c[1][1], 18);
    EXPECT_EQ(c[2][2], 27);
}

TEST(TMatrix, SizeMismatchThrows)
{
    TMatrix<int> a(3), b(4);
    EXPECT_THROW(a + b, std::invalid_argument);
    EXPECT_THROW(a - b, std::invalid_argument);
}

TEST(TMatrix, EdgeCaseSizeZero)
{
    TMatrix<int> m(0);
    EXPECT_EQ(m.GetSize(), 0);
    TMatrix<int> n(0);
    EXPECT_EQ(m, n);
}

TEST(TMatrix, ConversionFromTVectorOfTVector)
{
    TVector<TVector<int>> vv(2, 0);
    vv[0] = TVector<int>(2, 0);
    vv[1] = TVector<int>(2, 1);
    vv[0][0] = 5; vv[0][1] = 6;
    vv[1][1] = 7;

    TMatrix<int> m(vv);
    EXPECT_EQ(m.GetSize(), 2);
    EXPECT_EQ(m[0][0], 5);
    EXPECT_EQ(m[0][1], 6);
    EXPECT_EQ(m[1][1], 7);
}

TEST(TMatrix, OutputUpperTriangular)
{
    TMatrix<int> m(3);
    m[0][0] = 1; m[0][1] = 2; m[0][2] = 3;
    m[1][1] = 4; m[1][2] = 5;
    m[2][2] = 6;

    std::ostringstream os;
    os << m;
    EXPECT_EQ(os.str(), "1 2 3\n0 4 5\n0 0 6");
}