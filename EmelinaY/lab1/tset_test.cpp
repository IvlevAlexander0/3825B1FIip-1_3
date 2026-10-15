#include "pch.h"
#include "tset.h"
#include <sstream>
#include <stdexcept>
#include <cstdlib>

TEST(TSetTest, ConstructEmpty) {
    TSet s(10);
    EXPECT_EQ(s.GetMaxPower(), 10);
    for (int i = 0; i < 10; ++i) {
        EXPECT_EQ(s.IsMember(i), 0) << "element " << i << " should not be in set";
    }
}

TEST(TSetTest, CopyConstructor) {
    TSet a(10);
    a.InsElem(1);
    a.InsElem(5);

    TSet b(a);
    EXPECT_EQ(b.GetMaxPower(), 10);
    EXPECT_EQ(b.IsMember(1), 1);
    EXPECT_EQ(b.IsMember(5), 1);

    b.DelElem(1);
    EXPECT_EQ(a.IsMember(1), 1);
    EXPECT_EQ(b.IsMember(1), 0);
}

TEST(TSetTest, ConstructFromBitField) {
    TBitField bf(8);
    bf.SetBit(0);
    bf.SetBit(3);
    bf.SetBit(7);

    TSet s(bf);
    EXPECT_EQ(s.GetMaxPower(), 8);
    EXPECT_EQ(s.IsMember(0), 1);
    EXPECT_EQ(s.IsMember(3), 1);
    EXPECT_EQ(s.IsMember(7), 1);
    EXPECT_EQ(s.IsMember(1), 0);
}

TEST(TSetTest, ConvertToBitField) {
    TSet s(6);
    s.InsElem(1);
    s.InsElem(4);

    TBitField bf = s;
    EXPECT_EQ(bf.GetLength(), 6);
    EXPECT_EQ(bf.GetBit(1), 1);
    EXPECT_EQ(bf.GetBit(4), 1);
    EXPECT_EQ(bf.GetBit(0), 0);
}

TEST(TSetTest, InsertAndCheck) {
    TSet s(10);
    s.InsElem(3);
    EXPECT_EQ(s.IsMember(3), 1);

    s.InsElem(7);
    EXPECT_EQ(s.IsMember(3), 1);
    EXPECT_EQ(s.IsMember(7), 1);
}

TEST(TSetTest, DeleteElement) {
    TSet s(10);
    s.InsElem(4);
    s.DelElem(4);
    EXPECT_EQ(s.IsMember(4), 0);
}

TEST(TSetTest, InsertOutOfRangeThrows) {
    TSet s(8);
    EXPECT_THROW(s.InsElem(-1), std::out_of_range);
    EXPECT_THROW(s.InsElem(8), std::out_of_range);
    EXPECT_THROW(s.InsElem(100), std::out_of_range);
}

TEST(TSetTest, DeleteOutOfRangeThrows) {
    TSet s(8);
    EXPECT_THROW(s.DelElem(-1), std::out_of_range);
    EXPECT_THROW(s.DelElem(8), std::out_of_range);
}

TEST(TSetTest, IsMemberOutOfRangeThrows) {
    TSet s(8);
    EXPECT_THROW(s.IsMember(-1), std::out_of_range);
    EXPECT_THROW(s.IsMember(8), std::out_of_range);
}

TEST(TSetTest, EqualitySameContent) {
    TSet a(10), b(10);
    a.InsElem(1);
    b.InsElem(1);
    a.InsElem(5);
    b.InsElem(5);
    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);
}

TEST(TSetTest, EqualityDifferentContent) {
    TSet a(10), b(10);
    a.InsElem(1);
    b.InsElem(2);
    EXPECT_FALSE(a == b);
    EXPECT_TRUE(a != b);
}

TEST(TSetTest, EqualityDifferentPower) {
    TSet a(10), b(12);
    a.InsElem(1);
    b.InsElem(1);
    EXPECT_FALSE(a == b);
}

TEST(TSetTest, Assignment) {
    TSet a(10), b(5);
    a.InsElem(1);
    a.InsElem(9);

    b = a;
    EXPECT_EQ(b.GetMaxPower(), 10);
    EXPECT_TRUE(a == b);

    b.DelElem(1);
    EXPECT_EQ(a.IsMember(1), 1);
}

TEST(TSetTest, PlusElement) {
    TSet a(10);
    a.InsElem(1);

    TSet b = a + 5;
    EXPECT_EQ(b.IsMember(1), 1);
    EXPECT_EQ(b.IsMember(5), 1);
    EXPECT_EQ(a.IsMember(5), 0);
}

TEST(TSetTest, PlusElementOutOfRangeThrows) {
    TSet a(8);
    EXPECT_THROW(a + (-1), std::out_of_range);
    EXPECT_THROW(a + 8, std::out_of_range);
}

TEST(TSetTest, MinusElement) {
    TSet a(10);
    a.InsElem(1);
    a.InsElem(5);

    TSet b = a - 5;
    EXPECT_EQ(b.IsMember(1), 1);
    EXPECT_EQ(b.IsMember(5), 0);
    EXPECT_EQ(a.IsMember(5), 1);
}

