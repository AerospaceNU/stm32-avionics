/*
 * circular_stat_buffer.h
 */

#ifndef UTILS_CIRCULAR_STAT_BUFFER_H_
#define UTILS_CIRCULAR_STAT_BUFFER_H_

#include <cmath>
#include <cstring>

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "circular_buffer.h"

/**
 * Wrapper for running-buffers in filters
 */
template <typename T>
class CircularStatBuffer {
 private:
  T backingArray;
  CircularBuffer_s circBuffer;
  uint8_t currentSize;
  uint8_t capacity;
  size_t elementSize;

 public:
  /**
   * Average of values in running buffer.
   */
  T csb_average();

  /**
   * Median of values in running buffer.
   */
  T csb_median(T* input);

  /**
   * Empty the running buffer.
   */
  void csb_flush();

  /**
   * Add a new element to the running buffer.
   */
  bool csb_add_new_element(T newElem);

  /**
   * Return the element at the provided index.
   */
  T csb_get_element_at(uint8_t index);

  /**
   * Returns the number of elements currently of the running buffer.
   */
  uint8_t csb_cur_size();

  /**
   * Returns the sum of the elements in the running buffer.
   */
  T csb_sum();
};

#ifdef __cplusplus
}
#endif

#endif  // UTILS_CIRCULAR_STAT_BUFFER_H_
