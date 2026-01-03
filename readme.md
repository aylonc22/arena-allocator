# Arena Allocator (C)

A **simple, high-performance memory arena allocator** in C.

This project implements a **bump-pointer allocator**, also known as an **arena allocator**, which allows fast allocation of objects and arrays without per-allocation overhead or individual `free()` calls. Memory is released all at once when the arena is reset or destroyed.

---

## Features

- **Simple and Fast**: Bump-pointer allocation is $O(1)$.
- **Optional Alignment**: Support for memory alignment requirements.
- **Efficient Reuse**: Reset the arena to reuse memory without reallocating from the OS.
- **Type-Safe Macros**: Convenience macros for easy object and array allocation.
- **Fully Portable**: Written in standard C99.

---

## Usage Example

```c
#include "arena.h"
#include <stdio.h>

typedef struct Node {
    int value;
    struct Node* next;
} Node;

int main() {
    // Initialize arena with 1KB of memory
    Arena arena = arena_create(1024);

    // Allocate a single object
    Node* n1 = arena_alloc_type(&arena, Node);
    n1->value = 42;
    n1->next = NULL;

    // Allocate an array of 10 objects
    Node* n2 = arena_alloc_array(&arena, Node, 10);
    for (int i = 0; i < 10; i++) {
        n2[i].value = i;
        n2[i].next = NULL;
    }

    printf("n1 value: %d\n", n1->value);
    printf("n2[5] value: %d\n", n2[5].value);

    arena_reset(&arena);   // All memory is reclaimed for reuse
    arena_destroy(&arena); // Free memory back to the system

    return 0;
}
```
## API

```Arena arena_create(size_t capacity)```	Creates an arena with the given number of bytes.

```void* arena_alloc(Arena* arena, size_t size)```	Allocates size bytes. Returns NULL if out of space.

```void* arena_alloc_aligned(Arena* arena, size_t size, size_t alignment)```	Allocates bytes with specific power-of-two alignment.

```void arena_reset(Arena* arena)```	Resets the bump pointer, invalidating all previous allocations.

```void arena_destroy(Arena* arena)```	Frees the underlying memory back to the OS.

## Convenience Macros
```arena_alloc_type(arena, T)```: Allocate a single object of type T.

```arena_alloc_array(arena, T, N)```: Allocate an array of N objects of type T.

## Benefits
- Minimal Overhead: No per-object metadata (like the hidden headers in malloc).Zero Fragmentation: Memory is filled linearly, eliminating external fragmentation within the arena.Deterministic Performance: Allocation is a simple pointer increment ($O(1)$), making it ideal for real-time systems.Simplified Lifetime Management: Instead of tracking hundreds of pointers, you manage one: the arena itself.Data 

- Locality: Objects allocated together are stored together in physical memory, improving CPU cache performance.


## Future Improvements
- Arena Marks: Support "savepoints" to reset memory back to a specific point rather than clearing the whole arena.

- Thread Safety: Implementation of thread-local storage or atomic bump pointers for multi-threaded environments.

- Dynamic Growth: Allow the arena to grow by chaining memory blocks when the initial capacity is reached.

- Memory Tracking: Add debug-mode headers to detect buffer overflows or track usage statistics.

- Custom Backing: Allow the arena to be initialized from a stack-allocated buffer or a pre-existing memory pool.