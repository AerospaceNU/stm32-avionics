/*
 * circular_buffer.c
 */

#include "circular_buffer.h"

#include <string.h>

void cbInit(CircularBuffer_t *cb, void* buffer, size_t capacity, size_t size) {

	if (cb == NULL) {
		return;
	}

	cb->buffer = buffer;
	cb->capacity = capacity;
	cb->size = size;

	cb->buffer_end = cb->buffer + cb->capacity * cb->size;
	cb->count = 0;
	cb->head = cb->buffer;
	cb->tail = cb->buffer;
}

void cbPeek(CircularBuffer_t *cb, void* outputBuffer, size_t* numElements) {

	if (cb == NULL || outputBuffer == NULL)
		return;

	if (numElements == NULL)
		numElements = &(cb->count);
	else if (*numElements > cb->count)
		*numElements = cb->count;

	if (cb->tail + *numElements <= cb->buffer_end) {
		// One copy to outputBuffer needed
		memcpy(outputBuffer, cb->tail, *numElements);
	}
	else {
		// Two copies to outputBuffer needed
		size_t bytesToEnd = cb->buffer_end - cb->tail;
		memcpy(outputBuffer, cb->tail, bytesToEnd);
		memcpy(outputBuffer + bytesToEnd, cb->buffer, *numElements - bytesToEnd);
	}
}

bool cbEnqueue(CircularBuffer_t *cb, const void *item) {

	// Don't let circular buffer overflow
    if(cb->count == cb->capacity)
        return false;

    memcpy(cb->head, item, cb->size);
    cb->head += cb->size;
    if(cb->head == cb->buffer_end) {
    	cb->head = cb->buffer;
    }
    cb->count++;
    return true;
}

void cbDequeue(CircularBuffer_t *cb, size_t numElements) {

	if (numElements > cb->count) {
		numElements = cb->count;
	}

    cb->tail += cb->size * numElements;
    if(cb->tail >= cb->buffer_end) {
        cb->tail = cb->buffer + (cb->tail - cb->buffer_end);
    }
	cb->count -= numElements;
}

void cbFlush(CircularBuffer_t *cb) {
	cbInit(cb, cb->buffer, cb->capacity, cb->size);
}
