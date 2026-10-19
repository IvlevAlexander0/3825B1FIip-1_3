#include "pch.h"
#include "tset.h"
#include "tbitfield.h"

TEST(TBitField, CreateAndLength)
{
    TBitField bf(10);
    EXPECT_EQ(bf.GetLength(), 10);
    for (int i = 0; i < 10; ++i)
        EXPECT_EQ(bf.GetBit(i), 0);
}

TEST(TBitField, SetAndGetBit)
{
    TBitField bf(20);
    bf.SetBit(0); bf.SetBit(5); bf.SetBit(19);
    EXPECT_EQ(bf.GetBit(0), 1);
    EXPECT_EQ(bf.GetBit(5), 1);
    EXPECT_EQ(bf.GetBit(19), 1);
    EXPECT_EQ(bf.GetBit(1), 0);
    EXPECT_EQ(bf.GetBit(18), 0);
}

TEST(TBitField, ClrBit)
{
    TBitField bf(8);
    bf.SetBit(3);
    EXPECT_EQ(bf.GetBit(3), 1);
    bf.ClrBit(3);
    EXPECT_EQ(bf.GetBit(3), 0);
}

TEST(TBitField, OutOfRange)
{
    TBitField bf(8);
    EXPECT_THROW(bf.SetBit(-1), std::out_of_range);
    EXPECT_THROW(bf.SetBit(8), std::out_of_range);
    EXPECT_THROW(bf.ClrBit(8), std::out_of_range);
    EXPECT_THROW(bf.GetBit(-5), std::out_of_range);
}

TEST(TBitField, CopyCtorAndAssign)
{
    TBitField a(16);
    a.SetBit(0); a.SetBit(7); a.SetBit(15);

    TBitField b(a);
    EXPECT_TRUE(a == b);

    TBitField c(4);
    c = a;
    EXPECT_TRUE(a == c);
    EXPECT_EQ(c.GetLength(), 16);
}

TEST(TBitField, Equality)
{
    TBitField a(10), b(10);
    EXPECT_TRUE(a == b);
    a.SetBit(3);
    EXPECT_TRUE(a != b);
    b.SetBit(3);
    EXPECT_TRUE(a == b);

    TBitField c(11);
    EXPECT_TRUE(a != c);
}

TEST(TBitField, Or)
{
    TBitField a(10), b(10);
    a.SetBit(1); a.SetBit(3);
    b.SetBit(3); b.SetBit(5);
    TBitField c = a | b;
    EXPECT_EQ(c.GetBit(1), 1);
    EXPECT_EQ(c.GetBit(3), 1);
    EXPECT_EQ(c.GetBit(5), 1);
    EXPECT_EQ(c.GetBit(2), 0);
}

TEST(TBitField, And)
{
    TBitField a(10), b(10);
    a.SetBit(1); a.SetBit(3);
    b.SetBit(3); b.SetBit(5);
    TBitField c = a & b;
    EXPECT_EQ(c.GetBit(1), 0);
    EXPECT_EQ(c.GetBit(3), 1);
    EXPECT_EQ(c.GetBit(5), 0);
}

TEST(TBitField, NotWithTail)
{
    TBitField a(5);
    a.SetBit(0); a.SetBit(2);
    TBitField b = ~a;
    for (int i = 0; i < 5; ++i)
        EXPECT_EQ(b.GetBit(i), a.GetBit(i) ? 0 : 1);
    EXPECT_EQ(b.GetLength(), 5);
}

TEST(TBitField, OrDifferentLengths)
{
    TBitField a(5), b(10);
    a.SetBit(0); b.SetBit(9);
    EXPECT_THROW(a | b, std::invalid_argument);
    EXPECT_THROW(a & b, std::invalid_argument);
}

TEST(TBitField, StreamOut)
{
    TBitField a(8);
    a.SetBit(7); a.SetBit(5); a.SetBit(4); a.SetBit(1);

    std::ostringstream out;
    out << a;
    EXPECT_EQ(out.str(), "10110010");
}

TEST(TBitField, StreamIn)
{
    TBitField a(8);
    std::istringstream in("10110010");
    in >> a;
    EXPECT_FALSE(in.fail());

    EXPECT_EQ(a.GetBit(7), 1);
    EXPECT_EQ(a.GetBit(6), 0);
    EXPECT_EQ(a.GetBit(5), 1);
    EXPECT_EQ(a.GetBit(4), 1);
    EXPECT_EQ(a.GetBit(0), 0);

    std::istringstream bad("101");
    TBitField b(8);
    bad >> b;
    EXPECT_TRUE(bad.fail());
}




TEST(TSet, Empty)
{
    TSet s(10);
    EXPECT_EQ(s.GetMaxPower(), 10);
    for (int i = 0; i < 10; ++i)
        EXPECT_EQ(s.IsMember(i), 0);
}

