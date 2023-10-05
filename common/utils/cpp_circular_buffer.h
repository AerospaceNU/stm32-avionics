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
class CircularBuffer {
 private:
  /**
   * To be interrupt-safe (at least to the extent that the previous CB
   * implementation is), the current size and element locations must only be
   * determined by the locations of the head and tail pointers. This is to
   * remove any dependence on multiple instructions being executed for the
   * buffer to be valid. For example, if the head pointer is moved to dequeue an
   * element, but a separate size variable is not changed before jumping to an
   * interrupt, the state of the buffer would be inconsistent.
   *
   * What this means is that, if only the head and tail pointers are used to
   * calculate current size, a full buffer and an empty buffer would be
   * identical (head == tail). As such, we need to allocate one extra element
   * from what the client requests so that a "full" buffer actually has one open
   * spot to avoid full vs empty issues.
   */
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
   * Virtual destructor.
   */
  virtual ~CircularBuffer() = default;

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
  bool peek(Type* destination, size_t count) const {
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
  size_t count() const {
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
  bool full() const { return this->count() == Capacity; }

  /**
   * Get the average of all elements in the buffer.
   * 
   * @return Type: The average of all elements in the buffer.
   */
  Type average() {
    Type sum = this->sum();
    return sum / this->count();
  }

  /**
   * Get the median of all elements in the buffer.
   * 
   * @return Type: The median of all elements in the buffer.
   */
  Type median() {
    size_t size = this->count();
    std::vector<Type> linearData(size);

    for (size_t i = 0; i < size; i++) {
      linearData[i] = getNthValue(i);
    }

    std::sort(linearData.begin(), linearData.end());
    return linearData[size / 2];
  }

  /**
   * Get the n'th elements in the buffer.
   * 
   * @return Type: The n'th element in the buffer.
   */
  Type getNthValue(int n) {
    if (this->head + n <= endPtr) {
      return *(this->head + n);
    } else {
      return *(this->backingArray + n - (1 + this->endPtr - this->head));
    }
  }

  /**
   * Get the sum of all elements in the buffer.
   * 
   * @return Type: The sum of all elements in the buffer.
   */
  Type sum() {
    Type sum = 0;
    for (size_t i = 0; i < this->count(); i++) {
      Type item = this->getNthValue(i);
      sum += item;
    }
    return sum;
  }
};

#endif /* COMMON_UTILS_CPP_CIRCULAR_BUFFER_H_ */
