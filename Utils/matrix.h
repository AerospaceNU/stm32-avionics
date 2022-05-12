/*
 * Matrix.h
 *
 *  Created on: Dec 16, 2021
 *      Author: matth
 */

#ifndef UTILS_MATRIX_H_
#define UTILS_MATRIX_H_

#define ARM_MATH_CM7

#if FCB_VERSION == 0
#include "stm32h750xx.h"
#elif FCB_VERSION == 1
#include "stm32h743xx.h"
#endif

#include <initializer_list>

#include "arm_math.h"

/**
 * @brief Holds an arm matrix internally, with wrappers for common operations.
 * Currently supported:
 * +-, dot, inverse
 *
 * @tparam Rows
 * @tparam Cols
 */
template <int Rows, int Cols>
class Matrix {
 public:
  Matrix() { arm_mat_init_f32(&m_matrix, Rows, Cols, m_backingArray); }

  template <int N>
  explicit Matrix(const float (&list)[N]) {
    static_assert(N == Rows * Cols,
                  "Array must be the same length as the matrix!");
    memcpy(m_backingArray, list, Rows * Cols * sizeof(float));
    arm_mat_init_f32(&m_matrix, Rows, Cols, m_backingArray);
  }

  template <int oRows, int oCols>
  Matrix<Rows, oCols> operator*(const Matrix<oRows, oCols> &other) {
    static_assert(Cols == oRows,
                  "Must multiply matrices with matching column and row count "
                  "(a x n * n x b)");
    Matrix<Rows, oCols> ret = {};
    arm_mat_mult_f32(&this->m_matrix, &other.m_matrix, &ret.m_matrix);
    return ret;
  }

  Matrix<Rows, Cols> operator*(const float factor) {
    float newElems[Rows * Cols] = {0};
    for (int i = 0; i < Rows * Cols; ++i) {
      newElems[i] = m_backingArray[i] * factor;
    }
    Matrix<Rows, Cols> ret(newElems);
    return ret;
  }

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

  template <int oRows, int oCols>
  Matrix<Rows, Cols> operator+(const Matrix<oRows, oCols> &other) {
    static_assert(Rows == oRows && Cols == oCols,
                  "Matrices must have same dimension!");
    Matrix<Rows, Cols> ret = {};
    arm_mat_add_f32(&this->m_matrix, &other.m_matrix, &ret.m_matrix);
    return ret;
  }

  template <int oRows, int oCols>
  Matrix<Rows, Cols> operator-(const Matrix<oRows, oCols> &other) {
    static_assert(Rows == oRows && Cols == oCols,
                  "Matrices must have same dimension!");
    Matrix<Rows, Cols> ret = {};
    arm_mat_sub_f32(&this->m_matrix, &other.m_matrix, &ret.m_matrix);
    return ret;
  }

  float operator()(const int row, const int col) {
    // Offset start of backing array by the size of a row
    // times however many rows we want to go to
    // plus the individual column index
    return m_backingArray[row * Cols + col];
  }

  template <int oRows, int oCols>
  Matrix<Rows, Cols> inverse() {
    static_assert(Rows == Cols, "Matrix must be square to invert!");
    Matrix<Rows, Cols> ret = {};
    arm_mat_inverse_f32(&this->m_matrix, &ret.m_matrix);
    return ret;
  }

  float norm() {
    static_assert(Rows == 1 || Cols == 1, "Must be a 1D-vector");
    float square_sum;
    arm_power_f32(m_backingArray, Rows == 1 ? Cols : Rows, &square_sum);
    return sqrt(square_sum);
  }

  static Matrix<Rows, Rows> identity() {
    static_assert(Rows == Cols, "Identity matrix must be square!");
    Matrix<Rows, Rows> ret = {};
    float ident[Rows][Rows] = {0.0};
    for (int i = 0; i < Rows; ++i) {
      ident[i][i] = 1.0;
    }
    memcpy(ret.m_backingArray, &ident, sizeof(ident));
    arm_mat_init_f32(&ret.m_matrix, Rows, Rows, ret.m_backingArray);
    return ret;
  }

  static Matrix<Rows, Cols> zeroes() {
    Matrix<Rows, Cols> ret = {};
    float32_t zeroes[Rows][Cols] = {0.0};
    memcpy(ret.m_backingArray, &zeroes, sizeof(zeroes));
    arm_mat_init_f32(&ret.m_matrix, Rows, Cols, ret.m_backingArray);
    return ret;
  }
  template <int oRows, int oCols>
  static Matrix<Rows, Cols> cos(const Matrix<oRows, oCols> &start) {
    static_assert(Rows == oRows && Cols == oCols,
                  "Matrices must have same dimension!");
    float32_t values[Rows * Cols] = {0};
    for (int i = 0; i < Rows * Cols; ++i) {
      values[i] = arm_cos_f32(start.m_backingArray[i]);
    }
    return Matrix<Rows, Cols>(values);
  }

  template <int oRows, int oCols>
  static Matrix<Rows, Cols> sin(const Matrix<oRows, oCols> &start) {
    static_assert(Rows == oRows && Cols == oCols,
                  "Matrices must have same dimension!");
    float32_t values[Rows * Cols] = {0};
    for (int i = 0; i < Rows * Cols; ++i) {
      values[i] = arm_sin_f32(start.m_backingArray[i]);
    }
    return Matrix<Rows, Cols>(values);
  }

  float trace() {
    static_assert(Rows == Cols, "Matrix must be square to find trace!");
    float tr = 0;
    for (int i = 0; i < Rows; ++i) {
      tr += m_backingArray[i * Cols];
    }
    return tr;
  }

  arm_matrix_instance_f32 m_matrix{};
  float32_t m_backingArray[Rows * Cols] = {0};
};

#endif  // UTILS_MATRIX_H_
