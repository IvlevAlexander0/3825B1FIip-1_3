#include <gtest/gtest.h>
#include "tbitfield.h"
#include <stdexcept>
#include <sstream>

TEST(TBitFieldTest, can_create_bitfield_with_positive_length) {
    EXPECT_NO_THROW(TBitField bf(10));
}

TEST(TBitFieldTest, can_get_length) {
    TBitField bf(42);
    EXPECT_EQ(bf.GetLength(), 42);
}

TEST(TBitFieldTest, throws_when_create_bitfield_with_negative_or_zero_length) {
    EXPECT_THROW(TBitField bf(-10), std::invalid_argument);
    EXPECT_THROW(TBitField bf(0), std::invalid_argument);
}

TEST(TBitFieldTest, new_bitfield_is_initialized_to_zero) {
    TBitField bf(100);
    for (int i = 0; i < bf.GetLength(); i++) {
        EXPECT_EQ(bf.GetBit(i), 0);
    }
}

TEST(TBitFieldTest, can_set_and_get_bit_across_telem_boundaries) {
    TBitField bf(70);
    bf.SetBit(0);
    bf.SetBit(31);
    bf.SetBit(32);
    bf.SetBit(63);
    bf.SetBit(64);
    bf.SetBit(69);

    EXPECT_EQ(bf.GetBit(0), 1);
    EXPECT_EQ(bf.GetBit(31), 1);
    EXPECT_EQ(bf.GetBit(32), 1);
    EXPECT_EQ(bf.GetBit(63), 1);
    EXPECT_EQ(bf.GetBit(64), 1);
    EXPECT_EQ(bf.GetBit(69), 1);
    EXPECT_EQ(bf.GetBit(33), 0);
}

TEST(TBitFieldTest, setting_bit_twice_keeps_it_set) {
    TBitField bf(10);
    bf.SetBit(4);
    bf.SetBit(4);
    EXPECT_EQ(bf.GetBit(4), 1);
}

TEST(TBitFieldTest, can_clear_bit) {
    TBitField bf(40);
    bf.SetBit(32);
    bf.ClrBit(32);
    EXPECT_EQ(bf.GetBit(32), 0);
}

TEST(TBitFieldTest, clearing_zero_bit_keeps_it_zero) {
    TBitField bf(10);
    bf.ClrBit(2);
    EXPECT_EQ(bf.GetBit(2), 0);
}

TEST(TBitFieldTest, throws_when_set_bit_out_of_bounds) {
    TBitField bf(10);
    EXPECT_THROW(bf.SetBit(-1), std::out_of_range);
    EXPECT_THROW(bf.SetBit(10), std::out_of_range);
}

TEST(TBitFieldTest, throws_when_clear_bit_out_of_bounds) {
    TBitField bf(10);
    EXPECT_THROW(bf.ClrBit(-1), std::out_of_range);
    EXPECT_THROW(bf.ClrBit(10), std::out_of_range);
}

TEST(TBitFieldTest, throws_when_get_bit_out_of_bounds) {
    TBitField bf(10);
    EXPECT_THROW(bf.GetBit(-1), std::out_of_range);
    EXPECT_THROW(bf.GetBit(10), std::out_of_range);
}

TEST(TBitFieldTest, copy_constructor_creates_independent_deep_copy) {
    TBitField bf1(40);
    bf1.SetBit(35);
    TBitField bf2(bf1);

    EXPECT_EQ(bf1, bf2);
    bf2.SetBit(1);
    EXPECT_NE(bf1, bf2);
    EXPECT_EQ(bf1.GetBit(1), 0);
}

TEST(TBitFieldTest, can_assign_bitfields_of_equal_and_different_sizes) {
    TBitField bf1(10), bf2(50);
    bf1.SetBit(3);
    bf2 = bf1;

    EXPECT_EQ(bf1, bf2);
    EXPECT_EQ(bf2.GetLength(), 10);
}

