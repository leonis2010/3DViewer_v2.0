#include <iostream>
#include "gtest/gtest.h"
#include "model/s21_matrix_oop.h"


class S21MatrixTest : public ::testing::Test {
 protected:
  S21Matrix matrix_1, matrix_2, matrix_3, result_expected, matrix_equal_1;
  const S21Matrix const_matrix;

  S21MatrixTest()
      : const_matrix(
            3, 3,
            (const double[9]){1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0}) {}

  void SetUp() override {
    matrix_1 = S21Matrix(3, 3);
    matrix_2 = S21Matrix(3, 3);
    matrix_3 = S21Matrix(3, 3);
    result_expected = S21Matrix(3, 3);
    matrix_equal_1 = S21Matrix(3, 3);
    double values_1[] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0};
    double values_2[] = {9.0, 8.0, 7.0, 6.0, 5.0, 4.0, 3.0, 2.0, 1.0};
    double values_3[] = {7.0, -2.0, 9.0, 2.0, 1.0, 6.0, -3.0, -2.0, 7.0};

    for (size_t i = 0; i < matrix_1.get_rows(); ++i) {
      for (size_t j = 0; j < matrix_1.get_columns(); ++j) {
        matrix_equal_1(i, j) = matrix_1(i, j) =
            values_1[i * matrix_1.get_columns() + j];
        matrix_2(i, j) = values_2[i * matrix_1.get_columns() + j];
        matrix_3(i, j) = values_3[i * matrix_3.get_columns() + j];
      }
    }
  }

  void TearDown() override {}
};

// Тест для оператора -=
TEST_F(S21MatrixTest, SubtractionAssignmentTest) {
  // Устанавливаем ожидаемый результат для вычитания
  result_expected(0, 0) = -8.0;
  result_expected(0, 1) = -6.0;
  result_expected(0, 2) = -4.0;
  result_expected(1, 0) = -2.0;
  result_expected(1, 1) = 0.0;
  result_expected(1, 2) = 2.0;
  result_expected(2, 0) = 4.0;
  result_expected(2, 1) = 6.0;
  result_expected(2, 2) = 8.0;

  S21Matrix matrix_result = matrix_1;
  S21Matrix matrix_result_func = matrix_1;

  matrix_result -= matrix_2;
  matrix_result_func.SubMatrix(matrix_2);

  for (size_t i = 0; i < result_expected.get_rows(); ++i) {
    for (size_t j = 0; j < result_expected.get_columns(); ++j) {
      EXPECT_EQ(matrix_result(i, j), result_expected(i, j));
      EXPECT_EQ(matrix_result_func(i, j), result_expected(i, j));
    }
  }
}

// // Тест на выброс исключения для оператора -=
// TEST_F(S21MatrixTest, SubtractionAssignmentDifferentDimensions) {
//   S21Matrix matrix1(3, 3);
//   // Создаем матрицу другого размера
//   S21Matrix matrix2(2, 2);

//   try {
//     matrix1 -= matrix2;
//   } catch (...) {
//     std::cout << "caught error\n";
//   }
// }

TEST_F(S21MatrixTest, SubtractionTest) {
  // Ожидаемый результат после вычитания
  result_expected(0, 0) = -8.0;
  result_expected(0, 1) = -6.0;
  result_expected(0, 2) = -4.0;
  result_expected(1, 0) = -2.0;
  result_expected(1, 1) = 0.0;
  result_expected(1, 2) = 2.0;
  result_expected(2, 0) = 4.0;
  result_expected(2, 1) = 6.0;
  result_expected(2, 2) = 8.0;

  S21Matrix result = matrix_1 - matrix_2;

  for (size_t i = 0; i < result.get_rows(); ++i) {
    for (size_t j = 0; j < result.get_columns(); ++j) {
      EXPECT_EQ(result(i, j), result_expected(i, j));
    }
  }
}

// // Тест для проверки исключений при вычитании матриц разного размера
// TEST_F(S21MatrixTest, SubtractionDifferentDimensions) {
//   S21Matrix matrix1(3, 3);
//   S21Matrix matrix2(2, 2);

//   try {
//     S21Matrix result = matrix1 - matrix2;
//   } catch (...) {
//     std::cout << "caught error\n";
//   }
// }

