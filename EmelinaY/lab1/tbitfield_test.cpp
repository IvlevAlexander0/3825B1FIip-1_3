#include "pch.h"
#include "tbitfield.h"
#include <sstream>
#include <stdexcept>
#include <cstdlib>

TEST(TBitFieldTest, ConstructZeroLength) {
    TBitField bf(0);
    EXPECT_EQ(bf.GetLength(), 0);
}

TEST(TBitFieldTest, ConstructPositiveLengthAllBitsZero) {
    TBitField bf(10);
    EXPECT_EQ(bf.GetLength(), 10);
    for (int i = 0; i < 10; ++i) {
        EXPECT_EQ(bf.GetBit(i), 0) << "bit " << i << " should be 0";
    }
}

TEST(TBitFieldTest, ConstructNonAlignedLength) {
    TBitField bf(37);
    EXPECT_EQ(bf.GetLength(), 37);
    for (int i = 0; i < 37; ++i) {
        EXPECT_EQ(bf.GetBit(i), 0);
    }
}

TEST(TBitFieldTest, ConstructNegativeLengthThrows) {
    EXPECT_THROW(TBitField bf(-1), std::invalid_argument);
}

TEST(TBitFieldTest, CopyConstructor) {
    TBitField a(10);
    a.SetBit(0);
    a.SetBit(5);
    a.SetBit(9);

    TBitField b(a);

    EXPECT_EQ(b.GetLength(), a.GetLength());
    EXPECT_TRUE(a == b);

    b.ClrBit(5);
    EXPECT_EQ(a.GetBit(5), 1);
    EXPECT_EQ(b.GetBit(5), 0);
}

TEST(TBitFieldTest, SetAndClrBit) {
    TBitField bf(16);
    bf.SetBit(3);
    EXPECT_EQ(bf.GetBit(3), 1);

    bf.ClrBit(3);
    EXPECT_EQ(bf.GetBit(3), 0);
}

TEST(TBitFieldTest, SetBitOutOfRangeThrows) {
    TBitField bf(8);
    EXPECT_THROW(bf.SetBit(-1), std::out_of_range);
    EXPECT_THROW(bf.SetBit(8), std::out_of_range);
    EXPECT_THROW(bf.SetBit(100), std::out_of_range);
}

TEST(TBitFieldTest, ClrBitOutOfRangeThrows) {
    TBitField bf(8);
    EXPECT_THROW(bf.ClrBit(-1), std::out_of_range);
    EXPECT_THROW(bf.ClrBit(8), std::out_of_range);
}

TEST(TBitFieldTest, GetBitOutOfRangeThrows) {
    TBitField bf(8);
    EXPECT_THROW(bf.GetBit(-1), std::out_of_range);
    EXPECT_THROW(bf.GetBit(8), std::out_of_range);
}

TEST(TBitFieldTest, EqualitySameContent) {
    TBitField a(10), b(10);
    a.SetBit(2);
    b.SetBit(2);
    a.SetBit(7);
    b.SetBit(7);
    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);
}

TEST(TBitFieldTest, EqualityDifferentContent) {
    TBitField a(10), b(10);
    a.SetBit(2);
    b.SetBit(3);
    EXPECT_FALSE(a == b);
    EXPECT_TRUE(a != b);
}

TEST(TBitFieldTest, EqualityDifferentLength) {
    TBitField a(10), b(11);
    a.SetBit(2);
    b.SetBit(2);
    EXPECT_FALSE(a == b);
    EXPECT_TRUE(a != b);
}

TEST(TBitFieldTest, Assignment) {
    TBitField a(10), b(5);
    a.SetBit(1);
    a.SetBit(9);

    b = a;
    EXPECT_EQ(b.GetLength(), 10);
    EXPECT_TRUE(a == b);

    b.ClrBit(1);
    EXPECT_EQ(a.GetBit(1), 1);
}

TEST(TBitFieldTest, OrSameLength) {
    TBitField a(8), b(8);
    a.SetBit(0);
    a.SetBit(2);
    b.SetBit(1);
    b.SetBit(2);

    TBitField c = a | b;
    EXPECT_EQ(c.GetLength(), 8);
    EXPECT_EQ(c.GetBit(0), 1);
    EXPECT_EQ(c.GetBit(1), 1);
    EXPECT_EQ(c.GetBit(2), 1);
    EXPECT_EQ(c.GetBit(3), 0);
}

TEST(TBitFieldTest, OrDifferentLengthThrows) {
    TBitField a(4), b(8);
    EXPECT_THROW(a | b, std::invalid_argument);
}

TEST(TBitFieldTest, AndSameLength) {
    TBitField a(8), b(8);
    a.SetBit(0);
    a.SetBit(2);
    a.SetBit(5);
    b.SetBit(2);
    b.SetBit(5);
    b.SetBit(7);

    TBitField c = a & b;
    EXPECT_EQ(c.GetLength(), 8);
    EXPECT_EQ(c.GetBit(0), 0);
    EXPECT_EQ(c.GetBit(2), 1);
    EXPECT_EQ(c.GetBit(5), 1);
    EXPECT_EQ(c.GetBit(7), 0);
}

TEST(TBitFieldTest, AndDifferentLengthThrows) {
    TBitField a(4), b(8);
    EXPECT_THROW(a & b, std::invalid_argument);
}

TEST(TBitFieldTest, NotSimple) {
    TBitField a(5);
    a.SetBit(0);
    a.SetBit(4);

    TBitField b = ~a;
    EXPECT_EQ(b.GetLength(), 5);
    EXPECT_EQ(b.GetBit(0), 0);
    EXPECT_EQ(b.GetBit(1), 1);
    EXPECT_EQ(b.GetBit(2), 1);
    EXPECT_EQ(b.GetBit(3), 1);
    EXPECT_EQ(b.GetBit(4), 0);
}

TEST(TBitFieldTest, DoubleNot) {
    TBitField a(13);
    a.SetBit(0);
    a.SetBit(5);
    a.SetBit(12);
    TBitField b = ~(~a);
    EXPECT_TRUE(a == b);
}

TEST(TBitFieldTest, Output) {
    TBitField bf(5);
    bf.SetBit(0);
    bf.SetBit(2);
    bf.SetBit(4);

    std::ostringstream os;
    os << bf;
    EXPECT_EQ(os.str(), "10101");
}

TEST(TBitFieldTest, InputValid) {
    TBitField bf(5);
    std::istringstream is("10101");
    is >> bf;

    EXPECT_EQ(bf.GetBit(0), 1);
    EXPECT_EQ(bf.GetBit(1), 0);
    EXPECT_EQ(bf.GetBit(2), 1);
    EXPECT_EQ(bf.GetBit(3), 0);
    EXPECT_EQ(bf.GetBit(4), 1);
}

TEST(TBitFieldTest, RandomSetGetClr) {
    srand(12345);
    const int N = 64;
    TBitField bf(N);
    bool expected[N] = { false };

    for (int step = 0; step < 300; ++step) {
        int pos = rand() % N;
        int op = rand() % 3; 
        if (op == 0) {
            bf.SetBit(pos);
            expected[pos] = true;
        }
        else if (op == 1) {
            bf.ClrBit(pos);
            expected[pos] = false;
        }
        else {
            EXPECT_EQ(bf.GetBit(pos), expected[pos] ? 1 : 0);
        }
    }
    for (int i = 0; i < N; ++i) {
        EXPECT_EQ(bf.GetBit(i), expected[i] ? 1 : 0) << "bit " << i;
    }
}