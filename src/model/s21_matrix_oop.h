#ifndef __S21MATRIX_H__
#define __S21MATRIX_H__

#include <algorithm>
#include <cfloat>
#include <cmath>
#include <cstring>
#include <iostream>

#define SUCCESS 1
#define FAILURE 0

class S21Matrix {
 private:
  size_t rows_, columns_;
  double* matrix_;
  friend class S21MatrixTest;

 public:
  S21Matrix();
  S21Matrix(size_t rows, size_t cols);
  S21Matrix(const S21Matrix& other);
  S21Matrix(S21Matrix&& other) noexcept;
  S21Matrix(size_t rows, size_t cols, const double* values);
  ~S21Matrix();

  // Общие методы
  S21Matrix create_minor_matrix(size_t row, size_t column);
  // getter_setters
  size_t get_rows() const;
  size_t get_columns() const;
  void set_rows(size_t newRows);
  void set_columns(size_t newColumns);
  void resize(size_t new_rows, size_t new_cols);
  double* get_matrix() const;

  // Основные методы
  S21Matrix CalcComplements();
  double Determinant();
  bool EqMatrix(const S21Matrix& other);
  S21Matrix InverseMatrix();
  void MulMatrix(const S21Matrix& other);
  void MulNumber(const double num);
  void SubMatrix(const S21Matrix& other);
  void SumMatrix(const S21Matrix& other);
  S21Matrix Transpose();

  // Перегруженные операторы
  S21Matrix& operator*=(const S21Matrix& other);
  S21Matrix& operator*=(double number);
  S21Matrix operator*(const S21Matrix& other) const;
  S21Matrix operator*(double number) const;
  S21Matrix& operator+=(const S21Matrix& other);
  S21Matrix operator+(const S21Matrix& other);
  bool operator==(const S21Matrix& other) const;
  S21Matrix& operator=(const S21Matrix& other) noexcept;
  S21Matrix& operator=(S21Matrix&& other) noexcept;
  S21Matrix& operator-=(const S21Matrix& other);
  S21Matrix operator-(const S21Matrix& other);
  double& operator()(size_t row, size_t col);
  const double& operator()(size_t row, size_t col) const;

  // Debug
};
// за пределами класса
double s21_determinant_2x2(double a, double b, double c, double d);
bool isZeroMatrix(size_t rows, size_t columns, double* matrix);

#endif