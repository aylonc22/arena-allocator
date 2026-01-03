#include "arena.h"

#include <stdlib.h>
#include <stdint.h>

/* ------------------------------------------------------------
 * Internal helper: align value up to alignment
 * ------------------------------------------------------------ */
static size_t align_up(size_t value, size_t alignment) {
    return (value + alignment - 1) & ~(alignment - 1);
}

/* ------------------------------------------------------------
 * Create arena
 * ------------------------------------------------------------ */
Arena arena_create(size_t capacity) {
    Arena arena = {0};

    if (capacity == 0)
        return arena;

    arena.memory = (unsigned char*)malloc(capacity);
    if (!arena.memory)
        return arena;

    arena.capacity = capacity;
    arena.offset = 0;

    return arena;
}

/* ------------------------------------------------------------
 * Allocate memory (default alignment)
 * ------------------------------------------------------------ */
void* arena_alloc(Arena* arena, size_t size) {
    if (!arena || !arena->memory || size == 0)
        return NULL;

    /* Default alignment: pointer size */
    const size_t alignment = sizeof(void*);
    size_t aligned_offset = align_up(arena->offset, alignment);

    if (aligned_offset + size > arena->capacity)
        return NULL;

    void* ptr = arena->memory + aligned_offset;
    arena->offset = aligned_offset + size;

    return ptr;
}

/* ------------------------------------------------------------
 * Allocate memory with explicit alignment
 * ------------------------------------------------------------ */
void* arena_alloc_aligned(Arena* arena, size_t size, size_t alignment) {
    if (!arena || !arena->memory || size == 0)
        return NULL;

    /* Alignment must be power of two */
    if ((alignment & (alignment - 1)) != 0)
        return NULL;

    size_t aligned_offset = align_up(arena->offset, alignment);

    if (aligned_offset + size > arena->capacity)
        return NULL;

    void* ptr = arena->memory + aligned_offset;
    arena->offset = aligned_offset + size;

    return ptr;
}

/* ------------------------------------------------------------
 * Reset arena (reuse memory)
 * ------------------------------------------------------------ */
void arena_reset(Arena* arena) {
    if (!arena)
        return;

    arena->offset = 0;
}

/* ------------------------------------------------------------
 * Destroy arena
 * ------------------------------------------------------------ */
void arena_destroy(Arena* arena) {
    if (!arena)
        return;

    free(arena->memory);
    arena->memory = NULL;
    arena->capacity = 0;
    arena->offset = 0;
}
