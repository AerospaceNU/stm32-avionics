/*
 * circular_buffer.c
 */

#include "circular_buffer.h"

#include <string.h>

void cbInit(CircularBuffer_s *cb, void *buffer, size_t capacity, size_t size) {
  if (cb == NULL) {
    return;
  }

  cb->buffer = buffer;
  cb->size = size;

  cb->buffer_end = cb->buffer + capacity * cb->size;
  cb->head = cb->buffer;
  cb->tail = cb->buffer;
}

void cbPeek(CircularBuffer_s *cb, void *outputBuffer, size_t *numElements) {
  if (cb == NULL || outputBuffer == NULL) return;

  size_t count = cbCount(cb);
  if (numElements == NULL)
    numElements = &(count);
  else if (*numElements > count)
    *numElements = count;

  if (cb->tail + *numElements * cb->size <= cb->buffer_end) {
    // One copy to outputBuffer needed
    memcpy(outputBuffer, cb->tail, *numElements * cb->size);
  } else {
    // Two copies to outputBuffer needed
    size_t bytesToEnd = cb->buffer_end - cb->tail;
    memcpy(outputBuffer, cb->tail, bytesToEnd);
    memcpy(outputBuffer + bytesToEnd, cb->buffer,
           *numElements * cb->size - bytesToEnd);
  }
}

size_t cbCount(CircularBuffer_s *cb) {
  if (!cb) {
    return 0;
  }

  size_t ret_bytes = 0;
  if (cb->head < cb->tail) {
    ret_bytes += cb->buffer_end - cb->tail + cb->head - cb->buffer;
  } else {
    ret_bytes = cb->head - cb->tail;
  }
  return ret_bytes / cb->size;
}

size_t cbCapacity(CircularBuffer_s *cb) {
  return ((cb->buffer_end - cb->buffer) / cb->size) - 1;
}

bool cbEnqueue(CircularBuffer_s *cb, const void *item) {
  // Figure out what next spot to fill in buffer is
  void *next = cb->head + cb->size;
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

void cbDequeue(CircularBuffer_s *cb, size_t numElements) {
  size_t count = cbCount(cb);
  if (numElements > count) {
    numElements = count;
  }

  cb->tail += cb->size * numElements;
  if (cb->tail >= cb->buffer_end) {
    cb->tail = cb->buffer + (cb->tail - cb->buffer_end);
  }
}

bool cbFull(CircularBuffer_s *cb) {
  void *next = cb->head + cb->size;
  if (next >= cb->buffer_end) {
    next = cb->buffer;
  }

  return next == cb->tail;
}

void cbFlush(CircularBuffer_s *cb) { cb->tail = cb->head; }
