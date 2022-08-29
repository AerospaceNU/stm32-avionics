#include <stdint.h>
#pragma once

typedef float float32_t;
typedef struct {
  uint16_t numRows; /**< number of rows of the matrix.     */
  uint16_t numCols; /**< number of columns of the matrix.  */
  float32_t *pData; /**< points to the data of the matrix. */
} arm_matrix_instance_f32;
