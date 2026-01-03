#include "arena.h"
#include <stdio.h>

typedef struct Node {
    int value;
    struct Node* next;
} Node;

int main() {
    Arena arena = arena_create(1024);

    Node* n1 = arena_alloc_type(&arena, Node);
    n1->value = 42;
    n1->next = NULL;

    Node* n2 = arena_alloc_array(&arena, Node, 10);
    for (int i = 0; i < 10; i++) {
        n2[i].value = i;
        n2[i].next = NULL;
    }

    printf("n1 value: %d\n", n1->value);
    printf("n2[5] value: %d\n", n2[5].value);

    arena_reset(&arena);   
    arena_destroy(&arena);

    return 0;
}