TEST_F(S21MatrixTest, MultiplyMatrices) {
  // Инициализация матриц (границы размерностей и значения!)
  // Например, 3 строки, 2 столбца
  matrix_1 = S21Matrix(3, 2);
  matrix_1(0, 0) = 1.0;
  matrix_1(0, 1) = 2.0;
  matrix_1(1, 0) = 3.0;
  matrix_1(1, 1) = 4.0;
  matrix_1(2, 0) = 5.0;
  matrix_1(2, 1) = 6.0;

  // Например, 2 строки, 3 столбца
  matrix_2 = S21Matrix(2, 3);
  matrix_2(0, 0) = 7.0;
  matrix_2(0, 1) = 8.0;
  matrix_2(0, 2) = 9.0;
  matrix_2(1, 0) = 10.0;
  matrix_2(1, 1) = 11.0;
  matrix_2(1, 2) = 12.0;

  // Ожидаемый результат после умножения:
  result_expected(0, 0) = 27.0;
  result_expected(0, 1) = 30.0;
  result_expected(0, 2) = 33.0;
  result_expected(1, 0) = 61.0;
  result_expected(1, 1) = 68.0;
  result_expected(1, 2) = 75.0;
  result_expected(2, 0) = 95.0;
  result_expected(2, 1) = 106.0;
  result_expected(2, 2) = 117.0;

  S21Matrix result_1 = matrix_1 * matrix_2;

  for (size_t i = 0; i < result_1.get_rows(); ++i) {
    for (size_t j = 0; j < result_1.get_columns(); ++j) {
      EXPECT_EQ(result_1(i, j), result_expected(i, j));
    }
  }
}

// TEST_F(S21MatrixTest, MultiplyMatricesWithIncompatibleSizes) {
//   // Инициализация матриц
//   // 3 строки, 2 столбца
//   S21Matrix matrix_incompatible(3, 2);
//   // matrix_1 должна быть, например, 3 строки, 2 столбца
//   matrix_1 = S21Matrix(3, 2);
//   try {
//     S21Matrix result = matrix_1 * matrix_incompatible;
//   } catch (...) {
//     std::cout << "caught error\n";
//   }
// }

// TEST_F(S21MatrixTest, MultiplyEmptyMatrix) {
//   // Пустая матрица
//   S21Matrix empty_matrix_1 = S21Matrix(0, 0);
//   // Непустая матрица
//   S21Matrix empty_matrix_2 = S21Matrix(3, 3);

//   try {
//     S21Matrix result = empty_matrix_1 * empty_matrix_2;
//     result = matrix_1 * empty_matrix_1;
//   } catch (...) {
//     std::cout << "caught error\n";
//   }
// }

TEST_F(S21MatrixTest, MultiplyByZero) {
  double multiplier = 0.0;
  result_expected(0, 0) = 0.0;
  result_expected(0, 1) = 0.0;
  result_expected(0, 2) = 0.0;
  result_expected(1, 0) = 0.0;
  result_expected(1, 1) = 0.0;
  result_expected(1, 2) = 0.0;
  result_expected(2, 0) = 0.0;
  result_expected(2, 1) = 0.0;
  result_expected(2, 2) = 0.0;

  S21Matrix result = matrix_1 * multiplier;

  for (size_t i = 0; i < result.get_rows(); ++i) {
    for (size_t j = 0; j < result.get_columns(); ++j) {
      EXPECT_EQ(result(i, j), result_expected(i, j));
    }
  }
}

// TEST_F(S21MatrixTest, MultiplyEmptyMatrixNumber) {
//   S21Matrix empty_matrix = S21Matrix(0, 0);
//   try {
//     empty_matrix * 2.0;
//   } catch (...) {
//     std::cout << "caught error\n";
//   }
// }

