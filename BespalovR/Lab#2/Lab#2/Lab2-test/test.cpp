#include "pch.h"
#include "../Lab#2/RMatrix.h"
namespace {

    void ensureSeeded() {
        static bool seeded = false;
        if (!seeded) {
            srand(static_cast<unsigned>(time(NULL)));
            seeded = true;
        }
    }

    int randInt(int lo, int hi) {
        ensureSeeded();
        return lo + rand() % (hi - lo + 1);
    }

    double randDouble(double lo, double hi) {
        ensureSeeded();
        return lo + (hi - lo) * (rand() / (double)RAND_MAX);
    }

    TVector<int> makeRandomVector(int size, int startIndex, int lo = -100, int hi = 100) {
        TVector<int> v(size, startIndex);
        for (int i = startIndex; i < size; ++i) {
            v[i] = randInt(lo, hi);
        }
        return v;
    }

    TMatrix<int> makeRandomUpperMatrix(int n, int lo = -100, int hi = 100) {
        TVector<TVector<int>> rows(n);
        for (int i = 0; i < n; ++i) {
            TVector<int> row(n, i);
            for (int j = i; j < n; ++j) {
                row[j] = randInt(lo, hi);
            }
            rows[i] = row;
        }
        return TMatrix<int>(rows);
    }

    TMatrix<int> makeZeroUpperMatrix(int n) {
        TVector<TVector<int>> rows(n);
        for (int i = 0; i < n; ++i) {
            rows[i] = TVector<int>(n, i);
        }
        return TMatrix<int>(rows);
    }

}



TEST(TVectorTest, DefaultConstructor) {
    TVector<int> v;
    ASSERT_EQ(10, v.GetSize());
    ASSERT_EQ(0, v.GetStartIndex());
    for (int i = 0; i < 10; ++i) {
        ASSERT_EQ(0, v[i]);
    }
}


TEST(TVectorTest, ConstructorWithSizeAndStartIndex) {
    TVector<int> v(5, 2);

    ASSERT_EQ(5, v.GetSize());
    ASSERT_EQ(2, v.GetStartIndex());

    ASSERT_THROW(v[0], std::out_of_range);
    ASSERT_THROW(v[1], std::out_of_range);
    ASSERT_THROW(v[5], std::out_of_range);

    v[2] = 7;
    v[3] = 8;
    v[4] = 9;

    ASSERT_EQ(7, v[2]);
    ASSERT_EQ(8, v[3]);
    ASSERT_EQ(9, v[4]);
}


TEST(TVectorTest, InvalidConstructorParams) {
    ASSERT_THROW(TVector<int>(-1), std::out_of_range);
    ASSERT_THROW(TVector<int>(5, -1), std::out_of_range);
    ASSERT_THROW(TVector<int>(5, 6), std::out_of_range);
}


TEST(TVectorTest, StartIndexEqualsSize) {
    TVector<int> v(5, 5);

    ASSERT_EQ(5, v.GetSize());
    ASSERT_EQ(5, v.GetStartIndex());

    for (int i = 0; i < 5; ++i) {
        ASSERT_THROW(v[i], std::out_of_range);
    }
}


TEST(TVectorTest, AssignmentOperator) {
    TVector<int> a(5, 2);
    a[2] = 1;
    a[3] = 2;
    a[4] = 3;

    TVector<int> b(3, 0);
    b = a;
    ASSERT_TRUE(b == a);

    b[2] = 99;
    ASSERT_TRUE(b != a);
    ASSERT_EQ(1, a[2]);
}


TEST(TVectorTest, VectorAndDotSizeMismatchThrows) {
    TVector<int> a(4, 0);

    ASSERT_THROW(a + TVector<int>(5, 0), std::invalid_argument);
    ASSERT_THROW(a - TVector<int>(5, 0), std::invalid_argument);
    ASSERT_THROW(a + TVector<int>(4, 1), std::invalid_argument);
    ASSERT_THROW(a - TVector<int>(4, 1), std::invalid_argument);
    ASSERT_THROW(a * TVector<int>(3, 0), std::invalid_argument);
    ASSERT_THROW(a * TVector<int>(4, 1), std::invalid_argument);
}



