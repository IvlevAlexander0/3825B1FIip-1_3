#include "tset.h"
#include <gtest/gtest.h>
#include <sstream>
#include <stdexcept>

TEST(Set, NewSetIsEmpty) {
    TSet set(6);

    EXPECT_EQ(set.GetMaxPower(), 6);
    EXPECT_EQ(set.IsMember(0), 0);
    EXPECT_EQ(set.IsMember(5), 0);
}

TEST(Set, NegativeSize) {
    EXPECT_THROW(TSet(-3), std::invalid_argument);
}

TEST(Set, InsertElementTwice) {
    TSet set(8);
    set.InsElem(3);
    set.InsElem(3);

    EXPECT_EQ(set.IsMember(3), 1);
    EXPECT_EQ(set.IsMember(4), 0);
}

TEST(Set, RemoveElementTwice) {
    TSet set(8);
    set.InsElem(3);
    set.InsElem(4);
    set.DelElem(3);
    set.DelElem(3);

    EXPECT_EQ(set.IsMember(3), 0);
    EXPECT_EQ(set.IsMember(4), 1);
}

TEST(Set, InvalidInsert) {
    TSet set(6);

    EXPECT_THROW(set.InsElem(-1), std::out_of_range);
    EXPECT_THROW(set.InsElem(6), std::out_of_range);
}

TEST(Set, InvalidRemove) {
    TSet set(6);

    EXPECT_THROW(set.DelElem(-1), std::out_of_range);
    EXPECT_THROW(set.DelElem(6), std::out_of_range);
}

TEST(Set, InvalidMembershipCheck) {
    TSet set(6);

    EXPECT_THROW(set.IsMember(-1), std::out_of_range);
    EXPECT_THROW(set.IsMember(6), std::out_of_range);
}

TEST(Set, CopyHasOwnMemory) {
    TSet first(8);
    first.InsElem(3);
    TSet second(first);
    EXPECT_EQ(first, second);

    second.DelElem(3);

    EXPECT_EQ(first.IsMember(3), 1);
    EXPECT_EQ(second.IsMember(3), 0);
}

TEST(Set, AssignmentChangesSize) {
    TSet first(3);
    TSet second(70);
    second.InsElem(65);
    first = second;
    second.DelElem(65);

    EXPECT_EQ(first.GetMaxPower(), 70);
    EXPECT_EQ(first.IsMember(65), 1);
    EXPECT_EQ(second.IsMember(65), 0);
}

TEST(Set, SelfAssignment) {
    TSet set(8);
    set.InsElem(3);
    set = set;

    EXPECT_EQ(set.GetMaxPower(), 8);
    EXPECT_EQ(set.IsMember(3), 1);
}

TEST(Set, CreateFromBitField) {
    TBitField field(8);
    field.SetBit(3);
    TSet set(field);
    field.ClrBit(3);

    EXPECT_EQ(set.GetMaxPower(), 8);
    EXPECT_EQ(set.IsMember(3), 1);
}

TEST(Set, ConvertToBitField) {
    TSet set(8);
    set.InsElem(3);
    TBitField field = set;
    EXPECT_EQ(field.GetLength(), 8);
    EXPECT_EQ(field.GetBit(3), 1);

    field.ClrBit(3);

    EXPECT_EQ(set.IsMember(3), 1);
}

TEST(Set, AddElementReturnsNewSet) {
    TSet set(8);
    set.InsElem(1);
    TSet result = set + 3;

    EXPECT_EQ(result.IsMember(1), 1);
    EXPECT_EQ(result.IsMember(3), 1);
    EXPECT_EQ(set.IsMember(3), 0);
}

TEST(Set, SubtractElementReturnsNewSet) {
    TSet set(8);
    set.InsElem(3);
    TSet result = set - 3;

    EXPECT_EQ(result.IsMember(3), 0);
    EXPECT_EQ(set.IsMember(3), 1);
}

TEST(Set, AddInvalidElement) {
    TSet set(6);

    EXPECT_THROW(set + 6, std::out_of_range);
}

TEST(Set, SubtractInvalidElement) {
    TSet set(6);

    EXPECT_THROW(set - 6, std::out_of_range);
}