// Тест для оператора +=
TEST_F(S21MatrixTest, AdditionAssignmentTest) {
  // Устанавливаем ожидаемый результат для сложения
  result_expected(0, 0) = 10.0;
  result_expected(0, 1) = 10.0;
  result_expected(0, 2) = 10.0;
  result_expected(1, 0) = 10.0;
  result_expected(1, 1) = 10.0;
  result_expected(1, 2) = 10.0;
  result_expected(2, 0) = 10.0;
  result_expected(2, 1) = 10.0;
  result_expected(2, 2) = 10.0;
  S21Matrix matrix_result = matrix_1;
  S21Matrix matrix_result_func = matrix_1;
  matrix_result += matrix_2;
  matrix_result_func.SumMatrix(matrix_2);

  for (size_t i = 0; i < result_expected.get_rows(); ++i) {
    for (size_t j = 0; j < result_expected.get_columns(); ++j) {
      EXPECT_EQ(matrix_result(i, j), result_expected(i, j));
      EXPECT_EQ(matrix_result_func(i, j), result_expected(i, j));
    }
  }
}

// // Тест на выброс исключения для оператора +=
// TEST_F(S21MatrixTest, AdditionAssignmentDifferentDimensions) {
//   S21Matrix matrix1(3, 3);
//   // Создаем матрицу другого размера
//   S21Matrix matrix2(2, 2);

//   try {
//     matrix1 += matrix2;
//   } catch (...) {
//     std::cout << "caught error\n";
//   }
// }

// Тест для сложения матриц
TEST_F(S21MatrixTest, AdditionTest) {
  for (size_t i = 0; i < result_expected.get_rows(); ++i) {
    for (size_t j = 0; j < result_expected.get_columns(); ++j) {
      result_expected(i, j) = 10;
    }
  }
  S21Matrix result = matrix_1 + matrix_2;

  for (size_t i = 0; i < result.get_rows(); ++i) {
    for (size_t j = 0; j < result.get_columns(); ++j) {
      EXPECT_EQ(result(i, j), result_expected(i, j));
    }
  }
}

// Тест 1: Тестирование присваивания матрицы другой
TEST_F(S21MatrixTest, AssignmentOperator) {
  matrix_3 = matrix_2;
  for (size_t i = 0; i < matrix_3.get_rows(); ++i) {
    for (size_t j = 0; j < matrix_3.get_columns(); ++j) {
      EXPECT_EQ(matrix_3(i, j), matrix_2(i, j))
          << "Ошибка на позиции (" << i << ", " << j << ")";
    }
  }
}

// Тест 2: Присвоение самой себе
TEST_F(S21MatrixTest, SelfAssignment) {
  matrix_1 = matrix_1;
  for (size_t i = 0; i < matrix_1.get_rows(); ++i) {
    for (size_t j = 0; j < matrix_1.get_columns(); ++j) {
      EXPECT_EQ(matrix_1(i, j), 1.0 * (i * matrix_1.get_columns() + j + 1))
          << "Ошибка на позиции (" << i << ", " << j << ")";
    }
  }
}

// Тест 3: Присвоение пустой матрицы
TEST_F(S21MatrixTest, AssignmentEmptyMatrix) {
  S21Matrix empty_matrix(0, 0);
  matrix_1 = empty_matrix;
  EXPECT_EQ(matrix_1.get_rows(), 0);
  EXPECT_EQ(matrix_1.get_columns(), 0);
}

// Тест 4: Правильное освобождение памяти
TEST_F(S21MatrixTest, MemoryManagement) {
  S21Matrix matrix_temp(3, 3);
  matrix_temp = matrix_2;
  // Убедитесь, что память была корректно освобождена
  EXPECT_EQ(matrix_temp.get_rows(), matrix_2.get_rows());
  EXPECT_EQ(matrix_temp.get_columns(), matrix_2.get_columns());
  for (size_t i = 0; i < matrix_temp.get_rows(); ++i) {
    for (size_t j = 0; j < matrix_temp.get_columns(); ++j) {
      EXPECT_EQ(matrix_temp(i, j), matrix_2(i, j))
          << "Ошибка на позиции (" << i << ", " << j << ")";
    }
  }
}

// Тест на равенство матриц
TEST_F(S21MatrixTest, EqualityTest) {
  // Ожидаем, что они равны
  EXPECT_TRUE(matrix_1 == matrix_equal_1);
  EXPECT_TRUE(matrix_1.EqMatrix(matrix_equal_1));
  // Ожидаем, что они не равны
  EXPECT_FALSE(matrix_1 == matrix_2);
  EXPECT_FALSE(matrix_1.EqMatrix(matrix_2));
}

