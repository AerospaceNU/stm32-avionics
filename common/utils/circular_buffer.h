/*
 * circular_buffer.h
 */

#ifndef COMMON_UTILS_CIRCULAR_BUFFER_H_
#define COMMON_UTILS_CIRCULAR_BUFFER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define unknownPtr_t uint8_t *

typedef struct {
  // We use
  unknownPtr_t buffer;      // data buffer
  unknownPtr_t buffer_end;  // end of data buffer
  size_t size;              // size of each item in the buffer
  unknownPtr_t head;        // pointer to head
  unknownPtr_t tail;
} CircularBuffer_s;

/**
 * @brief Initializes new circular buffer, with given buffer, capacity, and
 * element size
 * @param cb: Circular buffer to initialize
 * @param buffer: Pointer to existing buffer to manage
 * @param capacity: Number of elements the existing buffer can hold
 * @param size: Size of a buffer element, in bytes
 *
 * Note that circular buffer wraps an existing buffer. Memory of the existing
 * buffer must be allocated externally.
 */
void cb_init(CircularBuffer_s *cb, unknownPtr_t buffer, size_t capacity,
             size_t size);

/**
 * @brief Copies a given number of elements from front of circular buffer to the
 * given output buffer. Doesn't dequeue.
 * @param cb: Circular buffer to peek at
 * @param outputBuffer: Where to copy read values. Must have numElements *
 * cb->size bytes allocated already
 * @param numElements: [in] Max number of elements to read, or NULL if all
 * elements should be read. [out] Number of elements actually read
 */
void cb_peek(CircularBuffer_s *cb, unknownPtr_t outputBuffer,
             size_t *numElements);

/**
 * @brief Get the number of items currently in the circular buffer
 * @param cb: Circular buffer to get count of
 * @return Number of items in buffer
 */
size_t cb_count(CircularBuffer_s *cb);

/**
 * @brief Get the maximum number of items this buffer can hold
 * @param cb: Circular buffer to get capacity of
 * @return Max items it can hold
 */
size_t cb_capacity(CircularBuffer_s *cb);

/**
 * @brief Adds an item to the queue if there's room. If buffer is at capacity,
 * will not overwrite existing values.
 * @param cb: Circular buffer to enqueue
 * @param item: Item to enqueue. Assumed to be same byte size as initialized cb
 * size
 * @return: True if successful, false if buffer full
 */
bool cb_enqueue(CircularBuffer_s *cb, const unknownPtr_t item);

/**
 * @brief Removes a given number of elements from the front of the circular
 * buffer, or all elements if given number too large.
 * @param cb: Circular buffer to dequeue
 * @param numElements: Max number of elements to dequeue
 */
void cb_dequeue(CircularBuffer_s *cb, size_t numElements);

/**
 * @brief Return whether circular buffer is full
 * @param cb: Circular buffer to check
 * @return: True if circular buffer full, False otherwises
 */
bool cb_full(CircularBuffer_s *cb);

/**
 * @brief Delete the circular buffer's contents by resetting it
 * @param cb: Circular buffer to flush
 */
void cb_flush(CircularBuffer_s *cb);

#ifdef __cplusplus
}
#endif

#endif  // COMMON_UTILS_CIRCULAR_BUFFER_H_
