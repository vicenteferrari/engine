//
// Created by vfs on 11-Jun-20.
//

#ifndef ZOMBIES_RING_BUFFER_H
#define ZOMBIES_RING_BUFFER_H

#define SAMPLES_COUNT 500

struct RingBuffer {
	float *buffer;		// data buffer
	size_t capacity;	// maximum number of items in the buffer
	size_t count;		// number of items in the buffer
	float *head;		// pointer to head
};

ZOMBIESCOMMON_API
ZOMBIES_RESULT rb_init(RingBuffer *rb, float *backing_buffer, size_t capacity);

ZOMBIESCOMMON_API
void rb_free(RingBuffer *rb);

ZOMBIESCOMMON_API
void rb_push_back(RingBuffer *rb, float item);

ZOMBIESCOMMON_API
void rb_read(RingBuffer *rb, float *flat_arr, size_t flat_size);

#endif
