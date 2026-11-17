#include <gtest/gtest.h>
#include "my_utmatrix.h"
#include <sstream>
#include <stdexcept>
#include <cstdlib>
#include <ctime>


// Тесты для TVector

TEST(TVectorTest, BasicConstructorAndAccess) {
    TVector<int> v(5, 1);
    EXPECT_EQ(v.GetSize(), 5);
    EXPECT_EQ(v.GetStartIndex(), 1);

    for (int i = 1; i <= 5; ++i) v[i] = i * 10;
    EXPECT_EQ(v[1], 10);
    EXPECT_EQ(v[5], 50);
}

TEST(TVectorTest, Exceptions) {
    TVector<int> v(5);
    EXPECT_THROW(v[-1], std::out_of_range);
    EXPECT_THROW(v[5], std::out_of_range);
    EXPECT_THROW(TVector<int>(-1), std::invalid_argument);
}

TEST(TVectorTest, VectorOperations) {
    TVector<int> v1(3); v1[0] = 1; v1[1] = 2; v1[2] = 3;
    TVector<int> v2(3); v2[0] = 4; v2[1] = 5; v2[2] = 6;

    TVector<int> v3 = v1 + v2;
    EXPECT_EQ(v3[0], 5);
    EXPECT_EQ(v3[2], 9);

    TVector<int> v4 = v1 * 2;
    EXPECT_EQ(v4[1], 4);

    int dot = v1 * v2;
    EXPECT_EQ(dot, 32);
}

TEST(TVectorTest, SizeMismatchThrows) {
    TVector<int> v1(3);
    TVector<int> v2(4);
    EXPECT_THROW(v1 + v2, std::invalid_argument);
    EXPECT_THROW(v1 * v2, std::invalid_argument);
}

//случанйые тесты
TEST(TVectorRandom, CommutativityAndAssociativity) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    for (int iter = 0; iter < 50; ++iter) {
        int size = 1 + std::rand() % 100;
        TVector<int> a(size), b(size), c(size);

        for (int i = 0; i < size; ++i) {
            a[i] = std::rand() % 200 - 100;
            b[i] = std::rand() % 200 - 100;
            c[i] = std::rand() % 200 - 100;
        }

        // коммутативность
        EXPECT_EQ(a + b, b + a);

        // ассоциативность
        EXPECT_EQ((a + b) + c, a + (b + c));

        // дистрибутивность
        int k = std::rand() % 10 - 5;
        EXPECT_EQ((a + b) * k, (a * k) + (b * k));
    }
}

//другие типы данных
TEST(TVectorTest, DoubleType) {
    TVector<double> v(3);
    v[0] = 1.5; v[1] = 2.5; v[2] = 3.5;
    TVector<double> v2 = v * 2.0;
    EXPECT_DOUBLE_EQ(v2[1], 5.0);
}

// самоприсваивание
TEST(TVectorTest, SelfAssignment) {
    TVector<int> v(3);
    v[0] = 10; v[1] = 20; v[2] = 30;
    v = v;
    EXPECT_EQ(v[0], 10);
    EXPECT_EQ(v[2], 30);
}

TEST(TVectorTest, ZeroSize) {
    TVector<int> v(0);
    EXPECT_EQ(v.GetSize(), 0);
    TVector<int> sum = v + v;
    EXPECT_EQ(sum.GetSize(), 0);
    EXPECT_EQ(v * v, 0);
}


// присваивание с разными размерами
TEST(TVectorTest, AssignmentDifferentSizes) {
    TVector<int> v1(5);
    v1[0] = 1;
    TVector<int> v2(2);
    v2[1] = 2;

    v1 = v2; // v1 теперь размера 2
    EXPECT_EQ(v1.GetSize(), 2);
    EXPECT_EQ(v1[1], 2);
}

// несовпадение StartIndex
TEST(TVectorTest, StartIndexMismatchThrows) {
    TVector<int> v1(3, 0);
    TVector<int> v2(3, 1);
    EXPECT_THROW(v1 + v2, std::invalid_argument);
    EXPECT_THROW(v1 * v2, std::invalid_argument);
}

// ввод/вывод
TEST(VectorIO, OutputFormat) {
    TVector<int> v(3);
    v[0] = 1; v[1] = 2; v[2] = 3;

    std::ostringstream out;
    out << v;
    EXPECT_EQ(out.str(), "1 2 3 ");
}

TEST(VectorIO, InputFormat) {
    TVector<int> v(3);
    std::istringstream in("4 5 6");
    in >> v;

    EXPECT_EQ(v[0], 4);
    EXPECT_EQ(v[1], 5);
    EXPECT_EQ(v[2], 6);
}


//Тесты для TMatrix

