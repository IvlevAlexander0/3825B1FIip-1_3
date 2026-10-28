#include <gtest/gtest.h>
#include "tbitfield.h"
#include "tset.h"
#include <sstream>

TEST(TBitFieldTest, Initialization) {
    TBitField bf(8);
    EXPECT_EQ(bf.GetLength(), 8);
    for (int i = 0; i < 8; i++) {
        EXPECT_EQ(bf.GetBit(i), 0);
    }
}

TEST(TBitFieldTest, EdgeZeroLength) {
    TBitField bf(0);
    EXPECT_EQ(bf.GetLength(), 0);
}

TEST(TBitFieldTest, SetAndClear) {
    TBitField bf(8);
    bf.SetBit(3);
    EXPECT_EQ(bf.GetBit(3), 1);
    bf.ClrBit(3);
    EXPECT_EQ(bf.GetBit(3), 0);
}

TEST(TBitFieldTest, OutOfBounds) {
    TBitField bf(8);
    bf.SetBit(100);
    EXPECT_EQ(bf.GetBit(100), 0);
    bf.ClrBit(-1);
    EXPECT_EQ(bf.GetBit(-1), 0);
}

TEST(TBitFieldTest, CopyConstructor) {
    TBitField a(8);
    a.SetBit(1);
    a.SetBit(5);
    TBitField b(a);
    EXPECT_EQ(b.GetLength(), 8);
    EXPECT_EQ(b.GetBit(1), 1);
    EXPECT_EQ(b.GetBit(5), 1);
}

TEST(TBitFieldTest, AssignmentOperator) {
    TBitField a(8);
    a.SetBit(2);
    TBitField b(4);
    b = a;
    EXPECT_EQ(b.GetLength(), 8);
    EXPECT_EQ(b.GetBit(2), 1);
}

TEST(TBitFieldTest, Equality) {
    TBitField a(4);
    TBitField b(4);
    EXPECT_TRUE(a == b);
    a.SetBit(1);
    EXPECT_FALSE(a == b);
    b.SetBit(1);
    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);
}

TEST(TBitFieldTest, LogicOperations) {
    TBitField a(4);
    TBitField b(4);
    a.SetBit(1); a.SetBit(3);
    b.SetBit(1); b.SetBit(2);

    TBitField orRes = a | b;
    EXPECT_EQ(orRes.GetBit(1), 1);
    EXPECT_EQ(orRes.GetBit(2), 1);
    EXPECT_EQ(orRes.GetBit(3), 1);

    TBitField andRes = a & b;
    EXPECT_EQ(andRes.GetBit(1), 1);
    EXPECT_EQ(andRes.GetBit(2), 0);

    TBitField notRes = ~a;
    EXPECT_EQ(notRes.GetBit(0), 1);
    EXPECT_EQ(notRes.GetBit(1), 0);
    EXPECT_EQ(notRes.GetBit(2), 1);
    EXPECT_EQ(notRes.GetBit(3), 0);
}

TEST(TBitFieldTest, IOOperator) {
    TBitField bf(4);
    std::stringstream ss;
    ss << "1010";
    ss >> bf;
    EXPECT_EQ(bf.GetBit(0), 1);
    EXPECT_EQ(bf.GetBit(1), 0);
    EXPECT_EQ(bf.GetBit(2), 1);
    EXPECT_EQ(bf.GetBit(3), 0);

    std::stringstream out;
    out << bf;
    EXPECT_EQ(out.str(), "1010");
}

TEST(TSetTest, InsAndDel) {
    TSet set(10);
    set.InsElem(5);
    EXPECT_TRUE(set.IsMember(5));
    EXPECT_FALSE(set.IsMember(1));
    set.DelElem(5);
    EXPECT_FALSE(set.IsMember(5));
}

TEST(TSetTest, EdgeCases) {
    TSet set(3);
    set.InsElem(-1);
    set.InsElem(4);
    EXPECT_FALSE(set.IsMember(-1));
    EXPECT_FALSE(set.IsMember(4));
}

TEST(TSetTest, UnionOperation) {
    TSet a(10);
    TSet b(10);
    a.InsElem(1); a.InsElem(2);
    b.InsElem(2); b.InsElem(3);
    TSet result = a + b;
    EXPECT_TRUE(result.IsMember(1));
    EXPECT_TRUE(result.IsMember(2));
    EXPECT_TRUE(result.IsMember(3));
    EXPECT_FALSE(result.IsMember(4));
}

TEST(TSetTest, Intersection) {
    TSet a(10);
    TSet b(10);
    a.InsElem(1); a.InsElem(2); a.InsElem(3);
    b.InsElem(2); b.InsElem(3); b.InsElem(4);
    TSet result = a * b;
    EXPECT_FALSE(result.IsMember(1));
    EXPECT_TRUE(result.IsMember(2));
    EXPECT_TRUE(result.IsMember(3));
    EXPECT_FALSE(result.IsMember(4));
}

TEST(TSetTest, Complement) {
    TSet set(3);
    set.InsElem(1);
    TSet comp = ~set;
    EXPECT_TRUE(comp.IsMember(0));
    EXPECT_FALSE(comp.IsMember(1));
    EXPECT_TRUE(comp.IsMember(2));
    EXPECT_TRUE(comp.IsMember(3));
}

TEST(TSetTest, AddSubElementOperator) {
    TSet a(5);
    a.InsElem(1);
    TSet b = a + 3;
    EXPECT_TRUE(b.IsMember(1));
    EXPECT_TRUE(b.IsMember(3));
    TSet c = b - 1;
    EXPECT_FALSE(c.IsMember(1));
    EXPECT_TRUE(c.IsMember(3));
}

TEST(TSetTest, EqualityAndAssignment) {
    TSet a(5);
    TSet b(5);
    a.InsElem(2);
    b.InsElem(2);
    EXPECT_TRUE(a == b);
    a.InsElem(3);
    EXPECT_FALSE(a == b);
    b = a;
    EXPECT_TRUE(a == b);
    EXPECT_TRUE(b.IsMember(3));
}

TEST(TSetTest, ConversionOperators) {
    TSet set(5);
    set.InsElem(2);
    TBitField bf = set;
    EXPECT_EQ(bf.GetBit(2), 1);

    TBitField bf2(6);
    bf2.SetBit(4);
    TSet set2(bf2);
    EXPECT_EQ(set2.GetMaxPower(), 5);
    EXPECT_TRUE(set2.IsMember(4));
}

TEST(TSetTest, IOOperator) {
    TSet set(10);
    std::stringstream ss("1 3 5 -1");
    ss >> set;
    EXPECT_TRUE(set.IsMember(1));
    EXPECT_TRUE(set.IsMember(3));
    EXPECT_TRUE(set.IsMember(5));
    EXPECT_FALSE(set.IsMember(2));

    std::stringstream out;
    out << set;
    EXPECT_EQ(out.str(), "{ 1, 3, 5 }");
}