TEST(TSet, InsDelMember)
{
    TSet s(10);
    s.InsElem(3);
    s.InsElem(7);
    EXPECT_EQ(s.IsMember(3), 1);
    EXPECT_EQ(s.IsMember(7), 1);
    EXPECT_EQ(s.IsMember(4), 0);

    s.DelElem(3);
    EXPECT_EQ(s.IsMember(3), 0);
}

TEST(TSet, OutOfUniverse)
{
    TSet s(5);
    EXPECT_THROW(s.InsElem(-1), std::out_of_range);
    EXPECT_THROW(s.InsElem(5), std::out_of_range);
    EXPECT_THROW(s.DelElem(5), std::out_of_range);
    EXPECT_THROW(s.IsMember(100), std::out_of_range);
}

TEST(TSet, CtorFromBitField)
{
    TBitField bf(6);
    bf.SetBit(1); bf.SetBit(4);
    TSet s(bf);
    EXPECT_EQ(s.GetMaxPower(), 6);
    EXPECT_EQ(s.IsMember(1), 1);
    EXPECT_EQ(s.IsMember(4), 1);
    EXPECT_EQ(s.IsMember(0), 0);
}

TEST(TSet, CastToBitField)
{
    TSet s(6);
    s.InsElem(2); s.InsElem(5);
    TBitField bf = s;
    EXPECT_EQ(bf.GetLength(), 6);
    EXPECT_EQ(bf.GetBit(2), 1);
    EXPECT_EQ(bf.GetBit(5), 1);
}

TEST(TSet, Equality)
{
    TSet a(5), b(5);
    a.InsElem(1); a.InsElem(3);
    b.InsElem(1); b.InsElem(3);
    EXPECT_TRUE(a == b);
    b.DelElem(3);
    EXPECT_TRUE(a != b);
}

TEST(TSet, Assign)
{
    TSet a(5), b(8);
    a.InsElem(1); a.InsElem(4);
    b = a;
    EXPECT_EQ(b.GetMaxPower(), 5);
    EXPECT_TRUE(a == b);
}

TEST(TSet, UnionWithElement)
{
    TSet a(5);
    a.InsElem(0);
    TSet b = a + 4;
    EXPECT_EQ(b.IsMember(0), 1);
    EXPECT_EQ(b.IsMember(4), 1);
    EXPECT_EQ(a.IsMember(4), 0);
}

TEST(TSet, DiffWithElement)
{
    TSet a(5);
    a.InsElem(0); a.InsElem(4);
    TSet b = a - 4;
    EXPECT_EQ(b.IsMember(0), 1);
    EXPECT_EQ(b.IsMember(4), 0);
    EXPECT_EQ(a.IsMember(4), 1);
}

TEST(TSet, Union)
{
    TSet a(5), b(5);
    a.InsElem(0); a.InsElem(1);
    b.InsElem(1); b.InsElem(2);
    TSet c = a + b;
    EXPECT_EQ(c.IsMember(0), 1);
    EXPECT_EQ(c.IsMember(1), 1);
    EXPECT_EQ(c.IsMember(2), 1);
    EXPECT_EQ(c.IsMember(3), 0);
}

TEST(TSet, Intersection)
{
    TSet a(5), b(5);
    a.InsElem(0); a.InsElem(1);
    b.InsElem(1); b.InsElem(2);
    TSet c = a * b;
    EXPECT_EQ(c.IsMember(0), 0);
    EXPECT_EQ(c.IsMember(1), 1);
    EXPECT_EQ(c.IsMember(2), 0);
}

TEST(TSet, Complement)
{
    TSet a(5);
    a.InsElem(1); a.InsElem(3);
    TSet c = ~a;
    EXPECT_EQ(c.IsMember(0), 1);
    EXPECT_EQ(c.IsMember(1), 0);
    EXPECT_EQ(c.IsMember(2), 1);
    EXPECT_EQ(c.IsMember(3), 0);
    EXPECT_EQ(c.IsMember(4), 1);
}

TEST(TSet, UnionDifferentUniverses)
{
    TSet a(3), b(6);
    a.InsElem(0);
    b.InsElem(5);
    EXPECT_THROW(a + b, std::invalid_argument);
    EXPECT_THROW(a * b, std::invalid_argument);
}

TEST(TSet, StreamOut)
{
    TSet s(6);
    s.InsElem(1); s.InsElem(4);
    std::ostringstream out;
    out << s;
    EXPECT_EQ(out.str(), "{ 1, 4 }");
}

TEST(TSet, StreamIn)
{
    TSet s(8);
    std::istringstream in("2 3 5");
    in >> s;
    EXPECT_FALSE(in.fail());
    EXPECT_EQ(s.IsMember(3), 1);
    EXPECT_EQ(s.IsMember(5), 1);
    EXPECT_EQ(s.IsMember(0), 0);

    std::istringstream bad("1 100");
    TSet t(8);
    bad >> t;
    EXPECT_TRUE(bad.fail());
}