// Тест на сравнение матриц с плавающей запятой
TEST_F(S21MatrixTest, FloatingPointEqualityTest) {
  S21Matrix matrix_d = S21Matrix(1, 1);
  S21Matrix matrix_e = S21Matrix(1, 1);
  // Значение чуть больше
  matrix_d(0, 0) = 1.0000001;
  // Ожидаемое значение
  matrix_e(0, 0) = 1.0;
  // Ожидаем, что результаты не равны из-за разницы большей EPSILON
  EXPECT_FALSE(matrix_d == matrix_e);
  EXPECT_FALSE(matrix_d.EqMatrix(matrix_e));

  // Уменьшение разницы между ними
  // Теперь в пределах EPSILON, Ожидаем, что результаты равны
  matrix_d(0, 0) = 1.00000005;
  EXPECT_TRUE(matrix_d == matrix_e);
  EXPECT_TRUE(matrix_d.EqMatrix(matrix_e));
}

TEST_F(S21MatrixTest, CalcComplements_Test) {
  // Задаем ожидаемый результат для matrix_3
  double expected_values[] = {19, -32, -1, -4, 76, 20, -21, -24, 11};
  for (size_t i = 0; i < result_expected.get_rows(); ++i) {
    for (size_t j = 0; j < result_expected.get_columns(); ++j) {
      result_expected(i, j) =
          expected_values[i * result_expected.get_columns() + j];
    }
  }

  S21Matrix complements = matrix_3.CalcComplements();
  // Проверяем размеры результирующей матрицы
  EXPECT_EQ(complements.get_rows(), matrix_3.get_rows());
  EXPECT_EQ(complements.get_columns(), matrix_3.get_columns());

  // Сравниваем дополнения с ожидаемыми значениями
  for (size_t i = 0; i < complements.get_rows(); ++i) {
    for (size_t j = 0; j < complements.get_columns(); ++j) {
      EXPECT_DOUBLE_EQ(complements(i, j), result_expected(i, j));
    }
  }
}

// Тест для проверки поведения конструктора с нулевыми размерами
TEST_F(S21MatrixTest, CopyConstructorWithZeroDimensions) {
  // Создаем матрицу с нулевыми размерами
  S21Matrix emptyMatrix(0, 0);

  // Создаем другую матрицу с использованием конструктора копирования
  S21Matrix copiedMatrix(emptyMatrix);

  // Проверяем, что указатель на матрицу равен nullptr
  ASSERT_EQ(copiedMatrix.get_matrix(), nullptr);
  // Проверяем, что количество строк и столбцов равно 0
  ASSERT_EQ(copiedMatrix.get_rows(), 0);
  ASSERT_EQ(copiedMatrix.get_columns(), 0);
}

// Тест для определителя 2x2
TEST_F(S21MatrixTest, Determinant2x2) {
  double a = 1.0, b = 2.0, c = 3.0, d = 4.0;
  double expected = -2.0;
  double result = s21_determinant_2x2(a, b, c, d);
  EXPECT_DOUBLE_EQ(result, expected);
}

// Тест для определителя 3x3
TEST_F(S21MatrixTest, Determinant3x3) {
  double expected_determinant = 0.0;
  double result = matrix_1.Determinant();
  EXPECT_DOUBLE_EQ(result, expected_determinant);
}

// Тест для матрицы 3x3 с нулевыми элементами
TEST_F(S21MatrixTest, DeterminantZeroMatrix) {
  S21Matrix zero_matrix(3, 3);
  double expected_determinant = 0.0;
  double result = matrix_1.Determinant();
  EXPECT_DOUBLE_EQ(result, expected_determinant);
}

// // Тест для не квадратной матрицы
// TEST_F(S21MatrixTest, DeterminantNonSquareMatrix) {
//   S21Matrix non_square_matrix(2, 3);
//   try {
//     non_square_matrix.Determinant();
//   } catch (...) {
//     std::cout << "caught error\n";
//   }
// }

// // Тест для пустой матрицы
// TEST_F(S21MatrixTest, DeterminantEmptyMatrix) {
//   S21Matrix empty_matrix(0, 0);
//   try {
//     empty_matrix.Determinant();
//   } catch (...) {
//     std::cout << "caught error\n";
//   }
// }

