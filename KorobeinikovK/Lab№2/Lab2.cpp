#include "pch.h"
#include "real_utmatrix.h"
#include <stdexcept>


template <typename T>
void RandVector(TVector<T>& v) {
	for (size_t i = v.GetStartIndex(); i < v.GetStartIndex() + v.GetSize(); ++i) {
		T val = static_cast<T>(rand() % 10000) / static_cast<T>(10) - static_cast<T>(500);
		v[i] = val;
	}
}
template <typename T>
void RandMatrix(TMatrix<T>& mt) {
	for (size_t i = 0; i < mt.GetSize(); ++i) {
		for (size_t j = mt[i].GetStartIndex(); j < mt.GetSize(); ++j) {
			T val = static_cast<T>(rand() % 10000) / static_cast<T>(10) - static_cast<T>(500);
			mt[i][j] = val;
		}
	}
}

TEST(VectorTest, CreateTest) {
	TVector<int> v1;
	TVector<double> v2;
	EXPECT_EQ(v1.GetSize(), 10);
	EXPECT_EQ(v1.GetStartIndex(), 0);
	EXPECT_EQ(v2.GetSize(), 10);
	EXPECT_EQ(v2.GetStartIndex(), 0);
	TVector<int> v3;
	v3[5] = 123;
	TVector<int> v4(v3);
	EXPECT_TRUE(v3 == v4);
	EXPECT_EQ(v3[5], v4[5]);
}
TEST(VectorTest, ComparisonTest) {
	TVector<int> v1(12,3), v2(12, 3);
	TVector<double> v3(20), v4(20);
	RandVector(v1);
	RandVector(v3);
	v2 = v1;
	v4 = v3;
	for (size_t i = v1.GetStartIndex(); i < v1.GetStartIndex()+v1.GetSize(); ++i) {
		EXPECT_EQ(v1[i], v2[i]);
	}
	for (size_t i = v3.GetStartIndex(); i < v3.GetStartIndex() + v3.GetSize(); ++i) {
		EXPECT_EQ(v3[i], v4[i]);
	}
	EXPECT_TRUE(v1 == v2);
	v3[9] = 0;
	EXPECT_TRUE(v3 != v4);
}
TEST(VectorTest, ScalarOperationsTest) {
	TVector<int> a(7,1);
	for (int i = 1; i < 8; ++i) {
		a[i] = i;
	}
	TVector<int> sum = a + 4;
	for (size_t i = 1; i < 8; ++i) {
		EXPECT_EQ(sum[i], i + 4);
	}
	TVector<int> sub = a - 11;
	for (int i = 1; i < 8; ++i) {
		EXPECT_EQ(sub[i], i - 11);
	}
	TVector<int> mul = a * 9;
	for (int i = 1; i < 8; ++i) {
		EXPECT_EQ(mul[i], i * 9);
	}
}
TEST(VectorTest, VectorOperationsTest) {
	TVector<double> a(20), b(20);
	for (int i = 0; i < 20; ++i) {
		a[i] = i;
		b[i] = i + 4;
	}
	TVector<double> sum = a + b;
	for (int i = 0; i < 20; ++i) {
		EXPECT_EQ(sum[i], i + i + 4);
	}
	TVector<double> sub = a - b;
	for (int i = 0; i < 20; ++i) {
		EXPECT_EQ(sub[i], i - (i + 4));
	}
	double mul = a * b;
	double val = 0.0;
	for (int i = 0; i < 20; ++i) {
		val += i * (i + 4);	
	}
	EXPECT_EQ(mul, val);
}
TEST(VectorTest, ScalarOperationsTest2) {
	TVector<int> a(14);
	for (int i = 0; i < 14; ++i) {
		a[i] = i * 2 - 1;
	}
	TVector<int> res = (a + 2) * 7 - 20;
	for (int i = 0; i < 14; ++i) {
		EXPECT_EQ(res[i], ((i * 2 - 1) + 2) * 7 - 20);
	}
}
TEST(VectorTest, VectorOperationsTest2) {
	TVector<int> a(17), b(17), c(17);
	for (int i = 0; i < 17; ++i) {
		a[i] = i * 3;
		b[i] = i + 1;
		c[i] = - i;
	}
	TVector<int> res = a + b - c;
	for (int i = 0; i < 17; ++i) {
		EXPECT_EQ(res[i], i * 3 + (i + 1) - (- i));
	}
	int val = a * b + b * c - c * a;
	int n1 = 0, n2 = 0, n3 = 0;
	for (int i = 0; i < 17; ++i) {
		n1 += i * 3 * (i + 1);
		n2 += (i + 1) * (- i);
		n3 += (- i) * i * 3;
	}
	EXPECT_EQ(val, n1 + n2 - n3);
}
TEST(VectorTest, ZeroSizeTest) {
	TVector<double> a(0), b(0);
	EXPECT_EQ(a.GetSize(), 0);
	EXPECT_EQ((a + b).GetSize(), 0);
}
TEST(VectorTest, StartIndAndDifSizeTest) {
	TVector<int> a(11), b(20), c(11, 4);
	RandVector(a);
	RandVector(b);
	RandVector(c);
	EXPECT_THROW(a[12], std::out_of_range);
	EXPECT_THROW(c[0], std::out_of_range);
	EXPECT_THROW(a + b, std::invalid_argument);
	EXPECT_THROW(a - b, std::invalid_argument);
	EXPECT_THROW(a * b, std::invalid_argument);
	EXPECT_THROW(a + c, std::invalid_argument);
	EXPECT_FALSE(a == c);
	EXPECT_TRUE(a != c);
}
TEST(VectorTest, NegativeIndexTest) {
	TVector<int> v(5, 2);
	EXPECT_THROW(v[-5], std::out_of_range);
}
TEST(VectorTest, MaxSizeTest) {
	EXPECT_THROW(TVector<int> v(MAX_VECTOR_SIZE + 1), std::out_of_range);
}
TEST(VectorTest, InvariantsOperationTest) {
	TVector<int> a(20,5), b(20,5),c(20,5);
	RandVector(a);
	RandVector(b);
	RandVector(c);
	EXPECT_TRUE((a + b) == (b + a));
	EXPECT_TRUE(((a + b) + c) == (a + (b + c)));
	int x1 = 2, x2 = 3;
	EXPECT_TRUE(a * (x1 + x2) == a * x1 + a * x2);
	EXPECT_TRUE(a * 1 == a);
	EXPECT_TRUE(a + 0 == a);
}

