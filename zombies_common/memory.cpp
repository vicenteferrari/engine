//
// Created by vfs on 5/23/2020.
//

#ifndef DEFAULT_ALIGNMENT
#define DEFAULT_ALIGNMENT (2*sizeof(void *))
#endif

bool is_power_of_two(uintptr_t x) {
	return (x & (x-1)) == 0;
}

uintptr_t align_forward(uintptr_t ptr, size_t align) {
	uintptr_t p, a, modulo;

	assert(is_power_of_two(align));

	p = ptr;
	a = (uintptr_t)align;
	// Same as (p % a) but faster as 'a' is a power of two
	modulo = p & (a-1);

	if (modulo != 0) {
		// If 'p' address is not aligned, push the address to the
		// next value which is aligned
		p += a - modulo;
	}
	return p;
}

void *arena_alloc_align(Arena *a, size_t size, size_t align) {
	// Align 'curr_offset' forward to the specified alignment
	uintptr_t curr_ptr = (uintptr_t)a->buffer + (uintptr_t)a->curr_offset;
	uintptr_t offset = align_forward(curr_ptr, align);
	offset -= (uintptr_t)a->buffer; // Change to relative offset

	// Check to see if the backing memory has space left
	if (offset+size <= a->buffer_len) {
		void *ptr = &a->buffer[offset];
		a->prev_offset = offset;
		a->curr_offset = offset+size;

		// Zero new memory by default
		memset(ptr, 0, size);
		return ptr;
	}
	// Return NULL if the arena is out of memory (or handle differently)
	return NULL;
}

void *arena_alloc(Arena *a, size_t size) {
	return arena_alloc_align(a, size, DEFAULT_ALIGNMENT);
}

ZOMBIES_RESULT arena_init(Arena *a, void *backing_buffer, size_t backing_buffer_length) {
	a->buffer = (unsigned char *)backing_buffer;
	a->buffer_len = backing_buffer_length;
	a->curr_offset = 0;
	a->prev_offset = 0;

	return ZOMBIES_SUCCESS;
}

void *arena_resize_align(Arena *a, void *old_memory, size_t old_size, size_t new_size, size_t align) {
	unsigned char *old_mem = (unsigned char *)old_memory;

	assert(is_power_of_two(align));

	if (old_mem == NULL || old_size == 0) {
		return arena_alloc_align(a, new_size, align);
	} else if (a->buffer <= old_mem && old_mem < a->buffer+a->buffer_len) {
		if (a->buffer+a->prev_offset == old_mem) {
			a->curr_offset = a->prev_offset + new_size;
			if (new_size > old_size) {
				// Zero the new memory by default
				memset(&a->buffer[a->curr_offset], 0, new_size-old_size);
			}
			return old_memory;
		} else {
			void *new_memory = arena_alloc_align(a, new_size, align);
			size_t copy_size = old_size < new_size ? old_size : new_size;
			// Copy across old memory to the new memory
			memmove(new_memory, old_memory, copy_size);
			return new_memory;
		}

	} else {
		assert(0 && "Memory is out of bounds of the buffer in this arena");
		return NULL;
	}
}

void *arena_resize(Arena *a, void *old_memory, size_t old_size, size_t new_size) {
	return arena_resize_align(a, old_memory, old_size, new_size, DEFAULT_ALIGNMENT);
}

void arena_free_all(Arena *a) {
	a->curr_offset = 0;
	a->prev_offset = 0;
}

ZOMBIES_RESULT init_memory(Memory *memory) {
	if (!memory->init) {
		memory->state_memory_size = MEGABYTES(64);
		memory->temp_memory_size = GIGABYTES(1LL);

		size_t total_size = memory->state_memory_size + memory->temp_memory_size;

		memory->state_memory = malloc(total_size);
		memory->temp_memory = (uint8_t *) memory->state_memory + memory->state_memory_size;

		if (!memory->state_memory || !memory->temp_memory)
			return ZOMBIES_ERROR;

		if (arena_init(&memory->state_memory_arena, memory->state_memory, memory->state_memory_size) == ZOMBIES_ERROR)
			return ZOMBIES_ERROR;
		if (arena_init(&memory->temp_memory_arena, memory->temp_memory, memory->temp_memory_size) == ZOMBIES_ERROR)
			return ZOMBIES_ERROR;
	}

	return ZOMBIES_SUCCESS;
}
