#pragma once

#include <iostream>

const int MAX_VECTOR_SIZE = 100000000;
const int MAX_MATRIX_SIZE = 10000;

// Шаблон вектора
template <class ValType>
class TVector
{
protected:
  int Size;       // размер вектора
  int StartIndex; // индекс первого элемента вектора
  ValType* pVector;
public:
  TVector(int s = 10, int si = 0);
  TVector(const TVector &v);                // конструктор копирования
  ~TVector();
  int GetSize() const { return Size; }             // размер вектора
  int GetStartIndex() const { return StartIndex; } // индекс первого элемента
  ValType& operator[](int pos);             // доступ
  const ValType& operator[](int pos) const; // доступ
  bool operator==(const TVector &v) const;  // сравнение
  bool operator!=(const TVector &v) const;  // сравнение
  TVector& operator=(const TVector &v);     // присваивание

  // скалярные операции
  TVector  operator+(const ValType &val) const;   // прибавить скаляр
  TVector  operator-(const ValType &val) const;   // вычесть скаляр
  TVector  operator*(const ValType &val) const;   // умножить на скаляр

  // векторные операции
  TVector  operator+(const TVector &v) const;     // сложение
  TVector  operator-(const TVector &v) const;     // вычитание
  ValType  operator*(const TVector &v) const;     // скалярное произведение

  // ввод-вывод

  friend std::istream& operator>>(std::istream& in, TVector& v);
  friend std::ostream& operator<<(std::ostream& out, const TVector& v);
};

// Верхнетреугольная матрица
template <class ValType>
class TMatrix : public TVector<TVector<ValType>>
{
public:
  TMatrix(int s = 10);                           
  TMatrix(const TMatrix &mt);                    // копирование
  TMatrix(const TVector<TVector<ValType>> &mt);  // преобразование типа
  bool operator==(const TMatrix &mt) const;      // сравнение
  bool operator!=(const TMatrix &mt) const;      // сравнение
  TMatrix& operator= (const TMatrix &mt);        // присваивание
  TMatrix  operator+ (const TMatrix &mt) const;        // сложение
  TMatrix  operator- (const TMatrix &mt) const;        // вычитание

  // ввод / вывод
  friend std::istream& operator>>(std::istream& in, TMatrix& mt);
  friend std::ostream& operator<<(std::ostream& out, const TMatrix& mt);
};

