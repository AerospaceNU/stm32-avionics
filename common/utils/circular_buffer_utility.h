#ifndef COMMON_UTILS_CIRCULAR_BUFFER_UTILITY_H_
#define COMMON_UTILS_CIRCULAR_BUFFER_UTILITY_H_

#include <cpp_circular_buffer.h>

#include <cstddef>

using std::size_t;

template <typename Type, size_t Capacity>
class CircularBufferUtility {
  CircularBuffer<Type, Capacity>* cb;

  /**
   * Insert a new element into the buffer and remove the oldest element if the
   * buffer is full.
   *
   * @param item: The element to insert into the buffer.
   * @return bool: Status, true if dequeue and enqueue succeeds, false if either
   * fails.
   */
  * / bool addElement(Type item) {
    if (this->cb->full()) {
      if (!this->cb->dequeue(1)) {
        return false;
      }
    }
    return this->cb->enqueue(item);
  }

  /**
   * Get the average of all elements in the buffer.
   *
   * @return Type: The average of all elements in the buffer.
   */
  Type average() {
    Type sum = this->sum();
    return sum / this->cb->count();
  }

  /**
   * Get the median of all elements in the buffer.
   *
   * @return Type: The median of all elements in the buffer.
   */
  Type median() {
    size_t size = this->cb->count();
    Type linearData[Capacity];

    for (size_t i = 0; i < size; i++) {
      linearData[i] = getNthValue(i);
    }

    std::sort(linearData, linearData + size);
    return linearData[size / 2];
  }

  /**
   * Get the Nth Value object
   *
   * @param destination
   * @param n
   * @return bool: Status, true if getting the n'th value succeeds, false
   * otherwise
   */
  bool getNthValue(Type* destination, int n) {
    if (n >= this->cb->count() || n < 0) {
      return false;
    }

    if (this->head + n <= endPtr) {
      memcpy(destination, this->head + n, sizeof(Type));
      return true;
    } else {
      memcpy(destination,
             this->backingArray + n - (1 + this->endPtr - this->head),
             sizeof(Type));
      return true;
    }
  }

  /**
   * Get the sum of all elements in the buffer.
   *
   * @return Type: The sum of all elements in the buffer.
   */
  Type sum() {
    Type sum = 0;
    for (size_t i = 0; i < this->cb->count(); i++) {
      Type item = this->getNthValue(i);
      sum += item;
    }
    return sum;
  }
};

#endif /* COMMON_UTILS_CIRCULAR_BUFFER_UTILITY_H_ */