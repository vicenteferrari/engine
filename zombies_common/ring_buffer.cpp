//
// Created by vfs on 11-Jun-20.
//

ZOMBIES_RESULT rb_init(RingBuffer *rb, float *backing_buffer, size_t capacity) {
	rb->buffer = backing_buffer;
	if(rb->buffer == NULL) {
		return ZOMBIES_ERROR;
	}

	rb->capacity = capacity;
	rb->count = 0;
	rb->head = rb->buffer;

	return ZOMBIES_SUCCESS;
}

void rb_free(RingBuffer *rb) {
	//free(cb->buffer);
	// clear out other fields too, just to be safe
}

void rb_push_back(RingBuffer *rb, const float item) {
	if(rb->count == rb->capacity){
		// handle error
	}
	memcpy(rb->head, &item, sizeof(float));
	rb->head += 1;
	if(rb->head == rb->buffer + rb->capacity)
		rb->head = rb->buffer;
	rb->count++;
}

void rb_read(RingBuffer *rb, float *flat_arr, size_t flat_size) {
	assert(rb->capacity == flat_size);
	if(rb->count == 0){
		// handle error
	}

	if (rb->head - rb->buffer == 0) {
		memcpy(flat_arr, rb->buffer, rb->capacity * sizeof(float));
	} else if (rb->head - rb->buffer > 0) {
		size_t head = (size_t) rb->head;
		size_t buffer = (size_t) rb->buffer;
		size_t start_to_head = head - buffer;
		size_t buffer_end = buffer + rb->capacity * sizeof(float);
		size_t head_to_end = buffer_end - head;


		memcpy(flat_arr, rb->head, head_to_end);

		size_t t = (size_t)flat_arr + head_to_end;
		memcpy((void *)t, rb->buffer, start_to_head);
	}
}