TEST_F(S21MatrixTest, GetRows) {
  // Проверяем, что количество строк равно 3
  EXPECT_EQ(matrix_1.get_rows(), 3);
}

TEST_F(S21MatrixTest, GetColumns) {
  // Проверяем, что количество столбцов равно 3
  EXPECT_EQ(matrix_1.get_columns(), 3);
}

TEST_F(S21MatrixTest, SetRows_ChangeRows) {
  // Устанавливаем новое количество строк
  matrix_1.set_rows(2);
  // Проверяем, что количество строк изменилось
  EXPECT_EQ(matrix_1.get_rows(), 2);
  // Количество столбцов не должно измениться
  EXPECT_EQ(matrix_1.get_columns(), 3);
}

TEST_F(S21MatrixTest, SetColumns_ChangeColumns) {
  // Устанавливаем новое количество столбцов
  matrix_1.set_columns(4);
  // Проверяем, что количество столбцов изменилось
  EXPECT_EQ(matrix_1.get_columns(), 4);
  // Количество строк не должно измениться
  EXPECT_EQ(matrix_1.get_rows(), 3);
}

TEST_F(S21MatrixTest, Resize_ValidSize) {
  // Изменяем размер матрицы на 4x5
  matrix_1.resize(4, 5);
  // Проверяем, что размеры изменены
  EXPECT_EQ(matrix_1.get_rows(), 4);
  EXPECT_EQ(matrix_1.get_columns(), 5);
}

// TEST_F(S21MatrixTest, Resize_ZeroRows) {
//   // Ожидаем выброс исключения при установке 0 строк
//   try {
//     matrix_1.resize(0, 3);
//   } catch (...) {
//     std::cout << "caught error\n";
//   }
// }

// TEST_F(S21MatrixTest, Resize_ZeroColumns) {
//   // Ожидаем выброс исключения при установке 0 столбцов
//   try {
//     matrix_1.resize(3, 0);
//   } catch (...) {
//     std::cout << "caught error\n";
//   }
// }

TEST_F(S21MatrixTest, Resize_ReducedSize) {
  // Уменьшаем размер матрицы на 2x2
  matrix_1.resize(2, 2);
  // Проверяем, что размеры изменены
  EXPECT_EQ(matrix_1.get_rows(), 2);
  EXPECT_EQ(matrix_1.get_columns(), 2);
}

TEST_F(S21MatrixTest, Resize_IncreasedSize) {
  // Увеличиваем размер матрицы на 4x4
  matrix_1.resize(4, 4);
  // Проверяем, что размеры изменены
  EXPECT_EQ(matrix_1.get_rows(), 4);
  EXPECT_EQ(matrix_1.get_columns(), 4);
}

TEST_F(S21MatrixTest, Resize_CopyOldValues) {
  // Сохраняем старое значение для проверки
  matrix_1.resize(2, 2);
  double old_value = matrix_1(0, 0);
  // Увеличиваем размер обратно
  matrix_1.resize(3, 3);
  // Проверяем, что старое значение все еще доступно
  EXPECT_EQ(matrix_1(0, 0), old_value);
}

TEST_F(S21MatrixTest, SetRows_SameValue) {
  // Устанавливаем то же количество строк
  matrix_1.set_rows(3);
  // Проверяем, что количество строк не изменилось
  EXPECT_EQ(matrix_1.get_rows(), 3);
}

TEST_F(S21MatrixTest, SetColumns_SameValue) {
  // Устанавливаем то же количество столбцов
  matrix_1.set_columns(3);
  // Проверяем, что количество столбцов не изменилось
  EXPECT_EQ(matrix_1.get_columns(), 3);
}

// Тест для обратимой матрицы
TEST_F(S21MatrixTest, InverseMatrix_SquareMatrix) {
  // Заполняем матрицу, которая имеет обратную
  S21Matrix square_matrix(2, 2);
  square_matrix(0, 0) = 4.0;
  square_matrix(0, 1) = 7.0;
  square_matrix(1, 0) = 2.0;
  square_matrix(1, 1) = 6.0;

  S21Matrix inverse_matrix = square_matrix.InverseMatrix();

  // Проверка на корректность полученной обратной матрицы
  EXPECT_DOUBLE_EQ(inverse_matrix(0, 0), 0.6);
  EXPECT_DOUBLE_EQ(inverse_matrix(0, 1), -0.7);
  EXPECT_DOUBLE_EQ(inverse_matrix(1, 0), -0.2);
  EXPECT_DOUBLE_EQ(inverse_matrix(1, 1), 0.4);
}