TEST(TMatrixTest, ConstructorCreatesUpperTriangularRows) {
    TMatrix<int> m(4);

    ASSERT_EQ(4, m.GetSize());

    for (int i = 0; i < 4; ++i) {
        ASSERT_EQ(4, m[i].GetSize());
        ASSERT_EQ(i, m[i].GetStartIndex());

        for (int j = 0; j < i; ++j) {
            ASSERT_THROW(m[i][j], std::out_of_range);
        }
        for (int j = i; j < 4; ++j) {
            ASSERT_EQ(0, m[i][j]);
        }
    }
}


TEST(TMatrixTest, InvalidConstructorSize) {
    ASSERT_THROW(TMatrix<int>(-1), std::out_of_range);
}


TEST(TMatrixTest, AssignmentOperator) {
    TVector<TVector<int>> rows(3);

    TVector<int> r0(3, 0);
    r0[0] = 1; r0[1] = 2; r0[2] = 3;
    rows[0] = r0;

    TVector<int> r1(3, 1);
    r1[1] = 4; r1[2] = 5;
    rows[1] = r1;

    TVector<int> r2(3, 2);
    r2[2] = 6;
    rows[2] = r2;

    TMatrix<int> a(rows);

    TMatrix<int> b = makeRandomUpperMatrix(3);
    b = a;
    ASSERT_TRUE(b == a);

    b[0][0] = 999;
    ASSERT_TRUE(b != a);
    ASSERT_EQ(1, a[0][0]);
}


TEST(TMatrixTest, DoubleTemplate) {
    TVector<TVector<double>> rows(2);

    TVector<double> r0(2, 0);
    r0[0] = 1.5; r0[1] = 2.5;
    rows[0] = r0;

    TVector<double> r1(2, 1);
    r1[1] = 3.5;
    rows[1] = r1;

    TMatrix<double> m(rows);

    ASSERT_DOUBLE_EQ(1.5, m[0][0]);
    ASSERT_DOUBLE_EQ(2.5, m[0][1]);
    ASSERT_DOUBLE_EQ(3.5, m[1][1]);
    ASSERT_THROW(m[1][0], std::out_of_range);
}



TEST(TVectorRandomTest, AdditionIsCommutative) {
    for (int i = 0; i < 50; ++i) {
        int size = randInt(0, 20);
        int si = randInt(0, size);
        TVector<int> a = makeRandomVector(size, si);
        TVector<int> b = makeRandomVector(size, si);
        ASSERT_TRUE((a + b) == (b + a));
    }
}


TEST(TVectorRandomTest, AdditionIsAssociative) {
    for (int i = 0; i < 50; ++i) {
        int size = randInt(0, 20);
        int si = randInt(0, size);
        TVector<int> a = makeRandomVector(size, si);
        TVector<int> b = makeRandomVector(size, si);
        TVector<int> c = makeRandomVector(size, si);
        ASSERT_TRUE(((a + b) + c) == (a + (b + c)));
    }
}


TEST(TVectorRandomTest, SubtractionInvertsAddition) {
    for (int i = 0; i < 50; ++i) {
        int size = randInt(1, 20);
        int si = randInt(0, size);
        TVector<int> a = makeRandomVector(size, si);
        TVector<int> b = makeRandomVector(size, si);
        ASSERT_TRUE(((a + b) - b) == a);
        ASSERT_TRUE(((a - b) + b) == a);
    }
}


TEST(TVectorRandomTest, SubtractionIsAntisymmetric) {
    for (int i = 0; i < 50; ++i) {
        int size = randInt(1, 20);
        int si = randInt(0, size);
        TVector<int> a = makeRandomVector(size, si);
        TVector<int> b = makeRandomVector(size, si);
        ASSERT_TRUE((a - b) == (b - a) * (-1));
    }
}


TEST(TVectorRandomTest, ZeroIsNeutral) {
    for (int i = 0; i < 30; ++i) {
        int size = randInt(1, 20);
        int si = randInt(0, size);
        TVector<int> a = makeRandomVector(size, si);
        TVector<int> zero(size, si);
        ASSERT_TRUE((a + zero) == a);
        ASSERT_TRUE((a - zero) == a);
        ASSERT_TRUE(a * 0 == zero);
        ASSERT_TRUE(a * 1 == a);
    }
}


