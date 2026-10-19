#include <gtest/gtest.h>
#include "tbitfield.h"
#include <sstream>
#include <stdexcept>

TEST(TBitFieldSmoke, ConstructorAndLength) {
    TBitField bf(100);
    EXPECT_EQ(bf.GetLength(), 100);
}

TEST(TBitFieldFunctional, SetAndGetBit) {
    TBitField bf(64);
    bf.SetBit(5);
    bf.SetBit(31);
    bf.SetBit(63);
    EXPECT_EQ(bf.GetBit(5), 1);
    EXPECT_EQ(bf.GetBit(31), 1);
    EXPECT_EQ(bf.GetBit(63), 1);
}

TEST(TBitFieldExceptions, ZeroLength) {
    EXPECT_THROW(TBitField(0), std::invalid_argument);
}

TEST(TBitFieldExceptions, NegativeLength) {
    EXPECT_THROW(TBitField(-5), std::invalid_argument);
}

TEST(TBitFieldExceptions, SetBitOutOfRange) {
    TBitField bf(10);
    EXPECT_THROW(bf.SetBit(-1), std::out_of_range);
    EXPECT_THROW(bf.SetBit(10), std::out_of_range);
}

TEST(TBitFieldExceptions, ClrBitOutOfRange) {
    TBitField bf(10);
    EXPECT_THROW(bf.ClrBit(-1), std::out_of_range);
    EXPECT_THROW(bf.ClrBit(10), std::out_of_range);
}

TEST(TBitFieldExceptions, GetBitOutOfRange) {
    TBitField bf(10);
    EXPECT_THROW(bf.GetBit(-1), std::out_of_range);
    EXPECT_THROW(bf.GetBit(10), std::out_of_range);
}

TEST(TBitFieldExceptions, OrDifferentLengths) {
    TBitField a(32), b(64);
    EXPECT_THROW(a | b, std::invalid_argument);
}

TEST(TBitFieldExceptions, AndDifferentLengths) {
    TBitField a(32), b(64);
    EXPECT_THROW(a & b, std::invalid_argument);
}

//случайные тесты
TEST(TBitFieldRandom, Idempotence) {
    for (int iter = 0; iter < 100; ++iter) {
        int len = 1 + rand() % 200;
        TBitField a(len);
        for (int i = 0; i < len; ++i) {
            if (rand() % 2) a.SetBit(i);
        }
        EXPECT_EQ(a | a, a);
        EXPECT_EQ(a & a, a);
    }
}

TEST(TBitFieldRandom, Commutativity) {
    for (int iter = 0; iter < 100; ++iter) {
        int len = 1 + rand() % 100;
        TBitField a(len), b(len);
        for (int i = 0; i < len; ++i) {
            if (rand() % 2) a.SetBit(i);
            if (rand() % 2) b.SetBit(i);
        }
        EXPECT_EQ(a | b, b | a);
        EXPECT_EQ(a & b, b & a);
    }
}

TEST(TBitFieldRandom, DoubleNegation) {
    for (int iter = 0; iter < 100; ++iter) {
        int len = 1 + rand() % 100;
        TBitField a(len);
        for (int i = 0; i < len; ++i) {
            if (rand() % 2) a.SetBit(i);
        }
        EXPECT_EQ(~~a, a);
    }
}

TEST(TBitFieldRandom, DeMorgan) {
    for (int iter = 0; iter < 100; ++iter) {
        int len = 1 + rand() % 100;
        TBitField a(len), b(len);
        for (int i = 0; i < len; ++i) {
            if (rand() % 2) a.SetBit(i);
            if (rand() % 2) b.SetBit(i);
        }
        EXPECT_EQ(~(a | b), (~a) & (~b));
        EXPECT_EQ(~(a & b), (~a) | (~b));
    }
}

//ввод вывод
TEST(TBitFieldFunctional, Output) {
    TBitField bf(5);
    bf.SetBit(0); bf.SetBit(2); bf.SetBit(4);
    std::ostringstream oss;
    oss << bf;
    EXPECT_EQ(oss.str(), "10101");
}

TEST(TBitFieldFunctional, Input) {
    TBitField bf(5);
    std::istringstream iss("1 0 1 0 1");
    iss >> bf;
    EXPECT_EQ(bf.GetBit(0), 1);
    EXPECT_EQ(bf.GetBit(1), 0);
    EXPECT_EQ(bf.GetBit(2), 1);
}