TEST(TSetTest, MinusElementOutOfRangeThrows) {
    TSet a(8);
    EXPECT_THROW(a - (-1), std::out_of_range);
    EXPECT_THROW(a - 8, std::out_of_range);
}

TEST(TSetTest, UnionTwoSets) {
    TSet a(8), b(8);
    a.InsElem(0);
    a.InsElem(2);
    b.InsElem(1);
    b.InsElem(2);

    TSet c = a + b;
    EXPECT_EQ(c.IsMember(0), 1);
    EXPECT_EQ(c.IsMember(1), 1);
    EXPECT_EQ(c.IsMember(2), 1);
    EXPECT_EQ(c.IsMember(3), 0);
}

TEST(TSetTest, UnionDifferentPowerThrows) {
    TSet a(4), b(8);
    a.InsElem(0);
    b.InsElem(7);
    EXPECT_THROW(a + b, std::invalid_argument);
}

TEST(TSetTest, IntersectionTwoSets) {
    TSet a(8), b(8);
    a.InsElem(0);
    a.InsElem(2);
    a.InsElem(5);
    b.InsElem(2);
    b.InsElem(5);
    b.InsElem(7);

    TSet c = a * b;
    EXPECT_EQ(c.IsMember(0), 0);
    EXPECT_EQ(c.IsMember(2), 1);
    EXPECT_EQ(c.IsMember(5), 1);
    EXPECT_EQ(c.IsMember(7), 0);
}


TEST(TSetTest, IntersectionDifferentPowerThrows) {
    TSet a(4), b(8);
    a.InsElem(1);
    b.InsElem(1);
    b.InsElem(7);
    EXPECT_THROW(a * b, std::invalid_argument);
}

TEST(TSetTest, ComplementSimple) {
    TSet a(5);
    a.InsElem(0);
    a.InsElem(4);

    TSet b = ~a;
    EXPECT_EQ(b.GetMaxPower(), 5);
    EXPECT_EQ(b.IsMember(0), 0);
    EXPECT_EQ(b.IsMember(1), 1);
    EXPECT_EQ(b.IsMember(2), 1);
    EXPECT_EQ(b.IsMember(3), 1);
    EXPECT_EQ(b.IsMember(4), 0);
}

TEST(TSetTest, DoubleComplement) {
    TSet a(7);
    a.InsElem(0);
    a.InsElem(3);
    a.InsElem(6);
    TSet b = ~(~a);
    EXPECT_TRUE(a == b);
}

TEST(TSetTest, OutputEmpty) {
    TSet s(10);
    std::ostringstream os;
    os << s;
    EXPECT_EQ(os.str(), "{}");
}

TEST(TSetTest, OutputNonEmpty) {
    TSet s(10);
    s.InsElem(0);
    s.InsElem(2);
    s.InsElem(5);

    std::ostringstream os;
    os << s;
    EXPECT_EQ(os.str(), "{0, 2, 5}");
}

TEST(TSetTest, InputValid) {
    TSet s(10);
    std::istringstream is("{0, 2, 5}");
    is >> s;

    EXPECT_EQ(s.IsMember(0), 1);
    EXPECT_EQ(s.IsMember(1), 0);
    EXPECT_EQ(s.IsMember(2), 1);
    EXPECT_EQ(s.IsMember(5), 1);
    EXPECT_EQ(s.IsMember(6), 0);
}

TEST(TSetTest, InputOverwritesPrevious) {
    TSet s(10);
    s.InsElem(0);
    s.InsElem(1);

    std::istringstream is("{5}");
    is >> s;

    EXPECT_EQ(s.IsMember(0), 0);
    EXPECT_EQ(s.IsMember(1), 0);
    EXPECT_EQ(s.IsMember(5), 1);
}

TEST(TSetTest, InputInvalidCharacterThrows) {
    TSet s(10);
    std::istringstream is("{0, x, 2}");
    EXPECT_THROW(is >> s, std::invalid_argument);
}

TEST(TSetTest, InputMissingCloseBraceThrows) {
    TSet s(10);
    std::istringstream is("{0, 2, 5");
    EXPECT_THROW(is >> s, std::invalid_argument);
}

TEST(TSetTest, InputElementOutOfRangeThrows) {
    TSet s(5);
    std::istringstream is("{0, 7}");
    EXPECT_THROW(is >> s, std::out_of_range);
}

TEST(TSetTest, RandomMembership) {
    srand(54321);
    const int N = 32;
    TSet s(N);
    bool expected[N] = { false };

    for (int step = 0; step < 300; ++step) {
        int pos = rand() % N;
        int op = rand() % 3;
        if (op == 0) {
            s.InsElem(pos);
            expected[pos] = true;
        }
        else if (op == 1) {
            s.DelElem(pos);
            expected[pos] = false;
        }
        else {
            EXPECT_EQ(s.IsMember(pos), expected[pos] ? 1 : 0);
        }
    }
    for (int i = 0; i < N; ++i) {
        EXPECT_EQ(s.IsMember(i), expected[i] ? 1 : 0) << "element " << i;
    }
}