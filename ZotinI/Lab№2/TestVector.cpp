#include "pch.h"
#include <cstdlib>
#include <stdexcept>
#include <ctime>
#include <fstream>
#include "TVector.cpp"

TEST(TestVector, Size) {
	EXPECT_ANY_THROW(TVector<int> a(-1));
	EXPECT_ANY_THROW(TVector<int> a(10, -2));
	EXPECT_NO_THROW(TVector<int> a(10, 4));

	TVector<double> b(5, 1);
	EXPECT_EQ(5, b.GetSize());
	EXPECT_EQ(1, b.GetStartIndex());
}

TEST(TestVector, Index) {
	TVector<double> b(5, 2);
	EXPECT_ANY_THROW(b[1]);
	EXPECT_ANY_THROW(b[7]);
	EXPECT_NO_THROW(b[3]);
	b[4] = 9;
	EXPECT_EQ(b[4], 9);
}

TEST(TestVector, Compare) {
	TVector<double> a(5, 2);
	TVector<double> b(6, 2);
	EXPECT_FALSE(a == b);
	EXPECT_TRUE(a != b);

	TVector<double> c(5, 2);
	EXPECT_TRUE(a == c);
	TVector<double> d(b);
	EXPECT_TRUE(d == b);
}

TEST(TestVector, Operations) {
	srand(time(nullptr));
	TVector<int> v1(3, 0);
	int a1 = rand() % 100;
	int a2 = rand() % 100;
	int sc = 5;

	v1[0] = a1; 
	v1[1] = a2; 
	v1[2] = 0;

	TVector<int> v1_plus = v1 + sc;
	EXPECT_EQ(a1 + sc, v1_plus[0]);

	TVector<int> v1_minus = v1 - sc;
	EXPECT_EQ(a2 - sc, v1_minus[1]);

	TVector<int> v1_mult = v1 * sc;
	EXPECT_EQ(a1 * sc, v1_mult[0]);

	TVector<int> v2(3, 0);
	int b1 = rand() % 100;
	int b2 = rand() % 100;
	v2[0] = b1;
	v2[1] = b2;
	v2[2] = 1;
	TVector<int> v_plus = v1 + v2;
	for (size_t i = 0; i < 3; ++i) {
		EXPECT_EQ(v_plus[i], v1[i] + v2[i]);
	}
	TVector<int> v_minus = v1 - v2;
	for (size_t i = 0; i < 3; ++i) {
		EXPECT_EQ(v_minus[i], v1[i] - v2[i]);
	}
	int v_mult = v1 * v2;
	int temp = 0;
	for (size_t i = 0; i < 3; ++i) {
		temp += v1[i] * v2[i];
	}
	EXPECT_EQ(v_mult, temp);
}

TEST(TestVector, Edgecases) {
	srand(time(nullptr));
	TVector<int> v1(3, 0);
	TVector<int> v2(5, 0);
	TVector<int> v3(5, 2);
	EXPECT_ANY_THROW(v1 + v2);
	EXPECT_ANY_THROW(v2 + v3);

	EXPECT_ANY_THROW(TVector<int> v(MAX_VECTOR_SIZE + 1, 0));
}
