#include "tbitfield.h"
#include <gtest/gtest.h>
#include <climits>
#include <sstream>
#include <stdexcept>

TEST(BitField, NewFieldIsZero) {
    TBitField field(6);

    EXPECT_EQ(field.GetLength(), 6);
    EXPECT_EQ(field.GetBit(0), 0);
    EXPECT_EQ(field.GetBit(3), 0);
    EXPECT_EQ(field.GetBit(5), 0);
}

TEST(BitField, ZeroLength) {
    TBitField field(0);

    EXPECT_EQ(field.GetLength(), 0);
    EXPECT_THROW(field.GetBit(0), std::out_of_range);
}

TEST(BitField, NegativeLength) {
    EXPECT_THROW(TBitField(-3), std::invalid_argument);
}

TEST(BitField, SetBit) {
    TBitField field(8);
    field.SetBit(4);

    EXPECT_EQ(field.GetBit(4), 1);
    EXPECT_EQ(field.GetBit(3), 0);
}

TEST(BitField, ClearBit) {
    TBitField field(8);
    field.SetBit(4);
    field.ClrBit(4);

    EXPECT_EQ(field.GetBit(4), 0);
}

TEST(BitField, BitsOnWordBoundary) {
    int wordSize = sizeof(TELEM) * CHAR_BIT;
    TBitField field(wordSize + 2);
    field.SetBit(wordSize - 1);
    field.SetBit(wordSize);
    field.ClrBit(wordSize - 1);

    EXPECT_EQ(field.GetBit(wordSize - 1), 0);
    EXPECT_EQ(field.GetBit(wordSize), 1);
    EXPECT_EQ(field.GetBit(wordSize + 1), 0);
}

TEST(BitField, InvalidSetIndex) {
    TBitField field(6);

    EXPECT_THROW(field.SetBit(-1), std::out_of_range);
    EXPECT_THROW(field.SetBit(6), std::out_of_range);
}

TEST(BitField, InvalidClearIndex) {
    TBitField field(6);

    EXPECT_THROW(field.ClrBit(-1), std::out_of_range);
    EXPECT_THROW(field.ClrBit(6), std::out_of_range);
}

TEST(BitField, InvalidGetIndex) {
    TBitField field(6);

    EXPECT_THROW(field.GetBit(-1), std::out_of_range);
    EXPECT_THROW(field.GetBit(6), std::out_of_range);
}

TEST(BitField, CopyHasOwnMemory) {
    TBitField first(8);
    first.SetBit(4);
    TBitField second(first);
    EXPECT_EQ(first, second);

    second.ClrBit(4);

    EXPECT_EQ(first.GetBit(4), 1);
    EXPECT_EQ(second.GetBit(4), 0);
}

TEST(BitField, AssignLargerField) {
    TBitField first(3);
    TBitField second(70);
    second.SetBit(65);
    first = second;
    second.ClrBit(65);

    EXPECT_EQ(first.GetLength(), 70);
    EXPECT_EQ(first.GetBit(65), 1);
    EXPECT_EQ(second.GetBit(65), 0);
}

TEST(BitField, AssignSmallerField) {
    TBitField first(70);
    first.SetBit(65);
    TBitField second(3);
    second.SetBit(1);
    first = second;

    EXPECT_EQ(first.GetLength(), 3);
    EXPECT_EQ(first, second);
}

TEST(BitField, UnionOfDifferentLengths) {
    TBitField first(5);
    TBitField second(70);
    first.SetBit(2);
    second.SetBit(65);
    TBitField result = first | second;

    EXPECT_EQ(result.GetLength(), 70);
    EXPECT_EQ(result.GetBit(2), 1);
    EXPECT_EQ(result.GetBit(65), 1);
    EXPECT_EQ(result.GetBit(3), 0);
    EXPECT_EQ(second.GetBit(2), 0);
}

TEST(BitField, IntersectionOfDifferentLengths) {
    TBitField first(5);
    TBitField second(70);
    first.SetBit(1);
    first.SetBit(2);
    second.SetBit(2);
    second.SetBit(65);
    TBitField result = first & second;

    EXPECT_EQ(result.GetLength(), 70);
    EXPECT_EQ(result.GetBit(2), 1);
    EXPECT_EQ(result.GetBit(1), 0);
    EXPECT_EQ(result.GetBit(65), 0);
    EXPECT_EQ(first.GetBit(1), 1);
}

TEST(BitField, Complement) {
    TBitField field(3);
    field.SetBit(1);
    TBitField result = ~field;

    EXPECT_EQ(result.GetBit(0), 1);
    EXPECT_EQ(result.GetBit(1), 0);
    EXPECT_EQ(result.GetBit(2), 1);
    EXPECT_EQ(~result, field);
}

TEST(BitField, ComplementDoesNotSetExtraBits) {
    TBitField shortField(3);
    TBitField longerField(8);
    TBitField inverted = ~shortField;
    TBitField result = inverted | longerField;

    EXPECT_EQ(result.GetBit(2), 1);
    EXPECT_EQ(result.GetBit(3), 0);
    EXPECT_EQ(result.GetBit(7), 0);
}

TEST(BitField, Output) {
    TBitField field(5);
    field.SetBit(0);
    field.SetBit(3);
    std::ostringstream stream;
    stream << field;

    EXPECT_EQ(stream.str(), "10010");
}

TEST(BitField, Input) {
    TBitField field(5);
    field.SetBit(4);
    std::istringstream stream("10010");
    stream >> field;

    EXPECT_FALSE(stream.fail());
    EXPECT_EQ(field.GetBit(0), 1);
    EXPECT_EQ(field.GetBit(3), 1);
    EXPECT_EQ(field.GetBit(4), 0);
}

TEST(BitField, InvalidInputCharacter) {
    TBitField field(3);
    field.SetBit(1);
    TBitField before(field);
    std::istringstream stream("1x0");
    stream >> field;

    EXPECT_TRUE(stream.fail());
    EXPECT_EQ(field, before);
}

TEST(BitField, InputTooShort) {
    TBitField field(3);
    std::istringstream stream("10");
    stream >> field;

    EXPECT_TRUE(stream.fail());
    EXPECT_EQ(field.GetBit(0), 0);
}

TEST(BitField, InputTooLong) {
    TBitField field(3);
    std::istringstream stream("1001");
    stream >> field;

    EXPECT_TRUE(stream.fail());
    EXPECT_EQ(field.GetBit(0), 0);
}

TEST(BitField, EmptyInput) {
    TBitField field(3);
    field.SetBit(1);
    std::istringstream stream("");
    stream >> field;

    EXPECT_TRUE(stream.fail());
    EXPECT_EQ(field.GetBit(1), 1);
}