TEST(TVectorRandomTest, ScalarOperationsCompose) {
    for (int i = 0; i < 50; ++i) {
        int size = randInt(1, 20);
        int si = randInt(0, size);
        TVector<int> a = makeRandomVector(size, si, -10, 10);
        int k1 = randInt(-5, 5);
        int k2 = randInt(-5, 5);
        ASSERT_TRUE((a * k1) * k2 == a * (k1 * k2));
    }
}


TEST(TVectorRandomTest, ScalarDistributivityOverVectorSum) {
    for (int i = 0; i < 50; ++i) {
        int size = randInt(1, 20);
        int si = randInt(0, size);
        TVector<int> a = makeRandomVector(size, si, -10, 10);
        TVector<int> b = makeRandomVector(size, si, -10, 10);
        int k = randInt(-5, 5);
        ASSERT_TRUE((a + b) * k == (a * k) + (b * k));
        ASSERT_TRUE((a - b) * k == (a * k) - (b * k));
    }
}


TEST(TVectorRandomTest, ScalarAddAssociates) {
    for (int i = 0; i < 50; ++i) {
        int size = randInt(1, 20);
        int si = randInt(0, size);
        TVector<int> a = makeRandomVector(size, si, -10, 10);
        int k1 = randInt(-10, 10);
        int k2 = randInt(-10, 10);
        ASSERT_TRUE((a + k1) + k2 == a + (k1 + k2));
    }
}


TEST(TVectorRandomTest, DoubleOfVectorMatchesScalarMul) {
    for (int i = 0; i < 30; ++i) {
        int size = randInt(1, 20);
        int si = randInt(0, size);
        TVector<int> a = makeRandomVector(size, si);
        ASSERT_TRUE((a + a) == a * 2);
    }
}


TEST(TVectorRandomTest, DotProductIsCommutative) {
    for (int i = 0; i < 50; ++i) {
        int size = randInt(1, 20);
        int si = randInt(0, size);
        TVector<int> a = makeRandomVector(size, si);
        TVector<int> b = makeRandomVector(size, si);
        ASSERT_EQ(a * b, b * a);
    }
}


TEST(TVectorRandomTest, DotProductMatchesManual) {
    for (int i = 0; i < 50; ++i) {
        int size = randInt(1, 20);
        int si = randInt(0, size);
        TVector<int> a = makeRandomVector(size, si);
        TVector<int> b = makeRandomVector(size, si);
        int expected = 0;
        for (int i = si; i < size; ++i) {
            expected += a[i] * b[i];
        }
        ASSERT_EQ(a * b, expected);
    }
}


TEST(TVectorRandomTest, AdditionMatchesManual) {
    for (int i = 0; i < 50; ++i) {
        int size = randInt(1, 20);
        int si = randInt(0, size);
        TVector<int> a = makeRandomVector(size, si);
        TVector<int> b = makeRandomVector(size, si);
        TVector<int> sum = a + b;
        for (int i = si; i < size; ++i) {
            ASSERT_EQ(sum[i], a[i] + b[i]);
        }
    }
}


TEST(TVectorRandomTest, ScalarOpsKeepTailZero) {
    for (int i = 0; i < 30; ++i) {
        int size = randInt(1, 20);
        int si = randInt(0, size);
        TVector<int> a = makeRandomVector(size, si, -100, 100);
        int k = randInt(-5, 5);

        TVector<int> expected_plus(size, si);
        TVector<int> expected_minus(size, si);
        TVector<int> expected_mul(size, si);
        for (int i = si; i < size; ++i) {
            expected_plus[i] = a[i] + 10;
            expected_minus[i] = a[i] - 10;
            expected_mul[i] = a[i] * k;
        }

        ASSERT_TRUE((a + 10) == expected_plus);
        ASSERT_TRUE((a - 10) == expected_minus);
        ASSERT_TRUE((a * k) == expected_mul);
    }
}


TEST(TVectorRandomTest, CopyIsIndependent) {
    for (int i = 0; i < 30; ++i) {
        int size = randInt(1, 20);
        int si = randInt(0, size - 1);
        TVector<int> a = makeRandomVector(size, si);
        TVector<int> copy(a);
        ASSERT_TRUE(copy == a);

        int idx = randInt(si, size - 1);
        copy[idx] = a[idx] + 1;
        ASSERT_TRUE(copy != a);
    }
}


