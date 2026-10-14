#include "pch.h"
#include "C:\includes\Lab1\tset.h"
#include "C:\includes\Lab1\tbitfield.h"

TEST(TBitFieldTest, SetClrGet) {
	TBitField bf(100);
	bf.SetBit(56);
	EXPECT_EQ(bf.GetBit(56), 1);
	EXPECT_EQ(bf.GetBit(55), 0);
	EXPECT_EQ(bf.GetBit(57), 0);
	bf.ClrBit(56);
	EXPECT_EQ(bf.GetBit(56), 0);
	
	bf.SetBit(12);
	bf.SetBit(12);
	EXPECT_EQ(bf.GetBit(12), 1);
}
TEST(TBitFieldTest, SetClrBorder) {
	TBitField bf(12);
	bf.SetBit(0);
	EXPECT_EQ(bf.GetBit(0), 1);
	bf.ClrBit(0);
	EXPECT_EQ(bf.GetBit(0), 0);
	bf.SetBit(11);
	EXPECT_EQ(bf.GetBit(11), 1);
	bf.ClrBit(11);
	EXPECT_EQ(bf.GetBit(11), 0);
}
TEST(TBitFieldTest, LenTest) {
	int arr[6] = { 0,1,12,23,32,65 };
	for (size_t i = 0; i < 6; ++i) {
		int len = arr[i];
		TBitField bf(len);
		EXPECT_EQ(bf.GetLength(), len);
		for (int i = 0; i < len; ++i) {
			EXPECT_EQ(bf.GetBit(i), 0);
		}
	}
}
TEST(TBitFieldTest, CopyTest) {
	TBitField a(12);
	a.SetBit(8);
	TBitField b(a);
	EXPECT_EQ(b.GetBit(8), a.GetBit(8));
}
TEST(TBitFieldTest, AssignmentTest) {
	TBitField b(120), a(12);
	a.SetBit(6);
	b = a;
	EXPECT_EQ(b.GetLength(), a.GetLength());
	EXPECT_EQ(b.GetBit(6), 1);
 }
