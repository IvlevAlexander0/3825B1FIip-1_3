#include "pch.h"
#include "Utmatrixx.h"
#include <stdexcept>
#include <sstream>

TEST(TVectorTest, DefaultConstructor) {
    TVector<int> v;
    EXPECT_EQ(v.GetSize(), 10);
    EXPECT_EQ(v.GetStartIndex(), 0);
}

TEST(TVectorTest, ConstructorWithSizeAndStart) {
    TVector<int> v(5, 3);
    EXPECT_EQ(v.GetSize(), 5);
    EXPECT_EQ(v.GetStartIndex(), 3);
}

TEST(TVectorTest, ConstructorZeroSize) {
    TVector<int> v(0, 0);
    EXPECT_EQ(v.GetSize(), 0);
    EXPECT_EQ(v.GetStartIndex(), 0);
}

TEST(TVectorTest, ConstructorNegativeSizeThrows) {
    EXPECT_THROW(TVector<int> v(-1), std::out_of_range);
}

TEST(TVectorTest, ConstructorNegativeStartThrows) {
    EXPECT_THROW(TVector<int> v(5, -1), std::out_of_range);
}

TEST(TVectorTest, ConstructorTooLargeSizeThrows) {
    EXPECT_THROW(TVector<int> v(MAX_VECTOR_SIZE + 1), std::out_of_range);
}

TEST(TVectorTest, CopyConstructor) {
    TVector<int> a(4, 1);
    for (int i = 1; i <= 4; ++i) a[i] = i * 10;
    TVector<int> b(a);
    EXPECT_EQ(b.GetSize(), 4);
    EXPECT_EQ(b.GetStartIndex(), 1);
    for (int i = 1; i <= 4; ++i) EXPECT_EQ(b[i], i * 10);
}

TEST(TVectorTest, CopyConstructorIndependence) {
    TVector<int> a(3, 0);
    a[0] = 1; a[1] = 2; a[2] = 3;
    TVector<int> b(a);
    b[0] = 100;
    EXPECT_EQ(a[0], 1);
}

TEST(TVectorTest, IndexingAbsolute) {
    TVector<int> v(3, 5);
    v[5] = 1; v[6] = 2; v[7] = 3;
    EXPECT_EQ(v[5], 1);
    EXPECT_EQ(v[6], 2);
    EXPECT_EQ(v[7], 3);
}

TEST(TVectorTest, IndexBelowStartThrows) {
    TVector<int> v(3, 5);
    EXPECT_THROW(v[4], std::out_of_range);
}

TEST(TVectorTest, IndexAboveEndThrows) {
    TVector<int> v(3, 5);
    EXPECT_THROW(v[8], std::out_of_range);
}

TEST(TVectorTest, ConstIndexOutOfRangeThrows) {
    TVector<int> v(2, 0);
    const TVector<int>& cv = v;
    EXPECT_THROW(cv[2], std::out_of_range);
}

TEST(TVectorTest, EqualitySame) {
    TVector<int> a(3, 0), b(3, 0);
    a[0] = 1; a[1] = 2; a[2] = 3;
    b[0] = 1; b[1] = 2; b[2] = 3;
    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);
}

TEST(TVectorTest, EqualityDifferentContent) {
    TVector<int> a(3, 0), b(3, 0);
    a[0] = 1; b[0] = 2;
    EXPECT_FALSE(a == b);
    EXPECT_TRUE(a != b);
}

TEST(TVectorTest, EqualityDifferentSize) {
    TVector<int> a(3, 0), b(4, 0);
    EXPECT_FALSE(a == b);
}

TEST(TVectorTest, EqualityDifferentStartIndex) {
    TVector<int> a(3, 0), b(3, 1);
    EXPECT_FALSE(a == b);
}

TEST(TVectorTest, Assignment) {
    TVector<int> a(3, 0), b(2, 0);
    a[0] = 5; a[1] = 6; a[2] = 7;
    b = a;
    EXPECT_EQ(b.GetSize(), 3);
    EXPECT_TRUE(a == b);
}

TEST(TVectorTest, AssignmentIndependence) {
    TVector<int> a(3, 0), b(2, 0);
    a[0] = 5;
    b = a;
    b[0] = 100;
    EXPECT_EQ(a[0], 5);
}

