/*
 * Matrix.h
 *
 *  Created on: Dec 16, 2021
 *      Author: matth
 */

#ifndef COMMON_UTILS_FCB_MATRIX_H_
#define COMMON_UTILS_FCB_MATRIX_H_

#include "board_config_common.h"

#ifdef STM32H7
#define ARM_MATH_CM7
#endif  // STM32h7
#ifdef STM32L4
#define ARM_MATH_CM4
#endif  // STM32h7

#include <cmath>
#include <cstring>
#include <initializer_list>

// Board config needs to be above arm_math to include the hal header first
#ifdef USE_ARM_MATH
#include "arm_math.h"
#else
#define float32_t float_t
#endif

/**
 * @brief Holds an arm matrix internally, with wrappers for common operations.
 *
 * @tparam Rows
 * @tparam Cols
 */
template <int Rows, int Cols>
class Matrix {
 public:
#ifdef USE_ARM_MATH
  arm_matrix_instance_f32 m_matrix{};
#endif
  float32_t m_backingArray[Rows * Cols] = {0};

  /**
   * @brief Constructs a zero-valued matrix
   */
  Matrix() {
#ifdef USE_ARM_MATH
#ifdef __arm__
    arm_mat_init_f32(&m_matrix, Rows, Cols, m_backingArray);
#endif  // __arm__
#endif  // USE_ARM_MATH
  }

  /**
   * @brief Constructs a zero-valued matrix
   * @param list: Initializer list to initialize matrix values with
   * @tparam N: Length of list, must be equal to number of elements in the
   * matrix
   */
  template <int N>
  explicit Matrix(const float (&list)[N]) {
    static_assert(N == Rows * Cols,
                  "Array must be the same length as the matrix!");
    memcpy(m_backingArray, list, Rows * Cols * sizeof(float));
#ifdef USE_ARM_MATH
#ifdef __arm__
    arm_mat_init_f32(&m_matrix, Rows, Cols, m_backingArray);
#endif  // __arm__
#endif
  }

  /**
   * @brief Destructor for matrix
   */
  ~Matrix() {}

  /**
   * @brief Matrix multiplication operator
   * @param other: Matrix to multiply by this matrix
   * @tparam oRows: Rows of other matrix, must be equal to cols of this matrix
   * @tparam oCols: Columns of other matrix
   * @return New matrix from the multiplication, Rows * Columns of other
   */
  template <int oRows, int oCols>
  Matrix<Rows, oCols> operator*(const Matrix<oRows, oCols> &other) {
    static_assert(Cols == oRows,
                  "Must multiply matrices with matching column and row count "
                  "(a x n * n x b)");
    float newElems[Rows * oCols] = {0};
    for (int i = 0; i < Rows; ++i) {
      for (int j = 0; j < oCols; ++j) {
        for (int k = 0; k < Cols; ++k) {
          newElems[i * oCols + j] += this->m_backingArray[i * Cols + k] *
                                     other.m_backingArray[k * oCols + j];
        }
      }
    }
    Matrix<Rows, oCols> ret(newElems);
    return ret;
  }

  /**
   * @brief Elementwise multiplication by a constant factor
   * @param factor: Constant factor to multiply by this matrix
   * @return New matrix from the multiplication
   */
  Matrix<Rows, Cols> operator*(const float factor) {
    float newElems[Rows * Cols] = {0};
    for (int i = 0; i < Rows * Cols; ++i) {
      newElems[i] = m_backingArray[i] * factor;
    }
    Matrix<Rows, Cols> ret(newElems);
    return ret;
  }

  /**
   * @brief Elementwise division by a constant factor
   * @param factor: Constant factor to divide this matrix by
   * @return New matrix from the division
   */
  Matrix<Rows, Cols> operator/(const float factor) {
    return this->operator*(1.0f / factor);
  }

  /**
   * @brief Elementwise matrix multiplication operator
   * @param other: Matrix to multiply by this matrix
   * @tparam oRows: Rows of other matrix, must be equal to rows of this matrix
   * @tparam oCols: Columns of other matrix, must be equal to columns of this
   * matrix
   * @return New matrix from the multiplication
   */
  template <int oRows, int oCols>
  Matrix<Rows, Cols> operator^(const Matrix<oRows, oCols> &other) {
    static_assert(Rows == oRows && Cols == oCols,
                  "Matrices must have same dimension!");
    float newElems[Rows * Cols] = {0};
    for (int i = 0; i < Rows * Cols; ++i) {
      newElems[i] = m_backingArray[i] * other.m_backingArray[i];
    }
    Matrix<Rows, Cols> ret(newElems);
    return ret;
  }

  /**
   * @brief Elementwise matrix addition operator
   * @param other: Matrix to add to this matrix
   * @tparam oRows: Rows of other matrix, must be equal to rows of this matrix
   * @tparam oCols: Columns of other matrix, must be equal to columns of this
   * matrix
   * @return New matrix from the addition
   */
  template <int oRows, int oCols>
  Matrix<Rows, Cols> operator+(const Matrix<oRows, oCols> &other) {
    static_assert(Rows == oRows && Cols == oCols,
                  "Matrices must have same dimension!");
    float newElems[Rows * Cols] = {0};
    for (int i = 0; i < Rows; ++i) {
      for (int j = 0; j < Cols; ++j) {
        newElems[i * oCols + j] = this->m_backingArray[i * Cols + j] +
                                  other.m_backingArray[i * oCols + j];
      }
    }
    Matrix<Rows, Cols> ret(newElems);
    return ret;
  }

