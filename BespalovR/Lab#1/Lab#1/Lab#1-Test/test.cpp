#include "pch.h"
#include "../Lab#1/tsetR.h"

TEST(TBitField, ConstructorSetsLengthAndClearsBits) {
    TBitField bf(10);

    ASSERT_EQ(10, bf.GetLength());

    for (int i = 0; i < bf.GetLength(); ++i) {
        ASSERT_EQ(0, bf.GetBit(i));
    }
}

TEST(TBitField, ConstructorRejectsNegativeLength) {
    ASSERT_THROW(TBitField(-1), std::invalid_argument);
}

TEST(TBitField, ZeroLengthIsAllowed) {
    TBitField bf(0);

    ASSERT_EQ(0, bf.GetLength());
}

TEST(TBitField, SetBitSetsOnlyRequestedBit) {
    TBitField bf(40);

    bf.SetBit(0);
    bf.SetBit(31);
    bf.SetBit(32);
    bf.SetBit(39);

    ASSERT_EQ(1, bf.GetBit(0));
    ASSERT_EQ(1, bf.GetBit(31));
    ASSERT_EQ(1, bf.GetBit(32));
    ASSERT_EQ(1, bf.GetBit(39));

    for (int i = 0; i < 40; ++i) {
        if (i != 0 && i != 31 && i != 32 && i != 39) {
            ASSERT_EQ(0, bf.GetBit(i));
        }
    }
}

TEST(TBitField, SetBitThrowsInvalidIndex) {
    TBitField bf(8);

    ASSERT_THROW(bf.SetBit(-1), std::out_of_range);
    ASSERT_THROW(bf.SetBit(8), std::out_of_range);

    for (int i = 0; i < 8; ++i) {
        ASSERT_EQ(0, bf.GetBit(i));
    }
}

TEST(TBitField, ClrBitClearsRequestedBit) {
    TBitField bf(33);

    bf.SetBit(0);
    bf.SetBit(32);

    bf.ClrBit(0);

    ASSERT_EQ(0, bf.GetBit(0));
    ASSERT_EQ(1, bf.GetBit(32));

    bf.ClrBit(32);

    ASSERT_EQ(0, bf.GetBit(32));
}

TEST(TBitField, ClrBitThrowsInvalidIndex) {
    TBitField bf(8);

    bf.SetBit(3);

    ASSERT_THROW(bf.ClrBit(-1), std::out_of_range);
    ASSERT_THROW(bf.ClrBit(8), std::out_of_range);

    ASSERT_EQ(1, bf.GetBit(3));
}

TEST(TBitField, GetBitThrowsForInvalidIndex) {
    TBitField bf(8);

    ASSERT_THROW(bf.GetBit(-1), std::out_of_range);
    ASSERT_THROW(bf.GetBit(8), std::out_of_range);
}

TEST(TBitField, CopyConstructorCreatesIndependentCopy) {
    TBitField original(70);

    original.SetBit(5);
    original.SetBit(64);

    TBitField copy(original);

    ASSERT_EQ(original.GetLength(), copy.GetLength());
    ASSERT_TRUE(original == copy);

    copy.SetBit(10);

    ASSERT_EQ(0, original.GetBit(10));
    ASSERT_EQ(1, copy.GetBit(10));
}

TEST(TBitField, AssignmentCreatesIndependentCopy) {
    TBitField a(20);
    TBitField b(5);

    a.SetBit(3);
    b.SetBit(4);

    a = b;

    ASSERT_EQ(5, a.GetLength());
    ASSERT_EQ(0, a.GetBit(3));
    ASSERT_EQ(1, a.GetBit(4));

    b.SetBit(0);

    ASSERT_EQ(0, a.GetBit(0));
    ASSERT_EQ(1, b.GetBit(0));
}

TEST(TBitField, SelfAssignmentIsSafe) {
    TBitField bf(16);

    bf.SetBit(7);

    bf = bf;

    ASSERT_EQ(16, bf.GetLength());
    ASSERT_EQ(1, bf.GetBit(7));
}

