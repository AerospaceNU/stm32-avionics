/*
 * circular_stat_buffer.c
 */

#include "circular_stat_buffer.h"

template <typename T>
T CircularStatBuffer<T>::csb_average() {
  T avg = 0;

  for (int i = 0; i < this->currentSize; i++) {
    avg += this->csb_get_element_at(i);
  }
  avg / this->currentSize;

  return avg;
}

template <typename T>
T CircularStatBuffer<T>::csb_median(T* input, uint8_t count) {
  // Insertion sort
  int8_t i, j;
  double tmp;
  for (i = 1; i < count; ++i) {
    tmp = input[i];
    j = i - 1;
    while (j >= 0 && tmp <= input[j]) {
      input[j + 1] = input[j];
      --j;
    }
    input[j + 1] = tmp;
  }

  if (count % 2 == 0) {
    return (input[count / 2 - 1] + input[count / 2]) / 2.0;
  } else {
    return input[count / 2];
  }
}

template <typename T>
void CircularStatBuffer<T>::csb_flush() {
  // set current size to 0
  this->currentSize = 0;
  // flush buffer
  cb_flush(this->circBuffer);
  // flush backing array
  memset(this->backingArray, 0, sizeof this->backingArray);
}

template <typename T>
bool CircularStatBuffer<T>::csb_add_new_element(T newElem) {
  // check if full
  if (this->currentSize == this->capacity) {
    return false;
  }

  // try to add to circ buffer
  if (!cb_enqueue(this->circBuffer, newElem)) {
    return false;
  }

  // add to backing array
  this->backingArray[this->currentSize] = newElem;

  this->currentSize++;
  return true;
}

template <typename T>
T CircularStatBuffer<T>::csb_get_element_at(uint8_t index) {
  return this->backingArray[index];
}

template <typename T>
uint8_t CircularStatBuffer<T>::csb_cur_size() {
  return this->currentSize;
}

template <typename T>
T CircularStatBuffer<T>::csb_sum() {
  int sum = 0;
  for (int i = 0; i < this->csb_cur_size(); i++) {
    sum += this->csb_get_element_at(i);
  }

  return sum;
}
