
#include "model/s21_matrix_oop.h"

size_t S21Matrix::get_rows() const { return rows_; }

size_t S21Matrix::get_columns() const { return columns_; }

void S21Matrix::set_rows(size_t new_rows) {
  if (new_rows != rows_) {
    resize(new_rows, columns_);
  }
}

void S21Matrix::set_columns(size_t new_cols) {
  if (new_cols != columns_) {
    resize(rows_, new_cols);
  }
}

void S21Matrix::resize(size_t new_rows, size_t new_cols) {
  if (new_rows == 0 || new_cols == 0) {
    throw std::invalid_argument("Некорректное значение");
  }
  double* new_matrix = new double[new_rows * new_cols]();
  for (size_t i = 0; i < std::min<size_t>(rows_, new_rows); ++i) {
    for (size_t j = 0; j < std::min<size_t>(columns_, new_cols); ++j) {
      new_matrix[i * new_cols + j] = matrix_[i * columns_ + j];
    }
  }
  delete[] matrix_;

  matrix_ = new_matrix;
  rows_ = new_rows;
  columns_ = new_cols;
}

double* S21Matrix::get_matrix() const { return matrix_; }

// Вычисление минора матрицы
S21Matrix S21Matrix::create_minor_matrix(size_t row, size_t column) {
  S21Matrix minor(rows_ - 1, columns_ - 1);
  size_t minor_row = 0, minor_col = 0;
  for (size_t i = 0; i < rows_; i++) {
    if (i == row) continue;
    minor_col = 0;
    for (size_t j = 0; j < columns_; j++) {
      if (j == column) continue;
      minor.matrix_[minor_row * minor.columns_ + minor_col] =
          matrix_[i * columns_ + j];
      minor_col++;
    }
    minor_row++;
  }
  return minor;
}

S21Matrix::S21Matrix() : rows_(0), columns_(0), matrix_(nullptr) {}

S21Matrix::S21Matrix(size_t rows, size_t cols) : rows_(rows), columns_(cols) {
  // Выделение памяти и () для инициализация нулями
  matrix_ = new double[rows_ * columns_]();
}

S21Matrix::S21Matrix(const S21Matrix& other)
    : rows_(other.rows_), columns_(other.columns_) {
  if (rows_ > 0 && columns_ > 0) {
    matrix_ = new double[rows_ * columns_];
    std::memcpy(matrix_, other.matrix_, rows_ * columns_ * sizeof(double));
  } else {
    matrix_ = nullptr;
  }
}

S21Matrix::S21Matrix(S21Matrix&& other) noexcept
    : rows_(other.rows_), columns_(other.columns_), matrix_(other.matrix_) {
  other.matrix_ = nullptr;
  other.rows_ = 0;
  other.columns_ = 0;
}

S21Matrix::S21Matrix(size_t rows, size_t cols, const double* values)
    : S21Matrix(rows, cols) {  // Сначала выделяем память
  if (values) {  // Проверка, что указатель не nullptr
    for (size_t i = 0; i < rows; ++i) {
      for (size_t j = 0; j < cols; ++j) {
        matrix_[i * cols + j] = values[i * cols + j];
      }
    }
  }
}

S21Matrix::~S21Matrix() { delete[] matrix_; }

S21Matrix S21Matrix::CalcComplements() {
  if (rows_ == 0 || columns_ == 0 || matrix_ == nullptr) {
    throw std::invalid_argument("Недопустимое входное значение матрицы.");
  }
  if (rows_ != columns_) {
    throw std::logic_error("Матрица не является квадратной.");
  }
  S21Matrix result(rows_, columns_);
  for (size_t i = 0; i < rows_; i++) {
    for (size_t j = 0; j < columns_; j++) {
      S21Matrix minor = create_minor_matrix(i, j);
      double minor_det = minor.Determinant();
      result.matrix_[i * columns_ + j] =
          minor_det * (((i + j) % 2 == 0) ? 1 : -1);
    }
  }
  return result;
}

double S21Matrix::Determinant() {
  if (rows_ == 0 || columns_ == 0 || matrix_ == nullptr) {
    throw std::invalid_argument("Недопустимое входное значение.");
  }
  if (rows_ != columns_) {
    throw std::logic_error("Матрица не является квадратной.");
  }
  double result;
  if (isZeroMatrix(rows_, columns_, matrix_)) {
    result = 0.0;
  } else if (rows_ == 1) {
    result = matrix_[0];
  } else if (rows_ == 2) {
    result =
        s21_determinant_2x2(matrix_[0], matrix_[1], matrix_[2], matrix_[3]);
  } else {
    result = 0.0;
    for (size_t j = 0; j < columns_; j++) {
      S21Matrix minor = create_minor_matrix(0, j);
      double minor_det = minor.Determinant();
      result += matrix_[j] * minor_det * ((j % 2 == 0) ? 1 : -1);
    }
  }
  return result;
}