  /**
   * @brief Elementwise matrix subtraction operator
   * @param other: Matrix to subtract from this matrix
   * @tparam oRows: Rows of other matrix, must be equal to rows of this matrix
   * @tparam oCols: Columns of other matrix, must be equal to columns of this
   * matrix
   * @return New matrix from the subtraction
   */
  template <int oRows, int oCols>
  Matrix<Rows, Cols> operator-(const Matrix<oRows, oCols> &other) {
    static_assert(Rows == oRows && Cols == oCols,
                  "Matrices must have same dimension!");
    float newElems[Rows * Cols] = {0};
    for (int i = 0; i < Rows; ++i) {
      for (int j = 0; j < Cols; ++j) {
        newElems[i * oCols + j] = this->m_backingArray[i * Cols + j] -
                                  other.m_backingArray[i * oCols + j];
      }
    }
    Matrix<Rows, Cols> ret(newElems);
    return ret;
  }

  /**
   * @brief Index operator on this matrix
   * @param row: Row to get element from
   * @param col: Column to get element from
   * @return Element located at the specified row, column
   */
  float operator()(const int row, const int col) {
    // Offset start of backing array by the size of a row
    // times however many rows we want to go to
    // plus the individual column index
    return m_backingArray[row * Cols + col];
  }

  /**
   * @brief Inverse of this matrix
   */
  Matrix<Rows, Cols> inverse() {
    static_assert(Rows == Cols, "Matrix must be square to invert!");
#ifdef USE_ARM_MATH
#ifdef __arm__
    Matrix<Rows, Cols> ret = {};
    arm_mat_inverse_f32(&(this->m_matrix), &(ret.m_matrix));
    return ret;
#endif  // __arm__
#else
    // Not implemented yet
    assert(0);
#endif
  }

  /**
   * @brief Norm of this matrix
   * Must be a vector
   */
  float norm() {
    static_assert(Rows == 1 || Cols == 1, "Must be a 1D-vector");

    float square_sum = 0;
#ifdef USE_ARM_MATH
#ifdef __arm__
    arm_power_f32(m_backingArray, Rows == 1 ? Cols : Rows, &square_sum);
#endif
#else
    for (int elem = 0; elem < Rows * Cols; elem++) {
      square_sum += pow(m_backingArray[elem], 2);
    }
#endif  // USE_ARM_MATH

    return sqrt(square_sum);
  }

  /**
   * @brief Generates an identity matrix of the given dimension
   */
  static Matrix<Rows, Rows> identity() {
    static_assert(Rows == Cols, "Identity matrix must be square!");
    Matrix<Rows, Rows> ret = {};
    float ident[Rows][Rows] = {0.0};
    for (int i = 0; i < Rows; ++i) {
      ident[i][i] = 1.0;
    }
    memcpy(ret.m_backingArray, &ident, sizeof(ident));
#ifdef USE_ARM_MATH
#ifdef __arm__
    arm_mat_init_f32(&(ret.m_matrix), Rows, Rows, ret.m_backingArray);
#endif  // __arm__
#endif
    return ret;
  }

  /**
   * @brief Trace of a square matrix
   */

  float trace() {
    static_assert(Rows == Cols, "Matrix must be square to find trace!");
    float tr = 0;
    for (int i = 0; i < Rows; ++i) {
      tr += m_backingArray[i * Cols];
    }
    return tr;
  }

  /**
   * @brief Generates a zero-filled matrix of the given dimension
   */
  static Matrix<Rows, Cols> zeroes() {
    Matrix<Rows, Cols> ret = {};
    float32_t zeroes[Rows][Cols] = {0.0};
    memcpy(ret.m_backingArray, &zeroes, sizeof(zeroes));
#ifdef USE_ARM_MATH
#ifdef __arm__
    arm_mat_init_f32(ret.m_matrix, Rows, Cols, ret.m_backingArray);
#endif  // __arm__
#endif
    return ret;
  }

  /**
   * @brief Calculate elementwise cosine of the values in the given matrix
   */
  static Matrix<Rows, Cols> cos(const Matrix<Rows, Cols> &start) {
    float32_t values[Rows * Cols] = {0};
    for (int i = 0; i < Rows * Cols; ++i) {
      values[i] = arm_cos_f32(start.m_backingArray[i]);
    }
    return Matrix<Rows, Cols>(values);
  }

  /**
   * @brief Calculate elementwise sine of the values in the given matrix
   */
  static Matrix<Rows, Cols> sin(const Matrix<Rows, Cols> &start) {
    float32_t values[Rows * Cols] = {0};
    for (int i = 0; i < Rows * Cols; ++i) {
      values[i] = arm_sin_f32(start.m_backingArray[i]);
    }
    return Matrix<Rows, Cols>(values);
  }
};

#endif  // COMMON_UTILS_FCB_MATRIX_H_