// // Тест на выбрасывание исключения для не квадратной матрицы
// TEST_F(S21MatrixTest, InverseMatrix_NonSquareMatrix) {
//   S21Matrix non_square_matrix(2, 3);  // Прямоугольная матрица
//   try {
//     non_square_matrix.InverseMatrix();
//   } catch (...) {
//     std::cout << "caught error\n";
//   }
// }

// // Тест на выбрасывание исключения для вырожденной матрицы
// TEST_F(S21MatrixTest, InverseMatrix_SingularMatrix) {
//   S21Matrix singular_matrix(2, 2);
//   singular_matrix(0, 0) = 1.0;
//   singular_matrix(0, 1) = 2.0;
//   singular_matrix(1, 0) = 2.0;
//   singular_matrix(1, 1) = 4.0;  // Определитель = 0

//   try {
//     singular_matrix.InverseMatrix();
//   } catch (...) {
//     std::cout << "caught error\n";
//   }
// }

// // Тест на выбрасывание исключения для пустой матрицы
// TEST_F(S21MatrixTest, InverseMatrix_EmptyMatrix) {
//   S21Matrix empty_matrix(0, 0);
//   try {
//     empty_matrix.InverseMatrix();
//   } catch (...) {
//     std::cout << "caught error\n";
//   }
// }

TEST_F(S21MatrixTest, CreateMinorMatrix) {
  S21Matrix minor = matrix_1.create_minor_matrix(0, 0);
  EXPECT_EQ(minor.get_rows(), 2);
  EXPECT_DOUBLE_EQ(minor(0, 0), 5.0);
  EXPECT_DOUBLE_EQ(minor(0, 1), 6.0);
  EXPECT_DOUBLE_EQ(minor(1, 0), 8.0);
  EXPECT_DOUBLE_EQ(minor(1, 1), 9.0);
}

// Тест для проверки умножения матрицы на число
TEST_F(S21MatrixTest, OperatorMulAssign_Scalar) {
  double scalar = 2.0;
  matrix_1 *= scalar;

  // Проверка результатов
  EXPECT_EQ(matrix_1(0, 0), 2.0);
  EXPECT_EQ(matrix_1(1, 1), 10.0);
  EXPECT_EQ(matrix_1(2, 2), 18.0);
}

// Тест для проверки умножения матрицы на другую матрицу
TEST_F(S21MatrixTest, OperatorMulAssign_Matrix) {
  // Инициализируем матрицу для умножения
  S21Matrix other(3, 3);
  // Единичная матрица
  double values_other[] = {1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0};
  for (size_t i = 0; i < other.get_rows(); ++i) {
    for (size_t j = 0; j < other.get_columns(); ++j) {
      other(i, j) = values_other[i * other.get_columns() + j];
    }
  }

  S21Matrix result = matrix_1;
  result *= other;
  S21Matrix result_func = matrix_equal_1;
  result_func.MulMatrix(other);

  // Проверяем результат умножения
  EXPECT_EQ(result(0, 0), 1.0);
  EXPECT_EQ(result(1, 1), 5.0);
  EXPECT_EQ(result(2, 2), 9.0);

  EXPECT_EQ(result_func(0, 0), 1.0);
  EXPECT_EQ(result_func(1, 1), 5.0);
  EXPECT_EQ(result_func(2, 2), 9.0);
}

// Тест для проверки умножения матрицы на 0
TEST_F(S21MatrixTest, OperatorMulAssign_ZeroValue) {
  matrix_1 *= 0.0;
  matrix_2.MulNumber(0.0);
  // Проверка всех остальных элементов
  for (size_t i = 0; i < matrix_1.get_rows(); ++i) {
    for (size_t j = 0; j < matrix_1.get_columns(); ++j) {
      EXPECT_EQ(matrix_1(i, j), 0.0);
      EXPECT_EQ(matrix_2(i, j), 0.0);
    }
  }
}