double s21_determinant_2x2(double a, double b, double c, double d) {
  return a * d - b * c;
}

bool isZeroMatrix(size_t rows, size_t columns, double* matrix) {
  // пробегает по массиву от начала matrix и до конца matrix + rows * columns,
  // использует lambda функцию для проверки на ноль
  return std::all_of(matrix, matrix + rows * columns,
                     [](double value) { return value == 0; });
}

bool S21Matrix::EqMatrix(const S21Matrix& other) { return *this == other; }

S21Matrix S21Matrix::InverseMatrix() {
  double determinant;
  if (rows_ == 0 || columns_ == 0 || matrix_ == nullptr) {
    throw std::invalid_argument("Недопустимое входное значение матрицы.");
  } else if (rows_ != columns_) {
    throw std::logic_error("Матрица не является квадратной.");
  } else if ((determinant = Determinant()) == 0) {
    throw std::logic_error("Матрица вырождена.");
  } else {
    S21Matrix complements_matrix = CalcComplements();
    S21Matrix transpose_matrix = complements_matrix.Transpose();
    S21Matrix inverse_matrix = transpose_matrix * (1.0 / determinant);
    return inverse_matrix;
  }
}

void S21Matrix ::MulMatrix(const S21Matrix& other) { *this *= other; }

void S21Matrix ::MulNumber(const double num) { *this *= num; }

// Функция для вычитания двух матриц
void S21Matrix ::SubMatrix(const S21Matrix& other) { *this -= other; }

// Функция для сложения двух матриц
void S21Matrix ::SumMatrix(const S21Matrix& other) { *this += other; }

S21Matrix S21Matrix::Transpose() {
  if (rows_ == 0 || columns_ == 0 || matrix_ == nullptr) {
    throw std::out_of_range("Неверный размер матрицы.");
  }
  S21Matrix result(columns_, rows_);
  for (size_t i = 0; i < rows_; i++) {
    for (size_t j = 0; j < columns_; j++) {
      result.matrix_[j * rows_ + i] = matrix_[i * columns_ + j];
    }
  }
  return result;
}

S21Matrix& S21Matrix::operator+=(const S21Matrix& other) {
  // Проверка на совместимость размеров
  if (this->rows_ != other.rows_ || this->columns_ != other.columns_) {
    throw std::invalid_argument("Различная размерность матриц.");
  }

  // Сложение матриц
  for (size_t i = 0; i < this->rows_; ++i) {
    for (size_t j = 0; j < this->columns_; ++j) {
      this->matrix_[i * this->columns_ + j] +=
          other.matrix_[i * other.columns_ + j];
    }
  }
  return *this;
}

S21Matrix S21Matrix::operator+(const S21Matrix& other) {
  if (this->rows_ != other.rows_ || this->columns_ != other.columns_) {
    throw std::invalid_argument("Различная размерность матриц.");
  }
  S21Matrix result(this->rows_, this->columns_);

  for (size_t i = 0; i < this->rows_; ++i) {
    for (size_t j = 0; j < this->columns_; ++j) {
      result.matrix_[i * this->columns_ + j] =
          this->matrix_[i * this->columns_ + j] +
          other.matrix_[i * other.columns_ + j];
    }
  }
  return result;
}

S21Matrix& S21Matrix::operator-=(const S21Matrix& other) {
  if (this->rows_ != other.rows_ || this->columns_ != other.columns_) {
    throw std::invalid_argument("Различная размерность матриц.");
  }

  for (size_t i = 0; i < this->rows_; ++i) {
    for (size_t j = 0; j < this->columns_; ++j) {
      this->matrix_[i * this->columns_ + j] -=
          other.matrix_[i * other.columns_ + j];
    }
  }
  return *this;
}

S21Matrix S21Matrix::operator-(const S21Matrix& other) {
  if (this->rows_ != other.rows_ || this->columns_ != other.columns_) {
    throw std::invalid_argument("Различная размерность матриц.");
  }
  S21Matrix result(this->rows_, this->columns_);

  for (size_t i = 0; i < this->rows_; ++i) {
    for (size_t j = 0; j < this->columns_; ++j) {
      result.matrix_[i * this->columns_ + j] =
          this->matrix_[i * this->columns_ + j] -
          other.matrix_[i * other.columns_ + j];
    }
  }
  return result;
}

