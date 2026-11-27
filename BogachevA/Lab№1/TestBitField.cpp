#include "pch.h"
TEST(BitField, Create) {
    ASSERT_NO_THROW(TBitField bf(10));
}
TEST(BitField, CreateNegative) {
    ASSERT_ANY_THROW(TBitField bf(-1));
}
TEST(BitField, SetAndGet) {
    TBitField bf(10);
    bf.SetBit(3);
    bf.SetBit(7);
    EXPECT_EQ(1, bf.GetBit(3));
    EXPECT_EQ(1, bf.GetBit(7));
    EXPECT_EQ(0, bf.GetBit(5));
}
TEST(BitField, Clear) {
    TBitField bf(10);
    bf.SetBit(3);
    bf.ClrBit(3);
    EXPECT_EQ(0, bf.GetBit(3));
}
TEST(BitField, CopyConstructor) {
    TBitField bf(10);
    bf.SetBit(2);
    bf.SetBit(8);
    TBitField copy(bf);
    EXPECT_EQ(1, copy.GetBit(2));
    EXPECT_EQ(1, copy.GetBit(8));
    EXPECT_EQ(1, bf == copy);
}
TEST(BitField, Assignment) {
    TBitField bf1(10);
    TBitField bf2(10);
    bf1.SetBit(5);
    bf2 = bf1;
    EXPECT_EQ(1, bf2.GetBit(5));
    EXPECT_EQ(1, bf1 == bf2);
}
TEST(BitField, Compare) {
    TBitField bf1(10);
    TBitField bf2(10);
    EXPECT_EQ(1, bf1 == bf2);
    bf1.SetBit(4);
    EXPECT_EQ(1, bf1 != bf2);
}
TEST(BitField, OrOperation) {
    TBitField bf1(10);
    TBitField bf2(10);
    bf1.SetBit(2);
    bf2.SetBit(5);
    TBitField result = bf1 | bf2;
    EXPECT_EQ(1, result.GetBit(2));
    EXPECT_EQ(1, result.GetBit(5));
}
TEST(BitField, AndOperation) {
    TBitField bf1(10);
    TBitField bf2(10);
    bf1.SetBit(3);
    bf1.SetBit(5);
    bf2.SetBit(5);
    bf2.SetBit(7);
    TBitField result = bf1 & bf2;
    EXPECT_EQ(0, result.GetBit(3));
    EXPECT_EQ(1, result.GetBit(5));
    EXPECT_EQ(0, result.GetBit(7));
}
TEST(BitField, AndDifferentSize) {
    TBitField bf1(40);
    TBitField bf2(10);
    bf1.SetBit(2);
    bf1.SetBit(30);
    bf2.SetBit(2);
    bf2.SetBit(5);
    TBitField result = bf1 & bf2;
    EXPECT_EQ(1, result.GetBit(2));
    EXPECT_EQ(0, result.GetBit(5));
}
TEST(BitField, NotOperation) {
    TBitField bf(5);
    bf.SetBit(2);
    TBitField result = ~bf;
    EXPECT_EQ(1, result.GetBit(0));
    EXPECT_EQ(1, result.GetBit(1));
    EXPECT_EQ(0, result.GetBit(2));
    EXPECT_EQ(1, result.GetBit(3));
    EXPECT_EQ(1, result.GetBit(4));
}
TEST(BitField, OutOfRange) {
    TBitField bf(10);
    ASSERT_ANY_THROW(bf.SetBit(10));
    ASSERT_ANY_THROW(bf.SetBit(-1));
    ASSERT_ANY_THROW(bf.GetBit(20));
}
TEST(BitField, EmptyField) {
    TBitField bf(0);
    EXPECT_EQ(0, bf.GetLength());
}
TEST(BitField, RandomSetAndGet) {
    srand((unsigned)time(nullptr));
    for (int k = 0; k < 100; k++) {
        int size = rand() % 200 + 1;
        TBitField bf(size);
        std::vector<int> values(size, 0);
        for (int i = 0; i < size; i++) {
            if (rand() % 2) {
                bf.SetBit(i);
                values[i] = 1;
            }
        }
        for (int i = 0; i < size; i++) {
            EXPECT_EQ(values[i], bf.GetBit(i));
        }
    }
}