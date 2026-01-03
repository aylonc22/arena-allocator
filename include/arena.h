#ifndef ARENA_H
#define ARENA_H

#include <stddef.h>   
#include <stdbool.h> 

/* 
 * Arena
 * -----
 * A simple bump-pointer memory arena.
 *
 * 
 */
typedef struct Arena {
    unsigned char* memory;  /* Base pointer to arena memory */
    size_t capacity;        /* Total size of the arena */
    size_t offset;          /* Current allocation offset */
} Arena;

/* 
 * Create an arena with the given capacity (in bytes).
 * Returns an arena with memory allocated, or memory == NULL on failure.
 */
Arena arena_create(size_t capacity);

/*
 * Allocate `size` bytes from the arena.
 * Returns a pointer to the allocated memory, or NULL if out of space.
 *
 * Memory is NOT zeroed.
 */
void* arena_alloc(Arena* arena, size_t size);

/*
 * Allocate `size` bytes with a specific alignment.
 * Alignment must be a power of two.
 */
void* arena_alloc_aligned(Arena* arena, size_t size, size_t alignment);

/*
 * Reset the arena.
 * All allocations become invalid.
 */
void arena_reset(Arena* arena);

/*
 * Destroy the arena and release its memory.
 * The arena must not be used after this call.
 */
void arena_destroy(Arena* arena);

/* ------------------------------------------------------------
 * Convenience macros
 * ------------------------------------------------------------ */

/* Allocate space for a single object of type T */
#define arena_alloc_type(arena, T) \
    ((T*) arena_alloc((arena), sizeof(T)))

/* Allocate space for an array of count elements of type T */
#define arena_alloc_array(arena, T, count) \
    ((T*) arena_alloc((arena), sizeof(T) * (count)))

#endif
