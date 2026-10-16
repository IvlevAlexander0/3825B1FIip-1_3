#include "pch.h"
#include "tset.h"

TEST(TestTSet, TestConstruct) {
	TSet a(50);
	EXPECT_EQ(a.GetMaxPower(), 50);
	TSet b(a);
	EXPECT_EQ(a.GetMaxPower(), b.GetMaxPower());
	TSet c = a;
	EXPECT_EQ(a.GetMaxPower(), c.GetMaxPower());

	TBitField bf(20);
	bf.SetBit(5);
	bf.SetBit(15);
	TSet s(bf);
	EXPECT_EQ(s.GetMaxPower(), 20);
	EXPECT_EQ(s.IsMember(5), 1);
	EXPECT_EQ(s.IsMember(15), 1);
	EXPECT_EQ(s.IsMember(10), 0);

	TBitField converted = s;
	EXPECT_EQ(converted.GetLength(), 20);
	EXPECT_EQ(converted.GetBit(5), 1);
}

TEST(TestTSet, TestInsDel) {
	TSet a(50);
	a.InsElem(35);
	EXPECT_EQ(TBitField(a).GetBit(35), 1);
	a.DelElem(35);
	EXPECT_EQ(TBitField(a).GetBit(35), 0);
	a.DelElem(30);
	EXPECT_EQ(TBitField(a).GetBit(30), 0);
}

TEST(TestTSet, TestCompareOperator) {
	TSet a(50), b(50);
	a.InsElem(10);
	b.InsElem(10);
	EXPECT_EQ(a == b, 1);
	EXPECT_EQ(a != b, 0);
	a.InsElem(12);
	EXPECT_EQ(a == b, 0);
	EXPECT_EQ(a != b, 1);
}

TEST(TestTSet, TestLogicalOperator) {
	TSet a(10), b(10);
	a.InsElem(4);
	b.InsElem(5);
	TSet un = a + b;
	for (int i = 0; i < 10; i++) {
		if (i == 4 || i == 5) {
			EXPECT_EQ(un.IsMember(i), 1);
			continue;
		}
		EXPECT_EQ(un.IsMember(i), 0);
	}

	b.InsElem(4);
	TSet inter = a * b;
	for (int i = 0; i < 10; i++) {
		if (i == 4) {
			EXPECT_EQ(inter.IsMember(i), 1);
			continue;
		}
		EXPECT_EQ(inter.IsMember(i), 0);
	}

	TSet small(10);
	TSet large(40);

	small.InsElem(3);
	large.InsElem(35);

	EXPECT_THROW(TSet Union = small + large, std::invalid_argument);
	EXPECT_THROW(TSet intersection = small * large;, std::invalid_argument);

	TSet t(5);
	t.InsElem(1);
	t.InsElem(3);

	TSet inv = ~t;
	EXPECT_EQ(inv.IsMember(0), 1);
	EXPECT_EQ(inv.IsMember(1), 0);
	EXPECT_EQ(inv.IsMember(2), 1);
	EXPECT_EQ(inv.IsMember(3), 0);
	EXPECT_EQ(inv.IsMember(4), 1);
}

TEST(TestTSet, TestComplexOperator) {
	TSet a(15);
	TSet b(15);

	a.InsElem(1);
	a.InsElem(2);
	b.InsElem(2);
	b.InsElem(3);
	TSet result = (a + 5) * b - 2;
	EXPECT_EQ(result.IsMember(1), 0);
	EXPECT_EQ(result.IsMember(2), 0);
	EXPECT_EQ(result.IsMember(3), 0);
	EXPECT_EQ(result.IsMember(5), 0);

	EXPECT_EQ(a.IsMember(1), 1);
	EXPECT_EQ(a.IsMember(2), 1);
	EXPECT_EQ(b.IsMember(2), 1);
	EXPECT_EQ(b.IsMember(3), 1);
}


TEST(TestTSet, TestSetEdgeCases) {
	TSet self(10);
	self.InsElem(7);
	self = self;
	EXPECT_EQ(self.GetMaxPower(), 10);
	EXPECT_EQ(self.IsMember(7), 1);

	TSet a(50);
	EXPECT_THROW(a.InsElem(-1), std::out_of_range);
	EXPECT_THROW(a.InsElem(50), std::out_of_range);
	EXPECT_THROW(a.InsElem(55), std::out_of_range);

	EXPECT_THROW(a.IsMember(-1), std::out_of_range);
	EXPECT_THROW(a.IsMember(50), std::out_of_range);

	TSet s(20);
	TSet plus = s + 5;
	TSet minus = plus - 5;
	EXPECT_EQ(s.IsMember(5), 0);
	EXPECT_EQ(plus.IsMember(5), 1);
	EXPECT_EQ(minus.IsMember(5), 0);

	EXPECT_THROW(s + 25, std::out_of_range);
	EXPECT_THROW(s - 25, std::out_of_range);
}