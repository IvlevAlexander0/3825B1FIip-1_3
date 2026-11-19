#include "pch.h"
TEST(BitField, Create)
{
    ASSERT_NO_THROW(TBitField bf(10));
}
TEST(BitField, CreateNegative)
{
    ASSERT_ANY_THROW(TBitField bf(-1));
}
TEST(BitField, SetAndGet)
{
    TBitField bf(10);
    bf.SetBit(3);
    EXPECT_EQ(1, bf.GetBit(3));
    EXPECT_EQ(0, bf.GetBit(2));
}
TEST(BitField, Clear)
{
    TBitField bf(10);
    bf.SetBit(3);
    bf.ClrBit(3);
    EXPECT_EQ(0, bf.GetBit(3));
}
TEST(Set, Create)
{
    ASSERT_NO_THROW(TSet s(10));
}
TEST(Set, InsertAndCheck)
{
    TSet s(10);
    s.InsElem(5);
    EXPECT_TRUE(s.IsMember(5));
    EXPECT_FALSE(s.IsMember(4));
}
TEST(Set, Delete)
{
    TSet s(10);
    s.InsElem(5);
    s.DelElem(5);
    EXPECT_FALSE(s.IsMember(5));
}