// Тест для проверки метода MulNumber
TEST_F(S21MatrixTest, MulNumber_Scalar) {
  double scalar = 3.0;
  matrix_1.MulNumber(scalar);

  EXPECT_EQ(matrix_1(0, 0), 3.0);
  EXPECT_EQ(matrix_1(1, 1), 15.0);
  EXPECT_EQ(matrix_1(2, 2), 27.0);
}

// Тест для доступа к элементам матрицы
TEST_F(S21MatrixTest, AccessingMatrixElements) {
  EXPECT_EQ(matrix_1(0, 0), 1.0);
  EXPECT_EQ(matrix_1(0, 1), 2.0);
  EXPECT_EQ(matrix_1(0, 2), 3.0);
  EXPECT_EQ(matrix_1(1, 0), 4.0);
  EXPECT_EQ(matrix_1(1, 1), 5.0);
  EXPECT_EQ(matrix_1(1, 2), 6.0);
  EXPECT_EQ(matrix_1(2, 0), 7.0);
  EXPECT_EQ(matrix_1(2, 1), 8.0);
  EXPECT_EQ(matrix_1(2, 2), 9.0);
}

TEST_F(S21MatrixTest, AccesingConstMatrixElements) {
  EXPECT_EQ(const_matrix(0, 0), 1.0);
  EXPECT_EQ(const_matrix(0, 1), 2.0);
  EXPECT_EQ(const_matrix(0, 2), 3.0);
  EXPECT_EQ(const_matrix(1, 0), 4.0);
  EXPECT_EQ(const_matrix(1, 1), 5.0);
  EXPECT_EQ(const_matrix(1, 2), 6.0);
  EXPECT_EQ(const_matrix(2, 0), 7.0);
  EXPECT_EQ(const_matrix(2, 1), 8.0);
  EXPECT_EQ(const_matrix(2, 2), 9.0);
}

// // Тест на выброс исключения для не существующих элементов
// TEST_F(S21MatrixTest, OutOfRangeAccess) {
//   // Выход за пределы по строке
//   try {
//     matrix_1(3, 0);
//   } catch (...) {
//     std::cout << "caught error\n";
//   }
//   // Выход за пределы по столбцу
//   try {
//     matrix_1(0, 3);
//   } catch (...) {
//     std::cout << "caught error\n";
//   }
//   // Выход за пределы по строке и столбцу
//   try {
//     matrix_1(3, 3);
//   } catch (...) {
//     std::cout << "caught error\n";
//   }
// }

// TEST_F(S21MatrixTest, OutOfRangeAccess_ConstMatrix) {
//   try {
//     const_matrix(3, 0);
//   } catch (...) {
//     std::cout << "caught error\n";
//   }
//   try {
//     const_matrix(0, 3);
//   } catch (...) {
//     std::cout << "caught error\n";
//   }
//   try {
//     const_matrix(3, 3);
//   } catch (...) {
//     std::cout << "caught error\n";
//   }
// }

TEST_F(S21MatrixTest, Transpose_matrix_Test) {
  // Задаем ожидаемый результат для matrix_3
  double expected_values[] = {1, 4, 7, 2, 5, 8, 3, 6, 9};
  for (size_t i = 0; i < result_expected.get_rows(); ++i) {
    for (size_t j = 0; j < result_expected.get_columns(); ++j) {
      result_expected(i, j) =
          expected_values[i * result_expected.get_columns() + j];
    }
  }

  S21Matrix transposed_matrix = matrix_1.Transpose();
  // Проверяем размеры результирующей матрицы
  EXPECT_EQ(transposed_matrix.get_rows(), matrix_1.get_columns());
  EXPECT_EQ(transposed_matrix.get_columns(), matrix_1.get_rows());

  // Сравниваем дополнения с ожидаемыми значениями
  for (size_t i = 0; i < transposed_matrix.get_rows(); ++i) {
    for (size_t j = 0; j < transposed_matrix.get_columns(); ++j) {
      EXPECT_DOUBLE_EQ(transposed_matrix(i, j), result_expected(i, j));
    }
  }
}