TEST(TBitFieldTest, self_assignment_is_safe) {
    TBitField bf(20);
    bf.SetBit(7);
    bf = bf;
    EXPECT_EQ(bf.GetLength(), 20);
    EXPECT_EQ(bf.GetBit(7), 1);
}

TEST(TBitFieldTest, compare_equal_and_non_equal_bitfields) {
    TBitField bf1(20), bf2(20), bf3(25);
    bf1.SetBit(10);
    bf2.SetBit(10);

    EXPECT_EQ(bf1 == bf2, 1);
    EXPECT_EQ(bf1 != bf2, 0);
    EXPECT_EQ(bf1 == bf3, 0);
    EXPECT_EQ(bf1 != bf3, 1);
}

TEST(TBitFieldTest, bitwise_OR_on_equal_and_different_sizes) {
    TBitField bf1(10), bf2(40);
    bf1.SetBit(2);
    bf2.SetBit(35);
    TBitField res = bf1 | bf2;

    EXPECT_EQ(res.GetLength(), 40);
    EXPECT_EQ(res.GetBit(2), 1);
    EXPECT_EQ(res.GetBit(35), 1);
    EXPECT_EQ(res.GetBit(0), 0);
}

TEST(TBitFieldTest, bitwise_AND_on_equal_and_different_sizes) {
    TBitField bf1(10), bf2(40);
    bf1.SetBit(2);
    bf1.SetBit(5);
    bf2.SetBit(5);
    bf2.SetBit(35);
    TBitField res = bf1 & bf2;

    EXPECT_EQ(res.GetLength(), 40);
    EXPECT_EQ(res.GetBit(5), 1);
    EXPECT_EQ(res.GetBit(2), 0);
    EXPECT_EQ(res.GetBit(35), 0);
}

TEST(TBitFieldTest, bitwise_NOT_on_exact_telem_multiple) {
    TBitField bf(32);
    bf.SetBit(0);
    bf.SetBit(31);
    TBitField inv = ~bf;

    EXPECT_EQ(inv.GetBit(0), 0);
    EXPECT_EQ(inv.GetBit(31), 0);
    EXPECT_EQ(inv.GetBit(15), 1);
    EXPECT_EQ(~inv, bf);
}

TEST(TBitFieldTest, bitwise_NOT_clears_tail_on_non_multiple_size) {
    TBitField bf(35);
    TBitField inv = ~bf;
    TBitField expected(35);
    for (int i = 0; i < 35; i++) {
        expected.SetBit(i);
    }
    EXPECT_EQ(inv, expected);
    EXPECT_EQ(~inv, bf);
}

TEST(TBitFieldTest, stream_output_and_input_roundtrip) {
    TBitField bf1(6);
    bf1.SetBit(0);
    bf1.SetBit(3);
    bf1.SetBit(5);

    std::stringstream ss;
    ss << bf1;
    EXPECT_EQ(ss.str(), "100101");

    TBitField bf2(6);
    bf2.SetBit(1);
    ss >> bf2;
    EXPECT_EQ(bf1, bf2);
}

TEST(TBitFieldTest, stream_input_overwrites_object_on_short_input) {
    TBitField bf(5);
    for (int i = 0; i < 5; i++) bf.SetBit(i);

    std::istringstream iss("10");
    iss >> bf;

    EXPECT_EQ(bf.GetBit(0), 1);
    EXPECT_EQ(bf.GetBit(1), 0);
    EXPECT_EQ(bf.GetBit(2), 0);
    EXPECT_EQ(bf.GetBit(3), 0);
    EXPECT_EQ(bf.GetBit(4), 0);
}

TEST(TBitFieldTest, stream_input_stops_on_invalid_character) {
    TBitField bf(5);
    for (int i = 0; i < 5; i++) bf.SetBit(i);

    std::istringstream iss("11x11");
    iss >> bf;

    EXPECT_EQ(bf.GetBit(0), 1);
    EXPECT_EQ(bf.GetBit(1), 1);
    EXPECT_EQ(bf.GetBit(2), 0);
    EXPECT_EQ(bf.GetBit(3), 0);
    EXPECT_EQ(bf.GetBit(4), 0);
}