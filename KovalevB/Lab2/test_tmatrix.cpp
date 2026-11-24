#include <gtest/gtest.h>
#include "utmatrix.h"

TEST(TMatrix, can_create_matrix_with_positive_length)
{
    ASSERT_NO_THROW(TMatrix<int> m(5));
}

TEST(TMatrix, can_create_zero_size_matrix)
{
    ASSERT_NO_THROW(TMatrix<int> m(0));
    TMatrix<int> m(0);
    EXPECT_EQ(0, m.GetSize());
}

TEST(TMatrix, cant_create_too_large_matrix)
{
    ASSERT_ANY_THROW(TMatrix<int> m(MAX_MATRIX_SIZE + 1));
}

TEST(TMatrix, throws_when_create_matrix_with_negative_length)
{
    ASSERT_ANY_THROW(TMatrix<int> m(-5));
}

TEST(TMatrix, can_create_copied_matrix)
{
    TMatrix<int> m(5);
    ASSERT_NO_THROW(TMatrix<int> m1(m));
}

TEST(TMatrix, copied_matrix_is_equal_to_source_one)
{
    TMatrix<int> m(4);
    m[0][0] = 1;
    m[1][2] = 5;
    m[3][3] = 9;
    TMatrix<int> mCopy(m);
    EXPECT_EQ(m, mCopy);
}

TEST(TMatrix, copied_matrix_has_its_own_memory)
{
    TMatrix<int> m(4);
    m[0][0] = 10;
    TMatrix<int> mCopy(m);
    mCopy[0][0] = 99;
    EXPECT_EQ(10, m[0][0]);
    EXPECT_EQ(99, mCopy[0][0]);
}

TEST(TMatrix, can_get_size)
{
    TMatrix<int> m(6);
    EXPECT_EQ(6, m.GetSize());
}

TEST(TMatrix, rows_have_correct_upper_triangular_start_indices)
{
    TMatrix<int> m(4);
    for (int i = 0; i < 4; ++i) {
        EXPECT_EQ(4, m[i].GetSize());
        EXPECT_EQ(i, m[i].GetStartIndex());
    }
}

TEST(TMatrix, can_set_and_get_element)
{
    TMatrix<int> m(4);
    m[1][2] = 42;
    m[3][3] = -7;
    EXPECT_EQ(42, m[1][2]);
    EXPECT_EQ(-7, m[3][3]);
}

TEST(TMatrix, throws_when_set_element_with_negative_index)
{
    TMatrix<int> m(4);
    ASSERT_ANY_THROW(m[-1][0] = 1);
    ASSERT_ANY_THROW(m[0][-1] = 1);
}

TEST(TMatrix, throws_when_set_element_below_main_diagonal)
{
    TMatrix<int> m(4);
    ASSERT_ANY_THROW(m[2][1] = 10);
    ASSERT_ANY_THROW(m[3][0] = 10);
}

TEST(TMatrix, throws_when_set_element_with_too_large_index)
{
    TMatrix<int> m(4);
    ASSERT_ANY_THROW(m[4][0] = 1);
    ASSERT_ANY_THROW(m[0][4] = 1);
}

TEST(TMatrix, can_assign_matrix_to_itself)
{
    TMatrix<int> m(3);
    m[0][1] = 11;
    ASSERT_NO_THROW(m = m);
    EXPECT_EQ(11, m[0][1]);
}

TEST(TMatrix, can_assign_matrices_of_equal_size)
{
    TMatrix<int> m1(3), m2(3);
    m1[0][0] = 1;
    m1[1][2] = 8;
    m2 = m1;
    EXPECT_EQ(m1, m2);
}

TEST(TMatrix, assign_operator_change_matrix_size)
{
    TMatrix<int> m1(5), m2(2);
    m2 = m1;
    EXPECT_EQ(5, m2.GetSize());
}

TEST(TMatrix, can_assign_matrices_of_different_size)
{
    TMatrix<int> m1(4), m2(2);
    m1[0][3] = 17;
    m1[2][2] = 23;
    m2 = m1;
    EXPECT_EQ(m1, m2);
}

TEST(TMatrix, compare_equal_matrices_return_true)
{
    TMatrix<int> m1(3), m2(3);
    m1[0][2] = 5;
    m2[0][2] = 5;
    EXPECT_TRUE(m1 == m2);
    EXPECT_FALSE(m1 != m2);
}

TEST(TMatrix, compare_matrix_with_itself_return_true)
{
    TMatrix<int> m(3);
    m[1][1] = 12;
    EXPECT_TRUE(m == m);
}

TEST(TMatrix, matrices_with_different_size_are_not_equal)
{
    TMatrix<int> m1(3), m2(4);
    EXPECT_FALSE(m1 == m2);
    EXPECT_TRUE(m1 != m2);
}

TEST(TMatrix, can_add_matrices_with_equal_size)
{
    TMatrix<int> m1(3), m2(3), expected(3);
    m1[0][0] = 1; m1[0][1] = 2; m1[1][1] = 3;
    m2[0][0] = 4; m2[0][1] = 5; m2[1][1] = 6;
    expected[0][0] = 5; expected[0][1] = 7; expected[1][1] = 9;

    TMatrix<int> res = m1 + m2;
    EXPECT_EQ(expected, res);
}

TEST(TMatrix, cant_add_matrices_with_not_equal_size)
{
    TMatrix<int> m1(3), m2(4);
    ASSERT_ANY_THROW(m1 + m2);
}

TEST(TMatrix, can_subtract_matrices_with_equal_size)
{
    TMatrix<int> m1(3), m2(3), expected(3);
    m1[0][0] = 10; m1[0][2] = 8; m1[2][2] = 6;
    m2[0][0] = 3;  m2[0][2] = 2; m2[2][2] = 1;
    expected[0][0] = 7; expected[0][2] = 6; expected[2][2] = 5;

    TMatrix<int> res = m1 - m2;
    EXPECT_EQ(expected, res);
}

TEST(TMatrix, cant_subtract_matrices_with_not_equal_size)
{
    TMatrix<int> m1(3), m2(4);
    ASSERT_ANY_THROW(m1 - m2);
}

TEST(TMatrix, throws_when_converting_invalid_vector_of_vectors_to_matrix)
{
    TVector<TVector<int>> badVec(3, 0);
    badVec[0] = TVector<int>(3, 0);
    badVec[1] = TVector<int>(3, 0);
    badVec[2] = TVector<int>(3, 2);
    ASSERT_ANY_THROW(TMatrix<int> m(badVec));
}