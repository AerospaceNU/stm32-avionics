#ifndef COMMON_UTILS_CPP_CIRCULAR_BUFFER_H_
#define COMMON_UTILS_CPP_CIRCULAR_BUFFER_H_

#include <cstddef>
#include <cstring>

using std::memcpy;
using std::size_t;

/**
 * Implementation of a thread-safe circular buffer.
 * @tparam Type: The type to be stored in the circular buffer
 * @tparam Capacity: The maximum capacity (number of elements) to allocate in
 * the buffer.
 */
template <typename Type, size_t Capacity>
class CircularBuffer final {
 private:
  Type backingArray[Capacity + 1];
  const Type* endPtr;
  Type* head;
  Type* tail;
  size_t MaxCapacity;

  /**
   * Helper method to increment a pointer (head or tail) by a given amount.
   * Precondition: Incrementing ptr by count should be valid, i.e. there
   * should be remaining capacity in the buffer for that increment. This method
   * will not check for this.
   *
   * @param ptr: Pointer to the pointer to increment;
   * @param count: The number of elements to increment by.
   */
  void incrementPointer(Type** ptr, size_t count) {
    *ptr += count;
    if (*ptr > this->endPtr) {
      *ptr = this->backingArray + (*ptr - this->endPtr - 1);
    }
  }

 public:
  /**
   * Construct a CircularBuffer and initialize necessary instance variables.
   */
  CircularBuffer() {
    this->head = this->backingArray;
    this->tail = this->backingArray;
    this->MaxCapacity = Capacity + 1;
    this->endPtr = backingArray + Capacity;
  }

  /**
   * Destructor..
   */
  ~CircularBuffer() = default;

  /**
   * Empty the buffer, removing all current elements.
   */
  void flush() { this->tail = this->head; }

  /**
   * Copy the first count elements into the destination array.
   *
   * @param destination: Destination pointer to copy elements into.
   * @param count: The number of elements to copy into the destination.
   *
   * @return bool: Status, true if peek succeeds, false otherwise (i.e. if
   * destination is null or if the buffer does not have count elements.
   */
  bool peek(Type* destination, size_t count) {
    if (destination == nullptr) {
      return false;
    }
    if (count > this->count()) {
      return false;
    }
    if (this->head + count <= endPtr) {
      memcpy(destination, this->head, sizeof(Type) * count);
      return true;
    } else {
      size_t firstCount = 1 + this->endPtr - this->head;
      memcpy(destination, this->head, sizeof(Type) * firstCount);
      memcpy(destination + firstCount, this->backingArray,
             sizeof(Type) * count - firstCount);
      return true;
    }
  }

  /**
   * Remove count elements from the front of the buffer.
   *
   * @param count: Number of elements to remove from the buffer.
   *
   * @return bool: Status, true if dequeue succeeds, false if the buffer does
   * not have count elements to remove.
   */
  bool dequeue(size_t count) {
    if (count > this->count()) {
      return false;
    }
    this->incrementPointer(&(this->head), count);
    return true;
  }

  /**
   * Insert a new element into the buffer.
   *
   * @param item: The element to insert into the buffer.
   *
   * @return bool: Status, true if enqueue succeeds, false if the item cannot be
   * inserted.
   */
  bool enqueue(Type item) {
    if (this->full()) {
      return false;
    }
    *this->tail = item;
    this->incrementPointer(&(this->tail), 1);
    return true;
  }

  /**
   * Get the current number of elements in the buffer.
   *
   * @return size_t: The number of elements currently in the buffer.
   */
  size_t count() {
    if (this->head == this->tail) {
      return 0;
    } else if (this->tail > this->head) {
      return (this->tail - this->head);
    } else {
      return Capacity - (this->head - this->tail - 1);
    }
  }

  /**
   * Check if the buffer is currently full or not.
   *
   * @return bool: True if buffer is full, false otherwise.
   */
  bool full() { return this->count() == Capacity; }
};

#endif /* COMMON_UTILS_CPP_CIRCULAR_BUFFER_H_ */