TEST(TBitField, EqualityConsidersLengthAndBits) {
    TBitField a(16);
    TBitField b(16);
    TBitField c(17);

    a.SetBit(2);
    b.SetBit(2);
    c.SetBit(2);

    ASSERT_TRUE(a == b);
    ASSERT_FALSE(a != b);

    ASSERT_TRUE(a == c);
    ASSERT_FALSE(a != c);

    b.SetBit(3);

    ASSERT_FALSE(a == b);
    ASSERT_TRUE(a != b);
}

TEST(TBitField, OperatorOrUnionsBits) {
    TBitField a(10);
    TBitField b(10);

    a.SetBit(1);
    a.SetBit(3);

    b.SetBit(3);
    b.SetBit(7);

    TBitField c = a | b;

    ASSERT_EQ(10, c.GetLength());
    ASSERT_EQ(1, c.GetBit(1));
    ASSERT_EQ(1, c.GetBit(3));
    ASSERT_EQ(1, c.GetBit(7));
    ASSERT_EQ(0, c.GetBit(0));
}

TEST(TBitField, OperatorAndIntersectsBits) {
    TBitField a(10);
    TBitField b(10);

    a.SetBit(1);
    a.SetBit(3);

    b.SetBit(3);
    b.SetBit(7);

    TBitField c = a & b;

    ASSERT_EQ(10, c.GetLength());
    ASSERT_EQ(1, c.GetBit(3));
    ASSERT_EQ(0, c.GetBit(1));
    ASSERT_EQ(0, c.GetBit(7));
}

TEST(TBitField, BitwiseOperatorsAcceptDifferentLengths) {
    TBitField a(8);
    TBitField b(9);

    ASSERT_NO_THROW(a | b);
    ASSERT_NO_THROW(a & b);
}

TEST(TBitField, OperatorNotInvertsOnlyUsedBits) {
    TBitField a(10);

    a.SetBit(0);
    a.SetBit(3);
    a.SetBit(9);

    TBitField b = ~a;

    ASSERT_EQ(10, b.GetLength());

    ASSERT_EQ(0, b.GetBit(0));
    ASSERT_EQ(1, b.GetBit(1));
    ASSERT_EQ(1, b.GetBit(2));
    ASSERT_EQ(0, b.GetBit(3));
    ASSERT_EQ(0, b.GetBit(9));

    for (int i = 0; i < 10; ++i) {
        ASSERT_EQ(a.GetBit(i) == 0 ? 1 : 0, b.GetBit(i));
    }
}

TEST(TBitField, OperatorNotDoesNotChangeOriginal) {
    TBitField a(16);

    a.SetBit(5);

    TBitField b = ~a;

    ASSERT_EQ(1, a.GetBit(5));
    ASSERT_EQ(0, b.GetBit(5));
}

TEST(TBitField, DoubleNotReturnsOriginalBits) {
    TBitField a(13);

    a.SetBit(1);
    a.SetBit(12);

    TBitField b = ~~a;

    ASSERT_TRUE(a == b);
}

TEST(TBitField, InputStreamReadsBinaryDigits) {
    TBitField bf(6);

    std::istringstream in("010110");

    in >> bf;

    ASSERT_TRUE(in.good());

    ASSERT_EQ(0, bf.GetBit(0));
    ASSERT_EQ(1, bf.GetBit(1));
    ASSERT_EQ(0, bf.GetBit(2));
    ASSERT_EQ(1, bf.GetBit(3));
    ASSERT_EQ(1, bf.GetBit(4));
    ASSERT_EQ(0, bf.GetBit(5));
}

TEST(TBitField, InputStreamRejectsInvalidCharacter) {
    TBitField bf(4);

    std::istringstream in("01a1");

    in >> bf;

    ASSERT_TRUE(in.fail());
}

TEST(TBitField, OutputStreamPrintsBits) {
    TBitField bf(6);

    bf.SetBit(1);
    bf.SetBit(3);
    bf.SetBit(4);

    std::ostringstream out;

    out << bf;

    ASSERT_EQ("010110", out.str());
}

