/*
 * circular_buffer.c
 */

#include "circular_buffer.h"

#include <string.h>

void cb_init(CircularBuffer_s *cb, unknownPtr_t buffer, size_t capacity,
             size_t size) {
  if (cb == NULL) {
    return;
  }

  cb->buffer = buffer;
  cb->size = size;

  cb->buffer_end = cb->buffer + capacity * cb->size;
  cb->head = cb->buffer;
  cb->tail = cb->buffer;
}

void cb_peek(CircularBuffer_s *cb, unknownPtr_t outputBuffer,
             size_t *numElements) {
  if (cb == NULL || outputBuffer == NULL) return;

  size_t count = cb_count(cb);
  if (numElements == NULL)
    numElements = &(count);
  else if (*numElements > count)
    *numElements = count;

  if (cb->tail + *numElements * cb->size <= cb->buffer_end) {
    // One copy to outputBuffer needed
    memcpy(outputBuffer, cb->tail, *numElements * cb->size);
  } else {
    // Two copies to outputBuffer needed
    size_t bytesToEnd = (size_t)cb->buffer_end - (size_t)cb->tail;
    memcpy(outputBuffer, cb->tail, bytesToEnd);
    memcpy(outputBuffer + bytesToEnd, cb->buffer,
           *numElements * cb->size - bytesToEnd);
  }
}

size_t cb_count(CircularBuffer_s *cb) {
  if (!cb) {
    return 0;
  }

  size_t ret_bytes = 0;
  if (cb->head < cb->tail) {
    ret_bytes += (size_t)(cb->buffer_end - cb->tail + cb->head - cb->buffer);
  } else {
    ret_bytes = (size_t)cb->head - (size_t)cb->tail;
  }

  size_t size = cb->size;
  if (size == 0) return 0;
  return ret_bytes / cb->size;
}

size_t cb_capacity(CircularBuffer_s *cb) {
  return ((size_t)(cb->buffer_end - cb->buffer) / cb->size) - (size_t)1u;
}

bool cb_enqueue(CircularBuffer_s *cb, const unknownPtr_t item) {
  // Figure out what next spot to fill in buffer is
  unknownPtr_t next = cb->head + cb->size;
  if (next >= cb->buffer_end) {
    next = cb->buffer;
  }

  // Don't let buffer overflow
  if (next == cb->tail) {
    return false;
  }

  // Copy item to buffer
  memcpy(cb->head, item, cb->size);
  cb->head = next;
  return true;
}

void cb_dequeue(CircularBuffer_s *cb, size_t numElements) {
  size_t count = cb_count(cb);
  if (numElements > count) {
    numElements = count;
  }

  cb->tail += cb->size * numElements;
  if (cb->tail >= cb->buffer_end) {
    cb->tail = cb->buffer + (cb->tail - cb->buffer_end);
  }
}

bool cb_full(CircularBuffer_s *cb) {
  unknownPtr_t next = cb->head + cb->size;
  if (next >= cb->buffer_end) {
    next = cb->buffer;
  }

  return next == cb->tail;
}

void cb_flush(CircularBuffer_s *cb) { cb->tail = cb->head; }
