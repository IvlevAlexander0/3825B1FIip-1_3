#include "gtest/gtest.h"
#include "pch.h"
#include <iostream>
#include <stdexcept>
#include <vector>
#include <cstdlib>
#include <ctime>

#include "TMatrix.h"


TEST(TestMatrix, Constructs) {
	EXPECT_ANY_THROW(TMatrix<int> a(-1));
    EXPECT_NO_THROW(TMatrix<int> a(5));

    TMatrix<int> m(4);
    EXPECT_EQ(4, m.GetSize());

    for (int i = 0; i < 4; i++) {
        EXPECT_EQ(i, m[i].GetStartIndex());
        EXPECT_EQ(4 - i, m[i].GetSize());
    }
}

TEST(TestMatrix, Index) {
    srand(time(nullptr));
    TMatrix<int> m(3);
    int r = rand() % 100;

    EXPECT_NO_THROW(m[0][0] = r);
    EXPECT_NO_THROW(m[1][2] = r);

    EXPECT_EQ(r, m[0][0]);
    EXPECT_EQ(r, m[1][2]);

    EXPECT_ANY_THROW(m[1][0] = r);
    EXPECT_ANY_THROW(m[2][1] = r);
}

TEST(TestMatrix, Copy) {
    srand(time(nullptr));
    TMatrix<int> m1(5);

    for (size_t i = 0; i < 5; ++i) {
        for (size_t j = i; j < 5; ++j) {
            m1[i][j] = rand() % 100 + 50;
        }
    }

    TMatrix<int> m2(m1);
    for (size_t i = 0; i < 5; ++i) {
        for (size_t j = i; j < 5; ++j) {
            EXPECT_EQ(m1[i][j], m2[i][j]);
        }
    }

    TMatrix<int> m3(5);
    m3 = m1;
    for (size_t i = 0; i < 5; ++i) {
        for (size_t j = i; j < 5; ++j) {
            EXPECT_EQ(m1[i][j], m3[i][j]);
        }
    }

    TMatrix<int> m4(7);
    EXPECT_ANY_THROW(m4 = m3);
}

TEST(TestMatrix, Operations) {
    srand(time(nullptr));
    TMatrix<int> m1(5);
    TMatrix<int> m2(5);

    std::vector<std::vector<int>> temp_plus(5, std::vector<int>(5, 0));
    std::vector<std::vector<int>> temp_minus(5, std::vector<int>(5, 0));

    for (size_t i = 0; i < 5; ++i) {
        for (size_t j = i; j < 5; ++j) {
            m1[i][j] = rand() % 1000;
            m2[i][j] = rand() % 1000;

            temp_plus[i][j] = m1[i][j] + m2[i][j];
            temp_minus[i][j] = m1[i][j] - m2[i][j];
        }
    }

    TMatrix<int> m_plus = m1 + m2;
    TMatrix<int> m_minus = m1 - m2;
    for (size_t i = 0; i < 5; ++i) {
        for (size_t j = i; j < 5; ++j) {
            EXPECT_EQ(m_plus[i][j], temp_plus[i][j]);
            EXPECT_EQ(m_minus[i][j], temp_minus[i][j]);
        }
    }

    TMatrix<int> m(10);
    EXPECT_ANY_THROW(m_plus = m + m1);
}

TEST(TestMatrix, EdgeCases) {
    TMatrix<int> m1(3);
    TMatrix<int> m2(3);
    TMatrix<int> m3(4);

    for (int i = 0; i < 3; i++) {
        for (int j = i; j < 3; j++) {
            m1[i][j] = i + j;
            m2[i][j] = i + j;
        }
    }

    EXPECT_TRUE(m1 == m2);
    EXPECT_FALSE(m1 != m2);
    EXPECT_TRUE(m1 != m3); 

    m2[0][2] = 999; 
    EXPECT_FALSE(m1 == m2);
    EXPECT_TRUE(m1 != m2);
    EXPECT_ANY_THROW(TMatrix<int> m(MAX_MATRIX_SIZE + 1));
    EXPECT_ANY_THROW(m1 - m3);

    TMatrix<int> zero(3);
    EXPECT_TRUE((m1 - m1) == zero);
    EXPECT_TRUE((m1 - zero) == m1);
}