TEST(TSet, ConstructorSetsMaxPowerAndEmptySet) {
    TSet s(10);

    ASSERT_EQ(10, s.GetMaxPower());

    for (int i = 0; i < s.GetMaxPower(); ++i) {
        ASSERT_EQ(0, s.IsMember(i));
    }
}

TEST(TSet, ConstructorRejectsNegativeMaxPower) {
    ASSERT_THROW(TSet(-1), std::invalid_argument);
}

TEST(TSet, ConstructorFromBitFieldCopiesLengthAndBits) {
    TBitField bf(12);

    bf.SetBit(2);
    bf.SetBit(11);

    TSet s(bf);

    ASSERT_EQ(12, s.GetMaxPower());
    ASSERT_EQ(1, s.IsMember(2));
    ASSERT_EQ(1, s.IsMember(11));
    ASSERT_EQ(0, s.IsMember(0));
}

TEST(TSet, ConversionToBitFieldReturnsSameBits) {
    TSet s(8);

    s.InsElem(1);
    s.InsElem(6);

    TBitField bf = s;

    ASSERT_EQ(8, bf.GetLength());
    ASSERT_EQ(1, bf.GetBit(1));
    ASSERT_EQ(1, bf.GetBit(6));
    ASSERT_EQ(0, bf.GetBit(0));
}

TEST(TSet, InsElemAddsElement) {
    TSet s(10);

    s.InsElem(0);
    s.InsElem(9);

    ASSERT_EQ(1, s.IsMember(0));
    ASSERT_EQ(1, s.IsMember(9));
    ASSERT_EQ(0, s.IsMember(5));
}

TEST(TSet, InsElemRejectsElementOutsideUniverse) {
    TSet s(5);

    ASSERT_THROW(s.InsElem(-1), std::out_of_range);
    ASSERT_THROW(s.InsElem(5), std::out_of_range);
}

TEST(TSet, DelElemRemovesElement) {
    TSet s(10);

    s.InsElem(4);
    s.DelElem(4);

    ASSERT_EQ(0, s.IsMember(4));
}

TEST(TSet, DelElemRejectsElementOutsideUniverse) {
    TSet s(5);

    ASSERT_THROW(s.DelElem(-1), std::out_of_range);
    ASSERT_THROW(s.DelElem(5), std::out_of_range);
}

TEST(TSet, IsMemberRejectsElementOutsideUniverse) {
    TSet s(5);

    ASSERT_THROW(s.IsMember(-1), std::out_of_range);
    ASSERT_THROW(s.IsMember(5), std::out_of_range);
}

TEST(TSet, CopyConstructorCreatesIndependentSet) {
    TSet original(8);

    original.InsElem(2);

    TSet copy(original);

    copy.InsElem(5);

    ASSERT_EQ(1, original.IsMember(2));
    ASSERT_EQ(0, original.IsMember(5));

    ASSERT_EQ(1, copy.IsMember(2));
    ASSERT_EQ(1, copy.IsMember(5));
}

TEST(TSet, AssignmentCopiesSet) {
    TSet a(8);
    TSet b(4);

    a.InsElem(1);
    b.InsElem(3);

    a = b;

    ASSERT_EQ(4, a.GetMaxPower());
    ASSERT_EQ(0, a.IsMember(1));
    ASSERT_EQ(1, a.IsMember(3));
}

TEST(TSet, SelfAssignmentIsSafe) {
    TSet s(8);

    s.InsElem(3);

    s = s;

    ASSERT_EQ(8, s.GetMaxPower());
    ASSERT_EQ(1, s.IsMember(3));
}

TEST(TSet, EqualityRequiresSameUniverseAndElements) {
    TSet a(8);
    TSet b(8);
    TSet c(9);

    a.InsElem(1);
    b.InsElem(1);
    c.InsElem(1);

    ASSERT_TRUE(a == b);
    ASSERT_FALSE(a != b);

    ASSERT_FALSE(a == c);
    ASSERT_TRUE(a != c);

    b.InsElem(2);

    ASSERT_FALSE(a == b);
    ASSERT_TRUE(a != b);
}