#define EPSILON 1e-7  // Точность сравнения

bool S21Matrix::operator==(const S21Matrix& other) const {
  bool status = SUCCESS;
  if (matrix_ == nullptr || other.matrix_ == nullptr || rows_ != other.rows_ ||
      columns_ != other.columns_) {
    status = FAILURE;
  } else {
    // Сравнение элементов матриц
    for (size_t i = 0; i < rows_; i++) {
      for (size_t j = 0; j < columns_; j++) {
        if (std::fabs(matrix_[i * columns_ + j] -
                      other.matrix_[i * columns_ + j]) > EPSILON) {
          status = FAILURE;
        }
      }
    }
  }
  return status;
}

S21Matrix& S21Matrix::operator=(const S21Matrix& other) noexcept {
  if (this != &other) {
    delete[] matrix_;
    rows_ = other.rows_;
    columns_ = other.columns_;
    if (rows_ > 0 && columns_ > 0) {
      matrix_ = new double[rows_ * columns_];
      std::memcpy(matrix_, other.matrix_, rows_ * columns_ * sizeof(double));
    } else {
      matrix_ = nullptr;
    }
  }
  return *this;
}

S21Matrix& S21Matrix::operator=(S21Matrix&& other) noexcept {
  if (this != &other) {
    delete[] matrix_;
    rows_ = other.rows_;
    columns_ = other.columns_;
    matrix_ = other.matrix_;

    other.rows_ = 0;
    other.columns_ = 0;
    other.matrix_ = nullptr;
  }
  return *this;
}

S21Matrix& S21Matrix::operator*=(double number) {
  if ((fabs(number) != 0.0 && fabs(number) < DBL_MIN) ||
      fabs(number) > DBL_MAX || matrix_ == nullptr || rows_ == 0 ||
      columns_ == 0) {
    throw std::invalid_argument("Недопустимое входное значение");
  }
  for (size_t i = 0; i < rows_; ++i) {
    for (size_t j = 0; j < columns_; ++j) {
      matrix_[i * columns_ + j] *= number;
    }
  }
  return *this;
}

S21Matrix& S21Matrix::operator*=(const S21Matrix& other) {
  return *this = (*this * other);
}

S21Matrix S21Matrix::operator*(double number) const {
  if ((fabs(number) != 0.0 && fabs(number) < DBL_MIN) ||
      fabs(number) > DBL_MAX || matrix_ == nullptr || rows_ == 0 ||
      columns_ == 0) {
    throw std::invalid_argument("Недопустимое входное значение");
  }
  S21Matrix result(rows_, columns_);
  for (size_t i = 0; i < rows_; ++i) {
    for (size_t j = 0; j < columns_; ++j) {
      result.matrix_[i * columns_ + j] = matrix_[i * columns_ + j] * number;
    }
  }
  return result;
}

S21Matrix S21Matrix::operator*(const S21Matrix& other) const {
  if (matrix_ == nullptr || other.matrix_ == nullptr || rows_ == 0 ||
      columns_ == 0 || other.rows_ == 0 || other.columns_ == 0) {
    throw std::out_of_range(
        "Неверный размер матрицы. Матрицы должны иметь размеры больше нуля.");
  }
  if (columns_ != other.rows_) {
    throw std::invalid_argument(
        "Число столбцов первой матрицы не равно числу строк второй матрицы.");
  }
  S21Matrix result(rows_, other.columns_);
  double sum;
  for (size_t i = 0; i < rows_; ++i) {
    for (size_t j = 0; j < other.columns_; ++j) {
      sum = 0.0;
      for (size_t k = 0; k < columns_; ++k) {
        sum +=
            matrix_[i * columns_ + k] * other.matrix_[k * other.columns_ + j];
      }
      result.matrix_[i * result.columns_ + j] = sum;
    }
  }
  return result;
}

double& S21Matrix::operator()(size_t row, size_t col) {
  if (row >= rows_ || col >= columns_) {
    throw std::out_of_range("Индекс за пределами матрицы");
  }
  return matrix_[row * columns_ + col];
}

const double& S21Matrix::operator()(size_t row, size_t col) const {
  if (row >= rows_ || col >= columns_) {
    throw std::out_of_range("Индекс за пределами матрицы");
  }
  return matrix_[row * columns_ + col];
}
