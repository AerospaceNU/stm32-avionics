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
    size_t countToEnd = 1 + this->endPtr - this->head;
    if (countToEnd >= count) {
      memcpy(destination, this->head, sizeof(Type) * count);
      return true;
    } else {
      memcpy(destination, this->head, sizeof(Type) * countToEnd);
      memcpy(destination + countToEnd, this->backingArray,
             sizeof(Type) * (count - countToEnd));
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

  float get_sum(){
    Type* tailTracker = this->head;
	  float sum = 0;
	  while (tailTracker != this->tail){
		  sum += *tailTracker;
		  this->incrementPointer(&tailTracker,1);
	  }
    return sum;
  }


  float get_avg(){
	  Type* tailTracker = this->head;
	  float sum = 0;
	  while (tailTracker != this->tail){
		  sum += *tailTracker;
		  this->incrementPointer(&tailTracker,1);
	  }
	  float avg = sum / count();
	  return avg;
  }

  float get_min(){
	  Type* tailTracker = this->head;
	  float min = *tailTracker;
	  while (tailTracker != this->tail){
		  float new_val = *tailTracker;
		  if (new_val < min){
			  min = new_val;
		  }
		  this->incrementPointer(&tailTracker,1);
	  }
	  return min;
  }
float get_max(){
	  Type* tailTracker = this->head;
	  float max = *tailTracker;
	  while (tailTracker != this->tail){
		  float new_val = *tailTracker;
		  if (new_val > max){
			  max = new_val;
		  }
		  this->incrementPointer(&tailTracker,1);
	  }
	  return max;
  }

float get_med(){
Type* tailTracker = this->head;
int length = count();
float arr[length];
int i = 0;
float med = 0;
while (tailTracker != this->tail){
float cur_val = *tailTracker;
arr[i] = cur_val;
arr[i] = *tailTracker;
  this->incrementPointer(&tailTracker,1);
  i += 1;
    }
for (int i = 0; i < length; ++i) {
        for (int j = 0; j < length - 1 - i; ++j) {
            if (arr[j] > arr[j + 1]) {
                // Swap arr[j] and arr[j + 1]
                float temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
if (length % 2 == 1)  med = arr[length / 2];
else  med = (arr[length / 2 - 1] + arr[length / 2]) / 2;
return med;
}
  /**
   * Check if the buffer is currently full or not.
   *
   * @return bool: True if buffer is full, false otherwise.
   */

   
  bool full() const { return this->count() == Capacity; }
};

#endif /* COMMON_UTILS_CPP_CIRCULAR_BUFFER_H_ */
