#include <gtest/gtest.h>
#include "tbitfield.h"
#include <sstream>

// Базовые тесты
TEST(TBitFieldSmoke, ConstructorAndLength) {
    TBitField bf(100);
    EXPECT_EQ(bf.GetLength(), 100);
}

TEST(TBitFieldSmoke, AllBitsInitiallyZero) {
    TBitField bf(100);
    for (int i = 0; i < 100; ++i) {
        EXPECT_EQ(bf.GetBit(i), 0) << "Бит " << i << " должен быть 0";
    }
}


// Тесты установки/очистки битов
TEST(TBitFieldFunctional, SetAndGetBit) {
    TBitField bf(64);
    bf.SetBit(5);
    bf.SetBit(31);
    bf.SetBit(63);

    EXPECT_EQ(bf.GetBit(5), 1);
    EXPECT_EQ(bf.GetBit(31), 1);
    EXPECT_EQ(bf.GetBit(63), 1);
    EXPECT_EQ(bf.GetBit(0), 0);
    EXPECT_EQ(bf.GetBit(30), 0);
}

TEST(TBitFieldFunctional, ClrBit) {
    TBitField bf(64);
    bf.SetBit(10);
    EXPECT_EQ(bf.GetBit(10), 1);

    bf.ClrBit(10);
    EXPECT_EQ(bf.GetBit(10), 0);
}


// Краевые случаи
TEST(TBitFieldEdgeCases, OutOfRangeAccess) {
    TBitField bf(10);
    bf.SetBit(-1);
    bf.SetBit(10);
    bf.SetBit(1000);

    EXPECT_EQ(bf.GetBit(-1), 0);
    EXPECT_EQ(bf.GetBit(10), 0);
    EXPECT_EQ(bf.GetBit(1000), 0);
}

TEST(TBitFieldEdgeCases, LengthNotMultipleOf32) {
    TBitField bf(33);
    bf.SetBit(32);
    EXPECT_EQ(bf.GetBit(32), 1);

    bf.SetBit(33);
    EXPECT_EQ(bf.GetBit(33), 0);
}

TEST(TBitFieldEdgeCases, SingleBit) {
    TBitField bf(1);
    EXPECT_EQ(bf.GetLength(), 1);
    bf.SetBit(0);
    EXPECT_EQ(bf.GetBit(0), 1);
}


// Копирование и присваивание
TEST(TBitFieldFunctional, CopyConstructor) {
    TBitField a(64);
    a.SetBit(5);
    a.SetBit(40);

    TBitField b(a);
    EXPECT_EQ(b.GetBit(5), 1);
    EXPECT_EQ(b.GetBit(40), 1);
    EXPECT_EQ(a, b);
}

TEST(TBitFieldFunctional, AssignmentOperator) {
    TBitField a(64);
    a.SetBit(5);
    a.SetBit(40);

    TBitField b(64);
    b = a;
    EXPECT_EQ(b, a);
}

TEST(TBitFieldEdgeCases, AssignmentDifferentLength) {
    TBitField a(64);
    a.SetBit(5);
    a.SetBit(40);

    TBitField b(32);
    b = a;
    EXPECT_EQ(b.GetLength(), 64);
    EXPECT_EQ(b.GetBit(5), 1);
    EXPECT_EQ(b.GetBit(40), 1);
}

TEST(TBitFieldEdgeCases, SelfAssignment) {
    TBitField a(64);
    a.SetBit(5);
    a = a;//самоприсваивание
    EXPECT_EQ(a.GetBit(5), 1);
}

// Сравнение
TEST(TBitFieldFunctional, Equality) {
    TBitField a(64);
    TBitField b(64);
    EXPECT_EQ(a, b);

    a.SetBit(5);
    EXPECT_NE(a, b);

    b.SetBit(5);
    EXPECT_EQ(a, b);
}

TEST(TBitFieldEdgeCases, EqualityWithJunkBits) {
    TBitField a(33);
    TBitField b(33);

    a.SetBit(0);
    b.SetBit(0);
    EXPECT_EQ(a, b);
}

TEST(TBitFieldEdgeCases, EqualityDifferentLength) {
    TBitField a(32);
    TBitField b(64);
    EXPECT_NE(a, b);
}

// Битовые операции
TEST(TBitFieldFunctional, OrOperation) {
    TBitField a(32);
    TBitField b(32);

    a.SetBit(1);
    a.SetBit(2);
    b.SetBit(2);
    b.SetBit(3);

    TBitField c = a | b;
    EXPECT_EQ(c.GetBit(1), 1);
    EXPECT_EQ(c.GetBit(2), 1);
    EXPECT_EQ(c.GetBit(3), 1);
    EXPECT_EQ(c.GetBit(0), 0);
}

TEST(TBitFieldFunctional, AndOperation) {
    TBitField a(32);
    TBitField b(32);

    a.SetBit(1);
    a.SetBit(2);
    b.SetBit(2);
    b.SetBit(3);

    TBitField c = a & b;
    EXPECT_EQ(c.GetBit(1), 0);
    EXPECT_EQ(c.GetBit(2), 1);
    EXPECT_EQ(c.GetBit(3), 0);
}

TEST(TBitFieldFunctional, NotOperation) {
    TBitField a(8);
    a.SetBit(0);
    a.SetBit(1);

    TBitField c = ~a;
    EXPECT_EQ(c.GetBit(0), 0);
    EXPECT_EQ(c.GetBit(1), 0);
    EXPECT_EQ(c.GetBit(2), 1);
    EXPECT_EQ(c.GetBit(7), 1);
}

TEST(TBitFieldEdgeCases, NotOperationJunkBits) {
    TBitField a(10);
    TBitField c = ~a;

    for (int i = 0; i < 10; ++i) {
        EXPECT_EQ(c.GetBit(i), 1) << "Бит " << i;
    }

    for (int i = 10; i < 32; ++i) {
        EXPECT_EQ(c.GetBit(i), 0) << "Бит " << i;
    }
}

TEST(TBitFieldEdgeCases, OrWithDifferentLengths) {
    TBitField a(8);
    TBitField b(16);
    a.SetBit(3);
    b.SetBit(15);

    TBitField c = a | b;
    EXPECT_EQ(c.GetLength(), 16);
    EXPECT_EQ(c.GetBit(3), 1);
    EXPECT_EQ(c.GetBit(15), 1);
}

// Ввод/вывод
TEST(TBitFieldFunctional, Output) {
    TBitField bf(5);
    bf.SetBit(0);
    bf.SetBit(2);
    bf.SetBit(4);

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
    EXPECT_EQ(bf.GetBit(3), 0);
    EXPECT_EQ(bf.GetBit(4), 1);
}