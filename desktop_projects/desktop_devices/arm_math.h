#ifndef DESKTOP_PROJECTS_DESKTOP_DEVICES_ARM_MATH_H_
#define DESKTOP_PROJECTS_DESKTOP_DEVICES_ARM_MATH_H_

#include <stdint.h>

typedef float float32_t;
typedef struct {
  uint16_t numRows; /**< number of rows of the matrix.     */
  uint16_t numCols; /**< number of columns of the matrix.  */
  float32_t *pData; /**< points to the data of the matrix. */
} arm_matrix_instance_f32;

#endif  // DESKTOP_PROJECTS_DESKTOP_DEVICES_ARM_MATH_H_
