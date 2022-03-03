/*
 * Matrix.h
 *
 *  Created on: Dec 16, 2021
 *      Author: matth
 */

#ifndef FILTERING_MATRIX_H_
#define FILTERING_MATRIX_H_

#define ARM_MATH_CM7

#if FCB_VERSION == 0
#include "stm32h750xx.h"
#elif FCB_VERSION == 1
#include "stm32h743xx.h"
#endif

#include "arm_math.h"
#include <initializer_list>

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
	Matrix() {
		arm_mat_init_f32(&m_matrix, Rows, Cols, m_backingArray);
	}

	template <int N>
	Matrix(const float32_t(&list)[N]) {
		static_assert(N==Rows*Cols, "Array must be the same length as the matrix!");
		memcpy(m_backingArray, list, Rows*Cols*sizeof(float32_t));
		arm_mat_init_f32(&m_matrix, Rows, Cols, m_backingArray);
	}

	template<int N>
	Matrix<Rows, N> operator*(const Matrix<Cols, N> &other) {
		Matrix<Rows, N> ret = {};
		arm_mat_mult_f32(&this->m_matrix, &other.m_matrix, &ret.m_matrix);
		return ret;
	}
	
	Matrix<Rows, Cols> operator+(const Matrix<Rows, Cols> &other) {
		Matrix<Rows, Cols> ret = {};
		arm_mat_add_f32(&this->m_matrix, &other.m_matrix, &ret.m_matrix);
		return ret;
	}

	Matrix<Rows, Cols> operator-(const Matrix<Rows, Cols> &other) {
		Matrix<Rows, Cols> ret = {};
		arm_mat_sub_f32(&this->m_matrix, &other.m_matrix, &ret.m_matrix);
		return ret;
	}

	float32_t operator()(const int row, const int col) {
		// Offset start of backing array by the size of a row
		// times however many rows we want to go to
		// plus the individual column index
		return m_backingArray[row * Cols + col];
	}

	Matrix<Rows, Rows> inverse() {
		static_assert(Rows == Cols, "Matrix must be square to invert!");
		Matrix<Rows, Rows> ret = {};
		arm_mat_inverse_f32(&this->m_matrix, &ret.m_matrix);
		return ret;
	}

	float32_t norm() {
		static_assert(Rows == 1, "Must be a single-row vector");
		float32_t square_sum;
		arm_power_f32(m_backingArray, Cols, &square_sum);
		return sqrt(square_sum);
	}

	static Matrix<Rows, Rows> identity() {
		static_assert(Rows == Cols, "Identity matrix must be square!");
		Matrix<Rows, Rows> ret = {};
		float32_t ident[Rows][Rows] = {0.0};
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

	float32_t trace() {
		static_assert(Rows == Cols, "Matrix must be square to find trace!");
		float32_t tr = 0;
		for (int i = 0; i < Rows; ++i) {
			tr += m_backingArray[i * Cols];
		}
		return tr;
	}

	arm_matrix_instance_f32 m_matrix{};
	float32_t m_backingArray[Rows * Cols] = {0};
};


#endif /* FILTERING_MATRIX_H_ */
