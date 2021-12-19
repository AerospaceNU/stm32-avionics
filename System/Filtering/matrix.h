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


	Matrix<Rows, Rows> inverse() {
		static_assert(Rows == Cols, "Matrix must be square to invert!");
		Matrix<Rows, Rows> ret = {};
		arm_mat_inverse_f32(&this->m_matrix, &ret.m_matrix);
		return ret;
	}

	arm_matrix_instance_f32 m_matrix = {};
	float32_t m_backingArray[Rows * Cols] = {};
};


#endif /* FILTERING_MATRIX_H_ */
