#include <gtest/gtest.h>
#include "tset.h"
#include <stdexcept>
#include <sstream>

TEST(TSetTest, can_create_set_with_positive_max_power) {
    TSet s(20);
    EXPECT_EQ(s.GetMaxPower(), 20);
}

TEST(TSetTest, throws_when_create_set_with_negative_or_zero_max_power) {
    EXPECT_THROW(TSet s(-5), std::invalid_argument);
    EXPECT_THROW(TSet s(0), std::invalid_argument);
}

TEST(TSetTest, can_copy_set) {
    TSet s1(15);
    s1.InsElem(7);
    TSet s2(s1);

    EXPECT_EQ(s1, s2);
    s2.InsElem(3);
    EXPECT_NE(s1, s2);
}

TEST(TSetTest, can_convert_between_tset_and_tbitfield) {
    TBitField bf(12);
    bf.SetBit(4);
    bf.SetBit(11);

    TSet s(bf);
    EXPECT_EQ(s.GetMaxPower(), 12);
    EXPECT_EQ(s.IsMember(4), 1);
    EXPECT_EQ(s.IsMember(11), 1);

    TBitField bf_back = static_cast<TBitField>(s);
    EXPECT_EQ(bf, bf_back);
}

TEST(TSetTest, can_insert_check_and_delete_elements_at_boundaries) {
    TSet s(40);
    s.InsElem(0);
    s.InsElem(39);

    EXPECT_EQ(s.IsMember(0), 1);
    EXPECT_EQ(s.IsMember(39), 1);
    EXPECT_EQ(s.IsMember(20), 0);

    s.DelElem(0);
    EXPECT_EQ(s.IsMember(0), 0);
}

TEST(TSetTest, throws_when_insert_delete_or_check_out_of_bounds) {
    TSet s(10);
    EXPECT_THROW(s.InsElem(-1), std::out_of_range);
    EXPECT_THROW(s.InsElem(10), std::out_of_range);
    EXPECT_THROW(s.DelElem(-1), std::out_of_range);
    EXPECT_THROW(s.DelElem(10), std::out_of_range);
    EXPECT_THROW(s.IsMember(-1), std::out_of_range);
    EXPECT_THROW(s.IsMember(10), std::out_of_range);
}

TEST(TSetTest, can_assign_sets_of_different_sizes) {
    TSet s1(10), s2(30);
    s1.InsElem(5);
    s2 = s1;

    EXPECT_EQ(s2.GetMaxPower(), 10);
    EXPECT_EQ(s1, s2);
}

TEST(TSetTest, compare_sets_of_equal_and_different_universes) {
    TSet s1(10), s2(10), s3(20);
    s1.InsElem(3);
    s2.InsElem(3);
    s3.InsElem(3);

    EXPECT_EQ(s1 == s2, 1);
    EXPECT_EQ(s1 != s2, 0);
    EXPECT_EQ(s1 == s3, 0);
    EXPECT_EQ(s1 != s3, 1);
}

TEST(TSetTest, operator_plus_and_minus_element_do_not_modify_original_set) {
    TSet s(10);
    TSet s_plus = s + 4;
    EXPECT_EQ(s.IsMember(4), 0);
    EXPECT_EQ(s_plus.IsMember(4), 1);

    TSet s_minus = s_plus - 4;
    EXPECT_EQ(s_plus.IsMember(4), 1);
    EXPECT_EQ(s_minus.IsMember(4), 0);
}

TEST(TSetTest, operator_plus_and_minus_element_throw_out_of_bounds) {
    TSet s(10);
    EXPECT_THROW(s + 10, std::out_of_range);
    EXPECT_THROW(s + (-1), std::out_of_range);
    EXPECT_THROW(s - 10, std::out_of_range);
    EXPECT_THROW(s - (-1), std::out_of_range);
}

TEST(TSetTest, can_union_two_sets_from_same_universe) {
    TSet s1(15), s2(15);
    s1.InsElem(2);
    s2.InsElem(12);
    TSet s3 = s1 + s2;

    EXPECT_EQ(s3.GetMaxPower(), 15);
    EXPECT_EQ(s3.IsMember(2), 1);
    EXPECT_EQ(s3.IsMember(12), 1);
    EXPECT_EQ(s3.IsMember(5), 0);
}

TEST(TSetTest, throws_when_union_sets_from_different_universes) {
    TSet s1(10), s2(15);
    EXPECT_THROW(s1 + s2, std::invalid_argument);
}

TEST(TSetTest, can_intersect_two_sets_from_same_universe) {
    TSet s1(15), s2(15);
    s1.InsElem(2);
    s1.InsElem(7);
    s2.InsElem(7);
    s2.InsElem(12);

    TSet s3 = s1 * s2;
    EXPECT_EQ(s3.IsMember(7), 1);
    EXPECT_EQ(s3.IsMember(2), 0);
    EXPECT_EQ(s3.IsMember(12), 0);
}

TEST(TSetTest, throws_when_intersect_sets_from_different_universes) {
    TSet s1(10), s2(15);
    EXPECT_THROW(s1 * s2, std::invalid_argument);
}

TEST(TSetTest, can_complement_set_with_large_power) {
    TSet s(35);
    s.InsElem(0);
    s.InsElem(34);
    TSet inv = ~s;

    EXPECT_EQ(inv.IsMember(0), 0);
    EXPECT_EQ(inv.IsMember(34), 0);
    EXPECT_EQ(inv.IsMember(15), 1);
    EXPECT_EQ(~inv, s);
}

TEST(TSetTest, stream_output_and_input_roundtrip_with_multidigit_elements) {
    TSet s1(20);
    s1.InsElem(1);
    s1.InsElem(2);
    s1.InsElem(12);
    s1.InsElem(19);

    std::stringstream ss;
    ss << s1;

    TSet s2(20);
    s2.InsElem(5);
    ss >> s2;

    EXPECT_EQ(s1, s2);
    EXPECT_EQ(s2.IsMember(5), 0);
}

TEST(TSetTest, stream_input_handles_empty_set_and_overwrites_previous_state) {
    TSet s(10);
    s.InsElem(3);
    s.InsElem(8);

    std::istringstream iss("{ }");
    iss >> s;

    for (int i = 0; i < 10; i++) {
        EXPECT_EQ(s.IsMember(i), 0);
    }
}