TEST(TBitFieldTest, ComparisonTest) {
	TBitField a(29), b(17), c(29);
	EXPECT_TRUE(a == c);
	a.SetBit(2);
	EXPECT_TRUE(a != c);
	EXPECT_FALSE(c == b);
}
TEST(TBitFieldTest, BitOperationsTest) {
	TBitField a(45), b(45);
	for (size_t i = 0; i < 13; ++i) {
		a.SetBit(i);
	}
	for (size_t i = 5; i < 20; ++i) {
		b.SetBit(i);
	}
	TBitField Or = a | b;
	EXPECT_EQ(Or.GetBit(1), 1);
	EXPECT_EQ(Or.GetBit(8), 1);
	EXPECT_EQ(Or.GetBit(19), 1);
	TBitField And = a & b;
	EXPECT_EQ(And.GetBit(4), 0);
	EXPECT_EQ(And.GetBit(12), 1);
	EXPECT_EQ(And.GetBit(19), 0);
	TBitField Not = ~b;
	EXPECT_EQ(Not.GetBit(2), 1);
	EXPECT_EQ(Not.GetBit(19), 0);
}
TEST(TBitFieldTest, BitOperationsInvariantsTest) {
	TBitField a(45), b(45);
	for (size_t i = 2; i < 23; ++i) {
		a.SetBit(i);
	}
	for (size_t i = 17; i < 42; ++i) {
		b.SetBit(i);
	}
	EXPECT_TRUE((a | b) == (b | a));
	EXPECT_TRUE((a & b) == (b & a));
	EXPECT_TRUE(a == ~~a);
}
TEST(TSetTest, InsDelIsTest) {
	TSet s(123);
	for (size_t i = 32; i < 100; ++i) {
		s.InsElem(i);
	}
	for (size_t i = 32; i < 100; ++i) {
		EXPECT_EQ(s.IsMember(i), 1);
	}
	for (size_t i = 52; i < 67; ++i) {
		s.DelElem(i);
	}
	for (size_t i = 52; i < 67; ++i) {
		EXPECT_EQ(s.IsMember(i), 0);
	}
	EXPECT_EQ(s.IsMember(51), 1);
	EXPECT_EQ(s.IsMember(67), 1);
}
TEST(TSetTest, PowerAndIsMemberTest) {
	TSet s1(12);
	EXPECT_EQ(s1.GetMaxPower(), 12);
	s1.InsElem(10);
	EXPECT_EQ(s1.IsMember(10), 1);
	TBitField bf(24);
	bf.SetBit(6);
	TSet s2(bf);
	EXPECT_EQ(s2.GetMaxPower(), 24);
	EXPECT_EQ(s2.IsMember(6), 1);
}
TEST(TSetTest, CopyTest) {
	TSet a(10);
	for (size_t i = 3; i < 8; ++i) {
		a.InsElem(i);
	}
	TSet b(a);
	EXPECT_EQ(b.GetMaxPower(), a.GetMaxPower());
	for (size_t i = 3; i < 8; ++i) {
		EXPECT_EQ(b.IsMember(i), 1);
	}
	b.DelElem(5);
	EXPECT_FALSE(a.IsMember(5) == b.IsMember(5));
	TBitField bf(20);
	for (size_t i = 7; i < 18; ++i) {
		bf.SetBit(i);
	}
	TSet c(bf);
	TSet d(c);
	EXPECT_EQ(c.GetMaxPower(), d.GetMaxPower());
	for (size_t i = 7; i < 18; ++i) {
		EXPECT_EQ(d.IsMember(i), 1);
	}
	d.InsElem(5);
	EXPECT_FALSE(c.IsMember(5) == d.IsMember(5));
}
TEST(TSetTest, ComparisonTest) {
	TSet a(32), b(32), c(40);
	for (size_t i = 12; i < 32; ++i) {
		a.InsElem(i);
		b.InsElem(i);
		c.InsElem(i);
	}
	EXPECT_TRUE(a == b);
	EXPECT_FALSE(b == c);
	for (size_t i = 12; i <= 20; ++i) {
		b.DelElem(i);
	}
	EXPECT_TRUE(a != b);
}
TEST(TSetTest, AssignmentTest) {
	TSet a(20);
	a.InsElem(12);
	a.InsElem(19);
	TSet b(40);
	b = a;
	EXPECT_EQ(b.GetMaxPower(), 20);
	EXPECT_EQ(b.IsMember(12), 1);
	EXPECT_EQ(b.IsMember(19), 1);
	b.DelElem(19);
	EXPECT_FALSE(b == a);
}
TEST(TSetTest, AddRemTest) {
	TSet a(54);
	for (size_t i = 20; i < 35; ++i) {
		a.InsElem(i);
	}
	TSet b = a + 53;
	EXPECT_EQ(a.IsMember(53), 0);
	EXPECT_EQ(b.IsMember(53), 1);
	EXPECT_TRUE(a.GetMaxPower() == b.GetMaxPower());
	b = b - 23;
	EXPECT_EQ(b.IsMember(23), 0);
	TSet c = a + 23;
	EXPECT_EQ(c.IsMember(23), 1);
	EXPECT_TRUE(a == c);
	c = b - 23;
	EXPECT_EQ(c.IsMember(23), 0);
	EXPECT_TRUE(b == c);
	TSet d = a + 100;
	EXPECT_TRUE(d == a);
	d = a - 60;
	EXPECT_TRUE(d == a);
}
TEST(TSetTest, OperationsTest) {
	TSet a(40), b(40);
	for (size_t i = 0; i < 25; ++i) {
		a.InsElem(i);
	}
	for (size_t i = 10; i < 38; ++i) {
		b.InsElem(i);
	}
	TSet ob = a + b;
	for (size_t i = 0; i < 38; ++i) {
		EXPECT_EQ(ob.IsMember(i), 1);
	}
	TSet per = a * b;
	EXPECT_EQ(a.IsMember(20), 1);
	EXPECT_EQ(b.IsMember(20), 1);
	EXPECT_EQ(per.IsMember(20), 1);
	EXPECT_EQ(a.IsMember(0), 1);
	EXPECT_EQ(b.IsMember(0), 0);
	EXPECT_EQ(per.IsMember(0), 0);
	TSet dop = ~a;
	EXPECT_FALSE(dop == a);
	for (size_t i = 0; i < 25; ++i) {
		EXPECT_EQ(dop.IsMember(i), 0);
	}
}
TEST(TSetTest, OperationsInvariantsTest) {
	TSet a(40), b(40);
	for (size_t i = 0; i < 25; ++i) {
		a.InsElem(i);
	}
	for (size_t i = 10; i < 38; ++i) {
		b.InsElem(i);
	}
	EXPECT_TRUE((a + a) == a);
	EXPECT_TRUE((b * b) == b);
	EXPECT_TRUE((a + b) == (b + a));
	EXPECT_TRUE((a * b) == (b * a));
	EXPECT_TRUE(~~b == b);
}