TEST(TVectorTest, SelfAssignment) {
    TVector<int> a(3, 0);
    a[0] = 1;
    a = a;
    EXPECT_EQ(a.GetSize(), 3);
    EXPECT_EQ(a[0], 1);
}

TEST(TVectorTest, ScalarAddition) {
    TVector<int> v(3, 0);
    v[0] = 1; v[1] = 2; v[2] = 3;
    TVector<int> r = v + 10;
    EXPECT_EQ(r[0], 11); EXPECT_EQ(r[1], 12); EXPECT_EQ(r[2], 13);
}

TEST(TVectorTest, ScalarSubtraction) {
    TVector<int> v(3, 0);
    v[0] = 10; v[1] = 20; v[2] = 30;
    TVector<int> r = v - 5;
    EXPECT_EQ(r[0], 5); EXPECT_EQ(r[1], 15); EXPECT_EQ(r[2], 25);
}

TEST(TVectorTest, ScalarMultiplication) {
    TVector<int> v(3, 0);
    v[0] = 1; v[1] = 2; v[2] = 3;
    TVector<int> r = v * 3;
    EXPECT_EQ(r[0], 3); EXPECT_EQ(r[1], 6); EXPECT_EQ(r[2], 9);
}

TEST(TVectorTest, ScalarPreservesStartIndex) {
    TVector<int> v(3, 5);
    v[5] = 1; v[6] = 2; v[7] = 3;
    TVector<int> r = v + 1;
    EXPECT_EQ(r.GetStartIndex(), 5);
    EXPECT_EQ(r[5], 2);
    EXPECT_EQ(r[7], 4);
}

TEST(TVectorTest, VectorAddition) {
    TVector<int> a(3, 0), b(3, 0);
    a[0] = 1; a[1] = 2; a[2] = 3;
    b[0] = 10; b[1] = 20; b[2] = 30;
    TVector<int> r = a + b;
    EXPECT_EQ(r[0], 11); EXPECT_EQ(r[1], 22); EXPECT_EQ(r[2], 33);
}

TEST(TVectorTest, VectorAdditionSizeMismatchThrows) {
    TVector<int> a(3, 0), b(4, 0);
    EXPECT_THROW(a + b, std::invalid_argument);
}

TEST(TVectorTest, VectorAdditionStartMismatchThrows) {
    TVector<int> a(3, 0), b(3, 1);
    EXPECT_THROW(a + b, std::invalid_argument);
}

TEST(TVectorTest, VectorSubtraction) {
    TVector<int> a(3, 0), b(3, 0);
    a[0] = 10; a[1] = 20; a[2] = 30;
    b[0] = 1; b[1] = 2; b[2] = 3;
    TVector<int> r = a - b;
    EXPECT_EQ(r[0], 9); EXPECT_EQ(r[1], 18); EXPECT_EQ(r[2], 27);
}

TEST(TVectorTest, VectorSubtractionSizeMismatchThrows) {
    TVector<int> a(3, 0), b(2, 0);
    EXPECT_THROW(a - b, std::invalid_argument);
}

TEST(TVectorTest, DotProduct) {
    TVector<int> a(3, 0), b(3, 0);
    a[0] = 1; a[1] = 2; a[2] = 3;
    b[0] = 4; b[1] = 5; b[2] = 6;
    EXPECT_EQ(a * b, 32);
}

TEST(TVectorTest, DotProductSizeMismatchThrows) {
    TVector<int> a(3, 0), b(2, 0);
    EXPECT_THROW(a * b, std::invalid_argument);
}

TEST(TVectorTest, DotProductDouble) {
    TVector<double> a(2, 0), b(2, 0);
    a[0] = 1.5; a[1] = 2.0;
    b[0] = 2.0; b[1] = 3.0;
    EXPECT_DOUBLE_EQ(a * b, 9.0);
}

TEST(VectorIO, OutputFormat) {
    TVector<int> v(3, 0);
    v[0] = 1; v[1] = 2; v[2] = 3;
    std::ostringstream out;
    out << v;
    EXPECT_EQ(out.str(), "1 2 3 ");
}

TEST(VectorIO, InputFormat) {
    TVector<int> v(3, 0);
    std::istringstream in("4 5 6");
    in >> v;
    EXPECT_EQ(v[0], 4);
    EXPECT_EQ(v[1], 5);
    EXPECT_EQ(v[2], 6);
}