#include <gtest/gtest.h>
#include "../../includes/Lab1/tbitfield.h"
#include "../../includes/Lab1/tset.h"

TEST(TBitFieldTest, Initialization) {
    TBitField bf(8);
    EXPECT_EQ(bf.GetLength(), 8);
    for (int i = 0; i < 8; i++) {
        EXPECT_EQ(bf.GetBit(i), 0);
    }
}

TEST(TBitFieldTest, SetAndClear) {
    TBitField bf(8);
    bf.SetBit(3);
    EXPECT_EQ(bf.GetBit(3), 1);
    bf.ClrBit(3);
    EXPECT_EQ(bf.GetBit(3), 0);
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
}

TEST(TSetTest, InsAndDel) {
    TSet set(10);
    set.InsElem(5);
    EXPECT_TRUE(set.IsMember(5));
    EXPECT_FALSE(set.IsMember(1));

    set.DelElem(5);
    EXPECT_FALSE(set.IsMember(5));
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

TEST(TSetTest, AddElementViaOperator) {
    TSet a(5);
    a.InsElem(1);

    TSet b = a + 3;

    EXPECT_TRUE(b.IsMember(1));
    EXPECT_TRUE(b.IsMember(3));
}