TEST(TMatrixTest, ConstructorAndMemorySaving) {
    TMatrix<int> mt(3);
    EXPECT_EQ(mt.GetSize(), 3);

    EXPECT_EQ(mt[0].GetSize(), 3);
    EXPECT_EQ(mt[1].GetSize(), 2);
    EXPECT_EQ(mt[2].GetSize(), 1);
    EXPECT_EQ(mt[1].GetStartIndex(), 1);
}

TEST(TMatrixTest, AccessAndAssignment) {
    TMatrix<int> mt(3);
    mt[0][0] = 1; mt[0][1] = 2; mt[0][2] = 3;
    mt[1][1] = 5; mt[1][2] = 6;
    mt[2][2] = 9;

    EXPECT_EQ(mt[0][0], 1);
    EXPECT_EQ(mt[1][2], 6);
    EXPECT_EQ(mt[2][2], 9);

    EXPECT_THROW(mt[1][0], std::out_of_range);
}

TEST(TMatrixTest, MatrixOperations) {
    TMatrix<int> m1(2), m2(2);
    m1[0][0] = 1; m1[0][1] = 2; m1[1][1] = 3;
    m2[0][0] = 4; m2[0][1] = 5; m2[1][1] = 6;

    // cложение матриц
    TMatrix<int> sum = m1 + m2;
    EXPECT_EQ(sum[0][0], 5);
    EXPECT_EQ(sum[0][1], 7);
    EXPECT_EQ(sum[1][1], 9);

    // вычитание матриц
    TMatrix<int> diff = m2 - m1;
    EXPECT_EQ(diff[0][0], 3);
    EXPECT_EQ(diff[1][1], 3);
}

TEST(TMatrixTest, CopyAndCompare) {
    TMatrix<int> m1(2);
    m1[0][0] = 1; m1[1][1] = 2;

    TMatrix<int> m2 = m1;
    EXPECT_EQ(m1, m2);

    m2[0][0] = 99;
    EXPECT_NE(m1, m2);
}

//случайные тесты
TEST(TMatrixRandom, AdditionAndSubtractionProperties) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    for (int iter = 0; iter < 50; ++iter) {
        int size = 1 + std::rand() % 20;
        TMatrix<int> m1(size), m2(size);

        for (int i = 0; i < size; ++i) {
            for (int j = i; j < size; ++j) {
                m1[i][j] = std::rand() % 100;
                m2[i][j] = std::rand() % 100;
            }
        }

        // коммутативность
        EXPECT_EQ(m1 + m2, m2 + m1);

        // свойство нуля
        TMatrix<int> zero(size);
        EXPECT_EQ(m1 + zero, m1);

        // вычитание
        EXPECT_EQ((m1 + m2) - m2, m1);
    }
}

//другие типы данных
TEST(TMatrixTest, DoubleType) {
    TMatrix<double> mt(2);
    mt[0][0] = 1.1; mt[1][1] = 2.2;
    TMatrix<double> sum = mt + mt;
    EXPECT_DOUBLE_EQ(sum[0][0], 2.2);
    EXPECT_DOUBLE_EQ(sum[1][1], 4.4);
}

// самоприсваивание
TEST(TMatrixTest, SelfAssignment) {
    TMatrix<int> mt(2);
    mt[0][0] = 5;
    mt = mt;
    EXPECT_EQ(mt[0][0], 5);
}

// тест на констуктор преобразования типов
TEST(TMatrixTest, ConvertFromVector) {
    TVector<TVector<int>> rows(2);
    TVector<int> r0(2, 0); r0[0] = 1; r0[1] = 2;
    TVector<int> r1(1, 1); r1[1] = 3;
    rows[0] = r0;
    rows[1] = r1;

    TMatrix<int> mt(rows);
    EXPECT_EQ(mt[0][0], 1);
    EXPECT_EQ(mt[1][1], 3);
}

// копирование матрицы
TEST(TMatrixTest, DeepCopy) {
    TMatrix<int> m1(2);
    m1[0][0] = 1; m1[1][1] = 2;

    TMatrix<int> m2 = m1;
    m2[0][0] = 99;

    EXPECT_EQ(m1[0][0], 1);
}

//ввыод\вывод
TEST(MatrixIO, OutputFormat) {
    TMatrix<int> mt(2);
    mt[0][0] = 1; mt[0][1] = 2;
    mt[1][1] = 3;

    std::ostringstream out;
    out << mt;

    EXPECT_EQ(out.str(), "1 2 \n3 \n");
}

TEST(MatrixIO, InputFormat) {
    TMatrix<int> mt(2);
    std::istringstream in("10 20 30");
    in >> mt;

    EXPECT_EQ(mt[0][0], 10);
    EXPECT_EQ(mt[0][1], 20);
    EXPECT_EQ(mt[1][1], 30);

    EXPECT_EQ(mt.GetSize(), 2);
    EXPECT_EQ(mt[0].GetSize(), 2);
    EXPECT_EQ(mt[1].GetSize(), 1);
}