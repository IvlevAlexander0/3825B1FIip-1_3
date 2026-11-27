#include "pch.h"
TEST(Set, Create) {
    ASSERT_NO_THROW(TSet s(10));
}
TEST(Set, InsertAndCheck) {
    TSet s(10);
    s.InsElem(5);
    EXPECT_EQ(1, s.IsMember(5));
    EXPECT_EQ(0, s.IsMember(4));
}
TEST(Set, Delete) {
    TSet s(10);
    s.InsElem(5);
    s.DelElem(5);
    EXPECT_EQ(0, s.IsMember(5));
}
TEST(Set, CopyConstructor) {
    TSet s(10);
    s.InsElem(3);
    TSet copy(s);
    EXPECT_EQ(1, copy.IsMember(3));
    EXPECT_EQ(1, s == copy);
}
TEST(Set, Assignment) {
    TSet s1(10);
    TSet s2(10);
    s1.InsElem(7);
    s2 = s1;
    EXPECT_EQ(1, s2.IsMember(7));
    EXPECT_EQ(1, s1 == s2);
}
TEST(Set, Compare) {
    TSet s1(10);
    TSet s2(10);
    EXPECT_EQ(1, s1 == s2);
    s1.InsElem(1);
    EXPECT_EQ(1, s1 != s2);
}
TEST(Set, Union) {
    TSet s1(10);
    TSet s2(10);
    s1.InsElem(2);
    s2.InsElem(5);
    TSet result = s1 + s2;
    EXPECT_EQ(1, result.IsMember(2));
    EXPECT_EQ(1, result.IsMember(5));
}
TEST(Set, Intersection) {
    TSet s1(10);
    TSet s2(10);
    s1.InsElem(3);
    s1.InsElem(5);
    s2.InsElem(5);
    s2.InsElem(7);
    TSet result = s1 * s2;
    EXPECT_EQ(0, result.IsMember(3));
    EXPECT_EQ(1, result.IsMember(5));
    EXPECT_EQ(0, result.IsMember(7));
}
TEST(Set, AddElement) {
    TSet s(10);
    TSet result = s + 4;
    EXPECT_EQ(1, result.IsMember(4));
}
TEST(Set, DeleteElement) {
    TSet s(10);
    s.InsElem(4);
    TSet result = s - 4;
    EXPECT_EQ(0, result.IsMember(4));
}
TEST(Set, NotOperation) {
    TSet s(5);
    s.InsElem(2);
    TSet result = ~s;
    EXPECT_EQ(1, result.IsMember(0));
    EXPECT_EQ(1, result.IsMember(1));
    EXPECT_EQ(0, result.IsMember(2));
}
TEST(Set, RandomOperations) {
    srand((unsigned)time(nullptr));
    for (int k = 0; k < 100; k++) {
        int size = rand() % 100 + 1;
        TSet a(size);
        TSet b(size);
        std::vector<int> va(size, 0);
        std::vector<int> vb(size, 0);
        for (int i = 0; i < size; i++) {
            if (rand() % 2) {
                a.InsElem(i);
                va[i] = 1;
            }
            if (rand() % 2) {
                b.InsElem(i);
                vb[i] = 1;
            }
        }
        TSet result = a + b;
        for (int i = 0; i < size; i++) {
            EXPECT_EQ(va[i] || vb[i], result.IsMember(i));
        }
    }
}
TEST(Set, RandomIntersection) {
    srand((unsigned)time(nullptr));
    for (int k = 0; k < 100; k++) {
        int size = rand() % 100 + 1;
        TSet a(size);
        TSet b(size);
        std::vector<int> expected(size, 0);
        for (int i = 0; i < size; i++) {
            if (rand() % 2) {
                a.InsElem(i);
            }
            if (rand() % 2) {
                b.InsElem(i);
            }
            expected[i] = a.IsMember(i) && b.IsMember(i);
        }
        TSet result = a * b;
        for (int i = 0; i < size; i++) {
            EXPECT_EQ(expected[i], result.IsMember(i));
        }
    }
}