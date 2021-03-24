//
// Created by vfs on 5/23/2020.
//

#ifndef ZOMBIES_MEMORY_H
#define ZOMBIES_MEMORY_H

struct Arena {
	u8 *buffer;
	size_t buffer_len;
	size_t prev_offset;
	size_t curr_offset;
};

ZOMBIESCOMMON_API
void *arena_alloc_align(Arena *a, size_t size, size_t align);

ZOMBIESCOMMON_API
void *arena_alloc(Arena *a, size_t size);

ZOMBIESCOMMON_API
ZOMBIES_RESULT arena_init(Arena *a, void *backing_buffer, size_t backing_buffer_length);

ZOMBIESCOMMON_API
void *arena_resize_align(Arena *a, void *old_memory, size_t old_size, size_t new_size, size_t align);

ZOMBIESCOMMON_API
void *arena_resize(Arena *a, void *old_memory, size_t old_size, size_t new_size);

ZOMBIESCOMMON_API
void arena_free_all(Arena *a);

struct Memory {
	bool init;

	// FIXME: maybe make one partition for each of the three states
	void *state_memory;
	size_t state_memory_size;
	Arena state_memory_arena;

	void *temp_memory;
	size_t temp_memory_size;
	Arena temp_memory_arena;
};

ZOMBIESCOMMON_API
ZOMBIES_RESULT init_memory(Memory *memory);

#endif
