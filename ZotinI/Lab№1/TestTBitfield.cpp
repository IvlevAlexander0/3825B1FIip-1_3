#include "pch.h"
#include <stdexcept>
#include "tbitfield.h"

TEST(TestTbitfield, TestConstruct) {
	TBitField a(10);
	EXPECT_EQ(a.GetLength(), 10);
	TBitField b(a);
	EXPECT_EQ(a.GetLength(), b.GetLength());
	TBitField c = a;
	EXPECT_EQ(a.GetLength(), c.GetLength());
}

TEST(TestTbitfield, TestClrSetGet) {
	TBitField a(10);
	a.SetBit(5);
	EXPECT_EQ(a.GetBit(4), 0);
	EXPECT_EQ(a.GetBit(5), 1);
	EXPECT_EQ(a.GetBit(6), 0);
	a.ClrBit(5);
	EXPECT_EQ(a.GetBit(5), 0);
	a.ClrBit(4);
	EXPECT_EQ(a.GetBit(4), 0);
}

TEST(TestTbitfield, TestCompareOperator) {
	TBitField a(10);
	TBitField b(10);
	a.SetBit(5);
	b.SetBit(5);
	EXPECT_EQ(a == b, 1);
	EXPECT_EQ(a != b, 0);
	b.SetBit(9);
	EXPECT_EQ(a == b, 0);
	EXPECT_EQ(a != b, 1);
}

TEST(TestTbitfield, TestLogicalOperator) {
	TBitField a(10);
	TBitField b(10);
	a.SetBit(5);
	b.SetBit(5);
	EXPECT_EQ(a | b, a);
	EXPECT_EQ(a & b, a);

	b.SetBit(8);
	a.SetBit(7);
	TBitField c = a | b;
	TBitField d = a & b;
	EXPECT_EQ(c.GetBit(7), 1);
	EXPECT_EQ(c.GetBit(8), 1);
	EXPECT_EQ(c.GetBit(5), 1);
	EXPECT_EQ(d.GetBit(7), 0);
	EXPECT_EQ(d.GetBit(8), 0);
	EXPECT_EQ(d.GetBit(5), 1);

	TBitField n = ~a;
	for (int i = 0; i < 10; i++) {
		if (i == 5 || i == 7) {
			EXPECT_EQ(n.GetBit(i), 0);
		}
		else {
			EXPECT_EQ(n.GetBit(i), 1);
		}
	}
}

TEST(TestTbitfield, TestEdgeCases) {
	TBitField sa(15);
	sa.SetBit(3);
	sa = sa;
	EXPECT_EQ(sa.GetLength(), 15);
	EXPECT_EQ(sa.GetBit(3), 1);

	EXPECT_THROW(TBitField f(-2), std::out_of_range);
	TBitField h(0);
	EXPECT_EQ(h.GetLength(), 0);

	TBitField e(10);
	EXPECT_THROW(e.SetBit(-1), std::out_of_range);
	EXPECT_THROW(e.SetBit(10), std::out_of_range);
	EXPECT_THROW(e.SetBit(100), std::out_of_range);
	EXPECT_THROW(e.GetBit(-1), std::out_of_range);
	EXPECT_THROW(e.GetBit(10), std::out_of_range);

	TBitField s(5);
	TBitField l(40);
	s.SetBit(2);
	l.SetBit(35);

	TBitField r = s | l;
	EXPECT_EQ(r.GetLength(), 40);
	EXPECT_EQ(r.GetBit(2), 1);
	EXPECT_EQ(r.GetBit(35), 1);

	TBitField a = s & l;
	EXPECT_EQ(a.GetLength(), 40);
	EXPECT_EQ(a.GetBit(2), 0);
	EXPECT_EQ(a.GetBit(35), 0);

	TBitField small(5);
	TBitField large(100);

	small.SetBit(2);
	large.SetBit(95);
	large = small;
	EXPECT_EQ(large.GetLength(), 5);
	EXPECT_EQ(large.GetBit(2), 1);
	EXPECT_THROW(large.GetBit(95), std::out_of_range);
}