TEST(TVectorRandomTest, DoubleTemplateArithmetic) {
    for (int i = 0; i < 20; ++i) {
        int size = randInt(1, 10);
        int si = randInt(0, size);
        TVector<double> a(size, si);
        TVector<double> b(size, si);
        for (int i = si; i < size; ++i) {
            a[i] = randDouble(-100.0, 100.0);
            b[i] = randDouble(-100.0, 100.0);
        }

        TVector<double> sum = a + b;
        for (int i = si; i < size; ++i) {
            ASSERT_NEAR(sum[i], a[i] + b[i], 1e-9);
        }
    }
}



TEST(TMatrixRandomTest, AdditionIsCommutative) {
    for (int i = 0; i < 50; ++i) {
        int n = randInt(1, 10);
        TMatrix<int> A = makeRandomUpperMatrix(n);
        TMatrix<int> B = makeRandomUpperMatrix(n);
        ASSERT_TRUE((A + B) == (B + A));
    }
}


TEST(TMatrixRandomTest, AdditionIsAssociative) {
    for (int i = 0; i < 30; ++i) {
        int n = randInt(1, 10);
        TMatrix<int> A = makeRandomUpperMatrix(n);
        TMatrix<int> B = makeRandomUpperMatrix(n);
        TMatrix<int> C = makeRandomUpperMatrix(n);
        ASSERT_TRUE(((A + B) + C) == (A + (B + C)));
    }
}


TEST(TMatrixRandomTest, SubtractionInvertsAddition) {
    for (int i = 0; i < 50; ++i) {
        int n = randInt(1, 10);
        TMatrix<int> A = makeRandomUpperMatrix(n);
        TMatrix<int> B = makeRandomUpperMatrix(n);
        ASSERT_TRUE(((A + B) - B) == A);
        ASSERT_TRUE(((A - B) + B) == A);
    }
}


TEST(TMatrixRandomTest, ZeroMatrixIsNeutral) {
    for (int i = 0; i < 30; ++i) {
        int n = randInt(1, 10);
        TMatrix<int> A = makeRandomUpperMatrix(n);
        TMatrix<int> Z = makeZeroUpperMatrix(n);
        ASSERT_TRUE((A + Z) == A);
        ASSERT_TRUE((A - Z) == A);
        ASSERT_TRUE((A - A) == Z);
    }
}


TEST(TMatrixRandomTest, AccessMatchesStorage) {
    for (int i = 0; i < 30; ++i) {
        int n = randInt(1, 10);
        TVector<TVector<int>> rows(n);
        std::vector<std::vector<int>> expected(n, std::vector<int>(n, 0));

        for (int i = 0; i < n; ++i) {
            TVector<int> row(n, i);
            for (int j = i; j < n; ++j) {
                int val = randInt(-100, 100);
                row[j] = val;
                expected[i][j] = val;
            }
            rows[i] = row;
        }
        TMatrix<int> M(rows);

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < i; ++j) {
                ASSERT_THROW(M[i][j], std::out_of_range);
            }
            for (int j = i; j < n; ++j) {
                ASSERT_EQ(M[i][j], expected[i][j]);
            }
        }
    }
}


TEST(TMatrixRandomTest, CopyIsIndependent) {
    for (int i = 0; i < 30; ++i) {
        int n = randInt(1, 10);
        TMatrix<int> A = makeRandomUpperMatrix(n);
        TMatrix<int> copy(A);
        ASSERT_TRUE(copy == A);
        int k = randInt(0, n - 1);
        int j = randInt(k, n - 1);
        copy[k][j] += 1;
        ASSERT_TRUE(copy != A);
    }
}


TEST(TMatrixRandomTest, SizeMismatchThrows) {
    for (int i = 0; i < 30; ++i) {
        int n1 = randInt(1, 10);
        int n2 = randInt(1, 10);
        if (n1 == n2) {
            continue;
        }
        TMatrix<int> A = makeRandomUpperMatrix(n1);
        TMatrix<int> B = makeRandomUpperMatrix(n2);

        ASSERT_THROW(A + B, std::invalid_argument);
        ASSERT_THROW(A - B, std::invalid_argument);
        ASSERT_THROW(B + A, std::invalid_argument);
        ASSERT_THROW(B - A, std::invalid_argument);
    }
}