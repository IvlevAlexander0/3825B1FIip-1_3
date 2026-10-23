//#include "pch.h"
#include <gtest/gtest.h>
#include <stdexcept>
#include "../../../includes/Lab1/tbitfield.h"
#include "../../../includes/Lab1/tset.h"

TEST(TBitField, constructor) {
    TBitField bf1(10);
    EXPECT_EQ(bf1.GetLength(), 10);
    TBitField bf2(0);
    EXPECT_EQ(bf2.GetLength(), 0);
    TBitField bf3(64);
    EXPECT_EQ(bf3.GetLength(), 64);
    EXPECT_THROW(TBitField(-1), std::invalid_argument);
}

TEST(TBitField, setgetclr) {
    TBitField bf(20);
    for (int i = 0; i < 20; i++) {
        EXPECT_EQ(bf.GetBit(i), 0);
    }
    bf.SetBit(5);
    EXPECT_EQ(bf.GetBit(5), 1);
    EXPECT_EQ(bf.GetBit(4), 0);
    EXPECT_EQ(bf.GetBit(6), 0);
    bf.ClrBit(5);
    EXPECT_EQ(bf.GetBit(5), 0);
    bf.SetBit(3);
    bf.SetBit(3);
    EXPECT_EQ(bf.GetBit(3), 1);
    bf.ClrBit(3);
    bf.ClrBit(3);
    EXPECT_EQ(bf.GetBit(3), 0);
}

TEST(TBitField, boundary) {
    TBitField bf(64);
    bf.SetBit(0);
    bf.SetBit(31);
    bf.SetBit(32);
    bf.SetBit(63);
    EXPECT_EQ(bf.GetBit(0), 1);
    EXPECT_EQ(bf.GetBit(31), 1);
    EXPECT_EQ(bf.GetBit(32), 1);
    EXPECT_EQ(bf.GetBit(63), 1);
    EXPECT_EQ(bf.GetBit(1), 0);
    EXPECT_EQ(bf.GetBit(30), 0);
    EXPECT_EQ(bf.GetBit(33), 0);
    EXPECT_EQ(bf.GetBit(62), 0);
}

TEST(TBitField, badindex) {
    TBitField bf(10);
    EXPECT_THROW(bf.SetBit(-1), std::out_of_range);
    EXPECT_THROW(bf.SetBit(10), std::out_of_range);
    EXPECT_THROW(bf.SetBit(100), std::out_of_range);
    EXPECT_THROW(bf.ClrBit(-1), std::out_of_range);
    EXPECT_THROW(bf.ClrBit(10), std::out_of_range);
    EXPECT_THROW(bf.GetBit(-1), std::out_of_range);
    EXPECT_THROW(bf.GetBit(10), std::out_of_range);
}

TEST(TBitField, copy) {
    TBitField bf1(20);
    bf1.SetBit(1);
    bf1.SetBit(15);
    TBitField bf2(bf1);
    EXPECT_EQ(bf2.GetLength(), 20);
    EXPECT_TRUE(bf1 == bf2);
    bf1.SetBit(3);
    EXPECT_EQ(bf2.GetBit(3), 0);
    EXPECT_EQ(bf1.GetBit(3), 1);
}

TEST(TBitField, assign) {
    TBitField bf1(20);
    bf1.SetBit(2);
    bf1.SetBit(19);
    TBitField bf2(5);
    bf2 = bf1;
    EXPECT_EQ(bf2.GetLength(), 20);
    EXPECT_TRUE(bf1 == bf2);
    bf1 = bf1;
    EXPECT_EQ(bf1.GetBit(2), 1);
    EXPECT_EQ(bf1.GetBit(19), 1);
    bf1.SetBit(0);
    EXPECT_EQ(bf2.GetBit(0), 0);
}

TEST(TBitField, equal) {
    TBitField a(10), b(10);
    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);
    a.SetBit(3);
    EXPECT_FALSE(a == b);
    EXPECT_TRUE(a != b);
    b.SetBit(3);
    EXPECT_TRUE(a == b);
    TBitField c(20);
    EXPECT_FALSE(a == c);
    EXPECT_TRUE(a != c);
}

TEST(TBitField, orop) {
    TBitField a(10), b(10);
    a.SetBit(1); a.SetBit(3);
    b.SetBit(3); b.SetBit(5);
    TBitField c = a | b;
    EXPECT_EQ(c.GetBit(1), 1);
    EXPECT_EQ(c.GetBit(3), 1);
    EXPECT_EQ(c.GetBit(5), 1);
    EXPECT_EQ(c.GetBit(2), 0);
    EXPECT_EQ(c.GetBit(4), 0);
    EXPECT_EQ(a.GetBit(5), 0);
    EXPECT_EQ(b.GetBit(1), 0);
}

TEST(TBitField, andop) {
    TBitField a(10), b(10);
    a.SetBit(1); a.SetBit(3); a.SetBit(5);
    b.SetBit(3); b.SetBit(5); b.SetBit(7);
    TBitField c = a & b;
    EXPECT_EQ(c.GetBit(3), 1);
    EXPECT_EQ(c.GetBit(5), 1);
    EXPECT_EQ(c.GetBit(1), 0);
    EXPECT_EQ(c.GetBit(7), 0);
}

TEST(TBitField, notop) {
    TBitField a(10);
    a.SetBit(0);
    a.SetBit(9);
    TBitField b = ~a;
    EXPECT_EQ(b.GetBit(0), 0);
    EXPECT_EQ(b.GetBit(9), 0);
    EXPECT_EQ(b.GetBit(1), 1);
    EXPECT_EQ(b.GetBit(5), 1);
    EXPECT_EQ(b.GetLength(), 10);
    TBitField c = ~b;
    EXPECT_TRUE(a == c);
}