TEST(MatrixTest, CreateTest) {
	TMatrix<int> mt1;
	TMatrix<double> mt3;
	EXPECT_EQ(mt1.GetSize(), 10);
	EXPECT_EQ(mt3.GetSize(), 10);
	mt1[3][6] = 34;
	TMatrix<int> mt2(mt1);
	EXPECT_TRUE(mt1 == mt2);
	EXPECT_EQ(mt1[3][6], mt2[3][6]);
}
TEST(MatrixTest, ComparisonTest) {
	TMatrix<int> mt1(20), mt2(20);
	TMatrix<double> mt3(17), mt4(17);
	RandMatrix(mt1);
	RandMatrix(mt3);
	mt2 = mt1;
	mt4 = mt3;
	for (size_t i = 0; i < mt1.GetSize(); ++i) {
		for (size_t j = mt1[i].GetStartIndex(); j < mt1.GetSize(); ++j) {
			EXPECT_EQ(mt1[i][j], mt2[i][j]);
		}
	}
	for (size_t i = 0; i < mt3.GetSize(); ++i) {
		for (size_t j = mt3[i].GetStartIndex(); j < mt3.GetSize(); ++j) {
			EXPECT_EQ(mt3[i][j], mt4[i][j]);
		}
	}
	EXPECT_TRUE(mt1 == mt2);
	mt4[8][10] = 12.21;
	EXPECT_TRUE(mt3 != mt4);
}
TEST(MatrixTest, OperationTest) {
	TMatrix<int> a(21), b(21);
	for (int i = 0; i < 21; ++i) {
		for (int j = i; j < 21; j++) {
			a[i][j] = i + j - 1;
			b[i][j] = i * j;
		}
	}
	TMatrix<int> sum = a + b;
	for (int i = 0; i < 21; ++i) {
		for (int j = i; j < 21; j++) {
			EXPECT_EQ(sum[i][j], (i + j - 1) + (i * j));
		}
	}
	TMatrix<int> sub = b - a;
	for (int i = 0; i < 21; ++i) {
		for (int j = i; j < 21; j++) {
			EXPECT_EQ(sub[i][j], (i * j) - (i + j - 1));
		}
	}
}
TEST(MatrixTest, DifferentSizeTest) {
	TMatrix<double> a(200), b(291);
	RandMatrix(a);
	RandMatrix(b);
	EXPECT_THROW(a[24][12], std::out_of_range);
	EXPECT_THROW(a + b, std::invalid_argument);
	EXPECT_THROW(a - b, std::invalid_argument);
	EXPECT_FALSE(a == b);
	EXPECT_TRUE(a != b);
}
TEST(MatrixTest, NegativeIndexTest) {
	TMatrix<int> m(5);
	EXPECT_THROW(m[-1][0], std::out_of_range);
	EXPECT_THROW(m[0][-1], std::out_of_range);
}
TEST(MatrixTest, MaxSizeTest) {
	EXPECT_THROW(TMatrix<int> m(MAX_MATRIX_SIZE + 1), std::out_of_range);
}
TEST(MatrixTest, ZeroSizeTest) {
	TMatrix<int> a(0), b(0);
	EXPECT_EQ(a.GetSize(), 0);
	EXPECT_EQ((a + b).GetSize(), 0);
	EXPECT_TRUE(a == b);
}
TEST(MatrixTest, InvariantsTest) {
	TMatrix<double> m1(4), m2(4), m3(4);
	RandMatrix(m1);
	RandMatrix(m2);
	RandMatrix(m3);
	TMatrix<double> a(4);
	EXPECT_TRUE((m1 + a) == m1);
	EXPECT_TRUE((m1 - m1) == a);
	EXPECT_TRUE((m1 + m2) == (m2 + m1));
	EXPECT_TRUE(((m1 + m2) + m3) == (m1 + (m2 + m3)));
	EXPECT_TRUE(((m1 + m2) - m2) == m1);
}