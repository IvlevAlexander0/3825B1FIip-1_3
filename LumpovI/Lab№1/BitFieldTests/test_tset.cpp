#include <gtest/gtest.h>
#include "tset.h"
#include <sstream>
#include <stdexcept>

TEST(TSetSmoke, ConstructorAndMaxPower) {
    TSet s(100);
    EXPECT_EQ(s.GetMaxPower(), 100);
}

TEST(TSetFunctional, InsertAndCheck) {
    TSet s(20);
    s.InsElem(5);
    s.InsElem(10);
    EXPECT_EQ(s.IsMember(5), 1);
    EXPECT_EQ(s.IsMember(10), 1);
    EXPECT_EQ(s.IsMember(0), 0);
}

//исключения
TEST(TSetExceptions, InvalidMaxPower) {
    EXPECT_THROW(TSet(0), std::invalid_argument);
    EXPECT_THROW(TSet(-1), std::invalid_argument);
}

TEST(TSetExceptions, InsElemOutOfRange) {
    TSet s(10);
    EXPECT_THROW(s.InsElem(-1), std::out_of_range);
    EXPECT_THROW(s.InsElem(10), std::out_of_range);
}

TEST(TSetExceptions, DelElemOutOfRange) {
    TSet s(10);
    EXPECT_THROW(s.DelElem(-1), std::out_of_range);
    EXPECT_THROW(s.DelElem(10), std::out_of_range);
}

TEST(TSetExceptions, IsMemberOutOfRange) {
    TSet s(10);
    EXPECT_THROW(s.IsMember(-1), std::out_of_range);
    EXPECT_THROW(s.IsMember(10), std::out_of_range);
}

TEST(TSetExceptions, UnionDifferentUniverses) {
    TSet a(10), b(20);
    EXPECT_THROW(a + b, std::invalid_argument);
}

TEST(TSetExceptions, IntersectionDifferentUniverses) {
    TSet a(10), b(20);
    EXPECT_THROW(a * b, std::invalid_argument);
}

//случайные тесты
TEST(TSetRandom, Idempotence) {
    for (int iter = 0; iter < 100; ++iter) {
        int mp = 1 + rand() % 100;
        TSet a(mp);
        for (int i = 0; i < mp; ++i) {
            if (rand() % 2) a.InsElem(i);
        }
        EXPECT_EQ(a + a, a);
        EXPECT_EQ(a * a, a);
    }
}

TEST(TSetRandom, Commutativity) {
    for (int iter = 0; iter < 100; ++iter) {
        int mp = 1 + rand() % 100;
        TSet a(mp), b(mp);
        for (int i = 0; i < mp; ++i) {
            if (rand() % 2) a.InsElem(i);
            if (rand() % 2) b.InsElem(i);
        }
        EXPECT_EQ(a + b, b + a);
        EXPECT_EQ(a * b, b * a);
    }
}

TEST(TSetRandom, DeMorgan) {
    for (int iter = 0; iter < 100; ++iter) {
        int mp = 1 + rand() % 100;
        TSet a(mp), b(mp);
        for (int i = 0; i < mp; ++i) {
            if (rand() % 2) a.InsElem(i);
            if (rand() % 2) b.InsElem(i);
        }
        EXPECT_EQ(~(a + b), (~a) * (~b));
        EXPECT_EQ(~(a * b), (~a) + (~b));
    }
}

//ввод вывод
TEST(TSetFunctional, Output) {
    TSet s(10);
    s.InsElem(1); s.InsElem(3); s.InsElem(5);
    std::ostringstream oss;
    oss << s;
    EXPECT_EQ(oss.str(), "{ 1, 3, 5 }");
}

TEST(TSetFunctional, Input) {
    TSet s(10);
    std::istringstream iss("{ 1, 3, 5 }");
    iss >> s;
    EXPECT_EQ(s.IsMember(1), 1);
    EXPECT_EQ(s.IsMember(3), 1);
    EXPECT_EQ(s.IsMember(5), 1);
}