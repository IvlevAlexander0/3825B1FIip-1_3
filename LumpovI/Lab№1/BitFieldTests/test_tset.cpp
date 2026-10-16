#include <gtest/gtest.h>
#include "tset.h"
#include <sstream>

// Базовые тесты
TEST(TSetSmoke, ConstructorAndMaxPower) {
    TSet s(100);
    EXPECT_EQ(s.GetMaxPower(), 100);
}

TEST(TSetSmoke, InitiallyEmpty) {
    TSet s(100);
    for (int i = 0; i < 100; ++i) {
        EXPECT_EQ(s.IsMember(i), 0);
    }
}

// Добавление удаление жлементов
TEST(TSetFunctional, InsertAndCheck) {
    TSet s(20);
    s.InsElem(5);
    s.InsElem(10);
    s.InsElem(15);

    EXPECT_EQ(s.IsMember(5), 1);
    EXPECT_EQ(s.IsMember(10), 1);
    EXPECT_EQ(s.IsMember(15), 1);
    EXPECT_EQ(s.IsMember(0), 0);
    EXPECT_EQ(s.IsMember(19), 0);
}

TEST(TSetFunctional, DeleteElement) {
    TSet s(20);
    s.InsElem(5);
    EXPECT_EQ(s.IsMember(5), 1);

    s.DelElem(5);
    EXPECT_EQ(s.IsMember(5), 0);
}

// Краевые случаи
TEST(TSetEdgeCases, OutOfRangeOperations) {
    TSet s(10);
    s.InsElem(-1);
    s.InsElem(10);
    s.InsElem(100);

    EXPECT_EQ(s.IsMember(-1), 0);
    EXPECT_EQ(s.IsMember(10), 0);
    EXPECT_EQ(s.IsMember(100), 0);
}

TEST(TSetEdgeCases, BoundaryElements) {
    TSet s(10);
    s.InsElem(0);
    s.InsElem(9);

    EXPECT_EQ(s.IsMember(0), 1);
    EXPECT_EQ(s.IsMember(9), 1);
}

// Операции с элементом
TEST(TSetFunctional, UnionWithElement) {
    TSet s(10);
    s.InsElem(2);

    TSet s2 = s + 5;
    EXPECT_EQ(s2.IsMember(2), 1);
    EXPECT_EQ(s2.IsMember(5), 1);
    EXPECT_EQ(s2.IsMember(3), 0);

    EXPECT_EQ(s.IsMember(5), 0);
}

TEST(TSetFunctional, DifferenceWithElement) {
    TSet s(10);
    s.InsElem(2);
    s.InsElem(5);

    TSet s2 = s - 5;
    EXPECT_EQ(s2.IsMember(2), 1);
    EXPECT_EQ(s2.IsMember(5), 0);

    EXPECT_EQ(s.IsMember(5), 1);
}

// Теоретико-множетсвенные операции
TEST(TSetFunctional, UnionOfSets) {
    TSet a(10);
    TSet b(10);
    a.InsElem(1); a.InsElem(2);
    b.InsElem(2); b.InsElem(3);

    TSet c = a + b;
    EXPECT_EQ(c.IsMember(1), 1);
    EXPECT_EQ(c.IsMember(2), 1);
    EXPECT_EQ(c.IsMember(3), 1);
    EXPECT_EQ(c.IsMember(0), 0);
}

TEST(TSetFunctional, IntersectionOfSets) {
    TSet a(10);
    TSet b(10);
    a.InsElem(1); a.InsElem(2);
    b.InsElem(2); b.InsElem(3);

    TSet c = a * b;
    EXPECT_EQ(c.IsMember(1), 0);
    EXPECT_EQ(c.IsMember(2), 1);
    EXPECT_EQ(c.IsMember(3), 0);
}

TEST(TSetFunctional, Complement) {
    TSet a(5);
    a.InsElem(1);
    a.InsElem(3);

    TSet c = ~a;
    EXPECT_EQ(c.IsMember(0), 1);
    EXPECT_EQ(c.IsMember(1), 0);
    EXPECT_EQ(c.IsMember(2), 1);
    EXPECT_EQ(c.IsMember(3), 0);
    EXPECT_EQ(c.IsMember(4), 1);
}

// Сравнение
TEST(TSetFunctional, Equality) {
    TSet a(10);
    TSet b(10);
    EXPECT_EQ(a, b);

    a.InsElem(5);
    EXPECT_NE(a, b);

    b.InsElem(5);
    EXPECT_EQ(a, b);
}

// Копирование
TEST(TSetFunctional, CopyConstructor) {
    TSet a(10);
    a.InsElem(3);
    a.InsElem(7);

    TSet b(a);
    EXPECT_EQ(b.IsMember(3), 1);
    EXPECT_EQ(b.IsMember(7), 1);
    EXPECT_EQ(a, b);
}

TEST(TSetFunctional, Assignment) {
    TSet a(10);
    a.InsElem(3);

    TSet b(10);
    b = a;
    EXPECT_EQ(b.IsMember(3), 1);
}

// Преобразвоние типов
TEST(TSetFunctional, ConvertFromBitField) {
    TBitField bf(10);
    bf.SetBit(2);
    bf.SetBit(5);

    TSet s(bf);
    EXPECT_EQ(s.GetMaxPower(), 10);
    EXPECT_EQ(s.IsMember(2), 1);
    EXPECT_EQ(s.IsMember(5), 1);
}

TEST(TSetFunctional, ConvertToBitField) {
    TSet s(10);
    s.InsElem(2);
    s.InsElem(5);

    TBitField bf = s;
    EXPECT_EQ(bf.GetBit(2), 1);
    EXPECT_EQ(bf.GetBit(5), 1);
}

// Ввод/вывод
TEST(TSetFunctional, Output) {
    TSet s(10);
    s.InsElem(1);
    s.InsElem(3);
    s.InsElem(5);

    std::ostringstream oss;
    oss << s;
    EXPECT_EQ(oss.str(), "{ 1, 3, 5 }");
}

TEST(TSetFunctional, OutputEmpty) {
    TSet s(10);
    std::ostringstream oss;
    oss << s;
    EXPECT_EQ(oss.str(), "{  }");
}

TEST(TSetFunctional, Input) {
    TSet s(10);
    std::istringstream iss("{ 1, 3, 5 }");
    iss >> s;

    EXPECT_EQ(s.IsMember(1), 1);
    EXPECT_EQ(s.IsMember(3), 1);
    EXPECT_EQ(s.IsMember(5), 1);
    EXPECT_EQ(s.IsMember(0), 0);
    EXPECT_EQ(s.IsMember(2), 0);
}