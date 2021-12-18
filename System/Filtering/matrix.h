/*
 * Matrix.h
 *
 *  Created on: Dec 16, 2021
 *      Author: matth
 */

#ifndef FILTERING_MATRIX_H_
#define FILTERING_MATRIX_H_

#define ARM_MATH_CM7
#include "stm32h750xx.h"
#include "arm_math.h"

template <int Rows, int Cols>
class Matrix {
public:
	Matrix() {
		  arm_mat_init_f32(&m_matrix, Rows, Cols, m_backingArray);
	}

	template<int N>
	Matrix<Rows, N> operator*(const Matrix<Cols, N> &other) {
		return Dot(other);
	}

	template<int N>
	Matrix<Rows, N> Dot(const Matrix<Cols, N>& other) {
		Matrix<Rows, N> ret = {};
		arm_mat_mult_f32(&this->m_matrix, &other.m_matrix, &ret.m_matrix);
		return ret;
	}

	arm_matrix_instance_f32 m_matrix = {};
	float32_t m_backingArray[Rows * Cols] = {};
};


#endif /* FILTERING_MATRIX_H_ */