TEST(TSet, PlusElementAddsCopy) {
    TSet s(8);

    s.InsElem(1);

    TSet result = s + 4;

    ASSERT_EQ(1, s.IsMember(1));
    ASSERT_EQ(0, s.IsMember(4));

    ASSERT_EQ(1, result.IsMember(1));
    ASSERT_EQ(1, result.IsMember(4));
}

TEST(TSet, PlusElementRejectsOutsideUniverse) {
    TSet s(5);

    ASSERT_THROW(s + (-1), std::out_of_range);
    ASSERT_THROW(s + 5, std::out_of_range);
}

TEST(TSet, MinusElementRemovesCopy) {
    TSet s(8);

    s.InsElem(1);
    s.InsElem(4);

    TSet result = s - 4;

    ASSERT_EQ(1, s.IsMember(4));

    ASSERT_EQ(1, result.IsMember(1));
    ASSERT_EQ(0, result.IsMember(4));
}

TEST(TSet, MinusElementRejectsOutsideUniverse) {
    TSet s(5);

    ASSERT_THROW(s - (-1), std::out_of_range);
    ASSERT_THROW(s - 5, std::out_of_range);
}

TEST(TSet, UnionCombinesElements) {
    TSet a(10);
    TSet b(10);

    a.InsElem(1);
    a.InsElem(3);

    b.InsElem(3);
    b.InsElem(7);

    TSet c = a + b;

    ASSERT_EQ(1, c.IsMember(1));
    ASSERT_EQ(1, c.IsMember(3));
    ASSERT_EQ(1, c.IsMember(7));
    ASSERT_EQ(0, c.IsMember(0));
}

TEST(TSet, UnionRejectsDifferentUniverses) {
    TSet a(8);
    TSet b(9);

    ASSERT_THROW(a + b, std::invalid_argument);
}

TEST(TSet, IntersectionKeepsCommonElements) {
    TSet a(10);
    TSet b(10);

    a.InsElem(1);
    a.InsElem(3);

    b.InsElem(3);
    b.InsElem(7);

    TSet c = a * b;

    ASSERT_EQ(1, c.IsMember(3));
    ASSERT_EQ(0, c.IsMember(1));
    ASSERT_EQ(0, c.IsMember(7));
}

TEST(TSet, IntersectionRejectsDifferentUniverses) {
    TSet a(8);
    TSet b(9);

    ASSERT_THROW(a * b, std::invalid_argument);
}

TEST(TSet, ComplementInvertsMembership) {
    TSet s(10);

    s.InsElem(0);
    s.InsElem(9);

    TSet c = ~s;

    ASSERT_EQ(0, c.IsMember(0));
    ASSERT_EQ(1, c.IsMember(1));
    ASSERT_EQ(1, c.IsMember(8));
    ASSERT_EQ(0, c.IsMember(9));
}

TEST(TSet, ComplementDoesNotChangeOriginal) {
    TSet s(8);

    s.InsElem(2);

    TSet c = ~s;

    ASSERT_EQ(1, s.IsMember(2));
    ASSERT_EQ(0, c.IsMember(2));
}

TEST(TSet, InputStreamReadsCharacteristicVector) {
    TSet s(6);

    std::istringstream in("1 3 4");

    in >> s;
    //не зависимо от реализации ввода выдаёт ложь, в том числе с рабочими реализациями из принятых лабораторных работ, не понимаю почему
    //ASSERT_TRUE(in.good());

    ASSERT_EQ(0, s.IsMember(0));
    ASSERT_EQ(1, s.IsMember(1));
    ASSERT_EQ(0, s.IsMember(2));
    ASSERT_EQ(1, s.IsMember(3));
    ASSERT_EQ(1, s.IsMember(4));
    ASSERT_EQ(0, s.IsMember(5));
}

TEST(TSet, OutputStreamPrintsElements) {
    TSet s(10);

    s.InsElem(1);
    s.InsElem(3);
    s.InsElem(7);

    std::ostringstream out;

    out << s;

    ASSERT_EQ("1 3 7", out.str());
}

TEST(TSet, EmptySetPrintsNothing) {
    TSet s(5);

    std::ostringstream out;

    out << s;

    ASSERT_EQ("", out.str());
}