TEST(TSet, constructor) {
    TSet s(10);
    EXPECT_EQ(s.GetMaxPower(), 10);
    for (int i = 0; i < 10; i++) {
        EXPECT_EQ(s.IsMember(i), 0);
    }
    EXPECT_THROW(TSet(-1), std::invalid_argument);
}

TEST(TSet, inselem) {
    TSet s(20);
    s.InsElem(5);
    EXPECT_TRUE(s.IsMember(5));
    EXPECT_FALSE(s.IsMember(4));
    EXPECT_FALSE(s.IsMember(6));
    s.DelElem(5);
    EXPECT_FALSE(s.IsMember(5));
    s.InsElem(3);
    s.InsElem(3);
    EXPECT_TRUE(s.IsMember(3));
    s.DelElem(3);
    s.DelElem(3);
    EXPECT_FALSE(s.IsMember(3));
}

TEST(TSet, badindex) {
    TSet s(10);
    EXPECT_THROW(s.InsElem(-1), std::out_of_range);
    EXPECT_THROW(s.InsElem(10), std::out_of_range);
    EXPECT_THROW(s.InsElem(100), std::out_of_range);
    EXPECT_THROW(s.DelElem(-1), std::out_of_range);
    EXPECT_THROW(s.DelElem(10), std::out_of_range);
    EXPECT_THROW(s.IsMember(-1), std::out_of_range);
    EXPECT_THROW(s.IsMember(10), std::out_of_range);
}

TEST(TSet, copy) {
    TSet a(10);
    a.InsElem(1); a.InsElem(5);
    TSet b(a);
    EXPECT_EQ(b.GetMaxPower(), 10);
    EXPECT_TRUE(b.IsMember(1));
    EXPECT_TRUE(b.IsMember(5));
    a.InsElem(2);
    EXPECT_FALSE(b.IsMember(2));
}

TEST(TSet, assign) {
    TSet a(10);
    a.InsElem(1); a.InsElem(5);
    TSet b(5);
    b = a;
    EXPECT_EQ(b.GetMaxPower(), 10);
    EXPECT_TRUE(b.IsMember(1));
    EXPECT_TRUE(b.IsMember(5));
    a = a;
    EXPECT_TRUE(a.IsMember(1));
    EXPECT_TRUE(a.IsMember(5));
}

TEST(TSet, equal) {
    TSet a(10), b(10);
    EXPECT_TRUE(a == b);
    a.InsElem(3);
    EXPECT_FALSE(a == b);
    EXPECT_TRUE(a != b);
    b.InsElem(3);
    EXPECT_TRUE(a == b);
    TSet c(20);
    EXPECT_FALSE(a == c);
}

TEST(TSet, pluselem) {
    TSet s(10);
    s.InsElem(1);
    TSet r = s + 5;
    EXPECT_TRUE(r.IsMember(1));
    EXPECT_TRUE(r.IsMember(5));
    EXPECT_FALSE(r.IsMember(3));
    EXPECT_FALSE(s.IsMember(5));
    EXPECT_THROW(s + 100, std::out_of_range);
}

TEST(TSet, minuselem) {
    TSet s(10);
    s.InsElem(1); s.InsElem(5);
    TSet r = s - 1;
    EXPECT_FALSE(r.IsMember(1));
    EXPECT_TRUE(r.IsMember(5));
    EXPECT_TRUE(s.IsMember(1));
    EXPECT_THROW(s - 100, std::out_of_range);
}

TEST(TSet, pluset) {
    TSet a(10), b(10);
    a.InsElem(1); a.InsElem(3);
    b.InsElem(3); b.InsElem(5);
    TSet c = a + b;
    EXPECT_TRUE(c.IsMember(1));
    EXPECT_TRUE(c.IsMember(3));
    EXPECT_TRUE(c.IsMember(5));
    EXPECT_FALSE(c.IsMember(2));
    EXPECT_FALSE(c.IsMember(4));
    EXPECT_FALSE(a.IsMember(5));
    EXPECT_FALSE(b.IsMember(1));
}

TEST(TSet, multset) {
    TSet a(10), b(10);
    a.InsElem(1); a.InsElem(3); a.InsElem(5);
    b.InsElem(3); b.InsElem(5); b.InsElem(7);
    TSet c = a * b;
    EXPECT_TRUE(c.IsMember(3));
    EXPECT_TRUE(c.IsMember(5));
    EXPECT_FALSE(c.IsMember(1));
    EXPECT_FALSE(c.IsMember(7));
}

TEST(TSet, complement) {
    TSet s(10);
    s.InsElem(0);
    s.InsElem(9);
    TSet c = ~s;
    EXPECT_FALSE(c.IsMember(0));
    EXPECT_FALSE(c.IsMember(9));
    EXPECT_TRUE(c.IsMember(1));
    EXPECT_TRUE(c.IsMember(5));
    TSet cc = ~c;
    EXPECT_TRUE(s == cc);
}

TEST(TSet, tobitfield) {
    TSet s(10);
    s.InsElem(3);
    TBitField bf = s;
    EXPECT_EQ(bf.GetLength(), 10);
    EXPECT_EQ(bf.GetBit(3), 1);
    EXPECT_EQ(bf.GetBit(4), 0);
}

TEST(TSet, frombitfield) {
    TBitField bf(10);
    bf.SetBit(2);
    bf.SetBit(7);
    TSet s(bf);
    EXPECT_EQ(s.GetMaxPower(), 10);
    EXPECT_TRUE(s.IsMember(2));
    EXPECT_TRUE(s.IsMember(7));
    EXPECT_FALSE(s.IsMember(3));
}