TEST(Set, Union) {
    TSet first(8);
    TSet second(8);
    first.InsElem(2);
    second.InsElem(6);
    TSet result = first + second;

    EXPECT_EQ(result.GetMaxPower(), 8);
    EXPECT_EQ(result.IsMember(2), 1);
    EXPECT_EQ(result.IsMember(6), 1);
    EXPECT_EQ(result.IsMember(0), 0);
}

TEST(Set, Intersection) {
    TSet first(8);
    TSet second(8);
    first.InsElem(1);
    first.InsElem(2);
    second.InsElem(2);
    second.InsElem(6);
    TSet result = first * second;

    EXPECT_EQ(result.GetMaxPower(), 8);
    EXPECT_EQ(result.IsMember(2), 1);
    EXPECT_EQ(result.IsMember(1), 0);
    EXPECT_EQ(result.IsMember(6), 0);
}

TEST(Set, Complement) {
    TSet set(3);
    set.InsElem(1);
    TSet result = ~set;

    EXPECT_EQ(result.IsMember(0), 1);
    EXPECT_EQ(result.IsMember(1), 0);
    EXPECT_EQ(result.IsMember(2), 1);
}

TEST(Set, Output) {
    TSet set(8);
    set.InsElem(6);
    set.InsElem(2);
    std::ostringstream stream;
    stream << set;

    EXPECT_EQ(stream.str(), "{2 6}");
}

TEST(Set, InputReplacesOldElements) {
    TSet set(8);
    set.InsElem(7);
    std::istringstream stream("{2 6}");
    stream >> set;

    EXPECT_FALSE(stream.fail());
    EXPECT_EQ(set.IsMember(2), 1);
    EXPECT_EQ(set.IsMember(6), 1);
    EXPECT_EQ(set.IsMember(7), 0);
}

TEST(Set, InputEmptySet) {
    TSet set(8);
    set.InsElem(2);
    std::istringstream stream("{}");
    stream >> set;

    EXPECT_FALSE(stream.fail());
    EXPECT_EQ(set.IsMember(2), 0);
}

TEST(Set, InputMissingClosingBrace) {
    TSet set(8);
    set.InsElem(7);
    TSet before(set);
    std::istringstream stream("{2 6");
    stream >> set;

    EXPECT_TRUE(stream.fail());
    EXPECT_EQ(set, before);
}

TEST(Set, InputElementOutsideUniverse) {
    TSet set(8);
    set.InsElem(7);
    TSet before(set);
    std::istringstream stream("{2 8}");
    stream >> set;

    EXPECT_TRUE(stream.fail());
    EXPECT_EQ(set, before);
}

TEST(Set, InputWithLetter) {
    TSet set(8);
    set.InsElem(7);
    TSet before(set);
    std::istringstream stream("{2 x}");
    stream >> set;

    EXPECT_TRUE(stream.fail());
    EXPECT_EQ(set, before);
}

TEST(Set, UnionOfDifferentUniverses) {
    TSet first(5);
    TSet second(8);
    first.InsElem(2);
    second.InsElem(6);

    EXPECT_THROW(first + second, std::invalid_argument);
    EXPECT_THROW(second + first, std::invalid_argument);
    EXPECT_EQ(first.GetMaxPower(), 5);
    EXPECT_EQ(second.GetMaxPower(), 8);
    EXPECT_EQ(first.IsMember(2), 1);
    EXPECT_EQ(second.IsMember(6), 1);
}

TEST(Set, IntersectionOfDifferentUniverses) {
    TSet first(5);
    TSet second(8);
    first.InsElem(2);
    second.InsElem(2);

    EXPECT_THROW(first * second, std::invalid_argument);
    EXPECT_THROW(second * first, std::invalid_argument);
    EXPECT_EQ(first.IsMember(2), 1);
    EXPECT_EQ(second.IsMember(2), 1);
}

TEST(Set, EmptySetsOfDifferentUniverses) {
    TSet first(5);
    TSet second(8);

    EXPECT_THROW(first + second, std::invalid_argument);
    EXPECT_THROW(first * second, std::invalid_argument);
}

TEST(Set, OperationsInEmptyUniverse) {
    TSet first(0);
    TSet second(0);
    TSet united = first + second;
    TSet common = first * second;

    EXPECT_EQ(united.GetMaxPower(), 0);
    EXPECT_EQ(common.GetMaxPower(), 0);
    EXPECT_EQ(united, first);
    EXPECT_EQ(common, first);
}
